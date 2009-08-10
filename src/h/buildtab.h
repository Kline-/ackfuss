/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_BUILDTAB_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* buildtab.c */
unsigned long int table_lookup P_((LOOKUP_TYPE *table, char *name));
char *rev_table_lookup P_((LOOKUP_TYPE *table, unsigned long int number));
char *bit_table_lookup P_((LOOKUP_TYPE *table, unsigned long int number));
void table_printout P_((LOOKUP_TYPE *table, char *buf));
void wide_table_printout P_((LOOKUP_TYPE *table, char *buf));
char *show_values P_((LOOKUP_TYPE *table, unsigned long int value, bool fBit));
char *bs_show_values P_((LOOKUP_TYPE *table, bitset<MAX_BITSET> &bit));

extern LOOKUP_TYPE tab_mob_class[];
extern LOOKUP_TYPE tab_value_meanings[];
extern LOOKUP_TYPE tab_drink_types[];
extern LOOKUP_TYPE tab_weapon_types[];
extern LOOKUP_TYPE tab_charm_types[];
extern LOOKUP_TYPE tab_mob_act[];
extern LOOKUP_TYPE tab_player_act[];
extern LOOKUP_TYPE tab_affected_by[];
extern LOOKUP_TYPE tab_item_types[];
extern LOOKUP_TYPE tab_obj_flags[];
extern LOOKUP_TYPE tab_wear_flags[];
extern LOOKUP_TYPE tab_item_apply[];
extern LOOKUP_TYPE tab_wear_loc[];
extern LOOKUP_TYPE tab_obj_aff[];
extern LOOKUP_TYPE tab_room_flags[];
extern LOOKUP_TYPE tab_sector_types[];
extern LOOKUP_TYPE tab_door_types[];
extern LOOKUP_TYPE tab_door_states[];
extern LOOKUP_TYPE tab_mob_skill[];
extern LOOKUP_TYPE tab_mob_cast[];
extern LOOKUP_TYPE tab_magic_realms[];
extern LOOKUP_TYPE tab_magic_realms_col[];
extern LOOKUP_TYPE tab_mob_race_mods[];
extern LOOKUP_TYPE tab_mob_race_mods_col[];
extern LOOKUP_TYPE tab_cast_name[];
extern LOOKUP_TYPE tab_mob_def[];
extern EQ_TYPE tab_auto_obj[];

#undef P_
