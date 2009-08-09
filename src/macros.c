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

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_MACROS_H
#include "h/macros.h"
#endif

void reset_gain_stats( CHAR_DATA * ch )
{
   short index = 0;
   short index2 = 0;
   short add_move = 0;
   short add_mana = 0;
   short add_hp = 0;

   ch->pcdata->mana_from_gain = 100;
   ch->pcdata->hp_from_gain = 25;
   ch->pcdata->move_from_gain = 0;


   for( index = 0; index < MAX_CLASS; index++ )
   {
      if( ch->lvl[index] > 0 )
         for( index2 = 1; index2 <= ch->lvl[index]; index2++ )
         {
            add_hp = con_app[ch->pcdata->max_con].hitp + number_range( class_table[index].hp_min,
                                                                       class_table[index].hp_max );

            add_mana = class_table[index2].fMana
               ? number_range( 2, ( 2 * ch->pcdata->max_int + ch->pcdata->max_wis ) / 16 ) : 0;
            add_move = number_range( 2, ( ch->pcdata->max_con + ch->pcdata->max_dex ) / 5 );
            add_hp = UMAX( 1, add_hp );
            add_mana = UMAX( 0, add_mana );
            add_move = UMAX( 7, add_move );

            ch->pcdata->mana_from_gain += add_mana;
            ch->pcdata->hp_from_gain += add_hp;
            ch->pcdata->move_from_gain += add_move;

         }
      if( ch->lvl2[index] > 0 )
         for( index2 = 1; index2 <= ch->lvl2[index]; index2++ )
         {
            add_hp = con_app[ch->pcdata->max_con].hitp + number_range( remort_table[index].hp_min,
                                                                       remort_table[index].hp_max );
            add_mana = remort_table[index].fMana
               ? number_range( 2, ( 2 * ch->pcdata->max_int + ch->pcdata->max_wis ) / 16 ) : 0;
            add_move = number_range( 2, ( ch->pcdata->max_con + ch->pcdata->max_dex ) / 5 );
            add_hp = UMAX( 1, add_hp );
            add_mana = UMAX( 0, add_mana );
            add_move = UMAX( 7, add_move );

            ch->pcdata->mana_from_gain += add_mana;
            ch->pcdata->hp_from_gain += add_hp;
            ch->pcdata->move_from_gain += add_move;

         }

   }


   if( IS_ADEPT(ch) )
      for( index2 = 1; index2 <= ch->get_level("adept"); index2++ )
      {

         add_hp = con_app[ch->pcdata->max_con].hitp + number_range( 10, 50 );
         add_mana = number_range( 10, ( 3 * ch->pcdata->max_int + ch->pcdata->max_wis ) / 4 );
         add_hp = UMAX( 1, add_hp );
         add_mana = UMAX( 0, add_mana );

         ch->pcdata->mana_from_gain += add_mana;
         ch->pcdata->hp_from_gain += add_hp;

      }
}

bool ok_to_use( CHAR_DATA * ch, int value )
{
   if( value == NO_USE && get_trust( ch ) < 85 )
   {
      send_to_char( "Only Creators may use this value.\r\n", ch );
      return FALSE;
   }
   return TRUE;
}

bool check_level_use( CHAR_DATA * ch, int level )
{
   char buf[MAX_STRING_LENGTH];
   char out[MAX_STRING_LENGTH];

   if( get_trust( ch ) >= level )
      return TRUE;

   snprintf( out, MSL, "This option limited to " );

   switch ( level )
   {
      case 85:
         strncat( out, "Creators only.\r\n", MSL );
         break;
      case 84:
         strncat( out, "Supremes or higher.\r\n", MSL );
         break;
      case 83:
         strncat( out, "Dieties or higher.\r\n", MSL );
         break;
      case 82:
         strncat( out, "Immortals or higher.\r\n", MSL );
         break;
      case 81:
         strncat( out, "Adepts or higher.\r\n", MSL );
         break;
      default:
         snprintf( buf, MSL, "level %d players and higher.\r\n", level );
         strncat( out, buf, MSL-1 );
   }
   send_to_char( out, ch );
   return FALSE;
}



int exp_to_level_adept( CHAR_DATA * ch )
{
   int exp;

   exp = ( 30000 + ( ch->get_level("adept") * 5000 ) );
   exp = UMAX( exp, exp * ch->get_level("adept") / 2 );
   return exp;
}



int exp_to_level( CHAR_DATA * ch, int p_class, int index )
{
 /*
  * To get remort costs, call with index == 5
  */

 int max_level = 0;
 int level, next_level_index, diff, totlevels = 0;
 float cost;
 short i, mult;


 if( (index == 5) && (ch->lvl2[p_class] <= 0) ) /* Freebie on that first one ;) */
  return 0;

 for( i = 0; i < MAX_CLASS; i++ ) /* Find the highest level of any class a player has */
  if( ch->lvl[i] > max_level )
   max_level = ch->lvl[i];

 switch( index ) /* Classes cost less -> more based on order */
 {
  case 0:  mult = 3;  break;
  case 1:  mult = 4;  break;
  case 2:  mult = 5;  break;
  case 3:  mult = 6;  break;
  case 4:  mult = 7;  break;
  default: mult = 23; break; /* Remort classes, bugs :P */
 }

 if( index == 5 )
  level = UMAX(0,ch->lvl2[p_class]); /* Grab the remort class level */
 else
  level = UMAX(0,ch->lvl[p_class]);  /* Grab the mortal class level */

 /*
  * Adjust level to make costs higher
  */
 for( i = 0; i < MAX_CLASS; i++ )
 {
  totlevels += ch->lvl[i];
  if( ch->lvl2[i] > 0 )
   totlevels += ch->lvl2[i];
 }

 if( index != 5 )
  next_level_index = ch->lvl[p_class];
 else
  next_level_index = UMIN((ch->lvl2[p_class] + 20),79);

 if( next_level_index < 0 )
  next_level_index = 0;

 cost = (exp_mob_base(next_level_index) * sysdata.killperlev);

 /*
  * Now multiply by a factor dependant on total number of levels 
  */
 diff = (totlevels / MAX_CLASS) - (level + 20);

 if( index == 5 )
  diff -= 30;
 if( diff < 10 )
  diff = 10;

 /*
  * Discourage uneven levelling
  */
 cost *= (diff / 10);

 /*
  * REALLY discourage uneven levelling :P
  */
 if( (index != 5) && ((ch->level - ch->lvl[p_class]) > 25) )
  cost *= (diff / 7);

 /*
  * Now multiply by order index/remort index...other factors will come here later, like race mod, etc.
  */
 cost *= mult;

 /*
  * Now refudge the order multiplier... Divide by some factor, 5.4 feels nice and arbitrary :P
  */
 cost /= 5.4;

 return (int)cost;
}

int exp_to_level_vamp( int level )
{

   int exp = 0;

   switch ( level )
   {
      case 0:
      case 1:
      case 3:
      case 4:
      case 5:
         exp = ( 250 + level * 5 );
         break;
      case 6:
      case 7:
      case 8:
      case 9:
         exp = ( 300 + level * 6 );
         break;
      case 10:
      case 11:
      case 12:
         exp = ( 350 + level * 5 );
         break;
      case 13:
      case 14:
      case 15:
      case 16:
         exp = ( 450 + level * 6 );
         break;
      case 17:
      case 18:
      case 19:
      case 20:
         exp = ( 500 + level * 5 );
         break;

   }
   return ( exp );

}

int exp_to_level_wolf( int level )
{

   float exp = 0;

   switch ( level )
   {
      case 0:
      case 1:
      case 3:
      case 4:
      case 5:
         exp = ( 250 + level * 5 );
         break;
      case 6:
      case 7:
      case 8:
      case 9:
         exp = ( 300 + level * 6 );
         break;
      case 10:
      case 11:
      case 12:
         exp = ( 400 + level * 6 );
         break;
      case 13:
      case 14:
      case 15:
      case 16:
         exp = ( 550 + level * 6.5 );
         break;
      case 17:
      case 18:
      case 19:
      case 20:
         exp = ( 500 + level * 7 );
         break;

   }
   return (int)exp;

}

int exp_mob_base( int level )
{
 float value = 0;
 short i = 0;

 for( i = -1; i < level; i++ )
 {
  value += 100;
  value *= 1.04;
 }

 if( value < 1 )
  value = 100;

 return (int)value;
}

int exp_for_mobile( int level, CHAR_DATA * mob )
{

   float value, base_value = 0;

   base_value = exp_mob_base(level);
   value = base_value;

/* now we have the base for the mobs level..let's add multipliers based on the skills it has
   thse multpliers should add up to no more than 150% of the base, for a total of 2.5 times base
   max exp for the mob--that's with EVERY skill in the book :)   */


   if( IS_SET( mob->npcdata->skills, MOB_REFLEXES ) )
      value += .05 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_SLEIGHT ) )
      value += .200 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_CRUSHING ) )
      value += .1 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_PROWESS ) )
      value += .200 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_QUICKSTRIKE ) )
      value += .1 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_PUNCH ) )
      value += .050 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_HEADBUTT ) )
      value += .100 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_KNEE ) )
      value += .050 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_DISARM ) )
      value += .050 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_TRIP ) )
      value += .050 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_NODISARM ) )
      value += .150 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_DODGE ) )
      value += .150 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_PARRY ) )
      value += .05 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_MARTIAL ) )
      value += .200 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_ENHANCED ) )
      value += .300 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_NOTRIP ) )
      value += .100 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_DUALWIELD ) )
      value += .050 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_DIRT ) )
      value += .150 * base_value;
   if( IS_SET( mob->npcdata->skills, MOB_CHARGE ) )
      value += .050 * base_value;


   if( IS_AFFECTED( mob, AFF_SANCTUARY ) )
      value += .400 * base_value;
   if( IS_AFFECTED( mob, AFF_INVISIBLE ) )
      value += .050 * base_value;
   if( IS_AFFECTED( mob, AFF_DETECT_INVIS ) )
      value += .100 * base_value;
   if( IS_AFFECTED( mob, AFF_ANTI_MAGIC ) )
      value += .350 * base_value;

   if( IS_AFFECTED( mob, AFF_CLOAK_FLAMING ) )
      value += .600 * base_value;
   if( IS_AFFECTED( mob, AFF_CLOAK_ABSORPTION ) )
      value += .150 * base_value;
   if( IS_AFFECTED( mob, AFF_CLOAK_REFLECTION ) )
      value += .350 * base_value;

   if( ( IS_AFFECTED( mob, AFF_CLOAK_REFLECTION ) )
       && ( IS_AFFECTED( mob, AFF_CLOAK_ABSORPTION ) ) && ( IS_AFFECTED( mob, AFF_CLOAK_FLAMING ) ) )
      value += .200 * base_value;


   if( IS_SET( mob->npcdata->def, DEF_CURE_LIGHT ) )
      value += .100 * base_value;
   if( IS_SET( mob->npcdata->def, DEF_CURE_SERIOUS ) )
      value += .200 * base_value;
   if( IS_SET( mob->npcdata->def, DEF_CURE_CRITIC ) )
      value += .350 * base_value;
   if( IS_SET( mob->npcdata->def, DEF_CURE_HEAL ) )
      value += .400 * base_value;
   if( IS_SET( mob->npcdata->def, DEF_SHIELD_FIRE ) )
      value += .300 * base_value;
   if( IS_SET( mob->npcdata->def, DEF_SHIELD_SHOCK ) )
      value += .200 * base_value;
   if( IS_SET( mob->npcdata->def, DEF_SHIELD_ICE ) )
      value += .100 * base_value;

   if( mob->act.test(ACT_SOLO) )
      value += .500 * base_value;



   return (int)value;
}


/*
 * Functions to return details regarding a PC and skill_table
 * Uses 2 #defined values in merc.h to determine what to return
 * These also make adapting to remort classes a lot easier - all
 * the code goes here instead of in all the skills and do_cast()
 *
 * skill_table[] now includes flags to determine if for mortals
 * or remortals, so use that here, and use correct array.
 */

int skill_table_lookup( CHAR_DATA * ch, int sn, int return_type )
{
   int best_class = -1;
   int best_level = -1;
   int return_value;
   int cnt;

   if( IS_NPC( ch ) )
   {
      best_class = ch->p_class;
      best_level = ch->level;
   }
   else
   {
      /*
       * Check normal classes first 
       */

      switch ( skill_table[sn].flag1 )
      {
         case MORTAL:
            for( cnt = 0; cnt < MAX_CLASS; cnt++ )
            {
               if( ch->lvl[cnt] >= skill_table[sn].skill_level[cnt] && ch->lvl[cnt] > best_level )
               {
                  best_level = ch->lvl[cnt];
                  best_class = cnt;
               }
            }
            break;
         case REMORT:
            for( cnt = 0; cnt < MAX_CLASS; cnt++ )
            {
               if( ch->lvl2[cnt] >= skill_table[sn].skill_level[cnt] && ch->lvl2[cnt] > best_level )
               {
                  best_level = ch->lvl2[cnt];
                  best_class = cnt;
               }
            }
            break;
      }
   }
   switch ( return_type )
   {
      case RETURN_BEST_CLASS:
         return_value = best_class;
         break;
      case RETURN_BEST_LEVEL:
         return_value = best_level;
         break;
      default:
         bug( "skill_table_lookup: invalid return_type:%d", return_type );
         return_value = -1;
         break;
   }
   return ( return_value );
}

int get_item_value( OBJ_DATA * obj )
{
   AFFECT_DATA *this_aff;
   int ac_mod = 0;
   int dr_mod = 0;
   int hp_mod = 0;
   int hr_mod = 0;
   int mana_mod = 0;
   int move_mod = 0;
   int save_mod = 0;
   float cost = 0;
   short wear_loc = WEAR_NONE;
   char buf[MSL];



/* fix this up to use apply_ac by getting the wear loc */
   for( wear_loc = 1; wear_loc < MAX_WEAR; wear_loc++ )
   {
      if( CAN_WEAR( obj, wear_loc ) )
         break;
   }
   if( wear_loc == MAX_WEAR )
   {
      /*
       * snprintf( buf, MSL, "Object has no wear loc" );
       * monitor_chan( buf, MONITOR_OBJ );
       */
      ac_mod = 0;
   }
   else
      ac_mod = apply_ac( obj, wear_loc );

   for( this_aff = obj->first_apply; this_aff != NULL; this_aff = this_aff->next )
   {
      switch ( this_aff->location )
      {
         default:
            bug( "Get_item_value: unknown location %d.", this_aff->location );
            snprintf( buf, MSL, "Get_item_value called for unknown location %d.", this_aff->location );
            monitor_chan( buf, MONITOR_OBJ );
            break;
         case APPLY_NONE:
            break;
         case APPLY_STR:
            break;
         case APPLY_DEX:
            break;
         case APPLY_INT:
            break;
         case APPLY_WIS:
            break;
         case APPLY_CON:
            break;
         case APPLY_SEX:
            break;
         case APPLY_CLASS:
            break;
         case APPLY_LEVEL:
            break;
         case APPLY_AGE:
            break;
         case APPLY_HEIGHT:
            break;
         case APPLY_WEIGHT:
            break;
         case APPLY_MANA:
            mana_mod += this_aff->modifier;
            break;
         case APPLY_HIT:
            hp_mod += this_aff->modifier;
            break;
         case APPLY_MOVE:
            move_mod += this_aff->modifier;
            break;
         case APPLY_EXP:
            break;
         case APPLY_AC:
            ac_mod += this_aff->modifier;
            break;
         case APPLY_HITROLL:
            hr_mod += this_aff->modifier;
            break;
         case APPLY_DAMROLL:
            dr_mod += this_aff->modifier;
            break;
         case APPLY_SAVING_PARA:
         case APPLY_SAVING_ROD:
         case APPLY_SAVING_PETRI:
         case APPLY_SAVING_BREATH:
         case APPLY_SAVING_SPELL:
            save_mod += this_aff->modifier;
            break;
      }
   }

   cost = obj->level * 1 + ac_mod * -8 + dr_mod * 5 + hr_mod * 5 + save_mod * 2 + hp_mod * 4 + mana_mod * 3 + move_mod;

   if( IS_SET( obj->item_apply, ITEM_APPLY_ENHANCED ) )
      cost = cost * 1.3;

   if( IS_SET( obj->item_apply, ITEM_APPLY_SANC ) )
      cost = cost * 1.3;

   snprintf( buf, MSL, "Cost computed for item %.0f.", cost );
/*  monitor_chan( buf, MONITOR_OBJ );  */

   if( obj->item_type == ITEM_ENCHANTMENT )
      cost = abs( obj->value[1] * 100 );
   return UMAX( 10, (int)cost );

   return -1;
}
