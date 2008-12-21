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

#include <stdio.h>
#include <time.h>
#include "globals.h"

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
   "                      ",  /* light  */
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
    RACE_MOD_SMALL | RACE_MOD_WOODLAND | RACE_MOD_STRONG_MAGIC,
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
    RACE_MOD_TINY | RACE_MOD_IMMUNE_POISON | RACE_MOD_DARKNESS,
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
    RACE_MOD_STRONG_MAGIC | RACE_MOD_TINY | RACE_MOD_RESIST_SPELL,
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
    RACE_MOD_SLOW_HEAL | RACE_MOD_STRONG_MAGIC | RACE_MOD_RESIST_SPELL | RACE_MOD_HUGE,
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
    MORTAL, NORM,
    "reserved", {99, 99, 99, 99, 99},
    0, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 0 ), 0, 0,
    "", "",
    ""},

   {
    MORTAL, NORM,
    "acid blast", {50, 82, 82, 82, 82},
    spell_acid_blast, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 70 ), 20, 275,
    "acid blast", "!Acid Blast!",
    ""},

   {
    MORTAL, NORM,
    "armor", {3, 82, 82, 82, 82},
    spell_armor, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 1 ), 5, 275,
    "", "You feel less protected.",
    ""},

   {
    REMORT, NORM,
    "animate", {82, 82, 82, 82, 70},
    spell_animate, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 525 ), 50, 550,
    "", "!Animate!",
    ""},

   {
    MORTAL, NORM,
    "bad breath", {4, 82, 82, 82, 82},
    spell_badbreath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 502 ), 10, 275,
    "bad breath", "!Bad Breath!",
    ""},

   {
    MORTAL, NORM,
    "bark skin", {82, 20, 82, 82, 82},
    spell_bark_skin, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 514 ), 20, 550,
    "", "Your skin feels softer.",
    "$n's skin looks less wooden."},

   {
    MORTAL, NORM,
    "bless", {82, 8, 82, 82, 82},
    spell_bless, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 3 ), 5, 275,
    "", "You feel less righteous.",
    "$n looks less Holy."},

   {
    MORTAL, NORM,
    "blindness", {16, 82, 82, 82, 82},
    spell_blindness, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_blindness, SLOT( 4 ), 5, 275,
    "", "You can see again.",
    "$n's vision returns."},

   {
    MORTAL, NORM,
    "bloody tears", {82, 82, 82, 82, 26},
    spell_bloody_tears, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 518 ), 12, 275,
    "Bloody Tears", "!Bloody Tears!",
    ""},

   {
    MORTAL, NORM,
    "burning hands", {6, 82, 82, 82, 82},
    spell_burning_hands, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 5 ), 15, 275,
    "burning hands", "!Burning Hands!",
    ""},

   {
    MORTAL, NORM,
    "call lightning", {82, 36, 82, 82, 82},
    spell_call_lightning, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 6 ), 15, 275,
    "lightning bolt", "!Call Lightning!",
    ""},

   {
    MORTAL, NORM,
    "calm", {82, 82, 82, 82, 52},
    spell_calm, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 531 ), 30, 550,
    "", "!Calm!",
    ""},

   {
    MORTAL, NORM,
    "cause critical", {82, 58, 82, 82, 82},
    spell_cause_critical, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 63 ), 20, 275,
    "spell", "!Cause Critical!",
    ""},

   {
    MORTAL, NORM,
    "cause light", {82, 4, 82, 82, 82},
    spell_cause_light, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 62 ), 15, 275,
    "spell", "!Cause Light!",
    ""},

   {
    MORTAL, NORM,
    "cause serious", {82, 40, 82, 82, 82},
    spell_cause_serious, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 64 ), 17, 275,
    "spell", "!Cause Serious!",
    ""},

   {
    MORTAL, NORM,
    "change sex", {66, 82, 82, 82, 82},
    spell_change_sex, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 82 ), 15, 275,
    "", "Your body feels familiar again.",
    "$n looks $mself again."},
/* 7, 15, 82, 82, 82    SRZ take out charm until fix wanted flag */
/* It's not a bug... attacking charmed mob is murder to owner -S- :P */
   {
    MORTAL, NORM,
    "charm person", {49, 60, 82, 82, 82},
    spell_charm_person, TAR_CHAR_OFFENSIVE, POS_STANDING,
    &gsn_charm_person, SLOT( 7 ), 5, 275,
    "", "You feel more self-confident.",
    "The glazed look in $n's eyes fades."},

   {
    MORTAL, NORM,
    "chill touch", {8, 82, 82, 82, 82},
    spell_chill_touch, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 8 ), 15, 275,
    "chilling touch", "You feel less cold.",
    ""},

   {
    MORTAL, NORM,
    "color spray", {7, 82, 82, 82, 82},
    spell_color_spray, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 10 ), 15, 275,
    "color spray", "!color Spray!",
    ""},

   {
    MORTAL, NORM,
    "continual light", {11, 27, 82, 82, 82},
    spell_continual_light, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 57 ), 7, 275,
    "", "!Continual Light!",
    ""},

   {
    MORTAL, NORM,
    "control weather", {58, 79, 82, 82, 82},
    spell_control_weather, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 11 ), 25, 275,
    "", "!Control Weather!",
    ""},

   {
    MORTAL, NORM,
    "create food", {82, 26, 82, 82, 82},
    spell_create_food, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 12 ), 5, 275,
    "", "!Create Food!",
    ""},

   {
    MORTAL, NORM,
    "create spring", {82, 65, 82, 82, 82},
    spell_create_spring, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 80 ), 20, 275,
    "", "!Create Spring!",
    ""},

   {
    MORTAL, NORM,
    "create water", {82, 9, 82, 82, 82},
    spell_create_water, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 13 ), 5, 275,
    "", "!Create Water!",
    ""},

   {
    MORTAL, NORM,
    "cure blindness", {82, 28, 82, 82, 82},
    spell_cure_blindness, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 14 ), 5, 275,
    "", "!Cure Blindness!",
    ""},

   {
    MORTAL, NORM,
    "cure critical", {82, 59, 82, 82, 82},
    spell_cure_critical, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 15 ), 20, 275,
    "", "!Cure Critical!",
    ""},

   {
    MORTAL, NORM,
    "cure light", {82, 5, 82, 82, 82},
    spell_cure_light, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 16 ), 10, 275,
    "", "!Cure Light!",
    ""},

   {
    MORTAL, NORM,
    "cure poison", {82, 32, 82, 82, 82},
    spell_cure_poison, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 43 ), 5, 275,
    "", "!Cure Poison!",
    ""},

   {
    MORTAL, NORM,
    "cure serious", {82, 41, 82, 82, 82},
    spell_cure_serious, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 61 ), 15, 275,
    "", "!Cure Serious!",
    ""},

   {
    MORTAL, NORM,
    "curse", {35, 26, 82, 82, 82},
    spell_curse, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_curse, SLOT( 17 ), 20, 275,
    "curse", "The curse wears off.",
    "$n starts to look more Holy."},

   {
    MORTAL, NORM,
    "detect evil", {12, 8, 82, 82, 82},
    spell_detect_evil, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 18 ), 5, 275,
    "", "The red in your vision disappears.",
    ""},

   {
    MORTAL, NORM,
    "detect hidden", {17, 16, 82, 82, 82},
    spell_detect_hidden, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 44 ), 5, 275,
    "", "You feel less aware of your suroundings.",
    ""},

   {
    MORTAL, NORM,
    "detect invis", {19, 82, 82, 82, 82},
    spell_detect_invis, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 19 ), 5, 275,
    "", "You no longer see invisible objects.",
    ""},

   {
    MORTAL, NORM,
    "detect magic", {7, 7, 82, 82, 82},
    spell_detect_magic, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 20 ), 5, 275,
    "", "The detect magic wears off.",
    ""},

   {
    MORTAL, NORM,
    "detect poison", {82, 10, 82, 82, 82},
    spell_detect_poison, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 21 ), 5, 275,
    "", "!Detect Poison!",
    ""},

   {
    MORTAL, NORM,
    "detect undead", {8, 82, 82, 82, 82},
    spell_detect_undead, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 513 ), 8, 275,
    "", "You no longer sense undead beings.",
    ""},

   {
    MORTAL, NORM,
    "detection", {82, 82, 82, 82, 22},
    spell_detection, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 527 ), 12, 550,
    "", "!Detection!",
    ""},

   {
    MORTAL, NORM,
    "dimension blade", {86, 86, 86, 86, 86},
    spell_dimension_blade, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 523 ), 100, 675,
    "", "!Dimension Blade!",
    ""},

/*    Doesn't work well with intelligent mobs. Therefore get rid of it... :P */

   {
    MORTAL, NORM,
    "dispel evil", {82, 82, 82, 82, 82},  /* was 82, 63, ... */
    spell_dispel_evil, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 22 ), 15, 275,
    "dispel evil", "!Dispel Evil!",
    ""},

   {
    MORTAL, NORM,
    "dispel magic", {68, 85, 85, 85, 85},
    spell_dispel_magic, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 59 ), 15, 275,
    "", "!Dispel Magic!",
    ""},

   {
    MORTAL, NORM,
    "earthquake", {82, 78, 82, 82, 82},
    spell_earthquake, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 23 ), 15, 275,
    "earthquake", "!Earthquake!",
    ""},

   {
    MORTAL, NORM,
    "ego whip", {82, 82, 82, 82, 25},
    spell_ego_whip, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 535 ), 35, 550,
    "ego whip", "!Ego Whip!",
    ""},

   {
    MORTAL, NORM,
    "enchant weapon", {39, 82, 82, 82, 82},
    spell_enchant_weapon, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 24 ), 100, 550,
    "", "!Enchant Weapon!",
    ""},

   {
    MORTAL, VAMP,
    "energy drain", {15, 7, 8, 11, 9},
    spell_energy_drain, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 25 ), 12, 275,
    "energy drain", "!Energy Drain!",
    ""},

   {
    MORTAL, NORM,
    "enhance weapon", {86, 86, 86, 86, 86},
    spell_enhance_weapon, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 517 ), 50, 550,
    "", "!Enhance Weapon!",
    ""},

   {
    MORTAL, NORM,
    "faerie fire", {3, 82, 82, 82, 82},
    spell_faerie_fire, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 72 ), 5, 275,
    "faerie fire", "The pink aura around you fades away.",
    "The pink aura around $n fades away."},

   {
    MORTAL, NORM,
    "faerie fog", {82, 14, 82, 82, 82},
    spell_faerie_fog, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 73 ), 12, 275,
    "faerie fog", "!Faerie Fog!",
    ""},

   {
    MORTAL, NORM,
    "fighting trance", {82, 82, 82, 82, 60},
    spell_fighting_trance, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 521 ), 60, 550,
    "", "Your fighting trance fades.",
    "$n's fighting trance fades."},

   {
    MORTAL, NORM,
    "fireball", {28, 82, 82, 82, 82},
    spell_fireball, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 26 ), 15, 275,
    "fireball", "!Fireball!",
    ""},

   {
    MORTAL, NORM,
    "fire blade", {82, 82, 82, 82, 50},
    spell_fire_blade, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 528 ), 50, 550,
    "", "!FireBlade!",
    ""},

   {
    MORTAL, NORM,
    "flamestrike", {82, 54, 82, 82, 82},
    spell_flamestrike, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 65 ), 20, 275,
    "flamestrike", "!Flamestrike!",
    ""},

   {
    MORTAL, NORM,
    "flare", {82, 20, 82, 82, 82},
    spell_flare, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 591 ), 18, 275,
    "flare", "Your vision returns.",
    "$n blinks, and starts to see again."},


   {
    MORTAL, NORM,
    "fly", {24, 82, 82, 82, 82},
    spell_fly, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 56 ), 10, 275,
    "", "You slowly float to the ground.",
    "$n slowly floats to the ground."},

   {
    MORTAL, NORM,
    "gate", {82, 82, 82, 82, 82},
    spell_gate, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 83 ), 50, 275,
    "", "!Gate!",
    ""},

   {
    MORTAL, NORM,
    "giant strength", {18, 82, 82, 82, 82},
    spell_giant_strength, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 39 ), 20, 275,
    "", "You feel weaker.",
    "$n looks weaker."},

   {
    MORTAL, NORM,
    "harm", {82, 74, 82, 82, 82},
    spell_harm, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 27 ), 35, 275,
    "harm spell", "!Harm!",
    ""},

   {
    MORTAL, NORM,
    "heal", {82, 75, 82, 82, 82},
    spell_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 28 ), 50, 275,
    "", "!Heal!",
    ""},

   {
    MORTAL, NORM,
    "hypnosis", {82, 82, 82, 82, 14},
    spell_hypnosis, TAR_CHAR_OFFENSIVE, POS_STANDING,
    NULL, SLOT( 532 ), 30, 550,
    "", "!Hypnosis!",
    ""},

   {
    MORTAL, NORM,
    "identify", {9, 82, 82, 82, 82},
    spell_identify, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 53 ), 12, 275,
    "", "!Identify!",
    ""},

   {
    MORTAL, NORM,
    "infravision", {20, 82, 82, 82, 82},
    spell_infravision, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 77 ), 5, 275,
    "", "You no longer see in the dark.",
    ""},

   {
    MORTAL, NORM,
    "influx", {82, 65, 82, 82, 82},
    spell_influx, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 503 ), 75, 550,
    "", "!Influx!",
    ""},

   {
    MORTAL, NORM,
    "invis", {17, 82, 82, 82, 82},
    spell_invis, TAR_IGNORE, POS_STANDING,
    &gsn_invis, SLOT( 29 ), 5, 275,
    "", "You are no longer invisible.",
    "$n's invisibilty fades."},

   {
    MORTAL, NORM,
    "know alignment", {13, 22, 82, 82, 82},
    spell_know_alignment, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 58 ), 9, 275,
    "", "!Know Alignment!",
    ""},

   {
    MORTAL, NORM,
    "know weakness", {82, 82, 82, 82, 18},
    spell_know_weakness, TAR_CHAR_SELF, POS_FIGHTING,
    NULL, SLOT( 530 ), 15, 275,
    "", "You are less aware of your enemy's weaknesses.",
    ""},

   {
    MORTAL, NORM,
    "know critical", {82, 82, 82, 82, 28},
    spell_know_weakness, TAR_CHAR_SELF, POS_FIGHTING,
    NULL, SLOT( 530 ), 15, 275,
    "", "You are less aware of critical damage points.",
    ""},

   {
    MORTAL, NORM,
    "know item", {82, 82, 82, 82, 9},
    spell_know_item, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 533 ), 20, 550,
    "", "!Know Item!",
    ""},

   {
    MORTAL, NORM,
    "laser bolt", {82, 82, 82, 82, 82},
    spell_laserbolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 511 ), 35, 275,
    "laserbolt", "!laserbolt!",
    ""},

   {
    MORTAL, NORM,
    "lightning bolt", {24, 82, 82, 82, 82},
    spell_lightning_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 30 ), 15, 275,
    "lightning bolt", "!Lightning Bolt!",
    ""},

   {
    MORTAL, NORM,
    "locate object", {16, 30, 82, 82, 82},
    spell_locate_object, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 31 ), 20, 275,
    "", "!Locate Object!",
    ""},

   {
    MORTAL, NORM,
    "magic missile", {3, 82, 82, 82, 82},
    spell_magic_missile, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 32 ), 15, 275,
    "magic missile", "!Magic Missile!",
    ""},

   {
    MORTAL, NORM,
    "mass invis", {60, 82, 82, 82, 82},
    spell_mass_invis, TAR_IGNORE, POS_STANDING,
    &gsn_mass_invis, SLOT( 69 ), 20, 550,
    "", "!Mass Invis!",
    ""},


   {
    MORTAL, NORM,
    "mind flail", {82, 82, 82, 82, 4},
    spell_mind_flail, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 536 ), 12, 550,
    "mind flail", "!Mind Flail!",
    ""},

   {
    MORTAL, NORM,
    "mystic armor", {10, 82, 82, 82, 82},
    spell_mystic_armor, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 590 ), 18, 275,
    "" "Your Mystic Armor fades away",
    "The Mystic Armor around someone in the room fades away."},


   {
    MORTAL, NORM,
    "nerve fire", {82, 82, 82, 82, 60},
    spell_nerve_fire, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 520 ), 50, 550,
    "nerve fire", "!Nerve Fire!",
    ""},


   {
    MORTAL, NORM,
    "night vision", {82, 82, 82, 82, 19},
    spell_night_vision, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 538 ), 17, 275,
    "", "Your eyes feel weaker.",
    ""},


   {
    MORTAL, NORM,
    "pass door", {46, 82, 82, 82, 82},
    spell_pass_door, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 74 ), 20, 275,
    "", "You feel solid again.",
    "$n's body becomes more solid."},

   {
    MORTAL, NORM,
    "phase", {82, 82, 82, 82, 45},
    spell_phase, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 522 ), 20, 275,
    "", "You feel solid again.",
    "$n's body becomes more solid."},

   {
    MORTAL, NORM,
    "psychic crush", {82, 82, 82, 82, 19},
    spell_physic_thrust, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 537 ), 33, 550,
    "psychic crush", "!Psychic Crush!",
    ""},

   {
    MORTAL, NORM,
    "psychic thrust", {82, 82, 82, 82, 7},
    spell_physic_thrust, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 537 ), 22, 550,
    "psychic thrust", "!Physic Thrust!",
    ""},

   {
    MORTAL, NORM,
    "poison", {82, 31, 82, 82, 82},
    spell_poison, TAR_CHAR_OFFENSIVE, POS_STANDING,
    &gsn_poison, SLOT( 33 ), 10, 275,
    "poison", "You feel less sick.",
    "$n looks less sick."},

   {
    MORTAL, NORM,
    "produce food", {82, 82, 82, 82, 24},
    spell_produce_food, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 524 ), 16, 275,
    "", "!Produce Food!",
    ""},

   {
    MORTAL, NORM,
    "protection", {9, 12, 82, 82, 82},
    spell_protection, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 34 ), 5, 275,
    "", "You feel less protected.",
    ""},

   {
    MORTAL, NORM,
    "refresh", {82, 11, 82, 82, 82},
    spell_refresh, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 81 ), 12, 275,
    "refresh", "!Refresh!",
    ""},

   {
    MORTAL, NORM,
    "remove curse", {36, 27, 82, 82, 82},
    spell_remove_curse, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 35 ), 5, 275,
    "", "!Remove Curse!",
    ""},

   {
    MORTAL, NORM,
    "sanctuary", {82, 70, 82, 82, 82},
    spell_sanctuary, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 36 ), 75, 275,
    "", "The white aura around your body fades.",
    "The white aura around $n's body fades."},

   {
    MORTAL, NORM,
    "seal room", {70, 50, 82, 82, 50},
    spell_seal_room, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 541 ), 75, 275,
    "", "The Energy web around this room fades.",
    ""},

   {
    MORTAL, NORM,
    "see magic", {82, 82, 82, 82, 7},
    spell_see_magic, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 526 ), 8, 275,
    "", "You no longer see magical auras.",
    ""},

   {
    MORTAL, NORM,
    "sense evil", {82, 82, 82, 82, 10},
    spell_sense_evil, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 515 ), 12, 275,
    "", "You no longer sense evil.",
    ""},

   {
    MORTAL, VAMP,
    "shade", {12, 7, 6, 8, 7},
    spell_shade, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 542 ), 5, 275,
    "", "The room gets lighter.",
    ""},

   {
    MORTAL, NORM,
    "shield", {9, 82, 82, 82, 82},
    spell_shield, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 67 ), 12, 275,
    "", "Your force shield shimmers, then fades away.",
    "$n's force field shimmers, then fades away."},

   {
    MORTAL, NORM,
    "shocking grasp", {5, 82, 82, 82, 82},
    spell_shocking_grasp, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 82 ), 15, 275,
    "shocking grasp", "!Shocking Grasp!",
    ""},

   {
    MORTAL, NORM,
    "sight", {30, 82, 82, 82, 82},
    spell_cure_blindness, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 592 ), 20, 550,
    "", "!Sight!",
    ""},

   {
    MORTAL, NORM,
    "sleep", {9, 82, 82, 82, 82},
    spell_sleep, TAR_CHAR_OFFENSIVE, POS_STANDING,
    &gsn_sleep, SLOT( 38 ), 15, 275,
    "", "You feel less tired.",
    "$n starts looking much more awake."},

   {
    MORTAL, NORM,
    "stalker", {82, 82, 82, 82, 82},
    spell_stalker, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 401 ), 100, 550,
    "", "!Stalker!",
    ""},

   {
    MORTAL, NORM,
    "stone skin", {36, 82, 82, 82, 82},
    spell_stone_skin, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 66 ), 12, 275,
    "", "Your skin feels soft again.",
    "$n's skin loses it's stone-like look."},

   {
    MORTAL, NORM,
    "suffocate", {82, 82, 82, 82, 46},
    spell_suffocate, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 516 ), 30, 550,
    "a choking hand", "!Suffocate!",
    ""},

   {
    MORTAL, NORM,
    "summon", {35, 82, 82, 82, 82},
    spell_summon, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 40 ), 50, 275,
    "", "!Summon!",
    ""},

   {
    MORTAL, NORM,
    "teleport", {40, 82, 82, 82, 82},
    spell_teleport, TAR_CHAR_SELF, POS_FIGHTING,
    NULL, SLOT( 2 ), 35, 275,
    "", "!Teleport!",
    ""},

   {
    MORTAL, NORM,
    "travel", {82, 82, 82, 82, 82},
    spell_travel, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 599 ), 50, 550,
    "", "!Travel!",
    ""},

   {
    MORTAL, NORM,
    "vampire bite", {86, 86, 86, 86, 86},
    spell_null, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 0 ), 5, 275,
    "", "Your blood feels cooler.",
    ""},


   {
    MORTAL, NORM,
    "ventriloquate", {3, 82, 82, 82, 82},
    spell_ventriloquate, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 41 ), 5, 275,
    "", "!Ventriloquate!",
    ""},

   {
    MORTAL, NORM,
    "warcry", {82, 82, 82, 5, 82},
    spell_null, TAR_CHAR_SELF, POS_STANDING,
    &gsn_warcry, SLOT( 501 ), 5, 275,
    "", "You feel your courage subside.",
    ""},

   {
    MORTAL, NORM,
    "weaken", {82, 20, 82, 82, 82},
    spell_weaken, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 68 ), 20, 275,
    "spell", "You feel stronger.",
    "$n looks stronger."},

   {
    MORTAL, NORM,
    "beacon", {70, 82, 82, 82, 70},
    spell_beacon, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 399 ), 25, 275,
    "", "!Beacon!",
    ""},

   {
    MORTAL, NORM,
    "portal", {80, 82, 82, 82, 82},
    spell_portal, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 334 ), 100, 275,
    "", "!Portal!",
    ""},

   {
    MORTAL, NORM,
    "window", {70, 82, 82, 82, 70},
    spell_window, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 333 ), 100, 275,
    "", "!Window!",
    ""},

   {
    MORTAL, NORM,
    "word of recall", {82, 66, 82, 82, 82},
    spell_word_of_recall, TAR_CHAR_SELF, POS_RESTING,
    NULL, SLOT( 42 ), 5, 275,
    "", "!Word of Recall!",
    ""},

   {
    MORTAL, NORM,
    "hellspawn", {43, 82, 82, 82, 82},
    spell_hellspawn, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 512 ), 50, 275,
    "HellSpawn", "!Hellspawn!",
    ""},

   {
    MORTAL, NORM,
    "planergy", {82, 82, 82, 82, 5},
    spell_planergy, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 504 ), 25, 550,
    "energy", "!planergy!",
    ""},

   {
    MORTAL, NORM,
    "visit", {82, 82, 82, 82, 32},
    spell_visit, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 505 ), 50, 550,
    "", "!visit!",
    ""},

   {
    MORTAL, NORM,
    "barrier", {82, 82, 82, 82, 8},
    spell_barrier, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 506 ), 30, 550,
    "", "Your barrier slowly fades.",
    "The barrier around $n fades."},

   {
    MORTAL, NORM,
    "phobia", {82, 82, 82, 82, 10},
    spell_phobia, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 507 ), 32, 550,
    "phobia attack", "!phobia!",
    ""},

   {
    MORTAL, NORM,
    "mind bolt", {82, 82, 82, 82, 12},
    spell_mind_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 519 ), 40, 275,
    "Mind Bolt", "!MindBolt!",
    ""},

   {
    MORTAL, NORM,
    "mindflame", {82, 82, 82, 82, 35},
    spell_mindflame, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 508 ), 40, 550,
    "Mind Flame", "!mindflame!",
    ""},

   {
    MORTAL, NORM,
    "chain lightning", {65, 82, 82, 82, 82},
    spell_chain_lightning, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 509 ), 25, 550,
    "lightening bolt", "!chain-light!",
    ""},

   {
    MORTAL, NORM,
    "static", {82, 82, 82, 82, 20},
    spell_static, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 510 ), 40, 550,
    "discharge", "!static!",
    ""},

   {
    REMORT, NORM,
    "cloak:absorption", {60, 43, 82, 82, 82},
    spell_cloak_absorb, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 221 ), 500, 275,
    "", "@@NThe @@lcloak@@N around your body fades.",
    "@@NThe @@lcloak@@N around $n's body fades."},

   {
    REMORT, NORM,
    "cloak:reflection", {48, 82, 82, 82, 82},
    spell_cloak_reflect, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 222 ), 500, 275,
    "", "@@NThe @@lc@@el@@ro@@ya@@ak@@N around your body fades.",
    "@@NThe @@lc@@el@@ro@@ya@@ak@@N around $n's body fades."},

   {
    REMORT, NORM,
    "cloak:flaming", {70, 82, 78, 82, 60},
    spell_cloak_flaming, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 223 ), 750, 275,
    "", "@@NThe @@ecloak@@N around your body fades.",
    "@@NThe @@ecloak@@N around $n's body fades."},

   {
    MORTAL, VAMP,
    "cloak:darkness", {18, 13, 14, 14, 16},
    spell_cloak_darkness, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 224 ), 45, 275,
    "", "The @@dcloak@@N around your body fades.",
    "@@NThe @@dcloak@@N around $n's body fades."},

   {
    ADEPT, NORM,
    "cloak:adept", {1, 82, 82, 82, 82},
    spell_cloak_adept, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 226 ), 500, 275,
    "", "@@NThe @@Wcloak@@N around your body fades.",
    "@@NThe @@Wcloak@@N around $n's body fades."},

   {
    REMORT, NORM,
    "cloak:regeneration", {82, 73, 82, 52, 82},
    spell_cloak_regen, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 227 ), 500, 275,
    "", "@@NThe @@rcloak@@N around your body fades.",
    "@@NThe @@rcloak@@N around $n's body fades."},

/*
 * Dragon breath                                
 */
   {
    MORTAL, NORM,
    "acid breath", {82, 82, 82, 82, 82},
    spell_acid_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 200 ), 0, 125,
    "blast of acid", "!Acid Breath!",
    ""},

   {
    MORTAL, NORM,
    "fire breath", {82, 82, 82, 82, 82},
    spell_fire_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 201 ), 0, 125,
    "blast of flame", "!Fire Breath!",
    ""},

   {
    MORTAL, NORM,
    "frost breath", {82, 82, 82, 82, 82},
    spell_frost_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 202 ), 0, 125,
    "blast of frost", "!Frost Breath!",
    ""},

   {
    MORTAL, NORM,
    "gas breath", {82, 82, 82, 82, 82},
    spell_gas_breath, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 203 ), 0, 125,
    "blast of gas", "!Gas Breath!",
    ""},

   {
    MORTAL, NORM,
    "lightning breath", {82, 82, 82, 82, 82},
    spell_lightning_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 204 ), 0, 125,
    "blast of lightning", "!Lightning Breath!",
    ""},

   {
    MORTAL, VAMP,
    "blood leach", {8, 4, 3, 4, 4},
    spell_blood_leach, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 540 ), 6, 275,
    "", "Your body feels stronger again.",
    ""},
/*
 * Fighter and thief skills.
 */

   {
    MORTAL, NORM,
    "appraise", {82, 82, 12, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_appraise, SLOT( 0 ), 0, 275,
    "", "!Appraise!",
    ""},

   {
    MORTAL, NORM,
    "bash", {82, 82, 82, 28, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_bash, SLOT( 0 ), 0, 275,
    "bash", "!bash!",
    ""},

   {
    MORTAL, NORM,
    "berserk", {82, 82, 82, 34, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_berserk, SLOT( 0 ), 0, 275,
    "", "You calm down!",
    "$n looks calmer!"},

   {
    MORTAL, NORM,
    "climb", {82, 82, 25, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_climb, SLOT( 0 ), 0, 275,
    "", "!Climb!",
    ""},

   {
    MORTAL, NORM,
    "dirt", {82, 82, 82, 35, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_dirt, SLOT( 0 ), 0, 275,
    "", "!dirt!",
    ""},

   {
    MORTAL, NORM,
    "nodisarm", {82, 82, 40, 30, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_nodisarm, SLOT( 0 ), 0, 0,
    "", "!nodisarm!",
    ""},

   {
    MORTAL, NORM,
    "notrip", {82, 82, 30, 40, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_notrip, SLOT( 0 ), 0, 0,
    "", "!notrip!",
    ""},

   {
    MORTAL, NORM,
    "smash", {82, 82, 82, 30, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_smash, SLOT( 0 ), 0, 0,
    "", "!smash!",
    ""},

   {
    MORTAL, NORM,
    "trip", {82, 82, 18, 25, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_trip, SLOT( 0 ), 0, 275,
    "", "!trip!",
    ""},


   {
    MORTAL, NORM,
    "circle", {82, 82, 19, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_circle, SLOT( 0 ), 0, 275,
    "backstab", "!circle!",
    ""},

   {
    MORTAL, NORM,
    "backstab", {82, 82, 10, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_backstab, SLOT( 0 ), 0, 275,
    "backstab", "!Backstab!",
    ""},

   {
    MORTAL, NORM,
    "disarm", {82, 82, 82, 12, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_disarm, SLOT( 0 ), 0, 275,
    "", "!Disarm!",
    ""},

   {
    MORTAL, NORM,
    "dodge", {82, 82, 7, 16, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_dodge, SLOT( 0 ), 0, 0,
    "", "!Dodge!",
    ""},

   {
    REMORT, NORM,
    "dualwield", {82, 7, 8, 5, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_dualwield, SLOT( 0 ), 0, 0,
    "", "!DualWield!",
    ""},

   {
    MORTAL, NORM,
    "enhanced damage", {82, 82, 82, 42, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_enhanced_damage, SLOT( 0 ), 0, 0,
    "", "!Enhanced Damage!",
    ""},

   {
    MORTAL, VAMP,
    "feed", {1, 1, 1, 1, 1},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_feed, SLOT( 0 ), 0, 550,
    "", "!feed!",
    ""},

   {
    MORTAL, NORM,
    "find doors", {82, 82, 16, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_find_doors, SLOT( 0 ), 0, 0,
    "", "!Find Doors!",
    ""},

   {
    MORTAL, NORM,
    "crushing blow", {82, 82, 82, 50, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_crushing_blow, SLOT( 0 ), 0, 0,
    "", "!Crushing Blow!",
    ""},

   {
    MORTAL, NORM,
    "headbutt", {82, 82, 82, 6, 82},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_headbutt, SLOT( 0 ), 0, 275,
    "headbutt", "!HeadButt!",
    ""},

   {
    MORTAL, NORM,
    "hide", {82, 82, 12, 82, 82},
    spell_null, TAR_IGNORE, POS_RESTING,
    &gsn_hide, SLOT( 0 ), 0, 275,
    "", "!Hide!",
    ""},

   {
    MORTAL, NORM,
    "hunt", {82, 82, 70, 82, 82},
    spell_null, TAR_IGNORE, POS_RESTING,
    &gsn_hunt, SLOT( 0 ), 0, 275,
    "", "!Hunt!",
    ""},

   {
    MORTAL, NORM,
    "kick", {82, 82, 82, 14, 82},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_kick, SLOT( 0 ), 0, 275,
    "kick", "!Kick!",
    ""},

   {
    MORTAL, NORM,
    "knee", {82, 82, 82, 8, 82},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_knee, SLOT( 0 ), 0, 275,
    "knee", "!Knee",
    ""},

   {
    MORTAL, NORM,
    "martial arts", {82, 82, 30, 20, 30},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_martial_arts, SLOT( 0 ), 0, 0,
    "", "!Martial Arts!",
    ""},

   {
    MORTAL, NORM,
    "parry", {82, 82, 82, 10, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_parry, SLOT( 0 ), 0, 0,
    "", "!Parry!",
    ""},

   {
    MORTAL, NORM,
    "peek", {82, 82, 20, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_peek, SLOT( 0 ), 0, 0,
    "", "!Peek!",
    ""},

   {
    MORTAL, NORM,
    "pick lock", {82, 82, 25, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_pick_lock, SLOT( 0 ), 0, 275,
    "", "!Pick!",
    ""},

   {
    MORTAL, NORM,
    "punch", {82, 82, 82, 5, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_punch, SLOT( 0 ), 0, 275,
    "", "!Punch!",
    ""},

   {
    MORTAL, NORM,
    "rescue", {82, 82, 82, 15, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_rescue, SLOT( 0 ), 0, 275,
    "", "!Rescue!",
    ""},

   {
    MORTAL, NORM,
    "enhanced reflexes", {82, 82, 10, 15, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_enhanced_reflexes, SLOT( 0 ), 0, 0,
    "", "!Enhanced Reflexes!",
    ""},

   {
    MORTAL, NORM,
    "shadowform", {82, 82, 82, 82, 9},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_shadow, SLOT( 0 ), 0, 0,
    "", NULL,
    ""},

   {
    MORTAL, NORM,
    "shield block", {82, 82, 82, 40, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_shield_block, SLOT( 0 ), 0, 275,
    "", "!Shield Block!",
    ""},

   {
    MORTAL, NORM,
    "sneak", {82, 82, 5, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_sneak, SLOT( 0 ), 0, 275,
    "", NULL,
    ""},

   {
    MORTAL, NORM,
    "stake", {85, 85, 85, 85, 85},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_stake, SLOT( 0 ), 0, 550,
    "", NULL,
    ""},

   {
    MORTAL, NORM,
    "steal", {82, 82, 1, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_steal, SLOT( 0 ), 0, 550,
    "", "!Steal!",
    ""},

   {
    REMORT, NORM,
    "stun", {82, 25, 82, 18, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_stun, SLOT( 0 ), 0, 550,
    "", "",
    ""},

   {
    MORTAL, NORM,
    "sleight of hand", {82, 82, 60, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_sleight_of_hand, SLOT( 0 ), 0, 0,
    "", "!Sleight of Hand!",
    ""},

   {
    REMORT, NORM,
    "disguise", {82, 82, 25, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_disguise, SLOT( 0 ), 0, 0,
    "", "!Disguise!",
    ""},

   {
    MORTAL, VAMP,
    "instruct", {10, 5, 5, 5, 5},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_instruct, SLOT( 0 ), 0, 0,
    "", "!INSTRUCT!",
    ""},

   {
    REMORT, NORM,
    "frenzy", {82, 18, 82, 23, 82},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_frenzy, SLOT( 0 ), 0, 550,
    "frenzy", "!FRENZY!",
    ""},

   {
    MORTAL, WOLF,
    "imprint", {20, 10, 10, 10, 10},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_imprint, SLOT( 0 ), 0, 0,
    "", "!INSTRUCT!",
    ""},

   {
    MORTAL, NORM,
    "emotion control", {82, 82, 82, 82, 75},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_emotion_control, SLOT( 0 ), 0, 0,
    "emotion control", "!EMOTION CONTROL!",
    ""},

/*
 *  Spells for mega1.are from Glop/Erkenbrand.
*/
   {
    MORTAL, NORM,
    "general purpose", {82, 82, 82, 82, 82},
    spell_general_purpose, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 205 ), 0, 275,
    "general purpose ammo", "!General Purpose Ammo!",
    ""},

   {
    MORTAL, NORM,
    "high explosive", {82, 82, 82, 82, 82},
    spell_high_explosive, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 206 ), 0, 275,
    "high explosive ammo", "!High Explosive Ammo!",
    ""},

   {
    REMORT, NORM,
    "deflect weapon", {82, 10, 82, 82, 82},
    spell_deflect_weapon, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 600 ), 25, 275,
    "", "Your mind shield melts away.",
    ""},

   {
    REMORT, NORM,
    "black hand", {82, 82, 82, 82, 7},
    spell_black_hand, TAR_CHAR_OFFENSIVE, POS_STANDING,
    NULL, SLOT( 601 ), 50, 275,
    "", "The hand dissolves from around your throat into nothingness.",
    ""},

   {
    REMORT, NORM,
    "throwing needle", {82, 82, 5, 82, 82},
    spell_throw_needle, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 602 ), 40, 275,
    "Throwing Needle", "!Throwing Needle!",
    ""},

   {
    REMORT, NORM,
    "morale", {82, 82, 82, 10, 82},
    spell_morale, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 603 ), 75, 275,
    "Morale", "!Morale!",
    ""},

   {
    REMORT, NORM,
    "leadership", {82, 82, 82, 13, 82},
    spell_leadership, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 604 ), 75, 275,
    "Leadership", "!Leadership!",
    ""},

   {
    REMORT, NORM,
    "ice bolt", {3, 82, 82, 82, 82},
    spell_ice_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 605 ), 20, 275,
    "Ice Bolt", "!Ice Bolt!",
    ""},

   {
    REMORT, NORM,
    "water elemental", {6, 82, 82, 82, 82},
    spell_waterelem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 606 ), 80, 275,
    "", "!Stalker!",
    ""},

   {
    REMORT, NORM,
    "skeleton", {82, 82, 82, 82, 9},
    spell_skeleton, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 607 ), 80, 275,
    "", "!Stalker!",
    ""},

   {
    REMORT, NORM,
    "poison weapon", {82, 82, 18, 82, 25},
    spell_poison_weapon, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 608 ), 100, 275,
    "", "!Enchant Weapon!",
    ""},

   {
    MORTAL, VAMP,
    "embrace", {86, 15, 15, 15, 15},
    spell_embrace, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 609 ), 15, 550,
    "", "!EMBRACE!",
    ""},

   {
    MORTAL, VAMP,
    "mesmerise", {5, 2, 2, 2, 2},
    spell_mesmerise, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 610 ), 4, 125,
    "", "!MESMERISE!",
    ""},

   {
    REMORT, NORM,
    "ethereal travel", {30, 24, 82, 82, 40},
    spell_ethereal, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 611 ), 250, 550,
    "", "ETHEREAL TRAVEL!",
    ""},

   {
    REMORT, NORM,
    "adrenaline", {82, 28, 82, 38, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_adrenaline, SLOT( 0 ), 0, 0,
    "", "!ADRENALINE!",
    ""},

   {
    REMORT, NORM,
    "throwing star", {82, 82, 43, 82, 82},
    spell_throw_needle, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 628 ), 85, 275,
    "Throwing Star", "!Throwing Star!",
    ""},

/* THIS IS A SPELL AFFECT HOLDER FOR THE SKILL ADRENALINE..DO NOT CAST! */

   {
    REMORT, NORM,
    "adrenaline bonus", {82, 82, 82, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 612 ), 0, 0,
    "", "!ADRENALINE BONUS!",
    ""},

   {
    REMORT, NORM,
    "fire elemental", {11, 82, 82, 82, 82},
    spell_fireelem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 612 ), 120, 275,
    "", "!FIRE ELEMENTAL!",
    ""},

   {
    REMORT, NORM,
    "rune:fire", {11, 82, 82, 82, 15},
    spell_rune_fire, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 613 ), 150, 275,
    "", "The rune dissipates.",
    ""},

   {
    REMORT, NORM,
    "rune:shock", {13, 15, 82, 82, 82},
    spell_rune_shock, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 614 ), 150, 275,
    "", "The rune dissipates.",
    ""},

   {
    REMORT, NORM,
    "rune:poison", {82, 82, 15, 82, 12},
    spell_rune_poison, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 615 ), 80, 275,
    "", "The rune dissipates.",
    ""},

   {
    REMORT, NORM,
    "healing light", {82, 33, 82, 25, 82},
    spell_healing_light, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 616 ), 150, 275,
    "", "@@NThe @@mHealing Light@@N dissipates.",
    ""},

   {
    REMORT, NORM,
    "withering shadow", {82, 82, 35, 82, 17},
    spell_wither_shadow, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 617 ), 150, 275,
    "", "@@NThe @@dWithering Shadow@@N dissipates.",
    ""},

   {
    REMORT, NORM,
    "mana flare", {18, 22, 82, 82, 82},
    spell_mana_flare, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 618 ), 80, 275,
    "", "@@NThe @@eMana Flare@@N dissipates.",
    ""},

   {
    REMORT, NORM,
    "mana drain", {11, 82, 82, 82, 15},
    spell_mana_drain, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 619 ), 150, 275,
    "", "@@NThe @@dMana Drain@@N dissipates.",
    ""},

   {
    REMORT, NORM,
    "cage", {18, 13, 31, 40, 19},
    spell_cage, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 620 ), 150, 275,
    "", "@@NThe @@rCage@@N dissipates.",
    ""},

   {
    REMORT, NORM,
    "room dispel", {22, 82, 82, 33, 82},
    spell_room_dispel, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 625 ), 120, 275,
    "", "",
    ""},

   {
    REMORT, NORM,
    "soul net", {82, 82, 82, 82, 58},
    spell_soul_net, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 629 ), 350, 275,
    "", "@@NThe @@dSoul Net@@N dissipates.",
    ""},

   {
    REMORT, NORM,
    "condense soul", {82, 82, 82, 82, 79},
    spell_condense_soul, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 630 ), 800, 550,
    "", "!Soul Potion!",
    ""},

   {
    MORTAL, VAMP,
    "blood walk", {20, 19, 20, 18, 19},
    spell_blood_walk, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 631 ), 40, 550,
    "", "!blood walk!!",
    ""},

   {
    REMORT, NORM,
    "restoration", {82, 82, 82, 82, 82},
    spell_restoration, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 632 ), 50, 275,
    "", "!restoration!",
    ""},

   {
    REMORT, NORM,
    "infuse", {82, 82, 82, 82, 71},
    spell_infuse, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 633 ), 1000, 550,
    "", "!Infuse SOul!",
    ""},

   {
    REMORT, NORM,
    "combat prowess", {82, 37, 82, 29, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_combat_prowess, SLOT( 0 ), 0, 0,
    "", "!Combat Prowess!",
    ""},

   {
    REMORT, NORM,
    "quickstrike", {82, 58, 49, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_quickstrike, SLOT( 0 ), 0, 0,
    "", "!Quickstrike!",
    ""},

   {
    MORTAL, WOLF,
    "scent", {5, 1, 1, 1, 1},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_scent, SLOT( 0 ), 0, 275,
    "", "",
    ""},

   {
    REMORT, NORM,
    "holy light", {82, 82, 82, 43, 82},
    spell_holy_light, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 634 ), 75, 275,
    "Holy Light", "!Holy Light!",
    ""},

   {
    REMORT, NORM,
    "target", {82, 82, 82, 20, 82},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_target, SLOT( 0 ), 0, 275,
    "target", "!target",
    ""},

   {
    REMORT, NORM,
    "charge", {82, 82, 82, 65, 82},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_charge, SLOT( 0 ), 0, 275,
    "Charge", "!Charge",
    ""},

   {
    REMORT, NORM,
    "scout", {82, 82, 26, 45, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_scout, SLOT( 0 ), 0, 0,
    "", "",
    ""},

   {
    MORTAL, NORM,
    "mount", {82, 82, 82, 20, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_mount, SLOT( 0 ), 0, 0,
    "", "",
    ""},

   {
    REMORT, NORM,
    "divine intervention", {82, 82, 82, 70, 82},
    spell_divine_intervention, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 635 ), 150, 275,
    "", "!Intervention!",
    ""},

   {
    REMORT, NORM,
    "holy armor", {82, 82, 82, 30, 82},
    spell_holy_armor, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 636 ), 100, 275,
    "", "Your armor is no longer blessed.",
    ""},

   {
    REMORT, NORM,
    "unit tactics", {82, 82, 82, 16, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_unit_tactics, SLOT( 0 ), 0, 275,
    "Unit Tactics", "!Unit Tactics!",
    ""},

   {
    REMORT, NORM,
    "earth elemental", {52, 82, 82, 82, 82},
    spell_earthelem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 637 ), 500, 275,
    "", "!EARTH ELEMENTAL!",
    ""},

   {
    REMORT, NORM,
    "iron golem", {63, 82, 82, 82, 82},
    spell_iron_golem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 638 ), 800, 275,
    "", "!IRON GOLEM!",
    ""},

   {
    REMORT, NORM,
    "diamond golem", {77, 82, 82, 82, 82},
    spell_diamond_golem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 639 ), 1100, 275,
    "", "!DIAMOND GOLEM!",
    ""},

   {

    REMORT, NORM,
    "soul thief", {82, 82, 82, 82, 67},
    spell_soul_thief, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 640 ), 900, 275,
    "", "!SOUL THIEF!",
    ""},
   {

    REMORT, NORM,
    "holy avenger", {82, 82, 82, 76, 82},
    spell_holy_avenger, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 641 ), 1200, 275,
    "", "!HOLY AVENGER!",
    ""},

   {
    REMORT, NORM,
    "heat armor", {57, 78, 82, 82, 82},
    spell_heat_armor, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 642 ), 350, 275,
    "Heat Armor", "!Heat Armor!",
    ""},

   {
    REMORT, NORM,
    "retributive strike", {72, 82, 82, 82, 78},
    spell_retri_strike, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 643 ), 800, 275,
    "Retributive strike", "!Retributive Strike!",
    ""},

   {
    REMORT, NORM,
    "lava burst", {40, 82, 82, 82, 82},
    spell_lava_burst, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 644 ), 350, 275,
    "Lava Burst", "!Lava Burst!",
    ""},

   {
    REMORT, NORM,
    "fireshield", {80, 82, 82, 82, 82},
    spell_fireshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 645 ), 400, 275,
    "", "",
    ""},

   {
    REMORT, NORM,
    "iceshield", {65, 82, 82, 82, 82},
    spell_iceshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 646 ), 350, 275,
    "", "",
    ""},

   {
    REMORT, NORM,
    "shockshield", {55, 82, 82, 82, 82},
    spell_shockshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 647 ), 400, 275,
    "", "",
    ""},

   {
    REMORT, NORM,
    "shadowshield", {82, 82, 82, 82, 82},
    spell_shadowshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 648 ), 400, 275,
    "", "@@NThe @@rshield@@N shatters!!",
    "@@NThe @@rshield@@N protecting $n shatters!!"},

   {
    REMORT, NORM,
    "thoughtshield", {82, 82, 82, 82, 82},
    spell_thoughtshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 649 ), 400, 275,
    "", "@@NThe @@rshield@@N shatters!!",
    "@@NThe @@rshield@@N protecting $n shatters!!"},

   {
    REMORT, NORM,
    "rage:wolven strength", {82, 82, 82, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 650 ), 0, 0,
    "", "",
    ""},

   {
    REMORT, NORM,
    "rage:sharpened claws", {82, 82, 82, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 651 ), 0, 0,
    "", "",
    ""},
   {
    REMORT, NORM,
    "rage:disregard for pain", {82, 82, 82, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 652 ), 0, 0,
    "", "",
    ""},

   {
    REMORT, NORM,
    "enraged", {82, 82, 82, 82, 82},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 653 ), 0, 0,
    "", "",
    ""},

   {
    MORTAL, WOLF,
    "tribal claw", {86, 15, 15, 15, 15},
    spell_tribe_claw, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 655 ), 50, 550,
    "", "",
    ""},

   {
    MORTAL, WOLF,
    "mark", {10, 5, 5, 5, 5},
    spell_wolf_mark, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 656 ), 5, 275,
    "", "",
    ""},

   {
    MORTAL, VAMP,
    "bloodsign", {12, 8, 10, 9, 7},
    spell_blood_sign, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 657 ), 5, 275,
    "", "",
    ""},
	
   {
    MORTAL, VAMP,
    "bloodsense", {10, 3, 3, 3, 3},
    spell_blood_sense, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 658 ), 1, 275,
    "", "",
    ""},

   {

    REMORT, NORM,
    "summon pegasus", {82, 82, 82, 60, 82},
    spell_summon_pegasus, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 659 ), 1200, 275,
    "", "",
    ""},

   {

    REMORT, NORM,
    "summon nightmare", {82, 82, 82, 82, 60},
    spell_summon_nightmare, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 660 ), 1200, 275,
    "", "",
    ""},

   {

    REMORT, NORM,
    "summon beast", {60, 82, 82, 82, 82},
    spell_summon_beast, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 661 ), 1200, 275,
    "", "",
    ""},

   {

    REMORT, NORM,
    "summon devourer", {82, 60, 82, 82, 82},
    spell_summon_devourer, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 662 ), 1200, 275,
    "", "",
    ""},

   {

    REMORT, NORM,
    "summon shadow", {82, 82, 60, 82, 82},
    spell_summon_shadow, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 663 ), 1200, 275,
    "", "",
    ""},

   {
    REMORT, NORM,
    "creature bond", {77, 55, 82, 82, 82},
    spell_creature_bond, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 664 ), 100, 275,
    "", "",
    ""},

   {
    REMORT, NORM,
    "corrupt bond", {82, 82, 82, 82, 33},
    spell_corrupt_bond, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 665 ), 100, 275,
    "", "",
    ""},

   {
    MORTAL, NORM,
    "decapitate", {85, 85, 85, 85, 85},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_decapitate, SLOT( 0 ), 0, 0,
    "", "!decapitate!",
    ""},

   {
    REMORT, NORM,
    "fire blast", {34, 82, 82, 82, 55},
    spell_fireblast, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 666 ), 80, 275,
    "Fire Blast", "!Fire Blast!",
    ""},
   {
    REMORT, NORM,
    "shock storm", {42, 82, 82, 82, 68},
    spell_shockstorm, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 667 ), 100, 275,
    "Shock Storm", "!Shock Storm!",
    ""},
   {
    REMORT, NORM,
    "cone of cold", {56, 82, 82, 82, 82},
    spell_cone_cold, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 668 ), 100, 275,
    "Cone of Cold", "!Cone of Cold!",
    ""},
   {
    REMORT, NORM,
    "holy wrath", {82, 82, 82, 80, 82},
    spell_holy_wrath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 669 ), 140, 275,
    "Holy Wrath", "!Holy Wrath!",
    ""},
   {
    REMORT, NORM,
    "wraith touch", {82, 82, 82, 82, 46},
    spell_wraith_touch, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 670 ), 120, 275,
    "Wraith Touch", "!Wraith Touch!",
    ""},

   {
    REMORT, NORM,
    "thought vise", {82, 70, 82, 82, 82},
    spell_thought_vise, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 670 ), 200, 275,
    "Thought Vise", "!Thought Vise!",
    ""},

   {
    REMORT, NORM,
    "black curse", {82, 82, 82, 82, 65},
    spell_black_curse, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 671 ), 120, 275,
    "Black Curse", "!Black Curse!",
    ""},

   {
    REMORT, NORM,
    "cloak:misery", {82, 82, 80, 82, 73},
    spell_cloak_misery, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 672 ), 550, 275,
    "", "@@NThe @@Rcloak@@N around your body fades.",
    "@@NThe @@Rcloak@@N around $n's body fades."},

   {
    REMORT, NORM,
    "poison:quinine", {82, 82, 36, 82, 82},
    spell_poison_quinine, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 673 ), 80, 125,
    "Poison Dart", "@@N$n feels much better!",
    ""},

   {
    REMORT, NORM,
    "poison:arsenic", {82, 82, 28, 82, 82},
    spell_poison_arsenic, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 674 ), 80, 125,
    "Poison Dart", "@@N$nFeels much better!",
    ""},

   {
    REMORT, NORM,
    "sonic blast", {82, 80, 82, 82, 82},
    spell_sonic_blast, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 675 ), 140, 275,
    "Sonic Blast", "@@N$nFeels much better!",
    ""},

   {
    MORTAL, NORM,
    "mystical potency", {40, 82, 82, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_potency, SLOT( 0 ), 0, 0,
    "", "!Enhanced Damage!",
    ""},

   {
    REMORT, NORM,
    "thaumatergy", {26, 82, 82, 82, 82},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_thaumatergy, SLOT( 0 ), 0, 0,
    "", "!Enhanced Damage!",
    ""},

   {
    MORTAL, NORM,
    "mystical focus", {55, 82, 82, 82, 82},
    spell_mystical_focus, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 676 ), 200, 275,
    "", "Your lose your mystical focus.",
    "$N shakes his head, and seems less focused."},

   {
    MORTAL, NORM,
    "redemption", {82, 15, 82, 82, 82},
    spell_redemption, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 678 ), 200, 275,
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
