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

#ifndef DEC_BUILD_H
#include "h/build.h"
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

#ifndef DEC_INTERP_H
#include "h/interp.h"
#endif

#ifndef DEC_MONEY_H
#include "h/money.h"
#endif

#ifndef DEC_SAVE_H
#include "h/save.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

/* Added back-in the note code, but made more like PO system */
bool is_note_to( CHAR_DATA * ch, NOTE_DATA * pnote )
{
   if( is_name( ch->name.c_str(), pnote->to_list ) )
      return TRUE;

   return FALSE;
}

void note_attach( CHAR_DATA * ch )
{
   NOTE_DATA *pnote;

   if( ch->pcdata->pnote )
      return;

   pnote = new NOTE_DATA;

   pnote->sender = str_dup(ch->name.c_str());
   ch->pcdata->pnote = pnote;
   return;
}



void note_remove( CHAR_DATA * ch, NOTE_DATA * pnote )
{
   FILE *fp;
   char *to_list;
   char to_new[MSL];
   char to_one[MSL];
   list<NOTE_DATA *>::iterator li;

   /*
    * Build a new to_list.
    * Strip out this recipient.
    */
   to_one[0] = '\0';

   to_new[0] = '\0';
   to_list = pnote->to_list;
   while( *to_list != '\0' )
   {
      to_list = one_argument( to_list, to_one );
      if( to_one[0] != '\0' && str_cmp( ch->name, to_one ) )
      {
         strncat( to_new, " ", MSL );
         strncat( to_new, to_one, MSL-1 );
      }
   }

   /*
    * Just a simple recipient removal?
    */
   if( str_cmp( ch->name, pnote->sender ) && to_new[0] != '\0' )
   {
      free_string( pnote->to_list );
      pnote->to_list = str_dup( to_new + 1 );
      return;
   }

   /*
    * Remove note from linked list.
    */
   note_list.remove(pnote);
   delete pnote;

   /*
    * Rewrite entire list.
    */
   if( !( fp = file_open( NOTE_FILE, "w" ) ) )
   {
      perror( NOTE_FILE );
   }
   else
   {
      for( li = note_list.begin(); li != note_list.end(); li++ )
      {
         pnote = *li;
         fprintf( fp, "Sender  %s~\n", pnote->sender );
         fprintf( fp, "Date    %s~\n", pnote->date );
         fprintf( fp, "Stamp   %ld\n", pnote->date_stamp );
         fprintf( fp, "To      %s~\n", pnote->to_list );
         fprintf( fp, "Subject %s~\n", pnote->subject );
         fprintf( fp, "Text\n%s~\n\n", pnote->text );
      }
   }
   file_close(fp);
   return;
}


/* Date stamp idea comes from Alander of ROM */
DO_FUN(do_note)
{
   CHAR_DATA *postie;
   NOTE_DATA *pnote;
   list<NOTE_DATA *>::iterator li;
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH * 7];
   char arg[MAX_INPUT_LENGTH];
   int vnum;
   int anum;
   char *tilde;

   if( IS_NPC( ch ) )
      return;
   buf[0] = '\0';
   buf1[0] = '\0';

   for( postie = ch->in_room->first_person; postie != NULL; postie = postie->next_in_room )
      if( IS_NPC( postie ) && postie->act.test(ACT_POSTMAN) )
         break;


   if( postie == NULL )
   {
      send_to_char( "But there is no PostMaster here!\r\n", ch );
      return;
   }

   argument = one_argument( argument, arg );
   smash_tilde( argument );

   if( arg[0] == '\0' )
   {
      do_note( ch, "read" );
      return;
   }

   if( !str_cmp( arg, "list" ) )
   {
      vnum = 0;
      buf1[0] = '\0';
      for( li = note_list.begin(); li != note_list.end(); li++ )
      {
         pnote = *li;
         if( is_note_to( ch, pnote ) )
         {
            snprintf( buf, MSL, "[%3d%s] %s: %s\r\n",
                     vnum,
                     ( pnote->date_stamp > ch->pcdata->last_note
                       && str_cmp( pnote->sender, ch->name ) ) ? "N" : " ", pnote->sender, pnote->subject );
            strncat( buf1, buf, MSL );
            vnum++;
         }
      }
      if( vnum == 0 )
         send_to_char( "There are no messages waiting for you.  Sorry!\r\n", ch );
      else
      {
         /*
          * act message 
          */
         send_to_char( buf1, ch );
      }
      return;
   }

   if( !str_cmp( arg, "read" ) )
   {
      bool fAll;

      if( !str_cmp( argument, "all" ) )
      {
         fAll = TRUE;
         anum = 0;
      }
      else if( argument[0] == '\0' || !str_prefix( argument, "next" ) )
         /*
          * read next unread note 
          */
      {
         vnum = 0;
         buf1[0] = '\0';
         for( li = note_list.begin(); li != note_list.end(); li++ )
         {
            pnote = *li;
            if( is_note_to( ch, pnote ) && str_cmp( ch->name, pnote->sender ) && ch->pcdata->last_note < pnote->date_stamp )
            {
               snprintf( buf, MSL, "The letter is postmarked %d:%s\r\n", vnum, pnote->date );
               strncat( buf1, buf, MSL );

               snprintf( buf, MSL, "It bears the mark of %s on the envelope,\r\n", pnote->sender );
               strncat( buf1, buf, MSL );

               snprintf( buf, MSL, "Across the top of the letter it says: %s\r\n", pnote->subject );
               strncat( buf1, buf, MSL );

               strncat( buf1, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n", MSL );

               strncat( buf1, pnote->text, MSL );
               ch->pcdata->last_note = UMAX( ch->pcdata->last_note, pnote->date_stamp );
               send_to_char( buf1, ch );
               return;
            }
            else
               vnum++;
         }
         send_to_char( "You have no unread letters.\r\n", ch );
         return;
      }
      else if( is_number( argument ) )
      {
         fAll = FALSE;
         anum = atoi( argument );
      }
      else
      {
         send_to_char( "Read which letter?\r\n", ch );
         return;
      }

      vnum = 0;
      buf1[0] = '\0';
      for( li = note_list.begin(); li != note_list.end(); li++ )
      {
         pnote = *li;
         if( is_note_to( ch, pnote ) && ( vnum++ == anum || fAll ) )
         {
            snprintf( buf, MSL, "The letter is postmarked %d:%s\r\n", vnum, pnote->date );
            strncat( buf1, buf, MSL );

            snprintf( buf, MSL, "It bears the mark of %s on the envelope,\r\n", pnote->sender );
            strncat( buf1, buf, MSL );

            snprintf( buf, MSL, "Across the top of the letter it says: %s\r\n", pnote->subject );
            strncat( buf1, buf, MSL );

            strncat( buf1, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n", MSL );

            strncat( buf1, pnote->text, MSL );
            if( !fAll )
               send_to_char( buf1, ch );
            else
               strncat( buf1, "\r\n", MSL );
            ch->pcdata->last_note = UMAX( ch->pcdata->last_note, pnote->date_stamp );
            if( !fAll )
               return;
         }
      }

      if( !fAll )
         send_to_char( "No such letter.\r\n", ch );
      else
         send_to_char( buf1, ch );
      return;
   }

   if( !str_cmp( arg, "write" ) || !str_cmp( arg, "edit" ) )
   {
      note_attach( ch );
      build_strdup( &ch->pcdata->pnote->text, "$edit", TRUE, FALSE, ch );
      return;
   }


   if( !str_cmp( arg, "+" ) )
   {
      note_attach( ch );
      strcpy( buf, ch->pcdata->pnote->text );
      if( strlen( buf ) + strlen( argument ) >= MAX_STRING_LENGTH - 200 )
      {
         send_to_char( "Letter too long.\r\n", ch );
         return;
      }

      while( ( tilde = strchr( argument, '~' ) ) != NULL )
      {
         *tilde = '-';
      }

      strncat( buf, argument, MSL-1 );
      strncat( buf, "\r\n", MSL );
      free_string( ch->pcdata->pnote->text );
      ch->pcdata->pnote->text = str_dup( buf );
      send_to_char( "Ok.\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "subject" ) )
   {
      note_attach( ch );
      free_string( ch->pcdata->pnote->subject );
      ch->pcdata->pnote->subject = str_dup( argument );
      send_to_char( "Ok.\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "to" ) )
   {
      note_attach( ch );
      free_string( ch->pcdata->pnote->to_list );
      ch->pcdata->pnote->to_list = str_dup( argument );
      send_to_char( "Ok.\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "clear" ) )
   {
      if( ch->pcdata->pnote )
      {
         delete ch->pcdata->pnote;
         ch->pcdata->pnote = NULL;
      }

      send_to_char( "Ok.\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "show" ) )
   {
      if( !ch->pcdata->pnote )
      {
         send_to_char( "You have no letter in progress.\r\n", ch );
         return;
      }

      snprintf( buf, MSL, "%s: %s\r\nTo: %s\r\n", ch->pcdata->pnote->sender, ch->pcdata->pnote->subject, ch->pcdata->pnote->to_list );
      send_to_char( buf, ch );
      send_to_char( ch->pcdata->pnote->text, ch );
      return;
   }

   if( !str_cmp( arg, "post" ) || !str_prefix( arg, "send" ) )
   {
      FILE *fp;
      char *strtime;

      if( !ch->pcdata->pnote )
      {
         send_to_char( "You have no letter in progress.\r\n", ch );
         return;
      }

      if( !str_cmp( ch->pcdata->pnote->to_list, "" ) )
      {
         send_to_char( "You need to provide recipient name(s).\r\n", ch );
         return;
      }

      if( !str_cmp( ch->pcdata->pnote->subject, "" ) )
      {
         send_to_char( "You need to provide a subject.\r\n", ch );
         return;
      }

      strtime = ctime( &current_time );
      strtime[strlen( strtime ) - 1] = '\0';
      free_string( ch->pcdata->pnote->date );
      ch->pcdata->pnote->date = str_dup( strtime );
      ch->pcdata->pnote->date_stamp = current_time;

      pnote = ch->pcdata->pnote;
      ch->pcdata->pnote = NULL;

      if( !( fp = file_open( NOTE_FILE, "a" ) ) )
      {
         perror( NOTE_FILE );
      }
      else
      {
         fprintf( fp, "Sender  %s~\n", pnote->sender );
         fprintf( fp, "Date    %s~\n", pnote->date );
         fprintf( fp, "Stamp   %ld\n", pnote->date_stamp );
         fprintf( fp, "To      %s~\n", pnote->to_list );
         fprintf( fp, "Subject %s~\n", pnote->subject );
         fprintf( fp, "Text\n%s~\n\n", pnote->text );
      }

      file_close(fp);
      send_to_char( "Ok.\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "remove" ) )
   {
      if( !is_number( argument ) )
      {
         send_to_char( "Remove which letter number?\r\n", ch );
         return;
      }

      anum = atoi( argument );
      vnum = 0;
      for( li = note_list.begin(); li != note_list.end(); li++ )
      {
         pnote = *li;
         if( is_note_to( ch, pnote ) && vnum++ == anum )
         {
            note_remove( ch, pnote );
            send_to_char( "Ok.\r\n", ch );
            return;
         }
      }

      send_to_char( "No such letter.\r\n", ch );
      return;
   }

   send_to_char( "Huh?  Type 'help letter' for usage.\r\n", ch );
   return;
}



/*
 * Generic channel function.
 */

void talk_channel( CHAR_DATA * ch, char *argument, int channel, const char *verb )
{
   char buf[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;
   int position;
   char ansi[MAX_STRING_LENGTH];

   buf[0] = '\0';
   ansi[0] = '\0';

   if( IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) && ( channel != CHANNEL_HOWL ) )
   {
      send_to_char( "You are too @@rENRAGED @@NTo talk to mortals!\r\n", ch );
      return;
   }

   /*
    * Allows immortals to communicate in silent rooms 
    */
   if( ch->in_room->room_flags.test(RFLAG_QUIET) && !IS_IMMORTAL( ch ) )  /* Sssshhh! */
   {
      send_to_char( "Ssshhh!  This is a quiet room!\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      snprintf( buf, MSL, "%s what?\r\n", verb );
      buf[0] = UPPER( buf[0] );
      return;
   }

   ch->deaf.reset(channel);
   if( ch->deaf.test(CHANNEL_HERMIT) )
      send_to_char( "You are hermit right now, and will not hear the response.\r\n ", ch );

   if( !str_cmp(argument,"history") && !IS_NPC(ch) )
   {
    check_chistory(ch,channel);
    return;
   }

   if( !IS_NPC( ch ) && ch->act.test(ACT_SILENCE) )
   {
      snprintf( buf, MSL, "You can't %s.\r\n", verb );
      send_to_char( buf, ch );
      return;
   }

   if( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 10 )
      argument = slur_text( argument );

   switch ( channel )
   {
      default:
         snprintf( buf, MSL, "You %s '%s'.\r\n", verb, argument );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "$n %ss '$t'.", verb );
         break;

      case CHANNEL_GOSSIP:
         snprintf( buf, MSL, "%sYou %s '%s'.%s\r\n", color_string( ch, "gossip" ), verb, argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "$n %ss '$t'.", verb );
         break;

      case CHANNEL_CRUSADE:
         snprintf( buf, MSL, "%sYou %s '%s'.%s\r\n", color_string( ch, "crusade" ), verb, argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "$n %ss '$t'.", verb );
         break;

      case CHANNEL_AUCTION:
         snprintf( buf, MSL, "%sYou %s '%s'.%s\r\n", color_string( ch, "auction" ), verb, argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "$n %ss '$t'.", verb );
         break;

      case CHANNEL_MUSIC:
         snprintf( buf, MSL, "%sYou %s '%s'.%s\r\n", color_string( ch, "music" ), verb, argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "$n %ss '$t'.", verb );
         break;

      case CHANNEL_YELL:
         snprintf( buf, MSL, "%sYou %s '%s'.%s\r\n", color_string( ch, "yell" ), verb, argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "$n %ss '$t'.", verb );
         break;

      case CHANNEL_ADEPT:
         snprintf( buf, MSL, "%s %s: '%s%s%s'.\r\n", verb, ch->name.c_str(), color_string( ch, "adept" ), argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "$n %s: '$t'.", verb );
         break;

      case CHANNEL_FLAME:
         snprintf( buf, MSL, "%sYou %s '%s'.%s\r\n", color_string( ch, "flame" ), verb, argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "$n %ss '$t'.", verb );
         break;

      case CHANNEL_SHOUT:
         snprintf( buf, MSL, "%sYou %s '%s'.%s\r\n", color_string( ch, "shout" ), verb, argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "$n %ss '$t'.", verb );
         break;

      case CHANNEL_CLAN:
         snprintf( buf, MSL, "%s %s: '%s%s%s'.\r\n", verb, ch->name.c_str(), color_string( ch, "clan" ), argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "$n %s: '$t'.", verb );
         break;

      case CHANNEL_FAMILY:
         snprintf( buf, MSL, "%s %s: '%s%s%s'.\r\n", verb, ch->name.c_str(), color_string( ch, "clan" ), argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "$n %s: '$t'.", verb );
         break;

      case CHANNEL_HOWL:
         snprintf( buf, MSL, "%sYou %s '%s'.%s\r\n", color_string( ch, "clan" ), verb, argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "$n %ss '$t'.", verb );
         break;

      case CHANNEL_RACE:
         snprintf( buf, MSL, "%s%s %s: '%s'.%s\r\n", color_string( ch, "race" ), verb, ch->name.c_str(), argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "$n %s: '$t'.", verb );
         break;

      case CHANNEL_CREATOR:
         snprintf( buf, MSL, "%s $n: $t.", verb );
         position = ch->position;
         ch->position = POS_STANDING;
         act( buf, ch, argument, NULL, TO_CHAR );
         ch->position = position;
         break;

      case CHANNEL_IMMTALK:
         snprintf( buf, MSL, "$n: $t." );
         position = ch->position;
         ch->position = POS_STANDING;
         act( buf, ch, argument, NULL, TO_CHAR );
         ch->position = position;
         break;

      case CHANNEL_REMORTTALK:
         snprintf( buf, MSL, "%s %s: '%s%s%s'.\r\n", verb, ch->name.c_str(), color_string( ch, "remort" ), argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "%s $n: $t.", verb );
         break;

      case CHANNEL_DIPLOMAT:
         snprintf( buf, MSL, "%s %s: '%s%s%s'.\r\n", verb, ch->name.c_str(), color_string( ch, "diplomat" ), argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "%s $n: $t.", verb );
         break;

      case CHANNEL_QUEST:
         snprintf( buf, MSL, "%s %s: '%s%s%s'.\r\n", verb, ch->name.c_str(), color_string( ch, "quest" ), argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "%s $n: $t.", verb );
         break;

      case CHANNEL_OOC:
         snprintf( buf, MSL, "%s %s: '%s%s%s'.\r\n", verb, ch->name.c_str(), color_string( ch, "ooc" ), argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "%s $n: $t.", verb );
         break;

      case CHANNEL_GAME:
         snprintf( buf, MSL, "%s %s: '%s%s%s'.\r\n", verb, ch->name.c_str(), color_string( ch, "game" ), argument, color_string( ch, "normal" ) );
         send_to_char( buf, ch );
         snprintf( buf, MSL, "%s $n: $t.", verb );
         break;

   }
   {
      bool has_howled = FALSE;

      for( d = first_desc; d != NULL; d = d->next )
      {
         CHAR_DATA *och;
         CHAR_DATA *vch;

         och = ( d->original ) ? ( d->original ) : ( d->character );
         vch = d->character;

         if( d->connected == CON_PLAYING && vch != ch && !och->deaf.test(channel) && !och->deaf.test(CHANNEL_HERMIT) )
         {
            if( vch->in_room->room_flags.test(RFLAG_QUIET) && !IS_IMMORTAL( ch ) )
               continue;
            if( channel == CHANNEL_CREATOR && get_trust( och ) < MAX_LEVEL )
               continue;
            if( channel == CHANNEL_IMMTALK && !IS_HERO( och ) )
               continue;
            if( channel == CHANNEL_DIPLOMAT && !och->act.test(ACT_CDIPLOMAT) )
               continue;
            if( channel == CHANNEL_REMORTTALK && !IS_REMORT( och ) )
               continue;
            if( channel == CHANNEL_YELL && vch->in_room->area != ch->in_room->area )
               continue;
            if( channel == CHANNEL_ZZZ && vch->position != POS_SLEEPING && och->level != 85 )
               continue;
            if( channel == CHANNEL_RACE && vch->race != ch->race
                && ( och->level != 85 || och->deaf.test(CHANNEL_ALLRACE) ) )
               continue;
            if( channel == CHANNEL_CLAN && och->clan != ch->clan
                && ( och->deaf.test(CHANNEL_ALLCLAN) || get_trust( och ) != MAX_LEVEL ) )
               continue;
            if( ( channel == CHANNEL_FAMILY )
                && ( ( !IS_VAMP( och ) || !IS_VAMP( ch ) )
                     || ( och->pcdata->super->bloodline != ch->pcdata->super->bloodline ) ) )
               continue;
            if( channel == CHANNEL_ADEPT && !IS_ADEPT(vch) )
               continue;
            if( ( channel == CHANNEL_HOWL ) && ( !IS_WOLF( och ) ) )
            {
               if( och->in_room->area != ch->in_room->area )
                  continue;
               else
               {
                  if( och->in_room == ch->in_room )
                  {
                     if( !has_howled )
                     {
                        check_social( ch, "howl", "" );
                        has_howled = TRUE;
                     }
                  }
                  else
                     send_to_char( "@@rAn eerie @@bHowl@@r sends chills up your spine.@@N\r\n", och );
                  continue;
               }
            }

            position = vch->position;
            if( channel != CHANNEL_SHOUT && channel != CHANNEL_YELL )
               vch->position = POS_STANDING;

            switch ( channel )
            {
               default:
                  snprintf( ansi, MSL, "%s", buf );
                  break;
               case CHANNEL_SHOUT:
                  snprintf( ansi, MSL, "%s%s%s", color_string( vch, "shout" ), buf, color_string( vch, "normal" ) );
                  break;
               case CHANNEL_RACE:
                  snprintf( ansi, MSL, "%s%s%s", color_string( vch, "race" ), buf, color_string( vch, "normal" ) );
                  break;
               case CHANNEL_MUSIC:
                  snprintf( ansi, MSL, "%s%s%s", color_string( vch, "music" ), buf, color_string( vch, "normal" ) );
                  break;
               case CHANNEL_CLAN:
                  snprintf( ansi, MSL, "%s%s%s", color_string( vch, "clan" ), buf, color_string( vch, "normal" ) );
                  break;
               case CHANNEL_FLAME:
                  snprintf( ansi, MSL, "%s%s%s", color_string( vch, "flame" ), buf, color_string( vch, "normal" ) );
                  break;
               case CHANNEL_YELL:
                  snprintf( ansi, MSL, "%s%s%s", color_string( vch, "yell" ), buf, color_string( vch, "normal" ) );
                  break;
               case CHANNEL_GOSSIP:
                  snprintf( ansi, MSL, "%s%s%s", color_string( vch, "gossip" ), buf, color_string( vch, "normal" ) );
                  break;
               case CHANNEL_OOC:
                  snprintf( ansi, MSL, "%s%s%s", color_string( vch, "gossip" ), buf, color_string( vch, "normal" ) );
                  break;
               case CHANNEL_CRUSADE:
                  snprintf( ansi, MSL, "%s%s%s", color_string( vch, "gossip" ), buf, color_string( vch, "normal" ) );
                  break;

            } //switch
            act( ansi, ch, IS_GHOST(ch) ? IS_GHOST(vch) ? argument : ghost_text(argument) : argument, vch, TO_VICT );
            vch->position = position;
         } //if connected
      } //d loop

      update_chistory(ch,argument,channel);

      return;
   } //no reason
}

DO_FUN(do_creator)
{
   talk_channel( ch, argument, CHANNEL_CREATOR, "[C-Net]" );
   return;
}


DO_FUN(do_gossip)
{
   talk_channel( ch, argument, CHANNEL_GOSSIP, "gossip" );
   return;
}

DO_FUN(do_crusade)
{
   talk_channel( ch, argument, CHANNEL_CRUSADE, "@@lcrusade" );
   if( ( !str_cmp( argument, "what mob?" ) )
       || ( !str_cmp( argument, "who was the thief?" ) )
       || ( !str_cmp( argument, "who is the thief?" ) )
       || ( !str_cmp( argument, "what item?" ) )
       || ( !str_cmp( argument, "where are you?" ) )
       || ( !str_cmp( argument, "who stole the item?" ) ) || ( !str_cmp( argument, "where is the thief?" ) ) )
      ask_quest_question( ch, argument );

   return;
}


DO_FUN(do_music)
{
   talk_channel( ch, argument, CHANNEL_MUSIC, "music" );
   return;
}

DO_FUN(do_quest)
{
   talk_channel( ch, argument, CHANNEL_QUEST, "@@a[@@cQUEST@@a]@@W" );
   return;
}

DO_FUN(do_game)
{
   talk_channel( ch, argument, CHANNEL_GAME, "@@y[@@rGAME@@y]@@W" );
   return;
}

DO_FUN(do_race)
{
   char buf[MAX_STRING_LENGTH];

   if( IS_NPC( ch ) )
   {
      send_to_char( "NPCs cannot use this channel.\r\n", ch );
      return;
   }

   snprintf( buf, MSL, "[  %s  ]", race_table[ch->race].race_name );
   talk_channel( ch, argument, CHANNEL_RACE, buf );
   return;
}

DO_FUN(do_clan)
{
   char buf[MAX_STRING_LENGTH];

   if( IS_NPC( ch ) )
   {
      send_to_char( "NPCs cannot use this channel.\r\n", ch );
      return;
   }

   if( ch->clan == 0 )
   {
      send_to_char( "Only players in clans may use this channel.\r\n", ch );
      return;
   }

   snprintf( buf, MSL, "[%s]", clan_table[ch->clan].clan_abbr );
   talk_channel( ch, argument, CHANNEL_CLAN, buf );
   return;
}

DO_FUN(do_familytalk)
{
   char buf[MAX_STRING_LENGTH];

   if( IS_NPC( ch ) )
   {
      send_to_char( "NPCs cannot use this channel.\r\n", ch );
      return;
   }

   if( !IS_VAMP( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   snprintf( buf, MSL, "[%s]", get_family_name( ch ) );
   talk_channel( ch, argument, CHANNEL_FAMILY, buf );
   return;
}

DO_FUN(do_howl)
{
   /*
    * char buf[MAX_STRING_LENGTH];    
    */

   if( IS_NPC( ch ) )
   {
      send_to_char( "NPCs cannot use this channel.\r\n", ch );
      return;
   }

   if( !IS_WOLF( ch ) )
   {
      check_social( ch, "howl", "" );
      return;
   }

   /*
    * snprintf( buf, "[%s]", get_family_name ( ch ) );  
    */
   talk_channel( ch, argument, CHANNEL_HOWL, "@@bHowl@@N" );
   return;
}

DO_FUN(do_newbie)
{
   talk_channel( ch, argument, CHANNEL_NEWBIE, "newbie" );
   return;
}

DO_FUN(do_question)
{
   talk_channel( ch, argument, CHANNEL_QUESTION, "question" );
   return;
}

DO_FUN(do_answer)
{
   talk_channel( ch, argument, CHANNEL_QUESTION, "answer" );
   return;
}

DO_FUN(do_shout)
{
   talk_channel( ch, argument, CHANNEL_SHOUT, "shout" );
   return;
}

/*
 * Flame and zzz added by Stephen
 * zzz will only be available to sleeping chars ;)
 */

DO_FUN(do_flame)
{
   if( ch->level < 3 )
   {
      send_to_char( "You must be level 3 to use this channel.\r\n", ch );
      return;
   }

   talk_channel( ch, argument, CHANNEL_FLAME, "flame" );
   return;
}

DO_FUN(do_zzz)
{
   if( ch->level < 3 )
   {
      send_to_char( "You must be level 3 to use this channel.\r\n", ch );
      if( !( ch->position == POS_SLEEPING ) )
      {
         send_to_char( "And you have to be asleep anyway!!!\r\n", ch );
      }
      return;
   }

   /*
    * check is asleep, if not then say not when awake, and return 
    */
   if( !( ch->position == POS_SLEEPING ) )
   {
      send_to_char( "You can only use this channel when asleep!\r\n", ch );
      return;
   }
   talk_channel( ch, argument, CHANNEL_ZZZ, "zzz" );
   return;
}

DO_FUN(do_yell)
{
   talk_channel( ch, argument, CHANNEL_YELL, "yell" );
   return;
}

DO_FUN(do_immtalk)
{
   talk_channel( ch, argument, CHANNEL_IMMTALK, "immtalk" );
   return;
}

DO_FUN(do_diptalk)
{
   if( !ch->act.test(ACT_CDIPLOMAT) )
   {
      send_to_char( "You are not a clan diplomat.\r\n", ch );
      return;
   }
   talk_channel( ch, argument, CHANNEL_DIPLOMAT, "@@lNEGOTIATE@@N" );
   return;
}

DO_FUN(do_remorttalk)
{
   if( !IS_REMORT( ch ) )
   {
      send_to_char( "You are not a @@mREMORT@@N!!!\r\n", ch );
      return;
   }
   talk_channel( ch, argument, CHANNEL_REMORTTALK, "[@@mREMORT@@N]" );
   return;
}

DO_FUN(do_ooc)
{
   talk_channel( ch, argument, CHANNEL_OOC, "[OOC]" );
   return;
}

DO_FUN(do_adepttalk)
{
   if( !IS_ADEPT(ch) )
   {
      send_to_char( "You are not @@cADEPT!!@@N\r\n", ch );
      return;
   }

   talk_channel( ch, argument, CHANNEL_ADEPT, "[@@lADEPT@@N]" );
   return;

}

DO_FUN(do_say)
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *ppl;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You are too @@rENRAGED @@NTo talk to mortals!\r\n", ch );
      return;
   }

   if( ch->in_room->room_flags.test(RFLAG_QUIET) && !IS_IMMORTAL( ch ) )
   {
      send_to_char( "Sssshhhh! This is a quiet room!\r\n", ch );
      return;
   }


   if( argument[0] == '\0' )
   {
      send_to_char( "Say what?\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 10 )
      argument = slur_text( argument );

   snprintf( buf, MSL, "You say '%s$T%s'.", color_string( ch, "say" ), color_string( ch, "normal" ) );
   act( buf, ch, NULL, argument, TO_CHAR );
   for( ppl = ch->in_room->first_person; ppl != NULL; ppl = ppl->next_in_room )
   {
      snprintf( buf, MSL, "$n says '%s$t%s'.", color_string( ppl, "say" ), color_string( ppl, "normal" ) );
      act( buf, ch, IS_GHOST(ch) ? IS_GHOST(ppl) ? argument : ghost_text(argument) : argument, ppl, TO_VICT );
   }
   return;
}

/* added by Eligan, July 10 */

DO_FUN(do_ignore)
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   char *arg[3];
   int i;


   if( IS_NPC( ch ) )
   {
      send_to_char( "NOT for NPCs!\r\n", ch );
      return;
   }



   if( argument[0] == '\0' )
   {
      send_to_char( "\n@@ySyntax@@g: ignore <victim>\n\r\n", ch );
      send_to_char( "Current people to be ignored:\r\n ", ch );
      for( i = 0; i < 3; i++ )
      {
         if( ch->pcdata->ignore_list[i] != NULL )
         {
            snprintf( buf, MSL, "  %d) @@R%s@@g\r\n", i + 1, ch->pcdata->ignore_list[i] );
            send_to_char( buf, ch );
         }
         else
         {
            snprintf( buf, MSL, "  %d) @@Rnobody@@g\r\n", i + 1 );
            send_to_char( buf, ch );
         }
      }
      return;
   }


   if( !str_cmp( argument, "self" ) )
   {
      send_to_char( "Why would you want to do that?\r\n", ch );
      return;
   }


   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }
   if( IS_NPC( victim ) )
   {
      send_to_char( "You can only ignore players!!\r\n", ch );
      return;
   }

   if( IS_IMMORTAL( victim ) )
   {
      send_to_char( "You cannot ignore immortals!\r\n", ch );
      return;
   }
/*    
    if (  argument[0] == '\0' )
    {
        send_to_char( "\n@@ySyntax@@g: ignore <victim>\n\r\n", ch );
        send_to_char( "Current people to be ignored:\r\n ", ch );
        for( i=0 ; i<3 ; i++ )
        {
            if( ch->pcdata->ignore_list[i] != NULL )
            {
                snprintf(buf, MSL, "  %d) @@R%s@@g\r\n",i+1,ch->pcdata->ignore_list[i]);
                send_to_char( buf, ch );
            }
            else
            {
		snprintf( buf, MSL, "  %d) @@Rnobody@@g\r\n", i+1 );
                send_to_char( buf, ch );
            }
        }
        return;
    }
*/
   /*
    * else 
    *//*
    * it has an argument 
    */
/*    {    */
   for( i = 0; i < 3; i++ )
   {
      if( ch->pcdata->ignore_list[i] == NULL )
      {
         ch->pcdata->ignore_list[i] = str_dup( "nobody" );
      }
      arg[i] = str_dup( ch->pcdata->ignore_list[i] );
   }

   if( !str_cmp( victim->name, arg[0] ) || !str_cmp( victim->name, arg[1] ) || !str_cmp( victim->name, arg[2] ) )
   {  /* if already on the list, remove them */
      i = 0;
      while( str_cmp( victim->name, arg[i] ) )
      {
         i++;
      }
      while( i < 2 )
      {
         free_string( ch->pcdata->ignore_list[i] );
         ch->pcdata->ignore_list[i] = str_dup( arg[i + 1] );
         i++;
      }
      free_string( ch->pcdata->ignore_list[2] );
      ch->pcdata->ignore_list[2] = str_dup( "nobody" );
   }
   else  /* if not on list already */
   {
      if( !( !str_cmp( arg[0], "nobody" ) || !str_cmp( arg[1], "nobody" ) || !str_cmp( arg[2], "nobody" ) ) )
      {
         send_to_char( "Too many names, remove one first.\r\n", ch );
         for( i = 0; i < 3; i++ )
            free_string( arg[i] );
         return;
      }
      for( i = 0; i < 3; i++ )
      {
         if( !str_cmp( arg[i], "nobody" ) )
         {
            free_string( ch->pcdata->ignore_list[i] );
            ch->pcdata->ignore_list[i] = str_dup( argument );
            break;
         }
      }
   }
   send_to_char( "\nCurrent person to be ignored:\r\n", ch );
   for( i = 0; i < 3; i++ )
   {
      snprintf( buf, MSL, "  %d) @@R%s@@g\r\n", i + 1, ch->pcdata->ignore_list[i] );
      send_to_char( buf, ch );
   }
   for( i = 0; i < 3; i++ )
      free_string( arg[i] );

   return;
/*    }  */
}

DO_FUN(do_tell)
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int position;

   if( ch->in_room->vnum == ROOM_VNUM_JAIL && !IS_IMMORTAL( ch ) )
   {
      send_to_char( "You cannot send tells from jail.\r\n", ch );
      return;
   }

   if( IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You are too @@rENRAGED @@NTo talk to mortals!\r\n", ch );
      return;
   }

   if( ch->act.test(ACT_AFK) )
   {
      ch->act.reset(ACT_AFK);
      act( "You are no longer AFK.", ch, NULL, NULL, TO_CHAR );
   }

   if( ch->act.test(ACT_SILENCE) )
   {
      send_to_char( "Your message didn't get through.\r\n", ch );
      return;
   }

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "Tell whom what?\r\n", ch );
      return;
   }

   /*
    * See if character is playing and visible to ch... if victim is
    * * an immortal they are told that a player is trying to talk to them.
    * * -- Stephen
    */

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }
   if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( IS_SHIFTED( victim ) || IS_RAGED( victim ) ) && !IS_IMMORTAL(ch) )
   {
      send_to_char( "They can't hear you.\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) && !victim->desc )
   {
      act( "Sorry, but $N is currently link dead.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( victim->act.test(ACT_AFK) && !IS_IMMORTAL(ch) )
   {
      act( "$N is currently away from keyboard.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( victim->in_room->room_flags.test(RFLAG_QUIET) && !IS_IMMORTAL( ch ) )
   {
      act( "$N is in a quiet room, $E can't hear you.", ch, NULL, victim, TO_CHAR );
      return;
   }


   if( ( !IS_NPC( victim ) )
       && ( !str_cmp( victim->pcdata->ignore_list[0], ch->name ) ||
            !str_cmp( victim->pcdata->ignore_list[1], ch->name ) || !str_cmp( victim->pcdata->ignore_list[2], ch->name ) ) && !IS_IMMORTAL(ch) )
   {
      snprintf( buf, MSL, "%s @@Ris ignoring you!!@@g\r\n", victim->name.c_str() );
      send_to_char( buf, ch );
      return;
   }

/*    if( victim->pcdata->ignore_list == ch->name ){ */
/*    if( !str_cmp(victim->pcdata->ignore_list, ch->name) ){
	send_to_char( "They are ignoring you\r\n", ch);
	return;
    }

             if( victim->pcdata->ignore_list != NULL ){ *//*
    * { == ch->name ){
    */
/*	snprintf( buf, MSL, "%s, %s\r\n", victim->pcdata->ignore_list, ch->name );
	send_to_char( buf, ch);
	return;
    }*/

   if( !IS_IMMORTAL( ch ) && !IS_AWAKE( victim ) )
   {
      act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
      return;
   }

   if( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 10 )
      argument = slur_text( argument );

   snprintf( buf, MSL, "You tell $N '%s$t%s'.", color_string( ch, "tell" ), color_string( ch, "normal" ) );
   act( buf, ch, argument, victim, TO_CHAR );

   position = victim->position;
   victim->position = POS_STANDING;
   snprintf( buf, MSL, "$n tells you '%s$t%s'.", color_string( victim, "tell" ), color_string( victim, "normal" ) );

   act( buf, ch, argument, victim, TO_VICT );
   victim->position = position;
   victim->reply = ch;

   return;
}

DO_FUN(do_reply)
{
   CHAR_DATA *victim;
   int position;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You are too @@rENRAGED @@NTo talk to mortals!\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && ch->act.test(ACT_SILENCE) )
   {
      send_to_char( "Your message didn't get through.\r\n", ch );
      return;
   }

   if( ( victim = ch->reply ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }
   if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( IS_SHIFTED( victim ) || IS_RAGED( victim ) ) )
   {
      send_to_char( "They can't hear you.\r\n", ch );
      return;
   }


   if( victim->in_room->room_flags.test(RFLAG_QUIET) && !IS_IMMORTAL( ch ) )
   {
      act( "$N is in a quiet room.  $E can't hear you.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( !IS_IMMORTAL( ch ) && !IS_AWAKE( victim ) )
   {
      act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
      return;
   }


   if( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 10 )
      argument = slur_text( argument );

   act( "You tell $N '$t'.", ch, argument, victim, TO_CHAR );
   position = victim->position;
   victim->position = POS_STANDING;
   act( "$n tells you '$t'.", ch, argument, victim, TO_VICT );
   victim->position = position;
   victim->reply = ch;

   return;
}

DO_FUN(do_emote)
{
   char buf[MAX_STRING_LENGTH];
   char *plast;
   buf[0] = '\0';

   if( !IS_NPC( ch ) && ch->act.test(ACT_NO_EMOTE) )
   {
      send_to_char( "You can't show your emotions.\r\n", ch );
      return;
   }

   if( ch->in_room->room_flags.test(RFLAG_QUIET) )
   {
      send_to_char( "You can't show your emotions in a quiet room!\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "Emote what?\r\n", ch );
      return;
   }

   for( plast = argument; *plast != '\0'; plast++ )
      ;

   strcpy( buf, argument );
   if( isalpha( plast[-1] ) )
      strncat( buf, ".", MSL );

   act( "$n $T", ch, NULL, buf, TO_ROOM );
   act( "$n $T", ch, NULL, buf, TO_CHAR );
   return;
}



/*
 * All the posing stuff.
 */
struct pose_table_type
{
/* MAG Mod */
   /*
    * OLD  char *      message[2*MAX_CLASS]; 
    */
   /*
    * New 
    */
   char *message[8];
};

const struct pose_table_type pose_table[] = {
   {
    {
     "You sizzle with energy.",
     "$n sizzles with energy.",
     "You feel very holy.",
     "$n looks very holy.",
     "You perform a small card trick.",
     "$n performs a small card trick.",
     "You show your bulging muscles.",
     "$n shows $s bulging muscles."}
    },

   {
    {
     "You turn into a butterfly, then return to your normal shape.",
     "$n turns into a butterfly, then returns to $s normal shape.",
     "You nonchalantly turn wine into water.",
     "$n nonchalantly turns wine into water.",
     "You wiggle your ears alternately.",
     "$n wiggles $s ears alternately.",
     "You crack nuts between your fingers.",
     "$n cracks nuts between $s fingers."}
    },

   {
    {
     "Blue sparks fly from your fingers.",
     "Blue sparks fly from $n's fingers.",
     "A halo appears over your head.",
     "A halo appears over $n's head.",
     "You nimbly tie yourself into a knot.",
     "$n nimbly ties $mself into a knot.",
     "You grizzle your teeth and look mean.",
     "$n grizzles $s teeth and looks mean."}
    },

   {
    {
     "Little red lights dance in your eyes.",
     "Little red lights dance in $n's eyes.",
     "You recite words of wisdom.",
     "$n recites words of wisdom.",
     "You juggle with daggers, apples, and eyeballs.",
     "$n juggles with daggers, apples, and eyeballs.",
     "You hit your head, and your eyes roll.",
     "$n hits $s head, and $s eyes roll."}
    },

   {
    {
     "A slimy green monster appears before you and bows.",
     "A slimy green monster appears before $n and bows.",
     "Deep in prayer, you levitate.",
     "Deep in prayer, $n levitates.",
     "You steal the underwear off every person in the room.",
     "Your underwear is gone!  $n stole it!",
     "Crunch, crunch -- you munch a bottle.",
     "Crunch, crunch -- $n munches a bottle."}
    },

   {
    {
     "You turn everybody into a little pink elephant.",
     "You are turned into a little pink elephant by $n.",
     "An angel consults you.",
     "An angel consults $n.",
     "The dice roll ... and you win again.",
     "The dice roll ... and $n wins again.",
     "... 98, 99, 100 ... you do pushups.",
     "... 98, 99, 100 ... $n does pushups."}
    },

   {
    {
     "A small ball of light dances on your fingertips.",
     "A small ball of light dances on $n's fingertips.",
     "Your body glows with an unearthly light.",
     "$n's body glows with an unearthly light.",
     "You count the money in everyone's pockets.",
     "Check your money, $n is counting it.",
     "Arnold Schwarzenegger admires your physique.",
     "Arnold Schwarzenegger admires $n's physique."}
    },

   {
    {
     "Smoke and fumes leak from your nostrils.",
     "Smoke and fumes leak from $n's nostrils.",
     "A spot light hits you.",
     "A spot light hits $n.",
     "You balance a pocket knife on your tongue.",
     "$n balances a pocket knife on your tongue.",
     "Watch your feet, you are juggling granite boulders.",
     "Watch your feet, $n is juggling granite boulders."}
    },

   {
    {
     "The light flickers as you rap in magical languages.",
     "The light flickers as $n raps in magical languages.",
     "Everyone levitates as you pray.",
     "You levitate as $n prays.",
     "You produce a coin from everyone's ear.",
     "$n produces a coin from your ear.",
     "Oomph!  You squeeze water out of a granite boulder.",
     "Oomph!  $n squeezes water out of a granite boulder."}
    },

   {
    {
     "Your head disappears.",
     "$n's head disappears.",
     "A cool breeze refreshes you.",
     "A cool breeze refreshes $n.",
     "You step behind your shadow.",
     "$n steps behind $s shadow.",
     "You pick your teeth with a spear.",
     "$n picks $s teeth with a spear."}
    },

   {
    {
     "A fire elemental singes your hair.",
     "A fire elemental singes $n's hair.",
     "The sun pierces through the clouds to illuminate you.",
     "The sun pierces through the clouds to illuminate $n.",
     "Your eyes dance with greed.",
     "$n's eyes dance with greed.",
     "Everyone is swept off their foot by your hug.",
     "You are swept off your feet by $n's hug."}
    },

   {
    {
     "The sky changes color to match your eyes.",
     "The sky changes color to match $n's eyes.",
     "The ocean parts before you.",
     "The ocean parts before $n.",
     "You deftly steal everyone's weapon.",
     "$n deftly steals your weapon.",
     "Your karate chop splits a tree.",
     "$n's karate chop splits a tree."}
    },

   {
    {
     "The stones dance to your command.",
     "The stones dance to $n's command.",
     "A thunder cloud kneels to you.",
     "A thunder cloud kneels to $n.",
     "The Grey Mouser buys you a beer.",
     "The Grey Mouser buys $n a beer.",
     "A strap of your armor breaks over your mighty thews.",
     "A strap of $n's armor breaks over $s mighty thews."}
    },

   {
    {
     "The heavens and grass change color as you smile.",
     "The heavens and grass change color as $n smiles.",
     "The Burning Man speaks to you.",
     "The Burning Man speaks to $n.",
     "Everyone's pocket explodes with your fireworks.",
     "Your pocket explodes with $n's fireworks.",
     "A boulder cracks at your frown.",
     "A boulder cracks at $n's frown."}
    },

   {
    {
     "Everyone's clothes are transparent, and you are laughing.",
     "Your clothes are transparent, and $n is laughing.",
     "An eye in a pyramid winks at you.",
     "An eye in a pyramid winks at $n.",
     "Everyone discovers your dagger a centimeter from their eye.",
     "You discover $n's dagger a centimeter from your eye.",
     "Mercenaries arrive to do your bidding.",
     "Mercenaries arrive to do $n's bidding."}
    },

   {
    {
     "A black hole swallows you.",
     "A black hole swallows $n.",
     "Valentine Michael Smith offers you a glass of water.",
     "Valentine Michael Smith offers $n a glass of water.",
     "Where did you go?",
     "Where did $n go?",
     "Four matched Percherons bring in your chariot.",
     "Four matched Percherons bring in $n's chariot."}
    },

   {
    {
     "The world shimmers in time with your whistling.",
     "The world shimmers in time with $n's whistling.",
     "The great god Mota dances on your soul.",
     "The great god Mota dances on $n's soul.",
     "Click.",
     "Click.",
     "Atlas asks you to relieve him.",
     "Atlas asks $n to relieve him."}
    }
};

DO_FUN(do_pose)
{
   short level;
   short pose;

   if( IS_NPC( ch ) )
      return;

   if( ch->in_room->room_flags.test(RFLAG_QUIET) )
   {
      send_to_char( "You can't pose in a quiet room!\r\n", ch );
      return;
   }

   level = UMIN( ch->level, ( (short)sizeof( pose_table ) / (short)sizeof( pose_table[0] ) - 1 ) );
   pose = number_range( 0, level );

   act( pose_table[pose].message[2 * ch->p_class + 0], ch, NULL, NULL, TO_CHAR );
   act( pose_table[pose].message[2 * ch->p_class + 1], ch, NULL, NULL, TO_ROOM );

   return;
}

DO_FUN(do_bug)
{
   append_file( ch, BUG_FILE, argument );
   send_to_char( "Ok.  Thanks.\r\n", ch );
   return;
}

DO_FUN(do_idea)
{
   append_file( ch, IDEA_FILE, argument );
   send_to_char( "Ok.  Thanks.\r\n", ch );
   return;
}

DO_FUN(do_typo)
{
   append_file( ch, TYPO_FILE, argument );
   send_to_char( "Ok.  Thanks.\r\n", ch );
   return;
}

DO_FUN(do_qui)
{
   send_to_char( "If you want to QUIT, you have to spell it out.\r\n", ch );
   return;
}

DO_FUN(do_quit)
{
   DESCRIPTOR_DATA *d;
   DESCRIPTOR_DATA *other_logins;
   DESCRIPTOR_DATA *other_logins_next;

   if( IS_NPC( ch ) )
      return;

   if( ch->position == POS_FIGHTING )
   {
      send_to_char( "No way! You are fighting.\r\n", ch );
      return;
   }

   if( ch->position < POS_STUNNED )
   {
      send_to_char( "You're not DEAD yet.\r\n", ch );
      return;
   }

   /*
    * Quit message changed to cheesy bye text ;)    
    * * -- Stephen
    */

   send_to_char( "\r\n", ch );
   act( "      @@WFarewell, $n.  Return safe to these realms.", ch, NULL, NULL, TO_CHAR );
   send_to_char( "\r\n", ch );
   send_to_char( "@@y'If both the past and the external world exist only in the mind,\r\n", ch );
   send_to_char( "and if the mind itself is controllable, what then?'\r\n", ch );
   send_to_char( "@@W-- George Orwell, '1984'@@g\r\n", ch );

   act( "$n waves, and leaves the game.", ch, NULL, NULL, TO_ROOM );
   snprintf( log_buf, (2 * MIL), "%s quits ACK!", ch->name.c_str() );
   if( ch->level != 85 )
      notify( log_buf, MAX_LEVEL - 2 );
   log_string( log_buf );

   /*
    * After extract_char the ch is no longer valid!
    */
   d = ch->desc;

   for( other_logins = first_desc; other_logins != NULL; other_logins = other_logins_next )
   {
      other_logins_next = other_logins->next;

      if( ( other_logins != d )
          && ( other_logins->character != NULL )
          && ( other_logins->connected != CON_RECONNECTING ) && ( other_logins->character->name == ch->name ) )
      {
         if( other_logins->connected == CON_GET_OLD_PASSWORD )
         {
            char logbuf[MSL];
            snprintf( logbuf, MSL, "CHEATER!!! Possible attempt to utilize eq dup bug, %s", other_logins->character->name.c_str() );

            log_string( logbuf );
         }
         close_socket( other_logins );
      }
   }

   save_char_obj( ch );
   ch->is_quitting = true;

   extract_char( ch, TRUE );
   if( d != NULL )
      close_socket( d );

   return;
}

DO_FUN(do_save)
{
   char buf[MAX_STRING_LENGTH];

   if( IS_NPC( ch ) )
   {
      send_to_char( "NPCs are not able to save (at the moment?)\r\n", ch );
      return;
   }

   save_char_obj( ch );
   snprintf( buf, MSL, "Saving %s.\r\n", ch->name.c_str() );
   if( str_cmp(argument,"auto") )
    send_to_char( buf, ch );
   return;
}

DO_FUN(do_follow)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Follow whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_AFFECTED( ch, AFF_CHARM ) && ch->master != NULL )
   {
      act( "But you'd rather follow $N!", ch, NULL, ch->master, TO_CHAR );
      return;
   }

   if( victim == ch )
   {
      if( ch->master == NULL )
      {
         send_to_char( "You already follow yourself.\r\n", ch );
         return;
      }
      stop_follower( ch );
      return;
   }

   if( IS_RIDING( ch ) && is_same_group( ch, ch->riding ) )
      do_group( ch, const_cast<char *>(ch->riding->name.c_str()) );

   if( ch->master != NULL )
      stop_follower( ch );

   add_follower( ch, victim );
   return;
}



void add_follower( CHAR_DATA * ch, CHAR_DATA * master )
{

   if( ch->master != NULL )
   {
      bug( "Add_follower: non-null master.", 0 );
      return;
   }

   if( IS_NPC( ch ) && !IS_NPC( master ) )
   {
      short max_orders;

      max_orders = ( get_curr_int( master ) / 5 );
      if( ( !IS_NPC( master ) ) && ( master->pcdata->learned[gsn_unit_tactics] > 10 ) )
         max_orders += master->pcdata->learned[gsn_unit_tactics] / 28;
      if( max_orders <= master->num_followers )
      {
         send_to_char( "You cannot control anymore followers.\r\n", master );
         ch->npcdata->extract_timer = 1;
         return;
      }
      else
      {
         master->num_followers++;
      }
   }
   ch->master = master;
   ch->leader = NULL;

   if( can_see( master, ch ) )
      act( "$n starts following you.", ch, NULL, master, TO_VICT );

   act( "You start following $N.", ch, NULL, master, TO_CHAR );
   return;
}



void stop_follower( CHAR_DATA * ch )
{

   if( ch->master == NULL )
   {
      bug( "Stop_follower: null master.", 0 );
      return;
   }

   if( IS_AFFECTED( ch, AFF_CHARM ) )
   {
      REMOVE_BIT( ch->affected_by, AFF_CHARM );
      affect_strip( ch, gsn_charm_person );
   }

   if( can_see( ch->master, ch ) )
      act( "$n stops following you.", ch, NULL, ch->master, TO_VICT );
   act( "You stop following $N.", ch, NULL, ch->master, TO_CHAR );
   ch->master->num_followers--;
   ch->master = NULL;
   ch->leader = NULL;
   return;
}



void die_follower( CHAR_DATA * ch )
{
   CHAR_DATA *fch;
   list<CHAR_DATA *>::iterator li;

   if( ch->master != NULL )
      stop_follower( ch );

   ch->leader = NULL;

   for( li = char_list.begin(); li!= char_list.end(); li++ )
   {
      fch = *li;
      if( fch->master == ch )
         stop_follower( fch );
      if( fch->leader == ch )
         fch->leader = fch;
   }

   return;
}

DO_FUN(do_order)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   CHAR_DATA *och;
   CHAR_DATA *och_next;
   bool found;
   bool fAll;
   short num_followers = 0, max_orders = 0;


   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "Order whom to do what?\r\n", ch );
      return;
   }

   if( IS_AFFECTED( ch, AFF_CHARM ) )
   {
      send_to_char( "You feel like taking, not giving, orders.\r\n", ch );
      return;
   }
   if( !str_cmp( arg, "all" ) )
   {
      fAll = TRUE;
      victim = NULL;
   }
   else
   {
      fAll = FALSE;
      if( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\r\n", ch );
         return;
      }

      if( victim == ch )
      {
         send_to_char( "Aye aye, right away!\r\n", ch );
         return;
      }

      if( !IS_AFFECTED( victim, AFF_CHARM ) || victim->master != ch )
      {
         send_to_char( "Do it yourself!\r\n", ch );
         return;
      }
   }
   one_argument( argument, arg );

   if( ( !str_prefix( arg, "murder" ) )
       || ( !str_prefix( arg, "punch" ) )
       || ( !str_prefix( arg, "kick" ) )
       || ( !str_prefix( arg, "frenzy" ) )
       || ( !str_prefix( arg, "backstab" ) )
       || ( !str_prefix( arg, "circle" ) )
       || ( !str_prefix( arg, "dirt" ) )
       || ( !str_prefix( arg, "headbutt" ) )
       || ( !str_prefix( arg, "knee" ) )
       || ( !str_prefix( arg, "trip" ) )
       || ( !str_prefix( arg, "smash" ) )
       || ( !str_prefix( arg, "stake" ) )
       || ( !str_prefix( arg, "kill" ) )
       || ( !str_prefix( arg, "assassinate" ) )
       || ( !str_prefix( arg, "stun" ) )
       || ( !str_prefix( arg, "loot" ) )
       || ( !str_prefix( arg, "bs" ) )
       || ( !str_prefix( arg, "charge" ) )
       || ( !str_prefix( arg, "disarm" ) )
       || ( !str_prefix( arg, "steal" ) ) || ( !str_prefix( arg, "target" ) ) || ( !str_prefix( arg, "bash" ) ) )



   {
      send_to_char( "I won't do that, it's @@eEVIL!!!@@N\r\n", ch );
      return;
   }

   if( ( !str_prefix( arg, "recall" ) )
       || ( !str_prefix( arg, "/" ) )
       || ( !str_prefix( arg, "beep" ) )
       || ( !str_prefix( arg, "tell" ) )
       || ( !str_prefix( arg, "ooc" ) )
       || ( !str_prefix( arg, "shout" ) )
       || ( !str_prefix( arg, "flame" ) )
       || ( !str_prefix( arg, "home" ) )
       || ( !str_prefix( arg, "music" ) )
       || ( !str_prefix( arg, "." ) )
       || ( !str_prefix( arg, "crusade" ) )
       || ( !str_prefix( arg, "{" ) )
       || ( !str_prefix( arg, "answer" ) )
       || ( !str_prefix( arg, "diplomat" ) )
       || ( !str_prefix( arg, "enter" ) )
       || ( !str_prefix( arg, "vampire" ) )
       || ( !str_prefix( arg, "newbie" ) )
       || ( !str_prefix( arg, "adept" ) )
       || ( !str_prefix( arg, "zzz" ) )
       || ( !str_prefix( arg, "quest" ) )
       || ( !str_prefix( arg, "disguise" ) )
       || ( !str_prefix( arg, "question" ) )
       || ( !str_prefix( arg, "game" ) )
       || ( !str_prefix( arg, "ooc" ) ) || ( !str_prefix( arg, "auction" ) ) || ( !str_prefix( arg, "bid" ) ) )



   {
      send_to_char( "I won't do that, it's @@eNOT NICE!!!@@N\r\n", ch );
      return;
   }

   if( ( fAll != TRUE ) && ( victim->master == ch ) )
   {
      act( "$n orders you to '$t'.", ch, argument, victim, TO_VICT );
      interpret( victim, argument );
      send_to_char( "Ok.\r\n", ch );
      return;
   }

   found = FALSE;
   max_orders = ( get_curr_int( ch ) / 5 );
   if( !IS_NPC( ch ) && ( ch->pcdata->learned[gsn_unit_tactics] > 10 ) )
      max_orders += ch->pcdata->learned[gsn_unit_tactics] / 28;

   for( och = ch->in_room->first_person; och != NULL; och = och_next )
   {
      och_next = och->next_in_room;
      if( ( IS_NPC( och ) ) && ( IS_AFFECTED( och, AFF_CHARM ) ) && ( och->master == ch ) )
      {
         num_followers++;
         if( num_followers <= max_orders )
         {
            found = TRUE;
            act( "$n orders you to '$t'.", ch, argument, och, TO_VICT );
            interpret( och, argument );
         }
         else
         {
            send_to_char( "@@eYou can't control anymore followers!!@@N", ch );
            break;
         }
      }
   }

   if( found )
      send_to_char( "Ok.\r\n", ch );
   else
      send_to_char( "You have no followers here.\r\n", ch );
   return;
}

DO_FUN(do_group)
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   list<CHAR_DATA *>::iterator li;
   short new_members = 0;
   float percent = 0.00;
   int tot_level = 0;
   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      CHAR_DATA *leader;

      for( li = char_list.begin(); li != char_list.end(); li++ )
      {
       victim = *li;
       if( is_same_group(victim,ch) )
        tot_level += victim->get_level("psuedo");
      }

      leader = ( ch->leader != NULL ) ? ch->leader : ch;
      snprintf( buf, MSL, "%s's group:\r\n", leader->get_name(ch) );
      send_to_char( buf, ch );

      for( li = char_list.begin(); li != char_list.end(); li++ )
      {
         victim = *li;
         if( is_same_group( victim, ch ) )
         {
            percent = ((victim->get_level("psuedo") * 100) / tot_level);

            if( ch->act.test(ACT_BLIND_PLAYER) )
            {
               snprintf( buf, MSL,
                        "%-16s %4d of %4d hp %4d of %4d mana %4d of %4d move %5d xp %0.2f%% split\r\n",
                        capitalize( victim->get_name(ch) ),
                        victim->hit, victim->max_hit, victim->mana, victim->max_mana, victim->move, victim->max_move, victim->exp, percent );
            }
            else
            {
               snprintf( buf, MSL,
                        "[%2d %s] %-16s %4d/%4d hp %4d/%4d mana %4d/%4d mv %5d xp [%0.2f%%]\r\n",
                        victim->level,
                        IS_NPC( victim ) ? "Mob" : class_table[victim->p_class].who_name,
                        capitalize( victim->get_name(ch) ),
                        victim->hit, victim->max_hit, victim->mana, victim->max_mana, victim->move, victim->max_move, victim->exp, percent );
            }

            send_to_char( buf, ch );
         }
      }
      return;
   }

   if (  !str_cmp ( arg, "all" )  )
   {
     for( victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room )
     {
        if( ( victim->master == ch ) && ( victim->leader != ch ) )
        {
         victim->leader = ch;
         act( "$N joins $n's group.", ch, NULL, victim, TO_NOTVICT );
         act( "You join $n's group.", ch, NULL, victim, TO_VICT );
         act( "$N joins your group.", ch, NULL, victim, TO_CHAR );
         new_members++;
        }
     }
     if( new_members == 0 )
        send_to_char( "No one else wants to join your group.\r\n", ch );

     return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( ch->master != NULL || ( ch->leader != NULL && ch->leader != ch ) )
   {
      send_to_char( "But you are following someone else!\r\n", ch );
      return;
   }

   if( victim->master != ch && ch != victim )
   {
      act( "$N isn't following you.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( is_same_group( victim, ch ) && ch != victim )
   {
      victim->leader = NULL;
      act( "$n removes $N from $s group.", ch, NULL, victim, TO_NOTVICT );
      act( "$n removes you from $s group.", ch, NULL, victim, TO_VICT );
      act( "You remove $N from your group.", ch, NULL, victim, TO_CHAR );
      return;
   }

   victim->leader = ch;
   act( "$N joins $n's group.", ch, NULL, victim, TO_NOTVICT );
   act( "You join $n's group.", ch, NULL, victim, TO_VICT );
   act( "$N joins your group.", ch, NULL, victim, TO_CHAR );
   return;
}



/*
 * 'Split' originally by Gnort, God of Chaos.
 */
DO_FUN(do_split)
{
   char givebuf[MSL], numbuf[MSL], coinbuf[MSL], catbuf[MSL];
   CHAR_DATA *gch;
   int members;
   int share;
   bool valid = TRUE;

   givebuf[0] = '\0';
   if( money_to_cost( argument ) < 0 )
   {
      send_to_char( "Your group wouldn't like that.\r\n", ch );
      return;
   }
   members = 0;
   for( gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room )
   {
      if( is_same_group( gch, ch ) )
         members++;
   }

   if( members < 2 )
   {
      send_to_char( "Just keep it all.\r\n", ch );
      return;
   }

   for( ;; )
   {
      argument = one_argument( argument, numbuf );
      if( numbuf[0] == '\0' )
         break;
      argument = one_argument( argument, coinbuf );
      if( coinbuf[0] == '\0' )
         break;
      if( ( !is_number( numbuf ) ) || ( money_lookup( coinbuf ) < 0 ) )
      {
         valid = FALSE;
         break;
      }
      share = atoi( numbuf ) / members;
      if( share < 0 )
      {
         send_to_char( "Your group members wouldn't like that!\r\n", ch );
         return;
      }
      snprintf( catbuf, MSL, "%d %s ", share, coinbuf );
      strncat( givebuf, catbuf, MSL-1 );
   }
   if( valid )
   {
      char outbuf[MSL];
      send_to_char( "You distribute the shares.\r\n", ch );
      for( gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room )
      {
         if( gch != ch && is_same_group( gch, ch ) )
         {
            give_money( ch, gch, givebuf );
            snprintf( outbuf, MSL, "%s gives you %s.\r\n", ch->get_name(gch), cost_to_money( money_to_cost( givebuf ) ) );
            send_to_char( outbuf, gch );
         }
      }
   }
   else
   {
      send_to_char( "That is not a valid money amount.\r\n", ch );
   }
   return;
}

DO_FUN(do_gtell)
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *gch;
   list<CHAR_DATA *>::iterator li;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You are too @@rENRAGED @@NTo talk to mortals!\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "Tell your group what?\r\n", ch );
      return;
   }

   if( ch->act.test(ACT_NO_TELL) )
   {
      send_to_char( "Your message didn't get through!\r\n", ch );
      return;
   }

   /*
    * Note use of send_to_char, so gtell works on sleepers.
    */

   if( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 10 )
      argument = slur_text( argument );

   snprintf( buf, MSL, "%s tells the group '%s'.\r\n", ch->name.c_str(), argument );
   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      gch = *li;
      if( is_same_group( gch, ch ) )
         send_to_char( buf, gch );
   }

   return;
}



/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group( CHAR_DATA * ach, CHAR_DATA * bch )
{
   if( ach->leader != NULL )
      ach = ach->leader;
   if( bch->leader != NULL )
      bch = bch->leader;
   return ach == bch;
}

bool is_group_leader( CHAR_DATA * ch )
{
   CHAR_DATA *vch;
   bool rvalue;

   rvalue = FALSE;

   for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
   {
      if( vch != ch && vch->leader == ch )
      {
         rvalue = TRUE;
         break;
      }
   }
   return rvalue;
}

DO_FUN(do_pemote)
{
   char buf[MAX_STRING_LENGTH];
   char *plast;
   buf[0] = '\0';
   if( !IS_NPC( ch ) && ch->act.test(ACT_NO_EMOTE) )
   {
      send_to_char( "You can't pemote.\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "Pemote what?\r\n", ch );
      return;
   }

   for( plast = argument; *plast != '\0'; plast++ )
      ;

   strcpy( buf, argument );
   if( isalpha( plast[-1] ) )
      strncat( buf, ".", MSL );

   act( "$n's $T", ch, NULL, buf, TO_ROOM );
   act( "$n's $T", ch, NULL, buf, TO_CHAR );
   return;
}

DO_FUN(do_pray)
{
   char buf[MSL];

   if( ch->level > LEVEL_HERO )
   {
      send_to_char( "Hey, try immtalk why don't you?\r\n", ch );
      return;
   }

   if( IS_NPC( ch ) )
   {
      send_to_char( "You pray hard... who knows if it got heard?\r\n", ch );
      return;
   }

   /*
    * allows mortals to send messages to immortals...
    * * uses notify to send the message.
    * * -- Stephen
    */

   if( argument[0] == '\0' )
   {
      send_to_char( "What do you wish to pray?\r\n", ch );
      return;
   }

   if( ch->act.test(ACT_NO_PRAY) )
   {
      send_to_char( "The Gods are not listening to you today.\r\n", ch );
      return;
   }

   send_to_char( "You pray, concentrating on your message.\r\n", ch );
   snprintf( buf, MSL, "%s sends the following message via prayer:\r\n`%s'\r\n", ch->name.c_str(), argument );
   notify( buf, LEVEL_HERO );

   return;
}

DO_FUN(do_tongue)
{
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   char buf3[MAX_STRING_LENGTH];
   CHAR_DATA *rch;
   char *pName;
   int iSyl;
   int length;

   struct syl_type
   {
      char *sold;
      char *snew;
   };

   static const struct syl_type syl_table[] = {
      {" ", " "},
      {"fuck", "love"},
      {"go", "swim"},
      {"the", "woi"},
      {"hi", "yibba"},
      {"hello", "smeg"},
      {"me", "ug"},
      {"follow", "grep"},
      {"kill", "banzai"},
      {"ing", "pft"},
      {"er", "sf"},
      {"you", "lpt"},
      {"ast", "pal"},
      {"nd", "ja"},
      {"re", "qa"},
      {"tell", "argh"},
      {"who", "wib"},
      {"which", "fvl"},
      {"ts", "glup"},
      {"st", "plop"},
      {"ck", "fim"},
      {"ord", "xio"},
      {"sw", "cow"},
      {"ic", "er"},
      {"ea", "ox"},

      {"?", "?"}, {"!", "!"}, {":", ":"}, {")", ")"},
      {"(", "("}, {";", ";"}, {"*", "*"}, {"-", "-"},
      {".", ","}, {",", ","},

      {"a", "y"}, {"b", "d"}, {"c", "g"}, {"d", "b"},
      {"e", "q"}, {"f", "i"}, {"g", "u"}, {"h", "r"},
      {"i", "t"}, {"j", "m"}, {"k", "j"}, {"l", "v"},
      {"m", "o"}, {"n", "t"}, {"o", "s"}, {"p", "f"},
      {"q", "k"}, {"r", "x"}, {"s", "z"}, {"t", "e"},
      {"u", "p"}, {"v", "c"}, {"w", "n"}, {"x", "h"},
      {"y", "a"}, {"z", "l"},
      {"", ""}
   };

   buf[0] = '\0';
   buf2[0] = '\0';
   buf3[0] = '\0';

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You are too @@rENRAGED @@NTo talk to mortals!\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 10 )
      argument = slur_text( argument );

   for( pName = argument; *pName != '\0'; pName += length )
   {
      for( iSyl = 0; ( length = strlen( syl_table[iSyl].sold ) ) != 0; iSyl++ )
      {
         if( !str_prefix( syl_table[iSyl].sold, pName ) )
         {
            strncat( buf, syl_table[iSyl].snew, MSL-1 );
            break;
         }
      }
      if( length == 0 )
         length = 1;
   }


   snprintf( buf3, MSL, "$n tongues, '%s'", buf );
   snprintf( buf2, MSL, "$n tongues, '%s'.", argument );

   for( rch = ch->in_room->first_person; rch; rch = rch->next_in_room )
   {
      if( IS_NPC( rch ) )
      {
         act( buf3, ch, NULL, rch, TO_VICT );
         continue;
      }

      if( rch != ch )
         act( ( ch->race == rch->race || rch->level > LEVEL_IMMORTAL ) ? buf2 : buf3, ch, NULL, rch, TO_VICT );
   }

   snprintf( buf2, MSL, "You tongue, '%s'.\r\n", buf );
   send_to_char( buf2, ch );
   snprintf( buf2, MSL, "Translation: %s.\r\n", argument );
   send_to_char( buf2, ch );
   return;
}


char *ghost_text( char *argument )
{
 static char buf[MSL];
 unsigned int i = 0;

 buf[0] = '\0';

 for( i = 0; i < strlen(argument); i++ )
 {
  if( number_range(1,2) == 1 )
   strncat(buf,"o",MSL);
  else
   strncat(buf,"O",MSL);
 }

 return buf;
}

char *slur_text( char *argument )
{
   /*
    * Used to slur text, if a player is DRUNK. 
    */

   static char buf[MAX_STRING_LENGTH];
   char *pName;
   int iSyl;
   int length;

   struct syl_type
   {
      char *sold;
      char *snew;
   };

   static const struct syl_type syl_table[] = {
      {" ", " "},
      {"th", "f"},
      {"ck", "k"},

      {"?", "?"}, {"!", "!"}, {":", ":"}, {")", ")"},
      {"(", "("}, {";", ";"}, {"*", "*"}, {"-", "-"},
      {".", ","}, {",", ","},

      {"a", "a"}, {"b", "b"}, {"c", "see"}, {"d", "d"},
      {"e", "e"}, {"f", "f"}, {"g", "g"}, {"h", "h"},
      {"i", "i"}, {"j", "j"}, {"k", "g"}, {"l", "l"},
      {"m", "m"}, {"n", "n"}, {"o", "o"}, {"p", "p"},
      {"q", "q"}, {"r", "r"}, {"s", "ss"}, {"t", "t"},
      {"u", "u"}, {"v", "s"}, {"w", "w"}, {"x", "x"},
      {"y", "y"}, {"z", "s"},
      {NULL, NULL}
   };

   /*
    * might be fixed now! return argument; 
    */

   buf[0] = '\0';
   for( pName = argument; *pName != '\0'; pName += length )
   {
      length = 1;

      if( *pName == '$' )  /* do not modify args to act. */
      {
         if( *pName != '\0' )
            length++;
         continue;
      }

      for( iSyl = 0; syl_table[iSyl].sold != NULL; iSyl++ )
      {
         if( !str_prefix( syl_table[iSyl].sold, pName ) )
         {
            strncat( buf, syl_table[iSyl].snew, MSL );
            length = strlen( syl_table[iSyl].sold );
            break;
         }
      }
   }

   return ( buf );
}

DO_FUN(do_whisper)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int position;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You are too @@rENRAGED @@NTo talk to mortals!\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && ch->act.test(ACT_SILENCE) )
   {
      send_to_char( "Your whispering skills seem rusty today.\r\n", ch );
      return;
   }


   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "Whisper what to whom?\r\n", ch );
      return;
   }


   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( ch->in_room != victim->in_room )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }
   if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( IS_SHIFTED( victim ) || IS_RAGED( victim ) ) )
   {
      send_to_char( "They can't hear you.\r\n", ch );
      return;
   }


   if( !IS_IMMORTAL( ch ) && !IS_AWAKE( victim ) )
   {
      act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
      return;
   }

   if( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 10 )
      argument = slur_text( argument );

   act( "You whisper to $N '$t'.", ch, argument, victim, TO_CHAR );
   position = victim->position;
   victim->position = POS_STANDING;
   act( "$n whispers to you '$t'.", ch, argument, victim, TO_VICT );
   act( "$n whispers something secret to $N.", ch, NULL, victim, TO_NOTVICT );
   victim->position = position;

   return;
}

DO_FUN(do_ask)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int position;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You are too @@rENRAGED @@NTo talk to mortals!\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && ch->act.test(ACT_SILENCE) )
   {
      send_to_char( "You seem to have problems speaking!\r\n", ch );
      return;
   }


   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "Ask whom what?\r\n", ch );
      return;
   }


   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( ch->in_room != victim->in_room )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }
   if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( IS_SHIFTED( victim ) || IS_RAGED( victim ) ) )
   {
      send_to_char( "They can't hear you.\r\n", ch );
      return;
   }


   if( !IS_IMMORTAL( ch ) && !IS_AWAKE( victim ) )
   {
      act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
      return;
   }

   if( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 10 )
      argument = slur_text( argument );

   act( "You ask $N '$t'.", ch, argument, victim, TO_CHAR );
   position = victim->position;
   victim->position = POS_STANDING;
   act( "$n asks you '$t'.", ch, argument, victim, TO_VICT );
   act( "$n asks $N a question.", ch, NULL, victim, TO_NOTVICT );
   victim->position = position;

   return;
}

void send_to_room( char *message, ROOM_INDEX_DATA * room )
{
   CHAR_DATA *vch;
   list<CHAR_DATA *>::iterator li;

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      vch = *li;
      if( IS_AWAKE( vch ) && vch->in_room == room )
      {
         send_to_char( message, vch );
         send_to_char( "\r\n", vch );
      }
   }
   return;
}

DO_FUN(do_beep)
{
   int pos;
   char arg1[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg1 );
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You are too @@rENRAGED @@NTo talk to mortals!\r\n", ch );
      return;
   }

   if( arg1[0] == '\0' )
   {
      send_to_char( "Usage: BEEP <victim>\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "Couldn't find the victim.\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( IS_SHIFTED( victim ) || IS_RAGED( victim ) ) )
   {
      send_to_char( "They can't hear you.\r\n", ch );
      return;
   }


   if( IS_NPC( victim ) )
   {
      send_to_char( "Can only beep players...\r\n", ch );
      return;
   }

   if( ch == victim )
   {
      send_to_char( "Beep yourself?  Sure, ok... beep! beep! beep!\r\n", ch );
      return;
   }

   if( victim->deaf.test(CHANNEL_BEEP) )
   {
      send_to_char( "Your victim is ignoring beeps.  Sorry!\r\n", ch );
      return;
   }

   pos = victim->position;
   victim->position = POS_STANDING;

   act( "You beep $N...", ch, NULL, victim, TO_CHAR );
   act( "\a$n is beeping you...", ch, NULL, victim, TO_VICT );

   victim->position = pos;
   return;
}


void ask_quest_question( CHAR_DATA *ch, char *argument )
{
   extern CHAR_DATA *quest_mob;
   extern CHAR_DATA *quest_target;
   extern OBJ_DATA *quest_object;
   extern short quest_timer;
   extern bool quest;
   char buf[MAX_STRING_LENGTH];
   buf[0] = '\0';

   if( !quest || IS_NPC( ch ) )
      return;

   if( ( !str_cmp( argument, "who is the thief?" ) )
       || ( !str_cmp( argument, "who was the thief?" ) )
       || ( !str_cmp( argument, "what mob?" ) ) || ( !str_cmp( argument, "who stole the item?" ) ) )
   {
      if( quest_mob )
      {
         if( quest_timer < 7 )
         {
            snprintf( buf, MSL, "@@eI don't even know who stole it yet!@@N" );
         }
         else if( quest_object && quest_target )
         {
            snprintf( buf, MSL, "@@NIt was %s @@N who stole my %s@@N.", quest_target->get_name(), quest_object->short_descr );
         }
      }
      else if( quest_object )
      {
         snprintf( buf, MSL, "@@NDon't worry about who stole my %s@@N, he has recieved his just reward!",
                  quest_object->short_descr );
      }
      if( quest_mob != NULL )
         do_crusade( quest_mob, buf );
      return;
   }

   if( !str_cmp( argument, "what item?" ) )
   {
      if( quest_mob && quest_object )
      {
         snprintf( buf, MSL, "@@NMy %s @@Nwas stolen from me.", quest_object->short_descr );
         do_crusade( quest_mob, buf );
         return;
      }
   }

   if( !str_cmp( argument, "where are you?" ) )
      if( quest_mob )
      {
         snprintf( buf, MSL, "@@NYou can find me in %s@@N, please hurry!!", quest_mob->in_room->area->name );
         do_crusade( quest_mob, buf );
         return;
      }

   if( !str_cmp( argument, "where is the thief?" ) )
   {
      if( quest_mob )
      {
         if( ( quest_target ) && ( quest_timer > 7 ) )
         {
            if( quest_timer < 10 )
            {
               snprintf( buf, MSL, "@@NI don't really know where %s@@N is, let me try and find out.", quest_target->get_name() );
            }
            else if( quest_target )
            {
               snprintf( buf, MSL, "@@NI'm not really sure, but I THINK %s@@N is in %s@@N",
                        quest_target->get_name(), quest_target->in_room->area->name );
            }

         }
         else if( ( quest_target ) && ( quest_timer <= 7 ) )
         {
            snprintf( buf, MSL, "@@eI don't even know who stole it yet!@@N" );
         }
         else
         {
            snprintf( buf, MSL, "@@NDon't worry about where the thief who stole my %s@@N is, he has recieved his just reward",
                     quest_object->short_descr );
         }
         do_crusade( quest_mob, buf );
      }
   }
   return;
}
