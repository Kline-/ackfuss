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


extern unsigned long int table_lookup( const struct lookup_type *, char * );
extern char *rev_table_lookup( const struct lookup_type *, unsigned long int );
extern char *bit_table_lookup( const struct lookup_type *, unsigned long int );
extern void table_printout( const struct lookup_type *, char * );
extern void wide_table_printout( const struct lookup_type *, char * );  /* -S- mod */

extern const struct lookup_type tab_mob_class[];
extern const struct lookup_type tab_value_meanings[];
extern const struct lookup_type tab_drink_types[];
extern const struct lookup_type tab_weapon_types[];
extern const struct lookup_type tab_mob_flags[];
extern const struct lookup_type tab_affected_by[];
extern const struct lookup_type tab_item_types[];
extern const struct lookup_type tab_obj_flags[];
extern const struct lookup_type tab_wear_flags[];
extern const struct lookup_type tab_item_apply[];
extern const struct lookup_type tab_wear_loc[];
extern const struct lookup_type tab_obj_aff[];
extern const struct lookup_type tab_room_flags[];
extern const struct lookup_type tab_sector_types[];
extern const struct lookup_type tab_door_types[];
extern const struct lookup_type tab_door_states[];
extern const struct lookup_type tab_player_flags[];
extern const struct lookup_type tab_mob_skill[];
extern const struct lookup_type tab_mob_cast[];
extern const struct lookup_type tab_mob_powerskill[];
extern const struct lookup_type tab_mob_powercast[];
extern const struct lookup_type tab_magic_realms[];
extern const struct lookup_type tab_mob_race_mods[];
extern const struct lookup_type tab_cast_name[];
extern const struct lookup_type tab_mob_def[];
extern const struct lookup_type tab_mob_race_mods[];
