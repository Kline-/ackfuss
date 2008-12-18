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
void do_get P_((CHAR_DATA *ch, char *argument));
void do_put P_((CHAR_DATA *ch, char *argument));
void do_drop P_((CHAR_DATA *ch, char *argument));
void do_give P_((CHAR_DATA *ch, char *argument));
void do_fill P_((CHAR_DATA *ch, char *argument));
void do_drink P_((CHAR_DATA *ch, char *argument));
void do_eat P_((CHAR_DATA *ch, char *argument));
bool remove_obj P_((CHAR_DATA * ch, int iWear, bool fReplace));
void do_wear P_((CHAR_DATA *ch, char *argument));
void remove_all P_((CHAR_DATA *ch));
void do_remove P_((CHAR_DATA *ch, char *argument));
void do_sacrifice P_((CHAR_DATA *ch, char *argument));
void do_quaff P_((CHAR_DATA *ch, char *argument));
void do_recite P_((CHAR_DATA *ch, char *argument));
void do_brandish P_((CHAR_DATA *ch, char *argument));
void do_zap P_((CHAR_DATA *ch, char *argument));
void do_steal P_((CHAR_DATA *ch, char *argument));
CHAR_DATA *find_keeper P_((CHAR_DATA *ch));
void check_guards P_((CHAR_DATA *ch));
void do_buy P_((CHAR_DATA *ch, char *argument));
void do_list P_((CHAR_DATA *ch, char *argument));
void do_sell P_((CHAR_DATA *ch, char *argument));
void do_value P_((CHAR_DATA *ch, char *argument));
void do_donate P_((CHAR_DATA *ch, char *argument));
void do_adapt P_((CHAR_DATA *ch, char *argument));
void do_cdonate P_((CHAR_DATA *ch, char *argument));
void do_appraise P_((CHAR_DATA *ch, char *argument));
void do_bid P_((CHAR_DATA *ch, char *argument));
void do_auction P_((CHAR_DATA *ch, char *argument));
void do_connect P_((CHAR_DATA *ch, char *argument));
void do_repair P_((CHAR_DATA *ch, char *argument));
bool can_wear_at P_(( CHAR_DATA * ch, OBJ_DATA * obj, int location ));

#undef P_
