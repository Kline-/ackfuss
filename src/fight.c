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

#include "h/globals.h"

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
#endif

#ifndef DEC_ACT_INFO_H
#include "h/act_info.h"
#endif

#ifndef DEC_ACT_MOB_H
#include "h/act_mob.h"
#endif

#ifndef DEC_ACT_MOVE_H
#include "h/act_move.h"
#endif

#ifndef DEC_ACT_OBJ_H
#include "h/act_obj.h"
#endif

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_BUILDTAB_H
#include "h/buildtab.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_FIGHT_H
#include "h/fight.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_HUNT_H
#include "h/hunt.h"
#endif

#ifndef DEC_MACROS_H
#include "h/macros.h"
#endif

#ifndef DEC_MAGIC_H
#include "h/magic.h"
#endif

#ifndef DEC_MONEY_H
#include "h/money.h"
#endif

#ifndef DEC_MOUNT_H
#include "h/mount.h"
#endif

#ifndef DEC_SAVE_H
#include "h/save.h"
#endif

#ifndef DEC_SPEC_FUN_H
#include "h/spec_fun.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

#ifndef DEC_UPDATE_H
#include "h/update.h"
#endif

bool deathmatch;

extern POL_DATA politics_data;
extern CHAR_DATA *quest_target;
extern CHAR_DATA *quest_mob;
extern OBJ_DATA *quest_object;
extern int quest_timer;

const float hr_damTable[121] = {
   0.311,
   0.325,
   0.341,
   0.358,
   0.378,
   0.399,
   0.423,
   0.449,
   0.478,
   0.509,
   0.543,
   0.579,
   0.618,
   0.659,
   0.703,
   0.749,
   0.797,
   0.846,
   0.897,
   0.948,
   1.000,
   1.012,
   1.024,
   1.036,
   1.048,
   1.060,
   1.071,
   1.083,
   1.094,
   1.105,
   1.117,
   1.127,
   1.138,
   1.149,
   1.159,
   1.169,
   1.178,
   1.188,
   1.197,
   1.206,
   1.215,
   1.223,
   1.231,
   1.239,
   1.247,
   1.254,
   1.261,
   1.268,
   1.274,
   1.281,
   1.287,
   1.292,
   1.298,
   1.303,
   1.308,
   1.313,
   1.317,
   1.322,
   1.326,
   1.330,
   1.333,
   1.337,
   1.340,
   1.344,
   1.347,
   1.350,
   1.352,
   1.355,
   1.357,
   1.360,
   1.362,
   1.364,
   1.366,
   1.368,
   1.370,
   1.372,
   1.373,
   1.375,
   1.376,
   1.377,
   1.379,
   1.380,
   1.381,
   1.382,
   1.383,
   1.384,
   1.385,
   1.386,
   1.387,
   1.387,
   1.388,
   1.389,
   1.389,
   1.390,
   1.391,
   1.391,
   1.392,
   1.392,
   1.393,
   1.393,
   1.393,
   1.394,
   1.394,
   1.395,
   1.395,
   1.395,
   1.395,
   1.396,
   1.396,
   1.396,
   1.396,
   1.397,
   1.397,
   1.397,
   1.397,
   1.397,
   1.397,
   1.398,
   1.398,
   1.398,
   1.398
};

/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update( void )
{
   CHAR_DATA *ch;
   CHAR_DATA *victim;
   CHAR_DATA *rch;
   CHAR_DATA *rch_next;
   list<CHAR_DATA *>::iterator li;
   bool has_cast = FALSE;

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      ch = *li;

      /*
       * For stunning during combat
       * -Damane-    4/26/96
       */

      if( ch->position == POS_STUNNED )
      {
         if( ch->wait > 0 )
         {
            ch->wait -= 1;
            continue;
         }
         else
         {
            ch->position = POS_STANDING;
            send_to_char( "You are no longer stunned.\r\n", ch );
            continue;
         }
      }
      has_cast = FALSE;

/* Healing rapidly for raged wolves  */

      if( !IS_NPC(ch) && (IS_WOLF( ch ) && IS_RAGED( ch )) )
      {
         if( !is_affected( ch, skill_lookup( "Enraged" ) ) )
            ch->act.reset(ACT_RAGED);
         ch->hit = ( UMIN( ch->max_hit, ( ch->hit + ch->max_hit / 150 ) ) );

      }

/* slight damage for players in a speeded stance, simulates fatigue */

      if( !IS_NPC( ch ) && ( stance_app[ch->stance].speed_mod > 0 ) )
      {
         ch->hit =
            UMAX( 10, ch->hit - number_range( ch->get_level("psuedo") * 5 / 1000, ch->get_level("psuedo") * 10 / 1000 ) );
      }

/*
 *  for when wolves can't cast normal spells, 
 *  increase regen rate mucho to compensate.  
 *  let's make it a skill instead..hehehe
 *
   if ( ch->fighting != NULL )
     ch->hit = ( UMIN( ch->max_hit, ( ch->hit + ( number_range( (20 * ch->max_hit /100) , 

 */

/* Heated armor damage :) ZEN  */

      if( ch->hit > 0
          && ch->in_room != NULL && get_room_index( ch->in_room->vnum ) != NULL && item_has_apply( ch, ITEM_APPLY_HEATED ) )
      {
         OBJ_DATA *heated_item;
         int heat_damage = 0;

         for( heated_item = ch->first_carry; heated_item != NULL; heated_item = heated_item->next_in_carry_list )
         {
            if( IS_SET( heated_item->item_apply, ITEM_APPLY_HEATED ) && heated_item->wear_loc != WEAR_NONE )
            {
               heat_damage = heated_item->level;
               if( IS_OBJ_STAT(heated_item,ITEM_EXTRA_REMORT) )
                  heat_damage *= 2;
               obj_damage( heated_item, ch, heat_damage );
               act( "@@W   $p@@N you are wearing are @@eBURNING@@N you!!!", ch, heated_item, NULL, TO_CHAR );
               act( "@@W   $p worn by $n is @@eBURNING@@N!!!", ch, heated_item, NULL, TO_ROOM );
               if( IS_NPC( ch ) )
                  do_remove( ch, heated_item->name );
            }
         }
      }

      if( ch->stun_timer > 0 )
      {
         ch->stun_timer--;
         continue;
      }
      /*
       * heal check for  solos ( mob )..   Zen  
       */


      if( IS_NPC( ch ) && ch->act.test(ACT_SOLO) && ch->hit > 0 )
      {
         if( ( ch->hit < ch->max_hit * 3 / 4 ) && ( ch->mana > mana_cost( ch, skill_lookup( "heal" ) ) ) )
         {
            do_cast( ch, "heal self" );
            has_cast = TRUE;
         }
      }
      else if( IS_NPC( ch ) && ch->hit < 0 )
      {
         ch->position = POS_DEAD;
         if( ch->fighting == NULL )
            if( ch->in_room != NULL )
               act( "Suddenly, $n is enveloped in a @@mBeam of light@@N, and is gone!", ch, NULL, NULL, TO_ROOM );
         stop_fighting( ch, TRUE );
         extract_char( ch, TRUE );
         continue;
      }

      area_resetting_global = TRUE;
      if( IS_NPC( ch ) && !IS_SET( ch->npcdata->def, DEF_NONE ) && ch->hit > 0 )
      {
         if( ch->hit < ch->max_hit * 2 / 3 )
         {
            if( IS_SET( ch->npcdata->def, DEF_CURE_LIGHT ) )
            {
               if( ch->mana > mana_cost( ch, skill_lookup( "cure light" ) ) )
               {
                  do_cast( ch, "\'cure light\' self" );
                  has_cast = TRUE;
               }
            }
            else if( IS_SET( ch->npcdata->def, DEF_CURE_SERIOUS ) )
            {
               if( ch->mana > mana_cost( ch, skill_lookup( "cure serious" ) ) )
               {
                  do_cast( ch, "\'cure serious\' self" );
                  has_cast = TRUE;
               }
            }
            else if( IS_SET( ch->npcdata->def, DEF_CURE_CRITIC ) )
            {
               if( ch->mana > mana_cost( ch, skill_lookup( "cure critical" ) ) )
               {
                  do_cast( ch, "\'cure critical\' self" );
                  has_cast = TRUE;
               }
            }
            else if( IS_SET( ch->npcdata->def, DEF_CURE_HEAL ) )
            {
               if( ch->mana > mana_cost( ch, skill_lookup( "heal" ) ) )
               {
                  do_cast( ch, "heal self" );
                  has_cast = TRUE;
               }
            }
         }
      }
      else if( IS_NPC( ch ) && ch->hit < 0 )
      {
         ch->position = POS_DEAD;
         if( ch->fighting == NULL )
            if( ch->in_room != NULL )
               act( "Suddenly, $n is enveloped in a @@mBeam of light@@N, and is gone!", ch, NULL, NULL, TO_ROOM );
         extract_char( ch, TRUE );
         continue;
      }

      if( ( IS_NPC( ch ) )
          && ( !IS_SET( ch->npcdata->def, DEF_NONE ) )
          && ( ch->hit > 0 ) && ( ch->first_shield == NULL ) && ( !has_cast ) && ( ch->fighting == NULL ) )
      {
         if( ( IS_SET( ch->npcdata->def, DEF_SHIELD_FIRE ) )
             && ( !is_affected( ch, skill_lookup( "fireshield" ) ) )
             && ( ch->mana > mana_cost( ch, skill_lookup( "fireshield" ) ) ) )
         {
            do_cast( ch, "fireshield" );
            has_cast = TRUE;
         }
         else
            if( ( IS_SET( ch->npcdata->def, DEF_SHIELD_ICE ) )
                && ( !is_affected( ch, skill_lookup( "iceshield" ) ) )
                && ( ch->mana > mana_cost( ch, skill_lookup( "iceshield" ) ) ) )
         {
            do_cast( ch, "iceshield" );
            has_cast = TRUE;
         }
         else
            if( ( IS_SET( ch->npcdata->def, DEF_SHIELD_SHOCK ) )
                && ( !is_affected( ch, skill_lookup( "shockshield" ) ) )
                && ( ch->mana > mana_cost( ch, skill_lookup( "shockshield" ) ) ) )
         {
            do_cast( ch, "shockshield" );
            has_cast = TRUE;
         }
      }
      area_resetting_global = FALSE;
/* Offensive spell handler, only use when actually fighting.. */

      if( ( IS_NPC( ch ) )
          && ( ch->npcdata->cast > 1 )
          && ( !has_cast )
          && ( ch->position > POS_RESTING )
          && ( ch->fighting != NULL )
          && ( ch->in_room != NULL ) && ( ch->hit > 1 ) && ( ch->position == POS_FIGHTING ) )

      {
         short cast_frequency;
         short index;

         cast_frequency = ch->get_level("psuedo") / 2; /* maybe set in olc later? */
         if( ( number_range( 0, 99 ) < cast_frequency ) && ( ch->mana >= ( 40 * ch->max_mana / 100 ) ) )
         {
            for( index = 1; index < 32; index++ )
            {
               if( ( IS_SET( ch->npcdata->cast, ( 1 << index ) ) )
                   && ( number_range( 0, 99 ) < ( index * 3 + number_range( 0, 25 ) ) )
                   && ( ch->mana > mana_cost( ch, skill_lookup( rev_table_lookup( tab_cast_name, ( 1 << index ) ) ) ) ) )
               {
                  char cast_name[MSL];
                  snprintf( cast_name, MSL, "%s %s", rev_table_lookup( tab_cast_name, ( 1 << index ) ), ch->fighting->name.c_str() );
                  do_cast( ch, cast_name );
                  has_cast = TRUE;
                  break;
               }
            }
         }

      }
      /*
       * Hunting mobs.
       * -S- Mod: use flags to work out what to do.... 
       */
      if( IS_NPC( ch ) && ch->fighting == NULL && IS_AWAKE( ch ) )
      {
         mob_hunt( ch );
         continue;
      }
      if( ( victim = ch->fighting ) == NULL || ch->in_room == NULL )
      {
         continue;
      }


      if( ( IS_AWAKE( ch ) )
          && ( ch->in_room != NULL ) && ( victim->in_room != NULL ) && ( ch->in_room == victim->in_room ) )
      {
         check_brawl(ch);
         one_hit( ch, victim, TYPE_UNDEFINED );
      }
      else
         stop_fighting( ch, FALSE );

      if( ( victim = ch->fighting ) == NULL )
         continue;

      /*
       * Fun for the whole family!   RCH is a non-fighting mob
       */
      if( IS_NPC( victim ) && ( victim->get_level("psuedo") > 15 ) )
      {
         for( rch = ch->in_room->first_person; rch != NULL; rch = rch_next )
         {
            rch_next = rch->next_in_room;
            if( !IS_NPC( rch ) )
               continue;

            if( IS_AWAKE( rch ) && rch->fighting == NULL )
            {
               /*
                * NPC's assist NPC's of same type or 45% chance regardless.
                */
               if( !IS_AFFECTED( rch, AFF_CHARM ) )
               {
                  if( ( rch->npcdata->pIndexData == victim->npcdata->pIndexData )   /* is it the same as a target here?  */
                      || ( ( number_percent(  ) < 20 )
                           && ( abs( rch->get_level("psuedo") - victim->get_level("psuedo") ) < 35 ) ) )
                  {
                     CHAR_DATA *vch;
                     CHAR_DATA *target;
                     list<CHAR_DATA *>::iterator li2;
                     int number;

                     target = NULL;
                     number = 0;

                     /*
                      * vch is the target of the lazy mob...a player 
                      */
                     for( li2 = char_list.begin(); li2 != char_list.end(); li2++ )
                     {
                        vch = *li2;
                        if( ( can_see( rch, vch ) ) && ( !IS_NPC( vch ) ) )
                        {
                           target = vch;
                           number++;
                        }
                     }

                     if( target != NULL )
                     {
                        if( abs( target->level - rch->level ) < 40 && rch != quest_mob ) /* Don't want the quest mob involved in the brawl. --Kline */
                           one_hit( rch, target, TYPE_UNDEFINED );
                     }
                  }
               }
            }
         }
      }
   }
   return;
}

#define ZERO_RATIO 20
#define ONE_STEPS 100l
#define MAX_IX 120
#define MAX_DAM_MOD 1.4



/*
 * Hit one guy once.
 */
void one_hit( CHAR_DATA * ch, CHAR_DATA * victim, int dt )
{
   OBJ_DATA *wield;
   OBJ_DATA *shield;
   int victim_ac;
   int remort_bonus;
   float dam;
   int diceroll;
   int ix;
   float dam_mod;
   /*
    * Can't beat a dead char!
    * Guard against weird room-leavings.
    */
   if( ( ch->stance == STANCE_CASTER ) || ( ch->stance == STANCE_WIZARD ) || IS_CASTING(ch) )
      return;
   if( IS_GHOST(ch) || IS_GHOST(victim) )
      return;

   if( ch->position == POS_RIDING )
   {
      if( ch->riding && ( ch->riding->in_room == ch->in_room ) )
      {
         do_dismount( ch, "" );
      }
      else
      {
         ch->position = POS_FIGHTING;
      }
   }

   if( victim->position == POS_DEAD || ch->in_room != victim->in_room )
      return;

   if( !IS_NPC( ch ) && ( ( ch->stance == STANCE_AMBUSH ) || ( ch->stance == STANCE_AC_BEST ) ) )
   {
      send_to_char( "You step out of the shadows.\r\n", ch );
      ch->stance = STANCE_WARRIOR;
      ch->stance_ac_mod = 0;
      ch->stance_dr_mod = 0;
      ch->stance_hr_mod = 0;
      act( "$n steps out of the Shadows!", ch, NULL, NULL, TO_ROOM );
   }

   /*
    * Figure out the type of damage message.
    */
   wield = get_eq_char( ch, WEAR_HOLD_HAND_L );
   if( !IS_WEAPON( wield ) )
      wield = get_eq_char( ch, WEAR_HOLD_HAND_R );
   if( !IS_WEAPON( wield ) )
      wield = NULL;
   if( dt == TYPE_UNDEFINED )
   {
      dt = TYPE_HIT;
      if( wield != NULL && wield->item_type == ITEM_WEAPON )
         dt += wield->value[3];
   }
   /*
    * check for martial arts  Taken out for wierd act crash bug with type_martial ZEN
    * if ( dt == TYPE_HIT && wield == NULL )
    * if ( number_percent() < ( IS_NPC(ch) ? 
    * ( IS_SET(ch->npcdata->skills, MOB_MARTIAL ) ? 75 : 0 ) : ( ch->pcdata->learned[gsn_martial_arts]/4) ) )
    * dt = TYPE_MARTIAL;
    * 
    */
   /*
    * Calculate to-hit-armor-class-0 versus armor.
    */


   victim_ac = GET_AC( victim );
   if( !can_see( ch, victim ) )
      victim_ac -= 200;

   if( dt == gsn_backstab || dt == gsn_circle )
      victim_ac += 300;

   /*
    * The moment of excitement!
    */
   diceroll = number_range( ( ch->get_level("psuedo") * 5 ), ( ch->get_level("psuedo") * 21 ) ) + GET_HITROLL( ch );

/* players get a tohit bonus for now  */

   if( !IS_NPC( ch ) )
      diceroll += number_range( ch->get_level("psuedo"), ( ch->get_level("psuedo") * 1 ) );
   if( ( remort_bonus = ch->get_level("psuedo") > 100 ) )
      diceroll += remort_bonus * 1;

   if( IS_NPC( ch ) )
   {
      diceroll += ( ch->get_level("psuedo") * 5 );
      if( ch->act.test(ACT_SOLO) )
         diceroll += ( ch->get_level("psuedo") * 5 );
   }
   if( IS_AFFECTED( ch, AFF_CLOAK_ADEPT ) )
      diceroll += ch->get_level("psuedo") * 2;

/* Player vs player bonus, to handle unbalanced hitroll vs ac */
   if( !IS_NPC( ch ) && !IS_NPC( victim ) && ch->get_level("psuedo") > 80 && victim->get_level("psuedo") > 80 )
      diceroll += number_range( 1000, 2000 );
   if( IS_NPC( ch ) && victim_ac < -3000 && ch->get_level("psuedo") > 110 && ( number_range( 0, 100 ) < 10 ) )
      diceroll += 3000;

   if( victim_ac > -100 )
   {
      if( diceroll + victim_ac < 0 )
         ix = -1;
      else
         ix = ZERO_RATIO;
   }
   else
   {
      /*
       * This finds the ratio of excess hit roll to AC, and
       * breaks it into steps as defined by constants above,
       * for use in damage modifier lookup table.
       * Long int calculation avoids overflow problems. 
       */
      ix = -( ONE_STEPS * ( long int )( diceroll + victim_ac ) ) / ( long int )victim_ac;
      ix += ZERO_RATIO;
   }



   if( ix < 0 )
   {
      dam_mod = 0.0;

      damage( ch, victim, 0, dt );
      return;
   }
   else if( ix <= MAX_IX )
      dam_mod = hr_damTable[ix];
   else
      dam_mod = MAX_DAM_MOD;
   /*
    * Hit.
    * Calc damage.
    * Tried to make it easy for players to hit mobs... --Stephen
    */
   if( IS_NPC( ch ) )
   {
      if( wield )
         dam = number_range( wield->value[1], wield->value[2] );
      else
         dam = number_range( ch->level / 3, ch->level / 2 );
      if( ch->act.test(ACT_SOLO ) )
         dam *= 1.5;

   }
   else
   {
      if( wield != NULL )
         dam = number_range( wield->value[1], wield->value[2] );
      else
         dam = UMAX( number_range( 2, 4 ), ch->level / 4 );
   }
   /*
    * Bonuses.
    */
   dam += number_range( GET_DAMROLL( ch ) * 13 / 20, GET_DAMROLL( ch ) * 15 / 20 );
   if( ( !IS_NPC( ch ) && ch->pcdata->learned[gsn_enhanced_damage] > 0 )
       || item_has_apply( ch, ITEM_APPLY_ENHANCED ) || ( IS_NPC( ch ) && IS_SET( ch->npcdata->skills, MOB_ENHANCED ) ) )
   {
      if( IS_NPC( ch ) )
         dam += dam * 1 / 20;
      else
         dam += ( ch->pcdata->learned[gsn_enhanced_damage] > 0 ) ?
            dam * ch->pcdata->learned[gsn_enhanced_damage] / 150 : dam * .4;
   }
   if( IS_NPC(ch) && IS_SET(ch->npcdata->skills,MOB_PROWESS) )
      dam += number_range((int)(ch->level * 1.5),(int)(ch->level * 3));
   if( !IS_NPC(ch) && ch->pcdata->learned[gsn_combat_prowess] > 0 )
      dam += number_range((int)(ch->pcdata->learned[gsn_combat_prowess] * 1.5),(int)(ch->pcdata->learned[gsn_combat_prowess] * 3));
   if( !IS_AWAKE( victim ) )
      dam *= 1.5;
   /*
    * extra damage from martial arts 
    */
   if( dt == TYPE_MARTIAL )
      dam = ( dam * 4 ) / 3;

   if( dt == gsn_backstab )
      dam *= 1.4;
   if( dt == gsn_circle )
      dam *= 1.1;
   if( IS_AFFECTED( ch, AFF_CLOAK_ADEPT ) )
      dam *= 1.2;
   if( check_charm_aff(ch,CHARM_AFF_BATTLE) )
      dam *= ((100 + get_charm_bonus(ch,CHARM_AFF_BATTLE)) / 100);
   if( check_charm_aff(victim,CHARM_AFF_BATTLE) )
      dam *= ((100 - get_charm_bonus(victim,CHARM_AFF_BATTLE)) / 100);
   if( dam < 0 )
      dam = 0;
   shield = NULL;
   if( IS_SHIELD( shield = get_eq_char( victim, WEAR_HOLD_HAND_R ) ) )
   {
   }
   else if( IS_SHIELD( shield = get_eq_char( victim, WEAR_HOLD_HAND_L ) ) )
   {
   }
   else
      shield = NULL;
   {
      int skin_mods;
      if( !IS_NPC( victim ) )
      {
         skin_mods = race_table[victim->race].race_flags;
      }
      else
      {
         skin_mods = ( victim->race == 0 ? victim->npcdata->pIndexData->race_mods : race_table[victim->race].race_flags );
      }
      if( IS_SET( skin_mods, RACE_MOD_TOUGH_SKIN ) )
         dam_mod -= .1;
      else if( IS_SET( skin_mods, RACE_MOD_STONE_SKIN ) )
         dam_mod -= .2;
      else if( IS_SET( skin_mods, RACE_MOD_IRON_SKIN ) )
         dam_mod -= .3;
   }
   if( ( ( IS_NPC( victim ) )
         || ( victim->pcdata->learned[gsn_shield_block] > 0 ) )
       && ( shield )
       && ( number_range( 1, 4 ) != 3 )
       && ( number_percent(  ) <
            ( IS_NPC( victim ) ?
              victim->get_level("psuedo") / 7 :
              victim->pcdata->learned[gsn_shield_block] / 5 )
            + ( IS_NPC( victim ) ? 10 : ( 1 * ( victim->get_level("war") - ch->level ) + victim->get_level("kni") / 8 ) ) ) )
      /*
       * Shield Block! 
       */
   {
      act( "$n blocks $N's attack with $p", victim, shield, ch, TO_NOTVICT );
      act( "$N blocks your attack with $p", ch, shield, victim, TO_CHAR );
      act( "You block $N's attack with $p", victim, shield, ch, TO_CHAR );
      damage( ch, victim, 0, -1 );
   }
   else
   {
      dam *= dam_mod;
      if( ( wield )
          && ( dam > 0 ) && ( IS_OBJ_STAT(wield,ITEM_EXTRA_LIFESTEALER) ) && ( number_range( 0, 99 ) < ( wield->level / 4 ) ) )
      {
         act( "@@W$n screams in @@Ragony@@W as an evil @@da@@eur@@da@@W flows from $p!@@N", victim, wield, ch, TO_NOTVICT );
         act( "@@WYou feel a surge of health as $p sucks the life of $N@@N!!", ch, wield, victim, TO_CHAR );
         act( "@@WYou scream in @@Ragony@@W as $p shrieks, and shrouds you in an evil @@da@@eur@@da@@N!!", victim, wield, ch,
              TO_CHAR );
         if( ( ch->get_level("sor") > 0 ) || ( ch->get_level("nec") > 0 ) )
         {
            ch->hit = UMIN( ch->max_hit, ch->hit + number_range( (int)(dam * .075), (int)(dam * 1.72) ) );
         }
         else
         {
            ch->hit = UMIN( ch->max_hit, ch->hit + number_range( (int)(dam * .03), (int)(dam * .13) ) );
         }

         ch->alignment = UMAX( -1000, ch->alignment - 50 );
      }
   }
   if( !IS_NPC(victim) )
    damage_gear(victim);
   damage( ch, victim, (int)dam, dt );

   return;
}



/*
 * Inflict damage from a hit.
 */
void damage( CHAR_DATA * ch, CHAR_DATA * victim, float dam, int dt )
{
   OBJ_DATA *sil_weapon;
   int sn;
   int tmp_dt;
   bool npc_vict = true;

   tmp_dt = dt;

   if( dt == gsn_circle || dt == gsn_backstab || dt == gsn_charge ) /* Ugly workarounds for records and dam management --Kline */
    dt = -1;

   if( victim != ch )
   {
      /*
       * Certain attacks are forbidden.
       * Most other attacks are returned.
       */
      if( is_safe( ch, victim ) )
         return;
      if( victim != ch->fighting )
         check_killer( ch, victim );

      if( victim->position > POS_STUNNED )
      {
         if( victim->fighting == NULL )
            set_fighting( victim, ch, FALSE );
         victim->position = POS_FIGHTING;
      }

      if( victim->position > POS_STUNNED )
      {
         if( ch->fighting == NULL )
         {
            set_fighting( ch, victim, TRUE );
            /*
             * check_killer( ch, victim );
             */
         }

         /*
          * If victim is charmed, ch might attack victim's master.
          */
         if( IS_NPC( ch )
             && IS_NPC( victim )
             && IS_AFFECTED( victim, AFF_CHARM )
             && victim->master != NULL && victim->master->in_room == ch->in_room && number_bits( 3 ) == 0 )
         {
            stop_fighting( ch, FALSE );
            one_hit( ch, victim->master, TYPE_UNDEFINED );
            return;
         }
      }

      /*
       * More charm stuff.
       */
      if( victim->master == ch )
         stop_follower( victim );

      /*
       * Inviso attacks ... not.
       */
      if( IS_AFFECTED( ch, AFF_INVISIBLE ) )
      {
         affect_strip( ch, gsn_invis );
         affect_strip( ch, gsn_mass_invis );
         REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
         act( "$n shimmers into existence.", ch, NULL, NULL, TO_ROOM );
      }

      /*
       * Damage modifiers.
       */
      if( IS_AFFECTED( victim, AFF_SANCTUARY ) || item_has_apply( victim, ITEM_APPLY_SANC ) )
         dam /= 2;

      if( ( IS_AFFECTED( victim, AFF_PROTECT ) || item_has_apply( ch, ITEM_APPLY_PROT ) ) && IS_EVIL( ch ) )
         dam -= dam / 4;

      if( dam < 0 )
         dam = 0;

      /*
       * Check for disarm, trip, parry, and dodge.
       */
      if( dt >= TYPE_HIT )
      {
         if( IS_NPC( ch ) && ( number_percent(  ) < ch->level / 6 ) && IS_SET( ch->npcdata->skills, MOB_DISARM ) )
            disarm( ch, victim, NULL );

         if( IS_NPC( ch ) && ( number_percent(  ) < ch->level / 6 ) && IS_SET( ch->npcdata->skills, MOB_TRIP ) )
            trip( ch, victim );

         if( check_parry( ch, victim ) )
            return;
         if( check_dodge( ch, victim ) )
            return;
         if( check_skills( ch, victim ) )
            return;
      }

   if( ((IS_NPC(ch) && IS_SET(ch->npcdata->skills,MOB_CRUSHING) && number_percent() < 80) || (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_crushing_blow])) && dt != -1 )
   {
    if( (IS_NPC(ch) && number_percent() < 9) || (!IS_NPC(ch) && (number_range(85,300) - ch->pcdata->learned[gsn_crushing_blow]) < 10) )
    {
     act("@@WYou @@e*@@RCRUSH@@e* @@W$N's body with the force of your blow!@@N",ch,NULL,victim,TO_CHAR);
     act("@@W$n @@e*@@RCRUSHES@@e* @@Wyour body with the force of $s blow!@@N",ch,NULL,victim,TO_VICT);
     act("@@W$n @@e*@@RCRUSHES@@e* @@W$N's body with the force of $s blow!@@N",ch,NULL,victim,TO_NOTVICT);
     dam *= 2.5;
    }
   }

   dam = combat_damcap(ch,victim,dam,dt);

      if( dt != -1 )
         dam_message( ch, victim, dam, dt );
   }

/* for now, can only have one shield up, or alternatively, only the first
   shield does anything		 */

   if( ( victim->first_shield != NULL ) && ( ch != victim ) && ( dam > 0 ) )
   {
      char buf1[MSL];
      char buf2[MSL];
      char buf3[MSL];
      snprintf( buf1, MSL, "%s", victim->first_shield->absorb_message_room );
      if( ch->act.test(ACT_BLIND_PLAYER ) )
         snprintf( buf2, MSL, "%s", "$K shield ouch" );
      else
         snprintf( buf2, MSL, "%s", victim->first_shield->absorb_message_victim );
      if( victim->act.test(ACT_BLIND_PLAYER ) )
         snprintf( buf3, MSL, "%s", "Your shield cool" );
      else
         snprintf( buf3, MSL, "%s", victim->first_shield->absorb_message_self );
      victim->first_shield->hits -= (short)dam;
      dam = dam - dam * ( victim->first_shield->percent / 100 );
      if( victim->first_shield->harmful == TRUE )
      {

         ch->hit = UMAX( 10, ( ch->hit - victim->first_shield->attack_dam ) );
      }
      act( buf1, victim, NULL, ch, TO_NOTVICT );
      act( buf2, ch, NULL, victim, TO_CHAR );
      act( buf3, victim, NULL, ch, TO_CHAR );

      if( victim->first_shield->hits <= 0 )
      {
         AFFECT_DATA *shield_aff;

         for( shield_aff = victim->first_affect; shield_aff != NULL; shield_aff = shield_aff->next )
            if( shield_aff->type == victim->first_shield->sn )
               break;
         if( shield_aff != NULL )
            affect_remove( victim, shield_aff );
      }
   }
   /*
    * Hurt the victim.
    * Inform the victim of his new state.
    */
   victim->hit -= (int)dam;
   if( IS_CASTING(victim) )
   {
    if( victim->casting->pushback < victim->casting->max_time )
    {
     switch( number_range(0,2) )
     {
      case 0:
       victim->casting->pushback += 0.05;
       victim->casting->time += 0.05;
       break;
      case 1:
       victim->casting->pushback += 0.10;
       victim->casting->time += 0.10;
       break;
      case 2:
       victim->casting->pushback += 0.15;
       victim->casting->time += 0.15;
       break;
     }
    }
   }

   if( !IS_NPC(ch) )
   {
    if( dam > ch->pcdata->records->pdam_amt )
    {
     send_to_char("@@yYou've broken your physical damage record!@@N\r\n",ch);
     ch->pcdata->records->pdam_amt = (int)dam;
     ch->pcdata->records->pdam_gsn = tmp_dt;
    }
   }

   if( !IS_NPC( victim ) )
      check_adrenaline( victim, dam );

   if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( dam > 350 ) )
      do_rage( victim, "FORCE" );


   if( ( IS_AFFECTED( victim, AFF_CLOAK_FLAMING ) ) && ( ch != victim )
       /*
        * && ( dt == TYPE_HIT ) 
        */  )
   {


      act( "@@N$n's @@ecloak@@N flares and envelops $N in @@eflames@@N!!", victim, NULL, ch, TO_NOTVICT );
      if( ch->act.test(ACT_BLIND_PLAYER ) )
         act( "Flame cloak on $K ouch", ch, NULL, victim, TO_CHAR );
      else
         act( "@@N$N's @@ecloak@@N flares, and envelops you with @@eflame@@N!!", ch, NULL, victim, TO_CHAR );
      if( victim->act.test(ACT_BLIND_PLAYER ) )
         act( "Your Flame cloak flares", victim, NULL, ch, TO_CHAR );
      else
         act( "@@NYour @@ecloak@@N flares, and envelops $N with @@eflame@@N!!!", victim, NULL, ch, TO_CHAR );

      if( ( is_shielded( ch, ICE_SHIELD ) ) && ( number_range( 1, 100 ) < 30 ) && ( ch != victim ) )
      {
         CHAR_DATA *rch;
         CHAR_DATA *rch_next;
         OBJ_DATA *explosion;
         CHAR_DATA *elemental;

         act( "@@e------------------------@@N", ch, NULL, NULL, TO_CHAR );
         act( "@@l************************@@N", ch, NULL, NULL, TO_CHAR );
         act( "@@W!!!!!!!!!!!!!!!!!!!!!!!!@@N", ch, NULL, NULL, TO_CHAR );
         act( "@@NThe elemental forces of @@eFire@@N and @@aIce@@N destroy each other!!!", ch, NULL, NULL, TO_CHAR );
         act( "@@W!!!!!!!!!!!!!!!!!!!!!!!!@@N", ch, NULL, NULL, TO_CHAR );
         act( "@@l************************@@N", ch, NULL, NULL, TO_CHAR );
         act( "@@e------------------------@@N", ch, NULL, NULL, TO_CHAR );
         act( "@@e------------------------@@N", ch, NULL, NULL, TO_ROOM );
         act( "@@l************************@@N", ch, NULL, NULL, TO_ROOM );
         act( "@@W!!!!!!!!!!!!!!!!!!!!!!!!@@N", ch, NULL, NULL, TO_ROOM );
         act( "@@NThe elemental forces of @@eFire@@N and @@aIce@@N destroy each other!!!", ch, NULL, NULL, TO_ROOM );
         act( "@@W!!!!!!!!!!!!!!!!!!!!!!!!@@N", ch, NULL, NULL, TO_ROOM );
         act( "@@l************************@@N", ch, NULL, NULL, TO_ROOM );
         act( "@@e------------------------@@N", ch, NULL, NULL, TO_ROOM );
         act( "@@N$N's @@ecloak@@N is ripped to shreds!!!@@N", ch, NULL, victim, TO_ROOM );
         act( "@@NYour @@ecloak@@N is ripped to shreds!!!@@N", ch, NULL, victim, TO_VICT );


         affect_strip( ch, skill_lookup( "iceshield" ) );
         affect_strip( victim, skill_lookup( "iceshield" ) );
         affect_strip( ch, skill_lookup( "fireshield" ) );
         affect_strip( victim, skill_lookup( "fireshield" ) );
         affect_strip( ch, skill_lookup( "cloak:flaming" ) );
         affect_strip( victim, skill_lookup( "cloak:flaming" ) );
         if( IS_SET( ch->affected_by, AFF_CLOAK_FLAMING ) )
            REMOVE_BIT( ch->affected_by, AFF_CLOAK_FLAMING );
         if( IS_SET( victim->affected_by, AFF_CLOAK_FLAMING ) )
            REMOVE_BIT( victim->affected_by, AFF_CLOAK_FLAMING );


         if( ( explosion = create_object( get_obj_index( OBJ_VNUM_CONFLAGRATION ), 120 ) ) != NULL )
            if( ( elemental = create_mobile( get_mob_index( MOB_VNUM_COMBAT_ELEMENTAL ) ) ) != NULL )
            {
               char bufz[MSL];

               explosion->level = 120;
               free_string( explosion->short_descr );
               explosion->short_descr = str_dup( "@@mConflagration@@N" );
               free_string( explosion->long_descr );
               explosion->long_descr = str_dup( "@@N A @@eFlaming @@NStaff of @@aIce@@N is supsended in mid air!" );

               elemental->level = 140;
               elemental->name = ".hidden";
               free_string( elemental->npcdata->short_descr );
               elemental->npcdata->short_descr = str_dup( "@@NThe @@rConflict@@N of @@eFire @@Nand @@aIce@@N" );
               elemental->long_descr = "@@NA @@rPillar@@N of @@eFire @@Nand @@aIce@@N immolates itself!";

               char_to_room( elemental, ch->in_room );
               obj_to_char( explosion, elemental );
               snprintf( bufz, MSL, "%s", explosion->name );
               do_wear( elemental, bufz );

               if( number_range( 0, 99 ) < 40 )
                  obj_cast_spell( skill_lookup( "Retributive strike" ), 100, elemental, NULL, explosion );
               else
               {
                  for( rch = ch->in_room->first_person; rch != NULL; rch = rch_next )
                  {
                     rch_next = rch->next_in_room;
                     if( rch->get_level("psuedo") < 70 )
                        continue;
                     send_to_char( "\r\n@@NYou are @@ablasted@@N by the @@econflagration@@N!\r\n", rch );
                     if( number_range( 0, 99 ) < 50 )
                        obj_cast_spell( skill_lookup( "frost breath" ), 120, elemental, rch, explosion );
                     else
                        obj_cast_spell( skill_lookup( "lava burst" ), 120, elemental, rch, explosion );
                  }
                  if( ( elemental->fighting != NULL ) && ( IS_NPC( elemental->fighting ) ) )
                  {
                     stop_fighting( elemental, TRUE );
                     if( ( number_range( 0, 99 ) < 50 ) && ( !IS_NPC( ch ) ) )
                     {
                        one_hit( elemental, ch, TYPE_UNDEFINED );
                     }
                     else if( !IS_NPC( victim ) )
                     {
                        one_hit( elemental, victim, TYPE_UNDEFINED );
                     }
                  }
               }
            }
      }
      else
      {
         float flame_damage;
         flame_damage = dam * victim->get_level("psuedo") / 300;
         if( IS_NPC( ch ) )
         {
            if( ch->act.test(ACT_SOLO ) )
               flame_damage = flame_damage / 5;
            else
               flame_damage = flame_damage / 3;
         }
         if( ( IS_NPC( victim ) ) && ( victim->act.test(ACT_SOLO ) ) )
            flame_damage = flame_damage * 1.5;

         ch->hit = UMAX( -5, ch->hit - (int)flame_damage );
      }

   }
   if( ( is_affected( victim, skill_lookup( "cloak:misery" ) ) ) && ( ch != victim ) && ( number_range( 0, 99 ) < 35 )
       /*
        * && ( dt == TYPE_HIT ) 
        */  )
   {
      AFFECT_DATA af;
      act( "@@N$n's @@dcloak@@N flares and shrouds $N in @@dmisery@@N!!", victim, NULL, ch, TO_NOTVICT );
      if( ch->act.test(ACT_BLIND_PLAYER ) )
         act( "Flame cloak on $K ouch", ch, NULL, victim, TO_CHAR );
      else
         act( "@@N$N's @@dcloak@@N flares, and shrouds you with @@dmisery@@N!!", ch, NULL, victim, TO_CHAR );
      if( victim->act.test(ACT_BLIND_PLAYER ) )
         act( "Your Death cloak flares", victim, NULL, ch, TO_CHAR );
      else
         act( "@@NYour @@dcloak@@N flares, and shrouds $N with @@dmisery@@N!!!", victim, NULL, ch, TO_CHAR );
      if( TRUE )
      {
         af.type = skill_lookup( "curse" );
         af.duration = 1;
         af.location = APPLY_HITROLL;
         af.modifier = -1;
         af.bitvector = AFF_CURSE;
         affect_join( ch, &af );

         af.location = APPLY_DAMROLL;
         af.modifier = -1;
         affect_join( ch, &af );

         af.location = APPLY_AC;
         af.modifier = -5;
         affect_join( ch, &af );
      }
   }
   update_pos( victim );

   if( ( IS_NPC( victim ) || !IS_VAMP( victim ) ) && !( deathmatch ) )
   {
      switch ( victim->position )
      {
         case POS_MORTAL:
            act( "$n is mortally wounded, and will die soon, if not aided.", victim, NULL, NULL, TO_ROOM );
            send_to_char( "You are mortally wounded, and will die soon, if not aided.\r\n", victim );
            break;

         case POS_INCAP:
            act( "$n is incapacitated and will slowly die, if not aided.", victim, NULL, NULL, TO_ROOM );
            send_to_char( "You are incapacitated and will slowly die, if not aided.\r\n", victim );
            break;

         case POS_STUNNED:
            act( "$n is too stunned to do anything!", victim, NULL, NULL, TO_ROOM );
            send_to_char( "You are too stunned to do anything!\r\n", victim );
            break;

         case POS_DEAD:
            if( ( sil_weapon = get_eq_char( ch, WEAR_HOLD_HAND_L ) ) == NULL )
               sil_weapon = get_eq_char( ch, WEAR_HOLD_HAND_R );
            if( IS_WOLF( victim ) && ( !IS_NPC( ch ) ) && ( ch->pcdata->learned[gsn_decapitate] != 0 )
                && ( sil_weapon != NULL ) && ( IS_OBJ_STAT(sil_weapon,ITEM_EXTRA_SILVER) ) )
            {
               int chance;

               chance = IS_NPC( ch ) ? ch->level * 2 : ch->pcdata->learned[gsn_decapitate];
               chance += 25;

               if( ( victim->pcdata->super->level * 5 ) > ch->level )
                  chance -= ( victim->pcdata->super->level * 5 ) - ch->level;

               if( number_percent(  ) < chance )
               {
                  act( "You DECAPITATE $N's head off with one skillful stroke!", ch, NULL, victim, TO_CHAR );
                  act( "$n DECAPITATES $N's head off with one skillful stroke!", ch, NULL, victim, TO_NOTVICT );
                  send_to_char( "You suddenly feel the world is spinning away from you!", victim );
                  send_to_char( "You have been DECAPITATED!!", victim );
                  send_to_char( "You feel the rage of the wolf flow from your body.....\r\n", victim );
                  send_to_char( "You THINK you are dead!  Ooops....\r\n", victim );
               }

               victim->act.reset(ACT_WEREWOLF);
               victim->pcdata->super->level = 0;
               victim->pcdata->super->exp = 0;
               victim->pcdata->super->energy = 0;
               victim->pcdata->super->energy_max = 0;
               victim->pcdata->super->generation = -1;
               victim->pcdata->super->bloodline = 0;
               victim->pcdata->recall_vnum = ROOM_VNUM_TEMPLE;

               for( sn = 0; sn <= MAX_SKILL; sn++ )
                  if( ( skill_table[sn].flag2 == WOLF ) && ( victim->pcdata->learned[sn] > 0 ) )
                     victim->pcdata->learned[sn] = 0;
            }

            act( "$n is DEAD!!", victim, 0, 0, TO_ROOM );
            send_to_char( "You have been KILLED!!\r\n\r\n", victim );
            break;

         default:
            if( dam > victim->max_hit / 4 )
               send_to_char( "That really did HURT!\r\n", victim );
            if( victim->hit < victim->max_hit / 4 )
               send_to_char( "You sure are BLEEDING!\r\n", victim );
            break;
      }
   }  /* end of if statement */
   /*
    * Sleep spells and extremely wounded folks.
    */
   if( !IS_AWAKE( victim ) )
      stop_fighting( victim, FALSE );

   /*
    * Payoff for killing things.
    */

   if( victim->position == POS_DEAD && ( IS_NPC( victim ) || !IS_VAMP( victim ) || ( deathmatch ) ) )
   {
      stop_fighting(ch,false);
      stop_casting(victim);
      group_gain( ch, victim );

      /* Update mquest */
      update_mquest_kill(ch,victim);

      /*
       * Sort out kill counts..... 
       */
      if( IS_NPC(victim) && IS_NPC(ch) )
       mudinfo.mk_by_npc++;
      if( IS_NPC(victim) && !IS_NPC(ch) )
       mudinfo.mk_by_pc++;
      if( !IS_NPC(victim) && IS_NPC(ch) )
       mudinfo.pk_by_npc++;
      if( !IS_NPC(victim) && !IS_NPC(ch) )
       mudinfo.pk_by_pc++;

      if( !IS_NPC( ch ) )
      {
         if( !IS_NPC( victim ) )
            ch->pcdata->records->pk++;
         else
            ch->pcdata->records->mk++;
      }
      if( !IS_NPC( victim ) )
      {
         if( !IS_NPC( ch ) )
            victim->pcdata->records->pd++;
         else
            victim->pcdata->records->md++;
      }

      if( !IS_NPC( victim ) || victim->act.test(ACT_INTELLIGENT ) )
      {
       char buf[MSL];

	    if ( !IS_NPC(ch) && !IS_NPC(victim)
	         && ch->act.test(ACT_PKOK)
	         && victim->act.test(ACT_PKOK) )
	    {

	     snprintf(buf, MSL, "%s kills %s in mortal combat.", ch->name.c_str(), victim->name.c_str());
	     info(buf, 1);
	    }
	    else
	    {
             snprintf( buf, MSL, "%s turns %s into a corpse.  Whooops.",
               ( IS_NPC(ch) ? ch->npcdata->short_descr : ch->name.c_str() ),
               ( IS_NPC(victim) ? victim->npcdata->short_descr : victim->name.c_str()) );
             info( buf, 1 );
            }


         snprintf( log_buf, (2 * MIL), "%s killed by %s at %d", victim->get_name(), ch->get_name(), victim->in_room->vnum );
         log_string( log_buf );

         notify( log_buf, 82 );

         /*
          * As level gain is no longer automatic, a dead char loses
          * * 1/2 their gained exp.  -S-
          * * Fixed my bug here too, hehe!
          */

         if( victim->exp > 0 )
         {
            int lose = victim->exp / 2;
            lose *= -1;
            victim->gain_exp(lose);
         }

      }

      npc_vict = IS_NPC(victim);

      if( IS_NPC( ch ) )
         raw_kill( victim, "" );
      else
      {
         char name_buf[MAX_STRING_LENGTH];
         snprintf( name_buf, MSL, "%s", ch->name.c_str() );
         raw_kill( victim, name_buf );
      }
      /* NPC victims are no longer valid past this point. raw_kill() will extract_char() and deallocate memory. --Kline */
      if( deathmatch && !npc_vict )
         do_quit( victim, "" );

      if( IS_NPC( ch ) && npc_vict && ch->act.test(ACT_INTELLIGENT ) )
      {
         do_get( ch, "all from corpse" );
         do_sacrifice( ch, "corpse" );
         check_reequip( ch );
         check_rewield( ch );
      }

      if( !IS_NPC( ch ) && npc_vict )
      {
         if( ch->act.test(ACT_AUTOLOOT ) )
            do_get( ch, "all from corpse" );
         else
            do_look( ch, "in corpse" );

         if( ch->act.test(ACT_AUTOSAC ) )
            do_sacrifice( ch, "corpse" );
      }

      return;
   }

   if( victim == ch )
      return;

   /*
    * Take care of link dead people.
    */
   if( !IS_NPC( victim ) && victim->desc == NULL )
   {
      if( number_range( 0, victim->wait ) == 0 )
      {
         do_recall( victim, "" );
         return;
      }
   }

   /*
    * Wimp out?
    */
   if( IS_NPC( victim ) && dam > 0 )
   {
      if( ( victim->act.test(ACT_WIMPY ) && number_bits( 1 ) == 0
            && victim->hit < victim->max_hit / 2 )
          || ( IS_AFFECTED( victim, AFF_CHARM ) && victim->master != NULL && victim->master->in_room != victim->in_room ) )
         do_flee( victim, "" );
   }

   if( !IS_NPC( victim ) && victim->hit > 0 && victim->hit <= victim->wimpy && victim->wait == 0 )
      do_flee( victim, "" );

   return;

}



bool is_safe( CHAR_DATA * ch, CHAR_DATA * victim )
{
   if( IS_GHOST(ch) || IS_GHOST(victim) )
      return true;

   if( deathmatch )
      return false;  /* Deathmatch? Anything goes!! */

   /*
    * No PC vs. PC attacks in safe rooms 
    */
   if( ( victim->in_room != NULL ) && victim->in_room->room_flags.test(RFLAG_SAFE) )
   {
      send_to_char( "Not a chance!  This is a safe room.\r\n", ch );
      return true;
   }

   if( victim->act.test(ACT_KILLER ) )
      return false;
   if( victim->act.test(ACT_THIEF ) )
      return false;
   /*
    * Vampires are considered PKOK 
    */

   if( !IS_NPC( ch ) && !IS_NPC( victim )
       && ( victim->act.test(ACT_PKOK )
            || victim->act.test(ACT_VAMPIRE ) )
       && ( ch->act.test(ACT_PKOK ) || ch->act.test(ACT_VAMPIRE ) ) )
      return false;

   if( ( ( victim->level < 10 ) || ( victim->level + 20 < ch->level ) ) && ( !IS_NPC( victim ) ) && ( !IS_NPC( ch ) ) )
   {
      send_to_char( "The Gods prevent your foul deed.\r\n", ch );
      return true;
   }

   return false;
}



struct hunt_mobs_tp
{
   int mob_vnum;
   int room_vnum;
   int min_level;
   char *name;
}
hunt_mobs[] =
{
   {
   3062, ROOM_VNUM_TEMPLE, 0, "Bug"},
   {
   3561, ROOM_VNUM_TEMPLE, 40, "Shadow Dragon"},
   {
   18302, ROOM_VNUM_TEMPLE, 80, "Bounty Hunter"},
   {
   18301, ROOM_VNUM_TEMPLE, 100, "The Avenger"},
   {
   18306, ROOM_VNUM_TEMPLE, 240, "Blayze"},
   {
   -1, -1, -1, " "}
};

/* SRZ huntlist!  */
/* Scrapped MAX_HUNTERS - can now change table without 
 * full recompile.  Was unused in checking loop. -S-
 **** LEAVE THE LAST ELEMENT of the table in place! ****/


/*
 * See if an attack justifies a KILLER flag.
 */
void check_killer( CHAR_DATA * ch, CHAR_DATA * victim )
{
   /*
    * Follow charm thread to responsible character.
    * Attacking someone's charmed char is hostile!
    * -S- Mod:  Set sentence according to difference in levels -
    * this is no. times the player must die before the killer flag
    * will be removed.
    */

   int diff;
   int a;
   MOB_INDEX_DATA *pMobIndex;
   ROOM_INDEX_DATA *pRoomIndex;
   CHAR_DATA *hunter;

   if( ( ch->fighting == victim ) )
      return;
/*
   if ( ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL )
      && ( ch->fighting != victim )  )
	victim = victim->master;   */

   /*
    * NPC's are fair game.
    * So are killers and thieves.
    */
   if( IS_NPC( victim ) || victim->act.test(ACT_KILLER ) || victim->in_room->room_flags.test(RFLAG_PK) /* -S- Mod */
       || victim->act.test(ACT_THIEF ) || IS_VAMP( victim ) || IS_WOLF( victim )   /*
                                                                                        * || ( ch->fighting == victim )  */
       || ( ch == victim ) )
      return;

   /*
    * if in deathmatch, anything goes
    */
   if( deathmatch )
      return;
   if( !IS_NPC(ch) && ch->pcdata->sentence > 5000 )
      return;

   /*
    * Check to see if ch & victim are in clans, and enemies
    */

   if( !IS_NPC( ch ) && !IS_NPC( victim ) )
   {

      if( ( ch->clan != 0 ) && ( victim->clan != 0 )
          && ( politics_data.diplomacy[ch->clan][victim->clan] < -450 ) )
         return;

      if( ch->act.test(ACT_PKOK ) && victim->act.test(ACT_PKOK ) )
         return;
   }



   /*
    * Charm-o-rama.
    */
/*    if ( IS_SET(ch->affected_by, AFF_CHARM) && ( ch->fighting != victim ) )
    {
	if ( ch->master == NULL )
	{
	    char buf[MAX_STRING_LENGTH];

	    snprintf( buf, MSL, "Check_killer: %s bad AFF_CHARM",
		IS_NPC(ch) ? ch->short_descr : ch->name );
	    bug( buf, 0 );
	    affect_strip( ch, gsn_charm_person );
	    REMOVE_BIT( ch->affected_by, AFF_CHARM );
	    return;
	}
	
	ch=ch->master;  */

   /*
    * stop_follower( ch ); Don't think we need to do this 
    */

   /*
    * }    
    */

   /*
    * if ( IS_SET(victim->affected_by, AFF_CHARM) )
    * {
    * if ( victim->master == NULL)
    * {
    * char buf[MAX_STRING_LENGTH];
    * 
    * snprintf( buf, MSL, "Check_killer: %s bad AFF_CHARM",
    * IS_NPC(ch) ? victim->short_descr : victim->name );
    * bug( buf, 0 );
    * affect_strip( ch, gsn_charm_person );
    * REMOVE_BIT( victim->affected_by, AFF_CHARM );
    * return;
    * }
    * victim=victim->master;
    * }
    * 
    */

   /*
    * NPC's are cool of course (as long as not charmed).
    * Hitting yourself is cool too (bleeding).
    * So is being immortal (Alander's idea).
    * BAH.  Imms get flagged too now, unless pkok.
    * And current killers stay as they are.
    */
   if( IS_NPC( ch ) || ch == victim )
      return;

   send_to_char( "*** You are a PLAYER KILLER!! ***\r\n", ch );

   {
      char buf[MAX_STRING_LENGTH];

      snprintf( buf, MSL, "%s flagged as a KILLER for attack on %s.", ch->name.c_str(), victim->name.c_str() );
      monitor_chan( buf, MONITOR_COMBAT );
   }
   diff = 3;
   if( ch->get_level("psuedo") > victim->get_level("psuedo") )
   {
      diff += ( ch->get_level("psuedo") - victim->get_level("psuedo") ) / 7;
      if( diff > 5 )
         diff = 5;
   }
   ch->pcdata->sentence += diff * ch->get_level("psuedo") * 3; /* Magic # - Ramias */
   if( IS_ADEPT(ch) )
      ch->pcdata->sentence += diff * ch->get_level("psuedo") * 2;

   ch->act.set(ACT_KILLER);
   save_char_obj( ch );


   /*
    * MAG Create a hunter for the person 
    */
   diff = ch->get_level("psuedo");

   /*
    * Added if check back... meant to penalize for attacking lower
    * * level players -S-
    */

   if( ch->get_level("psuedo") > victim->get_level("psuedo") )
      diff += ch->get_level("psuedo") - victim->get_level("psuedo");
/*     
    if (diff > MAX_LEVEL)
     diff=MAX_LEVEL;
                       SRZ just set the level of the hunter to the level of the player :) */

   /*
    * Fixed for loop so it can't become infinite -S- 
    */
   for( a = 0; hunt_mobs[a].min_level != -1; a++ )
   {
      if( diff <= hunt_mobs[a].min_level )
         break;
   }

   if( hunt_mobs[a].mob_vnum == -1 )
   {
      notify( "No killer hunter found in check_killer!", 83 );
      return;
   }

   if( ( pMobIndex = get_mob_index( hunt_mobs[a].mob_vnum ) ) == NULL )
   {
      bug( "check_killer: No such mob %i.", hunt_mobs[a].mob_vnum );
      return;
   }

   if( ( pRoomIndex = get_room_index( hunt_mobs[a].room_vnum ) ) == NULL )
   {
      bug( "check_killer: No such room %i.", hunt_mobs[a].room_vnum );
      return;
   }

   hunter = create_mobile( pMobIndex );
   char_to_room( hunter, pRoomIndex );
   /*
    * do_gossip( hunter, "Well, someone's gonna get it!" ); 
    */
   set_hunt( hunter, NULL, ch, NULL, HUNT_WORLD | HUNT_OPENDOOR | HUNT_PICKDOOR | HUNT_UNLOCKDOOR | HUNT_INFORM,
             HUNT_CR | HUNT_MERC );

   return;
}



/*
 * Check for parry.
 */
bool check_parry( CHAR_DATA * ch, CHAR_DATA * victim )
{
   float chance = 0;

   if( !IS_AWAKE( victim ) )
      return FALSE;

   if( IS_NPC( victim ) && !IS_SET( victim->npcdata->skills, MOB_PARRY ) )
      return FALSE;

   if( IS_NPC( victim ) )
   {
      /*
       * Tuan was here.  :) 
       */
      chance = victim->get_level("psuedo") / 3.2 + get_curr_str( victim ) * 2 / 5;
      if( victim->act.test(ACT_SOLO ) )
         chance += 15;
   }
   else
   {
      OBJ_DATA *weapon;
      if( ( weapon = get_eq_char( victim, WEAR_HOLD_HAND_L ) ) == NULL )
         weapon = get_eq_char( victim, WEAR_HOLD_HAND_R );
      if( ( weapon == NULL ) || ( !IS_WEAPON( weapon ) ) )
      {
         return FALSE;
      }
      if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( IS_SHIFTED( victim ) || IS_RAGED( victim ) ) )
         return FALSE;

      chance = ( victim->pcdata->learned[gsn_parry] / 3.5 ) + get_curr_str( victim ) * 3 / 5;
   }
   if( IS_AFFECTED( victim, AFF_CLOAK_ADEPT ) )
      chance += 5;

   if( number_percent(  ) < ( chance + ( victim->get_level("psuedo") - ch->get_level("psuedo") ) / 2 ) )
   {

      /*
       * act( "You parry $n's attack.",  ch, NULL, victim, TO_VICT    );
       * act( "$N parries your attack.", ch, NULL, victim, TO_CHAR    );  
       */
      return TRUE;
   }
   return FALSE;
}



/*
 * Check for dodge.
 */
bool check_dodge( CHAR_DATA * ch, CHAR_DATA * victim )
{
   float chance = 0;

   if( !IS_AWAKE( victim ) )
      return FALSE;

   if( IS_NPC( victim ) && !IS_SET( victim->npcdata->skills, MOB_DODGE ) )
      return FALSE;

   if( IS_NPC( victim ) )
   {
      /*
       * Tuan was here.  :) 
       */
      chance = victim->get_level("psuedo") / 3.1 + get_curr_dex( victim ) * 2 / 5;
      if( victim->act.test(ACT_SOLO ) )
         chance += 15;
   }
   else
   {
      chance = ( victim->pcdata->learned[gsn_dodge] / 3.5 ) + get_curr_dex( victim ) * 3 / 5;
      if( ch->get_level("mon") > 0 )   /* Monk  */
         chance += ch->get_level("mon") / 8;
   }
   if( IS_AFFECTED( victim, AFF_CLOAK_ADEPT ) )
      chance += 5;

   if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( IS_SHIFTED( victim ) || IS_RAGED( victim ) ) )
      chance += 20;


   if( number_percent(  ) < ( chance + ( victim->get_level("psuedo") - ch->get_level("psuedo") ) / 2 ) )
   {

      /*
       * act( "You dodge $n's attack.", ch, NULL, victim, TO_VICT    );
       * act( "$N dodges your attack.", ch, NULL, victim, TO_CHAR    );  
       */
      return TRUE;
   }
   return FALSE;
}


/*
 * Check_skills : if IS_NPC(ch) then check ch->npcdata->skills to see if there are
 * any extra attack skills available for use --Stephen
 */

bool check_skills( CHAR_DATA * ch, CHAR_DATA * victim )
{
   int cnt, check;

   if( !IS_NPC( ch ) )
      return FALSE;

   if( number_percent(  ) < 30 + ( ch->level - victim->level ) )
      return FALSE;

   /*
    * Count how many of the attack skills are available 
    */

   cnt = 0;
   if( IS_SET( ch->npcdata->skills, MOB_PUNCH ) )
      cnt++;
   if( IS_SET( ch->npcdata->skills, MOB_HEADBUTT ) )
      cnt++;
   if( IS_SET( ch->npcdata->skills, MOB_KNEE ) )
      cnt++;
   if( IS_SET( ch->npcdata->skills, MOB_DIRT ) )
      cnt++;
   if( IS_SET( ch->npcdata->skills, MOB_CHARGE ) )
      cnt++;

   if( cnt == 0 )
      return FALSE;  /* There were no attack skills set */

   check = number_range( 1, cnt );

   cnt = 0;
   if( IS_SET( ch->npcdata->skills, MOB_PUNCH ) && ( ++cnt == check ) )
   {
      do_punch( ch, "" );
      return TRUE;
   }
   if( IS_SET( ch->npcdata->skills, MOB_HEADBUTT ) && ( ++cnt == check ) )
   {
      do_headbutt( ch, "" );
      return TRUE;
   }
   if( IS_SET( ch->npcdata->skills, MOB_KNEE ) && ( ++cnt == check ) )
   {
      do_knee( ch, "" );
      return TRUE;
   }
   if( IS_SET( ch->npcdata->skills, MOB_DIRT ) && ( ++cnt == check ) )
   {
      do_dirt( ch, "" );
      return TRUE;
   }
   if( IS_SET( ch->npcdata->skills, MOB_CHARGE ) && ( ++cnt == check ) )
   {
      do_charge( ch, "" );
      return TRUE;
   }

   return FALSE;
}



/*
 * Set position of a victim.
 */
void update_pos( CHAR_DATA * victim )
{
   char buf[MAX_STRING_LENGTH];
   OBJ_DATA *obj;
   int num, counter;

   if( victim->hit > 0 )
   {
      if( IS_SET( victim->affected_by, AFF_VAMP_HEALING ) )
         REMOVE_BIT( victim->affected_by, AFF_VAMP_HEALING );

      if( victim->position <= POS_STUNNED && victim->stun_timer == 0 )
      {
         act( "$n stands, and gets to $s feet.", victim, NULL, NULL, TO_ROOM );
         victim->position = POS_STANDING;
      }
      return;
   }
   if( IS_VAMP( victim )
       && ( victim->hit < 1 ) && !IS_SET( victim->affected_by, AFF_VAMP_HEALING ) && !IS_NPC( victim ) && !( deathmatch ) )
   {
      CHAR_DATA *check;
      list<CHAR_DATA *>::iterator li;

      if( !IS_NPC( victim ) )
      {
         int lose = victim->exp / 4;
         lose *= -1;
         victim->gain_exp(lose);
      }

      snprintf( buf, MSL, "%s (vampire) has been misted!", victim->name.c_str() );
      monitor_chan( buf, MONITOR_COMBAT );

      act( "$n turns to mist and floats away....", victim, NULL, NULL, TO_ROOM );
      act( "You turn to mist and float back to the morgue...", victim, NULL, NULL, TO_CHAR );

/* Universe -  need to add junk here */
      if( IS_VAMP( victim ) && ( ( victim->fighting != NULL ) && !IS_NPC( victim->fighting ) ) )
      {
         counter = number_range( 2, 4 );

         while( counter > 0 )
         {
            num = number_range( 1, UMIN( 1, victim->carry_number ) );
            obj = victim->first_carry;
            if( obj == NULL )
               break;

            for( obj = victim->first_carry; obj != NULL; obj = obj->next_in_carry_list )
            {
               num = num - 1;
               if( num == 0 )
                  break;
            }

            if( obj == NULL )
               continue;

            if( obj != NULL && !IS_OBJ_STAT(obj,ITEM_EXTRA_NO_LOOT) )
            {
               obj_from_char( obj );
               obj_to_room( obj, victim->in_room );
            }
            counter = counter - 1;
         }
      }
      /*
       * drop stuff if is (WANTED) 
       */
      if( ( victim->act.test(ACT_KILLER ) || victim->act.test(ACT_THIEF ) )
          && ( ( victim->fighting != NULL )
               && ( ( !IS_NPC( victim->fighting ) )
                    || ( IS_NPC(victim->fighting) && !str_cmp( rev_spec_lookup( victim->fighting->npcdata->spec_fun ), "spec_executioner" ) ) ) ) )
      {
         counter = number_range( 2, 3 );

         while( counter > 0 )
         {
            num = number_range( 1, UMIN( 1, victim->carry_number ) );
            obj = victim->first_carry;
            if( obj == NULL )
               break;

            for( obj = victim->first_carry; obj != NULL; obj = obj->next_in_carry_list )
            {
               num = num - 1;
               if( num == 0 )
                  break;
            }

            if( obj == NULL )
               continue;

            if( obj != NULL && !IS_OBJ_STAT(obj,ITEM_EXTRA_NO_LOOT) )
            {
               obj_from_char( obj );
               obj_to_room( obj, victim->in_room );
            }
            counter = counter - 1;
         }
      }

      /*
       * drop stuff if is (WANTED) 
       */
      if( ( victim->fighting != NULL ) && ( !IS_NPC( victim->fighting ) ) && ( IS_WOLF( victim->fighting ) ) )
      {
         counter = number_range( 4, 8 );

         while( counter > 0 )
         {
            num = number_range( 1, UMIN( 1, victim->carry_number ) );
            obj = victim->first_carry;
            if( obj == NULL )
               break;

            for( obj = victim->first_carry; obj != NULL; obj = obj->next_in_carry_list )
            {
               num = num - 1;
               if( num == 0 )
                  break;
            }

            if( obj == NULL )
               continue;

            if( obj != NULL && !IS_OBJ_STAT(obj,ITEM_EXTRA_NO_LOOT) )
            {
               obj_from_char( obj );
               obj_to_room( obj, victim->in_room );
            }
            counter = counter - 1;
         }
      }
      char_from_room( victim );
      char_to_room( victim, get_room_index( victim->pcdata->recall_vnum ) );
      act( "A mist floats in, and forms into $n's corpse!", victim, NULL, NULL, TO_ROOM );

      SET_BIT( victim->affected_by, AFF_VAMP_HEALING );

      stop_fighting( victim, TRUE );
      victim->hit = -20;
      for( li = char_list.begin(); li != char_list.end(); li++ )
      {
         check = *li;
         if( check->hunting == victim )
            end_hunt( check );
      }
      return;
   }

   if( IS_VAMP( victim ) && IS_SET( victim->affected_by, AFF_VAMP_HEALING ) && !( deathmatch ) && !IS_NPC( victim ) )
   {

      /*
       * Then they don't really die... just regen slowly...! 
       */
      if( victim->hit < -20 )
         victim->hit = -20;

      if( victim->hit == -20 )
      {
         act( "$n's body slowly regains it's shape!", victim, NULL, NULL, TO_ROOM );
         send_to_char( "Your body slowly regains it's shape!\r\n", victim );
         victim->hit++;
         victim->position = POS_MORTAL;
      }
      else if( victim->hit > -20 && victim->hit < -15 )
         victim->hit++;
      else if( victim->hit == -15 )
      {
         act( "$n's skin grows back into place!", victim, NULL, NULL, TO_ROOM );
         send_to_char( "Your skin grows back into place!\r\n", victim );
         victim->hit++;
      }
      else if( victim->hit > -15 && victim->hit < -10 )
         victim->hit++;
      else if( victim->hit == -10 )
      {
         act( "$n's wounds form scar tissue and vanish!", victim, NULL, NULL, TO_ROOM );
         send_to_char( "Your wounds form scar tissue and vanish!\r\n", victim );
         victim->hit++;
      }
      else if( victim->hit > -10 && victim->hit < -5 )
         victim->hit++;
      else if( victim->hit == -5 )
      {
         act( "$n's looks like $s old self again.", victim, NULL, NULL, TO_ROOM );
         send_to_char( "Your look like your old self again.\r\n", victim );
         victim->hit++;
      }
      else if( victim->hit > -5 && victim->hit < -1 )
         victim->hit++;
      else
      {
         REMOVE_BIT( victim->affected_by, AFF_VAMP_HEALING );
         victim->position = POS_STUNNED;
         victim->hit = 5;
      }
      return;
   }

   if( ( !IS_NPC( victim ) ) && ( IS_VAMP( victim ) ) && !( deathmatch ) )
      return;

   if( IS_NPC( victim ) && victim->hit <= 0 )
   {
      victim->position = POS_DEAD;
      return;
   }
   if( victim->hit <= -10 )
      victim->position = POS_DEAD;
   else if( victim->hit <= -6 )
      victim->position = POS_MORTAL;
   else if( victim->hit <= -3 )
      victim->position = POS_INCAP;
   else
      victim->position = POS_STUNNED;

   return;
}



/*
 * Start fights.
 */
void set_fighting( CHAR_DATA * ch, CHAR_DATA * victim, bool check )
{
   list<CHAR_DATA *>::iterator li;

   if( ch->fighting != NULL )
   {
      /*
       * bug( "Set_fighting: already fighting", 0 );  
       */
      return;
   }

   /*
    * Check here for killer flag 
    */
   if( check )
      check_killer( ch, victim );


/*    if ( IS_NPC(victim) && is_set(victim->act, ACT_HUNTER ) )
                make_hunt( victim, ch );*//*
    * fun fun FUN!
    */


   if( IS_AFFECTED( ch, AFF_SLEEP ) )
      affect_strip( ch, gsn_sleep );

   ch->fighting = victim;
   ch->position = POS_FIGHTING;

   /*
    * Check if mob has ACT_REMEMBER (ch to attack) SET
    */

   if( IS_NPC( victim ) && !IS_NPC( ch ) && victim->act.test(ACT_REMEMBER ) )
   {
      /*
       * Then set victim->target to player's name...
       */
      if( ch != NULL && victim->target.find(ch->name) == string::npos )
      {
       victim->target += ch->name;
       victim->target += " ";
      }
   }

   /* Keep out duplicates */
   for( li = fight_list.begin(); li != fight_list.end(); li++ )
    if( *li == ch )
     return;
   /*
   snprintf_2(log_buf,"Adding %s vs %s to the fight queue.",ch->name,victim->name);
   monitor_chan(log_buf,MONITOR_DEBUG);
   */
   fight_list.push_back(ch);

   return;
}



/*
 * Stop fights.
 */
void stop_fighting( CHAR_DATA * ch, bool fBoth )
{
   list<CHAR_DATA *>::iterator li;
   CHAR_DATA *victim = ch->fighting;

   ch->position = POS_STANDING;
   update_pos( ch );
   fight_list.remove(ch);

   if( !fBoth )
   {
      ch->fighting = NULL;
      return;
   }

   li = fight_list.begin();
   while( li != fight_list.end() )
   {
      if( *li == victim )
      {
         victim->position = POS_STANDING;
         update_pos( victim );
         victim->fighting = NULL;
         li = fight_list.erase(li);
      }
      else
       ++li;
   }
   ch->fighting = NULL;
   return;
}



/*
 * Make a corpse out of a character.
 */
void make_corpse( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *corpse;
   OBJ_DATA *obj;
   OBJ_DATA *obj_next;
   CHAR_DATA *target = NULL;
   CHAR_DATA *wch;
   list<CHAR_DATA *>::iterator li;
   char *name = NULL;

   one_argument( argument, arg );

   if( deathmatch )
      return;

   /*
    * Vampire bodies crumble to dust when really killed
    */
   if( IS_VAMP( ch ) && !IS_NPC( ch ) )
      return;

   if( IS_NPC( ch ) )
   {
      if( ( ch->in_room != NULL ) && IS_SET( ch->in_room->affected_by, ROOM_BV_SOUL_NET ) )
      {
         ROOM_INDEX_DATA *room;
         ROOM_AFFECT_DATA *raf;
         ROOM_AFFECT_DATA *raf_next;
         corpse = create_object( get_obj_index( OBJ_VNUM_CAPTURED_SOUL ), ch->level );
         corpse->level = ch->level;
         obj_to_room( corpse, ch->in_room );

         for( obj = ch->first_carry; obj != NULL; obj = obj_next )
         {
            obj_next = obj->next_in_carry_list;
            obj_from_char( obj );
            extract_obj( obj );
         }

         act( "@@eAs $n's soul attempts to fade from the room, the @@dSoul Net@@e quickly collapses, entombing the soul into a small figurine!!", ch, NULL, NULL, TO_ROOM );
         room = ch->in_room;
         for( raf = room->first_room_affect; raf != NULL; raf = raf_next )
         {
            raf_next = raf->next;
            if( raf->bitvector == ROOM_BV_SOUL_NET )
            {
               r_affect_remove( room, raf );
            }
         }
         return;
      }
      else  /* still NPC, no soul net */
      {
         int gold;
         time_t lifetime;

         snprintf(name,MSL,"%s",ch->get_name());
         corpse = create_object( get_obj_index( OBJ_VNUM_CORPSE_NPC ), 0 );
         corpse->timer = number_range( 3, 6 );
         corpse->level = ch->level; /* for animate spell */
         /*
          * Takes a mob 8 rl hours to gain full gold.
          */
         lifetime = current_time - ( ch->logon );
         gold = 5 * ( ch->level ) * ( UMIN( 100, lifetime * 100 / ( 8 * 3600 ) ) ) / 100;
         /*
          * Then take from 1/5 of maximum (i.e. level) to maximum gold.
          */
         gold = number_range( gold / 5, gold );

         /*
          * Not everybody is rich. --Kline
          */
         if( !race_table[ch->race].has_money )
          gold = 0;

         if( gold > 0 )
            join_money( round_money( gold, TRUE ), corpse->money );
      }
   }
   else  /* player */
   {
      snprintf(name,MSL,"%s",ch->name.c_str());
      corpse = create_object( get_obj_index( OBJ_VNUM_CORPSE_PC ), 0 );
      corpse->timer = number_range( 20, 30 );

      snprintf( buf, MSL, "%s", ch->name.c_str() );
      free_string( corpse->owner );
      corpse->owner = str_dup( buf );

      if( ( arg[0] != '\0' ) && ( !IS_NPC( ch ) ) )
      {
         target = NULL;
         for( li = char_list.begin(); li != char_list.end(); li++ )
         {
            wch = *li;
            if( !IS_NPC( wch ) && is_name( arg, const_cast<char *>(wch->name.c_str()) ) )
            {
               target = wch;
               break;
            }
         }
         if( ( target != NULL ) && !IS_NPC( target ) )
         {
            if( ( IS_WOLF( ch ) ) && ( IS_VAMP( target ) || IS_WOLF( target ) ) )
               corpse->value[0] = 1;
            if( ch->act.test(ACT_PKOK ) )
               corpse->value[0] = 1;
            if( ch->clan > 0 )
            {
               if( target->clan != ch->clan &&
                   ( politics_data.diplomacy[ch->clan][target->clan] < -450 ) )
               {
                  corpse->value[2] = target->clan;
               }
            }
            else
               corpse->value[2] = -1;
            corpse->value[3] = number_range( 2, 5 );
         }
      }
   }  /* end of player only */


   if( ( ch->act.test(ACT_KILLER ) || ch->act.test(ACT_THIEF ) )
       && ( ( target != NULL )
            && ( ( !IS_NPC( target ) ) || ( IS_NPC(target) && !str_cmp( rev_spec_lookup( target->npcdata->spec_fun ), "spec_executioner" ) ) ) ) )

   {
      corpse->value[0] = 1;
      corpse->value[3] = number_range( 3, 6 );
   }
   snprintf( buf, MSL, corpse->short_descr, name );
   free_string( corpse->short_descr );
   corpse->short_descr = str_dup( buf );

   snprintf( buf, MSL, corpse->long_descr, name );
   free_string( corpse->long_descr );
   corpse->long_descr = str_dup( buf );

   for( obj = ch->first_carry; obj != NULL; obj = obj_next )
   {
      obj_next = obj->next_in_carry_list;
      obj_from_char( obj );
      if( ( obj == quest_object ) && ( ch == quest_target ) )
      {
         obj->value[0] = UMAX( 1, obj->value[0] * quest_timer / 10 );
         obj->value[1] = UMAX( 1, obj->value[1] * quest_timer / 10 );
         obj->value[2] = UMAX( 1, obj->value[2] * quest_timer / 10 );
      }
      if( IS_OBJ_STAT(obj,ITEM_EXTRA_INVENTORY) )
         extract_obj( obj );
      else
         obj_to_obj( obj, corpse );
   }

   if( !IS_NPC( ch ) )
   {
      obj_to_room( corpse, ch->in_room );
      OBJ_DATA *this_corpse = new OBJ_DATA;
      this_corpse = corpse;
      corpse_list.push_back(this_corpse);
      save_corpses(  );
      return;
   }
   else
   {
      if( ch->act.test(ACT_INTELLIGENT ) )
      {
         obj_to_room( corpse, get_room_index( ROOM_VNUM_INT_HEAL ) );
      }
      else
      {
         obj_to_room( corpse, ch->in_room );
      }
      return;
   }

}



/*
 * Improved Death_cry contributed by Diavolo.
 */
void death_cry( CHAR_DATA * ch )
{
   /*
    * FIXME:  *ONLY* PKOK victims get a head loaded. 
    */
   /*
    * Make this give 'cry' message, don't load anything 
    */
   ROOM_INDEX_DATA *was_in_room;
   char *msg;
   int door;



   if( IS_NPC( ch ) )
      msg = "You hear something's death cry.";
   else
      msg = "You hear someone's death cry.";

   was_in_room = ch->in_room;
   if( ( was_in_room == NULL ) || ( was_in_room->vnum == 0 ) )
      return;

   for( door = 0; door < MAX_DIR; door++ )
   {
      EXIT_DATA *pexit;

      if( ( pexit = was_in_room->exit[door] ) != NULL && pexit->to_room != NULL && pexit->to_room != was_in_room )
      {
         ch->in_room = pexit->to_room;
         act( msg, ch, NULL, NULL, TO_ROOM );
      }
   }
   ch->in_room = was_in_room;

   return;
}



void raw_kill( CHAR_DATA * victim, char *argument )
{
   CHAR_DATA *check;
   list<CHAR_DATA *>::iterator li;
   char arg[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];

   one_argument( argument, arg );

   stop_fighting( victim, TRUE );
   if( victim == quest_target )
      quest_target = NULL;
   if( victim == quest_mob )
   {
      snprintf( buf, MSL, "Oh well, I guess the quest is over, since I am about to @@eDIE!!!!!@@N" );
      do_crusade( victim, buf );

      quest_mob = NULL;
   }



   if( !IS_VAMP( victim ) && ( victim->in_room != NULL ) )
      make_corpse( victim, arg );

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      check = *li;
      if( check->hunting == victim )
         end_hunt( check );
/*        unhunt(check);*/
   }


   if( IS_NPC( victim ) && !victim->act.test(ACT_INTELLIGENT ) )
   {
      victim->npcdata->pIndexData->killed++;
      kill_table[URANGE( 0, victim->level, MAX_LEVEL - 1 )].killed++;
      extract_char( victim, TRUE );
      return;

   }

   extract_char( victim, FALSE );
   while( victim->first_affect )
    affect_remove( victim, victim->first_affect );
   victim->affected_by = 0;
   victim->armor = 100;
   victim->position = POS_STANDING;
   victim->hit = 1;
   victim->mana = 1;
   victim->move = 1;
   save_char_obj( victim );
   if( IS_NPC( victim ) && victim->act.test(ACT_INTELLIGENT ) )
   {

      OBJ_DATA *my_corpse = NULL;

      for( my_corpse = victim->in_room->first_content; my_corpse; my_corpse = my_corpse->next_in_room )
      {
         if( ( my_corpse->item_type == ITEM_CORPSE_NPC )
             /*
              * && ( strstr( victim->short_descr, my_corpse->description ) != NULL ) 
              */  )
         {
            break;
         }
      }
      if( my_corpse == NULL )
      {
         char monbuf[MSL];
         snprintf( monbuf, MSL, "%s is looking for their corpse in room %d, but it's not there",
                  victim->name.c_str(), victim->in_room->vnum );
         monitor_chan( monbuf, MONITOR_MOB );
      }
      else
      {
         OBJ_DATA *obj;
         victim->is_quitting = false;
         do_stand( victim, "" );
         for( obj = my_corpse->first_content; obj; obj = obj->next_content )
         {
            obj_from_obj( obj );
            obj_to_char( obj, victim );
         }
         do_get( victim, "all from corpse" );
         do_sacrifice( victim, "corpse" );
         do_wear( victim, "all" );
      }
   }
   return;
}



void group_gain( CHAR_DATA * ch, CHAR_DATA * victim )
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *gch;
   CHAR_DATA *lch;
   int members = 0, tot_level = 0, xp = 0;
   float old_gain = 0, gain = 0, percent = 0;

   /*
    * Monsters don't get kill xp's or alignment changes.
    * P-killing doesn't help either.
    * Dying of mortal wounds or poison doesn't give xp to anyone!
    * -S- Mod: INTELLIGENT mobs *can* gain exp.  So there!
    */

   if( ( ( IS_NPC( ch ) )
         && ( ( !ch->act.test(ACT_INTELLIGENT ) )
              && ( ch->rider == NULL ) ) ) || ( !IS_NPC( victim ) ) || ( victim == ch ) )
      return;

   gain = victim->exp;  /* Now share this out... */
   if( victim->act.test(ACT_INTELLIGENT ) )
      gain = exp_for_mobile( victim->level, victim );

   for( gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room )
   {
      if( is_same_group( gch, ch ) )
      {
         members++;
         tot_level += gch->get_level("psuedo");
         gain *= 1.10; /* Group bonus */
      }
   }

   if( members == 0 )
   {
      bug( "Group_gain: members.", members );
      members = 1;
   }


   /*
    * Question is, if someone's exp is capped, do you dole out the rest
    * to the other people? or just reduce the total amount given to the
    * group (by not doling it out).
    * As it is, the total exp is reduced. I'll leave it that way.
    * 
    */

   lch = ( ch->leader != NULL ) ? ch->leader : ch;
   old_gain = gain;

   for( gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room )
   {
      OBJ_DATA *obj;
      OBJ_DATA *obj_next;
      int align;

      if( !is_same_group( gch, ch ) )
         continue;

      /*
       * Calc each char's xp seperately, but mult by ch->lev/tot_group_lev.
       */

      gain = old_gain;

      if( check_charm_aff(gch,CHARM_AFF_EXP) )
       gain *= ((100 + get_charm_bonus(gch,CHARM_AFF_EXP)) / 100);

      percent = (gch->get_level("psuedo") / tot_level);
      gain *= percent;

      if( gain < 0 )
       gain = 0;

      if( IS_ADEPT(gch) )
         gain /= 1000;

      /* Support changing exp on the fly. --Kline */
      gain *= sysdata.expmult;
      xp = static_cast<int>(gain);

      snprintf( buf, MSL, "You Receive %d Experience Points.\r\n", xp );
      send_to_char( buf, gch );
      gch->gain_exp(xp);

      if( !IS_NPC(gch) && (IS_VAMP( gch ) || IS_WOLF( gch )) )

         gch->pcdata->super->exp++;

      if( !IS_NPC( gch ) && ( gch->pcdata->learned[gsn_emotion_control] < 73 ) )
      {
         align = gch->alignment - ( victim->alignment * ( 80 - gch->pcdata->learned[gsn_emotion_control] ) / 100 );

         if( align > 500 )
            gch->alignment = UMIN( gch->alignment + ( align - 500 ) / 4, 1000 );
         else if( align < -500 )
            gch->alignment = UMAX( gch->alignment + ( align + 500 ) / 4, -1000 );
         else
            gch->alignment -= gch->alignment / 4;
      }
      for( obj = ch->first_carry; obj != NULL; obj = obj_next )
      {
         obj_next = obj->next_in_carry_list;
         if( obj->wear_loc == WEAR_NONE )
            continue;

         if( ( IS_OBJ_STAT( obj, ITEM_EXTRA_ANTI_EVIL ) && IS_EVIL( ch ) )
             || ( IS_OBJ_STAT( obj, ITEM_EXTRA_ANTI_GOOD ) && IS_GOOD( ch ) )
             || ( IS_OBJ_STAT( obj, ITEM_EXTRA_ANTI_NEUTRAL ) && IS_NEUTRAL( ch ) ) )
         {
            if( !IS_OBJ_STAT( obj, ITEM_EXTRA_NO_DROP ) )
            {
               act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR );
               act( "$n is zapped by $p and drops it.", ch, obj, NULL, TO_ROOM );
               obj_from_char( obj );
               obj_to_room( obj, ch->in_room );
            }
            else
            {
               act( "You feel $p slither out of your grasp, and back into your inventory!", ch, obj, NULL, TO_CHAR );
               act( "$p slithers out of $n's hands and back into $s inventory!", ch, obj, NULL, TO_ROOM );
               unequip_char( ch, obj );
            }
            return;
         }
      }
   }


   return;
}

struct dam_table_str
{
   int min_dam;
   const char *col;
   const char *vs;
   const char *vp;
   const char *str;
};
struct dam_table_str dam_table[] = {
   {2900, "@@l", "!!!!VIVESECT!!!!", "!!!!VIVESECTS!!!!", " into a living corpse with"},
   {2400, "@@m", "!!!!OBLITERATE!!!!", "!!!!OBLITERATES!!!!", " into disappearing particles with"},
   {2100, "@@m", "!!!!DEVASTATE!!!!", "!!!!DEVASTATES!!!!", "'s very existence with"},
   {1700, "@@m", "!!!!MUTILATE!!!!", "!!!!MUTILATES!!!!", " with"},
   {1450, "@@e", "****ANNIHILATE****", "****ANNIHILATES****", " into the bloody ground with"},
   {1375, "@@e", "***EVISCERATE***", "***EVISCERATES***", " into lots of small pieces with"},
   {1300, "@@e", "**DEMOLISH**", "**DEMOLISHES**", ", spraying $S guts all over the ground with"},
   {1200, "@@R", "*PULVERISE*", "*PULVERISES*", ", spreading blood and gore over the floor with"},
   {1150, "@@R", "THWACK", "THWACKS", ", leaving $M dazed and reeling with"},
   {1100, "@@R", "annihilate", "annihilates", " with"},
   {900, "@@p", "eviscerate", "eviscerates", " with"},
   {850, "@@p", "demolish", "demolishes", " with"},
   {800, "@@p", "pulverise", "pulverises", " with"},
   {700, "@@y", "thwack", "thwacks", " with"},
   {650, "@@y", "flay", "flays", "'s body mercillesly with"},
   {600, "@@y", "lacerate", "lacerates", " into a map of the mud with"},
   {500, "@@G", "maul", "mauls", " with great vengeance and FURIOUS anger from"},
   {450, "@@G", "tear", "tears", "'s skin into shreds with"},
   {400, "@@r", "rip apart", "rips apart", "'s skull with"},
   {350, "@@r", "remove", "removes", "'s ability to have children from"},
   {300, "@@c", "smash", "smashes", " forcefully with"},
   {250, "@@c", "cause", "causes", " to scream in agony from"},
   {200, "@@c", "laugh at", "laughs at", ", drawing trails of blood with"},
   {100, "@@a", "make", "makes", " wince in pain from"},
   {50, "@@a", "mark", "marks", " with an X from"},
   {28, "@@a", "wound", "wounds", " painfully with"},
   {20, "@@b", "graze", "grazes", " with"},
   {10, "@@b", "bonk", "bonks", " on the head with"},
   {7, "@@b", "scratch", "scratches", " roughly with"},
   {5, "@@b", "nick", "nicks", " gently with"},
   {2, "@@b", "tickle", "tickles", " softly with"},
   {0, "@@g", "fail to hit", "fails to hit", " with"}
};

void dam_message( CHAR_DATA * ch, CHAR_DATA * victim, float dam, int dt )
{
   static char *const attack_table[] = {
      "hit",
      "slice", "stab", "slash", "whip", "claw",
      "blast", "pound", "crush", "grip", "bite",
      "pierce", "suction", "tail whip"
   };

   char buf1[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH], buf3[MAX_STRING_LENGTH];
   const char *vs;   /* Singular */
   const char *vp;   /* Plural   */
   const char *str;  /* Strength */
   const char *attack;
   const char *col;
   char punct;
   int dam_table_num, check_dt;
   bool dead = FALSE;

   if( dam >= victim->hit )
      dead = TRUE;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      dt = TYPE_HIT + 5;   /* claw attack */
   }

   for( dam_table_num = 0; dam_table[dam_table_num].min_dam > dam; dam_table_num++ )
      ;

   col = dam_table[dam_table_num].col;
   vs = dam_table[dam_table_num].vs;
   vp = dam_table[dam_table_num].vp;
   str = dam_table[dam_table_num].str;




   punct = ( dam <= 64 ) ? '.' : '!';

   /*
    * Use combat skills, etc to make unarmed combat more fun :) 
    */

   if( dt == TYPE_MARTIAL )
   {


      col = "@@m";

      switch ( number_range( 0, 13 ) )
      {
         case 0:
            vs = "head punch";
            vp = "head punches";
            break;
         case 1:
            vs = "high kick";
            vp = "high kicks";
            break;
         case 2:
            vs = "vital kick";
            vp = "vital kick";
            break;
         case 3:
            vs = "head bash";
            vp = "head bashes";
            break;
         case 4:
            vs = "side kick";
            vp = "side kicks";
            break;
         case 5:
            vs = "elbow";
            vp = "elbows";
            break;
         case 6:
            vs = "body punch";
            vp = "body punches";
            break;
         case 7:
            vs = "low kick";
            vp = "low kicks";
            break;
         case 8:
            vs = "graze";
            vp = "grazes";
            break;
         case 9:
            vs = "knee smash";
            vp = "knee smashes";
            break;
         case 10:
            vs = "kidney punch";
            vp = "kidney punches";
            break;
         case 11:
            vs = "arm twist";
            vp = "arm twists";
            break;
         case 12:
            vs = "uppercut";
            vp = "uppercuts";
            break;
         case 13:
            vs = "rabbit punch";
            vp = "rabbit punches";
            break;
         default:
            vs = "foot sweep";
            vp = "foot sweeps";
            break;

      }
   }
   else if( dt == TYPE_HIT )
   {
      col = "@@m";
      if( dam == 0 )
      {
         vs = "miss";
         vp = "misses";
      }
      else if( dam <= 50 )
      {
         col = "@@c";
         vs = "tickle";
         vp = "tickles";
      }
      else if( dam <= 125 )
      {
         col = "@@a";
         vs = "glance";
         vp = "glances";
      }
      else if( dam <= 275 )
      {
         col = "@@G";
         vs = "slice";
         vp = "slices";
      }
      else if( dam <= 400 )
      {
         col = "@@r";
         vs = "strike";
         vp = "strikes";
      }
      else if( dam <= 550 )
      {
         col = "@@R";
         vs = "whallop";
         vp = "whallops";
      }
      else if( dam <= 800 )
      {
         col = "@@e";
         vs = "maul";
         vp = "mauls";
      }
      else
      {
         col = "@@2@@W";
         vs = "thwack";
         vp = "thwacks";
      }
      if( sysdata.shownumbers == TRUE )
      {
         snprintf( buf1, MSL, "%s$n %s%s $N%c@@g @@l(@@d%.0f@@l)@@N", col, col, vp, punct, dam );
         snprintf( buf2, MSL, "%sYou %s%s $N%c@@g @@l(@@d%.0f@@l)@@N", col, col, vs, punct, dam );
         snprintf( buf3, MSL, "%s$n %s%s you%c@@g @@l(@@d%.0f@@l)@@N", col, col, vp, punct, dam );
      }
      else
      {
         snprintf( buf1, MSL, "%s$n %s%s $N%c@@g", col, col, vp, punct );
         snprintf( buf2, MSL, "%sYou %s%s $N%c@@g", col, col, vs, punct );
         snprintf( buf3, MSL, "%s$n %s%s you%c@@g", col, col, vp, punct );
      }

   }
   else
   {
      if( dt >= 0 && dt < MAX_SKILL )
         attack = skill_table[dt].noun_damage;
      else if( dt >= TYPE_HIT && dt < TYPE_HIT + (int)sizeof( attack_table ) / (int)sizeof( attack_table[0] ) )
      {
         check_dt = UMAX( 0, ( dt - TYPE_HIT ) );
         check_dt = UMIN( check_dt, 13 );
         attack = attack_table[check_dt];
      }
      else
      {
         bug( "Dam_message: bad dt %d.", dt );
         dt = TYPE_HIT;
         attack = attack_table[0];
      }

      if( sysdata.shownumbers == TRUE )
      {
         if( dt != TYPE_MARTIAL )
         {
            snprintf( buf1, MSL, "%s$n %s%s $N%s%s $s %s%c@@g @@l(@@d%.0f@@l)@@N", col, col, vp, col, str, attack, punct, dam );
            snprintf( buf2, MSL, "%sYou %s%s $N%s%s your %s%c@@g @@l(@@d%.0f@@l)@@N", col, col, vs, col, str, attack, punct, dam );
            if( *str == '\'' )
               snprintf( buf3, MSL, "%s$n %s%s your%s%s $s %s%c@@g @@l(@@d%.0f@@l)@@N", col, col, vp, col, str + 2, attack, punct,
                        dam );
            else
               snprintf( buf3, MSL, "%s$n %s%s you%s%s $s %s%c@@g @@l(@@d%.0f@@l)@@N", col, col, vp, col, str, attack, punct, dam );
         }
         else
         {
            snprintf( buf1, MSL, "$n pulls a karate move on $N!" );
            snprintf( buf2, MSL, "You pull a karate move on $N!" );
            snprintf( buf3, MSL, "$n pulls a karate move on you!" );
         }
      }
      else
      {

         if( dt != TYPE_MARTIAL )
         {
            snprintf( buf1, MSL, "%s$n %s%s $N%s%s $s %s%c@@g", col, col, vp, col, str, attack, punct );
            snprintf( buf2, MSL, "%sYou %s%s $N%s%s your %s%c@@g", col, col, vs, col, str, attack, punct );
            if( *str == '\'' )
               snprintf( buf3, MSL, "%s$n %s%s your%s%s $s %s%c@@g", col, col, vp, col, str + 2, attack, punct );
            else
               snprintf( buf3, MSL, "%s$n %s%s you%s%s $s %s%c@@g", col, col, vp, col, str, attack, punct );
         }
         else
         {
            snprintf( buf1, MSL, "$n pulls a karate move on $N!" );
            snprintf( buf2, MSL, "You pull a karate move on $N!" );
            snprintf( buf3, MSL, "$n pulls a karate move on you!" );
         }

      }
   }
   act( buf1, ch, NULL, victim, TO_NOTVICT );
   if( ch->act.test(ACT_BLIND_PLAYER ) )
   {
      if( dam < victim->max_hit / 30 )
         act( "You glance $K", ch, NULL, victim, TO_CHAR );
      else if( dam < victim->max_hit / 20 )
         act( "You hit $K", ch, NULL, victim, TO_CHAR );
      else if( dam < victim->max_hit / 10 )
         act( "You nail $K", ch, NULL, victim, TO_CHAR );
      else
         act( "You thwack $K", ch, NULL, victim, TO_CHAR );
   }
   else
      act( buf2, ch, NULL, victim, TO_CHAR );
   if( victim->act.test(ACT_BLIND_PLAYER ) )
   {
      if( dam < victim->max_hit / 30 )
         act( "$k glances you", ch, NULL, victim, TO_VICT );
      else if( dam < victim->max_hit / 20 )
         act( "$k hits you", ch, NULL, victim, TO_VICT );
      else if( dam < victim->max_hit / 10 )
         act( "$k nails you", ch, NULL, victim, TO_VICT );
      else
         act( "$k thwacks you", ch, NULL, victim, TO_VICT );
   }
   else
      act( buf3, ch, NULL, victim, TO_VICT );

   if( dead )
   {
      int foo;

      foo = sizeof( attack_table ) / sizeof( attack_table[0] );
      death_message( ch, victim, ( dt - TYPE_HIT ), foo );
   }
   return;
}



/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm( CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * obj )
{
   int chance;

   set_fighting( ch, victim, TRUE );
   if( obj == NULL )
   {
      if( ( ( obj = get_eq_char( victim, WEAR_HOLD_HAND_L ) ) == NULL ) || ( obj->item_type != ITEM_WEAPON ) )
         if( ( ( obj = get_eq_char( victim, WEAR_HOLD_HAND_R ) ) == NULL ) || ( obj->item_type != ITEM_WEAPON ) )
         {
            send_to_char( "Your opponent is not wielding a weapon.\r\n", ch );
            return;
         }
   }
   if( IS_OBJ_STAT(obj,ITEM_EXTRA_NO_DISARM) )
      return;

   if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( IS_SHIFTED( victim ) || IS_RAGED( victim ) ) )
      return;


   /*
    * Check to see if victim is warrior, and if they deflect the disarm
    * In part, this idea comes from HiddenWorlds,
    * but it's also pretty common sense really ;)
    * Stephen
    */

   chance = IS_NPC( victim ) ? IS_SET( victim->npcdata->skills, MOB_NODISARM ) ? 90 : 0 : victim->pcdata->learned[gsn_nodisarm];

   if( number_percent(  ) < chance )
   {
      act( "You dodge $n's disarm attempt!", ch, NULL, victim, TO_VICT );
      act( "You fail to disarm $N!", ch, NULL, victim, TO_CHAR );
      act( "$N dodges $n's disarm attempt!", ch, NULL, victim, TO_NOTVICT );
      return;
   }

   act( "$n DISARMS you!", ch, NULL, victim, TO_VICT );
   act( "You disarm $N!", ch, NULL, victim, TO_CHAR );
   act( "$n DISARMS $N!", ch, NULL, victim, TO_NOTVICT );

   obj_from_char( obj );
   /*
    * if ( IS_NPC(victim) )
    * obj_to_char( obj, victim );
    * else 
    */

   obj_to_room( obj, victim->in_room );

   return;
}



/*
 * Trip a creature.
 * Caller must check for successful attack.
 */
void trip( CHAR_DATA * ch, CHAR_DATA * victim )
{
   int chance;

   if( ch->check_cooldown("trip") )
    return;
   chance = IS_NPC( victim ) ? IS_SET( victim->npcdata->skills, MOB_NOTRIP ) ? 75 : 0 : victim->pcdata->learned[gsn_notrip];

   /*
    * Check for no-trip 
    */
   if( number_percent(  ) < chance )
   {
      act( "You sidestep $n's attempt to trip you!", ch, NULL, victim, TO_VICT );
      act( "$N sidesteps your attempt to trip $M!", ch, NULL, victim, TO_CHAR );
      act( "$N sidesteps $n's attempt to trip $m!", ch, NULL, victim, TO_NOTVICT );
      return;
   }

   act( "$n trips you and you go down!", ch, NULL, victim, TO_VICT );
   act( "You trip $N and $N goes down!", ch, NULL, victim, TO_CHAR );
   act( "$n trips $N and $N goes down!", ch, NULL, victim, TO_NOTVICT );

   ch->set_cooldown("trip");
   victim->set_cooldown(COOLDOWN_OFF,3.00);
   victim->set_cooldown(COOLDOWN_DEF,3.00);
   victim->position = POS_RESTING;

   return;
}



DO_FUN(do_kill)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( ch->check_cooldown(COOLDOWN_OFF) )
    return;

   if( arg[0] == '\0' )
   {
      send_to_char( "Kill whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) && !( deathmatch ) )
   {
      if( !victim->act.test(ACT_KILLER ) && !victim->act.test(ACT_THIEF ) )
      {
         send_to_char( "You must MURDER a player.\r\n", ch );
         return;
      }
   }
   else
   {
      if( IS_AFFECTED( victim, AFF_CHARM ) && victim->master != NULL )
      {
         send_to_char( "You must MURDER a charmed creature.\r\n", ch );
         return;
      }
   }

   if( victim == ch )
   {
      send_to_char( "You hit yourself.  Ouch!\r\n", ch );
      one_hit( ch, ch, TYPE_UNDEFINED );
      return;
   }

   if( is_safe( ch, victim ) )
      return;

   if( IS_AFFECTED( ch, AFF_CHARM ) && ch->master == victim )
   {
      act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( ch->position == POS_FIGHTING )
   {
      send_to_char( "You do the best you can!\r\n", ch );
      return;
   }

   ch->set_cooldown(COOLDOWN_OFF,1.50);
   check_killer( ch, victim );
   one_hit( ch, victim, TYPE_UNDEFINED );
   return;
}

DO_FUN(do_target)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   if( ( !IS_NPC( ch ) ) && ( ch->pcdata->learned[gsn_target] < 65 ) )
   {
      send_to_char( "You are not trained enough in this skill!!\r\n", ch );
      return;
   }

   if( ch->check_cooldown("target") )
    return;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Target whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) && !( deathmatch ) )
   {
      if( !victim->act.test(ACT_KILLER ) && !victim->act.test(ACT_THIEF ) )
      {
         send_to_char( "You must MURDER a player.\r\n", ch );
         return;
      }
   }
   else
   {
      if( IS_AFFECTED( victim, AFF_CHARM ) && victim->master != NULL )
      {
         send_to_char( "You must MURDER a charmed creature.\r\n", ch );
         return;
      }
   }

   if( victim == ch )
   {
      send_to_char( "You hit yourself.  Ouch!\r\n", ch );
      one_hit( ch, ch, TYPE_UNDEFINED );
      return;
   }

   if( is_safe( ch, victim ) )
      return;

   if( IS_AFFECTED( ch, AFF_CHARM ) && ch->master == victim )
   {
      act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( ch->position == POS_FIGHTING )
   {
      send_to_char( "@@rTracking, tracking, tracking...@@eGOT HIM!!!@@N\r\n", ch );
      stop_fighting( ch, FALSE );
   }

   ch->set_cooldown("target");
   check_killer( ch, victim );
   one_hit( ch, victim, TYPE_UNDEFINED );
   return;
}



DO_FUN(do_murde)
{
   send_to_char( "If you want to MURDER, spell it out.\r\n", ch );
   return;
}



DO_FUN(do_murder)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if( ch->check_cooldown(COOLDOWN_OFF) )
    return;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Murder whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "Suicide is a mortal sin.\r\n", ch );
      return;
   }

   if( is_safe( ch, victim ) )
   {
      send_to_char( "Not here.\r\n", ch );
      return;
   }

   if( IS_AFFECTED( ch, AFF_CHARM ) && ch->master == victim )
   {
      act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( ch->position == POS_FIGHTING )
   {
      send_to_char( "You do the best you can!\r\n", ch );
      return;
   }

   ch->set_cooldown(COOLDOWN_OFF,1.50);
   snprintf( log_buf, (2 * MIL), "%s attacked by %s.\r\n", victim->name.c_str(), ch->name.c_str() );
   notify( log_buf, MAX_LEVEL - 2 );

   if( IS_NPC( ch ) || IS_NPC( victim )
       || !ch->act.test(ACT_PKOK ) || !victim->act.test(ACT_PKOK ) )
   {
      /*
       * If not pkok people, do yell. 
       */
      do_yell( victim, "Help! I'M BEING ATTACKED!!! ARRRGGGHHHHHH!" );
   }
   check_killer( ch, victim );
   one_hit( ch, victim, TYPE_UNDEFINED );
   return;
}



DO_FUN(do_backstab)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   int cnt;
   int best;
   int level;
   int mult;
   int chance;
   float dam;
   bool crack = FALSE;

   if( ch->check_cooldown("backstab") )
    return;

    /******************************************************************
     * Modified:  'damage' may now be called with sn = -1, in order to *
     * stop damage message being displayed.  Handle ALL the related    *
     * calculations here, including how many hits.  Check for critical *
     * hits too, as well as things like invis and sneak.               * 
      ******************************************************************/

   best = -1;
   level = 0;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You cannot do that while in this form.\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) )
   {
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         if( ch->lvl[cnt] >= skill_table[gsn_backstab].skill_level[cnt] && ch->lvl[cnt] > level )
         {
            best = cnt;
            level = ch->lvl[cnt];
         }
      if( ch->get_level("ass") > 0 )
         level = level + ch->get_level("ass") / 2;
   }
   else
   {
      best = ch->p_class;
      level = ch->level;
   }

   if( best == -1 )
   {
      send_to_char( "You better leave the assassin trade to thieves.\r\n", ch );
      return;
   }

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Backstab whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }
   if( victim == NULL )
      return;
   if( IS_NPC( victim ) && victim->act.test(ACT_NO_BODY ) )
   {
      act( "$N has no body to backstab!", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "How can you sneak up on yourself?\r\n", ch );
      return;
   }

   if( is_safe( ch, victim ) )
      return;

   if( ( obj = get_eq_char( ch, WEAR_HOLD_HAND_L ) ) == NULL || obj->value[3] != 11 )
      if( ( obj = get_eq_char( ch, WEAR_HOLD_HAND_R ) ) == NULL || obj->value[3] != 11 )
      {
         send_to_char( "You need to wield a piercing weapon.\r\n", ch );
         return;
      }

   if( victim->fighting != NULL )
   {
      send_to_char( "You can't backstab a fighting person.\r\n", ch );
      return;
   }

   chance = ( IS_NPC( ch ) ? 50 : ch->pcdata->learned[gsn_backstab] / 2 );

   /*
    * Handle Modifiers -- chance will affect thac, etc
    */

   if( !IS_AWAKE( victim ) )
      chance += 75;

   if( IS_AFFECTED( victim, AFF_SNEAK ) || item_has_apply( victim, ITEM_APPLY_SNEAK ) )
      chance -= 10;

   if( IS_AFFECTED( ch, AFF_SNEAK ) || item_has_apply( ch, ITEM_APPLY_SNEAK ) )
      chance += 20;

   if( IS_AFFECTED( ch, AFF_INVISIBLE ) || item_has_apply( ch, ITEM_APPLY_INV ) )
      chance += ( IS_AFFECTED( victim, AFF_DETECT_INVIS ) ? -20 : 20 );

   if( IS_AFFECTED( victim, AFF_INVISIBLE ) || item_has_apply( victim, ITEM_APPLY_INV ) )
      chance -= 10;

   if( ch->get_level("psuedo") >= victim->get_level("psuedo") )
      chance += 10;
   else
      chance -= 10;

   /*
    * Work out multiplier 
    */
   mult = 1 + ( level >= 20 ) + ( level >= 50 ) + ( level >= 90 ) + ( level >= 120 );

   /*
    * Work out damage 
    */
   dam = number_range( obj->value[1], obj->value[2] );
   dam += number_range( level / 2, level * 2 ) + GET_DAMROLL( ch ) / 2;
   dam *= mult;
   check_killer( ch, victim );
   if( chance < number_percent(  ) )
   {
      /*
       * Miss 
       */
      act( "$n tries to backstab $N, but misses!", ch, NULL, victim, TO_NOTVICT );
      act( "You try to backstab $N, but miss!", ch, NULL, victim, TO_CHAR );
      act( "$N tries to backstab you, but misses!", victim, NULL, ch, TO_CHAR );
      damage( ch, victim, 0, -1 );
   }
   else
   {
     /*
      * HIT!
      */

      if( !IS_NPC( ch ) && ch->pcdata->order[0] == 2 && number_percent( ) == chance )
      {
       crack = TRUE;
       dam *= 2;
      }

      dam = combat_damcap(ch,victim,dam,gsn_backstab);

      if( sysdata.shownumbers == TRUE )
      {
         char actbuf[MSL];
         snprintf( actbuf, MSL, "$n places $p into the back of $N!! @@l(@@W%.0f@@l)@@N", dam );
         act( actbuf, ch, obj, victim, TO_NOTVICT );
         snprintf( actbuf, MSL, "You place $p into the back of $N!! @@l(@@W%.0f@@l)@@N", dam );
         act( actbuf, ch, obj, victim, TO_CHAR );
         snprintf( actbuf, MSL, "$N places $p into your back!! @@l(@@W%.0f@@l)@@N", dam );
         act( actbuf, victim, obj, ch, TO_CHAR );
      }
      else
      {
         act( "$n places $p into the back of $N!!", ch, obj, victim, TO_NOTVICT );
         act( "You place $p into the back of $N!!", ch, obj, victim, TO_CHAR );
         act( "$N places $p into your back.  OUCH!", victim, obj, ch, TO_CHAR );
      }
      if( crack )
         send_to_room( "You hear a large CRACK!\r\n", ch->in_room );

      damage( ch, victim, dam, gsn_backstab );
   }

   ch->set_cooldown("backstab");
   return;
}



DO_FUN(do_flee)
{
   ROOM_INDEX_DATA *was_in;
   ROOM_INDEX_DATA *now_in;
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];
   int attempt;
   int cost;   /* xp cost for a flee */

   if( ch->check_cooldown(COOLDOWN_DEF) )
    return;

   if( ( victim = ch->fighting ) == NULL )
   {
      if( ch->position == POS_FIGHTING )
         ch->position = POS_STANDING;
      send_to_char( "You aren't fighting anyone.\r\n", ch );
      return;
   }

   /*
    * Check if mob will "allow" ch to flee...
    */

   ch->set_cooldown(COOLDOWN_DEF,4.50);
   if( IS_CASTING(ch) )
    do_stop(ch,"");
   if( victim->act.test(ACT_NO_FLEE ) && !IS_NPC( ch ) && IS_NPC( victim ) )
   {
      send_to_char( "You attempt to flee from battle, but fail!\r\n", ch );
      snprintf( buf, MSL, "%s tells you 'No way will you escape ME!!'\r\n", ch->get_name(victim) );
      send_to_char( buf, ch );
      return;
   }
   if( deathmatch )
   {
      if( ch->hit < 200 )
      {
         send_to_char( "@@eYou are in too much pain!!@@N\r\n", ch );
         return;
      }
      else
      {
         send_to_char( "@@eYour enemy nails you one more time!@@N\r\n", ch );
         ch->hit -= 150;
      }
   }

   was_in = ch->in_room;
   for( attempt = 0; attempt < 6; attempt++ )
   {
      EXIT_DATA *pexit;
      int door;

      door = number_door(  );
      if( ( pexit = was_in->exit[door] ) == 0
          || pexit->to_room == NULL
          || pexit->exit_info.test(EX_CLOSED)
          || ( IS_NPC( ch )
               && ( pexit->to_room->room_flags.test(RFLAG_NO_MOB)
                    || ( ch->act.test(ACT_STAY_AREA ) && pexit->to_room->area != ch->in_room->area ) ) ) )
         continue;

      move_char( ch, door, TRUE );
      if( ( now_in = ch->in_room ) == was_in )
         continue;

      ch->in_room = was_in;
      act( "$n has fled!", ch, NULL, NULL, TO_ROOM );
      ch->in_room = now_in;

      if( !IS_NPC( ch ) )
      {
         cost = number_range( ch->exp / 15, ch->exp / 10 );
         if( IS_ADEPT(ch) )
            cost /= 1000;
         cost = UMIN( cost, ch->exp );
         snprintf( buf, MSL, "You flee from combat!  You lose %d exps.\r\n", cost );
         send_to_char( buf, ch );
         cost *= -1;
         ch->gain_exp(cost);
      }
      if( ( ch->fighting != NULL ) && ( AI_MOB( ch->fighting ) ) )
      {
         ch->fighting->npcdata->ngroup->state = GRP_STATE_HUNTING;
         ch->fighting->npcdata->ngroup->leader->hunting = ch;
      }
      stop_fighting( ch, TRUE );
      /*
       * 75% chance that mobs will hunt fleeing people. -- Alty 
       */
      if( IS_NPC( victim ) && !victim->act.test(ACT_SENTINEL ) && number_bits( 2 ) > 0 )
         set_hunt( victim, NULL, ch, NULL, HUNT_WORLD | HUNT_INFORM | HUNT_OPENDOOR, HUNT_MERC | HUNT_CR );
      return;
   }

   cost = ch->get_level("psuedo") * 3;
   if( IS_ADEPT(ch) )
      cost = 0;
   cost = UMIN( cost, ch->exp );
   snprintf( buf, MSL, "You failed!  You lose %d exps.\r\n", cost );
   send_to_char( buf, ch );
   cost *= -1;
   ch->gain_exp(cost);
   return;
}


DO_FUN(do_rescue)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   CHAR_DATA *fch;
   int best;
   int cnt;

   if( ch->check_cooldown("rescue") )
    return;

   best = -1;

   if( !IS_NPC( ch ) )
   {
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         if( ch->lvl[cnt] >= skill_table[gsn_rescue].skill_level[cnt] && ch->lvl[cnt] >= best )
            best = cnt;
   }
   else
      best = ch->level;

   if( best == -1 )
   {
      send_to_char( "You don't know how to rescue!!\r\n", ch );
      return;
   }

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Rescue whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "What about fleeing instead?\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && IS_NPC( victim ) )
   {
      send_to_char( "Doesn't need your help!\r\n", ch );
      return;
   }

   if( ch->fighting == victim )
   {
      send_to_char( "Too late.\r\n", ch );
      return;
   }

   if( ( fch = victim->fighting ) == NULL )
   {
      send_to_char( "That person is not fighting right now.\r\n", ch );
      return;
   }

   ch->set_cooldown("rescue");
   if( !IS_NPC( ch ) && number_percent(  ) > ch->pcdata->learned[gsn_rescue] )
   {
      send_to_char( "You fail the rescue.\r\n", ch );
      return;
   }

   act( "You rescue $N!", ch, NULL, victim, TO_CHAR );
   act( "$n rescues you!", ch, NULL, victim, TO_VICT );
   act( "$n rescues $N!", ch, NULL, victim, TO_NOTVICT );

   stop_fighting( fch, FALSE );
   stop_fighting( victim, FALSE );

   set_fighting( ch, fch, TRUE );
   set_fighting( fch, ch, FALSE );
   return;
}


DO_FUN(do_disarm)
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   int percent;
   int best;
   int cnt;

   if( ch->check_cooldown("disarm") )
    return;

   best = -1;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You cannot do that while in this form.\r\n", ch );
      return;
   }


   if( !IS_NPC( ch ) )
   {
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         if( ch->lvl[cnt] >= skill_table[gsn_disarm].skill_level[cnt] && ch->lvl[cnt] >= best )
            best = cnt;
   }
   else
      best = ch->level;

   if( best == -1 )
   {
      send_to_char( "You don't know how to disarm!\r\n", ch );
      return;
   }

   if( ( !IS_WEAPON( get_eq_char( ch, WEAR_HOLD_HAND_L ) ) ) && ( !IS_WEAPON( get_eq_char( ch, WEAR_HOLD_HAND_R ) ) ) )
   {
      send_to_char( "You must wield a weapon to disarm.\r\n", ch );
      return;
   }

   if( ( victim = ch->fighting ) == NULL )
   {
      send_to_char( "You aren't fighting anyone.\r\n", ch );
      return;
   }
   if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( IS_SHIFTED( victim ) || IS_RAGED( victim ) ) )
   {
      send_to_char( "Try slicing off their claws instead!\r\n", ch );
      return;
   }

   if( ( ( obj = get_eq_char( victim, WEAR_HOLD_HAND_L ) ) == NULL ) || ( obj->item_type != ITEM_WEAPON ) )
      if( ( ( obj = get_eq_char( victim, WEAR_HOLD_HAND_R ) ) == NULL ) || ( obj->item_type != ITEM_WEAPON ) )
      {
         send_to_char( "Your opponent is not wielding a weapon.\r\n", ch );
         return;
      }
   if( IS_OBJ_STAT(obj,ITEM_EXTRA_NO_DISARM) )
      return;


   ch->set_cooldown("disarm");
   percent = number_percent(  ) + victim->level - ch->level;
   if( IS_NPC( ch ) || percent < ch->pcdata->learned[gsn_disarm] * 2 / 3 )
      disarm( ch, victim, obj );
   else
      send_to_char( "You failed.\r\n", ch );
   return;
}

DO_FUN(do_circle)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   int cnt;
   int best;
   int level;
   int mult;
   float chance;
   float dam;
   bool crack = FALSE;

   if( ch->check_cooldown("circle") )
    return;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You cannot do that while in this form.\r\n", ch );
      return;
   }


    /******************************************************************
     * Modified:  'damage' may now be called with sn = -1, in order to *
     * stop damage message being displayed.  Handle ALL the related    *
     * calculations here, including how many hits.  Check for critical *
     * hits too, as well as things like invis and sneak.               * 
      ******************************************************************/

   best = -1;
   level = 0;


   if( !IS_NPC( ch ) )
   {
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         if( ch->lvl[cnt] >= skill_table[gsn_circle].skill_level[cnt] && ch->lvl[cnt] > level )
         {
            best = cnt;
            level = ch->lvl[cnt];
         }
      if( ch->get_level("ass") > 0 )
         level += ch->get_level("ass") / 2;
   }
   else
   {
      best = ch->p_class;
      level = ch->level;
   }

   if( best == -1 )
   {
      send_to_char( "You better leave the assassin trade to thieves.\r\n", ch );
      return;
   }

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Backstab whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }
   if( victim == NULL )
      return;

   if( IS_NPC( victim ) && victim->act.test(ACT_NO_BODY ) )
   {
      act( "$N has no body to backstab!", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "How can you sneak up on yourself?\r\n", ch );
      return;
   }

   if( is_safe( ch, victim ) )
      return;

   if( ( obj = get_eq_char( ch, WEAR_HOLD_HAND_L ) ) == NULL || obj->value[3] != 11 )
      if( ( obj = get_eq_char( ch, WEAR_HOLD_HAND_R ) ) == NULL || obj->value[3] != 11 )
      {
         send_to_char( "You need to wield a piercing weapon.\r\n", ch );
         return;
      }

   if( victim->fighting == NULL )
   {
      send_to_char( "You're victim must be fighting!\r\n", ch );
      return;
   }

   chance = ( IS_NPC( ch ) ? 50 : ch->pcdata->learned[gsn_circle] / 4.5 );

   /*
    * Handle Modifiers -- chance will affect thac, etc
    */

   if( !IS_AWAKE( victim ) )
      chance += 75;

   if( IS_AFFECTED( victim, AFF_SNEAK ) || item_has_apply( victim, ITEM_APPLY_SNEAK ) )
      chance -= 10;

   if( IS_AFFECTED( ch, AFF_SNEAK ) || item_has_apply( ch, ITEM_APPLY_SNEAK ) )
      chance += 10;

   if( IS_AFFECTED( ch, AFF_INVISIBLE ) || item_has_apply( ch, ITEM_APPLY_INV ) )
      chance += ( IS_AFFECTED( victim, AFF_DETECT_INVIS ) ? -20 : 20 );

   if( IS_AFFECTED( victim, AFF_INVISIBLE ) || item_has_apply( victim, ITEM_APPLY_INV ) )
      chance -= 10;

   if( ch->get_level("psuedo") >= victim->get_level("psuedo") )
      chance += 10;
   else
      chance -= 10;

   /*
    * Work out multiplier 
    */
   mult = 1 + ( level >= 20 ) + ( level >= 50 ) + ( level >= 90 ) + ( level >= 120 );

   /*
    * Work out damage 
    */
   dam = number_range( obj->value[1], obj->value[2] );
   dam += number_range( level / 2, level * 2 ) + GET_DAMROLL( ch ) / 2;
   dam *= mult;
   if( victim != ch->fighting )
      check_killer( ch, victim );
   if( chance < number_percent(  ) )
   {
      /*
       * Miss 
       */
      act( "$n tries to backstab $N, but misses!", ch, NULL, victim, TO_NOTVICT );
      act( "You try to backstab $N, but miss!", ch, NULL, victim, TO_CHAR );
      act( "$N tries to backstab you, but misses!", victim, NULL, ch, TO_CHAR );
      damage( ch, victim, 0, -1 );
   }
   else
   {
      /*
       * HIT! 
       */

      if( !IS_NPC( ch ) && ch->pcdata->order[0] == 2 && number_percent(  ) == chance )
      {
       crack = TRUE;
       dam *= 2;
      }

      dam = combat_damcap(ch,victim,dam,gsn_circle);

      if( sysdata.shownumbers == TRUE )
      {
         char actbuf[MSL];
         snprintf( actbuf, MSL, "$n places $p into the back of $N!! @@l(@@W%.0f@@l)@@N", dam );
         act( actbuf, ch, obj, victim, TO_NOTVICT );
         snprintf( actbuf, MSL, "You place $p into the back of $N!! @@l(@@W%.0f@@l)@@N", dam );
         act( actbuf, ch, obj, victim, TO_CHAR );
         snprintf( actbuf, MSL, "$N places $p into your back!! @@l(@@W%.0f@@l)@@N", dam );
         act( actbuf, victim, obj, ch, TO_CHAR );
      }
      else
      {
         act( "$n places $p into the back of $N!!", ch, obj, victim, TO_NOTVICT );
         act( "You place $p into the back of $N!!", ch, obj, victim, TO_CHAR );
         act( "$N places $p into your back.  OUCH!", victim, obj, ch, TO_CHAR );
      }

      if( crack )
         send_to_room( "You hear a large CRACK!\r\n", ch->in_room );

      damage( ch, victim, dam, gsn_circle );
   }

   ch->set_cooldown("circle");
   return;
}



DO_FUN(do_trip)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int best;
/*    int cnt;  */

   if( ch->check_cooldown("trip") )
    return;

   best = -1;

   if( !IS_NPC( ch ) )
   {
      /*
       * for ( cnt = 0; cnt < MAX_CLASS; cnt++ )
       * if ( ch->lvl[cnt] >= skill_table[gsn_trip].skill_level[cnt] 
       * && ch->lvl[cnt] >= best )
       * best = cnt;  
       */
      if( ch->pcdata->learned[gsn_trip] > 75 )
         best = UMAX( 79, ch->get_level("psuedo") );
   }
   else
      best = ch->level;

   if( best == -1 )
   {
      send_to_char( "You don't know of such a skill.\r\n", ch );
      return;
   }

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Trip whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "That wouldn't even be funny.\r\n", ch );
      return;
   }

   if( is_safe( ch, victim ) )
      return;

   if( victim->fighting == NULL )
   {
      send_to_char( "You can't trip someone who isn't fighting.\r\n", ch );
      return;
   }

   check_killer( ch, victim );

   if( number_percent(  ) < ( IS_NPC( ch ) ? 50 : 2 * ch->pcdata->learned[gsn_trip] ) )
   {
      check_killer( ch, victim );
      trip( ch, victim );
      ch->set_cooldown("trip");

   }

   return;
}

DO_FUN(do_dirt)
{

   AFFECT_DATA af;
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int best;
   /*
    * int cnt;   
    */

   if( ch->check_cooldown("dirt") )
    return;

   best = -1;

   if( !IS_NPC( ch ) )
   {
      /*
       * for ( cnt = 0; cnt < MAX_CLASS; cnt++ )
       * if ( ch->lvl[cnt] >= skill_table[gsn_dirt].skill_level[cnt] 
       * && ch->lvl[cnt] >= best )
       * best = cnt;  
       */
      if( ch->pcdata->learned[gsn_dirt] > 75 )
         best = UMAX( 79, ch->get_level("psuedo") );
   }
   else
      best = ch->level;

   if( best == -1 )
   {
      send_to_char( "You don't know of such a skill.\r\n", ch );
      return;
   }

   one_argument( argument, arg );

   if( arg[0] == '\0' && ch->fighting == NULL )
   {
      send_to_char( "Kick dirt at whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL && ch->fighting == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( victim == NULL )
      victim = ch->fighting;

   if( victim == ch )
   {
      send_to_char( "That wouldn't be too smart, would it??.\r\n", ch );
      return;
   }

   if( is_safe( ch, victim ) )
      return;

   if( victim->fighting == NULL )
   {
      send_to_char( "Try doing this to them, when fighting...\r\n", ch );
      return;
   }

   if( IS_AFFECTED( victim, AFF_BLIND ) )
      return;

   ch->set_cooldown("dirt");

   if( number_percent(  ) > ( IS_NPC( ch ) ? 50 : ch->pcdata->learned[gsn_dirt] ) )
   {
      act( "You kick dirt at $M but miss!", ch, NULL, victim, TO_CHAR );
      act( "$n kicks dirt at you but misses!", ch, NULL, victim, TO_VICT );
      act( "$n kicks dirt at $N but misses!", ch, NULL, victim, TO_NOTVICT );
      return;
   }
   else
   {
      act( "You kick dirt at $S eyes!", ch, NULL, victim, TO_CHAR );
      act( "$n kicks dirt in your eyes!", ch, NULL, victim, TO_VICT );
      act( "$n kicks dirt at $N's eyes!", ch, NULL, victim, TO_NOTVICT );

      af.type = skill_lookup( "blindness" );
      af.location = APPLY_HITROLL;
      af.modifier = -2;
      af.duration = 1;
      af.bitvector = AFF_BLIND;
      affect_to_char( victim, &af );
   }
   return;
}

DO_FUN(do_bash)
{

   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int best;
/*    int cnt;  */

   if( ch->check_cooldown("bash") )
    return;

   best = -1;

   if( !IS_NPC( ch ) )
   {
      /*
       * for ( cnt = 0; cnt < MAX_CLASS; cnt++ )
       * if ( ch->lvl[cnt] >= skill_table[gsn_dirt].skill_level[cnt] 
       * && ch->lvl[cnt] >= best )
       * best = cnt;  
       */
      if( ch->pcdata->learned[gsn_bash] > 75 )
         best = UMAX( 79, ch->get_level("psuedo") );
   }
   else
      best = ch->level;

   if( best == -1 )
   {
      send_to_char( "You don't know of such a skill.\r\n", ch );
      return;
   }

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Bash whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "Forget it!\r\n", ch );
      return;
   }

   if( is_safe( ch, victim ) )
      return;

   if( victim->fighting == NULL )
   {
      send_to_char( "It might help if you were fighting.....\r\n", ch );
      return;
   }

   ch->set_cooldown("bash");

   if( ( IS_NPC( ch ) && ( number_percent(  ) > 75 + ( ch->level ) / 2 ) )
       || ( !IS_NPC( ch ) && ( 2 * number_percent(  ) > ch->pcdata->learned[gsn_bash] ) ) )
   {
      act( "Your bash misses $M, and you fall!", ch, NULL, victim, TO_CHAR );
      act( "$n trys to bash you, misses, and falls!", ch, NULL, victim, TO_VICT );
      act( "$n trys to bash $N but misses, and falls!", ch, NULL, victim, TO_NOTVICT );
      return;
   }
   else
   {
      act( "Your bash sends $M flying!", ch, NULL, victim, TO_CHAR );
      act( "$n bashes you, sending you flying!", ch, NULL, victim, TO_VICT );
      act( "$n's bash sends $N's flying!", ch, NULL, victim, TO_NOTVICT );

      /*
       * If victim very weak, set pos to stunned, stop fighting 
       */
      if( victim->hit < ( victim->max_hit / 50 ) + 1 )
      {
         act( "$N stays on the floor.", ch, NULL, victim, TO_CHAR );
         act( "You are unable to get up.", ch, NULL, victim, TO_VICT );
         act( "$N stays on the floor.", ch, NULL, victim, TO_NOTVICT );
         stop_fighting( victim, TRUE );   /* MAG: might del this? -S- */
         victim->position = POS_RESTING;
         update_pos( victim );
      }
      else
      {
         /*
          * Do some damage instead... 
          */
         damage( ch, victim, ( best + 12 ) * 2, -1 );
      }

   }

   return;

}


DO_FUN(do_berserk)
{
   AFFECT_DATA af;
   int best;
   int level;
   bool prime;

   if( ch->check_cooldown("berserk") )
    return;

   prime = FALSE;
   best = -1;


   if( IS_NPC( ch ) )
      return;

   if( ch->pcdata->order[0] == 2 )
      prime = TRUE;

   level = ch->get_level("war");


   if( ch->fighting == NULL )
   {
      send_to_char( "You can only go berserk when fighting!\r\n", ch );
      return;
   }

   if( is_affected( ch, gsn_berserk ) )
   {
      send_to_char( "You are already berserk!!\r\n", ch );
      return;
   }

   if( ch->pcdata->learned[gsn_berserk] == 0 )
   {
      send_to_char( "You don't know how to use this skill!\r\n", ch );
      return;
   }

   ch->set_cooldown("berserk");


   if( number_percent(  ) < ( ch->pcdata->learned[gsn_berserk] / 2 ) )
   {
      /*
       * Failure 
       */
      act( "$n calls on the Dark Powers, but they don't answer!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You call on the Dark Powers.  They don't answer!", ch );
      return;
   }



   af.type = gsn_berserk;
   af.duration = ( best == 2 ) ? 4 : 2;
   af.location = APPLY_AC;
   af.modifier = ( best == 2 ) ? level : ( level * 2 );
   af.bitvector = 0;
   affect_to_char( ch, &af );

   af.location = APPLY_HITROLL;
   af.modifier = ( prime )
      ? ( 10 + ( level >= 20 ) + ( level >= 40 ) + ( level >= 60 ) + ( level >= 80 ) )
      : ( 5 + ( level >= 25 ) + ( level >= 50 ) );
   affect_to_char( ch, &af );

   af.location = APPLY_DAMROLL;
   af.modifier = ( prime )
      ? ( 12 + ( level >= 20 ) + ( level >= 40 ) + ( level >= 60 ) + ( level >= 80 ) )
      : ( 2 + ( level >= 25 ) + ( level >= 50 ) );
   affect_to_char( ch, &af );


   af.location = APPLY_SAVING_SPELL;
   af.modifier = ( prime )
      ? ( 10 - ( level >= 20 ) - ( level >= 40 ) - ( level >= 60 ) - ( level >= 80 ) )
      : ( 20 - ( level >= 25 ) - ( level >= 50 ) );
   affect_to_char( ch, &af );

   act( "$n calls on the Dark Powers, who answer!!!", ch, NULL, NULL, TO_ROOM );
   send_to_char( "You call on the Dark Powers.  They answer!!!\r\n", ch );
   return;
}

DO_FUN(do_punch)
{

   CHAR_DATA *victim;
   int dam;
   bool prime;
   int chance;

   if( ch->check_cooldown("punch") )
    return;

   prime = FALSE;


   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_punch] == 0 )
   {
      send_to_char( "You are not trained in this skill!\r\n", ch );
      return;
   }



   if( ( ( victim = get_char_room( ch, argument ) ) == NULL ) && ch->fighting == NULL )
   {
      send_to_char( "No such victim!\r\n", ch );
      return;
   }


   if( victim == NULL )
      victim = ch->fighting;

   if( !IS_NPC( ch ) && ch->pcdata->order[0] == 3 )
      prime = TRUE;

   if( IS_NPC( ch ) )
      dam = number_range( ch->level / 3, ch->level / 2 );
   else
      dam = number_range( ch->get_level("war") / 2, ch->get_level("war") * ( prime ? 2 : 1 ) );

   chance = ( IS_NPC( ch ) ? 50 : ch->pcdata->learned[gsn_punch] / 2 );

   chance += ( ch->get_level("war") - victim->level );

   ch->set_cooldown("punch");

   check_killer( ch, victim );
   if( number_percent(  ) < chance )
   {

      if( sysdata.shownumbers == TRUE )
      {
         /*
          * HIT! 
          */
         char actbuf[MSL];
         snprintf( actbuf, MSL, "$n punches $N!! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, ch, NULL, victim, TO_NOTVICT );
         snprintf( actbuf, MSL, "$N punches you really hard!! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, victim, NULL, ch, TO_CHAR );
         snprintf( actbuf, MSL, "You punch $N!! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, ch, NULL, victim, TO_CHAR );
      }
      else
      {
         act( "$n punches $N really hard!", ch, NULL, victim, TO_NOTVICT );
         act( "$N punches you really hard!", victim, NULL, ch, TO_CHAR );
         act( "You punch $N really hard!", ch, NULL, victim, TO_CHAR );
      }
      damage( ch, victim, dam, -1 );
   }
   else
   {
      /*
       * MISS 
       */
      act( "$n tries to punch $N, but misses!", ch, NULL, victim, TO_NOTVICT );
      act( "$N tries to punch you, but misses!", victim, NULL, ch, TO_CHAR );
      act( "You try to punch $N, but miss!", ch, NULL, victim, TO_CHAR );
      damage( ch, victim, 0, -1 );
   }
   return;
}


DO_FUN(do_headbutt)
{

   CHAR_DATA *victim;
   int dam;
   bool prime;
   int chance;

   if( ch->check_cooldown("headbutt") )
    return;

   prime = FALSE;


   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_headbutt] == 0 )
   {
      send_to_char( "You are not trained in this skill!\r\n", ch );
      return;
   }



   if( ( ( victim = get_char_room( ch, argument ) ) == NULL ) && ch->fighting == NULL )
   {
      send_to_char( "No such victim!\r\n", ch );
      return;
   }


   if( victim == NULL )
      victim = ch->fighting;

   if( victim == ch )
   {
      send_to_char( "You can't reach!\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && ch->pcdata->order[0] == 3 )
      prime = TRUE;

   if( IS_NPC( ch ) )
      dam = number_range( ch->level / 3, ch->level / 2 );
   else
      dam = number_range( ch->get_level("war") / 2, ch->get_level("war") * ( prime ? 2 : 1 ) );

   chance = ( IS_NPC( ch ) ? 50 : ch->pcdata->learned[gsn_headbutt] / 2 );

   chance += ( ch->get_level("war") - victim->level );

   ch->set_cooldown("headbutt");


   if( number_percent(  ) < chance )
   {
      /*
       * HIT 
       */
      check_killer( ch, victim );
      if( sysdata.shownumbers == TRUE )
      {
         /*
          * HIT! 
          */
         char actbuf[MSL];
         snprintf( actbuf, MSL, "$n headbutts $N in the face! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, ch, NULL, victim, TO_NOTVICT );
         snprintf( actbuf, MSL, "$N headbutts you in the face! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, victim, NULL, ch, TO_CHAR );
         snprintf( actbuf, MSL, "You headbutt $N in the face! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, ch, NULL, victim, TO_CHAR );
      }
      else
      {
         act( "$n headbutts $N in the face!", ch, NULL, victim, TO_NOTVICT );
         act( "$N headbutts you in the face!", victim, NULL, ch, TO_CHAR );
         act( "You headbutt $N in the face!", ch, NULL, victim, TO_CHAR );
      }

      damage( ch, victim, ( dam * 4 ) / 5, -1 );
   }
   else
   {
      /*
       * MISS 
       */
      check_killer( ch, victim );
      act( "$n tries to headbutt $N, but misses!", ch, NULL, victim, TO_NOTVICT );
      act( "$N tries to headbutt you, but misses!", victim, NULL, ch, TO_CHAR );
      act( "You try to headbutt $N, but miss!", ch, NULL, victim, TO_CHAR );
      damage( ch, victim, 0, -1 );
   }
   return;
}


DO_FUN(do_charge)
{

   CHAR_DATA *victim;
   float dam;
   bool prime;
   int chance;

   if( ch->check_cooldown("charge") )
    return;

   prime = FALSE;


   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_charge] == 0 )
   {
      send_to_char( "You are not trained in this skill!\r\n", ch );
      return;
   }

   if( ( IS_NPC( ch ) ) && ( ch->get_level("psuedo") < 80 ) )
      return;

   if( ( ( victim = get_char_room( ch, argument ) ) == NULL ) && ch->fighting == NULL )
   {
      send_to_char( "No such victim!\r\n", ch );
      return;
   }


   if( victim == NULL )
      victim = ch->fighting;

   if( victim == ch )
   {
      send_to_char( "You can't reach!\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && ch->pcdata->order[0] == 3 )
      prime = TRUE;


   dam = number_range( ch->get_level("psuedo"), ch->get_level("psuedo") * 3 );

   if( !IS_NPC( ch ) )
      chance = ch->pcdata->learned[gsn_charge] / 2;
   else
      chance = 50;

   chance += ( ( ch->get_level("psuedo") - ( victim->get_level("psuedo") - 30 ) ) / 2 );

   ch->set_cooldown("charge");

   check_killer( ch, victim );

   if( number_percent(  ) < chance )
   {
      /*
       * HIT 
       */

      dam = combat_damcap(ch,victim,dam,gsn_charge);

      if( sysdata.shownumbers == TRUE )
      {
         /*
          * HIT! 
          */
         char actbuf[MSL];
         snprintf( actbuf, MSL, "@@a$n @@acharges $N@@a, and knocks them over!@@N @@l(@@W%.0f@@l)@@N", dam );
         act( actbuf, ch, NULL, victim, TO_NOTVICT );
         snprintf( actbuf, MSL, "@@a$N @@acharges right into you!@@N @@l(@@W%.0f@@l)@@N", dam );
         act( actbuf, victim, NULL, ch, TO_CHAR );
         snprintf( actbuf, MSL, "@@aYou charge right into $N@@a, and knock him over!@@N @@l(@@W%.0f@@l)@@N", dam );
         act( actbuf, ch, NULL, victim, TO_CHAR );
      }
      else
      {
         act( "@@a$n @@acharges $N@@a, and knocks them over!@@N", ch, NULL, victim, TO_NOTVICT );
         act( "@@a$N @@acharges right into you!@@N", victim, NULL, ch, TO_CHAR );
         act( "@@aYou charge right into $N@@a, and knock him over!@@N", ch, NULL, victim, TO_CHAR );
      }

      damage( ch, victim, dam, gsn_charge );
   }
   else
   {
      /*
       * MISS 
       */
      act( "$n@@b charges $N@@b, but runs right past!@@b", ch, NULL, victim, TO_NOTVICT );
      act( "$N @@bcharges you, but runs right past!@@N", victim, NULL, ch, TO_CHAR );
      act( "@@bYou try to charge $N@@b, but run past him. DOH!!@@N", ch, NULL, victim, TO_CHAR );
      damage( ch, victim, 0, -1 );
   }
   return;
}



DO_FUN(do_knee)
{

   CHAR_DATA *victim;
   int dam;
   bool prime;
   int chance;

   if( ch->check_cooldown("knee") )
    return;

   prime = FALSE;


   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_knee] == 0 )
   {
      send_to_char( "You are not trained in this skill!\r\n", ch );
      return;
   }



   if( ( ( victim = get_char_room( ch, argument ) ) == NULL ) && ch->fighting == NULL )
   {
      send_to_char( "No such victim!\r\n", ch );
      return;
   }


   if( victim == NULL )
      victim = ch->fighting;

   if( IS_NPC( victim ) && victim->act.test(ACT_NO_BODY ) )
   {
      act( "$N doesn't have a definable body to knee!", ch, NULL, victim, TO_CHAR );
      return;
   }



   if( !IS_NPC( ch ) && ch->pcdata->order[0] == 3 )
      prime = TRUE;

   if( IS_NPC( ch ) )
      dam = number_range( ch->level / 3, ch->level / 2 );
   else
      dam = number_range( ch->get_level("war") / 2, ch->get_level("war") * ( prime ? 2 : 1 ) );

   chance = ( IS_NPC( ch ) ? 50 : ch->pcdata->learned[gsn_knee] / 2 );

   chance += ( ch->get_level("war") - victim->level );

   ch->set_cooldown("knee");

   check_killer( ch, victim );

   if( number_percent(  ) < chance )
   {
      /*
       * HIT 
       */
      if( sysdata.shownumbers == TRUE )
      {
         /*
          * HIT! 
          */
         char actbuf[MSL];
         snprintf( actbuf, MSL, "$n grabs $N and knees $M in the groin! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, ch, NULL, victim, TO_NOTVICT );
         snprintf( actbuf, MSL, "$N grabs you, and knees you in the groin! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, victim, NULL, ch, TO_CHAR );
         snprintf( actbuf, MSL, "You grab $M and knee $M in the groin! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, ch, NULL, victim, TO_CHAR );
      }
      else
      {
         act( "$n grabs $N and knees $M in the groin!", ch, NULL, victim, TO_NOTVICT );
         act( "$N grabs you, and knees you in the groin!", victim, NULL, ch, TO_CHAR );
         act( "You grab $M and knee $M in the groin!", ch, NULL, victim, TO_CHAR );
      }

      damage( ch, victim, ( dam * 3 ) / 5, -1 );
   }
   else
   {
      /*
       * MISS 
       */
      act( "$n tries to grab $N, but $E twists free!", ch, NULL, victim, TO_NOTVICT );
      act( "$N tries to grab you, but you twist free!", victim, NULL, ch, TO_CHAR );
      act( "You try to grab $N, but $E twists free!", ch, NULL, victim, TO_CHAR );
      damage( ch, victim, 0, -1 );
   }
   return;
}


DO_FUN(do_kick)
{

   CHAR_DATA *victim;
   int dam;
   bool prime;
   int chance;

   if( ch->check_cooldown("kick") )
    return;

   prime = FALSE;


   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_kick] == 0 )
   {
      send_to_char( "You are not trained in this skill!\r\n", ch );
      return;
   }

   if( ch->in_room->vnum == ROOM_VNUM_JAIL )
   {
      send_to_char( "Isn't jail punishment enough for you?", ch );
      return;
   }

   if( ( ( victim = get_char_room( ch, argument ) ) == NULL ) && ch->fighting == NULL )
   {
      send_to_char( "No such victim!\r\n", ch );
      return;
   }


   if( victim == NULL )
      victim = ch->fighting;

   if( !IS_NPC( ch ) && ch->pcdata->order[0] == 3 )
      prime = TRUE;

   if( IS_NPC( ch ) )
      dam = number_range( ch->level / 3, ch->level / 2 );
   else
      dam = number_range( ch->get_level("war") / 2, ch->get_level("war") * ( prime ? 2 : 1 ) );

   chance = ( IS_NPC( ch ) ? 50 : ch->pcdata->learned[gsn_kick] / 2 );

   chance += ( ch->get_level("war") - ( victim->level + 5 ) );

   ch->set_cooldown("kick");

   check_killer( ch, victim );
   if( number_percent(  ) < chance )
   {
      /*
       * HIT 
       */

      if( sysdata.shownumbers == TRUE )
      {
         /*
          * HIT! 
          */
         char actbuf[MSL];
         snprintf( actbuf, MSL, "$n kicks $N really hard! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, ch, NULL, victim, TO_NOTVICT );
         snprintf( actbuf, MSL, "$N kicks you really hard! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, victim, NULL, ch, TO_CHAR );
         snprintf( actbuf, MSL, "You kick $N really hard! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, ch, NULL, victim, TO_CHAR );
      }
      else
      {
         act( "$n kicks $N really hard!", ch, NULL, victim, TO_NOTVICT );
         act( "$N kicks you really hard!", victim, NULL, ch, TO_CHAR );
         act( "You kick $N really hard!", ch, NULL, victim, TO_CHAR );
      }

      damage( ch, victim, dam, -1 );
   }
   else
   {
      /*
       * MISS 
       */
      act( "$n tries to kick $N, but misses!", ch, NULL, victim, TO_NOTVICT );
      act( "$N tries to kick you, but misses!", victim, NULL, ch, TO_CHAR );
      act( "You try to kick $N, but miss!", ch, NULL, victim, TO_CHAR );
      damage( ch, victim, 0, -1 );
   }
   return;
}

/* -S- Addition: Like damage() but called by objects, no message */
/* WARNING: No killer checks made, etc.  Only use if no keeper for obj */

void obj_damage( OBJ_DATA * obj, CHAR_DATA * victim, float dam )
{
   if( victim->position == POS_DEAD )
      return;

   /*
    * Damage modifiers.
    */
   if( IS_AFFECTED( victim, AFF_SANCTUARY ) || item_has_apply( victim, ITEM_APPLY_SANC ) )
      dam /= 2;

   if( ( IS_AFFECTED( victim, AFF_PROTECT ) || item_has_apply( victim, ITEM_APPLY_PROT ) )
       && IS_OBJ_STAT(obj,ITEM_EXTRA_EVIL) )
      dam -= dam / 4;

   if( dam < 0 )
      dam = 0;

   /*
    * Hurt the victim.
    * Inform the victim of his new state.
    */
   victim->hit -= (int)dam;


   update_pos( victim );

   switch ( victim->position )
   {
      case POS_MORTAL:
         act( "$n is mortally wounded, and will die soon, if not aided.", victim, NULL, NULL, TO_ROOM );
         send_to_char( "You are mortally wounded, and will die soon, if not aided.\r\n", victim );
         break;

      case POS_INCAP:
         act( "$n is incapacitated and will slowly die, if not aided.", victim, NULL, NULL, TO_ROOM );
         send_to_char( "You are incapacitated and will slowly die, if not aided.\r\n", victim );
         break;

      case POS_STUNNED:
         act( "$n is stunned, but will probably recover.", victim, NULL, NULL, TO_ROOM );
         send_to_char( "You are stunned, but will probably recover.\r\n", victim );
         break;

      case POS_DEAD:
         act( "$n is DEAD!!", victim, 0, 0, TO_ROOM );
         send_to_char( "You have been KILLED!!\r\n\r\n", victim );
         break;

      default:
         if( dam > victim->max_hit / 4 )
            send_to_char( "That really did HURT!\r\n", victim );
         if( victim->hit < victim->max_hit / 4 )
            send_to_char( "You sure are BLEEDING!\r\n", victim );
         break;
   }

   /*
    * Payoff for killing things.
    */
   if( victim->position == POS_DEAD )
   {

      if( !IS_NPC( victim ) )
      {

         snprintf( log_buf, (2 * MIL), "%s killed by %s at %d", victim->name.c_str(), obj->short_descr, victim->in_room->vnum );
         log_string( log_buf );

         notify( log_buf, 82 );

         /*
          * As level gain is no longer automatic, a dead char loses
          * * 1/2 their gained exp.  -S-
          * * Fixed my bug here too, hehe!
          */

         if( victim->exp > 0 )
         {
            int lose = victim->exp / 2;
            lose *= -1;
            victim->gain_exp(lose);
         }

      }

      raw_kill( victim, "" );

      if( deathmatch && !IS_NPC( victim ) )
         do_quit( victim, "" );


      return;
   }

   return;
}


void death_message( CHAR_DATA * ch, CHAR_DATA * victim, int dt, int max_dt )
{
   /*
    * Used to display assorted death messages, based on dt 
    * * max_dt == number of entries in attack table in dam_message,
    * * so we know if kill was unarmed, armed, or through spells or skills.
    * * Stephen
    */

   char buf1[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   char buf3[MAX_STRING_LENGTH];
   int vnum;

   /*
    * For debugging purposes  
    * snprintf( buf, MSL, "dt: %d max_dt: %d\r\n", dt, max_dt );
    * notify( buf, 1 );  
    */

   vnum = 0;   /* Used to load up body parts.... eewwwwww! */

   if( dt == 0 )
   {
      if( !( IS_NPC( victim ) && victim->act.test(ACT_NO_BODY ) ) )
         switch ( number_range( 0, 9 ) )
         {
            case 0:
               snprintf( buf1, MSL, "$n grabs $N's neck, and twists until there is a loud SNAP!" );
               snprintf( buf2, MSL, "You grab $N's neck, and twist until there is a loud SNAP!" );
               snprintf( buf3, MSL, "$n grabs your neck, and twists until there is a loud SNAP!" );
               break;
            case 1:
               snprintf( buf1, MSL, "$n slams $s fist into $N and crushes $S heart!" );
               snprintf( buf2, MSL, "You slam your fist into $N and crush $S heart!" );
               snprintf( buf3, MSL, "$n slams $s fist into you, and crushes your heart!" );
               break;
            case 2:
               snprintf( buf1, MSL, "$n rams $s hand into $N's ribcage, and rips out $S guts!" );
               snprintf( buf2, MSL, "You ram your hand into $N's ribcage, and rip out $S guts!" );
               snprintf( buf3, MSL, "$n rams $s hand into your ribcage, and rips out your guts!" );
               break;
            case 3:
               snprintf( buf1, MSL, "$n grabs $N, and rips $S head clean off!" );
               snprintf( buf2, MSL, "You grab $N, and rip his head clean off!" );
               snprintf( buf3, MSL, "$n grabs you, and rips your head clean off!" );
               break;
            case 4:
               snprintf( buf1, MSL, "$n reaches behind $N, and rips $S spine out of $S back!" );
               snprintf( buf2, MSL, "You reach behind $N, and rip $S spine out of $S back!" );
               snprintf( buf3, MSL, "$n reaches behind you, and rips your spine out of your back!" );
               break;
            case 5:
               snprintf( buf1, MSL, "$n rips $N's face open via $S eye sockets!" );
               snprintf( buf2, MSL, "You rip $N's face open via $S eye sockets!" );
               snprintf( buf3, MSL, "$n rips your face open via your eye sockets!" );
               break;
            case 6:
               snprintf( buf1, MSL, "$n rips off $N's head and vomits down $S throat!" );
               snprintf( buf2, MSL, "You rip off $N's head and vomit down $S throat!" );
               snprintf( buf3, MSL, "$n rips off your head and vomits down your throat!" );
               break;
            case 7:
               snprintf( buf1, MSL, "$N splurts blood as $n rips open $S chest with $s teeth!" );
               snprintf( buf2, MSL, "$N splurts blood as you rip open $S chest with your teeth!" );
               snprintf( buf3, MSL, "You splurt blood as $n rips open your chest with $s teeth!" );
               break;
            case 8:
               snprintf( buf1, MSL, "$n wrenches $N's arms out from their sockets!" );
               snprintf( buf2, MSL, "You wrench $N's arms out from their sockets!" );
               snprintf( buf3, MSL, "$n wrenches your arms out of thier sockets!" );
               break;
            case 9:
               snprintf( buf1, MSL, "$n shatters $N's skull with a punch.  Brains leak out." );
               snprintf( buf2, MSL, "You shatter $N's skull with a punch.  Brains leak out." );
               snprintf( buf3, MSL, "$n shatters your skull with a punch.  Brains leak out." );
               break;
         }
      else  /* Unarmed, mob has no_body */
      {
         snprintf( buf1, MSL, "$n shatters $N's skull with a punch.  Brains leak out." );
         snprintf( buf2, MSL, "You shatter $N's skull with a punch.  Brains leak out." );
         snprintf( buf3, MSL, "$n shatters your skull with a punch.  Brains leak out." );
      }
   }


   if( dt <= max_dt && dt > 0 )
      switch ( dt )
      {
         case 1: /* slice */
         case 3: /* slash */
            switch ( number_range( 0, 4 ) )
            {
               case 0:
                  snprintf( buf1, MSL, "$n slices $N's head clean from $S neck" );
                  snprintf( buf2, MSL, "You slice $N's head clean from $S neck" );
                  snprintf( buf3, MSL, "$n slices your head clean from your neck" );
                  break;
               case 1:
                  snprintf( buf1, MSL, "$n slashes open $N's chest; $S entrails pour out!" );
                  snprintf( buf2, MSL, "You slash open $N's chest; $S entrails pour out!" );
                  snprintf( buf3, MSL, "$n slashes open your chest; your entrails pour out!" );
                  break;
               case 2:
                  snprintf( buf1, MSL, "$n slices $N's throat open.  Blood sprays out wildly!" );
                  snprintf( buf2, MSL, "You slice $N's throat open.  Blood sprays out wildly!" );
                  snprintf( buf3, MSL, "$n slices your throat open.  Blood sprays out wildly!" );
                  break;
               case 3:
                  snprintf( buf1, MSL, "$n slices $N's legs off, leaving two bloody stumps!" );
                  snprintf( buf2, MSL, "You slice $N's legs off, leaving two bloody stumps!" );
                  snprintf( buf3, MSL, "$n slices your legs off, leaving two bloody stumps!" );
                  break;
               case 4:
                  snprintf( buf1, MSL, "$n slashes $N's eyeballs out!" );
                  snprintf( buf2, MSL, "You slash $N's eyeballs out!" );
                  snprintf( buf3, MSL, "$n slashes your eyeballs out!" );
                  break;
            }
            break;
         case 2: /* Stab */
         case 11:   /* Pierce */
            switch ( number_range( 0, 4 ) )
            {
               case 0:
                  snprintf( buf1, MSL, "$n rips a gaping hole down $N's back!" );
                  snprintf( buf2, MSL, "You rip a gaping hole down $N's back!" );
                  snprintf( buf3, MSL, "$n rips a gaping hole down your back!" );
                  break;
               case 1:
                  snprintf( buf1, MSL, "$n stabs into $N, and cuts $S heart out!" );
                  snprintf( buf2, MSL, "You stab into $N, and cut $S heart out!" );
                  snprintf( buf3, MSL, "$n stabs into you, and cuts your heart out!" );
                  break;
               case 2:
                  snprintf( buf1, MSL, "$n stabs into $N's back, and wrenches out $S spine!" );
                  snprintf( buf2, MSL, "You stab into $N's back, and wrench out $S spine!" );
                  snprintf( buf3, MSL, "$n stabs into your back, and wrenches out your spine!" );
                  break;
               case 3:
                  snprintf( buf1, MSL, "$n plunges $s weapon into $N's head!" );
                  snprintf( buf2, MSL, "You plunge your weapon into $N's head!" );
                  snprintf( buf3, MSL, "$n plunges $s weapon into your head!" );
                  break;
               case 4:
                  snprintf( buf1, MSL, "$n stabs into $N's chest, skewering $S heart!" );
                  snprintf( buf1, MSL, "$n stabs into $N's chest, skewering $S heart!" );
                  snprintf( buf2, MSL, "You stab into $N's chest, skewering $S heart!" );
                  snprintf( buf2, MSL, "You stab into $N's chest, skewering $S heart!" );
                  snprintf( buf3, MSL, "$n stabs into your chest, skewering your heart!" );
                  break;
            }
            break;
         case 4: /* Whip */
            switch ( number_range( 0, 3 ) )
            {
               case 0:
                  snprintf( buf1, MSL, "$n whips out $N's eyes, spraying blood everywhere!" );
                  snprintf( buf2, MSL, "You whip out $N's eyes, spraying blood everywhere!" );
                  snprintf( buf3, MSL, "$n whips out your eyes, spraying blood everywhere!" );
                  break;
               case 1:
                  snprintf( buf1, MSL, "$n's whip catches $N's head, and rips it off!" );
                  snprintf( buf2, MSL, "Your whip catches $N's head, and rips it off!" );
                  snprintf( buf3, MSL, "$n's whip catches your head, and rips it off!" );
                  break;
               case 2:
                  snprintf( buf1, MSL, "$n's whip wraps around $N's arms, yanking them off!" );
                  snprintf( buf2, MSL, "Your whip wraps around $N's arms, yanking them off!" );
                  snprintf( buf3, MSL, "$n's whip wraps around your arms, yanking them off!" );
                  break;
               case 3:
                  snprintf( buf1, MSL, "$n's whip cuts open $N's main artery, spraying blood!" );
                  snprintf( buf2, MSL, "Your whip cuts open $N's main artery, spraying blood!" );
                  snprintf( buf3, MSL, "$n's whip cuts open your main artery, spraying blood!" );
                  break;
            }
            break;
         case 5: /* Claw */
            switch ( number_range( 0, 4 ) )
            {
               case 0:
                  snprintf( buf1, MSL, "$n claws out $N's heart!" );
                  snprintf( buf2, MSL, "You claw out $N's heart!" );
                  snprintf( buf3, MSL, "$n claws out your heart!" );
                  break;
               case 1:
                  snprintf( buf1, MSL, "$n's claw catches $N's back, slicing it open!" );
                  snprintf( buf2, MSL, "Your claw catches $N's back, slicing it open!" );
                  snprintf( buf3, MSL, "$n's claw catches your back, slicing it open!" );
                  break;
               case 2:
                  snprintf( buf1, MSL, "$N screams in agony, as $n's claw removes $S eyes!" );
                  snprintf( buf1, MSL, "$N screams in agony, as $n's claw removes $S eyes!" );
                  snprintf( buf2, MSL, "$N screams in agony, as your claw removes $S eyes!" );
                  snprintf( buf2, MSL, "$N screams in agony, as your claw removes $S eyes!" );
                  snprintf( buf3, MSL, "You scream in agony, as $n's claw removes your eyes!" );
                  break;
               case 3:
                  snprintf( buf1, MSL, "$n's claw ruptures $N's ribcage, shredding $S heart!" );
                  snprintf( buf2, MSL, "Your claw ruptures $N's ribcage, shredding $S heart!" );
                  snprintf( buf3, MSL, "$n's claw ruptures your ribcage, shredding your heart!" );
                  break;
               case 4:
                  snprintf( buf1, MSL, "$n's claw slashes $N's neck, decapitating $M!" );
                  snprintf( buf2, MSL, "Your claw slashes $N's neck, decapitating $M!" );
                  snprintf( buf3, MSL, "$n's claw slashes your neck, decapitating you!" );
                  break;
            }
            break;
         case 7: /* Pound */
         case 8: /* Crush */
            switch ( number_range( 0, 4 ) )
            {
               case 0:
                  snprintf( buf1, MSL, "$n pounds $N's head; $S brains leak from $S ears!" );
                  snprintf( buf2, MSL, "You pound $N's head; $S brains leak from $S ears!" );
                  snprintf( buf3, MSL, "$n pounds your head; your brains leak from your ears!" );
                  break;
               case 1:
                  snprintf( buf1, MSL, "$n crushes $N's ribcage, and $S entrails slop out!" );
                  snprintf( buf2, MSL, "You crush $N's ribcage, and $S entrails slop out!" );
                  snprintf( buf3, MSL, "$n crushes your ribcage, and your entrails slop out!" );
                  break;
               case 2:
                  snprintf( buf1, MSL, "$n pounds $N's spine until you hear it CRACK!" );
                  snprintf( buf2, MSL, "You pound $N's spine until you hear it CRACK!" );
                  snprintf( buf3, MSL, "$n pounds your spine until you hear it CRACK!" );
                  break;
               case 3:
                  snprintf( buf1, MSL, "$n pounds $N's face, forcing $S nose into $S brain!" );
                  snprintf( buf2, MSL, "You pound $N's face, forcing $S nose into $S brain!" );
                  snprintf( buf3, MSL, "$n pounds your face, forcing your nose into your brain!" );
                  break;
               case 4:
                  snprintf( buf1, MSL, "$n crushes $N's head down into $S neck!" );
                  snprintf( buf2, MSL, "You crush $N's head down into $S neck!" );
                  snprintf( buf3, MSL, "$n crushes your head down into your neck!" );
                  break;
            }
            break;
         case 6: /* Blast */
            switch ( number_range( 0, 4 ) )
            {
               case 0:
                  snprintf( buf1, MSL, "$n's blast totally obliterates $N's head!" );
                  snprintf( buf2, MSL, "Your blast totally obliterates $N's head!" );
                  snprintf( buf3, MSL, "$n's blast totally obliterates your head!" );
                  break;
               case 1:
                  snprintf( buf1, MSL, "$n's blast makes $N's head fly into the air!" );
                  snprintf( buf2, MSL, "Your blast makes $N's head fly into the air!" );
                  snprintf( buf3, MSL, "$n's blast makes your head fly into the air!" );
                  break;
               case 2:
                  snprintf( buf1, MSL, "$n blasts $N's stomach open - entrails plop out!" );
                  snprintf( buf2, MSL, "You blast $N's stomach open - entrails plop out!" );
                  snprintf( buf3, MSL, "$n blasts your stomach open - entrails plop out!" );
                  break;
               case 3:
                  snprintf( buf1, MSL, "$n's blast removes $N's legs from $S body!" );
                  snprintf( buf2, MSL, "Your blast removes $N's legs from $S body!" );
                  snprintf( buf3, MSL, "$n's blast removes your legs from your body!" );
                  break;
               case 4:
                  snprintf( buf1, MSL, "$n's blast splits $N's back, and $S spine falls out!" );
                  snprintf( buf2, MSL, "Your blast splits $N's back, and $S spine falls out!" );
                  snprintf( buf3, MSL, "$n's blast splits your back, and your spine falls out!" );
                  break;
            }
            break;
         default:   /* grep, bite, suction */
            switch ( number_range( 0, 3 ) )
            {
               case 0:
                  snprintf( buf1, MSL, "$n pulls $N's heart clean from $S ribcage!" );
                  snprintf( buf2, MSL, "You pull $N's heart clean from $S ribcage!" );
                  snprintf( buf3, MSL, "$n pulls your heart clean from your ribcage!" );
                  break;
               case 1:
                  snprintf( buf1, MSL, "$n snags $N's spine, and rips it out!" );
                  snprintf( buf2, MSL, "You snag $N's spine, and rip it out!" );
                  snprintf( buf3, MSL, "$n snags your spine, and rips it out!" );
                  break;
               case 2:
                  snprintf( buf1, MSL, "$N's stomach splits open, and $S entrails slip out!" );
                  snprintf( buf2, MSL, "$N's stomach splits open, and $S entrails slip out!" );
                  snprintf( buf3, MSL, "Your stomach splits open, and your entrails slip out!" );
                  break;
               case 3:
                  snprintf( buf1, MSL, "$n pulls $N's heart from $S chest!" );
                  snprintf( buf2, MSL, "You pull $N's heart from $S chest!" );
                  snprintf( buf3, MSL, "$n pulls your heart from your chest!" );
                  break;
            }
            break;
      }
   act( buf1, ch, NULL, victim, TO_NOTVICT );
   act( buf2, ch, NULL, victim, TO_CHAR );
   act( buf3, ch, NULL, victim, TO_VICT );

   /*
    * Load up object, if needed.
    */
   if( vnum != 0 )
   {
      char buf[MAX_STRING_LENGTH];
      OBJ_DATA *obj;
      char *name = NULL;

      snprintf(name,MSL,"%s",ch->get_name());
      obj = create_object( get_obj_index( vnum ), 0 );
      obj->timer = number_range( 4, 7 );

      snprintf( buf, MSL, obj->short_descr, name );
      free_string( obj->short_descr );
      obj->short_descr = str_dup( buf );

      snprintf( buf, MSL, obj->long_descr, name );
      free_string( obj->long_descr );
      obj->long_descr = str_dup( buf );

      obj_to_room( obj, ch->in_room );
   }



   return;
}

DO_FUN(do_assist)
{
   /*
    * Allow players to join in fight, by typing assist, 
    * * or assist <name>.  Will only ever allow players to
    * * assist a group member  -- Stephen
    */

   CHAR_DATA *assist = NULL;
   CHAR_DATA *ppl;
   char actbuf[MSL];

   if( argument[0] != '\0' )  /* then check for assist <name> */
   {
      if( ( assist = get_char_room( ch, argument ) ) == NULL )
      {
         send_to_char( "They don't seem to be here right now...\r\n", ch );
         return;
      }
      if( assist == ch )
      {
         send_to_char( "You sure need help!\r\n", ch );
         return;
      }
      if( !is_same_group( ch, assist ) )
      {
         act( "Sorry, $N isn't in your group...", ch, NULL, assist, TO_CHAR );
         return;
      }
      if( ( assist->fighting != NULL ) && ( ch->fighting == NULL ) )
      {
         snprintf( actbuf, MSL, "$n screams, '%s'", IS_NPC( ch ) ? "BANZAI!! $N must be assisted!!" : ch->pcdata->assist_msg );
         act( actbuf, ch, NULL, assist, TO_ROOM );
         snprintf( actbuf, MSL, "You scream, '%s'", IS_NPC( ch ) ? "BANZAI!! $N must be assisted!!" : ch->pcdata->assist_msg );
         act( actbuf, ch, NULL, assist, TO_CHAR );
         set_fighting( ch, assist->fighting, TRUE );
         return;
      }
      send_to_char( "Doesn't look like anyone needs your help right now...\r\n", ch );
      return;
   }

   /*
    * Check if any group members in room AND fighting 
    */

   for( ppl = ch->in_room->first_person; ppl != NULL; ppl = ppl->next_in_room )
      if( ( ppl != ch ) && ( is_same_group( ch, ppl ) ) )
         break;

   if( ppl == NULL )
   {
      send_to_char( "Doesn't look like anyone needs your help right now...\r\n", ch );
      return;
   }

   /*
    * Assisting leader is main priority 
    */
   if( ( ch->leader != NULL )
       && ( ch->leader->in_room == ch->in_room )
       && ( ch->leader->fighting != NULL ) && ( ch->fighting == NULL ) && ( ch->leader != ch ) )
   {
      snprintf( actbuf, MSL, "$n screams, '%s'", IS_NPC( ch ) ? "BANZAI!! $N must be assisted!!" : ch->pcdata->assist_msg );
      act( actbuf, ch, NULL, ch->leader, TO_ROOM );
      snprintf( actbuf, MSL, "You scream, '%s'", IS_NPC( ch ) ? "BANZAI!! $N must be assisted!!" : ch->pcdata->assist_msg );
      act( actbuf, ch, NULL, ch->leader, TO_CHAR );
      set_fighting( ch, ch->leader->fighting, TRUE );
      return;
   }

   /*
    * Ok, so no leader to help... lets look for other group members 
    */
   for( ppl = ch->in_room->first_person; ppl != NULL; ppl = ppl->next_in_room )
      if( ( is_same_group( ch, ppl ) ) && ( ppl != ch ) && ( ppl->fighting != NULL ) && ( ch->fighting == NULL ) )
      {
         snprintf( actbuf, MSL, "$n screams, '%s'", IS_NPC( ch ) ? "BANZAI!! $N must be assisted!!" : ch->pcdata->assist_msg );
         act( actbuf, ch, NULL, ppl, TO_ROOM );
         snprintf( actbuf, MSL, "You scream, '%s'", IS_NPC( ch ) ? "BANZAI!! $N must be assisted!!" : ch->pcdata->assist_msg );
         act( actbuf, ch, NULL, ppl, TO_CHAR );
         set_fighting( ch, ppl->fighting, TRUE );
         return;
      }


   send_to_char( "Doesn't look like anyone needs your help right now...\r\n", ch );
   return;
}

DO_FUN(do_stake)
{
   /*
    * If player has an object of TYPE_STAKE (s)he can try and stake
    * * a fallen ( hit<0 ) vampire.  Note that there is still a check
    * * for pkilling here... PKOK players can be vamp hunters with
    * * no worries! (:  Player must also have learnt the stake skill.
    * * A sleeping vamp is also considered vulnerable.
    */

   CHAR_DATA *victim;
   OBJ_DATA *stake;
   int chance;
   char arg[MAX_INPUT_LENGTH];
   int sn;

   if( ch->check_cooldown("stake") )
    return;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Stake who?\r\n", ch );
      return;
   }

   victim = get_char_room( ch, arg );

   if( victim == NULL )
   {
      send_to_char( "Couldn't find the target.\r\n", ch );
      return;
   }
   if( victim == ch )
   {
      send_to_char( "Oh yeah, that would be a really good idea!\r\n", ch );
      return;
   }

   if( ( stake = get_eq_char( ch, WEAR_HOLD_HAND_L ) ) == NULL )
      stake = get_eq_char( ch, WEAR_HOLD_HAND_R );
   if( stake == NULL || stake->item_type != ITEM_STAKE )
   {
      send_to_char( "You have to be holding some sort of stake to do that\r\n", ch );
      return;
   }
   if( IS_NPC( victim ) )
   {
      send_to_char( "This is only used against PLAYER @@dVampyres@@N!!\r\n", ch );
      return;
   }

   chance = IS_NPC( ch ) ? ch->level * 2 : ch->pcdata->learned[gsn_stake];
   if( !IS_AWAKE( victim ) )
      chance += 25;

   /*
    * Make it harder to stake higher level targets 
    */
   if( ( victim->pcdata->super->level * 5 ) > ch->level )
      chance -= ( victim->pcdata->super->level * 5 ) - ch->level;

   if( victim->hit > 0 && IS_AWAKE( victim ) )  /* i.e. not vulnerable! */
      chance = 0;

   if( !IS_VAMP( victim ) )
      chance = 0;

   ch->set_cooldown("stake");
   check_killer( ch, victim );

   if( number_percent(  ) < chance )
   {
      /*
       * Success 
       */
      act( "$n raises $p above $s head and plunges it into $N's heart!", ch, stake, victim, TO_NOTVICT );
      act( "You raise $p above your head and plunge it into $N's heart!", ch, stake, victim, TO_CHAR );
      act( "$n raises $p above his head and plunges it into your heart!", ch, stake, victim, TO_VICT );
      act( "$n screams in agony and withers to black dust!", victim, NULL, NULL, TO_ROOM );
      send_to_char( "You scream in agony and withers to black dust!\r\n", victim );
      send_to_char( "@@NYou find yourself back in Moribund....\r\n", victim );
      send_to_char( "@@NSuddenly, you realize that your @@eKindred@@Nknowledge\r\n", victim );
      send_to_char( "@@Nis gone! So, THAT's what the sun looks like to mortals!\r\n", victim );
      send_to_char( "@@NYou had almost forgotten.....\r\n", victim );



      victim->act.reset(ACT_VAMPIRE);
      victim->pcdata->super->level = 0;
      victim->pcdata->super->exp = 0;
      victim->pcdata->super->energy = 0;
      victim->pcdata->super->energy_max = 0;
      victim->pcdata->super->generation = -1;
      victim->pcdata->super->bloodline = 0;
      victim->pcdata->recall_vnum = ROOM_VNUM_TEMPLE;

/* remove vamp skills from dead vamp  */

      for( sn = 0; sn <= MAX_SKILL; sn++ )
         if( ( skill_table[sn].flag2 == VAMP ) && ( victim->pcdata->learned[sn] > 0 ) )
            victim->pcdata->learned[sn] = 0;

      if( !IS_NPC( victim ) )
      {
         int lose = 3 * victim->exp / 4;
         lose *= -1;
         victim->gain_exp(lose);
      }

      raw_kill( victim, "" );
      return;
   }
   else
   {
      act( "$n tries to plunge $p into $N's chest, but misses!", ch, stake, victim, TO_NOTVICT );
      act( "You try to plunge $p into $N's chest, but miss!", ch, stake, victim, TO_CHAR );
      act( "$n tries to plunge $p into your chest, but misses!", ch, stake, victim, TO_VICT );
      set_fighting( ch, victim, TRUE );
   }
   return;
}

DO_FUN(do_stun)
{
   CHAR_DATA *victim;
   int chance;
   int chance2;

   if( ch->check_cooldown("stun") )
    return;

   if( IS_NPC( ch ) )   /* for now */
      return;


   if( ( ( victim = ch->fighting ) == NULL ) || ( victim->in_room == NULL ) )
   {
      send_to_char( "You must be fighting someone first!\r\n", ch );
      return;
   }
   if( !IS_NPC( ch ) && ( ch->pcdata->learned[gsn_stun] == 0 ) )
   {
      send_to_char( "Huh?", ch );
      return;
   }

   chance = 45 - ( 100 * victim->hit / victim->max_hit );
   /*
    * The lower the victim's hp, the greater the chance 
    */

   chance2 = IS_NPC( ch ) ? ch->level * 2 : ch->pcdata->learned[gsn_stun];

   ch->set_cooldown("stun");

   if( ( number_percent(  ) + number_percent(  ) ) < ( chance + chance2 ) )
   {
      /*
       * Success 
       */
      act( "You slam into $N, leaving $M stunned.", ch, NULL, victim, TO_CHAR );
      act( "$n slams into you, leaving you stunned.", ch, NULL, victim, TO_VICT );
      act( "$n slams into $N, leaving $M stunned.", ch, NULL, victim, TO_NOTVICT );

      victim->stun_timer += number_range( 1, ch->get_level("psuedo") / 30 );
      if( ch->get_level("kni") > 40 )
         victim->stun_timer += number_range( 1, 2 );

/*      if ( !IS_NPC( ch ) )
      {
        if( ch->get_level("mon") > ch->get_level("kni") )
        {
  	  victim->stun_timer += ch->get_level("mon") / 15;
	  return;
        }      
        else 
        {
          set_stun( victim, ch->get_level("kni") / 15 );
          return;
        }
      }   */
   }
   else
   {
      /*
       * Ooops! 
       */
      act( "You try to slam into $N, but miss and fall onto your face!", ch, NULL, victim, TO_CHAR );
      act( "$n tries to slam into you, but misses and falls onto $s face!", ch, NULL, victim, TO_VICT );
      act( "$n tries to slam into $N, but misses and falls onto $s face!", ch, NULL, victim, TO_NOTVICT );
      damage( ch, ch, number_range( 5, 20 ), TYPE_UNDEFINED );
      return;
   }
   return;
}

DO_FUN(do_feed)
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int chance;
   int bloodgain = 0;
   one_argument( argument, arg );

   if( !IS_VAMP( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   if( ch->check_cooldown("feed") )
    return;

   if( ( victim = ch->fighting ) == NULL )
   {
      send_to_char( "You must be fighting someone first!\r\n", ch );
      return;

   }

   if( victim == ch )
   {
      send_to_char( "What are you?  A blood donor?\r\n", ch );
      return;
   }

   if( victim->in_room->room_flags.test(RFLAG_SAFE) )
   {
      send_to_char( "The Gods prevent your foul deed.  This is a safe room!\r\n", ch );
      return;
   }

   if( victim->hit > 0 && victim->position > POS_FIGHTING )
   {
      /*
       * failure 
       */
      act( "$N is far too alert to let you do that!", ch, NULL, victim, TO_CHAR );
      return;
   }

   chance = IS_NPC( ch ) ? ch->level : ch->pcdata->learned[gsn_feed];
   ch->set_cooldown("feed");
   check_killer( ch, victim );
   if( number_percent(  ) < chance )
   {
      /*
       * Success! 
       */
      act( "You plunge your fangs into $N's neck, and taste sweet blood!", ch, NULL, victim, TO_CHAR );
      act( "$n plunges $s fangs into your neck, and sucks your blood!", ch, NULL, victim, TO_VICT );
      act( "$n plunges $s fangs into $N's neck and sucks $S blood!", ch, NULL, victim, TO_NOTVICT );
      if( ch->pcdata->super->energy <= -5 )
      {
         ch->pcdata->super->energy = ch->pcdata->super->energy_max;
         send_to_char( " You have now entered the ranks of the @@dKINDRED@@N!!!!\r\n", ch );
      }
      else
      {

         bloodgain = ( ( 20 - ch->pcdata->super->generation ) + ch->pcdata->super->level );
         if( bloodgain > victim->level )
            bloodgain = victim->level;
      }

      if( ( ch->pcdata->super->energy + bloodgain ) > ch->pcdata->super->energy_max )
         ch->pcdata->super->energy = ch->pcdata->super->energy_max;
      else
         ch->pcdata->super->energy += bloodgain;

      if( victim->hit > 0 )
      {
         act( "$N screams in horror at you at jumps to $s feet!", ch, NULL, victim, TO_CHAR );
         act( "You scream in horror at $n and jump to your feet!", ch, NULL, victim, TO_VICT );
         act( "$N screams in horror at $n and jumps to $S feet!", ch, NULL, victim, TO_NOTVICT );
         do_say( victim, "You'll pay for that!" );
         victim->position = POS_STANDING;
         one_hit( victim, ch, TYPE_UNDEFINED );
      }



      if( IS_VAMP( victim ) )
      {
         return;  /* for now :P SRZ 
                   * if ( !IS_NPC(ch) && !IS_NPC(victim) )
                   * {
                   * ch->pcdata->super->energy = victim->pcdata->super->energy;
                   * victim->pcdata->super->energy = 0;
                   * }
                   * if ( !IS_NPC(ch) && IS_NPC(victim) )
                   * gain_bloodlust( ch, victim->level*2 );
                   * if ( IS_NPC(ch) && !IS_NPC(victim) )
                   * victim->pcdata->super->energy = 0;     */
      }



      return;
   }
   else
   {
      /*
       * D'oh! 
       */
      act( "You try to find a spot on $N's neck to suck blood, but fail!", ch, NULL, victim, TO_CHAR );
      act( "$n tries to find a spot on your neck to suck blood, but fails!", ch, NULL, victim, TO_VICT );
      act( "$n tries to find a spot on $N's neck to suck blood, but fails!", ch, NULL, victim, TO_NOTVICT );
      /*
       * Any penalty? 
       */
      return;
   }
   return;
}


void check_adrenaline( CHAR_DATA * ch, float damage )
{

   AFFECT_DATA af;



   if( damage > 200 && ch->pcdata->learned[gsn_adrenaline] > 70 )
   {

      af.type = skill_lookup( "adrenaline bonus" );
      af.duration = 1;
      af.location = APPLY_DAMROLL;
      af.modifier = 1;
      af.bitvector = 0;
      affect_join( ch, &af );
      send_to_char( "You feel a surge of @@eadrenaline@@N!!!\r\n", ch );
   }
   return;

}

DO_FUN(do_frenzy)
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int moves = 0;
   int damage = 0;

   if( ch->check_cooldown("frenzy") )
    return;

   if( IS_NPC( ch ) )
      return;

   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_frenzy] == 0 )
   {
      send_to_char( "You are not trained in this skill!\r\n", ch );
      return;
   }


   if( !IS_NPC( ch ) && ch->move < 100 && ch->position == POS_FIGHTING )
   {
      send_to_char( "You're too tired to go into a frenzy!\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && number_percent(  ) > ch->pcdata->learned[gsn_frenzy] )
   {
      send_to_char( "You try to go into a frenzy, but nearly cut your leg off!\r\n", ch );
      return;
   }

   if( ( ( ch->move / 10 ) < 75 ) && ch->move >= 75 )
      moves = 75;
   else
      moves = ch->move / 10;

   if( ch->hit > 200 )
      damage = 20;

   ch->set_cooldown("frenzy");

   if( !IS_NPC( ch ) && ch->position == POS_FIGHTING )
      ch->move -= moves;
   ch->hit -= damage;

   for( vch = ch->in_room->first_person; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
      if( vch->in_room == NULL )
         continue;

      if( vch->in_room == ch->in_room )
      {
         if( vch != ch && ( vch->in_room == ch->in_room )
             && ( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) ) && ( vch->master != ch ) && ( !is_same_group( ch, vch ) ) )
         {
            act( "$N is @@Wgored@@N by your FRENZY!!!\r\n", ch, NULL, vch, TO_CHAR );
            act( "$n goes into a FRENZY, @@Wgoring@@N $N!!!", ch, NULL, vch, TO_ROOM );
            check_killer( ch, vch );
            one_hit( ch, vch, -1 );
            continue;
         }
      }
   }

   return;
}




DO_FUN(do_rage)
{
   char arg[MAX_INPUT_LENGTH];
   int chance = 0;


   one_argument( argument, arg );



   if( IS_NPC( ch ) || !IS_WOLF( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }
   if( IS_RAGED( ch ) )
   {
      send_to_char( "You are already @@rRAGING@@N!!!\r\n", ch );
      return;
   }

   chance = 60 - ( ch->hit / ch->max_hit * 100 );
   if( weather_info.moon_loc == MOON_PEAK )
      chance += 15;
   if( IS_SHIFTED( ch ) )
      chance += 20;
   if( weather_info.moon_phase == MOON_FULL )
      chance += 30;
   if( weather_info.moon_phase == MOON_NEW )
      chance -= 30;
   if( weather_info.moon_loc == MOON_DOWN )
      chance -= 20;
   if( weather_info.moon_loc == MOON_PEAK )
      chance += 20;

   if( str_cmp( arg, "FORCE" ) )
   {
      chance += ( ( 5 - ch->pcdata->super->generation ) * 10 );
      chance += ch->pcdata->super->level;
      if( ( ch->pcdata->super->generation < 4 ) || ( ch->pcdata->super->level > 14 ) )
         chance += 50;
   }
   else
   {
      chance -= ( ( 5 - ch->pcdata->super->generation ) * 10 );
      chance -= ch->pcdata->super->level;
      if( ( ch->pcdata->super->generation < 4 ) || ( ch->pcdata->super->level > 14 ) )
         chance -= 50;
   }


   if( number_range( 0, 100 ) < chance )
   {
      AFFECT_DATA af1;
      AFFECT_DATA af2;
      AFFECT_DATA af3;
      AFFECT_DATA af4;

      int duration;

      af1.type = skill_lookup( "Rage:sharpened claws" );
      af2.type = skill_lookup( "Rage:wolven strength" );
      af3.type = skill_lookup( "Rage:disregard for pain" );
      af4.type = skill_lookup( "Enraged" );

      if( !str_cmp( arg, "FORCE" ) )
      {
         duration = number_range( ch->pcdata->super->generation / 4, 4 );

         af4.duration = duration;
         af4.location = APPLY_NONE;
         af4.modifier = 0;
         af4.bitvector = 0;
         affect_join( ch, &af4 );


         af1.duration = duration;
         af1.location = APPLY_DAMROLL;
         af1.modifier = ( 10 - ch->pcdata->super->generation ) * 4;
         af1.bitvector = 0;
         affect_join( ch, &af1 );

         af2.duration = duration;
         af2.location = APPLY_HITROLL;
         af2.modifier = ( 10 - ch->pcdata->super->generation ) * 4;
         af2.bitvector = 0;
         affect_join( ch, &af2 );

         af3.duration = duration;
         af3.location = APPLY_AC;
         af3.modifier = ( 5 + ch->pcdata->super->generation ) * 10;
         af3.bitvector = 0;
         affect_join( ch, &af3 );



      }
      else
      {
         duration = number_range( ch->pcdata->super->generation / 3, 5 );
         af4.duration = duration;
         af4.location = 0;
         af4.modifier = 0;
         af4.bitvector = 0;
         affect_join( ch, &af4 );


         af1.duration = duration;
         af1.location = APPLY_DAMROLL;
         af1.modifier = ( 10 - ch->pcdata->super->generation ) * 4;
         af1.bitvector = 0;
         affect_join( ch, &af1 );

         af2.duration = duration;
         af2.location = APPLY_HITROLL;
         af2.modifier = ( 10 - ch->pcdata->super->generation ) * 4;
         af2.bitvector = 0;
         affect_join( ch, &af2 );

         af3.duration = duration;
         af3.location = APPLY_AC;
         af3.modifier = ( 5 + ch->pcdata->super->generation ) * 15;
         af3.bitvector = 0;
         affect_join( ch, &af3 );


      }
      send_to_char( "You are @@eENRAGED!!!!!!\r\n", ch );
      ch->act.set(ACT_RAGED);
      ch->pcdata->super->energy = ( ch->pcdata->super->energy_max - number_range( 0, ch->pcdata->super->generation * 3 ) );
      ch->stance = STANCE_WARRIOR;
      ch->stance_ac_mod = 0;
      ch->stance_dr_mod = 0;
      ch->stance_hr_mod = 0;
   }
   else
      send_to_char( "You fail to become @@eENRAGED@@N.\r\n", ch );
   return;


};

float get_speed( CHAR_DATA *ch, int slot )
{
 float value = 4.00;
 int i = 0;
 OBJ_DATA *wield;

 if( !IS_NPC(ch) )
 {
  switch(slot)
  {
   case SPEED_LH:   wield = get_eq_char(ch,WEAR_HOLD_HAND_L);  break;
   case SPEED_RH:   wield = get_eq_char(ch,WEAR_HOLD_HAND_R);  break;
   case SPEED_TAIL: wield = NULL;                              break;
   default: wield = NULL; break;
  }
  if( wield != NULL && wield->item_type == ITEM_WEAPON )
   value = wield->speed;
  else
  {
   if( slot == SPEED_TAIL )
    value = number_range(30,60);
   for( i = ch->level; i > 0; i-- )
    value -= 0.01;
  }
 }
 else
 {
  for( i = ch->level; i > 0 && value > 0.99; i -= 13 )
   value -= 0.14;
 }
 value += stance_app[ch->stance].speed_mod;
 if( (IS_NPC(ch) && IS_SET(ch->npcdata->skills,MOB_REFLEXES) && number_percent() < 80) || (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_enhanced_reflexes]) )
  value -= 0.02;
 if( (IS_NPC(ch) && IS_SET(ch->npcdata->skills,MOB_SLEIGHT) && number_percent() < 80) || (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_sleight_of_hand]) )
  value -= 0.04;
 if( (IS_NPC(ch) && IS_SET(ch->npcdata->skills,MOB_QUICKSTRIKE) && number_percent() < 80) || (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_quickstrike]) )
  value -= 0.06;

 if( value < 1 )
  value = 1;

 return value;
}

void combat_update( void )
{
 list<CHAR_DATA *>::iterator li;
 CHAR_DATA *ch;
 CHAR_DATA *victim;

 li = fight_list.begin();
 while( li != fight_list.end() )
 {
  ch = *li;
  if( ch == NULL || (ch != NULL && ch->fighting == NULL) )
  {
   monitor_chan("Removing a null ch or ch->fighting from queue.",MONITOR_DEBUG);
   li = fight_list.erase(li);
  }
  victim = ch->fighting;

  /*
   * Speed based combat. Arms attack independantly of each other,
   * as do special racials such as fangs or tails. --Kline
   */
  if( ch->position == POS_FIGHTING && victim != NULL )
  {
   /* Left hand attack (primary) */
   ch->speed[SPEED_LH] -= 0.01;

   if( ch->speed[SPEED_LH] <= 0 )
   {
    one_hit(ch, victim, TYPE_UNDEFINED);
    ch->speed[SPEED_LH] = get_speed(ch,SPEED_LH);
   }

   /* Right hand attack (if we dualwield) */
   if( (IS_NPC(ch) && IS_SET(ch->npcdata->skills,MOB_DUALWIELD)) || (!IS_NPC(ch) && ch->pcdata->learned[gsn_dualwield] > 10) )
   {
    ch->speed[SPEED_RH] -= 0.01;

    if( ch->speed[SPEED_RH] <= 0 )
    {
     one_hit(ch, victim, TYPE_UNDEFINED);
     ch->speed[SPEED_RH] = get_speed(ch,SPEED_RH);
    }
   }

   /* Tail attack (if we have one) */
   if( IS_SET(race_table[ch->race].race_flags,RACE_MOD_TAIL) )
   {
    ch->speed[SPEED_TAIL] -= 0.01;

    if( ch->speed[SPEED_TAIL] <= 0 )
    {
     one_hit(ch,victim,(TYPE_HIT + 13));
     ch->speed[SPEED_TAIL] = get_speed(ch,SPEED_TAIL);
    }
   }
  }
  ++li;
 }
}

float combat_damcap( CHAR_DATA *ch, CHAR_DATA *victim, float dam, int dt )
{
 char buf[MSL];

 if( dam > sysdata.damcap )
 {
  snprintf( buf, MSL, "Combat: %1.0f damage by %s, attacking %s, dt %d", dam, ch->get_name(), victim->get_name(), dt );
  if( ch->level < 82 )
  {
   monitor_chan( buf, MONITOR_COMBAT );
   log_f( "%s", buf );
  }
  dam = sysdata.damcap;
 }

 return dam;
}

void check_brawl( CHAR_DATA *ch )
{
 #define MAX_BRAWLS 10
 CHAR_DATA *fight[MAX_BRAWLS], *rch, *rch_next, *vch, *vch_next;
 short i = 0, x = 0;

 for( i = 0; i < MAX_BRAWLS; i++ )
  fight[i] = NULL;

 for( rch = ch->in_room->first_person; rch != NULL; rch = rch_next )
 {
  rch_next = rch->next_in_room;

  if( rch->fighting == NULL && IS_AWAKE(rch) && rch->master != ch && !IS_IMMORTAL(rch) && number_percent() <= 2 )
  {
   if( IS_NPC(rch) && (rch->act.test(ACT_TRAIN) || rch->act.test(ACT_PRACTICE) || rch->npcdata->pIndexData->pShop != NULL) )
    continue;

   for( vch = ch->in_room->first_person; vch != NULL; vch = vch_next )
   {
    vch_next = vch->next_in_room;

    if( vch->fighting == NULL && rch->master != ch && !IS_IMMORTAL(vch) && i < MAX_BRAWLS )
    {
     fight[i] = vch;
     i++;
    }
   }
   x = number_range(0,i); /* Only search as many as we found */
   if( fight[x] != NULL )
   {
    if( !IS_NPC(rch) && !IS_NPC(fight[x]) ) /* Don't force PVP */
     continue;
    if( !can_see(rch,fight[x]) )
    continue;
    rch->fighting = fight[x];
    rch->position = POS_FIGHTING;
    act("@@eYou find yourself caught up in the brawl!@@N",rch,NULL,NULL,TO_CHAR);
    act("@@e$n finds $mself caught up in the brawl!@@N",rch,NULL,NULL,TO_ROOM);
   }
  }
 }
}

void damage_gear( CHAR_DATA *ch )
{
 OBJ_DATA *obj;

 for( obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list )
 {
  if( obj->level == 1 ) // Save newbie gear
   continue;
  if( obj->wear_loc == WEAR_NONE )
   continue;
  if( number_percent() < 15 )
   obj->durability--;
  if( obj->durability == 1 )
  {
   act("@@y$p breaks and falls off.@@N",ch,obj,NULL,TO_CHAR);
   act("@@y$n's $p breaks and falls off.@@N",ch,obj,NULL,TO_ROOM);
   remove_obj(ch,obj->wear_loc,TRUE);
  }
 }
}

DO_FUN(do_disguise)
{
 char *farg = argument;

 if( !IS_NPC(ch) && ch->pcdata->learned[gsn_disguise] == 0 )
 {
  send_to_char("You are not trained in this skill!\r\n",ch);
  return;
 }
 if( farg[0] == '\0' )
 {
  send_to_char("Enter the disguise name or [reset] to reset your description to normal.\r\n",ch);
  return;
 }
 if( !str_cmp(farg,"reset") )
 {
  ch->long_descr = ch->long_descr_orig;
  send_to_char("You have removed your disguise.\r\n",ch);
  return;
 }
 else
 {
  ch->long_descr = farg;
  send_to_char("You are now Disguised!!!\r\n",ch);
  return;
 }

 return;
}

DO_FUN(do_stance)
{
   char arg[MAX_STRING_LENGTH];
   bool legal_stance = FALSE;
   short i;

   if( ch->check_cooldown(COOLDOWN_DEF) )
    return;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Not for Npcs!\r\n", ch );
      return;
   }
   if( IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "This form is not capable of fighting Stances.\r\n", ch );
      return;
   }

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      char cat_buf[MSL];
      char msg_buf[MSL];


      snprintf( msg_buf, MSL, "\r\n%s\r\n", "Fighting Stances available to you:\r\n" );

      for( i = 0; i < MAX_STANCE; i++ )
      {

         switch ( i )
         {
            case STANCE_WARRIOR: /* Adventurer */
               snprintf( cat_buf, MSL, "%s\r\n", stance_app[i].name );
               break;
            case STANCE_CASTER: /* Mage */
               if( ( ch->get_level("mag") > 50 ) /* mage */
                   || ( ch->get_level("cle") > 70 ) /* cleric */
                   || ( ch->get_level("psi") > 60 ) )  /* psi */
                  snprintf( cat_buf, MSL, "%s\r\n", stance_app[i].name );
               break;
            case STANCE_AMBUSH: /* Ninja */
               if( ch->get_level("ass") > 30 )  /* assassin */
                  snprintf( cat_buf, MSL, "%s\r\n", stance_app[i].name );
               break;
            case STANCE_AC_BEST: /* Shadows */
               if( ( ch->get_level("mon") > 65 )   /* monk */
                   || ( ch->get_level("ass") > 30 ) ) /* assassin */
                  snprintf( cat_buf, MSL, "%s\r\n", stance_app[i].name );
               break;
            case STANCE_HR_BEST: /* Essence */
               if( ( ch->get_level("kni") > 45 )   /* knight */
                   || ( ch->get_level("mon") > 20 ) ) /* monk */
                  snprintf( cat_buf, MSL, "%s\r\n", stance_app[i].name );
               break;
            case STANCE_DR_BEST: /* Beast */
               if( ( ch->get_level("kni") > 35 )   /* knight */
                   || ( ch->get_level("mon") > 10 ) ) /* monk */
                  snprintf( cat_buf, MSL, "%s\r\n", stance_app[i].name );
               break;
            case STANCE_AC_WORST: /* Flame */
               if( ch->get_level("mon") > 45 )  /* monk */
                  snprintf( cat_buf, MSL, "%s\r\n", stance_app[i].name );
               break;
            case STANCE_HR_WORST: /* Spirit */
               if( ch->get_level("mon") > 60 )  /* monk */
                  snprintf( cat_buf, MSL, "%s\r\n", stance_app[i].name );
               break;
            case STANCE_DR_WORST: /* Void */
               if( ch->get_level("mon") > 70 )  /* monk */
                  snprintf( cat_buf, MSL, "%s\r\n", stance_app[i].name );
               break;
            case STANCE_SUPER_FIGHTER: /* Dragon */
               if( ( ch->get_level("mon") > 79 ) && ( ch->get_level("kni") > 79 ) ) /* both knight and monk */
                  snprintf( cat_buf, MSL, "%s\r\n", stance_app[i].name );
               break;
            case STANCE_SUPER_SPEED: /* Snake */
               if( ( ch->get_level("mon") > 70 ) && ( ch->get_level("kni") > 70 ) ) /* both knight and monk */
                  snprintf( cat_buf, MSL, "%s\r\n", stance_app[i].name );
               break;
            case STANCE_WIZARD: /* Wizard */
               if( ( ch->get_level("sor") > 20 )   /* sorc */
                   || ( ch->get_level("nec") > 40 )   /* necro */
                   || ( ch->get_level("mon") > 60 ) ) /* monk */
                  snprintf( cat_buf, MSL, "%s\r\n", stance_app[i].name );
               break;
            case STANCE_MAGI: /* Magi */
               if( IS_ADEPT(ch) && ch->get_level("adept") > 10 )   /*adept */
                  snprintf( cat_buf, MSL, "%s\r\n", stance_app[i].name );
               break;

         }

         strncat( msg_buf, cat_buf, MSL-1 );
         snprintf( cat_buf, MSL, "%s", "" );
      }
      snprintf( cat_buf, MSL, "%s",
               "Type stance <stance name> to change your current fighting stance.\r\n You may place your current Stance in your prompt with a \%s\r\n" );
      strncat( msg_buf, cat_buf, MSL-1 );
      send_to_char( msg_buf, ch );
      return;
   }

   for( i = 0; i < MAX_STANCE; i++ )
      if( !str_prefix( arg, stance_app[i].name ) )
         break;
   if( i == MAX_STANCE )
   {
      do_stance( ch, "" );
      act( "$n poses in a strange fashion, looking rather silly.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You twist about wildly, but are unable to figure out just the right Stance.\r\n", ch );
   }

   else
   {
      switch ( i )
      {
         case STANCE_WARRIOR:
         {
            legal_stance = TRUE;
            break;
         }
         case STANCE_CASTER:
            if( ( ch->get_level("mag") > 50 ) /* mage */
                || ( ch->get_level("cle") > 70 ) /* cleric */
                || ( ch->get_level("psi") > 60 ) )  /* psi */
            {
               legal_stance = TRUE;
               break;
            }
            break;
         case STANCE_WIZARD:
            if( ( ch->get_level("sor") > 20 )   /* sorc */
                || ( ch->get_level("nec") > 40 )   /* necro */
                || ( ch->get_level("mon") > 60 ) ) /* monk */
            {
               legal_stance = TRUE;
               break;
            }
            break;
         case STANCE_MAGI:
            if( IS_ADEPT(ch) && ch->get_level("adept") > 10 )   /*adept */
            {
               legal_stance = TRUE;
               break;
            }
            break;
         case STANCE_AMBUSH:
            if( ch->get_level("ass") > 30 )  /* assassin */
            {
               CHAR_DATA *other;
               for( other = ch->in_room->first_person; other != NULL; other = other->next_in_room )
                  if( ( other != ch ) && ( other != NULL ) )
                     break;
               if( other != NULL )
               {
                  send_to_char( "You can't set an ambush with people watching you!\r\n", ch );
               }
               else
               {
                  legal_stance = TRUE;
               }
               break;
            }
            break;
         case STANCE_AC_BEST:
            if( ( ch->get_level("kni") > 65 )   /* knight */
                || ( ch->get_level("ass") > 30 ) ) /* assassin */
            {
               legal_stance = TRUE;
               break;
            }
            break;

         case STANCE_HR_BEST:
            if( ( ch->get_level("kni") > 45 )   /* knight */
                || ( ch->get_level("mon") > 20 ) ) /* monk */
            {
               legal_stance = TRUE;
               break;
            }
            break;

         case STANCE_DR_BEST:
            if( ( ch->get_level("kni") > 35 )   /* knight */
                || ( ch->get_level("mon") > 10 ) ) /* monk */
            {
               legal_stance = TRUE;
               break;
            }
            break;

         case STANCE_AC_WORST:
            if( ch->get_level("mon") > 45 )  /* monk */
            {
               legal_stance = TRUE;
               break;
            }
            break;

         case STANCE_HR_WORST:
            if( ch->get_level("mon") > 60 )  /* monk */
            {
               legal_stance = TRUE;
               break;
            }
            break;

         case STANCE_DR_WORST:
            if( ch->get_level("mon") > 70 )  /* monk */
            {
               legal_stance = TRUE;
               break;
            }
            break;

         case STANCE_SUPER_FIGHTER:
            if( ( ch->get_level("kni") > 79 ) && ( ch->get_level("mon") > 79 ) ) /* both knight and monk */
            {
               legal_stance = TRUE;
               break;
            }
            break;

         case STANCE_SUPER_SPEED:
            if( ( ch->get_level("kni") > 70 ) && ( ch->get_level("mon") > 70 ) ) /* both knight and monk */
            {
               legal_stance = TRUE;
               break;
            }
            break;


      }

      if( legal_stance )
      {
         char stance_buf[MSL];
         if( stance_app[i].ac_mod > 0 )
            ch->stance_ac_mod = ( stance_app[i].ac_mod * ( 20 - ch->get_level("psuedo") / 12 ) );
         else
            ch->stance_ac_mod = stance_app[i].ac_mod * ch->get_level("psuedo") / 12;

         if( stance_app[i].dr_mod < 0 )
            ch->stance_dr_mod = ( stance_app[i].dr_mod * ( 20 - ch->get_level("psuedo") / 12 ) );
         else
            ch->stance_dr_mod = stance_app[i].dr_mod * ch->get_level("psuedo") / 10;

         if( stance_app[i].hr_mod < 0 )
            ch->stance_hr_mod = ( stance_app[i].hr_mod * ( 20 - ch->get_level("psuedo") / 12 ) );
         else
            ch->stance_hr_mod = stance_app[i].hr_mod * ch->get_level("psuedo") / 10;

         ch->stance = i;
         snprintf( stance_buf, MSL, "$n assumes the Stance of the %s!", stance_app[i].name );
         act( stance_buf, ch, NULL, NULL, TO_ROOM );
         snprintf( stance_buf, MSL,"You assume the Stance of the %s!\r\n", stance_app[i].name );
         send_to_char( stance_buf, ch );
      }
      ch->set_cooldown(COOLDOWN_DEF,3.50);
      return;
   }
}

void resurrect( CHAR_DATA *ch )
{
 if( !IS_GHOST(ch) )
  return;

 ch->act.reset(ACT_GHOST);
 ch->hit = number_range(1,(int)(ch->max_hit * 0.10));
 ch->move = number_range(1,(int)(ch->max_move * 0.10));
 ch->mana = number_range(1,(int)(ch->max_mana * 0.10));
 send_to_char("@@yYou have been granted life!@@N\r\n",ch);
 save_char_obj(ch);

 act("$n's soul returns to $s body!",ch,NULL,NULL,TO_ROOM);

 return;
}

void cooldown_update( void )
{
 list<CHAR_DATA *>::iterator li;
 CHAR_DATA *ch;

 for( li = char_list.begin(); li != char_list.end(); li++ )
 {
  ch = *li;

  if( !HAS_COOLDOWN(ch) )
   continue;

  if( ch->cooldown[COOLDOWN_OFF] > 0 )
  {
   ch->cooldown[COOLDOWN_OFF] -= 0.01;
   if( ch->cooldown[COOLDOWN_OFF] <= 0 && !ch->act.test(ACT_NO_COOLDOWN) )
    send_to_char("@@eYour offensive cooldown has been refreshed!@@N\r\n",ch);
  }

  if( ch->cooldown[COOLDOWN_DEF] > 0 )
  {
   ch->cooldown[COOLDOWN_DEF] -= 0.01;
   if( ch->cooldown[COOLDOWN_DEF] <= 0 && !ch->act.test(ACT_NO_COOLDOWN) )
    send_to_char("@@lYour defensive cooldown has been refreshed!@@N\r\n",ch);
  }
 }

 return;
}

void aggro_attack( CHAR_DATA *mob, CHAR_DATA *player )
{
 act("$n growls at $N!",player,NULL,mob,TO_NOTVICT);
 act("$N growls at you! Uh-oh!!",player,NULL,mob,TO_CHAR);
 act("You growl at $N. Get $M!!",mob,NULL,player,TO_CHAR);

 OBJ_DATA *wield = get_eq_char(mob,WEAR_HOLD_HAND_L);
 if( wield != NULL && wield->item_type == ITEM_WEAPON && wield->value[3] == 11 && player->fighting == NULL )
  do_backstab(mob,const_cast<char *>(player->name.c_str()));
 else
  one_hit(mob,player,TYPE_UNDEFINED);

 return;
}

void remember_attack( CHAR_DATA *mob, CHAR_DATA *player )
{
 /*
  * Called when an NPC ch encounters a PC victim, that tried to
  * kill it previously.
  * --Stephen
  *
  * Spruced up a bit. --Kline
  */

  int first = 0, last = 0;
  char buf[MSL];

 /*
  * Pick a random response for mob to give before attacking.
  */

  switch( number_range(0,6) )
  {
   case 0:
    snprintf(buf,MSL,"%s returns! I shall have my revenge at last!",player->name.c_str());
    do_yell(mob,buf);
    break;
   case 1:
    snprintf(buf,MSL,"You should never have returned %s. Ye shall DIE!",player->name.c_str());
    do_whisper(mob,buf);
    break;
   case 2:
    act("$n looks at $N, remembering $S attack.",mob,NULL,player,TO_ROOM);
    act("$n looks at you, remembering your attack.",mob,NULL,player,TO_VICT);
    act("You look at $N, remembering $S attack.",mob,NULL,player,TO_CHAR);
    do_say(mob,"I SHALL HAVE MY REVENGE!!!");
    break;
   case 3:
    snprintf(buf,MSL,"%s has wronged me, and now I will seek my revenge!",player->name.c_str());
    do_gossip(mob,buf);
    snprintf(buf,MSL,"Prepare to die, %s.",player->name.c_str());
    do_say(mob,buf);
    break;
   case 4:
    snprintf(buf,MSL,"So, %s. You have returned. Let us finish our fight this time!",player->name.c_str());
    do_say(mob,buf);
    break;
   case 5:
    snprintf(buf,MSL,"Only cowards flee from me, %s!",player->name.c_str());
    do_say(mob,buf);
    break;
   case 6:
    act("$n looks at $N, and recognizes $M!!",mob,NULL,player,TO_ROOM);
    act("$n looks at you, and recognizes you!!",mob,NULL,player,TO_VICT);
    act("You look at $N, and recognize $M!",mob,NULL,player,TO_CHAR);
    snprintf(buf,MSL,"There can only be one winner, %s.",player->name.c_str());
    do_say(mob,buf);
    break;
 }

 OBJ_DATA *wield = get_eq_char(mob,WEAR_HOLD_HAND_L);
 if( wield != NULL && wield->item_type == ITEM_WEAPON && wield->value[3] == 11 && player->fighting == NULL )
  do_backstab(mob,const_cast<char *>(player->name.c_str()));
 else
  one_hit(mob,player,TYPE_UNDEFINED);

 first = static_cast<int>(mob->target.find(player->name));
 last = (strlen(player->name.c_str()) + 1);
 mob->target.erase(first,last);

 return;
}
