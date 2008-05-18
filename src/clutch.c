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


#define SPELL_NUMBER 3

#define DECLARE_CLUTCH_FUN( fun )	CLUTCH_FUN fun

typedef void CLUTCH_FUN args( ( int v1, int v2, int v3, CHAR_DATA * ch, OBJ_DATA * obj ) );


DECLARE_CLUTCH_FUN( clutch_portal );
DECLARE_CLUTCH_FUN( clutch_gate );

struct clutch_type
{
   int command_number;
   bool destroy;  /* Destory object once clutched? */
   CLUTCH_FUN *func_name;  /* The name of the function.... */
};


const struct clutch_type clutch_table[SPELL_NUMBER] = {
   {1, TRUE, clutch_portal},
   {2, TRUE, clutch_gate},
   {3, FALSE, clutch_portal}
};


/* Other local functions... */
bool valid_clutch_number( int number );




bool valid_clutch_number( int number )
{
   /*
    * Simply check if number is valid... 
    */

   if( ( number + 1 ) > 0 && number < SPELL_NUMBER )
      return TRUE;

   return FALSE;
}


void do_clutchinfo( CHAR_DATA * ch, char *argument )
{

   /*
    * Imm command to show details 
    */
   send_to_char( "Details for spells available for clutchable items:\n\r", ch );
   send_to_char( "NB: The first number is the value required for value[0].\n\r", ch );
   send_to_char( "v1 - v3 are value[1] - value[3].\n\r\n\r", ch );

   send_to_char( " 1 : Portal.   v1 = room_vnum to portal to.\n\r", ch );
   send_to_char( " 2 : Gate.     v1 = mob_vnum to load, v2 = % chance will follow.n\r", ch );
   send_to_char( " 3 : Portal    (As #1, but object is not destroyed.)\n\r", ch );
   send_to_char( "\n\r", ch );

   /*
    * KEEP THIS UPDATED!! 
    */

   return;
}

void do_clutch( CHAR_DATA * ch, char *argument )
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
      send_to_char( "You have nothing to clutch!\n\r", ch );
      return;
   }

   if( !valid_clutch_number( obj->value[0] - 1 ) )
   {
      send_to_char( "Nothing happens.\n\r", ch );
      return;
   }

   act( "$n clutches $p tightly in $s hand!", ch, obj, NULL, TO_ROOM );
   act( "You clutch $p tightly in your hand!", ch, obj, NULL, TO_CHAR );
   if( !IS_SET( ch->in_room->room_flags, ROOM_NO_PORTAL ) )
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
      send_to_char( "Nothing seems to have happened!\n\r", ch );
      return;
   }


   act( "A glowing portal forms before $n, swallowing $m whole!", ch, NULL, NULL, TO_ROOM );
   send_to_char( "A glowing portal forms before you, swallowing you whole!\n\r", ch );
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
      send_to_char( "Nothing seems to have happened!\n\r", ch );
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
