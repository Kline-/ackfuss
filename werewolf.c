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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
#endif

#ifndef DEC_ACT_MOVE_H
#include "h/act_move.h"
#endif

#ifndef DEC_BITMASK_H
#include "h/bitmask.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

extern bool deathmatch;

/*
 * This file should contain:
 *	o Wolf Spells
 *	o Wolf skills
 *
 */

void do_tribe( CHAR_DATA * ch, char *argument )
{

   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
/*    int value, max;   */

   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   DESCRIPTOR_DATA *d;
   bool found;
   short index;

   if( IS_NPC( ch ) )
      return;

   if( ch->level < 85 )
      if( !IS_WOLF( ch ) )
      {
         send_to_char( "Huh?\n\r", ch );
         return;
      }


   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   argument = one_argument( argument, arg3 );

   if( ( arg1[0] == '\0' ) || ( ch->pcdata->super->generation > 1 ) )
   {

      if( get_trust( ch ) == 85 )
      {
         for( index = 0; index <= 4; index++ )
         {

            switch ( index )
            {
               case 0:
                  xprintf( buf, "\n\r@@WRENEGADE!!@@N\n\r" );
                  break;
               case 1:
                  xprintf( buf, "\n\r@@WCHANGE ME@@N\n\r" );
                  break;
               case 2:
                  xprintf( buf, "\n\r@@WCHANGE ME@@N\n\r" );
                  break;
               case 3:
                  xprintf( buf, "\n\r@@WCHANGE ME@@N\n\r" );
                  break;
               case 4:
                  xprintf( buf, "\n\r@@WCHANGE ME@@N\n\r" );
                  break;
            }

            send_to_char( buf, ch );
            found = FALSE;
            for( d = first_desc; d != NULL; d = d->next )
            {
               if( d->connected == CON_PLAYING
                   && ( victim = d->character ) != NULL && !IS_NPC( victim ) && victim->in_room != NULL )
               {
                  if( IS_WOLF( victim ) && ( victim->pcdata->super->bloodline == index ) )
                  {
                     found = TRUE;
                  }
                  else
                     continue;



                  xprintf( buf, "%-15s @@NStanding: @@r%s   @@NRank: @@d%d@@N  @@NRage: @@e%d@@N/@@e%d@@N   %-15s\n\r",
                           victim->name, get_tribe_standing_name( victim->pcdata->super->generation ), victim->pcdata->super->level,
                           victim->pcdata->super->energy, victim->pcdata->super->energy_max, victim->in_room->name );
                  send_to_char( buf, ch );
               }
            }

            if( !found )
               send_to_char( "@@NNo @@bGarou@@N Tribe members were found.\n\r", ch );

         }
      }
      else
      {

         xprintf( buf, "@@WMembers of the @@bGarou @@rTribe %s\n\r", get_tribe_name( ch ) );
         send_to_char( buf, ch );
         found = FALSE;
         for( d = first_desc; d != NULL; d = d->next )
         {
            if( d->connected == CON_PLAYING
                && ( victim = d->character ) != NULL
                && !IS_NPC( victim ) && victim->in_room != NULL && IS_WOLF( victim ) && !IS_IMMORTAL( victim ) )
            {
               if( victim->pcdata->super->bloodline != ch->pcdata->super->bloodline )
                  continue;

               found = TRUE;
               xprintf( buf, "%-15s @@NStanding: @@r%s   @@NRank: @@d%d@@N\n\r",
                        victim->name, get_tribe_standing_name( victim->pcdata->super->generation ), victim->pcdata->super->level );
               send_to_char( buf, ch );
            }
         }

         if( !found )
            send_to_char( "@@NNo other @@bGarou@@N @@rTribe@@N members were found.\n\r", ch );

      }
      return;

   }
   else  /* multiplexed use of tribe  */
   {
      if( ch->pcdata->super->generation > 1 )
      {
         send_to_char( "Huh?\n\r", ch );
         return;
      }



      if( ( victim = get_char_room( ch, arg1 ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }

      if( ( IS_NPC( victim ) )
          || ( !IS_WOLF( victim ) ) || ( victim->pcdata->super->bloodline != ch->pcdata->super->bloodline ) )
      {
         send_to_char( "They are not in your @@bTribe@@N!\n\r", ch );
         return;
      }

      if( !str_cmp( arg2, "standing" ) )
      {
         if( ( arg2[0] == '\0' ) || ( arg3[0] == '\0' ) )
         {
            send_to_char( "Use tribe <wolf> standing <new standing>\n\r", ch );
            return;
         }
         if( !str_cmp( arg3, "mate" ) )
            victim->pcdata->super->generation = 2;
         else if( !str_cmp( arg3, "warder" ) )
            victim->pcdata->super->generation = 3;
         else if( !str_cmp( arg3, "guardian" ) )
            victim->pcdata->super->generation = 4;
         else if( !str_cmp( arg3, "sentry" ) )
            victim->pcdata->super->generation = 5;
         else if( !str_cmp( arg3, "elder" ) )
            victim->pcdata->super->generation = 6;
         else if( !str_cmp( arg3, "adult" ) )
            victim->pcdata->super->generation = 7;
         else if( !str_cmp( arg3, "younger" ) )
            victim->pcdata->super->generation = 8;
         else if( !str_cmp( arg3, "cub" ) )
            victim->pcdata->super->generation = 9;

         else
         {
            send_to_char( "@@yStandings@@N: mate warder guardian sentry elder adult younger cub.\n\r", ch );
            return;
         }

         xprintf( buf, "@@N%s's @@yStanding@@N is now %s in @@bTribe@@N %s.\n\r",
                  victim->name, get_tribe_standing_name( victim->pcdata->super->generation ), get_tribe_name( ch ) );
         do_howl( ch, buf );
         return;

      }
      if( !str_cmp( arg2, "banish" ) )
      {
         int sn;
         xprintf( buf, "@@N%s has been @@eBANISHED@@N from @@bTribe@@N %s.\n\r", victim->name, get_tribe_name( ch ) );
         do_howl( ch, buf );
         victim->pcdata->super->bloodline = 0;
         victim->pcdata->super->generation = 9;
         victim->pcdata->recall_vnum = 3001;
         victim->pcdata->super->level = 1;
         for( sn = 0; sn <= MAX_SKILL; sn++ )
            if( ( skill_table[sn].flag2 == WOLF ) && ( victim->pcdata->learned[sn] > 0 ) )
               victim->pcdata->learned[sn] = 0;
         char_from_room( victim );
         char_to_room( victim, get_room_index( victim->pcdata->recall_vnum ) );
         victim->sentence += 15000;
         if( !is_set( victim->act, ACT_KILLER ) )
            set_bit( victim->act, ACT_KILLER );
         do_save( victim, "" );
         return;
      }


      send_to_char( "Uknown option--use tribe <wolf> standing <new standing>\n\r", ch );
      return;
   }
}


void do_imprint( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   int sn;
   char buf[MAX_STRING_LENGTH];

   if( IS_NPC( ch ) || !IS_WOLF( ch ) )
   {
      send_to_char( "Huh?\n\r", ch );
      return;
   }

   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_imprint] == 0 )
   {
      send_to_char( "You have not been imprinted with this skill!\n\r", ch );
      return;
   }

   if( ch->pcdata->super->generation > 3 )
   {
      send_to_char( "Your @@bTribal @@yStanding@@N is not high enough to imprint!\n\r", ch );
      return;
   }

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Imprint who?\n\r", ch );
      return;
   }

   victim = get_char_room( ch, arg );
   if( victim == NULL )
   {
      send_to_char( "Couldn't find the target.\n\r", ch );
      return;
   }
   if( victim == ch )
   {
      send_to_char( "Oh yeah, that would help a lot!\n\r", ch );
      return;
   }

   if( !IS_WOLF( victim ) )
   {
      send_to_char( "@@eWHAT????@@N You want to teach @@bGarou@@N skills to MORTAL@@N? NO WAY!!", ch );
      return;
   }

/* okay, we know the victim is good, and a wolf..check for skill known, levels, etc. */

   one_argument( argument, arg );
   sn = skill_lookup( arg );


   if( ch->pcdata->learned[sn] == 0 || skill_table[sn].flag2 != WOLF )
   {
      send_to_char( "@@NYou don't know that @@bGarou@@N skill!.\n\r", ch );
      return;
   }
   if( victim->pcdata->learned[sn] != 0 )
   {
      send_to_char( "They already know that skill.\n\r", ch );
      return;
   }

   if( victim->pcdata->super->level < skill_table[sn].skill_level[victim->pcdata->super->bloodline] )
   {
      send_to_char( "@@NThey are too inexperienced in the ways of the @@bGarou@@N to learn this skill.\n\r", ch );
      send_to_char( "@@NYou are to inexperienced in the ways of the @@bGarou@@N to learn this skill.\n\r", victim );
      return;
   }

   if( victim->pcdata->super->skills_learned >= victim->pcdata->super->skills_max )
   {
      send_to_char( "They seem unable to grasp the instinct.\n\r", ch );
      send_to_char( "@@NYou are unable to learn any more @@bGarou@@N knowledge at this time.\n\r", victim );
      return;
   }

/* Okay, the skill is good, the instructor knows it, and the victim doesn't  */

   victim->pcdata->learned[sn] = 90;
   victim->pcdata->super->pracs -= 1;
   victim->pcdata->super->skills_learned += 1;

   send_to_char( "You have increased their @@bGarou@@N instincts@@N!!!\n\r", ch );
   xprintf( buf, "You are now instinctively aware of @@e%s@@N!!!\n\r", skill_table[sn].name );
   send_to_char( buf, victim );


   return;
}


bool spell_tribe_claw( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   short wolves = 0;
   short mortals = 0;
   short vamps = 0;
   CHAR_DATA *local_dude;

   if( !IS_WOLF( ch ) )
   {
      send_to_char( "Huh?\n\r", ch );
      return FALSE;
   }
   if( ch->pcdata->super->bloodline == 0 )
   {
      send_to_char( "@@dOutcasts@@n can't tribal claw!!!\n\r", ch );
      return FALSE;
   }

   if( IS_WOLF( victim ) )
   {
      send_to_char( "@@NYour selection is already a @@bGarou@@N!!!.\n\r", ch );
      return FALSE;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Fergit it bub!!\n\r", ch );
      return FALSE;
   }

/*    if ( victim->pcdata->clan == 7 )
     {
       send_to_char( "@@mNO WAY!!!!!@@W They are @@dKindred @@eKillers!!!!!@@N\n\r", ch );
       return FALSE;
     }

*/
   if( IS_VAMP( victim ) )
   {
      send_to_char( "A sudden awareness comes over you, as you peer into their soul and see a @@dKindred@@N!!!\n\r", ch );
      if( !IS_AWAKE( victim ) )
         do_wake( victim, "" );
      if( victim->position < POS_STANDING )
         do_stand( victim, "" );
      if( ch->position < POS_STANDING )
         do_stand( ch, "" );
      one_hit( ch, victim, TYPE_UNDEFINED );
      return FALSE;
   }

   for( local_dude = ch->in_room->first_person; local_dude != NULL; local_dude = local_dude->next_in_room )
   {
      if( IS_NPC( local_dude ) )
         continue;
      if( IS_WOLF( local_dude ) )
         wolves++;
      else if( IS_VAMP( local_dude ) )
         vamps++;
      else
         mortals++;
   }

   if( vamps > 0 )
   {
      send_to_char( "In front of @@dKindred@@N??? Not a chance!!\n\r", ch );
      return FALSE;
   }
   else if( mortals > 1 )
   {
      send_to_char( "In front of @@aMortals@@N??? Not a chance!!\n\r", ch );
      return FALSE;
   }
   else if( wolves < 4 )
   {
      send_to_char( "You must have more @@bGarou@@N witnesses to the event.\n\r", ch );
      return FALSE;
   }



   if( victim->position > POS_RESTING )
   {
      /*
       * failure 
       */
      char socbuf[MSL];

      act( "$N must grovel at your feet before you will accept him under your control.\n\r", ch, NULL, victim, TO_CHAR );
      check_social( ch, "growl", victim->name );
      xprintf( socbuf, "%s", ch->name );
      check_social( victim, "grovel", socbuf );
      check_social( victim, "cower", socbuf );
      return FALSE;
   }
   else

   {
      check_social( victim, "rollover", ch->name );
      victim->pcdata->super->level = 1;
      victim->pcdata->super->exp = 0;
      victim->pcdata->super->skills_learned = 0;
      victim->pcdata->super->skills_max = 2;
      victim->pcdata->super->generation = 9;
      victim->pcdata->super->bloodline = ch->pcdata->super->bloodline;
      victim->pcdata->super->energy = -10;
      victim->pcdata->super->energy_max = 7;
      set_bit( victim->act, ACT_WEREWOLF );
      victim->pcdata->super->pracs = 2;
      victim->pcdata->recall_vnum = ch->pcdata->recall_vnum;


      send_to_char( "@@NYou thrash the cowering form beneath you, raking your @@eclaws@@N along it's soul.\n\r", ch );
      send_to_char( "Then, in the final death throes, you chomp it's neck, tearing out it's essence,@@N\n\r", ch );
      send_to_char
         ( "and breathe your Tribal strength into the bloody corpse, making them forever submissive to your @@btribe@@N!!!\n\r",
           ch );

      send_to_char( "@@NYou feel your soul ripped asunder as the @@bGarou@@N @@eClaws@@N your very soul to shreds@@N!!\n\r",
                    victim );
      send_to_char( "As it tears your throat out, and you begin to submit to death itself, you suddenly feel\n\r", victim );
      send_to_char( "a strong, earthly essence overcome your soul.  As the strength of the @@bGarou@@N begins to fill\n\r",
                    victim );
      send_to_char
         ( "your veins, your mind is flooded with the sudden knowledge of @@bTribal @@Npower and everlasting loyalty to the @@rEldest@@N.\n\r",
           victim );

      act( "$n suddenly reaches out and claws $N to shreds!", ch, NULL, victim, TO_NOTVICT );
      act( "$n then takes $N's throat in $s mouth, and tears it out!", ch, NULL, victim, TO_NOTVICT );
      act( "$N goes into convulsions, then slowly steadies, then peers up at $n with adoration.", ch, NULL, victim,
           TO_NOTVICT );
      victim->pcdata->learned[skill_lookup( "scent" )] = 90;
   }



   return TRUE;
}

bool spell_wolf_mark( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   MARK_DATA *mark;
   char buf[MSL];
   char *costring;

   if( IS_NPC( ch ) || ( !IS_WOLF( ch ) ) )
      return FALSE;

   GET_FREE( mark, mark_free );

   mark->room_vnum = ch->in_room->vnum;
   mark->message = str_dup( target_name );
   switch ( ch->pcdata->super->bloodline )
   {
      case 0:
         costring = "@@d";
         break;
      case 1:
         costring = "@@y";
         break;
      case 2:
         costring = "@@r";
         break;
      case 3:
         costring = "@@e";
         break;
      case 4:
         costring = "@@b";
         break;
      default:
         costring = "@@m";
         break;
   }
   xprintf( buf, "%s%s @@W: %s", costring, ch->name, get_tribe_standing_name( ch->pcdata->super->generation ) );
   mark->author = str_dup( buf );
   mark->duration = ( ( MAX_WOLF_LEVEL ) - ch->pcdata->super->generation ) * ( ch->pcdata->super->level ) * 10;
   mark->type = WOLF;
   mark_to_room( ch->in_room->vnum, mark );
   check_social( ch, "dogleg", "" );
   return TRUE;
}

void do_scent( CHAR_DATA * ch, char *argument )
{
   MARK_LIST_MEMBER *mark_list;

   if( IS_NPC( ch ) || ( !IS_WOLF( ch ) ) )
   {
      send_to_char( "Huh?\n\r", ch );
      return;
   }

   if( ch->in_room->first_mark_list == NULL )
   {
      send_to_char( "You do not smell any information here.\n\r", ch );
      return;
   }

   for( mark_list = ch->in_room->first_mark_list; mark_list != NULL; mark_list = mark_list->next )
   {
      char buf[MSL];
      if( mark_list->mark->type != WOLF )
         continue;

      xprintf( buf, "%s : %s\n\r", mark_list->mark->author, mark_list->mark->message );
      send_to_char( buf, ch );
   }
   check_social( ch, "sniff", "" );
}
