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

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

/*
 * All of the global linked lists, in one clump.  Variables here,
 * declarations in lists.h
 * -- Altrag
 */
list<AFFECT_DATA *> affect_list;
list<CHAR_DATA *> aggro_list;
list<AREA_DATA *> area_list;
list<BAN_DATA *> ban_list;
list<BOARD_DATA *> board_list;
list<BUILD_DATA_LIST *> build_dat_list;
list<CHAR_DATA *> cast_list;
list<CHAR_DATA *> char_list;
list<OBJ_DATA *> corpse_list;
list<DISABLED_DATA *> disabled_list;
list<EXTRA_DESCR_DATA *> exdesc_list;
list<EXIT_DATA *> exit_list;
list<CHAR_DATA *> fight_list;
list<FILE *> file_list;
list<LUA_DATA *> lua_list;
list<MOB_INDEX_DATA *> mob_index_list;
list<NOTE_DATA *> note_list;
list<OBJ_DATA *> obj_list;
list<OBJ_INDEX_DATA *> obj_index_list;
list<RESET_DATA *> reset_list;
list<ROOM_INDEX_DATA *> room_index_list;
list<RULER_DATA *> ruler_list;
list<SHOP_DATA *> shop_list;

DESCRIPTOR_DATA *first_desc = NULL;
DESCRIPTOR_DATA *last_desc = NULL;
BUILD_DATA_LIST *first_build = NULL;
BUILD_DATA_LIST *last_build = NULL;
DL_LIST *first_brand = NULL;
DL_LIST *last_brand = NULL;
BUF_DATA_STRUCT *first_buf = NULL;
BUF_DATA_STRUCT *last_buf = NULL;
NPC_GROUP_DATA *first_npc_group = NULL;
NPC_GROUP_DATA *last_npc_group = NULL;

MEMBER_DATA *member_free = NULL;
DL_LIST *dl_list_free = NULL;

void ( *member_free_destructor ) ( MEMBER_DATA * mdat ) = NULL;
void ( *dl_list_free_destructor ) ( DL_LIST * dldat ) = NULL;
