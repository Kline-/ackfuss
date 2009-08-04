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

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

const char *get_ruler_title( int ruler_rank, int sex )
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
   list<RULER_DATA *>::iterator li;
   RULER_DATA *ruler = NULL;

   snprintf( ruler_file_name, MSL, "%s", RULERS_FILE );

   if( ( fp = file_open( ruler_file_name, "w" ) ) == NULL )
   {
      bug( "Save ruler list: file_open", 0 );
      perror( "failed open of rulers.lst in save_ruler" );
   }
   else
   {
      for( li = ruler_list.begin(); li != ruler_list.end(); li++ )
      {
         ruler = *li;
         fprintf( fp, "#RULER~\n" );
         fprintf( fp, "%s~\n", ruler->name );
         fprintf( fp, "%d\n", ruler->affiliation_index );
         fprintf( fp, "%d\n", ruler->flags );
         fprintf( fp, "%d\n", ruler->ruler_rank );
      }
      fprintf( fp, "#END~\n\n" );
   }


   file_close( fp );
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


   snprintf( rulers_file_name, MSL, "%s", RULERS_FILE );


   snprintf( buf, MSL, "Loading %s",RULERS_FILE);
   log_f("%s",buf);

   if( ( rulersfp = file_open( rulers_file_name, "r" ) ) == NULL )
   {
      bug( "Load rulers Table: file_open", 0 );
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
            ruler = new RULER_DATA;
            ruler->name = fread_string( rulersfp );
            ruler->affiliation_index = fread_number( rulersfp );
            if( ruler->affiliation_index == 0 )
               ruler->affiliation_name = str_dup( "No Affiliation" );
            else
               ruler->affiliation_name = str_dup( clan_table[ruler->affiliation_index].clan_abbr );
            ruler->flags = fread_number( rulersfp );
            ruler->ruler_rank = fread_number( rulersfp );
            ruler->keywords = fread_string( rulersfp );

            free_string( word );

            ruler_list.push_back(ruler);

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

      file_close( rulersfp );

      log_f("Done.");

   }
}



void do_rulers( CHAR_DATA * ch, char *argument )
{
   char arg1[MSL];
   char arg2[MSL];
   char arg3[MSL];
   char outbuf[MSL];
   char catbuf[MSL];
   RULER_DATA *ruler = NULL;
   list<RULER_DATA *>::iterator li;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( IS_NPC( ch ) )
   {
      send_to_char( "This option is not for NPCs", ch );
      return;
   }

   if( arg1[0] == '\0' )   /* show current rulers */
   {

      snprintf( outbuf, MSL, "\r\nCurrent Rulers of " mudnamecolor "\r\n\r\n" );

      for( li = ruler_list.begin(); li != ruler_list.end(); li++ )
      {
         short sex;
         ruler = *li;
         sex = ( IS_SET( ruler->flags, RULER_NEUTRAL ) ? SEX_NEUTRAL
                 : IS_SET( ruler->flags, RULER_MALE ) ? SEX_MALE : SEX_FEMALE );
         snprintf( catbuf, MSL, " %s %s : %s \r\n",
                  get_ruler_title( ruler->ruler_rank, sex ),
                  capitalize( ruler->name ),
                  ( IS_SET( ruler->flags, RULER_GROUP ) ? ruler->affiliation_name : "Not Affiliated" ) );
         strncat( outbuf, catbuf, MSL-1 );
      }
      send_to_char( outbuf, ch );
      return;
   }

   if( !str_prefix( arg1, "new" ) )
   {
      if( get_trust( ch ) < L_SUP )
      {
         send_to_char( "Huh?\r\n", ch );
         return;
      }

      if( arg2[0] == '\0' )
      {
         send_to_char( "What name?\r\n", ch );
         return;
      }

      ruler = new RULER_DATA;
      ruler->name = str_dup( arg2 );
      ruler->ruler_rank = 1;
      ruler->affiliation_index = 0;
      ruler->affiliation_name = str_dup( "" );
      ruler->keywords = str_dup( "" );
      ruler->flags = RULER_NEUTRAL;

      ruler_list.push_back(ruler);
      save_rulers(  );
      do_rulers( ch, "" );
      return;
   }

   if( !str_prefix( arg1, "affiliation" ) )
   {
      short clan_number;

      if( get_trust( ch ) < L_SUP )
      {
         send_to_char( "Huh?\r\n", ch );
         return;
      }

      if( arg2[0] == '\0' )
      {
         send_to_char( "What name?\r\n", ch );
         return;
      }

      argument = one_argument( argument, arg3 );
      if( arg3[0] == '\0' )
      {
         send_to_char( "What Clan Number?\r\n", ch );
         return;
      }
      if( is_number( arg3 ) )
      {
         clan_number = atoi( arg3 );
         if( ( clan_number < 1 ) || ( clan_number > MAX_CLAN ) )
         {
            send_to_char( "Clan number must be between 1 and you Max Number of clans.\r\n", ch );
            return;
         }
         for( li = ruler_list.begin(); li != ruler_list.end(); li++ )
         {
            ruler = *li;
            if( !str_cmp( arg2, ruler->name ) )
               break;
         }
         if( ruler == NULL )
         {
            send_to_char( "That ruler is not in the current list.\r\n", ch );
            return;
         }
         if( ruler->affiliation_name != NULL )
            free_string( ruler->affiliation_name );

         ruler->affiliation_name = str_dup( clan_table[clan_number].clan_abbr );
         SET_BIT( ruler->flags, RULER_GROUP );
         ruler->affiliation_index = clan_number;
         save_rulers(  );
         do_rulers( ch, "" );
         return;
      }
      send_to_char( "That is not a legitimate clan number.\r\n", ch );
      return;
   }
   if( !str_prefix( arg1, "delete" ) )
   {
      if( get_trust( ch ) < L_SUP )
      {
         send_to_char( "Huh?\r\n", ch );
         return;
      }

      if( arg2[0] == '\0' )
      {
         send_to_char( "What name?\r\n", ch );
         return;
      }
      for( li = ruler_list.begin(); li != ruler_list.end(); li++ )
      {
         ruler = *li;
         if( !str_cmp( arg2, ruler->name ) )
            break;
      }
      if( ruler == NULL )
      {
         send_to_char( "That ruler is not in the current list.\r\n", ch );
         return;
      }

      ruler_list.remove(ruler);
      delete ruler;
      save_rulers(  );
      do_rulers( ch, "" );
      return;
   }

   if( !str_prefix( arg1, "sex" ) )
   {
      if( get_trust( ch ) < L_SUP )
      {
         send_to_char( "Huh?\r\n", ch );
         return;
      }

      if( arg2[0] == '\0' )
      {
         send_to_char( "What name?\r\n", ch );
         return;
      }
      for( li = ruler_list.begin(); li != ruler_list.end(); li++ )
      {
         ruler = *li;
         if( !str_cmp( arg2, ruler->name ) )
            break;
      }
      if( ruler == NULL )
      {
         send_to_char( "That ruler is not in the current list.\r\n", ch );
         return;
      }
      argument = one_argument( argument, arg3 );
      if( IS_SET( ruler->flags, RULER_MALE ) )
         REMOVE_BIT( ruler->flags, RULER_MALE );
      if( IS_SET( ruler->flags, RULER_FEMALE ) )
         REMOVE_BIT( ruler->flags, RULER_FEMALE );
      if( IS_SET( ruler->flags, RULER_NEUTRAL ) )
         REMOVE_BIT( ruler->flags, RULER_NEUTRAL );

      if( !str_cmp( arg3, "male" ) )
         SET_BIT( ruler->flags, RULER_MALE );
      else if( !str_cmp( arg3, "female" ) )
         SET_BIT( ruler->flags, RULER_FEMALE );
      else if( !str_cmp( arg3, "it" ) )
         SET_BIT( ruler->flags, RULER_NEUTRAL );
      else
      {
         send_to_char( "Use male/female/it\r\n", ch );
         SET_BIT( ruler->flags, RULER_MALE );
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
         send_to_char( "Huh?\r\n", ch );
         return;
      }

      if( arg2[0] == '\0' )
      {
         send_to_char( "What name?\r\n", ch );
         return;
      }
      argument = one_argument( argument, arg3 );

      if( arg3[0] == '\0' )
      {
         send_to_char( "What rank?\r\n", ch );
         return;
      }
      if( is_number( arg3 ) )
      {
         short rank;
         rank = atoi( arg3 );
         if( ( rank < 1 ) || ( rank > 5 ) )
         {
            send_to_char( "Rank must be between 1 and 5.\r\n", ch );
            return;
         }
         for( li = ruler_list.begin(); li != ruler_list.end(); li++ )
         {
            ruler = *li;
            if( !str_cmp( arg2, ruler->name ) )
               break;
         }
         if( ruler == NULL )
         {
            send_to_char( "That ruler is not in the current list.\r\n", ch );
            return;
         }
         ruler->ruler_rank = rank;
         save_rulers(  );
         do_rulers( ch, "" );
         return;
      }
      send_to_char( "That is not a legitimate rank.\r\n", ch );
      return;
   }


   send_to_char( "Syntax for rulers:\r\n", ch );
   send_to_char( "rulers new/affiliation/delete/sex name <clan number>\r\n", ch );
   return;

}
