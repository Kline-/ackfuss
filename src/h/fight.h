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
DECLARE_DO_FUN(do_kill);
DECLARE_DO_FUN(do_target);
DECLARE_DO_FUN(do_murde);
DECLARE_DO_FUN(do_murder);
DECLARE_DO_FUN(do_backstab);
DECLARE_DO_FUN(do_flee);
DECLARE_DO_FUN(do_rescue);
DECLARE_DO_FUN(do_disarm);
DECLARE_DO_FUN(do_circle);
DECLARE_DO_FUN(do_trip);
DECLARE_DO_FUN(do_dirt);
DECLARE_DO_FUN(do_bash);
DECLARE_DO_FUN(do_berserk);
DECLARE_DO_FUN(do_punch);
DECLARE_DO_FUN(do_headbutt);
DECLARE_DO_FUN(do_charge);
DECLARE_DO_FUN(do_knee);
DECLARE_DO_FUN(do_kick);
void obj_damage P_((OBJ_DATA *obj, CHAR_DATA *victim, float dam));
void death_message P_((CHAR_DATA *ch, CHAR_DATA *victim, int dt, int max_dt));
DECLARE_DO_FUN(do_assist);
DECLARE_DO_FUN(do_stake);
DECLARE_DO_FUN(do_stun);
DECLARE_DO_FUN(do_feed);
void check_adrenaline P_((CHAR_DATA *ch, float damage));
DECLARE_DO_FUN(do_frenzy);
DECLARE_DO_FUN(do_rage);
float get_speed P_((CHAR_DATA *ch, int slot));
void combat_update P_((void));
float combat_damcap P_((CHAR_DATA *ch, CHAR_DATA *victim, float dam, int dt));
void check_brawl P_((CHAR_DATA *ch));
void damage_gear P_((CHAR_DATA *ch));
DECLARE_DO_FUN(do_disguise);
DECLARE_DO_FUN(do_stance);
void resurrect P_((CHAR_DATA *ch));
void cooldown_update P_((void));
void aggro_attack P_((CHAR_DATA *mob, CHAR_DATA *player));
void remember_attack P_((CHAR_DATA *mob, CHAR_DATA *player));

#undef P_
