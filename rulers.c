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

#include "globals.h"
#include "tables.h"

char *get_ruler_title( int ruler_rank, int sex )
{

   switch ( ruler_rank )
   {
      case 0:
         return "";
      case 1:
         return ( ( sex == SEX_NEUTRAL ) ? "@@WMonitor@@N " : ( sex == SEX_MALE ) ? "@@WLord@@N " : "@@WLady@@N " );
         break;
      case 2:
         return ( ( sex == SEX_NEUTRAL ) ? "@@eOverseer@@N " : ( sex == SEX_MALE ) ? "@@eBaron@@N " : "@@eBaroness@@N " );
         break;
      case 3:
         return ( ( sex == SEX_NEUTRAL ) ? "@@mController@@N " : ( sex == SEX_MALE ) ? "@@mDuke@@N " : "@@mDuchess@@N " );
         break;
      case 4:
         return ( ( sex == SEX_NEUTRAL ) ? "@@lDirector@@N " : ( sex == SEX_MALE ) ? "@@lPrince@@N " : "@@lPrincess@@N " );
         break;
      case 5:
         return ( ( sex == SEX_NEUTRAL ) ? "@@aDominator@@N " : ( sex == SEX_MALE ) ? "@@aKing@@N " : "@@aQueen@@N " );
         break;
   }
   return "Lord";
}

void save_rulers(  )
{

   FILE *fp;
   char ruler_file_name[MAX_STRING_LENGTH];
   RULER_LIST *ruler;
   CONTROL_LIST *control;


   fclose( fpReserve );
   sprintf( ruler_file_name, "%s", RULERS_FILE );

   if( ( fp = fopen( ruler_file_name, "w" ) ) == NULL )
   {
      bug( "Save ruler list: fopen", 0 );
      perror( "failed open of rulers.lst in save_ruler" );
   }
   else
   {
      for( ruler = first_ruler_list; ruler != NULL; ruler = ruler->next )
      {
         char keybuf[MSL];
         char catkeybuf[MSL];

         fprintf( fp, "#RULER~\n" );
         fprintf( fp, "%s~\n", ruler->this_one->name );
         fprintf( fp, "%d\n", ruler->this_one->affiliation_index );
         fprintf( fp, "%d\n", ruler->this_one->flags );
         fprintf( fp, "%d\n", ruler->this_one->ruler_rank );

         sprintf( keybuf, "%s", "" );

         for( control = ruler->this_one->first_control; control; control = control->next )
         {
            sprintf( catkeybuf, "%s ", control->this_one->keyword );
            safe_strcat( MSL, keybuf, catkeybuf );
         }
         fprintf( fp, "%s~\n", keybuf );
      }
      fprintf( fp, "#END~\n\n" );
   }


   fflush( fp );
   fclose( fp );


   fpReserve = fopen( NULL_FILE, "r" );
   return;

}

/*
#define RKEY( literal, field, value )  if ( !str_cmp( word, literal ) ) { field  = value; fMatch = TRUE;  break;}
#define RSKEY( literal, field, value )  if ( !str_cmp( word, literal ) ) { if (field!=NULL) free_string(field);field  = value; fMatch = TRUE;  break;}
*/

void load_rulers( void )
{

   FILE *rulersfp;
   char rulers_file_name[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   RULER_DATA *ruler;
   RULER_LIST *ruler_member;


   sprintf( rulers_file_name, "%s", RULERS_FILE );


   sprintf( buf, "Loading %s\n\r", rulers_file_name );
   monitor_chan( buf, MONITOR_CLAN );

   if( ( rulersfp = fopen( rulers_file_name, "r" ) ) == NULL )
   {
      bug( "Load rulers Table: fopen", 0 );
      perror( "failed open of rulers_table.dat in load_rulers_table" );
   }
   else
   {
      for( ;; )
      {

         char *word;


         word = fread_string( rulersfp );
         if( !str_cmp( word, "#RULER" ) )
         {
            GET_FREE( ruler, ruler_data_free );
            GET_FREE( ruler_member, ruler_list_free );
            ruler->name = fread_string( rulersfp );
            ruler->affiliation_index = fread_number( rulersfp );
            if( ruler->affiliation_index == 0 )
               ruler->affiliation_name = str_dup( "No Affiliation" );
            else
               ruler->affiliation_name = str_dup( clan_table[ruler->affiliation_index].clan_abbr );
            ruler->flags = fread_number( rulersfp );
            ruler->ruler_rank = fread_number( rulersfp );
            ruler->keywords = fread_string( rulersfp );
            ruler->first_control = NULL;
            ruler->last_control = NULL;

            free_string( word );

            ruler_member->this_one = ruler;
            ruler_member->next = NULL;
            ruler_member->prev = NULL;
            LINK( ruler_member, first_ruler_list, last_ruler_list, next, prev );

         }
         else if( !str_cmp( word, "#END" ) )
         {
            free_string( word );
            break;
         }
         else
         {
            free_string( word );
            monitor_chan( "Load_marks: bad section.", MONITOR_BAD );
            break;
         }
      }

      fclose( rulersfp );

      sprintf( buf, "Done Loading %s\n\r", rulers_file_name );
      monitor_chan( buf, MONITOR_CLAN );

   }
}



void do_rulers( CHAR_DATA * ch, char *argument )
{
   char arg1[MSL];
   char arg2[MSL];
   char arg3[MSL];
   char outbuf[MSL];
   char catbuf[MSL];
   RULER_LIST *ruler;
   RULER_DATA *this_ruler;
/*  CONTROL_DATA	* this_control;  */

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( IS_NPC( ch ) )
   {
      send_to_char( "This option is not for NPCs", ch );
      return;
   }

   if( arg1[0] == '\0' )   /* show current rulers */
   {

      sprintf( outbuf, "\n\rCurrent Rulers of " mudnamecolor "\n\r\n\r" );

      for( ruler = first_ruler_list; ruler; ruler = ruler->next )
      {
         sh_int sex;
         sex = ( IS_SET( ruler->this_one->flags, RULER_NEUTRAL ) ? SEX_NEUTRAL
                 : IS_SET( ruler->this_one->flags, RULER_MALE ) ? SEX_MALE : SEX_FEMALE );
         sprintf( catbuf, " %s %s : %s \n\r",
                  get_ruler_title( ruler->this_one->ruler_rank, sex ),
                  capitalize( ruler->this_one->name ),
                  ( IS_SET( ruler->this_one->flags, RULER_GROUP ) ? ruler->this_one->affiliation_name : "Not Affiliated" ) );
         safe_strcat( MSL, outbuf, catbuf );
         if( ruler->this_one->first_control != NULL )
         {
            CONTROL_LIST *control;
            sprintf( catbuf, "  Ruler of :" );
            safe_strcat( MSL, catbuf, outbuf );
            for( control = ruler->this_one->first_control; control; control = control->next )
            {
               sprintf( catbuf, "%s\n\r", control->this_one->area->name );
               safe_strcat( MSL, outbuf, catbuf );
            }
         }
      }
      send_to_char( outbuf, ch );
      return;
   }

   if( !str_prefix( arg1, "new" ) )
   {
      if( get_trust( ch ) < L_SUP )
      {
         send_to_char( "Huh?\n\r", ch );
         return;
      }

      if( arg2[0] == '\0' )
      {
         send_to_char( "What name?\n\r", ch );
         return;
      }

      GET_FREE( ruler, ruler_list_free );
      GET_FREE( this_ruler, ruler_data_free );
      this_ruler->name = str_dup( arg2 );
      this_ruler->ruler_rank = 1;
      this_ruler->affiliation_index = 0;
      this_ruler->affiliation_name = str_dup( "" );
      this_ruler->keywords = str_dup( "" );
      this_ruler->flags = RULER_NEUTRAL;
      this_ruler->first_control = NULL;
      this_ruler->last_control = NULL;
      this_ruler->next = NULL;
      this_ruler->prev = NULL;
      ruler->this_one = this_ruler;
      ruler->next = NULL;
      ruler->prev = NULL;

      LINK( ruler, first_ruler_list, last_ruler_list, next, prev );
      save_rulers(  );
      do_rulers( ch, "" );
      return;
   }

   if( !str_prefix( arg1, "affiliation" ) )
   {
      sh_int clan_number;

      if( get_trust( ch ) < L_SUP )
      {
         send_to_char( "Huh?\n\r", ch );
         return;
      }

      if( arg2[0] == '\0' )
      {
         send_to_char( "What name?\n\r", ch );
         return;
      }

      argument = one_argument( argument, arg3 );
      if( arg3[0] == '\0' )
      {
         send_to_char( "What Clan Number?\n\r", ch );
         return;
      }
      if( is_number( arg3 ) )
      {
         clan_number = atoi( arg3 );
         if( ( clan_number < 1 ) || ( clan_number > MAX_CLAN ) )
         {
            send_to_char( "Clan number must be between 1 and you Max Number of clans.\n\r", ch );
            return;
         }
         for( ruler = first_ruler_list; ruler; ruler = ruler->next )
         {
            if( !str_cmp( arg2, ruler->this_one->name ) )
               break;
         }
         if( ruler == NULL )
         {
            send_to_char( "That ruler is not in the current list.\n\r", ch );
            return;
         }
         if( ruler->this_one->affiliation_name != NULL )
            free_string( ruler->this_one->affiliation_name );

         ruler->this_one->affiliation_name = str_dup( clan_table[clan_number].clan_abbr );
         SET_BIT( ruler->this_one->flags, RULER_GROUP );
         ruler->this_one->affiliation_index = clan_number;
         save_rulers(  );
         do_rulers( ch, "" );
         return;
      }
      send_to_char( "That is not a legitimate clan number.\n\r", ch );
      return;
   }
   if( !str_prefix( arg1, "delete" ) )
   {
      if( get_trust( ch ) < L_SUP )
      {
         send_to_char( "Huh?\n\r", ch );
         return;
      }

      if( arg2[0] == '\0' )
      {
         send_to_char( "What name?\n\r", ch );
         return;
      }
      for( ruler = first_ruler_list; ruler; ruler = ruler->next )
      {
         if( !str_cmp( arg2, ruler->this_one->name ) )
            break;
      }
      if( ruler == NULL )
      {
         send_to_char( "That ruler is not in the current list.\n\r", ch );
         return;
      }
      UNLINK( ruler, first_ruler_list, last_ruler_list, next, prev );

      if( ruler->this_one->first_control != NULL )
      {
         CONTROL_LIST *control;
         CONTROL_LIST *control_next;
         for( control = ruler->this_one->first_control; control; control = control_next )
         {
            control_next = control->next;
            UNLINK( control, ruler->this_one->first_control, ruler->this_one->last_control, next, prev );
            control->this_one = NULL;
            PUT_FREE( control, control_list_free );
         }
      }
      PUT_FREE( ruler->this_one, ruler_data_free );
      PUT_FREE( ruler, ruler_list_free );
      save_rulers(  );
      do_rulers( ch, "" );
      return;
   }

   if( !str_prefix( arg1, "sex" ) )
   {
      if( get_trust( ch ) < L_SUP )
      {
         send_to_char( "Huh?\n\r", ch );
         return;
      }

      if( arg2[0] == '\0' )
      {
         send_to_char( "What name?\n\r", ch );
         return;
      }
      for( ruler = first_ruler_list; ruler; ruler = ruler->next )
      {
         if( !str_cmp( arg2, ruler->this_one->name ) )
            break;
      }
      if( ruler == NULL )
      {
         send_to_char( "That ruler is not in the current list.\n\r", ch );
         return;
      }
      argument = one_argument( argument, arg3 );
      if( IS_SET( ruler->this_one->flags, RULER_MALE ) )
         REMOVE_BIT( ruler->this_one->flags, RULER_MALE );
      if( IS_SET( ruler->this_one->flags, RULER_FEMALE ) )
         REMOVE_BIT( ruler->this_one->flags, RULER_FEMALE );
      if( IS_SET( ruler->this_one->flags, RULER_NEUTRAL ) )
         REMOVE_BIT( ruler->this_one->flags, RULER_NEUTRAL );

      if( !str_cmp( arg3, "male" ) )
         SET_BIT( ruler->this_one->flags, RULER_MALE );
      else if( !str_cmp( arg3, "female" ) )
         SET_BIT( ruler->this_one->flags, RULER_FEMALE );
      else if( !str_cmp( arg3, "it" ) )
         SET_BIT( ruler->this_one->flags, RULER_NEUTRAL );
      else
      {
         send_to_char( "Use male/female/it\n\r", ch );
         SET_BIT( ruler->this_one->flags, RULER_MALE );
         return;
      }
      save_rulers(  );
      do_rulers( ch, "" );
      return;
   }

   if( !str_prefix( arg1, "rank" ) )
   {
      if( get_trust( ch ) < L_SUP )
      {
         send_to_char( "Huh?\n\r", ch );
         return;
      }

      if( arg2[0] == '\0' )
      {
         send_to_char( "What name?\n\r", ch );
         return;
      }
      argument = one_argument( argument, arg3 );

      if( arg3[0] == '\0' )
      {
         send_to_char( "What rank?\n\r", ch );
         return;
      }
      if( is_number( arg3 ) )
      {
         sh_int rank;
         rank = atoi( arg3 );
         if( ( rank < 1 ) || ( rank > 5 ) )
         {
            send_to_char( "Rank must be between 1 and 5.\n\r", ch );
            return;
         }
         for( ruler = first_ruler_list; ruler; ruler = ruler->next )
         {
            if( !str_cmp( arg2, ruler->this_one->name ) )
               break;
         }
         if( ruler == NULL )
         {
            send_to_char( "That ruler is not in the current list.\n\r", ch );
            return;
         }
         ruler->this_one->ruler_rank = rank;
         save_rulers(  );
         do_rulers( ch, "" );
         return;
      }
      send_to_char( "That is not a legitimate rank.\n\r", ch );
      return;
   }


   send_to_char( "Syntax for rulers:\n\r", ch );
   send_to_char( "rulers new/affiliation/delete/sex name <clan number>\n\r", ch );
   return;

}
