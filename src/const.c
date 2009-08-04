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

#ifndef DEC_MAGIC_H
#include "h/magic.h"
#endif

#ifndef DEC_VAMPIRE_H
#include "h/vampire.h"
#endif

#ifndef DEC_WEREWOLF_H
#include "h/werewolf.h"
#endif

const char *where_name[] = {
   "-*used as light*-     ",  /* light  */
   "-*floating above*-    ",
   "-*enveloped by*-      ",
   "-*worn on horns*-     ",
   "-*worn on head*-      ",
   "-*worn on face*-      ",
   "-*worn on beak*-      ",
   "-*worn on ear*-       ",
   "-*worn on ear*-       ",
   "-*worn on neck*-      ",
   "-*worn on neck*-      ",  /* 10 */
   "-*worn on wings*-     ",
   "-*worn on shoulders*- ",
   "-*worn on arms*-      ",
   "-*worn around wrist*- ",
   "-*worn around wrist*- ",  /* 15 */
   "-*worn on hands*-     ",
   "-*worn on finger*-    ",
   "-*worn on finger*-    ",
   "-*worn on claws*-     ",
   "-*held in left hand*- ",  /* 20 */
   "-*held in right hand*-",
   "-*worn as a cape*-    ",
   "-*worn about waist*-  ",
   "-*worn on body*-      ",
   "-*worn on tail*-      ",
   "-*worn on legs*-      ",
   "-*worn on feet*-      ",
   "-*worn on hooves*-    ",
};

/*
 * color table for say, gossip, shout, etc.
 * Put here for ease of editing. -S-
 */
const struct color_type color_table[MAX_COLOR] = {
   {"say",       0},
   {"tell",      1},
   {"gossip",    2},
   {"auction",   3},
   {"music",     4},
   {"shout",     5},
   {"yell",      6},
   {"clan",      7},
   {"race",      8},
   {"flame",     9},
   {"info",     10},
   {"stats",    11},
   {"rooms",    12},
   {"objects",  13},
   {"mobiles",  14},
   {"crusade",  15},
   {"adept",    16},
   {"remort",   17},
   {"diplomat", 18},
   {"quest",    19},
   {"ooc",      20},
   {"game",     21}
};

const struct ansi_type ansi_table[MAX_ANSI] = {
   {"gray",         "\033[0;37m",           0, 'g', 7},
   {"red",          "\033[0;31m",           1, 'R', 7},
   {"green",        "\033[0;32m",           2, 'G', 7},
   {"brown",        "\033[0;33m",           3, 'b', 7},
   {"blue",         "\033[0;34m",           4, 'B', 7},
   {"magenta",      "\033[0;35m",           5, 'm', 7},
   {"cyan",         "\033[0;36m",           6, 'c', 7},
   {"black",        "\033[0;30m",           7, 'k', 7}, /* was 0;33 */
   {"yellow",       "\033[1;33m",           8, 'y', 7},
   {"white",        "\033[1;37m",           9, 'W', 7},
   {"normal",       "\033[0;0m\033[0;40m", 10, 'N', 13},
   {"purple",       "\033[1;35m",          11, 'p', 7},
   {"dark_grey",    "\033[1;30m",          12, 'd', 7},
   {"light_blue",   "\033[1;34m",          13, 'l', 7},
   {"light_green",  "\033[1;32m",          14, 'r', 7},
   {"light_cyan",   "\033[1;36m",          15, 'a', 7},
   {"light_red",    "\033[1;31m",          16, 'e', 7},
   {"bold",         "\033[1m",             17, 'x', 4},
   {"flashing",     "\033[5m",             18, 'f', 4},
   {"inverse",      "\033[7m",             19, 'i', 4},
   {"back_red",     "\033[0;41m",          20, '2', 7},
   {"back_green",   "\033[0;42m",          21, '3', 7},
   {"back_yellow",  "\033[0;43m",          22, '4', 7},
   {"back_blue",    "\033[0;44m",          23, '1', 7},
   {"back_magenta", "\033[0;45m",          24, '5', 7},
   {"back_cyan",    "\033[0;46m",          25, '6', 7},
   {"back_black",   "\033[0;40m",          26, '0', 7},
   {"back_white",   "\033[1;47m",          27, '7', 7},

};



/*
 * Class table.
 */
const struct class_type class_table[MAX_CLASS] = {
   {
    "Mag", "Mage", APPLY_INT, "Int",
    1107, 90, 18, 10, 1, 2, TRUE,
    "faerie fire",
    },

   {
    "Cle", "Cleric", APPLY_WIS, "Wis",
    1105, 90, 18, 12, 2, 2, TRUE,
    "bless"},

   {
    "Thi", "Thief", APPLY_DEX, "Dex",
    1106, 90, 18, 8, 1, 3, FALSE,
    "steal"},

   {
    "War", "Warrior", APPLY_STR, "Str",
    1108, 90, 18, 6, 3, 4, FALSE,
    "punch"},

   {
    "Psi", "Psionicist", APPLY_CON, "Con",
    1125, 90, 18, 10, 2, 3, TRUE,
    "mind flail"}

};




/* Table for remort classes.... same format as class_table 
 * Note that alot of stuff is not needed... 
 */
const struct class_type remort_table[MAX_CLASS] = {
   {
    "Sor", "Sorcerer", APPLY_INT, "Int",
    /*
     * guild room -> 
     */ 0, 0, 0, 0, 2, 3, TRUE,
    ""},

   {
    "Mon", "Monk", APPLY_WIS, "Wis",
    /*
     * guild_room ->
     */ 0, 0, 0, 0, 2, 3, TRUE,
    ""},

   {
    "Ass", "Assassin", APPLY_DEX, "Dex",
    /*
     * guild room -> 
     */ 0, 0, 0, 0, 2, 3, FALSE,
    ""},

   {
    "Kni", "Knight", APPLY_STR, "Str",
    /*
     * guild room -> 
     */ 0, 0, 0, 0, 4, 4, FALSE,
    ""},

   {
    "Nec", "Necromancer", APPLY_CON, "Con",
    /*
     * guild room -> 
     */ 0, 0, 0, 0, 2, 3, TRUE,
    ""},
};



const struct race_type race_table[MAX_RACE] = {
   {
    "Hmn", "Human", ROOM_VNUM_TEMPLE, 0,
    19, 19, 19, 19, 19,
    RACE_MOD_NONE,
    /*
     * racial wear slots  
     */
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE},
    5, {4, 5, 3, 1, 2},
    "War Psi Thi Mag Cle",
    "",
    REALM_NONE,   /* strong magic realms for race */
    REALM_NONE,   /* weak */
    REALM_NONE,   /* resist */
    REALM_NONE,   /* suscept */
    TRUE, /* player race */
    TRUE /* carries money */
   },

   {
    "Hlf", "Halfling", ROOM_VNUM_TEMPLE, 0,
    14, 17, 21, 23, 20,
    RACE_MOD_SMALL | RACE_MOD_WOODLAND | RACE_MOD_RESIST_SPELL,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE},
    5, {3, 1, 2, 4, 5},
    "Thi Mag Cle War Psi",
    "steal \'pick lock\'",
    REALM_NONE,
    REALM_NONE,
    REALM_MIND | REALM_ACID | REALM_FIRE | REALM_COLD | REALM_HOLY,
    REALM_NONE,
    TRUE,
    TRUE
   },

   {
    "Dwf", "Dwarf", ROOM_VNUM_TEMPLE, 0,
    23, 13, 23, 13, 23,
    RACE_MOD_RESIST_SPELL | RACE_MOD_SMALL | RACE_MOD_IMMUNE_POISON,

    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE},
    5, {4, 2, 5, 3, 1},
    "War Cle Psi Thi Mag",
    "smash",
    REALM_NONE,
    REALM_ACID,
    REALM_ACID | REALM_FIRE | REALM_HOLY | REALM_POISON,
    REALM_NONE,
    TRUE,
    TRUE
   },

   {
    "Elf", "Elf", 9201, 0,
    15, 20, 19, 23, 18,
    RACE_MOD_SMALL | RACE_MOD_WOODLAND | RACE_MOD_STRONG_MAGIC | RACE_MOD_FAST_CAST,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE},
    5, {1, 3, 4, 5, 2},
    "Mag Thi War Psi Cle",
    "\'find doors\'",
    REALM_MIND | REALM_COLD,
    REALM_DRAIN | REALM_ACID,
    REALM_MIND,
    REALM_DRAIN,
    TRUE,
    TRUE
   },

   {
    "Gno", "Gnome", 6015, 0,
    14, 22, 20, 22, 17,
    RACE_MOD_TINY | RACE_MOD_IMMUNE_POISON | RACE_MOD_DARKNESS | RACE_MOD_FAST_CAST,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE},
    5, {2, 5, 1, 3, 4},
    "Cle Psi Mag Thi War",
    "\'faerie fire\' sneak",
    REALM_POISON | REALM_MIND | REALM_FIRE,
    REALM_IMPACT | REALM_ACID,
    REALM_MIND,
    REALM_ACID,
    TRUE,
    TRUE
   },

   {
    "Ogr", "Ogre", 3430, 0,
    23, 13, 13, 23, 23,
    RACE_MOD_WEAK_MAGIC | RACE_MOD_RESIST_SPELL | RACE_MOD_LARGE | RACE_MOD_TOUGH_SKIN,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE},
    5, {4, 1, 3, 2, 5},
    "War Mag Thi Cle Psi",
    "\'shield block\'",
    REALM_POISON | REALM_ACID,
    REALM_MIND | REALM_SOUND,
    REALM_NONE,
    REALM_MIND,
    TRUE,
    TRUE
   },

   {
    "Drw", "Drow", 9201, 0,
    18, 23, 16, 23, 15,
    RACE_MOD_STRONG_MAGIC | RACE_MOD_SLOW_HEAL | RACE_MOD_DARKNESS,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE},
    5, {5, 1, 4, 3, 2},
    "Psi Mag War Thi Cle",
    "\'find doors\' \'mind bolt\'",
    REALM_DRAIN | REALM_SHOCK | REALM_MIND | REALM_FIRE,
    REALM_HOLY | REALM_LIGHT,
    REALM_DRAIN,
    REALM_HOLY,
    TRUE,
    TRUE
   },

   {
    "Lam", "Lamia", ROOM_VNUM_TEMPLE, 0,
    22, 17, 16, 23, 17,
    RACE_MOD_LARGE | RACE_MOD_IMMUNE_POISON | RACE_MOD_TAIL,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE},
    5, {3, 4, 5, 1, 2},
    "Thi War Psi Mag Cle",
    "dodge",
    REALM_SOUND | REALM_ACID,
    REALM_MIND | REALM_FIRE,
    REALM_NONE,
    REALM_MIND | REALM_FIRE | REALM_HOLY,
    TRUE,
    TRUE
   },

   {
    "Drc", "Dracon", ROOM_VNUM_TEMPLE, 0,
    23, 23, 15, 11, 23,
    RACE_MOD_STRONG_MAGIC | RACE_MOD_TOUGH_SKIN | RACE_MOD_SLOW_HEAL | RACE_MOD_LARGE | RACE_MOD_TAIL |
    RACE_MOD_RESIST_SPELL,
    {TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE,
     TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE},
    5, {1, 2, 4, 5, 3},
    "Mag Cle War Psi Thi",
    "fly \'fire breath\'",
    REALM_FIRE | REALM_GAS | REALM_SHOCK | REALM_POISON | REALM_ACID,
    REALM_COLD | REALM_MIND,
    REALM_FIRE | REALM_SHOCK | REALM_ACID,
    REALM_MIND | REALM_SHOCK,
    TRUE,
    TRUE
   },

   {
    "Cen", "Centaur", ROOM_VNUM_TEMPLE, 0,
    21, 17, 21, 14, 22,
    RACE_MOD_LARGE | RACE_MOD_TOUGH_SKIN | RACE_MOD_WOODLAND,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE},
    5, {4, 2, 3, 1, 5},
    "War Cle Thi Mag Psi",
    "hunt",
    REALM_SOUND | REALM_FIRE,
    REALM_FIRE | REALM_ACID,
    REALM_MIND | REALM_SHOCK | REALM_GAS,
    REALM_FIRE,
    TRUE,
    TRUE
   },

   {
    "Ttn", "Titan", ROOM_VNUM_TEMPLE, 0,
    22, 19, 13, 19, 22,
    RACE_MOD_NO_MAGIC | RACE_MOD_HUGE | RACE_MOD_RESIST_SPELL,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE},
    5, {4, 5, 1, 2, 3},
    "War Psi Mag Cle Thi",
    "\'enhanced damage\'",
    REALM_MIND,
    REALM_DRAIN | REALM_GAS,
    REALM_MIND,
    REALM_NONE,
    TRUE,
    TRUE
   },

   {
    "Sde", "Sidhe", ROOM_VNUM_TEMPLE, 0,
    16, 22, 18, 23, 16,
    RACE_MOD_STRONG_MAGIC | RACE_MOD_TINY | RACE_MOD_RESIST_SPELL | RACE_MOD_FAST_CAST,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE},
    5, {5, 3, 1, 2, 4},
    "Psi Thi Mag Cle War",
    "fly",
    REALM_MIND | REALM_SHOCK | REALM_FIRE | REALM_DRAIN,
    REALM_FIRE | REALM_GAS | REALM_DRAIN,
    REALM_MIND | REALM_SHOCK,
    REALM_IMPACT | REALM_FIRE,
    TRUE,
    TRUE
   },

   {
    "Min", "Minotaur", ROOM_VNUM_TEMPLE, 0,
    23, 16, 15, 18, 23,
    RACE_MOD_LARGE | RACE_MOD_TOUGH_SKIN | RACE_MOD_DARKNESS,
    {TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE},
    5, {4, 1, 2, 5, 3},
    "War Mag Cle Psi Thi",
    "\'enhanced damage\'",
    REALM_POISON | REALM_SOUND,
    REALM_SHOCK | REALM_COLD,
    REALM_NONE,
    REALM_COLD,
    TRUE,
    TRUE
   },

   {
    "Trl", "Troll", ROOM_VNUM_TEMPLE, 0,
    22, 15, 15, 20, 23,
    RACE_MOD_FAST_HEAL | RACE_MOD_LARGE | RACE_MOD_DARKNESS | RACE_MOD_STONE_SKIN,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE},
    5, {4, 2, 3, 5, 1},
    "War Cle Thi Psi Mag",
    "\'stone skin\'",
    REALM_NONE,
    REALM_LIGHT | REALM_FIRE | REALM_HOLY,
    REALM_IMPACT,
    REALM_LIGHT | REALM_FIRE,
    TRUE,
    TRUE
   },

   {
    "Und", "Undead", ROOM_VNUM_TEMPLE, 0,
    23, 15, 15, 19, 23,
    RACE_MOD_DARKNESS | RACE_MOD_IMMUNE_POISON,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE},
    5, {4, 2, 3, 5, 1},
    "War Cle Thi Psi Mag",
    "\'enhanced damage\'",
    REALM_DRAIN | REALM_POISON,
    REALM_LIGHT | REALM_HOLY,
    REALM_DRAIN | REALM_POISON,
    REALM_LIGHT | REALM_HOLY,
    TRUE,
    FALSE
   },

   {
    "Gar", "Gargoyle", ROOM_VNUM_TEMPLE, 0,
    22, 22, 16, 16, 19,
    RACE_MOD_IRON_SKIN | RACE_MOD_SLOW_HEAL | RACE_MOD_LARGE,
    {TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, TRUE, TRUE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE},
    5, {1, 4, 5, 2, 3},
    "Mag War Psi Cle Thi",
    "fly \'stone skin\'",
    REALM_FIRE | REALM_GAS | REALM_SHOCK | REALM_POISON,
    REALM_COLD | REALM_MIND | REALM_ACID,
    REALM_FIRE | REALM_SHOCK | REALM_MIND,
    REALM_COLD | REALM_ACID,
    TRUE,
    FALSE
   },

   {
    "Dmn", "Demon", ROOM_VNUM_TEMPLE, 0,
    20, 17, 16, 20, 22,
    RACE_MOD_FAST_HEAL | RACE_MOD_LARGE | RACE_MOD_TAIL | RACE_MOD_TOUGH_SKIN,
    {TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE},
    5, {3, 4, 1, 5, 2},
    "Thi War Mag Psi Cle",
    "fireball",
    REALM_FIRE | REALM_POISON | REALM_ACID,
    REALM_COLD | REALM_HOLY,
    REALM_FIRE,
    REALM_COLD | REALM_HOLY | REALM_LIGHT,
    TRUE,
    TRUE
   },

   {
    "Ele", "Elemental", ROOM_VNUM_TEMPLE, 0,
    12, 23, 23, 23, 14,
    RACE_MOD_SLOW_HEAL | RACE_MOD_STRONG_MAGIC | RACE_MOD_RESIST_SPELL | RACE_MOD_HUGE | RACE_MOD_FAST_CAST,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE},
    5, {1, 5, 2, 3, 4},
    "Mag Psi Cle Thi War",
    "fly \'burning hands\' \'chill touch\'",
    REALM_FIRE | REALM_COLD | REALM_SHOCK | REALM_IMPACT,
    REALM_NONE,
    REALM_MIND | REALM_DRAIN,
    REALM_NONE,
    TRUE,
    FALSE
   },

   {
    "Avn", "Avian", ROOM_VNUM_TEMPLE, 0,
    22, 10, 23, 23, 17,
    RACE_MOD_NO_MAGIC | RACE_MOD_IMMUNE_POISON | RACE_MOD_WOODLAND,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
     TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE},
    5, {3, 4, 2, 1, 5},
    "Thi War Cle Mag Psi",
    "fly circle",
    REALM_NONE,
    REALM_HOLY | REALM_MIND,
    REALM_IMPACT | REALM_GAS | REALM_POISON,
    REALM_FIRE | REALM_SHOCK,
    TRUE,
    TRUE
   },

   {
    "Bst", "Beast", ROOM_VNUM_TEMPLE, 0,
    23, 10, 16, 23, 23,
    RACE_MOD_WOODLAND | RACE_MOD_TAIL,
    {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
     FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
     FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
    5, {4, 3, 5, 1, 2},
    "War Thi Psi Mag Cle",
    "",
    REALM_NONE,
    REALM_NONE,
    REALM_NONE,
    REALM_NONE,
    FALSE,
    FALSE
   }

};

/*
 * name, ac (int), dr (int), hr (int), speed (float)
 * negative speed is good, positive is bad
 */
const struct stance_app_type stance_app[MAX_STANCE] = {
   {"Adventurer", 0, 0, 0, 0},
   {"Mage", 20, -3, -3, 0.15},
   {"Ninja", -30, 0, 5, -0.12},
   {"Shadows", -20, -1, -2, -0.15},
   {"Essence", 0, 0, 3, 0},
   {"Beast", 0, 3, 0, 0},
   {"Flame", 10, 3, 3, -0.03},
   {"Spirit", -10, 3, -2, -0.03},
   {"Void", -10, -2, 3, -0.06},
   {"Dragon", -10, 2, 2, 0.03},
   {"Snake", 7, -3, -2, -0.15},
   {"Wizard", 30, -5, -5, 0.12},
   {"Magi", 40, -10, -10, 0.09}
};




const struct clan_type clan_table[MAX_CLAN] = {
   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },

   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },

   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },

   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },

   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },

   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },

   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },
   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },
   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },

};

#define CLASS_MAGE       0
#define CLASS_CLERIC     1
#define CLASS_THIEF      2
#define CLASS_WARRIOR    3
#define CLASS_PSI        4
#define CLASS_CONJURER   5
#define CLASS_TEMPLAR    6 /* Yes, i _know_ it's now paladin :P */
#define CLASS_RANGER     7


/*
 * Attribute bonus tables.
 */
const struct str_app_type str_app[26] = {
   {-5, -4, 0, 0},   /* 0  */
   {-5, -4, 3, 1},   /* 1  */
   {-3, -2, 3, 2},
   {-3, -1, 10, 3},  /* 3  */
   {-2, -1, 25, 4},
   {-2, -1, 55, 5},  /* 5  */
   {-1, 0, 80, 6},
   {-1, 0, 90, 7},
   {0, 0, 100, 8},
   {0, 0, 100, 9},
   {0, 0, 115, 10},  /* 10  */
   {0, 0, 115, 11},
   {0, 0, 140, 12},
   {0, 0, 140, 13},  /* 13  */
   {0, 1, 170, 14},
   {1, 1, 170, 15},  /* 15  */
   {1, 2, 195, 16},
   {2, 3, 220, 22},
   {2, 4, 250, 25},  /* 18  */
   {3, 5, 300, 30},
   {3, 6, 350, 35},  /* 20  */
   {4, 7, 400, 40},
   {5, 7, 450, 45},
   {6, 8, 500, 50},
   {8, 10, 550, 55},
   {10, 12, 600, 60} /* 25   */
};



const struct int_app_type int_app[26] = {
   {3, -50, 0},   /*  0 */
   {6, -50, 0},   /*  1 */
   {6, -50, 0},
   {7, -50, 0},   /*  3 */
   {7, -50, 0},
   {8, -50, 0},   /*  5 */
   {8, -40, 2},
   {9, -40, 2},
   {9, -40, 2},
   {10, -35, 2},
   {10, -35, 3},  /* 10 */
   {11, -30, 4},
   {12, -30, 5},
   {13, -20, 6},
   {13, -20, 7},
   {15, -15, 8},  /* 15 */
   {20, -5, 9},
   {23, 0, 10},
   {25, 10, 10},  /* 18 */
   {28, 10, 11},
   {34, 15, 11},  /* 20 */
   {39, 20, 12},
   {45, 25, 13},
   {52, 30, 15},
   {60, 35, 17},
   {70, 50, 20}   /* 25 */
};



const struct wis_app_type wis_app[26] = {
   {0, 50}, /*  0 */
   {0, 50}, /*  1 */
   {0, 50},
   {0, 50}, /*  3 */
   {0, 50},
   {1, 50}, /*  5 */
   {1, 50},
   {1, 45},
   {1, 45},
   {1, 40},
   {1, 35}, /* 10 */
   {2, 30},
   {2, 25},
   {2, 15},
   {2, 5},
   {3, 0},  /* 15 */
   {3, -5},
   {4, -15},
   {4, -20},   /* 18 */
   {5, -25},
   {5, -30},   /* 20 */
   {6, -35},
   {7, -45},
   {7, -55},
   {7, -70},
   {8, -80} /* 25 */
};



const struct dex_app_type dex_app[26] = {
   {60}, /* 0 */
   {50}, /* 1 */
   {50},
   {40},
   {30},
   {20}, /* 5 */
   {10},
   {0},
   {0},
   {0},
   {0},  /* 10 */
   {0},
   {0},
   {0},
   {0},
   {-10},   /* 15 */
   {-15},
   {-20},
   {-30},
   {-40},
   {-50},   /* 20 */
   {-65},
   {-75},
   {-90},
   {-105},
   {-120}   /* 25 */
};



const struct con_app_type con_app[26] = {
   {-4, 20},   /*  0 */
   {-3, 25},   /*  1 */
   {-2, 30},
   {-2, 35},   /*  3 */
   {-1, 40},
   {-1, 45},   /*  5 */
   {-1, 50},
   {0, 55},
   {0, 60},
   {0, 65},
   {0, 70}, /* 10 */
   {0, 75},
   {0, 80},
   {0, 85},
   {0, 88},
   {1, 90}, /* 15 */
   {2, 95},
   {2, 97},
   {3, 99}, /* 18 */
   {3, 99},
   {4, 99}, /* 20 */
   {4, 99},
   {5, 99},
   {6, 99},
   {7, 99},
   {8, 99}  /* 25 */
};



/*
 * Liquid properties.
 * Used in world.obj.
 */
const struct liq_type liq_table[LIQ_MAX] = {
   {"water", "clear", {0, 1, 10}},  /*  0 */
   {"beer", "amber", {3, 2, 5}},
   {"wine", "rose", {5, 2, 5}},
   {"ale", "brown", {2, 2, 5}},
   {"dark ale", "dark", {1, 2, 5}},

   {"whisky", "golden", {6, 1, 4}}, /*  5 */
   {"lemonade", "pink", {0, 1, 8}},
   {"firebreather", "boiling", {10, 0, 0}},
   {"local specialty", "everclear", {3, 3, 3}},
   {"slime mold juice", "green", {0, 4, -8}},

   {"milk", "white", {0, 3, 6}}, /* 10 */
   {"tea", "tan", {0, 1, 6}},
   {"coffee", "black", {0, 1, 6}},
   {"blood", "red", {0, 2, -1}},
   {"salt water", "clear", {0, 1, -2}},

   {"@@bchocolate milk@@N", "creamy", {0, 3, 6}},  /* 15 */
   {"mountain dew", "bubbly", {0, 1, 5}}
};

/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n) n

const struct skill_type skill_table[MAX_SKILL] = {

/*
 * Magic spells.
 */

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "reserved", {99, 99, 99, 99, 99},
    0, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 0 ), 0, 0,
    "", "",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "acid blast", {50, 82, 82, 82, 82},
    spell_acid_blast, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 70 ), 20, 2.75,
    "acid blast", "!Acid Blast!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "armor", {3, 82, 82, 82, 82},
    spell_armor, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 1 ), 5, 1.00,
    "", "You feel less protected.",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "animate", {82, 82, 82, 82, 70},
    spell_animate, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 525 ), 50, 5.50,
    "", "!Animate!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "bad breath", {4, 82, 82, 82, 82},
    spell_badbreath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 502 ), 10, 2.75,
    "bad breath", "!Bad Breath!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "bark skin", {82, 20, 82, 82, 82},
    spell_bark_skin, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 514 ), 20, 1.00,
    "", "Your skin feels softer.",
    "$n's skin looks less wooden."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "bless", {82, 8, 82, 82, 82},
    spell_bless, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 3 ), 5, 1.00,
    "", "You feel less righteous.",
    "$n looks less Holy."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "blindness", {16, 82, 82, 82, 82},
    spell_blindness, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_blindness, SLOT( 4 ), 5, 3.50,
    "", "You can see again.",
    "$n's vision returns."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "bloody tears", {82, 82, 82, 82, 26},
    spell_bloody_tears, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 518 ), 12, 3.50,
    "Bloody Tears", "!Bloody Tears!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "burning hands", {6, 82, 82, 82, 82},
    spell_burning_hands, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 5 ), 15, 1.50,
    "burning hands", "!Burning Hands!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "call lightning", {82, 36, 82, 82, 82},
    spell_call_lightning, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 6 ), 15, 4.00,
    "lightning bolt", "!Call Lightning!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "calm", {82, 82, 82, 82, 52},
    spell_calm, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 531 ), 30, 4.50,
    "", "!Calm!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "cause critical", {82, 58, 82, 82, 82},
    spell_cause_critical, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 63 ), 20, 3.00,
    "spell", "!Cause Critical!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "cause light", {82, 4, 82, 82, 82},
    spell_cause_light, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 62 ), 15, 1.00,
    "spell", "!Cause Light!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "cause serious", {82, 40, 82, 82, 82},
    spell_cause_serious, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 64 ), 17, 2.00,
    "spell", "!Cause Serious!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "change sex", {66, 82, 82, 82, 82},
    spell_change_sex, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 82 ), 15, 1.50,
    "", "Your body feels familiar again.",
    "$n looks $mself again."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "charm person", {49, 60, 82, 82, 82},
    spell_charm_person, TAR_CHAR_OFFENSIVE, POS_STANDING,
    &gsn_charm_person, SLOT( 7 ), 5, 2.75,
    "", "You feel more self-confident.",
    "The glazed look in $n's eyes fades."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "chill touch", {8, 82, 82, 82, 82},
    spell_chill_touch, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 8 ), 15, 1.25,
    "chilling touch", "You feel less cold.",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "color spray", {7, 82, 82, 82, 82},
    spell_color_spray, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 10 ), 15, 1.30,
    "color spray", "!color Spray!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "continual light", {11, 27, 82, 82, 82},
    spell_continual_light, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 57 ), 7, 0.50,
    "", "!Continual Light!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "control weather", {58, 79, 82, 82, 82},
    spell_control_weather, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 11 ), 25, 2.80,
    "", "!Control Weather!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "create food", {82, 26, 82, 82, 82},
    spell_create_food, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 12 ), 5, 0.25,
    "", "!Create Food!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "create spring", {82, 65, 82, 82, 82},
    spell_create_spring, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 80 ), 20, 0.25,
    "", "!Create Spring!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "create water", {82, 9, 82, 82, 82},
    spell_create_water, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 13 ), 5, 0.25,
    "", "!Create Water!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "cure blindness", {82, 28, 82, 82, 82},
    spell_cure_blindness, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 14 ), 5, 1.75,
    "", "!Cure Blindness!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "cure critical", {82, 59, 82, 82, 82},
    spell_cure_critical, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 15 ), 20, 3.00,
    "", "!Cure Critical!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "cure light", {82, 5, 82, 82, 82},
    spell_cure_light, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 16 ), 10, 1.00,
    "", "!Cure Light!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "cure poison", {82, 32, 82, 82, 82},
    spell_cure_poison, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 43 ), 5, 2.75,
    "", "!Cure Poison!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "cure serious", {82, 41, 82, 82, 82},
    spell_cure_serious, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 61 ), 15, 2.00,
    "", "!Cure Serious!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "curse", {35, 26, 82, 82, 82},
    spell_curse, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_curse, SLOT( 17 ), 20, 1.35,
    "curse", "The curse wears off.",
    "$n starts to look more Holy."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "detect evil", {12, 8, 82, 82, 82},
    spell_detect_evil, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 18 ), 5, 1.00,
    "", "The red in your vision disappears.",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "detect hidden", {17, 16, 82, 82, 82},
    spell_detect_hidden, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 44 ), 5, 1.00,
    "", "You feel less aware of your suroundings.",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "detect invis", {19, 82, 82, 82, 82},
    spell_detect_invis, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 19 ), 5, 1.00,
    "", "You no longer see invisible objects.",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "detect magic", {7, 7, 82, 82, 82},
    spell_detect_magic, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 20 ), 5, 1.00,
    "", "The detect magic wears off.",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "detect poison", {82, 10, 82, 82, 82},
    spell_detect_poison, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 21 ), 5, 1.00,
    "", "!Detect Poison!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "detect undead", {8, 82, 82, 82, 82},
    spell_detect_undead, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 513 ), 8, 1.00,
    "", "You no longer sense undead beings.",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "detection", {82, 82, 82, 82, 22},
    spell_detection, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 527 ), 12, 1.00,
    "", "!Detection!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "dimension blade", {86, 86, 86, 86, 86},
    spell_dimension_blade, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 523 ), 100, 6.75,
    "", "!Dimension Blade!",
    ""},

/*    Doesn't work well with intelligent mobs. Therefore get rid of it... :P */

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "dispel evil", {82, 82, 82, 82, 82},  /* was 82, 63, ... */
    spell_dispel_evil, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 22 ), 15, 2.75,
    "dispel evil", "!Dispel Evil!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "dispel magic", {68, 85, 85, 85, 85},
    spell_dispel_magic, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 59 ), 15, 2.25,
    "", "!Dispel Magic!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "earthquake", {82, 78, 82, 82, 82},
    spell_earthquake, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 23 ), 15, 3.00,
    "earthquake", "!Earthquake!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "ego whip", {82, 82, 82, 82, 25},
    spell_ego_whip, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 535 ), 35, 5.50,
    "ego whip", "!Ego Whip!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "enchant weapon", {39, 82, 82, 82, 82},
    spell_enchant_weapon, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 24 ), 100, 5.50,
    "", "!Enchant Weapon!",
    ""},

   {
    MORTAL, VAMP, COOLDOWN_NONE,
    "energy drain", {15, 7, 8, 11, 9},
    spell_energy_drain, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 25 ), 12, 2.75,
    "energy drain", "!Energy Drain!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "enhance weapon", {86, 86, 86, 86, 86},
    spell_enhance_weapon, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 517 ), 50, 5.50,
    "", "!Enhance Weapon!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "faerie fire", {3, 82, 82, 82, 82},
    spell_faerie_fire, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 72 ), 5, 1.45,
    "faerie fire", "The pink aura around you fades away.",
    "The pink aura around $n fades away."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "faerie fog", {82, 14, 82, 82, 82},
    spell_faerie_fog, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 73 ), 12, 1.30,
    "faerie fog", "!Faerie Fog!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "fighting trance", {82, 82, 82, 82, 60},
    spell_fighting_trance, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 521 ), 60, 1.00,
    "", "Your fighting trance fades.",
    "$n's fighting trance fades."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "fireball", {28, 82, 82, 82, 82},
    spell_fireball, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 26 ), 15, 3.10,
    "fireball", "!Fireball!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "fire blade", {82, 82, 82, 82, 50},
    spell_fire_blade, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 528 ), 50, 5.50,
    "", "!FireBlade!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "flamestrike", {82, 54, 82, 82, 82},
    spell_flamestrike, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 65 ), 20, 3.45,
    "flamestrike", "!Flamestrike!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "flare", {82, 20, 82, 82, 82},
    spell_flare, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 591 ), 18, 2.75,
    "flare", "Your vision returns.",
    "$n blinks, and starts to see again."},


   {
    MORTAL, NORM, COOLDOWN_NONE,
    "fly", {24, 82, 82, 82, 82},
    spell_fly, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 56 ), 10, 1.00,
    "", "You slowly float to the ground.",
    "$n slowly floats to the ground."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "gate", {82, 82, 82, 82, 82},
    spell_gate, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 83 ), 50, 2.75,
    "", "!Gate!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "giant strength", {18, 82, 82, 82, 82},
    spell_giant_strength, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 39 ), 20, 1.00,
    "", "You feel weaker.",
    "$n looks weaker."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "harm", {82, 74, 82, 82, 82},
    spell_harm, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 27 ), 35, 4.00,
    "harm spell", "!Harm!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "heal", {82, 75, 82, 82, 82},
    spell_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 28 ), 50, 4.00,
    "", "!Heal!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "hypnosis", {82, 82, 82, 82, 14},
    spell_hypnosis, TAR_CHAR_OFFENSIVE, POS_STANDING,
    NULL, SLOT( 532 ), 30, 5.50,
    "", "!Hypnosis!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "identify", {9, 82, 82, 82, 82},
    spell_identify, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 53 ), 12, 1.00,
    "", "!Identify!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "infravision", {20, 82, 82, 82, 82},
    spell_infravision, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 77 ), 5, 1.00,
    "", "You no longer see in the dark.",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "influx", {82, 65, 82, 82, 82},
    spell_influx, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 503 ), 75, 2.70,
    "", "!Influx!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "invis", {17, 82, 82, 82, 82},
    spell_invis, TAR_IGNORE, POS_STANDING,
    &gsn_invis, SLOT( 29 ), 5, 1.00,
    "", "You are no longer invisible.",
    "$n's invisibilty fades."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "know alignment", {13, 22, 82, 82, 82},
    spell_know_alignment, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 58 ), 9, 1.00,
    "", "!Know Alignment!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "know weakness", {82, 82, 82, 82, 18},
    spell_know_weakness, TAR_CHAR_SELF, POS_FIGHTING,
    NULL, SLOT( 530 ), 15, 1.00,
    "", "You are less aware of your enemy's weaknesses.",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "know critical", {82, 82, 82, 82, 28},
    spell_know_weakness, TAR_CHAR_SELF, POS_FIGHTING,
    NULL, SLOT( 530 ), 15, 1.00,
    "", "You are less aware of critical damage points.",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "know item", {82, 82, 82, 82, 9},
    spell_know_item, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 533 ), 20, 1.00,
    "", "!Know Item!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "laser bolt", {82, 82, 82, 82, 82},
    spell_laserbolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 511 ), 35, 2.75,
    "laserbolt", "!laserbolt!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "lightning bolt", {24, 82, 82, 82, 82},
    spell_lightning_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 30 ), 15, 2.75,
    "lightning bolt", "!Lightning Bolt!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "locate object", {16, 30, 82, 82, 82},
    spell_locate_object, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 31 ), 20, 2.00,
    "", "!Locate Object!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "magic missile", {3, 82, 82, 82, 82},
    spell_magic_missile, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 32 ), 15, 1.15,
    "magic missile", "!Magic Missile!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "mass invis", {60, 82, 82, 82, 82},
    spell_mass_invis, TAR_IGNORE, POS_STANDING,
    &gsn_mass_invis, SLOT( 69 ), 20, 5.50,
    "", "!Mass Invis!",
    ""},


   {
    MORTAL, NORM, COOLDOWN_NONE,
    "mind flail", {82, 82, 82, 82, 4},
    spell_mind_flail, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 536 ), 12, 2.25,
    "mind flail", "!Mind Flail!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "mystic armor", {10, 82, 82, 82, 82},
    spell_mystic_armor, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 590 ), 18, 1.00,
    "" "Your Mystic Armor fades away",
    "The Mystic Armor around someone in the room fades away."},


   {
    MORTAL, NORM, COOLDOWN_NONE,
    "nerve fire", {82, 82, 82, 82, 60},
    spell_nerve_fire, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 520 ), 50, 5.50,
    "nerve fire", "!Nerve Fire!",
    ""},


   {
    MORTAL, NORM, COOLDOWN_NONE,
    "night vision", {82, 82, 82, 82, 19},
    spell_night_vision, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 538 ), 17, 1.00,
    "", "Your eyes feel weaker.",
    ""},


   {
    MORTAL, NORM, COOLDOWN_NONE,
    "pass door", {46, 82, 82, 82, 82},
    spell_pass_door, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 74 ), 20, 1.00,
    "", "You feel solid again.",
    "$n's body becomes more solid."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "phase", {82, 82, 82, 82, 45},
    spell_phase, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 522 ), 20, 1.00,
    "", "You feel solid again.",
    "$n's body becomes more solid."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "psychic crush", {82, 82, 82, 82, 19},
    spell_physic_thrust, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 537 ), 33, 2.15,
    "psychic crush", "!Psychic Crush!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "psychic thrust", {82, 82, 82, 82, 7},
    spell_physic_thrust, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 537 ), 22, 1.10,
    "psychic thrust", "!Physic Thrust!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "poison", {82, 31, 82, 82, 82},
    spell_poison, TAR_CHAR_OFFENSIVE, POS_STANDING,
    &gsn_poison, SLOT( 33 ), 10, 2.75,
    "poison", "You feel less sick.",
    "$n looks less sick."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "produce food", {82, 82, 82, 82, 24},
    spell_produce_food, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 524 ), 16, 0.50,
    "", "!Produce Food!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "protection", {9, 12, 82, 82, 82},
    spell_protection, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 34 ), 5, 1.00,
    "", "You feel less protected.",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "refresh", {82, 11, 82, 82, 82},
    spell_refresh, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 81 ), 12, 1.30,
    "refresh", "!Refresh!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "remove curse", {36, 27, 82, 82, 82},
    spell_remove_curse, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 35 ), 5, 2.85,
    "", "!Remove Curse!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "sanctuary", {82, 70, 82, 82, 82},
    spell_sanctuary, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 36 ), 75, 1.00,
    "", "The white aura around your body fades.",
    "The white aura around $n's body fades."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "seal room", {70, 50, 82, 82, 50},
    spell_seal_room, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 541 ), 75, 3.75,
    "", "The Energy web around this room fades.",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "see magic", {82, 82, 82, 82, 7},
    spell_see_magic, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 526 ), 8, 1.00,
    "", "You no longer see magical auras.",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "sense evil", {82, 82, 82, 82, 10},
    spell_sense_evil, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 515 ), 12, 1.00,
    "", "You no longer sense evil.",
    ""},

   {
    MORTAL, VAMP, COOLDOWN_NONE,
    "shade", {12, 7, 6, 8, 7},
    spell_shade, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 542 ), 5, 3.75,
    "", "The room gets lighter.",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "shield", {9, 82, 82, 82, 82},
    spell_shield, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 67 ), 12, 1.00,
    "", "Your force shield shimmers, then fades away.",
    "$n's force field shimmers, then fades away."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "shocking grasp", {5, 82, 82, 82, 82},
    spell_shocking_grasp, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 82 ), 15, 0.75,
    "shocking grasp", "!Shocking Grasp!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "sight", {30, 82, 82, 82, 82},
    spell_cure_blindness, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 592 ), 20, 3.30,
    "", "!Sight!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "sleep", {9, 82, 82, 82, 82},
    spell_sleep, TAR_CHAR_OFFENSIVE, POS_STANDING,
    &gsn_sleep, SLOT( 38 ), 15, 2.75,
    "", "You feel less tired.",
    "$n starts looking much more awake."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "stalker", {82, 82, 82, 82, 82},
    spell_stalker, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 401 ), 100, 5.50,
    "", "!Stalker!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "stone skin", {36, 82, 82, 82, 82},
    spell_stone_skin, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 66 ), 12, 1.00,
    "", "Your skin feels soft again.",
    "$n's skin loses it's stone-like look."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "suffocate", {82, 82, 82, 82, 46},
    spell_suffocate, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 516 ), 30, 2.40,
    "a choking hand", "!Suffocate!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "summon", {35, 82, 82, 82, 82},
    spell_summon, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 40 ), 50, 2.75,
    "", "!Summon!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "teleport", {40, 82, 82, 82, 82},
    spell_teleport, TAR_CHAR_SELF, POS_FIGHTING,
    NULL, SLOT( 2 ), 35, 2.75,
    "", "!Teleport!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "travel", {82, 82, 82, 82, 82},
    spell_travel, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 599 ), 50, 5.50,
    "", "!Travel!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "vampire bite", {86, 86, 86, 86, 86},
    spell_null, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 0 ), 5, 2.75,
    "", "Your blood feels cooler.",
    ""},


   {
    MORTAL, NORM, COOLDOWN_NONE,
    "ventriloquate", {3, 82, 82, 82, 82},
    spell_ventriloquate, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 41 ), 5, 0.25,
    "", "!Ventriloquate!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_DEF,
    "warcry", {82, 82, 82, 5, 82},
    spell_null, TAR_CHAR_SELF, POS_STANDING,
    &gsn_warcry, SLOT( 501 ), 5, 1.00,
    "", "You feel your courage subside.",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "weaken", {82, 20, 82, 82, 82},
    spell_weaken, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 68 ), 20, 2.20,
    "spell", "You feel stronger.",
    "$n looks stronger."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "beacon", {70, 82, 82, 82, 70},
    spell_beacon, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 399 ), 25, 0.50,
    "", "!Beacon!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "portal", {80, 82, 82, 82, 82},
    spell_portal, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 334 ), 100, 6.00,
    "", "!Portal!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "window", {70, 82, 82, 82, 70},
    spell_window, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 333 ), 100, 4.00,
    "", "!Window!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "word of recall", {82, 66, 82, 82, 82},
    spell_word_of_recall, TAR_CHAR_SELF, POS_RESTING,
    NULL, SLOT( 42 ), 5, 2.75,
    "", "!Word of Recall!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "hellspawn", {43, 82, 82, 82, 82},
    spell_hellspawn, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 512 ), 50, 2.75,
    "HellSpawn", "!Hellspawn!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "planergy", {82, 82, 82, 82, 5},
    spell_planergy, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 504 ), 25, 1.10,
    "energy", "!planergy!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "visit", {82, 82, 82, 82, 32},
    spell_visit, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 505 ), 50, 2.75,
    "", "!visit!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "barrier", {82, 82, 82, 82, 8},
    spell_barrier, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 506 ), 30, 1.00,
    "", "Your barrier slowly fades.",
    "The barrier around $n fades."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "phobia", {82, 82, 82, 82, 10},
    spell_phobia, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 507 ), 32, 5.50,
    "phobia attack", "!phobia!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "mind bolt", {82, 82, 82, 82, 12},
    spell_mind_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 519 ), 40, 2.75,
    "Mind Bolt", "!MindBolt!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "mindflame", {82, 82, 82, 82, 35},
    spell_mindflame, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 508 ), 40, 5.50,
    "Mind Flame", "!mindflame!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "chain lightning", {65, 82, 82, 82, 82},
    spell_chain_lightning, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 509 ), 25, 5.50,
    "lightening bolt", "!chain-light!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "static", {82, 82, 82, 82, 20},
    spell_static, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 510 ), 40, 5.50,
    "discharge", "!static!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "cloak:absorption", {60, 43, 82, 82, 82},
    spell_cloak_absorb, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 221 ), 500, 1.00,
    "", "@@NThe @@lcloak@@N around your body fades.",
    "@@NThe @@lcloak@@N around $n's body fades."},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "cloak:reflection", {48, 82, 82, 82, 82},
    spell_cloak_reflect, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 222 ), 500, 1.00,
    "", "@@NThe @@lc@@el@@ro@@ya@@ak@@N around your body fades.",
    "@@NThe @@lc@@el@@ro@@ya@@ak@@N around $n's body fades."},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "cloak:flaming", {70, 82, 78, 82, 60},
    spell_cloak_flaming, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 223 ), 750, 1.00,
    "", "@@NThe @@ecloak@@N around your body fades.",
    "@@NThe @@ecloak@@N around $n's body fades."},

   {
    MORTAL, VAMP, COOLDOWN_NONE,
    "cloak:darkness", {18, 13, 14, 14, 16},
    spell_cloak_darkness, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 224 ), 45, 1.00,
    "", "The @@dcloak@@N around your body fades.",
    "@@NThe @@dcloak@@N around $n's body fades."},

   {
    ADEPT, NORM, COOLDOWN_NONE,
    "cloak:adept", {1, 82, 82, 82, 82},
    spell_cloak_adept, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 226 ), 500, 1.00,
    "", "@@NThe @@Wcloak@@N around your body fades.",
    "@@NThe @@Wcloak@@N around $n's body fades."},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "cloak:regeneration", {82, 73, 82, 52, 82},
    spell_cloak_regen, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 227 ), 500, 1.00,
    "", "@@NThe @@rcloak@@N around your body fades.",
    "@@NThe @@rcloak@@N around $n's body fades."},

/*
 * Dragon breath                                
 */
   {
    MORTAL, NORM, COOLDOWN_NONE,
    "acid breath", {82, 82, 82, 82, 82},
    spell_acid_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 200 ), 0, 1.25,
    "blast of acid", "!Acid Breath!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "fire breath", {82, 82, 82, 82, 82},
    spell_fire_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 201 ), 0, 1.25,
    "blast of flame", "!Fire Breath!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "frost breath", {82, 82, 82, 82, 82},
    spell_frost_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 202 ), 0, 1.25,
    "blast of frost", "!Frost Breath!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "gas breath", {82, 82, 82, 82, 82},
    spell_gas_breath, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 203 ), 0, 1.25,
    "blast of gas", "!Gas Breath!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "lightning breath", {82, 82, 82, 82, 82},
    spell_lightning_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 204 ), 0, 1.25,
    "blast of lightning", "!Lightning Breath!",
    ""},

   {
    MORTAL, VAMP, COOLDOWN_NONE,
    "blood leach", {8, 4, 3, 4, 4},
    spell_blood_leach, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 540 ), 6, 2.75,
    "", "Your body feels stronger again.",
    ""},
/*
 * Fighter and thief skills.
 */

   {
    MORTAL, NORM, COOLDOWN_DEF,
    "appraise", {82, 82, 12, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_appraise, SLOT( 0 ), 0, 1.00,
    "", "!Appraise!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_OFF,
    "bash", {82, 82, 82, 28, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_bash, SLOT( 0 ), 0, 2.75,
    "bash", "!bash!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_DEF,
    "berserk", {82, 82, 82, 34, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_berserk, SLOT( 0 ), 0, 2.75,
    "", "You calm down!",
    "$n looks calmer!"},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "climb", {82, 82, 25, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_climb, SLOT( 0 ), 0, 2.75,
    "", "!Climb!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_OFF,
    "dirt", {82, 82, 82, 35, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_dirt, SLOT( 0 ), 0, 2.75,
    "", "!dirt!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "nodisarm", {82, 82, 40, 30, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_nodisarm, SLOT( 0 ), 0, 0,
    "", "!nodisarm!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "notrip", {82, 82, 30, 40, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_notrip, SLOT( 0 ), 0, 0,
    "", "!notrip!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_DEF,
    "smash", {82, 82, 82, 30, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_smash, SLOT( 0 ), 0, 0,
    "", "!smash!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_OFF,
    "trip", {82, 82, 18, 25, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_trip, SLOT( 0 ), 0, 2.75,
    "", "!trip!",
    ""},


   {
    MORTAL, NORM, COOLDOWN_OFF,
    "circle", {82, 82, 19, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_circle, SLOT( 0 ), 0, 2.75,
    "backstab", "!circle!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_OFF,
    "backstab", {82, 82, 10, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_backstab, SLOT( 0 ), 0, 2.75,
    "backstab", "!Backstab!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_OFF,
    "disarm", {82, 82, 82, 12, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_disarm, SLOT( 0 ), 0, 2.75,
    "", "!Disarm!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "dodge", {82, 82, 7, 16, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_dodge, SLOT( 0 ), 0, 0,
    "", "!Dodge!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "dualwield", {82, 7, 8, 5, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_dualwield, SLOT( 0 ), 0, 0,
    "", "!DualWield!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "enhanced damage", {82, 82, 82, 42, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_enhanced_damage, SLOT( 0 ), 0, 0,
    "", "!Enhanced Damage!",
    ""},

   {
    MORTAL, VAMP, COOLDOWN_OFF,
    "feed", {1, 1, 1, 1, 1},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_feed, SLOT( 0 ), 0, 5.50,
    "", "!feed!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "find doors", {82, 82, 16, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_find_doors, SLOT( 0 ), 0, 0,
    "", "!Find Doors!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "crushing blow", {82, 82, 82, 50, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_crushing_blow, SLOT( 0 ), 0, 0,
    "", "!Crushing Blow!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_OFF,
    "headbutt", {82, 82, 82, 6, 82},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_headbutt, SLOT( 0 ), 0, 2.75,
    "headbutt", "!HeadButt!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_DEF,
    "hide", {82, 82, 12, 82, 82},
    spell_null, TAR_IGNORE, POS_RESTING,
    &gsn_hide, SLOT( 0 ), 0, 2.75,
    "", "!Hide!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_DEF,
    "hunt", {82, 82, 70, 82, 82},
    spell_null, TAR_IGNORE, POS_RESTING,
    &gsn_hunt, SLOT( 0 ), 0, 2.75,
    "", "!Hunt!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_OFF,
    "kick", {82, 82, 82, 14, 82},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_kick, SLOT( 0 ), 0, 2.75,
    "kick", "!Kick!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_OFF,
    "knee", {82, 82, 82, 8, 82},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_knee, SLOT( 0 ), 0, 2.75,
    "knee", "!Knee",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "martial arts", {82, 82, 30, 20, 30},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_martial_arts, SLOT( 0 ), 0, 0,
    "", "!Martial Arts!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "parry", {82, 82, 82, 10, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_parry, SLOT( 0 ), 0, 0,
    "", "!Parry!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "peek", {82, 82, 20, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_peek, SLOT( 0 ), 0, 0,
    "", "!Peek!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_DEF,
    "pick lock", {82, 82, 25, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_pick_lock, SLOT( 0 ), 0, 2.75,
    "", "!Pick!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_OFF,
    "punch", {82, 82, 82, 5, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_punch, SLOT( 0 ), 0, 2.75,
    "", "!Punch!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_DEF,
    "rescue", {82, 82, 82, 15, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_rescue, SLOT( 0 ), 0, 2.75,
    "", "!Rescue!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "enhanced reflexes", {82, 82, 10, 15, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_enhanced_reflexes, SLOT( 0 ), 0, 0,
    "", "!Enhanced Reflexes!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "shadowform", {82, 82, 82, 82, 9},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_shadow, SLOT( 0 ), 0, 0,
    "", "",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "shield block", {82, 82, 82, 40, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_shield_block, SLOT( 0 ), 0, 0,
    "", "!Shield Block!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_DEF,
    "sneak", {82, 82, 5, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_sneak, SLOT( 0 ), 0, 2.75,
    "", "",
    ""},

   {
    MORTAL, NORM, COOLDOWN_OFF,
    "stake", {85, 85, 85, 85, 85},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_stake, SLOT( 0 ), 0, 5.50,
    "", "",
    ""},

   {
    MORTAL, NORM, COOLDOWN_OFF,
    "steal", {82, 82, 1, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_steal, SLOT( 0 ), 0, 5.50,
    "", "!Steal!",
    ""},

   {
    REMORT, NORM, COOLDOWN_OFF,
    "stun", {82, 25, 82, 18, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_stun, SLOT( 0 ), 0, 5.50,
    "", "",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "sleight of hand", {82, 82, 60, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_sleight_of_hand, SLOT( 0 ), 0, 0,
    "", "!Sleight of Hand!",
    ""},

   {
    REMORT, NORM, COOLDOWN_DEF,
    "disguise", {82, 82, 25, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_disguise, SLOT( 0 ), 0, 0,
    "", "!Disguise!",
    ""},

   {
    MORTAL, VAMP, COOLDOWN_NONE,
    "instruct", {10, 5, 5, 5, 5},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_instruct, SLOT( 0 ), 0, 0,
    "", "!INSTRUCT!",
    ""},

   {
    REMORT, NORM, COOLDOWN_OFF,
    "frenzy", {82, 18, 82, 23, 82},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_frenzy, SLOT( 0 ), 0, 5.50,
    "frenzy", "!FRENZY!",
    ""},

   {
    MORTAL, WOLF, COOLDOWN_NONE,
    "imprint", {20, 10, 10, 10, 10},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_imprint, SLOT( 0 ), 0, 0,
    "", "!INSTRUCT!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "emotion control", {82, 82, 82, 82, 75},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_emotion_control, SLOT( 0 ), 0, 0,
    "emotion control", "!EMOTION CONTROL!",
    ""},

/*
 *  Spells for mega1.are from Glop/Erkenbrand.
*/
   {
    MORTAL, NORM, COOLDOWN_NONE,
    "general purpose", {82, 82, 82, 82, 82},
    spell_general_purpose, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 205 ), 0, 2.35,
    "general purpose ammo", "!General Purpose Ammo!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "high explosive", {82, 82, 82, 82, 82},
    spell_high_explosive, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 206 ), 0, 2.50,
    "high explosive ammo", "!High Explosive Ammo!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "deflect weapon", {82, 10, 82, 82, 82},
    spell_deflect_weapon, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 600 ), 25, 1.00,
    "", "Your mind shield melts away.",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "black hand", {82, 82, 82, 82, 7},
    spell_black_hand, TAR_CHAR_OFFENSIVE, POS_STANDING,
    NULL, SLOT( 601 ), 50, 2.75,
    "", "The hand dissolves from around your throat into nothingness.",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "throwing needle", {82, 82, 5, 82, 82},
    spell_throw_needle, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 602 ), 40, 2.75,
    "Throwing Needle", "!Throwing Needle!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "morale", {82, 82, 82, 10, 82},
    spell_morale, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 603 ), 75, 1.00,
    "Morale", "!Morale!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "leadership", {82, 82, 82, 13, 82},
    spell_leadership, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 604 ), 75, 1.00,
    "Leadership", "!Leadership!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "ice bolt", {3, 82, 82, 82, 82},
    spell_ice_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 605 ), 20, 1.50,
    "Ice Bolt", "!Ice Bolt!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "water elemental", {6, 82, 82, 82, 82},
    spell_waterelem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 606 ), 80, 1.75,
    "", "!Stalker!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "skeleton", {82, 82, 82, 82, 9},
    spell_skeleton, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 607 ), 80, 1.75,
    "", "!Stalker!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "poison weapon", {82, 82, 18, 82, 25},
    spell_poison_weapon, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 608 ), 100, 2.75,
    "", "!Enchant Weapon!",
    ""},

   {
    MORTAL, VAMP, COOLDOWN_NONE,
    "embrace", {86, 15, 15, 15, 15},
    spell_embrace, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 609 ), 15, 5.50,
    "", "!EMBRACE!",
    ""},

   {
    MORTAL, VAMP, COOLDOWN_NONE,
    "mesmerise", {5, 2, 2, 2, 2},
    spell_mesmerise, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 610 ), 4, 1.25,
    "", "!MESMERISE!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "ethereal travel", {30, 24, 82, 82, 40},
    spell_ethereal, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 611 ), 250, 5.50,
    "", "ETHEREAL TRAVEL!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "adrenaline", {82, 28, 82, 38, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_adrenaline, SLOT( 0 ), 0, 0,
    "", "!ADRENALINE!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "throwing star", {82, 82, 43, 82, 82},
    spell_throw_needle, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 628 ), 85, 2.75,
    "Throwing Star", "!Throwing Star!",
    ""},

/* THIS IS A SPELL AFFECT HOLDER FOR THE SKILL ADRENALINE..DO NOT CAST! */

   {
    REMORT, NORM, COOLDOWN_NONE,
    "adrenaline bonus", {82, 82, 82, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 612 ), 0, 0,
    "", "!ADRENALINE BONUS!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "fire elemental", {11, 82, 82, 82, 82},
    spell_fireelem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 612 ), 120, 2.00,
    "", "!FIRE ELEMENTAL!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "rune:fire", {11, 82, 82, 82, 15},
    spell_rune_fire, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 613 ), 150, 1.50,
    "", "The rune dissipates.",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "rune:shock", {13, 15, 82, 82, 82},
    spell_rune_shock, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 614 ), 150, 1.50,
    "", "The rune dissipates.",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "rune:poison", {82, 82, 15, 82, 12},
    spell_rune_poison, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 615 ), 80, 1.50,
    "", "The rune dissipates.",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "healing light", {82, 33, 82, 25, 82},
    spell_healing_light, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 616 ), 150, 3.25,
    "", "@@NThe @@mHealing Light@@N dissipates.",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "withering shadow", {82, 82, 35, 82, 17},
    spell_wither_shadow, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 617 ), 150, 3.25,
    "", "@@NThe @@dWithering Shadow@@N dissipates.",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "mana flare", {18, 22, 82, 82, 82},
    spell_mana_flare, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 618 ), 80, 3.25,
    "", "@@NThe @@eMana Flare@@N dissipates.",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "mana drain", {11, 82, 82, 82, 15},
    spell_mana_drain, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 619 ), 150, 3.25,
    "", "@@NThe @@dMana Drain@@N dissipates.",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "cage", {18, 13, 31, 40, 19},
    spell_cage, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 620 ), 150, 3.75,
    "", "@@NThe @@rCage@@N dissipates.",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "room dispel", {22, 82, 82, 33, 82},
    spell_room_dispel, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 625 ), 120, 4.00,
    "", "",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "soul net", {82, 82, 82, 82, 58},
    spell_soul_net, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 629 ), 350, 3.75,
    "", "@@NThe @@dSoul Net@@N dissipates.",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "condense soul", {82, 82, 82, 82, 79},
    spell_condense_soul, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 630 ), 800, 5.50,
    "", "!Soul Potion!",
    ""},

   {
    MORTAL, VAMP, COOLDOWN_NONE,
    "blood walk", {20, 19, 20, 18, 19},
    spell_blood_walk, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 631 ), 40, 5.50,
    "", "!blood walk!!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "restoration", {82, 82, 82, 82, 82},
    spell_restoration, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 632 ), 50, 2.75,
    "", "!restoration!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "infuse", {82, 82, 82, 82, 71},
    spell_infuse, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 633 ), 1000, 5.50,
    "", "!Infuse SOul!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "combat prowess", {82, 37, 82, 29, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_combat_prowess, SLOT( 0 ), 0, 0,
    "", "!Combat Prowess!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "quickstrike", {82, 58, 49, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_quickstrike, SLOT( 0 ), 0, 0,
    "", "!Quickstrike!",
    ""},

   {
    MORTAL, WOLF, COOLDOWN_NONE,
    "scent", {5, 1, 1, 1, 1},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_scent, SLOT( 0 ), 0, 1.00,
    "", "",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "holy light", {82, 82, 82, 43, 82},
    spell_holy_light, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 634 ), 75, 3.00,
    "Holy Light", "!Holy Light!",
    ""},

   {
    REMORT, NORM, COOLDOWN_OFF,
    "target", {82, 82, 82, 20, 82},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_target, SLOT( 0 ), 0, 2.75,
    "target", "!target",
    ""},

   {
    REMORT, NORM, COOLDOWN_OFF,
    "charge", {82, 82, 82, 65, 82},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_charge, SLOT( 0 ), 0, 2.75,
    "Charge", "!Charge",
    ""},

   {
    REMORT, NORM, COOLDOWN_DEF,
    "scout", {82, 82, 26, 45, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_scout, SLOT( 0 ), 0, 0,
    "", "",
    ""},

   {
    MORTAL, NORM, COOLDOWN_DEF,
    "mount", {82, 82, 82, 20, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_mount, SLOT( 0 ), 0, 0,
    "", "",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "divine intervention", {82, 82, 82, 70, 82},
    spell_divine_intervention, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 635 ), 150, 3.80,
    "", "!Intervention!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "holy armor", {82, 82, 82, 30, 82},
    spell_holy_armor, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 636 ), 100, 1.00,
    "", "Your armor is no longer blessed.",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "unit tactics", {82, 82, 82, 16, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_unit_tactics, SLOT( 0 ), 0, 0,
    "Unit Tactics", "!Unit Tactics!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "earth elemental", {52, 82, 82, 82, 82},
    spell_earthelem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 637 ), 500, 2.30,
    "", "!EARTH ELEMENTAL!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "iron golem", {63, 82, 82, 82, 82},
    spell_iron_golem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 638 ), 800, 2.60,
    "", "!IRON GOLEM!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "diamond golem", {77, 82, 82, 82, 82},
    spell_diamond_golem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 639 ), 1100, 2.90,
    "", "!DIAMOND GOLEM!",
    ""},

   {

    REMORT, NORM, COOLDOWN_NONE,
    "soul thief", {82, 82, 82, 82, 67},
    spell_soul_thief, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 640 ), 900, 2.60,
    "", "!SOUL THIEF!",
    ""},
   {

    REMORT, NORM, COOLDOWN_NONE,
    "holy avenger", {82, 82, 82, 76, 82},
    spell_holy_avenger, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 641 ), 1200, 2.60,
    "", "!HOLY AVENGER!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "heat armor", {57, 78, 82, 82, 82},
    spell_heat_armor, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 642 ), 350, 2.50,
    "Heat Armor", "!Heat Armor!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "retributive strike", {72, 82, 82, 82, 78},
    spell_retri_strike, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 643 ), 800, 2.75,
    "Retributive strike", "!Retributive Strike!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "lava burst", {40, 82, 82, 82, 82},
    spell_lava_burst, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 644 ), 350, 3.50,
    "Lava Burst", "!Lava Burst!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "fireshield", {80, 82, 82, 82, 82},
    spell_fireshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 645 ), 400, 1.00,
    "", "",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "iceshield", {65, 82, 82, 82, 82},
    spell_iceshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 646 ), 350, 1.00,
    "", "",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "shockshield", {55, 82, 82, 82, 82},
    spell_shockshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 647 ), 400, 1.00,
    "", "",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "shadowshield", {82, 82, 82, 82, 82},
    spell_shadowshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 648 ), 400, 1.00,
    "", "@@NThe @@rshield@@N shatters!!",
    "@@NThe @@rshield@@N protecting $n shatters!!"},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "thoughtshield", {82, 82, 82, 82, 82},
    spell_thoughtshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 649 ), 400, 1.00,
    "", "@@NThe @@rshield@@N shatters!!",
    "@@NThe @@rshield@@N protecting $n shatters!!"},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "rage:wolven strength", {82, 82, 82, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 650 ), 0, 0,
    "", "",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "rage:sharpened claws", {82, 82, 82, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 651 ), 0, 0,
    "", "",
    ""},
   {
    REMORT, NORM, COOLDOWN_NONE,
    "rage:disregard for pain", {82, 82, 82, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 652 ), 0, 0,
    "", "",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "enraged", {82, 82, 82, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 653 ), 0, 0,
    "", "",
    ""},

   {
    MORTAL, WOLF, COOLDOWN_NONE,
    "tribal claw", {86, 15, 15, 15, 15},
    spell_tribe_claw, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 655 ), 50, 5.50,
    "", "",
    ""},

   {
    MORTAL, WOLF, COOLDOWN_NONE,
    "mark", {10, 5, 5, 5, 5},
    spell_wolf_mark, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 656 ), 5, 1.00,
    "", "",
    ""},

   {
    MORTAL, VAMP, COOLDOWN_NONE,
    "bloodsign", {12, 8, 10, 9, 7},
    spell_blood_sign, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 657 ), 5, 1.00,
    "", "",
    ""},
	
   {
    MORTAL, VAMP, COOLDOWN_NONE,
    "bloodsense", {10, 3, 3, 3, 3},
    spell_blood_sense, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 658 ), 1, 1.00,
    "", "",
    ""},

   {

    REMORT, NORM, COOLDOWN_NONE,
    "summon pegasus", {82, 82, 82, 60, 82},
    spell_summon_pegasus, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 659 ), 1200, 3.00,
    "", "",
    ""},

   {

    REMORT, NORM, COOLDOWN_NONE,
    "summon nightmare", {82, 82, 82, 82, 60},
    spell_summon_nightmare, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 660 ), 1200, 3.00,
    "", "",
    ""},

   {

    REMORT, NORM, COOLDOWN_NONE,
    "summon beast", {60, 82, 82, 82, 82},
    spell_summon_beast, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 661 ), 1200, 3.00,
    "", "",
    ""},

   {

    REMORT, NORM, COOLDOWN_NONE,
    "summon devourer", {82, 60, 82, 82, 82},
    spell_summon_devourer, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 662 ), 1200, 3.00,
    "", "",
    ""},

   {

    REMORT, NORM, COOLDOWN_NONE,
    "summon shadow", {82, 82, 60, 82, 82},
    spell_summon_shadow, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 663 ), 1200, 3.00,
    "", "",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "creature bond", {77, 55, 82, 82, 82},
    spell_creature_bond, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 664 ), 100, 2.80,
    "", "",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "corrupt bond", {82, 82, 82, 82, 33},
    spell_corrupt_bond, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 665 ), 100, 3.30,
    "", "",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "decapitate", {85, 85, 85, 85, 85},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_decapitate, SLOT( 0 ), 0, 0,
    "", "!decapitate!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "fire blast", {34, 82, 82, 82, 55},
    spell_fireblast, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 666 ), 80, 1.40,
    "Fire Blast", "!Fire Blast!",
    ""},
   {
    REMORT, NORM, COOLDOWN_NONE,
    "shock storm", {42, 82, 82, 82, 68},
    spell_shockstorm, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 667 ), 100, 2.25,
    "Shock Storm", "!Shock Storm!",
    ""},
   {
    REMORT, NORM, COOLDOWN_NONE,
    "cone of cold", {56, 82, 82, 82, 82},
    spell_cone_cold, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 668 ), 100, 3.30,
    "Cone of Cold", "!Cone of Cold!",
    ""},
   {
    REMORT, NORM, COOLDOWN_NONE,
    "holy wrath", {82, 82, 82, 80, 82},
    spell_holy_wrath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 669 ), 140, 3.50,
    "Holy Wrath", "!Holy Wrath!",
    ""},
   {
    REMORT, NORM, COOLDOWN_NONE,
    "wraith touch", {82, 82, 82, 82, 46},
    spell_wraith_touch, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 670 ), 120, 2.40,
    "Wraith Touch", "!Wraith Touch!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "thought vise", {82, 70, 82, 82, 82},
    spell_thought_vise, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 670 ), 200, 3.40,
    "Thought Vise", "!Thought Vise!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "black curse", {82, 82, 82, 82, 65},
    spell_black_curse, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 671 ), 120, 2.00,
    "Black Curse", "!Black Curse!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "cloak:misery", {82, 82, 80, 82, 73},
    spell_cloak_misery, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 672 ), 550, 1.00,
    "", "@@NThe @@Rcloak@@N around your body fades.",
    "@@NThe @@Rcloak@@N around $n's body fades."},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "poison:quinine", {82, 82, 36, 82, 82},
    spell_poison_quinine, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 673 ), 80, 1.00,
    "Poison Dart", "@@N$n feels much better!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "poison:arsenic", {82, 82, 28, 82, 82},
    spell_poison_arsenic, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 674 ), 80, 1.00,
    "Poison Dart", "@@N$nFeels much better!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "sonic blast", {82, 80, 82, 82, 82},
    spell_sonic_blast, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 675 ), 140, 3.40,
    "Sonic Blast", "@@N$nFeels much better!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "mystical potency", {40, 82, 82, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_potency, SLOT( 0 ), 0, 0,
    "", "!Enhanced Damage!",
    ""},

   {
    REMORT, NORM, COOLDOWN_NONE,
    "thaumatergy", {26, 82, 82, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_thaumatergy, SLOT( 0 ), 0, 0,
    "", "!Enhanced Damage!",
    ""},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "mystical focus", {55, 82, 82, 82, 82},
    spell_mystical_focus, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 676 ), 200, 1.00,
    "", "Your lose your mystical focus.",
    "$N shakes his head, and seems less focused."},

   {
    MORTAL, NORM, COOLDOWN_NONE,
    "redemption", {82, 15, 82, 82, 82},
    spell_redemption, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 678 ), 200, 10.00,
    "", "!Redemption!",
    ""},


};

const char *compass_name[] = {
   "north", "east", "south", "west", "up", "down"
};

char *const dir_name[] = {
   "to the North", "to the East", "to the South", "to the West", "Up", "Down"
};

/* rev_name used to indicate where ch has come from when entering -S- */

char *const rev_name[] = {
   "the South", "the West", "the North", "the East", "Below", "Above"
};

short movement_loss[SECT_MAX] = {
   1, 2, 2, 3, 4, 6, 4, 1, 6, 10, 6, 1
};

short rev_dir[] = {
   2, 3, 0, 1, 5, 4
};
