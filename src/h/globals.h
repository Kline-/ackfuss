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
 *                                                                         *
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

#include "includes.h"

#define DEC_GLOBALS_H

#ifndef DEC_TYPEDEFS_H
#include "typedefs.h"
#endif

#ifndef DEC_ACK_H
#include "ack.h"
#endif

/*
 * These are skill_lookup return values for common skills and spells.
 */
extern short gsn_martial_arts;  /* For unarmed fighting */
extern short gsn_stake;
extern short gsn_dualwield;
extern short gsn_stun;
extern short gsn_feed;
extern short gsn_shadow;
extern short gsn_appraise;
extern short gsn_climb;
extern short gsn_find_doors;
extern short gsn_smash;
extern short gsn_notrip;
extern short gsn_nodisarm;
extern short gsn_trip;
extern short gsn_dirt;
extern short gsn_bash;
extern short gsn_shield_block;
extern short gsn_berserk;

extern short gsn_circle;
extern short gsn_backstab;
extern short gsn_dodge;
extern short gsn_hide;
extern short gsn_hunt;
extern short gsn_peek;
extern short gsn_pick_lock;
extern short gsn_sneak;
extern short gsn_warcry;
extern short gsn_steal;

extern short gsn_punch;
extern short gsn_headbutt;
extern short gsn_knee;

extern short gsn_disarm;
extern short gsn_enhanced_damage;
extern short gsn_kick;
extern short gsn_parry;
extern short gsn_rescue;
extern short gsn_enhanced_reflexes;
extern short gsn_sleight_of_hand;
extern short gsn_crushing_blow;
extern short gsn_combat_prowess;
extern short gsn_quickstrike;

extern short gsn_blindness;
extern short gsn_charm_person;
extern short gsn_curse;
extern short gsn_invis;
extern short gsn_mass_invis;
extern short gsn_poison;
extern short gsn_sleep;
extern short gsn_disguise;
extern short gsn_instruct;
extern short gsn_teach;
extern short gsn_adrenaline;
extern short gsn_frenzy;
extern short gsn_emotion_control;
extern short gsn_target;
extern short gsn_charge;
extern short gsn_unit_tactics;
extern short gsn_mount;   /* for mount skill */
extern short gsn_imprint;
extern short gsn_scent;
extern short gsn_scout;
extern short gsn_decapitate;
extern short gsn_potency;
extern short gsn_thaumatergy;

extern bool booting_up;
extern bool area_resetting_global;
extern char *const dir_name[];
extern char *const rev_name[];

/*
 * Global constants.
 */
extern const struct str_app_type str_app[26];
extern const struct int_app_type int_app[26];
extern const struct wis_app_type wis_app[26];
extern const struct dex_app_type dex_app[26];
extern const struct con_app_type con_app[26];

extern const struct color_type color_table[MAX_COLOR];
extern const struct ansi_type ansi_table[MAX_ANSI];
extern const struct class_type class_table[MAX_CLASS];
extern const struct class_type remort_table[MAX_CLASS];
extern const struct race_type race_table[MAX_RACE];
extern const struct clan_type clan_table[MAX_CLAN];
extern const struct cmd_type cmd_table[];
extern const struct liq_type liq_table[LIQ_MAX];
extern const struct skill_type skill_table[MAX_SKILL];
extern SOCIAL_TYPE *social_table;
extern char *const title_table[MAX_CLASS][36][2];

/* spec: log all calls to getmem/dispose when set */
extern bool mem_log;
extern const struct stance_app_type stance_app[MAX_STANCE];

/*
 * Global variables.
 */
extern char str_empty[1];
extern char bug_buf[2 * MAX_INPUT_LENGTH];
extern time_t current_time;
extern bool fLogAll;
extern FILE *fpReserve;
extern KILL_DATA kill_table[];
extern char log_buf[2 * MAX_INPUT_LENGTH];
extern char testerbuf[MSL];
extern TIME_INFO_DATA time_info;
extern WEATHER_DATA weather_info;
extern DESCRIPTOR_DATA *descriptor_list;
extern ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];
extern OBJ_INDEX_DATA *obj_index_hash[MAX_KEY_HASH];
extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
extern SYS_DATA_TYPE sysdata;
extern MUDINFO mudinfo;
extern CHAN_HISTORY chan_history;

/* YUCK! */
extern char *target_name;
