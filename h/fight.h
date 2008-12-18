/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_FIGHT_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* fight.c */
void violence_update P_((void));
void one_hit P_((CHAR_DATA *ch, CHAR_DATA *victim, int dt));
void damage P_((CHAR_DATA *ch, CHAR_DATA *victim, float dam, int dt));
bool is_safe P_((CHAR_DATA *ch, CHAR_DATA *victim));
void check_killer P_((CHAR_DATA *ch, CHAR_DATA *victim));
bool check_parry P_((CHAR_DATA *ch, CHAR_DATA *victim));
bool check_dodge P_((CHAR_DATA *ch, CHAR_DATA *victim));
bool check_skills P_((CHAR_DATA *ch, CHAR_DATA *victim));
void update_pos P_((CHAR_DATA *victim));
void set_fighting P_((CHAR_DATA *ch, CHAR_DATA *victim, bool check));
void stop_fighting P_((CHAR_DATA *ch, bool fBoth));
void make_corpse P_((CHAR_DATA *ch, char *argument));
void death_cry P_((CHAR_DATA *ch));
void raw_kill P_((CHAR_DATA *victim, char *argument));
void group_gain P_((CHAR_DATA *ch, CHAR_DATA *victim));
void dam_message P_((CHAR_DATA *ch, CHAR_DATA *victim, float dam, int dt));
void disarm P_((CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj));
void trip P_((CHAR_DATA *ch, CHAR_DATA *victim));
void do_kill P_((CHAR_DATA *ch, char *argument));
void do_target P_((CHAR_DATA *ch, char *argument));
void do_murde P_((CHAR_DATA *ch, char *argument));
void do_murder P_((CHAR_DATA *ch, char *argument));
void do_backstab P_((CHAR_DATA *ch, char *argument));
void do_flee P_((CHAR_DATA *ch, char *argument));
void do_rescue P_((CHAR_DATA *ch, char *argument));
void do_disarm P_((CHAR_DATA *ch, char *argument));
void do_circle P_((CHAR_DATA *ch, char *argument));
void do_trip P_((CHAR_DATA *ch, char *argument));
void do_dirt P_((CHAR_DATA *ch, char *argument));
void do_bash P_((CHAR_DATA *ch, char *argument));
void do_berserk P_((CHAR_DATA *ch, char *argument));
void do_punch P_((CHAR_DATA *ch, char *argument));
void do_headbutt P_((CHAR_DATA *ch, char *argument));
void do_charge P_((CHAR_DATA *ch, char *argument));
void do_knee P_((CHAR_DATA *ch, char *argument));
void do_kick P_((CHAR_DATA *ch, char *argument));
void obj_damage P_((OBJ_DATA *obj, CHAR_DATA *victim, float dam));
void death_message P_((CHAR_DATA *ch, CHAR_DATA *victim, int dt, int max_dt));
void do_assist P_((CHAR_DATA *ch, char *argument));
void do_stake P_((CHAR_DATA *ch, char *argument));
void do_stun P_((CHAR_DATA *ch, char *argument));
void do_feed P_((CHAR_DATA *ch, char *argument));
void check_adrenaline P_((CHAR_DATA *ch, float damage));
void do_frenzy P_((CHAR_DATA *ch, char *argument));
void do_rage P_((CHAR_DATA *ch, char *argument));
float get_speed P_((CHAR_DATA *ch, int slot));
void combat_update P_((void));
float combat_damcap P_((CHAR_DATA *ch, CHAR_DATA *victim, float dam, int dt));
void check_brawl P_((CHAR_DATA *ch));
void damage_gear P_((CHAR_DATA *ch));
void do_disguise P_((CHAR_DATA *ch, char *argument));
void do_stance P_((CHAR_DATA *ch, char *argument));

#undef P_
