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

#ifndef DEC_ACK_H
#include "ack.h"
#endif

#define DEC_MAGIC_H

#ifndef DEC_MONEY_H
#include "money.h"
#endif

bool sp_damage( OBJ_DATA * obj, CHAR_DATA * ch, CHAR_DATA * victim, int dam, int type, int sn, bool show_msg );
void group_gain args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool is_safe args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void set_fighting args( ( CHAR_DATA * ch, CHAR_DATA * victim, bool check ) );
void check_adrenaline args( ( CHAR_DATA * ch, sh_int damage ) );

/* magic.c */
int mana_cost( CHAR_DATA * ch, int sn );


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

/*
 * Spell functions.
 * Defined in magicX.c.
 */
DECLARE_SPELL_FUN( spell_null );

DECLARE_SPELL_FUN( spell_ego_whip );
DECLARE_SPELL_FUN( spell_physic_thrust );
DECLARE_SPELL_FUN( spell_physic_crush );
DECLARE_SPELL_FUN( spell_mind_flail );

DECLARE_SPELL_FUN( spell_acid_blast );
DECLARE_SPELL_FUN( spell_animate );
DECLARE_SPELL_FUN( spell_armor );
DECLARE_SPELL_FUN( spell_badbreath );
DECLARE_SPELL_FUN( spell_bark_skin );
DECLARE_SPELL_FUN( spell_bless );
DECLARE_SPELL_FUN( spell_blindness );
DECLARE_SPELL_FUN( spell_bloody_tears );
DECLARE_SPELL_FUN( spell_burning_hands );
DECLARE_SPELL_FUN( spell_call_lightning );
DECLARE_SPELL_FUN( spell_calm );
DECLARE_SPELL_FUN( spell_cause_critical );
DECLARE_SPELL_FUN( spell_cause_light );
DECLARE_SPELL_FUN( spell_cause_serious );
DECLARE_SPELL_FUN( spell_change_sex );
DECLARE_SPELL_FUN( spell_charm_person );
DECLARE_SPELL_FUN( spell_chill_touch );
DECLARE_SPELL_FUN( spell_color_spray );
DECLARE_SPELL_FUN( spell_continual_light );
DECLARE_SPELL_FUN( spell_control_weather );
DECLARE_SPELL_FUN( spell_create_food );
DECLARE_SPELL_FUN( spell_create_spring );
DECLARE_SPELL_FUN( spell_create_water );
DECLARE_SPELL_FUN( spell_cure_blindness );
DECLARE_SPELL_FUN( spell_cure_critical );
DECLARE_SPELL_FUN( spell_cure_light );
DECLARE_SPELL_FUN( spell_cure_poison );
DECLARE_SPELL_FUN( spell_cure_serious );
DECLARE_SPELL_FUN( spell_curse );
DECLARE_SPELL_FUN( spell_detect_evil );
DECLARE_SPELL_FUN( spell_detect_hidden );
DECLARE_SPELL_FUN( spell_detect_invis );
DECLARE_SPELL_FUN( spell_detect_magic );
DECLARE_SPELL_FUN( spell_detect_poison );
DECLARE_SPELL_FUN( spell_detect_undead );
DECLARE_SPELL_FUN( spell_detection );
DECLARE_SPELL_FUN( spell_dimension_blade );
DECLARE_SPELL_FUN( spell_dispel_evil );
DECLARE_SPELL_FUN( spell_dispel_magic );
DECLARE_SPELL_FUN( spell_earthquake );
DECLARE_SPELL_FUN( spell_enchant_weapon );
DECLARE_SPELL_FUN( spell_encumber );
DECLARE_SPELL_FUN( spell_enhance_weapon );
DECLARE_SPELL_FUN( spell_energy_drain );
DECLARE_SPELL_FUN( spell_faerie_fire );
DECLARE_SPELL_FUN( spell_faerie_fog );
DECLARE_SPELL_FUN( spell_fighting_trance );
DECLARE_SPELL_FUN( spell_fireball );
DECLARE_SPELL_FUN( spell_fire_blade );
DECLARE_SPELL_FUN( spell_flamestrike );
DECLARE_SPELL_FUN( spell_fly );
DECLARE_SPELL_FUN( spell_gate );
DECLARE_SPELL_FUN( spell_general_purpose );
DECLARE_SPELL_FUN( spell_giant_strength );
DECLARE_SPELL_FUN( spell_harm );
DECLARE_SPELL_FUN( spell_heal );
DECLARE_SPELL_FUN( spell_high_explosive );
DECLARE_SPELL_FUN( spell_hypnosis );
DECLARE_SPELL_FUN( spell_identify );
DECLARE_SPELL_FUN( spell_influx );
DECLARE_SPELL_FUN( spell_infravision );
DECLARE_SPELL_FUN( spell_invis );
DECLARE_SPELL_FUN( spell_know_alignment );
DECLARE_SPELL_FUN( spell_know_weakness );
DECLARE_SPELL_FUN( spell_know_critical );
DECLARE_SPELL_FUN( spell_know_item );
DECLARE_SPELL_FUN( spell_lightning_bolt );
DECLARE_SPELL_FUN( spell_locate_object );
DECLARE_SPELL_FUN( spell_magic_missile );
DECLARE_SPELL_FUN( spell_mass_invis );
DECLARE_SPELL_FUN( spell_mind_bolt );
DECLARE_SPELL_FUN( spell_nerve_fire );
DECLARE_SPELL_FUN( spell_night_vision );
DECLARE_SPELL_FUN( spell_pass_door );
DECLARE_SPELL_FUN( spell_phase );
DECLARE_SPELL_FUN( spell_poison );
DECLARE_SPELL_FUN( spell_produce_food );
DECLARE_SPELL_FUN( spell_protection );
DECLARE_SPELL_FUN( spell_refresh );
DECLARE_SPELL_FUN( spell_remove_curse );
DECLARE_SPELL_FUN( spell_sanctuary );
DECLARE_SPELL_FUN( spell_see_magic );
DECLARE_SPELL_FUN( spell_sense_evil );
DECLARE_SPELL_FUN( spell_shocking_grasp );
DECLARE_SPELL_FUN( spell_shield );
DECLARE_SPELL_FUN( spell_sleep );
DECLARE_SPELL_FUN( spell_stalker );
DECLARE_SPELL_FUN( spell_stone_skin );
DECLARE_SPELL_FUN( spell_suffocate );
DECLARE_SPELL_FUN( spell_summon );
DECLARE_SPELL_FUN( spell_teleport );
DECLARE_SPELL_FUN( spell_ventriloquate );
DECLARE_SPELL_FUN( spell_warcry );  /* --Stephen */
DECLARE_SPELL_FUN( spell_weaken );
DECLARE_SPELL_FUN( spell_window );
DECLARE_SPELL_FUN( spell_portal );
DECLARE_SPELL_FUN( spell_beacon );
DECLARE_SPELL_FUN( spell_word_of_recall );
DECLARE_SPELL_FUN( spell_acid_breath );
DECLARE_SPELL_FUN( spell_fire_breath );
DECLARE_SPELL_FUN( spell_frost_breath );
DECLARE_SPELL_FUN( spell_gas_breath );
DECLARE_SPELL_FUN( spell_lightning_breath );
DECLARE_SPELL_FUN( spell_planergy );
DECLARE_SPELL_FUN( spell_static );
DECLARE_SPELL_FUN( spell_visit );
DECLARE_SPELL_FUN( spell_chain_lightning );
DECLARE_SPELL_FUN( spell_phobia );
DECLARE_SPELL_FUN( spell_barrier );
DECLARE_SPELL_FUN( spell_mindflame );
DECLARE_SPELL_FUN( spell_laserbolt );
DECLARE_SPELL_FUN( spell_hellspawn );
DECLARE_SPELL_FUN( spell_travel );
DECLARE_SPELL_FUN( spell_flare );
DECLARE_SPELL_FUN( spell_mystic_armor );
DECLARE_SPELL_FUN( spell_blood_leach );
DECLARE_SPELL_FUN( spell_seal_room );
DECLARE_SPELL_FUN( spell_shade );   /* Slot 542 */
DECLARE_SPELL_FUN( spell_deflect_weapon );   /* slot 600 */
DECLARE_SPELL_FUN( spell_black_hand ); /* slot 601 */
DECLARE_SPELL_FUN( spell_throw_needle );  /* slot 602 */
DECLARE_SPELL_FUN( spell_morale );  /* slot 603 */
DECLARE_SPELL_FUN( spell_leadership ); /* slot 604 */
DECLARE_SPELL_FUN( spell_ice_bolt );   /* slot 605 */
DECLARE_SPELL_FUN( spell_waterelem );  /* slot 606 */
DECLARE_SPELL_FUN( spell_skeleton );   /* slot 607 */
DECLARE_SPELL_FUN( spell_poison_weapon ); /* slot 608 */
DECLARE_SPELL_FUN( spell_embrace ); /* slot 609 */
DECLARE_SPELL_FUN( spell_mesmerise );  /* slot 610 */
DECLARE_SPELL_FUN( spell_ethereal );   /* slot 611 */
DECLARE_SPELL_FUN( spell_fireelem );   /* slot 612 */
DECLARE_SPELL_FUN( spell_rune_fire );  /* slot 613 */
DECLARE_SPELL_FUN( spell_rune_shock ); /* slot 614 */
DECLARE_SPELL_FUN( spell_rune_poison );   /* slot 615 */
DECLARE_SPELL_FUN( spell_healing_light ); /* slot 616 */
DECLARE_SPELL_FUN( spell_wither_shadow ); /* slot 617 */
DECLARE_SPELL_FUN( spell_mana_flare ); /* slot 618 */
DECLARE_SPELL_FUN( spell_mana_drain ); /* slot 619 */
DECLARE_SPELL_FUN( spell_cage ); /* slot 620 */
DECLARE_SPELL_FUN( spell_cloak_absorb );  /* slot 621 */
DECLARE_SPELL_FUN( spell_cloak_reflect ); /* slot 622 */
DECLARE_SPELL_FUN( spell_cloak_flaming ); /* slot 623 */
DECLARE_SPELL_FUN( spell_cloak_darkness );   /* slot 624 */
DECLARE_SPELL_FUN( spell_room_dispel );   /* slot 625 */
DECLARE_SPELL_FUN( spell_cloak_adept );   /* slot 626 */
DECLARE_SPELL_FUN( spell_cloak_regen );   /* slot 627 */
DECLARE_SPELL_FUN( spell_throw_star ); /* slot 628 */
DECLARE_SPELL_FUN( spell_soul_net );   /* slot 629 */
DECLARE_SPELL_FUN( spell_condense_soul ); /* slot 630 */
DECLARE_SPELL_FUN( spell_blood_walk ); /* slot 631 */
DECLARE_SPELL_FUN( spell_restoration );   /* slot 632 */
DECLARE_SPELL_FUN( spell_infuse );  /* slot 633 */
DECLARE_SPELL_FUN( spell_holy_light ); /* slot 634 */
DECLARE_SPELL_FUN( spell_divine_intervention ); /* slot 635 */
DECLARE_SPELL_FUN( spell_holy_armor ); /* slot 636 */
DECLARE_SPELL_FUN( spell_earthelem );  /* slot 637 */
DECLARE_SPELL_FUN( spell_iron_golem ); /* slot 638 */
DECLARE_SPELL_FUN( spell_diamond_golem ); /* slot 639 */
DECLARE_SPELL_FUN( spell_soul_thief ); /* slot 640 */
DECLARE_SPELL_FUN( spell_holy_avenger );  /* slot 641 */
DECLARE_SPELL_FUN( spell_heat_armor ); /* slot 642 */
DECLARE_SPELL_FUN( spell_retri_strike );  /* slot 643 */
DECLARE_SPELL_FUN( spell_lava_burst ); /* slot 644 */
DECLARE_SPELL_FUN( spell_fireshield ); /* slot 645 */
DECLARE_SPELL_FUN( spell_iceshield );  /* slot 646 */
DECLARE_SPELL_FUN( spell_shockshield );   /* slot 647 */
DECLARE_SPELL_FUN( spell_shadowshield );  /* slot 648 */
DECLARE_SPELL_FUN( spell_thoughtshield ); /* slot 649 */
/* enraged holds slot 651-654   */
DECLARE_SPELL_FUN( spell_tribe_claw ); /* slot 655 */
DECLARE_SPELL_FUN( spell_wolf_mark );  /* slot 656 */
DECLARE_SPELL_FUN( spell_blood_sign ); /* 657 */
DECLARE_SPELL_FUN( spell_blood_sense );   /* 658 */
DECLARE_SPELL_FUN( spell_summon_pegasus );   /* slot 659 */
DECLARE_SPELL_FUN( spell_summon_nightmare ); /* slot 660 */
DECLARE_SPELL_FUN( spell_summon_beast );  /* 661 */
DECLARE_SPELL_FUN( spell_summon_devourer );  /* 662 */
DECLARE_SPELL_FUN( spell_summon_shadow ); /* 663 */
DECLARE_SPELL_FUN( spell_creature_bond ); /* 664 */
DECLARE_SPELL_FUN( spell_corrupt_bond );  /* 665 */
DECLARE_SPELL_FUN( spell_fireblast );  /* 666 */
DECLARE_SPELL_FUN( spell_shockstorm );
DECLARE_SPELL_FUN( spell_cone_cold );
DECLARE_SPELL_FUN( spell_holy_wrath );
DECLARE_SPELL_FUN( spell_wraith_touch );
DECLARE_SPELL_FUN( spell_thought_vise );  /* 670 */
DECLARE_SPELL_FUN( spell_black_curse );
DECLARE_SPELL_FUN( spell_cloak_misery );

DECLARE_SPELL_FUN( spell_poison_quinine );
DECLARE_SPELL_FUN( spell_poison_arsenic );
DECLARE_SPELL_FUN( spell_sonic_blast );
DECLARE_SPELL_FUN( spell_mystical_focus );   /* 676 */
