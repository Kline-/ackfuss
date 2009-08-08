/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/
/* Lua embedding inspired by Nick Gammon's work on Smaug                   */

#include "h/globals.h"

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
#define ROOM_STATE   "room.state"
#define MUD_LIBRARY  "mud"
#define CH_STARTUP   SCRIPT_DIR "ch_startup.lua"
#define OBJ_STARTUP  SCRIPT_DIR "obj_startup.lua"
#define ROOM_STARTUP SCRIPT_DIR "room_startup.lua"

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
 {"room_info",    L_room_info     },
 {"send_to_char", L_send_to_char  },
 {"recho",        L_recho         },
 {NULL, NULL}
};

void init_lua( CHAR_DATA *ch )
{
 ch->lua->owner = ch;
 ch->lua->type = LUA_TYPE_CH;
 luaL_openlibs(ch->lua->L); /* open all standard libraries */

 /* call as Lua function because we need the environment  */
 lua_pushcfunction(ch->lua->L,RegisterLuaRoutines);
 lua_pushstring(ch->lua->L,CH_STATE);  /* push address */
 lua_pushlightuserdata(ch->lua->L,(void *)ch);    /* push value */
 lua_call(ch->lua->L,2,0);
 
 /* run initialiation script */
 if( luaL_loadfile(ch->lua->L,CH_STARTUP) || CallLuaWithTraceBack(ch->lua->L,0,0) )
 {
  const char * sError = lua_tostring(ch->lua->L,-1);

  snprintf(log_buf,(2 * MIL),"Error loading ch Lua startup file:\n %s\n",sError);
  monitor_chan(log_buf,MONITOR_DEBUG);
 }

 lua_settop(ch->lua->L,0); /* get rid of stuff lying around */
 return;
}

void init_lua( OBJ_DATA *obj )
{
 obj->lua->owner = obj;
 obj->lua->type = LUA_TYPE_OB;
 luaL_openlibs(obj->lua->L);

 lua_pushcfunction(obj->lua->L,RegisterLuaRoutines);
 lua_pushstring(obj->lua->L,OBJ_STATE);
 lua_pushlightuserdata(obj->lua->L,(void *)obj);
 lua_call(obj->lua->L,2,0);

 if( luaL_loadfile(obj->lua->L,OBJ_STARTUP) || CallLuaWithTraceBack(obj->lua->L,0,0) )
 {
  const char * sError = lua_tostring(obj->lua->L,-1);

  snprintf(log_buf,(2 * MIL),"Error loading obj Lua startup file:\n %s\n",sError);
  monitor_chan(log_buf,MONITOR_DEBUG);
 }

 lua_settop(obj->lua->L,0);
 return;
}

void init_lua( ROOM_INDEX_DATA *room )
{
 room->lua->owner = room;
 room->lua->type = LUA_TYPE_RM;
 luaL_openlibs(room->lua->L);

 lua_pushcfunction(room->lua->L,RegisterLuaRoutines);
 lua_pushstring(room->lua->L,ROOM_STATE);
 lua_pushlightuserdata(room->lua->L,(void *)room);
 lua_call(room->lua->L,2,0);

 if( luaL_loadfile(room->lua->L,ROOM_STARTUP) || CallLuaWithTraceBack(room->lua->L,0,0) )
 {
  const char * sError = lua_tostring(room->lua->L,-1);

  snprintf(log_buf,(2 * MIL),"Error loading room Lua startup file:\n %s\n",sError);
  monitor_chan(log_buf,MONITOR_DEBUG);
 }

 lua_settop(room->lua->L,0);
 return;
}

lua_State *find_lua_function( CHAR_DATA *ch, string arg )
{
 lua_State *L = ch->lua->L;
  
 if( !L || arg.empty() )
  return NULL;  /* can't do it */
    
 /* find requested function */
  
 lua_getglobal(L,arg.c_str());  
 if( !lua_isfunction(L,-1) )
 {
  lua_pop(L,1);
  snprintf(log_buf,(2 * MIL),"Warning: Lua script function '%s' does not exist",arg.c_str());
  monitor_chan(log_buf,MONITOR_DEBUG);
  return NULL;  /* not there */
 }
      
 return L;  
}

lua_State *find_lua_function( OBJ_DATA *ob, string arg )
{
 lua_State *L = ob->lua->L;

 if( !L || arg.empty() )
  return NULL;  /* can't do it */

 /* find requested function */

 lua_getglobal(L,arg.c_str());
 if( !lua_isfunction(L,-1) )
 {
  lua_pop(L,1);
  snprintf(log_buf,(2 * MIL),"Warning: Lua script function '%s' does not exist",arg.c_str());
  monitor_chan(log_buf,MONITOR_DEBUG);
  return NULL;  /* not there */
 }

 return L;
}

lua_State *find_lua_function( ROOM_INDEX_DATA *rm, string arg )
{
 lua_State *L = rm->lua->L;

 if( !L || arg.empty() )
  return NULL;  /* can't do it */

 /* find requested function */

 lua_getglobal(L,arg.c_str());
 if( !lua_isfunction(L,-1) )
 {
  lua_pop(L,1);
  snprintf(log_buf,(2 * MIL),"Warning: Lua script function '%s' does not exist",arg.c_str());
  monitor_chan(log_buf,MONITOR_DEBUG);
  return NULL;  /* not there */
 }

 return L;
}

void call_lua_function( lua_State *L, string str, const int nArgs )
{
 if( CallLuaWithTraceBack(L,nArgs,0) )
 {
  snprintf(log_buf,(2 * MIL),"Error executing Lua function %s:\n %s",str.c_str(),lua_tostring(L,-1));
  monitor_chan(log_buf,MONITOR_DEBUG);
  lua_pop(L,1);  /* pop error */
 }  /* end of error */

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
 CHAR_DATA *ch;
 OBJ_DATA *ob;
 ROOM_INDEX_DATA *rm;
 LUA_DATA *lua;
 list<LUA_DATA *>::iterator li;

 for( li = lua_list.begin(); li != lua_list.end(); li++ )
 {
  lua = *li;

  switch( lua->type )
  {
   default:
   {
    snprintf(log_buf,(2 * MIL),"Invalid Lua owner type in lua_list: %d",lua->type);
    monitor_chan(log_buf,MONITOR_DEBUG);
    return NULL;
   }
   case LUA_TYPE_CH:
   {
    ch = static_cast<CHAR_DATA *>(lua->owner);
    if( ch->lua->L == L )
    {
     lua_pushstring(L,CH_STATE);
     lua_gettable(L,LUA_ENVIRONINDEX);
     lua_pop(L,1);
     return ch->in_room;
    }
   }
   case LUA_TYPE_OB:
   {
    ob = static_cast<OBJ_DATA *>(lua->owner);
    if( ob->lua->L == L )
    {
     lua_pushstring(L,OBJ_STATE);
     lua_gettable(L,LUA_ENVIRONINDEX);
     lua_pop(L,1);
     return in_loc(ob);
    }
   }
   case LUA_TYPE_RM:
   {
    rm = static_cast<ROOM_INDEX_DATA *>(lua->owner);
    if( rm->lua->L == L )
    {
     lua_pushstring(L,ROOM_STATE);
     lua_gettable(L,LUA_ENVIRONINDEX);
     lua_pop(L,1);
     return rm;
    }
   }
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
 list<LUA_DATA *>::iterator li;
 LUA_DATA *lua = NULL;

 for( li = lua_list.begin(); li != lua_list.end(); li++ )
 {
  lua = *li;
  if( lua->type == LUA_TYPE_CH )
  {
   ch = static_cast<CHAR_DATA *>(lua->owner);

   if( ch->lua->L == L )
    found = true;
  }
 }

 if( !found )
  return 0;

 lua_newtable(L);  /* table for the info */
  
 /* strings */
 PUSH_STR(ch,description.c_str());
 PUSH_STR(ch,long_descr.c_str());
 PUSH_STR(ch,long_descr_orig.c_str());
 PUSH_STR(ch,name.c_str());
 PUSH_STR(ch,old_prompt.c_str());
 PUSH_STR(ch,prompt.c_str());
 PUSH_STR(ch,searching.c_str());

 /* numbers */
 PUSH_NUM(ch,alignment);
 PUSH_NUM(ch,armor);
 PUSH_NUM(ch,carry_number);
 PUSH_NUM(ch,carry_weight);
 PUSH_NUM(ch,clan);
 PUSH_NUMA(ch,cooldown,MAX_COOLDOWN);
 PUSH_NUM(ch,damroll);
 PUSH_NUM(ch,exp);
 PUSH_NUM(ch,hit);
 PUSH_NUM(ch,hitroll);
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
 PUSH_NUM(ch,p_class);
 PUSH_NUM(ch,race);
 PUSH_NUM(ch,saving_throw);
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
  PUSH_NUM(ch->pcdata,adept_level);
  PUSH_NUMA(ch->pcdata,condition,MAX_COND);
  PUSH_NUM(ch->pcdata,death_cnt);
  PUSH_NUM(ch->pcdata,invis);
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
  PUSH_NUM(ch->pcdata,practice);
  PUSH_NUM(ch->pcdata,quest_points);
  PUSH_NUM(ch->pcdata,recall_vnum);
  PUSH_NUM(ch->pcdata,ruler_rank);
  PUSH_NUM(ch->pcdata,sentence);

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
  PUSH_NUM(ch->npcdata,extract_timer);
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
 list<LUA_DATA *>::iterator li;
 LUA_DATA *lua = NULL;

 for( li = lua_list.begin(); li != lua_list.end(); li++ )
 {
  lua = *li;
  if( lua->type == LUA_TYPE_OB )
  {
   ob = static_cast<OBJ_DATA *>(lua->owner);

   if( ob->lua->L == L )
    found = true;
  }
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

int L_room_info( lua_State *L )
{
 ROOM_INDEX_DATA *rm = NULL;
 bool found = false;
 list<LUA_DATA *>::iterator li;
 LUA_DATA *lua = NULL;

 for( li = lua_list.begin(); li != lua_list.end(); li++ )
 {
  lua = *li;
  if( lua->type == LUA_TYPE_RM )
  {
   rm = static_cast<ROOM_INDEX_DATA *>(lua->owner);

   if( rm->lua->L == L )
    found = true;
  }
 }

 if( !found )
  return 0;

 lua_newtable(L);

 PUSH_STR(rm,description);
 PUSH_STR(rm,name);
 PUSH_STR(rm,script_name);

 PUSH_NUM(rm,affected_by);
 PUSH_NUM(rm,light);
 PUSH_NUM(rm,sector_type);
 PUSH_NUM(rm,vnum);

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

void call_lua( LUA_DATA *lua, string str, string arg )
{
 if( lua )
 {
  if( lua->type == LUA_TYPE_CH )
   call_lua(static_cast<CHAR_DATA *>(lua->owner),str,arg);
  else if( lua->type == LUA_TYPE_OB )
   call_lua(static_cast<OBJ_DATA *>(lua->owner),str,arg);
  else if( lua->type == LUA_TYPE_RM )
   call_lua(static_cast<ROOM_INDEX_DATA *>(lua->owner),str,arg);
  else
   monitor_chan("Invalid Lua type in call_lua().",MONITOR_DEBUG);
 }
}

void call_lua( CHAR_DATA *ch, string str, string arg )
{
 int cnt = 0;

 if( !ch || str.empty() )  /* note, argument is OPTIONAL */
  return;

 lua_State *L = find_lua_function(ch,str.c_str());
   
 if( !L )
  return;
  
 /* if they want to send an argument, push it now */  
 if( !arg.empty() )
 {
  cnt++;
  lua_pushstring(L,arg.c_str());  /* push argument, if any */
 }

 call_lua_function(L,str,cnt);

 return;
}

void call_lua( OBJ_DATA *ob, string str, string arg )
{
 int cnt = 0;

 if( !ob || str.empty() )  /* note, argument is OPTIONAL */
  return;

 lua_State *L = find_lua_function(ob,str.c_str());

 if( !L )
  return;

 /* if they want to send an argument, push it now */
 if( !arg.empty() )
 {
  cnt++;
  lua_pushstring(L,arg.c_str());  /* push argument, if any */
 }

 call_lua_function(L,str,cnt);

 return;
}

void call_lua( ROOM_INDEX_DATA *rm, string str, string arg )
{
 int cnt = 0;

 if( !rm || str.empty() )  /* note, argument is OPTIONAL */
  return;

 lua_State *L = find_lua_function(rm,str.c_str());

 if( !L )
  return;

 /* if they want to send an argument, push it now */
 if( !arg.empty() )
 {
  cnt++;
  lua_pushstring(L,arg.c_str());  /* push argument, if any */
 }

 call_lua_function(L,str,cnt);

 return;
}
