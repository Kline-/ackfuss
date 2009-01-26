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

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

/*
 * All of the global linked lists, in one clump.  Variables here,
 * declarations in lists.h
 * -- Altrag
 */
std::list<AFFECT_DATA *> affect_list;
std::list<AREA_DATA *> area_list;
std::list<BAN_DATA *> ban_list;
std::list<BOARD_DATA *> board_list;
std::list<BUILD_DATA_LIST *> build_dat_list;
std::list<CHAR_DATA *> char_list;
std::list<OBJ_DATA *> corpse_list;
std::list<DISABLED_DATA *> disabled_list;
std::list<EXTRA_DESCR_DATA *> exdesc_list;
std::list<EXIT_DATA *> exit_list;
std::list<FILE *> file_list;
std::list<MOB_INDEX_DATA *> mob_index_list;
std::list<NOTE_DATA *> note_list;
std::list<OBJ_DATA *> obj_list;
std::list<OBJ_INDEX_DATA *> obj_index_list;
std::list<RESET_DATA *> reset_list;
std::list<ROOM_INDEX_DATA *> room_index_list;
std::list<SHOP_DATA *> shop_list;

DESCRIPTOR_DATA *first_desc = NULL;
DESCRIPTOR_DATA *last_desc = NULL;
BUILD_DATA_LIST *first_build = NULL;
BUILD_DATA_LIST *last_build = NULL;
MARK_DATA *first_mark = NULL;
MARK_DATA *last_mark = NULL;
MARK_LIST_MEMBER *first_mark_list = NULL;
MARK_LIST_MEMBER *last_mark_list = NULL;
CONTROL_LIST *first_control_list = NULL;
CONTROL_LIST *last_control_list = NULL;
QUEUED_INTERACT_LIST *first_queued_interact = NULL;
QUEUED_INTERACT_LIST *last_queued_interact = NULL;
INFLUENCE_LIST *first_influence_list = NULL;
INFLUENCE_LIST *last_influence_list = NULL;
RULER_LIST *first_ruler_list = NULL;
RULER_LIST *last_ruler_list = NULL;
DL_LIST *first_brand = NULL;
DL_LIST *last_brand = NULL;
BUF_DATA_STRUCT *first_buf = NULL;
BUF_DATA_STRUCT *last_buf = NULL;
NPC_GROUP_DATA *first_npc_group = NULL;
NPC_GROUP_DATA *last_npc_group = NULL;
FIGHT_DATA *first_fight = NULL;
FIGHT_DATA *last_fight = NULL;

MEMBER_DATA *member_free = NULL;
MARK_DATA *mark_free = NULL;
MARK_LIST_MEMBER *mark_list_free = NULL;
INTERACT_DATA *interact_free = NULL;
INFLUENCE_DATA *influence_free = NULL;
RULER_DATA *ruler_data_free = NULL;
CONTROL_DATA *control_data_free = NULL;
CONTROL_LIST *control_list_free = NULL;
QUEUED_INTERACT_LIST *queued_interact_free = NULL;
INFLUENCE_LIST *influence_list_free = NULL;
RULER_LIST *ruler_list_free = NULL;
DL_LIST *dl_list_free = NULL;

void ( *member_free_destructor ) ( MEMBER_DATA * mdat ) = NULL;
void ( *mark_list_free_destructor ) ( MARK_LIST_MEMBER * mldat ) = NULL;
void ( *interact_free_destructor ) ( INTERACT_DATA * idat ) = NULL;
void ( *influence_free_destructor ) ( INFLUENCE_DATA * idat ) = NULL;
void ( *control_data_free_destructor ) ( CONTROL_DATA * cdat ) = NULL;
void ( *control_list_free_destructor ) ( CONTROL_LIST * cldat ) = NULL;
void ( *queued_interact_free_destructor ) ( QUEUED_INTERACT_LIST * qildat ) = NULL;
void ( *influence_list_free_destructor ) ( INFLUENCE_LIST * ildat ) = NULL;
void ( *ruler_list_free_destructor ) ( RULER_LIST * rldat ) = NULL;
void ( *dl_list_free_destructor ) ( DL_LIST * dldat ) = NULL;

void mark_free_destructor( MARK_DATA * mdat )
{
   free_string( mdat->author );
   free_string( mdat->message );
}

void ruler_data_free_destructor( RULER_DATA * rdat )
{
   free_string( rdat->name );
   free_string( rdat->affiliation_name );
   free_string( rdat->keywords );
}
