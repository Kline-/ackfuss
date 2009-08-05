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

#include "h/globals.h"

/* The tables in here are:
     Mob act flags       :    tab_mob_act         : bitset
     Player act flags    :    tab_player_act      : bitset
     Mob affected by     :    tab_affected_by     : bit_vector
     Mob classes	 :    tab_mob_class	  : number
     Object item type    :    tab_item_types      : number
     Object extra flags  :    tab_obj_flags       : bitset
     Object wear flags   :    tab_wear_flags      : bitset
     Object affect types :    tab_obj_aff         : number
     Class types         :    tab_class           : bit_vector
     Wear locations      :    tab_wear_loc        : number
     Room flags          :    tab_room_flags      : bitset
     Sector types        :    tab_sector_types    : number
     Door types          :    tab_door_types      : bitset
     Door states         :    tab_door_states     : number
*/


/* Table for a mob's class... we'll mirror the PC classes in order here */
LOOKUP_TYPE tab_mob_class[] = {
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
LOOKUP_TYPE tab_value_meanings[] = {
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
   {"Minimum Damage", 51, 0},
   {"Maximum Damage", 52, 0},
   {"Weapon Type", 53, -1},
   {"Unused", 54, 0},
   {"Unused", 55, 0},
   {"Replacer vnum", 56, 0},
   {"Forced extract timer", 57, 0},
   {"Unused", 58, 0},
   {"Unused", 59, 0},

   {"Dest. Room vnum", 70, 0},
   {"Can Look In (1=yes 0=no)", 71, 0},
   {"Can Enter (1=yes 0=no)", 72, 0},
   {"Unused", 73, 0},
   {"Unused", 74, 0},
   {"Unused", 75, 0},
   {"Replacer vnum", 76, 0},
   {"Forced extract timer", 77, 0},
   {"Unused", 78, 0},
   {"Unused", 79, 0},

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

   {"Unused", 160, 10},
   {"Unused", 161, 10},
   {"Unused", 162, 10},
   {"Unused", 163, 10},
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

   {"Charm Affect", 330, 0},
   {"Bonus (%)", 331, 0},
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

   {"Unused", 360, 0},
   {"Unused", 361, 0},
   {"Unused", 362, 0},
   {"Unused", 363, 0},
   {"Unused", 364, 0},
   {"Unused", 365, 0},
   {"Replacer Vnum", 366, 0},
   {"Forced extract timer", 367, 0},
   {"Unused", 368, 0},
   {"Unused", 369, 0},

   {"Unused", 370, 0},
   {"Unused", 371, 0},
   {"Unused", 372, 0},
   {"Unused", 373, 0},
   {"Unused", 374, 0},
   {"Unused", 375, 0},
   {"Replacer Vnum", 376, 0},
   {"Forced extract timer", 377, 0},
   {"Unused", 378, 0},
   {"Unused", 379, 0},

   {"Unused", 380, 0},
   {"Unused", 381, 0},
   {"Unused", 382, 0},
   {"Unused", 383, 0},
   {"Unused", 384, 0},
   {"Unused", 385, 0},
   {"Replacer Vnum", 386, 0},
   {"Forced extract timer", 387, 0},
   {"Unused", 388, 0},
   {"Unused", 389, 0},

   {NULL, 0}
};



LOOKUP_TYPE tab_drink_types[] = {
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

LOOKUP_TYPE tab_weapon_types[] = {
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

LOOKUP_TYPE tab_charm_types[] = {
   {"exp",        CHARM_AFF_EXP,        10},
   {"gold",       CHARM_AFF_GOLD,       10},
   {"melee",      CHARM_AFF_BATTLE,     10},
   {"magic",      CHARM_AFF_MAGE,       10},
   {"regen",      CHARM_AFF_REGEN,      10},
   {"hungerless", CHARM_AFF_HUNGERLESS, 10},
   {NULL, 0}
};

LOOKUP_TYPE tab_mob_act[] = {
   {"nada",        ACT_NONE, 0},
   {"sentinel",    ACT_SENTINEL,    100    }, /* stays in one room    */
   {"scavenger",   ACT_SCAVENGER,   300    }, /* picks up objects     */
   {"remember",    ACT_REMEMBER,    100    }, /* remembers target     */
   {"no_flee",     ACT_NO_FLEE,     50     }, /* can't flee from mob  */
   {"aggressive",  ACT_AGGRESSIVE,  100    }, /* attacks pc's         */
   {"stay_area",   ACT_STAY_AREA,   10     }, /* won't leave area     */
   {"wimpy",       ACT_WIMPY,       100    }, /* flees when hurt      */
   {"pet",         ACT_PET,         100    }, /* auto set for pets    */
   {"train",       ACT_TRAIN,       2000   }, /* can train pc's       */
   {"practice",    ACT_PRACTICE,    2000   }, /* can practice pc's    */
   {"mercenary",   ACT_MERCENARY,   100    }, /* is a mercenary       */
   {"heal",        ACT_HEAL,        5000   }, /* sells heals          */
   {"adapt",       ACT_ADAPT,       4000   }, /* adapts weapons       */
   {"undead",      ACT_UNDEAD,      10000  }, /* TBA                  */
   {"bank",        ACT_BANKER,      400    }, /* is a bank            */
   {"no_body",     ACT_NO_BODY,     1000   }, /* Doesn't have body locations */
   {"hunter",      ACT_HUNTER,      4000   }, /* HUNTS */
   {"no_mind",     ACT_NO_MIND,     100    }, /* immune to some psi's  */
   {"postman",     ACT_POSTMAN,     1000   }, /* handles letters */
   {"rewield",     ACT_RE_WIELD,    1000   }, /* looks for better weapons */
   {"reequip",     ACT_RE_EQUIP,    1000   }, /* looks for better armor */
   {"intelligent", ACT_INTELLIGENT, NO_USE },
   {"vampire",     ACT_VAMPIRE,     1000000},
   {"breeder",     ACT_BREEDER,     NO_USE },
   {"solo",        ACT_SOLO,        5000   }, /*  mob is designed to fight solo */
   {"werewolf",    ACT_WEREWOLF,    NO_USE },
   {"mount",       ACT_MOUNT,       1000   },
   {"no_blood",    ACT_NO_BLOOD,    4000   },
   {"no_quest",    ACT_NO_QUEST,    0      },
   {"questmaster", ACT_QUESTMASTER, 0      },
   {"shifted",     ACT_SHIFTED,     NO_USE },
   {"raged",       ACT_RAGED,       NO_USE },
   {"shunter",     ACT_SHUNTER,     0      },
   {NULL, 0}
};

LOOKUP_TYPE tab_player_act[] = {
 {"nada",         ACT_NONE,         0},
 {"bought_pet",   ACT_BOUGHT_PET,   0},
 {"no_pray",      ACT_NO_PRAY,      0},
 {"holylight",    ACT_HOLYLIGHT,    0},
 {"wizinvis",     ACT_WIZINVIS,     0},
 {"builder",      ACT_BUILDER,      0},
 {"silence",      ACT_SILENCE,      0},
 {"no_emote",     ACT_NO_EMOTE,     0},
 {"no_tell",      ACT_NO_TELL,      0},
 {"log",          ACT_LOG,          0},
 {"deny",         ACT_DENY,         0},
 {"freeze",       ACT_FREEZE,       0},
 {"thief",        ACT_THIEF,        0},
 {"killer",       ACT_KILLER,       0},
 {"no_summon",    ACT_NO_SUMMON,    0},
 {"no_visit",     ACT_NO_VISIT,     0},
 /* 46, empty, ACT_INCOG -- unused   */
 {"undead",       ACT_UNDEAD,       0},
 {"vampire",      ACT_VAMPIRE,      0},
 {"werewolf",     ACT_WEREWOLF,     0},
 {"shifted",      ACT_SHIFTED,      0},
 {"raged",        ACT_RAGED,        0},
 {"afk",          ACT_AFK,          0},
 {"pkok",         ACT_PKOK,         0},
 {"ambassador",   ACT_AMBASSADOR,   0},
 {"cdiplomat",    ACT_CDIPLOMAT,    0},
 {"cboss",        ACT_CBOSS,        0},
 {"ctreasurer",   ACT_CTREASURER,   0},
 {"carmorer",     ACT_CARMORER,     0},
 {"cleader",      ACT_CLEADER,      0},
 {"council",      ACT_COUNCIL,      0},
 {"ruler",        ACT_RULER,        0},
 {"blind_player", ACT_BLIND_PLAYER, 0},
 {"tester",       ACT_TESTER,       0},
 {"autoexit",     ACT_AUTOEXIT,     0},
 {"autoloot",     ACT_AUTOLOOT,     0},
 {"autosac",      ACT_AUTOSAC,      0},
 {"blank",        ACT_BLANK,        0},
 {"brief",        ACT_BRIEF,        0},
 {"combine",      ACT_COMBINE,      0},
 {"prompt",       ACT_PROMPT,       0},
 {"telnet_ga",    ACT_TELNET_GA,    0},
 {"color",        ACT_COLOR,        0},
 {"autosplit",    ACT_AUTOSPLIT,    0},
 {"full_ansi",    ACT_FULL_ANSI,    0},
 {"mapper",       ACT_MAPPER,       0},
 {"justify",      ACT_JUSTIFY,      0},
 {"autodig",      ACT_AUTODIG,      0},
 {"autobrief",    ACT_AUTOBRIEF,    0},
 {"ghost",        ACT_GHOST,        0},
 {"cooldown",     ACT_NO_COOLDOWN,  0},
 {"shunter",      ACT_SHUNTER,      0},
 {"whitelist",    ACT_WHITELIST,    0},
 {NULL, 0}
};

/* New bits to handle how mobs act */

LOOKUP_TYPE tab_mob_skill[] = {
   {"nada", 1, 0},
   {"enhanced_reflexes", 2, 100},
   {"sleight_of_hand", 4, 200},
   {"crushing_blow", 8, 400},
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
   {"enhanced_dam", 16384, 400},
   {"dualwield", 32768, 350},
   {"dirt", 65536, 300},
   {"combat_prowess", 131072, 500},
   {"quickstrike", 262144, 600},
   {"charge", 524288, 700},
   {NULL, 0, 0}
};

LOOKUP_TYPE tab_mob_cast[] = {
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

LOOKUP_TYPE tab_cast_name[] = {
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

LOOKUP_TYPE tab_mob_def[] = {
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

LOOKUP_TYPE tab_affected_by[] =
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


LOOKUP_TYPE tab_item_types[] = {
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
   {"charm", 33, 1000},
   {"anvil", 34, 0},
   {"cooking_fire", 35, 0},
   {"alchemy_lab", 36, 0},
   {"forge", 37, 0},
   {"loom", 38, 0},
   {NULL, 0}
};

LOOKUP_TYPE tab_obj_flags[] = {
   {"nada",            ITEM_EXTRA_NONE,         0     },
   {"glow",            ITEM_EXTRA_GLOW,         10    },
   {"hum",             ITEM_EXTRA_HUM,          10    },
   {"nodisarm",        ITEM_EXTRA_NO_DISARM,    20    },
   {"lock",            ITEM_EXTRA_LOCK,         20    },
   {"evil",            ITEM_EXTRA_EVIL,         50    },
   {"invis",           ITEM_EXTRA_INVIS,        200   },
   {"magic",           ITEM_EXTRA_MAGIC,        100   },
   {"nodrop",          ITEM_EXTRA_NO_DROP,      20    },
   {"bless",           ITEM_EXTRA_BLESS,        200   },
   {"anti_good",       ITEM_EXTRA_ANTI_GOOD,    20    },
   {"anti_evil",       ITEM_EXTRA_ANTI_EVIL,    20    },
   {"anti_neutral",    ITEM_EXTRA_ANTI_NEUTRAL, 20    },
   {"noremove",        ITEM_EXTRA_NO_REMOVE,    100   },
   {"inventory",       ITEM_EXTRA_INVENTORY,    0     },
   {"nosave",          ITEM_EXTRA_NO_SAVE,      1000  },
   {"claneq",          ITEM_EXTRA_CLAN_EQ,      NO_USE},
   {"trigger:destroy", ITEM_EXTRA_TRIG_DESTROY, 10    },
   {"no_auction",      ITEM_EXTRA_NO_AUCTION,   0     },
   {"remort",          ITEM_EXTRA_REMORT,       9     },
   {"adept",           ITEM_EXTRA_ADEPT,        12    },
   {"rare",            ITEM_EXTRA_RARE,         1000  },
   {"vamp",            ITEM_EXTRA_VAMP,         9     },
   {"noloot",          ITEM_EXTRA_NO_LOOT,      20    },
   {"nosac",           ITEM_EXTRA_NO_SAC,       100   },
   {"unique",          ITEM_EXTRA_UNIQUE,       100   },
   {"lifestealer",     ITEM_EXTRA_LIFESTEALER,  100   },
   {"silver",          ITEM_EXTRA_SILVER,       100   },
   {"noquest",         ITEM_EXTRA_NO_QUEST,     0     },
   {"notake",          ITEM_EXTRA_NO_TAKE,      0     },
   {NULL, 0}
};

LOOKUP_TYPE tab_wear_flags[] = {
   {"nada",      ITEM_WEAR_NONE,      0  },
   {"halo",      ITEM_WEAR_HALO,      50 },
   {"aura",      ITEM_WEAR_AURA,      50 },
   {"horns",     ITEM_WEAR_HORNS,     20 },
   {"head",      ITEM_WEAR_HEAD,      40 },
   {"face",      ITEM_WEAR_FACE,      50 },
   {"beak",      ITEM_WEAR_BEAK,      50 },
   {"ear",       ITEM_WEAR_EAR,       40 },
   {"neck",      ITEM_WEAR_NECK,      20 },
   {"wings",     ITEM_WEAR_WINGS,     30 },
   {"shoulders", ITEM_WEAR_SHOULDERS, 40 },
   {"arms",      ITEM_WEAR_ARMS,      100},
   {"wrist",     ITEM_WEAR_WRIST,     20 },
   {"hands",     ITEM_WEAR_HANDS,     50 },
   {"finger",    ITEM_WEAR_FINGER,    40 },
   {"claws",     ITEM_WEAR_CLAWS,     200},
   {"hold",      ITEM_WEAR_HOLD_HAND, 100},
   {"about",     ITEM_WEAR_ABOUT,     30 },
   {"waist",     ITEM_WEAR_WAIST,     20 },
   {"body",      ITEM_WEAR_BODY,      100},
   {"tail",      ITEM_WEAR_TAIL,      900},
   {"legs",      ITEM_WEAR_LEGS,      100},
   {"feet",      ITEM_WEAR_FEET,      900},
   {"hooves",    ITEM_WEAR_HOOVES,    100},
   {"take",      ITEM_TAKE,           900},
   {NULL, 0}
};

/* This not used at present.  Thought i'd bung details in though :) */
LOOKUP_TYPE tab_item_apply[] = {

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



LOOKUP_TYPE tab_wear_loc[] = {
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


LOOKUP_TYPE tab_obj_aff[] = {
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
   /* 15 is empty */
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


LOOKUP_TYPE tab_room_flags[] = {
   {"nada",         RFLAG_NONE,         0     },
   {"dark",         RFLAG_DARK,         1000  },
   {"regen",        RFLAG_REGEN,        4000  },  /* increased regen in this room */
   {"no_mob",       RFLAG_NO_MOB,       3000  },
   {"indoors",      RFLAG_INDOORS,      500   },
   {"no_magic",     RFLAG_NO_MAGIC,     5000  },
   {"hot",          RFLAG_HOT,          1000  },   /* players lose hp each tick   */
   {"cold",         RFLAG_COLD,         1000  },  /* players lose hp each tick   */
   {"pk",           RFLAG_PK,           200   }, /* players may freely pk here  */
   {"quiet",        RFLAG_QUIET,        200   }, /* room is quiet               */
   {"private",      RFLAG_PRIVATE,      1000  },
   {"safe",         RFLAG_SAFE,         4000  },
   {"solitary",     RFLAG_SOLITARY,     1000  },
   {"pet_shop",     RFLAG_PET_SHOP,     NO_USE},
   {"no_recall",    RFLAG_NO_RECALL,    1000  },
   {"no_teleport",  RFLAG_NO_TELEPORT,  500   },
   {"hunt_hunt",    RFLAG_HUNT_MARK,    NO_USE},
   {"no_bloodwalk", RFLAG_NO_BLOODWALK, 4000  },
   {"no_portal",    RFLAG_NO_PORTAL,    1000  },
   {"no_repop",     RFLAG_NO_REPOP,     10    },
   {NULL, 0}
};

LOOKUP_TYPE tab_sector_types[] = {
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

LOOKUP_TYPE tab_door_types[] = {
   {"nada",       EX_NONE,       0     },
   {"door",       EX_ISDOOR,     50    },
   {"closed",     EX_CLOSED,     0     },
   {"locked",     EX_LOCKED,     0     },
   {"climb",      EX_CLIMB,      0     },
   {"immortal",   EX_IMMORTAL,   NO_USE},
   {"pickproof",  EX_PICKPROOF,  500   },
   {"smashproof", EX_SMASHPROOF, 500   },
   {"passproof",  EX_PASSPROOF,  500   },
   {"nodetect",   EX_NODETECT,   900   },
   {"nomob",      EX_NOMOB,      0     },
   {NULL, 0}
};

LOOKUP_TYPE tab_door_states[] = {
   {"open", 0, 0},
   {"closed", 1, 0},
   {"locked", 2, 0},
   {NULL, 0}
};

LOOKUP_TYPE tab_magic_realms[] = {

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

LOOKUP_TYPE tab_magic_realms_col[] = {

   {"nada", BIT_0, 0},
   {"@@eFire@@N", BIT_1, 0},
   {"@@lShock@@N", BIT_2, 0},
   {"@@WLight@@N", BIT_3, 0},
   {"@@cGas@@N", BIT_4, 0},
   {"@@GPoison@@N", BIT_5, 0},
   {"@@aCold@@N", BIT_6, 0},
   {"@@pSound@@N", BIT_7, 0},
   {"@@rAcid@@N", BIT_8, 0},
   {"@@RDrain@@N", BIT_9, 0},
   {"@@dImpact@@N", BIT_10, 0},
   {"@@mPsionic@@N", BIT_11, 0},
   {"@@yHoly@@N", BIT_12, 0},
   {NULL, 0}
};

LOOKUP_TYPE tab_mob_race_mods_col[] = {

   {"nada", BIT_0, 0},
   {"@@lFast Healing@@N", BIT_1, 0},
   {"@@BSlow Healing@@N", BIT_2, 0},
   {"@@eStrong Magic@@N", BIT_3, 0},
   {"@@RWeak Magic@@N", BIT_4, 0},
   {"@@mNo Magic@@N", BIT_5, 0},
   {"@@GPoison Immune@@N", BIT_6, 0},
   {"@@cSpell Resist@@N", BIT_7, 0},
   {"@@bWoodland@@N", BIT_8, 0},
   {"@@dDarkness@@N", BIT_9, 0},
   {"@@pHuge@@N", BIT_10, 0},
   {"@@pLarge@@N", BIT_11, 0},
   {"@@pTiny@@N", BIT_12, 0},
   {"@@pSmall@@N", BIT_13, 0},
   {"@@aTail@@N", BIT_14, 0},
   {"@@yTough Skin@@N", BIT_15, 0},
   {"@@yStone Skin@@N", BIT_16, 0},
   {"@@yIron Skin@@N", BIT_17, 0},
   {"@@eFast Casting@@N", BIT_18, 0},
   {NULL, 0}

};

LOOKUP_TYPE tab_mob_race_mods[] = {

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
   {"fast_cast", BIT_18, 0},
   {NULL, 0}

};

EQ_TYPE tab_auto_obj[] = {
 /*              ac    dr    hp    hr    mp    mv   svs */
 {"halo",         0, 0.12,    0, 0.12, 0.20,    0, 0.20},
 {"aura",         0,    0,    0,    0,    0,    0, 0.40},
 {"horns",     0.15, 0.11, 0.15,    0,    0,    0,    0},
 {"head",      0.15,    0, 0.15,    0,    0,    0,    0},
 {"face",      0.15,    0, 0.15,    0,    0,    0,    0},
 {"beak",      0.15, 0.11, 0.15,    0,    0,    0,    0},
 {"ear",          0,    0,    0,    0, 0.30, 0.30, 0.14},
 {"neck",         0,    0,    0,    0, 0.15, 0.15, 0.20},
 {"wings",        0,    0,    0, 0.18,    0, 0.15,    0},
 {"shoulders", 0.40, 0.15,    0, 0.17,    0,    0,    0},
 {"arms",      0.17, 0.15,    0, 0.17,    0,    0,    0},
 {"wrist",     0.12, 0.13, 0.15, 0.15,    0,    0,    0},
 {"hands",     0.15, 0.20,    0, 0.17,    0,    0,    0},
 {"finger",       0,    0,    0,    0, 0.35, 0.35, 0.16},
 {"claws",     0.15, 0.21, 0.15, 0.15,    0,    0,    0},
 {"weapon",       0, 0.67,    0, 0.50,    0,    0,    0},
 {"shield",    0.35,    0, 0.35,    0,    0,    0, 0.40},
 {"cape",      0.11,    0,    0,    0,    0, 0.12,    0},
 {"waist",     0.11,    0,    0,    0,    0,    0,    0},
 {"body",      0.42,    0, 0.50, 0.23, 0.15,    0,    0},
 {"tail",         0, 0.12,    0, 0.12,    0,    0,    0},
 {"legs",      0.17, 0.15, 0.15, 0.17,    0,    0,    0},
 {"feet",      0.15, 0.20, 0.15, 0.17,    0,    0,    0},
 {"hooves",    0.15, 0.21, 0.15, 0.17,    0,    0,    0},
 {NULL, 0, 0, 0, 0, 0, 0, 0}

};

/* Now for the functions */

unsigned long int table_lookup( LOOKUP_TYPE *table, char *name )
{
   int a;

   if( name[0] == '\0' )
      return /* table[0].value-1 */ 0;

   for( a = 0; table[a].text != NULL; a++ )
      if( !str_prefix( name, table[a].text ) )
         return ( ( !str_cmp( table[a].text, "nada" ) ) ? 0 : table[a].value );
   return /* table[0].value-1  */ 0;
}

char *rev_table_lookup( LOOKUP_TYPE *table, unsigned long int number )
{
   int a;

   for( a = 0; table[a].text != NULL; a++ )
      if( table[a].value == number )
         return table[a].text;
   return "";
}

/* spec: fixed to not assume contiguous bit use */

char *bit_table_lookup( LOOKUP_TYPE *table, unsigned long int number )
{
   int a;
   static char buf[MAX_STRING_LENGTH];

   buf[0] = '\0';

   for( a = 0; number && table[a].text; a++ )
   {
      if( ( number & table[a].value ) == table[a].value &&
          str_cmp( table[a].text, "nada" ) && str_cmp( table[a].text, "placeholder" ) )
      {
         strncat( buf, table[a].text, MSL-1 );
         strncat( buf, ", ", MSL-1 );
         number &= ~table[a].value;
      }
   }

   if( buf[0] == '\0' )
      strncat( buf, "None.", MSL-1 );
   else
   {
      a = strlen( buf );
      buf[a - 2] = '.';
      buf[a - 1] = '\0';
   }
   return buf;
}

void table_printout( LOOKUP_TYPE *table, char *buf )
{
   int a;

   a = 0;
   buf[0] = '\0';

   for( a = 0; table[a].text != NULL; a++ )
   {
      if( ( strcmp( table[a].text, "nada" ) ) && ( strcmp( table[a].text, "placeholder" ) ) )   /* If not an invalid choice */
      {
         strncat( buf, "          ", MSL );
         strncat( buf, table[a].text, MSL );
         strncat( buf, "\r\n", MSL );
      }
   }

   return;
}

void wide_table_printout( LOOKUP_TYPE *table, char *buf )
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
         strncat( buf, "     ", MSL-1 );
         snprintf( tmp, MSL, "%12s", table[a].text );
         strncat( buf, tmp, MSL-1 );
         if( ++foo % 4 == 0 )
            strncat( buf, "\r\n", MSL-1 );
      }
   }
   strncat( buf, "\r\n", MSL-1 );
   return;
}

char *show_values( LOOKUP_TYPE *table, unsigned long int value, bool fBit )
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
         strncat( buf, "     ", MSL-1 );
         snprintf( tmp, MSL, "%s%-13s",
                  fBit ? ( IS_SET( value, table[a].value ) ? "@@y*" : "@@g " ) :
                  ( value == table[a].value ? "@@y*" : "@@g " ), table[a].text );
         strncat( buf, tmp, MSL-1 );
         if( ++foo % 4 == 0 )
            strncat( buf, "\r\n", MSL-1 );
      }
   }
   strncat( buf, "@@g\r\n", MSL-1 );
   return ( buf );
}

char *bs_show_values( LOOKUP_TYPE *table, bitset<MAX_BITSET> &bit )
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
         strncat( buf, "     ", MSL );
         snprintf( tmp, MSL, "%s%-13s",
                  bit.test(table[a].value) ? "@@y*" : "@@g ",table[a].text);
         strncat( buf, tmp, MSL-1 );
         if( ++foo % 4 == 0 )
            strncat( buf, "\r\n", MSL );
      }
   }
   strncat( buf, "@@g\r\n", MSL );
   return ( buf );
}
