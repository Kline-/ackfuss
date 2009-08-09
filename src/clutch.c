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

#ifndef DEC_CLUTCH_H
#include "h/clutch.h"
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

CLUTCH_TYPE clutch_table[SPELL_NUMBER] = {
   {1, TRUE, clutch_portal},
   {2, TRUE, clutch_gate},
   {3, FALSE, clutch_portal}
};

bool valid_clutch_number( int number )
{
   /*
    * Simply check if number is valid...
    */

   if( ( number + 1 ) > 0 && number < SPELL_NUMBER )
      return TRUE;

   return FALSE;
}

DO_FUN(do_clutch)
{
   /*
    * Big top-level clutch funciton. 
    */

   OBJ_DATA *obj;
   bool found;


   /*
    * See if any object is clutched... 
    */
   found = FALSE;
   for( obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list )
      if( ( obj->wear_loc == WEAR_HOLD_HAND_L ) && ( obj->item_type == ITEM_CLUTCH ) )
         break;

   if( obj == NULL )
   {
      send_to_char( "You have nothing to clutch!\r\n", ch );
      return;
   }

   if( !valid_clutch_number( obj->value[0] - 1 ) )
   {
      send_to_char( "Nothing happens.\r\n", ch );
      return;
   }

   act( "$n clutches $p tightly in $s hand!", ch, obj, NULL, TO_ROOM );
   act( "You clutch $p tightly in your hand!", ch, obj, NULL, TO_CHAR );
   if( !ch->in_room->room_flags.test(RFLAG_NO_PORTAL) )
      ( *clutch_table[obj->value[0] - 1].func_name ) ( obj->value[1], obj->value[2], obj->value[3], ch, obj );

   else

      return;


   if( clutch_table[obj->value[0] - 1].destroy )
   {

      act( "$p glows brightly, and is GONE!", ch, obj, NULL, TO_ROOM );
      act( "$p glows brightly in your hand, and is GONE!", ch, obj, NULL, TO_CHAR );
      obj_from_char( obj );
   }

   return;
}

void clutch_portal( int v1, int v2, int v3, CHAR_DATA * ch, OBJ_DATA * obj )
{
   /*
    * A Nice simple test spell.
    * * Transport ch to the room indicated by v1
    */

   ROOM_INDEX_DATA *room;

   room = get_room_index( v1 );
   if( room == NULL )
   {
      send_to_char( "Nothing seems to have happened!\r\n", ch );
      return;
   }


   act( "A glowing portal forms before $n, swallowing $m whole!", ch, NULL, NULL, TO_ROOM );
   send_to_char( "A glowing portal forms before you, swallowing you whole!\r\n", ch );
   stop_fighting( ch, TRUE );
   char_from_room( ch );
   char_to_room( ch, room );
   act( "A swirling portal forms long enough for $n to step out!", ch, NULL, NULL, TO_ROOM );
   do_look( ch, "" );
   return;
}

void clutch_gate( int v1, int v2, int v3, CHAR_DATA * ch, OBJ_DATA * obj )
{
   /*
    * Load mob indicated by v1, v2 is % chance of following 
    */

   MOB_INDEX_DATA *mob;
   CHAR_DATA *monster;

   mob = get_mob_index( v1 );
   if( mob == NULL )
   {
      send_to_char( "Nothing seems to have happened!\r\n", ch );
      return;
   }

   monster = create_mobile( mob );
   char_to_room( monster, ch->in_room );

   act( "$n describes a pentagon with $s $p.  $N appears!", ch, obj, monster, TO_ROOM );
   act( "You describe a pentagon with you $p.  $N appears!", ch, obj, monster, TO_CHAR );

   if( number_percent(  ) < v2 )
      add_follower( monster, ch );

   return;
}
