/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/
/* Lua embedding inspired by Nick Gammon's work on Smaug                   */

#include "globals.h"

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
#define MUD_LIBRARY  "mud"
#define CH_STARTUP   SCRIPT_DIR "ch_startup.lua"

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
 {"send_to_char", L_send_to_char  },
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

  if( IS_IMMORTAL(ch) )
   ch_printf(ch,"Error loading Lua startup file:\n %s\n",sError);
 }

 lua_settop(ch->L,0); /* get rid of stuff lying around */
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

int L_send_to_char( lua_State *L )
{
 send_to_char(luaL_checkstring(L,1),L_getchar(L));
 return 0;
}
