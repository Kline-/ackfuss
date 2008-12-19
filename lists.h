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

#define DEC_LISTS_H

/*
 * All of the global linked lists, in one clump.  Declarations here,
 * actual variables in lists.c
 * -- Altrag
 */
extern AREA_DATA *first_area;
extern AREA_DATA *last_area;
extern BAN_DATA *first_ban;
extern BAN_DATA *last_ban;
extern CHAR_DATA *first_char;
extern CHAR_DATA *last_char;
extern DESCRIPTOR_DATA *first_desc;
extern DESCRIPTOR_DATA *last_desc;
extern NOTE_DATA *first_note;
extern NOTE_DATA *last_note;
extern OBJ_DATA *first_obj;
extern OBJ_DATA *last_obj;
extern SHOP_DATA *first_shop;
extern SHOP_DATA *last_shop;
extern CORPSE_DATA *first_corpse;
extern CORPSE_DATA *last_corpse;
extern MARK_LIST_MEMBER *first_mark_list;
extern MARK_LIST_MEMBER *last_mark_list;
extern CONTROL_LIST *first_control_list;
extern CONTROL_LIST *last_control_list;
extern QUEUED_INTERACT_LIST *first_queued_interact;
extern INFLUENCE_LIST *first_influence_list;
extern INFLUENCE_LIST *last_influence_list;
extern RULER_LIST *first_ruler_list;
extern RULER_LIST *last_ruler_list;
extern DL_LIST *first_brand;
extern DL_LIST *last_brand;
extern BOARD_DATA *first_board;
extern BOARD_DATA *last_board;
extern BUF_DATA_STRUCT *first_buf;
extern BUF_DATA_STRUCT *last_buf;
extern NPC_GROUP_DATA *first_npc_group;
extern NPC_GROUP_DATA *last_npc_group;
extern FIGHT_DATA *first_fight;
extern FIGHT_DATA *last_fight;
extern DESCRIPTOR_DATA *desc_free;
extern MPROG_DATA *mprog_free;
extern MPROG_ACT_LIST *mpact_free;
extern BUILD_DATA_LIST *build_free;
extern MAGIC_SHIELD *shield_free;
extern MEMBER_DATA *member_free;
extern CORPSE_DATA *corpse_free;
extern MARK_DATA *mark_free;
extern MARK_LIST_MEMBER *mark_list_free;
extern INTERACT_DATA *interact_free;
extern INFLUENCE_DATA *influence_free;
extern RULER_DATA *ruler_data_free;
extern CONTROL_DATA *control_data_free;
extern CONTROL_LIST *control_list_free;
extern QUEUED_INTERACT_LIST *queued_interact_free;
extern INFLUENCE_LIST *influence_list_free;
extern RULER_LIST *ruler_list_free;
extern DL_LIST *dl_list_free;
extern MONEY_TYPE *money_type_free;
extern BOARD_DATA *board_free;

extern void ( *desc_free_destructor ) ( DESCRIPTOR_DATA * ddat );
extern void ( *build_free_destructor ) ( BUILD_DATA_LIST * bddat );
extern void ( *member_free_destructor ) ( MEMBER_DATA * mdat );
extern void ( *corpse_free_destructor ) ( CORPSE_DATA * cdat );
extern void ( *mark_list_free_destructor ) ( MARK_LIST_MEMBER * mldat );
extern void ( *interact_free_destructor ) ( INTERACT_DATA * idat );
extern void ( *influence_free_destructor ) ( INFLUENCE_DATA * idat );
extern void ( *control_data_free_destructor ) ( CONTROL_DATA * cdat );
extern void ( *control_list_free_destructor ) ( CONTROL_LIST * cldat );
extern void ( *queued_interact_free_destructor ) ( QUEUED_INTERACT_LIST * qildat );
extern void ( *influence_list_free_destructor ) ( INFLUENCE_LIST * ildat );
extern void ( *ruler_list_free_destructor ) ( RULER_LIST * rldat );
extern void ( *dl_list_free_destructor ) ( DL_LIST * dldat );

#ifndef DEBUG_MONEY
extern void ( *money_type_free_destructor ) ( MONEY_TYPE * mtdat );
#endif
extern void ( *board_free_destructor ) ( BOARD_DATA * bdat );

#ifdef DEBUG_MONEY
void money_type_free_destructor( MONEY_TYPE * mtdat );
#endif

void mprog_free_destructor( MPROG_DATA * mpdat );
void mpact_free_destructor( MPROG_ACT_LIST * mpadat );
void mark_free_destructor( MARK_DATA * mdat );
void shield_free_destructor( MAGIC_SHIELD * msdat );
void ruler_data_free_destructor( RULER_DATA * rdat );

extern int free_get;
extern int free_put;

#define GET_FREE(item, freelist) \
do { \
  if ( !(freelist) ) \
    (item) = (typeof(item))getmem(sizeof(*(item))); \
  else { \
    if ( !(freelist)->is_free ) { \
      bug("GET_FREE: freelist head is NOT FREE!  Hanging...", 0); \
      for (;;); \
    } \
    (item) = (freelist); \
    (freelist) = (item)->next; \
    memset((item), 0, sizeof(*(item))); /* This clears is_free flag */ \
    free_get++; \
  } \
} while(0)

#define PUT_FREE(item, freelist) \
do { \
  if ( (item)->is_free ) { \
    bug("PUT_FREE: item is ALREADY FREE!  Aborting...", 0); \
    abort(); \
  } \
  (item)->next = (freelist); \
  (item)->is_free = TRUE; /* This sets is_free flag */ \
  (freelist) = (item); \
  if( freelist##_destructor != NULL ) freelist##_destructor(item); \
  free_put++; \
} while(0)
