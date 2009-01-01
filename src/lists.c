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
std::list<AREA_DATA *> area_list;
BAN_DATA *first_ban = NULL;
BAN_DATA *last_ban = NULL;
CHAR_DATA *first_char = NULL;
CHAR_DATA *last_char = NULL;
DESCRIPTOR_DATA *first_desc = NULL;
DESCRIPTOR_DATA *last_desc = NULL;
NOTE_DATA *first_note = NULL;
NOTE_DATA *last_note = NULL;
OBJ_DATA *first_obj = NULL;
OBJ_DATA *last_obj = NULL;
SHOP_DATA *first_shop = NULL;
SHOP_DATA *last_shop = NULL;
CORPSE_DATA *first_corpse = NULL;
CORPSE_DATA *last_corpse = NULL;
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
BOARD_DATA *first_board = NULL;
BOARD_DATA *last_board = NULL;
BUF_DATA_STRUCT *first_buf = NULL;
BUF_DATA_STRUCT *last_buf = NULL;
NPC_GROUP_DATA *first_npc_group = NULL;
NPC_GROUP_DATA *last_npc_group = NULL;
FIGHT_DATA *first_fight = NULL;
FIGHT_DATA *last_fight = NULL;
DESCRIPTOR_DATA *desc_free = NULL;
MPROG_DATA *mprog_free = NULL;
MPROG_ACT_LIST *mpact_free = NULL;
BUILD_DATA_LIST *build_free = NULL;
MAGIC_SHIELD *shield_free = NULL;
MEMBER_DATA *member_free = NULL;
CORPSE_DATA *corpse_free = NULL;
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
MONEY_TYPE *money_type_free = NULL;
BOARD_DATA *board_free = NULL;

void ( *desc_free_destructor ) ( DESCRIPTOR_DATA * ddat ) = NULL;
void ( *build_free_destructor ) ( BUILD_DATA_LIST * bddat ) = NULL;
void ( *member_free_destructor ) ( MEMBER_DATA * mdat ) = NULL;
void ( *corpse_free_destructor ) ( CORPSE_DATA * cdat ) = NULL;
void ( *mark_list_free_destructor ) ( MARK_LIST_MEMBER * mldat ) = NULL;
void ( *interact_free_destructor ) ( INTERACT_DATA * idat ) = NULL;
void ( *influence_free_destructor ) ( INFLUENCE_DATA * idat ) = NULL;
void ( *control_data_free_destructor ) ( CONTROL_DATA * cdat ) = NULL;
void ( *control_list_free_destructor ) ( CONTROL_LIST * cldat ) = NULL;
void ( *queued_interact_free_destructor ) ( QUEUED_INTERACT_LIST * qildat ) = NULL;
void ( *influence_list_free_destructor ) ( INFLUENCE_LIST * ildat ) = NULL;
void ( *ruler_list_free_destructor ) ( RULER_LIST * rldat ) = NULL;
void ( *dl_list_free_destructor ) ( DL_LIST * dldat ) = NULL;

#ifndef DEBUG_MONEY
void ( *money_type_free_destructor ) ( MONEY_TYPE * mtdat ) = NULL;
#endif
void ( *board_free_destructor ) ( BOARD_DATA * bdat ) = NULL;

#ifdef DEBUG_MONEY
void money_type_free_destructor( MONEY_TYPE * mtdat )
{
   free_string( mtdat->money_key );
}
#endif

void mprog_free_destructor( MPROG_DATA * mpdat )
{
   free_string( mpdat->arglist );
   free_string( mpdat->comlist );
   free_string( mpdat->filename );
}

void mpact_free_destructor( MPROG_ACT_LIST * mpadat )
{
   free_string( mpadat->buf );
}

void mark_free_destructor( MARK_DATA * mdat )
{
   free_string( mdat->author );
   free_string( mdat->message );
}

void shield_free_destructor( MAGIC_SHIELD * msdat )
{
   free_string( msdat->absorb_message_self );
   free_string( msdat->absorb_message_room );
   free_string( msdat->absorb_message_victim );
   free_string( msdat->name );
   free_string( msdat->wearoff_room );
   free_string( msdat->wearoff_self );
}

void ruler_data_free_destructor( RULER_DATA * rdat )
{
   free_string( rdat->name );
   free_string( rdat->affiliation_name );
   free_string( rdat->keywords );
}
