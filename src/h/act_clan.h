/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_ACT_CLAN_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* act_clan.c */
void save_clan_table P_((void));
void do_ctoggle P_((CHAR_DATA *ch, char *argument));
char *get_diplo_name P_((short value));
void do_politics P_((CHAR_DATA *ch, char *argument));
void do_negotiate P_((CHAR_DATA *ch, char *argument));
void do_leav P_((CHAR_DATA *ch, char *argument));
void do_cset P_((CHAR_DATA *ch, char *argument));
void do_accept P_((CHAR_DATA *ch, char *argument));
void do_cwhere P_((CHAR_DATA *ch, char *argument));
void do_leave P_((CHAR_DATA *ch, char *argument));
void do_banish P_((CHAR_DATA *ch, char *argument));
void do_make P_((CHAR_DATA *ch, char *argument));
void do_council P_((CHAR_DATA *ch, char *argument));

#undef P_
