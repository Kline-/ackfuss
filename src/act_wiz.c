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

#ifndef DEC_ACT_OBJ_H
#include "h/act_obj.h"
#endif

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_AREASAVE_H
#include "h/areasave.h"
#endif

#ifndef DEC_BUILD_H
#include "h/build.h"
#endif

#ifndef DEC_BUILDTAB_H
#include "h/buildtab.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_EMAIL_H
#include "h/email.h"
#endif

#ifndef DEC_FIGHT_H
#include "h/fight.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_HUNT_H
#include "h/hunt.h"
#endif

#ifndef DEC_INTERP_H
#include "h/interp.h"
#endif

#ifndef DEC_MACROS_H
#include "h/macros.h"
#endif

#ifndef DEC_MAGIC_H
#include "h/magic.h"
#endif

#ifndef DEC_MUDINFO_H
#include "h/mudinfo.h"
#endif

#ifndef DEC_OBJ_FUN_H
#include "h/obj_fun.h"
#endif

#ifndef DEC_SAVE_H
#include "h/save.h"
#endif

#ifndef DEC_SPEC_FUN_H
#include "h/spec_fun.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

#ifndef DEC_STRFUNS_H
#include "h/strfuns.h"
#endif

#ifndef DEC_SYSDATA_H
#include "h/sysdata.h"
#endif

#ifndef DEC_UPDATE_H
#include "h/update.h"
#endif

extern bool merc_down;
extern int saving_area;
extern bool deathmatch;
extern bool wizlock;
extern OBJ_DATA *auction_item;
extern bool disable_timer_abort;
extern int port;
extern int control;

DO_FUN(do_transdm)
{
   CHAR_DATA *wch;
   list<CHAR_DATA *>::iterator li;
   int room;
   char buf[MSL];
   ROOM_INDEX_DATA *location;

   if( !deathmatch )
   {
      send_to_char( "You must run a DEATHMATCH first.\r\n", ch );
      return;
   }

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      wch = *li;
      if( !IS_NPC( wch ) && ( ch != wch ) )
      {
         if( IS_IMMORTAL( wch ) && ( wch != ch ) )
         {
            send_to_char( "Everyone has been transferred to the DM arena.\r\n", wch );
            continue;
         }

         room = number_range(DM_ARENA_START,DM_ARENA_END);
         snprintf( buf, MSL, "%d", room );
         location = find_location( wch, buf );
         if( wch->fighting != NULL )
            stop_fighting( wch, TRUE );
         char_from_room( wch );
         char_to_room( wch, location );
         send_to_char( "You have been transferred somewhere inside the DEATHMATCH arena!\r\n", wch );
      }
   }
   send_to_char( "Everyone has been transferred to the DM arena.\r\n", ch );
}

DO_FUN(do_wizhelp)
{
   char buf[MSL];
   char buf1[MSL];
   int cmd;
   int col;

   buf1[0] = '\0';
   col = 0;
   for( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
   {
      if( cmd_table[cmd].level >= LEVEL_HERO && cmd_table[cmd].level <= get_trust( ch ) )
      {
         snprintf( buf, MSL, "%-12s", cmd_table[cmd].name );
         strncat( buf1, buf, MSL-1 );
         if( ++col % 6 == 0 )
            strncat( buf1, "\r\n", MSL );
      }
   }

   if( col % 6 != 0 )
      strncat( buf1, "\r\n", MSL );
   send_to_char( buf1, ch );
   return;
}

DO_FUN(do_bamfin)
{
   if( !IS_NPC( ch ) )
   {
      smash_tilde( argument );
      free_string( ch->pcdata->bamfin );
      ch->pcdata->bamfin = str_dup( argument );
      send_to_char( "Ok.\r\n", ch );
   }
   return;
}



DO_FUN(do_bamfout)
{
   if( !IS_NPC( ch ) )
   {
      smash_tilde( argument );
      free_string( ch->pcdata->bamfout );
      ch->pcdata->bamfout = str_dup( argument );
      send_to_char( "Ok.\r\n", ch );
   }
   return;
}



DO_FUN(do_deny)
{
   char arg[MSL];
   CHAR_DATA *victim;
//    DESCRIPTOR_DATA d;
   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Deny whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
//      bool found = FALSE;
//      found = load_char_obj( &d, arg, TRUE );

//      if (!found)
      {
         char buf[MSL];
         snprintf( buf, MSL, "%s is not online.\r\n", capitalize( arg ) );
//        snprintf( buf, MSL, "No pFile found for '%s'.\r\n", capitalize( arg ) );
         send_to_char( buf, ch );
//        delete( d.character );
         return;
      }

//      victim = d.character;
//      d.character = NULL;
//      victim->desc = NULL;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\r\n", ch );
//        if ( victim->desc == NULL )
//          do_quit( victim, "" );
      return;
   }

   victim->act.flip(ACT_DENY);
   if( victim->act.test(ACT_DENY) )
   {
        send_to_char( "You are denied access!\r\n", victim );
        do_quit( victim, "" );
   }

   send_to_char( "OK.\r\n", ch );

   return;
}



DO_FUN(do_disconnect)
{
   char arg[MSL];
   DESCRIPTOR_DATA *d;
   CHAR_DATA *victim;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Disconnect whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( victim->desc == NULL )
   {
      act( "$N doesn't have a descriptor.", ch, NULL, victim, TO_CHAR );
      return;
   }

   for( d = first_desc; d != NULL; d = d->next )
   {
      if( d == victim->desc )
      {
         close_socket( d );
         send_to_char( "Ok.\r\n", ch );
         return;
      }
   }

   bug( "Do_disconnect: desc not found.", 0 );
   send_to_char( "Descriptor not found!\r\n", ch );
   return;
}



DO_FUN(do_pardon)
{
   char arg1[MSL];
   char arg2[MSL];
   CHAR_DATA *victim;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: pardon <character> <killer|thief>.\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }


   if( !str_cmp( arg2, "killer" ) )
   {
      if( victim->act.test(ACT_KILLER) )
      {
         victim->act.reset(ACT_KILLER);
         send_to_char( "Killer flag removed.\r\n", ch );
         send_to_char( "You are no longer a KILLER.\r\n", victim );
      }
      return;
   }

   if( !str_cmp( arg2, "thief" ) )
   {
      if( victim->act.test(ACT_THIEF) )
      {
         victim->act.reset(ACT_THIEF);
         send_to_char( "Thief flag removed.\r\n", ch );
         send_to_char( "You are no longer a THIEF.\r\n", victim );
      }
      return;
   }

   send_to_char( "Syntax: pardon <character> <killer|thief>.\r\n", ch );
   return;
}



DO_FUN(do_echo)
{
   DESCRIPTOR_DATA *d;

   if( argument[0] == '\0' )
   {
      send_to_char( "Echo what?\r\n", ch );
      return;
   }

   for( d = first_desc; d; d = d->next )
   {
      if( d->connected == CON_PLAYING )
      {
         send_to_char( argument, d->character );
         send_to_char( "@@g\r\n", d->character );
      }
   }

   return;
}

void recho( ROOM_INDEX_DATA *room, const char *argument )
{
 string str = argument;
 DESCRIPTOR_DATA *d;

 str += "@@N";

 for( d = first_desc; d; d = d->next )
 {
  if( d->connected == CON_PLAYING && d->character->in_room == room )
   send_to_char(str.c_str(),d->character);
 }

 return;
}

DO_FUN(do_recho)
{
   string str = argument;
   str += "\r\n";

   if( argument[0] == '\0' )
   {
      send_to_char( "Recho what?\r\n", ch );
      return;
   }

   recho(ch->in_room,str.c_str());

   return;
}



ROOM_INDEX_DATA *find_location( CHAR_DATA * ch, char *arg )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;

   if( is_number( arg ) )
      return get_room_index( atoi( arg ) );

   if( ( victim = get_char_world( ch, arg ) ) != NULL )
      return victim->in_room;

   if( ( obj = get_obj_world( ch, arg ) ) != NULL )
      return obj->in_room;

   return NULL;
}



DO_FUN(do_transfer)
{
   char arg1[MSL];
   char arg2[MSL];
   ROOM_INDEX_DATA *location;
   DESCRIPTOR_DATA *d;
   CHAR_DATA *victim;
//    DESCRIPTOR_DATA df;
//    bool found = FALSE;
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Transfer whom (and where)?\r\n", ch );
      return;
   }

   if( !str_cmp( arg1, "all" ) )
   {
      for( d = first_desc; d != NULL; d = d->next )
      {
         if( d->connected == CON_PLAYING
             && !IS_IMMORTAL( d->character )
             && d->character != ch && d->character->in_room != NULL && can_see( ch, d->character ) )
         {
            char buf[MSL];
            snprintf( buf, MSL, "%s %s", d->character->name.c_str(), arg2 );
            do_transfer( ch, buf );
         }
      }
      return;
   }

   /*
    * Thanks to Grodyn for the optional location parameter.
    */
   if( arg2[0] == '\0' )
   {
      location = ch->in_room;
   }
   else
   {
      if( ( location = find_location( ch, arg2 ) ) == NULL )
      {
         send_to_char( "No such location.\r\n", ch );
         return;
      }

      if( room_is_private( location ) )
      {
         send_to_char( "That room is private right now.\r\n", ch );
         return;
      }
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {

//      found = load_char_obj( &df, arg1, TRUE );

//      if (!found)
      {
         char buf[MSL];
//        snprintf( buf, MSL, "No pFile found for '%s'.\r\n", capitalize( arg1 ) );
         snprintf( buf, MSL, "%s is not online.\r\n", capitalize( arg1 ) );
         send_to_char( buf, ch );
//        delete( df.character );
         return;
      }

//      victim = df.character;
//      df.character = NULL;
//      victim->desc = NULL;
   }
   if( victim->in_room == NULL )
   {
      send_to_char( "They are in limbo.\r\n", ch );
      return;
   }

   if( victim->fighting != NULL )
      stop_fighting( victim, TRUE );
   act( "$n is snatched by the Gods!", victim, NULL, NULL, TO_ROOM );
   char_from_room( victim );
   char_to_room( victim, location );
//    if ( found )
//    {
//      do_quit( victim, "" );
//      return;
//    }
   act( "$n arrives from a puff of smoke.", victim, NULL, NULL, TO_ROOM );
   if( ch != victim )
      act( "$n has transferred you.", ch, NULL, victim, TO_VICT );
   do_look( victim, "auto" );
   send_to_char( "Ok.\r\n", ch );
}

DO_FUN(do_at)
{
   char arg[MSL];
   ROOM_INDEX_DATA *location;
   ROOM_INDEX_DATA *original;
   CHAR_DATA *wch;
   list<CHAR_DATA *>::iterator li;

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "At where what?\r\n", ch );
      return;
   }

   if( ( location = find_location( ch, arg ) ) == NULL )
   {
      send_to_char( "No such location.\r\n", ch );
      return;
   }

   if( room_is_private( location ) && ( ch->level != 85 ) )
   {
      send_to_char( "That room is private right now.\r\n", ch );
      return;
   }

   original = ch->in_room;
   char_from_room( ch );
   char_to_room( ch, location );
   interpret( ch, argument );

   /*
    * See if 'ch' still exists before continuing!
    * Handles 'at XXXX quit' case.
    */
   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      wch = *li;
      if( wch == ch )
      {
         char_from_room( ch );
         char_to_room( ch, original );
         break;
      }
   }

   return;
}



DO_FUN(do_goto)
{
   char arg[MSL];
   ROOM_INDEX_DATA *location;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Goto where?\r\n", ch );
      return;
   }

   if( ( location = find_location( ch, arg ) ) == NULL )
   {
      send_to_char( "No such location.\r\n", ch );
      return;
   }

   if( room_is_private( location ) )
   {
      send_to_char( "That room is private right now.\r\n", ch );
      return;
   }

   if( ch->fighting != NULL )
      stop_fighting( ch, TRUE );
   /*
    * if ( !is_set(ch->act, ACT_WIZINVIS) )
    */
   {
      act( "$L$n $T.", ch, NULL,
           ( ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0' )
           ? ch->pcdata->bamfout : "leaves in a swirling mist", TO_ROOM );
   }

   char_from_room( ch );
   char_to_room( ch, location );

/*    if ( !is_set(ch->act, ACT_WIZINVIS) )   */
   {
      act( "$L$n $T.", ch, NULL,
           ( ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0' )
           ? ch->pcdata->bamfin : "appears in a swirling mist", TO_ROOM );
   }

   do_look( ch, "auto" );
   return;
}



DO_FUN(do_rstat)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   ROOM_INDEX_DATA *location;
   ROOM_AFFECT_DATA *raf;
   OBJ_DATA *obj;
   CHAR_DATA *rch;
   int door;

   one_argument( argument, arg );
   location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
   if( location == NULL )
   {
      send_to_char( "No such location.\r\n", ch );
      return;
   }

   if( ch->in_room != location && room_is_private( location ) && ( ch->level != 85 ) )
   {
      send_to_char( "That room is private right now.\r\n", ch );
      return;
   }

   buf1[0] = '\0';

   snprintf( buf, MSL, "Name: '%s'\r\nArea: '%s'.\r\n", location->name, location->area->name );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL,
            "Vnum: %d.  Light: %d.  Sector: %s.\r\n",
            location->vnum, location->light, rev_table_lookup( tab_sector_types, location->sector_type ) );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL,
            "Room flags: %s.\r\nDescription:\r\n%s",
            bs_show_values( tab_room_flags, location->room_flags ), location->description );
   strncat( buf1, buf, MSL-1 );

   if( location->script_name != &str_empty[0] )
   {
      snprintf( buf, MSL, "\r\nRoom has Lua script: %s\r\n", location->script_name );
      strncat( buf1, buf, MSL-1 );
   }

   if( location->first_exdesc != NULL )
   {
      EXTRA_DESCR_DATA *ed;

      strncat( buf1, "Extra description keywords: '", MSL );
      for( ed = location->first_exdesc; ed; ed = ed->next )
      {
         strncat( buf1, ed->keyword, MSL-1 );
         if( ed->next != NULL )
            strncat( buf1, " ", MSL );
      }
      strncat( buf1, "'.\r\n", MSL );
   }

   strncat( buf1, ".\r\nCharacters:", MSL );
   for( rch = location->first_person; rch; rch = rch->next_in_room )
   {
      strncat( buf1, " ", MSL );
      one_argument( const_cast<char *>(rch->name.c_str()), buf );
      strncat( buf1, buf, MSL-1 );
   }

   strncat( buf1, ".\r\nObjects:   ", MSL );
   for( obj = location->first_content; obj; obj = obj->next_in_room )
   {
      strncat( buf1, " ", MSL );
      one_argument( obj->name, buf );
      strncat( buf1, buf, MSL-1 );
   }
   strncat( buf1, ".\r\n", MSL );

   for( door = 0; door < MAX_DIR; door++ )
   {
      EXIT_DATA *pexit;

      if( ( pexit = location->exit[door] ) != NULL )
      {
         snprintf( buf, MSL,
                  "Door: %d.  To: %d.  Key: %d.  Keyword: '%s'.  Description: %s\r\nExit flags: %s\r\n",
                  door,
                  pexit->to_room != NULL ? pexit->to_room->vnum : 0,
                  pexit->key, pexit->keyword, pexit->description[0] != '\0' ? pexit->description : "(none).",
                  exit_bit_name(pexit->exit_info) );
         strncat( buf1, buf, MSL-1 );
      }
   }

   if( location->first_room_affect != NULL )
   {
      for( raf = location->first_room_affect; raf != NULL; raf = raf->next )
      {
         snprintf( buf, MSL, "Room_Affect: '%s', level %d, duration %d\r\n",
                  skill_table[raf->type].name, raf->level, raf->duration );
         strncat( buf1, buf, MSL-1 );
      }
   }

   send_to_char( buf1, ch );
   return;
}



DO_FUN(do_ostat)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   AFFECT_DATA *paf;
   OBJ_DATA *obj;
   int cnt;
   int fubar;
   char *foo;
   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Ostat what?\r\n", ch );
      return;
   }

   buf1[0] = '\0';

   if( ( obj = get_obj_world( ch, arg ) ) == NULL )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );
      return;
   }

   snprintf( buf, MSL, "Name: %s.\r\n", obj->name );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "Vnum: %d.  Type: %s.\r\n", obj->pIndexData->vnum, item_type_name( obj ) );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "Short description: %s.\r\nLong description: %s\r\n", obj->short_descr, obj->long_descr );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "Wear bits: %s.\r\nExtra bits: %s\r\n",
            wear_bit_name( obj->wear_flags ), extra_bit_name( obj->extra_flags ) );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "ITEM_APPLY: %d.\r\n", obj->item_apply );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "Number: %d/%d.  Weight: %d/%d.\r\n", 1, get_obj_number( obj ), obj->weight, get_obj_weight( obj ) );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "Cost: %d.  Timer: %d.  Level: %d.\r\n", obj->cost, obj->timer, obj->level );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "Durability: %d/%d (%1.0f%%).\r\n", obj->durability, obj->max_durability, (float)(((float)obj->durability / (float)obj->max_durability) * 100) );
   strncat( buf1, buf, MSL-1 );

   snprintf(buf, MSL, "Speed %4.2f\r\n", obj->speed);
   strncat(buf1,buf,MSL-1);

   if( obj->obj_fun != NULL )
   {
      snprintf( buf, MSL, "@@WObject has objfun: @@y%s.@@N\r\n", rev_obj_fun_lookup( obj->obj_fun ) );
      strncat( buf1, buf, MSL-1 );
   }

   if( obj->pIndexData->script_name != &str_empty[0] )
   {
      snprintf( buf, MSL, "@@WObject has Lua script: @@y%s@@N\r\n", obj->pIndexData->script_name );
      strncat( buf1, buf, MSL-1 );
   }

   snprintf( buf, MSL,
            "In room: %d.  In object: %s.  Carried by: %s.  Wear_loc: %d.\r\n",
            obj->in_room == NULL ? 0 : obj->in_room->vnum,
            obj->in_obj == NULL ? "(none)" : obj->in_obj->short_descr,
            obj->carried_by == NULL ? "(none)" : obj->carried_by->name.c_str(), obj->wear_loc );
   strncat( buf1, buf, MSL-1 );

   strncat( buf1, "Item Values:\r\n", MSL );
   for( cnt = 0; cnt < 10; cnt++ )
   {
      snprintf( buf, MSL, "@@W[Value%d : @@y%6d@@W] %s",
               cnt, obj->value[cnt], rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) );
      strncat( buf1, buf, MSL-1 );
      if( is_name( "Spell", rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) ) )
      {
         fubar = obj->value[cnt];
         if( fubar < 0 || fubar > MAX_SKILL )
            snprintf( buf, MSL, "               @@R(?)@@g\r\n" );
         else
            snprintf( buf, MSL, "               @@y(%s)@@g\r\n", skill_table[fubar].name );

      }
      else if( is_name( "Liquid", rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) ) )
      {
         foo = str_dup( rev_table_lookup( tab_drink_types, obj->value[cnt] ) );
         if( foo[0] == '\0' )
            snprintf( buf, MSL, "                  @@R(INVALID!)@@g\r\n" );
         else
            snprintf( buf, MSL, "                  @@y(%s)@@g\r\n", foo );
      }
      else if( is_name( "Weapon", rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) ) )
      {
         foo = rev_table_lookup( tab_weapon_types, obj->value[cnt] );
         if( foo[0] == '\0' )
            snprintf( buf, MSL, "                  @@R(INVALID!)@@g\r\n" );
         else
            snprintf( buf, MSL, "                  @@y(%s)@@g\r\n", foo );
      }
      else
         snprintf( buf, MSL, "@@g\r\n" );
      strncat( buf1, buf, MSL-1 );
   }

   /*
    * snprintf( buf, MSL, "Values: %d %d %d %d.\r\n",
    * obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
    * strncat( buf1, buf, MSL-1 );
    */

   if( obj->first_exdesc != NULL || obj->pIndexData->first_exdesc != NULL )
   {
      EXTRA_DESCR_DATA *ed;

      strncat( buf1, "Extra description keywords: '", MSL );

      for( ed = obj->first_exdesc; ed != NULL; ed = ed->next )
      {
         strncat( buf1, ed->keyword, MSL-1 );
         if( ed->next != NULL )
            strncat( buf1, " ", MSL );
      }

      for( ed = obj->pIndexData->first_exdesc; ed != NULL; ed = ed->next )
      {
         strncat( buf1, ed->keyword, MSL-1 );
         if( ed->next != NULL )
            strncat( buf1, " ", MSL );
      }

      strncat( buf1, "'.\r\n", MSL );
   }

   for( paf = obj->first_apply; paf != NULL; paf = paf->next )
   {
      snprintf( buf, MSL, "Affects %s by %d.\r\n", affect_loc_name( paf->location ), paf->modifier );
      strncat( buf1, buf, MSL-1 );
   }

   send_to_char( buf1, ch );
   return;
}


DO_FUN(do_mstat)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   AFFECT_DATA *paf;
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Mstat whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   buf1[0] = '\0';

   snprintf( buf, MSL, "Name: %s.  Race %i\r\n", victim->name.c_str(), victim->race );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "Vnum: %d.  Sex: %s.  Room: %d.\r\n",
            IS_NPC( victim ) ? victim->npcdata->pIndexData->vnum : 0,
            victim->sex == SEX_MALE ? "male" :
            victim->sex == SEX_FEMALE ? "female" : "neutral", victim->in_room == NULL ? 0 : victim->in_room->vnum );
   strncat( buf1, buf, MSL-1 );

   if( IS_NPC( victim ) )
   {
      snprintf( buf, MSL, "Str: %d.  Int: %d.  Wis: %d.  Dex: %d.  Con: %d.\r\n",
               get_curr_str( victim ),
               get_curr_int( victim ), get_curr_wis( victim ), get_curr_dex( victim ), get_curr_con( victim ) );
      strncat( buf1, buf, MSL-1 );
   }
   else
   {
      snprintf( buf, MSL, "Str:%d/%d  Int:%d/%d  Wis:%d/%d  Dex:%d/%d Con:%d/%d.\r\n",
               get_curr_str( victim ), victim->pcdata->max_str,
               get_curr_int( victim ), victim->pcdata->max_int,
               get_curr_wis( victim ), victim->pcdata->max_wis,
               get_curr_dex( victim ), victim->pcdata->max_dex, get_curr_con( victim ), victim->pcdata->max_con );
      strncat( buf1, buf, MSL-1 );
   }

   if( !IS_NPC( victim ) )
   {
      snprintf( buf, MSL,
               "Mag: %d Cle: %d Thi:%d War:%d Psi:%d\r\n",
               victim->get_level("mag"), victim->get_level("cle"), victim->get_level("thi"), victim->get_level("war"), victim->get_level("psi") );
      strncat( buf1, buf, MSL-1 );
      snprintf( buf, MSL,
               "Sor: %d Mon: %d Ass:%d Kni:%d Nec:%d\r\n",
               victim->get_level("sor"), victim->get_level("mon"), victim->get_level("ass"), victim->get_level("kni"), victim->get_level("nec") );
      strncat( buf1, buf, MSL-1 );

      snprintf( buf, MSL, "Age: " );
      my_get_age( victim, buf );
      strncat( buf1, buf, MSL-1 );
      snprintf( buf, MSL, "   (%d Hours RL play).\r\n", my_get_hours( victim ) );
      strncat( buf1, buf, MSL-1 );

      snprintf( buf, MSL, "Class Order: %s %s %s %s %s\r\n",
               class_table[victim->pcdata->order[0]].who_name,
               class_table[victim->pcdata->order[1]].who_name,
               class_table[victim->pcdata->order[2]].who_name,
               class_table[victim->pcdata->order[3]].who_name, class_table[victim->pcdata->order[4]].who_name );
      strncat( buf1, buf, MSL-1 );


   }

   snprintf( buf, MSL, "Hp: %d/%d.  Mana: %d/%d.  Move: %d/%d.  Practices: %d.\r\n",
            victim->hit, victim->max_hit, victim->mana, victim->max_mana, victim->move, victim->max_move, IS_NPC(victim) ? 0 : victim->pcdata->practice );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL,
            "Lv: %d.  Class: %d.  Align: %d.  AC: %d.  Exp: %d.\r\n",
            victim->level, victim->p_class, victim->alignment, GET_AC( victim ), victim->exp );
   strncat( buf1, buf, MSL-1 );

   if( !IS_NPC( victim ) )
   {
      snprintf( buf, MSL, "Race: %d (%s)%s.   Clan: %d (%s).\r\n",
               victim->race,
               race_table[victim->race].race_name,
               IS_VAMP( victim ) ? "[VAMPIRE]" : "",
               victim->clan, clan_table[victim->clan].clan_abbr );
      strncat( buf1, buf, MSL-1 );
   }


   snprintf( buf, MSL, "Hitroll: %d.  Damroll: %d.  Position: %d.  Wimpy: %d.\r\n",
            GET_HITROLL( victim ), GET_DAMROLL( victim ), victim->position, victim->wimpy );
   strncat( buf1, buf, MSL-1 );

   if( IS_NPC( victim ) )
   {
      snprintf( buf, MSL, "MODIFIERS: AC: %d.  Hitroll: %d.  Damroll: %d.\r\n", victim->npcdata->ac_mod, victim->npcdata->hr_mod, victim->npcdata->dr_mod );
      strncat( buf1, buf, MSL-1 );
      snprintf( buf, MSL, "TARGET: %s\r\n", victim->target.c_str() );
      strncat( buf1, buf, MSL-1 );
      snprintf( buf, MSL, "TIMER: %d\r\n", victim->npcdata->extract_timer );
      strncat( buf1, buf, MSL-1 );
   }

   if( !IS_NPC( victim ) )
   {
      snprintf( buf, MSL, "Page Lines: %d.\r\n", victim->pcdata->pagelen );
      strncat( buf1, buf, MSL-1 );
   }

   snprintf( buf, MSL, "Fighting: %s.\r\n", victim->fighting ? victim->fighting->name.c_str() : "(none)" );
   strncat( buf1, buf, MSL-1 );

   snprintf(buf,MSL,"Fight speed: LH: %4.2f (%4.2f) RH: %4.2f (%4.2f)\r\n", get_speed(victim,SPEED_LH), victim->speed[SPEED_LH], get_speed(victim,SPEED_RH), victim->speed[SPEED_RH]);
   strncat(buf1,buf,MSL-1);


   if( IS_CASTING(victim) )
    snprintf(buf,MSL,"Casting: %s Time: %0.2f (Max: %0.2f) Pushback: %0.2f\r\n", victim->casting->arg,victim->casting->time, victim->casting->max_time, victim->casting->pushback );
   else
    snprintf(buf,MSL,"Casting: nothing Time: 0.00 (Max: 0.00) Pushback: 0.00\r\n" );
   strncat(buf1,buf,MSL-1);

   snprintf(buf,MSL,"Cooldowns: OFF: %0.2f DEF: %0.2f\r\n", victim->cooldown[COOLDOWN_OFF], victim->cooldown[COOLDOWN_DEF] );
   strncat(buf1,buf,MSL-1);

   if( !IS_NPC( victim ) )
   {
      snprintf( buf, MSL, "@@RBLOODLUST@@g: %d\r\n", victim->pcdata->super->energy );
      strncat( buf1, buf, MSL-1 );

      snprintf( buf, MSL,
               "Thirst: %d.  Full: %d.  Drunk: %d.  Saving throw: %d.\r\n",
               victim->pcdata->condition[COND_THIRST],
               victim->pcdata->condition[COND_FULL], victim->pcdata->condition[COND_DRUNK], victim->saving_throw );
      strncat( buf1, buf, MSL-1 );
   }

   snprintf( buf, MSL, "Carry number: %d.  Carry weight: %4.2f.   @@aQuest Points@@W: @@y%3d@@N\r\n",
            victim->carry_number, victim->carry_weight, IS_NPC(victim) ? 0 : victim->pcdata->quest_points );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "Age: %d.  Played: %d.  Timer: %d.\r\n",
            get_age( victim ), ( int )victim->played, victim->timer );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "Act:\r\n%s\r\n", bs_show_values( IS_NPC(victim) ? tab_mob_act : tab_player_act, victim->act ) );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "Master: %s.  Leader: %s.  Affected by: %s.\r\n",
            victim->master ? victim->master->name.c_str() : "(none)",
            victim->leader ? victim->leader->name.c_str() : "(none)", affect_bit_name( victim->affected_by ) );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "Short description: %s.\r\nLong  description: %s\r\n",
            IS_NPC(victim) ? victim->npcdata->short_descr : "(none)", !victim->long_descr.empty() ? victim->long_descr.c_str() : "(none)." );
   strncat( buf1, buf, MSL-1 );

   if( IS_NPC( victim ) )
   {
    if( victim->npcdata->spec_fun != 0 )
      strncat( buf1, "Mobile has spec fun.\r\n", MSL );
    if( victim->npcdata->pIndexData->script_name != &str_empty[0] )
    {
     snprintf( buf, MSL, "Mobile has Lua script: %s\r\n", victim->npcdata->pIndexData->script_name );
     strncat( buf1, buf, MSL-1 );
    }
   }

/*    if ( IS_NPC( victim ) 
    && IS_SET( victim->act_hunt, ACT_HUNT_MOVE )
    && victim->move_to != NO_VNUM )
    {
       snprintf( buf, MSL, "@@GMoving to room vnum: (%d) %s.@@g\r\n", victim->move_to,
       victim->movename );
       strncat( buf1, buf, MSL );
    }*/

   strcpy( buf, "@@GMoving to room vnum: (%d) %s.@@g\r\n" );
   if( victim->hunting )
      snprintf( buf1 + strlen( buf1 ), MSL, buf, victim->hunting->in_room->vnum, victim->hunting->in_room->name );
   else if( victim->hunt_obj && victim->hunt_obj->in_room )
      snprintf( buf1 + strlen( buf1 ), MSL, buf, victim->hunt_obj->in_room->vnum, victim->hunt_obj->in_room->name );


/*    if ( IS_NPC(victim) && victim->hunting != NULL)
    {
       switch( (int) victim->hunting)
       {
        case -1:
         snprintf(buf, MSL, "Hunting victim: %s (waiting)\r\n",victim->huntname);
         strncat(buf1,buf, MSL);
         break;

        case -2:
         snprintf(buf, MSL, "Returning home\r\n");
         strncat(buf1,buf, MSL);
         break;

        default:
         snprintf(buf, MSL, "Hunting victim: %s (%s)\r\n",
                victim->hunting->get_name(),
                IS_NPC(victim->hunting) ? "MOB" : "PLAYER" );
         strncat(buf1, buf, MSL);
         if (victim->huntdirs != NULL)
         {
          snprintf(buf,MSL,"Steps to victim: %i\r\n",
                             strlen(victim->huntdirs)-victim->huntdirno);
          strncat(buf1,buf,MSL);
         }
       }
    }*/

   if( victim->hunting || victim->hunt_obj )
   {
      buf[0] = '\0';
      if( victim->hunting )
         snprintf( buf + strlen( buf ), MSL, " hunting for (%s) %s",
                  ( IS_NPC( victim->hunting ) ? "mobile" : "player" ), victim->hunting->get_name() );
      if( victim->hunt_obj )
      {
         if( victim->hunting && IS_SET( victim->hunt_flags, HUNT_CR ) && victim->hunt_obj->item_type == ITEM_CORPSE_PC )
            strncat( buf, " to return a corpse", MSL );
         else
            snprintf( buf + strlen( buf ), MSL, " looking for (object) %s", victim->hunt_obj->short_descr );
      }
      if( IS_NPC( victim ) && IS_SET( victim->hunt_flags, HUNT_MERC | HUNT_CR ) && victim->npcdata->hunt_for )
         snprintf( buf + strlen( buf ), MSL, ", employed by %s", victim->npcdata->hunt_for->get_name() );
      strncat( buf, ".\r\n", MSL );
      buf[1] = UPPER( buf[1] );
      strncat( buf1, buf + 1, MSL-1 );
   }
   else if( !victim->searching.empty() )
   {
      snprintf( buf, MSL, "Searching for %s.\r\n", victim->searching.c_str() );
      strncat( buf1, buf, MSL-1 );
   }

   for( paf = victim->first_affect; paf != NULL; paf = paf->next )
   {
      snprintf( buf, MSL,
               "Spell: '%s' modifies %s by %d for %d hours with bits %s.\r\n",
               skill_table[( int )paf->type].name,
               affect_loc_name( paf->location ), paf->modifier, paf->duration, affect_bit_name( paf->bitvector ) );
      strncat( buf1, buf, MSL-1 );
   }

   send_to_char( buf1, ch );
   return;
}

DO_FUN(do_olmsg)
{
 if( argument[0] == '\0' )
 {
  send_to_char("Syntax: olmsg <string> or olmsg none for default.\r\nMessage needs to contain $n for you and $p for the object.\r\n",ch);
  return;
 }
 if( !IS_NPC( ch ) )
 {
  smash_tilde( argument );
  free_string(ch->pcdata->load_msg);
  if( !str_cmp(argument,"none") )
   ch->pcdata->load_msg = str_dup("");
  else
   ch->pcdata->load_msg = str_dup(argument);
  send_to_char("Ok.\r\n",ch);
 }
 return;
}

DO_FUN(do_ofindlev)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   char arg2[MSL];
   OBJ_INDEX_DATA *pObjIndex;
   int vnum;
   int nMatch;
   bool fAll;
   bool found;
   int level;
   int level_top;
   int objlev;
   bool mailme = FALSE;
   if( is_name( "mailme", argument ) )
      mailme = TRUE;
   argument = one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Ofindlev what lev.?\r\n", ch );
      return;
   }
   level = is_number( arg ) ? atoi( arg ) : 0;

   argument = one_argument( argument, arg2 );
   if( ( arg2[0] == '\0' ) || ( !is_number( arg2 ) ) )
   {
      level_top = level;
   }
   else
   {
      level_top = atoi( arg2 );
   }
   buf1[0] = '\0';
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;


   /*
    * Yeah, so iterating over all vnum's takes 10,000 loops.
    * Get_mob_index is fast, and I don't feel like threading another link.
    * Do you?
    * -- Furey
    */
   for( vnum = 0; nMatch < static_cast<int>(obj_index_list.size()); vnum++ )
   {
      if( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
      {
         nMatch++;
         if( ( fAll ) || ( ( pObjIndex->level >= level ) && ( pObjIndex->level <= level_top ) ) )

         {
            found = TRUE;
            objlev = pObjIndex->level;

            if( IS_OBJ_STAT(pObjIndex,ITEM_EXTRA_REMORT) )
            {
               snprintf( buf, MSL, "\r\n(@@mREMORT@@N) [%3d] [%5d] %s", pObjIndex->level,
                        pObjIndex->vnum, capitalize( pObjIndex->short_descr ) );
               strncat( buf1, buf, MSL-1 );
            }
            else
            {
               snprintf( buf, MSL, "\r\n(@@aMORTAL@@N) [%3d] [%5d] %s", pObjIndex->level,
                        pObjIndex->vnum, capitalize( pObjIndex->short_descr ) );
               strncat( buf1, buf, MSL-1 );
            }
         }
      }
   }
   strncat( buf1, "\r\n", MSL );
   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );
      return;
   }
   snprintf( buf, MSL, "Ofindlev report for level range %d to %d", level, level_top );
   send_rep_out( ch, buf1, mailme, buf );
   return;
}

DO_FUN(do_mfind)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   MOB_INDEX_DATA *pMobIndex;
   int vnum;
   int nMatch;
   bool fAll;
   bool found;
   bool mailme = FALSE;
   if( is_name( "mailme", argument ) )
      mailme = TRUE;
   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Mfind whom?\r\n", ch );
      return;
   }

   snprintf( buf1, MSL, "%s", "  Vnum   Lvl    Mob\r\n" );
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   /*
    * Yeah, so iterating over all vnum's takes 10,000 loops.
    * Get_mob_index is fast, and I don't feel like threading another link.
    * Do you?
    * -- Furey
    */
   for( vnum = 0; nMatch < static_cast<int>(mob_index_list.size()); vnum++ )
   {
      if( ( pMobIndex = get_mob_index( vnum ) ) != NULL )
      {
         nMatch++;
         if( fAll || is_name( arg, pMobIndex->player_name ) )
         {
            found = TRUE;
            snprintf( buf, MSL, "[%5d] [%3d] %s\r\n", pMobIndex->vnum, pMobIndex->level, capitalize( pMobIndex->short_descr ) );
            strncat( buf1, buf, MSL-1 );
         }
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );
      return;
   }

   snprintf( buf, MSL, "Mfind report for %s", arg );
   send_rep_out( ch, buf1, mailme, buf );
   return;
}

DO_FUN(do_mfindlev)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   char arg2[MSL];
   MOB_INDEX_DATA *pMobIndex;
   int vnum;
   int nMatch;
   bool fAll;
   bool found;
   int level, level_top;
   int perkills, moblev;
   bool mailme = FALSE;
   if( is_name( "mailme", argument ) )
      mailme = TRUE;

   argument = one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Mfindlev what lev.?\r\n", ch );
      return;
   }

   buf1[0] = '\0';
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;
   level = is_number( arg ) ? atoi( arg ) : 0;
   if( ( arg2[0] == '\0' ) || ( !is_number( arg2 ) ) )
   {
      level_top = level;
   }
   else
   {
      level_top = atoi( arg2 );
   }

   /*
    * Yeah, so iterating over all vnum's takes 10,000 loops.
    * Get_mob_index is fast, and I don't feel like threading another link.
    * Do you?
    * -- Furey
    */
   snprintf(buf1,MSL,"[Kls] [Lvl] [Vnum ] [Name ]\r\n");
   for( vnum = 0; nMatch < static_cast<int>(mob_index_list.size()); vnum++ )
   {
      if( ( pMobIndex = get_mob_index( vnum ) ) != NULL )
      {
         nMatch++;
         if( ( fAll ) || ( ( pMobIndex->level >= level ) && ( pMobIndex->level <= level_top ) ) )
         {
            found = TRUE;
            moblev = pMobIndex->level;
            if( kill_table[moblev].killed == 0 )
               perkills = 0;
            else
               perkills = ( pMobIndex->killed * 100 ) / ( kill_table[moblev].killed );

            snprintf( buf, MSL, "(%3d) [%3d] [%5d] %s\r\n",
                     perkills, pMobIndex->level, pMobIndex->vnum, capitalize( pMobIndex->short_descr ) );
            strncat( buf1, buf, MSL-1 );
         }
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );
      return;
   }

   snprintf( buf, MSL, "Mfindlev report for level range %d to %d", level, level_top );
   send_rep_out( ch, buf1, mailme, buf );
   return;
}


DO_FUN(do_ofind)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   OBJ_INDEX_DATA *pObjIndex;
   int vnum;
   int nMatch;
   bool fAll;
   bool found;
   bool mailme = FALSE;
   if( is_name( "mailme", argument ) )
      mailme = TRUE;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Ofind what?\r\n", ch );
      return;
   }

   snprintf( buf1, MSL, "%s", " Vnum  Lvl  Flag    Item\r\n" );
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   /*
    * Yeah, so iterating over all vnum's takes 10,000 loops.
    * Get_obj_index is fast, and I don't feel like threading another link.
    * Do you?
    * -- Furey
    */
   for( vnum = 0; nMatch < static_cast<int>(obj_index_list.size()); vnum++ )
   {
      if( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
      {
         nMatch++;
         if( fAll || is_name( arg, pObjIndex->name ) )
         {
            found = TRUE;
            snprintf( buf, MSL, "[%5d] [%3d] %s %s\r\n",
                     pObjIndex->vnum, pObjIndex->level,
                     ( IS_OBJ_STAT(pObjIndex,ITEM_EXTRA_REMORT) ?
                       "@@mRemort@@N" : "@@aMortal@@N" ), capitalize( pObjIndex->short_descr ) );
            strncat( buf1, buf, MSL-1 );
         }
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );
      return;
   }

   snprintf( buf, MSL, "Ofind report for %s", arg );
   send_rep_out( ch, buf1, mailme, buf );
   return;
}


DO_FUN(do_mwhere)
{
   char buf[MSL];
   char arg[MSL];
   CHAR_DATA *victim;
   list<CHAR_DATA *>::iterator li;
   bool found;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Mwhere whom? or int to see the intelligent mobs.\r\n", ch );
      return;
   }
   if( !str_cmp( arg, "int" ) )
   {
      for( li = char_list.begin(); li != char_list.end(); li++ )
      {
         victim = *li;
         if( AI_MOB(victim) && victim->in_room != NULL )
         {
            snprintf( buf, MSL, "[%5d] %-20s [%5d] %-30s\r\n",
                     victim->npcdata->pIndexData->vnum, victim->get_name(), victim->in_room->vnum, victim->in_room->name );
            send_to_char( buf, ch );
         }

      }
      return;
   }

   found = FALSE;
   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      victim = *li;
      if( IS_NPC( victim ) && victim->in_room != NULL && is_name( arg, const_cast<char *>(victim->name.c_str()) ) )
      {
         found = TRUE;
         snprintf( buf, MSL, "[%5d] %-20s [%5d] %-30s\r\n",
                  victim->npcdata->pIndexData->vnum, victim->get_name(), victim->in_room->vnum, victim->in_room->name );
         send_to_char( buf, ch );
      }
   }

   if( !found )
   {
      send_to_char( "Nope, couldn't find it.\r\n", ch );
      return;
   }

   return;
}



DO_FUN(do_reboo)
{
   send_to_char( "If you want to REBOOT, spell it out.\r\n", ch );
   return;
}



DO_FUN(do_reboot)
{
   char buf[MSL];

   build_save_flush(  );

   if( saving_area )
   {
      send_to_char( "Please wait until area saving is complete.\n", ch );
      return;
   }

   save_mudinfo();

   snprintf( buf, MSL, "Reboot by %s.", ch->name.c_str() );
   do_echo( ch, buf );
   merc_down = TRUE;
   return;
}



DO_FUN(do_shutdow)
{
   send_to_char( "If you want to SHUTDOWN, spell it out.\r\n", ch );
   return;
}



DO_FUN(do_shutdown)
{
   char buf[MSL];

   build_save_flush(  );

   if( saving_area )
   {
      send_to_char( "Please wait until area saving is complete.\n", ch );
      return;
   }

   save_mudinfo();

   snprintf( buf, MSL, "Shutdown by %s.", ch->name.c_str() );
   append_file( ch, SHUTDOWN_FILE, buf );
   strncat( buf, "\r\n", MSL );
   do_echo( ch, buf );
   merc_down = TRUE;
   return;
}



DO_FUN(do_snoop)
{
   char arg[MSL];
   DESCRIPTOR_DATA *d;
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Snoop whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( victim->desc == NULL )
   {
      send_to_char( "No descriptor to snoop.\r\n", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "Cancelling all snoops.\r\n", ch );
      for( d = first_desc; d != NULL; d = d->next )
      {
         if( d->snoop_by == ch->desc )
            d->snoop_by = NULL;
      }
      return;
   }

   if( victim->desc->snoop_by != NULL )
   {
      send_to_char( "Busy already.\r\n", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   if( ch->desc != NULL )
   {
      for( d = ch->desc->snoop_by; d != NULL; d = d->snoop_by )
      {
         if( d->character == victim || d->original == victim )
         {
            send_to_char( "No snoop loops.\r\n", ch );
            return;
         }
      }
   }

   victim->desc->snoop_by = ch->desc;
   send_to_char( "Ok.\r\n", ch );
   return;
}



DO_FUN(do_switch)
{
   char arg[MSL];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Switch into whom?\r\n", ch );
      return;
   }

   if( ch->desc == NULL )
      return;

   if( ch->desc->original != NULL )
   {
      send_to_char( "You are already switched.\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "Ok.\r\n", ch );
      return;
   }

   /*
    * Pointed out by Da Pub (What Mud)
    */
   if( !IS_NPC( victim ) )
   {
      send_to_char( "You cannot switch into a player!\r\n", ch );
      return;
   }

   if( victim->desc != NULL )
   {
      send_to_char( "Character in use.\r\n", ch );
      return;
   }

   ch->desc->character = victim;
   ch->desc->original = ch;
   victim->desc = ch->desc;
   ch->desc = NULL;
   send_to_char( "Ok.\r\n", victim );
   return;
}



DO_FUN(do_return)
{
   if( ch->desc == NULL )
      return;

   if( ch->desc->original == NULL )
   {
      send_to_char( "You aren't switched.\r\n", ch );
      return;
   }

   send_to_char( "You return to your original body.\r\n", ch );
   ch->desc->character = ch->desc->original;
   ch->desc->original = NULL;
   ch->desc->character->desc = ch->desc;
   ch->desc = NULL;
   return;
}



DO_FUN(do_mload)
{
   char arg[MSL];
   MOB_INDEX_DATA *pMobIndex;
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' || !is_number( arg ) )
   {
      send_to_char( "Syntax: mload <vnum>.\r\n", ch );
      return;
   }

   if( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
   {
      send_to_char( "No mob has that vnum.\r\n", ch );
      return;
   }

   victim = create_mobile( pMobIndex );
   char_to_room( victim, ch->in_room );
   act( "$n has created $N!", ch, NULL, victim, TO_ROOM );
   send_to_char( "Ok.\r\n", ch );
   return;
}



DO_FUN(do_oload)
{
   char arg1[MSL];
   char arg2[MSL];
   OBJ_INDEX_DATA *pObjIndex;
   OBJ_DATA *obj;
   int level;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' || !is_number( arg1 ) )
   {
      send_to_char( "Syntax: oload <vnum> <level>.\r\n", ch );
      return;
   }

   if( arg2[0] == '\0' )
   {
      level = get_trust( ch );
   }
   else
   {
      /*
       * New feature from Alander.
       */
      if( !is_number( arg2 ) )
      {
         send_to_char( "Syntax: oload <vnum> <level>.\r\n", ch );
         return;
      }
      level = atoi( arg2 );
      if( level < 0 || level > get_trust( ch ) )
      {
         send_to_char( "Limited to your trust level.\r\n", ch );
         return;
      }
   }

   if( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
   {
      send_to_char( "No object has that vnum.\r\n", ch );
      return;
   }

   if( IS_OBJ_STAT(pObjIndex,ITEM_EXTRA_CLAN_EQ) && ( ch->level != MAX_LEVEL ) )
   {
      send_to_char( "Only Creators can OLOAD clan equipment.\r\n", ch );
      return;
   }

   obj = create_object( pObjIndex, level );
   if( CAN_WEAR( obj, ITEM_TAKE ) )
   {
      if( strlen(ch->pcdata->load_msg) > 0 )
       act(ch->pcdata->load_msg,ch,obj,NULL,TO_ROOM);
      else
       act( "$n @@mgestures majestically, and@@N $p @@mappears with a crash of @@WTHUNDER!!@@N", ch, obj, NULL, TO_ROOM );
      obj_to_char( obj, ch );
   }
   else
   {
      if( strlen(ch->pcdata->load_msg) > 0 )
       act(ch->pcdata->load_msg,ch,obj,NULL,TO_ROOM);
      else
       act( "$n @@mgestures, and a @@N$p@@M appears with a thunderous crash@@N!!!", ch, obj, NULL, TO_ROOM );
      obj_to_room( obj, ch->in_room );
   }
   send_to_char( "Ok.\r\n", ch );
   return;
}



DO_FUN(do_purge)
{
   char arg[MSL];
   CHAR_DATA *victim;
   OBJ_DATA *obj;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      /*
       * 'purge' 
       */
      CHAR_DATA *vnext;
      OBJ_DATA *obj_next;

      for( victim = ch->in_room->first_person; victim != NULL; victim = vnext )
      {
         vnext = victim->next_in_room;
         if( IS_NPC( victim ) && victim != ch )
            extract_char( victim, TRUE );
      }

      for( obj = ch->in_room->first_content; obj != NULL; obj = obj_next )
      {
         obj_next = obj->next_in_room;
         extract_obj( obj );
      }

      act( "$n cleanses the room with Holy fire!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "Your burst of Holy fire cleanses the room!\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) )
   {
      send_to_char( "Not on PC's.\r\n", ch );
      return;
   }

   act( "$n obliterates $N with Holy fire!", ch, NULL, victim, TO_NOTVICT );
   act( "You obliterate $N with Holy fire!", ch, NULL, victim, TO_CHAR );
   extract_char( victim, TRUE );
   return;
}

DO_FUN(do_trust)
{
   char arg1[MSL];
   char arg2[MSL];
   char buf[MSL];
   CHAR_DATA *victim;
   int level;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
   {
      send_to_char( "Syntax: trust <char> <level>.\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg1 ) ) == NULL )
   {
      send_to_char( "That player is not here.\r\n", ch );
      return;
   }

   if( ( level = atoi( arg2 ) ) < 0 || level > MAX_LEVEL )
   {
      snprintf( buf, MSL, "Level must be 0 (reset) or 1 to %d.\r\n", MAX_LEVEL );
      send_to_char( buf, ch );
      return;
   }

   if( level > get_trust( ch ) )
   {
      send_to_char( "Limited to your trust.\r\n", ch );
      return;
   }

   victim->trust = level;
   return;
}



DO_FUN(do_restore)
{
   char arg[MSL];
   CHAR_DATA *victim;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Restore whom?\r\n", ch );
      return;
   }


   if( !str_cmp( arg, "all" ) )
   {
      /*
       * then loop through all players and restore them 
       */
      CHAR_DATA *vch;
      list<CHAR_DATA *>::iterator li;

      for( li = char_list.begin(); li != char_list.end(); li++ )
      {
         vch = *li;
         if( !IS_NPC( vch ) )
         {
            if( IS_IMMORTAL( vch ) && ( vch != ch ) )
            {
               act( "Everyone has been restored by $n.", ch, NULL, vch, TO_VICT );
            }
            else
            {
               vch->hit = vch->max_hit;
               vch->mana = vch->max_mana;
               vch->move = vch->max_move;
               update_pos( vch );
               act( "$n kindly restores you.", ch, NULL, vch, TO_VICT );
            }
         }
      }

      send_to_char( "Everyone has been restored.\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here!\r\n", ch );
      return;
   }

   victim->hit = victim->max_hit;
   victim->mana = victim->max_mana;
   victim->move = victim->max_move;
   update_pos( victim );
   act( "$n kindly restores you.", ch, NULL, victim, TO_VICT );
   send_to_char( "Ok.\r\n", ch );
   return;
}



DO_FUN(do_freeze)
{
   char arg[MSL];
   char buf[MSL];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Freeze whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   victim->act.flip(ACT_FREEZE);

   if( !victim->act.test(ACT_FREEZE) )
   {
      send_to_char( "You can play again.\r\n", victim );
      send_to_char( "FREEZE removed.\r\n", ch );
   }
   else
   {
      send_to_char( "You can't do ANYthing!\r\n", victim );
      snprintf( buf, MSL, "You have been FROZEN by %s!!\r\n", ch->name.c_str() );
      send_to_char( buf, victim );
      send_to_char( "Freeze set.\r\n", ch );

      snprintf( buf, MSL, "%s has been FROZEN by %s.\r\n", victim->name.c_str(), ch->name.c_str() );
      notify( buf, ch->level + 1 );
   }

   save_char_obj( victim );

   return;
}



DO_FUN(do_log)
{
   char arg[MSL];
   CHAR_DATA *victim;

// If anyone has a way to do this with the character not logged in,
// please let zenithar@mindspring.com know :)

//    DESCRIPTOR_DATA d;
//    bool found = FALSE;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Log whom?\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "all" ) )
   {
      if( fLogAll )
      {
         fLogAll = FALSE;
         send_to_char( "Log ALL off.\r\n", ch );
      }
      else
      {
         fLogAll = TRUE;
         send_to_char( "Log ALL on.\r\n", ch );
      }
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
//      found = load_char_obj( &d, arg, TRUE );

//      if (!found)
      {
         char buf[MSL];
//        snprintf( buf, MSL, "No pFile found for '%s'.\r\n", capitalize( arg ) );
         snprintf( buf, MSL, "%s is not online.\r\n", capitalize( arg ) );
         send_to_char( buf, ch );
//        delete( d.character );
         return;
      }

//      victim = d.character;
//      d.character = NULL;
//      victim->desc = NULL;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   /*
    * No level check, gods can log anyone.
    */
   victim->act.flip(ACT_LOG);

   if( victim->act.test(ACT_LOG) )
      send_to_char( "LOG set.\r\n", ch );
   else
      send_to_char( "LOG removed.\r\n", ch );

   return;
}


DO_FUN(do_noemote)
{
   char arg[MSL];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Noemote whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   victim->act.flip(ACT_NO_EMOTE);

   if( !victim->act.test(ACT_NO_EMOTE) )
   {
      send_to_char( "You can emote again.\r\n", victim );
      send_to_char( "NO_EMOTE removed.\r\n", ch );
   }
   else
   {
      send_to_char( "Your ability to emote has been removed!\r\n", victim );
      send_to_char( "NO_EMOTE set.\r\n", ch );
   }

   return;
}



DO_FUN(do_notell)
{
   char arg[MSL];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Notell whom?", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   victim->act.flip(ACT_NO_TELL);

   if( !victim->act.test(ACT_NO_TELL) )
   {
      send_to_char( "You can tell again.\r\n", victim );
      send_to_char( "NO_TELL removed.\r\n", ch );
   }
   else
   {
      send_to_char( "You now can not use the tell command!\r\n", victim );
      send_to_char( "NO_TELL set.\r\n", ch );
   }

   return;
}



DO_FUN(do_silence)
{
   char arg[MSL];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Silence whom?", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   victim->act.flip(ACT_SILENCE);

   if( !victim->act.test(ACT_SILENCE) )
   {
      send_to_char( "You can use channels again.\r\n", victim );
      send_to_char( "SILENCE removed.\r\n", ch );
   }
   else
   {
      send_to_char( "You can't use channels!\r\n", victim );
      send_to_char( "SILENCE set.\r\n", ch );
   }

   return;
}

DO_FUN(do_nopray)
{
   /*
    * Remove victim's ability to use pray channel.. -S- 
    */

   char arg[MSL];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "NoPray whom?", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   victim->act.flip(ACT_NO_PRAY);

   if( !victim->act.test(ACT_NO_PRAY) )
   {
      send_to_char( "You can use 'PRAY' again.\r\n", victim );
      send_to_char( "NOPRAY removed.\r\n", ch );
   }
   else
   {
      send_to_char( "You can't use 'PRAY'!\r\n", victim );
      send_to_char( "NOPRAY set.\r\n", ch );
   }

   return;
}

DO_FUN(do_peace)
{
   CHAR_DATA *rch;

   for( rch = ch->in_room->first_person; rch != NULL; rch = rch->next_in_room )
   {
      if( rch->fighting != NULL )
         stop_fighting( rch, TRUE );
   }

   send_to_char( "Ok.\r\n", ch );
   return;
}



DO_FUN(do_ban)
{
   char buf[MSL];
   char arg[MSL];
   char arg2[MSL];
   char buf2[MSL];

   BAN_DATA *pban;
   list<BAN_DATA *>::iterator li;
   buf[0] = '\0';
   buf2[0] = '\0';


   if( IS_NPC( ch ) )
      return;

   argument = one_argument( argument, arg );
   one_argument( argument, arg2 );


   if( arg[0] == '\0' )
   {
      strcpy( buf, "Banned sites:\r\n" );
      for( li = ban_list.begin(); li != ban_list.end(); li++ )
      {
         pban = *li;
         strncat( buf, pban->name, MSL-1 );
         snprintf( buf2, MSL, ( pban->newbie ? " Newbies" : " All" ) );
         strncat( buf, buf2, MSL-1 );
         snprintf( buf2, MSL, "  Banned by: %s", pban->banned_by );
         strncat( buf, buf2, MSL-1 );
         strncat( buf, "\r\n", MSL );
      }
      send_to_char( buf, ch );
      return;
   }

   for( li = ban_list.begin(); li != ban_list.end(); li++ )
   {
      pban = *li;
      if( !str_cmp( arg, pban->name ) )
      {
         send_to_char( "That site is already banned!\r\n", ch );
         return;
      }
   }

   pban = new BAN_DATA;
   if( !str_cmp( arg2, "newbie" ) )
      pban->newbie = TRUE;
   else
      pban->newbie = FALSE;

   pban->name = str_dup( arg );
   pban->banned_by = str_dup(ch->name.c_str());
   save_bans(  );
   send_to_char( "Ok.\r\n", ch );
   return;
}



DO_FUN(do_allow)
{
   char arg[MSL];
   BAN_DATA *curr;
   list<BAN_DATA *>::iterator li;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Remove which site from the ban list?\r\n", ch );
      return;
   }

   for( li = ban_list.begin(); li != ban_list.end(); li++ )
   {
      curr = *li;
      if( !str_cmp( arg, curr->name ) )
      {
         ban_list.remove(curr);
         delete curr;
         send_to_char( "Ok.\r\n", ch );
         save_bans(  );
         return;
      }
   }

   send_to_char( "Site is not banned.\r\n", ch );
   return;
}


DO_FUN(do_deathmatc)
{
   send_to_char( "If you want to start or end a DEATHMATCH, say it in full!\r\n", ch );
   return;
}


DO_FUN(do_deathmatch)
{
   /*
    * This basically locks the mud, and stops saving.
    * * PKing is allowed with no limits ;)
    * * Only allow HIGHEST level imms to use...
    */

   deathmatch = !deathmatch;


   if( IS_NPC( ch ) )
   {
      send_to_char( "Deathmatch is not a NPC command.\r\n", ch );
      return;
   }

   if( deathmatch )
   {
      CHAR_DATA *vch;
      list<CHAR_DATA *>::iterator li;

      for( li = char_list.begin(); li != char_list.end(); li++ )
      {
         vch = *li;
         if( !IS_NPC( vch ) )
            do_help( vch, "dm" );
      }

   }
   else
   {
      send_to_char( "Deathmatch ended...\r\n", ch );
      info( "+=========================+", 1 );
      info( "| DEATHMATCH HAS ENDED!!! |", 1 );
      info( "+=========================+", 1 );
   }
   return;
}

DO_FUN(do_wizlock)
{
   char buf[MSL];
   wizlock = !wizlock;
   sysdata.w_lock = wizlock;
   save_sysdata(  );

   if( wizlock )
   {
      send_to_char( "Game wizlocked.\r\n", ch );
      snprintf( buf, MSL, "%s wizlocks ACK! Mud.\r\n", ch->name.c_str() );
   }
   else
   {
      send_to_char( "Game un-wizlocked.\r\n", ch );
      snprintf( buf, MSL, "%s un-wizlocks ACK! Mud.\r\n", ch->name.c_str() );
   }
   notify( buf, get_trust( ch ) );
   return;
}



DO_FUN(do_slookup)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   int sn;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Slookup what?\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "all" ) )
   {
      buf1[0] = '\0';
      for( sn = 0; sn < MAX_SKILL; sn++ )
      {
         if( skill_table[sn].name == NULL )
            break;
         snprintf( buf, MSL, "Sn: %4d Slot: %4d Skill/spell: '%s'\r\n", sn, skill_table[sn].slot, skill_table[sn].name );
         strncat( buf1, buf, MSL-1 );
      }
      send_to_char( buf1, ch );
   }
   else
   {
      if( ( sn = skill_lookup( arg ) ) < 0 )
      {
         send_to_char( "No such skill or spell.\r\n", ch );
         return;
      }

      snprintf( buf, MSL, "Sn: %4d Slot: %4d Skill/spell: '%s'\r\n", sn, skill_table[sn].slot, skill_table[sn].name );
      send_to_char( buf, ch );
   }

   return;
}



DO_FUN(do_sset)
{
   char arg1[MSL];
   char arg2[MSL];
   char arg3[MSL];
   CHAR_DATA *victim;
   int value;
   int sn;
   bool fAll;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   argument = one_argument( argument, arg3 );

   if( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
   {
      send_to_char( "Syntax: sset <victim> <skill> <value>\r\n", ch );
      send_to_char( "or:     sset <victim> all     <value>\r\n", ch );
      send_to_char( "Skill being any skill or spell.\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   fAll = !str_cmp( arg2, "all" );

   if( fAll && ch->level != 85 )
   {
      send_to_char( "Only Creators may SSET all.\r\n", ch );
      return;
   }

   sn = 0;
   if( !fAll && ( sn = skill_lookup( arg2 ) ) < 0 )
   {
      send_to_char( "No such skill or spell.\r\n", ch );
      return;
   }

   /*
    * Snarf the value.
    */
   if( !is_number( arg3 ) )
   {
      send_to_char( "Value must be numeric.\r\n", ch );
      return;
   }

   value = atoi( arg3 );
   if( value < 0 || value > 99 )
   {
      send_to_char( "Value range is 0 to 99.\r\n", ch );
      return;
   }

   if( fAll )
   {
      for( sn = 0; sn < MAX_SKILL; sn++ )
      {
         if( skill_table[sn].name != NULL )
            victim->pcdata->learned[sn] = value;
      }
   }
   else
   {
      victim->pcdata->learned[sn] = value;
   }

   return;
}



DO_FUN(do_mset)
{
   char arg1[MSL];
   char arg2[MSL];
   char arg3[MSL];
   char buf[MSL];
   CHAR_DATA *victim;
   int value, max;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   if( str_cmp( arg1, "order" ) )
      strcpy( arg3, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
   {
      send_to_char( "Syntax: mset <victim> <field>  <value>\r\n", ch );
      send_to_char( "or:     mset <victim> <string> <value>\r\n", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "Field being one of:\r\n", ch );
      send_to_char( "  str int wis dex con sex class level exp\r\n", ch );
      send_to_char( "  hp mana move practice align mquest_time\r\n", ch );
      send_to_char( "  thirst drunk full race hunt flags aff recall\r\n", ch );
      send_to_char( "  order (Sumpremes and higher only)\r\n", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "String being one of:\r\n", ch );
      send_to_char( "  name short long description title spec\r\n", ch );
      send_to_char( " entry exit rulerrank\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   /*
    * Snarf the value (which need not be numeric).
    */
   value = is_number( arg3 ) ? atoi( arg3 ) : -1;

   /*
    * Set something.
    */

   if( !str_cmp( arg2, "order" ) )
   {

      int cnt;
      int p_class[MAX_CLASS];
      int parity[MAX_CLASS];
      int foo;
      bool ok = TRUE;
      char arg[MSL];


      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPCs!\r\n", ch );
         return;
      }

      if( get_trust( ch ) < 84 )
      {
         send_to_char( "Only a Supreme or above may use this option.\r\n", ch );
         return;
      }
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         parity[cnt] = -1;


      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      {
         argument = one_argument( argument, arg );
         if( arg[0] == '\0' )
         {
            ok = FALSE;
            break;
         }
         for( foo = 0; foo < MAX_CLASS; foo++ )
            if( !str_cmp( arg, class_table[foo].who_name ) )
            {
               p_class[cnt] = foo;
               parity[foo] = 1;
               break;
            }
         if( foo == MAX_CLASS )
         {
            ok = FALSE;
            break;
         }
      }

      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         if( parity[cnt] == -1 )
            ok = FALSE;

      if( !ok )
      {
         send_to_char( "Must be 5 3-letter abbrev for different classes.\r\n", ch );
         return;
      }

      /*
       * Copy classes to pcdata 
       */
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
       victim->pcdata->order[cnt] = p_class[cnt];

      send_to_char( "Your classes have been re-ordered.\r\n", victim );
      send_to_char( "Done.\r\n", ch );
      return;
   }



   if( !str_cmp( arg2, "intel" ) )
   {
      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PCs.\r\n", ch );
         return;
      }

      victim->act.flip(ACT_INTELLIGENT);

      if( !victim->act.test(ACT_INTELLIGENT) )
         send_to_char( "Removing intelligence.\r\n", ch );
      else
         send_to_char( "Adding intelligence.\r\n", ch );
   }

   if( !str_cmp( arg2, "str" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      max = victim->pcdata->max_str;

      if( value < 3 || value > max )
      {
         snprintf( buf, MSL, "Strength range is 3 to %d.\r\n", max );
         send_to_char( buf, ch );
         return;
      }

      victim->pcdata->perm_str = value;
      return;
   }

   if( !str_cmp( arg2, "int" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      max = victim->pcdata->max_int;

      if( value < 3 || value > max )
      {
         snprintf( buf, MSL, "Intelligence range is 3 to %d.\r\n", max );
         send_to_char( buf, ch );
         return;
      }

      victim->pcdata->perm_int = value;
      return;
   }

   if( !str_cmp( arg2, "wis" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      max = victim->pcdata->max_wis;

      if( value < 3 || value > max )
      {
         snprintf( buf, MSL, "Wisdom range is 3 to %d.\r\n", max );
         send_to_char( buf, ch );
         return;
      }

      victim->pcdata->perm_wis = value;
      return;
   }

   if( !str_cmp( arg2, "dex" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      max = victim->pcdata->max_dex;

      if( value < 3 || value > max )
      {
         snprintf( buf, MSL, "Dexterity range is 3 to %d.\r\n", max );
         send_to_char( buf, ch );
         return;
      }

      victim->pcdata->perm_dex = value;
      return;
   }

   if( !str_cmp( arg2, "con" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      max = victim->pcdata->max_con;

      if( value < 3 || value > max )
      {
         snprintf( buf, MSL, "Constitution range is 3 to %d.\r\n", max );
         send_to_char( buf, ch );
         return;
      }

      victim->pcdata->perm_con = value;
      return;
   }

   if( !str_cmp( arg2, "sex" ) )
   {
      if( value < 0 || value > 2 )
      {
         send_to_char( "Sex range is 0 to 2.\r\n", ch );
         return;
      }
      victim->sex = value;
      if( !IS_NPC(victim) )
       victim->pcdata->login_sex = value;
      return;
   }

   if( !str_cmp( arg2, "class" ) )
   {
      if( value < 0 || value >= MAX_CLASS )
      {
         snprintf( buf, MSL, "Class range is 0 to %d.\n", MAX_CLASS - 1 );
         send_to_char( buf, ch );
         return;
      }
      victim->p_class = value;
      return;
   }

   if( !str_cmp( arg2, "race" ) )
   {
      if( value < 0 || value >= MAX_RACE )
      {
         snprintf( buf, MSL, "Race range is 0 to %d.\n", MAX_RACE - 1 );
         send_to_char( buf, ch );
         return;
      }
      if( ( !IS_NPC( victim ) ) && ( race_table[value].player_allowed == FALSE ) )
      {
         send_to_char( "That is an NPC ONLY race.\r\n", ch );
         return;
      }
      victim->race = value;
      return;
   }
   if( !str_cmp( arg2, "rulerrank" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on Npcs.\r\n", ch );
         return;
      }
      if( value < 0 || value > 5 )
      {
         snprintf( buf, MSL, "%s", "Ruler Rank is 1 to 5.\r\n" );
         send_to_char( buf, ch );
         return;
      }
      victim->pcdata->ruler_rank = value;
      return;
   }

   if( !str_cmp( arg2, "hunt" ) )
   {

      CHAR_DATA *hunted = 0;

      if( ch->level < 84 )
      {
         send_to_char( "Currently restricted to reduce abuses.\r\n", ch );
         return;
      }


      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PC's.\r\n", ch );
         return;
      }

      if( str_cmp( arg3, "." ) )
      {
         if( ( hunted = get_char_world( victim, arg3 ) ) == NULL )
         {
            send_to_char( "Mob couldn't locate the victim to hunt.\r\n", ch );
            return;
         }
         if( !set_hunt( victim, NULL, hunted, NULL, 0, 0 ) )
/*           if ( ! make_hunt(victim,hunted) )*/
            send_to_char( "Mob could not hunt victim.\r\n", ch );
      }
      else
      {
         victim->hunting = NULL;
         victim->hunt_obj = NULL;
         if( IS_NPC(victim) )
          victim->npcdata->hunt_for = NULL;
         if( !victim->searching.empty() )
            victim->searching.clear();
         victim->hunt_flags = victim->npcdata->pIndexData->hunt_flags;
      }

      return;
   }

   if( !str_cmp( arg2, "level" ) )
   {
      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PC's.\r\n", ch );
         return;
      }

      if( value < 0 || value > 100 )
      {
         send_to_char( "Level range is 0 to 100.\r\n", ch );
         return;
      }
      victim->level = value;
      return;
   }

   if( !str_cmp( arg2, "timer" ) )
   {
      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PC's.\r\n", ch );
         return;
      }

      victim->npcdata->extract_timer = value;
      return;
   }

   if( !str_cmp( arg2, "hp" ) )
   {
      if( value < -10 || value > 30000 )
      {
         send_to_char( "Hp range is -10 to 30,000 hit points.\r\n", ch );
         return;
      }
      victim->max_hit = value;

      return;
   }

   if( !str_cmp( arg2, "mana" ) )
   {
      if( value < 0 || value > 30000 )
      {
         send_to_char( "Mana range is 0 to 30,000 mana points.\r\n", ch );
         return;
      }
      victim->max_mana = value;
      return;
   }

   if( !str_cmp( arg2, "move" ) )
   {
      if( value < 0 || value > 2000 )
      {
         send_to_char( "Move range is 0 to 30,000 move points.\r\n", ch );
         return;
      }
      victim->max_move = value;
      return;
   }

   if( !str_cmp( arg2, "practice" ) )
   {
      if( IS_NPC(victim) )
      {
       send_to_char("Not on NPCs.\r\n",ch);
       return;
      }
      if( value < 0 || value > 100 )
      {
         send_to_char( "Practice range is 0 to 100 sessions.\r\n", ch );
         return;
      }
      victim->pcdata->practice = value;
      return;
   }

   if( !str_cmp( arg2, "align" ) )
   {
      if( value < -1000 || value > 1000 )
      {
         send_to_char( "Alignment range is -1000 to 1000.\r\n", ch );
         return;
      }
      victim->alignment = value;
      return;
   }

   if( !str_cmp( arg2, "supermana" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPCs.\r\n", ch );
         return;
      }
      if( IS_VAMP( victim ) || IS_WOLF( victim ) )
      {
         send_to_char( "@@eDone!!!@@N\r\n", ch );
         victim->pcdata->super->energy_max = value;
      }
      else
         send_to_char( "They are not a supernatural!!\r\n", ch );
      return;

   }

   if( !str_cmp( arg2, "thirst" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPCs.\r\n", ch );
         return;
      }

      if( value < 0 || value > 100 )
      {
         send_to_char( "Thirst range is 0 to 100.\r\n", ch );
         return;
      }

      victim->pcdata->condition[COND_THIRST] = value;
      return;
   }

   if( !str_cmp( arg2, "drunk" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      if( value < 0 || value > 100 )
      {
         send_to_char( "Drunk range is 0 to 100.\r\n", ch );
         return;
      }

      victim->pcdata->condition[COND_DRUNK] = value;
      return;
   }

   if( !str_cmp( arg2, "full" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      if( value < 0 || value > 100 )
      {
         send_to_char( "Full range is 0 to 100.\r\n", ch );
         return;
      }

      victim->pcdata->condition[COND_FULL] = value;
      return;
   }

   if( !str_cmp( arg2, "flags" ) )
   {
      int neg = 0;
      char *lookupstr = arg3;

      if( get_trust( ch ) < MAX_LEVEL - 1 )
      {
         send_to_char( "Only supreme or creator level immortals may use this.\r\n", ch );
         return;
      }

      if( lookupstr[0] == '-' )
      {
         neg = 1;
         lookupstr++;
      }
      if( lookupstr[0] == '+' )
         lookupstr++;

      value = table_lookup( tab_player_act, lookupstr );
      if( value < 1 )
      {
         snprintf( buf, MSL, "Valid player flags are :\r\n" );
         table_printout( tab_player_act, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }

      if( neg )
         victim->act.reset(value);
      else
         victim->act.set(value);
      return;
   }

   if( !str_cmp( arg2, "aff" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPCs.\r\n", ch );
         return;
      }

      victim->affected_by = 0;
      return;
   }


   if( !str_cmp( arg2, "name" ) )
   {
      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PC's.\r\n", ch );
         return;
      }

      victim->name = arg3;
      return;
   }

   if( !str_cmp( arg2, "short" ) )
   {
      if( !IS_NPC(victim) )
      {
       send_to_char("Not on PC's.\r\n",ch);
       return;
      }
      free_string( victim->npcdata->short_descr );
      snprintf( buf, MSL, "%s", arg3 );
      victim->npcdata->short_descr = str_dup( buf );
      return;
   }

   if( !str_cmp( arg2, "long" ) )
   {
      victim->long_descr = arg3;
      victim->long_descr += "\r\n";
      return;
   }

   if( !str_cmp( arg2, "title" ) )
   {
      if( ch->level < 85 )
      {
         send_to_char( "This option only available to Creators.\r\n", ch );
         return;
      }

      victim->set_title(arg3);
      return;
   }


   if( !str_cmp( arg2, "entry" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

/*	if ( ch->level < 85 )
	{
	  send_to_char( "This option only available to Creators.\r\n", ch );
	  return;
	}
*/
      free_string( victim->pcdata->room_enter );

      snprintf( buf, MSL, "%s", arg3 );
      victim->pcdata->room_enter = str_dup( buf );

      return;
   }

   if( !str_cmp( arg2, "exit" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

/*	if ( ch->level < 85 )
	{
	  send_to_char( "This option only available to Creators.\r\n", ch );
	  return;
	}
*/
      free_string( victim->pcdata->room_exit );

      snprintf( buf, MSL, "%s", arg3 );
      victim->pcdata->room_exit = str_dup( buf );

      return;
   }

   if( !str_cmp( arg2, "recall" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPCs.\r\n", ch );
         return;
      }
      else
      {
         victim->pcdata->recall_vnum = victim->in_room->vnum;
         send_to_char( "Done!\r\n", ch );
         send_to_char( "You will now recall to....HERE!\r\n", victim );
      }
      return;
   }

   if( !str_cmp( arg2, "spec" ) )
   {
      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PC's.\r\n", ch );
         return;
      }

      if( ( victim->npcdata->spec_fun = spec_lookup( arg3 ) ) == 0 )
      {
         send_to_char( "No such spec fun.\r\n", ch );
         return;
      }

      return;
   }

   if( !str_cmp( arg2, "mquest_time" ) )
   {
    if( IS_NPC(victim) )
    {
     send_to_char("Not on NPC's.\r\n",ch);
     return;
    }
    if( value < 0 )
    {
     send_to_char("Valid values are above -1.\r\n",ch);
     return;
    }
    victim->pcdata->quest_info->wait_time = value;
    send_to_char("New wait time has been set.\r\n",ch);
    return;
   }

   if( !str_cmp( arg2, "exp" ) )
   {
    victim->exp = value;
    send_to_char("Done.\r\n",ch);
    return;
   }

   /*
    * Generate usage message.
    */
   do_mset( ch, "" );
   return;
}



DO_FUN(do_oset)
{
   char arg1[MSL];
   char arg2[MSL];
   char arg3[MSL];
   char buf[MSL];
   OBJ_DATA *obj;
   int value;
   int num;
   char *argn;


   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   strcpy( arg3, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
   {
      send_to_char( "Syntax: oset <object> <field>  <value>\r\n", ch );
      send_to_char( "or:     oset <object> <string> <value>\r\n", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "Field being one of:\r\n", ch );
      send_to_char( "  value0 value1 value2 value3 [v0,v1,v2,v3] speed\r\n", ch );
      send_to_char( "  extra wear level weight cost timer durability\r\n", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "String being one of:\r\n", ch );
      send_to_char( "  name short long ed script\r\n", ch );
      return;
   }

   if( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );
      return;
   }

   if( IS_OBJ_STAT(obj,ITEM_EXTRA_CLAN_EQ) && ch->level != MAX_LEVEL )
   {
      send_to_char( "Only creators can OSET Clan equipment!\r\n", ch );
      return;
   }

   /*
    * Snarf the value (which need not be numeric).
    */
   value = atoi( arg3 );

   /*
    * Set something.
    */
   if( !str_prefix( "v", arg2 ) )
   {
      int indexer = -1;
      char numbuf[10];
      if( ch->level < 85 )
         return;
      snprintf( numbuf, 10, "%c", arg2[1] );
      if( is_number( numbuf ) )
      {
         indexer = atoi( numbuf );
      }
      else
      {
         snprintf( numbuf, 10, "%c", arg1[5] );
         if( is_number( numbuf ) )
         {
            indexer = atoi( numbuf );
         }
      }
      if( ( indexer < 0 ) || ( indexer > 9 ) )
      {
         send_to_char( "Value numbers are 0 - 9.\r\n", ch );
         return;
      }
      obj->value[indexer] = value;
      return;
   }


   if( !str_cmp( arg2, "durability" ) || !str_cmp( arg2, "dura" ) )
   {
      if( value < 2 )
      {
       send_to_char("Value must be at least 2. An object with only 1 durability is considered 'broken' and un-usable.\r\n",ch);
       return;
      }
      obj->max_durability = value;
      obj->durability = value;
      return;
   }

   if( !str_cmp( arg2, "extra" ) )
   {
      num = 1;
      argn = arg3;
      if( argn[0] == '+' )
      {
         num = 1;
         argn++;
      }
      if( argn[0] == '-' )
      {
         num = 0;
         argn++;
      }
      value = table_lookup( tab_obj_flags, argn );
      if( value == 0 )
      {
         snprintf( buf, MSL, "Values for extra flags are +/- :\r\n" );
         wide_table_printout( tab_obj_flags, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      if( !ok_to_use( ch, value ) )
         return;

      if( num == 1 )
         obj->extra_flags.set(value);
      else
         obj->extra_flags.reset(value);
      return;
   }

   if( !str_cmp( arg2, "wear" ) )
   {
      num = 1;
      argn = arg3;
      if( argn[0] == '+' )
      {
         num = 1;
         argn++;
      }
      if( argn[0] == '-' )
      {
         num = 0;
         argn++;
      }
      value = table_lookup( tab_wear_flags, argn );
      if( value == 0 )
      {
         snprintf( buf, MSL, "Values for wear flags are +/- :\r\n" );
         wide_table_printout( tab_wear_flags, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      if( !ok_to_use( ch, value ) )
         return;

      if( num == 1 )
         obj->wear_flags.set(value);
      else
         obj->wear_flags.reset(value);
      return;
   }

   if( !str_cmp( arg2, "level" ) )
   {
      if( ch->level < 85 )
         return;

      obj->level = value;
      return;
   }

   if( !str_cmp( arg2, "weight" ) )
   {
      obj->weight = value;
      return;
   }

   if( !str_cmp( arg2, "cost" ) )
   {
      obj->cost = value;
      return;
   }

   if( !str_cmp( arg2, "timer" ) )
   {
      obj->timer = value;
      return;
   }

   if( !str_cmp( arg2, "name" ) )
   {
      free_string( obj->name );
      obj->name = str_dup( arg3 );
      return;
   }

   if( !str_cmp( arg2, "short" ) )
   {
      free_string( obj->short_descr );
      obj->short_descr = str_dup( arg3 );
      return;
   }

   if( !str_cmp( arg2, "long" ) )
   {
      free_string( obj->long_descr );
      obj->long_descr = str_dup( arg3 );
      return;
   }

   if( !str_cmp( arg2, "script" ) )
   {
      free_string( obj->script_name );
      if( !str_cmp( arg3, "-" ) )
       obj->script_name = &str_empty[0];
      else
       obj->script_name = str_dup( arg3 );
      return;
   }

   if( !str_cmp( arg2, "ed" ) )
   {
      EXTRA_DESCR_DATA *ed;

      argument = one_argument( argument, arg3 );
      if( argument == NULL )
      {
         send_to_char( "Syntax: oset <object> ed <keyword> <string>\r\n", ch );
         return;
      }

      ed = new EXTRA_DESCR_DATA;
      ed->keyword = str_dup( arg3 );
      ed->description = str_dup( argument );
      LINK( ed, obj->first_exdesc, obj->last_exdesc, next, prev );
      return;
   }

   if( !str_cmp( arg2, "speed" ) )
   {
    atof(arg3) > 0 ? (obj->speed = atof(arg3)) : (obj->speed = 0.01);
    return;
   }

   /*
    * Generate usage message.
    */
   do_oset( ch, "" );
   return;
}



DO_FUN(do_rset)
{
   char arg1[MSL];
   char arg2[MSL];
   char arg3[MSL];
   ROOM_INDEX_DATA *location;
   int value;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   strcpy( arg3, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
   {
      send_to_char( "Syntax: rset <location> <field> value\r\n", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "Field being one of:\r\n", ch );
      send_to_char( "  sector\r\n", ch );
      return;
   }

   if( ( location = find_location( ch, arg1 ) ) == NULL )
   {
      send_to_char( "No such location.\r\n", ch );
      return;
   }

   /*
    * Snarf the value.
    */
   if( !is_number( arg3 ) )
   {
      send_to_char( "Value must be numeric.\r\n", ch );
      return;
   }
   value = atoi( arg3 );

   /*
    * Set something.
    */

   if( !str_cmp( arg2, "sector" ) )
   {
      location->sector_type = value;
      return;
   }

   /*
    * Generate usage message.
    */
   do_rset( ch, "" );
   return;
}



DO_FUN(do_users)
{
   char buf[MSL];
   char buf2[MSL];
   char buf3[MSL];
   DESCRIPTOR_DATA *d;
   int count;


   count = 0;
   buf[0] = '\0';
   buf2[0] = '\0';

   send_to_char( "\r\n Desc.  Connection State.   Player Name.     Login Site.", ch );
   if( get_trust( ch ) == 85 )
      send_to_char( "                 Port.\r\n", ch );
   else
      send_to_char( "\r\n", ch );


   for( d = first_desc; d != NULL; d = d->next )
   {
     count++;
     switch ( d->connected )
     {
        case CON_PLAYING:
           snprintf( buf3, MSL, "%s", "Playing         " );
           break;
        case CON_GET_NAME:
           snprintf( buf3, MSL, "%s", "Get Name        " );
           break;
        case CON_GET_OLD_PASSWORD:
           snprintf( buf3, MSL, "%s", "Get Old Passwd  " );
           break;
        case CON_CONFIRM_NEW_NAME:
           snprintf( buf3, MSL, "%s", "Cnrm New Name   " );
           break;
        case CON_GET_NEW_PASSWORD:
           snprintf( buf3, MSL, "%s", "Get New Passwd  " );
           break;
        case CON_CONFIRM_NEW_PASSWORD:
           snprintf( buf3, MSL, "%s", "Cnfm New Passwd " );
           break;
        case CON_GET_NEW_SEX:
           snprintf( buf3, MSL, "%s", "Get New Sex     " );
           break;
        case CON_GET_NEW_CLASS:
           snprintf( buf3, MSL, "%s", "Get New Class   " );
           break;
        case CON_GET_RACE:
           snprintf( buf3, MSL, "%s", "Get New Race    " );
           break;
        case CON_READ_MOTD:
           snprintf( buf3, MSL, "%s", "Reading MOTD    " );
           break;
        default:
           snprintf( buf3, MSL, "%s", "Unknown...      " );
           break;
     }

     snprintf( buf + strlen( buf ), MSL, "[%3d %2d %18s] %-12s %-30s",
              d->descriptor,
              d->connected,
              buf3, d->original ? d->original->name.c_str() : d->character ? d->character->name.c_str() : "(none)", d->host );
     if( get_trust( ch ) == 85 )
        snprintf( buf + strlen( buf ), MSL, "  %5d\r\n", d->remote_port );
     else
        snprintf( buf + strlen( buf ), MSL, "\r\n" );

   }

   snprintf( buf2, MSL, "%d user%s\r\n", count, count == 1 ? "" : "s" );
   strncat( buf, buf2, MSL-1 );
   snprintf( buf2, MSL, "%s%s%s", color_string( ch, "stats" ), buf, color_string( ch, "normal" ) );
   send_to_char( buf2, ch );
   return;
}



/*
 * Thanks to Grodyn for pointing out bugs in this function.
 */
DO_FUN(do_force)
{
   char arg[MSL];
   int trust;
   int cmd;
   list<CHAR_DATA *>::iterator li;

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "Force whom to do what?\r\n", ch );
      return;
   }

   /*
    * Look for command in command table.
    */
   trust = get_trust( ch );
   for( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
   {
      if( argument[0] == cmd_table[cmd].name[0]
          && !str_prefix( argument, cmd_table[cmd].name ) && ( cmd_table[cmd].level > trust && cmd_table[cmd].level != 41 ) )
      {
         send_to_char( "You can't even do that yourself!\r\n", ch );
         return;
      }
   }

   /*
    * Allow force to be used on ALL mobs....
    * Only highest level players to use this... it can cause trouble!!!
    * Good for mob "invasions"
    * This could get interesting ;)
    * -- Stephen
    */

   if( !str_cmp( arg, "everymob" ) )
   {
      CHAR_DATA *vch;

      if( ch->level < MAX_LEVEL )
      {
         send_to_char( "This option is only available to true Gods.\r\n", ch );
         return;
      }

      for( li = char_list.begin(); li != char_list.end(); li++ )
      {
         vch = *li;

         if( IS_NPC( vch ) )
         {
            interpret( vch, argument );
         }
      }
      return;
   }

   /*
    * Like above but for mobs in same area as ch 
    */

   if( !str_cmp( arg, "localmobs" ) )
   {
      CHAR_DATA *vim;

      for( li = char_list.begin(); li != char_list.end(); li++ )
      {
         vim = *li;

         if( IS_NPC( vim ) && ( vim->in_room->area == ch->in_room->area ) )
         {
            interpret( vim, argument );
         }
      }
      return;
   }

   if( !str_cmp( arg, "all" ) )
   {
      CHAR_DATA *vch;

      for( li = char_list.begin(); li != char_list.end(); li++ )
      {
         vch = *li;

         if( !IS_NPC( vch ) && !IS_IMMORTAL( vch ) )
         {
            act( "$n forces you to '$t'.", ch, argument, vch, TO_VICT );
            interpret( vch, argument );
         }
      }
   }
   else
   {
      CHAR_DATA *victim;

      if( ( victim = get_char_world( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\r\n", ch );
         return;
      }

      if( victim == ch )
      {
         send_to_char( "Aye aye, right away!\r\n", ch );
         return;
      }

      if( get_trust( victim ) >= get_trust( ch ) )
      {
         send_to_char( "Do it yourself!\r\n", ch );
         return;
      }

      act( "$n forces you to '$t'.", ch, argument, victim, TO_VICT );
      interpret( victim, argument );
   }

   send_to_char( "Ok.\r\n", ch );
   return;
}



/*
 * New routines by Dionysos.
 */
DO_FUN(do_invis)
{

   short level;
   char buf[MSL];

   level = -1;

   if( IS_NPC(ch) )
    return;

   if( argument[0] != '\0' )
      /*
       * Then we have a level argument 
       */
   {
      if( !is_number( argument ) )
      {
         level = get_trust( ch );
      }
      level = UMAX( 1, atoi( argument ) );
      level = UMIN( ch->level, level );

      if( ch->act.test(ACT_WIZINVIS) )
      {
         ch->pcdata->invis = level;
         snprintf( buf, MSL, "Wizinvis changed to level: %d\r\n", level );
         send_to_char( buf, ch );
         return;
      }


   }

   if( level == -1 )
      level = get_trust( ch );

   ch->pcdata->invis = level;

   ch->act.flip(ACT_WIZINVIS);

   if( !ch->act.test(ACT_WIZINVIS) )
   {
      act( "Small, dazzling spots of light focus into the shape of $n!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "Your body becomes solid again.\r\n", ch );
   }
   else
   {
      act( "$n dissolves into a storm of dazzling points of light!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You slowly vanish into thin air.\r\n", ch );
      snprintf( buf, MSL, "Setting Wizinvis to level: %d.\r\n", level );
      send_to_char( buf, ch );
   }

   return;
}

DO_FUN(do_holylight)
{
   if( IS_NPC( ch ) )
      return;

   ch->act.flip(ACT_HOLYLIGHT);

   if( ch->act.test(ACT_HOLYLIGHT) )
      send_to_char( "Holy light mode on.\r\n", ch );
   else
      send_to_char( "Holy light mode off.\r\n", ch );

   return;
}

/* Wizify and Wizbit sent in by M. B. King */

DO_FUN(do_wizify)
{
   char arg1[MSL];
   CHAR_DATA *victim;

   argument = one_argument( argument, arg1 );
   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax: wizify <name>\r\n", ch );
      return;
   }
   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }
   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on mobs.\r\n", ch );
      return;
   }
   victim->wizbit = !victim->wizbit;
   if( victim->wizbit )
   {
      act( "$N wizified.\r\n", ch, NULL, victim, TO_CHAR );
      act( "$n has wizified you!\r\n", ch, NULL, victim, TO_VICT );
   }
   else
   {
      act( "$N dewizzed.\r\n", ch, NULL, victim, TO_CHAR );
      act( "$n has dewizzed you!\r\n", ch, NULL, victim, TO_VICT );
   }

   do_save( victim, "auto" );
   return;
}

/* Idea from Talen of Vego's do_where command */

DO_FUN(do_owhere)
{
   char buf[MSL];
   char catbuf[MSL];
   char arg[MSL];
   bool found = FALSE;
   OBJ_DATA *obj;
   OBJ_DATA *in_obj;
   int obj_counter = 1;
   bool mailme = FALSE;
   list<OBJ_DATA *>::iterator li;

   if( is_name( "mailme", argument ) )
      mailme = TRUE;
   one_argument( argument, arg );
   snprintf( buf, MSL, "Output for Owhere %s\r\n", arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Syntax:  owhere <object/rare>.\r\n", ch );
      return;
   }
   else if( !str_prefix(arg,"rare") ) /* Check for outstanding rares not held by players --Kline */
   {
    for( li = obj_list.begin(); li != obj_list.end(); li++ )
    {
     obj = *li;
     if( obj == auction_item )
      continue;
     if( !IS_OBJ_STAT(obj,ITEM_EXTRA_RARE) )
      continue;
     found = TRUE;

     for( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj );

     if( in_obj->carried_by != NULL )
     {
      if( !IS_NPC(in_obj->carried_by) )
       continue;
      else
       snprintf( catbuf, MSL, "[%2d] %s carried by %s [Room:%d].\r\n",
                obj_counter, obj->short_descr, in_obj->carried_by->get_name(ch), in_obj->carried_by->in_room->vnum );
     }
     else
     {
      snprintf( catbuf, MSL, "[%2d] %s in %s [Room:%d].\r\n",
               obj_counter,
               obj->short_descr, ( in_obj->in_room == NULL ) ?
               "somewhere" : in_obj->in_room->name, in_obj->in_room->vnum );
     }

     obj_counter++;
     buf[0] = UPPER( buf[0] );
     strncat( buf, catbuf, MSL-1 );
    }
   }
   else
   {
      for( li = obj_list.begin(); li != obj_list.end(); li++ )
      {
         obj = *li;
         if( !is_name( arg, obj->name ) )
            continue;
         if( obj == auction_item )
            continue;
         found = TRUE;

         for( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj );

         if( in_obj->carried_by != NULL )
         {
            snprintf( catbuf, MSL, "[%2d] %s carried by %s [Room:%d].\r\n",
                     obj_counter, obj->short_descr, in_obj->carried_by->get_name(ch), in_obj->carried_by->in_room->vnum );
         }
         else
         {
            snprintf( catbuf, MSL, "[%2d] %s in %s [Room:%d].\r\n",
                     obj_counter,
                     obj->short_descr, ( in_obj->in_room == NULL ) ?
                     "somewhere" : in_obj->in_room->name, in_obj->in_room->vnum );
         }

         obj_counter++;
         buf[0] = UPPER( buf[0] );
         strncat( buf, catbuf, MSL-1 );

      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );
   }
   else
   {
      snprintf( catbuf, MSL, "Owhere report for %s", arg );
      send_rep_out( ch, buf, mailme, catbuf );
   }
   return;
}

DO_FUN(do_mpcr)
{
/* A Function to perform Corpse Retrivals (CRs)
 * Gets first corpse (if any) matching argument.
 * Will NOT get corpses from the room ch is in, allowing the
 * (N)PC to keep calling this function, and to 'pile up' all the
 * corpses.
 * -- Stephen
 */


   OBJ_DATA *obj;
   bool found = FALSE;
   char arg[MSL];
   list<OBJ_DATA *>::iterator li;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Retrive WHICH corpse??\r\n", ch );
      return;
   }

   for( li = obj_list.begin(); li != obj_list.end(); li++ )
   {
      obj = *li;
      if( ( ( obj->pIndexData->vnum ) == OBJ_VNUM_CORPSE_PC ) && ( !str_cmp( arg, obj->owner ) ) && ( !( obj->in_room == ch->in_room ) ) )   /*don't work! */
      {
         found = TRUE;
         obj_from_room( obj );
         obj_to_room( obj, ch->in_room );
         if( !IS_IMMORTAL(ch) )
          act( "Got the blighter!", ch, NULL, NULL, TO_CHAR );

      }

   }

   /*
    * act used to enable mobiles to check for CR triggers...
    */

   if( !found && !IS_IMMORTAL(ch) )
   {
      act( "Couldn't find it.", ch, NULL, NULL, TO_CHAR );
   }
   return;
}

DO_FUN(do_resetpassword)
{
   char arg1[MSL];
   char arg2[MSL];
   /*
    * char buf[MSL];  
    */
   CHAR_DATA *victim;
   char *pwdnew;

   if( IS_NPC( ch ) )
      return;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   victim = get_char_world( ch, arg1 );

   if( victim == '\0' )
   {
      send_to_char( "This character is not playing at this time\r\n", ch );
      return;
   }
   if( ( ch->level != L_GOD ) && ch->level < victim->level )
   {
      send_to_char( "You cannot change the password of immortals!\r\n", ch );
      return;
   }
   if( IS_NPC( victim ) )
   {
      send_to_char( "You cannot change the password of NPCs!\r\n", ch );
      return;
   }


   if( ( ch->pcdata->pwd != '\0' ) && ( arg1[0] == '\0' || arg2[0] == '\0' ) )
   {
      send_to_char( "Syntax: password <char> <new>.\r\n", ch );
      return;
   }


   if( strlen( arg2 ) < 5 )
   {
      send_to_char( "New password must be at least five characters long.\r\n", ch );
      return;
   }

   pwdnew = crypt( arg2, victim->name.c_str() );


   free_string( victim->pcdata->pwd );
   victim->pcdata->pwd = str_dup( pwdnew );
   save_char_obj( victim );
   send_to_char( "Ok.\r\n", ch );
   return;
}


DO_FUN(do_iscore)
{
   /*
    * Show the imm bamfin/out if invis, and if wizlock/deathmatch 
    * * iscore = immortal 'score' --Stephen
    */

   char buf[MSL];

   snprintf( buf, MSL, "(wiz) Invis: %s   Holylight: %s\r\n",
            ch->act.test(ACT_WIZINVIS) ? "YES" : "NO ", ch->act.test(ACT_HOLYLIGHT) ? "YES" : "NO " );
   send_to_char( buf, ch );

   if( !IS_NPC(ch) && ch->act.test(ACT_WIZINVIS) )
   {
      snprintf( buf, MSL, "You are wizinvis at level %d.\r\n", ch->pcdata->invis );
      send_to_char( buf, ch );
   }

   snprintf( buf, MSL, "Bamfin:  %s\r\n",
            ( ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0' ) ? ch->pcdata->bamfin : "Not changed/Switched." );
   send_to_char( buf, ch );

   snprintf( buf, MSL, "Bamfout: %s\r\n",
            ( ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0' ) ? ch->pcdata->bamfout : "Not changed/Switched." );
   send_to_char( buf, ch );

   snprintf( buf, MSL, "Mud Info:\r\nDeathmatch: %s   Wizlock: %s\r\n", deathmatch ? "YES" : "NO ", wizlock ? "YES" : "NO " );
   send_to_char( buf, ch );
   return;
}

DO_FUN(do_fights)
{
 list<CHAR_DATA *>::iterator li;
 CHAR_DATA *vict = NULL;
 char buf[MSL];

 send_to_char("Active Fights:\r\n",ch);

 for( li = fight_list.begin(); li != fight_list.end(); li++ )
 {
  vict = *li;
  snprintf(buf,MSL,"%s vs %s [Room:%5d]\r\n",vict->fighting->get_name(),vict->get_name(),vict->in_room->vnum);
  send_to_char(buf,ch);
 }
 if( fight_list.empty() )
  send_to_char("No fights right now!\r\n",ch);
 else
 {
  snprintf(buf,MSL,"%d fight%s right now.\r\n",static_cast<int>(fight_list.size()),( fight_list.size() > 1 ) ? "s" : "");
  send_to_char(buf,ch);
 }
 return;
}

DO_FUN(do_iwhere)
{
   /*
    * Like WHERE, except is global, and shows area & room.
    * * --Stephen
    */

   CHAR_DATA *vch;
   list<CHAR_DATA *>::iterator li;
   char buf[MSL];
   char buf2[MSL];
   int count = 0;
   buf2[0] = '\0';


   send_to_char( "Name          Room        Area\r\n", ch );
   send_to_char( "----          ----        ----\r\n", ch );

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      vch = *li;
      if( !IS_NPC( vch ) && can_see( ch, vch ) && !vch->switched )
      {

         count++;
         snprintf( buf, MSL, "%-12s [%5d] %-20s\r\n",
                  vch->name.c_str(), vch->in_room == NULL ? 0 : vch->in_room->vnum, vch->in_room->area->name );
         strncat( buf2, buf, MSL-1 );
      }
   }

   if( count == 0 )
      strncat( buf2, "No Players found!\r\n", MSL );
   else
   {
      snprintf( buf, MSL, "%d Player%s found.\r\n", count, ( count > 1 ) ? "s" : "" );
      strncat( buf2, buf, MSL-1 );
   }

   send_to_char( buf2, ch );
   return;
}

DO_FUN(do_setclass)
{
   /*
    * New version of advance, using some of old code.
    * and a slight change to what was left.  -S-
    * Added support for setting remort class levels.
    */

   char arg1[MSL];
   char arg2[MSL];
   char arg3[MSL];
   char buf[MSL];
   CHAR_DATA *victim;
   int value;
   int iClass;
   bool cok, remort;
   int p_class = 0;
   int cnt;
   int lose;
   bool vamp = FALSE;


   argument = one_argument( argument, arg1 );   /* Player */
   argument = one_argument( argument, arg2 );   /* class */
   strcpy( arg3, argument );  /* arg3 = value */

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: SetClass <player> <class> <value>\r\n", ch );
      send_to_char( "if value = -1 then player will NOT be able to level in that class.\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC(victim) )
   {
    send_to_char("Not on NPCs.\r\n",ch);
    return;
   }

   cok = FALSE;
   remort = FALSE;

   for( iClass = 0; iClass < MAX_CLASS; iClass++ )
   {
      if( !str_cmp( arg2, class_table[iClass].who_name ) )
      {
         p_class = iClass;
         cok = TRUE;
      }
      if( !str_cmp( arg2, remort_table[iClass].who_name ) )
      {
         p_class = iClass;
         cok = TRUE;
         remort = TRUE;
      }

   }

   if( !str_prefix( arg2, "VAMPYRE" ) )
   {
      send_to_char( "@@eNO WAY!!!@@N", ch );
      return;
   }
   if( !str_prefix( arg2, "ADEPT" ) )
   {
      if( IS_ADEPT(victim) )
      {
         send_to_char( "They are already an adept.\r\n", ch );
         return;
      }
      else
      {
         p_class = ADVANCE_ADEPT;
         advance_level( victim, p_class, TRUE, FALSE );
         victim->pcdata->adept_level = 1;
         snprintf( buf, MSL, " %s %s", victim->name.c_str(), get_adept_name( victim ) );
         do_whoname( ch, buf );
         victim->exp = 0;
         do_save( victim, "auto" );
         return;
      }
   }

   if( !cok )
   {
      send_to_char( "That's not a class!!\r\n", ch );
      return;
   }

   value = is_number( arg3 ) ? atoi( arg3 ) : -9;

   if( value == -9 )
   {
      send_to_char( "Invalid value for value\r\n\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( ( value < -1 || value > MAX_LEVEL )
/*       || ( ( vamp ) && ( value < -1 || value > MAX_VAMP_LEVEL ) ) */  )
   {
      snprintf( buf, MSL, "%d is not a valid value.\r\n", value );
      send_to_char( buf, ch );
      snprintf( buf, MSL, "Use a value between -1 and %d.\r\n\r\n", MAX_LEVEL - 1 );
      send_to_char( buf, ch );
      return;
   }

   if( value > get_trust( ch ) )
   {
      send_to_char( "Limited to your trust level.\r\n", ch );
      return;
   }

   /*
    * Lower level:
    *   Reset to level 1.
    *   Then raise again.
    *   Currently, an imp can lower another imp.
    *   -- Swiftest
    */

   if( value == ( remort ? victim->lvl2[p_class] : victim->lvl[p_class] ) )
   {
      send_to_char( "That wouldn't accomplish much!\r\n", ch );
      return;
   }
   if( ( value < ( remort ? victim->lvl2[p_class] : victim->lvl[p_class] ) )
       || ( ( vamp ) && ( value <= victim->pcdata->super->level ) ) )

   {
      int sn;

      lose = ( remort ? victim->lvl2[p_class] - 1 : victim->lvl[p_class] - 1 );

      send_to_char( "Lowering a player's level!\r\n", ch );
      send_to_char( "**** OOOOHHHHHHHHHH  NNNNOOOO ****\r\n", victim );

      if( vamp )
      {
         if( value != -1 )
            victim->pcdata->super->level = 1;
         else
            victim->pcdata->super->level = -1;
         victim->pcdata->super->exp = 0;
      }

      else if( remort )
      {
         if( value != -1 )
            victim->lvl2[p_class] = 1;
         else
            victim->lvl2[p_class] = -1;
      }
      else
         victim->lvl[p_class] = 1;
      victim->exp = 0;
      if( vamp )
      {
         victim->pcdata->super->energy_max = 10;
         victim->pcdata->super->pracs = 2;
         victim->pcdata->super->skills_max = 2;
         victim->pcdata->super->skills_learned = 1;
         victim->pcdata->super->energy = 10;
         for( sn = 0; sn < MAX_SKILL; sn++ )
         {
            victim->pcdata->learned[sn] = 0;
         }
         victim->pcdata->learned[skill_lookup( "feed" )] = 90;
      }

      if( remort )
         victim->max_hit -= UMIN( victim->max_hit, lose * remort_table[p_class].hp_min );
      else
         victim->max_hit -= UMIN( victim->max_hit, lose * class_table[p_class].hp_min );

      victim->max_mana = 100;
      victim->max_move = 100;
      for( sn = 0; sn < MAX_SKILL; sn++ )
         victim->pcdata->learned[sn] = 0;
      victim->pcdata->practice = 0;
      victim->hit = victim->max_hit;
      victim->mana = victim->max_mana;
      victim->move = victim->max_move;
      if( vamp )
      {
         p_class = ADVANCE_VAMP;
         advance_level( victim, p_class, FALSE, remort );
      }
      else
         advance_level( victim, p_class, FALSE, remort );
   }
   else
   {
      send_to_char( "Raising a player's level!\r\n", ch );
      send_to_char( "**** OOOOHHHHHHHHHH  YYYYEEEESSS ****\r\n", victim );
   }

   if( value != -1 && !remort && !( vamp ) )
   {
      snprintf( buf, MSL, "You are now level %d in your %s class.\r\n", value, class_table[p_class].class_name );
      send_to_char( buf, victim );
      for( iClass = victim->lvl[p_class]; iClass < value; iClass++ )
      {
         victim->lvl[p_class] += 1;
         advance_level( victim, p_class, FALSE, remort );
      }
   }
   if( remort )
   {
      snprintf( buf, MSL, "You are now level %d in your %s class.\r\n", value, remort_table[p_class].class_name );
      send_to_char( buf, victim );
      for( iClass = victim->lvl2[p_class]; iClass < value; iClass++ )
      {
         victim->lvl2[p_class] += 1;
         advance_level( victim, p_class, FALSE, remort );
      }
   }
   if( vamp )
   {
      send_to_char( "@@NYou are now a level %d @@eKindred@@N!!!\r\n", victim );
      for( iClass = victim->pcdata->super->level; iClass < value; iClass++ )
      {
         p_class = ADVANCE_VAMP;
         victim->pcdata->super->level += 1;
         advance_level( victim, p_class, FALSE, remort );
      }
   }


   victim->exp = 0;
   victim->trust = 0;

   /*
    * Make sure that ch->level holds vicitm's max level
    */
   victim->level = 0;
   for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      if( victim->lvl[cnt] > victim->level )
         victim->level = victim->lvl[cnt];

   /*
    * check for remort levels too...
    */
   for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      if( victim->lvl2[cnt] > victim->level )
         victim->level = victim->lvl2[cnt];


   send_to_char( "Ok.\r\n", ch );
   return;
}

DO_FUN(do_isnoop)
{
   /*
    * Creator-only command.  Lists who (if anyone) is being snooped.
    * * -S- 
    */


   DESCRIPTOR_DATA *d;
   char buf[MSL];
   int count = 0;


   send_to_char( "Snoop List:\r\n-=-=-=-=-=-\r\n", ch );


   for( d = first_desc; d != NULL; d = d->next )
   {
      if( d->snoop_by != NULL )
      {
         count++;
         snprintf( buf, MSL, "%s by %s.\r\n", d->character->name.c_str(), d->snoop_by->character->name.c_str() );
         send_to_char( buf, ch );
      }
   }

   if( count != 0 )
      snprintf( buf, MSL, "%d snoops found.\r\n", count );
   else
      snprintf( buf, MSL, "No snoops found.\r\n" );

   send_to_char( buf, ch );
   return;
}

DO_FUN(do_dog)
{
   /*
    * A real silly command which switches the (mortal) victim into
    * * a mob.  As the victim is mortal, they won't be able to use
    * * return ;P  So will have to be released by someone...
    * * -S-
    */

   ROOM_INDEX_DATA *location;
   MOB_INDEX_DATA *pMobIndex;
   CHAR_DATA *mob;
   CHAR_DATA *victim;

   if( ch->level < MAX_LEVEL )
   {
      send_to_char( "Only for creators.\r\n", ch );
      return;
   }


   if( argument[0] == '\0' )
   {
      send_to_char( "Turn WHO into a little doggy?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Cannot do this to mobs, only pcs.\r\n", ch );
      return;
   }

   if( ( pMobIndex = get_mob_index( MOB_VNUM_DOGGY ) ) == NULL )
   {
      send_to_char( "Couldn't find the doggy's vnum!!\r\n", ch );
      return;
   }

   if( victim->desc == NULL )
   {
      send_to_char( "Already switched, like.\r\n", ch );
      return;
   }

   mob = create_mobile( pMobIndex );
   location = victim->in_room;   /* Remember where to load doggy! */
   char_from_room( victim );

   char_to_room( victim, get_room_index( ROOM_VNUM_LIMBO ) );
   char_to_room( mob, location );

   /*
    * ch->desc->character = victim;
    * ch->desc->original  = ch;
    * victim->desc        = ch->desc;
    * ch->desc            = NULL;
    */

   /*
    * Instead of calling do switch, just do the relevant bit here
    */
   victim->desc->character = mob;
   victim->desc->original = victim;
   mob->desc = victim->desc;
   victim->desc = NULL;

   act( "$n is suddenly turned into a small doggy!!", victim, NULL, NULL, TO_NOTVICT );
   send_to_char( "You suddenly turn into a small doggy!", victim );
   send_to_char( "Ok.\r\n", ch );
   return;
}

DO_FUN(do_togbuild)
{
   /*
    * Toggles PC's ch->act ACT_BUILDER value 
    * * -S-
    */

   CHAR_DATA *victim;


   if( argument[0] == '\0' )
   {
      send_to_char( "Toggle who as a builder??\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPCs!\r\n", ch );
      return;
   }

   victim->act.flip(ACT_BUILDER);

   if( victim->act.test(ACT_BUILDER) )
   {
      send_to_char( "Bit set to ALLOW building.\r\n", ch );
      send_to_char( "You have been authorized to use the builder.\r\n", victim );
   }
   else
   {
      send_to_char( "Bit set to DISALLOW building.\r\n", ch );
      send_to_char( "You authorization to build has been revoked.\r\n", victim );
   }

   return;
}


DO_FUN(do_togleader)
{
   CHAR_DATA *victim;


   if( argument[0] == '\0' )
   {
      send_to_char( "Toggle who as a clan boss??\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPCs!\r\n", ch );
      return;
   }

   victim->act.flip(ACT_CBOSS);

   if( victim->act.test(ACT_CBOSS) )
   {
      send_to_char( "Bit set for CLAN_BOSS.\r\n", ch );
      send_to_char( "You have been set as a clan boss.\r\n", victim );
   }
   else
   {
      send_to_char( "Bit removed for CLAN_BOSS.\r\n", ch );
      send_to_char( "You are no longer a clan boss.\r\n", victim );
   }

   return;
}


DO_FUN(do_whoname)
{
   /*
    * Set victim's who name - 
    * * what appears on who list in place of their levels
    * * --Stephen
    */

   CHAR_DATA *victim;
   char arg[MSL];
   char foo[MSL];
   int side;   /* -1 = left, +1 = right side */


   if( IS_NPC( ch ) )
      return;

   argument = one_argument( argument, arg );

   if( argument[0] == '\0' || arg[0] == '\0' )
   {
      send_to_char( "Usage: whoname <victim> <string>\r\n\r\n", ch );
      send_to_char( "Where string is no more than 14 letters long.\r\n", ch );
      send_to_char( "Use 'off' as name to use default who name.\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "Couldn't find target.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Mobiles don't have a whoname!\r\n", ch );
      return;
   }

   if( ( get_trust( ch ) < ( MAX_LEVEL - 1 ) ) && ch != victim )
   {
      send_to_char( "Only Supremes and Creators can set the whoname of others.\r\n", ch );
      return;
   }

   if( !str_cmp( argument, "off" ) )
   {
      free_string( victim->pcdata->who_name );
      victim->pcdata->who_name = str_dup( "off" );
      send_to_char( "Who name set to default value.\r\n", ch );
      return;
   }

   if( nocol_strlen( argument ) > 14 )
   {
      send_to_char( "Name too long.\r\n", ch );
      do_whoname( ch, "" );   /* Usage message */
      return;
   }

   smash_tilde( argument );

   /*
    * Now for the fun part -
    * * CENTER the string if less than 14 chars <g>
    * * Add spaces to alternate sides - UGLY
    */
   /*
    * foo = str_dup( argument );
    */
   side = -1;

   while( nocol_strlen( argument ) < 14 )
   {
      switch ( side )
      {
         case -1:   /* left side */
            snprintf( foo, MIL, " %s", argument );
            break;
         case 1:
            snprintf( foo, MIL, "%s ", argument );
            break;
      }
      side *= -1; /* Switch sides for next loop */
      snprintf( argument, MSL, "%s", foo );
   }

   free_string( victim->pcdata->who_name );
   victim->pcdata->who_name = str_dup( argument );
   send_to_char( "Ok, done.\r\n", ch );
   return;
}

DO_FUN(do_lhunt)
{
   /*
    * Simple function for Imms... loops through all mobs, and
    * * shows details of any currently hunting someone. -S-
    */
   /*
    * Rewritten to suit new hunt functions.. :) -- Alty
    */
   CHAR_DATA *lch;
   list<CHAR_DATA *>::iterator li;
   char buf[MSL];
   bool found = FALSE;

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      lch = *li;
      if( !lch->hunting && !lch->hunt_obj )
      {
         if( !lch->searching.empty() )
         {
            snprintf( buf, MSL, "%s searching for %s.\r\n", lch->get_name(), lch->searching.c_str() );
            send_to_char( buf, ch );
            found = TRUE;
         }
         continue;
      }
      found = TRUE;
      snprintf( buf, MSL, "%s (%s)", lch->get_name(), ( IS_NPC( lch ) ? "mobile" : "player" ) );
      if( lch->hunting )
         snprintf( buf + strlen( buf ), MSL, " hunting for (%s) %s",
                  ( IS_NPC( lch->hunting ) ? "mobile" : "player" ), lch->hunting->get_name() );
      if( lch->hunt_obj )
      {
         if( lch->hunting && IS_SET( lch->hunt_flags, HUNT_CR ) && lch->hunt_obj->item_type == ITEM_CORPSE_PC )
            strncat( buf, " to return a corpse", MSL );
         else
            snprintf( buf + strlen( buf ), MSL, " looking for (object) %s", lch->hunt_obj->short_descr );
      }
      if( IS_NPC( lch ) && IS_SET( lch->hunt_flags, HUNT_MERC | HUNT_CR ) && lch->npcdata->hunt_for )
         snprintf( buf + strlen( buf ), MSL, ", employed by %s", lch->npcdata->hunt_for->get_name() );
      strncat( buf, ".\r\n", MSL );
      send_to_char( buf, ch );
   }
   if( !found )
      send_to_char( "No one is currently hunting.\r\n", ch );
   return;
}

DO_FUN(do_sstat)
{
   /*
    * Lists the % for a player's skill(s)
    * * Either shows all, or value for just a given skill
    * * -S-
    */

   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   int skill = -1;
   int sn;
   int col;
   CHAR_DATA *victim;

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Usage: sstat <victim> [skill]\r\n", ch );
      send_to_char( "Where skill is an optional argument.\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "Couldn't find target.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Mobiles don't have skills!\r\n", ch );
      return;
   }

   if( argument[0] != '\0' )
   {
      skill = skill_lookup( argument );
      if( skill <= 0 )
      {
         send_to_char( "No such skill/spell!\r\n", ch );
         return;
      }
      snprintf( buf, MSL, "%17s - %3d%%\r\n", skill_table[skill].name, victim->pcdata->learned[skill] );
      send_to_char( buf, ch );
      return;
   }

   col = 0;
   buf1[0] = '\0';

   for( sn = 0; sn < MAX_SKILL; sn++ )
   {
      if( skill_table[sn].name == NULL )
         break;

      snprintf( buf, MSL, "%16s - %3d%%  ", skill_table[sn].name, victim->pcdata->learned[sn] );
      strncat( buf1, buf, MSL-1 );

      if( ++col % 3 == 0 )
         strncat( buf1, "\r\n", MSL );
   }
   if( col % 3 != 0 )
      strncat( buf1, "\r\n", MSL );

   send_to_char( buf1, ch );
   return;
}

struct monitor_type
{
   char *name;
   int channel;
   int min_level;
   char *col;
   char *id;
   char *on_name;
   char *off_name;
};

struct monitor_type monitor_table[] = {
   {"connection", MONITOR_CONNECT, 83, "@@l", "CON",
    "[ CONNECTION   ] Shows details of players connecting to the mud.\r\n",
    "[ connection   ] Not showing details of players connecting.\r\n"},

   {"area_update", MONITOR_AREA_UPDATE, 82, "@@p", "A_UPD",
    "[ AREA_UPDATE  ] Informs you of ALL area updates.\r\n",
    "[ area_update  ] You are not informed of area updates.\r\n"},

   {"area_bugs", MONITOR_AREA_BUGS, 82, "@@p", "A_BUG",
    "[ AREA_BUGS    ] Notifies you of any errors within areas.\r\n",
    "[ area_bugs    ] You are not told of errors within areas.\r\n"},

   {"area_save", MONITOR_AREA_SAVING, 83, "@@p", "A_SAVE",
    "[ AREA_SAVE    ] You get told of all area saving.\r\n",
    "[ area_save    ] You don't get told of all area saves.\r\n"},

   {"objects", MONITOR_OBJ, 83, "@@r", "OBJ",
    "[ OBJECTS      ] You are told of problems relating to objects.\r\n",
    "[ objects      ] You are not told of object-related problems.\r\n"},

   {"mobile", MONITOR_MOB, 83, "@@a", "MOB",
    "[ MOBILE       ] Watching mobile/player problems.\r\n",
    "[ mobile       ] Not watching problems with mobiles/players\r\n"},

   {"room", MONITOR_ROOM, 83, "@@e", "ROOM",
    "[ ROOM         ] You are informed of problems involved with rooms.\r\n",
    "[ room         ] Not informed of problems with rooms.\r\n"},

   {"magic", MONITOR_MAGIC, 83, "@@a", "MAGIC",
    "[ MAGIC        ] You are informed of various spell casting info.\r\n",
    "[ magic        ] Not informed of spell casting info.\r\n"},

   {"imm_general", MONITOR_GEN_IMM, 85, "@@y", "IMM_GEN",
    "[ IMM_GENERAL  ] You are notified of use of logged immortal commands.\r\n",
    "[ imm_general  ] You are not told of the use of logged immortal commands.\r\n"},

   {"mort_general", MONITOR_GEN_MORT, 84, "@@y", "MORT_GEN",
    "[ MORT_GENERAL ] You are notified of use of logged mortal commands.\r\n",
    "[ mort_general ] You are not told of the use of logged mortal commands.\r\n"},

   {"combat", MONITOR_COMBAT, 82, "@@R", "COMBAT",
    "[ COMBAT       ] You are monitoring problems in combat.\r\n",
    "[ combat       ] Not monitoring any combat problems.\r\n"},

   {"hunting", MONITOR_HUNTING, 82, "@@B", "HUNT",
    "[ HUNTING      ] You are told of all mobile hunting.\r\n",
    "[ hunting      ] Not told about mobiles hunting players.\r\n"},

   {"build", MONITOR_BUILD, 85, "@@y", "BUILD",
    "[ BUILD        ] You receive logged building commands.\r\n",
    "[ build        ] You don't monitor logged building commands.\r\n"},

   {"clan", MONITOR_CLAN, 84, "@@b", "CLAN",
    "[ CLAN         ] You are informed of use of certain clan commands.\r\n",
    "[ clan         ] You are not told of use of certain clan commands.\r\n"},

   {"bad", MONITOR_BAD, 85, "@@W", "BAD",
    "[ BAD          ] You are told of 'bad' things players (try to) do!\r\n",
    "[ bad          ] Not told of 'bad' things players do.\r\n"},

   {"debug", MONITOR_DEBUG, 85, "@@W", "DEBUG",
    "[ DEBUG        ] You are watching code debugging info!\r\n",
    "[ debug        ] Not watching code debugging info.\r\n"},

   {"imc", MONITOR_IMC, 84, "@@W", "IMC",
    "[ IMC          ] You are told of imc net traffic.\r\n",
    "[ imc          ] Not told of imc net traffic.\r\n"},

   {"system", MONITOR_SYSTEM, 84, "@@W", "SYSTEM",
    "[ SYSTEM       ] You are told of system messages.\r\n",
    "[ system       ] Not told of system messages.\r\n"},

   {"help", MONITOR_HELPS, 82, "@@W", "HELP",
    "[ HELP         ] You are told of all missing helpfiles.\r\n",
    "[ help         ] Not told about missing helpfiles.\r\n"},

   {"log", MONITOR_LOG, 85, "@@W", "LOG",
    "[ LOG          ] You are told of all log entries.\r\n",
    "[ log          ] Not told about log entries.\r\n"},

   {"typo", MONITOR_TYPO, 81, "@@W", "TYPO",
    "[ TYPO         ] You are told of all submitted typos.\r\n",
    "[ typo         ] Not told about submitted typos.\r\n"},

   {"idea", MONITOR_IDEA, 81, "@@W", "IDEA",
    "[ IDEA         ] You are told of all submitted ideas.\r\n",
    "[ idea         ] Not told about submitted ideas.\r\n"},

   {"bug", MONITOR_BUG, 81, "@@W", "BUG",
    "[ BUG          ] You are told of all submitted bugs.\r\n",
    "[ bug          ] Not told about submitted bugs.\r\n"},

   {NULL, 0, 0, NULL, NULL}
};

DO_FUN(do_monitor)
{
   int a;
   bool found = FALSE;
   char buf[MSL];
   buf[0] = '\0';

   if( argument[0] == '\0' )
   {
      send_to_char( "@@yMonitor Channel Details:@@g\r\n\r\n", ch );
      for( a = 0; monitor_table[a].min_level != 0; a++ )
      {
         char colbuf[10];
         colbuf[0] = '\0';

         if( monitor_table[a].min_level > get_trust( ch ) )
            continue;

         if( ch->pcdata->monitor.test(monitor_table[a].channel) )
         {
            if( !IS_NPC( ch ) )
            {
               snprintf( colbuf, 10, "@@%c", ch->pcdata->hicol );
               strncat( buf, colbuf, MSL-1 );
            }
            strncat( buf, monitor_table[a].on_name, MSL-1 );
         }

         else
         {
            if( !IS_NPC( ch ) )
            {
               snprintf( colbuf, 10, "@@%c", ch->pcdata->dimcol );
               strncat( buf, colbuf, MSL-1 );
            }
            strncat( buf, monitor_table[a].off_name, MSL-1 );
         }

      }


      send_to_char( buf, ch );

      send_to_char( "\r\n@@yMONITOR <name> toggles the monitor channels.@@g\r\n", ch );
      return;
   }
   /*
    * Search for monitor channel to turn on/off
    */
   for( a = 0; monitor_table[a].min_level != 0; a++ )
   {
      if( !strcmp( argument, monitor_table[a].name ) )
      {
         found = TRUE;
         ch->pcdata->monitor.flip(monitor_table[a].channel);
         break;
      }
   }
   if( !found )
   {
      do_monitor( ch, "" );
      return;
   }
   send_to_char( "Ok, monitor channel toggled.\r\n", ch );
   return;
}


void monitor_chan( const char *message, int channel )
{
   char buf[MSL];
   DESCRIPTOR_DATA *d;
   int a;
   int level = 85;


   if( ( area_resetting_global ) && ( channel == MONITOR_MAGIC ) )
      return;
   for( a = 0; monitor_table[a].min_level != 0; a++ )
      if( monitor_table[a].channel == channel )
      {
         level = monitor_table[a].min_level;
         break;
      }

   snprintf( buf, MSL, "%s[%7s]@@N %s@@N\r\n", monitor_table[a].col, monitor_table[a].id, strip_out( message, "\r\n" ) );

   for( d = first_desc; d; d = d->next )
   {
      if( d->connected == CON_PLAYING
          && !IS_NPC( d->character )
          && d->character->pcdata->monitor.test(channel) && level <= get_trust( d->character ) )
      {
         send_to_char( buf, d->character );
      }

   }
   return;
}

DO_FUN(do_reward)
{
   char arg1[MSL];
   char arg2[MSL];
   char buf[MSL];
   CHAR_DATA *victim;
   int value;


   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );


   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: reward <victim> <value>\r\n", ch );
      send_to_char( "Value being pos to give points, or neg to take points.\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }




   /*
    * Snarf the value.
    */
   if( !is_number( arg2 ) )
   {
      send_to_char( "Value must be numeric.\r\n", ch );
      return;
   }

   value = atoi( arg2 );
   if( value < -100 || value > 100 )
   {
      send_to_char( "Value range is -100 to 100.\r\n", ch );
      return;
   }
   snprintf( buf, MSL, "@@NYou have been rewarded @@y%3d @@aQuest Points@@N by @@m %s @@N!!!\r\n", value, ch->name.c_str() );
   send_to_char( buf, victim );
   snprintf( buf, MSL, "@@NYou have rewarded @@r%s  @@y%3d @@aQuest Points@@N!!!\r\n", victim->name.c_str(), value );
   send_to_char( buf, ch );

   victim->pcdata->quest_points += value;
   return;
}



DO_FUN(do_fhunt)
{
   char buf[MSL];
   char arg1[MSL];
   char arg2[MSL];
   CHAR_DATA *target;
   CHAR_DATA *victim;

   one_argument( argument, arg1 );
   one_argument( argument, arg2 );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: fhunt <victim> <target/stop>.\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "Your victim is not here.\r\n", ch );
      return;
   }

   /*
    * Do not force players to hunt. Can only force mobs
    */
   if( !IS_NPC( victim ) )
   {
      send_to_char( "You can't force a player character to hunt.\r\n", ch );
      return;
   }

   /*
    * force a mob to stop hunting
    */
   if( str_cmp( arg2, "stop" ) )
   {
      if( victim->hunting != NULL )
      {
         snprintf( buf, MSL, "%s stops hunting %s.\r\n", victim->get_name(), victim->hunting->get_name() );
         end_hunt( victim );
         send_to_char( buf, ch );
         return;
      }
      else
      {
         send_to_char( "They aren't hunting anyone.\r\n", ch );
         return;
      }
   }

   if( ( target = get_char_world( victim, arg2 ) ) == NULL )
   {
      send_to_char( "The new target to be hunted is not here.\r\n", ch );
      return;
   }

   /*
    * Can not force mobs to hunt non-vamp players
    */
   if( ( !IS_VAMP( target ) ) && ( !IS_NPC( target ) ) )
   {
      send_to_char( "Mobs cannot hunt non-vamp players\r\n", ch );
      return;
   }

   /*
    * By Now:
    * * You can only force mobs to hunt.
    * * All mobs and vamp players are legal prey.
    */

   /*
    * if victim is currently in a group, leave group
    */
/*   if (  ( victim->leader != NULL )
      || ( victim->master != NULL )  )
      do_follow ( victim, victim );  */

   if( victim->hunting != NULL )
   {
      snprintf( buf, MSL, "%s stops hunting %s.\r\n", victim->get_name(), victim->hunting->get_name() );
      end_hunt( victim );
      send_to_char( buf, ch );
   }

   victim->hunting = target;
   snprintf( buf, MSL, "%s starts hunting %s.\r\n", victim->get_name(), victim->hunting->get_name() );
   send_to_char( buf, ch );

   return;

}


DO_FUN(do_alink)
{

   AREA_DATA *this_area;
   ROOM_INDEX_DATA *this_room;

   BUILD_DATA_LIST *pointer;
   ROOM_INDEX_DATA *current_room;
   int area_top, area_bottom;
   short doorway;
   char buf[MSL];


   this_room = ch->in_room;
   this_area = ch->in_room->area;
   area_top = this_area->max_vnum;
   area_bottom = this_area->min_vnum;
   snprintf( buf, MSL, "External room links for %s.\r\n  THIS DOES NOT INCLUDE ONE WAY DOORS INTO THIS AREA.\r\n",
            this_area->name + 21 );
   send_to_char( buf, ch );

   for( pointer = this_area->first_area_room; pointer != NULL; pointer = pointer->next )
   {
      current_room = (ROOM_INDEX_DATA *)pointer->data;

      for( doorway = 0; doorway < 6; doorway++ )
      {
         EXIT_DATA *pexit;

         if( ( ( pexit = current_room->exit[doorway] ) == NULL )
             || ( pexit->to_room == NULL )
             || ( ( pexit->to_room->vnum >= area_bottom ) && ( pexit->to_room->vnum <= area_top ) ) )
            continue;
         snprintf( buf, MSL, "Room: %d linked to room: %d.\r\n", current_room->vnum, pexit->to_room->vnum );
         send_to_char( buf, ch );
      }
   }


   return;
}

DO_FUN(do_togcouncil)
{
   CHAR_DATA *victim;


   if( argument[0] == '\0' )
   {
      send_to_char( "Toggle who as a council member??\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPCs!\r\n", ch );
      return;
   }

   victim->act.flip(ACT_COUNCIL);

   if( victim->act.test(ACT_COUNCIL) )
   {
      send_to_char( "Bit set for Council Member.\r\n", ch );
      send_to_char( "You have been set as a Council Member.\r\n", victim );
   }
   else
   {
      send_to_char( "Bit removed for COUNCIL MEMBER.\r\n", ch );
      send_to_char( "You are no longer a COUNCIL MEMBER.\r\n", victim );
   }

   return;
}

DO_FUN(do_gain_stat_reset)
{
   CHAR_DATA *victim;
   OBJ_DATA *wear_object;
   CHAR_DATA *rch;


   rch = get_char( ch );

   if( argument[0] == '\0' )
   {
      send_to_char( "Reset who's gain stats??\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPCs!\r\n", ch );
      return;
   }

   reset_gain_stats( victim );

   victim->desc->connected = CON_SETTING_STATS;
   victim->hitroll = 0;
   victim->damroll = 0;
   victim->armor = 100;

   victim->max_mana = victim->pcdata->mana_from_gain;
   victim->max_hit = victim->pcdata->hp_from_gain;
   victim->max_move = victim->pcdata->move_from_gain;

   for( wear_object = victim->first_carry; wear_object != NULL; wear_object = wear_object->next_in_carry_list )
   {
      if( wear_object->wear_loc > WEAR_NONE )
         equip_char( victim, wear_object, wear_object->wear_loc );
   }

   victim->desc->connected = CON_PLAYING;

   send_to_char( "Done!\r\n", ch );
   send_to_char( "Your stats have been reset.\r\n", victim );

}

/* Expand the name of a character into a string that identifies THAT
   character within a room. E.g. the second 'guard' -> 2. guard */
const char *name_expand( CHAR_DATA * ch )
{
   int count = 1;
   CHAR_DATA *rch;
   char name[MSL];  /*  HOPEFULLY no mob has a name longer than THAT */

   static char outbuf[MSL];

   if( !IS_NPC( ch ) )
      return ch->name.c_str();

   one_argument( const_cast<char *>(ch->name.c_str()), name );  /* copy the first word into name */

   if( !name[0] ) /* weird mob .. no keywords */
   {
      strcpy( outbuf, "" );   /* Do not return NULL, just an empty buffer */
      return outbuf;
   }

   for( rch = ch->in_room->first_person; rch && ( rch != ch ); rch = rch->next_in_room )
      if( is_name( name, const_cast<char *>(rch->name.c_str()) ) )
         count++;


   snprintf( outbuf, MSL, "%d.%s", count, name );
   return outbuf;
}

/*
 * For by Erwin S. Andreasen (4u2@aabc.dk)
 */
/* Super-AT command:

FOR ALL <action>
FOR MORTALS <action>
FOR GODS <action>
FOR MOBS <action>
FOR EVERYWHERE <action>


Executes action several times, either on ALL players (not including yourself),
MORTALS (including trusted characters), GODS (characters with level higher than
L_HERO), MOBS (Not recommended) or every room (not recommended either!)

If you insert a # in the action, it will be replaced by the name of the target.

If # is a part of the action, the action will be executed for every target
in game. If there is no #, the action will be executed for every room containg
at least one target, but only once per room. # cannot be used with FOR EVERY-
WHERE. # can be anywhere in the action.

Example:

FOR ALL SMILE -> you will only smile once in a room with 2 players.
FOR ALL TWIDDLE # -> In a room with A and B, you will twiddle A then B.

Destroying the characters this command acts upon MAY cause it to fail. Try to
avoid something like FOR MOBS PURGE (although it actually works at my MUD).

FOR MOBS TRANS 3054 (transfer ALL the mobs to Midgaard temple) does NOT work
though :)

The command works by transporting the character to each of the rooms with
target in them. Private rooms are not violated.

*/

DO_FUN(do_for)
{
   char range[MSL];
   char buf[MSL];
   bool fGods = FALSE, fMortals = FALSE, fMobs = FALSE, fEverywhere = FALSE, found;
   ROOM_INDEX_DATA *room, *old_room;
   CHAR_DATA *p;
   list<CHAR_DATA *>::iterator li;
   int i;

   disable_timer_abort = TRUE;

   argument = one_argument( argument, range );

   if( !range[0] || !argument[0] )  /* invalid usage? */
   {
      do_help( ch, "for" );
      disable_timer_abort = FALSE;
      return;
   }

   if( !str_prefix( "quit", argument ) )
   {
      send_to_char( "Are you trying to crash the MUD or something?\r\n", ch );
      disable_timer_abort = FALSE;
      return;
   }


   if( !str_cmp( range, "all" ) )
   {
      fMortals = TRUE;
      fGods = TRUE;
   }
   else if( !str_cmp( range, "gods" ) )
      fGods = TRUE;
   else if( !str_cmp( range, "mortals" ) )
      fMortals = TRUE;
   else if( !str_cmp( range, "mobs" ) )
      fMobs = TRUE;
   else if( !str_cmp( range, "everywhere" ) )
      fEverywhere = TRUE;
   else
      do_help( ch, "for" );   /* show syntax */

   /*
    * do not allow # to make it easier 
    */
   if( fEverywhere && strchr( argument, '#' ) )
   {
      send_to_char( "Cannot use FOR EVERYWHERE with the # thingie.\r\n", ch );
      disable_timer_abort = FALSE;
      return;
   }

   if( fMobs && strchr( argument, '#' ) )
   {
      send_to_char( "Cannot use FOR MOBS with the # thingie.\r\n", ch );
      disable_timer_abort = FALSE;
      return;
   }

   if( strchr( argument, '#' ) ) /* replace # ? */
   {
      for( li = char_list.begin(); li != char_list.end(); li++ )
      {
         p = *li;
         found = FALSE;

         if( !( p->in_room ) || room_is_private( p->in_room ) || ( p == ch ) )
            continue;

         if( IS_NPC( p ) && fMobs )
            found = TRUE;
         else if( !IS_NPC( p ) && p->level >= LEVEL_IMMORTAL && fGods )
            found = TRUE;
         else if( !IS_NPC( p ) && p->level < LEVEL_IMMORTAL && fMortals )
            found = TRUE;

         /*
          * It looks ugly to me.. but it works :) 
          */
         if( found ) /* p is 'appropriate' */
         {
            char *pSource = argument;  /* head of buffer to be parsed */
            char *pDest = buf;   /* parse into this */

            while( *pSource )
            {
               if( *pSource == '#' )   /* Replace # with name of target */
               {
                  const char *namebuf = name_expand( p );

                  if( namebuf )  /* in case there is no mob name ?? */
                     while( *namebuf ) /* copy name over */
                        *( pDest++ ) = *( namebuf++ );

                  pSource++;
               }
               else
                  *( pDest++ ) = *( pSource++ );
            }  /* while */
            *pDest = '\0'; /* Terminate */

            /*
             * Execute 
             */
            old_room = ch->in_room;
            char_from_room( ch );
            char_to_room( ch, p->in_room );
            interpret( ch, buf );
            char_from_room( ch );
            char_to_room( ch, old_room );

         }  /* if found */
      }  /* for every char */
   }
   else  /* just for every room with the appropriate people in it */
   {
      for( i = 0; i < MAX_KEY_HASH; i++ ) /* run through all the buckets */
         for( room = room_index_hash[i]; room; room = room->next )
         {
            found = FALSE;

            /*
             * Anyone in here at all? 
             */
            if( fEverywhere ) /* Everywhere executes always */
               found = TRUE;
            else if( !room->first_person )   /* Skip it if room is empty */
               continue;


            /*
             * Check if there is anyone here of the requried type 
             */
            /*
             * Stop as soon as a match is found or there are no more ppl in room 
             */
            for( p = room->first_person; p && !found; p = p->next_in_room )
            {

               if( p == ch )  /* do not execute on oneself */
                  continue;

               if( IS_NPC( p ) && fMobs )
                  found = TRUE;
               else if( !IS_NPC( p ) && ( p->level >= LEVEL_IMMORTAL ) && fGods )
                  found = TRUE;
               else if( !IS_NPC( p ) && ( p->level <= LEVEL_IMMORTAL ) && fMortals )
                  found = TRUE;
            }  /* for everyone inside the room */

            if( found && !room_is_private( room ) )   /* Any of the required type here AND room not private? */
            {
               /*
                * This may be ineffective. Consider moving character out of old_room
                * once at beginning of command then moving back at the end.
                * This however, is more safe?
                */

               old_room = ch->in_room;
               char_from_room( ch );
               char_to_room( ch, room );
               interpret( ch, argument );
               char_from_room( ch );
               char_to_room( ch, old_room );
            }  /* if found */
         }  /* for every room in a bucket */
   }  /* if strchr */
   disable_timer_abort = FALSE;
}  /* do_for */

DO_FUN(do_otype)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   OBJ_INDEX_DATA *pObjIndex;
   int vnum;
   int nMatch;
   bool fAll;
   bool found;
   bool mailme = FALSE;
   if( is_name( "mailme", argument ) )
      mailme = TRUE;
   one_argument( argument, arg );
   if( arg[0] == '\0' )
      if( arg[0] == '\0' )
      {
         snprintf( buf, MSL, "Values for object types:\r\n" );
         wide_table_printout( tab_item_types, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }

   buf1[0] = '\0';
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   /*
    * Yeah, so iterating over all vnum's takes 10,000 loops.
    * Get_obj_index is fast, and I don't feel like threading another link.
    * Do you?
    * -- Furey
    */
   for( vnum = 0; nMatch < static_cast<int>(obj_index_list.size()); vnum++ )
   {
      if( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
      {
         nMatch++;
         if( fAll || is_name( arg, tab_item_types[( pObjIndex->item_type ) - 1].text ) )
         {
            found = TRUE;
            snprintf( buf, MSL, "<%d> %s [%5d] %s\r\n", pObjIndex->level,
                     ( IS_OBJ_STAT(pObjIndex,ITEM_EXTRA_REMORT) ?
                       "@@mRemort@@N" : "@@aMortal@@N" ), pObjIndex->vnum, pObjIndex->short_descr );
            strncat( buf1, buf, MSL-1 );
         }
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );
      return;
   }

   snprintf( buf, MSL, "Otype report for %s", arg );
   send_rep_out( ch, buf1, mailme, buf );
   return;
}

DO_FUN(do_owear)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   OBJ_INDEX_DATA *pObjIndex;
   int vnum;
   int nMatch;
   bool fAll;
   bool found;
   bool mailme = FALSE;
   if( is_name( "mailme", argument ) )
      mailme = TRUE;
   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      snprintf( buf, MSL, "Values for wear slots:\r\n" );
      wide_table_printout( tab_wear_flags, buf + strlen( buf ) );
      send_to_char( buf, ch );
      return;
   }
   buf1[0] = '\0';
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   /*
    * Yeah, so iterating over all vnum's takes 10,000 loops.
    * Get_obj_index is fast, and I don't feel like threading another link.
    * Do you?
    * -- Furey
    */
   for( vnum = 0; nMatch < static_cast<int>(obj_index_list.size()); vnum++ )
   {
      if( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
      {
         nMatch++;
         if( fAll || !str_infix( arg, bs_show_values( tab_wear_flags, pObjIndex->wear_flags ) ) )
         {
            found = TRUE;
            snprintf( buf, MSL, "<%s> [%5d] [%3d] %s %s\r\n",
                     bs_show_values( tab_wear_flags, pObjIndex->wear_flags ),
                     pObjIndex->vnum,
                     pObjIndex->level,
                     ( IS_OBJ_STAT(pObjIndex,ITEM_EXTRA_REMORT) ?
                       "@@mRemort@@N" : "@@aMortal@@N" ), pObjIndex->short_descr );
            strncat( buf1, buf, MSL-1 );
         }
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );
      return;
   }

   snprintf( buf, MSL, "Owear report for %s", arg );
   send_rep_out( ch, buf1, mailme, buf );
   return;
}

DO_FUN(do_areasave)
{
   list<AREA_DATA *>::iterator li;
   AREA_DATA *pArea;

   for( li = area_list.begin(); li != area_list.end(); li++ )
   {
      pArea = *li;
      area_modified( pArea );
   }
   send_to_char( "Done.\r\n", ch );

   return;
}

DO_FUN(do_findreset)
{
   char arg1[MSL], arg2[MSL];
   char outbuf[MSL], catbuf[MSL];
   bool mworld = FALSE;
   bool mailme = FALSE;
   bool fmob = FALSE, fobj = FALSE;
   int vnum = 0;
   char mailsub[MSL];
   RESET_DATA *reset;
   MOB_INDEX_DATA *pMob;
   OBJ_INDEX_DATA *pObj;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Not for NPCs.\r\n", ch );
      return;
   }
   if( is_name( "+w", argument ) )
      mworld = TRUE;
   if( is_name( argument, "mailme" ) )
      mailme = TRUE;

   argument = one_argument( argument, arg1 );
   if( arg1[0] == '\0' )
   {
      snprintf( outbuf, MSL, "%s", "Syntax for findreset:\r\n" );
      snprintf( catbuf, MSL, "%s",
               "findreset obj/mob <vnum> [+w]\r\n+w shows all resets in the world, default is current area only.\r\n" );
      strncat( outbuf, catbuf, MSL-1 );
      send_to_char( outbuf, ch );
      return;
   }
   if( !is_name( arg1, "mob obj" ) )
   {
      do_findreset( ch, "" );
      return;
   }
   if( !str_cmp( arg1, "mob" ) )
      fmob = TRUE;
   else
      fobj = TRUE;
   argument = one_argument( argument, arg2 );
   if( ( arg2[0] == '\0' ) || ( !is_number( arg2 ) ) )
   {
      do_findreset( ch, "" );
      return;
   }
   vnum = atoi( arg2 );

   snprintf( outbuf, MSL, "Resets for %s %d:\r\n", arg1, vnum );
   if( mworld )
   {
      list<AREA_DATA *>::iterator li;
      AREA_DATA *pArea;

      pMob = get_mob_index( vnum );
      if( pMob == NULL )
      {
         send_to_char( "Invalid mobile.\r\n", ch );
         return;
      }
      for( li = area_list.begin(); li != area_list.end(); li++ )
      {
         pArea = *li;
         for( reset = pArea->first_reset; reset; reset = reset->next )
         {
            if( fmob )
            {
               switch ( reset->command )
               {
                  default:
                     continue;
                     break;
                  case 'M':
                  {
                     RESET_DATA *similar = reset;
                     if( reset->arg1 != vnum )
                        continue;
                     snprintf( catbuf, MSL, "Room: %d, limit of: %d\r\n", reset->arg3, reset->arg2 );
                     strncat( outbuf, catbuf, MSL-1 );
                     /*
                      * scan for give and equip commands for this mob 
                      */

                     while( similar )
                     {
                        similar = similar->next;
                        if( similar->command != 'G' && similar->command != 'E' )
                           break;

                        if( similar->command == 'G' )
                        {
                           pObj = get_obj_index( similar->arg1 );
                           if( pObj != NULL )
                           {
                              if( pMob->pShop != NULL )
                                 strncat( outbuf, "  sells ", MSL );
                              else
                                 strncat( outbuf, "  with ", MSL );
                              snprintf( catbuf, MSL, "[%d] %s.\r\n", pObj->vnum, pObj->name );
                              strncat( outbuf, catbuf, MSL-1 );
                           }
                           else
                           {
                              snprintf( catbuf, MSL, "[%d] unknown object in give reset!\r\n", similar->arg1 );
                              strncat( outbuf, catbuf, MSL-1 );
                           }
                        }
                        else if( similar->command == 'E' )
                        {
                           pObj = get_obj_index( similar->arg1 );
                           if( pObj != NULL )
                              snprintf( catbuf, MSL, "  equiped with [%d] %s, on %s.\r\n", pObj->vnum,
                                       pObj->name, tab_wear_loc[( similar->arg3 )].text );
                           else
                              snprintf( catbuf, MSL, "[%d] unknown object equipped on %s.\r\n",
                                       similar->arg1, tab_wear_loc[similar->arg3].text );
                           strncat( outbuf, catbuf, MSL-1 );
                        }
                     }
                     break;
                  }  /* case M */
               }  /* switch */
            }  /* if ( fmob ) */
         }  /* for reset */
      }  /* for pArea */
      snprintf( mailsub, MSL, "Findresets for %s %d:\r\n", arg1, vnum );
      send_rep_out( ch, outbuf, mailme, mailsub );
      return;
   }  /* if ( mworld ) */

   send_to_char( "Currently not implemented.\r\n", ch );
   return;
}

DO_FUN(do_census)
{
 CHAR_DATA *vch;
 list<CHAR_DATA *>::iterator li;
 char buf[MSL];
 int rcnt[MAX_RACE];
 int ccnt[MAX_CLASS];
 int scnt[3];
 short i = 0;
 float tf0, tf1, tf2, tf3 = 0;
 int ti1 = 0;

 for( i = 0; i < MAX_RACE; i++ )
  rcnt[i] = 0;
 for( i = 0; i < MAX_CLASS; i++ )
  ccnt[i] = 0;
 for( i = 0; i < 3; i++ )
  scnt[i] = 0;

 if( argument[0] == '\0' )
 {
  send_to_char("Syntax: census <world/area>\r\n",ch);
  return;
 }

 if( !str_prefix(argument,"world") )
 {
  snprintf(buf,MSL,"Census For: %s",mudnamecolor);
  send_to_char(center_text(buf,132),ch);
  send_to_char("\r\n------------------------------------------------------------------------------------------------------------------------------------\r\n",ch);
  for( li = char_list.begin(); li != char_list.end(); li++ )
  {
   vch = *li;
   if( !IS_NPC(vch) )
    continue;
   rcnt[vch->race]++;
   ccnt[vch->p_class]++;
   scnt[vch->sex]++;
  }
 }
 else if( !str_prefix(argument,"area") )
 {
  snprintf(buf,MSL,"Census For: %s",ch->in_room->area->name);
  send_to_char(center_text(buf,132),ch);
  send_to_char("\r\n------------------------------------------------------------------------------------------------------------------------------------\r\n",ch);
  for( li = char_list.begin(); li != char_list.end(); li++ )
  {
   vch = *li;
   if( !IS_NPC(vch) )
    continue;
   if( vch->in_room->area != ch->in_room->area )
    continue;
   rcnt[vch->race]++;
   ccnt[vch->p_class]++;
   scnt[vch->sex]++;
  }
 }
 else
 {
  do_census(ch,"");
  return;
 }

 /* Tally the sexes! */
 tf0 = (scnt[SEX_NEUTRAL] + scnt[SEX_MALE] + scnt[SEX_FEMALE]);
 tf1 = (scnt[SEX_NEUTRAL] / tf0) * 100;
 tf2 = (scnt[SEX_MALE] / tf0) * 100;
 tf3 = (scnt[SEX_FEMALE] / tf0) * 100;
 snprintf(buf,MSL,"[SEX  ] ");
 send_to_char(buf,ch);
 snprintf(buf,MSL,"%9s: %4d (%05.2f%%) %9s: %4d (%05.2f%%) %9s: %4d (%05.2f%%)\r\n","Neutral",scnt[SEX_NEUTRAL],tf1,"Male",scnt[SEX_MALE],tf2,"Female",scnt[SEX_FEMALE],tf3);
 send_to_char(buf,ch);

 /* Tally the classes! */
 tf0 = 0;
 for( i = 0; i < MAX_CLASS; i++ )
  tf0 += ccnt[i];
 snprintf(buf,MSL,"[CLASS] ");
 for( i = 0; i < MAX_CLASS; i++ )
 {
  snprintf(argument,MSL,"%9s: %4d (%05.2f%%) ",class_table[i].who_name,ccnt[i],((ccnt[i] / tf0) * 100));
  strncat(buf,argument,MSL-1);
 }
 strncat(buf,"\r\n",MSL);
 send_to_char(buf,ch);

 /* Tally the races! */
 tf0 = 0;
 ti1 = 0;
 for( i = 0; i < MAX_RACE; i++ )
  tf0 += rcnt[i];
 snprintf(buf,MSL,"[RACE ] ");
 for( i = 0; i < MAX_RACE; i++ )
 {
  snprintf(argument,MSL,"%9s: %4d (%05.2f%%) ",race_table[i].race_title,rcnt[i],((rcnt[i] / tf0) * 100));
  strncat(buf,argument,MSL-1);
  if( ++ti1 % 5 == 0 && i < (MAX_RACE -1) )
   strncat(buf,"\r\n[RACE ] ",MSL);
 }

 snprintf(argument,MSL,"\r\n\r\nFound %0.0f total mobs.\r\n",tf0);
 strncat(buf,argument,MSL-1);
 send_to_char(buf,ch);

 return;
}

DO_FUN(do_sla)
{
   send_to_char( "If you want to SLAY, spell it out.\r\n", ch );
   return;
}

DO_FUN(do_slay)
{
   CHAR_DATA *victim;
   char arg[MSL];

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Slay whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( ch == victim )
   {
      send_to_char( "Suicide is a mortal sin.\r\n", ch );
      return;
   }


   if( IS_HERO( victim ) )
   {
      send_to_char( "Not on other Immortal / Adept players!\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) && victim->level >= ch->level )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   act( "You suck the life energy out of $M!", ch, NULL, victim, TO_CHAR );
   act( "$n sucks out your life energy!", ch, NULL, victim, TO_VICT );
   act( "$n sucks out $N's life energy!", ch, NULL, victim, TO_NOTVICT );
   raw_kill( victim, "" );
   return;
}

/* Here it is boys and girls the HOT reboot function and all its nifty  * little parts!! - Flar
 */
DO_FUN(do_hotreboo)
{
   send_to_char( "If you want to do a @@R@@fHOT@@Breboot@@N....spell it out.\r\n", ch );
   return;
}

DO_FUN(do_hotreboot)
{
   FILE *fp;
   DESCRIPTOR_DATA *d, *d_next;
   char buf[256], buf2[256], buf3[256];
   extern int saving_area;

   if( saving_area )
   {
      send_to_char( "Please wait until area saving is complete.\n", ch );
      return;
   }

   save_mudinfo();

   fp = fopen( COPYOVER_FILE, "w" );

   if( !fp )
   {
      send_to_char( "Copyover file not writeable, aborted.\r\n", ch );
      log_f( "Could not write to copyover file: %s", COPYOVER_FILE );
      perror( "do_copyover:file_open" );
      return;
   }

   if( auction_item != NULL )
      do_auction( ch, "stop" );

   snprintf( buf, 256,
            "\r\n**** HOTreboot by An Immortal - Please remain ONLINE ****\r\n*********** We will be back in 30 seconds!! *************\n\r\n");

   /*
    * For each PLAYING descriptor( non-negative ), save its state
    */
   for( d = first_desc; d; d = d_next )
   {
      CHAR_DATA *och = CH( d );
      d_next = d->next; /* We delete from the list , so need to save this */

      if( !d->character || d->connected < 0 )   /* drop those logging on */
      {
         write_to_descriptor( d->descriptor, "\r\n@Sorry, " mudnamecolor " is rebooting. Come back in a few minutes.\r\n" );
         close_socket( d );   /* throw'em out */
      }
      else
      {
         fprintf( fp, "%d %s %s\n", d->descriptor, och->name.c_str(), d->host );
         save_char_obj( och );
         write_to_descriptor( d->descriptor, buf );
      }
   }

   fprintf( fp, "-1\n" );
   fclose( fp );

   /*
    * Close reserve and other always-open files and release other resources
    */

   fclose( fpReserve );

#ifdef IMC
   imc_hotboot(  );
#endif

   /*
    * exec - descriptors are inherited
    */

   snprintf( buf, 256, "%d", port );
   snprintf( buf2, 256, "%d", control );
#ifdef IMC
   if( this_imcmud )
      snprintf( buf3, 256, "%d", this_imcmud->desc );
   else
      strncpy( buf3, "-1", 256 );
#else
   strncpy( buf3, "-1", 256 );
#endif

   /*
    * spec: handle profiling cleanly here
    */
#ifdef PROF
   if( !fork(  ) )   /* dump profile info */
      exit( 0 );
   signal( SIGPROF, SIG_IGN );
#endif

   execl( EXE_FILE, "AckFUSS", buf, "HOTreboot", buf2, buf3, ( char * )NULL );

   /*
    * Failed - sucessful exec will not return
    */

   perror( "do_copyover: execl" );
   send_to_char( "HOTreboot FAILED! Something is wrong in the shell!\r\n", ch );
}

/* Syntax is:
 * disable - shows disabled commands
 * disable <command> - toggles disable status of command
 */
DO_FUN(do_disable)
{
 short i;
 DISABLED_DATA *p = NULL;
 list<DISABLED_DATA *>::iterator li;
 char buf[MSL];

 if( IS_NPC(ch) )
  return;

 if( argument[0] == '\0' )
 {
  if( disabled_list.empty() )
  {
   send_to_char("There are no disabled commands.\r\n",ch);
   return;
  }

  send_to_char("Disabled commands:\r\nCommand      Level   Disabled by\r\n",ch);
  for( li = disabled_list.begin(); li != disabled_list.end(); li++ )
  {
   p = *li;

   snprintf(buf,MSL,"%-12s %5d   %-12s\r\n",p->command->name,p->level,p->disabled_by);
   send_to_char(buf,ch);
  }

  return;
 } 

 for( li = disabled_list.begin(); li != disabled_list.end(); li++ )
 {
  p = *li;

  if( !str_cmp(argument,p->command->name) )
  {

   if( get_trust(ch) < p->level )
   {
    send_to_char("This command was disabled by a higher power.\r\n",ch);
    return;
   }

   disabled_list.remove(p);
   delete p;
   save_disabled();
   send_to_char("Command enabled.\r\n",ch);
   return;
  }
 }

 if( !str_cmp(argument,"disable") )
 {
  send_to_char("You can't disable the disable command.\r\n",ch);
  return;
 }

 for( i = 0; cmd_table[i].name[0] != '\0'; i++ )
  if( !str_cmp(argument,cmd_table[i].name) )
   break;

 if( cmd_table[i].name[0] == '\0' )
 {
  send_to_char("No such command.\r\n",ch);
  return;
 }

 if( cmd_table[i].level > get_trust(ch) )
 {
  send_to_char("You can't disable a command that you cannot use.\r\n",ch);
  return;
 }

 p = new DISABLED_DATA;
 p->disabled_by = str_dup(ch->name.c_str());
 p->level = get_trust(ch);
 p->command = &cmd_table[i];

 save_disabled();
 send_to_char("Command disabled.\r\n",ch);

 return;
}

DO_FUN(do_lua)
{
 string str = SCRIPT_DIR;
 str += argument;
 if( ch->lua->L )
 {
  luaL_dofile(ch->lua->L,str.c_str());
  const char * sError = lua_tostring(ch->lua->L,-1);
  if( str_cmp(sError,"(null)") && sError != NULL )
   monitor_chan(sError,MONITOR_DEBUG);
 }
 return;
}

DO_FUN(do_olua)
{
 char arg[MSL];

 argument = one_argument(argument,arg);
 string str = SCRIPT_DIR;
 str += argument;
 OBJ_DATA *obj;

 if( (obj = get_obj_world(ch,arg)) == NULL )
  return;

 if( obj->lua )
 {
  luaL_dofile(obj->lua->L,str.c_str());
  const char * sError = lua_tostring(obj->lua->L,-1);
  if( str_cmp(sError,"(null)") && sError != NULL )
   monitor_chan(sError,MONITOR_DEBUG);
 }

 return;
}

DO_FUN(do_rlua)
{
 string str = SCRIPT_DIR;
 str += argument;
 ROOM_INDEX_DATA *room = ch->in_room;

 if( room == NULL )
  return;

 if( room->lua )
 {
  luaL_dofile(room->lua->L,str.c_str());
  const char * sError = lua_tostring(room->lua->L,-1);
  if( str_cmp(sError,"(null)") && sError != NULL )
   monitor_chan(sError,MONITOR_DEBUG);
 }

 return;
}

/* Just debug stuff past here, will remove it later. --Kline */

struct ShowObject
{
 template <typename T> void operator() (const T* ptr) const { snprintf(log_buf,(2 * MIL),"ptr: %p ptr->loaded: %d ptr->L: %p ptr->owner: %p ptr->type: %d",ptr,ptr->loaded,ptr->L,ptr->owner,ptr->type);
 monitor_chan(log_buf,MONITOR_DEBUG); };
};

DO_FUN(do_ldebug)
{
 for_each( lua_list.begin(), lua_list.end(), ShowObject() );
 return;
}
