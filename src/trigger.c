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


#define DEC_TRIGGER( fun )		TRIGGER_FUN  fun

typedef void TRIGGER_FUN args( ( CHAR_DATA * ch, OBJ_DATA * obj ) );

struct trigger_type
{
   char *name;
   bool always_extract;
   TRIGGER_FUN *func;
};

DEC_TRIGGER( trig_transfer );
DEC_TRIGGER( trig_restore );
DEC_TRIGGER( trig_slay );
DEC_TRIGGER( trig_transform );

const struct trigger_type trigger_table[] = {
   {"Transfer", FALSE, trig_transfer},
   {"Restore", TRUE, trig_restore},
   {"Slay", FALSE, trig_slay},
   {"Transform", TRUE, trig_transform},
   {NULL, FALSE, NULL,}
};

void trigger_show( CHAR_DATA * ch )
{
   char buf[MAX_STRING_LENGTH];

   sprintf( buf, "Trigger objects information:\n\r" );

   strcat( buf, "v1  Name           v2		    v3\n\r" );
   strcat( buf, "--  ----        ---------       ---------\n\r" );
   strcat( buf, " 1  Transport   Room vnum          N/A\n\r" );
   strcat( buf, " 2  Restore	    N/A		    N/A\n\r" );
   strcat( buf, " 3  Slay	    N/A	            N/A\n\r" );
   strcat( buf, " 4  Transform   Mob Vnum	    N/A\n\r" );
   strcat( buf, "v0 determines what action causes the trigger:\n\r" );
   strcat( buf, "blah blah blah" );
   send_to_char( buf, ch );
   return;
}


void trigger_handler( CHAR_DATA * ch, OBJ_DATA * obj, int trigger )
{
   int a;

   if( ( obj->item_type != ITEM_TRIGGER ) || ( obj->value[0] != trigger ) )
      return;

   /*
    * Find trigger in table and call function 
    */
   for( a = 0; trigger_table[a].name != NULL; a++ )
      if( a == obj->value[1] )
      {
         ( *trigger_table[a].func ) ( ch, obj );
         if( IS_SET( obj->extra_flags, ITEM_TRIG_DESTROY ) || trigger_table[a].always_extract )
         {
            extract_obj( obj );
         }
         return;
      }

   bug( "Trigger_handler: trigger number %d not found.", trigger );
   return;
}

void trig_transfer( CHAR_DATA * ch, OBJ_DATA * obj )
{
   ROOM_INDEX_DATA *location;

   if( ( location = get_room_index( obj->value[2] ) ) == NULL )
   {
      bug( "Trig_transfer: Obj has invalid v2.", 0 );
      return;
   }

   act( "$p glows brightly!", ch, obj, NULL, TO_CHAR );
   act( "$p glows brightly!", ch, obj, NULL, TO_ROOM );
   if( ch->position == POS_FIGHTING )
      stop_fighting( ch, TRUE );

   act( "$n vanishes suddenly!", ch, NULL, NULL, TO_ROOM );
   send_to_char( "You vanish suddenly!\n\r", ch );
   char_from_room( ch );
   char_to_room( ch, location );
   do_look( ch, "auto" );
   act( "$n suddenly appears before you!", ch, NULL, NULL, TO_ROOM );
   return;
}

void trig_restore( CHAR_DATA * ch, OBJ_DATA * obj )
{
   act( "A beam of white light from $p sweeps over $n!", ch, obj, NULL, TO_ROOM );
   act( "A beam of white light from $p sweeps over you!", ch, obj, NULL, TO_ROOM );
   ch->hit = ch->max_hit;
   ch->mana = ch->max_mana;
   ch->move = ch->max_move;
   send_to_char( "You feel very healthy!\n\r", ch );
   return;
}

void trig_slay( CHAR_DATA * ch, OBJ_DATA * obj )
{
   act( "A beam of black light from $p obliterates $n!!", ch, obj, NULL, TO_ROOM );
   act( "A beam of black light from $p obliterates you!", ch, obj, NULL, TO_CHAR );
   raw_kill( ch, "" );
   return;
}

void trig_transform( CHAR_DATA * ch, OBJ_DATA * obj )
{
   MOB_INDEX_DATA *mob;
   CHAR_DATA *new;

   if( ( mob = get_mob_index( obj->value[2] ) ) == NULL )
   {
      bugf( "Trig_transform: invalid value[2] (%d) for object vnum %d", obj->value[2], obj->pIndexData->vnum );
      return;
   }

   act( "$p starts glowing brightly!", ch, obj, NULL, TO_ROOM );
   act( "$p starts glowing brightly!", ch, obj, NULL, TO_CHAR );

   if( obj->carried_by != NULL )
   {
      act( "$n drops $p in shock and steps back.", ch, obj, NULL, TO_ROOM );
      act( "You drop $p in shock and step back.", ch, obj, NULL, TO_CHAR );
   }

   new = create_mobile( mob );
   char_to_room( new, ch->in_room );

   act( "$p slowly turns into $n!!!", new, obj, NULL, TO_ROOM );
   return;
}
