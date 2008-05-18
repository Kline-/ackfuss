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

DECLARE_OBJ_FUN( objfun_giggle );   /* test obj_fun   */
DECLARE_OBJ_FUN( objfun_cast_fight );  /* Casts in fights   */
DECLARE_OBJ_FUN( objfun_sword_aggro ); /* starts fights  */
DECLARE_OBJ_FUN( objfun_soul_moan );   /* moaning souls  */
DECLARE_OBJ_FUN( objfun_infused_soul );   /* objs with a soul in them */
DECLARE_OBJ_FUN( objfun_flaming );  /* test obj_fun   */
DECLARE_OBJ_FUN( objfun_healing );  /* test obj_fun   */
DECLARE_OBJ_FUN( objfun_dispeller );   /* test obj_fun   */
DECLARE_OBJ_FUN( objfun_regen ); /* test obj_fun   */


OBJ_FUN *obj_fun_lookup( const char *name )
{
   if( !str_cmp( name, "objfun_giggle" ) )
      return objfun_giggle;
   if( !str_cmp( name, "objfun_cast_fight" ) )
      return objfun_cast_fight;
   if( !str_cmp( name, "objfun_sword_aggro" ) )
      return objfun_sword_aggro;
   if( !str_cmp( name, "objfun_soul_moan" ) )
      return objfun_soul_moan;
   if( !str_cmp( name, "objfun_infused_soul" ) )
      return objfun_infused_soul;
   if( !str_cmp( name, "objfun_flaming" ) )
      return objfun_flaming;
   if( !str_cmp( name, "objfun_healing" ) )
      return objfun_healing;
   if( !str_cmp( name, "objfun_dispeller" ) )
      return objfun_dispeller;
   if( !str_cmp( name, "objfun_regen" ) )
      return objfun_regen;

   return 0;
}


char *rev_obj_fun_lookup( void *func )
{
   if( func == objfun_giggle )
      return "objfun_giggle";
   if( func == objfun_cast_fight )
      return "objfun_cast_fight";
   if( func == objfun_sword_aggro )
      return "objfun_sword_aggro";
   if( func == objfun_soul_moan )
      return "objfun_soul_moan";
   if( func == objfun_infused_soul )
      return "objfun_infused_soul";
   if( func == objfun_flaming )
      return "objfun_flaming";
   if( func == objfun_healing )
      return "objfun_healing";
   if( func == objfun_dispeller )
      return "objfun_dispeller";
   if( func == objfun_regen )
      return "objfun_regen";

   return 0;
}

void print_obj_fun_lookup( char *buf )
{
   strcat( buf, " objfun_giggle \n\r" );
   strcat( buf, " objfun_cast_fight	\n\r" );
   strcat( buf, " objfun_sword_aggro \n\r" );
   strcat( buf, " objfun_soul_moan \n\r  " );
   strcat( buf, " objfun_infused_soul \n\r  " );
   strcat( buf, " objfun_flaming \n\r" );
   strcat( buf, " objfun_healing \n\r  " );
   strcat( buf, " objfun_dispeller \n\r  " );
   strcat( buf, " objfun_regen \n\r  " );

   return;
}

/********************* OBJFUN FUNCTIONS ********************************/

void objfun_giggle( OBJ_DATA * obj, CHAR_DATA * keeper )
{
   if( keeper == NULL || keeper->in_room == NULL )
      return;

   /*
    * Come on... it was SO annoying! 
    */

   if( number_percent(  ) < 5 )
   {
      act( "$p carried by $n starts giggling to itself!", keeper, obj, NULL, TO_ROOM );
      act( "$p carried by you starts giggling to itself!", keeper, obj, NULL, TO_CHAR );
      return;
   }
   return;
}

void objfun_soul_moan( OBJ_DATA * obj, CHAR_DATA * keeper )
{
   if( keeper == NULL || keeper->in_room == NULL )
      return;

   /*
    * Come on... it was SO annoying! 
    */
   if( number_percent(  ) < 2 )
   {
      act( "@@NThe @@eSoul@@N in $p@@N carried by @@a$n@@N moans in agony.", keeper, obj, NULL, TO_ROOM );
      act( "@@NThe @@eSoul@@N in $p @@Ncarried by you moans to be set free!", keeper, obj, NULL, TO_CHAR );
      return;
   }

   if( number_percent(  ) < 2 )
   {
      act( "@@NThe @@eSoul@@N in $p@@N carried by @@a$n@@N tries to break free of it's inprisonment!", keeper, obj, NULL,
           TO_ROOM );
      act( "@@NThe @@eSoul@@N in $p @@Ncarried by you starts writhing--look out!!", keeper, obj, NULL, TO_CHAR );
      return;
   }

   return;
}

void objfun_infused_soul( OBJ_DATA * obj, CHAR_DATA * keeper )
{

   int sn;
   CHAR_DATA *victim;
   int min_lev;
   char *spell = "";

   if( keeper == NULL || keeper->in_room == NULL )
      return;
   if( ( keeper == NULL ) || ( obj->item_type != ITEM_WEAPON ) )
      return;

   if( ( get_eq_char( keeper, WEAR_HOLD_HAND_L ) != obj ) && ( get_eq_char( keeper, WEAR_HOLD_HAND_R ) != obj ) )
      return;  /* Must be wielded to work */

   if( keeper->fighting != NULL )
   {

      for( victim = keeper->in_room->first_person; victim != NULL; victim = victim->next_in_room )
         if( victim->fighting == keeper && number_bits( 1 ) == 0 )
            break;

      if( ( victim == NULL ) || ( victim->is_free != FALSE ) )
         return;

      switch ( number_range( 0, 3 ) )
      {
         case 0:
            min_lev = 43;
            spell = "throwing star";
            break;
         case 1:
            min_lev = 55;
            spell = "acid blast";
            break;
         case 2:
            min_lev = 68;
            spell = "dispel magic";
            break;
         case 3:
            min_lev = 55;
            spell = "flamestrike";
            break;

      }
      if( ( sn = skill_lookup( spell ) ) < 0 )
         return;
      act( "$p glows brightly at $n!", victim, obj, NULL, TO_ROOM );
      act( "$p glows brightly at you!", victim, obj, NULL, TO_CHAR );
      obj_cast_spell( sn, obj->level, keeper, victim, obj );
      return;
   }
   else
   {
      /*
       * Come on... it was SO annoying! 
       */
      if( number_percent(  ) < 2 )
      {
         act( "@@NThe @@eSoul@@N in $p@@N carried by @@a$n@@N moans in agony.", keeper, obj, NULL, TO_ROOM );
         act( "@@NThe @@eSoul@@N in $p @@Ncarried by you moans to be set free!", keeper, obj, NULL, TO_CHAR );
         return;
      }

      if( number_percent(  ) < 2 )
      {
         act( "@@NThe @@eSoul@@N in $p@@N carried by @@a$n@@N tries to break free of it's inprisonment!", keeper, obj, NULL,
              TO_ROOM );
         act( "@@NThe @@eSoul@@N in $p @@Ncarried by you starts writhing--look out!!", keeper, obj, NULL, TO_CHAR );
         return;
      }
   }
   return;
}

void objfun_cast_fight( OBJ_DATA * obj, CHAR_DATA * keeper )
{
   int sn;
   CHAR_DATA *victim;
   int min_lev;
   char *spell = "";
   if( keeper == NULL || ( keeper->fighting == NULL ) || ( obj->item_type != ITEM_WEAPON ) )
      return;
   if( ( get_eq_char( keeper, WEAR_HOLD_HAND_L ) != obj ) && ( get_eq_char( keeper, WEAR_HOLD_HAND_R ) != obj ) )
      return;  /* Must be wielded to work */

   for( victim = keeper->in_room->first_person; victim != NULL; victim = victim->next_in_room )
      if( victim->fighting == keeper && number_bits( 2 ) == 0 )
         break;

   if( ( victim == NULL ) || ( victim->is_free != FALSE ) )
      return;
   switch ( number_range( 0, 5 ) )
   {
      case 0:
         min_lev = 5;
         spell = "magic missile";
         break;
      case 1:
         min_lev = 7;
         spell = "color spray";
         break;
      case 2:
         min_lev = 8;
         spell = "chill touch";
         break;
      case 3:
         min_lev = 30;
         spell = "fireball";
         break;
      case 4:
         min_lev = 55;
         spell = "flamestrike";
         break;
      case 5:
         min_lev = 30;
         spell = "lightning bolt";
         break;
   }
   if( ( sn = skill_lookup( spell ) ) < 0 )
      return;
   act( "$p glows brightly at $n!", victim, obj, NULL, TO_ROOM );
   act( "$p glows brightly at you!", victim, obj, NULL, TO_CHAR );
   obj_cast_spell( sn, obj->level, keeper, victim, obj );
   return;
}

void objfun_sword_aggro( OBJ_DATA * obj, CHAR_DATA * keeper )
{
   /*
    * Weapon 'draws' an aggro mob's attention to the player 
    */
   /*
    * If fighting,  make cast spells? 
    */
   CHAR_DATA *vch;
   if( obj->item_type != ITEM_WEAPON )
      return;
   if( ( keeper == NULL ) || ( obj == NULL ) )
      return;

   if( ( get_eq_char( keeper, WEAR_HOLD_HAND_L ) != obj ) && ( get_eq_char( keeper, WEAR_HOLD_HAND_R ) != obj ) )
      return;  /* Must be wielded to work */
   if( keeper == NULL || keeper->fighting == NULL )
      return;

   for( vch = keeper->in_room->first_person; vch != NULL; vch = vch->next_in_room )
   {
      if( IS_NPC( vch )
          && ( vch->level > keeper->level )
          && IS_SET( vch->act, ACT_AGGRESSIVE ) && vch->fighting == NULL && number_bits( 4 ) == 0 )
      {
         act( "$p carried by $n glows in $s hands.", keeper, obj, NULL, TO_ROOM );
         act( "$p carried by you glows in your hands.", keeper, obj, NULL, TO_CHAR );
         act( "$p says 'LOOK! LOOK!  $n is here!!'", keeper, obj, NULL, TO_ROOM );
         act( "$p says 'LOOK! LOOK!  $n is here!!'", keeper, obj, NULL, TO_CHAR );
         multi_hit( vch, keeper, TYPE_UNDEFINED );
         break;
      }
   }
   return;
}

void objfun_flaming( OBJ_DATA * obj, CHAR_DATA * keeper )
{
   int sn;
   CHAR_DATA *victim;
   int min_lev;
   char *spell = "";
   if( keeper == NULL || keeper->in_room == NULL )
      return;
   if( ( keeper == NULL ) || ( obj->item_type != ITEM_WEAPON ) )
      return;

   for( victim = keeper->in_room->first_person; victim != NULL; victim = victim->next_in_room )
      if( victim->fighting == keeper && number_bits( 2 ) == 0 )
         break;

   if( victim == NULL )
   {
      if( number_percent(  ) < 2 )
      {
         act( "@@N$p@@N carried by @@a$n@@e flames @@ybrightly@@N!", keeper, obj, NULL, TO_ROOM );
         act( "@@N$p @@Ncarried by you@@e flames @@ybrightly@@N!", keeper, obj, NULL, TO_CHAR );
         return;
      }

      if( number_percent(  ) < 2 )
      {
         act( "@@N$p@@N carried by @@a$n@@N goes @@ddim@@N.", keeper, obj, NULL, TO_ROOM );
         act( "@@N$p @@Ncarried by you@@N goes @@ddim@@N.", keeper, obj, NULL, TO_CHAR );
         return;
      }
      return;
   }
   if( ( victim == NULL ) || ( victim->is_free != FALSE ) )
      return;

   if( ( get_eq_char( keeper, WEAR_HOLD_HAND_L ) != obj ) && ( get_eq_char( keeper, WEAR_HOLD_HAND_R ) != obj ) )
      return;  /* Must be wielded to work */

   switch ( number_range( 0, 11 ) )
   {
      case 0:
      case 1:
      case 2:
      case 3:
         min_lev = 5;
         spell = "fireball";
         break;
      case 4:
      case 5:
      case 6:
      case 7:
         min_lev = 7;
         spell = "flamestrike";
         break;
      case 8:
      case 9:
      case 10:
         min_lev = 8;
         spell = "lava burst";
         break;
      case 11:
         min_lev = 30;
         spell = "heat armor";
         break;

   }
   if( ( sn = skill_lookup( spell ) ) < 0 )
      return;
   act( "$p flames at $n!", victim, obj, NULL, TO_ROOM );
   act( "$p flames brightly at you!", victim, obj, NULL, TO_CHAR );
   obj_cast_spell( sn, obj->level, keeper, victim, obj );
   return;
}

void objfun_healing( OBJ_DATA * obj, CHAR_DATA * keeper )
{
   int sn;
   CHAR_DATA *victim;
   int min_lev;
   char *spell = "";
   if( keeper == NULL || keeper->in_room == NULL )
      return;
   if( ( keeper == NULL ) || ( obj->item_type != ITEM_WEAPON ) )
      return;

   for( victim = keeper->in_room->first_person; victim != NULL; victim = victim->next_in_room )
      if( victim->fighting == keeper && number_bits( 1 ) == 0 )
         break;

   if( victim == NULL )
   {
      if( number_percent(  ) < 2 )
      {
         act( "@@N$p@@N carried by @@a$n@@y glows@@N with a @@mHoly @@Wlight@@N!", keeper, obj, NULL, TO_ROOM );
         act( "@@N$p @@Ncarried by you@@y glows@@N with a @@mHoly @@Wlight@@N!", keeper, obj, NULL, TO_CHAR );
         return;
      }

      if( number_percent(  ) < 2 )
      {
         act( "@@N$p@@N carried by @@a$n@@N sings a @@mhymn@@N.", keeper, obj, NULL, TO_ROOM );
         act( "@@N$p @@Ncarried by you@@N sings a hymn@@N.", keeper, obj, NULL, TO_CHAR );
         return;
      }
      return;
   }
   if( ( get_eq_char( keeper, WEAR_HOLD_HAND_L ) != obj ) && ( get_eq_char( keeper, WEAR_HOLD_HAND_R ) != obj ) )
      return;  /* Must be wielded to work */
   switch ( number_range( 0, 11 ) )
   {
      case 0:
      case 1:
         min_lev = 5;
         spell = "cure light";
         break;
      case 2:
      case 3:
      case 4:
         min_lev = 7;
         spell = "cure serious";
         break;
      case 5:
      case 6:
      case 7:
         min_lev = 8;
         spell = "cure critical";
         break;
      case 8:
      case 9:
      case 10:
      case 11:
         min_lev = 30;
         spell = "heal";
         break;

   }
   if( ( sn = skill_lookup( spell ) ) < 0 )
      return;
   act( "$p blankets $n @@Win a @@mhealing @@Waura@@N!", keeper, obj, NULL, TO_ROOM );
   act( "$p blankets you in a @@mhealing @@Waura@@N!", keeper, obj, NULL, TO_CHAR );
   obj_cast_spell( sn, obj->level, keeper, keeper, obj );
   return;
}

void objfun_dispeller( OBJ_DATA * obj, CHAR_DATA * keeper )
{
   int sn;
   CHAR_DATA *victim;
   int min_lev;
   char *spell = "";
   if( keeper == NULL || keeper->in_room == NULL )
      return;
   if( ( keeper == NULL ) || ( obj->item_type != ITEM_WEAPON ) )
      return;

   for( victim = keeper->in_room->first_person; victim != NULL; victim = victim->next_in_room )
      if( victim->fighting == keeper && number_bits( 2 ) == 0 )
         break;

   if( victim == NULL )
   {
      if( number_percent(  ) < 2 )
      {
         act( "@@N$p@@N carried by @@a$n@@a vibrates @@Nsuddenly@@N!", keeper, obj, NULL, TO_ROOM );
         act( "@@N$p @@Ncarried by you@@a vibrates @@Nsuddenly@@N!", keeper, obj, NULL, TO_CHAR );
         return;
      }

      if( number_percent(  ) < 2 )
      {
         act( "@@N$p@@N carried by @@a$n@@a peers@@N at your inventory@@N.", keeper, obj, NULL, TO_ROOM );
         act( "@@N$p @@Ncarried by you@@a peers@@N around the room@@N.", keeper, obj, NULL, TO_CHAR );
         return;
      }
      return;
   }
   if( ( victim == NULL ) || ( victim->is_free != FALSE ) )
      return;

   if( ( get_eq_char( keeper, WEAR_HOLD_HAND_L ) != obj ) && ( get_eq_char( keeper, WEAR_HOLD_HAND_R ) != obj ) )
      return;  /* Must be wielded to work */

   switch ( number_range( 0, 11 ) )
   {
      case 0:
      case 1:
      case 2:
      case 3:
         min_lev = 5;
         spell = "ego whip";
         break;
      case 4:
      case 5:
      case 6:
         min_lev = 7;
         spell = "bloody tears";
         break;
      case 7:
      case 8:
      case 9:
      case 10:
      case 11:
         min_lev = 30;
         spell = "dispel magic";
         break;

   }
   if( ( sn = skill_lookup( spell ) ) < 0 )
      return;
   act( "$p @@apeers@@N deeply into the mind of $n!", victim, obj, NULL, TO_ROOM );
   act( "$p @@apeers @@Ninto your thoughts!", victim, obj, NULL, TO_CHAR );
   obj_cast_spell( sn, obj->level, keeper, victim, obj );
   return;
}

void objfun_regen( OBJ_DATA * obj, CHAR_DATA * keeper )
{
   if( keeper == NULL || keeper->in_room == NULL )
      return;

   if( obj->wear_loc < 0 )
      return;
   keeper->hit = UMIN( keeper->max_hit, keeper->hit + ( number_range( obj->level / 20, obj->level / 5 ) ) );
   return;
}
