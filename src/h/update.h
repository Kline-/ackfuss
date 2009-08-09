/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_UPDATE_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* update.c */
int get_user_seconds P_((void));
void alarm_update P_((void));
void reset_itimer P_((void));
void alarm_handler P_((int signo));
void init_alarm_handler P_((void));
void advance_level P_((CHAR_DATA *ch, int p_class, bool show, bool remort));
int hit_gain P_((CHAR_DATA *ch));
int mana_gain P_((CHAR_DATA *ch));
int move_gain P_((CHAR_DATA *ch));
void rage_gain P_((CHAR_DATA *ch));
void bloodlust_gain P_((CHAR_DATA *ch, int value));
void condition_gain P_((CHAR_DATA *ch, int iCond, int value));
void mobile_update P_((void));
void clean_donate_rooms P_((void));
void weather_update P_((void));
void gain_update P_((void));
void char_update P_((void));
void check_vamp P_((CHAR_DATA *ch));
void objfun_update P_((void));
void obj_update P_((void));
void aggr_update P_((void));
void rooms_update P_((void));
void update_handler P_((void));
bool check_rewield P_((CHAR_DATA *ch));
bool check_reequip P_((CHAR_DATA *ch));
void auction_update P_((void));
void quest_update P_((void));
void lua_update P_((void));

#undef P_
