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

#include "globals.h"

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
#endif

#ifndef DEC_EMAIL_H
#include "email.h"
#endif

bool valid_email_addy( char *address )
{
/* checks for simple email address, person@place.cat...if valid, returns
   TRUE, otherwise, sends a message to ch, and stores it anyway, but
   ch->valid_email is set to false
*/

   char *checkme = address;
   bool valid = TRUE;

   for( ; *checkme != '\0'; checkme++ )
   {
      if( ( IS_LETTER( *checkme ) ) || ( *checkme == '.' ) || ( *checkme == '@' ) || ( *checkme == '_' ) )
         continue;
      else
      {
         valid = FALSE;
         break;
      }
   }
   return valid;
}


void do_email( CHAR_DATA * ch, char *argument )
{
/* interface for setting up email addresses */
   char arg1[MSL];
   char arg2[MSL];
   char outbuf[MSL];
   char catbuf[MSL];
   bool valid_email = FALSE;

   if( IS_NPC( ch ) )
      return;
   argument = one_argument( argument, arg1 );
   if( arg1[0] == '\0' )
   {
      xprintf( outbuf, "%s", "Syntax for email:\n\r" );
      xprintf( catbuf, "%s", "set <email address>\n\r" );
      xcat( outbuf, catbuf );
      if( get_trust( ch ) == MAX_LEVEL )
      {
         xprintf( catbuf, "%s", "validate <playername>\n\r" );
         xcat( outbuf, catbuf );
      }
      if( ch->pcdata->valid_email )
      {
         xprintf( catbuf, "Your email address is currently set to %s.\n\r", ch->pcdata->email_address );
         xcat( outbuf, catbuf );
      }
      else
      {
         if( !str_cmp( ch->pcdata->email_address, "not set" ) )
            xcat( outbuf, "Your email address has not been set.\n\r" );
         else
         {
            xprintf( catbuf, "Your email address has been set to %s, but has not been authorized by an Implementor.\n\r",
                     ch->pcdata->email_address );
            xcat( outbuf, catbuf );
         }
      }
      send_to_char( outbuf, ch );
      return;
   }

   argument = one_argument( argument, arg2 );
   if( arg2[0] == '\0' )
   {
      do_email( ch, "" );
      return;
   }
   if( !str_cmp( arg1, "set" ) )
   {
      valid_email = valid_email_addy( arg2 );
      if( valid_email )
      {
         free_string( ch->pcdata->email_address );
         ch->pcdata->email_address = str_dup( arg2 );
         ch->pcdata->valid_email = FALSE;
         do_save( ch, "auto" );
         xprintf( outbuf,
                  "Your email address has been set to %s. The Implementors have been notified, please be patient.\n\r",
                  ch->pcdata->email_address );
         send_to_char( outbuf, ch );
         {
            BRAND_DATA *brand;
            DL_LIST *brand_member;
            char brandbuf[MSL];
            char *strtime;

            for( brand_member = first_brand; brand_member; brand_member = brand_member->next )
            {
               brand = (BRAND_DATA *)brand_member->this_one;
               if( ( !str_cmp( brand->branded, ch->name ) ) && ( !str_cmp( brand->priority, "Email Validation" ) ) )
                  break;
            }
            if( brand_member )
            {
               brand = (BRAND_DATA *)brand_member->this_one;
               PUT_FREE( brand, brand_data_free );
               PUT_FREE( brand_member, dl_list_free );
            }
            xprintf( brandbuf,
                     "Email address validation request for %s, address %s\n\rPlease type email validate %s to authorize.\n\r",
                     ch->name, ch->pcdata->email_address, ch->name );
            GET_FREE( brand, brand_data_free );
            GET_FREE( brand_member, dl_list_free );
            brand->branded = str_dup( ch->name );
            brand->branded_by = str_dup( "@@rSystem@@N" );
            brand->priority = str_dup( "Email Validation" );
            brand->message = str_dup( brandbuf );
            strtime = ctime( &current_time );
            strtime[strlen( strtime ) - 1] = '\0';
            brand->dt_stamp = str_dup( strtime );
            brand_member->next = NULL;
            brand_member->prev = NULL;
            brand_member->this_one = brand;
            LINK( brand_member, first_brand, last_brand, next, prev );
            save_brands(  );
            monitor_chan( "New Immbrand posted.", MONITOR_SYSTEM );
         }
         return;
      }
      else
      {
         xprintf( outbuf, "%s is not an acceptable email address.\n\r", arg2 );
         send_to_char( outbuf, ch );
         return;
      }
   }
   if( !str_cmp( arg1, "validate" ) )
   {

      if( get_trust( ch ) < MAX_LEVEL )
      {
         send_to_char( "Only Implementors may use this command.\n\r", ch );
         return;
      }
      else
      {
         CHAR_DATA *victim;
         DESCRIPTOR_DATA d;
         BRAND_DATA *brand;
         DL_LIST *brand_list;

         bool logged_in = FALSE;
         if( arg2[0] == '\0' )
         {
            send_to_char( "Authorize email for whom?\n\r", ch );
            return;
         }

         if( ( victim = get_char_world( ch, arg2 ) ) == NULL )
         {
            bool found = FALSE;
            found = load_char_obj( &d, arg2, TRUE );

            if( !found )
            {
               char buf[MSL];
               xprintf( buf, "No pFile found for '%s'.\n\r", capitalize( arg2 ) );
               send_to_char( buf, ch );
               free_char( d.character );
               return;
            }

            victim = d.character;
            d.character = NULL;
            victim->desc = NULL;
            LINK( victim, first_char, last_char, next, prev );
         }
         else
         {
            logged_in = TRUE;
         }

         if( IS_NPC( victim ) )
         {
            send_to_char( "Not on NPC's.\n\r", ch );
            return;
         }

         victim->pcdata->valid_email = TRUE;
         send_to_char( "OK.\n\r", ch );

         for( brand_list = first_brand; brand_list; brand_list = brand_list->next )
         {
            brand = (BRAND_DATA *)brand_list->this_one;
            if( ( !str_cmp( brand->branded, victim->name ) ) && ( !str_cmp( brand->priority, "Email Validation" ) ) )
               break;
         }
         if( brand_list != NULL )
         {
            UNLINK( brand_list, first_brand, last_brand, next, prev );
            brand = (BRAND_DATA *)brand_list->this_one;
            PUT_FREE( brand, brand_data_free );
            brand_list->this_one = NULL;
            PUT_FREE( brand_list, dl_list_free );
            save_brands(  );
         }
         if( !logged_in )
            do_quit( victim, "" );

         return;
      }
   }
   do_email( ch, "" );
   return;
}




void send_email( const char *m_address, const char *m_subject, const char *mfilename )
{
   FILE *mailfp;
   char mailbuf[MSL];
   char mailfpbuf[MSL];
   char delbuf[MSL];
   char dbbuf[MSL];
   int forkval;

   xprintf( mailbuf, "mail -s \"%s\" %s <%s%s", m_subject, m_address, MAIL_DIR, capitalize( mfilename ) );
   signal( SIGCHLD, SIG_IGN );
   if( ( forkval = fork(  ) ) > 0 )
   {
      xprintf( dbbuf, "Just sent email: %s", mailbuf );
      monitor_chan( dbbuf, MONITOR_SYSTEM );
      return;
   }
   else if( forkval < 0 )
   {
      xprintf( dbbuf, "Error in fork for sent email: %s", mailbuf );
      monitor_chan( dbbuf, MONITOR_SYSTEM );

      return;
   }
   xprintf( mailfpbuf, "%s%s", MAIL_DIR, mfilename );
   if( ( mailfp = file_open( mailfpbuf, "r" ) ) == NULL )
   {
      file_close(mailfp);
      kill( getpid(  ), SIGKILL );  /* didn't have a valid file to mail */
   }

   file_close(mailfp);
   system( mailbuf );
   xprintf( delbuf, "rm %s%s", MAIL_DIR, mfilename );
   system( delbuf );
   kill( getpid(  ), SIGKILL );
   return;
}

bool save_mail_file( const char *mfilename, char *mtext )
{
   FILE *mailfp;
   char mailfpfilename[MSL];

   xprintf( mailfpfilename, "%s%s", MAIL_DIR, mfilename );
   if( ( mailfp = file_open( mailfpfilename, "w" ) ) == NULL )
   {
      file_close(mailfp);
      return FALSE;
   }
   fprintf( mailfp, "%s\n", strip_color( mtext, "@@" ) );
   file_close(mailfp);
   return TRUE;
}

void send_rep_out( CHAR_DATA * ch, char *outbuf, bool mailme, char *msub )
{
   if( ( IS_NPC( ch ) ) || ( ch->pcdata->valid_email == FALSE ) )
   {
      mailme = FALSE;
   }
   if( mailme )
   {
      bool saved_mail = FALSE;
      if( ( !IS_NPC( ch ) ) && ( str_cmp( ch->pcdata->email_address, "not set" ) ) )
      {
         char mailfilename[MSL];
         xprintf( mailfilename, "%s.mail", ch->name );
         saved_mail = save_mail_file( mailfilename, outbuf );
         if( saved_mail )
         {
            char outbuf2[MSL];
            xprintf( outbuf2, "Email sent to %s\n\r", ch->pcdata->email_address );
            send_to_char( outbuf2, ch );
            send_email( ch->pcdata->email_address, msub, mailfilename );
         }
         else
         {
            send_to_char( outbuf, ch );
            send_to_char( "\n\r@@eUNABLE TO SEND SYSTEM MAIL. @@WCheck your sendmail settings.@@N\n\r", ch );
         }
      }
      else
      {
         send_to_char( outbuf, ch );
      }
   }
   else
   {
      send_to_char( outbuf, ch );
   }
}
