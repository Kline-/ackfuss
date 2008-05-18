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
#define DEC_ACT_MOB_H  1

#define GRP_STATE_FLEE            0
#define GRP_STATE_FIGHT           1
#define GRP_STATE_CRIT_HEAL       2
#define GRP_STATE_REFORM          3
#define GRP_STATE_CRIT_MANA       4
#define GRP_STATE_HUNTING         5
#define GRP_STATE_NORM_HEAL       6
#define GRP_STATE_NORM_MANA       7
#define GRP_STATE_LEVELING        8
#define GRP_STATE_GET_EQ          9
#define GRP_STATE_IDLE            10
#define GRP_STATE_LOST            11
#define GRP_STATE_NO_CHANGE       99

#define NPC_STATE_FLEE        0
#define NPC_STATE_CRIT_HEAL   1
#define NPC_STATE_CRIT_MANA   2

#define AI_MOB( ch )    ( IS_NPC( (ch) ) && IS_SET((ch)->act, ACT_INTELLIGENT ) )

typedef struct npc_group_data NPC_GROUP_DATA;

struct npc_group_data
{
   bool is_free;
   NPC_GROUP_DATA *next;
   NPC_GROUP_DATA *prev;
   sh_int state;
   DL_LIST *first_follower;
   DL_LIST *last_follower;
   CHAR_DATA *leader;
   ROOM_INDEX_DATA *seek_room;
   char *enemies;
   char *last_fighting;
   char *wants;
   char *needs;
};


bool able_to_level( CHAR_DATA * ch );
void gain_level( CHAR_DATA * ch );
int find_spell( CHAR_DATA * ch, int type );
void hunt_move args( ( CHAR_DATA * mob, sh_int dir ) );
sh_int h_find_dir args( ( ROOM_INDEX_DATA * room, ROOM_INDEX_DATA * target, int h_flags ) );
void mob_group_follow( CHAR_DATA * ch, CHAR_DATA * target );
void get_mob_group( CHAR_DATA * ch, CHAR_DATA * target );
bool ready_heal_room( CHAR_DATA * ch );
void need_to_stand( CHAR_DATA * ch );
void get_up( CHAR_DATA * ch, int current_state );
void mob_regen_check( CHAR_DATA * ch, CHAR_DATA * target, bool need_flee );
void mob_is_fighting( CHAR_DATA * ch );
void power_up_mob( CHAR_DATA * ch );
void mob_is_standing( CHAR_DATA * ch );
bool valid_target( CHAR_DATA * ch, CHAR_DATA * victim, int l );
void select_target( CHAR_DATA * ch );
void int_handler( CHAR_DATA * ch );
void int_combat_handler( CHAR_DATA * ch, CHAR_DATA * victim );
void int_group_handler( NPC_GROUP_DATA * ngroup );
