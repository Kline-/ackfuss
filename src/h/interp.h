/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_INTERP_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* interp.c */
void comlog P_((CHAR_DATA * ch, bool cleanup, int cmd, char *args));
void interpret P_((CHAR_DATA * ch, char *argument));
bool check_social P_((CHAR_DATA * ch, char *command, char *argument));
bool check_disabled P_((const struct cmd_type *command));

#undef P_
