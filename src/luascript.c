/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/
/* Lua embedding inspired by Nick Gammon's work on Smaug                   */

#include "globals.h"

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_LUASCRIPT_H
#include "h/luascript.h"
#endif

#define CH_STATE     "ch.state"
#define OBJ_STATE    "obj.state"
#define MUD_LIBRARY  "mud"
#define CH_STARTUP   SCRIPT_DIR "ch_startup.lua"
#define OBJ_STARTUP  SCRIPT_DIR "obj_startup.lua"

#define PUSH_STR(from,x) \
 lua_pushstring(L,from->x); \
 lua_setfield(L,-2,#x)

#define PUSH_STRA(from,x,max) \
 lua_newtable(L); \
 for( int i = 0; i < max; ) \
 { \
  lua_pushstring(L,from->x[i]); \
  i++; \
  lua_rawseti(L,-2,i); \
 } \
 lua_setfield(L,-2,#x)

#define PUSH_NUM(from,x) \
 lua_pushnumber(L,from->x); \
 lua_setfield(L,-2,#x)

#define PUSH_NUMA(from,x,max) \
 lua_newtable(L); \
 for( int i = 0; i < max; ) \
 { \
  lua_pushnumber(L,from->x[i]); \
  i++; \
  lua_rawseti(L,-2,i); \
 } \
 lua_setfield(L,-2,#x)

static const struct luaL_reg mudlib [] = 
{
 {"char_info",    L_character_info},
 {"obj_info",     L_obj_info      },
 {"send_to_char", L_send_to_char  },
 {"recho",        L_recho         },
 {NULL, NULL}
};

void init_lua( CHAR_DATA *ch )
{
 luaL_openlibs(ch->L); /* open all standard libraries */

 /* call as Lua function because we need the environment  */
 lua_pushcfunction(ch->L,RegisterLuaRoutines);
 lua_pushstring(ch->L,CH_STATE);  /* push address */
 lua_pushlightuserdata(ch->L,(void *)ch);    /* push value */
 lua_call(ch->L,2,0);
 
 /* run initialiation script */
 if( luaL_loadfile(ch->L,CH_STARTUP) || CallLuaWithTraceBack(ch->L,0,0) )
 {
  const char * sError = lua_tostring(ch->L,-1);

  snprintf(log_buf,(2 * MIL),"Error loading ch Lua startup file:\n %s\n",sError);
  monitor_chan(log_buf,MONITOR_DEBUG);
 }

 lua_settop(ch->L,0); /* get rid of stuff lying around */
 return;
}

void init_lua( OBJ_DATA *obj )
{
 luaL_openlibs(obj->L);

 lua_pushcfunction(obj->L,RegisterLuaRoutines);
 lua_pushstring(obj->L,OBJ_STATE);
 lua_pushlightuserdata(obj->L,(void *)obj);
 lua_call(obj->L,2,0);

 if( luaL_loadfile(obj->L,OBJ_STARTUP) || CallLuaWithTraceBack(obj->L,0,0) )
 {
  const char * sError = lua_tostring(obj->L,-1);

  snprintf(log_buf,(2 * MIL),"Error loading obj Lua startup file:\n %s\n",sError);
  monitor_chan(log_buf,MONITOR_DEBUG);
 }

 lua_settop(obj->L,0);
 return;
}

int RegisterLuaRoutines( lua_State *L )
{

 lua_newtable(L);  /* environment */
 lua_replace(L,LUA_ENVIRONINDEX);

 /* this makes environment variable "ch.state" by the pointer to our character */
 lua_settable(L,LUA_ENVIRONINDEX);

 /* register all mud.xxx routines */
 luaL_register(L,MUD_LIBRARY,mudlib);
  
 /* using interpret now
 RegisterLuaCommands (L);
 */

 return 0;
}

int CallLuaWithTraceBack( lua_State *L, const int iArguments, const int iReturn )
{
 int error;
 int base = lua_gettop(L) - iArguments;  /* function index */

 GetTracebackFunction(L);
 if( lua_isnil(L,-1) )
 {
  lua_pop(L,1);   /* pop non-existent function  */
  error = lua_pcall(L,iArguments,iReturn,0);
 }  
 else
 {
  lua_insert(L,base);  /* put it under chunk and args */
  error = lua_pcall(L,iArguments,iReturn,base);
  lua_remove(L,base);  /* remove traceback function */
 }

 return error;
}

void GetTracebackFunction( lua_State *L )
{
 lua_pushliteral(L,LUA_DBLIBNAME);  /* "debug"   */
 lua_rawget(L,LUA_GLOBALSINDEX);    /* get debug library   */

 if( !lua_istable(L,-1) )
 {
  lua_pop(L,2);   /* pop result and debug table  */
  lua_pushnil(L);
  return;
 }

 /* get debug.traceback  */
 lua_pushstring(L,"traceback");  
 lua_rawget(L,-2);               /* get traceback function  */
  
 if( !lua_isfunction(L,-1) )
 {
  lua_pop(L,2);   /* pop result and debug table  */
  lua_pushnil(L);
  return;
 }

 lua_remove (L, -2);   /* remove debug table, leave traceback function  */
}

ROOM_INDEX_DATA *L_getroom( lua_State *L )
{
 CHAR_DATA *ch, *cv;
 OBJ_DATA *ob, *ov;
 std::list<CHAR_DATA *>::iterator ci;
 std::list<OBJ_DATA *>::iterator oi;

 for( ci = char_list.begin(); ci != char_list.end(); ci++ )
 {
  ch = *ci;
  if( ch->L == L )
  {
   lua_pushstring(L,CH_STATE);
   lua_gettable(L,LUA_ENVIRONINDEX);
   cv = (CHAR_DATA *)lua_touserdata(L,-1);
   lua_pop(L,1);
   return cv->in_room;
  }
 }

 for( oi = obj_list.begin(); oi != obj_list.end(); oi++ )
 {
  ob = *oi;
  if( ob->L == L )
  {
   lua_pushstring(L,OBJ_STATE);
   lua_gettable(L,LUA_ENVIRONINDEX);
   ov = (OBJ_DATA *)lua_touserdata(L,-1);
   lua_pop(L,1);
   if( ov->carried_by )
    return ov->carried_by->in_room;
   else
    return ov->in_room;
  }
 }

 return NULL;
}

CHAR_DATA *L_getchar( lua_State *L )
{
 /* retrieve our character */
  CHAR_DATA * ch;
  
 /* retrieve the character */
 lua_pushstring(L,CH_STATE);  /* push address */
 lua_gettable(L,LUA_ENVIRONINDEX);  /* retrieve value */

 ch = (CHAR_DATA *)lua_touserdata(L,-1);  /* convert to data */
 lua_pop(L,1);  /* pop result */

 return ch;
}

int L_character_info( lua_State *L )
{
 CHAR_DATA *ch = NULL;
 bool found = false;
 std::list<CHAR_DATA *>::iterator li;

 for( li = char_list.begin(); li != char_list.end(); li++ )
 {
  ch = *li;
  if( ch->L == L )
   found = true;
 }

 if( !found )
 return 0;

 lua_newtable(L);  /* table for the info */
  
 /* strings */
 PUSH_STR(ch,description);
 PUSH_STR(ch,long_descr);
 PUSH_STR(ch,long_descr_orig);
 PUSH_STR(ch,name);
 PUSH_STR(ch,old_prompt);
 PUSH_STR(ch,prompt);
 PUSH_STR(ch,searching);
 PUSH_STR(ch,target);

 /* numbers */
 PUSH_NUM(ch,adept_level);
 PUSH_NUM(ch,alignment);
 PUSH_NUM(ch,armor);
 PUSH_NUM(ch,carry_number);
 PUSH_NUM(ch,carry_weight);
 PUSH_NUM(ch,clan);
 PUSH_NUMA(ch,cooldown,MAX_COOLDOWN);
 PUSH_NUM(ch,damroll);
 PUSH_NUM(ch,death_cnt);
 PUSH_NUM(ch,exp);
 PUSH_NUM(ch,extract_timer);
 PUSH_NUM(ch,hit);
 PUSH_NUM(ch,hitroll);
 PUSH_NUM(ch,incog);
 PUSH_NUM(ch,invis);
 PUSH_NUM(ch,level);
 PUSH_NUMA(ch,lvl,MAX_CLASS);
 PUSH_NUMA(ch,lvl2,MAX_CLASS);
 PUSH_NUM(ch,mana);
 PUSH_NUM(ch,max_hit);
 PUSH_NUM(ch,max_mana);
 PUSH_NUM(ch,max_move);
 PUSH_NUM(ch,move);
 PUSH_NUM(ch,npc);
 PUSH_NUM(ch,num_followers);
 PUSH_NUM(ch,played);
 PUSH_NUM(ch,poly_level);
 PUSH_NUM(ch,position);
 PUSH_NUM(ch,practice);
 PUSH_NUM(ch,p_class);
 PUSH_NUM(ch,race);
 PUSH_NUM(ch,saving_throw);
 PUSH_NUM(ch,sentence);
 PUSH_NUM(ch,sex);
 PUSH_NUMA(ch,speed,MAX_SPEED);
 PUSH_NUM(ch,stance);
 PUSH_NUM(ch,stun_timer);
 PUSH_NUM(ch,switched);
 PUSH_NUM(ch,timer);
 PUSH_NUM(ch,trust);
 PUSH_NUM(ch,wait);
 PUSH_NUM(ch,wimpy);
 PUSH_NUM(ch,wizbit);

 if( !IS_NPC(ch) )
 {
  lua_newtable(L);
  /* strings */
  PUSH_STR(ch->pcdata,assist_msg);
  PUSH_STR(ch->pcdata,bamfin);
  PUSH_STR(ch->pcdata,bamfout);
  PUSH_STR(ch->pcdata,email_address);
  PUSH_STR(ch->pcdata,load_msg);
  PUSH_STR(ch->pcdata,room_enter);
  PUSH_STR(ch->pcdata,room_exit);
  PUSH_STR(ch->pcdata,title);
  PUSH_STR(ch->pcdata,who_name);

  /* numbers */
  PUSH_NUMA(ch->pcdata,condition,MAX_COND);
  PUSH_NUMA(ch->pcdata,learned,MAX_SKILL);
  PUSH_NUM(ch->pcdata,max_con);
  PUSH_NUM(ch->pcdata,max_dex);
  PUSH_NUM(ch->pcdata,max_int);
  PUSH_NUM(ch->pcdata,max_str);
  PUSH_NUM(ch->pcdata,max_wis);
  PUSH_NUM(ch->pcdata,mod_con);
  PUSH_NUM(ch->pcdata,mod_dex);
  PUSH_NUM(ch->pcdata,mod_int);
  PUSH_NUM(ch->pcdata,mod_str);
  PUSH_NUM(ch->pcdata,mod_wis);
  PUSH_NUMA(ch->pcdata,order,MAX_CLASS);
  PUSH_NUM(ch->pcdata,perm_con);
  PUSH_NUM(ch->pcdata,perm_dex);
  PUSH_NUM(ch->pcdata,perm_int);
  PUSH_NUM(ch->pcdata,perm_str);
  PUSH_NUM(ch->pcdata,perm_wis);
  PUSH_NUM(ch->pcdata,quest_points);
  PUSH_NUM(ch->pcdata,recall_vnum);
  PUSH_NUM(ch->pcdata,ruler_rank);

  lua_setfield(L,-2,"pcdata");
 }
 else
 {
  lua_newtable(L);
  /* strings */
  PUSH_STR(ch->npcdata,short_descr);

  /* numbers */
  PUSH_NUM(ch->npcdata,cast);
  PUSH_NUM(ch->npcdata,def);
  PUSH_NUM(ch->npcdata,resist);
  PUSH_NUM(ch->npcdata,skills);
  PUSH_NUM(ch->npcdata,strong_magic);
  PUSH_NUM(ch->npcdata,suscept);
  PUSH_NUM(ch->npcdata,weak_magic);

  lua_setfield(L,-2,"npcdata");
 }

 return 1;
}

int L_obj_info( lua_State *L )
{
 OBJ_DATA *ob = NULL;
 bool found = false;
 std::list<OBJ_DATA *>::iterator li;

 for( li = obj_list.begin(); li != obj_list.end(); li++ )
 {
  ob = *li;
  if( ob->L == L )
   found = true;
 }

 if( !found )
 return 0;

 lua_newtable(L);

 PUSH_STR(ob,owner);
 PUSH_STR(ob,name);
 PUSH_STR(ob,script_name);
 PUSH_STR(ob,short_descr);
 PUSH_STR(ob,long_descr);

 PUSH_NUM(ob,item_type);
 PUSH_NUM(ob,item_apply);
 PUSH_NUM(ob,wear_loc);
 PUSH_NUM(ob,weight);
 PUSH_NUM(ob,cost);
 PUSH_NUM(ob,level);
 PUSH_NUM(ob,timer);
 PUSH_NUMA(ob,value,MAX_OBJ_VALUE);
 PUSH_NUM(ob,durability);
 PUSH_NUM(ob,max_durability);
 PUSH_NUM(ob,speed);

 return 1;
}

int L_send_to_char( lua_State *L )
{
 send_to_char(luaL_checkstring(L,1),L_getchar(L));
 return 0;
}

int L_recho( lua_State *L )
{
 recho(L_getroom(L),luaL_checkstring(L,1));
 return 0;
}
