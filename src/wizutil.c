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

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_SAVE_H
#include "h/save.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

/*
===========================================================================
This snippet was written by Erwin S. Andreasen, 4u2@aabc.dk. You may use
this code freely, as long as you retain my name in all of the files. You
also have to mail me telling that you are using it. I am giving this,
hopefully useful, piece of source code to you for free, and all I require
from you is some feedback.

Please mail me if you find any bugs or have any new ideas or just comments.

All my snippets are publically available at:

http://pip.dknet.dk/~pip1773/

If you do not have WWW access, try ftp'ing to pip.dknet.dk and examine
the /pub/pip1773 directory.
===========================================================================

  Various administrative utility commands.
  Version: 3 - Last update: January 1996.

  To use these 2 commands you will have to add a filename field to AREA_DATA.
  This value can be found easily in load_area while booting - the filename
  of the current area boot_db is reading from is in the strArea global.

  Since version 2 following was added:

  A rename command which renames a player. Search for do_rename to see
  more info on it.

  Fixes since last release: None.
*/

/* To have VLIST show more than vnum 0 - 9900, change the number below: */

#define MAX_SHOW_VNUM   326   /* show only 1 - 100*100 */

#define NUL '\0'

/* opposite directions */
const short opposite_dir[6] = { DIR_SOUTH, DIR_WEST, DIR_NORTH, DIR_EAST, DIR_DOWN, DIR_UP };


/* get the 'short' name of an area (e.g. MIDGAARD, MIRROR etc. */
/* assumes that the filename saved in the AREA_DATA struct is something like midgaard.are */
char *area_name( AREA_DATA * pArea )
{
   static char buffer[64]; /* short filename */
   char *period;

   assert( pArea != NULL );

   strncpy( buffer, pArea->filename, 64 );   /* copy the filename */
   period = strchr( buffer, '.' );  /* find the period (midgaard.are) */
   if( period )   /* if there was one */
      *period = '\0';   /* terminate the string there (midgaard) */

   return buffer;
}

/* depending on status print > or < or <> between the 2 rooms */
void room_pair( ROOM_INDEX_DATA * left, ROOM_INDEX_DATA * right, exit_status ex, char *buffer )
{
   char *sExit;

   switch ( ex )
   {
      default:
         sExit = "??";
         break;   /* invalid usage */
      case exit_from:
         sExit = "< ";
         break;
      case exit_to:
         sExit = " >";
         break;
      case exit_both:
         sExit = "<>";
         break;
   }

   snprintf( buffer, sizeof(buffer), "%5d %-26.26s %s%5d %-26.26s(%-8.8s)\r\n",
            left->vnum, left->name, sExit, right->vnum, right->name, area_name( right->area ) );
}

/* for every exit in 'room' which leads to or from pArea but NOT both, print it */
void checkexits( ROOM_INDEX_DATA * room, AREA_DATA * pArea, char *buffer )
{
   char buf[MAX_STRING_LENGTH];
   int i;
   EXIT_DATA *exit;
   ROOM_INDEX_DATA *to_room;

   strcpy( buffer, "" );
   for( i = 0; i < 6; i++ )
   {
      exit = room->exit[i];
      if( !exit )
         continue;
      else
         to_room = exit->to_room;

      if( to_room )  /* there is something on the other side */
      {
         if( ( room->area == pArea ) && ( to_room->area != pArea ) )
         {  /* an exit from our area to another area */
            /*
             * check first if it is a two-way exit
             */

            if( to_room->exit[opposite_dir[i]] && to_room->exit[opposite_dir[i]]->to_room == room )
               room_pair( room, to_room, exit_both, buf );  /* <> */
            else
               room_pair( room, to_room, exit_to, buf ); /* > */

            strncat( buffer, buf, MSL );
         }
         else if( ( room->area != pArea ) && ( exit->to_room->area == pArea ) )
         {  /* an exit from another area to our area */

            if( !( to_room->exit[opposite_dir[i]] && to_room->exit[opposite_dir[i]]->to_room == room ) )
               /*
                * two-way exits are handled in the other if
                */
            {
               room_pair( to_room, room, exit_from, buf );
               strncat( buffer, buf, MSL );
            }

         }  /* if room->area */
      }

   }  /* for */

}

/* for now, no arguments, just list the current area */
DO_FUN(do_exlist)
{
   AREA_DATA *pArea;
   ROOM_INDEX_DATA *room;
   int i;
   char buffer[MAX_STRING_LENGTH];

   pArea = ch->in_room->area; /* this is the area we want info on */
   for( i = 0; i < MAX_KEY_HASH; i++ ) /* room index hash table */
      for( room = room_index_hash[i]; room != NULL; room = room->next )
         /*
          * run through all the rooms on the MUD
          */

      {
         checkexits( room, pArea, buffer );
         send_to_char( buffer, ch );
      }
}

/* show a list of all used VNUMS */

#define COLUMNS 		5  /* number of columns */
#define MAX_ROW 		((MAX_SHOW_VNUM / COLUMNS)+1) /* rows */

DO_FUN(do_vlist)
{
   int i, j, vnum;
   ROOM_INDEX_DATA *room;
   char buffer[MAX_ROW * 100];   /* should be plenty */
   char buf2[100];

   for( i = 0; i < MAX_ROW; i++ )
   {
      strcpy( buffer, "" );   /* clear the buffer for this row */

      for( j = 0; j < COLUMNS; j++ )   /* for each column */
      {
         vnum = ( ( j * MAX_ROW ) + i );  /* find a vnum whih should be there */
         if( vnum < MAX_SHOW_VNUM )
         {
            room = get_room_index( vnum * 100 + 1 );  /* each zone has to have a XXX01 room */
            snprintf( buf2, 100, "%3d %-8.8s  ", vnum, room ? area_name( room->area ) : "-" );
            /*
             * something there or unused ?
             */
            strncat( buffer, buf2, (MAX_ROW * 100)-1 );
         }
      }  /* for columns */

      send_to_char( buffer, ch );
      send_to_char( "\r\n", ch );
   }  /* for rows */
}

/*
 * do_rename renames a player to another name.
 * PCs only. Previous file is deleted, if it exists.
 * Char is then saved to new file.
 * New name is checked against std. checks, existing offline players and
 * online players.
 * .gz files are checked for too, just in case.
 */

DO_FUN(do_rename)
{
   char old_name[MAX_INPUT_LENGTH], new_name[MAX_INPUT_LENGTH], strsave[MAX_INPUT_LENGTH];

   CHAR_DATA *victim;
   FILE *file;

   argument = one_argument( argument, old_name );  /* find new/old name */
   one_argument( argument, new_name );

   /*
    * Trivial checks
    */
   if( !old_name[0] )
   {
      send_to_char( "Rename who?\r\n", ch );
      return;
   }

   victim = get_char_world( ch, old_name );

   if( !victim )
   {
      send_to_char( "There is no such a person online.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "You cannot use Rename on NPCs.\r\n", ch );
      return;
   }

   /*
    * allow rename self new_name,but otherwise only lower level
    */
   if( ( victim != ch ) && ( get_trust( victim ) >= get_trust( ch ) ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   if( !victim->desc || ( victim->desc->connected != CON_PLAYING ) )
   {
      send_to_char( "This player has lost his link or is inside a pager or the like.\r\n", ch );
      return;
   }

   if( !new_name[0] )
   {
      send_to_char( "Rename to what new name?\r\n", ch );
      return;
   }

   /*
    * Insert check for clan here!!
    */
   /*
    *
    * if (victim->clan)
    * {
    * send_to_char ("This player is member of a clan, remove him from there first.\r\n",ch);
    * return;
    * }
    */

   if( !check_parse_name( new_name ) )
   {
      send_to_char( "The new name is illegal.\r\n", ch );
      return;
   }

   /*
    * First, check if there is a player named that off-line
    */
#if !defined(machintosh) && !defined(MSDOS)
   snprintf( strsave, MIL, "%s%s%s%s", PLAYER_DIR, initial( new_name ), "/", capitalize( new_name ) );
#else
   snprintf( strsave, MIL, "%s%s", PLAYER_DIR, capitalize( new_name ) );
#endif

   file = file_open( strsave, "r" ); /* attempt to to open pfile */
   if( file )
   {
      send_to_char( "A player with that name already exists!\r\n", ch );
      file_close( file );
      return;
   }
   file_close( file );

   /*
    * Check .gz file !
    */
#if !defined(machintosh) && !defined(MSDOS)
   snprintf( strsave, MIL, "%s%s%s%s.gz", PLAYER_DIR, initial( new_name ), "/", capitalize( new_name ) );
#else
   snprintf( strsave, MIL, "%s%s.gz", PLAYER_DIR, capitalize( new_name ) );
#endif

   file = file_open( strsave, "r" ); /* attempt to to open pfile */
   if( file )
   {
      send_to_char( "A player with that name already exists in a compressed file!\r\n", ch );
      file_close( file );
      return;
   }
   file_close( file );

   if( get_char_world( ch, new_name ) )   /* check for playing level-1 non-saved */
   {
      send_to_char( "A player with the name you specified already exists!\r\n", ch );
      return;
   }

   /*
    * Save the filename of the old name
    */

#if !defined(machintosh) && !defined(MSDOS)
   snprintf( strsave, MIL, "%s%s%s%s", PLAYER_DIR, initial( victim->name.c_str() ), "/", capitalize( victim->name.c_str() ) );
#else
   snprintf( strsave, MIL, "%s%s", PLAYER_DIR, capitalize( victim->name.c_str() ) );
#endif


   /*
    * Rename the character and save him to a new file
    */
   /*
    * NOTE: Players who are level 1 do NOT get saved under a new name
    */

   victim->name = capitalize( new_name );

   save_char_obj( victim );

   /*
    * unlink the old file
    */
   unlink( strsave );   /* unlink does return a value.. but we do not care */

   /*
    * That's it!
    */

   send_to_char( "Character renamed.\r\n", ch );

   victim->position = POS_STANDING; /* I am laaazy */
   act( "$n has renamed you to $N!", ch, NULL, victim, TO_VICT );

}  /* do_rename */
