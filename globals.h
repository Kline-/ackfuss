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
#define DEC_GLOBALS_H 1

#ifndef DEC_TYPEDEFS_H
#include "typedefs.h"
#endif

#ifndef DEC_CONFIG_H
#include "config.h"
#endif

#ifndef DEC_ACK_H
#include "ack.h"
#endif

/*
 * These are skill_lookup return values for common skills and spells.
 */
extern sh_int gsn_martial_arts;  /* For unarmed fighting */
extern sh_int gsn_stake;
extern sh_int gsn_dualwield;
extern sh_int gsn_stun;
extern sh_int gsn_feed;
extern sh_int gsn_shadow;
extern sh_int gsn_appraise;
extern sh_int gsn_climb;
extern sh_int gsn_find_doors;
extern sh_int gsn_smash;
extern sh_int gsn_notrip;
extern sh_int gsn_nodisarm;
extern sh_int gsn_trip;
extern sh_int gsn_dirt;
extern sh_int gsn_bash;

extern sh_int gsn_fourth_attack;
extern sh_int gsn_shield_block;
extern sh_int gsn_beserk;

extern sh_int gsn_circle;
extern sh_int gsn_backstab;
extern sh_int gsn_dodge;
extern sh_int gsn_hide;
extern sh_int gsn_hunt;
extern sh_int gsn_peek;
extern sh_int gsn_pick_lock;
extern sh_int gsn_sneak;
extern sh_int gsn_steal;

extern sh_int gsn_punch;
extern sh_int gsn_headbutt;
extern sh_int gsn_knee;

extern sh_int gsn_disarm;
extern sh_int gsn_enhanced_damage;
extern sh_int gsn_kick;
extern sh_int gsn_parry;
extern sh_int gsn_rescue;
extern sh_int gsn_second_attack;
extern sh_int gsn_third_attack;

extern sh_int gsn_blindness;
extern sh_int gsn_charm_person;
extern sh_int gsn_curse;
extern sh_int gsn_invis;
extern sh_int gsn_mass_invis;
extern sh_int gsn_poison;
extern sh_int gsn_sleep;
extern sh_int gsn_disguise;
extern sh_int gsn_instruct;
extern sh_int gsn_teach;
extern sh_int gsn_adrenaline;
extern sh_int gsn_frenzy;
extern sh_int gsn_emotion_control;
extern sh_int gsn_fifth_attack;
extern sh_int gsn_sixth_attack;
extern sh_int gsn_target;
extern sh_int gsn_charge;
extern sh_int gsn_unit_tactics;

extern sh_int gsn_mount;   /* for mount skill */
extern sh_int gsn_imprint;
extern sh_int gsn_scent;
extern sh_int gsn_scout;
extern sh_int gsn_decapitate;
extern sh_int gsn_potency;
extern sh_int gsn_thaumatergy;

#ifdef TFS
extern sh_int gsn_mana_sense;
#endif

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

extern const struct color_type color_table[MAX_color];
extern const struct ansi_type ansi_table[MAX_ANSI];
extern const struct class_type class_table[MAX_CLASS];
extern const struct class_type remort_table[MAX_CLASS];
extern const struct race_type race_table[MAX_RACE];
extern const struct exp_type exp_table[141];
extern const struct clan_type clan_table[MAX_CLAN];
extern const struct cmd_type cmd_table[];
extern const struct liq_type liq_table[LIQ_MAX];
extern const struct skill_type skill_table[MAX_SKILL];
extern struct social_type *social_table;
extern char *const title_table[MAX_CLASS][36][2];

/* spec: log all calls to getmem/dispose when set */
extern bool mem_log;
extern const struct stance_app_type stance_app[MAX_STANCE];

/*
 * Global variables.
 */
extern char bug_buf[];
extern time_t current_time;
extern bool fLogAll;
extern FILE *fpReserve;
extern KILL_DATA kill_table[];
extern char log_buf[];
extern char testerbuf[];
extern TIME_INFO_DATA time_info;
extern WEATHER_DATA weather_info;
extern DESCRIPTOR_DATA *descriptor_list;
extern CHAR_DATA *char_list;
extern ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];
extern OBJ_INDEX_DATA *obj_index_hash[MAX_KEY_HASH];
extern SYS_DATA_TYPE sysdata;

/* YUCK! */
extern char *target_name;

/* board junk */
extern BOARD_DATA *first_board;
extern BOARD_DATA *last_board;
extern BOARD_DATA *board_free;
extern MESSAGE_DATA *message_free;




/*
 * Command functions.
 * Defined in act_*.c (mostly).
 */

/**************************
 * This bunch of commands *
 * added by Stephen ;)    *
 **************************/

DECLARE_DO_FUN( do_test );

DECLARE_DO_FUN( do_exlist );
DECLARE_DO_FUN( do_vlist );
DECLARE_DO_FUN( do_rename );

DECLARE_DO_FUN( do_sdelete );

DECLARE_DO_FUN( do_accept );
DECLARE_DO_FUN( do_adapt );
DECLARE_DO_FUN( do_affected );
DECLARE_DO_FUN( do_afk );
DECLARE_DO_FUN( do_alias );
DECLARE_DO_FUN( do_appraise );
DECLARE_DO_FUN( build_arealist );
DECLARE_DO_FUN( do_ask );
DECLARE_DO_FUN( do_assassinate );
DECLARE_DO_FUN( do_assist );
DECLARE_DO_FUN( do_bamfin );
DECLARE_DO_FUN( do_bamfout );
DECLARE_DO_FUN( do_bank );
DECLARE_DO_FUN( do_banish );
DECLARE_DO_FUN( do_bash );
DECLARE_DO_FUN( do_beep );
DECLARE_DO_FUN( do_beserk );
DECLARE_DO_FUN( do_bid );
DECLARE_DO_FUN( do_rbid );
DECLARE_DO_FUN( do_mbid );
DECLARE_DO_FUN( do_cdonate );
DECLARE_DO_FUN( do_circle );
DECLARE_DO_FUN( do_clan );
DECLARE_DO_FUN( do_clan_list );
DECLARE_DO_FUN( do_clan_recall );
DECLARE_DO_FUN( do_clutch );
DECLARE_DO_FUN( do_clutchinfo );
DECLARE_DO_FUN( do_colist );
DECLARE_DO_FUN( do_color );
DECLARE_DO_FUN( do_creator );
DECLARE_DO_FUN( do_cset );
DECLARE_DO_FUN( do_cwhere );
DECLARE_DO_FUN( do_deathmatc );
DECLARE_DO_FUN( do_deathmatch );
DECLARE_DO_FUN( do_diagnose );
DECLARE_DO_FUN( do_dirt );
DECLARE_DO_FUN( do_dog );
DECLARE_DO_FUN( do_donate );
DECLARE_DO_FUN( do_edit );
DECLARE_DO_FUN( do_enter );
DECLARE_DO_FUN( do_feed );
DECLARE_DO_FUN( do_fights );
DECLARE_DO_FUN( do_finger );
DECLARE_DO_FUN( do_flame );
DECLARE_DO_FUN( do_gain );
DECLARE_DO_FUN( do_gold );
DECLARE_DO_FUN( do_gossip );
DECLARE_DO_FUN( do_ooc );
DECLARE_DO_FUN( do_espanol );
DECLARE_DO_FUN( do_francais );
DECLARE_DO_FUN( do_quest2 );
DECLARE_DO_FUN( do_guild );
DECLARE_DO_FUN( do_halls );
DECLARE_DO_FUN( do_headbutt );
DECLARE_DO_FUN( do_knee );
DECLARE_DO_FUN( do_heal );
DECLARE_DO_FUN( do_hunt );
DECLARE_DO_FUN( do_resetpassword );
DECLARE_DO_FUN( do_iscore );
DECLARE_DO_FUN( do_isnoop );
DECLARE_DO_FUN( do_iwhere );
DECLARE_DO_FUN( do_leav );
DECLARE_DO_FUN( do_leave );
DECLARE_DO_FUN( do_lhunt );
DECLARE_DO_FUN( do_make );
DECLARE_DO_FUN( do_monitor );
DECLARE_DO_FUN( do_mpcr );
DECLARE_DO_FUN( do_music );
DECLARE_DO_FUN( do_newbie );
DECLARE_DO_FUN( do_nopray );
DECLARE_DO_FUN( do_pemote );
DECLARE_DO_FUN( do_players );
DECLARE_DO_FUN( do_pray );
DECLARE_DO_FUN( do_punch );
DECLARE_DO_FUN( do_quest );
DECLARE_DO_FUN( do_race );
DECLARE_DO_FUN( do_race_list );
DECLARE_DO_FUN( do_retrieve );
DECLARE_DO_FUN( do_scan );
DECLARE_DO_FUN( do_setclass );
DECLARE_DO_FUN( do_smash );
DECLARE_DO_FUN( do_shadowform );
DECLARE_DO_FUN( do_shelp );
DECLARE_DO_FUN( do_sstat );
DECLARE_DO_FUN( do_stake );
DECLARE_DO_FUN( do_status );
DECLARE_DO_FUN( do_stun );
DECLARE_DO_FUN( do_togbuild );
DECLARE_DO_FUN( do_togleader );
DECLARE_DO_FUN( do_tongue );
DECLARE_DO_FUN( do_trip );
DECLARE_DO_FUN( do_whisper );
DECLARE_DO_FUN( do_whois );
DECLARE_DO_FUN( do_whoname );
DECLARE_DO_FUN( do_worth );
DECLARE_DO_FUN( do_zzz );
DECLARE_DO_FUN( do_listspells );
DECLARE_DO_FUN( do_reward );
DECLARE_DO_FUN( do_togcouncil );
DECLARE_DO_FUN( do_council );
DECLARE_DO_FUN( do_rename );
DECLARE_DO_FUN( do_sdelete );



/* And by MAG  
 *    
 */
DECLARE_DO_FUN( build_interpret );
DECLARE_DO_FUN( do_build );
DECLARE_DO_FUN( do_delete );
DECLARE_DO_FUN( do_read );
DECLARE_DO_FUN( do_savearea );
DECLARE_DO_FUN( do_write );
DECLARE_DO_FUN( do_mfindlev );
DECLARE_DO_FUN( do_check_areas );
DECLARE_DO_FUN( do_check_area );

DECLARE_DO_FUN( do_ofindlev );
DECLARE_DO_FUN( do_howl );

/****************************
 * Now the 'standard' stuff:*
 ****************************/

DECLARE_DO_FUN( do_advance );
DECLARE_DO_FUN( do_allow );
DECLARE_DO_FUN( do_answer );
DECLARE_DO_FUN( do_areas );
DECLARE_DO_FUN( do_at );
DECLARE_DO_FUN( do_auction );
DECLARE_DO_FUN( do_rauction );
DECLARE_DO_FUN( do_mauction );
DECLARE_DO_FUN( do_auto );
DECLARE_DO_FUN( do_autoexit );
DECLARE_DO_FUN( do_autoloot );
DECLARE_DO_FUN( do_autosac );
DECLARE_DO_FUN( do_autosplit );
DECLARE_DO_FUN( do_autoassist );
DECLARE_DO_FUN( do_backstab );
DECLARE_DO_FUN( do_ban );
DECLARE_DO_FUN( do_blank );
DECLARE_DO_FUN( do_brandish );
DECLARE_DO_FUN( do_brief );
DECLARE_DO_FUN( do_bug );
DECLARE_DO_FUN( do_buy );
DECLARE_DO_FUN( do_cast );
DECLARE_DO_FUN( do_channels );
DECLARE_DO_FUN( do_close );
DECLARE_DO_FUN( do_combine );
DECLARE_DO_FUN( do_commands );
DECLARE_DO_FUN( do_compare );
DECLARE_DO_FUN( do_config );
DECLARE_DO_FUN( do_consider );
DECLARE_DO_FUN( do_credits );
DECLARE_DO_FUN( do_deny );
DECLARE_DO_FUN( do_description );
DECLARE_DO_FUN( do_disarm );
DECLARE_DO_FUN( do_disconnect );
DECLARE_DO_FUN( do_down );
DECLARE_DO_FUN( do_drink );
DECLARE_DO_FUN( do_drop );
DECLARE_DO_FUN( do_east );
DECLARE_DO_FUN( do_eat );
DECLARE_DO_FUN( do_echo );
DECLARE_DO_FUN( do_emote );
DECLARE_DO_FUN( do_equipment );
DECLARE_DO_FUN( do_examine );
DECLARE_DO_FUN( do_exits );
DECLARE_DO_FUN( do_fill );
DECLARE_DO_FUN( do_flee );
DECLARE_DO_FUN( do_follow );
DECLARE_DO_FUN( do_force );
DECLARE_DO_FUN( do_freeze );
DECLARE_DO_FUN( do_game );
DECLARE_DO_FUN( do_get );
DECLARE_DO_FUN( do_give );
DECLARE_DO_FUN( do_goto );
DECLARE_DO_FUN( do_group );
DECLARE_DO_FUN( do_gtell );
DECLARE_DO_FUN( do_help );
DECLARE_DO_FUN( do_hide );
DECLARE_DO_FUN( do_holylight );
DECLARE_DO_FUN( do_idea );
DECLARE_DO_FUN( do_immtalk );
DECLARE_DO_FUN( do_incognito );
DECLARE_DO_FUN( do_inventory );
DECLARE_DO_FUN( do_invis );
DECLARE_DO_FUN( do_kick );
DECLARE_DO_FUN( do_kill );
DECLARE_DO_FUN( do_list );
DECLARE_DO_FUN( do_lock );
DECLARE_DO_FUN( do_log );
DECLARE_DO_FUN( do_look );
DECLARE_DO_FUN( do_loot );
DECLARE_DO_FUN( do_memory );
DECLARE_DO_FUN( do_mfind );
DECLARE_DO_FUN( do_mload );
DECLARE_DO_FUN( do_mpasound );
DECLARE_DO_FUN( do_mpat );
DECLARE_DO_FUN( do_mpecho );
DECLARE_DO_FUN( do_mpechoaround );
DECLARE_DO_FUN( do_mpechoat );
DECLARE_DO_FUN( do_mpforce );
DECLARE_DO_FUN( do_mpgoto );
DECLARE_DO_FUN( do_mpjunk );
DECLARE_DO_FUN( do_mpkill );
DECLARE_DO_FUN( do_mpmload );
DECLARE_DO_FUN( do_mpoload );
DECLARE_DO_FUN( do_mppurge );
DECLARE_DO_FUN( do_mpstat );
DECLARE_DO_FUN( do_mptransfer );
DECLARE_DO_FUN( do_mset );
DECLARE_DO_FUN( do_mstat );
DECLARE_DO_FUN( do_mwhere );
DECLARE_DO_FUN( do_murde );
DECLARE_DO_FUN( do_murder );
DECLARE_DO_FUN( do_noemote );
DECLARE_DO_FUN( do_north );
DECLARE_DO_FUN( do_note );
DECLARE_DO_FUN( do_notell );
DECLARE_DO_FUN( do_ofind );
DECLARE_DO_FUN( do_oload );
DECLARE_DO_FUN( do_open );
DECLARE_DO_FUN( do_order );
DECLARE_DO_FUN( do_oset );
DECLARE_DO_FUN( do_ostat );
DECLARE_DO_FUN( do_owhere );
DECLARE_DO_FUN( do_pagelen );
DECLARE_DO_FUN( do_pardon );
DECLARE_DO_FUN( do_password );
DECLARE_DO_FUN( do_peace );
DECLARE_DO_FUN( do_pick );
DECLARE_DO_FUN( do_pose );
DECLARE_DO_FUN( do_practice );
DECLARE_DO_FUN( do_prompt );
DECLARE_DO_FUN( do_purge );
DECLARE_DO_FUN( do_put );
DECLARE_DO_FUN( do_quaff );
DECLARE_DO_FUN( do_question );
DECLARE_DO_FUN( do_quest2 );
DECLARE_DO_FUN( do_qui );
DECLARE_DO_FUN( do_quit );
DECLARE_DO_FUN( do_reboo );
DECLARE_DO_FUN( do_reboot );
DECLARE_DO_FUN( do_recall );
DECLARE_DO_FUN( do_recho );
DECLARE_DO_FUN( do_recite );
DECLARE_DO_FUN( do_remove );
DECLARE_DO_FUN( do_rent );
DECLARE_DO_FUN( do_reply );
DECLARE_DO_FUN( do_report );
DECLARE_DO_FUN( do_rescue );
DECLARE_DO_FUN( do_rest );
DECLARE_DO_FUN( do_restore );
DECLARE_DO_FUN( do_return );
DECLARE_DO_FUN( do_rset );
DECLARE_DO_FUN( do_rstat );
DECLARE_DO_FUN( do_sacrifice );
DECLARE_DO_FUN( do_save );
DECLARE_DO_FUN( do_say );
DECLARE_DO_FUN( do_score );
DECLARE_DO_FUN( do_sell );
DECLARE_DO_FUN( do_shout );
DECLARE_DO_FUN( do_shutdow );
DECLARE_DO_FUN( do_shutdown );
DECLARE_DO_FUN( do_silence );
DECLARE_DO_FUN( do_sla );
DECLARE_DO_FUN( do_slay );
DECLARE_DO_FUN( do_sleep );
DECLARE_DO_FUN( do_slist );
DECLARE_DO_FUN( do_slookup );
DECLARE_DO_FUN( do_sneak );
DECLARE_DO_FUN( do_snoop );
DECLARE_DO_FUN( do_socials );
DECLARE_DO_FUN( do_south );
DECLARE_DO_FUN( do_spells );
DECLARE_DO_FUN( do_split );
DECLARE_DO_FUN( do_sset );
DECLARE_DO_FUN( do_stand );
DECLARE_DO_FUN( do_steal );
DECLARE_DO_FUN( do_switch );
DECLARE_DO_FUN( do_tell );
DECLARE_DO_FUN( do_time );
DECLARE_DO_FUN( do_title );
DECLARE_DO_FUN( do_train );
DECLARE_DO_FUN( do_transfer );
DECLARE_DO_FUN( do_transdm );
DECLARE_DO_FUN( do_trust );
DECLARE_DO_FUN( do_typo );
DECLARE_DO_FUN( do_unlock );
DECLARE_DO_FUN( do_up );
DECLARE_DO_FUN( do_users );
DECLARE_DO_FUN( do_value );
DECLARE_DO_FUN( do_visible );
DECLARE_DO_FUN( do_wake );
DECLARE_DO_FUN( do_wear );
DECLARE_DO_FUN( do_weather );
DECLARE_DO_FUN( do_west );
DECLARE_DO_FUN( do_where );
DECLARE_DO_FUN( do_who );
DECLARE_DO_FUN( do_wimpy );
DECLARE_DO_FUN( do_wizhelp );
DECLARE_DO_FUN( do_wizify );
DECLARE_DO_FUN( do_wizlist );
DECLARE_DO_FUN( do_wizlock );
DECLARE_DO_FUN( do_yell );
DECLARE_DO_FUN( do_zap );

/* from Titania@AA/Sylvana@LCN */
DECLARE_DO_FUN( do_otype );
DECLARE_DO_FUN( do_owear );

/* Added by Eligan, July 10 */

DECLARE_DO_FUN( do_ignore );

/*   
      NEW SKILLS  (SRZ)

*/
DECLARE_DO_FUN( do_family );
DECLARE_DO_FUN( do_mount );
DECLARE_DO_FUN( do_dismount );
DECLARE_DO_FUN( do_qpspend );
DECLARE_DO_FUN( do_disguise );
DECLARE_DO_FUN( do_instruct );
DECLARE_DO_FUN( do_frenzy );
DECLARE_DO_FUN( do_adrenaline );
DECLARE_DO_FUN( do_target );
DECLARE_DO_FUN( do_charge );
DECLARE_DO_FUN( do_connect );
DECLARE_DO_FUN( do_stance );
DECLARE_DO_FUN( do_enchant );

/*
DECLARE_DO_FUN( do_teach      );
*/


/* 
 *  NEW CLAN COMMANDS (ZEN)
 */

DECLARE_DO_FUN( do_ctoggle );
DECLARE_DO_FUN( do_politics );
DECLARE_DO_FUN( do_negotiate );

/*
 *   New Channels (ZEN)
 */

DECLARE_DO_FUN( do_diptalk );
DECLARE_DO_FUN( do_familytalk );
DECLARE_DO_FUN( do_remorttalk );
DECLARE_DO_FUN( do_crusade );
DECLARE_DO_FUN( do_adepttalk );  /* contrib by Hermes   */
DECLARE_DO_FUN( do_rulers );
DECLARE_DO_FUN( do_scout );



/*
 *  New immcommands
 */

DECLARE_DO_FUN( do_alink );
DECLARE_DO_FUN( do_for );  /* Flar */
DECLARE_DO_FUN( do_hotreboo );   /* Flar */
DECLARE_DO_FUN( do_hotreboot );  /* Flar */
DECLARE_DO_FUN( do_imtlset ); /* Flar */
DECLARE_DO_FUN( do_gain_stat_reset );
DECLARE_DO_FUN( do_sedit );
DECLARE_DO_FUN( do_olmsg );
DECLARE_DO_FUN( do_scheck );
DECLARE_DO_FUN( do_immbrand );
DECLARE_DO_FUN( do_sysdata );
DECLARE_DO_FUN( do_areasave );
DECLARE_DO_FUN( do_mapper );
DECLARE_DO_FUN( do_mgive );
DECLARE_DO_FUN( do_email );
DECLARE_DO_FUN( do_findreset );


/*
 * Werwolf
 */
DECLARE_DO_FUN( do_rage );
DECLARE_DO_FUN( do_howl );
DECLARE_DO_FUN( do_tribe );
DECLARE_DO_FUN( do_scent );
DECLARE_DO_FUN( do_imprint );
