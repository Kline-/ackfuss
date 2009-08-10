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

#ifndef DEC_BUILD_H
#include "h/build.h"
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

#ifndef DEC_MOUNT_H
#include "h/mount.h"
#endif

#ifndef DEC_RULERS_H
#include "h/rulers.h"
#endif

#ifndef DEC_UPDATE_H
#include "h/update.h"
#endif

extern bool deathmatch;
extern const short rev_dir[];
extern const short movement_loss[];

void move_char( CHAR_DATA * ch, int door, bool look )
{
   CHAR_DATA *fch;
   CHAR_DATA *fch_next;
   ROOM_INDEX_DATA *in_room;
   ROOM_INDEX_DATA *to_room;
   EXIT_DATA *pexit;
   char buf[MSL];
   char move_buf[MSL];
   char tmp[MSL];
   int need_movement;
   char door_name_leave[MSL];
   char door_name_enter[MSL];

   buf[0] = '\0';
   move_buf[0] = '\0';
   door_name_leave[0] = '\0';
   door_name_leave[0] = '\0';
   tmp[0] = '\0';

   if( IS_CASTING(ch) )
    do_stop(ch,"");
   check_autodig(ch,door);
   if( !IS_NPC(ch) && ch->pcdata->movement >= MAX_MOVE_DISPLAY && ch->act.test(ACT_AUTOBRIEF) )
    look = FALSE;

   if( door < 0 || door > MAX_DIR )
   {
      bug( "Do_move: bad door %d.", door );
      ch->using_named_door = FALSE;
      return;
   }

   if( !IS_NPC( ch ) && ( ( ch->stance == STANCE_AMBUSH ) || ( ch->stance == STANCE_AC_BEST ) ) )
   {
      send_to_char( "You step out of the shadows.\r\n", ch );
      ch->stance = STANCE_WARRIOR;
      ch->stance_ac_mod = 0;
      ch->stance_dr_mod = 0;
      ch->stance_hr_mod = 0;
      act( "$n steps out of the Shadows!", ch, NULL, NULL, TO_ROOM );
   }
   if( IS_NPC( ch ) )
      ch->using_named_door = TRUE;
   in_room = ch->in_room;
   if( ( pexit = in_room->exit[door] ) == NULL
       || ( to_room = pexit->to_room ) == NULL
       || ( ( str_cmp( pexit->keyword, "" ) ) && ( ch->using_named_door == FALSE ) ) )
   {
      send_to_char( "Alas, you cannot go that way.\r\n", ch );
      ch->using_named_door = FALSE;
      return;
   }

   if( IS_NPC(ch) && pexit->exit_info.test(EX_NOMOB) )
    return;

   if( str_cmp( pexit->keyword, "" ) )
   {
      if( pexit->keyword[0] == '^' )
      {
         snprintf( door_name_leave, MSL, "through the %s", pexit->keyword + 1 );
         snprintf( door_name_enter, MSL, "the %s", pexit->keyword + 1 );
      }
      else
      {
         snprintf( door_name_leave, MSL, "through the %s", pexit->keyword );
         snprintf( door_name_enter, MSL, "the %s", pexit->keyword );
      }

   }
   else
   {
      snprintf( door_name_leave, MSL, "%s", dir_name[door] );
      snprintf( door_name_enter, MSL, "%s", rev_name[door] );
   }

   if( IS_SET( in_room->affected_by, ROOM_BV_ENCAPS ) || IS_SET( to_room->affected_by, ROOM_BV_ENCAPS ) )
   {
      if( AI_MOB(ch) )
      {
         if( ch->mana > mana_cost( ch, skill_lookup( "room dispel" ) ) )
            do_cast( ch, "room dispel" );
      }
      else if( !IS_GHOST(ch) )
      {
         send_to_char( "A barely visible energy web stops your movement!\r\n", ch );
         ch->using_named_door = FALSE;
         return;
      }
   }
   if( IS_SET( in_room->affected_by, ROOM_BV_HOLD ) )
   {
      if( AI_MOB(ch) )
      {
         if( ch->mana > mana_cost( ch, skill_lookup( "room dispel" ) ) )
            do_cast( ch, "room dispel" );
      }
      else if( !IS_GHOST(ch) )
      {
         send_to_char( "A fleeting vision of bars appears before the exit, and stops your movement!\r\n", ch );
         ch->using_named_door = FALSE;
         return;
      }
   }

   if( pexit->exit_info.test(EX_CLOSED) )
   {
      if( !IS_AFFECTED( ch, AFF_PASS_DOOR ) && ( !item_has_apply( ch, ITEM_APPLY_PASS_DOOR ) ) && !IS_GHOST(ch) )
      {
         if( pexit->exit_info.test(EX_NODETECT) )
            send_to_char( "Alas, you cannot go that way.\r\n", ch );
         else
            act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
         ch->using_named_door = FALSE;
         return;
      }
      else if( pexit->exit_info.test(EX_PASSPROOF) )
      {
         if( pexit->exit_info.test(EX_NODETECT) )
            send_to_char( "Alas, you cannot go that way.\r\n", ch );
         else
            act( "You don't seem to be able to pass through $d.", ch, NULL, pexit->keyword, TO_CHAR );
         ch->using_named_door = FALSE;
         return;
      }
   }

   if( IS_AFFECTED( ch, AFF_CHARM ) && ch->master != NULL && in_room == ch->master->in_room )
   {
      send_to_char( "What?  And leave your beloved master?\r\n", ch );
      ch->using_named_door = FALSE;
      return;
   }

   if( room_is_private( to_room ) && !IS_IMMORTAL( ch ) )
   {
      send_to_char( "That room is private right now.\r\n", ch );
      ch->using_named_door = FALSE;
      return;
   }
   /*
    * need 1 move if riding - Celestian 
    */
   if( ch->position == POS_RIDING )
   {
      need_movement = 1;
      if( IS_RIDING( ch ) )
      {
         if( !IS_AWAKE( ch->riding ) )
         {
            send_to_char( "Your mount ignores you completely.\r\n", ch );
            ch->using_named_door = FALSE;
            return;
         }
         if( ( ch->riding )->position != POS_STANDING )
         {
            act( "$N twists $S head around and looks at you quizzically.", ch, 0, ch->riding, TO_CHAR );
            act( "$N looks quizzically at $n.", ch, 0, ch->riding, TO_CHAR );
            ch->using_named_door = FALSE;
            return;
         }
      }
      else
      {
         send_to_char( "Bummer, looks like your mount gave you the slip.\r\n", ch );
         ch->position = POS_STANDING;
         update_pos( ch );
         ch->using_named_door = FALSE;
         return;
      }
   }


   if( pexit->exit_info.test(EX_IMMORTAL) && !IS_IMMORTAL( ch ) )
   {
      send_to_char( "Only an Immortal may use that exit.\r\n", ch );
      ch->using_named_door = FALSE;
      return;
   }


   /*
    * Int Mobs need to be able to handle climbs  
    */
   if( pexit->exit_info.test(EX_CLIMB) && IS_NPC( ch ) && !IS_AFFECTED( ch, AFF_FLYING ) )
   {
      if( ch->mana > mana_cost( ch, skill_lookup( "fly" ) ) )
      {
         do_cast( ch, "fly" );
      }
      else
      {
         send_to_char( "Looks like you'd have to climb up there.\r\n", ch );
         ch->using_named_door = FALSE;
         return;
      }
   }

   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_climb] == 0
       && pexit->exit_info.test(EX_CLIMB) && !IS_AFFECTED( ch, AFF_FLYING ) && !item_has_apply( ch, ITEM_APPLY_FLY ) && !IS_GHOST(ch) )
   {
      send_to_char( "You'd need to be able to fly or climb to go that way!\r\n", ch );
      ch->using_named_door = FALSE;
      return;
   }


   {
      int iClass;
      int iRace;
      int iClan;
      int move;

      /*
       * Check if room is a guild-only one... 
       */

      for( iClass = 0; iClass < MAX_CLASS; iClass++ )
      {
         if( to_room->vnum == class_table[iClass].guild )
         {
            if( ( IS_NPC( ch ) ) || ( ( !IS_IMMORTAL( ch ) ) && ( ch->lvl[iClass] == -1 ) ) )
            {
               send_to_char( "You aren't allowed in there.\r\n", ch );
               ch->using_named_door = FALSE;
               return;
            }
         }
      }

      /*
       * Check if room is a race-only one... 
       */

      for( iRace = 0; iRace < MAX_RACE; iRace++ )
      {
         if( ( to_room->vnum == race_table[iRace].race_room ) && ( iRace != ch->race ) && ( !IS_IMMORTAL( ch ) ) )
         {
            send_to_char( "You aren't allowed in there.\r\n", ch );
            ch->using_named_door = FALSE;
            return;
         }
      }

      /*
       * Check if room is a clan-only one... 
       */

      for( iClan = 0; iClan < MAX_CLAN; iClan++ )
      {
         if( to_room->vnum == clan_table[iClan].clan_room )
         {
            if( ( IS_NPC( ch ) ) || ( ( !IS_IMMORTAL( ch ) ) && ( iClan != ch->clan ) ) )
            {
               send_to_char( "You aren't allowed in there.\r\n", ch );
               ch->using_named_door = FALSE;
               return;
            }
         }
      }

      if( to_room->vnum == ROOM_VNUM_BUILDER && ( !IS_IMMORTAL( ch ) && !ch->act.test(ACT_BUILDER) ) )
      {
         send_to_char( "The Portal allows entrance to builders only.\r\n", ch );
         ch->using_named_door = FALSE;
         return;
      }


      if( to_room->vnum == ROOM_VNUM_CLAN
          && ( IS_NPC( ch ) || ( ( !IS_IMMORTAL( ch ) ) && ( !ch->act.test(ACT_CBOSS) ) ) ) )
      {
         send_to_char( "Only Clan Bosses may enter this room.\r\n", ch );
         ch->using_named_door = FALSE;
         return;
      }



      if( in_room->sector_type == SECT_AIR || to_room->sector_type == SECT_AIR )
      {
         if( ( ( !IS_AFFECTED( ch, AFF_FLYING ) )
               && ( !item_has_apply( ch, ITEM_APPLY_FLY ) ) )
             && ( ( ch->riding != NULL ) && ( !IS_SET( ch->riding->affected_by, AFF_FLYING ) ) ) && !IS_GHOST(ch) )
         {

            if( AI_MOB(ch) && ( ch->mana > mana_cost( ch, skill_lookup( "fly" ) ) ) )
            {
               do_cast( ch, "fly" );
            }
            else
            {
               send_to_char( "You can't fly.\r\n", ch );
               ch->using_named_door = FALSE;
               return;
            }
         }
      }

      if( in_room->sector_type == SECT_WATER_NOSWIM || to_room->sector_type == SECT_WATER_NOSWIM )
      {
         OBJ_DATA *obj;
         bool found;

         /*
          * Look for a boat.
          */
         found = FALSE;

         /*
          * Suggestion for flying above water by Sludge
          */
         if( ( IS_AFFECTED( ch, AFF_FLYING ) ) || ( item_has_apply( ch, ITEM_APPLY_FLY ) ) || IS_GHOST(ch) )
            found = TRUE;
         if( ( ch->riding != NULL ) && ( IS_SET( ch->riding->affected_by, AFF_FLYING ) ) )
            found = TRUE;
         for( obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list )
         {
            if( obj->item_type == ITEM_BOAT )
            {
               found = TRUE;
               break;
            }
         }
         if( !found )
         {
            if( AI_MOB(ch) && ( ch->mana > mana_cost( ch, skill_lookup( "fly" ) ) ) )
            {
               do_cast( ch, "fly" );
            }
            else if( !IS_IMMORTAL( ch ) )
            {
               send_to_char( "You need a boat to go there.\r\n", ch );
               ch->using_named_door = FALSE;
               return;
            }
         }
      }

      move = movement_loss[UMIN( SECT_MAX - 1, in_room->sector_type )]
         + movement_loss[UMIN( SECT_MAX - 1, to_room->sector_type )];

      if( IS_AFFECTED( ch, AFF_FLYING ) || item_has_apply( ch, ITEM_APPLY_FLY ) )
         move = 1;
      if( IS_GHOST(ch) )
         move = 0;

      if( ch->move < move )
      {
         send_to_char( "You are too exhausted.\r\n", ch );
         ch->using_named_door = FALSE;
         return;
      }

      WAIT_STATE( ch, 25 );
      ch->move -= move;
   }
/* Mount code - Celestian */
   if( ch->position == POS_RIDING )
   {
      if( check_valid_ride( ch ) )
      {
         snprintf( tmp, MSL, "You ride %s on %s.\r\n", door_name_leave, ch->riding->get_name() );
         send_to_char( tmp, ch );
         snprintf( tmp, MSL, "$n rides %s on %s.", door_name_leave, ch->riding->get_name() );
      }
   }

/*    if ( ( !IS_AFFECTED(ch, AFF_SNEAK) && !item_has_apply( ch, ITEM_APPLY_SNEAK ) )
    && ( IS_NPC(ch) || ch->act.test(PLR_WIZINVIS) ) )  */
   {
      if( pexit->exit_info.test(EX_CLIMB) )   /* dir_name[door] */
         act( "$n climbs $T.", ch, NULL, door_name_leave, TO_ROOM );
      else if( pexit->exit_info.test(EX_CLOSED) )   /* using passdoor */
      {
         snprintf( buf, MSL, "$L$n floats %s.", door_name_leave );
         act( buf, ch, NULL, pexit->keyword, TO_ROOM );
      }
      else
      {
         if( IS_NPC( ch ) || ( !IS_NPC( ch ) && ( IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) ) ) )
            snprintf( move_buf, MSL, "$L$n %s $T.", "wanders" );
         else if( ch->riding == NULL )
         {
            snprintf( move_buf, MSL, "$L%s$n %s $T.",
                     get_ruler_title( ch->pcdata->ruler_rank, ch->pcdata->login_sex ), ch->pcdata->room_exit );
         }
         else if( ch->riding != NULL )
         {
            snprintf( move_buf, MSL, "$L%s$n rides $T on %s.",
                     get_ruler_title( ch->pcdata->ruler_rank, ch->pcdata->login_sex ), ch->riding->get_name() );
         }
         act( move_buf, ch, NULL, door_name_leave, TO_ROOM );
      }
   }
   char_from_room( ch );
   char_to_room( ch, to_room );
/* Mount code - Celestian */
   if( IS_RIDING( ch ) )
   {
      char_from_room( ch->riding );
      char_to_room( ch->riding, to_room );
      if( ( ch->riding )->in_room->vnum == NOWHERE )
      {
         ch->using_named_door = FALSE;
         return;
      }
   }

   /*
    * if ( ( !IS_AFFECTED(ch, AFF_SNEAK) && !item_has_apply( ch, ITEM_APPLY_SNEAK ) )
    * && ( IS_NPC(ch) || !ch->act.test(PLR_WIZINVIS) ) )
    */
   {
      if( IS_NPC( ch ) || ( !IS_NPC( ch ) && ( IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) ) ) )
         snprintf( move_buf, MSL, "$L$n %s $T.", "wanders in from" );
      else if( ch->riding == NULL )
      {
         snprintf( move_buf, MSL, "$L%s$n %s $T.",
                  get_ruler_title( ch->pcdata->ruler_rank, ch->pcdata->login_sex ), ch->pcdata->room_enter );
      }
      else if( ch->riding != NULL )
      {
         snprintf( move_buf, MSL, "$L%s$n rides in from $T on %s.",
                  get_ruler_title( ch->pcdata->ruler_rank, ch->pcdata->login_sex ), ch->riding->get_name() );
      }
      act( move_buf, ch, NULL, door_name_enter, TO_ROOM );

   }


   if( in_room != to_room )
   {
      for( fch = in_room->first_person; fch != NULL; fch = fch_next )
      {
         fch_next = fch->next_in_room;
         if( fch->master == ch && fch->position == POS_STANDING )
         {
            act( "You follow $N $t.", fch, dir_name[door], ch, TO_CHAR );
            fch->using_named_door = TRUE;
            move_char( fch, door, TRUE );
         }
      }
   }
   if( look )
    do_look( ch, "auto" );
   else
   {
    snprintf( buf, MSL, "%s%s%s\r\n", color_string( ch, "rooms" ), ch->in_room->name, color_string( ch, "normal" ) );
    send_to_char( buf, ch );
    if( is_switched(ch) || ( !IS_NPC( ch ) && ch->act.test(ACT_AUTOEXIT) ) )
     do_exits( ch, "autonr" );
   }

   if( !IS_NPC( ch ) && IS_VAMP( ch ) && !IS_IMMORTAL( ch ) )
      check_vamp( ch ); /* burn the vampire! */
   ch->using_named_door = FALSE;

   for( fch = ch->in_room->first_person; fch != NULL; fch = fch->next_in_room )
    if( IS_NPC(fch) && fch->act.test(ACT_REMEMBER) && fch->target.find(ch->name) != string::npos )
     remember_attack(fch,ch);

   return;
}



DO_FUN(do_north)
{
   move_char( ch, DIR_NORTH, TRUE );
   return;
}



DO_FUN(do_east)
{
   move_char( ch, DIR_EAST, TRUE );
   return;
}



DO_FUN(do_south)
{
   move_char( ch, DIR_SOUTH, TRUE );
   return;
}



DO_FUN(do_west)
{
   move_char( ch, DIR_WEST, TRUE );
   return;
}



DO_FUN(do_up)
{
   move_char( ch, DIR_UP, TRUE );
   return;
}



DO_FUN(do_down)
{
   move_char( ch, DIR_DOWN, TRUE );
   return;
}



int find_door( CHAR_DATA * ch, char *arg )
{
   EXIT_DATA *pexit;
   int door;

   if( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) )
      door = 0;
   else if( !str_cmp( arg, "e" ) || !str_cmp( arg, "east" ) )
      door = 1;
   else if( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) )
      door = 2;
   else if( !str_cmp( arg, "w" ) || !str_cmp( arg, "west" ) )
      door = 3;
   else if( !str_cmp( arg, "u" ) || !str_cmp( arg, "up" ) )
      door = 4;
   else if( !str_cmp( arg, "d" ) || !str_cmp( arg, "down" ) )
      door = 5;
   else
   {
      for( door = 0; door < MAX_DIR; door++ )
      {
         if( ( pexit = ch->in_room->exit[door] ) != NULL
             && pexit->exit_info.test(EX_ISDOOR) && pexit->keyword != NULL && is_name( arg, pexit->keyword ) )
            return door;
      }
      act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
      return -1;
   }

   if( ( ( pexit = ch->in_room->exit[door] ) == NULL )
       || ( str_cmp( pexit->keyword, "" ) && !is_name( arg, pexit->keyword ) ) )
   {
      act( "I see no door $T here.", ch, NULL, arg, TO_CHAR );
      return -1;
   }

   if( !pexit->exit_info.test(EX_ISDOOR) )
   {
      send_to_char( "You can't do that.\r\n", ch );
      return -1;
   }

   return door;
}



DO_FUN(do_open)
{
   char arg[MSL];
   OBJ_DATA *obj;
   int door;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Open what?\r\n", ch );
      return;
   }

   if( ( obj = get_obj_here( ch, arg ) ) != NULL )
   {
      /*
       * 'open object' 
       */
      if( obj->item_type != ITEM_CONTAINER )
      {
         send_to_char( "That's not a container.\r\n", ch );
         return;
      }
      if( !IS_SET( obj->value[1], CONT_CLOSED ) )
      {
         send_to_char( "It's already open.\r\n", ch );
         return;
      }
      if( !IS_SET( obj->value[1], CONT_CLOSEABLE ) )
      {
         send_to_char( "You can't do that.\r\n", ch );
         return;
      }
      if( IS_SET( obj->value[1], CONT_LOCKED ) )
      {
         send_to_char( "It's locked.\r\n", ch );
         return;
      }

      REMOVE_BIT( obj->value[1], CONT_CLOSED );
      send_to_char( "Ok.\r\n", ch );
      act( "$n opens $p.", ch, obj, NULL, TO_ROOM );
      return;
   }

   if( ( door = find_door( ch, arg ) ) >= 0 )
   {
      /*
       * 'open door' 
       */
      ROOM_INDEX_DATA *to_room;
      EXIT_DATA *pexit;
      EXIT_DATA *pexit_rev;

      pexit = ch->in_room->exit[door];
      if( !pexit->exit_info.test(EX_CLOSED) )
      {
         send_to_char( "It's already open.\r\n", ch );
         return;
      }
      if( pexit->exit_info.test(EX_LOCKED) )
      {
         send_to_char( "It's locked.\r\n", ch );
         return;
      }

      pexit->exit_info.reset(EX_CLOSED);
      act( "$n opens the $d.", ch, NULL, pexit->keyword, TO_ROOM );
      send_to_char( "Ok.\r\n", ch );

      /*
       * open the other side 
       */
      if( ( to_room = pexit->to_room ) != NULL
          && ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL && pexit_rev->to_room == ch->in_room )
      {
         CHAR_DATA *rch;

         pexit_rev->exit_info.reset(EX_CLOSED);
         for( rch = to_room->first_person; rch != NULL; rch = rch->next_in_room )
            act( "The $d opens.", rch, NULL, pexit_rev->keyword, TO_CHAR );
      }
   }

   return;
}



DO_FUN(do_close)
{
   char arg[MSL];
   OBJ_DATA *obj;
   int door;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Close what?\r\n", ch );
      return;
   }

   if( ( obj = get_obj_here( ch, arg ) ) != NULL )
   {
      /*
       * 'close object' 
       */
      if( obj->item_type != ITEM_CONTAINER )
      {
         send_to_char( "That's not a container.\r\n", ch );
         return;
      }
      if( IS_SET( obj->value[1], CONT_CLOSED ) )
      {
         send_to_char( "It's already closed.\r\n", ch );
         return;
      }
      if( !IS_SET( obj->value[1], CONT_CLOSEABLE ) )
      {
         send_to_char( "You can't do that.\r\n", ch );
         return;
      }

      SET_BIT( obj->value[1], CONT_CLOSED );
      send_to_char( "Ok.\r\n", ch );
      act( "$n closes $p.", ch, obj, NULL, TO_ROOM );
      return;
   }

   if( ( door = find_door( ch, arg ) ) >= 0 )
   {
      /*
       * 'close door' 
       */
      ROOM_INDEX_DATA *to_room;
      EXIT_DATA *pexit;
      EXIT_DATA *pexit_rev;

      pexit = ch->in_room->exit[door];
      if( pexit->exit_info.test(EX_CLOSED) )
      {
         send_to_char( "It's already closed.\r\n", ch );
         return;
      }

      pexit->exit_info.set(EX_CLOSED);
      act( "$n closes the $d.", ch, NULL, pexit->keyword, TO_ROOM );
      send_to_char( "Ok.\r\n", ch );

      /*
       * close the other side 
       */
      if( ( to_room = pexit->to_room ) != NULL
          && ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0 && pexit_rev->to_room == ch->in_room )
      {
         CHAR_DATA *rch;

         pexit_rev->exit_info.set(EX_CLOSED);
         for( rch = to_room->first_person; rch != NULL; rch = rch->next_in_room )
            act( "The $d closes.", rch, NULL, pexit_rev->keyword, TO_CHAR );
      }
   }

   return;
}



bool has_key( CHAR_DATA * ch, int key )
{
   OBJ_DATA *obj;

   for( obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list )
   {
      if( obj->pIndexData->vnum == key )
         return TRUE;
   }

   return FALSE;
}



DO_FUN(do_lock)
{
   char arg[MSL];
   OBJ_DATA *obj;
   int door;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Lock what?\r\n", ch );
      return;
   }

   if( ( obj = get_obj_here( ch, arg ) ) != NULL )
   {
      /*
       * 'lock object' 
       */
      if( obj->item_type != ITEM_CONTAINER )
      {
         send_to_char( "That's not a container.\r\n", ch );
         return;
      }
      if( !IS_SET( obj->value[1], CONT_CLOSED ) )
      {
         send_to_char( "It's not closed.\r\n", ch );
         return;
      }
      if( obj->value[2] < 0 )
      {
         send_to_char( "It can't be locked.\r\n", ch );
         return;
      }
      if( !has_key( ch, obj->value[2] ) )
      {
         send_to_char( "You lack the key.\r\n", ch );
         return;
      }
      if( IS_SET( obj->value[1], CONT_LOCKED ) )
      {
         send_to_char( "It's already locked.\r\n", ch );
         return;
      }

      SET_BIT( obj->value[1], CONT_LOCKED );
      send_to_char( "*Click*\r\n", ch );
      act( "$n locks $p.", ch, obj, NULL, TO_ROOM );
      return;
   }

   if( ( door = find_door( ch, arg ) ) >= 0 )
   {
      /*
       * 'lock door' 
       */
      ROOM_INDEX_DATA *to_room;
      EXIT_DATA *pexit;
      EXIT_DATA *pexit_rev;

      pexit = ch->in_room->exit[door];
      if( !pexit->exit_info.test(EX_CLOSED) )
      {
         send_to_char( "It's not closed.\r\n", ch );
         return;
      }
      if( pexit->key < 0 )
      {
         send_to_char( "It can't be locked.\r\n", ch );
         return;
      }
      if( !has_key( ch, pexit->key ) )
      {
         send_to_char( "You lack the key.\r\n", ch );
         return;
      }
      if( pexit->exit_info.test(EX_LOCKED) )
      {
         send_to_char( "It's already locked.\r\n", ch );
         return;
      }

      pexit->exit_info.set(EX_LOCKED);
      send_to_char( "*Click*\r\n", ch );
      act( "$n locks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

      /*
       * lock the other side 
       */
      if( ( to_room = pexit->to_room ) != NULL
          && ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0 && pexit_rev->to_room == ch->in_room )
      {
         pexit_rev->exit_info.set(EX_LOCKED);
      }
   }

   return;
}



DO_FUN(do_unlock)
{
   char arg[MSL];
   OBJ_DATA *obj;
   int door;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Unlock what?\r\n", ch );
      return;
   }

   if( ( obj = get_obj_here( ch, arg ) ) != NULL )
   {
      /*
       * 'unlock object' 
       */
      if( obj->item_type != ITEM_CONTAINER )
      {
         send_to_char( "That's not a container.\r\n", ch );
         return;
      }
      if( !IS_SET( obj->value[1], CONT_CLOSED ) )
      {
         send_to_char( "It's not closed.\r\n", ch );
         return;
      }
      if( obj->value[2] < 0 )
      {
         send_to_char( "It can't be unlocked.\r\n", ch );
         return;
      }
      if( !has_key( ch, obj->value[2] ) )
      {
         send_to_char( "You lack the key.\r\n", ch );
         return;
      }
      if( !IS_SET( obj->value[1], CONT_LOCKED ) )
      {
         send_to_char( "It's already unlocked.\r\n", ch );
         return;
      }

      REMOVE_BIT( obj->value[1], CONT_LOCKED );
      send_to_char( "*Click*\r\n", ch );
      act( "$n unlocks $p.", ch, obj, NULL, TO_ROOM );
      return;
   }

   if( ( door = find_door( ch, arg ) ) >= 0 )
   {
      /*
       * 'unlock door' 
       */
      ROOM_INDEX_DATA *to_room;
      EXIT_DATA *pexit;
      EXIT_DATA *pexit_rev;

      pexit = ch->in_room->exit[door];
      if( !pexit->exit_info.test(EX_CLOSED) )
      {
         send_to_char( "It's not closed.\r\n", ch );
         return;
      }
      if( pexit->key < 0 )
      {
         send_to_char( "It can't be unlocked.\r\n", ch );
         return;
      }
      if( !has_key( ch, pexit->key ) )
      {
         send_to_char( "You lack the key.\r\n", ch );
         return;
      }
      if( !pexit->exit_info.test(EX_LOCKED) )
      {
         send_to_char( "It's already unlocked.\r\n", ch );
         return;
      }

      pexit->exit_info.reset(EX_LOCKED);
      send_to_char( "*Click*\r\n", ch );
      act( "$n unlocks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

      /*
       * unlock the other side 
       */
      if( ( to_room = pexit->to_room ) != NULL
          && ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL && pexit_rev->to_room == ch->in_room )
      {
         pexit_rev->exit_info.reset(EX_LOCKED);
      }
   }

   return;
}



DO_FUN(do_pick)
{
   char arg[MSL];
   CHAR_DATA *gch;
   OBJ_DATA *obj;
   int door;

   one_argument( argument, arg );

   if( ch->check_cooldown("pick lock") )
    return;

   if( arg[0] == '\0' )
   {
      send_to_char( "Pick what?\r\n", ch );
      return;
   }

   ch->set_cooldown("pick lock");

   /*
    * look for guards 
    */
   for( gch = ch->in_room->first_person; gch; gch = gch->next_in_room )
   {
      if( IS_NPC( gch ) && IS_AWAKE( gch ) && ch->level + 5 < gch->level )
      {
         act( "$N is standing too close to the lock.", ch, NULL, gch, TO_CHAR );
         return;
      }
   }

   if( !IS_NPC( ch ) && number_percent(  ) > ch->pcdata->learned[gsn_pick_lock] )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   if( ( obj = get_obj_here( ch, arg ) ) != NULL )
   {
      /*
       * 'pick object' 
       */
      if( obj->item_type != ITEM_CONTAINER )
      {
         send_to_char( "That's not a container.\r\n", ch );
         return;
      }
      if( !IS_SET( obj->value[1], CONT_CLOSED ) )
      {
         send_to_char( "It's not closed.\r\n", ch );
         return;
      }
      if( obj->value[2] < 0 )
      {
         send_to_char( "It can't be unlocked.\r\n", ch );
         return;
      }
      if( !IS_SET( obj->value[1], CONT_LOCKED ) )
      {
         send_to_char( "It's already unlocked.\r\n", ch );
         return;
      }
      if( IS_SET( obj->value[1], CONT_PICKPROOF ) )
      {
         send_to_char( "You failed.\r\n", ch );
         return;
      }

      REMOVE_BIT( obj->value[1], CONT_LOCKED );
      send_to_char( "*Click*\r\n", ch );
      act( "$n picks $p.", ch, obj, NULL, TO_ROOM );
      return;
   }

   if( ( door = find_door( ch, arg ) ) >= 0 )
   {
      /*
       * 'pick door' 
       */
      ROOM_INDEX_DATA *to_room;
      EXIT_DATA *pexit;
      EXIT_DATA *pexit_rev;

      pexit = ch->in_room->exit[door];
      if( !pexit->exit_info.test(EX_CLOSED) )
      {
         send_to_char( "It's not closed.\r\n", ch );
         return;
      }
      if( pexit->key < 0 )
      {
         send_to_char( "It can't be picked.\r\n", ch );
         return;
      }
      if( !pexit->exit_info.test(EX_LOCKED) )
      {
         send_to_char( "It's already unlocked.\r\n", ch );
         return;
      }
      if( pexit->exit_info.test(EX_PICKPROOF) )
      {
         send_to_char( "You failed.\r\n", ch );
         return;
      }

      pexit->exit_info.reset(EX_LOCKED);
      send_to_char( "*Click*\r\n", ch );
      act( "$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

      /*
       * pick the other side 
       */
      if( ( to_room = pexit->to_room ) != NULL
          && ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL && pexit_rev->to_room == ch->in_room )
      {
         pexit_rev->exit_info.reset(EX_LOCKED);
      }
   }

   return;
}




DO_FUN(do_stand)
{
   switch ( ch->position )
   {
      case POS_SLEEPING:
         if( IS_AFFECTED( ch, AFF_SLEEP ) )
         {
            send_to_char( "You can't wake up!\r\n", ch );
            return;
         }

         send_to_char( "You wake and stand up.\r\n", ch );
         act( "$n wakes and stands up.", ch, NULL, NULL, TO_ROOM );
         ch->position = POS_STANDING;
         break;

      case POS_RESTING:
         send_to_char( "You stand up.\r\n", ch );
         act( "$n stands up.", ch, NULL, NULL, TO_ROOM );
         if( ch->sitting != NULL )
         {
            ch->sitting->value[1]--;
            ch->sitting = NULL;
         }
         ch->position = POS_STANDING;
         break;

      case POS_STANDING:
         send_to_char( "You are already standing.\r\n", ch );
         break;

      case POS_FIGHTING:
         send_to_char( "You are already fighting!\r\n", ch );
         break;
      case POS_RIDING:
         send_to_char( "Dismount first!\r\n", ch );
         break;

   }

   return;
}



DO_FUN(do_rest)
{

   OBJ_DATA *furn = NULL;
   bool sit;

   sit = FALSE;

   if( argument[0] != '\0' )
   {
      furn = get_obj_here( ch, argument );
      if( furn == NULL || furn->item_type != ITEM_FURNITURE )
      {
         send_to_char( "You can't rest on that!\r\n", ch );
         return;
      }
      if( furn->value[1] >= furn->value[0] )
      {
         send_to_char( "It's full already!\r\n", ch );
         return;
      }
      sit = TRUE;

   }
   switch ( ch->position )
   {
      case POS_SLEEPING:
         send_to_char( "You are already sleeping.\r\n", ch );
         break;

      case POS_RESTING:
         send_to_char( "You are already resting.\r\n", ch );
         break;

      case POS_STANDING:
         if( sit == FALSE )
         {
            send_to_char( "You rest.\r\n", ch );
            act( "$n rests.", ch, NULL, NULL, TO_ROOM );
         }
         else
         {
            ch->sitting = furn;
            act( "$n rests on $p", ch, furn, NULL, TO_ROOM );
            act( "You rest on $p", ch, furn, NULL, TO_CHAR );
            ch->sitting->value[1]++;
         }
         ch->position = POS_RESTING;
         break;

      case POS_FIGHTING:
         send_to_char( "You are already fighting!\r\n", ch );
         break;
      case POS_RIDING:
         send_to_char( "Dismount first!\r\n", ch );
         break;

   }

   return;
}



DO_FUN(do_sleep)
{
   switch ( ch->position )
   {
      case POS_SLEEPING:
         send_to_char( "You are already sleeping.\r\n", ch );
         break;

      case POS_RESTING:
      case POS_STANDING:
         send_to_char( "You sleep.\r\n", ch );
         act( "$n sleeps.", ch, NULL, NULL, TO_ROOM );
         ch->position = POS_SLEEPING;
         break;

      case POS_FIGHTING:
         send_to_char( "You are already fighting!\r\n", ch );
         break;
      case POS_RIDING:
         send_to_char( "Dismount first!\r\n", ch );
         break;

   }

   return;
}



DO_FUN(do_wake)
{
   char arg[MSL];
   CHAR_DATA *victim;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      do_stand( ch, argument );
      return;
   }

   if( !IS_AWAKE( ch ) )
   {
      send_to_char( "You are asleep yourself!\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_AWAKE( victim ) )
   {
      act( "$N is already awake.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( IS_AFFECTED( victim, AFF_SLEEP ) )
   {
      act( "You can't wake $M!", ch, NULL, victim, TO_CHAR );
      return;
   }

   act( "You wake $M.", ch, NULL, victim, TO_CHAR );
   act( "$n wakes you.", ch, NULL, victim, TO_VICT );
   victim->position = POS_STANDING;
   return;
}




DO_FUN(do_shadowform)
{
   AFFECT_DATA af;

   send_to_char( "You attempt to take on shadow form.\r\n", ch );
   act( "$n tries to take on shadow form.", ch, NULL, NULL, TO_ROOM );
   affect_strip( ch, gsn_sneak );

   if( IS_NPC( ch ) || number_percent(  ) < ch->pcdata->learned[gsn_shadow] )
   {
      af.type = gsn_sneak;
      af.duration = ch->level;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = AFF_SNEAK;
      affect_to_char( ch, &af );
   }

   return;
}

DO_FUN(do_sneak)
{
   AFFECT_DATA af;

   send_to_char( "You attempt to move silently.\r\n", ch );
   affect_strip( ch, gsn_sneak );

   if( IS_NPC( ch ) || number_percent(  ) < ch->pcdata->learned[gsn_sneak] )
   {
      af.type = gsn_sneak;
      af.duration = ch->level;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = AFF_SNEAK;
      affect_to_char( ch, &af );
   }

   return;
}

DO_FUN(do_warcry) /* Thanks Koron, saved me re-inventing the wheel :) --Kline */
{
 AFFECT_DATA af;
 short chance;

 if( ch->check_cooldown("warcry") )
  return;

 if( is_affected(ch,skill_lookup("warcry")) )
 {
  send_to_char("You've already let out a warcry!\r\n",ch);
  return;
 }

 if( IS_NPC(ch) )
  chance = 80;
 else
  chance = ch->pcdata->learned[skill_lookup("warcry")];

 if( chance < 10 )
 {
  send_to_char("You're really not sure how loud you should scream. Maybe you should practice more.\r\n",ch);
  return;
 }

 ch->set_cooldown("warcry");

 if( number_percent() > chance )
 {
  send_to_char("Your warcry comes out sounding more like an pubescent screech...\r\n",ch);
  act("$n makes some very strange noises...",ch,NULL,NULL,TO_ROOM);
  return;
 }

 af.type      = skill_lookup("warcry");
 af.duration  = 4 + (ch->get_level("psuedo") * 6);
 af.location  = APPLY_HITROLL;
 af.modifier  = 10 + (ch->get_level("psuedo") / 12);
 af.bitvector = 0;
 affect_to_char(ch,&af);

 af.location  = APPLY_SAVING_SPELL;
 af.modifier  = -1 - (ch->get_level("psuedo") / 12);
 affect_to_char(ch,&af);

 send_to_char("You feel ready for battle!\r\n",ch);
 act("$n screams a blood-curdling warcry!",ch,NULL,NULL,TO_ROOM);
 return;
}

DO_FUN(do_hide)
{
   send_to_char( "You attempt to hide.\r\n", ch );

   if( IS_AFFECTED( ch, AFF_HIDE ) )
      REMOVE_BIT( ch->affected_by, AFF_HIDE );

   if( IS_NPC( ch ) || number_percent(  ) < ch->pcdata->learned[gsn_hide] )
      SET_BIT( ch->affected_by, AFF_HIDE );

   return;
}



/*
 * Contributed by Alander.
 */
DO_FUN(do_visible)
{
   affect_strip( ch, gsn_invis );
   affect_strip( ch, gsn_mass_invis );
   affect_strip( ch, gsn_sneak );
   REMOVE_BIT( ch->affected_by, AFF_HIDE );
   REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
   REMOVE_BIT( ch->affected_by, AFF_SNEAK );
   send_to_char( "You become more visible again.\r\n", ch );
   return;
}

DO_FUN(do_clan_recall)
{
   ROOM_INDEX_DATA *location;

   if( IS_NPC( ch ) )
      return;

   if( deathmatch )
   {
      send_to_char( "Not during a DEATHMATCH!!!\r\n", ch );
      return;
   }

   if( ch->clan == 0 )
   {
      send_to_char( "You must be in a clan to use this command!!\r\n", ch );
      return;
   }
   else
      location = get_room_index( clan_table[ch->clan].clan_room );

   if( location == NULL )
   {
      send_to_char( "You are well lost, like.\r\n", ch );
      return;
   }

   if( ch->in_room->room_flags.test(RFLAG_NO_RECALL) || IS_AFFECTED( ch, AFF_CURSE ) )
   {
      send_to_char( "Some strange force prevents your transport.\r\n", ch );
      return;
   }

   if( ch->in_room == location )
   {
      send_to_char( "You didn't have far to go, did you?\r\n", ch );
      return;
   }

   if( ch->fighting != NULL )
   {
      send_to_char( "You can't use this command when fighting!!\r\n", ch );
      return;
   }

   act( "$n is engulfed in a stream of red light!!", ch, NULL, NULL, TO_ROOM );
   send_to_char( "You are engulfed in a stream of red light, carrying you home!\r\n", ch );
   char_from_room( ch );
   char_to_room( ch, location );
   if( ch->riding != NULL )
   {
      char_from_room( ch->riding );
      char_to_room( ch->riding, location );
   }

   act( "$n arrives, carried upon a stream of red light!!", ch, NULL, NULL, TO_ROOM );
   do_look( ch, "auto" );
   return;
}

DO_FUN(do_recall)
{
   char buf[MSL];
   CHAR_DATA *victim;
   ROOM_INDEX_DATA *location;

   if( ch->check_cooldown(COOLDOWN_DEF) )
    return;
   if( IS_CASTING(ch) )
    do_stop(ch,"");

   act( "$n makes the Holy Sign for transportation!", ch, 0, 0, TO_ROOM );
   buf[0] = '\0';


   /*
    * FIXME: Need to check if NPC is charmed - if so find
    * * owner's recall location..... -S- 
    */

   /*
    * if ( IS_NPC( ch ) || ch->level < 6 ) 
    */

   if( IS_VAMP( ch ) )
      location = get_room_index( VAMPIRE_RECALL );
   else
      location = get_room_index( ROOM_VNUM_TEMPLE );

   if( !IS_NPC( ch ) )
      location = get_room_index( ch->pcdata->recall_vnum );

   if( deathmatch )
   {
      location = get_room_index( ROOM_VNUM_DM_RECALL );

      if( ch->hit > 250 )
      {
         send_to_char( "@@lYou wince in pain as you feel your molecules disperse, then reform.@@N\r\n", ch );
         ch->hit -= 200;
      }
      else
      {
         send_to_char( "Your molecules begin to disperse, then reform quickly, leaving you here.\r\n", ch );
         return;
      }

   }

   if( location == NULL )
   {
      send_to_char( "You are completely lost.\r\n", ch );
      return;
   }

   if( ch->in_room == location )
      return;

   if( ch->in_room->room_flags.test(RFLAG_NO_RECALL) || IS_AFFECTED( ch, AFF_CURSE ) )
   {
      send_to_char( "Some strange force prevents your recall.\r\n", ch );
      return;
   }

   if( ( victim = ch->fighting ) != NULL )
   {
      int lose;

      if( number_bits( 1 ) == 0 )
      {
         ch->set_cooldown(COOLDOWN_DEF,2.75);
         lose = ( ch->level / 4 ) + 1;
         lose = UMIN( lose, ch->exp );
         lose *= -1;
         ch->gain_exp(lose);
         snprintf( buf, MSL, "You failed!  You lose %d exps.\r\n", lose );
         send_to_char( buf, ch );
         return;
      }

      lose = ( ch->level / 4 ) + 25;
      lose = UMIN( lose, ch->exp );
      lose *= -1;
      ch->gain_exp(lose);
      snprintf( buf, MSL, "You recall from combat!  You lose %d exps.\r\n", lose );
      send_to_char( buf, ch );
      stop_fighting( ch, TRUE );
   }

   ch->move = ( ch->move > 10 ) ? ( ch->move - 10 ) : 0;
   act( "$n is engulfed by a stream of blue light!", ch, NULL, NULL, TO_ROOM );

   char_from_room( ch );
   char_to_room( ch, location );
   if( ch->riding != NULL )
   {
      char_from_room( ch->riding );
      char_to_room( ch->riding, location );
   }

   act( "$n arrives, carried by a stream of blue light!", ch, NULL, NULL, TO_ROOM );
   do_look( ch, "auto" );

   return;
}


DO_FUN(do_train)
{
   char buf[MSL];
   char buf2[MSL];
   CHAR_DATA *mob;
   int hp_gain = 0;
   int mana_gain = 0;
   int *pAbilityI = NULL;
   short *pAbilityS = NULL; /* This is going to be ugly...But better on memory at least. --Kline */
   int pMax = 0;
   char *pOutput;
   int cost, cost1, cost2, cost3, cost4, cost5; /* Urrgghh */
   buf[0] = '\0';
   buf2[0] = '\0';


   if( IS_NPC( ch ) )
   {
      send_to_char( "Not for NPCs.  Sorry.\r\n", ch );
      return;
   }

   /*
    * Check for trainer.
    */

   for( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
   {
      if( IS_NPC( mob ) && mob->act.test(ACT_TRAIN) )
         break;
   }

   if( mob == NULL )
   {
      send_to_char( "You can't do that here.\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      snprintf( buf, MSL, "You have %d practice sessions.\r\n", ch->pcdata->practice );
      send_to_char( buf, ch );
      argument = "foo";
   }

   cost = 4;

   if( !str_cmp( argument, "str" ) )
   {
      if( class_table[ch->p_class].attr_prime == APPLY_STR )
         cost = 3;
      pAbilityS = &ch->pcdata->perm_str;
      pMax = ch->pcdata->max_str;
      pOutput = "strength";
   }

   else if( !str_cmp( argument, "int" ) )
   {
      if( class_table[ch->p_class].attr_prime == APPLY_INT )
         cost = 3;
      pAbilityS = &ch->pcdata->perm_int;
      pMax = ch->pcdata->max_int;
      pOutput = "intelligence";
   }

   else if( !str_cmp( argument, "wis" ) )
   {
      if( class_table[ch->p_class].attr_prime == APPLY_WIS )
         cost = 3;
      pAbilityS = &ch->pcdata->perm_wis;
      pMax = ch->pcdata->max_wis;
      pOutput = "wisdom";
   }

   else if( !str_cmp( argument, "dex" ) )
   {
      if( class_table[ch->p_class].attr_prime == APPLY_DEX )
         cost = 3;
      pAbilityS = &ch->pcdata->perm_dex;
      pMax = ch->pcdata->max_dex;
      pOutput = "dexterity";
   }

   else if( !str_cmp( argument, "con" ) )
   {
      if( class_table[ch->p_class].attr_prime == APPLY_CON )
         cost = 3;
      pAbilityS = &ch->pcdata->perm_con;
      pMax = ch->pcdata->max_con;
      pOutput = "constitution";
   }

   else if( !str_cmp( argument, "hp" ) )
   {
      pAbilityI = &ch->max_hit;
      pOutput = "number of hit points";
      cost = 4;   /* this is pracs per "train hp" */
      hp_gain = 1;   /* this is hp gained per "train hp" */
   }

   else if( !str_cmp( argument, "mana" ) )
   {
      pAbilityI = &ch->max_mana;
      pOutput = "amount of mana";
      cost = 5;
      mana_gain = 1;
   }

   else
   {
      /*
       * Work out the costs... 
       */
      cost1 = 4;
      cost2 = 4;
      cost3 = 4;
      cost4 = 4;
      cost5 = 4;
      if( class_table[ch->p_class].attr_prime == APPLY_STR )
         cost1 = 3;
      if( class_table[ch->p_class].attr_prime == APPLY_INT )
         cost2 = 3;
      if( class_table[ch->p_class].attr_prime == APPLY_WIS )
         cost3 = 3;
      if( class_table[ch->p_class].attr_prime == APPLY_DEX )
         cost4 = 3;
      if( class_table[ch->p_class].attr_prime == APPLY_CON )
         cost5 = 3;



      strcpy( buf, "You can train: hp (4)  mana (5)" );

      if( ch->pcdata->perm_str < ch->pcdata->max_str )
      {
         strncat( buf, "  str", MSL );
         snprintf( buf2, MSL, " (%d)", cost1 );
         strncat( buf, buf2, MSL-1 );
      }
      if( ch->pcdata->perm_int < ch->pcdata->max_int )
      {
         strncat( buf, "  int", MSL );
         snprintf( buf2, MSL, " (%d)", cost2 );
         strncat( buf, buf2, MSL-1 );
      }
      if( ch->pcdata->perm_wis < ch->pcdata->max_wis )
      {
         strncat( buf, "  wis", MSL );
         snprintf( buf2, MSL, " (%d)", cost3 );
         strncat( buf, buf2, MSL-1 );
      }
      if( ch->pcdata->perm_dex < ch->pcdata->max_dex )
      {
         strncat( buf, "  dex", MSL );
         snprintf( buf2, MSL, " (%d)", cost4 );
         strncat( buf, buf2, MSL-1 );
      }
      if( ch->pcdata->perm_con < ch->pcdata->max_con )
      {
         strncat( buf, "  con", MSL );
         snprintf( buf2, MSL, " (%d)", cost5 );
         strncat( buf, buf2, MSL-1 );
      }
      if( buf[strlen( buf ) - 1] != ':' )
      {
         strncat( buf, ".\r\n", MSL );
         send_to_char( buf, ch );
      }
      return;
   }



   if( !str_cmp( argument, "hp" ) )
   {

      if( cost > ch->pcdata->practice )
      {
         send_to_char( "You don't have enough practices.\r\n", ch );
         return;
      }

      ch->pcdata->practice -= cost;
      *pAbilityI += hp_gain;
      ch->pcdata->hp_from_gain += 1;
      act( "Your $T increases!", ch, NULL, pOutput, TO_CHAR );
      act( "$n's $T increases!", ch, NULL, pOutput, TO_ROOM );
      return;
   }

   if( !str_cmp( argument, "mana" ) )
   {

      if( cost > ch->pcdata->practice )
      {
         send_to_char( "You don't have enough practices.\r\n", ch );
         return;
      }

      ch->pcdata->practice -= cost;
      *pAbilityI += mana_gain;
      ch->pcdata->mana_from_gain += 1;

      act( "Your $T increases!", ch, NULL, pOutput, TO_CHAR );
      act( "$n's $T increases!", ch, NULL, pOutput, TO_ROOM );
      return;
   }
   if( *pAbilityS >= pMax )
   {
      act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
      return;
   }

   if( cost > ch->pcdata->practice )
   {
      send_to_char( "You don't have enough practices.\r\n", ch );
      return;
   }

   ch->pcdata->practice -= cost;
   *pAbilityS += 1;
   act( "Your $T increases!", ch, NULL, pOutput, TO_CHAR );
   act( "$n's $T increases!", ch, NULL, pOutput, TO_ROOM );
   return;
}

DO_FUN(do_halls)
    /*
     * Quite a lot of the following code derives from the original
     * * MERC2.2 code, although the tweaks are my own.
     * * -- Stephen
     */
{

   CHAR_DATA *victim;


   /*
    * Remember to limit use of this command to immorts! 
    */

   if( ch->level < 81 )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   act( "$n prepares to return to the Halls of the Immortals!", ch, 0, 0, TO_ROOM );

   /*
    * Check is immort is fighting.  
    * * Stop fight if they are!
    */

   if( ( victim = ch->fighting ) != NULL )
   {
      stop_fighting( ch, TRUE );
   }

   act( "$n vanishes into a beam of energy!", ch, NULL, NULL, TO_ROOM );
   char_from_room( ch );
   char_to_room( ch, get_room_index( ROOM_VNUM_HALLS ) );
   act( "$n steps out of the energy beam!", ch, NULL, NULL, TO_ROOM );

   do_look( ch, "auto" );
   return;
}


DO_FUN(do_smash)
{
   char arg[MSL];
   int door;
   int best;
   int cnt;
   bool joke;  /* Was it unlocked to start with? */

   joke = FALSE;
   one_argument( argument, arg );

   best = -1;
   if( !IS_NPC( ch ) )
   {
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         if( ch->lvl[cnt] >= skill_table[gsn_smash].skill_level[cnt] )
            best = cnt;
         else
            best = ch->level;
   }
   if( best == -1 )
   {
      send_to_char( "You don't know of such a skill.\r\n", ch );
      return;
   }

   if( arg[0] == '\0' )
   {
      send_to_char( "Smash what?\r\n", ch );
      return;
   }

   if( ( door = find_door( ch, arg ) ) >= 0 )
   {
      /*
       * 'open door' 
       */
      ROOM_INDEX_DATA *to_room;
      EXIT_DATA *pexit;
      EXIT_DATA *pexit_rev;

      pexit = ch->in_room->exit[door];
      if( !pexit->exit_info.test(EX_CLOSED) )
      {
         send_to_char( "It's already open.\r\n", ch );
         return;
      }

      if( ( 2 * number_percent(  ) > ch->pcdata->learned[gsn_smash] ) || ( pexit->exit_info.test(EX_SMASHPROOF) ) )
      {
         act( "$n tries to smash the $d, but fails.", ch, NULL, pexit->keyword, TO_ROOM );
         send_to_char( "You failed!\r\n", ch );
         return;
      }

      if( pexit->exit_info.test(EX_LOCKED) )
         pexit->exit_info.reset(EX_LOCKED);
      else
         joke = TRUE;

      pexit->exit_info.reset(EX_CLOSED);
      act( "$n smashes opens the $d.", ch, NULL, pexit->keyword, TO_ROOM );
      act( "You smash open the $d.", ch, NULL, pexit->keyword, TO_CHAR );

      if( joke )
      {
         act( "The $d was already unlocked!!!", ch, NULL, pexit->keyword, TO_ROOM );
         act( "The $d was already unlocked!!!", ch, NULL, pexit->keyword, TO_CHAR );
         act( "$n tumbles through $d!!!", ch, NULL, pexit->keyword, TO_ROOM );
         act( "You tumble through $d!!!", ch, NULL, pexit->keyword, TO_CHAR );

      }



      /*
       * open the other side 
       */
      if( ( to_room = pexit->to_room ) != NULL
          && ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL && pexit_rev->to_room == ch->in_room )
      {
         CHAR_DATA *rch;

         if( pexit_rev->exit_info.test(EX_LOCKED) )
            pexit_rev->exit_info.reset(EX_LOCKED);

         pexit_rev->exit_info.reset(EX_CLOSED);
         for( rch = to_room->first_person; rch != NULL; rch = rch->next_in_room )
            act( "The $d is smashed open.", rch, NULL, pexit_rev->keyword, TO_CHAR );

         if( joke )
         {
            char_from_room( ch );
            char_to_room( ch, to_room );
            act( "$n tumbles into the room, head over heels!", ch, NULL, NULL, TO_ROOM );
            do_look( ch, "" );
         }

      }
   }

   return;
}


DO_FUN(do_scan)
/* Informs ch if there are any (N)PCs in the 6 adjacent rooms.
 * I'm sure this could be written better.... ;)
 * -- Stephen
 */
{
   ROOM_INDEX_DATA *location;
   int door;
   char buf[MSL];
   char person[MSL];
   CHAR_DATA *d;
   bool found = FALSE;
   buf[0] = '\0';
   person[0] = '\0';

   act( "$n looks all around for anyone else.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "You look around.\r\n", ch );

   location = ch->in_room;
   for( door = 0; door < MAX_DIR; door++ )
   {
      EXIT_DATA *pexit;

      if( ( pexit = location->exit[door] ) == 0
          || pexit->to_room == NULL
          || pexit->exit_info.test(EX_CLOSED)
          || ( str_cmp( pexit->keyword, "" ) ) || ( pexit->to_room->vnum == ch->in_room->vnum ) )
      {
         /*
          * Then this direction is "invalid"
          */
         continue;
      }
      else
      {

         for( d = pexit->to_room->first_person; d != NULL; d = d->next_in_room )
         {

            if( ( pexit->to_room->vnum == d->in_room->vnum ) && can_see( ch, d ) )
            {
               found = TRUE;

               /*
                * bit here to set person to (N)PC name
                */

               if( IS_NPC( d ) )
               {
                  snprintf( person, MIL, "%s (NPC)", d->get_name() );
               }
               else
               {
                  snprintf( person, MIL, "%s", d->get_name(ch) );
               }

               snprintf( buf, MSL, "%s : %s\r\n", dir_name[door], person );

               send_to_char( buf, ch );

            }

         }
         if( ( pexit->to_room->affected_by != 0 )
             && ( ( is_affected( ch, skill_lookup( "detect magic" ) ) ) || ( item_has_apply( ch, ITEM_APPLY_DET_MAG ) ) ) )
         {
            snprintf( buf, MSL, "The room %s has a @@rMagical@@N Affect!!!\r\n", dir_name[door] );
            send_to_char( buf, ch );
         }

      }
   }
   if( !found )
   {
      send_to_char( "You fail to spot anyone around you.\r\n", ch );
   }
   return;

}


DO_FUN(do_enter)
{
   OBJ_DATA *portal;
   ROOM_INDEX_DATA *to_room;
   OBJ_DATA *foo;
   OBJ_DATA *foo_next;
   int door;

   if( argument[0] == '\0' )
   {
      send_to_char( "Enter what?\r\n", ch );
      return;
   }

   portal = get_obj_here( ch, argument );
   if( portal == NULL || portal->item_type != ITEM_PORTAL || portal->value[2] == 0 )
   {
/* look for a keyworded door */

      if( ( door = find_door( ch, argument ) ) >= 0 )
      {
         /*
          * enterable exit 
          */
         ch->using_named_door = TRUE;
         move_char( ch, door, TRUE );
         return;
      }
      else
      {
/*        send_to_char( "You can't enter that!\r\n", ch );  */
         return;
      }

      return;
   }

   if( ( to_room = get_room_index( portal->value[0] ) ) == NULL )
   {
      send_to_char( "Nothing happens.\r\n", ch );
      return;
   }
   if( (ch->in_room->room_flags.test(RFLAG_NO_PORTAL) || to_room->room_flags.test(RFLAG_NO_PORTAL )) && number_range( 0, 100 ) < 75 )
   {
      act( "The anti-magic zone causes the portal to @@eIMPLODE@@N!!!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "The anti-magic zone causes the portal to @@eIMPLODE@@N!!!", ch );

      extract_obj( portal );

      /*
       * Now check if there is a corresponding portal in to_room 
       */
      for( foo = to_room->first_content; foo != NULL; foo = foo_next )
      {
         foo_next = foo->next_in_room;

         if( foo->item_type == ITEM_PORTAL && foo->value[0] == ch->in_room->vnum )
            break;
         extract_obj( foo );

      }
      return;
   }


   act( "$n enters $p.", ch, portal, NULL, TO_ROOM );
   act( "You enter $p.", ch, portal, NULL, TO_CHAR );

   /*
    * Now check if there is a corresponding portal in to_room 
    */
   for( foo = to_room->first_content; foo != NULL; foo = foo->next_in_room )
      if( foo->item_type == ITEM_PORTAL && foo->value[0] == ch->in_room->vnum )
         break;

   char_from_room( ch );
   char_to_room( ch, to_room );

   if( foo == NULL )
      act( "A glowing sphere forms, and $n steps out!", ch, NULL, NULL, TO_ROOM );
   else
      act( "$n steps out of $p.", ch, foo, NULL, TO_ROOM );

   do_look( ch, "" );
   return;
}

DO_FUN(do_scout)
{
   ROOM_INDEX_DATA *loc;
   ROOM_INDEX_DATA *tmploc;
   int door;
   int depth = 0;
   int counter;
   CHAR_DATA *target;
   char buf[MSL];
   char pre[MSL];
   buf[0] = '\0';
   pre[0] = '\0';

   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_scout] == 0 )
   {
      send_to_char( "You are not trained in this skill!\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) )
   {
      /*
       * depth is the number of rooms AWAY from your present location 
       */
      if( ch->pcdata->learned[gsn_scout] > 78 )
         depth = 4;
      else if( ch->pcdata->learned[gsn_scout] > 64 )
         depth = 3;
      else if( ch->pcdata->learned[gsn_scout] > 40 )
         depth = 2;
      else
         depth = 1;
   }

   act( "$n looks all around for anyone else.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "You look around.\r\n", ch );

   while( depth > 0 )
   {
      loc = ch->in_room;
      for( door = 0; door < MAX_DIR; door++ )
      {
         tmploc = loc;

         /*
          * get the room 
          */
         for( counter = 0; counter < depth; counter++ )
         {
            if( tmploc->exit[door] == 0
                || tmploc->exit[door]->to_room == NULL
                || tmploc->exit[door]->exit_info.test(EX_CLOSED) || ( str_cmp( tmploc->exit[door]->keyword, "" ) ) )
               break;

            tmploc = tmploc->exit[door]->to_room;
         }

         /*
          * if room didn't exist 
          */
         if( counter < depth )
            continue;


         for( target = tmploc->first_person; target != NULL; target = target->next_in_room )
         {
            if( target->in_room->vnum == ch->in_room->vnum )
               break;

            if( !can_see( ch, target ) )
               continue;

            if( depth == 1 )
               snprintf( pre, MIL, "%s : ", dir_name[door] );
            else if( depth == 2 )
               snprintf( pre, MIL, "Far %s : ", dir_name[door] );
            else if( depth == 3 )
               snprintf( pre, MIL, "Very Far %s : ", dir_name[door] );
            else if( depth == 4 )
               snprintf( pre, MIL, "Distant %s : ", dir_name[door] );

            snprintf( buf, MIL, "%s%s\r\n", pre, target->get_name() );

            send_to_char( buf, ch );
         }
      }

      depth = depth - 1;
   }
}

DO_FUN(do_abandon) /* Thanks to Koron & Abel for the idea! --Kline */
{
 CHAR_DATA *vch;
 list<CHAR_DATA *>::iterator li;

 if( ch->num_followers < 1 )
 {
  send_to_char("Sadly you have no followers to abandon.\r\n",ch);
  return;
 }

 for( li = char_list.begin(); li != char_list.end(); li++ )
 {
  vch = *li;
  if( vch->master == ch )
   stop_follower(vch);
 }

 return;
}
