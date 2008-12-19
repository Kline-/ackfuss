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

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
/* For forks etc. */
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define DEC_TYPEDEFS_H

/*
 * Accommodate old non-Ansi compilers.
 */
#if defined(TRADITIONAL)
#define const
#define args( list )                    ( )
#define DECLARE_DO_FUN( fun )           void fun( )
#define DECLARE_SPEC_FUN( fun )         bool fun( )
#define DECLARE_SPELL_FUN( fun )        bool fun( )
#define DECLARE_OBJ_FUN( fun )		void fun( )
#define DECLARE_CLUTCH_FUN( fun )       void fun( )
#else
#define args( list )                    list
#define DECLARE_DO_FUN( fun )           DO_FUN    fun
#define DECLARE_SPEC_FUN( fun )         SPEC_FUN  fun
#define DECLARE_SPELL_FUN( fun )        SPELL_FUN fun
#define DECLARE_OBJ_FUN( fun )          OBJ_FUN   fun
#define DECLARE_SPEC_FUN( fun )         SPEC_FUN  fun
#endif

/*
 * Short scalar types.
 */
#if     !defined(NOWHERE)
#define NOWHERE -1
#endif

#if     !defined(FALSE)
#define FALSE    0
#endif

#if     !defined(TRUE)
#define TRUE     1
#endif

#ifndef __cplusplus
typedef int bool;
char *crypt args( ( const char *key, const char *salt ) );
#endif

/*
 * OS-dependent declarations.
 * These are all very standard library functions,
 *   but some systems have incomplete or non-ansi header files.
 */
#if     defined(macintosh)
#define NOCRYPT
#if     defined(unix)
#undef  unix
#endif
#endif

#if     defined(MSDOS)
#define NOCRYPT
#if     defined(unix)
#undef  unix
#endif
#endif

/*
 * The crypt(3) function is not available on some operating systems.
 * In particular, the U.S. Government prohibits its export from the
 *   United States to foreign countries.
 * Turn on NOCRYPT to keep passwords in plain text.
 */
#if     defined(NOCRYPT)
#define crypt(s1, s2)   (s1)
#endif

/*
 * Structure types.
 */
typedef struct portal_data PORTAL_DATA;
typedef struct affect_data AFFECT_DATA;
typedef struct room_affect_data ROOM_AFFECT_DATA;
typedef struct area_data AREA_DATA;
typedef struct ban_data BAN_DATA;
typedef struct char_data CHAR_DATA;
typedef struct descriptor_data DESCRIPTOR_DATA;
typedef struct exit_data EXIT_DATA;
typedef struct extra_descr_data EXTRA_DESCR_DATA;
typedef struct kill_data KILL_DATA;
typedef struct mob_index_data MOB_INDEX_DATA;
typedef struct note_data NOTE_DATA;
typedef struct obj_data OBJ_DATA;
typedef struct obj_index_data OBJ_INDEX_DATA;
typedef struct npc_data NPC_DATA;
typedef struct pc_data PC_DATA;
typedef struct reset_data RESET_DATA;
typedef struct room_index_data ROOM_INDEX_DATA;
typedef struct shop_data SHOP_DATA;
typedef struct time_info_data TIME_INFO_DATA;
typedef struct weather_data WEATHER_DATA;
typedef struct mob_prog_data MPROG_DATA;  /* MOBprogram */
typedef struct mob_prog_act_list MPROG_ACT_LIST;   /* MOBprogram */
/* MAG Mod */
typedef struct build_data_list BUILD_DATA_LIST; /* Online Building */
typedef struct mobprog_item MOBPROG_ITEM; /* Mobprogs */

/* Zen Mod */
typedef struct vamp_exp_table VAMP_EXP_TABLE;
typedef struct magic_shield MAGIC_SHIELD;
typedef struct politics_data_type POL_DATA;
typedef struct council_data COUNCIL_DATA;
typedef struct member_data MEMBER_DATA;
typedef struct trigger_data TRIGGER_DATA;
typedef struct corpse_data CORPSE_DATA;
typedef struct mark_data MARK_DATA;
typedef struct mark_list_member MARK_LIST_MEMBER;
typedef struct lookup_type LOOKUP_TYPE;
typedef struct message_data MESSAGE_DATA;
typedef struct board_data BOARD_DATA;
typedef struct control_data CONTROL_DATA;
typedef struct ruler_data RULER_DATA;
typedef struct influence_data INFLUENCE_DATA;
typedef struct interact_data INTERACT_DATA;
typedef struct influence_list INFLUENCE_LIST;
typedef struct control_list CONTROL_LIST;
typedef struct queued_interact_list QUEUED_INTERACT_LIST;
typedef struct ruler_list RULER_LIST;
typedef struct dl_list DL_LIST;
typedef struct brand_data BRAND_DATA;
typedef struct sysdata_type SYS_DATA_TYPE;
typedef struct money_type MONEY_TYPE;
typedef struct buf_data_struct BUF_DATA_STRUCT;
typedef struct hash_entry_tp HASH_ENTRY;
typedef struct npc_group_data NPC_GROUP_DATA;

/* Kline */
typedef struct chanhistory CHAN_HISTORY;
typedef struct super_data SUPER_DATA;
typedef struct fight_data FIGHT_DATA;
typedef struct record_data RECORD_DATA;
typedef struct quest_info QUEST_INFO;
typedef struct eq_type EQ_TYPE;
typedef struct h_queue H_QUEUE;
typedef struct social_type SOCIAL_TYPE;
typedef struct clutch_type CLUTCH_TYPE;

/*
 * Function types.
 */
typedef void CLUTCH_FUN args( ( int v1, int v2, int v3, CHAR_DATA * ch, OBJ_DATA * obj ) );
typedef void DO_FUN     args( ( CHAR_DATA * ch, char *argument ) );
typedef bool SPEC_FUN   args( ( CHAR_DATA * ch ) );
typedef bool SPELL_FUN  args( ( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj ) );
typedef void OBJ_FUN    args( ( OBJ_DATA * obj, CHAR_DATA * keeper ) );
typedef void RET_FUN    args( ( MESSAGE_DATA *, char **, CHAR_DATA *, bool ) );
