/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_LUASCRIPT_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* luascript.c */
void init_lua P_((CHAR_DATA *ch));
int RegisterLuaRoutines P_((lua_State *L));
int CallLuaWithTraceBack P_((lua_State *L, const int iArguments, const int iReturn));
void GetTracebackFunction P_((lua_State *L));
CHAR_DATA *L_getchar P_((lua_State *L));
int L_character_info P_((lua_State *L));
int L_send_to_char P_((lua_State *L));

#undef P_
