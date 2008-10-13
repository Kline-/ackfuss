/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  Ack 2.2 improvements copyright (C) 1994 by Stephen Dooley              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *       _/          _/_/_/     _/    _/     _/    ACK! MUD is modified    *
 *      _/_/        _/          _/  _/       _/    Merc2.0/2.1/2.2 code    *
 *     _/  _/      _/           _/_/         _/    (c)Stephen Zepp 1998    *
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3          *
 *   _/      _/      _/_/_/     _/    _/     _/                            *
 *                                                                         *
 *                        http://ackmud.nuc.net/                           *
 *                        zenithar@ackmud.nuc.net                          *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

unsigned long int table_lookup( LOOKUP_TYPE *table, char *name );
char *rev_table_lookup( LOOKUP_TYPE *table, unsigned long int );
char *bit_table_lookup( LOOKUP_TYPE *table, unsigned long int );
void table_printout( LOOKUP_TYPE *table, char *buf );
void wide_table_printout( LOOKUP_TYPE *table, char *buf );  /* -S- mod */

extern LOOKUP_TYPE tab_mob_class[];
extern LOOKUP_TYPE tab_value_meanings[];
extern LOOKUP_TYPE tab_drink_types[];
extern LOOKUP_TYPE tab_weapon_types[];
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
extern LOOKUP_TYPE tab_mob_powerskill[];
extern LOOKUP_TYPE tab_mob_powercast[];
extern LOOKUP_TYPE tab_magic_realms[];
extern LOOKUP_TYPE tab_magic_realms_col[];
extern LOOKUP_TYPE tab_mob_race_mods[];
extern LOOKUP_TYPE tab_mob_race_mods_col[];
extern LOOKUP_TYPE tab_cast_name[];
extern LOOKUP_TYPE tab_mob_def[];
extern LOOKUP_TYPE tab_mob_race_mods[];
extern eq_type tab_auto_obj[];
