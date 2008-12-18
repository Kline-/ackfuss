/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_MOB_COMMANDS_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* mob_commands.c */
char *mprog_type_to_name P_((int type));
void do_mpasound P_((CHAR_DATA *ch, char *argument));
void do_mpkill P_((CHAR_DATA *ch, char *argument));
void do_mpjunk P_((CHAR_DATA *ch, char *argument));
void do_mpechoaround P_((CHAR_DATA *ch, char *argument));
void do_mpechoat P_((CHAR_DATA *ch, char *argument));
void do_mpecho P_((CHAR_DATA *ch, char *argument));
void do_mpmload P_((CHAR_DATA *ch, char *argument));
void do_mpoload P_((CHAR_DATA *ch, char *argument));
void do_mppurge P_((CHAR_DATA *ch, char *argument));
void do_mpgoto P_((CHAR_DATA *ch, char *argument));
void do_mpat P_((CHAR_DATA *ch, char *argument));
void do_mptransfer P_((CHAR_DATA *ch, char *argument));
void do_mpforce P_((CHAR_DATA *ch, char *argument));

#undef P_
