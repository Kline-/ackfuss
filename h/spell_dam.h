/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_SPELL_DAM_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* spell_dam.c */
void sp_death_message P_((CHAR_DATA *ch, CHAR_DATA *victim, int realm));
void sp_dam_message P_((OBJ_DATA *obj, CHAR_DATA *ch, CHAR_DATA *victim, int dam, int realm, int dt));
bool sp_damage P_((OBJ_DATA *obj, CHAR_DATA *ch, CHAR_DATA *victim, int dam, int type, int sn, bool show_msg));

#undef P_
