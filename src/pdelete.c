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

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_SAVE_H
#include "h/save.h"
#endif

DO_FUN(do_sdelete)
{
   DESCRIPTOR_DATA *d;
   char strsave[MAX_INPUT_LENGTH];
   char arg1[MAX_INPUT_LENGTH];
   char *pArg;
   char cEnd;
   char buf[MAX_INPUT_LENGTH];

   if( IS_NPC( ch ) )
      return;
   strcpy( buf, ch->name.c_str() );
   snprintf( strsave, MIL, "%s%s%s%s", PLAYER_DIR, initial( buf ), "/", capitalize( buf ) );

   pArg = arg1;
   while( isspace( *argument ) )
      argument++;

   cEnd = ' ';
   if( *argument == '\'' || *argument == '"' )
      cEnd = *argument++;

   while( *argument != '\0' )
   {
      if( *argument == cEnd )
      {
         argument++;
         break;
      }
      *pArg++ = *argument++;
   }
   *pArg = '\0';

   if( ( ch->pcdata->pwd != '\0' ) && ( arg1[0] == '\0' ) )
   {
      send_to_char( "Syntax: pdelete <password>.\r\n", ch );
      return;
   }
   if( ( ch->pcdata->pwd != '\0' ) && ( strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) ) )
   {
      WAIT_STATE( ch, 1000 );
      send_to_char( "Wrong password.  Wait 10 seconds.\r\n", ch );
      return;
   }


   unlink( strsave );
   mudinfo.total_pfiles--;
   send_to_char( "Character deleted.\r\n", ch );

   d = ch->desc;
   extract_char( ch, TRUE );
   if( d != NULL )
      close_socket( d );
}
