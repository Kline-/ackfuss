/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_ACT_MOB_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

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

#define AI_MOB( ch )    ( IS_NPC( (ch) ) && (ch)->act.test(ACT_INTELLIGENT) )

/* act_mob.c */
void gain_level P_((CHAR_DATA *ch));
int find_spell P_((CHAR_DATA *ch, int type));
void mob_group_follow P_((CHAR_DATA *ch, CHAR_DATA *target));
void get_mob_group P_((CHAR_DATA *ch, CHAR_DATA *target));
void need_to_stand P_((CHAR_DATA *ch));
void get_up P_((CHAR_DATA *ch, int current_state));
void mob_is_fighting P_((CHAR_DATA *ch));
void power_up_mob P_((CHAR_DATA *ch));
void mob_is_standing P_((CHAR_DATA *ch));
void select_target P_((CHAR_DATA *ch));
void int_handler P_((CHAR_DATA *ch));
void int_combat_handler P_((CHAR_DATA *ch, CHAR_DATA *victim));

#undef P_
