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
#include <string.h>
#include <time.h>
#include "globals.h"
#include <math.h>
#include <stdlib.h>

void load_sysdata( void )
{
   FILE *sysfp;
   char sys_file_name[MAX_STRING_LENGTH];
   extern bool wizlock;
   sprintf( sys_file_name, "%s", SYSDAT_FILE );

   if( ( sysfp = fopen( sys_file_name, "r" ) ) == NULL )
   {
      bug( "Load Sys Table: fopen", 0 );
      log_f( "failed open of system.dat in load_sysdata" );
   }
   else
   {
      sh_int looper;
      sysdata.playtesters = fread_string( sysfp );
      for( looper = 0; looper < MAX_NUM_IMMS; looper++ )
         sysdata.imms[looper].this_string = fread_string( sysfp );
      sysdata.w_lock = fread_number( sysfp );
      sysdata.shownumbers = ( fread_number( sysfp ) == 1 ? TRUE : FALSE );
      if( sysdata.w_lock == 1 )
      {
         wizlock = TRUE;
      }
   }
   fclose( sysfp );
}


void save_sysdata( void )
{

   FILE *fp;
   char sys_file_name[MAX_STRING_LENGTH];
   extern bool wizlock;

   fclose( fpReserve );
   sprintf( sys_file_name, "%s", SYSDAT_FILE );

   if( ( fp = fopen( sys_file_name, "w" ) ) == NULL )
   {
      bug( "Save Sysdata: fopen", 0 );
      log_f( "failed open of system.dat in save_sysdata" );
   }
   else
   {
      sh_int looper;
      fprintf( fp, "%s~\n\r", sysdata.playtesters );
      for( looper = 0; looper < MAX_NUM_IMMS; looper++ )
         fprintf( fp, "%s~\n\r", sysdata.imms[looper].this_string );
      fprintf( fp, "%d\n\r", ( wizlock ? 1 : 0 ) );
      fprintf( fp, "%d\n\r", ( sysdata.shownumbers ? 1 : 0 ) );
      fflush( fp );
      fclose( fp );
   }
   fpReserve = fopen( NULL_FILE, "r" );
   return;

}


void do_sysdata( CHAR_DATA * ch, char *argument )
{
   char outbuf[MSL];
   char catbuf[MSL];
   char arg1[MSL];
   char arg2[MSL];
   extern bool wizlock;
   if( get_trust( ch ) < MAX_LEVEL )
   {
      send_to_char( "Nice try.\n\r", ch );
      return;
   }
   argument = one_argument( argument, arg1 );
   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax for sysedit:\n\r", ch );
      send_to_char( "  sysedit config <value> | <string> <+/-> <new_word>\n\r", ch );
      send_to_char( "  strings:  testers 81 82 83 84 85\n\r", ch );
      send_to_char( "  config values: shownumbers\n\r", ch );
      return;
   }
   if( !str_prefix( arg1, "show" ) )
   {
      sh_int looper;
      sprintf( outbuf, "%s", "System data for " mudnamecolor ":\n\r" );
      sprintf( catbuf, "Playtesters: %s\n\r", sysdata.playtesters );
      safe_strcat( MSL, outbuf, catbuf );
      for( looper = 0; looper < MAX_NUM_IMMS; looper++ )
      {
         sprintf( catbuf, "Level %d Immortals: %s\n\r", 81 + looper, sysdata.imms[looper].this_string );
         safe_strcat( MSL, outbuf, catbuf );
      }
      sprintf( catbuf, "Wizlocked: %s\n\r", ( wizlock ? "Yes" : "No" ) );
      safe_strcat( MSL, outbuf, catbuf );
      sprintf( catbuf, "Show Players Damage numbers in combat: %s\n\r", ( sysdata.shownumbers ? "Yes" : "No" ) );
      safe_strcat( MSL, outbuf, catbuf );
      send_to_char( outbuf, ch );
      return;
   }
   if( !str_cmp( arg1, "config" ) )
   {
      argument = one_argument( argument, arg2 );
      if( !str_cmp( arg2, "shownumbers" ) )
      {
         sysdata.shownumbers = ( sysdata.shownumbers ? FALSE : TRUE );
         save_sysdata(  );
         do_sysdata( ch, "show" );
         return;
      }
      else
      {
         send_to_char( "Option not implemented.\n\r", ch );
         return;
      }
   }

   if( !is_name( arg1, "testers 81 82 83 84 85" ) )
   {
      do_sysdata( ch, "" );
      return;
   }
   for( ;; )
   {
      sh_int imm_level = -1;
      argument = one_argument( argument, arg2 );
      if( is_number( arg2 ) )
         if( ( imm_level = atoi( arg2 ) - 81 ) < 0 || imm_level >= MAX_NUM_IMMS )
         {
            send_to_char( "Imm levels must be 81 to 85\n\r", ch );
            return;
         }

      if( str_cmp( arg2, "" ) )
      {
         if( !str_prefix( arg1, "testers" ) )
         {
            sysdata.playtesters = str_mod( sysdata.playtesters, arg2 );
         }
         else if( is_name( arg1, "81 82 83 84 85" ) )
         {
            char arg3[MSL];
            strcpy( arg3, argument );
            if( ( imm_level < 0 ) || ( imm_level >= MAX_NUM_IMMS ) )
            {
               send_to_char( "Illegal Immortal level selected.\n\r", ch );
               return;
            }
            sysdata.imms[imm_level].this_string = str_mod( sysdata.imms[imm_level].this_string, arg2 );
         }
         else
         {
            send_to_char( "Unknown system string.\n\r", ch );
            do_sysdata( ch, "" );
            return;
         }
      }
      else
         break;
   }
   save_sysdata(  );
   do_sysdata( ch, "show" );
   return;
}
