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

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
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

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_SAVE_H
#include "h/save.h"
#endif

#ifndef DEC_SPENDQP_H
#include "h/spendqp.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
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


DO_FUN(do_email)
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
      snprintf( outbuf, MSL, "%s", "Syntax for email:\r\n" );
      snprintf( catbuf, MSL, "%s", "set <email address>\r\n" );
      strncat( outbuf, catbuf, MSL-1 );
      if( get_trust( ch ) == MAX_LEVEL )
      {
         snprintf( catbuf, MSL, "%s", "validate <playername>\r\n" );
         strncat( outbuf, catbuf, MSL-1 );
      }
      if( ch->pcdata->valid_email )
      {
         snprintf( catbuf, MSL, "Your email address is currently set to %s.\r\n", ch->pcdata->email_address );
         strncat( outbuf, catbuf, MSL-1 );
      }
      else
      {
         if( !str_cmp( ch->pcdata->email_address, "not set" ) )
            strncat( outbuf, "Your email address has not been set.\r\n", MSL-1 );
         else
         {
            snprintf( catbuf, MSL, "Your email address has been set to %s, but has not been authorized by an Implementor.\r\n",
                     ch->pcdata->email_address );
            strncat( outbuf, catbuf, MSL-1 );
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
         snprintf( outbuf, MSL,
                  "Your email address has been set to %s. The Implementors have been notified, please be patient.\r\n",
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
               delete brand;
               PUT_FREE( brand_member, dl_list_free );
            }
            snprintf( brandbuf, MSL,
                     "Email address validation request for %s, address %s\r\nPlease type email validate %s to authorize.\r\n",
                     ch->name.c_str(), ch->pcdata->email_address, ch->name.c_str() );
            brand = new BRAND_DATA;
            GET_FREE( brand_member, dl_list_free );
            brand->branded = str_dup( ch->name.c_str() );
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
         snprintf( outbuf, MSL, "%s is not an acceptable email address.\r\n", arg2 );
         send_to_char( outbuf, ch );
         return;
      }
   }
   if( !str_cmp( arg1, "validate" ) )
   {

      if( get_trust( ch ) < MAX_LEVEL )
      {
         send_to_char( "Only Implementors may use this command.\r\n", ch );
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
            send_to_char( "Authorize email for whom?\r\n", ch );
            return;
         }

         if( ( victim = get_char_world( ch, arg2 ) ) == NULL )
         {
            bool found = FALSE;
            found = load_char_obj( &d, arg2, TRUE );

            if( !found )
            {
               char buf[MSL];
               snprintf( buf, MSL, "No pFile found for '%s'.\r\n", capitalize( arg2 ) );
               send_to_char( buf, ch );
               delete d.character;
               return;
            }

            victim = d.character;
            d.character = NULL;
            victim->desc = NULL;
         }
         else
         {
            logged_in = TRUE;
         }

         if( IS_NPC( victim ) )
         {
            send_to_char( "Not on NPC's.\r\n", ch );
            return;
         }

         victim->pcdata->valid_email = TRUE;
         send_to_char( "OK.\r\n", ch );

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
            delete brand;
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

   snprintf( mailbuf, MSL, "mail -s \"%s\" %s <%s%s", m_subject, m_address, MAIL_DIR, capitalize( mfilename ) );
   signal( SIGCHLD, SIG_IGN );
   if( ( forkval = fork(  ) ) > 0 )
   {
      snprintf( dbbuf, MSL, "Just sent email: %s", mailbuf );
      monitor_chan( dbbuf, MONITOR_SYSTEM );
      return;
   }
   else if( forkval < 0 )
   {
      snprintf( dbbuf, MSL, "Error in fork for sent email: %s", mailbuf );
      monitor_chan( dbbuf, MONITOR_SYSTEM );

      return;
   }
   snprintf( mailfpbuf, MSL, "%s%s", MAIL_DIR, mfilename );
   if( ( mailfp = file_open( mailfpbuf, "r" ) ) == NULL )
   {
      file_close(mailfp);
      kill( getpid(  ), SIGKILL );  /* didn't have a valid file to mail */
   }

   file_close(mailfp);
   /*
    * system() is if() encapsulated to suppress a warning. system() returns different results on different distros,
    * so there is no reliable return value to check against. --Kline
    */
   if( system( mailbuf ) ) {}
   snprintf( delbuf, MSL, "rm %s%s", MAIL_DIR, mfilename );
   if( system( delbuf ) ) {}
   kill( getpid(  ), SIGKILL );
   return;
}

bool save_mail_file( const char *mfilename, char *mtext )
{
   FILE *mailfp;
   char mailfpfilename[MSL];

   snprintf( mailfpfilename, MSL, "%s%s", MAIL_DIR, mfilename );
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
         snprintf( mailfilename, MSL, "%s.mail", ch->name.c_str() );
         saved_mail = save_mail_file( mailfilename, outbuf );
         if( saved_mail )
         {
            char outbuf2[MSL];
            snprintf( outbuf2, MSL, "Email sent to %s\r\n", ch->pcdata->email_address );
            send_to_char( outbuf2, ch );
            send_email( ch->pcdata->email_address, msub, mailfilename );
         }
         else
         {
            send_to_char( outbuf, ch );
            send_to_char( "\r\n@@eUNABLE TO SEND SYSTEM MAIL. @@WCheck your sendmail settings.@@N\r\n", ch );
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
