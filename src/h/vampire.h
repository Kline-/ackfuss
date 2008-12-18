/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_VAMPIRE_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* vampire.c */
bool spell_blood_leach P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_shade P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_embrace P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
void do_family P_((CHAR_DATA *ch, char *argument));
void do_instruct P_((CHAR_DATA *ch, char *argument));
bool spell_mesmerise P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cloak_darkness P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_blood_walk P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_blood_sign P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_blood_sense P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));

#undef P_
