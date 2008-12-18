/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_SPENDQP_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* spendqp.c */
void save_brands P_((void));
void load_brands P_((void));
void do_qpspend P_((CHAR_DATA *ch, char *argument));
void do_immbrand P_((CHAR_DATA *ch, char *argument));
void do_statraise P_((CHAR_DATA *ch, char *argument));

#undef P_
