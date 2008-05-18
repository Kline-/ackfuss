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


#if defined(macintosh)
include < types.h >
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#ifndef DEC_LISTS_H
#include "lists.h"
#endif

#define BOARD_DIR "boards"
#define T2000 -1  /* Terminator for files... */



/* Local functions */
BOARD_DATA *load_board( OBJ_INDEX_DATA * pObj );
void save_board( BOARD_DATA * board, CHAR_DATA * ch );
void finished_editing( MESSAGE_DATA * msg, char **dest, CHAR_DATA * ch, bool saved );

/* Some locals used to manage the list of messages: */



/**************************************************************************
 *               MAG Modified outline                                     *
 *       Use a directory called boards under area directory.              *
 *       Store a file for each board called board.vnum makes things a lot *
 *       easier, and means files can be transfered easily between boards  *
 *       use a time based means to get rid of messages.                   *
 *                                 					  *
 *       Values:							  *
 *          Value0  :  Expiry time for a message in days.                 *
 *          Value1  :  Min level of player to read the board.             *
 *          Value2  :  Max level of player to write to the board.         *
 *          Value3  :  Board number (usually it's vnum for simplicity)    *
 *									  *
 *       Uses commands in write.c for string editing.                     *
 *       This file does reading files, writing files and managing boards  *
 *									  *
 **************************************************************************/

/************************************************************************** 
 *                        Outline of BOARD.C                              *
 *                        ^^^^^^^^^^^^^^^^^^                              *
 * This code was written for use in ACK! MUD.  It should be easy to       *
 * include it into a Diku Merc 2.0+ Mud.                                  *
 *                                                                        *
 * The following functions are needed:                                    *
 * 1) Show the contents of a board to a player.                           *
 * 2) Show a message to a player                                          *
 * 3) Add to a message & finish writing a message.                        *
 * 4) Start writing a message                                             *
 * 5) Remove a message                                                    *
 * 6) Save the messages                                                   *
 * 7) Load the messages (only used at start-up)                           *
 *                                                                        *
 * Also, the code for the commands write and read are in this file.       *
 * WRITE checks for a board, and calls either 4) or 3) above.             *
 * READ calls 2) above.  The LOOK function was ammended to allow players  *
 * to type 'look board' or 'look at board' which calls 1) above.          *
 *                                                                        *
 * MESSAGE DATA holds the vnum of the board where the message was written *
 * and the message info.  There is no seperate save file or structure for *
 * each board.  Instead, they are all stored together.  I have used       *
 * OBJ_TYPE 23 for boards, with the following values used:                *
 * value 0: max number of messages allowed                                *
 * value 1: min level of player to read the board                         *
 * value 2: min level of player to write on the board                     *
 * value 3: the vnum of the board...NOT the vnum of the room...           *
 **************************************************************************/

/**************************************************************************
 * Ick ick ick!  Remove all the dammed builder functions, and use the     *
 * general merc memory functions.  -- Altrag                              *
 **************************************************************************/


void do_show_contents( CHAR_DATA * ch, OBJ_DATA * obj )
{
   /*
    * Show the list of messages that are present on the board that ch is
    * * looking at, indicated by board_vnum...
    */

   MESSAGE_DATA *msg;
   BOARD_DATA *board;
   OBJ_INDEX_DATA *pObj;
   char buf[MAX_INPUT_LENGTH];
   int cnt = 0;
   int board_num;

   pObj = obj->pIndexData;
   board_num = pObj->value[3];

   /*
    * First find the board, and if not there, create one. 
    */
   for( board = first_board; board != NULL; board = board->next )
   {
      if( board->vnum == board_num )
         break;
   }

   if( board == NULL )
      board = load_board( pObj );

   /*
    * check here to see if player allowed to read board 
    */

   if( board->min_read_lev > get_trust( ch ) )
   {
      send_to_char( "You are not allowed to look at this board.\n\r", ch );
      return;
   }

   if( ( board->clan != 0 ) && !IS_NPC( ch ) && ch->pcdata->clan != ( board->clan - 1 ) )
   {
      send_to_char( "You are not of the right clan to read this board.\n\r", ch );
      return;
   }


   send_to_char( "This is a notice board.\n\r", ch );
   send_to_char( "Type READ <num> to read message <num>, and WRITE <title> to post.\n\r", ch );
   send_to_char( "Type WRITE to: <player> for a private message.\n\r", ch );
   send_to_char( "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r", ch );


   for( msg = board->first_message; msg != NULL; msg = msg->next )
   {
      cnt++;
      sprintf( buf, "[%3d] %12s : %s", cnt, msg->author, msg->title );
      send_to_char( buf, ch );

   }

   if( cnt == 0 ) /* then there were no messages here */
   {
      send_to_char( "         There are no messages right now!\n\r", ch );
   }

   send_to_char( "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r", ch );

   return;
}

BOARD_DATA *load_board( OBJ_INDEX_DATA * pObj )
{
   FILE *board_file;
   char buf[255];
   char *word;
   char letter;
   time_t message_time;
   time_t expiry_time;
   BOARD_DATA *board;
   MESSAGE_DATA *message;

   GET_FREE( board, board_free );
   LINK( board, first_board, last_board, next, prev );

   board->expiry_time = pObj->value[0];
   board->min_read_lev = pObj->value[1];
   board->min_write_lev = pObj->value[2];
   board->vnum = pObj->value[3];
   board->clan = 0;
   board->first_message = NULL;
   board->last_message = NULL;

   fclose( fpReserve );

   sprintf( buf, "%s/board.%i", BOARD_DIR, board->vnum );

   if( ( board_file = fopen( buf, "r" ) ) != NULL )
   {
      /*
       * Read in Optional board parameters 
       */
      for( ;; )
      {
         if( feof( board_file ) )
            break;

         word = fread_word( board_file );
         if( !str_cmp( word, "ExpiryTime" ) )
         {
            board->expiry_time = fread_number( board_file );
            fread_to_eol( board_file );
         }
         if( !str_cmp( word, "MinReadLev" ) )
         {
            board->min_read_lev = fread_number( board_file );
            fread_to_eol( board_file );
         }
         if( !str_cmp( word, "MinWriteLev" ) )
         {
            board->min_write_lev = fread_number( board_file );
            fread_to_eol( board_file );
         }
         if( !str_cmp( word, "Clan" ) )
         {
            board->clan = fread_number( board_file );
            fread_to_eol( board_file );
         }
         if( !str_cmp( word, "Messages" ) )
         {
            fread_to_eol( board_file );
            break;
         }
      }

      if( board->expiry_time > 0 )
         expiry_time = time( NULL ) - ( board->expiry_time ) * 3600 * 24;
      else
         expiry_time = 0;

      /*
       * Now read in messages 
       */
      for( ;; )
      {
         if( feof( board_file ) )
            break;

         letter = fread_letter( board_file );
         if( letter == 'S' )
            break;

         if( letter != 'M' )
         {
            bug( "Letter in message file not M", 0 );
            break;
         }

         /*
          * check time 
          */
         message_time = ( time_t ) fread_number( board_file );
         if( feof( board_file ) )
            break;

         if( message_time < expiry_time )
         {
            char *dumpme;

            dumpme = fread_string( board_file );   /* author  */
            free_string( dumpme );
            dumpme = fread_string( board_file );   /* title   */
            free_string( dumpme );
            dumpme = fread_string( board_file );   /* message */
            free_string( dumpme );
         }
         else
         {
            GET_FREE( message, message_free );
            message->datetime = message_time;
            message->author = fread_string( board_file );
            message->title = fread_string( board_file );
            message->message = fread_string( board_file );
            LINK( message, board->first_message, board->last_message, next, prev );
            message->board = board;
         }
      }

      /*
       * Now close file 
       */
      fclose( board_file );
      fpReserve = fopen( NULL_FILE, "r" );
   }

   return board;
}

void save_board( BOARD_DATA * board, CHAR_DATA * ch )
{
   char buf[MAX_STRING_LENGTH];
   FILE *board_file;
   MESSAGE_DATA *message;

   fclose( fpReserve );


   sprintf( buf, "%s/board.%i", BOARD_DIR, board->vnum );
   if( ( board_file = fopen( buf, "w" ) ) == NULL )
   {
      send_to_char( "Cannot save board, please contact an immortal.\n\r", ch );
      bug( "Could not save file board.%i.", board->vnum );
      fpReserve = fopen( NULL_FILE, "r" );
      return;
   }

   fprintf( board_file, "ExpiryTime  %i\n", board->expiry_time );
   fprintf( board_file, "MinReadLev  %i\n", board->min_read_lev );
   fprintf( board_file, "MaxWriteLev %i\n", board->min_write_lev );
   fprintf( board_file, "Clan        %i\n", board->clan );

   /*
    * Now print messages 
    */
   fprintf( board_file, "Messages\n" );

   for( message = board->first_message; message; message = message->next )
   {
      fprintf( board_file, "M%i\n", ( int )( message->datetime ) );

      strcpy( buf, message->author );  /* Must do copy, not allowed to change string directly */
      smash_tilde( buf );
      fprintf( board_file, "%s~\n", buf );

      strcpy( buf, message->title );
      smash_tilde( buf );
      fprintf( board_file, "%s~\n", buf );

      strcpy( buf, message->message );
      smash_tilde( buf );
      fprintf( board_file, "%s~\n", buf );

   }

   fprintf( board_file, "S\n" );

   fclose( board_file );
   fpReserve = fopen( NULL_FILE, "r" );

   return;
}

void do_delete( CHAR_DATA * ch, char *argument )
{
   OBJ_DATA *object;
   BOARD_DATA *board;
   MESSAGE_DATA *msg;
   OBJ_INDEX_DATA *pObj;
   int vnum;
   int mess_num;
   int cnt = 0;

   if( IS_NPC( ch ) )
   {
      send_to_char( "NPCs may *not* delete messages.  So there.\n\r", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "You need to specify a message number to delete!\n\r", ch );
      return;
   }

   for( object = ch->in_room->first_content; object != NULL; object = object->next_in_room )
   {
      if( object->item_type == ITEM_BOARD )
         break;
   }

   if( object == NULL )
   {
      send_to_char( "Delete on what?\n\r.", ch );
      return;
   }

   pObj = object->pIndexData;
   vnum = pObj->value[3];

   /*
    * First find the board, and if not there, create one. 
    */
   for( board = first_board; board != NULL; board = board->next )
   {
      if( board->vnum == vnum )
         break;
   }

   if( board == NULL )
      board = load_board( pObj );

   /*
    * check here to see if player allowed to write to board 
    */

   if( board->min_write_lev > get_trust( ch ) )
   {
      send_to_char( "You are not allowed to delete on this board.\n\r", ch );
      return;
   }

   if( ( board->clan != 0 ) && ch->pcdata->clan != ( board->clan - 1 ) )
   {
      send_to_char( "You are not of the right clan to delete on this board.\n\r", ch );
      return;
   }


   cnt = 0;
   mess_num = is_number( argument ) ? atoi( argument ) : 0;

   for( msg = board->first_message; msg != NULL; msg = msg->next )
      if( ++cnt == mess_num ) /* then this the message!!! */
         break;

   if( msg == NULL )
   {
      send_to_char( "No such message!\n\r", ch );
      return;
   }

   /*
    * See if person is writer or is recipient 
    */
   if( str_cmp( ch->name, msg->author ) && !is_name( ch->name, msg->title )
       && get_trust( ch ) < MAX_LEVEL && str_cmp( ch->name, clan_table[board->clan].leader ) )
   {
      send_to_char( "Not your message.\n\r", ch );
      return;
   }

   /*
    * Now delete message 
    */

   UNLINK( msg, board->first_message, board->last_message, next, prev );

   PUT_FREE( msg, message_free );

   save_board( board, ch );

   return;
}

void do_show_message( CHAR_DATA * ch, int mess_num, OBJ_DATA * obj )
{
   /*
    * Show message <mess_num> to character. 
    * * check that message vnum == board vnum
    */

   BOARD_DATA *board;
   OBJ_INDEX_DATA *pObj;
   int vnum;
   MESSAGE_DATA *msg;
   int cnt = 0;
   bool mfound = FALSE;
   char buf[MAX_STRING_LENGTH];
   char to_check[MAX_INPUT_LENGTH];
   char *to_person;
   char private_name[MAX_INPUT_LENGTH];

   pObj = obj->pIndexData;
   vnum = pObj->value[3];

   /*
    * First find the board, and if not there, create one. 
    */
   for( board = first_board; board != NULL; board = board->next )
   {
      if( board->vnum == vnum )
         break;
   }

   if( board == NULL )
      board = load_board( pObj );

   /*
    * check here to see if player allowed to read board 
    */

   if( board->min_read_lev > get_trust( ch ) )
   {
      send_to_char( "You are not allowed to look at this board.\n\r", ch );
      return;
   }

   if( ( board->clan != 0 ) && !IS_NPC( ch ) && ch->pcdata->clan != ( board->clan - 1 ) )
   {
      send_to_char( "You are not of the right clan to read this board.\n\r", ch );
      return;
   }

   for( msg = board->first_message; msg != NULL; msg = msg->next )
   {
      if( ++cnt == mess_num ) /* then this the message!!! */
      {
         mfound = TRUE;

         to_person = one_argument( msg->title, to_check );
         to_person = one_argument( to_person, private_name );
         if( !str_cmp( to_check, "to:" ) && str_prefix( private_name, ch->name ) && str_cmp( msg->author, ch->name ) )
         {
            send_to_char( "This is a private message.\n\r", ch );
            break;
         }
         sprintf( buf, "** [%d] %12s : %s ** \n\r\n\r%s\n\r", cnt, msg->author, msg->title, msg->message );
         send_to_char( buf, ch );
         break;
      }
   }

   if( !mfound )
   {
      send_to_char( "No such message!\n\r", ch );
   }

   return;
}

void do_write( CHAR_DATA * ch, char *argument )
{
   OBJ_DATA *object;
   BOARD_DATA *board;
   MESSAGE_DATA *msg;
   OBJ_INDEX_DATA *pObj;
   int vnum;
   extern char str_empty[1];
   char buf[MAX_STRING_LENGTH];

   if( IS_NPC( ch ) )
   {
      send_to_char( "NPCs may *not* write messages.  So there.\n\r", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "You need to specify a header for your message!\n\r", ch );
      return;
   }

   for( object = ch->in_room->first_content; object != NULL; object = object->next_in_room )
   {
      if( object->item_type == ITEM_BOARD )
         break;
   }

   if( object == NULL )
   {
      send_to_char( "Write on what?\n\r.", ch );
      return;
   }

   pObj = object->pIndexData;
   vnum = pObj->value[3];

   /*
    * First find the board, and if not there, create one. 
    */
   for( board = first_board; board != NULL; board = board->next )
   {
      if( board->vnum == vnum )
         break;
   }

   if( board == NULL )
      board = load_board( pObj );

   /*
    * check here to see if player allowed to write to board 
    */

   if( board->min_write_lev > get_trust( ch ) )
   {
      send_to_char( "You are not allowed to write on this board.\n\r", ch );
      return;
   }

   if( ( board->clan != 0 ) && ch->pcdata->clan != ( board->clan - 1 ) )
   {
      send_to_char( "You are not of the right clan to write on this board.\n\r", ch );
      return;
   }

   GET_FREE( msg, message_free );   /* Dont put message in list till we  */
   msg->datetime = time( NULL ); /* we are sure we can edit.          */
   sprintf( buf, "%s @@a%s@@N", argument, ( char * )ctime( &current_time ) );
   if( msg->title != NULL )
      free_string( msg->title );
   msg->title = str_dup( buf );
   if( msg->author != NULL )
      free_string( msg->author );
   msg->author = str_dup( ch->name );
   msg->message = NULL;
   msg->board = board;

   /*
    * Now actually run the edit prog. 
    */
   write_start( &msg->message, finished_editing, msg, ch );

   if( msg->message != &str_empty[0] )
   {
      send_to_char( "Editing message. Type .help for help.\n\r", ch );
      LINK( msg, board->first_message, board->last_message, next, prev );
   }
   else
   {
      send_to_char( "Could not add message.\n\r", ch );
      PUT_FREE( msg, message_free );
   }
   return;
}


/* Deals with taking message out of list if user aborts... */

void finished_editing( MESSAGE_DATA * msg, char **dest, CHAR_DATA * ch, bool saved )
{
#ifdef CHECK_VALID_BOARD
   MESSAGE_DATA *SrchMsg;
#endif

   if( !saved )
   {
#ifdef CHECK_VALID_BOARD
      for( SrchMsg = msg->board->messages; SrchMsg != NULL; SrchMsg = SrchMsg->next )
         if( SrchMsg == msg )
            break;

      if( SrchMsg == NULL )
      {
         /*
          * Could not find this message in board list, just lose memory. 
          */
         return;
      }
#endif

      UNLINK( msg, msg->board->first_message, msg->board->last_message, next, prev );
      PUT_FREE( msg, message_free );
   }
   else
   {
      save_board( msg->board, ch );
   }
   return;
}

void do_read( CHAR_DATA * ch, char *argument )
{
   OBJ_DATA *obj;


   if( ( argument[0] == '\0' ) || !is_number( argument ) )
   {
      send_to_char( "Read what??\n\r", ch );
      return;
   }

   for( obj = ch->in_room->first_content; obj != NULL; obj = obj->next_in_room )
   {
      if( obj->item_type == ITEM_BOARD )
         break;
   }

   if( obj == NULL )
   {
      send_to_char( "Read What??\n\r", ch );
      return;
   }

   /*
    * Hopefully, by now there should be a board in the room, and the
    * * player should have supplied some sort of argument....
    */

   do_show_message( ch, atoi( argument ), obj );
   return;
}





void do_edit_message( CHAR_DATA * ch, int mess_num, OBJ_DATA * obj )
{
   /*
    * Show message <mess_num> to character. 
    * * check that message vnum == board vnum
    */

   BOARD_DATA *board;
   OBJ_INDEX_DATA *pObj;
   int vnum;
   MESSAGE_DATA *msg;
   int cnt = 0;
   bool mfound = FALSE;

   pObj = obj->pIndexData;
   vnum = pObj->value[3];

   /*
    * First find the board, and if not there, create one. 
    */
   for( board = first_board; board != NULL; board = board->next )
   {
      if( board->vnum == vnum )
         break;
   }

   if( board == NULL )
      board = load_board( pObj );

   /*
    * check here to see if player allowed to read board 
    */

   if( board->min_read_lev > get_trust( ch ) )
   {
      send_to_char( "You are not allowed to even look at this board!\n\r", ch );
      return;
   }

   if( ( board->clan != 0 ) && !IS_NPC( ch ) && ch->pcdata->clan != ( board->clan - 1 ) )
   {
      send_to_char( "You are not of the right clan to even read this board!\n\r", ch );
      return;
   }

   for( msg = board->first_message; msg != NULL; msg = msg->next )
   {
      if( ++cnt == mess_num ) /* then this the message!!! */
      {
         mfound = TRUE;

         if( str_cmp( msg->author, ch->name ) )
         {
            send_to_char( "Not your message to edit!\n\r", ch );
            return;
         }
         else
         {
            build_strdup( &msg->message, "$edit", TRUE, ch );
         }

      }
   }
   if( !mfound )
      send_to_char( "No such message!\n\r", ch );

   return;
}

void do_edit( CHAR_DATA * ch, char *argument )
{
   OBJ_DATA *obj;


   if( ( argument[0] == '\0' ) || !is_number( argument ) )
   {
      send_to_char( "Read what??\n\r", ch );
      return;
   }

   for( obj = ch->in_room->first_content; obj != NULL; obj = obj->next_in_room )
   {
      if( obj->item_type == ITEM_BOARD )
         break;
   }

   if( obj == NULL )
   {
      send_to_char( "Read What??\n\r", ch );
      return;
   }

   /*
    * Hopefully, by now there should be a board in the room, and the
    * * player should have supplied some sort of argument....
    */

   do_edit_message( ch, atoi( argument ), obj );
   return;
}
