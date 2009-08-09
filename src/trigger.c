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

#ifndef DEC_TRIGGER_H
#include "h/trigger.h"
#endif

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

   snprintf( buf, MSL, "Trigger objects information:\r\n" );

   strncat( buf, "v1  Name           v2		    v3\r\n", MSL );
   strncat( buf, "--  ----        ---------       ---------\r\n", MSL );
   strncat( buf, " 1  Transport   Room vnum          N/A\r\n", MSL );
   strncat( buf, " 2  Restore	    N/A		    N/A\r\n", MSL );
   strncat( buf, " 3  Slay	    N/A	            N/A\r\n", MSL );
   strncat( buf, " 4  Transform   Mob Vnum	    N/A\r\n", MSL );
   strncat( buf, "v0 determines what action causes the trigger:\r\n", MSL );
   strncat( buf, "blah blah blah", MSL );
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
         if(  IS_OBJ_STAT(obj,ITEM_EXTRA_TRIG_DESTROY) || trigger_table[a].always_extract )
         {
            extract_obj( obj );
         }
         return;
      }

   bug( "Trigger_handler: trigger number %d not found.", trigger );
   return;
}

TRIG_FUN(trig_transfer)
{
   ROOM_INDEX_DATA *location;

   if( ( location = get_room_index( obj->value[2] ) ) == NULL )
   {
      bug( "Trig_transfer: Obj has invalid v2.", 0 );
      return;
   }

   if( obj->short_descr != NULL && strlen(obj->short_descr) > 0 )
   {
    act( "$p glows brightly!", ch, obj, NULL, TO_CHAR );
    act( "$p glows brightly!", ch, obj, NULL, TO_ROOM );
   }
   if( ch->position == POS_FIGHTING )
      stop_fighting( ch, TRUE );

   act( "$n vanishes suddenly!", ch, NULL, NULL, TO_ROOM );
   send_to_char( "You vanish suddenly!\r\n", ch );
   char_from_room( ch );
   char_to_room( ch, location );
   do_look( ch, "auto" );
   act( "$n suddenly appears before you!", ch, NULL, NULL, TO_ROOM );
   return;
}

TRIG_FUN(trig_restore)
{
   act( "A beam of white light from $p sweeps over $n!", ch, obj, NULL, TO_ROOM );
   act( "A beam of white light from $p sweeps over you!", ch, obj, NULL, TO_ROOM );
   ch->hit = ch->max_hit;
   ch->mana = ch->max_mana;
   ch->move = ch->max_move;
   send_to_char( "You feel very healthy!\r\n", ch );
   return;
}

TRIG_FUN(trig_slay)
{
   act( "A beam of black light from $p obliterates $n!!", ch, obj, NULL, TO_ROOM );
   act( "A beam of black light from $p obliterates you!", ch, obj, NULL, TO_CHAR );
   raw_kill( ch, "" );
   return;
}

TRIG_FUN(trig_transform)
{
   MOB_INDEX_DATA *mob;
   CHAR_DATA *cnew;

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

   cnew = create_mobile( mob );
   char_to_room( cnew, ch->in_room );

   act( "$p slowly turns into $n!!!", cnew, obj, NULL, TO_ROOM );
   return;
}
