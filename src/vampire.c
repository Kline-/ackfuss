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

#ifndef DEC_ACT_INFO_H
#include "h/act_info.h"
#endif

#ifndef DEC_ACT_MOVE_H
#include "h/act_move.h"
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

#ifndef DEC_MAGIC_H
#include "h/magic.h"
#endif

#ifndef DEC_SAVE_H
#include "h/save.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

extern bool deathmatch;

/*
 * This file should contain:
 *	o Vamp Spells
 *	o Vamp skills
 *
 */

bool spell_blood_leach( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   CHAR_DATA *check;
   list<CHAR_DATA *>::iterator li;
   AFFECT_DATA *paf;
   AFFECT_DATA af;

   /*
    * check for vamps 
    */
   if( !IS_VAMP( ch ) )
   {
      send_to_char( "Only vampires are evil enough to use this spell!\r\n", ch );
      return FALSE;
   }

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      check = *li;
      for( paf = check->first_affect; paf != NULL; paf = paf->next )
         if( paf->type == sn && paf->caster == ch )
         {
            send_to_char( "You are already maintaining a blood leach on someone else!\r\n", ch );
            return FALSE;
         }
   }

   act( "You create a blood leach and hurl it at $N!", ch, NULL, victim, TO_CHAR );
   act( "$n creates a blood leach and hurls it at $N!", ch, NULL, victim, TO_NOTVICT );
   act( "$n creates a blood leach and hurls it at YOU!", ch, NULL, victim, TO_VICT );

   if( saves_spell( level, victim ) || IS_VAMP( victim ) || is_affected( victim, sn ) )
   {
      send_to_char( "The blood leach misses you, and vanishes!\r\n", victim );
      send_to_char( "The blood leach misses, and vanishes!\r\n", ch );
      act( "The blood leach misses $N, and vanishes!", ch, NULL, victim, TO_NOTVICT );
      return TRUE;
   }

   af.type = sn;
   af.duration = number_range( 2, UMAX( 4, level / 3 ) );
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.bitvector = 0;
   af.caster = ch;

   affect_to_char( victim, &af );

   act( "The blood leach hits $N and burrows into $S skin!", ch, NULL, victim, TO_ROOM );
   send_to_char( "The blood leach hits you, and burrows into your skin!\r\n", victim );
   return TRUE;
}

bool spell_shade( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   ROOM_INDEX_DATA *room;
   ROOM_AFFECT_DATA raf;

   room = ch->in_room;

   if( room == NULL )
      return FALSE;

   if( IS_SET( room->affected_by, ROOM_BV_SHADE ) )
   {
      send_to_char( "There is already a shade spell operating here!\r\n", ch );
      return FALSE;
   }

   if( !IS_OUTSIDE( ch ) )
   {
      send_to_char( "It might help if you tried this outside...\r\n", ch );
      return FALSE;
   }

   act( "$n throws a dark shadow into the air.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "You throw a dark shadow into the air.\r\n", ch );

   raf.type = sn;
   raf.duration = ( level / 8 ) + number_range( 1, 3 );
   raf.level = level;
   raf.bitvector = ROOM_BV_SHADE;
   raf.caster = ch;
   affect_to_room( room, &raf );
   return TRUE;
}


bool spell_embrace( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;

   if( !IS_VAMP( ch ) )
   {
      send_to_char( "Better leave that to the REAL Vampyres, you wanna be!!!\r\n", ch );
      return FALSE;
   }
   if( ch->pcdata->super->bloodline == 0 )
   {
      send_to_char( "@@dRENEGADES@@n can't embrace!!!\r\n", ch );
      return FALSE;
   }

   if( IS_VAMP( victim ) )
   {
      send_to_char( "@@NYour @@dchosen@@N has already been @@eEmbraced@@N!!!.\r\n", ch );
      return FALSE;
   }

   if( IS_WOLF( victim ) )
   {
      send_to_char( "A sudden awareness comes over you, as you peer into their soul and see a @@bGarou@@N!!!\r\n", ch );
      if( !IS_AWAKE( victim ) )
         do_wake( victim, "" );
      if( victim->position < POS_STANDING )
         do_stand( victim, "" );
      if( ch->position < POS_STANDING )
         do_stand( ch, "" );
      one_hit( ch, victim, TYPE_UNDEFINED );
      return FALSE;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Fergit it bub!!\r\n", ch );
      return FALSE;
   }

/*    if ( victim->clan == 7 )
     {
       send_to_char( "@@mNO WAY!!!!!@@W They are @@dKindred @@eKillers!!!!!@@N\r\n", ch );
       return FALSE;
     }
*/
   if( victim->position > POS_SLEEPING )
   {
      /*
       * failure 
       */
      act( "$N is far too alert to let you do that!", ch, NULL, victim, TO_CHAR );
      return FALSE;
   }
   else

   {

      victim->pcdata->super->level = 1;
      victim->pcdata->super->exp = 0;
      victim->pcdata->super->skills_learned = 0;
      victim->pcdata->super->skills_max = 2;
      if( victim->pcdata->super->generation == -1 )
         victim->pcdata->super->generation = ch->pcdata->super->generation + 2;
      else
         victim->pcdata->super->generation = ch->pcdata->super->generation + 1;

      if( victim->pcdata->super->generation < 4 )
         victim->pcdata->super->generation = 5;
      victim->pcdata->super->bloodline = ch->pcdata->super->bloodline;
      victim->pcdata->super->energy = -10;
      victim->pcdata->super->energy_max = 20;
      victim->act.set(ACT_VAMPIRE);
      victim->pcdata->condition[COND_FULL] = 20;
      victim->pcdata->condition[COND_THIRST] = 20;
      victim->pcdata->super->pracs = 2;
      ch->pcdata->super->energy_max -= 5;
      victim->pcdata->recall_vnum = 9001;


      send_to_char( "@@NYou sink your fangs into your helpless victim.\r\n", ch );
      send_to_char( "Then, in the final death throes, you bite your own arm and allow the @@eblood@@N\r\n", ch );
      send_to_char( "to drip into their mouth, damning them to @@dETERNAL NIGHT@@N!!!\r\n", ch );
      send_to_char( "you feel some of your essential @@eblood@@N potential drain into your victim!\r\n", ch );
      send_to_char( "@@NYou feel your soul ripped asunder as the Vampyre draws your lifeblood into it's mouth!!\r\n",
                    victim );
      send_to_char( "As your last breath escapes you, and you begin to submit to death itself, you suddenly feel\r\n",
                    victim );
      send_to_char
         ( "a hot, coppery tasting fluid enter your mouth.  As the strength of the @@dKindred@@N begins to fill\r\n",
           victim );
      send_to_char( "your veins, your mind is flooded with the sudden knowledge of damnation and @@dEternal Night@@N!!!\r\n",
                    victim );
      send_to_char( "Awakening to a new knowledge of your status as a @@eChosen@@N, you know that upon your\r\n", victim );
      send_to_char( "first taste of mortal @@eblood@@N, you shall forever enter the ranks of the @@dKindred@@N!!\r\n",
                    victim );
      send_to_char( "A choice awaits you....shall you accept this destiny and feed upon mortals, or shall you\r\n", victim );
      send_to_char( "seek @@Wcleansing@@N from this @@dEternal Damnation@@N???\r\n", victim );

      victim->pcdata->learned[skill_lookup( "feed" )] = 90;
   }



   return TRUE;
}


void do_family( CHAR_DATA * ch, char *argument )
{


   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   DESCRIPTOR_DATA *d;
   bool found;
   short index;

   if( ch->level < 85 )
      if( !IS_VAMP( ch ) )
      {
         send_to_char( "Huh?\r\n", ch );
         return;
      }
   if( get_trust( ch ) == 85 )
   {
      for( index = 0; index <= 4; index++ )
      {
         switch ( index )
         {
            case 0:
               snprintf( buf, MSL, "\r\n@@WNOT SET@@N\r\n" );
               break;
            case 1:
               snprintf( buf, MSL, "\r\n@@WNOT SET@@N\r\n" );
               break;
            case 2:
               snprintf( buf, MSL, "\r\n@@WNOT SET@@N\r\n" );
               break;
            case 3:
               snprintf( buf, MSL, "\r\n@@WNOT SET@@N\r\n" );
               break;
            case 4:
               snprintf( buf, MSL, "\r\n@@WNOT SET@@N\r\n" );
               break;
         }

         send_to_char( buf, ch );
         found = FALSE;
         for( d = first_desc; d != NULL; d = d->next )
         {
            if( d->connected == CON_PLAYING
                && ( victim = d->character ) != NULL && !IS_NPC( victim ) && victim->in_room != NULL )
            {
               if( IS_VAMP( victim ) && ( victim->pcdata->super->bloodline == index ) )
               {
                  found = TRUE;
               }
               else
                  continue;



               snprintf( buf, MSL, "%-15s @@NGen: @@r%d   @@NRank: @@d%d@@N  @@NBloodlust: @@e%d@@N/@@e%d@@N   %-15s\r\n",
                        victim->name.c_str(), victim->pcdata->super->generation, victim->pcdata->super->level, victim->pcdata->super->energy,
                        victim->pcdata->super->energy_max, victim->in_room->name );
               send_to_char( buf, ch );
            }
         }

         if( !found )
            send_to_char( "@@NNo @@dKindred@@N Family members were found.\r\n", ch );

      }
   }
   else
   {

      snprintf( buf, MSL, "@@WMembers of the @@dKindred @@NFamily %s\r\n", get_family_name( ch ) );
      send_to_char( buf, ch );
      found = FALSE;
      for( d = first_desc; d != NULL; d = d->next )
      {
         if( d->connected == CON_PLAYING
             && ( victim = d->character ) != NULL
             && !IS_NPC( victim ) && victim->in_room != NULL && IS_VAMP( victim ) && !IS_IMMORTAL( victim ) )
         {
            if( victim->pcdata->super->bloodline != ch->pcdata->super->bloodline )
               continue;

            found = TRUE;
            snprintf( buf, MSL, "%-15s @@NGeneration: @@r%d     @@NRank: @@d%d@@N\r\n",
                     victim->name.c_str(), victim->pcdata->super->generation, victim->pcdata->super->level );
            send_to_char( buf, ch );
         }
      }

      if( !found )
         send_to_char( "@@NNo other @@dKindred @@N Family members were found.\r\n", ch );

   }
   return;
}

void do_instruct( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   int sn;
   char buf[MAX_STRING_LENGTH];


   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_instruct] == 0 )
   {
      send_to_char( "You are not trained in this skill!\r\n", ch );
      return;
   }


   argument = one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Instruct who?\r\n", ch );
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
      send_to_char( "Oh yeah, that would help a lot!\r\n", ch );
      return;
   }

   if( !IS_VAMP( victim ) )
   {
      send_to_char( "@@eWHAT????@@N You want to teach @@dKindred@@N skills to someone not @@eCHOSEN@@N? NO WAY!!", ch );
      return;
   }

/* okay, we know the victim is good, and a vamp..check for skill known, levels, etc. */

   one_argument( argument, arg );
   sn = skill_lookup( arg );

   if( ch->pcdata->super->bloodline != victim->pcdata->super->bloodline )
   {
      send_to_char( "There not a member of your family!!\r\n", ch );
      return;
   }


   if( ch->pcdata->learned[sn] == 0 || skill_table[sn].flag2 != VAMP )

   {
      send_to_char( "@@NYou don't know that @@dKindred@@N skill!.\r\n", ch );
      return;
   }
   if( victim->pcdata->learned[sn] != 0 )
   {
      send_to_char( "They already know that skill.\r\n", ch );
      return;
   }

   if( victim->pcdata->super->level < skill_table[sn].skill_level[victim->pcdata->super->bloodline] )
   {
      send_to_char( "@@NThey are too inexperienced in the ways of the @@dKindred@@N to learn this skill.\r\n", ch );
      send_to_char( "@@NYou are to inexperienced in the ways of the @@dKindred@@N to learn this skill.\r\n", victim );
      return;
   }

   if( victim->pcdata->super->skills_learned >= victim->pcdata->super->skills_max )
   {
      send_to_char( "They seem unable to grasp the knowledge.\r\n", ch );
      send_to_char( "@@NYou are unable to learn any more @@dKindred@@N knowledge at this time.\r\n", victim );
      return;
   }

/* Okay, the skill is good, the instructor knows it, and the victim doesn't  */

   victim->pcdata->learned[sn] = 90;
   victim->pcdata->super->pracs -= 1;
   victim->pcdata->super->skills_learned += 1;

   send_to_char( "You have taught them another way of the @@dKindred@@N!!!\r\n", ch );
   snprintf( buf, MSL, "You are now learned in the way of @@e%s@@N!!!\r\n", skill_table[sn].name );
   send_to_char( buf, victim );


   return;
}

bool spell_mesmerise( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   /*
    * check for vamps 
    */
   if( !IS_VAMP( ch ) )
   {
      send_to_char( "Only vampires are evil enough to use this spell!\r\n", ch );
      return FALSE;
   }



   act( "You stare into $S eyes!", ch, NULL, victim, TO_CHAR );
   act( "$n stares into $S eyes!", ch, NULL, victim, TO_NOTVICT );
   act( "$n stares into your eyes!", ch, NULL, victim, TO_VICT );

   if( saves_spell( level, victim ) || IS_VAMP( victim ) || is_affected( victim, sn ) )
   {

      return TRUE;
   }
   send_to_char( " Spell removed for now.\r\n", ch );
   return FALSE;
   set_stun( victim, level / 1 );

   act( "$N seems to be mesmerized!", ch, NULL, victim, TO_ROOM );
   send_to_char( "You are mesmerised!\r\n", victim );
   return TRUE;
}

bool spell_cloak_darkness( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   AFFECT_DATA af;
   if( IS_NPC( ch ) )
   {
      send_to_char( "Not for NPCS!\r\n", ch );
      return FALSE;
   }

   if( is_affected( ch, sn ) )
      return FALSE;

   af.type = sn;
   af.duration = ch->pcdata->super->level;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char( ch, &af );

   return TRUE;
}


bool spell_blood_walk( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{


   CHAR_DATA *victim = ( CHAR_DATA * ) vo;

   if( deathmatch )
   {
      send_to_char( "Not during a @@eDeath Match@@N!!\r\n", ch );
      return FALSE;
   }
   if( ( victim = get_char_world( ch, target_name ) ) == NULL
       || victim == ch
       || !IS_NPC( victim )
       || victim->in_room == NULL
       || victim->in_room->room_flags.test(RFLAG_PRIVATE)
       || victim->in_room->room_flags.test(RFLAG_SOLITARY)
       || victim->in_room->room_flags.test(RFLAG_SAFE)
       || victim->act.test(ACT_NO_BLOOD)
       || victim->in_room->room_flags.test(RFLAG_NO_BLOODWALK)
       || ( ( victim->get_level("psuedo") - ch->get_level("psuedo") ) > 20 ) )
   {
      send_to_char( "Your @@eblood@@N burns with rage, as your efforts are shaken off.\r\n", ch );
      return TRUE;
   }
   if( ( time_info.hour < 19 ) && ( time_info.hour > 5 ) )
   {
      send_to_char( "@@NYou may only @@eblood walk@@N at night!!\r\n", ch );
      return FALSE;
   }


   if( victim->act.test(ACT_NO_VISIT) )
   {
      send_to_char( "You cannot sense your target's @@eblood@@N!\r\n", ch );
      return TRUE;
   }

   if( victim->in_room->room_flags.test(RFLAG_NO_BLOODWALK) )
   {
      send_to_char( "You cannot sense your target's @@eblood@@N!\r\n", ch );
      return FALSE;
   }

   if( victim->act.test(ACT_NO_BLOOD) )
   {
      send_to_char( "You cannot sense your target's @@eblood@@N!\r\n", ch );
      return FALSE;
   }

   act( "$n fades into the darkness.", ch, NULL, NULL, TO_ROOM );
   char_from_room( ch );
   char_to_room( ch, victim->in_room );
   act( "$n steps forth from the shadows.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "@@NYou feel yourself drawn to the @@eblood@@N within your victim!\r\n", ch );
   do_look( ch, "auto" );
   return TRUE;
}

bool spell_blood_sign( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   MARK_DATA *mark;
   char buf[MSL];
   char *costring;

   if( IS_NPC( ch ) || ( !IS_VAMP( ch ) ) )
      return FALSE;

   if( target_name[0] == '\0' )
   {
    send_to_char("You can't leave an empty sign!\r\n",ch);
    return FALSE;
   }

   mark = new MARK_DATA;

   mark->room_vnum = ch->in_room->vnum;
   mark->message = str_dup( target_name );
   switch ( ( int )ch->pcdata->super->level / 4 )
   {
      case 0:
         costring = "@@a";
         break;
      case 1:
         costring = "@@c";
         break;
      case 2:
         costring = "@@R";
         break;
      case 3:
         costring = "@@m";
         break;
      case 4:
         costring = "@@e";
         break;
      default:
         costring = "@@m";
         break;
   }
   snprintf( buf, MSL, "%s%s @@W: %s", costring, ch->name.c_str(), get_family_name( ch ) );
   mark->author = str_dup( buf );
   mark->duration = ( ( MAX_VAMP_LEVEL ) - ch->pcdata->super->generation ) * ( ch->pcdata->super->level );
   mark->type = VAMP;
   ch->in_room->mark_list.push_back(mark);
   save_marks();
   return TRUE;
}

bool spell_blood_sense( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   list<MARK_DATA *>::iterator li;
   MARK_DATA *mk = NULL;

   if( IS_NPC( ch ) || ( !IS_VAMP( ch ) ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return FALSE;
   }

   if( ch->in_room->mark_list.empty() )
   {
      send_to_char( "You do not sense any @@eBloodSign@@N here.\r\n", ch );
      return FALSE;
   }

   for( li = ch->in_room->mark_list.begin(); li != ch->in_room->mark_list.end(); li++ )
   {
      char buf[MSL];
      mk = *li;
      if( mk->type != VAMP )
         continue;

      snprintf( buf, MSL, "%s : %s\r\n", mk->author, mk->message );
      send_to_char( buf, ch );
   }
   return TRUE;
}
