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
#include <time.h>
#include "globals.h"

#ifndef DEC_MAGIC_H
#include "magic.h"
#endif


bool check_social args( ( CHAR_DATA * ch, char *command, char *argument ) );
bool MP_Commands args( ( CHAR_DATA * ch ) );

/*
 * Command logging types.
 */
#define LOG_NORMAL      0
#define LOG_ALWAYS      1
#define LOG_NEVER       2

/*
 * Log-all switch.
 */
bool fLogAll = FALSE;



/*
 * Command table.
 */
const struct cmd_type cmd_table[] = {
   /*
    * Common movement commands.
    */



   /*
    * command table modified by Aeria
    */
   {"north", do_north, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"east", do_east, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"south", do_south, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"west", do_west, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"up", do_up, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"down", do_down, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"map", do_mapper, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},



   /*
    * Common other commands.
    * Placed here so one and two letter abbreviations work.
    */
   {"buy", do_buy, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"cast", do_cast, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"exits", do_exits, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"get", do_get, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"gain", do_gain, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"inventory", do_inventory, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"kill", do_kill, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"look", do_look, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"order", do_order, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"rest", do_rest, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"sleep", do_sleep, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"stand", do_stand, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"tell", do_tell, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"whisper", do_whisper, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"wield", do_wear, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"wizhelp", do_wizhelp, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"loot", do_loot, POS_STANDING, 20, LOG_ALWAYS,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},


   /*
    * Informational commands.
    */


   {"test", do_test, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},

   {"affected", do_affected, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"areas", do_areas, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"appraise", do_appraise, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"bug", do_bug, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS},
   {"clist", do_clan_list, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"commands", do_commands, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"compare", do_compare, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"consider", do_consider, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"credits", do_credits, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"cwhere", do_cwhere, POS_DEAD, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"delete", do_delete, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"diagnose", do_diagnose, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"equipment", do_equipment, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"examine", do_examine, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"help", do_help, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"shelp", do_shelp, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"heal", do_heal, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"idea", do_idea, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS},

   {"note", do_note, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS},

   {"report", do_report, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"pagelength", do_pagelen, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"players", do_players, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"read", do_read, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"rlist", do_race_list, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"score", do_score, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"slist", do_slist, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"socials", do_socials, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"status", do_status, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"time", do_time, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"typo", do_typo, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS},
   {"weather", do_weather, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"who", do_who, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"whois", do_whois, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"wizlist", do_wizlist, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"write", do_write, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"edit", do_edit, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},



   /*
    * Configuration commands.
    */



   {"accept", do_accept, POS_DEAD, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"alias", do_alias, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"auto", do_auto, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"autoexit", do_autoexit, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"autoloot", do_autoloot, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"autosac", do_autosac, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"banish", do_banish, POS_DEAD, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"blank", do_blank, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"brief", do_brief, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"channels", do_channels, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"color", do_color, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"colist", do_colist, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG,
    C_SHOW_ALWAYS},
   {"combine", do_combine, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"config", do_config, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"description", do_description, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"ignore", do_ignore, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"password", do_password, POS_DEAD, 0, LOG_NEVER,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"prompt", do_prompt, POS_DEAD, 0, LOG_ALWAYS,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"title", do_title, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"wimpy", do_wimpy, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},
   {"worth", do_worth, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"stance", do_stance, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"email", do_email, POS_RESTING, 20, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS},

   /*
    * Communication commands.
    */


   {"answer", do_answer, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"ask", do_ask, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"beep", do_beep, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM,
    C_SHOW_ALWAYS},
   {"clan", do_clan, POS_RESTING, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"creator", do_creator, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {")", do_creator, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
/*    { ".",              do_gossip,      POS_RESTING,     0,  LOG_NORMAL,
      C_TYPE_COMM, C_SHOW_ALWAYS }, */
   {"pemote", do_pemote, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"emote", do_emote, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {",", do_emote, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"flame", do_flame, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"finger", do_finger, POS_DEAD, 86, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"gtell", do_gtell, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {";", do_gtell, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"music", do_music, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"newbie", do_newbie, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"pose", do_pose, POS_RESTING, 86, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
/* one or more of the poses crash us  */
   {"pray", do_pray, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"quest", do_quest2, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"question", do_question, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"race", do_race, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"reply", do_reply, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"say", do_say, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"'", do_say, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"shout", do_shout, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"tongue", do_tongue, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"yell", do_yell, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"zzz", do_zzz, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"game", do_game, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"vamp", do_familytalk, POS_RESTING, VAMP_ONLY, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"{", do_remorttalk, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"diplomat", do_diptalk, POS_RESTING, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"crusade", do_crusade, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"adept", do_adepttalk, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"ooc", do_ooc, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   /*
    * Combat commands.
    */
   {"assist", do_assist, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"backstab", do_backstab, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"bs", do_backstab, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"dirt", do_dirt, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"trip", do_trip, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"smash", do_smash, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"bash", do_bash, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"beserk", do_beserk, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"circle", do_circle, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"disarm", do_disarm, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"feed", do_feed, POS_FIGHTING, VAMP_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"flee", do_flee, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"headbutt", do_headbutt, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"kick", do_kick, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"knee", do_knee, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"murde", do_murde, POS_FIGHTING, 5, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_NEVER},
   {"murder", do_murder, POS_FIGHTING, 5, LOG_ALWAYS,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"punch", do_punch, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"rescue", do_rescue, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"stake", do_stake, POS_STANDING, 5, LOG_ALWAYS,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"stun", do_stun, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"frenzy", do_frenzy, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"charge", do_charge, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"target", do_target, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},



   /*
    * Object manipulation commands.
    */


   {"adapt", do_adapt, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"auction", do_auction, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"bank", do_bank, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"bid", do_bid, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"brandish", do_brandish, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"cdonate", do_cdonate, POS_RESTING, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"close", do_close, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"clutch", do_clutch, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"donate", do_donate, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"drink", do_drink, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"drop", do_drop, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"eat", do_eat, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"enter", do_enter, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"fill", do_fill, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"give", do_give, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"hold", do_wear, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"list", do_list, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"lock", do_lock, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"open", do_open, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},

   {"make", do_make, POS_DEAD, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"pick", do_pick, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"put", do_put, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"quaff", do_quaff, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"recite", do_recite, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"remove", do_remove, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"sedit", do_sedit, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_CONFIG, C_SHOW_NEVER},
   {"sell", do_sell, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"take", do_get, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"sacrifice", do_sacrifice, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"unlock", do_unlock, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"value", do_value, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"wear", do_wear, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"zap", do_zap, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"enchant", do_enchant, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},
   {"assemble", do_connect, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS},



   {"afk", do_afk, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS},
   {"assassinate", do_assassinate, POS_STANDING, 20, LOG_ALWAYS,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"finger", do_finger, POS_DEAD, 86, LOG_ALWAYS,
    C_TYPE_MISC, C_SHOW_ALWAYS},
   {"follow", do_follow, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"gold", do_gold, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"group", do_group, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"guild", do_guild, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_NEVER},
   {"hide", do_hide, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"hunt", do_hunt, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"dismount", do_dismount, POS_STANDING, 1, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"mount", do_mount, POS_STANDING, 1, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"leav", do_leav, POS_RESTING, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_NEVER},
   {"leave", do_leave, POS_RESTING, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},

   {"practice", do_practice, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"qui", do_qui, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_NEVER},
   {"quit", do_quit, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS},
   {"recall", do_recall, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"/", do_recall, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"home", do_clan_recall, POS_STANDING, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"rent", do_rent, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_NEVER},
   {"save", do_save, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS},
   {"scan", do_scan, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"shadowform", do_shadowform, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"sleep", do_sleep, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"sneak", do_sneak, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"spells", do_spells, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"split", do_split, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"steal", do_steal, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"train", do_train, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"visible", do_visible, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS},
   {"wake", do_wake, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"where", do_where, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},

   {"disguise", do_disguise, POS_STANDING, 0, LOG_ALWAYS,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"politics", do_politics, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"rulers", do_rulers, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},


   /*
    *    Vampyre and REMORT SKILLS Zen
    */



   {"family", do_family, POS_RESTING, VAMP_ONLY, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"disguise", do_disguise, POS_STANDING, 0, LOG_ALWAYS,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"instruct", do_instruct, POS_STANDING, VAMP_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"scout", do_scout, POS_STANDING, 1, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},


   /*
    * NEW CLAN COMMANDS Zen
    */


   {"ctoggle", do_ctoggle, POS_RESTING, BOSS_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"negotiate", do_negotiate, POS_RESTING, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS},
   {"council", do_council, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_NEVER},
   {"qpspend", do_qpspend, POS_STANDING, 1, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS},

   /*
    * Immortal commands.
    */


   {"alist", build_arealist, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"alink", do_alink, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"build", do_build, POS_STANDING, 2, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"mpcr", do_mpcr, POS_STANDING, L_DEI, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"mpstat", do_mpstat, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"halls", do_halls, POS_RESTING, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"lhunt", do_lhunt, POS_DEAD, L_SUP, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"sstat", do_sstat, POS_DEAD, L_SUP, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"cset", do_cset, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"whoname", do_whoname, POS_DEAD, L_ANG, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"sysdata", do_sysdata, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"clutchinfo", do_clutchinfo, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"advance", do_advance, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"setclass", do_setclass, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"doggy", do_dog, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"togbuild", do_togbuild, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"togleader", do_togleader, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"isnoop", do_isnoop, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"trust", do_trust, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"deathmatc", do_deathmatc, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_NEVER},
   {"deathmatch", do_deathmatch, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"resetpasswd", do_resetpassword, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"iscore", do_iscore, POS_DEAD, L_DEI, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"iwhere", do_iwhere, POS_DEAD, L_DEI, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"fights", do_fights, POS_DEAD, L_DEI, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"allow", do_allow, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"iban", do_ban, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"deny", do_deny, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"freeze", do_freeze, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"reboo", do_reboo, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_NEVER},
   {"reboot", do_reboot, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"hotreboo", do_hotreboo, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_NEVER},
   {"hotreboot", do_hotreboot, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"shutdow", do_shutdow, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_NEVER},
   {"shutdown", do_shutdown, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"users", do_users, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"wizify", do_wizify, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"wizlock", do_wizlock, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"imtlset", do_imtlset, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"for", do_for, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"force", do_force, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"log", do_log, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"mload", do_mload, POS_DEAD, L_HER, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"monitor", do_monitor, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"areasave", do_areasave, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"mset", do_mset, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"mgive", do_mgive, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"noemote", do_noemote, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"nopray", do_nopray, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"notell", do_notell, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"oload", do_oload, POS_DEAD, L_HER, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"oset", do_oset, POS_DEAD, L_ANG, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"owhere", do_owhere, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"pardon", do_pardon, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"peace", do_peace, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"purge", do_purge, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"iquest", do_quest, POS_DEAD, L_DEI, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"restore", do_restore, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"rset", do_rset, POS_DEAD, 86, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"silence", do_silence, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"sla", do_sla, POS_DEAD, 86, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"slay", do_slay, POS_DEAD, L_HER, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"snoop", do_snoop, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"sset", do_sset, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"transfer", do_transfer, POS_DEAD, L_HER, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"transdm", do_transdm, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},

   {"at", do_at, POS_DEAD, L_DEI, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"bamfin", do_bamfin, POS_DEAD, L_ANG, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"bamfout", do_bamfout, POS_DEAD, L_ANG, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"echo", do_echo, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"goto", do_goto, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"holylight", do_holylight, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"invis", do_invis, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"memory", do_memory, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"mfind", do_mfind, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"mfindlev", do_mfindlev, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"mstat", do_mstat, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"mwhere", do_mwhere, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"ofind", do_ofind, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"ostat", do_ostat, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"recho", do_recho, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"return", do_return, POS_DEAD, L_DEI, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"rstat", do_rstat, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"slookup", do_slookup, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"switch", do_switch, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"immtalk", do_immtalk, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {":", do_immtalk, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"reward", do_reward, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"setcouncil", do_togcouncil, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"resetgain", do_gain_stat_reset, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"exlist", do_exlist, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"vlist", do_vlist, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"olmsg", do_olmsg, POS_DEAD, L_HER, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"ofindlev", do_ofindlev, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"prename", do_rename, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"pdelete", do_sdelete, POS_DEAD, 2, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"scheck", do_scheck, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"immbrand", do_immbrand, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"otype", do_otype, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"owear", do_owear, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},
   {"findreset", do_findreset, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS},

   /*
    * Werewolf commands
    */
   {"howl", do_howl, POS_DEAD, WOLF_ONLY, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS},
   {"tribe", do_tribe, POS_DEAD, WOLF_ONLY, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS},
   {"rage", do_rage, POS_FIGHTING, WOLF_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"imprint", do_imprint, POS_STANDING, WOLF_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},
   {"scent", do_scent, POS_STANDING, WOLF_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL},

   /*
    * MOBprogram commands.
    */

   {"mpasound", do_mpasound, POS_DEAD, 90, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER},
   {"mpjunk", do_mpjunk, POS_DEAD, 90, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER},
   {"mpecho", do_mpecho, POS_DEAD, 90, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER},
   {"mpechoat", do_mpechoat, POS_DEAD, 90, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER},
   {"mpechoaround", do_mpechoaround, POS_DEAD, 90, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER},
   {"mpkill", do_mpkill, POS_DEAD, 90, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER},
   {"mpmload", do_mpmload, POS_DEAD, 90, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER},
   {"mpoload", do_mpoload, POS_DEAD, 90, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER},
   {"mppurge", do_mppurge, POS_DEAD, 90, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER},
   {"mpgoto", do_mpgoto, POS_DEAD, 90, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER},
   {"mpat", do_mpat, POS_DEAD, 90, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER},
   {"mptransfer", do_mptransfer, POS_DEAD, 90, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER},
   {"mpforce", do_mpforce, POS_DEAD, 90, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER},
   {"mpforce", do_mpforce, POS_DEAD, 90, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER},

   /*
    * End of list.
    */
   {"", 0, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_NEVER}



   /*
    * Immortal commands.
    */





};





/* Log all commands.. rewrite every 5 mins.. */
void comlog( CHAR_DATA * ch, int cmd, char *args )
{
   static FILE *fplog;
   static time_t ltime;

   if( !fplog || ltime + ( 5 * 60 ) <= current_time )
   {
      if( fplog )
         fclose( fplog );
      if( !( fplog = fopen( "../log/comlog.txt", "w" ) ) )
         return;
      ltime = current_time;
   }
   fprintf( fplog, "%.24s :: %12.12s (%5d): %s %s\n", ctime( &current_time ),
            ch->name, ( IS_NPC( ch ) ? ch->pIndexData->vnum : -1 ), cmd_table[cmd].name,
            ( cmd_table[cmd].log == LOG_NEVER ? "XXX" : args ) );
   fflush( fplog );
}



/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret( CHAR_DATA * ch, char *argument )
{
   /*
    * BUG with aliases: they can call themselves, which is
    * * a Bad Thing.  When an alias calls interp, we'll add
    * * a '~' char as the first char.  Checking for this will
    * * tell us if we need to check aliases again. -S-
    */

   bool alias_call;
   char command[MAX_INPUT_LENGTH];
   char logline[MAX_INPUT_LENGTH];
   int cmd;
   int trust;
   bool found;

   alias_call = FALSE;

   if( ch->position == POS_WRITING )
      /*
       * if player is writing, pass argument straight to write_interpret 
       */
   {
      write_interpret( ch, argument );
      return;
   }

   if( ch->position == POS_BUILDING )
   {
      if( argument[0] == ':' )
         argument++;
      else
      {
         build_interpret( ch, argument );
         return;
      }
   }


   /*
    * Strip leading spaces.
    */
   while( isspace( *argument ) )
      argument++;
   if( argument[0] == '\0' )
      return;


   if( argument[0] == '~' )
   {
      argument++;
      alias_call = TRUE;
   }

   /*
    * No hiding.
    */
   REMOVE_BIT( ch->affected_by, AFF_HIDE );


   /*
    * Implement freeze command.
    */
   if( !IS_NPC( ch ) && IS_SET( ch->act, PLR_FREEZE ) )
   {
      send_to_char( "@@a@@fYou're totally frozen!@@N\n\r", ch );
      return;
   }

   if( ch->stunTimer > 0 )
   {
      send_to_char( "You are too@@aSTUNNED@@N to act!\n\r", ch );
      return;
   }

   /*
    * Grab the command word.
    * Special parsing so ' can be a command,
    *   also no spaces needed after punctuation.
    */
   strcpy( logline, argument );
   if( !isalpha( argument[0] ) && !isdigit( argument[0] ) )
   {
      command[0] = argument[0];
      command[1] = '\0';
      argument++;
      while( isspace( *argument ) )
         argument++;
   }
   else
   {
      argument = one_argument( argument, command );
   }





   /*
    * Look for command in command table.
    */
   found = FALSE;
   trust = get_trust( ch );
   for( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
   {

      /*
       * Stephen Mod:  if level == CLAN_ONLY then for clan member only.
       * == BOSS_ONLY have to be leader.  
       * == -3 vamp
       * == -4 wolf  
       */

      if( cmd_table[cmd].level == CLAN_ONLY && !IS_NPC( ch ) && ch->pcdata->clan == 0 )
         continue;

      if( cmd_table[cmd].level == BOSS_ONLY && !IS_NPC( ch ) && !IS_SET( ch->pcdata->pflags, PFLAG_CLAN_BOSS ) )
         continue;

      if( cmd_table[cmd].level == VAMP_ONLY && !IS_NPC( ch ) && !IS_VAMP( ch ) && ( ch->level != L_GOD ) )
         continue;

      if( cmd_table[cmd].level == WOLF_ONLY && !IS_NPC( ch ) && !IS_WOLF( ch ) && ( ch->level != L_GOD ) )
         continue;



      if( command[0] == cmd_table[cmd].name[0]
          && !str_prefix( command, cmd_table[cmd].name ) && ( cmd_table[cmd].level <= trust || MP_Commands( ch ) ) )
      {



         found = TRUE;
         break;
      }





   }

   /*
    * Log and snoop.
    */
   if( cmd_table[cmd].log == LOG_NEVER )
      strcpy( logline, "XXXXXXXX XXXXXXXX XXXXXXXX@@N" );

   if( ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_LOG ) ) || fLogAll || cmd_table[cmd].log == LOG_ALWAYS )
   {
      sprintf( log_buf, "Log %s: %s", ch->name, logline );
      log_string( log_buf );
      if( IS_SET( ch->act, PLR_LOG ) )
         monitor_chan( log_buf, MONITOR_BAD );
      else if( cmd_table[cmd].level > LEVEL_HERO )
         monitor_chan( log_buf, MONITOR_GEN_IMM );
      else
         monitor_chan( log_buf, MONITOR_GEN_MORT );

   }

   if( ch->desc != NULL && ch->desc->snoop_by != NULL )  /* -S- Mod */
   {
      char snp[MAX_STRING_LENGTH];
      sprintf( snp, "[Snoop:%s] %s\n\r", ch->name, logline );
      write_to_buffer( ch->desc->snoop_by, snp, 0 );
   }

   if( !found && !IS_NPC( ch ) && ( !alias_call ) )
   {
      int cnt;
      char foo[MAX_STRING_LENGTH];
      /*
       * Check aliases -S- 
       */

      for( cnt = 0; cnt < MAX_ALIASES; cnt++ )
      {
         if( !str_cmp( ch->pcdata->alias_name[cnt], command ) && str_cmp( ch->pcdata->alias_name[cnt], "<none>@@N" ) )
         {
            found = TRUE;
            sprintf( foo, "~%s %s", ch->pcdata->alias[cnt], argument );
            interpret( ch, foo );
            return;
         }
      }
   }


   if( !found )
   {
      /*
       * Look for command in socials table.
       */
      if( !check_social( ch, command, argument )
#ifdef IMC
          && !imc_command_hook( ch, command, argument )
#endif
#ifdef I3
          && !I3_command_hook( ch, command, argument )
#endif
          )
         send_to_char( "Huh?\n\r", ch );
      return;
   }

   /*
    * Character not in position for command?
    */
   if( ch->position < cmd_table[cmd].position )
   {
      switch ( ch->position )
      {
         case POS_DEAD:
            send_to_char( "Lie still; you are @@dDEAD@@N.\n\r", ch );
            break;

         case POS_MORTAL:
         case POS_INCAP:
            send_to_char( "You are @@Rhurt@@N far too bad for that.\n\r", ch );
            break;

         case POS_STUNNED:
            send_to_char( "You are too @@estunned@@N to do that.\n\r", ch );
            break;

         case POS_SLEEPING:
            send_to_char( "Oh, go back to @@Wsleep!@@N\n\r", ch );
            break;

         case POS_RESTING:
            send_to_char( "Naaaaaah... You feel too @@brelaxed@@N...\n\r", ch );
            break;

         case POS_FIGHTING:
            send_to_char( "Not until you @@Rstop@@N fighting!\n\r", ch );
            break;

      }
      return;
   }

   /*
    * Dispatch the command.
    */
   if( !IS_NPC( ch )
       && ( ( ch->stance == STANCE_AMBUSH )
            || ( ch->stance == STANCE_AC_BEST ) )
       && ( ( str_prefix( command, "kill" ) )
            && ( str_prefix( command, "murder" ) )
            && ( str_prefix( command, "backstab" ) )
            && ( str_prefix( command, "bs" ) )
            && ( str_prefix( command, "whisper" ) )
            && ( str_prefix( command, "stake" ) ) && ( str_prefix( command, "steal" ) ) ) )


   {
      send_to_char( "You step out of the shadows.\n\r", ch );
      ch->stance = STANCE_WARRIOR;
      ch->stance_ac_mod = 0;
      ch->stance_dr_mod = 0;
      ch->stance_hr_mod = 0;
      act( "$n steps out of the Shadows!", ch, NULL, NULL, TO_ROOM );
   }
   comlog( ch, cmd, argument );
   ( *cmd_table[cmd].do_fun ) ( ch, argument );

   tail_chain(  );
   return;
}



bool check_social( CHAR_DATA * ch, char *command, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int cmd;
   bool found;

   found = FALSE;
   for( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
   {
      if( command[0] == social_table[cmd].name[0] && !str_prefix( command, social_table[cmd].name ) )
      {
         found = TRUE;
         break;
      }
   }

   if( !found )
      return FALSE;

   if( !IS_NPC( ch ) && IS_SET( ch->act, PLR_NO_EMOTE ) )
   {
      send_to_char( "You are anti-social!\n\r", ch );
      return TRUE;
   }

   switch ( ch->position )
   {
      case POS_DEAD:
         send_to_char( "Lie still; you are @@dDEAD@@N.\n\r", ch );
         return TRUE;

      case POS_INCAP:
      case POS_MORTAL:
         send_to_char( "You are @@Rhurt@@N far too bad for that.\n\r", ch );
         return TRUE;

      case POS_STUNNED:
         send_to_char( "You are too @@estunned@@N to do that.\n\r", ch );
         return TRUE;

      case POS_SLEEPING:
         /*
          * I just know this is the path to a 12" 'if' statement.  :(
          * But two players asked for it already!  -- Furey
          */
         if( !str_cmp( social_table[cmd].name, "snore" ) )
            break;
         send_to_char( "In your @@Wdreams@@N, or what?\n\r", ch );
         return TRUE;

   }

   one_argument( argument, arg );
   victim = NULL;
   if( arg[0] == '\0' )
   {
      act( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM );
      act( social_table[cmd].char_no_arg, ch, NULL, victim, TO_CHAR );
   }
   else if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
   }
   else if( victim == ch )
   {
      act( social_table[cmd].others_auto, ch, NULL, victim, TO_ROOM );
      act( social_table[cmd].char_auto, ch, NULL, victim, TO_CHAR );
   }
   else
   {
      act( social_table[cmd].others_found, ch, NULL, victim, TO_NOTVICT );
      act( social_table[cmd].char_found, ch, NULL, victim, TO_CHAR );
      act( social_table[cmd].vict_found, ch, NULL, victim, TO_VICT );

      if( !IS_NPC( ch ) && IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) && IS_AWAKE( victim ) )
      {
         switch ( number_bits( 4 ) )
         {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
               act( social_table[cmd].others_found, victim, NULL, ch, TO_NOTVICT );
               act( social_table[cmd].char_found, victim, NULL, ch, TO_CHAR );
               act( social_table[cmd].vict_found, victim, NULL, ch, TO_VICT );
               break;

            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
               act( "$n slaps $N.", victim, NULL, ch, TO_NOTVICT );
               act( "You slap $N.", victim, NULL, ch, TO_CHAR );
               act( "$n slaps you.", victim, NULL, ch, TO_VICT );
               break;
         }
      }
   }

   return TRUE;
}





bool IS_SWITCHED( CHAR_DATA * ch )
{
   if( !IS_NPC( ch ) )
      return FALSE;

   if( ch->desc == NULL )
      return FALSE;

   return TRUE;

}

bool MP_Commands( CHAR_DATA * ch )  /* Can MOBProged mobs
                                     * use mpcommands? TRUE if yes.
                                     * - Kahn */
{
   if( IS_SWITCHED( ch ) )
      return FALSE;

   if( IS_NPC( ch ) && ch->pIndexData->progtypes && !IS_AFFECTED( ch, AFF_CHARM ) )
      return TRUE;

   return FALSE;
}
