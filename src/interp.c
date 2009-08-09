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

#ifndef DEC_ACT_CLAN_H
#include "h/act_clan.h"
#endif

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
#endif

#ifndef DEC_ACT_INFO_H
#include "h/act_info.h"
#endif

#ifndef DEC_ACT_MOVE_H
#include "h/act_move.h"
#endif

#ifndef DEC_ACT_OBJ_H
#include "h/act_obj.h"
#endif

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_BOARD_H
#include "h/board.h"
#endif

#ifndef DEC_BUILD_H
#include "h/build.h"
#endif

#ifndef DEC_BUILDARE_H
#include "h/buildare.h"
#endif

#ifndef DEC_CLUTCH_H
#include "h/clutch.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_EMAIL_H
#include "h/email.h"
#endif

#ifndef DEC_ENCHANT_H
#include "h/enchant.h"
#endif

#ifndef DEC_FIGHT_H
#include "h/fight.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_HUNT_H
#include "h/hunt.h"
#endif

#ifndef DEC_INTERP_H
#include "h/interp.h"
#endif

#ifndef DEC_MAGIC_H
#include "h/magic.h"
#endif

#ifndef DEC_MAPPER_H
#include "h/mapper.h"
#endif

#ifndef DEC_MONEY_H
#include "h/money.h"
#endif

#ifndef DEC_MOUNT_H
#include "h/mount.h"
#endif

#ifndef DEC_MUDINFO_H
#include "h/mudinfo.h"
#endif

#ifndef DEC_PDELETE_H
#include "h/pdelete.h"
#endif

#ifndef DEC_QUEST_H
#include "h/quest.h"
#endif

#ifndef DEC_RULERS_H
#include "h/rulers.h"
#endif

#ifndef DEC_SCHECK_H
#include "h/scheck.h"
#endif

#ifndef DEC_SOCIAL_EDIT_H
#include "h/social_edit.h"
#endif

#ifndef DEC_SPENDQP_H
#include "h/spendqp.h"
#endif

#ifndef DEC_SYSDATA_H
#include "h/sysdata.h"
#endif

#ifndef DEC_VAMPIRE_H
#include "h/vampire.h"
#endif

#ifndef DEC_WEREWOLF_H
#include "h/werewolf.h"
#endif

#ifndef DEC_WIZUTIL_H
#include "h/wizutil.h"
#endif

#ifndef DEC_WRITE_H
#include "h/write.h"
#endif

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
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"east", do_east, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"south", do_south, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"west", do_west, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"up", do_up, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"down", do_down, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"map", do_mapper, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},

   /*
    * Common other commands.
    * Placed here so one and two letter abbreviations work.
    */
   {"buy", do_buy, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"cast", do_cast, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"exits", do_exits, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"get", do_get, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"gain", do_gain, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"inventory", do_inventory, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,true},
   {"kill", do_kill, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"look", do_look, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"order", do_order, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"rest", do_rest, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"sleep", do_sleep, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"stand", do_stand, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"tell", do_tell, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"whisper", do_whisper, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"wield", do_wear, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"wizhelp", do_wizhelp, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"loot", do_loot, POS_STANDING, 20, LOG_ALWAYS,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},

   /*
    * Informational commands.
    */
   {"abandon", do_abandon, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"affected", do_affected, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"areas", do_areas, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"appraise", do_appraise, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,false},
   {"bug", do_bug, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS,true},
   {"clist", do_clan_list, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"commands", do_commands, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"compare", do_compare, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"consider", do_consider, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"credits", do_credits, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"cwhere", do_cwhere, POS_DEAD, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"delete", do_delete, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"diagnose", do_diagnose, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"equipment", do_equipment, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"examine", do_examine, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"help", do_help, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"shelp", do_shelp, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"heal", do_heal, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"idea", do_idea, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS,true},
   {"note", do_note, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS,false},
   {"report", do_report, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"pagelength", do_pagelen, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"read", do_read, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"rlist", do_race_list, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"score", do_score, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"slist", do_slist, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"socials", do_socials, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"status", do_status, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"time", do_time, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"typo", do_typo, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS,true},
   {"weather", do_weather, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"who", do_who, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"whois", do_whois, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"wizlist", do_wizlist, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"write", do_write, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"edit", do_edit, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},

   /*
    * Configuration commands.
    */
   {"accept", do_accept, POS_DEAD, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"alias", do_alias, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"autobrief", do_autobrief, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"autodig", do_autodig, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"autoexit", do_autoexit, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"autoloot", do_autoloot, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"autosac", do_autosac, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"banish", do_banish, POS_DEAD, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"blank", do_blank, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"brief", do_brief, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"channels", do_channels, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"color", do_color, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"colist", do_colist, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG,
    C_SHOW_ALWAYS,true},
   {"combine", do_combine, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"config", do_config, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"description", do_description, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,false},
   {"ignore", do_ignore, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"password", do_password, POS_DEAD, 0, LOG_NEVER,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"prompt", do_prompt, POS_DEAD, 0, LOG_ALWAYS,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"title", do_title, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"wimpy", do_wimpy, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},
   {"worth", do_worth, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"stance", do_stance, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"email", do_email, POS_RESTING, 20, LOG_NORMAL,
    C_TYPE_CONFIG, C_SHOW_ALWAYS,true},

   /*
    * Communication commands.
    */
   {"answer", do_answer, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"ask", do_ask, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"beep", do_beep, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM,
    C_SHOW_ALWAYS,true},
   {"clan", do_clan, POS_RESTING, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"creator", do_creator, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {")", do_creator, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   { ".",              do_gossip,      POS_RESTING,     0,  LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   { "gossip", do_gossip, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"pemote", do_pemote, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"emote", do_emote, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {",", do_emote, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"flame", do_flame, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"finger", do_finger, POS_DEAD, 86, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"gtell", do_gtell, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {";", do_gtell, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"music", do_music, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"newbie", do_newbie, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"pose", do_pose, POS_RESTING, 86, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,false},
/* one or more of the poses crash us  */
   {"pray", do_pray, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"quest", do_quest, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"question", do_question, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"race", do_race, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"reply", do_reply, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"say", do_say, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"'", do_say, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"shout", do_shout, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"tongue", do_tongue, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"yell", do_yell, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"zzz", do_zzz, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"game", do_game, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"vamp", do_familytalk, POS_RESTING, VAMP_ONLY, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"remort", do_remorttalk, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"{", do_remorttalk, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"diplomat", do_diptalk, POS_RESTING, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"crusade", do_crusade, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"adept", do_adepttalk, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"ooc", do_ooc, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},

   /*
    * Combat commands.
    */
   {"assist", do_assist, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"backstab", do_backstab, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"bs", do_backstab, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"dirt", do_dirt, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"trip", do_trip, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"smash", do_smash, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"bash", do_bash, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"berserk", do_berserk, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"circle", do_circle, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"disarm", do_disarm, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"feed", do_feed, POS_FIGHTING, VAMP_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"flee", do_flee, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"headbutt", do_headbutt, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"kick", do_kick, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"knee", do_knee, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"murde", do_murde, POS_FIGHTING, 5, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_NEVER,false},
   {"murder", do_murder, POS_FIGHTING, 5, LOG_ALWAYS,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"punch", do_punch, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"rescue", do_rescue, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"stake", do_stake, POS_STANDING, 5, LOG_ALWAYS,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"stun", do_stun, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"frenzy", do_frenzy, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"charge", do_charge, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"target", do_target, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},

   /*
    * Object manipulation commands.
    */
   {"adapt", do_adapt, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"auction", do_auction, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"bank", do_bank, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"bid", do_bid, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"brandish", do_brandish, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"cdonate", do_cdonate, POS_RESTING, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"close", do_close, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"clutch", do_clutch, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"donate", do_donate, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"drink", do_drink, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"drop", do_drop, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"eat", do_eat, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"enter", do_enter, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"fill", do_fill, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"give", do_give, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"hold", do_wear, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"list", do_list, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"lock", do_lock, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"open", do_open, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"make", do_make, POS_DEAD, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"pick", do_pick, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"put", do_put, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"quaff", do_quaff, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"recite", do_recite, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"remove", do_remove, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"sedit", do_sedit, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_CONFIG, C_SHOW_NEVER,false},
   {"sell", do_sell, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"take", do_get, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"sacrifice", do_sacrifice, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"unlock", do_unlock, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"value", do_value, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"wear", do_wear, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"zap", do_zap, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"enchant", do_enchant, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"assemble", do_connect, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_OBJECT, C_SHOW_ALWAYS,false},
   {"afk", do_afk, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS,true},
   {"assassinate", do_assassinate, POS_STANDING, 20, LOG_ALWAYS,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"finger", do_finger, POS_DEAD, 86, LOG_ALWAYS,
    C_TYPE_MISC, C_SHOW_ALWAYS,true},
   {"follow", do_follow, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"gold", do_gold, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"group", do_group, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"hide", do_hide, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"hunt", do_hunt, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"dismount", do_dismount, POS_STANDING, 1, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"mount", do_mount, POS_STANDING, 1, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"leav", do_leav, POS_RESTING, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_NEVER,false},
   {"leave", do_leave, POS_RESTING, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"practice", do_practice, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"qui", do_qui, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_NEVER,true},
   {"quit", do_quit, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS,true},
   {"recall", do_recall, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"/", do_recall, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"home", do_clan_recall, POS_STANDING, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"save", do_save, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS,true},
   {"scan", do_scan, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"shadowform", do_shadowform, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"sleep", do_sleep, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"sneak", do_sneak, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"spells", do_spells, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"split", do_split, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"steal", do_steal, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"train", do_train, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"visible", do_visible, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS,true},
   {"wake", do_wake, POS_SLEEPING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,true},
   {"warcry", do_warcry, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"where", do_where, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"disguise", do_disguise, POS_STANDING, 0, LOG_ALWAYS,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"politics", do_politics, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"rulers", do_rulers, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},

   /*
    * AckFUSS additions -Kline
    */
   {"stop", do_stop, POS_FIGHTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"statraise", do_statraise, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"census", do_census, POS_STANDING, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"mquest", do_mquest, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"qstat", do_qstat, POS_STANDING, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"safe", do_safe, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"repair", do_repair, POS_STANDING, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,false},
   {"disable", do_disable, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,false},
   {"mudinfo", do_mudinfo, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"lua", do_lua, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,false},
   {"olua", do_olua, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,false},
   {"rlua", do_rlua, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,false},
   {"ldebug", do_ldebug, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,false},
   {"whitelist", do_whitelist, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,false},
   {"logins", do_logins, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,false},

   /*
    *    Vampyre and REMORT SKILLS Zen
    */
   {"family", do_family, POS_RESTING, VAMP_ONLY, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"disguise", do_disguise, POS_STANDING, 0, LOG_ALWAYS,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"instruct", do_instruct, POS_STANDING, VAMP_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"scout", do_scout, POS_STANDING, 1, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,true},

   /*
    * NEW CLAN COMMANDS Zen
    */
   {"ctoggle", do_ctoggle, POS_RESTING, BOSS_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"negotiate", do_negotiate, POS_RESTING, CLAN_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_ALWAYS,false},
   {"council", do_council, POS_RESTING, 0, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_NEVER,false},
   {"qpspend", do_qpspend, POS_STANDING, 1, LOG_NORMAL,
    C_TYPE_MISC, C_SHOW_ALWAYS,false},

   /*
    * Immortal commands.
    */
   {"alist", build_arealist, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"alink", do_alink, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"build", do_build, POS_STANDING, L_HER, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"mpcr", do_mpcr, POS_STANDING, L_DEI, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"halls", do_halls, POS_RESTING, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"lhunt", do_lhunt, POS_DEAD, L_SUP, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"sstat", do_sstat, POS_DEAD, L_SUP, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"cset", do_cset, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"whoname", do_whoname, POS_DEAD, L_ANG, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"sysdata", do_sysdata, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"setclass", do_setclass, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"doggy", do_dog, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"togbuild", do_togbuild, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"togleader", do_togleader, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"isnoop", do_isnoop, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"trust", do_trust, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"deathmatc", do_deathmatc, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_NEVER,true},
   {"deathmatch", do_deathmatch, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"resetpasswd", do_resetpassword, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"iscore", do_iscore, POS_DEAD, L_DEI, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"iwhere", do_iwhere, POS_DEAD, L_DEI, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"fights", do_fights, POS_DEAD, L_DEI, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"allow", do_allow, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"iban", do_ban, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"deny", do_deny, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"freeze", do_freeze, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"reboo", do_reboo, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_NEVER,true},
   {"reboot", do_reboot, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"hotreboo", do_hotreboo, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_NEVER,true},
   {"hotreboot", do_hotreboot, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"shutdow", do_shutdow, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_NEVER,true},
   {"shutdown", do_shutdown, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"users", do_users, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"wizify", do_wizify, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"wizlock", do_wizlock, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"for", do_for, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"force", do_force, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"log", do_log, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"mload", do_mload, POS_DEAD, L_HER, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"monitor", do_monitor, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"areasave", do_areasave, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"mset", do_mset, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"mgive", do_mgive, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"noemote", do_noemote, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"nopray", do_nopray, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"notell", do_notell, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"oload", do_oload, POS_DEAD, L_HER, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"oset", do_oset, POS_DEAD, L_ANG, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"owhere", do_owhere, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"pardon", do_pardon, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"peace", do_peace, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"purge", do_purge, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"iquest", do_iquest, POS_DEAD, L_DEI, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"restore", do_restore, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"rset", do_rset, POS_DEAD, 86, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"silence", do_silence, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"sla", do_sla, POS_DEAD, 86, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"slay", do_slay, POS_DEAD, L_HER, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"snoop", do_snoop, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"sset", do_sset, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"transfer", do_transfer, POS_DEAD, L_HER, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"transdm", do_transdm, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"at", do_at, POS_DEAD, L_DEI, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"bamfin", do_bamfin, POS_DEAD, L_ANG, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"bamfout", do_bamfout, POS_DEAD, L_ANG, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"echo", do_echo, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"goto", do_goto, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"holylight", do_holylight, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"invis", do_invis, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"memory", do_memory, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"mfind", do_mfind, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"mfindlev", do_mfindlev, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"mstat", do_mstat, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"mwhere", do_mwhere, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"ofind", do_ofind, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"ostat", do_ostat, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"recho", do_recho, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"return", do_return, POS_DEAD, L_DEI, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"rstat", do_rstat, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"slookup", do_slookup, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"switch", do_switch, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"immtalk", do_immtalk, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {":", do_immtalk, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"reward", do_reward, POS_DEAD, L_DEI, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"setcouncil", do_togcouncil, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"resetgain", do_gain_stat_reset, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"exlist", do_exlist, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"vlist", do_vlist, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"olmsg", do_olmsg, POS_DEAD, L_HER, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"ofindlev", do_ofindlev, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"prename", do_rename, POS_DEAD, L_GOD, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"pdelete", do_sdelete, POS_DEAD, 2, LOG_ALWAYS,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"scheck", do_scheck, POS_DEAD, L_GOD, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"immbrand", do_immbrand, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"otype", do_otype, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"owear", do_owear, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},
   {"findreset", do_findreset, POS_DEAD, L_HER, LOG_NORMAL,
    C_TYPE_IMM, C_SHOW_ALWAYS,true},

   /*
    * Werewolf commands
    */
   {"howl", do_howl, POS_DEAD, WOLF_ONLY, LOG_NORMAL,
    C_TYPE_COMM, C_SHOW_ALWAYS,true},
   {"tribe", do_tribe, POS_DEAD, WOLF_ONLY, LOG_NORMAL,
    C_TYPE_INFO, C_SHOW_ALWAYS,true},
   {"rage", do_rage, POS_FIGHTING, WOLF_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"imprint", do_imprint, POS_STANDING, WOLF_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},
   {"scent", do_scent, POS_STANDING, WOLF_ONLY, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_SKILL,false},

   /*
    * End of list.
    */
   {"", 0, POS_DEAD, 0, LOG_NORMAL,
    C_TYPE_ACTION, C_SHOW_NEVER,false}
};

/* Log all commands.. rewrite every 5 mins.. */
void comlog( CHAR_DATA * ch, bool cleanup, int cmd, char *args )
{
   static FILE *fplog;
   static time_t ltime;

   if( !fplog || ltime + ( 5 * 60 ) <= current_time )
   {
      if( fplog )
         file_close( fplog );
      if( !( fplog = file_open( "../log/comlog.txt", "w" ) ) )
         return;
      ltime = current_time;
   }

   if( cleanup )
   {
    if( fplog )
     file_close( fplog );
    return;
   }

   fprintf( fplog, "%.24s :: %12.12s (%5d): %s %s\n", ctime( &current_time ),
            ch->name.c_str(), ( IS_NPC( ch ) ? ch->npcdata->pIndexData->vnum : -1 ), cmd_table[cmd].name,
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
   if( !IS_NPC( ch ) && ch->act.test(ACT_FREEZE) )
   {
      send_to_char( "@@a@@fYou're totally frozen!@@N\r\n", ch );
      return;
   }

   if( ch->stun_timer > 0 )
   {
      send_to_char( "You are too@@aSTUNNED@@N to act!\r\n", ch );
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

      if( cmd_table[cmd].level == CLAN_ONLY && !IS_NPC( ch ) && ch->clan == 0 )
         continue;

      if( cmd_table[cmd].level == BOSS_ONLY && !IS_NPC( ch ) && ch->act.test(ACT_CLEADER) )
         continue;

      if( cmd_table[cmd].level == VAMP_ONLY && !IS_NPC( ch ) && !IS_VAMP( ch ) && ( ch->level != L_GOD ) )
         continue;

      if( cmd_table[cmd].level == WOLF_ONLY && !IS_NPC( ch ) && !IS_WOLF( ch ) && ( ch->level != L_GOD ) )
         continue;

      if( command[0] == cmd_table[cmd].name[0]
          && !str_prefix( command, cmd_table[cmd].name ) && ( cmd_table[cmd].level <= trust ) )
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

   if( ( !IS_NPC( ch ) && ch->act.test(ACT_LOG) ) || fLogAll || cmd_table[cmd].log == LOG_ALWAYS )
   {
      snprintf( log_buf, (2 * MIL), "Log %s: %s", ch->name.c_str(), logline );
      log_string( log_buf );
      if( ch->act.test(ACT_LOG) )
         monitor_chan( log_buf, MONITOR_BAD );
      else if( cmd_table[cmd].level > LEVEL_HERO )
         monitor_chan( log_buf, MONITOR_GEN_IMM );
      else
         monitor_chan( log_buf, MONITOR_GEN_MORT );

   }

   if( ch->desc != NULL && ch->desc->snoop_by != NULL )  /* -S- Mod */
   {
      char snp[MAX_STRING_LENGTH];
      snprintf( snp, MSL, "[Snoop:%s] %s\r\n", ch->name.c_str(), logline );
      write_to_buffer( ch->desc->snoop_by, snp );
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
            snprintf( foo, MSL, "~%s %s", ch->pcdata->alias[cnt], argument );
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
         send_to_char( "Huh?\r\n", ch );
      return;
   }

   if( check_disabled(&cmd_table[cmd]) )
   {
    send_to_char("This command has been temporarily disabled.\r\n",ch);
    return;
   }
   /*
    * Character not in position for command?
    */
   if( IS_GHOST(ch) && cmd_table[cmd].ghost_cmd == false )
   {
    send_to_char("Not while you're @@R@@fdead@@N!\r\n",ch);
    return;
   }

   if( ch->position < cmd_table[cmd].position )
   {
      switch ( ch->position )
      {
         case POS_DEAD:
            send_to_char( "Lie still; you are @@dDEAD@@N.\r\n", ch );
            break;

         case POS_MORTAL:
         case POS_INCAP:
            send_to_char( "You are @@Rhurt@@N far too bad for that.\r\n", ch );
            break;

         case POS_STUNNED:
            send_to_char( "You are too @@estunned@@N to do that.\r\n", ch );
            break;

         case POS_SLEEPING:
            send_to_char( "Oh, go back to @@Wsleep!@@N\r\n", ch );
            break;

         case POS_RESTING:
            send_to_char( "Naaaaaah... You feel too @@brelaxed@@N...\r\n", ch );
            break;

         case POS_FIGHTING:
            send_to_char( "Not until you @@Rstop@@N fighting!\r\n", ch );
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
      send_to_char( "You step out of the shadows.\r\n", ch );
      ch->stance = STANCE_WARRIOR;
      ch->stance_ac_mod = 0;
      ch->stance_dr_mod = 0;
      ch->stance_hr_mod = 0;
      act( "$n steps out of the Shadows!", ch, NULL, NULL, TO_ROOM );
   }

   /* Check for movement */
   if( !IS_NPC(ch) )
   {
    if( !str_cmp(cmd_table[cmd].name,"north") || !str_cmp(cmd_table[cmd].name,"east") || !str_cmp(cmd_table[cmd].name,"south") || !str_cmp(cmd_table[cmd].name,"west") || !str_cmp(cmd_table[cmd].name,"up") || !str_cmp(cmd_table[cmd].name,"down") )
     ch->pcdata->movement++;
    else
     ch->pcdata->movement = 0;
   }

   comlog( ch, false, cmd, argument );
   ( *cmd_table[cmd].do_fun ) ( ch, argument );

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

   if( !IS_NPC( ch ) && ch->act.test(ACT_NO_EMOTE) )
   {
      send_to_char( "You are anti-social!\r\n", ch );
      return TRUE;
   }

   if( IS_GHOST(ch) )
   {
    send_to_char("Your ghostly form can't seem to convery emotions...\r\n",ch);
    return true;
   }

   switch ( ch->position )
   {
      case POS_DEAD:
         send_to_char( "Lie still; you are @@dDEAD@@N.\r\n", ch );
         return TRUE;

      case POS_INCAP:
      case POS_MORTAL:
         send_to_char( "You are @@Rhurt@@N far too bad for that.\r\n", ch );
         return TRUE;

      case POS_STUNNED:
         send_to_char( "You are too @@estunned@@N to do that.\r\n", ch );
         return TRUE;

      case POS_SLEEPING:
         /*
          * I just know this is the path to a 12" 'if' statement.  :(
          * But two players asked for it already!  -- Furey
          */
         if( !str_cmp( social_table[cmd].name, "snore" ) )
            break;
         send_to_char( "In your @@Wdreams@@N, or what?\r\n", ch );
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
      send_to_char( "They aren't here.\r\n", ch );
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

/*
 * Check if that command is disabled
 * Note that we check for equivalence of the do_fun pointers; this means
 * that disabling 'chat' will also disable the '.' command
 */
bool check_disabled( const struct cmd_type *command )
{
 DISABLED_DATA *p;
 list<DISABLED_DATA *>::iterator li;

 for( li = disabled_list.begin(); li != disabled_list.end(); li++ )
 {
  p = *li;
  if( p->command->do_fun == command->do_fun )
   return true;
 }

 return false;
}
