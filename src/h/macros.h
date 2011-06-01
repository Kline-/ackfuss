/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_MACROS_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* macros.c */
void reset_gain_stats P_((CHAR_DATA *ch));
bool ok_to_use P_((CHAR_DATA *ch, int value));
bool check_level_use P_((CHAR_DATA *ch, int level));
int exp_to_level_adept P_((CHAR_DATA *ch));
int exp_to_level P_((CHAR_DATA *ch, int p_class, int index));
int exp_to_level_vamp P_((int level));
int exp_to_level_wolf P_((int level));
int exp_mob_base P_((int level));
int exp_for_mobile P_((int level, CHAR_DATA *mob));
int skill_table_lookup P_((CHAR_DATA *ch, int sn, int return_type));
int get_item_value P_((OBJ_DATA *obj));

#undef P_
