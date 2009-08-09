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

/* Mob/player hunting.. */
/* Copied this from one of my old MUDs..  Much cleaner than the sillymud
   hunt routines  -- Alty */

#include "h/globals.h"

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
#endif

#ifndef DEC_ACT_MOVE_H
#include "h/act_move.h"
#endif

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
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

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif
 
H_QUEUE *h_head = NULL;
H_QUEUE *h_tail = NULL;

#ifdef DEBUG_HUNT_CODE
static FILE *h_fp;
#endif

void h_dequeue( void )
{
H_QUEUE *hunt;

if( !( hunt = h_head ) )
return;
h_head = hunt->next;
if( h_tail == hunt )
h_tail = NULL;
hunt->room->room_flags.reset(RFLAG_HUNT_MARK);
#ifdef DEBUG_HUNT_CODE
fprintf( h_fp, "Dequeue: %5d\n", hunt->room->vnum );
fflush( h_fp );
#endif
dispose( hunt, sizeof( *hunt ) );
return;
}

void h_clear( void )
{
#ifdef DEBUG_HUNT_CODE
fprintf( h_fp, "h_clear\n" );
fflush( h_fp );
#endif
while( h_head != NULL )
h_dequeue( );
#ifdef DEBUG_HUNT_CODE
fprintf( h_fp, "Cleared\n" );
fflush( h_fp );
#endif
}

void h_enqueue( ROOM_INDEX_DATA * room, short dir )
{
   H_QUEUE *hunt;

   hunt = (H_QUEUE *)getmem( sizeof( *hunt ) );
   hunt->next = NULL;
   hunt->room = room;
   hunt->dir = dir;
   room->room_flags.set(RFLAG_HUNT_MARK);
   if( !h_head )
    h_head = hunt;
   else
    h_tail->next = hunt;
   h_tail = hunt;
#ifdef DEBUG_HUNT_CODE
   fprintf( h_fp, "Enqueue: %5d - %s\n", room->vnum, room->room_flags.test(RFLAG_HUNT_MARK) ? "set" : "unset" );
   fflush( h_fp );
#endif
   return;
}

bool h_is_valid_exit( ROOM_INDEX_DATA * room, short dir, int h_flags )
{
   EXIT_DATA *exit = room->exit[dir];

   if( !exit )
      return FALSE;
   if( !exit->to_room )
      return FALSE;
#ifdef DEBUG_HUNT_CODE
   fprintf( h_fp, "IsValid: %5d - %s\n", exit->to_room->vnum,
            ( exit->to_room->room_flags.test(RFLAG_HUNT_MARK) ? "set" : "unset" ) );
   fflush( h_fp );
#endif
   if( exit->to_room->room_flags.test(RFLAG_HUNT_MARK) )
      return FALSE;
   if( !IS_SET( h_flags, HUNT_WORLD ) && room->area != exit->to_room->area )
      return FALSE;
   if( exit->exit_info.test(EX_CLOSED) )
   {
      if( !IS_SET( h_flags, HUNT_OPENDOOR ) )
         return FALSE;
      if( exit->exit_info.test(EX_LOCKED) )
      {
         if( !IS_SET( h_flags, HUNT_UNLOCKDOOR | HUNT_PICKDOOR ) )
            return FALSE;
         if( !IS_SET( h_flags, HUNT_UNLOCKDOOR ) && exit->exit_info.test(EX_PICKPROOF) )
            return FALSE;
      }
   }
   return TRUE;
}

void h_enqueue_room( ROOM_INDEX_DATA * room, short dir, int h_flags )
{
   short edir;

#ifdef DEBUG_HUNT_CODE
   fprintf( h_fp, "h_enqueue_room\n" );
   fflush( h_fp );
#endif
   for( edir = 0; edir < MAX_DIR; edir++ )
      if( h_is_valid_exit( room, edir, h_flags ) )
         h_enqueue( room->exit[edir]->to_room, ( dir == -1 ? edir : dir ) );
   return;
}

short h_find_dir( ROOM_INDEX_DATA * room, ROOM_INDEX_DATA * target, int h_flags )
{
   H_QUEUE *hunt;

   if( room == target )
      return -1;
#ifdef DEBUG_HUNT_CODE
   if( !h_fp )
      h_fp = file_open( "hunt.out", "w" );
   fprintf( h_fp, "h_find_dir\n" );
   fflush( h_fp );
#endif
   room->room_flags.set(RFLAG_HUNT_MARK);
   h_enqueue_room( room, -1, h_flags );
   for( hunt = h_head; hunt; hunt = hunt->next )
   {
      if( hunt->room == target )
      {
         short dir = hunt->dir;

#ifdef DEBUG_HUNT_CODE
         fprintf( h_fp, "Found dir %d\n", dir );
         fflush( h_fp );
#endif
         h_clear();
         room->room_flags.reset(RFLAG_HUNT_MARK);
         return dir;
      }
      h_enqueue_room( hunt->room, hunt->dir, h_flags );
   }
#ifdef DEBUG_HUNT_CODE
   fprintf( h_fp, "Invalid dir\n" );
   fflush( h_fp );
#endif
   h_clear();
   room->room_flags.reset(RFLAG_HUNT_MARK);
   return -1;
}

bool set_hunt( CHAR_DATA * ch, CHAR_DATA * fch, CHAR_DATA * vch, OBJ_DATA * vobj, int set_flags, int rem_flags )
{
   int nflags;
   ROOM_INDEX_DATA *troom;
   char buf[MAX_STRING_LENGTH];

   troom = ( vch ? vch->in_room : vobj ? vobj->in_room : NULL );
   nflags = ( ( ch ? ch->hunt_flags : 0 ) | set_flags ) & ~rem_flags;
   if( !ch || !troom || h_find_dir( ch->in_room, troom, nflags ) < 0 )
      return FALSE;
   ch->hunting = vch;
   ch->hunt_obj = vobj;
   if( IS_NPC(ch) )
    ch->npcdata->hunt_for = fch;
   if( IS_NPC( ch ) )
      ch->npcdata->hunt_home = ( ch->npcdata->hunt_home ? ch->npcdata->hunt_home : ch->in_room );
   if( !ch->searching.empty() )
    ch->searching.clear();
   ch->hunt_flags = nflags;
   snprintf( buf, MSL, "%s has started hunting (%s) %s",
            ch->get_name(),
            ( vch ? IS_NPC( vch ) ? "mobile" : "player" : "object" ), ( vch ? vch->get_name() : vobj->short_descr ) );
   if( fch )
      snprintf( buf + strlen( buf ), (2 * MIL), " for %s", fch->get_name() );
   monitor_chan( buf, MONITOR_HUNTING );
   return TRUE;
}

void end_hunt( CHAR_DATA * ch )
{
   ch->hunting = NULL;
   ch->hunt_obj = NULL;
   if( IS_NPC(ch) )
   {
    ch->npcdata->hunt_for = NULL;
    ch->npcdata->hunt_home = NULL;
   }
   if( !IS_NPC( ch ) )
      ch->hunt_flags = 0;
   else
      ch->hunt_flags = ch->npcdata->pIndexData->hunt_flags;
   if( !ch->searching.empty() )
    ch->searching.clear();
}

void hunt_move( CHAR_DATA * mob, short dir )
{
   EXIT_DATA *exit = mob->in_room->exit[dir];

   if( exit->exit_info.test(EX_CLOSED) )
   {
      if( exit->exit_info.test(EX_LOCKED) )
      {
         if( IS_SET( mob->hunt_flags, HUNT_UNLOCKDOOR ) && has_key( mob, exit->key ) )
            do_unlock( mob, dir_name[dir] );
         else if( IS_SET( mob->hunt_flags, HUNT_PICKDOOR ) && !exit->exit_info.test(EX_PICKPROOF) )
            do_pick( mob, dir_name[dir] );
      }
      else if( IS_SET( mob->hunt_flags, HUNT_OPENDOOR ) )
         do_open( mob, dir_name[dir] );
   }
   else
      move_char( mob, dir, FALSE );
   return;
}

bool mob_hunt( CHAR_DATA * mob )
{
   short dir;
   char buf[MSL];

   if( !mob || !IS_NPC( mob ) )
      return FALSE;
   if( !mob->hunting )
   {
      if( mob->hunt_obj != NULL )
      {
         if( mob->hunt_obj->in_room == NULL )
         {
            if( IS_SET( mob->hunt_flags, HUNT_CR ) && mob->npcdata->hunt_for && mob->hunt_obj->item_type == ITEM_CORPSE_PC )
               act( "$N tells you 'Someone else seems to have gotten to your "
                    "corpse before me.'", mob->npcdata->hunt_for, NULL, mob, TO_CHAR );
            end_hunt( mob );
            return TRUE;
         }
         if( can_see_obj( mob, mob->hunt_obj ) && mob->in_room == mob->hunt_obj->in_room )
         {
            if( IS_SET( mob->hunt_flags, HUNT_CR ) && mob->npcdata->hunt_for && mob->hunt_obj->item_type == ITEM_CORPSE_PC )
               act( "$N tell you 'I have found your corpse.  I shall return it "
                    "to you now.", mob->npcdata->hunt_for, NULL, mob, TO_CHAR );
            obj_from_room( mob->hunt_obj );
            obj_to_char( mob->hunt_obj, mob );
            act( "$n gets $o.", mob, mob->hunt_obj, NULL, TO_ROOM );
            set_hunt( mob, NULL, mob->npcdata->hunt_for, mob->hunt_obj, 0, 0 );
            return TRUE;
         }
         if( !can_see_obj( mob, mob->hunt_obj ) ||
             ( dir = h_find_dir( mob->in_room, mob->hunt_obj->in_room, mob->hunt_flags ) ) < 0 )
         {
            if( IS_SET( mob->hunt_flags, HUNT_CR ) && mob->npcdata->hunt_for && mob->hunt_obj->item_type == ITEM_CORPSE_PC )
               act( "$N tells you 'I seem to have lost the way to your corpse.'", mob->npcdata->hunt_for, NULL, mob, TO_CHAR );
            end_hunt( mob );
            return TRUE;
         }
         hunt_move( mob, dir );
         if( mob->in_room == mob->hunt_obj->in_room )
            mob_hunt( mob );
      }
      /*
       * deleted because the hunt is started in mobile_update
       * else if ( mob->searching &&
       * (mob->hunting = get_char_world(mob, mob->searching)) != NULL )
       * {
       * free_string(mob->searching);
       * mob->searching = NULL;
       * }
       * --Aeria
       */
      else if( mob->npcdata->hunt_home )
      {
         dir = -1;
         if( mob->in_room == mob->npcdata->hunt_home || ( dir = h_find_dir( mob->in_room, mob->npcdata->hunt_home, mob->hunt_flags ) ) < 0 )
         {
            mob->npcdata->hunt_home = NULL;
            mob->hunt_flags = mob->npcdata->pIndexData->hunt_flags;
         }
         else if( dir >= 0 )
            hunt_move( mob, dir );
      }
      else
      {
         return FALSE;
      }
      return TRUE;
   }
   if( !can_see( mob, mob->hunting ) )
   {
      if( IS_SET( mob->hunt_flags, HUNT_MERC ) && mob->npcdata->hunt_for )
      {
         /*
          * 6.25% chance of giving up, 18.75% chance of telling employer.
          */
         switch ( number_bits( 4 ) )
         {
            case 0:
               snprintf( buf, MSL, "$N tells you '%s seems to have disappeared!'", mob->hunting->get_name() );
               act( buf, mob->npcdata->hunt_for, NULL, mob, TO_CHAR );
               end_hunt( mob );
               return TRUE;
            case 1:
            case 2:
            case 3:
               snprintf( buf, MSL, "$N tells you '%s seems to have disappeared!  I shall "
                        "find %s though!'", mob->hunting->get_name(),
                        ( mob->hunting->sex == SEX_MALE ? "him" : mob->hunting->sex == SEX_FEMALE ? "her" : "it" ) );
               act( buf, mob->npcdata->hunt_for, NULL, mob, TO_CHAR );
               return TRUE;
         }
      }
      if( IS_SET( mob->hunt_flags, HUNT_INFORM ) )
      {
         switch ( number_bits( 5 ) )
         {
            case 0:
               snprintf( buf, MSL, "Where are you, %s?", mob->hunting->get_name() );
               break;
            case 1:
               snprintf( buf, MSL, "Why can't I find you, %s?", mob->hunting->get_name() );
               break;
            case 2:
               snprintf( buf, MSL, "I know you're out there, %s!", mob->hunting->get_name() );
               break;
            case 3:
               snprintf( buf, MSL, "I'll find you, %s, just wait!", mob->hunting->get_name() );
               break;
            default:
               return FALSE;
         }
         do_yell( mob, buf );
         return TRUE;
      }
      return FALSE;
   }
   if( mob->in_room == mob->hunting->in_room )
   {
      if( IS_SET( mob->hunt_flags, HUNT_CR ) && mob->hunt_obj && mob->hunt_obj->item_type == ITEM_CORPSE_PC )
      {
         act( "$N tells you 'I have returned with your corpse!'", mob->hunting, NULL, mob, TO_CHAR );
         /*
          * Ok maybe im a little paranoid here.. :).. -- Alty
          */
         if( mob->hunt_obj->carried_by == mob )
         {
            if( mob->hunt_obj->wear_loc != WEAR_NONE )
               unequip_char( mob, mob->hunt_obj );
            act( "$n drops $o.", mob, mob->hunt_obj, NULL, TO_ROOM );
            obj_from_char( mob->hunt_obj );
            obj_to_room( mob->hunt_obj, mob->in_room );
         }
         end_hunt( mob );
         return TRUE;
      }
      if( IS_SET( mob->hunt_flags, HUNT_MERC ) && mob->npcdata->hunt_for )
      {
         snprintf( buf, MSL, "$N tells you 'I have found %s!  Now %s shall die!'",
                  mob->hunting->get_name(),
                  ( mob->hunting->sex == SEX_FEMALE ? "she" : mob->hunting->sex == SEX_MALE ? "he" : "it" ) );
         act( buf, mob->npcdata->hunt_for, NULL, mob, TO_CHAR );
      }
      switch ( number_bits( 2 ) )
      {
         case 0:
            snprintf( buf, MSL, "Now I have you, %s!", mob->hunting->get_name() );
            break;
         case 1:
            snprintf( buf, MSL, "I knew you'd be here, %s!", mob->hunting->get_name() );
            break;
         case 2:
            snprintf( buf, MSL, "Did you really think you were safe, %s?", mob->hunting->get_name() );
            break;
         case 3:
            snprintf( buf, MSL, "So here you are, %s!", mob->hunting->get_name() );
            break;
      }
      if( IS_SET( mob->hunt_flags, HUNT_INFORM ) )
         do_yell( mob, buf );
      else
         do_say( mob, buf );
      one_hit( mob, mob->hunting, TYPE_UNDEFINED );
      end_hunt( mob );
      return TRUE;
   }

   if( ( dir = h_find_dir( mob->in_room, mob->hunting->in_room, mob->hunt_flags ) ) == -1 )
   {
      if( IS_SET( mob->hunt_flags, HUNT_MERC ) && mob->npcdata->hunt_for )
      {
         /*
          * 6.25% chance of giving up, 18.75% chance of informing employer
          */
         switch ( number_bits( 4 ) )
         {
            case 0:
               snprintf( buf, MSL, "$N tells you 'I seem to have lost %s's trail.'", mob->hunting->get_name() );
               act( buf, mob->npcdata->hunt_for, NULL, mob, TO_CHAR );
               end_hunt( mob );
               return TRUE;
            case 1:
            case 2:
            case 3:
               snprintf( buf, MSL, "$N tells you 'I seem to have lost %s's trail.  I shall "
                        "find it again, though!'", mob->hunting->get_name() );
               act( buf, mob->npcdata->hunt_for, NULL, mob, TO_CHAR );
               return TRUE;
         }
      }
      if( IS_SET( mob->hunt_flags, HUNT_INFORM ) )
      {
         switch ( number_bits( 6 ) )
         {
            case 0:
               snprintf( buf, MSL, "Where are you hiding, %s?", mob->hunting->get_name() );
               break;
            case 1:
               snprintf( buf, MSL, "You can't run forever, %s!", mob->hunting->get_name() );
               break;
            case 2:
               snprintf( buf, MSL, "Come out, come out, wherever you are, %s!", mob->hunting->get_name() );
               break;
            case 3:
               snprintf( buf, MSL, "I promise I won't hurt you, %s.", mob->hunting->get_name() );
               break;
            default:
               return FALSE;
         }
         do_yell( mob, buf );
         return TRUE;
      }
      return FALSE;
   }
   hunt_move( mob, dir );
   if( mob->in_room == mob->hunting->in_room )
      mob_hunt( mob );
   return TRUE;
}

void char_hunt( CHAR_DATA * ch )
{
   short dir;
   char buf[MAX_STRING_LENGTH];

   if( IS_NPC( ch ) )
      return;
   if( !ch->hunting )
   {
      if( ch->hunt_obj )
      {
         if( !can_see_obj( ch, ch->hunt_obj ) || !ch->hunt_obj->in_room )
         {
            snprintf( buf, MSL, "@@RYou seem to have lost the trail to %s.@@N\r\n", ch->hunt_obj->short_descr );
            send_to_char( buf, ch );
            end_hunt( ch );
         }
         else if( ch->hunt_obj->in_room == ch->in_room )
         {
            snprintf( buf, MSL, "@@RAhhh.  You have found %s!@@N\r\n", ch->hunt_obj->short_descr );
            send_to_char( buf, ch );
            end_hunt( ch );
         }
         else if( ( dir = h_find_dir( ch->in_room, ch->hunt_obj->in_room, ch->hunt_flags ) ) < 0 )
         {
            snprintf( buf, MSL, "@@RYou seem to have lost the trail to %s.@@N\r\n", ch->hunt_obj->short_descr );
            send_to_char( buf, ch );
            end_hunt( ch );
         }
         else
         {
            snprintf( buf, MSL, "@@RYou sense that %s is %s of here.@@N\r\n", ch->hunt_obj->short_descr, dir_name[dir] );
            send_to_char( buf, ch );
         }
      }
      return;
   }
   if( !can_see( ch, ch->hunting ) )
   {
      send_to_char( "@@RYou seem to have lost your prey.@@N\r\n", ch );
      end_hunt( ch );
   }
   else if( ch->in_room == ch->hunting->in_room )
   {
      send_to_char( "@@RAhhh.  You have found your prey!@@N\r\n", ch );
      end_hunt( ch );
   }
   else if( ( dir = h_find_dir( ch->in_room, ch->hunting->in_room, ch->hunt_flags ) ) < 0 )
   {
      send_to_char( "@@RYou seem to have lost your prey.@@N\r\n", ch );
      end_hunt( ch );
   }
   else
   {
      snprintf( buf, MSL, "@@RYou sense your prey is %s of here.@@N\r\n", dir_name[dir] );
      send_to_char( buf, ch );
   }
   return;
}

DO_FUN(do_hunt)
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   short chance;

   if( IS_NPC( ch ) && ( ch->hunting || ch->hunt_obj ) )
      return;
   argument = one_argument( argument, arg );
   if( !*arg )
   {
      send_to_char( "Hunt for whom?\r\n", ch );
      return;
   }
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
      victim = get_char_world( ch, arg );
   else
      victim = ( get_trust( ch ) >= MAX_LEVEL ? get_char_world( ch, arg ) : get_char_area( ch, arg ) );
   if( victim == ch )
   {
      if( ch->hunting )
      {
         snprintf( arg, MIL, "You stop hunting %s.\r\n", ch->hunting->get_name() );
         send_to_char( arg, ch );
      }
      else if( ch->hunt_obj )
      {
         snprintf( arg, MIL, "You stop looking for %s.\r\n", ch->hunt_obj->short_descr );
         send_to_char( arg, ch );
      }
      else
      {
         send_to_char( "You find yourself right where you're standing!\r\n", ch );
      }
      end_hunt( ch );
      return;
   }
   else if( !IS_IMMORTAL( ch ) && ( victim != NULL ) && !IS_NPC( victim ) && IS_IMMORTAL( victim ) )
   {
      snprintf( arg, MIL, "You can't hunt Immortal %s!\r\n", victim->get_name() );
      send_to_char( arg, ch );
      return;
   }

   /*
    * 100% chance of finding them if you're looking right at them.. :) 
    */
   if( victim != NULL && victim->in_room == ch->in_room )
   {
      send_to_char( "You're already there!\r\n", ch );
      return;
   }
   /*
    * Chance of picking up wrong trail.. (25% for NPC, PC @ normal) 
    */
   chance = ( IS_NPC( ch ) ? 75 : ch->pcdata->learned[gsn_hunt] );
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
      chance = ( ( MAX_WOLF_LEVEL - ch->pcdata->super->generation ) * 4 ) + ch->pcdata->super->level;

   if( chance < number_percent(  ) )
   {
      CHAR_DATA *vch;
      list<CHAR_DATA *>::iterator li;
      short vcnt = 0;

      victim = NULL;
      for( li = char_list.begin(); li != char_list.end(); li++ )
      {
         vch = *li;
         if( IS_NPC( vch ) && vch->in_room &&
             vch->in_room->area == ch->in_room->area && vch->in_room != ch->in_room && number_range( 0, vcnt ) == 0 )
            victim = vch;
         vcnt++;
      }
   }
   /*
    * Max_level people can hunt through the world, and anyone who has
    * practiced over 70% can hunt through doors.. -- Alty 
    */
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
      ch->hunt_flags = HUNT_WORLD | HUNT_OPENDOOR | HUNT_UNLOCKDOOR | HUNT_PICKDOOR;
   else if( !IS_NPC( ch ) )
      ch->hunt_flags = ( get_trust( ch ) >= MAX_LEVEL ? HUNT_WORLD : 0 )
         | ( ch->pcdata->learned[gsn_hunt] >= 70 ? HUNT_OPENDOOR | HUNT_UNLOCKDOOR | HUNT_PICKDOOR : 0 );
   if( !victim || !set_hunt( ch, NULL, victim, NULL, 0, HUNT_CR | HUNT_MERC ) )
   {
      send_to_char( "You couldn't find a trail.\r\n", ch );
      return;
   }
   return;
}
