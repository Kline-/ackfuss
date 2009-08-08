/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_ACT_OBJ_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* act_obj.c */
void get_obj P_((CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container));
DECLARE_DO_FUN(do_get);
DECLARE_DO_FUN(do_put);
DECLARE_DO_FUN(do_drop);
DECLARE_DO_FUN(do_give);
DECLARE_DO_FUN(do_fill);
DECLARE_DO_FUN(do_drink);
DECLARE_DO_FUN(do_eat);
bool remove_obj P_((CHAR_DATA * ch, int iWear, bool fReplace));
DECLARE_DO_FUN(do_wear);
void remove_all P_((CHAR_DATA *ch));
DECLARE_DO_FUN(do_remove);
DECLARE_DO_FUN(do_sacrifice);
DECLARE_DO_FUN(do_quaff);
DECLARE_DO_FUN(do_recite);
DECLARE_DO_FUN(do_brandish);
DECLARE_DO_FUN(do_zap);
DECLARE_DO_FUN(do_steal);
CHAR_DATA *find_keeper P_((CHAR_DATA *ch));
void check_guards P_((CHAR_DATA *ch));
DECLARE_DO_FUN(do_buy);
DECLARE_DO_FUN(do_list);
DECLARE_DO_FUN(do_sell);
DECLARE_DO_FUN(do_value);
DECLARE_DO_FUN(do_donate);
DECLARE_DO_FUN(do_adapt);
DECLARE_DO_FUN(do_cdonate);
DECLARE_DO_FUN(do_appraise);
DECLARE_DO_FUN(do_bid);
DECLARE_DO_FUN(do_auction);
DECLARE_DO_FUN(do_connect);
DECLARE_DO_FUN(do_repair);
bool can_wear_at P_(( CHAR_DATA * ch, OBJ_DATA * obj, int location ));

#undef P_
