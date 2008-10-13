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

#ifdef TFS
extern short gsn_mana_sense;
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

extern const struct color_type color_table[MAX_COLOR];
extern const struct ansi_type ansi_table[MAX_ANSI];
extern const struct class_type class_table[MAX_CLASS];
extern const struct class_type remort_table[MAX_CLASS];
extern const struct race_type race_table[MAX_RACE];
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
extern CHAN_HISTORY chan_history;

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
DECLARE_DO_FUN( do_adapt );
DECLARE_DO_FUN( do_appraise );
DECLARE_DO_FUN( build_arealist );
DECLARE_DO_FUN( do_assist );
DECLARE_DO_FUN( do_bamfin );
DECLARE_DO_FUN( do_bamfout );
DECLARE_DO_FUN( do_bank );
DECLARE_DO_FUN( do_bash );
DECLARE_DO_FUN( do_berserk );
DECLARE_DO_FUN( do_bid );
DECLARE_DO_FUN( do_rbid );
DECLARE_DO_FUN( do_mbid );
DECLARE_DO_FUN( do_cdonate );
DECLARE_DO_FUN( do_circle );
DECLARE_DO_FUN( do_clan_recall );
DECLARE_DO_FUN( do_clutch );
DECLARE_DO_FUN( do_deathmatc );
DECLARE_DO_FUN( do_deathmatch );
DECLARE_DO_FUN( do_dirt );
DECLARE_DO_FUN( do_dog );
DECLARE_DO_FUN( do_donate );
DECLARE_DO_FUN( do_edit );
DECLARE_DO_FUN( do_enter );
DECLARE_DO_FUN( do_feed );
DECLARE_DO_FUN( do_fights );
DECLARE_DO_FUN( do_finger );
DECLARE_DO_FUN( do_gold );
DECLARE_DO_FUN( do_guild );
DECLARE_DO_FUN( do_halls );
DECLARE_DO_FUN( do_headbutt );
DECLARE_DO_FUN( do_knee );
DECLARE_DO_FUN( do_hunt );
DECLARE_DO_FUN( do_resetpassword );
DECLARE_DO_FUN( do_iscore );
DECLARE_DO_FUN( do_isnoop );
DECLARE_DO_FUN( do_iwhere );
DECLARE_DO_FUN( do_lhunt );
DECLARE_DO_FUN( do_monitor );
DECLARE_DO_FUN( do_mpcr );
DECLARE_DO_FUN( do_nopray );
DECLARE_DO_FUN( do_punch );
DECLARE_DO_FUN( do_quest );
DECLARE_DO_FUN( do_retrieve );
DECLARE_DO_FUN( do_scan );
DECLARE_DO_FUN( do_setclass );
DECLARE_DO_FUN( do_smash );
DECLARE_DO_FUN( do_shadowform );
DECLARE_DO_FUN( do_sstat );
DECLARE_DO_FUN( do_stake );
DECLARE_DO_FUN( do_status );
DECLARE_DO_FUN( do_stun );
DECLARE_DO_FUN( do_togbuild );
DECLARE_DO_FUN( do_togleader );
DECLARE_DO_FUN( do_trip );
DECLARE_DO_FUN( do_whoname );
DECLARE_DO_FUN( do_listspells );
DECLARE_DO_FUN( do_reward );
DECLARE_DO_FUN( do_togcouncil );

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

/****************************
 * Now the 'standard' stuff:*
 ****************************/

DECLARE_DO_FUN( do_allow );
DECLARE_DO_FUN( do_areas );
DECLARE_DO_FUN( do_at );
DECLARE_DO_FUN( do_auction );
DECLARE_DO_FUN( do_rauction );
DECLARE_DO_FUN( do_mauction );
DECLARE_DO_FUN( do_autosplit );
DECLARE_DO_FUN( do_autoassist );
DECLARE_DO_FUN( do_backstab );
DECLARE_DO_FUN( do_ban );
DECLARE_DO_FUN( do_brandish );
DECLARE_DO_FUN( do_buy );
DECLARE_DO_FUN( do_cast );
DECLARE_DO_FUN( do_close );
DECLARE_DO_FUN( do_deny );
DECLARE_DO_FUN( do_disarm );
DECLARE_DO_FUN( do_disconnect );
DECLARE_DO_FUN( do_down );
DECLARE_DO_FUN( do_drink );
DECLARE_DO_FUN( do_drop );
DECLARE_DO_FUN( do_east );
DECLARE_DO_FUN( do_eat );
DECLARE_DO_FUN( do_echo );
DECLARE_DO_FUN( do_fill );
DECLARE_DO_FUN( do_flee );
DECLARE_DO_FUN( do_force );
DECLARE_DO_FUN( do_freeze );
DECLARE_DO_FUN( do_get );
DECLARE_DO_FUN( do_give );
DECLARE_DO_FUN( do_goto );
DECLARE_DO_FUN( do_hide );
DECLARE_DO_FUN( do_holylight );
DECLARE_DO_FUN( do_incognito );
DECLARE_DO_FUN( do_invis );
DECLARE_DO_FUN( do_kick );
DECLARE_DO_FUN( do_kill );
DECLARE_DO_FUN( do_list );
DECLARE_DO_FUN( do_lock );
DECLARE_DO_FUN( do_log );
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
DECLARE_DO_FUN( do_mptransfer );
DECLARE_DO_FUN( do_mset );
DECLARE_DO_FUN( do_mstat );
DECLARE_DO_FUN( do_mwhere );
DECLARE_DO_FUN( do_murde );
DECLARE_DO_FUN( do_murder );
DECLARE_DO_FUN( do_noemote );
DECLARE_DO_FUN( do_north );
DECLARE_DO_FUN( do_notell );
DECLARE_DO_FUN( do_ofind );
DECLARE_DO_FUN( do_oload );
DECLARE_DO_FUN( do_open );
DECLARE_DO_FUN( do_oset );
DECLARE_DO_FUN( do_ostat );
DECLARE_DO_FUN( do_owhere );
DECLARE_DO_FUN( do_pardon );
DECLARE_DO_FUN( do_peace );
DECLARE_DO_FUN( do_pick );
DECLARE_DO_FUN( do_purge );
DECLARE_DO_FUN( do_put );
DECLARE_DO_FUN( do_quaff );
DECLARE_DO_FUN( do_reboo );
DECLARE_DO_FUN( do_reboot );
DECLARE_DO_FUN( do_recall );
DECLARE_DO_FUN( do_recho );
DECLARE_DO_FUN( do_recite );
DECLARE_DO_FUN( do_remove );
DECLARE_DO_FUN( do_rescue );
DECLARE_DO_FUN( do_rest );
DECLARE_DO_FUN( do_restore );
DECLARE_DO_FUN( do_return );
DECLARE_DO_FUN( do_rset );
DECLARE_DO_FUN( do_rstat );
DECLARE_DO_FUN( do_sacrifice );
DECLARE_DO_FUN( do_sell );
DECLARE_DO_FUN( do_shutdow );
DECLARE_DO_FUN( do_shutdown );
DECLARE_DO_FUN( do_silence );
DECLARE_DO_FUN( do_sla );
DECLARE_DO_FUN( do_slay );
DECLARE_DO_FUN( do_sleep );
DECLARE_DO_FUN( do_slookup );
DECLARE_DO_FUN( do_sneak );
DECLARE_DO_FUN( do_warcry );
DECLARE_DO_FUN( do_snoop );
DECLARE_DO_FUN( do_south );
DECLARE_DO_FUN( do_sset );
DECLARE_DO_FUN( do_stand );
DECLARE_DO_FUN( do_steal );
DECLARE_DO_FUN( do_switch );
DECLARE_DO_FUN( do_train );
DECLARE_DO_FUN( do_transfer );
DECLARE_DO_FUN( do_transdm );
DECLARE_DO_FUN( do_trust );
DECLARE_DO_FUN( do_unlock );
DECLARE_DO_FUN( do_up );
DECLARE_DO_FUN( do_users );
DECLARE_DO_FUN( do_value );
DECLARE_DO_FUN( do_visible );
DECLARE_DO_FUN( do_wake );
DECLARE_DO_FUN( do_wear );
DECLARE_DO_FUN( do_west );
DECLARE_DO_FUN( do_wizhelp );
DECLARE_DO_FUN( do_wizify );
DECLARE_DO_FUN( do_wizlock );
DECLARE_DO_FUN( do_zap );

/* from Titania@AA/Sylvana@LCN */
DECLARE_DO_FUN( do_otype );
DECLARE_DO_FUN( do_owear );

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
 *   New Channels (ZEN)
 */
DECLARE_DO_FUN( do_rulers );
DECLARE_DO_FUN( do_scout );



/*
 *  New immcommands
 */

DECLARE_DO_FUN( do_alink );
DECLARE_DO_FUN( do_for );  /* Flar */
DECLARE_DO_FUN( do_hotreboo );   /* Flar */
DECLARE_DO_FUN( do_hotreboot );  /* Flar */
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
DECLARE_DO_FUN( do_tribe );
DECLARE_DO_FUN( do_scent );
DECLARE_DO_FUN( do_imprint );

/*
 * My improvements -Kline
 */
DECLARE_DO_FUN( do_statraise );
DECLARE_DO_FUN( do_census );
DECLARE_DO_FUN( do_abandon );
DECLARE_DO_FUN( do_mquest );
DECLARE_DO_FUN( do_qstat );
DECLARE_DO_FUN( do_repair );
