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

#define DEC_MAGIC_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* magic.c */
int mana_cost P_((CHAR_DATA *ch, int sn));
int skill_lookup P_((const char *name));
int slot_lookup P_((int slot));
void say_spell P_((CHAR_DATA *ch, int sn));
bool saves_spell P_((int level, CHAR_DATA *victim));
DECLARE_DO_FUN(do_cast);
float cast_speed P_((CHAR_DATA *ch, int sn));
DECLARE_DO_FUN(do_stop);
void stop_casting P_((CHAR_DATA *ch));
void cast P_((CHAR_DATA *ch, char *argument));
void obj_cast_spell P_((int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj));
bool spell_acid_blast P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_armor P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_badbreath P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_bark_skin P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_bless P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_blindness P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_burning_hands P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_call_lightning P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cause_light P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cause_critical P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cause_serious P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_change_sex P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_charm_person P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_chill_touch P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_color_spray P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_continual_light P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_control_weather P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_create_food P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_create_spring P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_create_water P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cure_blindness P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cure_critical P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cure_light P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cure_poison P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cure_serious P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_curse P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_detect_evil P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_detect_hidden P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_detect_invis P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_detect_magic P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_detect_undead P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_detect_poison P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_dispel_magic P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_dispel_evil P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_earthquake P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_enchant_weapon P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_encumber P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_energy_drain P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_fireball P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_flamestrike P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_faerie_fire P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_faerie_fog P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_fly P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_gate P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_general_purpose P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_giant_strength P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_harm P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_heal P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_influx P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_high_explosive P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_laserbolt P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_identify P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_infravision P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_invis P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_know_alignment P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_lightning_bolt P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_locate_object P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_magic_missile P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_mass_invis P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_null P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_pass_door P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_poison P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_protection P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_refresh P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_remove_curse P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_sanctuary P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_sense_evil P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_shield P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_shocking_grasp P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_sleep P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_stone_skin P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_summon P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_teleport P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_ventriloquate P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_weaken P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_word_of_recall P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_acid_breath P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_fire_breath P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_frost_breath P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_gas_breath P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_lightning_breath P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_hellspawn P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_planergy P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_visit P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_barrier P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_static P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_phobia P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_mindflame P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_chain_lightning P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_suffocate P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_enhance_weapon P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_bloody_tears P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_mind_bolt P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_nerve_fire P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_fighting_trance P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_phase P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_dimension_blade P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_produce_food P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_animate P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_see_magic P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_detection P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_fire_blade P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_know_weakness P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_know_critical P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_calm P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_hypnosis P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_mind_flail P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_know_item P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_physic_thrust P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_physic_crush P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_ego_whip P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_night_vision P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_stalker P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_mystic_armor P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_flare P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_travel P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_window P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_portal P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_beacon P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_seal_room P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_deflect_weapon P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_black_hand P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_throw_needle P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_morale P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_leadership P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_ice_bolt P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_waterelem P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_skeleton P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_poison_weapon P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_fireshield P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_iceshield P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_shockshield P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_shadowshield P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_thoughtshield P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_ethereal P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_fireelem P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_rune_fire P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_rune_poison P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_rune_shock P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_healing_light P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_wither_shadow P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_mana_flare P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_mana_drain P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cage P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cloak_flaming P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cloak_reflect P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cloak_absorb P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cloak_adept P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cloak_regen P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_room_dispel P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_throw_star P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_soul_net P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_condense_soul P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_restoration P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_infuse P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_holy_light P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_holy_armor P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_divine_intervention P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_earthelem P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_iron_golem P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_soul_thief P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_holy_avenger P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_diamond_golem P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_summon_pegasus P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_summon_nightmare P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_summon_beast P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_summon_devourer P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_summon_shadow P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_lava_burst P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_heat_armor P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_retri_strike P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_creature_bond P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_corrupt_bond P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_fireblast P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_shockstorm P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cone_cold P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_holy_wrath P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_wraith_touch P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_thought_vise P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_black_curse P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_cloak_misery P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_poison_quinine P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_poison_arsenic P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_sonic_blast P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_mystical_focus P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
bool spell_redemption P_((int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj));
void cast_update P_((void));

#undef P_

#define REALM_NONE    	  BIT_0
#define REALM_FIRE		    BIT_1
#define REALM_SHOCK		    BIT_2
#define REALM_LIGHT		    BIT_3
#define REALM_GAS		      BIT_4
#define REALM_POISON	    BIT_5
#define REALM_COLD		    BIT_6
#define REALM_SOUND		    BIT_7
#define REALM_ACID		    BIT_8
#define REALM_DRAIN       BIT_9
#define REALM_IMPACT      BIT_10
#define REALM_MIND        BIT_11
#define REALM_HOLY        BIT_12
#define NO_REFLECT        BIT_30
#define NO_ABSORB          BIT_31

#define RE_NONE    	  0
#define RE_FIRE		    1
#define RE_SHOCK		    2
#define RE_LIGHT		    3
#define RE_GAS		      4
#define RE_POISON	    5
#define RE_COLD		    6
#define RE_SOUND		    7
#define RE_ACID		    8
#define RE_DRAIN       9
#define RE_IMPACT      10
#define RE_MIND        11
#define RE_HOLY        12

struct sp_dam_str_type
{
   int realm;
   char *basecol;
   char *invertcol;
   char *stresscol;
   char *backcol;
   char *formatter;
   char *vs;
   char *vs_stress;
   char *vp;
   char *vp_stress;
};

extern struct sp_dam_str_type sp_dam_str[];
extern bool deathmatch;
#define MAX_REALM 13
