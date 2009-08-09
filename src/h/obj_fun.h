/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_OBJ_FUN_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* obj_fun.c */
OBJ_FUN *obj_fun_lookup P_((const char *name));
char *rev_obj_fun_lookup P_((OBJ_FUN *func));
void print_obj_fun_lookup P_((char *buf));
DECLARE_OBJ_FUN(objfun_giggle);
DECLARE_OBJ_FUN(objfun_soul_moan);
DECLARE_OBJ_FUN(objfun_infused_soul);
DECLARE_OBJ_FUN(objfun_cast_fight);
DECLARE_OBJ_FUN(objfun_sword_aggro);
DECLARE_OBJ_FUN(objfun_flaming);
DECLARE_OBJ_FUN(objfun_healing);
DECLARE_OBJ_FUN(objfun_dispeller);
DECLARE_OBJ_FUN(objfun_regen);

#undef P_
