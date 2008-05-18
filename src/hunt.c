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

/* Mob/player hunting.. */
/* Copied this from one of my old MUDs..  Much cleaner than the sillymud
   hunt routines  -- Alty */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"

#define NEVER_FREE_HUNT

struct h_queue
{
   struct h_queue *next;
   ROOM_INDEX_DATA *room;
   sh_int dir;
};

struct h_queue *h_head = NULL;
struct h_queue *h_tail = NULL;
extern char *const dir_name[];

#ifdef NEVER_FREE_HUNT
struct h_queue *h_free = NULL;
void setup_hunt( void );
#endif

#ifdef DEBUG_HUNT_CODE
static FILE *h_fp;
#endif

void h_enqueue( ROOM_INDEX_DATA * room, sh_int dir )
{
   struct h_queue *hunt;

#ifdef NEVER_FREE_HUNT
   if( h_free )
   {
      hunt = h_free;
      h_free = h_free->next;
   }
   else
#endif
      hunt = getmem( sizeof( *hunt ) );
   hunt->next = NULL;
   hunt->room = room;
   hunt->dir = dir;
   SET_BIT( room->room_flags, ROOM_HUNT_MARK );
   if( !h_head )
      h_head = hunt;
   else
      h_tail->next = hunt;
   h_tail = hunt;
#ifdef DEBUG_HUNT_CODE
   fprintf( h_fp, "Enqueue: %5d - %d\n", room->vnum, IS_SET( room->room_flags, ROOM_HUNT_MARK ) );
   fflush( h_fp );
#endif
   return;
}

void h_dequeue( void )
{
   struct h_queue *hunt;

   if( !( hunt = h_head ) )
      return;
   h_head = hunt->next;
   if( h_tail == hunt )
      h_tail = NULL;
   REMOVE_BIT( hunt->room->room_flags, ROOM_HUNT_MARK );
#ifdef DEBUG_HUNT_CODE
   fprintf( h_fp, "Dequeue: %5d\n", hunt->room->vnum );
   fflush( h_fp );
#endif
#ifdef NEVER_FREE_HUNT
   hunt->next = h_free;
   h_free = hunt;
#else
   dispose( hunt, sizeof( *hunt ) );
#endif
   return;
}

void h_clear( void )
{
#ifdef DEBUG_HUNT_CODE
   fprintf( h_fp, "h_clear\n" );
   fflush( h_fp );
#endif
   while( h_head != NULL )
      h_dequeue(  );
#ifdef DEBUG_HUNT_CODE
   fprintf( h_fp, "Cleared\n" );
   fflush( h_fp );
#endif
}

bool h_is_valid_exit( ROOM_INDEX_DATA * room, sh_int dir, int h_flags )
{
   EXIT_DATA *exit = room->exit[dir];

   if( !exit )
      return FALSE;
   if( !exit->to_room )
      return FALSE;
#ifdef DEBUG_HUNT_CODE
   fprintf( h_fp, "IsValid: %5d - %s\n", exit->to_room->vnum,
            ( IS_SET( exit->to_room->room_flags, ROOM_HUNT_MARK ) ? "set" : "unset" ) );
   fflush( h_fp );
#endif
   if( IS_SET( exit->to_room->room_flags, ROOM_HUNT_MARK ) )
      return FALSE;
   if( !IS_SET( h_flags, HUNT_WORLD ) && room->area != exit->to_room->area )
      return FALSE;
   if( IS_SET( exit->exit_info, EX_CLOSED ) )
   {
      if( !IS_SET( h_flags, HUNT_OPENDOOR ) )
         return FALSE;
      if( IS_SET( exit->exit_info, EX_LOCKED ) )
      {
         if( !IS_SET( h_flags, HUNT_UNLOCKDOOR | HUNT_PICKDOOR ) )
            return FALSE;
         if( !IS_SET( h_flags, HUNT_UNLOCKDOOR ) && IS_SET( exit->exit_info, EX_PICKPROOF ) )
            return FALSE;
      }
   }
   return TRUE;
}

void h_enqueue_room( ROOM_INDEX_DATA * room, sh_int dir, int h_flags )
{
   sh_int edir;

#ifdef DEBUG_HUNT_CODE
   fprintf( h_fp, "h_enqueue_room\n" );
   fflush( h_fp );
#endif
   for( edir = 0; edir < 6; edir++ )
      if( h_is_valid_exit( room, edir, h_flags ) )
         h_enqueue( room->exit[edir]->to_room, ( dir == -1 ? edir : dir ) );
   return;
}

sh_int h_find_dir( ROOM_INDEX_DATA * room, ROOM_INDEX_DATA * target, int h_flags )
{
   struct h_queue *hunt;

   if( room == target )
      return -1;
#ifdef DEBUG_HUNT_CODE
   if( !h_fp )
      h_fp = fopen( "hunt.out", "w" );
   fprintf( h_fp, "h_find_dir\n" );
   fflush( h_fp );
#endif
#ifdef NEVER_FREE_HUNT
   if( !h_free && !h_head && !h_tail )
      setup_hunt(  );
#endif
   SET_BIT( room->room_flags, ROOM_HUNT_MARK );
   h_enqueue_room( room, -1, h_flags );
   for( hunt = h_head; hunt; hunt = hunt->next )
   {
      if( hunt->room == target )
      {
         sh_int dir = hunt->dir;

#ifdef DEBUG_HUNT_CODE
         fprintf( h_fp, "Found dir %d\n", dir );
         fflush( h_fp );
#endif
         h_clear(  );
         REMOVE_BIT( room->room_flags, ROOM_HUNT_MARK );
         return dir;
      }
      h_enqueue_room( hunt->room, hunt->dir, h_flags );
   }
#ifdef DEBUG_HUNT_CODE
   fprintf( h_fp, "Invalid dir\n" );
   fflush( h_fp );
#endif
   h_clear(  );
   REMOVE_BIT( room->room_flags, ROOM_HUNT_MARK );
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
   ch->hunt_for = fch;
   if( IS_NPC( ch ) )
      ch->hunt_home = ( ch->hunt_home ? ch->hunt_home : ch->in_room );
   else
      ch->hunt_home = NULL;
   if( ch->searching )
   {
      free_string( ch->searching );
      ch->searching = NULL;
   }
   ch->hunt_flags = nflags;
   sprintf( buf, "%s has started hunting (%s) %s",
            NAME( ch ),
            ( vch ? IS_NPC( vch ) ? "mobile" : "player" : "object" ), ( vch ? NAME( vch ) : vobj->short_descr ) );
   if( fch )
      sprintf( buf + strlen( buf ), " for %s", NAME( fch ) );
   monitor_chan( buf, MONITOR_HUNTING );
/*  bug(buf, 0);  */
   return TRUE;
}

void end_hunt( CHAR_DATA * ch )
{
   ch->hunting = NULL;
   ch->hunt_obj = NULL;
   ch->hunt_for = NULL;
   if( !IS_NPC( ch ) )
   {
      ch->hunt_home = NULL;
      ch->hunt_flags = 0;
   }
   else
      ch->hunt_flags = ch->pIndexData->hunt_flags;
   if( ch->searching )
   {
      free_string( ch->searching );
      ch->searching = NULL;
   }
}

bool has_key args( ( CHAR_DATA * ch, int key ) );
void hunt_move( CHAR_DATA * mob, sh_int dir )
{
   EXIT_DATA *exit = mob->in_room->exit[dir];

   if( IS_SET( exit->exit_info, EX_CLOSED ) )
   {
      if( IS_SET( exit->exit_info, EX_LOCKED ) )
      {
         if( IS_SET( mob->hunt_flags, HUNT_UNLOCKDOOR ) && has_key( mob, exit->key ) )
            do_unlock( mob, dir_name[dir] );
         else if( IS_SET( mob->hunt_flags, HUNT_PICKDOOR ) && !IS_SET( exit->exit_info, EX_PICKPROOF ) )
            do_pick( mob, dir_name[dir] );
      }
      else if( IS_SET( mob->hunt_flags, HUNT_OPENDOOR ) )
         do_open( mob, dir_name[dir] );
   }
   else
      move_char( mob, dir );
   return;
}

/*#define NAME(ch) (IS_NPC(ch) ? ch->short_descr : ch->name)*/
bool mob_hunt( CHAR_DATA * mob )
{
   sh_int dir;
   char buf[128];

   if( !mob || !IS_NPC( mob ) )
      return FALSE;
   if( !mob->hunting )
   {
      if( mob->hunt_obj != NULL )
      {
         if( mob->hunt_obj->in_room == NULL )
         {
            if( IS_SET( mob->hunt_flags, HUNT_CR ) && mob->hunt_for && mob->hunt_obj->item_type == ITEM_CORPSE_PC )
               act( "$N tells you 'Someone else seems to have gotten to your "
                    "corpse before me.'", mob->hunt_for, NULL, mob, TO_CHAR );
            end_hunt( mob );
            return TRUE;
         }
         if( can_see_obj( mob, mob->hunt_obj ) && mob->in_room == mob->hunt_obj->in_room )
         {
            if( IS_SET( mob->hunt_flags, HUNT_CR ) && mob->hunt_for && mob->hunt_obj->item_type == ITEM_CORPSE_PC )
               act( "$N tell you 'I have found your corpse.  I shall return it "
                    "to you now.", mob->hunt_for, NULL, mob, TO_CHAR );
            obj_from_room( mob->hunt_obj );
            obj_to_char( mob->hunt_obj, mob );
            act( "$n gets $o.", mob, mob->hunt_obj, NULL, TO_ROOM );
            set_hunt( mob, NULL, mob->hunt_for, mob->hunt_obj, 0, 0 );
            return TRUE;
         }
         if( !can_see_obj( mob, mob->hunt_obj ) ||
             ( dir = h_find_dir( mob->in_room, mob->hunt_obj->in_room, mob->hunt_flags ) ) < 0 )
         {
            if( IS_SET( mob->hunt_flags, HUNT_CR ) && mob->hunt_for && mob->hunt_obj->item_type == ITEM_CORPSE_PC )
               act( "$N tells you 'I seem to have lost the way to your corpse.'", mob->hunt_for, NULL, mob, TO_CHAR );
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
      else if( mob->hunt_home )
      {
         dir = -1;
         if( mob->in_room == mob->hunt_home || ( dir = h_find_dir( mob->in_room, mob->hunt_home, mob->hunt_flags ) ) < 0 )
         {
            mob->hunt_home = NULL;
            mob->hunt_flags = mob->pIndexData->hunt_flags;
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
      if( IS_SET( mob->hunt_flags, HUNT_MERC ) && mob->hunt_for )
      {
         /*
          * 6.25% chance of giving up, 18.75% chance of telling employer. 
          */
         switch ( number_bits( 4 ) )
         {
            case 0:
               sprintf( buf, "$N tells you '%s seems to have disappeared!'", NAME( mob->hunting ) );
               act( buf, mob->hunt_for, NULL, mob, TO_CHAR );
               end_hunt( mob );
               return TRUE;
            case 1:
            case 2:
            case 3:
               sprintf( buf, "$N tells you '%s seems to have disappeared!  I shall "
                        "find %s though!'", NAME( mob->hunting ),
                        ( mob->hunting->sex == SEX_MALE ? "him" : mob->hunting->sex == SEX_FEMALE ? "her" : "it" ) );
               act( buf, mob->hunt_for, NULL, mob, TO_CHAR );
               return TRUE;
         }
      }
      if( IS_SET( mob->hunt_flags, HUNT_INFORM ) )
      {
         switch ( number_bits( 5 ) )
         {
            case 0:
               sprintf( buf, "Where are you, %s?", NAME( mob->hunting ) );
               break;
            case 1:
               sprintf( buf, "Why can't I find you, %s?", NAME( mob->hunting ) );
               break;
            case 2:
               sprintf( buf, "I know you're out there, %s!", NAME( mob->hunting ) );
               break;
            case 3:
               sprintf( buf, "I'll find you, %s, just wait!", NAME( mob->hunting ) );
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
      if( IS_SET( mob->hunt_flags, HUNT_MERC ) && mob->hunt_for )
      {
         sprintf( buf, "$N tells you 'I have found %s!  Now %s shall die!'",
                  NAME( mob->hunting ),
                  ( mob->hunting->sex == SEX_FEMALE ? "she" : mob->hunting->sex == SEX_MALE ? "he" : "it" ) );
         act( buf, mob->hunt_for, NULL, mob, TO_CHAR );
      }
      switch ( number_bits( 2 ) )
      {
         case 0:
            sprintf( buf, "Now I have you, %s!", NAME( mob->hunting ) );
            break;
         case 1:
            sprintf( buf, "I knew you'd be here, %s!", NAME( mob->hunting ) );
            break;
         case 2:
            sprintf( buf, "Did you really think you were safe, %s?", NAME( mob->hunting ) );
            break;
         case 3:
            sprintf( buf, "So here you are, %s!", NAME( mob->hunting ) );
            break;
      }
      if( IS_SET( mob->hunt_flags, HUNT_INFORM ) )
         do_yell( mob, buf );
      else
         do_say( mob, buf );
      multi_hit( mob, mob->hunting, TYPE_UNDEFINED );
      end_hunt( mob );
      return TRUE;
   }

   if( ( dir = h_find_dir( mob->in_room, mob->hunting->in_room, mob->hunt_flags ) ) == -1 )
   {
      if( IS_SET( mob->hunt_flags, HUNT_MERC ) && mob->hunt_for )
      {
         /*
          * 6.25% chance of giving up, 18.75% chance of informing employer 
          */
         switch ( number_bits( 4 ) )
         {
            case 0:
               sprintf( buf, "$N tells you 'I seem to have lost %s's trail.'", NAME( mob->hunting ) );
               act( buf, mob->hunt_for, NULL, mob, TO_CHAR );
               end_hunt( mob );
               return TRUE;
            case 1:
            case 2:
            case 3:
               sprintf( buf, "$N tells you 'I seem to have lost %s's trail.  I shall "
                        "find it again, though!'", NAME( mob->hunting ) );
               act( buf, mob->hunt_for, NULL, mob, TO_CHAR );
               return TRUE;
         }
      }
      if( IS_SET( mob->hunt_flags, HUNT_INFORM ) )
      {
         switch ( number_bits( 6 ) )
         {
            case 0:
               sprintf( buf, "Where are you hiding, %s?", NAME( mob->hunting ) );
               break;
            case 1:
               sprintf( buf, "You can't run forever, %s!", NAME( mob->hunting ) );
               break;
            case 2:
               sprintf( buf, "Come out, come out, wherever you are, %s!", NAME( mob->hunting ) );
               break;
            case 3:
               sprintf( buf, "I promise I won't hurt you, %s.", NAME( mob->hunting ) );
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

/*#undef NAME*/

void char_hunt( CHAR_DATA * ch )
{
   sh_int dir;
   char buf[MAX_STRING_LENGTH];

   if( IS_NPC( ch ) )
      return;
   if( !ch->hunting )
   {
      if( ch->hunt_obj )
      {
         if( !can_see_obj( ch, ch->hunt_obj ) || !ch->hunt_obj->in_room )
         {
            sprintf( buf, "@@RYou seem to have lost the trail to %s.@@N\n\r", ch->hunt_obj->short_descr );
            send_to_char( buf, ch );
            end_hunt( ch );
         }
         else if( ch->hunt_obj->in_room == ch->in_room )
         {
            sprintf( buf, "@@RAhhh.  You have found %s!@@N\n\r", ch->hunt_obj->short_descr );
            send_to_char( buf, ch );
            end_hunt( ch );
         }
         else if( ( dir = h_find_dir( ch->in_room, ch->hunt_obj->in_room, ch->hunt_flags ) ) < 0 )
         {
            sprintf( buf, "@@RYou seem to have lost the trail to %s.@@N\n\r", ch->hunt_obj->short_descr );
            send_to_char( buf, ch );
            end_hunt( ch );
         }
         else
         {
            sprintf( buf, "@@RYou sense that %s is %s of here.@@N\n\r", ch->hunt_obj->short_descr, dir_name[dir] );
            send_to_char( buf, ch );
         }
      }
      return;
   }
   if( !can_see( ch, ch->hunting ) )
   {
      send_to_char( "@@RYou seem to have lost your prey.@@N\n\r", ch );
      end_hunt( ch );
   }
   else if( ch->in_room == ch->hunting->in_room )
   {
      send_to_char( "@@RAhhh.  You have found your prey!@@N\n\r", ch );
      end_hunt( ch );
   }
   else if( ( dir = h_find_dir( ch->in_room, ch->hunting->in_room, ch->hunt_flags ) ) < 0 )
   {
      send_to_char( "@@RYou seem to have lost your prey.@@N\n\r", ch );
      end_hunt( ch );
   }
   else
   {
      sprintf( buf, "@@RYou sense your prey is %s of here.@@N\n\r", dir_name[dir] );
      send_to_char( buf, ch );
   }
   return;
}

void do_hunt( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   sh_int chance;

   if( IS_NPC( ch ) && ( ch->hunting || ch->hunt_obj ) )
      return;
   argument = one_argument( argument, arg );
   if( !*arg )
   {
      send_to_char( "Hunt for whom?\n\r", ch );
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
         sprintf( arg, "You stop hunting %s.\n\r", NAME( ch->hunting ) );
         send_to_char( arg, ch );
      }
      else if( ch->hunt_obj )
      {
         sprintf( arg, "You stop looking for %s.\n\r", ch->hunt_obj->short_descr );
         send_to_char( arg, ch );
      }
      else
      {
         send_to_char( "You find yourself right where you're standing!\n\r", ch );
      }
      end_hunt( ch );
      return;
   }
   else if( !IS_IMMORTAL( ch ) && ( victim != NULL ) && !IS_NPC( victim ) && IS_IMMORTAL( victim ) )
   {
      sprintf( arg, "You can't hunt Immortal %s!\n\r", NAME( victim ) );
      send_to_char( arg, ch );
      return;
   }

   /*
    * 100% chance of finding them if you're looking right at them.. :) 
    */
   if( victim != NULL && victim->in_room == ch->in_room )
   {
      send_to_char( "You're already there!\n\r", ch );
      return;
   }
   /*
    * Chance of picking up wrong trail.. (25% for NPC, PC @ normal) 
    */
   chance = ( IS_NPC( ch ) ? 75 : ch->pcdata->learned[gsn_hunt] );
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
      chance = ( ( MAX_WOLF_LEVEL - ch->pcdata->generation ) * 4 ) + ch->pcdata->vamp_level;

   if( chance < number_percent(  ) )
   {
      CHAR_DATA *vch;
      sh_int vcnt = 0;

      victim = NULL;
      for( vch = first_char; vch; vch = vch->next )
      {
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
      send_to_char( "You couldn't find a trail.\n\r", ch );
      return;
   }
   return;
}

#ifdef NEVER_FREE_HUNT
#define MAX_BUCKET_SIZE	4096
#define TOP_BUCKET_LIST	4095
void setup_hunt( void )
{
   struct h_queue *bucket;
   int bcnt;

   bucket = getmem( MAX_BUCKET_SIZE * sizeof( *bucket ) );
   for( bcnt = 0; bcnt < MAX_BUCKET_SIZE; bcnt++ )
      bucket[bcnt].next = ( bcnt < TOP_BUCKET_LIST ? &bucket[bcnt + 1] : h_free );
   h_free = &bucket[0];
   return;
}

#undef TOP_BUCKET_SIZE
#undef MAX_BUCKET_SIZE
#endif
