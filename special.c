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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"

#ifndef DEC_MAGIC_H
#include "magic.h"
#endif


/*
 * The following special functions are available for mobiles.
 */
DECLARE_SPEC_FUN( spec_breath_any );
DECLARE_SPEC_FUN( spec_breath_acid );
DECLARE_SPEC_FUN( spec_breath_fire );
DECLARE_SPEC_FUN( spec_breath_frost );
DECLARE_SPEC_FUN( spec_breath_gas );
DECLARE_SPEC_FUN( spec_breath_lightning );
DECLARE_SPEC_FUN( spec_cast_adept );
DECLARE_SPEC_FUN( spec_cast_cleric );
DECLARE_SPEC_FUN( spec_cast_judge );
DECLARE_SPEC_FUN( spec_cast_mage );
DECLARE_SPEC_FUN( spec_cast_undead );
DECLARE_SPEC_FUN( spec_executioner );
DECLARE_SPEC_FUN( spec_fido );
DECLARE_SPEC_FUN( spec_guard );
DECLARE_SPEC_FUN( spec_janitor );
DECLARE_SPEC_FUN( spec_mayor );
DECLARE_SPEC_FUN( spec_poison );
DECLARE_SPEC_FUN( spec_thief );
DECLARE_SPEC_FUN( spec_policeman );
DECLARE_SPEC_FUN( spec_cast_cadaver );
DECLARE_SPEC_FUN( spec_undead );
DECLARE_SPEC_FUN( spec_stephen );
DECLARE_SPEC_FUN( spec_rewield );
DECLARE_SPEC_FUN( spec_sylai_priest );
DECLARE_SPEC_FUN( spec_cast_bigtime );
DECLARE_SPEC_FUN( spec_sage );
DECLARE_SPEC_FUN( spec_wizardofoz );
DECLARE_SPEC_FUN( spec_vamp_hunter );
DECLARE_SPEC_FUN( spec_mino_guard );
DECLARE_SPEC_FUN( spec_tax_man );

/*
 * Given a name, return the appropriate spec fun.
 */


SPEC_FUN *spec_lookup( const char *name )
{
   if( !str_cmp( name, "spec_breath_any" ) )
      return spec_breath_any;
   if( !str_cmp( name, "spec_breath_acid" ) )
      return spec_breath_acid;
   if( !str_cmp( name, "spec_breath_fire" ) )
      return spec_breath_fire;
   if( !str_cmp( name, "spec_breath_frost" ) )
      return spec_breath_frost;
   if( !str_cmp( name, "spec_breath_gas" ) )
      return spec_breath_gas;
   if( !str_cmp( name, "spec_breath_lightning" ) )
      return spec_breath_lightning;
   if( !str_cmp( name, "spec_cast_adept" ) )
      return spec_cast_adept;
   if( !str_cmp( name, "spec_cast_cleric" ) )
      return spec_cast_cleric;
   if( !str_cmp( name, "spec_cast_judge" ) )
      return spec_cast_judge;
   if( !str_cmp( name, "spec_cast_mage" ) )
      return spec_cast_mage;
   if( !str_cmp( name, "spec_cast_undead" ) )
      return spec_cast_undead;
   if( !str_cmp( name, "spec_executioner" ) )
      return spec_executioner;
   if( !str_cmp( name, "spec_fido" ) )
      return spec_fido;
   if( !str_cmp( name, "spec_guard" ) )
      return spec_policeman;
   if( !str_cmp( name, "spec_janitor" ) )
      return spec_janitor;
   if( !str_cmp( name, "spec_mayor" ) )
      return spec_mayor;
   if( !str_cmp( name, "spec_poison" ) )
      return spec_poison;
   if( !str_cmp( name, "spec_thief" ) )
      return spec_thief;
   if( !str_cmp( name, "spec_policeman" ) )
      return spec_policeman;
   if( !str_cmp( name, "spec_cast_cadaver" ) )
      return spec_cast_cadaver;
   if( !str_cmp( name, "spec_undead" ) )
      return spec_undead;
   if( !str_cmp( name, "spec_stephen" ) )
      return spec_stephen;
   if( !str_cmp( name, "spec_rewield" ) )
      return spec_rewield;
   if( !str_cmp( name, "spec_sylai_priest" ) )
      return spec_sylai_priest;
   if( !str_cmp( name, "spec_cast_bigtime" ) )
      return spec_cast_bigtime;
   if( !str_cmp( name, "spec_sage" ) )
      return spec_sage;
   if( !str_cmp( name, "spec_wizardofoz" ) )
      return spec_wizardofoz;
   if( !str_cmp( name, "spec_vamp_hunter" ) )
      return spec_vamp_hunter;
   if( !str_cmp( name, "spec_mino_guard" ) )
      return spec_mino_guard;
   if( !str_cmp( name, "spec_tax_man" ) )
      return spec_tax_man;

   return 0;
}


/* MAG Mod */

char *rev_spec_lookup( void *func )
{
   if( func == spec_breath_any )
      return "spec_breath_any";
   if( func == spec_breath_acid )
      return "spec_breath_acid";
   if( func == spec_breath_fire )
      return "spec_breath_fire";
   if( func == spec_breath_frost )
      return "spec_breath_frost";
   if( func == spec_breath_gas )
      return "spec_breath_gas";
   if( func == spec_breath_lightning )
      return "spec_breath_lightning";
   if( func == spec_cast_adept )
      return "spec_cast_adept";
   if( func == spec_cast_cleric )
      return "spec_cast_cleric";
   if( func == spec_cast_judge )
      return "spec_cast_judge";
   if( func == spec_cast_mage )
      return "spec_cast_mage";
   if( func == spec_cast_undead )
      return "spec_cast_undead";
   if( func == spec_executioner )
      return "spec_executioner";
   if( func == spec_fido )
      return "spec_fido";
   if( func == spec_guard )
      return "spec_policeman";
   if( func == spec_janitor )
      return "spec_janitor";
   if( func == spec_mayor )
      return "spec_mayor";
   if( func == spec_poison )
      return "spec_poison";
   if( func == spec_thief )
      return "spec_thief";
   if( func == spec_policeman )
      return "spec_policeman";
   if( func == spec_cast_cadaver )
      return "spec_cast_cadaver";
   if( func == spec_undead )
      return "spec_undead";
   if( func == spec_stephen )
      return "spec_stephen";
   if( func == spec_rewield )
      return "spec_rewield";
   if( func == spec_sylai_priest )
      return "spec_sylai_priest";
   if( func == spec_cast_bigtime )
      return "spec_cast_bigtime";
   if( func == spec_sage )
      return "spec_sage";
   if( func == spec_wizardofoz )
      return "spec_wizardofoz";
   if( func == spec_vamp_hunter )
      return "spec_vamp_hunter";
   if( func == spec_mino_guard )
      return "spec_mino_guard";
   if( func == spec_tax_man )
      return "spec_tax_man";

   return 0;
}

void print_spec_lookup( char *buf )
{
   strcat( buf, "       spec_breath_any         \n\r" );
   strcat( buf, "       spec_breath_acid        \n\r" );
   strcat( buf, "       spec_breath_fire        \n\r" );
   strcat( buf, "       spec_breath_frost       \n\r" );
   strcat( buf, "       spec_breath_gas         \n\r" );
   strcat( buf, "       spec_breath_lightning   \n\r" );
   strcat( buf, "       spec_cast_adept         \n\r" );
   strcat( buf, "       spec_cast_cleric        \n\r" );
   strcat( buf, "       spec_cast_judge         \n\r" );
   strcat( buf, "       spec_cast_mage          \n\r" );
   strcat( buf, "       spec_cast_undead        \n\r" );
   strcat( buf, "       spec_executioner        \n\r" );
   strcat( buf, "       spec_fido               \n\r" );
   strcat( buf, "       spec_guard              \n\r" );
   strcat( buf, "       spec_janitor            \n\r" );
   strcat( buf, "       spec_mayor              \n\r" );
   strcat( buf, "       spec_poison             \n\r" );
   strcat( buf, "       spec_thief              \n\r" );
   strcat( buf, "       spec_policeman          \n\r" );
   strcat( buf, "       spec_cast_cadaver       \n\r" );
   strcat( buf, "       spec_undead             \n\r" );
   strcat( buf, "       spec_rewield            \n\r" );
   strcat( buf, "	     spec_cast_bigtime       \n\r" );
   strcat( buf, "       spec_wizardofoz         \n\r" );
   strcat( buf, "       spec_vamp_hunter (Int mobs only) \n\r" );
   strcat( buf, "       spec_mino_guard \n\r" );
   strcat( buf, "       spec_tax_man \n\r" );

   return;
}




/*
 * Core procedure for dragons.
 */
bool dragon( CHAR_DATA * ch, char *spell_name )
{
   CHAR_DATA *victim;
   int sn;

   if( ch->position != POS_FIGHTING )
      return FALSE;

   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
      if( victim->fighting == ch && number_bits( 2 ) == 0 )
         break;

   if( victim == NULL )
      return FALSE;

   if( ( sn = skill_lookup( spell_name ) ) < 0 )
      return FALSE;
   ( *skill_table[sn].spell_fun ) ( sn, ch->level, ch, victim, NULL );
   return TRUE;
}



/*
 * Special procedures for mobiles.
 */
bool spec_breath_any( CHAR_DATA * ch )
{
   if( ch->position != POS_FIGHTING )
      return FALSE;

   switch ( number_bits( 3 ) )
   {
      case 0:
         return spec_breath_fire( ch );
      case 1:
      case 2:
         return spec_breath_lightning( ch );
      case 3:
         return spec_breath_gas( ch );
      case 4:
         return spec_breath_acid( ch );
      case 5:
      case 6:
      case 7:
         return spec_breath_frost( ch );
   }

   return FALSE;
}



bool spec_breath_acid( CHAR_DATA * ch )
{
   return dragon( ch, "acid breath" );
}



bool spec_breath_fire( CHAR_DATA * ch )
{
   return dragon( ch, "fire breath" );
}



bool spec_breath_frost( CHAR_DATA * ch )
{
   return dragon( ch, "frost breath" );
}



bool spec_breath_gas( CHAR_DATA * ch )
{
   int sn;

   if( ch->position != POS_FIGHTING )
      return FALSE;

   if( ( sn = skill_lookup( "gas breath" ) ) < 0 )
      return FALSE;
   ( *skill_table[sn].spell_fun ) ( sn, ch->level, ch, NULL, NULL );
   return TRUE;
}



bool spec_breath_lightning( CHAR_DATA * ch )
{
   return dragon( ch, "lightning breath" );
}



bool spec_cast_adept( CHAR_DATA * ch )
{
   CHAR_DATA *victim;
   int cl;  /* cast level */

   if( !IS_AWAKE( ch ) )
      return FALSE;


   if( ch->position == POS_FIGHTING )
      return FALSE;


   if( ch->in_room != NULL )
   {
      if( !IS_SET( ch->in_room->affected_by, ROOM_BV_HEAL_REGEN ) )
         spell_healing_light( skill_lookup( "healing light" ), 79, ch, NULL, NULL );
      if( !IS_SET( ch->in_room->affected_by, ROOM_BV_MANA_REGEN ) )
         spell_mana_flare( skill_lookup( "mana flare" ), 79, ch, NULL, NULL );
   }

   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
      if( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0 )
         break;

   if( victim == NULL )
      return FALSE;



   cl = number_range( 1, ch->level );

   switch ( number_bits( 3 ) )
   {
      case 0:
         act( "$n utters the word 'tehctah'.", ch, NULL, NULL, TO_ROOM );
         spell_armor( skill_lookup( "armor" ), cl, ch, victim, NULL );
         return TRUE;

      case 1:
         act( "$n utters the word 'nhak'.", ch, NULL, NULL, TO_ROOM );
         spell_bless( skill_lookup( "bless" ), cl, ch, victim, NULL );
         return TRUE;

      case 2:
         act( "$n utters the word 'yeruf'.", ch, NULL, NULL, TO_ROOM );
         spell_cure_blindness( skill_lookup( "cure blindness" ), cl, ch, victim, NULL );
         return TRUE;

      case 3:
         act( "$n utters the word 'garf'.", ch, NULL, NULL, TO_ROOM );
         spell_cure_light( skill_lookup( "cure light" ), cl, ch, victim, NULL );
         return TRUE;

      case 4:
         act( "$n utters the words 'rozar'.", ch, NULL, NULL, TO_ROOM );
         spell_cure_poison( skill_lookup( "cure poison" ), cl, ch, victim, NULL );
         return TRUE;

      case 5:
      {
         time_t cur_time, xmas_time;
         struct tm tm_xmas;
         int days;
         char buffer[255];

         tm_xmas.tm_sec = 0;
         tm_xmas.tm_min = 0;
         tm_xmas.tm_hour = 0;
         tm_xmas.tm_mday = 25;
         tm_xmas.tm_mon = 11;
         tm_xmas.tm_year = 98;

         xmas_time = mktime( &tm_xmas );
         days = difftime( xmas_time, time( &cur_time ) ) / ( 3600 * 24 );

         sprintf( buffer, "$n utters the words '%i days to Christmas!'.", days );

         act( buffer, ch, NULL, NULL, TO_ROOM );
         spell_refresh( skill_lookup( "refresh" ), ch->level, ch, victim, NULL );
         return TRUE;
      }

   }

   return FALSE;
}



bool spec_cast_cleric( CHAR_DATA * ch )
{
   CHAR_DATA *victim;
   char *spell;
   int sn;

   if( ch->position != POS_FIGHTING )
      return FALSE;

   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
      if( victim->fighting == ch && number_bits( 2 ) == 0 )
         break;

   if( victim == NULL )
      return FALSE;

   for( ;; )
   {
      int min_level;

      switch ( number_bits( 4 ) )
      {
         case 0:
            min_level = 0;
            spell = "blindness";
            break;
         case 1:
            min_level = 3;
            spell = "cause serious";
            break;
         case 2:
            min_level = 7;
            spell = "earthquake";
            break;
         case 3:
            min_level = 9;
            spell = "cause critical";
            break;
         case 4:
            min_level = 10;
            spell = "dispel evil";
            break;
         case 5:
            min_level = 12;
            spell = "curse";
            break;
         case 6:
            min_level = 12;
            spell = "change sex";
            break;
         case 7:
            min_level = 13;
            spell = "flamestrike";
            break;
         case 8:
         case 9:
         case 10:
            min_level = 15;
            spell = "harm";
            break;
         default:
            min_level = 16;
            spell = "dispel magic";
            break;
      }

      if( ch->level >= min_level )
         break;
      /*
       * Can add infinite loop checking here, but its kinda pointless,
       * * as no mob can have ch->level < 0 
       */
   }

   if( ( sn = skill_lookup( spell ) ) < 0 )
      return FALSE;

   act( "The eyes of $n glow brightly!", ch, NULL, NULL, TO_ROOM );
   ( *skill_table[sn].spell_fun ) ( sn, ch->level, ch, victim, NULL );
   return TRUE;
}



bool spec_cast_judge( CHAR_DATA * ch )
{
   CHAR_DATA *victim;
   char *spell;
   int sn;

   if( ch->position != POS_FIGHTING )
      return FALSE;

   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
      if( victim->fighting == ch && number_bits( 2 ) == 0 )
         break;

   if( victim == NULL )
      return FALSE;

   spell = "high explosive";
   if( ( sn = skill_lookup( spell ) ) < 0 )
      return FALSE;
   ( *skill_table[sn].spell_fun ) ( sn, ch->level, ch, victim, NULL );
   return TRUE;
}



bool spec_cast_mage( CHAR_DATA * ch )
{
   CHAR_DATA *victim;
   char *spell;
   int sn;

   if( ch->position != POS_FIGHTING )
      return FALSE;

   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
      if( victim->fighting == ch && number_bits( 2 ) == 0 )
         break;

   if( victim == NULL )
      return FALSE;

   for( ;; )
   {
      int min_level;

      switch ( number_bits( 4 ) )
      {
         case 0:
            min_level = 0;
            spell = "blindness";
            break;
         case 1:
            min_level = 3;
            spell = "chill touch";
            break;
         case 2:
            min_level = 7;
            spell = "weaken";
            break;
         case 3:
            min_level = 8;
            spell = "teleport";
            break;
         case 4:
            min_level = 11;
            spell = "color spray";
            break;
         case 5:
            min_level = 12;
            spell = "change sex";
            break;
         case 6:
            min_level = 13;
            spell = "energy drain";
            break;
         case 7:
         case 8:
         case 9:
            min_level = 15;
            spell = "fireball";
            break;
         default:
            min_level = 20;
            spell = "acid blast";
            break;
      }

      if( ch->level >= min_level )
         break;
      /*
       * Can add infinite loop checking here, but its kinda pointless,
       * * as no mob can have ch->level < 0.  -- Alty 
       */
   }

   if( ( sn = skill_lookup( spell ) ) < 0 )
      return FALSE;

   act( "An eerie sound comes from $n as $e stares at $N!!", ch, NULL, victim, TO_ROOM );
   ( *skill_table[sn].spell_fun ) ( sn, ch->level, ch, victim, NULL );
   return TRUE;
}



bool spec_cast_undead( CHAR_DATA * ch )
{
   CHAR_DATA *victim;
   char *spell;
   int sn;

   if( ch->position != POS_FIGHTING )
      return FALSE;

   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
      if( victim->fighting == ch && number_bits( 2 ) == 0 )
         break;

   if( victim == NULL )
      return FALSE;

   for( ;; )
   {
      int min_level;

      switch ( number_bits( 4 ) )
      {
         case 0:
            min_level = 0;
            spell = "curse";
            break;
         case 1:
            min_level = 3;
            spell = "weaken";
            break;
         case 2:
            min_level = 6;
            spell = "chill touch";
            break;
         case 3:
            min_level = 9;
            spell = "blindness";
            break;
         case 4:
            min_level = 12;
            spell = "poison";
            break;
         case 5:
            min_level = 15;
            spell = "energy drain";
            break;
         case 6:
            min_level = 18;
            spell = "harm";
            break;
         case 7:
            min_level = 21;
            spell = "teleport";
            break;
         default:
            min_level = 24;
            spell = "gate";
            break;
      }

      if( ch->level >= min_level )
         break;
      /*
       * Can add infinite loop checking here, but its kinda pointless,
       * * as no mob can have ch->level < 0. -- Alty 
       */
   }

   if( ( sn = skill_lookup( spell ) ) < 0 )
      return FALSE;
   act( "$n summons forth the powers of darkness!", ch, NULL, NULL, TO_ROOM );
   ( *skill_table[sn].spell_fun ) ( sn, ch->level, ch, victim, NULL );
   return TRUE;
}



bool spec_executioner( CHAR_DATA * ch )
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   char *crime;
   bool undead;

   undead = FALSE;

   if( !IS_AWAKE( ch ) || ch->fighting != NULL )
      return FALSE;

   crime = "";
   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
   {
      if( IS_VAMP( victim ) && IS_AFFECTED( victim, AFF_VAMP_HEALING ) )
         continue;


      if( !IS_NPC( victim ) && IS_SET( victim->act, PLR_KILLER ) )
      {
         crime = "KILLER";
         break;
      }

      if( !IS_NPC( victim ) && IS_SET( victim->act, PLR_THIEF ) )
      {
         crime = "THIEF";
         break;
      }
      if( IS_NPC( victim ) && IS_SET( victim->act, ACT_UNDEAD ) )
      {
         crime = "UNDEAD";
         undead = TRUE;
         break;
      }

   }

   if( victim == NULL )
      return FALSE;

   if( undead )
      sprintf( buf, "BANZAI!!! UNDEAD HAVE ARRIVED!!! PROTECT THE LIVING!!!" );
   else
      sprintf( buf, "%s is a %s!  PROTECT THE INNOCENT!  MORE BLOOOOD!!!", victim->name, crime );

   do_yell( ch, buf );
   multi_hit( ch, victim, TYPE_UNDEFINED );
   char_to_room( create_mobile( get_mob_index( MOB_VNUM_CITYGUARD ) ), ch->in_room );
   char_to_room( create_mobile( get_mob_index( MOB_VNUM_CITYGUARD ) ), ch->in_room );
   return TRUE;
}

bool spec_mino_guard( CHAR_DATA * ch )
{

   CHAR_DATA *victim;
   OBJ_DATA *pass;

   if( !IS_AWAKE( ch ) || ch->fighting != NULL )
      return FALSE;

   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
   {
      if( IS_VAMP( victim ) && IS_AFFECTED( victim, AFF_VAMP_HEALING ) )
         continue;
      if( ( IS_NPC( victim ) ) || ( ch == victim ) || ( IS_IMMORTAL( victim ) ) )
      {
         continue;
      }
      else
         if( ( ( ( ( pass = get_eq_char( victim, WEAR_HOLD_HAND_R ) ) != NULL )
                 && ( pass->pIndexData->vnum == OBJ_VNUM_MINO_PASS ) ) )
             || ( ( ( pass = get_eq_char( victim, WEAR_HOLD_HAND_L ) ) != NULL )
                  && ( pass->pIndexData->vnum == OBJ_VNUM_MINO_PASS ) ) )
      {
         do_say( ch, "@@eLet this one pass@@N" );
         continue;
      }

      break;
   }

   if( ( victim == NULL ) || ( ch == victim ) )
      return FALSE;


   do_yell( ch, "Intruder Alert! Man the Walls!" );
   multi_hit( ch, victim, TYPE_UNDEFINED );
   return TRUE;
}


bool spec_fido( CHAR_DATA * ch )
{
   OBJ_DATA *corpse;
   OBJ_DATA *obj;
   OBJ_DATA *obj_next;

   if( !IS_AWAKE( ch ) )
      return FALSE;

   for( corpse = ch->in_room->first_content; corpse != NULL; corpse = corpse->next_in_room )
   {
      if( corpse->item_type != ITEM_CORPSE_NPC || number_bits( 1 ) != 0 )
         continue;

      act( "$n savagely devours a corpse.", ch, NULL, NULL, TO_ROOM );
      for( obj = corpse->first_in_carry_list; obj; obj = obj_next )
      {
         obj_next = obj->next_in_carry_list;
         obj_from_obj( obj );
         obj_to_room( obj, ch->in_room );
      }
      extract_obj( corpse );
      return TRUE;
   }

   return FALSE;
}



bool spec_guard( CHAR_DATA * ch )
{
   return spec_policeman( ch );
}



bool spec_janitor( CHAR_DATA * ch )
{
   OBJ_DATA *trash;

   if( !IS_AWAKE( ch ) )
      return FALSE;

   for( trash = ch->in_room->first_content; trash != NULL; trash = trash->next_in_room )
   {
      if( !IS_SET( trash->wear_flags, ITEM_TAKE ) )
         continue;
      if( trash->item_type == ITEM_DRINK_CON || trash->item_type == ITEM_TRASH || trash->cost < 10 )
      {
         act( "$n picks up some trash.", ch, NULL, NULL, TO_ROOM );
         obj_from_room( trash );
         obj_to_char( trash, ch );
         return TRUE;
      }
   }

   return FALSE;
}



bool spec_mayor( CHAR_DATA * ch )
{
   static const char open_path[] = "W3a3003b33000c111d0d111Oe333333Oe22c222112212111a1S.";

   static const char close_path[] = "W3a3003b33000c111d0d111CE333333CE22c222112212111a1S.";

   static const char *path;
   static int pos;
   static bool move;

   if( !move )
   {
      if( time_info.hour == 6 )
      {
         path = open_path;
         move = TRUE;
         pos = 0;
      }

      if( time_info.hour == 20 )
      {
         path = close_path;
         move = TRUE;
         pos = 0;
      }
   }

   if( ch->fighting != NULL )
      return spec_cast_cleric( ch );
   if( !move || ch->position < POS_SLEEPING )
      return FALSE;

   switch ( path[pos] )
   {
      case '0':
      case '1':
      case '2':
      case '3':
         move_char( ch, path[pos] - '0' );
         break;

      case 'W':
         ch->position = POS_STANDING;
         act( "$n awakens and groans loudly.", ch, NULL, NULL, TO_ROOM );
         break;

      case 'S':
         ch->position = POS_SLEEPING;
         act( "$n lies down and falls asleep.", ch, NULL, NULL, TO_ROOM );
         break;

      case 'a':
         act( "$n says 'Hello Honey!'", ch, NULL, NULL, TO_ROOM );
         break;

      case 'b':
         act( "$n says 'What a view!  I must do something about that dump!'", ch, NULL, NULL, TO_ROOM );
         break;

      case 'c':
         act( "$n says 'Vandals!  Youngsters have no respect for anything!'", ch, NULL, NULL, TO_ROOM );
         break;

      case 'd':
         act( "$n says 'Good day, citizens!'", ch, NULL, NULL, TO_ROOM );
         break;

      case 'e':
/*	do_yell( ch, "I hereby declare the city of Midgaard open!" ); */
         do_yell( ch, "It's the Groovy Guru of ACK! ZenDude!" );
         break;

      case 'E':
/*	do_yell( ch, "I hereby declare the city of Midgaard closed!" );*/
         do_yell( ch, "See my wiggle? My twist? My FORBIDDEN DANCE?" );
         break;

      case 'O':
         do_unlock( ch, "gate" );
         do_open( ch, "gate" );
         break;

      case 'C':
         do_close( ch, "gate" );
         do_lock( ch, "gate" );
         break;

      case '.':
         move = FALSE;
         break;
   }

   pos++;
   return FALSE;
}



bool spec_poison( CHAR_DATA * ch )
{
   CHAR_DATA *victim;

   if( ch->position != POS_FIGHTING || ( victim = ch->fighting ) == NULL || number_percent(  ) > 2 * ch->level )
      return FALSE;

   act( "You bite $N!", ch, NULL, victim, TO_CHAR );
   act( "$n bites $N!", ch, NULL, victim, TO_NOTVICT );
   act( "$n bites you!", ch, NULL, victim, TO_VICT );
   spell_poison( gsn_poison, ch->level, ch, victim, NULL );
   return TRUE;
}



bool spec_thief( CHAR_DATA * ch )
{
   CHAR_DATA *victim;
   int gold;

   if( ch->position != POS_STANDING )
      return FALSE;

   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
   {
      if( IS_NPC( victim ) || victim->level >= LEVEL_IMMORTAL || number_bits( 2 ) != 0 || !can_see( ch, victim ) )   /* Thx Glop */
         continue;

      if( IS_AWAKE( victim ) && number_range( 0, ch->level ) == 0 )
      {
         act( "You discover $n's hands in your wallet!", ch, NULL, victim, TO_VICT );
         act( "$N discovers $n's hands in $S wallet!", ch, NULL, victim, TO_NOTVICT );
         return TRUE;
      }
      else
      {
         gold = victim->gold * number_range( 1, 20 ) / 80;
         ch->gold += 7 * gold / 8;
         victim->gold -= gold;
         return TRUE;
      }
   }

   return FALSE;
}

bool spec_policeman( CHAR_DATA * ch )
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   char *crime;

   if( !IS_AWAKE( ch ) || ch->fighting != NULL )
      return FALSE;

   crime = "";
   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
   {
      if( IS_VAMP( victim ) && IS_AFFECTED( victim, AFF_VAMP_HEALING ) )
         continue;

      if( !IS_NPC( victim ) && IS_SET( victim->act, PLR_KILLER ) )
      {
         crime = "KILLER";
         break;
      }

      if( !IS_NPC( victim ) && IS_SET( victim->act, PLR_THIEF ) )
      {
         crime = "THIEF";
         break;
      }
   }

   if( victim == NULL )
      return FALSE;

   if( ch->hunting || ch->hunt_obj )
      /*
       * if ( (int) (ch->hunting) > 0)
       *//*
       * Don't hunt someone if already hunting 
       */
      return FALSE;


   sprintf( buf, "%s is a %s, I shall not rest till justice is done!", victim->name, crime );
   do_yell( ch, buf );

   set_hunt( ch, NULL, victim, NULL, 0, 0 );

   return TRUE;
}


bool spec_cast_cadaver( CHAR_DATA * ch )
{
   /*
    * Quick rip-off of spec_cast_adept, to make cadaver cast detect invis
    * * with short duration. -S-
    */

   CHAR_DATA *victim;
/*    CHAR_DATA *vch;    */


   if( !IS_AWAKE( ch ) )
      return FALSE;

   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
      if( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0 )
         break;

   if( victim == NULL )
      return FALSE;

   /*
    * for ( vch = ch->in_room->first_person; vch != NULL; vch = vch->next )
    * {
    * OBJ_DATA *obj;
    * sh_int    count = 0;
    * bool    match = FALSE;
    * 
    * 
    * if ( IS_NPC( vch ) )
    * continue;
    * 
    * 
    * for ( obj = ch->in_room->first_content; obj != NULL; obj = obj->in_room->first_content->next ) 
    * {
    * if ( obj->item_type != ITEM_CORPSE_PC )
    * continue;
    * count++;
    * if (  ( !str_cmp( vch->name, obj->owner ) )
    * && ( obj->first_content )    )
    * {
    * match = TRUE;
    * break;
    * }
    * }
    * if ( match )
    * {
    * char   buf[MAX_STRING_LENGTH];
    * 
    * sprintf( buf, "Try 'get all %d.corpse' to recover your belongings, %s.\n\r",
    * count, PERS( ch, vch ) );
    * do_say( ch, buf );
    * }
    * }
    */

   switch ( number_bits( 3 ) )
   {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
         if( !IS_AFFECTED( victim, AFF_DETECT_INVIS ) )
         {
            act( "$n utters the word 'Sight'.", ch, NULL, NULL, TO_ROOM );
            spell_detect_invis( skill_lookup( "detect invis" ), 0, ch, victim, NULL );
            return TRUE;
         }
   }
   return FALSE;
}

bool spec_undead( CHAR_DATA * ch )
{
   CHAR_DATA *victim;
   CHAR_DATA *ach;
   char *spell;
   int sn, sum_lev;

   if( ch->position != POS_FIGHTING )
      return FALSE;


   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
      if( victim->fighting == ch && number_bits( 2 ) != 0 )
         break;

   if( victim == NULL )
      return FALSE;



   for( ;; )
   {
      int min_level;

      switch ( number_bits( 4 ) )
      {
         case 0:
            min_level = 0;
            spell = "blindness";
            break;
         case 1:
            min_level = 3;
            spell = "chill touch";
            break;
         case 2:
            min_level = 7;
            spell = "weaken";
            break;
         case 3:
            min_level = 30;
            spell = "earthquake";
            break;
         case 4:
            min_level = 60;
            spell = "dispel magic";
            break;
         case 5:
            min_level = 12;
            spell = "curse";
            break;
         case 6:
            min_level = 20;
            spell = "energy drain";
            break;
         case 7:
            min_level = 40;
            spell = "gate";
            break;
         case 8:
            min_level = 30;
            spell = "fireball";
            break;
         default:
            min_level = 1;
            spell = "summon";
            break;
      }

      if( ch->level >= min_level )
         break;
      /*
       * Can put infinite loop checking here if you want, but its pointless
       * * since nothing can have a ch->level < 0. -- Alty 
       */
   }

   if( ( sn = skill_lookup( spell ) ) < 0 )
      return FALSE;

   if( !str_cmp( spell, "summon" ) )   /* CHECK FOR NPC!!!!!!!!!!!!! */
   {
      sum_lev = ch->level * 2 / 3;
      for( ach = first_char; ach != NULL; ach = ach->next )
      {
         if( !IS_SET( ach->act, ACT_UNDEAD ) || ach->level > sum_lev || !IS_NPC( ach ) /* Kavir got summoned!  :P */
             || ach->in_room == ch->in_room || !can_see( ch, ach ) || number_bits( 2 ) != 0 )
            continue;

         victim = ach;
         break;
      }
      if( ach == NULL )
         return FALSE;

      act( "$n's eyes turn black for an instant!", ch, NULL, NULL, TO_ROOM );
      act( "$n disappears suddenly.", victim, NULL, NULL, TO_ROOM );
      char_from_room( victim );
      char_to_room( victim, ch->in_room );
      act( "$n arrives suddenly.", victim, NULL, NULL, TO_ROOM );
      multi_hit( victim, ch->fighting, TYPE_UNDEFINED );
   }
   else
   {
      ( *skill_table[sn].spell_fun ) ( sn, ch->level, ch, victim, NULL );
   }

   return TRUE;
}

bool spec_stephen( CHAR_DATA * ch )
{
   /*
    * Award winner for daftest spec_fun of the week contest.
    * * mobs hops around, if finds PC, kisses them and gives
    * * a cure light.  -S-
    */

   ROOM_INDEX_DATA *room;
   CHAR_DATA *victim;
   int count;
   int vic_cnt;
//    int sn;



/*    if ( ch->position == POS_FIGHTING )
       return FALSE;*/

   /*
    * Uhh.. sleeping mobs and the like shouldn't either. -- Alty 
    */
   if( ch->position < POS_STANDING )
      return FALSE;


/*    for ( ; ; )
    {
       room = get_room_index( number_range( 0, 65535 ) );
       if ( room == NULL )
	  continue;*/
   /*
    * No checking for private rooms, etc. :P 
    */
/*       break;
    }*/

   /*
    * Check this loop.. -- Alty 
    */
   for( room = get_room_index( number_range( 0, 65535 ) ); !room; room = get_room_index( number_range( 0, 65535 ) ) )
      ;


   act( "$n climbs up the chimney.", ch, NULL, NULL, TO_ROOM );
   char_from_room( ch );

   char_to_room( ch, room );
   act( "$n emerges from the fireplace, carrying a huge sack of gifts!", ch, NULL, NULL, TO_ROOM );

   count = 0;
   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
      if( !IS_NPC( victim ) )
         count++;

   if( count == 0 )
      return FALSE;

   vic_cnt = number_range( 1, count );

   count = 0;
   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
   {
      if( !IS_NPC( victim ) && ( ++count ) == vic_cnt )
         break;
   }

   if( victim == NULL )
      return FALSE;  /* just in case :) */

   act( "$n says 'Ho Ho Ho, Merry Christmas, $N!", ch, NULL, victim, TO_ROOM );

   act( "$n smiles at $N, and hands $M a present!.", ch, NULL, victim, TO_NOTVICT );
   act( "$N smiles at you, and hands you a present!.", victim, NULL, ch, TO_CHAR );


   return TRUE;
}

bool spec_rewield( CHAR_DATA * ch )
{
   /*
    * If mob is fighting, try and get weapon and wield it 
    * * if not fighting, check room and inv to see if any better weapons
    * * -Stephen
    */

   OBJ_DATA *obj;
   OBJ_DATA *weapon = NULL;
   int dam;
   int chance;
   bool pickup;
   char buf[MAX_STRING_LENGTH];


   pickup = TRUE;
   dam = 0;

   chance = ( ch->fighting == NULL ? 35 : 60 );

   if( number_percent(  ) < chance )
   {
      for( obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list )
         if( obj->item_type == ITEM_WEAPON && dam < obj->value[2] )
         {
            dam = obj->value[2];
            pickup = FALSE;
            weapon = obj;
         }


      /*
       * Then check inventory and room for any weapons 
       */
      for( obj = ch->in_room->first_content; obj != NULL; obj = obj->next_in_room )
      {
         if( obj->item_type == ITEM_WEAPON )
         {
            if( obj->value[2] > dam )
            {
               dam = obj->value[2];
               weapon = obj;
               pickup = TRUE;
            }
         }
      }



      if( weapon == NULL )
         return FALSE;

      if( weapon->wear_loc == WEAR_HOLD_HAND_L )
         return FALSE;


      if( pickup )
      {
         sprintf( buf, "Great!  %s!  Just what i've always wanted!", weapon->short_descr );
         do_say( ch, buf );
      }

      if( weapon != NULL )
      {
         /*
          * Now make the mob get the weapon 
          */
         if( pickup )
            get_obj( ch, weapon, NULL );

         do_wear( ch, weapon->name );

         /*
          * Check is mob wielded weapon ok... 
          */
         if( weapon->wear_loc == WEAR_NONE )
         {
            act( "$n sniffs sadly.  'Baah!  It's no good to me!'", ch, NULL, NULL, TO_ROOM );
            extract_obj( weapon );
            act( "$n sacrifices $p.", ch, weapon, NULL, TO_ROOM );
         }



         return TRUE;
      }

   }



   return FALSE;
}


bool spec_sylai_priest( CHAR_DATA * ch )
{
   /*
    * For sylai priest only.  Pushes player from overhang into pit below.
    * * -Players are gonna love this.  :)
    * * -S-
    */

   CHAR_DATA *victim;
   ROOM_INDEX_DATA *location;

   if( ch->fighting == NULL )
      return FALSE;

   if( ch->in_room->vnum != 439 )
      return FALSE;

   if( number_percent(  ) > 30 )
      return FALSE;

   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
   {
      if( victim->fighting == ch && number_bits( 2 ) == 0 )
         break;
   }

   if( victim == NULL )
      return FALSE;


   if( number_percent(  ) > 60 )
   {
      act( "$n tries to knock $N off the outcrop!", ch, NULL, victim, TO_NOTVICT );
      act( "$N tries to knock you off the outcrop!", victim, NULL, ch, TO_CHAR );
      return FALSE;
   }

   if( number_percent(  ) > 70 )
      location = get_room_index( 436 );
   else
      location = get_room_index( 433 );

   if( location == NULL )
   {
      notify( "Fucked up spec_sylai_priest.\n\r", 82 );
      return FALSE;
   }

   act( "$n rushes towards $N and shoves $M off the outcrop!", ch, NULL, victim, TO_NOTVICT );
   act( "$N rushes at you, and shoves you off the cliff!", victim, NULL, ch, TO_CHAR );
   stop_fighting( ch, TRUE );
   char_from_room( victim );


   char_to_room( victim, location );
   act( "$n lands on the floor, having fallen from the outcrop above!", victim, NULL, NULL, TO_NOTVICT );
   send_to_char( "You crash to the floor below!\n\r", victim );
   do_look( victim, "" );
   return TRUE;
}


bool spec_cast_bigtime( CHAR_DATA * ch )
{
   CHAR_DATA *victim;
   char *spell;
   int sn;
   int crun;

   if( ch->position != POS_FIGHTING )
      return FALSE;

   for( victim = ch->in_room->first_person; victim; victim = victim->next_in_room )
      if( victim->fighting == ch && number_bits( 2 ) == 0 )
         break;

   if( victim == NULL )
      return FALSE;

   for( crun = 0;; crun++ )
   {
      int min_level;

      switch ( number_bits( 4 ) )
      {
         case 0:
            min_level = 45;
            spell = "dispel magic";
            break;
         case 1:
            min_level = 46;
            spell = "acid blast";
            break;
         case 2:
            min_level = 30;
            spell = "phobia";
            break;
         case 3:
            min_level = 55;
            spell = "chain lightning";
            break;
         case 4:
            min_level = 65;
            spell = "energy drain";
            break;
         case 5:
            min_level = 43;
            spell = "mindflame";
            break;
         case 6:
            min_level = 12;
            spell = "lava burst";
            break;
         case 7:
            min_level = 44;
            spell = "static";
            break;
         default:
            min_level = 25;
            spell = "hellspawn";
            break;
      }

      if( ch->level >= min_level )
         break;
      /*
       * Stop infinite recursion on low level mobs.. (ch->level < 12)
       * * -- Alty 
       */
      if( crun > 10 )
         return FALSE;
   }

   if( ( sn = skill_lookup( spell ) ) < 0 )
      return FALSE;
   act( "$n invokes the dark powers!!", ch, NULL, NULL, TO_ROOM );
   ( *skill_table[sn].spell_fun ) ( sn, ch->level, ch, victim, NULL );
   return TRUE;
}



bool spec_sage( CHAR_DATA * ch )
{

   /*
    * Give the Sage some life.  He should chat with player, heal too, etc 
    */

   /*
    * What the options are: 
    * * W - Wake;   H - Greet;       C - make some food
    * * 1 - First Part of Story;      2-Second Part;    3-3rd part;
    * * 4- 4th part    5- 5th part;
    * * 6- 6th part    7-7th part;       8-8th part;
    * * F - rake the fire I - cast influx;
    * * G - Gossip     S - sleep.
    * * D - Do nothing.          X - Smile.    Y - Sigh.
    * * B - say byebye
    * * U - get food V - Eat food
    */


   static const char open_path[] = "WDDDHDFDCUVD1XDD2YDD3DD4DFD5DGDD6D7XDD7YDD8DFD9XDDDBDFD.";


   static const char *path;
   static int pos;
   static bool move;

   if( !move )
   {
      if( time_info.hour == 6 )
      {
         path = open_path;
         move = TRUE;
         pos = 0;
      }
   }

   if( ch->fighting != NULL )
      return spec_cast_cleric( ch );
   if( !move || ch->position < POS_SLEEPING )
      return FALSE;

   switch ( path[pos] )
   {
      case 'D':
         /*
          * Do Nothing. 
          */
         break;
      case 'W':
         ch->position = POS_RESTING;
         act( "$n awakens and groans loudly.", ch, NULL, NULL, TO_ROOM );
         break;

      case 'S':
         ch->position = POS_SLEEPING;
         act( "$n lies down and falls asleep.", ch, NULL, NULL, TO_ROOM );
         break;

      case 'H':
         do_say( ch, "Hello There!  Welcome to my humble cave." );
         break;
      case 'F':
         act( "$n leans forward, and rakes the fire.", ch, NULL, NULL, TO_ROOM );
         break;
      case 'C':
         do_say( ch, "You must be hungry.  Share some food with me." );
         spell_produce_food( skill_lookup( "produce food" ), 1, ch, NULL, NULL );
         spell_produce_food( skill_lookup( "produce food" ), 1, ch, NULL, NULL );
         break;
      case 'U':
         do_get( ch, "bundle" );
         break;
      case 'V':
         do_eat( ch, "bundle" );
         break;
      case 'X':
         act( "$n smiles sadly, remembering better days.", ch, NULL, NULL, TO_ROOM );
         break;
      case 'Y':
         act( "$n sighes sadly.", ch, NULL, NULL, TO_ROOM );
         break;
      case '1':
         do_say( ch, "Rest yourselves, and listen to what i have to tell you about the Sylai beasts." );
         break;
      case '2':
         do_say( ch, "Many decades ago, my race lived in these tunnels, living in peace." );
         do_say( ch, "Then, one day the Sylai came.  No one knows where they originated from." );
         do_say( ch, "But when they came, they brought the Dark Powers with them." );
         break;
      case '3':
         do_say( ch, "My race did not believe in killing.  We tried to reason with the Sylai." );
         do_say( ch, "but they retaliated with violence, killing us one by one." );
         break;
      case '4':
         do_say( ch, "The Sylai had no mercy.  They killed to satisfy the Dark Powers." );
         do_say( ch, "They enjoyed killing us.  My kind were enslaved to the Sylai." );
         do_say( ch, "They used us for their evil experiments, but we started to fight back." );
         break;
      case '5':
         do_say( ch, "But we were weak.  The Dark Powers within the Sylai was too much." );
         do_say( ch, "Slowly, they butchered my kind.  Some of us fled, and hid here." );
         break;
      case '6':
         do_say( ch, "There were originally five.  Now there is just me." );
         do_say( ch, "I grow weak now - i know i don't have long left." );
         break;
      case '7':
         do_say( ch, "I just wish that we had struck back when stronger in number." );
         do_say( ch, "But now there is just me.  And what can one old fool do?" );
         break;
      case '8':
         do_say( ch, "If you have come this far, you must be strong." );
         do_say( ch, "Seek revenge for my race.  Search out their leader, Sylar." );
         do_say( ch, "And wipe him from this plane of existence." );
         break;
      case '9':
         do_say( ch, "Sylar stole my race's greatest power.  Our artifacts." );
         do_say( ch, "Without those, we were powerless.  But they would help you greatly." );
         do_say( ch, "Take them, and remember my race." );
         break;
      case 'G':
         do_say( ch, "I remember when old Caryl was still with me.  Oh, the fun we had." );
         do_say( ch, "But that was years ago.  And now she's gone, like all the rest." );
         break;
      case 'B':
         do_say( ch, "I grow tired.  It is time for me to sleep." );
         do_say( ch, "Go now, and strike back at Sylar" );
         act( "$n waves goodbye to you.", ch, NULL, NULL, TO_ROOM );
         break;
      case '.':
         act( "$n lies down slowly, and falls into a troubled sleep.", ch, NULL, NULL, TO_ROOM );
         ch->position = POS_SLEEPING;
         move = FALSE;
         break;
   }

   pos++;
   return FALSE;
}

bool spec_wizardofoz( CHAR_DATA * ch )
{
   /*
    * This disabled - intel mobs dropped 
    */
   return FALSE;
}


bool spec_vamp_hunter( CHAR_DATA * ch )
{

   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];

   if( ch->hunting == NULL )
      return FALSE;

   if( ( !IS_VAMP( ch->hunting ) ) || ( IS_NPC( ch->hunting ) ) )
      return FALSE;


   switch ( number_range( 1, 21 ) )
   {

      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
         break;
      case 6:
         sprintf( buf, "%s", ch->hunting->name );
         sprintf( buf1, "@@eI know that you are a Vampyre, and I shall not rest until your are destroyed!!!@@N\n\r" );
         safe_strcat( MAX_STRING_LENGTH, buf, buf1 );
         do_tell( ch, buf );
         break;
      case 7:
      case 8:
      case 9:
      case 10:
      case 11:
         break;
      case 12:
      case 13:
         sprintf( buf, "My informants have told me that %s is a Vampyre, and I have vowed to destroy him!!\n\r",
                  ch->hunting->name );
         do_yell( ch, buf );
         break;

      case 14:
      case 15:
      case 16:
         break;
      case 17:
         sprintf( buf, " %s ", ch->hunting->name );
         sprintf( buf1, "@@Do you finally know fear? I shall not rest until ALL of your kind are destroyed!!!@@N\n\r" );
         safe_strcat( MAX_STRING_LENGTH, buf, buf1 );
         do_tell( ch, buf );
         break;

      case 18:
      case 19:
         break;
      case 20:
         sprintf( buf,
                  "All the realm shall know that %s is a Vampyre, and I shall eridicate all of these vile creatures!!\n\r",
                  ch->hunting->name );
         do_yell( ch, buf );
         break;
      default:
         break;

   }

   return TRUE;

}





bool spec_tax_man( CHAR_DATA * ch )
{

   ROOM_INDEX_DATA *room;
   CHAR_DATA *victim;
   int count;
   int vic_cnt;
   int sn = 0;
   int remainder = 0;
   int loss = 0, bank_loss = 0, char_loss = 0, old_char = 0, old_bank = 0;
   char mon_buf[MSL];
   char cat_buf[MSL];



/*    if ( ch->position == POS_FIGHTING )
       return FALSE;*/

   /*
    * Uhh.. sleeping mobs and the like shouldn't either. -- Alty 
    */
   if( ch->position < POS_STANDING )
      return FALSE;


/*    for ( ; ; )
    {
       room = get_room_index( number_range( 0, 65535 ) );
       if ( room == NULL )
	  continue;*/
   /*
    * No checking for private rooms, etc. :P 
    */
/*       break;
    }*/

   /*
    * Check this loop.. -- Alty 
    */
   for( room = get_room_index( number_range( 0, 65535 ) ); !room; room = get_room_index( number_range( 0, 65535 ) ) )
      ;


   act( "$n suddenly vanishes!", ch, NULL, NULL, TO_ROOM );
   char_from_room( ch );

   char_to_room( ch, room );
   act( "$n suddenly appears in the room!", ch, NULL, NULL, TO_ROOM );

   count = 0;
   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
      if( !IS_NPC( victim ) )
         count++;

   if( count == 0 )
      return FALSE;

   vic_cnt = number_range( 1, count );

   count = 0;
   for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
   {
      if( !IS_NPC( victim ) && ( ++count ) == vic_cnt )
         break;
   }

   if( victim == NULL )
      return FALSE;  /* just in case :) */

   if( ( victim->gold < 100000 ) && ( victim->balance < 1000000 ) )
      return FALSE;

   act( "$n says 'Excuse me, $N, but our records state you haven't payed your taxes recently.", ch, NULL, victim, TO_ROOM );

   act( "$n says, 'Since you refuse to pay what you owe, it has automatically been deducted from your funds'.", ch, NULL,
        victim, TO_ROOM );
   act( "$n says, 'Have a nice day'.", ch, NULL, ch, TO_ROOM );

   sn = ( victim->balance + victim->gold ) / 100;
   old_char = victim->gold;
   old_bank = victim->balance;

   if( victim->balance > sn )
   {
      loss = victim->balance - sn;
      victim->balance = UMIN( victim->balance, loss );
      bank_loss = sn;
      char_loss = 0;
   }
   else
   {
      remainder = sn - victim->balance;
      bank_loss = victim->balance;
      char_loss = remainder;
      loss = victim->gold - remainder;
      victim->balance = 0;
      if( remainder > 0 )
         victim->gold = UMIN( victim->gold, loss );
   }
   sprintf( mon_buf, "Tax Collector visited %s ", victim->name );
   sprintf( cat_buf, "Collected %i from bank, and %i from gold on hand.\n\r", bank_loss, char_loss );
   safe_strcat( MSL, mon_buf, cat_buf );
   sprintf( cat_buf, "New totals are balance: %i/%i  on hand: %i/%i\n\r",
            victim->balance, old_bank, victim->gold, old_char );
   safe_strcat( MSL, mon_buf, cat_buf );
   monitor_chan( mon_buf, MONITOR_MOB );

   do_save( victim, "" );
   return TRUE;
}
