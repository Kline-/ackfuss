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

#define CH_STR_ITEM(arg) \
  if (ch->arg)  \
  {   \
  lua_pushstring (L, ch->arg);  \
  lua_setfield (L, -2, #arg); \
  }
  
#define CH_NUM_ITEM(arg) \
  lua_pushnumber (L, ch->arg);  \
  lua_setfield (L, -2, #arg)

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
 CH_STR_ITEM(name);
 CH_STR_ITEM(long_descr);
 CH_STR_ITEM(description);

 return 1;
}

int L_send_to_char( lua_State *L )
{
 send_to_char(luaL_checkstring(L,1),L_getchar(L));
 return 0;
}
