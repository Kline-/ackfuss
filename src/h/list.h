/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/
/**
 * @file list.h
 * @brief All globally referenced list / map / vector types.
 *
 *  All lists-type objects that are global in scope are referenced via this file.
 *  These are usually, but not always, containers that reference pointer objects
 *  to ensure proper tracking and memory cleanup later.
 */
#ifndef DEC_LIST_H
#define DEC_LIST_H

extern list<AFFECT_DATA *> affect_list;
extern list<CHAR_DATA *> aggro_list;
extern list<AREA_DATA *> area_list;
extern list<BAN_DATA *> ban_list;
extern list<BOARD_DATA *> board_list;
extern list<BUILD_DATA_LIST *> build_dat_list;
extern list<CHAR_DATA *> cast_list;
extern list<CHAR_DATA *> char_list;
extern list<OBJ_DATA *> corpse_list;
extern list<DISABLED_DATA *> disabled_list;
extern list<EXTRA_DESCR_DATA *> exdesc_list;
extern list<EXIT_DATA *> exit_list;
extern list<CHAR_DATA *> fight_list;
extern list<FILE *> file_list;
extern list<MOB_INDEX_DATA *> mob_index_list;
extern list<NOTE_DATA *> note_list;
extern list<OBJ_DATA *> obj_list;
extern list<OBJ_INDEX_DATA *> obj_index_list;
extern list<DESCRIPTOR_DATA *> pload_list;
extern list<RESET_DATA *> reset_list;
extern list<ROOM_INDEX_DATA *> room_index_list;
extern list<RULER_DATA *> ruler_list;
extern list<SHOP_DATA *> shop_list;
extern list<SOCIAL_DATA *> social_list;
extern list<HELP_DATA *> help_list;

extern DESCRIPTOR_DATA *first_desc;
extern DESCRIPTOR_DATA *last_desc;
extern BUILD_DATA_LIST *first_build;
extern BUILD_DATA_LIST *last_build;
extern BUF_DATA_STRUCT *first_buf;
extern BUF_DATA_STRUCT *last_buf;
extern NPC_GROUP_DATA *first_npc_group;
extern NPC_GROUP_DATA *last_npc_group;

#endif
