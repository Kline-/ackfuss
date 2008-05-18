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
#include <ctype.h>
/* For forks etc. */
#include <unistd.h>
#include <fcntl.h>
#if defined(__CYGWIN__)
#include <crypt.h>
#endif
#include "globals.h"
#include "tables.h"

#ifndef DEC_EMAIL_H
#include "email.h"
#endif

void do_transdm( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *wch;
   int room;
   extern bool deathmatch;
   char buf[MAX_STRING_LENGTH];
   ROOM_INDEX_DATA *location;

   if( !deathmatch )
   {
      send_to_char( "You must run a DEATHMATCH first.\n\r", ch );
      return;
   }

   for( wch = first_char; wch != NULL; wch = wch->next )
   {
      if( !IS_NPC( wch ) && ( ch != wch ) )
      {
         if( IS_IMMORTAL( wch ) && ( wch != ch ) )
         {
            send_to_char( "Everyone has been transferred to the DM arena.\n\r", wch );
            continue;
         }

         room = number_range( 4051, 4099 );
         sprintf( buf, "%d", room );
         location = find_location( wch, buf );
         if( wch->fighting != NULL )
            stop_fighting( wch, TRUE );
         char_from_room( wch );
         char_to_room( wch, location );
         send_to_char( "You have been transferred somewhere inside the DEATHMATCH arena!\n\r", wch );
      }
   }
   send_to_char( "Everyone has been transferred to the DM arena.\n\r", ch );
}

void do_wizhelp( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   int cmd;
   int col;

   buf1[0] = '\0';
   col = 0;
   for( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
   {
      if( cmd_table[cmd].level >= LEVEL_HERO && cmd_table[cmd].level <= get_trust( ch ) )
      {
         sprintf( buf, "%-12s", cmd_table[cmd].name );
         strcat( buf1, buf );
         if( ++col % 6 == 0 )
            strcat( buf1, "\n\r" );
      }
   }

   if( col % 6 != 0 )
      strcat( buf1, "\n\r" );
   send_to_char( buf1, ch );
   return;
}

/*
void do_wizhelp( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;
    char       buf  [ MAX_STRING_LENGTH ];
    char       buf1 [ MAX_STRING_LENGTH ];
    int        cmd;
    int        col;

    rch = get_char( ch );
    
    if ( !authorized( rch, "wizhelp" ) )
        return;

    buf1[0] = '\0';
    col     = 0;

    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level < LEVEL_HERO
	    || str_infix( cmd_table[cmd].name, rch->pcdata->immskll ) )
	    continue;
	sprintf( buf, "%-10s", cmd_table[cmd].name );
	strcat( buf1, buf );
	if ( ++col % 8 == 0 )
	    strcat( buf1, "\n\r" );
    }
 
    if ( col % 8 != 0 )
	strcat( buf1, "\n\r" );
    send_to_char( buf1, ch );
    return;
}

*/

void do_bamfin( CHAR_DATA * ch, char *argument )
{
   if( !IS_NPC( ch ) )
   {
      smash_tilde( argument );
      free_string( ch->pcdata->bamfin );
      ch->pcdata->bamfin = str_dup( argument );
      send_to_char( "Ok.\n\r", ch );
   }
   return;
}



void do_bamfout( CHAR_DATA * ch, char *argument )
{
   if( !IS_NPC( ch ) )
   {
      smash_tilde( argument );
      free_string( ch->pcdata->bamfout );
      ch->pcdata->bamfout = str_dup( argument );
      send_to_char( "Ok.\n\r", ch );
   }
   return;
}



void do_deny( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
//    DESCRIPTOR_DATA d;
   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Deny whom?\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
//      bool found = FALSE;
//      found = load_char_obj( &d, arg, TRUE );

//      if (!found)
      {
         char buf[MSL];
         sprintf( buf, "%s is not online.\n\r", capitalize( arg ) );
//        sprintf( buf, "No pFile found for '%s'.\n\r", capitalize( arg ) );
         send_to_char( buf, ch );
//        free_char( d.character );
         return;
      }

//      victim = d.character;
//      d.character = NULL;
//      victim->desc = NULL;
//      LINK( victim, first_char, last_char, next, prev );
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\n\r", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\n\r", ch );
//        if ( victim->desc == NULL )
//          do_quit( victim, "" );
      return;
   }
   if( IS_SET( victim->act, PLR_DENY ) )
   {
      REMOVE_BIT( victim->act, PLR_DENY );
   }
   else
   {
      SET_BIT( victim->act, PLR_DENY );
      send_to_char( "You are denied access!\n\r", victim );
   }
   send_to_char( "OK.\n\r", ch );
   do_quit( victim, "" );

   return;
}



void do_disconnect( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   DESCRIPTOR_DATA *d;
   CHAR_DATA *victim;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Disconnect whom?\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
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
         send_to_char( "Ok.\n\r", ch );
         return;
      }
   }

   bug( "Do_disconnect: desc not found.", 0 );
   send_to_char( "Descriptor not found!\n\r", ch );
   return;
}



void do_pardon( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: pardon <character> <killer|thief>.\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\n\r", ch );
      return;
   }

   if( !str_cmp( arg2, "killer" ) )
   {
      if( IS_SET( victim->act, PLR_KILLER ) )
      {
         REMOVE_BIT( victim->act, PLR_KILLER );
         send_to_char( "Killer flag removed.\n\r", ch );
         send_to_char( "You are no longer a KILLER.\n\r", victim );
      }
      return;
   }

   if( !str_cmp( arg2, "thief" ) )
   {
      if( IS_SET( victim->act, PLR_THIEF ) )
      {
         REMOVE_BIT( victim->act, PLR_THIEF );
         send_to_char( "Thief flag removed.\n\r", ch );
         send_to_char( "You are no longer a THIEF.\n\r", victim );
      }
      return;
   }

   send_to_char( "Syntax: pardon <character> <killer|thief>.\n\r", ch );
   return;
}



void do_echo( CHAR_DATA * ch, char *argument )
{
   DESCRIPTOR_DATA *d;

   if( argument[0] == '\0' )
   {
      send_to_char( "Echo what?\n\r", ch );
      return;
   }

   for( d = first_desc; d; d = d->next )
   {
      if( d->connected == CON_PLAYING )
      {
         send_to_char( argument, d->character );
         send_to_char( "@@g\n\r", d->character );
      }
   }

   return;
}



void do_recho( CHAR_DATA * ch, char *argument )
{
   DESCRIPTOR_DATA *d;

   if( argument[0] == '\0' )
   {
      send_to_char( "Recho what?\n\r", ch );
      return;
   }

   for( d = first_desc; d; d = d->next )
   {
      if( d->connected == CON_PLAYING && d->character->in_room == ch->in_room )
      {
         send_to_char( argument, d->character );
         send_to_char( "@@g\n\r", d->character );
      }
   }

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



void do_transfer( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *location;
   DESCRIPTOR_DATA *d;
   CHAR_DATA *victim;
//    DESCRIPTOR_DATA df;
//    bool found = FALSE;
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Transfer whom (and where)?\n\r", ch );
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
            char buf[MAX_STRING_LENGTH];
            sprintf( buf, "%s %s", d->character->name, arg2 );
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
         send_to_char( "No such location.\n\r", ch );
         return;
      }

      if( room_is_private( location ) )
      {
         send_to_char( "That room is private right now.\n\r", ch );
         return;
      }
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {

//      found = load_char_obj( &df, arg1, TRUE );

//      if (!found)
      {
         char buf[MSL];
//        sprintf( buf, "No pFile found for '%s'.\n\r", capitalize( arg1 ) );
         sprintf( buf, "%s is not online.\n\r", capitalize( arg1 ) );
         send_to_char( buf, ch );
//        free_char( df.character );
         return;
      }

//      victim = df.character;
//      df.character = NULL;
//      victim->desc = NULL;
//      LINK( victim, first_char, last_char, next, prev );
   }
   if( victim->in_room == NULL )
   {
      send_to_char( "They are in limbo.\n\r", ch );
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
   send_to_char( "Ok.\n\r", ch );
}

void do_at( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *location;
   ROOM_INDEX_DATA *original;
   CHAR_DATA *wch;

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "At where what?\n\r", ch );
      return;
   }

   if( ( location = find_location( ch, arg ) ) == NULL )
   {
      send_to_char( "No such location.\n\r", ch );
      return;
   }

   if( room_is_private( location ) && ( ch->level != 85 ) )
   {
      send_to_char( "That room is private right now.\n\r", ch );
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
   for( wch = first_char; wch != NULL; wch = wch->next )
   {
      if( wch == ch )
      {
         char_from_room( ch );
         char_to_room( ch, original );
         break;
      }
   }

   return;
}



void do_goto( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *location;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Goto where?\n\r", ch );
      return;
   }

   if( ( location = find_location( ch, arg ) ) == NULL )
   {
      send_to_char( "No such location.\n\r", ch );
      return;
   }

   if( room_is_private( location ) )
   {
      send_to_char( "That room is private right now.\n\r", ch );
      return;
   }

   if( ch->fighting != NULL )
      stop_fighting( ch, TRUE );
   /*
    * if ( !IS_SET(ch->act, PLR_WIZINVIS) )  
    */
   {
      act( "$L$n $T.", ch, NULL,
           ( ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0' )
           ? ch->pcdata->bamfout : "leaves in a swirling mist", TO_ROOM );
   }

   char_from_room( ch );
   char_to_room( ch, location );

/*    if ( !IS_SET(ch->act, PLR_WIZINVIS) )   */
   {
      act( "$L$n $T.", ch, NULL,
           ( ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0' )
           ? ch->pcdata->bamfin : "appears in a swirling mist", TO_ROOM );
   }

   do_look( ch, "auto" );
   return;
}



void do_rstat( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *location;
   ROOM_AFFECT_DATA *raf;
   OBJ_DATA *obj;
   CHAR_DATA *rch;
   int door;

   one_argument( argument, arg );
   location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
   if( location == NULL )
   {
      send_to_char( "No such location.\n\r", ch );
      return;
   }

   if( ch->in_room != location && room_is_private( location ) && ( ch->level != 85 ) )
   {
      send_to_char( "That room is private right now.\n\r", ch );
      return;
   }

   buf1[0] = '\0';

   sprintf( buf, "Name: '%s.'\n\rArea: '%s'.\n\r", location->name, location->area->name );
   strcat( buf1, buf );

   sprintf( buf,
            "Vnum: %d.  Light: %d.  Sector: %s.\n\r",
            location->vnum, location->light, rev_table_lookup( tab_sector_types, location->sector_type ) );
   strcat( buf1, buf );

   sprintf( buf,
            "Room flags: %s.\n\rDescription:\n\r%s",
            bit_table_lookup( tab_room_flags, location->room_flags ), location->description );
   strcat( buf1, buf );

   if( location->first_exdesc != NULL )
   {
      EXTRA_DESCR_DATA *ed;

      strcat( buf1, "Extra description keywords: '" );
      for( ed = location->first_exdesc; ed; ed = ed->next )
      {
         strcat( buf1, ed->keyword );
         if( ed->next != NULL )
            strcat( buf1, " " );
      }
      strcat( buf1, "'.\n\r" );
   }

   strcat( buf1, "Characters:" );
   for( rch = location->first_person; rch; rch = rch->next_in_room )
   {
      strcat( buf1, " " );
      one_argument( rch->name, buf );
      strcat( buf1, buf );
   }

   strcat( buf1, ".\n\rObjects:   " );
   for( obj = location->first_content; obj; obj = obj->next_in_room )
   {
      strcat( buf1, " " );
      one_argument( obj->name, buf );
      strcat( buf1, buf );
   }
   strcat( buf1, ".\n\r" );

   for( door = 0; door <= 5; door++ )
   {
      EXIT_DATA *pexit;

      if( ( pexit = location->exit[door] ) != NULL )
      {
         sprintf( buf,
                  "Door: %d.  To: %d.  Key: %d.  Exit flags: %d.\n\rKeyword: '%s'.  Description: %s",
                  door,
                  pexit->to_room != NULL ? pexit->to_room->vnum : 0,
                  pexit->key,
                  pexit->exit_info, pexit->keyword, pexit->description[0] != '\0' ? pexit->description : "(none).\n\r" );
         strcat( buf1, buf );
      }
   }

   if( location->first_room_affect != NULL )
   {
      for( raf = location->first_room_affect; raf != NULL; raf = raf->next )
      {
         sprintf( buf, "Room_Affect: '%s', level %d, duration %d\n\r",
                  skill_table[raf->type].name, raf->level, raf->duration );
         strcat( buf1, buf );
      }
   }

   send_to_char( buf1, ch );
   return;
}



void do_ostat( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   AFFECT_DATA *paf;
   OBJ_DATA *obj;
   int cnt;
   int fubar;
   char *foo;
   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Ostat what?\n\r", ch );
      return;
   }

   buf1[0] = '\0';

   if( ( obj = get_obj_world( ch, arg ) ) == NULL )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
      return;
   }

   sprintf( buf, "Name: %s.\n\r", obj->name );
   strcat( buf1, buf );

   sprintf( buf, "Vnum: %d.  Type: %s.\n\r", obj->pIndexData->vnum, item_type_name( obj ) );
   strcat( buf1, buf );

   sprintf( buf, "Short description: %s.\n\rLong description: %s\n\r", obj->short_descr, obj->description );
   strcat( buf1, buf );

   sprintf( buf, "Wear bits: %s.\n\rExtra bits: %s.\n\r",
            bit_table_lookup( tab_wear_flags, obj->wear_flags ), extra_bit_name( obj->extra_flags ) );
   strcat( buf1, buf );

   sprintf( buf, "ITEM_APPLY: %d.\n\r", obj->item_apply );
   strcat( buf1, buf );

   sprintf( buf, "Number: %d/%d.  Weight: %d/%d.\n\r", 1, get_obj_number( obj ), obj->weight, get_obj_weight( obj ) );
   strcat( buf1, buf );

   sprintf( buf, "Cost: %d.  Timer: %d.  Level: %d.\n\r", obj->cost, obj->timer, obj->level );
   strcat( buf1, buf );

   sprintf( buf, "Condition: %d%%.\n\r", obj->condition );
   strcat( buf1, buf );

   sprintf( buf,
            "In room: %d.  In object: %s.  Carried by: %s.  Wear_loc: %d.\n\r",
            obj->in_room == NULL ? 0 : obj->in_room->vnum,
            obj->in_obj == NULL ? "(none)" : obj->in_obj->short_descr,
            obj->carried_by == NULL ? "(none)" : obj->carried_by->name, obj->wear_loc );
   strcat( buf1, buf );

   strcat( buf1, "Item Values:\n\r" );
   for( cnt = 0; cnt < 10; cnt++ )
   {
      sprintf( buf, "@@W[Value%d : @@y%6d@@W] %s",
               cnt, obj->value[cnt], rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) );
      strcat( buf1, buf );
      if( is_name( "Spell", rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) ) )
      {
         fubar = obj->value[cnt];
         if( fubar < 0 || fubar > MAX_SKILL )
            sprintf( buf, "               @@R(?)@@g\n\r" );
         else
            sprintf( buf, "               @@y(%s)@@g\n\r", skill_table[fubar].name );

      }
      else if( is_name( "Liquid", rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) ) )
      {
         foo = str_dup( rev_table_lookup( tab_drink_types, obj->value[cnt] ) );
         if( foo[0] == '\0' )
            sprintf( buf, "                  @@R(INVALID!)@@g\n\r" );
         else
            sprintf( buf, "                  @@y(%s)@@g\n\r", foo );
      }
      else if( is_name( "Weapon", rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) ) )
      {
         foo = rev_table_lookup( tab_weapon_types, obj->value[cnt] );
         if( foo[0] == '\0' )
            sprintf( buf, "                  @@R(INVALID!)@@g\n\r" );
         else
            sprintf( buf, "                  @@y(%s)@@g\n\r", foo );
      }
      else
         sprintf( buf, "@@g\n\r" );
      strcat( buf1, buf );
   }

   /*
    * sprintf( buf, "Values: %d %d %d %d.\n\r",
    * obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
    * strcat( buf1, buf );
    */

   if( obj->first_exdesc != NULL || obj->pIndexData->first_exdesc != NULL )
   {
      EXTRA_DESCR_DATA *ed;

      strcat( buf1, "Extra description keywords: '" );

      for( ed = obj->first_exdesc; ed != NULL; ed = ed->next )
      {
         strcat( buf1, ed->keyword );
         if( ed->next != NULL )
            strcat( buf1, " " );
      }

      for( ed = obj->pIndexData->first_exdesc; ed != NULL; ed = ed->next )
      {
         strcat( buf1, ed->keyword );
         if( ed->next != NULL )
            strcat( buf1, " " );
      }

      strcat( buf1, "'.\n\r" );
   }

   for( paf = obj->first_apply; paf != NULL; paf = paf->next )
   {
      sprintf( buf, "Affects %s by %d.\n\r", affect_loc_name( paf->location ), paf->modifier );
      strcat( buf1, buf );
   }

   send_to_char( buf1, ch );
   return;
}


void do_mstat( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   AFFECT_DATA *paf;
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Mstat whom?\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   buf1[0] = '\0';

   sprintf( buf, "Name: %s.  Race %i\n\r", victim->name, victim->race );
   strcat( buf1, buf );

   sprintf( buf, "Vnum: %d.  Sex: %s.  Room: %d.\n\r",
            IS_NPC( victim ) ? victim->pIndexData->vnum : 0,
            victim->sex == SEX_MALE ? "male" :
            victim->sex == SEX_FEMALE ? "female" : "neutral", victim->in_room == NULL ? 0 : victim->in_room->vnum );
   strcat( buf1, buf );

   if( IS_NPC( victim ) )
   {
      sprintf( buf, "Str: %d.  Int: %d.  Wis: %d.  Dex: %d.  Con: %d.\n\r",
               get_curr_str( victim ),
               get_curr_int( victim ), get_curr_wis( victim ), get_curr_dex( victim ), get_curr_con( victim ) );
      strcat( buf1, buf );
   }
   else
   {
      sprintf( buf, "Str:%d/%d  Int:%d/%d  Wis:%d/%d  Dex:%d/%d Con:%d/%d.\n\r",
               get_curr_str( victim ), victim->pcdata->max_str,
               get_curr_int( victim ), victim->pcdata->max_int,
               get_curr_wis( victim ), victim->pcdata->max_wis,
               get_curr_dex( victim ), victim->pcdata->max_dex, get_curr_con( victim ), victim->pcdata->max_con );
      strcat( buf1, buf );
   }

   if( !IS_NPC( victim ) )
   {
      sprintf( buf,
               "Mag: %d Cle: %d Thi:%d War:%d Psi:%d\n\r",
               victim->lvl[0], victim->lvl[1], victim->lvl[2], victim->lvl[3], victim->lvl[4] );
      strcat( buf1, buf );

      sprintf( buf, "Age: " );
      my_get_age( victim, buf );
      strcat( buf1, buf );
      sprintf( buf, "   (%d Hours RL play).\n\r", my_get_hours( victim ) );
      strcat( buf1, buf );

      sprintf( buf, "Class Order: %s %s %s %s %s\n\r",
               class_table[victim->pcdata->order[0]].who_name,
               class_table[victim->pcdata->order[1]].who_name,
               class_table[victim->pcdata->order[2]].who_name,
               class_table[victim->pcdata->order[3]].who_name, class_table[victim->pcdata->order[4]].who_name );
      strcat( buf1, buf );


   }

   sprintf( buf, "Hp: %d/%d.  Mana: %d/%d.  Move: %d/%d.  Practices: %d.\n\r",
            victim->hit, victim->max_hit, victim->mana, victim->max_mana, victim->move, victim->max_move, victim->practice );
   strcat( buf1, buf );

   sprintf( buf,
            "Lv: %d.  Class: %d.  Align: %d.  AC: %d.  Gold: %d.  Exp: %d.\n\r",
            victim->level, victim->class, victim->alignment, GET_AC( victim ), victim->gold, victim->exp );
   strcat( buf1, buf );

   if( !IS_NPC( victim ) )
   {
      sprintf( buf, "Race: %d (%s)%s.   Clan: %d (%s).  Balance: %d.\n\r",
               victim->race,
               race_table[victim->race].race_name,
               IS_VAMP( victim ) ? "[VAMPIRE]" : "",
               victim->pcdata->clan, clan_table[victim->pcdata->clan].clan_abbr, victim->balance );
      strcat( buf1, buf );
   }


   sprintf( buf, "Hitroll: %d.  Damroll: %d.  Position: %d.  Wimpy: %d.\n\r",
            GET_HITROLL( victim ), GET_DAMROLL( victim ), victim->position, victim->wimpy );
   strcat( buf1, buf );

   if( IS_NPC( victim ) )
   {
      sprintf( buf, "MODIFIERS: AC: %d.  Hitroll: %d.  Damroll: %d.\n\r", victim->ac_mod, victim->hr_mod, victim->dr_mod );
      strcat( buf1, buf );
      sprintf( buf, "TARGET: %s\n\r", victim->target );
      strcat( buf1, buf );
      sprintf( buf, "TIMER: %d\n\r", victim->extract_timer );
      strcat( buf1, buf );
   }

   if( !IS_NPC( victim ) )
   {
      sprintf( buf, "Page Lines: %d.\n\r", victim->pcdata->pagelen );
      strcat( buf1, buf );
   }

   sprintf( buf, "Fighting: %s.\n\r", victim->fighting ? victim->fighting->name : "(none)" );
   strcat( buf1, buf );

   if( !IS_NPC( victim ) )
   {
      sprintf( buf, "@@RBLOODLUST@@g: %d\n\r", victim->pcdata->bloodlust );
      strcat( buf1, buf );

      sprintf( buf,
               "Thirst: %d.  Full: %d.  Drunk: %d.  Saving throw: %d.\n\r",
               victim->pcdata->condition[COND_THIRST],
               victim->pcdata->condition[COND_FULL], victim->pcdata->condition[COND_DRUNK], victim->saving_throw );
      strcat( buf1, buf );
   }

   sprintf( buf, "Carry number: %d.  Carry weight: %4.2f.   @@aQuest Points@@W: @@y%3d\n\r",
            victim->carry_number, victim->carry_weight, victim->quest_points );
   strcat( buf1, buf );

   sprintf( buf, "Age: %d.  Played: %d.  Timer: %d.  Act: %d.\n\r",
            get_age( victim ), ( int )victim->played, victim->timer, victim->act );
   strcat( buf1, buf );

   sprintf( buf, "Master: %s.  Leader: %s.  Affected by: %s.\n\r",
            victim->master ? victim->master->name : "(none)",
            victim->leader ? victim->leader->name : "(none)", affect_bit_name( victim->affected_by ) );
   strcat( buf1, buf );

   sprintf( buf, "Short description: %s.\n\rLong  description: %s",
            victim->short_descr, victim->long_descr[0] != '\0' ? victim->long_descr : "(none).\n\r" );
   strcat( buf1, buf );

   if( IS_NPC( victim ) && victim->spec_fun != 0 )
      strcat( buf1, "Mobile has spec fun.\n\r" );

/*    if ( IS_NPC( victim ) 
    && IS_SET( victim->act_hunt, ACT_HUNT_MOVE )
    && victim->move_to != NO_VNUM )
    {
       sprintf( buf, "@@GMoving to room vnum: (%d) %s.@@g\n\r", victim->move_to,
       victim->movename );
       strcat( buf1, buf );
    }*/

   strcpy( buf, "@@GMoving to room vnum: (%d) %s.@@g\n\r" );
   if( victim->hunting )
      sprintf( buf1 + strlen( buf1 ), buf, victim->hunting->in_room->vnum, victim->hunting->in_room->name );
   else if( victim->hunt_obj && victim->hunt_obj->in_room )
      sprintf( buf1 + strlen( buf1 ), buf, victim->hunt_obj->in_room->vnum, victim->hunt_obj->in_room->name );


/*    if ( IS_NPC(victim) && victim->hunting != NULL)
    {
       switch( (int) victim->hunting)
       {
        case -1:
         sprintf(buf, "Hunting victim: %s (waiting)\n\r",victim->huntname);
         strcat(buf1,buf);
         break;
         
        case -2:
         sprintf(buf, "Returning home\n\r");
         strcat(buf1,buf);
         break;
         
        default:
         sprintf(buf, "Hunting victim: %s (%s)\n\r",
                IS_NPC(victim->hunting) ? victim->hunting->short_descr
                                        : victim->hunting->name,
                IS_NPC(victim->hunting) ? "MOB" : "PLAYER" );
         strcat(buf1, buf);
         if (victim->huntdirs != NULL)
         {
          sprintf(buf,"Steps to victim: %i\n\r",
                             strlen(victim->huntdirs)-victim->huntdirno);
          strcat(buf1,buf);
         }
       }
    }*/

   if( victim->hunting || victim->hunt_obj )
   {
      buf[0] = '\0';
      if( victim->hunting )
         sprintf( buf + strlen( buf ), " hunting for (%s) %s",
                  ( IS_NPC( victim->hunting ) ? "mobile" : "player" ), NAME( victim->hunting ) );
      if( victim->hunt_obj )
      {
         if( victim->hunting && IS_SET( victim->hunt_flags, HUNT_CR ) && victim->hunt_obj->item_type == ITEM_CORPSE_PC )
            strcat( buf, " to return a corpse" );
         else
            sprintf( buf + strlen( buf ), " looking for (object) %s", victim->hunt_obj->short_descr );
      }
      if( IS_NPC( victim ) && IS_SET( victim->hunt_flags, HUNT_MERC | HUNT_CR ) && victim->hunt_for )
         sprintf( buf + strlen( buf ), ", employed by %s", NAME( victim->hunt_for ) );
      strcat( buf, ".\n\r" );
      buf[1] = UPPER( buf[1] );
      strcat( buf1, buf + 1 );
   }
   else if( victim->searching )
   {
      sprintf( buf, "Searching for %s.\n\r", victim->searching );
      strcat( buf1, buf );
   }

   for( paf = victim->first_affect; paf != NULL; paf = paf->next )
   {
      sprintf( buf,
               "Spell: '%s' modifies %s by %d for %d hours with bits %s.\n\r",
               skill_table[( int )paf->type].name,
               affect_loc_name( paf->location ), paf->modifier, paf->duration, affect_bit_name( paf->bitvector ) );
      strcat( buf1, buf );
   }

   send_to_char( buf1, ch );
   return;
}

void do_olmsg( CHAR_DATA * ch, char *argument )
{
   if( !IS_NPC( ch ) )
   {
      smash_tilde( argument );
      free_string( ch->pcdata->load_msg );
      ch->pcdata->load_msg = str_dup( argument );
      send_to_char( "Ok.\n\r", ch );
   }
   return;
}
void do_ofindlev( CHAR_DATA * ch, char *argument )
{
   extern int top_obj_index;
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
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
      send_to_char( "Ofindlev what lev.?\n\r", ch );
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
   for( vnum = 0; nMatch < top_obj_index; vnum++ )
   {
      if( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
      {
         nMatch++;
         if( ( fAll ) || ( ( pObjIndex->level >= level ) && ( pObjIndex->level <= level_top ) ) )

         {
            found = TRUE;
            objlev = pObjIndex->level;

            if( IS_SET( pObjIndex->extra_flags, ITEM_REMORT ) )
            {
               sprintf( buf, "\n\r(@@mREMORT@@N) [%3d] [%5d] %s", pObjIndex->level,
                        pObjIndex->vnum, capitalize( pObjIndex->short_descr ) );
               safe_strcat( MAX_STRING_LENGTH, buf1, buf );
            }
            else
            {
               sprintf( buf, "\n\r(@@aMORTAL@@N) [%3d] [%5d] %s", pObjIndex->level,
                        pObjIndex->vnum, capitalize( pObjIndex->short_descr ) );
               safe_strcat( MAX_STRING_LENGTH, buf1, buf );
            }
         }
      }
   }
   safe_strcat( MSL, buf1, "\n\r" );
   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
      return;
   }
   sprintf( buf, "Ofindlev report for level range %d to %d", level, level_top );
   send_rep_out( ch, buf1, mailme, buf );
   return;
}

void do_mfind( CHAR_DATA * ch, char *argument )
{
   extern int top_mob_index;
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
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
      send_to_char( "Mfind whom?\n\r", ch );
      return;
   }

   sprintf( buf1, "%s", "  Vnum   Lvl    Mob\n\r" );
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   /*
    * Yeah, so iterating over all vnum's takes 10,000 loops.
    * Get_mob_index is fast, and I don't feel like threading another link.
    * Do you?
    * -- Furey
    */
   for( vnum = 0; nMatch < top_mob_index; vnum++ )
   {
      if( ( pMobIndex = get_mob_index( vnum ) ) != NULL )
      {
         nMatch++;
         if( fAll || is_name( arg, pMobIndex->player_name ) )
         {
            found = TRUE;
            sprintf( buf, "[%5d] [%3d] %s\n\r", pMobIndex->vnum, pMobIndex->level, capitalize( pMobIndex->short_descr ) );
            safe_strcat( MAX_STRING_LENGTH, buf1, buf );
         }
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
      return;
   }

   sprintf( buf, "Mfind report for %s", arg );
   send_rep_out( ch, buf1, mailme, buf );
   return;
}

void do_mfindlev( CHAR_DATA * ch, char *argument )
{
   extern int top_mob_index;
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
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
      send_to_char( "Mfindlev what lev.?\n\r", ch );
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
   for( vnum = 0; nMatch < top_mob_index; vnum++ )
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

            sprintf( buf, "(%3d) [%3d] [%5d] %s\n\r",
                     perkills, pMobIndex->level, pMobIndex->vnum, capitalize( pMobIndex->short_descr ) );
            safe_strcat( MAX_STRING_LENGTH, buf1, buf );
         }
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
      return;
   }

   sprintf( buf, "Mfindlev report for level range %d to %d", level, level_top );
   send_rep_out( ch, buf1, mailme, buf );
   return;
}


void do_ofind( CHAR_DATA * ch, char *argument )
{
   extern int top_obj_index;
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
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
      send_to_char( "Ofind what?\n\r", ch );
      return;
   }

   sprintf( buf1, "%s", " Vnum  Lvl  Flag    Item\n\r" );
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   /*
    * Yeah, so iterating over all vnum's takes 10,000 loops.
    * Get_obj_index is fast, and I don't feel like threading another link.
    * Do you?
    * -- Furey
    */
   for( vnum = 0; nMatch < top_obj_index; vnum++ )
   {
      if( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
      {
         nMatch++;
         if( fAll || is_name( arg, pObjIndex->name ) )
         {
            found = TRUE;
            sprintf( buf, "[%5d] [%3d] %s %s\n\r",
                     pObjIndex->vnum, pObjIndex->level,
                     ( IS_SET( pObjIndex->extra_flags, ITEM_REMORT ) ?
                       "@@mRemort@@N" : "@@aMortal@@N" ), capitalize( pObjIndex->short_descr ) );
            safe_strcat( MAX_STRING_LENGTH, buf1, buf );
         }
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
      return;
   }

   sprintf( buf, "Ofind report for %s", arg );
   send_rep_out( ch, buf1, mailme, buf );
   return;
}


void do_mwhere( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   bool found;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Mwhere whom? or int to see the intelligent mobs.\n\r", ch );
      return;
   }
   if( !str_cmp( arg, "int" ) )
   {
      for( victim = first_char; victim != NULL; victim = victim->next )
      {
         if( IS_NPC( victim ) && victim->in_room != NULL && IS_SET( victim->act, ACT_INTELLIGENT ) )
         {
            sprintf( buf, "[%5d] %-20s [%5d] %-30s\n\r",
                     victim->pIndexData->vnum, victim->short_descr, victim->in_room->vnum, victim->in_room->name );
            send_to_char( buf, ch );
         }

      }
      return;
   }

   found = FALSE;
   for( victim = first_char; victim != NULL; victim = victim->next )
   {
      if( IS_NPC( victim ) && victim->in_room != NULL && is_name( arg, victim->name ) )
      {
         found = TRUE;
         sprintf( buf, "[%5d] %-20s [%5d] %-30s\n\r",
                  victim->pIndexData->vnum, victim->short_descr, victim->in_room->vnum, victim->in_room->name );
         send_to_char( buf, ch );
      }
   }

   if( !found )
   {
      send_to_char( "Nope, couldn't find it.\n\r", ch );
      return;
   }

   return;
}



void do_reboo( CHAR_DATA * ch, char *argument )
{
   send_to_char( "If you want to REBOOT, spell it out.\n\r", ch );
   return;
}



void do_reboot( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   extern bool merc_down;
   extern int saving_area;

   build_save_flush(  );

   if( saving_area )
   {
      send_to_char( "Please wait until area saving complete.\n", ch );
      return;
   }

   sprintf( buf, "Reboot by %s.", ch->name );
   do_echo( ch, buf );
   merc_down = TRUE;
   return;
}



void do_shutdow( CHAR_DATA * ch, char *argument )
{
   send_to_char( "If you want to SHUTDOWN, spell it out.\n\r", ch );
   return;
}



void do_shutdown( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   extern bool merc_down;
   extern int saving_area;

   build_save_flush(  );

   if( saving_area )
   {
      send_to_char( "Please wait until area saving complete.\n", ch );
      return;
   }

   sprintf( buf, "Shutdown by %s.", ch->name );
   append_file( ch, SHUTDOWN_FILE, buf );
   strcat( buf, "\n\r" );
   do_echo( ch, buf );
   merc_down = TRUE;
   return;
}



void do_snoop( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   DESCRIPTOR_DATA *d;
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Snoop whom?\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( victim->desc == NULL )
   {
      send_to_char( "No descriptor to snoop.\n\r", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "Cancelling all snoops.\n\r", ch );
      for( d = first_desc; d != NULL; d = d->next )
      {
         if( d->snoop_by == ch->desc )
            d->snoop_by = NULL;
      }
      return;
   }

   if( victim->desc->snoop_by != NULL )
   {
      send_to_char( "Busy already.\n\r", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\n\r", ch );
      return;
   }

   if( ch->desc != NULL )
   {
      for( d = ch->desc->snoop_by; d != NULL; d = d->snoop_by )
      {
         if( d->character == victim || d->original == victim )
         {
            send_to_char( "No snoop loops.\n\r", ch );
            return;
         }
      }
   }

   victim->desc->snoop_by = ch->desc;
   send_to_char( "Ok.\n\r", ch );
   return;
}



void do_switch( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Switch into whom?\n\r", ch );
      return;
   }

   if( ch->desc == NULL )
      return;

   if( ch->desc->original != NULL )
   {
      send_to_char( "You are already switched.\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "Ok.\n\r", ch );
      return;
   }

   /*
    * Pointed out by Da Pub (What Mud)
    */
   if( !IS_NPC( victim ) )
   {
      send_to_char( "You cannot switch into a player!\n\r", ch );
      return;
   }

   if( victim->desc != NULL )
   {
      send_to_char( "Character in use.\n\r", ch );
      return;
   }

   ch->desc->character = victim;
   ch->desc->original = ch;
   victim->desc = ch->desc;
   ch->desc = NULL;
   send_to_char( "Ok.\n\r", victim );
   return;
}



void do_return( CHAR_DATA * ch, char *argument )
{
   if( ch->desc == NULL )
      return;

   if( ch->desc->original == NULL )
   {
      send_to_char( "You aren't switched.\n\r", ch );
      return;
   }

   send_to_char( "You return to your original body.\n\r", ch );
   ch->desc->character = ch->desc->original;
   ch->desc->original = NULL;
   ch->desc->character->desc = ch->desc;
   ch->desc = NULL;
   return;
}



void do_mload( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   MOB_INDEX_DATA *pMobIndex;
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' || !is_number( arg ) )
   {
      send_to_char( "Syntax: mload <vnum>.\n\r", ch );
      return;
   }

   if( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
   {
      send_to_char( "No mob has that vnum.\n\r", ch );
      return;
   }

   victim = create_mobile( pMobIndex );
   char_to_room( victim, ch->in_room );
   act( "$n has created $N!", ch, NULL, victim, TO_ROOM );
   send_to_char( "Ok.\n\r", ch );
   return;
}



void do_oload( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   OBJ_INDEX_DATA *pObjIndex;
   OBJ_DATA *obj;
   int level;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' || !is_number( arg1 ) )
   {
      send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );
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
         send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );
         return;
      }
      level = atoi( arg2 );
      if( level < 0 || level > get_trust( ch ) )
      {
         send_to_char( "Limited to your trust level.\n\r", ch );
         return;
      }
   }

   if( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
   {
      send_to_char( "No object has that vnum.\n\r", ch );
      return;
   }

   if( IS_SET( pObjIndex->extra_flags, ITEM_CLAN_EQ ) && ( ch->level != MAX_LEVEL ) )
   {
      send_to_char( "Only Creators can OLOAD clan equipment.\n\r", ch );
      return;
   }

   obj = create_object( pObjIndex, level );
   if( CAN_WEAR( obj, ITEM_TAKE ) )
   {
      act( "$n @@mgestures majestically, and@@N $p @@mappears with a crash of @@WTHUNDER!!@@N", ch, obj, NULL, TO_ROOM );
      obj_to_char( obj, ch );
   }
   else
   {
      obj_to_room( obj, ch->in_room );
      act( "$n @@mgestures, and a @@N$p@@M appears with a thunderous crash@@N!!!", ch, obj, NULL, TO_ROOM );
   }
   send_to_char( "Ok.\n\r", ch );
   return;
}



void do_purge( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
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
      send_to_char( "Your burst of Holy fire cleanses the room!\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( !IS_NPC( victim ) )
   {
      send_to_char( "Not on PC's.\n\r", ch );
      return;
   }

   act( "$n obliterates $N with Holy fire!", ch, NULL, victim, TO_NOTVICT );
   act( "You obliterate $N with Holy fire!", ch, NULL, victim, TO_CHAR );
   extract_char( victim, TRUE );
   return;
}



void do_advance( CHAR_DATA * ch, char *argument )
{
   send_to_char( "Use setclass instead.  Advance no longer works here.\n\r", ch );
   return;
}



void do_trust( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int level;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
   {
      send_to_char( "Syntax: trust <char> <level>.\n\r", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg1 ) ) == NULL )
   {
      send_to_char( "That player is not here.\n\r", ch );
      return;
   }

   if( ( level = atoi( arg2 ) ) < 0 || level > MAX_LEVEL )
   {
      sprintf( buf, "Level must be 0 (reset) or 1 to %d.\n\r", MAX_LEVEL );
      send_to_char( buf, ch );
      return;
   }

   if( level > get_trust( ch ) )
   {
      send_to_char( "Limited to your trust.\n\r", ch );
      return;
   }

   victim->trust = level;
   return;
}



void do_restore( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Restore whom?\n\r", ch );
      return;
   }


   if( !str_cmp( arg, "all" ) )
   {
      /*
       * then loop through all players and restore them 
       */
      CHAR_DATA *vch;
      CHAR_DATA *vch_next;


      for( vch = first_char; vch != NULL; vch = vch_next )
      {
         vch_next = vch->next;
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

      send_to_char( "Everyone has been restored.\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here!\n\r", ch );
      return;
   }

   victim->hit = victim->max_hit;
   victim->mana = victim->max_mana;
   victim->move = victim->max_move;
   update_pos( victim );
   act( "$n kindly restores you.", ch, NULL, victim, TO_VICT );
   send_to_char( "Ok.\n\r", ch );
   return;
}



void do_freeze( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Freeze whom?\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\n\r", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\n\r", ch );
      return;
   }

   if( IS_SET( victim->act, PLR_FREEZE ) )
   {
      REMOVE_BIT( victim->act, PLR_FREEZE );
      send_to_char( "You can play again.\n\r", victim );
      send_to_char( "FREEZE removed.\n\r", ch );
   }
   else
   {
      SET_BIT( victim->act, PLR_FREEZE );
      send_to_char( "You can't do ANYthing!\n\r", victim );
      sprintf( buf, "You have been FROZEN by %s!!\n\r", ch->name );
      send_to_char( buf, victim );
      send_to_char( "Freeze set.\n\r", ch );

      sprintf( buf, "%s has been FROZEN by %s.\n\r", victim->name, ch->name );
      notify( buf, ch->level + 1 );
   }

   save_char_obj( victim );

   return;
}



void do_log( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

// If anyone has a way to do this with the character not logged in,
// please let zenithar@mindspring.com know :)

//    DESCRIPTOR_DATA d;
//    bool found = FALSE;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Log whom?\n\r", ch );
      return;
   }

   if( !str_cmp( arg, "all" ) )
   {
      if( fLogAll )
      {
         fLogAll = FALSE;
         send_to_char( "Log ALL off.\n\r", ch );
      }
      else
      {
         fLogAll = TRUE;
         send_to_char( "Log ALL on.\n\r", ch );
      }
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
//      found = load_char_obj( &d, arg, TRUE );

//      if (!found)
      {
         char buf[MSL];
//        sprintf( buf, "No pFile found for '%s'.\n\r", capitalize( arg ) );
         sprintf( buf, "%s is not online.\n\r", capitalize( arg ) );
         send_to_char( buf, ch );
//        free_char( d.character );
         return;
      }

//      victim = d.character;
//      d.character = NULL;
//      victim->desc = NULL;
//      LINK( victim, first_char, last_char, next, prev );
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\n\r", ch );
      return;
   }

   /*
    * No level check, gods can log anyone.
    */
   if( IS_SET( victim->act, PLR_LOG ) )
   {
      REMOVE_BIT( victim->act, PLR_LOG );
      send_to_char( "LOG removed.\n\r", ch );
   }
   else
   {
      SET_BIT( victim->act, PLR_LOG );
      send_to_char( "LOG set.\n\r", ch );
   }
//    if ( found )
//    {
//      do_quit( victim, "" );
//    }
   return;
}


void do_noemote( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Noemote whom?\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\n\r", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\n\r", ch );
      return;
   }

   if( IS_SET( victim->act, PLR_NO_EMOTE ) )
   {
      REMOVE_BIT( victim->act, PLR_NO_EMOTE );
      send_to_char( "You can emote again.\n\r", victim );
      send_to_char( "NO_EMOTE removed.\n\r", ch );
   }
   else
   {
      SET_BIT( victim->act, PLR_NO_EMOTE );
      send_to_char( "Your ability to emote has been removed!\n\r", victim );
      send_to_char( "NO_EMOTE set.\n\r", ch );
   }

   return;
}



void do_notell( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Notell whom?", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\n\r", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\n\r", ch );
      return;
   }

   if( IS_SET( victim->act, PLR_NO_TELL ) )
   {
      REMOVE_BIT( victim->act, PLR_NO_TELL );
      send_to_char( "You can tell again.\n\r", victim );
      send_to_char( "NO_TELL removed.\n\r", ch );
   }
   else
   {
      SET_BIT( victim->act, PLR_NO_TELL );
      send_to_char( "You now can not use the tell command!\n\r", victim );
      send_to_char( "NO_TELL set.\n\r", ch );
   }

   return;
}



void do_silence( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Silence whom?", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\n\r", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\n\r", ch );
      return;
   }

   if( IS_SET( victim->act, PLR_SILENCE ) )
   {
      REMOVE_BIT( victim->act, PLR_SILENCE );
      send_to_char( "You can use channels again.\n\r", victim );
      send_to_char( "SILENCE removed.\n\r", ch );
   }
   else
   {
      SET_BIT( victim->act, PLR_SILENCE );
      send_to_char( "You can't use channels!\n\r", victim );
      send_to_char( "SILENCE set.\n\r", ch );
   }

   return;
}

void do_nopray( CHAR_DATA * ch, char *argument )
{
   /*
    * Remove victim's ability to use pray channel.. -S- 
    */

   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "NoPray whom?", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\n\r", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\n\r", ch );
      return;
   }

   if( IS_SET( victim->act, PLR_NO_PRAY ) )
   {
      REMOVE_BIT( victim->act, PLR_NO_PRAY );
      send_to_char( "You can use 'PRAY' again.\n\r", victim );
      send_to_char( "NOPRAY removed.\n\r", ch );
   }
   else
   {
      SET_BIT( victim->act, PLR_NO_PRAY );
      send_to_char( "You can't use 'PRAY'!\n\r", victim );
      send_to_char( "NOPRAY set.\n\r", ch );
   }

   return;
}

void do_peace( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *rch;

   for( rch = ch->in_room->first_person; rch != NULL; rch = rch->next_in_room )
   {
      if( rch->fighting != NULL )
         stop_fighting( rch, TRUE );
   }

   send_to_char( "Ok.\n\r", ch );
   return;
}



void do_ban( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   char arg2[MSL];
   char buf2[MSL];

   BAN_DATA *pban;
   buf[0] = '\0';
   buf2[0] = '\0';


   if( IS_NPC( ch ) )
      return;

   argument = one_argument( argument, arg );
   one_argument( argument, arg2 );


   if( arg[0] == '\0' )
   {
      strcpy( buf, "Banned sites:\n\r" );
      for( pban = first_ban; pban != NULL; pban = pban->next )
      {
         strcat( buf, pban->name );
         sprintf( buf2, ( pban->newbie ? " Newbies" : " All" ) );
         safe_strcat( MSL, buf, buf2 );
         sprintf( buf2, "  Banned by: %s", pban->banned_by );
         safe_strcat( MSL, buf, buf2 );
         strcat( buf, "\n\r" );
      }
      send_to_char( buf, ch );
      return;
   }

   for( pban = first_ban; pban != NULL; pban = pban->next )
   {
      if( !str_cmp( arg, pban->name ) )
      {
         send_to_char( "That site is already banned!\n\r", ch );
         return;
      }
   }

   GET_FREE( pban, ban_free );
   if( !str_cmp( arg2, "newbie" ) )
      pban->newbie = TRUE;
   else
      pban->newbie = FALSE;

   pban->name = str_dup( arg );
   pban->banned_by = str_dup( ch->name );
   LINK( pban, first_ban, last_ban, next, prev );
   save_bans(  );
   send_to_char( "Ok.\n\r", ch );
   return;
}



void do_allow( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   BAN_DATA *curr;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Remove which site from the ban list?\n\r", ch );
      return;
   }

   for( curr = first_ban; curr != NULL; curr = curr->next )
   {
      if( !str_cmp( arg, curr->name ) )
      {
         UNLINK( curr, first_ban, last_ban, next, prev );

         PUT_FREE( curr, ban_free );
         send_to_char( "Ok.\n\r", ch );
         save_bans(  );
         return;
      }
   }

   send_to_char( "Site is not banned.\n\r", ch );
   return;
}


void do_deathmatc( CHAR_DATA * ch, char *argument )
{
   send_to_char( "If you want to start or end a DEATHMATCH, say it in full!\n\r", ch );
   return;
}


void do_deathmatch( CHAR_DATA * ch, char *argument )
{
   /*
    * This basically locks the mud, and stops saving.
    * * PKing is allowed with no limits ;)
    * * Only allow HIGHEST level imms to use...
    */

   extern bool deathmatch;
   deathmatch = !deathmatch;


   if( IS_NPC( ch ) )
   {
      send_to_char( "Deathmatch is not a NPC command.\n\r", ch );
      return;
   }

   if( deathmatch )
   {
      CHAR_DATA *vch;
      CHAR_DATA *vch_next;

      for( vch = first_char; vch != NULL; vch = vch_next )
      {
         vch_next = vch->next;
         if( !IS_NPC( vch ) )
            do_help( vch, "dm" );
      }

   }
   else
   {
      send_to_char( "Deathmatch ended...\n\r", ch );
      info( "+=========================+", 1 );
      info( "| DEATHMATCH HAS ENDED!!! |", 1 );
      info( "+=========================+", 1 );
   }
   return;
}

void do_wizlock( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_INPUT_LENGTH];
   extern bool wizlock;
   wizlock = !wizlock;
   sysdata.w_lock = wizlock;
   save_sysdata(  );

   if( wizlock )
   {
      send_to_char( "Game wizlocked.\n\r", ch );
      sprintf( buf, "%s wizlocks ACK! Mud.\n\r", ch->name );
   }
   else
   {
      send_to_char( "Game un-wizlocked.\n\r", ch );
      sprintf( buf, "%s un-wizlocks ACK! Mud.\n\r", ch->name );
   }
   notify( buf, get_trust( ch ) );
   return;
}



void do_slookup( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   int sn;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Slookup what?\n\r", ch );
      return;
   }

   if( !str_cmp( arg, "all" ) )
   {
      buf1[0] = '\0';
      for( sn = 0; sn < MAX_SKILL; sn++ )
      {
         if( skill_table[sn].name == NULL )
            break;
         sprintf( buf, "Sn: %4d Slot: %4d Skill/spell: '%s'\n\r", sn, skill_table[sn].slot, skill_table[sn].name );
         strcat( buf1, buf );
      }
      send_to_char( buf1, ch );
   }
   else
   {
      if( ( sn = skill_lookup( arg ) ) < 0 )
      {
         send_to_char( "No such skill or spell.\n\r", ch );
         return;
      }

      sprintf( buf, "Sn: %4d Slot: %4d Skill/spell: '%s'\n\r", sn, skill_table[sn].slot, skill_table[sn].name );
      send_to_char( buf, ch );
   }

   return;
}



void do_sset( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int value;
   int sn;
   bool fAll;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   argument = one_argument( argument, arg3 );

   if( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
   {
      send_to_char( "Syntax: sset <victim> <skill> <value>\n\r", ch );
      send_to_char( "or:     sset <victim> all     <value>\n\r", ch );
      send_to_char( "Skill being any skill or spell.\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\n\r", ch );
      return;
   }

   fAll = !str_cmp( arg2, "all" );

   if( fAll && ch->level != 85 )
   {
      send_to_char( "Only Creators may SSET all.\n\r", ch );
      return;
   }

   sn = 0;
   if( !fAll && ( sn = skill_lookup( arg2 ) ) < 0 )
   {
      send_to_char( "No such skill or spell.\n\r", ch );
      return;
   }

   /*
    * Snarf the value.
    */
   if( !is_number( arg3 ) )
   {
      send_to_char( "Value must be numeric.\n\r", ch );
      return;
   }

   value = atoi( arg3 );
   if( value < 0 || value > 99 )
   {
      send_to_char( "Value range is 0 to 99.\n\r", ch );
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



void do_mset( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   int value, max;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   if( str_cmp( arg1, "order" ) )
      strcpy( arg3, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
   {
      send_to_char( "Syntax: mset <victim> <field>  <value>\n\r", ch );
      send_to_char( "or:     mset <victim> <string> <value>\n\r", ch );
      send_to_char( "\n\r", ch );
      send_to_char( "Field being one of:\n\r", ch );
      send_to_char( "  str int wis dex con sex class level\n\r", ch );
      send_to_char( "  gold hp mana move practice align\n\r", ch );
      send_to_char( "  thirst drunk full race hunt flags aff recall\n\r", ch );
      send_to_char( "  order (Sumpremes and higher only)\n\r", ch );
      send_to_char( "\n\r", ch );
      send_to_char( "String being one of:\n\r", ch );
      send_to_char( "  name short long description title spec\n\r", ch );
      send_to_char( " entry exit rulerrank\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
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
      int class[MAX_CLASS];
      int parity[MAX_CLASS];
      int index[MAX_CLASS];
      int foo;
      bool ok = TRUE;
      char arg[MAX_STRING_LENGTH];


      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPCs!\n\r", ch );
         return;
      }

      if( get_trust( ch ) < 84 )
      {
         send_to_char( "Only a Supreme or above may use this option.\n\r", ch );
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
               class[cnt] = foo;
               index[foo] = ( cnt );
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
         send_to_char( "Must be 5 3-letter abbrev for different classes.\n\r", ch );
         return;
      }

      /*
       * Copy classes to pcdata 
       */
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      {
         victim->pcdata->order[cnt] = class[cnt];
         victim->pcdata->index[cnt] = index[cnt];
      }

      send_to_char( "Your classes have been re-ordered.\n\r", victim );
      send_to_char( "Done.\n\r", ch );
      return;
   }



   if( !str_cmp( arg2, "intel" ) )
   {
      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PCs.\n\r", ch );
         return;
      }

      if( IS_SET( victim->act, ACT_INTELLIGENT ) )
      {
         send_to_char( "Removing intelligence.\n\r", ch );
         REMOVE_BIT( victim->act, ACT_INTELLIGENT );
      }
      else
      {
         send_to_char( "Adding intelligence.\n\r", ch );
         SET_BIT( victim->act, ACT_INTELLIGENT );
      }
   }








   if( !str_cmp( arg2, "str" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\n\r", ch );
         return;
      }

      max = victim->pcdata->max_str;

      if( value < 3 || value > max )
      {
         sprintf( buf, "Strength range is 3 to %d.\n\r", max );
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
         send_to_char( "Not on NPC's.\n\r", ch );
         return;
      }

      max = victim->pcdata->max_int;

      if( value < 3 || value > max )
      {
         sprintf( buf, "Intelligence range is 3 to %d.\n\r", max );
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
         send_to_char( "Not on NPC's.\n\r", ch );
         return;
      }

      max = victim->pcdata->max_wis;

      if( value < 3 || value > max )
      {
         sprintf( buf, "Wisdom range is 3 to %d.\n\r", max );
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
         send_to_char( "Not on NPC's.\n\r", ch );
         return;
      }

      max = victim->pcdata->max_dex;

      if( value < 3 || value > max )
      {
         sprintf( buf, "Dexterity range is 3 to %d.\n\r", max );
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
         send_to_char( "Not on NPC's.\n\r", ch );
         return;
      }

      max = victim->pcdata->max_con;

      if( value < 3 || value > max )
      {
         sprintf( buf, "Constitution range is 3 to %d.\n\r", max );
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
         send_to_char( "Sex range is 0 to 2.\n\r", ch );
         return;
      }
      victim->sex = value;
      victim->login_sex = value;
      return;
   }

   if( !str_cmp( arg2, "class" ) )
   {
      if( value < 0 || value >= MAX_CLASS )
      {
         char buf[MAX_STRING_LENGTH];

         sprintf( buf, "Class range is 0 to %d.\n", MAX_CLASS - 1 );
         send_to_char( buf, ch );
         return;
      }
      victim->class = value;
      return;
   }

   if( !str_cmp( arg2, "race" ) )
   {
      if( value < 0 || value >= MAX_RACE )
      {
         char buf[MAX_STRING_LENGTH];

         sprintf( buf, "Race range is 0 to %d.\n", MAX_RACE - 1 );
         send_to_char( buf, ch );
         return;
      }
      if( ( !IS_NPC( victim ) ) && ( race_table[value].player_allowed == FALSE ) )
      {
         send_to_char( "That is an NPC ONLY race.\n\r", ch );
         return;
      }
      victim->race = value;
      return;
   }
   if( !str_cmp( arg2, "rulerrank" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on Npcs.\n\r", ch );
         return;
      }
      if( value < 0 || value > 5 )
      {
         char buf[MAX_STRING_LENGTH];

         sprintf( buf, "%s", "Ruler Rank is 1 to 5.\n\r" );
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
         send_to_char( "Currently restricted to reduce abuses.\n\r", ch );
         return;
      }


      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PC's.\n\r", ch );
         return;
      }

      if( str_cmp( arg3, "." ) )
      {
         if( ( hunted = get_char_world( victim, arg3 ) ) == NULL )
         {
            send_to_char( "Mob couldn't locate the victim to hunt.\n\r", ch );
            return;
         }
         if( !set_hunt( victim, NULL, hunted, NULL, 0, 0 ) )
/*           if ( ! make_hunt(victim,hunted) )*/
            send_to_char( "Mob could not hunt victim.\n\r", ch );
      }
      else
      {
         victim->hunting = NULL;
         victim->hunt_obj = NULL;
         victim->hunt_for = NULL;
         if( victim->searching )
         {
            free_string( victim->searching );
            victim->searching = NULL;
         }
         victim->hunt_flags = victim->pIndexData->hunt_flags;
      }

      return;
   }

   if( !str_cmp( arg2, "level" ) )
   {
      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PC's.\n\r", ch );
         return;
      }

      if( value < 0 || value > 100 )
      {
         send_to_char( "Level range is 0 to 100.\n\r", ch );
         return;
      }
      victim->level = value;
      return;
   }
   if( !str_cmp( arg2, "timer" ) )
   {
      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PC's.\n\r", ch );
         return;
      }

      victim->extract_timer = value;
      return;
   }

   if( !str_cmp( arg2, "gold" ) )
   {
      victim->gold = value;
      return;
   }

   if( !str_cmp( arg2, "hp" ) )
   {
      if( value < -10 || value > 30000 )
      {
         send_to_char( "Hp range is -10 to 30,000 hit points.\n\r", ch );
         return;
      }
      victim->max_hit = value;

      return;
   }

   if( !str_cmp( arg2, "mana" ) )
   {
      if( value < 0 || value > 30000 )
      {
         send_to_char( "Mana range is 0 to 30,000 mana points.\n\r", ch );
         return;
      }
      victim->max_mana = value;
      return;
   }

   if( !str_cmp( arg2, "move" ) )
   {
      if( value < 0 || value > 2000 )
      {
         send_to_char( "Move range is 0 to 30,000 move points.\n\r", ch );
         return;
      }
      victim->max_move = value;
      return;
   }

   if( !str_cmp( arg2, "practice" ) )
   {
      if( value < 0 || value > 100 )
      {
         send_to_char( "Practice range is 0 to 100 sessions.\n\r", ch );
         return;
      }
      victim->practice = value;
      return;
   }

   if( !str_cmp( arg2, "align" ) )
   {
      if( value < -1000 || value > 1000 )
      {
         send_to_char( "Alignment range is -1000 to 1000.\n\r", ch );
         return;
      }
      victim->alignment = value;
      return;
   }

   if( !str_cmp( arg2, "supermana" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPCs.\n\r", ch );
         return;
      }
      if( IS_VAMP( victim ) || IS_WOLF( victim ) )
      {
         send_to_char( "@@eDone!!!@@N\n\r", ch );
         victim->pcdata->bloodlust_max = value;
      }
      else
         send_to_char( "They are not a supernatural!!\n\r", ch );
      return;


   }


   if( !str_cmp( arg2, "thirst" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPCs.\n\r", ch );
         return;
      }

      if( value < 0 || value > 100 )
      {
         send_to_char( "Thirst range is 0 to 100.\n\r", ch );
         return;
      }

      victim->pcdata->condition[COND_THIRST] = value;
      return;
   }

   if( !str_cmp( arg2, "drunk" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\n\r", ch );
         return;
      }

      if( value < 0 || value > 100 )
      {
         send_to_char( "Drunk range is 0 to 100.\n\r", ch );
         return;
      }

      victim->pcdata->condition[COND_DRUNK] = value;
      return;
   }

   if( !str_cmp( arg2, "full" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\n\r", ch );
         return;
      }

      if( value < 0 || value > 100 )
      {
         send_to_char( "Full range is 0 to 100.\n\r", ch );
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
         send_to_char( "Only supreme or creator level immortals may use this.\n\r", ch );
         return;
      }

      if( lookupstr[0] == '-' )
      {
         neg = 1;
         lookupstr++;
      }
      if( lookupstr[0] == '+' )
         lookupstr++;

      value = table_lookup( tab_player_flags, lookupstr );
      if( value < 1 )
      {
         sprintf( buf, "Valid player flags are :\n\r" );
         table_printout( tab_player_flags, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      if( value == PFLAG_VAMP )
      {
         send_to_char( "@@eNO WAY!!!@@N\n\r", ch );
         return;
      }

      if( neg )
         REMOVE_BIT( victim->pcdata->pflags, value );
      else
         SET_BIT( victim->pcdata->pflags, value );
      return;
   }
   if( !str_cmp( arg2, "aff" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPCs.\n\r", ch );
         return;
      }

      victim->affected_by = 0;
      return;
   }


   if( !str_cmp( arg2, "name" ) )
   {
      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PC's.\n\r", ch );
         return;
      }

      free_string( victim->name );
      victim->name = str_dup( arg3 );
      return;
   }

   if( !str_cmp( arg2, "short" ) )
   {
      free_string( victim->short_descr );
      sprintf( buf, "%s", arg3 );
      victim->short_descr = str_dup( buf );
      return;
   }

   if( !str_cmp( arg2, "long" ) )
   {
      free_string( victim->long_descr );

      sprintf( buf, "%s\n\r", arg3 );
      victim->long_descr = str_dup( buf );
      return;
   }

   if( !str_cmp( arg2, "title" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\n\r", ch );
         return;
      }

      if( ch->level < 85 )
      {
         send_to_char( "This option only available to Creators.\n\r", ch );
         return;
      }

      set_title( victim, arg3 );
      return;
   }


   if( !str_cmp( arg2, "entry" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\n\r", ch );
         return;
      }

/*	if ( ch->level < 85 )
	{
	  send_to_char( "This option only available to Creators.\n\r", ch );
	  return;
	}
*/
      free_string( victim->pcdata->room_enter );

      sprintf( buf, "%s", arg3 );
      victim->pcdata->room_enter = str_dup( buf );

      return;
   }




   if( !str_cmp( arg2, "exit" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\n\r", ch );
         return;
      }

/*	if ( ch->level < 85 )
	{
	  send_to_char( "This option only available to Creators.\n\r", ch );
	  return;
	}
*/
      free_string( victim->pcdata->room_exit );

      sprintf( buf, "%s", arg3 );
      victim->pcdata->room_exit = str_dup( buf );

      return;
   }

   if( !str_cmp( arg2, "recall" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPCs.\n\r", ch );
         return;
      }
      else
      {
         victim->pcdata->recall_vnum = victim->in_room->vnum;
         send_to_char( "Done!\n\r", ch );
         send_to_char( "You will now recall to....HERE!\n\r", victim );
      }
      return;
   }

   if( !str_cmp( arg2, "spec" ) )
   {
      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PC's.\n\r", ch );
         return;
      }

      if( ( victim->spec_fun = spec_lookup( arg3 ) ) == 0 )
      {
         send_to_char( "No such spec fun.\n\r", ch );
         return;
      }

      return;
   }

   /*
    * Generate usage message.
    */
   do_mset( ch, "" );
   return;
}



void do_oset( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
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
      send_to_char( "Syntax: oset <object> <field>  <value>\n\r", ch );
      send_to_char( "or:     oset <object> <string> <value>\n\r", ch );
      send_to_char( "\n\r", ch );
      send_to_char( "Field being one of:\n\r", ch );
      send_to_char( "  value0 value1 value2 value3 [v0,v1,v2,v3]\n\r", ch );
      send_to_char( "  extra wear level weight cost timer condition\n\r", ch );
      send_to_char( "\n\r", ch );
      send_to_char( "String being one of:\n\r", ch );
      send_to_char( "  name short long ed\n\r", ch );
      return;
   }

   if( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
      return;
   }

   if( IS_SET( obj->extra_flags, ITEM_CLAN_EQ ) && ch->level != MAX_LEVEL )
   {
      send_to_char( "Only creators can OSET Clan equipment!\n\r", ch );
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
      sprintf( numbuf, "%c", arg2[1] );
      if( is_number( numbuf ) )
      {
         indexer = atoi( numbuf );
      }
      else
      {
         sprintf( numbuf, "%c", arg1[5] );
         if( is_number( numbuf ) )
         {
            indexer = atoi( numbuf );
         }
      }
      if( ( indexer < 0 ) || ( indexer > 9 ) )
      {
         send_to_char( "Value numbers are 0 - 9.\n\r", ch );
         return;
      }
      obj->value[indexer] = value;
      return;
   }


   if( !str_cmp( arg2, "condition" ) || !str_cmp( arg2, "cond" ) )
   {
      if( value < 0 || value > 100 )
      {
         send_to_char( "Condition must be a value between 0 and 100.\n\r", ch );
         return;
      }
      obj->condition = value;
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
         sprintf( buf, "Values for extra flags are +/- :\n\r" );
         wide_table_printout( tab_obj_flags, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      if( !ok_to_use( ch, value ) )
         return;

      if( num == 1 )
         SET_BIT( obj->extra_flags, value );
      else
         REMOVE_BIT( obj->extra_flags, value );
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
         sprintf( buf, "Values for wear flags are +/- :\n\r" );
         wide_table_printout( tab_wear_flags, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      if( !ok_to_use( ch, value ) )
         return;

      if( num == 1 )
         SET_BIT( obj->wear_flags, value );
      else
         REMOVE_BIT( obj->wear_flags, value );
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
      free_string( obj->description );
      obj->description = str_dup( arg3 );
      return;
   }

   if( !str_cmp( arg2, "ed" ) )
   {
      EXTRA_DESCR_DATA *ed;

      argument = one_argument( argument, arg3 );
      if( argument == NULL )
      {
         send_to_char( "Syntax: oset <object> ed <keyword> <string>\n\r", ch );
         return;
      }

      GET_FREE( ed, exdesc_free );
      ed->keyword = str_dup( arg3 );
      ed->description = str_dup( argument );
      LINK( ed, obj->first_exdesc, obj->last_exdesc, next, prev );
      return;
   }

   /*
    * Generate usage message.
    */
   do_oset( ch, "" );
   return;
}



void do_rset( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *location;
   int value;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   strcpy( arg3, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
   {
      send_to_char( "Syntax: rset <location> <field> value\n\r", ch );
      send_to_char( "\n\r", ch );
      send_to_char( "Field being one of:\n\r", ch );
      send_to_char( "  flags sector\n\r", ch );
      return;
   }

   if( ( location = find_location( ch, arg1 ) ) == NULL )
   {
      send_to_char( "No such location.\n\r", ch );
      return;
   }

   /*
    * Snarf the value.
    */
   if( !is_number( arg3 ) )
   {
      send_to_char( "Value must be numeric.\n\r", ch );
      return;
   }
   value = atoi( arg3 );

   /*
    * Set something.
    */
   if( !str_cmp( arg2, "flags" ) )
   {
      location->room_flags = value;
      return;
   }

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



void do_users( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   char buf3[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;
   int count;


   count = 0;
   buf[0] = '\0';
   buf2[0] = '\0';

   send_to_char( "\n\r Desc.  Connection State.   Player Name.     Login Site.", ch );
   if( get_trust( ch ) == 85 )
      send_to_char( "                 Port.\n\r", ch );
   else
      send_to_char( "\n\r", ch );


   for( d = first_desc; d != NULL; d = d->next )
   {
      if( d->character != NULL )
      {
         count++;
         switch ( d->connected )
         {
            case CON_PLAYING:
               sprintf( buf3, "%s", "Playing         " );
               break;
            case CON_GET_NAME:
               sprintf( buf3, "%s", "Get Name        " );
               break;
            case CON_GET_OLD_PASSWORD:
               sprintf( buf3, "%s", "Get Old Passwd  " );
               break;
            case CON_CONFIRM_NEW_NAME:
               sprintf( buf3, "%s", "Cnrm New Name   " );
               break;
            case CON_GET_NEW_PASSWORD:
               sprintf( buf3, "%s", "Get New Passwd  " );
               break;
            case CON_CONFIRM_NEW_PASSWORD:
               sprintf( buf3, "%s", "Cnfm New Passwd " );
               break;
            case CON_GET_NEW_SEX:
               sprintf( buf3, "%s", "Get New Sex     " );
               break;
            case CON_GET_NEW_CLASS:
               sprintf( buf3, "%s", "Get New Class   " );
               break;
            case CON_GET_RACE:
               sprintf( buf3, "%s", "Get New Race    " );
               break;
            case CON_READ_MOTD:
               sprintf( buf3, "%s", "Reading MOTD    " );
               break;
            default:
               sprintf( buf3, "%s", "Unknown...      " );
               break;
         }

         sprintf( buf + strlen( buf ), "[%3d %2d %18s] %-12s %-30s",
                  d->descriptor,
                  d->connected,
                  buf3, d->original ? d->original->name : d->character ? d->character->name : "(none)", d->host );
         if( get_trust( ch ) == 85 )
            sprintf( buf + strlen( buf ), "  %5d\n\r", d->remote_port );
         else
            sprintf( buf + strlen( buf ), "\n\r" );


      }
   }

   sprintf( buf2, "%d user%s\n\r", count, count == 1 ? "" : "s" );
   strcat( buf, buf2 );
   sprintf( buf2, "%s%s%s", color_string( ch, "stats" ), buf, color_string( ch, "normal" ) );
   send_to_char( buf2, ch );
   return;
}



/*
 * Thanks to Grodyn for pointing out bugs in this function.
 */
void do_force( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   int trust;
   int cmd;

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "Force whom to do what?\n\r", ch );
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
         send_to_char( "You can't even do that yourself!\n\r", ch );
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
      CHAR_DATA *vch_next;

      if( ch->level < MAX_LEVEL )
      {
         send_to_char( "This option is only available to true Gods.\n\r", ch );
         return;
      }

      for( vch = first_char; vch != NULL; vch = vch_next )
      {
         vch_next = vch->next;

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
      CHAR_DATA *vim_next;

      for( vim = first_char; vim != NULL; vim = vim_next )
      {
         vim_next = vim->next;

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
      CHAR_DATA *vch_next;

      for( vch = first_char; vch != NULL; vch = vch_next )
      {
         vch_next = vch->next;

         if( !IS_NPC( vch ) && !IS_IMMORTAL( vch ) )
         {
            MOBtrigger = FALSE;
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
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }

      if( victim == ch )
      {
         send_to_char( "Aye aye, right away!\n\r", ch );
         return;
      }

      if( get_trust( victim ) >= get_trust( ch ) )
      {
         send_to_char( "Do it yourself!\n\r", ch );
         return;
      }

      MOBtrigger = FALSE;
      act( "$n forces you to '$t'.", ch, argument, victim, TO_VICT );
      interpret( victim, argument );
   }

   send_to_char( "Ok.\n\r", ch );
   return;
}



/*
 * New routines by Dionysos.
 */
void do_invis( CHAR_DATA * ch, char *argument )
{

   sh_int level;
   char buf[MAX_STRING_LENGTH];

   level = -1;

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

      if( IS_SET( ch->act, PLR_WIZINVIS ) )
      {
         ch->invis = level;
         sprintf( buf, "Wizinvis changed to level: %d\n\r", level );
         send_to_char( buf, ch );
         return;
      }


   }

   if( level == -1 )
      level = get_trust( ch );

   ch->invis = level;


   if( IS_NPC( ch ) )
      return;

   if( IS_SET( ch->act, PLR_WIZINVIS ) )
   {
      REMOVE_BIT( ch->act, PLR_WIZINVIS );
      act( "Small, dazzling spots of light focus into the shape of $n!", ch, NULL, NULL, TO_ROOM );

      send_to_char( "Your body becomes solid again.\n\r", ch );
   }
   else
   {
      SET_BIT( ch->act, PLR_WIZINVIS );
      act( "$n dissolves into a storm of dazzling points of light!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You slowly vanish into thin air.\n\r", ch );
      sprintf( buf, "Setting Wizinvis to level: %d.\n\r", level );
      send_to_char( buf, ch );
   }

   return;
}



void do_holylight( CHAR_DATA * ch, char *argument )
{
   if( IS_NPC( ch ) )
      return;

   if( IS_SET( ch->act, PLR_HOLYLIGHT ) )
   {
      REMOVE_BIT( ch->act, PLR_HOLYLIGHT );
      send_to_char( "Holy light mode off.\n\r", ch );
   }
   else
   {
      SET_BIT( ch->act, PLR_HOLYLIGHT );
      send_to_char( "Holy light mode on.\n\r", ch );
   }

   return;
}

/* Wizify and Wizbit sent in by M. B. King */

void do_wizify( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   argument = one_argument( argument, arg1 );
   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax: wizify <name>\n\r", ch );
      return;
   }
   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }
   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on mobs.\n\r", ch );
      return;
   }
   victim->wizbit = !victim->wizbit;
   if( victim->wizbit )
   {
      act( "$N wizified.\n\r", ch, NULL, victim, TO_CHAR );
      act( "$n has wizified you!\n\r", ch, NULL, victim, TO_VICT );
   }
   else
   {
      act( "$N dewizzed.\n\r", ch, NULL, victim, TO_CHAR );
      act( "$n has dewizzed you!\n\r", ch, NULL, victim, TO_VICT );
   }

   do_save( victim, "" );
   return;
}

/* Idea from Talen of Vego's do_where command */

void do_owhere( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char catbuf[MSL];
   char arg[MAX_INPUT_LENGTH];
   bool found = FALSE;
   OBJ_DATA *obj;
   OBJ_DATA *in_obj;
   int obj_counter = 1;
   extern OBJ_DATA *auction_item;
   bool mailme = FALSE;
   if( is_name( "mailme", argument ) )
      mailme = TRUE;
   one_argument( argument, arg );
   sprintf( buf, "Output for Owhere %s", arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Syntax:  owhere <object>.\n\r", ch );
      return;
   }
   else
   {
      for( obj = first_obj; obj != NULL; obj = obj->next )
      {
         if( !can_see_obj( ch, obj ) || !is_name( arg, obj->name ) )
            continue;
         if( obj == auction_item )
            continue;
         found = TRUE;

         for( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj );

         if( in_obj->carried_by != NULL )
         {
            sprintf( catbuf, "[%2d] %s carried by %s [Room:%d].\n\r",
                     obj_counter, obj->short_descr, PERS( in_obj->carried_by, ch ), in_obj->carried_by->in_room->vnum );
         }
         else
         {
            sprintf( catbuf, "[%2d] %s in %s [Room:%d].\n\r",
                     obj_counter,
                     obj->short_descr, ( in_obj->in_room == NULL ) ?
                     "somewhere" : in_obj->in_room->name, in_obj->in_room->vnum );
         }

         obj_counter++;
         buf[0] = UPPER( buf[0] );
         safe_strcat( MSL, buf, catbuf );

      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
   }
   else
   {
      sprintf( catbuf, "Owhere report for %s", arg );
      send_rep_out( ch, buf, mailme, catbuf );
   }
   return;
}

void do_mpcr( CHAR_DATA * ch, char *victim )
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
   char arg[MAX_INPUT_LENGTH];

   one_argument( victim, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Retrive WHICH corpse??\n\r", ch );
      return;
   }

   for( obj = first_obj; obj != NULL; obj = obj->next )
   {
      if( ( ( obj->pIndexData->vnum ) == OBJ_VNUM_CORPSE_PC ) && ( !str_cmp( arg, obj->owner ) ) && ( !( obj->in_room == ch->in_room ) ) )   /*don't work! */
      {
         found = TRUE;
         obj_from_room( obj );
         obj_to_room( obj, ch->in_room );
         act( "Got the blighter!", ch, NULL, NULL, TO_CHAR );

      }

   }

   /*
    * act used to enable mobiles to check for CR triggers... 
    */

   if( !found )
   {
      act( "Couldn't find it.", ch, NULL, NULL, TO_CHAR );
   }
   return;
}

void do_resetpassword( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   /*
    * char buf[MAX_STRING_LENGTH];  
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
      send_to_char( "This character is not playing at this time\n\r", ch );
      return;
   }
   if( ( ch->level != L_GOD ) && ch->level < victim->level )
   {
      send_to_char( "You cannot change the password of immortals!\n\r", ch );
      return;
   }
   if( IS_NPC( victim ) )
   {
      send_to_char( "You cannot change the password of NPCs!\n\r", ch );
      return;
   }


   if( ( ch->pcdata->pwd != '\0' ) && ( arg1[0] == '\0' || arg2[0] == '\0' ) )
   {
      send_to_char( "Syntax: password <char> <new>.\n\r", ch );
      return;
   }


   if( strlen( arg2 ) < 5 )
   {
      send_to_char( "New password must be at least five characters long.\n\r", ch );
      return;
   }

   pwdnew = crypt( arg2, victim->name );


   free_string( victim->pcdata->pwd );
   victim->pcdata->pwd = str_dup( pwdnew );
   save_char_obj( victim );
   send_to_char( "Ok.\n\r", ch );
   return;
}


void do_iscore( CHAR_DATA * ch, char *argument )
{
   /*
    * Show the imm bamfin/out if invis, and if wizlock/deathmatch 
    * * iscore = immortal 'score' --Stephen
    */

   char buf[MAX_STRING_LENGTH];
   extern bool deathmatch;
   extern bool wizlock;

   sprintf( buf, "(wiz) Invis: %s   Holylight: %s\n\r",
            IS_SET( ch->act, PLR_WIZINVIS ) ? "YES" : "NO ", IS_SET( ch->act, PLR_HOLYLIGHT ) ? "YES" : "NO " );
   send_to_char( buf, ch );

   if( IS_SET( ch->act, PLR_WIZINVIS ) )
   {
      sprintf( buf, "You are wizinvis at level %d.\n\r", ch->invis );
      send_to_char( buf, ch );
   }

   sprintf( buf, "Bamfin:  %s\n\r",
            ( ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0' ) ? ch->pcdata->bamfin : "Not changed/Switched." );
   send_to_char( buf, ch );

   sprintf( buf, "Bamfout: %s\n\r",
            ( ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0' ) ? ch->pcdata->bamfout : "Not changed/Switched." );
   send_to_char( buf, ch );

   sprintf( buf, "Mud Info:\n\rDeathmatch: %s   Wizlock: %s\n\r", deathmatch ? "YES" : "NO ", wizlock ? "YES" : "NO " );
   send_to_char( buf, ch );
   return;
}

void do_fights( CHAR_DATA * ch, char *argument )
{
   /*
    * Displays list of any PCs currently fighting.  Also shows 'victim'
    * * --Stephen
    */

   CHAR_DATA *vch;
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   int count;

   count = 0;
   buf2[0] = '\0';

   for( vch = first_char; vch != NULL; vch = vch->next )
   {
      if( !IS_NPC( vch ) && ( vch->fighting != NULL ) && can_see( ch, vch ) )
      {
         count++;
         sprintf( buf, "%s Vs. %s  [Room:%5d]\n\r", vch->name, IS_NPC( vch->fighting ) ?
                  vch->fighting->short_descr : vch->fighting->name, vch->in_room->vnum );
         strcat( buf2, buf );
      }
   }

   if( count == 0 )
      strcat( buf2, "No Players are currently fighting!\n\r" );
   else
   {
      sprintf( buf, "%d fight%s\n\r", count, ( count > 1 ) ? "s." : "." );
      strcat( buf2, buf );
   }

   strcat( buf2, "\n\r" );
   send_to_char( buf2, ch );
   return;
}

void do_iwhere( CHAR_DATA * ch, char *argument )
{
   /*
    * Like WHERE, except is global, and shows area & room.
    * * --Stephen
    */

   CHAR_DATA *vch;
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   int count = 0;
   buf2[0] = '\0';


   send_to_char( "Name          Room        Area\n\r", ch );
   send_to_char( "----          ----        ----\n\r", ch );

   for( vch = first_char; vch != NULL; vch = vch->next )
   {
      if( !IS_NPC( vch ) && can_see( ch, vch ) && !vch->switched )
      {

         count++;
         sprintf( buf, "%-12s [%5d] %-20s\n\r",
                  vch->name, vch->in_room == NULL ? 0 : vch->in_room->vnum, vch->in_room->area->name );
         safe_strcat( MSL, buf2, buf );
      }
   }

   if( count == 0 )
      strcat( buf2, "No Players found!\n\r" );
   else
   {
      sprintf( buf, "%d Player%s found.\n\r", count, ( count > 1 ) ? "s" : "" );
      strcat( buf2, buf );
   }

   strcat( buf2, "\n\r" );
   send_to_char( buf2, ch );
   return;
}

void do_setclass( CHAR_DATA * ch, char *argument )
{
   /*
    * New version of advance, using some of old code.
    * and a slight change to what was left.  -S-
    * Added support for setting remort class levels.
    */

   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   int value;
   int iClass;
   bool cok, remort;
   int class = 0;
   int cnt;
   int lose;
   bool vamp = FALSE;


   argument = one_argument( argument, arg1 );   /* Player */
   argument = one_argument( argument, arg2 );   /* class */
   strcpy( arg3, argument );  /* arg3 = value */

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: SetClass <player> <class> <value>\n\r", ch );
      send_to_char( "if value = -1 then player will NOT be able to level in that class.\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }
   cok = FALSE;
   remort = FALSE;

   for( iClass = 0; iClass < MAX_CLASS; iClass++ )
   {
      if( !str_cmp( arg2, class_table[iClass].who_name ) )
      {
         class = iClass;
         cok = TRUE;
      }
      if( !str_cmp( arg2, remort_table[iClass].who_name ) )
      {
         class = iClass;
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
      if( victim->adept_level > 0 )
      {
         send_to_char( "They are already an adept.\n\r", ch );
         return;
      }
      else
      {
         class = ADVANCE_ADEPT;
         advance_level( victim, class, TRUE, FALSE );
         victim->adept_level = 1;
         sprintf( buf, " %s %s", victim->name, get_adept_name( victim ) );
         do_whoname( ch, buf );
         victim->exp = 0;
         do_save( victim, "" );
         return;
      }
   }

   if( !cok )
   {
      send_to_char( "That's not a class!!\n\r", ch );
      return;
   }

   value = is_number( arg3 ) ? atoi( arg3 ) : -9;

   if( value == -9 )
   {
      send_to_char( "Invalid value for value\n\r\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\n\r", ch );
      return;
   }

   if( ( value < -1 || value > MAX_LEVEL )
/*       || ( ( vamp ) && ( value < -1 || value > MAX_VAMP_LEVEL ) ) */  )
   {
      sprintf( buf, "%d is not a valid value.\n\r", value );
      send_to_char( buf, ch );
      sprintf( buf, "Use a value between -1 and %d.\n\r\n\r", MAX_LEVEL - 1 );
      send_to_char( buf, ch );
      return;
   }

   if( value > get_trust( ch ) )
   {
      send_to_char( "Limited to your trust level.\n\r", ch );
      return;
   }

   /*
    * Lower level:
    *   Reset to level 1.
    *   Then raise again.
    *   Currently, an imp can lower another imp.
    *   -- Swiftest
    */

   if( value == ( remort ? victim->lvl2[class] : victim->lvl[class] ) )
   {
      send_to_char( "That wouldn't accomplish much!\n\r", ch );
      return;
   }
   if( ( value < ( remort ? victim->lvl2[class] : victim->lvl[class] ) )
       || ( ( vamp ) && ( value <= victim->pcdata->vamp_level ) ) )

   {
      int sn;

      lose = ( remort ? victim->lvl2[class] - 1 : victim->lvl[class] - 1 );

      send_to_char( "Lowering a player's level!\n\r", ch );
      send_to_char( "**** OOOOHHHHHHHHHH  NNNNOOOO ****\n\r", victim );

      if( vamp )
      {
         if( value != -1 )
            victim->pcdata->vamp_level = 1;
         else
            victim->pcdata->vamp_level = -1;
         victim->pcdata->vamp_exp = 0;
      }

      else if( remort )
      {
         if( value != -1 )
            victim->lvl2[class] = 1;
         else
            victim->lvl2[class] = -1;
      }
      else
         victim->lvl[class] = 1;
      victim->exp = 0;
      if( vamp )
      {
         victim->pcdata->bloodlust_max = 10;
         victim->pcdata->vamp_pracs = 2;
         victim->pcdata->vamp_skill_max = 2;
         victim->pcdata->vamp_skill_num = 1;
         victim->pcdata->bloodlust = 10;
         for( sn = 0; sn < MAX_SKILL; sn++ )
         {
            victim->pcdata->learned[sn] = 0;
         }
         victim->pcdata->learned[skill_lookup( "feed" )] = 90;
      }

      if( remort )
         victim->max_hit -= UMIN( victim->max_hit, lose * remort_table[class].hp_min );
      else
         victim->max_hit -= UMIN( victim->max_hit, lose * class_table[class].hp_min );

      victim->max_mana = 100;
      victim->max_move = 100;
      for( sn = 0; sn < MAX_SKILL; sn++ )
         victim->pcdata->learned[sn] = 0;
      victim->practice = 0;
      victim->hit = victim->max_hit;
      victim->mana = victim->max_mana;
      victim->move = victim->max_move;
      if( vamp )
      {
         class = ADVANCE_VAMP;
         advance_level( victim, class, FALSE, remort );
      }
      else
         advance_level( victim, class, FALSE, remort );
   }
   else
   {
      send_to_char( "Raising a player's level!\n\r", ch );
      send_to_char( "**** OOOOHHHHHHHHHH  YYYYEEEESSS ****\n\r", victim );
   }

   if( value != -1 && !remort && !( vamp ) )
   {
      sprintf( buf, "You are now level %d in your %s class.\n\r", value, class_table[class].class_name );
      send_to_char( buf, victim );
      for( iClass = victim->lvl[class]; iClass < value; iClass++ )
      {
         victim->lvl[class] += 1;
         advance_level( victim, class, FALSE, remort );
      }
   }
   if( remort )
   {
      sprintf( buf, "You are now level %d in your %s class.\n\r", value, remort_table[class].class_name );
      send_to_char( buf, victim );
      for( iClass = victim->lvl2[class]; iClass < value; iClass++ )
      {
         victim->lvl2[class] += 1;
         advance_level( victim, class, FALSE, remort );
      }
   }
   if( vamp )
   {
      send_to_char( "@@NYou are now a level %d @@eKindred@@N!!!\n\r", victim );
      for( iClass = victim->pcdata->vamp_level; iClass < value; iClass++ )
      {
         class = ADVANCE_VAMP;
         victim->pcdata->vamp_level += 1;
         advance_level( victim, class, FALSE, remort );
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


   send_to_char( "Ok.\n\r", ch );
   return;
}

void do_isnoop( CHAR_DATA * ch, char *argument )
{
   /*
    * Creator-only command.  Lists who (if anyone) is being snooped.
    * * -S- 
    */


   DESCRIPTOR_DATA *d;
   char buf[MAX_STRING_LENGTH];
   int count = 0;


   send_to_char( "Snoop List:\n\r-=-=-=-=-=-\n\r", ch );


   for( d = first_desc; d != NULL; d = d->next )
   {
      if( d->snoop_by != NULL )
      {
         count++;
         sprintf( buf, "%s by %s.\n\r", d->character->name, d->snoop_by->character->name );
         send_to_char( buf, ch );
      }
   }

   if( count != 0 )
      sprintf( buf, "%d snoops found.\n\r", count );
   else
      sprintf( buf, "No snoops found.\n\r" );

   send_to_char( buf, ch );
   return;
}

void do_dog( CHAR_DATA * ch, char *argument )
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
      send_to_char( "Only for creators.\n\r", ch );
      return;
   }


   if( argument[0] == '\0' )
   {
      send_to_char( "Turn WHO into a little doggy?\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Cannot do this to mobs, only pcs.\n\r", ch );
      return;
   }

   if( ( pMobIndex = get_mob_index( MOB_VNUM_DOGGY ) ) == NULL )
   {
      send_to_char( "Couldn't find the doggy's vnum!!\n\r", ch );
      return;
   }

   if( victim->desc == NULL )
   {
      send_to_char( "Already switched, like.\n\r", ch );
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
   send_to_char( "Ok.\n\r", ch );
   return;
}

void do_togbuild( CHAR_DATA * ch, char *argument )
{
   /*
    * Toggles PC's ch->act PLR_BUILDER value 
    * * -S-
    */

   CHAR_DATA *victim;


   if( argument[0] == '\0' )
   {
      send_to_char( "Toggle who as a builder??\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPCs!\n\r", ch );
      return;
   }

   if( !IS_SET( victim->act, PLR_BUILDER ) )
   {
      SET_BIT( victim->act, PLR_BUILDER );
      send_to_char( "Bit set to ALLOW building.\n\r", ch );
      send_to_char( "You have been authorized to use the builder.\n\r", victim );
   }
   else
   {
      REMOVE_BIT( victim->act, PLR_BUILDER );
      send_to_char( "Bit set to DISALLOW building.\n\r", ch );
      send_to_char( "You authorization to build has been revoked.\n\r", victim );
   }

   return;
}


void do_togleader( CHAR_DATA * ch, char *argument )
{
   /*
    * Toggles PC's ch->pcdata->pfalgs PLR_CLAN_BOSS value 
    * * -S-
    */

   CHAR_DATA *victim;


   if( argument[0] == '\0' )
   {
      send_to_char( "Toggle who as a clan boss??\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPCs!\n\r", ch );
      return;
   }

   if( !IS_SET( victim->pcdata->pflags, PFLAG_CLAN_BOSS ) )
   {
      SET_BIT( victim->pcdata->pflags, PFLAG_CLAN_BOSS );
      send_to_char( "Bit set for CLAN_BOSS.\n\r", ch );
      send_to_char( "You have been set as a clan boss.\n\r", victim );
   }
   else
   {
      REMOVE_BIT( victim->pcdata->pflags, PFLAG_CLAN_BOSS );
      send_to_char( "Bit removed for CLAN_BOSS.\n\r", ch );
      send_to_char( "You are no longer a clan boss.\n\r", victim );
   }

   return;
}


void do_whoname( CHAR_DATA * ch, char *argument )
{
   /*
    * Set victim's who name - 
    * * what appears on who list in place of their levels
    * * --Stephen
    */

   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   char foo[MAX_STRING_LENGTH];
   int side;   /* -1 = left, +1 = right side */


   if( IS_NPC( ch ) )
      return;

   argument = one_argument( argument, arg );

   if( argument[0] == '\0' || arg[0] == '\0' )
   {
      send_to_char( "Usage: whoname <victim> <string>\n\r\n\r", ch );
      send_to_char( "Where string is no more than 14 letters long.\n\r", ch );
      send_to_char( "Use 'off' as name to use default who name.\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "Couldn't find target.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Mobiles don't have a whoname!\n\r", ch );
      return;
   }

   if( ( get_trust( ch ) < ( MAX_LEVEL - 1 ) ) && ch != victim )
   {
      send_to_char( "Only Supremes and Creators can set the whoname of others.\n\r", ch );
      return;
   }

   if( !str_cmp( argument, "off" ) )
   {
      free_string( victim->pcdata->who_name );
      victim->pcdata->who_name = str_dup( "off" );
      send_to_char( "Who name set to default value.\n\r", ch );
      return;
   }

   if( nocol_strlen( argument ) > 14 )
   {
      send_to_char( "Name too long.\n\r", ch );
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
            sprintf( foo, " %s", argument );
            break;
         case 1:
            sprintf( foo, "%s ", argument );
            break;
      }
      side *= -1; /* Switch sides for next loop */
      sprintf( argument, "%s", foo );
   }

   free_string( victim->pcdata->who_name );
   victim->pcdata->who_name = str_dup( argument );
   send_to_char( "Ok, done.\n\r", ch );
   return;
}

void do_lhunt( CHAR_DATA * ch, char *argument )
{
   /*
    * Simple function for Imms... loops through all mobs, and
    * * shows details of any currently hunting someone. -S-
    */
   /*
    * Rewritten to suit new hunt functions.. :) -- Alty 
    */
   CHAR_DATA *lch;
   char buf[MAX_STRING_LENGTH];
   bool found = FALSE;

   for( lch = first_char; lch; lch = lch->next )
   {
      if( !lch->hunting && !lch->hunt_obj )
      {
         if( lch->searching )
         {
            sprintf( buf, "%s searching for %s.\n\r", NAME( lch ), lch->searching );
            send_to_char( buf, ch );
            found = TRUE;
         }
         continue;
      }
      found = TRUE;
      sprintf( buf, "%s (%s)", NAME( lch ), ( IS_NPC( lch ) ? "mobile" : "player" ) );
      if( lch->hunting )
         sprintf( buf + strlen( buf ), " hunting for (%s) %s",
                  ( IS_NPC( lch->hunting ) ? "mobile" : "player" ), NAME( lch->hunting ) );
      if( lch->hunt_obj )
      {
         if( lch->hunting && IS_SET( lch->hunt_flags, HUNT_CR ) && lch->hunt_obj->item_type == ITEM_CORPSE_PC )
            strcat( buf, " to return a corpse" );
         else
            sprintf( buf + strlen( buf ), " looking for (object) %s", lch->hunt_obj->short_descr );
      }
      if( IS_NPC( lch ) && IS_SET( lch->hunt_flags, HUNT_MERC | HUNT_CR ) && lch->hunt_for )
         sprintf( buf + strlen( buf ), ", employed by %s", NAME( lch->hunt_for ) );
      strcat( buf, ".\n\r" );
      send_to_char( buf, ch );
   }
   if( !found )
      send_to_char( "No one is currently hunting.\n\r", ch );
   return;
}

/*   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   
   for ( victim = first_char; victim != NULL; victim = victim->next )
   {
    if ( IS_NPC( victim ) 
    && IS_SET( victim->act_hunt, ACT_HUNT_MOVE )
    && victim->move_to != NO_VNUM )
    {
       sprintf( buf, "[%s] Moving to (%d) %s.g\n\r", 
          victim->short_descr,
          victim->move_to,
          victim->movename );
       strcat( buf1, buf );
    }
    
    
    if ( IS_NPC(victim) && victim->hunting != NULL)
    {
       switch( (int) victim->hunting)
       {
        case -1:
         sprintf(buf, "[%s] Hunting: %s (waiting)\n\r",
            victim->short_descr,
            victim->huntname);
         strcat(buf1,buf);
         break;
         
        case -2:
         sprintf(buf, "[%s] Returning home\n\r", victim->short_descr);
         strcat(buf1,buf);
         break;
         
        default:
         sprintf(buf, "[%s] Hunting: %s",
                victim->short_descr,
                IS_NPC(victim->hunting) ? victim->hunting->short_descr
                                        : victim->hunting->name );
         strcat(buf1, buf);
         if (victim->huntdirs != NULL)
         {
          sprintf(buf," (%i steps)",
                             strlen(victim->huntdirs)-victim->huntdirno);
          strcat(buf1,buf);
         }
         strcat( buf1, "\n\r" );
       }
    }
   } 
    send_to_char( buf1, ch );
    return;
}*/

void do_sstat( CHAR_DATA * ch, char *argument )
{
   /*
    * Lists the % for a player's skill(s)
    * * Either shows all, or value for just a given skill
    * * -S-
    */

   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   int skill = -1;
   int sn;
   int col;
   CHAR_DATA *victim;

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Usage: sstat <victim> [skill]\n\r", ch );
      send_to_char( "Where skill is an optional argument.\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "Couldn't find target.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Mobiles don't have skills!\n\r", ch );
      return;
   }

   if( argument[0] != '\0' )
   {
      skill = skill_lookup( argument );
      if( skill <= 0 )
      {
         send_to_char( "No such skill/spell!\n\r", ch );
         return;
      }
      sprintf( buf, "%17s - %3d%%\n\r", skill_table[skill].name, victim->pcdata->learned[skill] );
      send_to_char( buf, ch );
      return;
   }

   col = 0;
   buf1[0] = '\0';

   for( sn = 0; sn < MAX_SKILL; sn++ )
   {
      if( skill_table[sn].name == NULL )
         break;

      sprintf( buf, "%16s - %3d%%  ", skill_table[sn].name, victim->pcdata->learned[sn] );
      strcat( buf1, buf );

      if( ++col % 3 == 0 )
         strcat( buf1, "\n\r" );
   }
   if( col % 3 != 0 )
      strcat( buf1, "\n\r" );

   send_to_char( buf1, ch );
   return;
}

void do_test( CHAR_DATA * ch, char *argument )
{
   char testing[MSL];
   char catbuf[MSL];
   sprintf( testing, "%s", "Testing anti-color capitalize:" );
   sprintf( catbuf, argument );
   safe_strcat( MSL, testing, capitalize( catbuf ) );
   safe_strcat( MSL, testing, "\n\r" );
   send_to_char( testing, ch );
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
    "[ CONNECTION   ] Shows details of players connecting to the mud.\n\r",
    "[ connection   ] Not showing details of players connecting.\n\r"},

   {"area_update", MONITOR_AREA_UPDATE, 82, "@@p", "A_UPD",
    "[ AREA_UPDATE  ] Informs you of ALL area updates.\n\r",
    "[ area_update  ] You are not informed of area updates.\n\r"},

   {"area_bugs", MONITOR_AREA_BUGS, 82, "@@p", "A_BUG",
    "[ AREA_BUGS    ] Notifies you of any errors within areas.\n\r",
    "[ area_bugs    ] You are not told of errors within areas.\n\r"},

   {"area_save", MONITOR_AREA_SAVING, 83, "@@p", "A_SAVE",
    "[ AREA_SAVE    ] You get told of all area saving.\n\r",
    "[ area_save    ] You don't get told of all area saves.\n\r"},

   {"objects", MONITOR_OBJ, 83, "@@r", "OBJ",
    "[ OBJECTS      ] You are told of problems relating to objects.\n\r",
    "[ objects      ] You are not told of object-related problems.\n\r"},

   {"mobile", MONITOR_MOB, 83, "@@a", "MOB",
    "[ MOBILE       ] Watching mobile/player problems.\n\r",
    "[ mobile       ] Not watching problems with mobiles/players\n\r"},

   {"room", MONITOR_ROOM, 83, "@@e", "ROOM",
    "[ ROOM         ] You are informed of problems involved with rooms.\n\r",
    "[ room         ] Not informed of problems with rooms.\n\r"},
   {"magic", MONITOR_MAGIC, 83, "@@a", "MAGIC",
    "[ MAGIC        ] You are informed of various spell casting info.\n\r",
    "[ magic        ] Not informed of spell casting info.\n\r"},
   {"imm_general", MONITOR_GEN_IMM, 85, "@@y", "IMM_GEN",
    "[ IMM_GENERAL  ] You are notified of use of logged immortal commands.\n\r",
    "[ imm_general  ] You are not told of the use of logged immortal commands.\n\r"},

   {"mort_general", MONITOR_GEN_MORT, 84, "@@y", "MORT_GEN",
    "[ MORT_GENERAL ] You are notified of use of logged mortal commands.\n\r",
    "[ mort_general ] You are not told of the use of logged mortal commands.\n\r"},

   {"combat", MONITOR_COMBAT, 82, "@@R", "COMBAT",
    "[ COMBAT       ] You are monitoring problems in combat.\n\r",
    "[ combat       ] Not monitoring any combat problems.\n\r"},

   {"hunting", MONITOR_HUNTING, 82, "@@B", "HUNT",
    "[ HUNTING      ] You are told of all mobile hunting.\n\r",
    "[ hunting      ] Not told about mobiles hunting players.\n\r"},

   {"build", MONITOR_BUILD, 85, "@@y", "BUILD",
    "[ BUILD        ] You receive logged building commands.\n\r",
    "[ build        ] You don't monitor logged building commands.\n\r"},

   {"clan", MONITOR_CLAN, 84, "@@b", "CLAN",
    "[ CLAN         ] You are informed of use of certain clan commands.\n\r",
    "[ clan         ] You are not told of use of certain clan commands.\n\r"},

   {"bad", MONITOR_BAD, 85, "@@W", "BAD",
    "[ BAD          ] You are told of 'bad' things players (try to) do!\n\r",
    "[ bad          ] Not told of 'bad' things players do.\n\r"},
   {"debug", MONITOR_DEBUG, 85, "@@W", "DEBUG",
    "[ DEBUG        ] You are watching code debugging info!\n\r",
    "[ debug        ] Not watching code debugging info.\n\r"},
   {"imc", MONITOR_IMC, 84, "@@W", "IMC",
    "[ IMC          ] You are told of imc net traffic.\n\r",
    "[ imc          ] Not told of imc net traffic.\n\r"},
   {"system", MONITOR_SYSTEM, 84, "@@W", "SYSTEM",
    "[ SYSTEM       ] You are told of system messages.\n\r",
    "[ system       ] Not told of system messages.\n\r"},
   {NULL, 0, 0, NULL, NULL}
};

void do_monitor( CHAR_DATA * ch, char *argument )
{
   int a;
   bool found = FALSE;
   char buf[MAX_STRING_LENGTH];
   buf[0] = '\0';

   if( argument[0] == '\0' )
   {
      send_to_char( "@@yMonitor Channel Details:@@g\n\r\n\r", ch );
      for( a = 0; monitor_table[a].min_level != 0; a++ )
      {
         char colbuf[10];
         colbuf[0] = '\0';

         if( monitor_table[a].min_level > get_trust( ch ) )
            continue;

         if( IS_SET( ch->pcdata->monitor, monitor_table[a].channel ) )
         {
            if( !IS_NPC( ch ) )
            {
               sprintf( colbuf, "@@%c", ch->pcdata->hicol );
               safe_strcat( MSL, buf, colbuf );
            }
            safe_strcat( MAX_STRING_LENGTH, buf, monitor_table[a].on_name );
         }

         else
         {
            if( !IS_NPC( ch ) )
            {
               sprintf( colbuf, "@@%c", ch->pcdata->dimcol );
               safe_strcat( MSL, buf, colbuf );
            }
            safe_strcat( MAX_STRING_LENGTH, buf, monitor_table[a].off_name );
         }

      }


      send_to_char( buf, ch );

      send_to_char( "\n\r@@yMONITOR <name> toggles the monitor channels.@@g\n\r", ch );
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
         if( IS_SET( ch->pcdata->monitor, monitor_table[a].channel ) )
            REMOVE_BIT( ch->pcdata->monitor, monitor_table[a].channel );
         else
            SET_BIT( ch->pcdata->monitor, monitor_table[a].channel );
         break;
      }
   }
   if( !found )
   {
      do_monitor( ch, "" );
      return;
   }
   send_to_char( "Ok, monitor channel toggled.\n\r", ch );
   return;
}


void monitor_chan( const char *message, int channel )
{
   char buf[MAX_STRING_LENGTH];
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

   sprintf( buf, "%s[%7s]@@N %s@@N\n\r", monitor_table[a].col, monitor_table[a].id, strip_out( message, "\n\r" ) );

   for( d = first_desc; d; d = d->next )
   {
      if( d->connected == CON_PLAYING
          && !IS_NPC( d->character )
          && IS_SET( d->character->pcdata->monitor, channel ) && level <= get_trust( d->character ) )
      {
         send_to_char( buf, d->character );
      }

   }
   return;
}

void do_reward( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   int value;


   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );


   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: reward <victim> <value>\n\r", ch );
      send_to_char( "Value being pos to give points, or neg to take points.\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\n\r", ch );
      return;
   }




   /*
    * Snarf the value.
    */
   if( !is_number( arg2 ) )
   {
      send_to_char( "Value must be numeric.\n\r", ch );
      return;
   }

   value = atoi( arg2 );
   if( value < -100 || value > 100 )
   {
      send_to_char( "Value range is -100 to 100.\n\r", ch );
      return;
   }
   sprintf( buf, "@@NYou have been rewarded @@y%3d @@aQuest Points@@N by @@m %s @@N!!!\n\r", value, ch->name );
   send_to_char( buf, victim );
   sprintf( buf, "@@NYou have rewarded @@r%s  @@y%3d @@aQuest Points@@N!!!\n\r", victim->name, value );
   send_to_char( buf, ch );

   victim->quest_points += value;
   return;
}



void do_fhunt( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   CHAR_DATA *target;
   CHAR_DATA *victim;

   one_argument( argument, arg1 );
   one_argument( argument, arg2 );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: fhunt <victim> <target/stop>.\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "Your victim is not here.\n\r", ch );
      return;
   }

   /*
    * Do not force players to hunt. Can only force mobs 
    */
   if( !IS_NPC( victim ) )
   {
      send_to_char( "You can't force a player character to hunt.\n\r", ch );
      return;
   }

   /*
    * force a mob to stop hunting 
    */
   if( str_cmp( arg2, "stop" ) )
   {
      if( victim->hunting != NULL )
      {
         sprintf( buf, "%s stops hunting %s.\n\r", victim->short_descr, victim->hunting->short_descr );
         end_hunt( victim );
         send_to_char( buf, ch );
         return;
      }
      else
      {
         send_to_char( "They aren't hunting anyone.\n\r", ch );
         return;
      }
   }

   if( ( target = get_char_world( victim, arg2 ) ) == NULL )
   {
      send_to_char( "The new target to be hunted is not here.\n\r", ch );
      return;
   }

   /*
    * Can not force mobs to hunt non-vamp players 
    */
   if( ( !IS_VAMP( target ) ) && ( !IS_NPC( target ) ) )
   {
      send_to_char( "Mobs cannot hunt non-vamp players\n\r", ch );
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

   /*
    * once i put this skill in, remember to take out the brackets
    * * -- do_abandon will kick everyone out of the victim's group if the victim
    * * is the group leader -- or i guess the whole group can go hunting *shrug*
    * *          - Uni 
    */
/* do_abandon ( victim, "all" ); */


   if( victim->hunting != NULL )
   {
      sprintf( buf, "%s stops hunting %s.\n\r", victim->short_descr, victim->hunting->short_descr );
      end_hunt( victim );
      send_to_char( buf, ch );
   }

   victim->hunting = target;
   sprintf( buf, "%s starts hunting %s.\n\r", victim->short_descr, victim->hunting->short_descr );
   send_to_char( buf, ch );

   return;

}


void do_alink( CHAR_DATA * ch, char *argument )
{

   AREA_DATA *this_area;
   ROOM_INDEX_DATA *this_room;

   BUILD_DATA_LIST *pointer;
   ROOM_INDEX_DATA *current_room;
   int area_top, area_bottom;
   sh_int doorway;
   char buf[MAX_STRING_LENGTH];


   this_room = ch->in_room;
   this_area = ch->in_room->area;
   area_top = this_area->max_vnum;
   area_bottom = this_area->min_vnum;
   sprintf( buf, "External room links for %s.\n\r  THIS DOES NOT INCLUDE ONE WAY DOORS INTO THIS AREA.\n\r",
            this_area->name + 21 );
   send_to_char( buf, ch );

   for( pointer = this_area->first_area_room; pointer != NULL; pointer = pointer->next )
   {
      current_room = pointer->data;

      for( doorway = 0; doorway < 6; doorway++ )
      {
         EXIT_DATA *pexit;

         if( ( ( pexit = current_room->exit[doorway] ) == NULL )
             || ( pexit->to_room == NULL )
             || ( ( pexit->to_room->vnum >= area_bottom ) && ( pexit->to_room->vnum <= area_top ) ) )
            continue;
         sprintf( buf, "Room: %d linked to room: %d.\n\r", current_room->vnum, pexit->to_room->vnum );
         send_to_char( buf, ch );
      }
   }


   return;
}

void do_togcouncil( CHAR_DATA * ch, char *argument )
{
   /*
    * Toggles PC's ch->pcdata->pfalgs PLR_CLAN_BOSS value 
    * * -S-
    */

   CHAR_DATA *victim;


   if( argument[0] == '\0' )
   {
      send_to_char( "Toggle who as a council member??\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPCs!\n\r", ch );
      return;
   }

   if( !IS_SET( victim->pcdata->pflags, PFLAG_SUPER_COUNCIL ) )
   {
      SET_BIT( victim->pcdata->pflags, PFLAG_SUPER_COUNCIL );
      send_to_char( "Bit set for Council Member.\n\r", ch );
      send_to_char( "You have been set as a Council Member.\n\r", victim );
   }
   else
   {
      REMOVE_BIT( victim->pcdata->pflags, PFLAG_SUPER_COUNCIL );
      send_to_char( "Bit removed for COUNCIL MEMBER.\n\r", ch );
      send_to_char( "You are no longer a COUNCIL MEMBER.\n\r", victim );
   }

   return;
}

void do_imtlset( CHAR_DATA * ch, char *argument )
{

   CHAR_DATA *rch;
   CHAR_DATA *victim;
   char arg1[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char *buf2;
   char *buf3 = NULL;
   char *skill;
   int cmd;
   int col = 0;
   int i = 0;

   rch = get_char( ch );

   if( !authorized( rch, "imtlset" ) )
      return;

   argument = one_argument( argument, arg1 );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax: imtlset <victim> +|- <immortal skill>\n\r", ch );
      send_to_char( "or:     imtlset <victim> +|- all\n\r", ch );
      send_to_char( "or:     imtlset <victim>\n\r", ch );
      return;
   }

   if( !( victim = get_char_world( rch, arg1 ) ) )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\n\r", ch );
      return;
   }

   if( get_trust( rch ) <= get_trust( victim ) && rch != victim )
   {
      send_to_char( "You may not imtlset your peer nor your superior.\n\r", ch );
      return;
   }

   if( ( rch == victim ) && ( rch->level != MAX_LEVEL ) )
   {
      send_to_char( "You may not set your own immortal skills.\n\r", ch );
      return;
   }

   if( argument[0] == '+' || argument[0] == '-' )
   {
      buf[0] = '\0';
      smash_tilde( argument );
      if( argument[0] == '+' )
      {
         argument++;
         if( !str_cmp( "all", argument ) )

         {
            for( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
            {
               if( cmd_table[cmd].level > get_trust( rch ) )
                  continue;
               if( cmd_table[cmd].level <= victim->level && cmd_table[cmd].level >= LEVEL_HERO )
               {
                  strcat( buf, cmd_table[cmd].name );
                  strcat( buf, " " );
               }
            }
         }
         else
         {
            if( victim->pcdata->immskll )
               strcat( buf, victim->pcdata->immskll );
            while( isspace( *argument ) )
               argument++;
            for( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
            {
               if( cmd_table[cmd].level > get_trust( rch ) )
                  continue;
               if( !str_cmp( argument, cmd_table[cmd].name ) )
                  break;
            }
            if( cmd_table[cmd].name[0] == '\0' )
            {
               send_to_char( "That is not an immskill.\n\r", ch );
               return;
            }
            if( !str_infix( argument, victim->pcdata->immskll ) )
            {
               send_to_char( "That skill has already been set.\n\r", ch );
               return;
            }
            strcat( buf, argument );
            strcat( buf, " " );
         }
      }

      if( argument[0] == '-' )
      {
         argument++;
         one_argument( argument, arg1 );
         if( !str_cmp( "all", arg1 ) )
         {
            free_string( victim->pcdata->immskll );
            victim->pcdata->immskll = str_dup( "" );
            send_to_char( "All immskills have been deleted.\n\r", ch );
            return;
         }
         else if( arg1 )
         {
            /*
             * Cool great imtlset <victim> - <skill> code...
             * Idea from Canth (phule@xs4all.nl)
             * Code by Vego (v942429@si.hhs.nl)
             * Still needs memory improvements.... (I think)
             */
            buf2 = str_dup( victim->pcdata->immskll );
            buf3 = buf2;
            if( ( skill = strstr( buf2, arg1 ) ) == NULL )
            {
               send_to_char( "That person doesn't have that immskill", ch );
               return;
            }
            else
            {
               while( buf2 != skill )
                  buf[i++] = *( buf2++ );
               while( !isspace( *( buf2++ ) ) );
               buf[i] = '\0';
               strcat( buf, buf2 );
            }
         }
         else
         {
            send_to_char( "That's not an immskill\n\r", ch );
            return;
         }
      }

      free_string( buf3 );
      skill = buf2 = buf3 = NULL;
      free_string( victim->pcdata->immskll );
      victim->pcdata->immskll = str_dup( buf );
   }

   sprintf( buf, "Immortal skills set for %s:\n\r", victim->name );
   send_to_char( buf, ch );
   buf1[0] = '\0';
   for( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
   {
      if( cmd_table[cmd].level < LEVEL_HERO || str_infix( cmd_table[cmd].name, victim->pcdata->immskll ) )
         continue;

      sprintf( buf, "%-10s", cmd_table[cmd].name );
      strcat( buf1, buf );
      if( ++col % 8 == 0 )
         strcat( buf1, "\n\r" );
   }

   if( col % 8 != 0 )
      strcat( buf1, "\n\r" );
   send_to_char( buf1, ch );

   return;

}
void do_gain_stat_reset( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *wear_object;
   CHAR_DATA *rch;


   rch = get_char( ch );

   if( !authorized( rch, "resetgain" ) )
      return;

   if( argument[0] == '\0' )
   {
      send_to_char( "Reset who's gain stats??\n\r", ch );
      return;
   }

   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPCs!\n\r", ch );
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

   send_to_char( "Done!\n\r", ch );
   send_to_char( "Your stats have been reset.\n\r", victim );

}

/* Expand the name of a character into a string that identifies THAT
   character within a room. E.g. the second 'guard' -> 2. guard */
const char *name_expand( CHAR_DATA * ch )
{
   int count = 1;
   CHAR_DATA *rch;
   char name[MAX_INPUT_LENGTH];  /*  HOPEFULLY no mob has a name longer than THAT */

   static char outbuf[MAX_INPUT_LENGTH];

   if( !IS_NPC( ch ) )
      return ch->name;

   one_argument( ch->name, name );  /* copy the first word into name */

   if( !name[0] ) /* weird mob .. no keywords */
   {
      strcpy( outbuf, "" );   /* Do not return NULL, just an empty buffer */
      return outbuf;
   }

   for( rch = ch->in_room->first_person; rch && ( rch != ch ); rch = rch->next_in_room )
      if( is_name( name, rch->name ) )
         count++;


   sprintf( outbuf, "%d.%s", count, name );
   return outbuf;
}

/*
 * For by Erwin S. Andreasen (4u2@aabc.dk)
 */
void do_for( CHAR_DATA * ch, char *argument )
{
   char range[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   bool fGods = FALSE, fMortals = FALSE, fMobs = FALSE, fEverywhere = FALSE, found;
   ROOM_INDEX_DATA *room, *old_room;
   CHAR_DATA *p, *p_next;
   int i;

   extern bool disable_timer_abort;

   if( !authorized( ch, "for" ) )
      return;

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
      send_to_char( "Are you trying to crash the MUD or something?\n\r", ch );
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
      send_to_char( "Cannot use FOR EVERYWHERE with the # thingie.\n\r", ch );
      disable_timer_abort = FALSE;
      return;
   }

   if( fMobs && strchr( argument, '#' ) )
   {
      send_to_char( "Cannot use FOR MOBS with the # thingie.\n\r", ch );
      disable_timer_abort = FALSE;
      return;
   }

   if( strchr( argument, '#' ) ) /* replace # ? */
   {
      for( p = first_char; p; p = p_next )
      {
         p_next = p->next; /* In case someone DOES try to AT MOBS SLAY # */
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


void do_otype( CHAR_DATA * ch, char *argument )
{
   extern int top_obj_index;
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
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
         sprintf( buf, "Values for object types:\n\r" );
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
   for( vnum = 0; nMatch < top_obj_index; vnum++ )
   {
      if( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
      {
         nMatch++;
         if( fAll || is_name( arg, tab_item_types[( pObjIndex->item_type ) - 1].text ) )
         {
            found = TRUE;
            sprintf( buf, "<%d> %s [%5d] %s\n\r", pObjIndex->level,
                     ( IS_SET( pObjIndex->extra_flags, ITEM_REMORT ) ?
                       "@@mRemort@@N" : "@@aMortal@@N" ), pObjIndex->vnum, pObjIndex->short_descr );
            safe_strcat( MAX_STRING_LENGTH, buf1, buf );
         }
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
      return;
   }

   sprintf( buf, "Otype report for %s", arg );
   send_rep_out( ch, buf1, mailme, buf );
   return;
}

void do_owear( CHAR_DATA * ch, char *argument )
{
   extern int top_obj_index;
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
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
      sprintf( buf, "Values for wear slots:\n\r" );
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
   for( vnum = 0; nMatch < top_obj_index; vnum++ )
   {
      if( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
      {
         nMatch++;
         if( fAll || !str_infix( arg, bit_table_lookup( tab_wear_flags, pObjIndex->wear_flags ) ) )
         {
            found = TRUE;
            sprintf( buf, "<%s> [%5d] [%3d] %s %s\n\r",
                     bit_table_lookup( tab_wear_flags, pObjIndex->wear_flags ),
                     pObjIndex->vnum,
                     pObjIndex->level,
                     ( IS_SET( pObjIndex->extra_flags, ITEM_REMORT ) ?
                       "@@mRemort@@N" : "@@aMortal@@N" ), pObjIndex->short_descr );
            safe_strcat( MAX_STRING_LENGTH, buf1, buf );
         }
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
      return;
   }

   sprintf( buf, "Owear report for %s", arg );
   send_rep_out( ch, buf1, mailme, buf );
   return;
}
void do_areasave( CHAR_DATA * ch, char *argument )
{
   /*
    * -S- : Show list of areas, vnum range and owners. 
    */

   AREA_DATA *pArea;

   for( pArea = first_area; pArea != NULL; pArea = pArea->next )
      area_modified( pArea );
   send_to_char( "Done.\n\r", ch );

   return;
}



void do_findreset( CHAR_DATA * ch, char *argument )
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
      send_to_char( "Not for NPCs.\n\r", ch );
      return;
   }
   if( is_name( "+w", argument ) )
      mworld = TRUE;
   if( is_name( argument, "mailme" ) )
      mailme = TRUE;

   argument = one_argument( argument, arg1 );
   if( arg1[0] == '\0' )
   {
      sprintf( outbuf, "%s", "Syntax for findreset:\n\r" );
      sprintf( catbuf, "%s",
               "findreset obj/mob <vnum> [+w]\n\r+w shows all resets in the world, default is current area only.\n\r" );
      safe_strcat( MSL, outbuf, catbuf );
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

   sprintf( outbuf, "Resets for %s %d:\n\r", arg1, vnum );
   if( mworld )
   {
      AREA_DATA *pArea;

      pMob = get_mob_index( vnum );
      if( pMob == NULL )
      {
         send_to_char( "Invalid mobile.\n\r", ch );
         return;
      }
      for( pArea = first_area; pArea; pArea = pArea->next )
      {
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
                     sprintf( catbuf, "Room: %d, limit of: %d\n\r", reset->arg3, reset->arg2 );
                     safe_strcat( MSL, outbuf, catbuf );
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
                                 safe_strcat( MSL, outbuf, "  sells " );
                              else
                                 safe_strcat( MSL, outbuf, "  with " );
                              sprintf( catbuf, "[%d] %s.\n\r", pObj->vnum, pObj->name );
                              safe_strcat( MSL, outbuf, catbuf );
                           }
                           else
                           {
                              sprintf( catbuf, "[%d] unknown object in give reset!\n\r", similar->arg1 );
                              safe_strcat( MSL, outbuf, catbuf );
                           }
                        }
                        else if( similar->command == 'E' )
                        {
                           pObj = get_obj_index( similar->arg1 );
                           if( pObj != NULL )
                              sprintf( catbuf, "  equiped with [%d] %s, on %s.\n\r", pObj->vnum,
                                       pObj->name, tab_wear_loc[( similar->arg3 )].text );
                           else
                              sprintf( catbuf, "[%d] unknown object equipped on %s.\n\r",
                                       similar->arg1, tab_wear_loc[similar->arg3].text );
                           safe_strcat( MSL, outbuf, catbuf );
                        }
                     }
                     break;
                  }  /* case M */
               }  /* switch */
            }  /* if ( fmob ) */
         }  /* for reset */
      }  /* for pArea */
      sprintf( mailsub, "Findresets for %s %d:\n\r", arg1, vnum );
      send_rep_out( ch, outbuf, mailme, mailsub );
      return;
   }  /* if ( mworld ) */

   send_to_char( "Currently not implemented.\n\r", ch );
   return;
}
