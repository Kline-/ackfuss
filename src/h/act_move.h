/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_ACT_MOVE_H 1

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* act_move.c */
void do_north P_((CHAR_DATA *ch, char *argument));
void do_east P_((CHAR_DATA *ch, char *argument));
void do_south P_((CHAR_DATA *ch, char *argument));
void do_west P_((CHAR_DATA *ch, char *argument));
void do_up P_((CHAR_DATA *ch, char *argument));
void do_down P_((CHAR_DATA *ch, char *argument));
int find_door P_((CHAR_DATA *ch, char *arg));
void do_open P_((CHAR_DATA *ch, char *argument));
void do_close P_((CHAR_DATA *ch, char *argument));
void do_lock P_((CHAR_DATA *ch, char *argument));
void do_unlock P_((CHAR_DATA *ch, char *argument));
void do_pick P_((CHAR_DATA *ch, char *argument));
void do_stand P_((CHAR_DATA *ch, char *argument));
void do_rest P_((CHAR_DATA *ch, char *argument));
void do_sleep P_((CHAR_DATA *ch, char *argument));
void do_wake P_((CHAR_DATA *ch, char *argument));
void do_shadowform P_((CHAR_DATA *ch, char *argument));
void do_sneak P_((CHAR_DATA *ch, char *argument));
void do_warcry P_((CHAR_DATA *ch, char *argument));
void do_hide P_((CHAR_DATA *ch, char *argument));
void do_visible P_((CHAR_DATA *ch, char *argument));
void do_clan_recall P_((CHAR_DATA *ch, char *argument));
void do_recall P_((CHAR_DATA *ch, char *argument));
void do_train P_((CHAR_DATA *ch, char *argument));
void do_guild P_((CHAR_DATA *ch, char *argument));
void do_halls P_((CHAR_DATA *ch, char *argument));
void do_smash P_((CHAR_DATA *ch, char *argument));
void do_scan P_((CHAR_DATA *ch, char *argument));
void do_enter P_((CHAR_DATA *ch, char *argument));
void do_scout P_((CHAR_DATA *ch, char *argument));
void do_abandon P_((CHAR_DATA *ch, char *argument));
void move_char P_(( CHAR_DATA * ch, int door, bool look ));

#undef P_
