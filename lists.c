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


#include <stdio.h>
#include <time.h>
#include "globals.h"

#ifndef DEC_ACT_MOB_H
#include "act_mob.h"
#endif


/*
 * All of the global linked lists, in one clump.  Variables here,
 * declarations in lists.h
 * -- Altrag
 */
AREA_DATA *first_area = NULL;
AREA_DATA *last_area = NULL;
BAN_DATA *first_ban = NULL;
BAN_DATA *last_ban = NULL;
CHAR_DATA *first_char = NULL;
CHAR_DATA *last_char = NULL;
DESCRIPTOR_DATA *first_desc = NULL;
DESCRIPTOR_DATA *last_desc = NULL;
HELP_DATA *first_help = NULL;
HELP_DATA *last_help = NULL;
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




PORTAL_DATA *portal_free = NULL;
AFFECT_DATA *affect_free = NULL;
ROOM_AFFECT_DATA *raffect_free = NULL;
AREA_DATA *area_free = NULL;
BAN_DATA *ban_free = NULL;
CHAR_DATA *char_free = NULL;
DESCRIPTOR_DATA *desc_free = NULL;
EXIT_DATA *exit_free = NULL;
EXTRA_DESCR_DATA *exdesc_free = NULL;
HELP_DATA *help_free = NULL;
MOB_INDEX_DATA *mid_free = NULL;
NOTE_DATA *note_free = NULL;
OBJ_DATA *obj_free = NULL;
OBJ_INDEX_DATA *oid_free = NULL;
PC_DATA *pcd_free = NULL;
RESET_DATA *reset_free = NULL;
ROOM_INDEX_DATA *rid_free = NULL;
SHOP_DATA *shop_free = NULL;
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
BRAND_DATA *brand_data_free = NULL;
MONEY_TYPE *money_type_free = NULL;
BOARD_DATA *board_free = NULL;
MESSAGE_DATA *message_free = NULL;
BUF_DATA_STRUCT *buf_free = NULL;
HASH_ENTRY *hash_free = NULL;
NPC_GROUP_DATA *npc_group_free = NULL;


void ( *portal_free_destructor ) ( PORTAL_DATA * pdat ) = NULL;
void ( *affect_free_destructor ) ( AFFECT_DATA * adat ) = NULL;
void ( *raffect_free_destructor ) ( ROOM_AFFECT_DATA * radat ) = NULL;
void ( *area_free_destructor ) ( AREA_DATA * adat ) = NULL;
void ( *desc_free_destructor ) ( DESCRIPTOR_DATA * ddat ) = NULL;
void ( *help_free_destructor ) ( HELP_DATA * hdat ) = NULL;
void ( *shop_free_destructor ) ( SHOP_DATA * sdat ) = NULL;
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
void ( *buf_free_destructor ) ( BUF_DATA_STRUCT * bdat ) = NULL;
void ( *hash_free_destructor ) ( HASH_ENTRY * hdat ) = NULL;



void note_free_destructor( NOTE_DATA * ndat )
{
   free_string( ndat->text );
   free_string( ndat->subject );
   free_string( ndat->to_list );
   free_string( ndat->date );
   free_string( ndat->sender );
}

#ifdef DEBUG_MONEY
void money_type_free_destructor( MONEY_TYPE * mtdat )
{
   free_string( mtdat->money_key );
}
#endif

void message_free_destructor( MESSAGE_DATA * mdat )
{
   free_string( mdat->author );
   free_string( mdat->title );
   free_string( mdat->message );
}

void ban_free_destructor( BAN_DATA * bdat )
{
   free_string( bdat->name );
   free_string( bdat->banned_by );
}

void reset_free_destructor( RESET_DATA * rdat )
{
   free_string( rdat->notes );
   free_string( rdat->auto_message );
}

void exdesc_free_destructor( EXTRA_DESCR_DATA * eddat )
{
   free_string( eddat->keyword );   /* Free string memory */
   free_string( eddat->description );
}

void oid_free_destructor( OBJ_INDEX_DATA * oidat )
{
   /*
    * Free strings 
    */
   free_string( oidat->name );
   free_string( oidat->short_descr );
   free_string( oidat->description );
}

void mprog_free_destructor( MPROG_DATA * mpdat )
{
   free_string( mpdat->arglist );
   free_string( mpdat->comlist );
   free_string( mpdat->filename );
}

void mid_free_destructor( MOB_INDEX_DATA * midat )
{
   /*
    * Free strings 
    */
   free_string( midat->player_name );
   free_string( midat->short_descr );
   free_string( midat->description );
}

void exit_free_destructor( EXIT_DATA * edat )
{
   free_string( edat->description );
   free_string( edat->keyword );
}

void rid_free_destructor( ROOM_INDEX_DATA * ridat )
{
   /*
    * Now get rid of strings associated with room 
    */
   free_string( ridat->name );
   free_string( ridat->description );
}

void mpact_free_destructor( MPROG_ACT_LIST * mpadat )
{
   free_string( mpadat->buf );
}

void brand_data_free_destructor( BRAND_DATA * bdat )
{
   free_string( bdat->branded );
   free_string( bdat->branded_by );
   free_string( bdat->dt_stamp );
   free_string( bdat->message );
   free_string( bdat->priority );
}

void pcd_free_destructor( PC_DATA * pcdat )
{
   sh_int cnt;

   free_string( pcdat->pwd );
   free_string( pcdat->bamfin );
   free_string( pcdat->room_enter );
   free_string( pcdat->room_exit );
   free_string( pcdat->bamfout );
   free_string( pcdat->immskll );
   free_string( pcdat->title );
   free_string( pcdat->host );
   free_string( pcdat->header );
   free_string( pcdat->message );
   free_string( pcdat->who_name );
   free_string( pcdat->lastlogin );
   free_string( pcdat->assist_msg );
   for( cnt = 0; cnt < MAX_ALIASES; cnt++ )
   {
      free_string( pcdat->alias_name[cnt] );
      free_string( pcdat->alias[cnt] );
   }
   for( cnt = 0; cnt < MAX_IGNORES; cnt++ )
   {
      free_string( pcdat->ignore_list[cnt] );
   }
   free_string( pcdat->load_msg );
   free_string( pcdat->pedit_state );
   for( cnt = 0; cnt < 5; cnt++ )
   {
      free_string( pcdat->pedit_string[cnt] );
   }
   free_string( pcdat->email_address );
}

void char_free_destructor( CHAR_DATA * cdat )
{
   free_string( cdat->name );
   free_string( cdat->short_descr );
   free_string( cdat->long_descr );
   free_string( cdat->long_descr_orig );
   free_string( cdat->description );
   free_string( cdat->prompt );
   free_string( cdat->old_prompt );
   free_string( cdat->searching );
   free_string( cdat->target );
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

void obj_free_destructor( OBJ_DATA * odat )
{
   free_string( odat->owner );
   free_string( odat->name );
   free_string( odat->description );
   free_string( odat->short_descr );
}

void ruler_data_free_destructor( RULER_DATA * rdat )
{
   free_string( rdat->name );
   free_string( rdat->affiliation_name );
   free_string( rdat->keywords );
}

void npc_group_free_destructor( NPC_GROUP_DATA * ngrp )
{
   free_string( ngrp->enemies );
   free_string( ngrp->last_fighting );
   free_string( ngrp->wants );
   free_string( ngrp->needs );
}
