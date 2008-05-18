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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"


/* The tables in here are:
     Mob type flags      :    tab_mob_flags       : bit_vector
     Mob affected by     :    tab_affected_by     : bit_vector
     Mob classes	 :    tab_mob_class	  : number
     Object item type    :    tab_item_types      : number
     Object extra flags  :    tab_obj_flags       : bit_vector
     Object wear flags   :    tab_wear_flags      : bit_vector
     Object affect types :    tab_obj_aff         : number
     Class types         :    tab_class           : bit_vector
     Wear locations      :    tab_wear_loc        : number
     Room flags          :    tab_room_flags      : bit_vector
     Sector types        :    tab_sector_types    : number
     Door types          :    tab_door_types      : bit_vector
     Door states         :    tab_door_states     : number
     
*/


/* Table for a mob's class... we'll mirror the PC classes in order here */
const struct lookup_type tab_mob_class[] = {
   {"mage", 0, 0},
   {"cleric", 1, 0},
   {"thief", 2, 0},
   {"warrior", 3, 0},
   {"psionicist", 4, 0},
   {"sorcerer", 5, 0},
   {"assassin", 6, 0},
   {"knight", 7, 0},
   {"necromancer", 8, 0},
   {"monk", 9, 0},
   {NULL, 0, 0}
};

/* -S- mod: hold what value0-3 mean for each type */
const struct lookup_type tab_value_meanings[] = {
   {"Unused", 10, 0},
   {"Unused", 11, 0},
   {"Hours of light, -1 = infinite", 12, 5},
   {"Unused", 13, 0},
   {"Unused", 14, 0},
   {"Unused", 15, 0},
   {"Replacer vnum", 16, 0},
   {"Forced extract timer", 17, 0},
   {"Unused", 18, 0},
   {"Unused", 19, 0},

   {"Level", 20, 100},
   {"Spell 1", 21, -50},
   {"Spell 2", 22, -50},
   {"Spell 3", 23, -50},
   {"Unused", 24, 0},
   {"Unused", 25, 0},
   {"Replacer vnum", 26, 0},
   {"Forced extract timer", 27, 0},
   {"Unused", 28, 0},
   {"Unused", 29, 0},

   {"Level", 30, 100},
   {"Max Charges", 31, 100},
   {"Current Charges", 32, 100},
   {"Spell", 33, -50},
   {"Unused", 34, 0},
   {"Unused", 35, 0},
   {"Replacer vnum", 36, 0},
   {"Forced extract timer", 37, 0},
   {"Unused", 38, 0},
   {"Unused", 39, 0},

   {"Level", 40, 100},
   {"Max Charges", 41, 100},
   {"Current Charges", 42, 100},
   {"Spell", 43, -50},
   {"Unused", 44, 0},
   {"Unused", 45, 0},
   {"Replacer vnum", 46, 0},
   {"Forced extract timer", 47, 0},
   {"Unused", 48, 0},
   {"Unused", 49, 0},

   {"Unused", 50, 0},
   {"Unused", 51, 0},
   {"Unused", 52, 0},
   {"Weapon Type", 53, -1},
   {"Unused", 54, 0},
   {"Unused", 55, 0},
   {"Replacer vnum", 56, 0},
   {"Forced extract timer", 57, 0},
   {"Unused", 58, 0},
   {"Unused", 59, 0},

   {"Unused", 80, 0},
   {"Unused", 81, 0},
   {"Unused", 82, 0},
   {"Unused", 83, 0},
   {"Unused", 84, 0},
   {"Unused", 85, 0},
   {"Replacer vnum", 86, 0},
   {"Forced extract timer", 87, 0},
   {"Unused", 88, 0},
   {"Unused", 89, 0},

   {"Unused", 90, 0},
   {"Unused", 91, 0},
   {"Unused", 92, 0},
   {"Unused", 93, 0},
   {"Unused", 94, 0},
   {"Unused", 95, 0},
   {"Replacer vnum", 96, 0},
   {"Forced extract timer", 97, 0},
   {"Unused", 98, 0},
   {"Unused", 99, 0},

   {"Level", 100, 50},
   {"Spell 1", 101, -25},
   {"Spell 2", 102, -25},
   {"Spell 3", 103, -25},
   {"Unused", 104, 0},
   {"Unused", 105, 0},
   {"Replacer vnum", 106, 0},
   {"Forced extract timer", 107, 0},
   {"Unused", 108, 0},
   {"Unused", 109, 0},

   {"CLUTCH_FUN index", 110, 10},
   {"As needed by that clutch_fun", 111, 10},
   {"As needed by that clutch_fun", 112, 10},
   {"As needed by that clutch_fun", 113, 10},
   {"Unused", 114, 0},
   {"Unused", 115, 0},
   {"Replacer vnum", 116, 0},
   {"Forced extract timer", 117, 0},
   {"Unused", 118, 0},
   {"Unused", 119, 0},


   {"Number of people that can use it.", 120, 0},
   {"Unused", 121, 0},
   {"Unused", 122, 0},
   {"Unused", 123, 0},
   {"Unused", 124, 0},
   {"Unused", 125, 0},
   {"Replacer vnum", 126, 0},
   {"Forced extract timer", 127, 0},
   {"Unused", 128, 0},
   {"Unused", 129, 0},

   {"Unused", 130, 0},
   {"Unused", 131, 0},
   {"Unused", 132, 0},
   {"Unused", 133, 0},
   {"Unused", 134, 0},
   {"Unused", 135, 0},
   {"Replacer vnum", 136, 0},
   {"Forced extract timer", 137, 0},
   {"Unused", 138, 0},
   {"Unused", 139, 0},

   {"Trigger type", 140, 0},
   {"Action type", 141, 0},
   {"Optional argument", 142, 0},
   {"Optional argument", 143, 0},
   {"Unused", 144, 0},
   {"Unused", 145, 0},
   {"Replacer vnum", 146, 0},
   {"Forced extract timer", 147, 0},
   {"Unused", 148, 0},
   {"Unused", 149, 0},


   {"Weight Capacity", 150, 10},
   {"Flags: 1:closeable 2:pickproof 4:closed 8:locked", 151, 0},
   {"Key Vnum", 152, 0},
   {"Unused", 153, 0},
   {"Unused", 154, 0},
   {"Unused", 155, 0},
   {"Replacer vnum", 156, 0},
   {"Forced extract timer", 157, 0},
   {"Unused", 158, 0},
   {"Unused", 159, 0},

   {"Value in Quest Points", 160, 10},
   {"Value in Practices", 161, 10},
   {"Value in Gold Pieces", 162, 10},
   {"Value in Experience Points", 163, 10},
   {"Unused", 164, 0},
   {"Unused", 165, 0},
   {"Replacer vnum", 166, 0},
   {"Forced extract timer", 167, 0},
   {"Unused", 168, 0},
   {"Unused", 169, 0},

   {"Capacity", 170, 10},
   {"Current Quantity", 171, 0},
   {"Liquid Number", 172, 0},
   {"If non-zero, drink is poisoned", 173, 100},
   {"Unused", 174, 0},
   {"Unused", 175, 0},
   {"Replacer vnum", 176, 0},
   {"Forced extract timer", 177, 0},
   {"Unused", 178, 0},
   {"Unused", 179, 0},

   {"Unused", 180, 0},
   {"Unused", 181, 0},
   {"Unused", 182, 0},
   {"Unused", 183, 0},
   {"Unused", 184, 0},
   {"Unused", 185, 0},
   {"Replacer vnum", 186, 0},
   {"Forced extract timer", 187, 0},
   {"Unused", 188, 0},
   {"Unused", 189, 0},

   {"Hours of Food Value", 190, 10},
   {"Unused", 191, 0},
   {"Unused", 192, 0},
   {"If non-zero, food is poisoned", 193, 100},
   {"Unused", 194, 0},
   {"Unused", 195, 0},
   {"Replacer vnum", 196, 0},
   {"Forced extract timer", 197, 0},
   {"Unused", 198, 0},
   {"Unused", 199, 0},

   {"Value in GP", 200, 1},
   {"Unused", 201, 0},
   {"Unused", 202, 0},
   {"Unused", 203, 0},
   {"Unused", 204, 0},
   {"Unused", 205, 0},
   {"Replacer vnum", 206, 0},
   {"Forced extract timer", 207, 0},
   {"Unused", 208, 0},
   {"Unused", 209, 0},

   {"Unused", 220, 0},
   {"Unused", 221, 0},
   {"Unused", 222, 0},
   {"Unused", 223, 0},
   {"Unused", 224, 0},
   {"Unused", 225, 0},
   {"Replacer vnum", 226, 0},
   {"Forced extract timer", 227, 0},
   {"Unused", 228, 0},
   {"Unused", 229, 0},

   {"Unused", 230, 0},
   {"Unused", 231, 0},
   {"Unused", 232, 0},
   {"Unused", 233, 0},
   {"Unused", 234, 0},
   {"Unused", 235, 0},
   {"Replacer Vnum", 236, 0},
   {"Forced extract timer", 237, 0},
   {"Unused", 238, 0},
   {"Unused", 239, 0},

   {"Unused", 240, 0},
   {"Unused", 241, 0},
   {"Unused", 242, 0},
   {"Unused", 243, 0},
   {"Unused", 244, 0},
   {"Unused", 245, 0},
   {"Replacer Vnum", 246, 0},
   {"Forced extract timer", 247, 0},
   {"Unused", 248, 0},
   {"Unused", 249, 0},

   {"Liquid Number", 250, 10},
   {"Posioned if non-zero", 251, 0},
   {"Unused", 252, 0},
   {"Unused", 253, 0},
   {"Unused", 254, 0},
   {"Unused", 255, 0},
   {"Replacer Vnum", 256, 0},
   {"Forced extract timer", 257, 0},
   {"Unused", 258, 0},
   {"Unused", 259, 0},

   {"Level", 260, 50},
   {"Spell 1", 261, -25},
   {"Spell 2", 262, -25},
   {"Spell 3", 263, -25},
   {"Unused", 264, 0},
   {"Unused", 265, 0},
   {"Replacer Vnum", 266, 0},
   {"Forced extract timer", 267, 0},
   {"Unused", 268, 0},
   {"Unused", 269, 0},

   {"No. of days message will last.", 270, 10},
   {"Minimum level to write board (read?).", 271, 0},
   {"Minimum level to look board (write?).", 272, 0},
   {"BOARD vnum... set to object's vnum.", 273, 0},
   {"Unused", 274, 0},
   {"Unused", 275, 0},
   {"Unused", 276, 0},
   {"Unused", 277, 0},
   {"Unused", 278, 0},
   {"Unused", 279, 0},

   {"Unused", 280, 0},
   {"Unused", 281, 0},
   {"Unused", 282, 0},
   {"Unused", 283, 0},
   {"Unused", 284, 0},
   {"Unused", 285, 0},
   {"Replacer Vnum", 286, 0},
   {"Forced extract timer", 287, 0},
   {"Unused", 288, 0},
   {"Unused", 289, 0},

   {"Previous piece vnum in connect sequence", 290, 0},
   {"Next piece vnum in connect sequence", 291, 0},
   {"Replacement vnum to load on connect", 292, 0},
   {"Unused", 293, 0},
   {"Unused", 294, 0},
   {"Unused", 295, 0},
   {"Replacer Vnum", 296, 0},
   {"Forced extract timer", 297, 0},
   {"Unused", 298, 0},
   {"Unused", 299, 0},

   {"Unused", 300, 0},
   {"Unused", 301, 0},
   {"Unused", 302, 0},
   {"Unused", 303, 0},
   {"Unused", 304, 0},
   {"Unused", 305, 0},
   {"Replacer Vnum", 306, 0},
   {"Forced extract timer", 307, 0},
   {"Unused", 308, 0},
   {"Unused", 309, 0},

   {"Unused", 310, 0},
   {"Unused", 311, 0},
   {"Unused", 312, 0},
   {"Unused", 313, 0},
   {"Unused", 314, 0},
   {"Unused", 315, 0},
   {"Replacer Vnum", 316, 0},
   {"Forced extract timer", 317, 0},
   {"Unused", 318, 0},
   {"Unused", 319, 0},

   {"Unused", 320, 0},
   {"Unused", 321, 0},
   {"Unused", 322, 0},
   {"Unused", 323, 0},
   {"Unused", 324, 0},
   {"Unused", 325, 0},
   {"Replacer Vnum", 326, 0},
   {"Forced extract timer", 327, 0},
   {"Unused", 328, 0},
   {"Unused", 329, 0},

   {"Unused", 330, 0},
   {"Unused", 331, 0},
   {"Unused", 332, 0},
   {"Unused", 333, 0},
   {"Unused", 334, 0},
   {"Unused", 335, 0},
   {"Replacer Vnum", 336, 0},
   {"Forced extract timer", 337, 0},
   {"Unused", 338, 0},
   {"Unused", 339, 0},

   {"Volume", 340, 0},
   {"Weight", 341, 0},
   {"Unused", 342, 0},
   {"Unused", 343, 0},
   {"Unused", 344, 0},
   {"Unused", 345, 0},
   {"Replacer Vnum", 346, 0},
   {"Forced extract timer", 347, 0},
   {"Unused", 348, 0},
   {"Unused", 349, 0},

   {"Unused", 350, 0},
   {"Unused", 351, 0},
   {"Unused", 352, 0},
   {"Unused", 353, 0},
   {"Unused", 354, 0},
   {"Unused", 355, 0},
   {"Replacer Vnum", 356, 0},
   {"Forced extract timer", 357, 0},
   {"Unused", 358, 0},
   {"Unused", 359, 0},

   {NULL, 0}
};



const struct lookup_type tab_drink_types[] = {
   {"Water", 0, 10},
   {"Beer", 1, 10},
   {"Wine", 2, 10},
   {"Ale", 3, 10},
   {"Dark Ale", 4, 10},
   {"Whisky", 5, 10},
   {"Lemonade", 6, 10},
   {"Firebreather", 7, 10},
   {"Local Specialty", 8, 10},
   {"Slime Mold Juice", 9, 20},
   {"Milk", 10, 10},
   {"Tea", 11, 10},
   {"Coffee", 12, 10},
   {"Blood", 13, 20},
   {"Salt Water", 14, 10},
   {"Chocolate Milk", 15, 10},
   {NULL, 0, 0}
};

const struct lookup_type tab_weapon_types[] = {
   {"hit", 0, 10},
   {"slice", 1, 10},
   {"stab", 2, 30},
   {"slash", 3, 30},
   {"whip", 4, 10},
   {"claw", 5, 10},
   {"blast", 6, 10},
   {"pound", 7, 10},
   {"crush", 8, 10},
   {"grep", 9, 10},
   {"bite", 10, 10},
   {"pierce", 11, 10},
   {"suction", 12, 10}, /* kinky */
   {NULL, 0}
};

const struct lookup_type tab_mob_flags[] = {
   {"nada", 0, 0},
   {"is_npc", 1, 0}, /* Auto set for mobs    */
   {"sentinel", 2, 100},   /* stays in one room    */
   {"scavenger", 4, 300},  /* picks up objects     */
   {"remember", 8, 100},   /* remembers target     */
   {"no_flee", 16, 50}, /* can't flee from mob  */
   {"aggressive", 32, 100},   /* attacks pc's         */
   {"stay_area", 64, 10},  /* won't leave area     */
   {"wimpy", 128, 100}, /* flees when hurt      */
   {"pet", 256, 100},   /* auto set for pets    */
   {"train", 512, 2000},   /* can train pc's       */
   {"practice", 1024, 2000},  /* can practice pc's    */
   {"mercenary", 2048, 100},  /* is a mercenary       */
   {"heal", 4096, 5000},   /* sells heals          */
   {"adapt", 8192, 4000},  /* adapts weapons       */
   {"undead", 16384, 10000},  /* TBA                  */
   {"bank", 32768, 400},   /* is a bank            */
   {"no_body", 65536, 1000},  /* Doesn't have body locations */
   {"hunter", 131072, 4000},  /* HUNTS */
   {"no_mind", 262144, 100},  /* immune to some psi's  */
   {"postman", 524288, 1000}, /* handles letters */
   {"rewield", 1048576, 1000},   /* looks for better weapons */
   {"reequip", 2097152, 1000},   /* looks for better armor */
   {"intelligent", BIT_23, NO_USE},
   {"vampire", BIT_24, 1000000},
   {"breeder", BIT_25, NO_USE},
   {"solo", BIT_26, 5000}, /*  mob is designed to fight solo */
   {"werewolf", BIT_27, NO_USE},
   {"mount", BIT_28, 1000},
   {"no_blood", BIT_29, 4000},
   {NULL, 0}
};

/* New bits to handle how mobs act */

const struct lookup_type tab_mob_skill[] = {
   {"nada", 1, 0},
   {"2_attack", 2, 100},
   {"3_attack", 4, 200},
   {"4_attack", 8, 400},
   {"punch", 16, 200},
   {"headbutt", 32, 200},
   {"knee", 64, 200},
   {"disarm", 128, 400},
   {"trip", 256, 300},
   {"nodisarm", 512, 500},
   {"notrip", 1024, 500},
   {"dodge", 2048, 200},
   {"parry", 4096, 200},
   {"martial", 8192, 300},
   {"enhanced", 16384, 400},
   {"dualwield", 32768, 350},
   {"dirt", 65536, 300},
   {"5_attack", 131072, 500},
   {"6_attack", 262144, 600},
   {"charge", 524288, 700},
   {NULL, 0, 0}
};
const struct lookup_type tab_mob_powerskill[] = {
   {"nada", 1, 0},
   {"2_attack", 2, 100},
   {"3_attack", 4, 200},
   {"4_attack", 8, 400},
   {"punch", 16, 200},
   {"headbutt", 32, 200},
   {"knee", 64, 200},
   {"disarm", 128, 400},
   {"trip", 256, 300},
   {"nodisarm", 512, 500},
   {"notrip", 1024, 500},
   {"dodge", 2048, 200},
   {"parry", 4096, 200},
   {"martial", 8192, 300},
   {"enhanced", 16384, 400},
   {"dualwield", 32768, 350},
   {"dirt", 65536, 300},
   {"5_attack", 131072, 500},
   {"6_attack", 262144, 600},
   {"charge", 524288, 700},
   {NULL, 0, 0}
};
const struct lookup_type tab_mob_cast[] = {
   {"nada", 0, 0},
   {"placeholder", 1, 0},
   {"mag_missile", 2, 100},
   {"shock_grasp", 4, 110},
   {"burn_hands", 8, 130},
   {"col_spray", 16, 150},
   {"fireball", 32, 250},
   {"hellspawn", 64, 300},
   {"acid_blast", 128, 350},
   {"chain_light", 256, 400},
   {"faerie_fire", 512, 300},
   {"flare", 1024, 450},   /* 10 */
   {"flamestrike", 2048, 500},
   {"earthquake", 4096, 550},
   {"mind_flail", 8192, 100},
   {"planergy", 16384, 200},
   {"phobia", 32768, 250},
   {"mind_bolt", 65536, 300},
   {"static", 131072, 350},
   {"ego_whip", 262144, 375},
   {"bloody_tears", 524288, 500},
   {"mindflame", 1048576, 600},  /* 20 */
   {"suffocate", 2097152, 650},
   {"nerve_fire", 4194304, 700},
   {"light_bolt", 8388608, 200},
   {"heat_armor", 16777216, 400},
   {"lava_burst", 33554432, 300},
   {NULL, 0, 0}
};

const struct lookup_type tab_cast_name[] = {
   {"nada", 0, 0},
   {"placeholder", 1, 0},
   {"\'magic missile\'", 2, 100},
   {"\'shocking grasp\'", 4, 110},
   {"\'burning hands\'", 8, 130},
   {"\'color spray\'", 16, 150},
   {"fireball", 32, 250},
   {"hellspawn", 64, 300},
   {"\'acid blast\'", 128, 350},
   {"\'chain lightning\'", 256, 400},
   {"\'faerie fire\'", 512, 300},
   {"flare", 1024, 450},   /* 10 */
   {"flamestrike", 2048, 500},
   {"earthquake", 4096, 550},
   {"\'mind flail\'", 8192, 100},
   {"planergy", 16384, 200},
   {"phobia", 32768, 250},
   {"\'mind bolt\'", 65536, 300},
   {"static", 131072, 350},
   {"\'ego whip\'", 262144, 375},
   {"\'bloody tears\'", 524288, 500},
   {"mindflame", 1048576, 600},  /* 20 */
   {"suffocate", 2097152, 650},
   {"\'nerve fire\'", 4194304, 700},
   {"\'light bolt\'", 8388608, 200},
   {"\'heat armor\'", 16777216, 400},
   {"\'lava burst\'", 33554432, 300},
   {NULL, 0, 0}
};

const struct lookup_type tab_mob_powercast[] = {
   {"nada", 0, 0},
   {"placeholder", 1, 0},
   {"mag_missile", 2, 100},
   {"shock_grasp", 4, 110},
   {"burn_hands", 8, 130},
   {"col_spray", 16, 150},
   {"fireball", 32, 250},
   {"hellspawn", 64, 300},
   {"acid_blast", 128, 350},
   {"chain_light", 256, 400},
   {"faerie_fire", 512, 300},
   {"flare", 1024, 450},   /* 10 */
   {"flamestrike", 2048, 500},
   {"earthquake", 4096, 550},
   {"mind_flail", 8192, 100},
   {"planergy", 16384, 200},
   {"phobia", 32768, 250},
   {"mind_bolt", 65536, 300},
   {"static", 131072, 350},
   {"ego_whip", 262144, 375},
   {"bloody_tears", 524288, 500},
   {"mindflame", 1048576, 600},  /* 20 */
   {"suffocate", 2097152, 650},
   {"nerve_fire", 4194304, 700},
   {"light_bolt", 8388608, 200},
   {"heat_armor", 16777216, 400},
   {"lava_burst", 33554432, 300},
   {NULL, 0, 0}
};
const struct lookup_type tab_mob_def[] = {
   {"nada", 1, 0},
   {"cure_light", 2, 100},
   {"cure_serious", 4, 200},
   {"cure_critic", 8, 400},
   {"heal", 16, 800},
   {"fireshield", 32, 1000},
   {"iceshield", 64, 1500},
   {"shockshield", 128, 1200},
   {NULL, 0, 0}
};

const struct lookup_type tab_affected_by[] =
/*
 * Bits for 'affected_by'.
 * Used in #MOBILES.
 */
{
   {"nada", 0, 0},
   {"blind", 1, -100},
   {"invisible", 2, 1000},
   {"detect_evil", 4, 500},
   {"detect_invis", 8, 1000},
   {"detect_magic", 16, 500},
   {"detect_hidden", 32, 2000},
   {"cloak:reflection", 64, 2000},
   {"sanctuary", 128, 5000},
   {"faerie_fire", 256, 0},
   {"infrared", 512, 1000},
   {"curse", 1024, 0},
   {"cloak:flaming", 2048, 2000},
   {"poison", 4096, 0},
   {"protect", 8192, 1000},
   {"cloak:absorption", 16384, 2000},  /* unused       */
   {"sneak", 32768, 1000},
   {"hide", 65536, 1000},
   {"sleep", 131072, 0},
   {"charm", 262144, 2000},
   {"flying", 524288, 5000},
   {"pass_door", 1048576, 5000},
   {NULL, 0}
};


const struct lookup_type tab_item_types[] = {
/* { "nada",			0, 0 },  */
/* { "placeholder",		0, 0 },   */
   {"light", 1, 20},
   {"scroll", 2, 1000},
   {"wand", 3, 2000},
   {"staff", 4, 5000},
   {"weapon", 5, 500},
   {"beacon", 6, 5000},
   {"portal", 7, 0},
   {"treasure", 8, 0},
   {"armor", 9, 500},
   {"potion", 10, 1000},
   {"clutch", 11, 0},
   {"furniture", 12, 20},
   {"trash", 13, 0},
   {"trigger", 14, 0},
   {"container", 15, 50},
   {"quest", 16, NO_USE},
   {"drink_con", 17, 20},
   {"key", 18, 100},
   {"food", 19, 50},
   {"money", 20, 0},
   {"stake", 21, 0},
   {"boat", 22, 1000},
   {"corpse_npc", 23, 0},
   {"corpse_pc", 24, 0},
   {"fountain", 25, 500},
   {"pill", 26, 50},
   {"board", 27, 2000},
   {"soul", 28, 3000},
   {"piece", 29, 50},
   {"matrix", 30, 50},
   {"enchantment", 31, 50},
   {"present", 32, 0},
   {NULL, 0}
};

const struct lookup_type tab_obj_flags[] = {
   {"nada", 0, 0},
   {"glow", 1, 10},
   {"hum", 2, 10},
   {"nodisarm", 4, 20},
   {"lock", 8, 20},
   {"evil", 16, 50},
   {"invis", 32, 200},
   {"magic", 64, 100},
   {"nodrop", 128, 20},
   {"bless", 256, 200},
   {"anti_good", 512, 20},
   {"anti_evil", 1024, 20},
   {"anti_neutral", 2048, 20},
   {"noremove", 4096, 100},
   {"inventory", 8192, 0},
   {"nosave", 16384, 1000},
   {"claneq", 32768, NO_USE},
   {"trigger:destroy", 65536, 10},
   {"no_auction", 131072, 0},
   {"remort", 262144, 9},
   {"rare", 1048576, 1000},
   {"vamp", 2097152, 9},
   {"noloot", 4194304, 20},
   {"nosac", 8388608, 100},
   {"unique", 16777216, 100},
   {"lifestealer", BIT_26, 100},
   {"silver", BIT_27, 100},
   {NULL, 0}
};

const struct lookup_type tab_wear_flags[] = {
   {"halo", BIT_1, 50},
   {"aura", BIT_2, 50},
   {"horns", BIT_3, 20},
   {"head", BIT_4, 40},
   {"face", BIT_5, 50},
   {"beak", BIT_6, 50},
   {"ear", BIT_7, 40},
   {"neck", BIT_8, 20},
   {"wings", BIT_9, 30},
   {"shoulders", BIT_10, 40},
   {"arms", BIT_11, 100},
   {"wrist", BIT_12, 20},
   {"hands", BIT_13, 50},
   {"finger", BIT_14, 40},
   {"claws", BIT_15, 200},
   {"hold", BIT_16, 100},
   {"about", BIT_17, 30},
   {"waist", BIT_18, 20},
   {"body", BIT_19, 100},
   {"tail", BIT_20, 900},
   {"legs", BIT_21, 100},
   {"feet", BIT_22, 900},
   {"hooves", BIT_23, 100},
   {"take", BIT_24, 900},
   {NULL, 0}
};

/* This not used at present.  Thought i'd bung details in though :) */
const struct lookup_type tab_item_apply[] = {

   {"nada", 1, 0},
   {"infra", 2, 0},
   {"invis", 4, 0},
   {"det_invis", 8, 0},
   {"sanc", 16, 0},
   {"sneak", 32, 0},
   {"hide", 64, 0},
   {"prot", 128, 0},
   {"enhanced", 256, 0},
   {"det_mag", 512, 0},
   {"det_hid", 1024, 0},
   {"det_evil", 2048, 0},
   {"pass_door", 4096, 0},
   {"det_poison", 8192, 0},
   {"fly", 16384, 0},
   {"know_align", 32768, 0},
   {"detect_undead", 65536, 0},
   {"heated", 131072, 0},
   {NULL, 0}
};



const struct lookup_type tab_wear_loc[] = {
   {"halo", 1, 20},
   {"aura", 2, 20},
   {"horns", 3, 20},
   {"head", 4, 20},
   {"face", 5, 20},
   {"beak", 6, 20},
   {"ear_l", 7, 40},
   {"ear_r", 8, 40},
   {"neck_1", 9, 40},
   {"neck_2", 10, 40},
   {"wings", 11, 40},
   {"shoulders", 12, 20},
   {"arms", 13, 20},
   {"wrist_l", 14, 50},
   {"wrist_r", 15, 20},
   {"hands", 16, 20},
   {"finger_l", 17, 30},
   {"finger_r", 18, 30},
   {"claws", 19, 100},
   {"hold_l", 20, 100},
   {"hold_r", 21, 40},
   {"cape", 22, 20},
   {"waist", 23, 20},
   {"body", 24, 40},
   {"tail", 25, 80},
   {"legs", 26, 20},
   {"feet", 27, 40},
   {"hooves", 28, 80},

   {"max_wear", 29, 0},
   {NULL, 0}
};


const struct lookup_type tab_obj_aff[] = {
   {"nada", 0, 0},
   {"str", 1, 200},
   {"dex", 2, 200},
   {"int", 3, 200},
   {"wis", 4, 200},
   {"con", 5, 200},
   {"sex", 6, 30},
   {"class", 7, 500},
   {"level", 8, 2000},
   {"age", 9, 30},
   {"height", 10, 30},
   {"weight", 11, 30},
   {"mana", 12, 400},
   {"hit", 13, 500},
   {"move", 14, 200},
   {"gold", 15, 10},
   {"exp", 16, 50},
   {"ac", 17, 200},
   {"hitroll", 18, 500},
   {"damroll", 19, 500},
   {"saving_para", 20, 400},
   {"saving_rod", 21, 400},
   {"saving_petri", 22, 400},
   {"saving_breath", 23, 400},
   {"saving_spell", 24, 400},
   {NULL, 0}
};


const struct lookup_type tab_room_flags[] = {
   {"nada", 0, 0},
   {"dark", 1, 1000},
   {"regen", 2, 4000},  /* increased regen in this room */
   {"no_mob", 4, 3000},
   {"indoors", 8, 500},
   {"no_magic", 16, 5000},
   {"hot", 32, 1000},   /* players lose hp each tick   */
   {"cold", 64, 1000},  /* players lose hp each tick   */
   {"pk", 128, 200}, /* players may freely pk here  */
   {"quiet", 256, 200}, /* room is quiet               */
   {"private", 512, 1000},
   {"safe", 1024, 4000},
   {"solitary", 2048, 1000},
   {"pet_shop", 4096, NO_USE},
   {"no_recall", 8192, 1000},
   {"no_teleport", 16384, 500},
   {"hunt_hunt", 32768, NO_USE},
   {"no_bloodwalk", 65536, 4000},
   {"no_portal", 131072, 1000},
   {"no_repop", BIT_19, 10},
   {NULL, 0}
};

const struct lookup_type tab_sector_types[] = {
   {"nada", 0, 0},
   {"city", 1, 50},
   {"field", 2, 50},
   {"forest", 3, 50},
   {"hills", 4, 50},
   {"mountain", 5, 50},
   {"water_swim", 6, 50},
   {"water_noswim", 7, 50},
   {"recall_set", 8, 50},
   {"air", 9, 50},
   {"desert", 10, 50},
   {"inside", 11, 50},
   {"max", 12, 0},
   {NULL, 0}
};

const struct lookup_type tab_door_types[] = {
   /*
    * { "nada",        0, NO_USE },  
    */
   {"door", 1, 50},
   {"closed", 2, 0},
   {"locked", 4, 0},
   {"climb", 8, 0},
   {"immortal", 16, NO_USE},
   {"pickproof", 32, 500},
   {"smashproof", 64, 500},
   {"passproof", 128, 500},
   {"nodetect", 256, 900},
   {NULL, 0}
};

const struct lookup_type tab_door_states[] = {
   {"open", 0, 0},
   {"closed", 1, 0},
   {"locked", 2, 0},
   {NULL, 0}
};

const struct lookup_type tab_player_flags[] = {
   {"nada", 0, 0},
   {"PKOK", 1, 0},
   {"AFK", 2, 0},
   {"AMBASSADOR", 4, 0},
   {"VAMPIRE", 8, NO_USE},
   {"C_DIPLOMAT", 16, NO_USE},
   {"C_BOSS", 32, NO_USE},
   {"C_TREASURER", 64, NO_USE},
   {"C_ARMORER", 128, NO_USE},
   {"C_LEADER", 256, NO_USE},
   {"SUPER_COUNCIL", 512, NO_USE},
   {"WEREWOLF", 1024, 0},
   {"SHIFTED", 2048, 0},
   {"RAGED", 4096, 0},
   {"RULER", BIT_14, 0},
   {"BLIND_PLAYER", BIT_15, 0},
   {"TESTER", BIT_16, 0},
   {NULL, 0}
};


const struct lookup_type tab_magic_realms[] = {

   {"nada", BIT_0, 0},
   {"fire", BIT_1, 0},
   {"shock", BIT_2, 0},
   {"light", BIT_3, 0},
   {"gas", BIT_4, 0},
   {"poison", BIT_5, 0},
   {"cold", BIT_6, 0},
   {"sound", BIT_7, 0},
   {"acid", BIT_8, 0},
   {"negation", BIT_9, 0},
   {"impact", BIT_10, 0},
   {"psionic", BIT_11, 0},
   {"holy", BIT_12, 0},
   {NULL, 0}
};


const struct lookup_type tab_mob_race_mods[] = {

   {"nada", BIT_0, 0},
   {"fast_heal", BIT_1, 0},
   {"slow_heal", BIT_2, 0},
   {"strong_magic", BIT_3, 0},
   {"weak_magic", BIT_4, 0},
   {"no_magic", BIT_5, 0},
   {"immune_poison", BIT_6, 0},
   {"resist_spell", BIT_7, 0},
   {"woodland", BIT_8, 0},
   {"darkness", BIT_9, 0},
   {"huge", BIT_10, 0},
   {"large", BIT_11, 0},
   {"tiny", BIT_12, 0},
   {"small", BIT_13, 0},
   {"tail", BIT_14, 0},
   {"tough_skin", BIT_15, 0},
   {"stone_skin", BIT_16, 0},
   {"iron_skin", BIT_17, 0},
   {NULL, 0}

};

/* Now for the functions */

unsigned long int table_lookup( const struct lookup_type *table, char *name )
{
   int a;

   if( name[0] == '\0' )
      return /* table[0].value-1 */ 0;

   for( a = 0; table[a].text != NULL; a++ )
      if( !str_prefix( name, table[a].text ) )
         return ( ( !str_cmp( table[a].text, "nada" ) ) ? 0 : table[a].value );
   return /* table[0].value-1  */ 0;
}

char *rev_table_lookup( const struct lookup_type *table, unsigned long int number )
{
   int a;

   for( a = 0; table[a].text != NULL; a++ )
      if( table[a].value == number )
         return table[a].text;
   return "";
}

/* spec: fixed to not assume contiguous bit use */

char *bit_table_lookup( const struct lookup_type *table, unsigned long int number )
{
   int a;
   static char buf[MAX_STRING_LENGTH];

   buf[0] = '\0';

   for( a = 0; number && table[a].text; a++ )
   {
      if( ( number & table[a].value ) == table[a].value &&
          str_cmp( table[a].text, "nada" ) && str_cmp( table[a].text, "placeholder" ) )
      {
         strcat( buf, table[a].text );
         strcat( buf, ", " );
         number &= ~table[a].value;
      }
   }

   if( buf[0] == '\0' )
      strcat( buf, "none" );
   else
   {
      a = strlen( buf );
      buf[a - 2] = '.';
      buf[a - 1] = '\0';
   }
   return buf;
}

void table_printout( const struct lookup_type *table, char *buf )
{
   int a;

   a = 0;
   buf[0] = '\0';

   for( a = 0; table[a].text != NULL; a++ )
   {
      if( ( strcmp( table[a].text, "nada" ) ) && ( strcmp( table[a].text, "placeholder" ) ) )   /* If not an invalid choice */
      {
         strcat( buf, "          " );
         strcat( buf, table[a].text );
         strcat( buf, "\n\r" );
      }
   }

   return;
}

void wide_table_printout( const struct lookup_type *table, char *buf )
{
   /*
    * Like table_printout, but formats into columns 
    */

   char tmp[MAX_STRING_LENGTH];
   int a;
   int foo; /* work out how many values shown in a row */

   a = 0;
   foo = 0;
   buf[0] = '\0';

   for( a = 0; table[a].text != NULL; a++ )
   {
      if( ( strcmp( table[a].text, "nada" ) )   /* If not an invalid choice */
          && ( strcmp( table[a].text, "nada" ) ) )
      {
         strcat( buf, "     " );
         sprintf( tmp, "%12s", table[a].text );
         strcat( buf, tmp );
         if( ++foo % 4 == 0 )
            strcat( buf, "\n\r" );
      }
   }
   strcat( buf, "\n\r" );
   return;
}

char *show_values( const struct lookup_type *table, int value, bool fBit )
{

   char tmp[MAX_STRING_LENGTH];
   static char buf[MAX_STRING_LENGTH];
   int a;
   int foo; /* work out how many values shown in a row */

   a = 0;
   foo = 0;
   buf[0] = '\0';

   for( a = 0; table[a].text != NULL; a++ )
   {
      if( ( strcmp( table[a].text, "nada" ) )   /* If not an invalid choice */
          && ( strcmp( table[a].text, "placeholder" ) ) )
      {
         strcat( buf, "     " );
         sprintf( tmp, "%s%-13s",
                  fBit ? ( IS_SET( value, table[a].value ) ? "@@y*" : "@@g " ) :
                  ( value == table[a].value ? "@@y*" : "@@g " ), table[a].text );
         strcat( buf, tmp );
         if( ++foo % 4 == 0 )
            strcat( buf, "\n\r" );
      }
   }
   strcat( buf, "@@g\n\r" );
   return ( buf );
}
