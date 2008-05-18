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


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

/* This file deals with multi-line editing, and writing. */

typedef void RET_FUN( void *, char **, CHAR_DATA *, bool );


struct buf_data_struct
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   BUF_DATA_STRUCT *next;
   BUF_DATA_STRUCT *prev;
   CHAR_DATA *ch;
   char **dest;
   char *buf;
   int pos;
   RET_FUN *returnfunc;
   void *returnparm;
   int old_char_pos;
};



extern char str_empty[1];

/* A str function in build.c, used to duplicate strings into perm. mem. */
char *build_simpstrdup( char * );


void write_start( char **dest, void *retfunc, void *retparm, CHAR_DATA * ch )
{
   BUF_DATA_STRUCT *buf_data;
   char *buf;

   /*
    * Check that *dest != &str_empty[0]  when calling this func. 
    */
   /*
    * If it is, it's because we've run out of memory. 
    */


   buf = getmem( MAX_STRING_LENGTH );
   if( buf == NULL )
   {
      bug( "Not enough memory for string editing.", 0 );
      *dest = &str_empty[0];
      send_to_char( "WARNING: No memory left. Things will start to go wrong.\n\r", ch );
      return;
   }

   /*
    * Alloc mem. for a new buffer. 
    */
   GET_FREE( buf_data, buf_free );
   LINK( buf_data, first_buf, last_buf, next, prev );

   buf_data->ch = ch;
   buf_data->dest = dest;
   buf_data->buf = buf;
   buf_data->pos = 0;
   buf_data->returnfunc = retfunc;
   buf_data->returnparm = retparm;


   *buf = '\0';

   buf_data->old_char_pos = ch->position;
   ch->position = POS_WRITING;

   *dest = buf;

   return;
}


void write_interpret args( ( CHAR_DATA * ch, char *argument ) )
{

   BUF_DATA_STRUCT *buf_data;
   char *buf;
   int curlen;


   for( buf_data = first_buf; buf_data != NULL; buf_data = buf_data->next )
   {
      if( buf_data->ch == ch )
         break;
   }

   if( buf_data == NULL )
   {
      bugf( "Call to write_interpret when not writing (char=%s)\n\r", ch->name );
      ch->position = POS_STANDING;
      return;
   }

   buf = buf_data->buf;


   /*
    * Check to see if text was a command or simply addition 
    */
   if( argument[0] != '.' )
   {
      curlen = strlen( buf );
      if( curlen > MAX_STRING_LENGTH - 240 )
      {
         send_to_char( "String to long, cannot add new line.\n\r", ch );
         return;
      }
      for( buf = buf + curlen; *argument != '\0'; )
         *( buf++ ) = *( argument++ );
      *( buf++ ) = '\n';
      *( buf++ ) = '\r';
      *buf = '\0';
      return;
   }

   /*
    * We have a command. 
    */
   /*
    * Commands are .help .save .preview .- .clear .lines 
    */
   argument++;
   if( argument[0] == '\0' || UPPER( argument[0] == 'S' || UPPER( argument[0] ) == 'Q' ) )
   {
      bool save;
      char **dest;
      CHAR_DATA *ch;

      if( UPPER( argument[0] ) == 'Q' )
         save = 0;
      else
         save = 1;

      dest = buf_data->dest;
      ch = buf_data->ch;

      /*
       * Save routine. 
       */

      if( save )
      {
         /*
          * Check that dest still points to buf (to check for corruption) 
          */
         if( *dest != buf )
         {
            bug( "write_interpret: Original destination has been overwritten.", 0 );
            send_to_char( "Cannot save, string pointer been modified.\n\r", ch );
         }
         else
         {
            *dest = str_dup( buf );
            if( ( buf_data->returnfunc ) != NULL )
               ( *buf_data->returnfunc ) ( buf_data->returnparm, dest, ch, TRUE );
         }
      }
      else
      {
         *dest = &str_empty[0];
         if( ( buf_data->returnfunc ) != NULL )
            ( *buf_data->returnfunc ) ( buf_data->returnparm, dest, ch, FALSE );
      }

      /*
       * Re-use memory.
       */
      dispose( buf_data->buf, MAX_STRING_LENGTH );

      UNLINK( buf_data, first_buf, last_buf, next, prev );

      /*
       * Re-set char 
       */
      ch->position = buf_data->old_char_pos;

      PUT_FREE( buf_data, buf_free );

      return;
   }


   if( UPPER( argument[0] ) == 'H' )
   {
      /*
       * Help 
       */
      CHAR_DATA *ch;
      ch = buf_data->ch;

      send_to_char( "Normal type will be appended to the string, line by line.\n\r", ch );
      send_to_char( ".help     or .h :  displays this help.\n\r", ch );
      send_to_char( ".save     or .  :  saves and exits the editor.\n\r", ch );
      send_to_char( ".preview  or .p :  display a preview of the text.\n\r", ch );
      send_to_char( ".-[num]   or .- :  deletes [num] lines from the end, or just one line\n\r", ch );
      send_to_char( ".clear          :  deletes whole text.\n\r", ch );
      send_to_char( ".quit     or .q :  quits without saving.\n\r", ch );
      send_to_char( ".format   or .f :  formats text for 80 chars.\n\r", ch );
      send_to_char( ".replace  or .r :  replaces word with string.\n\r", ch );
      send_to_char( "	       (usage) :  .r <word> <string>.  If no string, arg1 deleted.\n\r", ch );
      return;
   }

   if( UPPER( argument[0] ) == 'R' )
   {
      /*
       * Mag:  I bet you take one look at this, and change it :) -S- 
       */

      char arg1[MAX_STRING_LENGTH];
      char arg2[MAX_STRING_LENGTH];
      char word[MAX_STRING_LENGTH];
      char new_buf[MAX_STRING_LENGTH];
      char pBuf;
      int pos;
      int npos;
      int wpos;
      int buf_length;
      int foo;
      int i;
      char *src;

      argument = one_argument( argument + 1, arg1 );  /* Skip the R */
      strcpy( arg2, argument );

      if( arg1[0] == '\0' )
      {
         send_to_char( "No arg1 supplied for replace command.\n\r", ch );
         return;
      }

      new_buf[0] = '\0';
      buf_length = strlen( buf );
      pos = 0;
      npos = 0;
      wpos = 0;
      word[0] = '\0';

      for( ;; )
      {
         pBuf = buf[pos];

         if( pBuf == '\0' || pos > buf_length )
            break;

         if( pBuf == ' ' )
         {
            new_buf[npos] = ' ';
            pos++;
            npos++;
            continue;
         }

         if( !isgraph( pBuf ) )
         {
            new_buf[npos] = pBuf;
            pos++;
            npos++;
            continue;
         }

         wpos = 0;
         for( ;; )
         {
            if( !isgraph( pBuf ) )
               break;

            word[wpos] = pBuf;
            pos++;
            wpos++;
            pBuf = buf[pos];
         }
         word[wpos] = '\0';

         if( !str_cmp( word, arg1 ) )
         {
            if( arg2[0] != '\0' )
               for( foo = 0; arg2[foo] != '\0'; foo++ )
               {
                  new_buf[npos] = arg2[foo];
                  npos++;
               }
            else
            {
               /*
                * Do nothing (much). 
                */
               if( npos > 0 )
                  npos--;
               send_to_char( "Arg1 deleted.\n\r", ch );
            }
         }
         else
         {
            for( foo = 0; word[foo] != '\0'; foo++ )
            {
               new_buf[npos] = word[foo];
               npos++;
            }
         }
      }

      /*
       * -gulp-  Copy new_buf into message structure... 
       */

      src = buf;
      for( i = 0; i < npos; i++ )
         *( src++ ) = new_buf[i];
      *( src ) = '\0';


      return;
   }



   if( UPPER( argument[0] ) == 'P' )
   {
      send_to_char( buf, ch );
      return;
   }

   if( argument[0] == '-' )
   {
      int num;
      int a;

      argument++;
      if( argument[0] == '\0' )
         num = 2;
      else
         num = atoi( argument ) + 1;

      if( num <= 0 )
         return;

      for( a = strlen( buf ); a >= 0; a-- )
      {
         if( buf[a] == '\n' )
         {
            num--;
            if( num == 0 )
               break;
         }
      }

      if( a == 0 )
      {
         send_to_char( "Tried to delete too many lines.\n\r", buf_data->ch );
         return;
      }

      a++;
      if( buf[a] == '\r' )
         a++;
      send_to_char( "Deleted:\n\r", buf_data->ch );
      send_to_char( buf + a, buf_data->ch );

      buf[a] = '\0';
      return;
   }

   if( argument[0] == 'f' )
   {
      char *src;
      char dest[MAX_STRING_LENGTH];
      int col;
      char *srcspc;
      int destspc;
      char c;
      int n, i;
      int lastcol;

      /*
       * Format text 
       */
      /*
       * Go through line by line, doing word wrapping 
       */

      lastcol = 79;

      col = 0;
      n = 0;
      srcspc = NULL;
      destspc = 0;

      for( src = buf; *src != '\0'; )
      {
         c = *( src++ );
         switch ( c )
         {
            case '\n': /* Convert /n/r into one space */
               if( ( *src == '\r' ) && ( *( src + 1 ) == '\n' ) && ( *( src + 2 ) == '\r' ) )
               {
                  /*
                   * Do not convert paragraph endings. 
                   */
                  dest[n++] = c; /* \n */
                  dest[n++] = *( src++ ); /* \r */
                  dest[n++] = *( src++ ); /* \n */
                  dest[n++] = *( src++ ); /* \r */
                  col = 0;
                  srcspc = NULL;
                  destspc = 0;
                  break;
               }

               /*
                * Also if there is a space on the next line, don't merge. 
                */
               if( ( *src == '\r' ) && ( *( src + 1 ) == ' ' ) )
               {
                  dest[n++] = c; /* \n */
                  dest[n++] = *( src++ ); /* \r */
                  col = 0;
                  srcspc = NULL;
                  destspc = 0;
                  break;
               }


               /*
                * Otherwise convert to a space 
                */
               /*
                * Get rid of spaces at end of a line. 
                */
               if( n > 0 )
               {
                  while( dest[--n] == ' ' );
                  n++;
               }
               dest[n++] = ' ';
               col++;

               srcspc = src - 1;
               destspc = n - 1;
               break;
            case '\r':
               break;

            case '\t': /* Tab */
               col += 7;
            case '.':  /* Punctuation */
            case ' ':
            case ',':
            case ';':
            case '?':
            case '!':
            case ')':
               srcspc = src - 1;
               destspc = n - 1;
            case '-':
               if( srcspc == NULL )
               {
                  srcspc = src - 1; /* Only use a dash if necessary */
                  destspc = n - 1;
               }
            default:
               dest[n++] = c;
               col++;
               break;
         }

         if( col >= lastcol )
         {
            /*
             * Need to do a line break 
             */
            if( srcspc == NULL )
            {
               /*
                * there were no breakable characters on the line. 
                */
               dest[n++] = '\n';
               dest[n++] = '\r';
            }
            else
            {
               n = destspc;   /* n now points to a breakable char. */
               src = srcspc;
               while( dest[n] == ' ' || dest[n] == '\n' )
               {
                  n--;
               }

               src++;
               n++;
               if( *src == '\r' )
                  src++;
               while( *src == ' ' )
                  src++;
               /*
                * src now points to the new line to be put in dest. 
                */
               dest[n++] = '\n';
               dest[n++] = '\r';
               col = 0;
               srcspc = NULL;
               destspc = 0;
            }
         }
      }

      /*
       * Get rid of spaces at end, and add a newline. 
       */

      while( dest[--n] == ' ' );
      n++;
      dest[n++] = '\n';
      dest[n++] = '\r';

      /*
       * Copy from dest back into buffer 
       */

      src = buf;
      for( i = 0; i < n; i++ )
         *( src++ ) = dest[i];
      *( src ) = '\0';

      return;
   }

   if( !str_cmp( argument, "clear" ) )
   {
      buf[0] = '\0';
      send_to_char( "Done.\n\r", buf_data->ch );
      return;
   }

   send_to_char( "Command not known, type .help for help.\n\r", buf_data->ch );
   return;
}
