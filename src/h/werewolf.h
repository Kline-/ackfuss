/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_WEREWOLF_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* werewolf.c */
void do_tribe P_((CHAR_DATA *ch, char *argument));
void do_imprint P_((CHAR_DATA *ch, char *argument));
bool spell_tribe_claw P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_wolf_mark P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
void do_scent P_((CHAR_DATA *ch, char *argument));

#undef P_
