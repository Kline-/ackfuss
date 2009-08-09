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

/* Mount, ok ppl lets mount them damn horses, i knew they had to be good
 * for something.
 *
 * Code written by Celestian on Oct 1st 1996
 *
 * Originally written for ThrynnMUD 2.0, Merc 2.2 based.
 *
 * Copyright (c) 1996 Virtual Research
 */

#include "h/globals.h"

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
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

#ifndef DEC_MOUNT_H
#include "h/mount.h"
#endif

#ifndef DEC_UPDATE_H
#include "h/update.h"
#endif

DO_FUN(do_mount)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *mount;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Mob can't ride anything!\r\n ", ch );
      return;
   }

   one_argument( argument, arg );

   if( *arg )
   {
      if( ( mount = get_char_room( ch, arg ) ) == NULL )
      {
         send_to_char( "I see no one by that name " "here!\r\n", ch );
         return;
      }
   }
   else
   {
      send_to_char( "Whom do you wish to ride?\r\n", ch );
      return;
   }

   if( ch->position == POS_RIDING )
   {
      if( ( ch->riding == NULL ) )
      {
         ch->position = POS_STANDING;
         send_to_char( "Odd, you were mounted on thin air!\r\n", ch );
         return;
      }
   }

   if( mount == ch )
   {
      send_to_char( "Ride on your own back?  How?\r\n", ch );
      return;
   }

   if( ch->position == POS_RIDING )
   {
      send_to_char( "You are already riding on something.\r\n", ch );
      return;
   }

   if( ch->in_room->room_flags.test(RFLAG_SOLITARY) )
   {
      send_to_char( "This room is too narrow to ride.\r\n", ch );
      return;
   }

   if( IS_NPC( mount ) )
   {
      if( !mount->act.test(ACT_MOUNT) )
      {
         act( "It's too difficult to ride on $N.", ch, 0, mount, TO_CHAR );
         return;
      }

      if( IS_AFFECTED( mount, AFF_CHARM ) && mount->master != ch )
      {
         act( "$N does not recognize you and refuses to " "let you ride $M.", ch, 0, mount, TO_CHAR );
         return;
      }

      if( IS_NPC( ch ) || ch->pcdata->learned[gsn_mount] == 0 || ( ch->get_level("psuedo") < mount->get_level("psuedo") ) )
      {
         act( "You are not skillful enough to ride $N.", ch, NULL, mount, TO_CHAR );
         return;
      }

      if( mount->rider && mount->rider != ch )
      {
         act( "Someone else is riding on $N.", ch, 0, mount, TO_CHAR );
         return;
      }

      if( ch->move < MOUNT_COST )
      {
         ch->move = 0;
         move_gain( ch );
         act( "You are too tired to ride $N.", ch, 0, mount, TO_CHAR );
         return;
      }
      ch->move -= MOUNT_COST;
      move_gain( ch );

      ch->riding = mount;
      mount->rider = ch;
      ch->position = POS_RIDING;
      if( !mount->act.test(ACT_SENTINEL) )
       mount->act.set(ACT_SENTINEL);
      act( "You climb on and ride $N.", ch, 0, mount, TO_CHAR );
      act( "$n climbs on and rides $N.", ch, 0, mount, TO_ROOM );
      act( "$n climbs on and rides you.", ch, 0, mount, TO_VICT );
   }
   else
   {
      send_to_char( "You can't ride on another player!", ch );
   }

}

/* returns true if char is in same room.  Don't want to check ch
 * directly because it may be an invalid pointer already.
 * i.e.  Someone summons ride away and kills it.
 */

static int valid_ride( int room, CHAR_DATA * ch )
{
   CHAR_DATA *i;

   if( room == NOWHERE )
      return FALSE;
   if( ch->in_room == NULL )
      return FALSE;
   for( i = ch->in_room->first_person; i; i = i->next_in_room )
   {
      if( IS_NPC( i ) && ( i == ch ) )
         return TRUE;
   }
   return FALSE;
}

DO_FUN(do_dismount)
{
   if( IS_NPC( ch ) )
   {
      send_to_char( "You can't dismount.\r\n", ch );
      return;
   }

   if( ch->position == POS_RIDING )
   {
      if( !IS_RIDING( ch ) )
      {
         ch->position = POS_STANDING;
         send_to_char( "Odd, you were mounted on thin air!\r\n", ch );
         return;
      }
   }

   if( ch->position == POS_RIDING )
   {
      if( valid_ride( ch->in_room->vnum, ch->riding ) )
      {
         if( ch->in_room->sector_type >= SECT_WATER_SWIM && ch->level < 5 && !IS_AFFECTED( ch, AFF_FLYING ) )
            act( "Here? That's not too wise.", ch, 0, 0, TO_CHAR );
         else
         {
            act( "You dismount $N.", ch, 0, ch->riding, TO_CHAR );
            act( "$n dismounts $N.", ch, 0, ch->riding, TO_ROOM );
            if( is_same_group( ch, ch->riding ) )
               do_group( ch, const_cast<char *>(ch->riding->name.c_str()) );
            ch->position = POS_STANDING;
            ch->riding->rider = 0;
            ch->riding = 0;
         }
      }
      else  /* mount not in same room??? How did this happen? */
         stop_riding( ch );
      if( ch->riding != NULL )
         stop_follower( ch->riding );
   }
   else
      send_to_char( "You are not riding anything!\r\n", ch );
}

/* update riding info when mount/rider no longer in same room
 */

void stop_riding( CHAR_DATA * ch )
{
   CHAR_DATA *mount, *t_ch;

   if( !ch )
      return;

   if( IS_NPC( ch ) )
   {
      mount = ch;
      t_ch = mount->rider;
   }
   else
   {
      t_ch = ch;
      mount = t_ch->riding;
   }

   if( !t_ch ) /* rider has gone poof */
   {
      if( !mount )   /* so has the mount! */
      {
         return;
      }
      else
      {
         if( !IS_NPC( mount ) )
         {
            send_to_char( "We would appreciate it if " "you explained how a player" "became a mount?\r\n", mount );
            mount->riding = NULL;
            mount->position = POS_STANDING;
            update_pos( mount );
            return;
         }
         else
         {
            act( "You realize that you no longer have " "a rider!", mount, 0, 0, TO_CHAR );
            mount->rider = 0;
            return;
         }
      }
   }
   else
   {  /* well we have a rider of some sort */
      if( IS_NPC( t_ch ) )
      {
         char buf[MAX_STRING_LENGTH];

         snprintf( buf, MSL, "NPC %s got on a mount somehow", t_ch->get_name() );
         log_string( buf );
         t_ch->rider = NULL;
         update_pos( t_ch );
         if( mount )
         {
            send_to_char( "Your analomous rider has " "been forcibly dismounted.\r\n", mount );
            if( IS_NPC( mount ) )
               mount->rider = NULL;
            else
            {
               send_to_char( "We would " "appreciate it if you " "explained how a player " "became a mount?\r\n", mount );
               mount->riding = NULL;
               mount->position = POS_STANDING;
               update_pos( mount );
            }
         }
         return;
      }
      if( t_ch->position != POS_RIDING )
      {
         if( IS_RIDING( t_ch ) )
         {
            char buf[MAX_STRING_LENGTH];
            snprintf( buf, MSL, "%s on mount, but not RIDING", t_ch->name.c_str() );
            log_string( buf );
            t_ch->position = POS_RIDING;
         }
         else
            return;
      }
      if( !mount )   /* but mount has vanished! */
      {
         act( "You fall to the ground.  You stop riding.", t_ch, 0, 0, TO_CHAR );
         t_ch->riding = NULL;
         t_ch->position = POS_RESTING;
         update_pos( t_ch );
      }
      else
      {  /* ok, mount and rider both exist, but t_ch is no longer riding */
         if( t_ch->in_room != mount->in_room )
         {
            act( "You fall to the ground.  You stop " "riding.", ch, 0, 0, TO_CHAR );
            t_ch->riding = NULL;
            ch->position = POS_RESTING;
            update_pos( ch );
            if( !IS_NPC( mount ) )
            {
               send_to_char( "We would " "appreciate it if you " "explained how a player " "became a mount?\r\n", mount );
               mount->riding = NULL;
               mount->position = POS_STANDING;
               update_pos( mount );
               return;
            }
            else
            {
               act( "You realize that you no " "longer have a rider!", mount, 0, 0, TO_CHAR );
               mount->rider = NULL;
            }
         }
         else
         {
            act( "You stop riding $N.", t_ch, 0, mount, TO_CHAR );
            if( is_same_group( ch, mount ) )
            {
               do_group( ch, const_cast<char *>(mount->name.c_str()) );
            }
            t_ch->riding = NULL;
            mount->rider = NULL;
            t_ch->position = POS_STANDING;

         }
      }
   }
}

/* check to make sure mount and rider both exist and are in same room
 */

bool check_valid_ride( CHAR_DATA * ch )
{
   if( !ch || IS_NPC( ch ) )
      return FALSE;

   if( valid_ride( ch->in_room->vnum, ch->riding ) )
      if( ( ch->riding )->rider == ch )
         return TRUE;

   stop_riding( ch );

   return FALSE;
}
