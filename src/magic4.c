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

extern bool deathmatch;


/*
 * This file should contain:
 *	o Adept Spells
 *	o additional remort spells
 */

bool spell_fireblast( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;


   if( obj == NULL )
   {
      dam = ( get_psuedo_level( ch ) / 2 )
         + dice( ( get_psuedo_level( ch ) / 6 + ADEPT_LEVEL( ch ) / 2 ), 10 )
         + dice( ( get_psuedo_level( ch ) / 6 + ADEPT_LEVEL( ch ) / 2 ), 10 );
      act( "@@gA blast of @@efire@@g flies from $n's hands!@@N", ch, NULL, NULL, TO_ROOM );
      send_to_char( "@@gA blast of @@efire@@g flies from your hands!@@N\n\r", ch );
   }
   else
   {
      dam = dice( level / 4, 15 );
      act( "@@gA blast of @@efire@@g flies from $p!@@N", ch, obj, NULL, TO_ROOM );
      act( "@@gA blast of @@efire@@g flies from $p!@@N", ch, obj, NULL, TO_CHAR );
   }
   if( saves_spell( level, victim ) )
      dam /= 2;
   act( "@@g$n is struck by the blast of @@efire@@g!!@@N", victim, NULL, NULL, TO_ROOM );
   send_to_char( "@@gYou are struck by the @@efire @@gblast!!@@N\n\r", victim );
   sp_damage( obj, ch, victim, dam, REALM_FIRE, sn, TRUE );
   return TRUE;
}

bool spell_shockstorm( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;


   if( obj == NULL )
   {
      dam = ( get_psuedo_level( ch ) / 2 )
         + dice( ( get_psuedo_level( ch ) / 5 + ADEPT_LEVEL( ch ) / 2 ), 10 )
         + dice( ( get_psuedo_level( ch ) / 5 + ADEPT_LEVEL( ch ) / 2 ), 10 );
      act( "@@gA storm of @@lsparks@@g flies from $n's hands!@@N", ch, NULL, NULL, TO_ROOM );
      send_to_char( "@@gA storm of @@lsparks@@g flies from your hands!@@N\n\r", ch );
   }
   else
   {
      dam = dice( level / 4, 20 );
      act( "@@gA storm of @@lsparks@@g flies from $p!@@N", ch, obj, NULL, TO_ROOM );
      act( "@@gA storm of @@lsparks@@g flies from $p!@@N", ch, obj, NULL, TO_CHAR );
   }
   if( saves_spell( level, victim ) )
      dam /= 2;
   act( "@@g$n is struck by the storm of @@lsparks@@g!!@@N", victim, NULL, NULL, TO_ROOM );
   send_to_char( "@@gYou are struck by the storm of @@lsparks@@g!!@@N\n\r", victim );
   sp_damage( obj, ch, victim, dam, REALM_SHOCK, sn, TRUE );
   return TRUE;
}

bool spell_cone_cold( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;


   if( obj == NULL )
   {
      dam = ( get_psuedo_level( ch ) / 2 )
         + dice( ( get_psuedo_level( ch ) / 5 + ADEPT_LEVEL( ch ) / 2 ), 10 )
         + dice( ( get_psuedo_level( ch ) / 5 + ADEPT_LEVEL( ch ) / 2 ), 10 );
      act( "@@gA cone of @@acold@@g bursts forth from $n's hands!@@N", ch, NULL, NULL, TO_ROOM );
      send_to_char( "@@gA cone of @@acold@@g bursts forth from your hands!@@N\n\r", ch );
   }
   else
   {
      dam = dice( level / 4, 20 );
      act( "@@gA cone of @@acold@@g bursts forth from $p!@@N", ch, obj, NULL, TO_ROOM );
      act( "@@gA cone of @@acold@@g bursts forth from $p!@@N", ch, obj, NULL, TO_CHAR );
   }
   if( saves_spell( level, victim ) )
      dam /= 2;
   act( "@@g$n is struck by the cone of @@acold@@g!!@@N", victim, NULL, NULL, TO_ROOM );
   send_to_char( "@@gYou are struck by the cone of @@acold@@g!!@@N\n\r", victim );
   sp_damage( obj, ch, victim, dam, REALM_COLD, sn, TRUE );
   return TRUE;
}

bool spell_holy_wrath( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;


   if( obj == NULL )
   {
      dam = ( get_psuedo_level( ch ) / 2 )
         + dice( ( get_psuedo_level( ch ) / 4 + ADEPT_LEVEL( ch ) ), 12 )
         + dice( ( get_psuedo_level( ch ) / 4 + ADEPT_LEVEL( ch ) ), 12 );
      act( "@@gA coruscating sphere of @@ylight@@g bursts forth from $n's hands!@@N", ch, NULL, NULL, TO_ROOM );
      send_to_char( "@@gA coruscating sphere of @@ylight@@g bursts forth from your hands!@@N\n\r", ch );
   }
   else
   {
      dam = dice( level / 5, 20 );
      act( "@@gA coruscating sphere of @@ylight@@g bursts forth from $p!@@N", ch, obj, NULL, TO_ROOM );
      act( "@@gA coruscating sphere of @@ylight@@g bursts forth from $p!@@N", ch, obj, NULL, TO_CHAR );
   }
   if( saves_spell( level, victim ) )
      dam /= 2;
   act( "@@g$n is struck by the coruscating sphere of @@ylight@@g!!@@N", victim, NULL, NULL, TO_ROOM );
   send_to_char( "@@gYou are struck by the coruscating sphere of @@ylight@@g!!@@N\n\r", victim );
   sp_damage( obj, ch, victim, dam, REALM_HOLY, sn, TRUE );
   return TRUE;
}

bool spell_wraith_touch( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;
   float drain_mod = 1.0;

   if( obj == NULL )
   {
      dam = ( get_psuedo_level( ch ) / 3 )
         + dice( ( get_psuedo_level( ch ) / 6 + ADEPT_LEVEL( ch ) ), 8 )
         + dice( ( get_psuedo_level( ch ) / 6 + ADEPT_LEVEL( ch ) ), 8 );
      act( "@@RA @@dwraithlike hand @@Rleaps forth from $n!@@N", ch, NULL, NULL, TO_ROOM );
      send_to_char( "@@RA @@dwraithlike hand @@Rleaps forth from your hands!@@N\n\r", ch );
   }
   else
   {
      dam = dice( level / 5, 20 );
      act( "@@RA @@dwraithlike hand @@Rleaps forth from $p!@@N", ch, obj, NULL, TO_ROOM );
      act( "@@RA @@dwraithlike hand @@Rleaps forth from $p!@@N", ch, obj, NULL, TO_CHAR );
   }
   if( saves_spell( level, victim ) )
      dam /= 2;
   act( "@@R$n is struck by the @@dwraithlike hand @@R!!@@N", victim, NULL, NULL, TO_ROOM );
   send_to_char( "@@RYou are struck by a @@dwraithlike hand @@R!!@@N\n\r", victim );
   drain_mod = get_psuedo_level( ch ) * dam / 130;
   if( sp_damage( obj, ch, victim, dam, REALM_DRAIN | NO_REFLECT | NO_ABSORB, sn, TRUE ) )
      ch->hit = UMIN( ch->max_hit, ( ch->hit + drain_mod ) );
   return TRUE;
}

bool spell_thought_vise( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;


   if( obj == NULL )
   {
      dam = ( get_psuedo_level( ch ) / 2 )
         + dice( ( get_psuedo_level( ch ) / 3 + ADEPT_LEVEL( ch ) ), 10 )
         + dice( ( get_psuedo_level( ch ) / 3 + ADEPT_LEVEL( ch ) ), 10 );
      act( "@@rA crushing weight brushes your mind, then is gone.@@N", ch, NULL, NULL, TO_ROOM );
      send_to_char( "@@rYou clench your mind, crushing the thoughts of your foe.\n\r@@N", ch );
   }
   else
   {
      dam = dice( level / 5, 20 );
      act( "@@rA crushing weight fills your mind.@@N", ch, obj, NULL, TO_CHAR );
   }
   if( saves_spell( level, victim ) )
      dam /= 2;
   act( "@@rA crushing weight brushes your mind, then is gone.@@N", victim, NULL, NULL, TO_ROOM );
   send_to_char( "@@rA crushing weight fills your mind.@@N", victim );
   sp_damage( obj, ch, victim, dam, REALM_MIND | NO_REFLECT | NO_ABSORB, sn, FALSE );
   return TRUE;
}

bool spell_black_curse( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_CURSE ) )
   {
      send_to_char( "They are already weakened!\n\r", ch );
      return FALSE;
   }
   if( saves_spell( level, victim ) )
      return TRUE;
   if( saves_spell( level, victim ) )
      return TRUE;
   af.type = sn;
   af.duration = 2 * ( level / 8 );
   af.location = APPLY_HITROLL;
   af.modifier = -1 * get_psuedo_level( ch ) / 12 * get_psuedo_level( victim ) / 10;
   af.bitvector = AFF_CURSE;
   affect_to_char( victim, &af );

   af.location = APPLY_AC;
   af.modifier = 7 * get_psuedo_level( ch ) / 12 * get_psuedo_level( victim ) / 10;
   affect_to_char( victim, &af );

   send_to_char( "@@RA Cloud of @@dDespair@@R washes over you.@@N\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}


bool spell_cloak_misery( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   AFFECT_DATA af;


   if( is_affected( ch, sn ) )
      return FALSE;



   af.type = sn;
   af.duration = get_psuedo_level( ch ) / 8;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char( ch, &af );

   return TRUE;
}


bool spell_poison_quinine( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, sn ) )
   {
      send_to_char( "They are already poisoned with quinine!\n\r", ch );
      return FALSE;
   }
   if( saves_spell( level, victim ) )
      if( saves_spell( level, victim ) )
         return TRUE;
   af.type = sn;
   af.duration = 2 * ( level / 8 );
   af.location = APPLY_HITROLL;
   af.modifier = -1 * get_psuedo_level( ch ) / 20 * get_psuedo_level( victim ) / 20;
   af.bitvector = AFF_POISON;
   affect_to_char( victim, &af );

   af.location = APPLY_AC;
   af.modifier = 7 * get_psuedo_level( ch ) / 20 * get_psuedo_level( victim ) / 20;
   affect_to_char( victim, &af );

   send_to_char( "@@You feel a dart pierce your neck.@@N\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}

bool spell_poison_arsenic( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, sn ) )
   {
      send_to_char( "They are already poisoned with arsenic!\n\r", ch );
      return FALSE;
   }
   if( saves_spell( level, victim ) )
      if( saves_spell( level, victim ) )
         return TRUE;
   af.type = sn;
   af.duration = 2 * ( level / 8 );
   af.location = APPLY_DAMROLL;
   af.modifier = -1 * get_psuedo_level( ch ) / 20 * get_psuedo_level( victim ) / 20;
   af.bitvector = AFF_POISON;
   affect_to_char( victim, &af );

   af.location = APPLY_AC;
   af.modifier = 5 * get_psuedo_level( ch ) / 20 * get_psuedo_level( victim ) / 20;
   affect_to_char( victim, &af );

   send_to_char( "@@You feel a dart pierce your neck.@@N\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}

bool spell_sonic_blast( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;
   int dam;
   dam = dice( get_psuedo_level( ch ) / 2, 3 ) + dice( get_psuedo_level( ch ) / 2, 3 );
   if( saves_spell( level, victim ) )
      dam = dam * 2 / 3;
   if( sp_damage( obj, ch, victim, dam, REALM_SOUND | NO_REFLECT | NO_ABSORB, sn, TRUE ) )
   {
      if( is_affected( ch, sn ) )
         return TRUE;
      af.type = sn;
      af.duration = ( level / 20 );
      af.location = APPLY_DAMROLL;
      af.modifier = -1 * get_psuedo_level( ch ) / 15 * get_psuedo_level( victim ) / 15;
      af.bitvector = AFF_BLASTED;
      affect_to_char( victim, &af );

      af.type = sn;
      af.duration = ( level / 20 );
      af.location = APPLY_HITROLL;
      af.modifier = -1 * get_psuedo_level( ch ) / 15 * get_psuedo_level( victim ) / 15;
      af.bitvector = AFF_BLASTED;
      affect_to_char( victim, &af );

   }
   return TRUE;
}

bool spell_mystical_focus( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   AFFECT_DATA af;


   if( is_affected( ch, sn ) )
      return FALSE;



   af.type = sn;
   af.duration = get_psuedo_level( ch ) / 7;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char( ch, &af );

   return TRUE;
}
