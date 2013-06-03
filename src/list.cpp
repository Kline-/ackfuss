/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/
#include "h/includes.h"
#include "h/list.h"

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
list<MOB_INDEX_DATA *> mob_index_list;
list<NOTE_DATA *> note_list;
list<OBJ_DATA *> obj_list;
list<OBJ_INDEX_DATA *> obj_index_list;
list<DESCRIPTOR_DATA *> pload_list;
list<RESET_DATA *> reset_list;
list<ROOM_INDEX_DATA *> room_index_list;
list<RULER_DATA *> ruler_list;
list<SHOP_DATA *> shop_list;
list<SOCIAL_DATA *> social_list;
list<HELP_DATA *> help_list;

DESCRIPTOR_DATA *first_desc = NULL;
DESCRIPTOR_DATA *last_desc = NULL;
BUILD_DATA_LIST *first_build = NULL;
BUILD_DATA_LIST *last_build = NULL;
BUF_DATA_STRUCT *first_buf = NULL;
BUF_DATA_STRUCT *last_buf = NULL;
NPC_GROUP_DATA *first_npc_group = NULL;
NPC_GROUP_DATA *last_npc_group = NULL;

MEMBER_DATA *member_free = NULL;
DL_LIST *dl_list_free = NULL;

void ( *member_free_destructor ) ( MEMBER_DATA * mdat ) = NULL;
void ( *dl_list_free_destructor ) ( DL_LIST * dldat ) = NULL;
