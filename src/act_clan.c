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

#ifndef DEC_ACT_CLAN_H
#include "h/act_clan.h"
#endif

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
#endif

#ifndef DEC_ACT_INFO_H
#include "h/act_info.h"
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

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

/* These set of clan functions by Zen */
extern POL_DATA politics_data;
extern COUNCIL_DATA super_councils[MAX_SUPER];

void save_clan_table(  )
{

   FILE *fp;
   char clan_file_name[MAX_STRING_LENGTH];
   short x, y;

   snprintf( clan_file_name, MSL, "%s", CLAN_FILE );

   if( ( fp = file_open( clan_file_name, "w" ) ) == NULL )
   {
      bug( "Save Clan Table: file_open", 0 );
      perror( "failed open of clan_table.dat in save_clan_table" );
   }
   else
   {
      for( x = 1; x < MAX_CLAN; x++ )
      {
         for( y = 1; y < MAX_CLAN; y++ )
         {
            fprintf( fp, "  %5d  ", politics_data.diplomacy[x][y] );
         }

         fprintf( fp, "\n" );

      }

      for( x = 1; x < MAX_CLAN; x++ )
      {
         fprintf( fp, "%li\n", politics_data.treasury[x] );
      }

      for( x = 1; x < MAX_CLAN; x++ )
      {
         for( y = 1; y < MAX_CLAN; y++ )
         {
            fprintf( fp, "  %5d  ", politics_data.end_current_state[x][y] );
         }

         fprintf( fp, "\n" );

      }

      file_close( fp );
   }
   return;

}

DO_FUN(do_ctoggle)
{
   /*
    * Toggles clan members various clan job flags
    * * Zen
    */

   CHAR_DATA *victim;
   int job;

   char arg1[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );


   if( IS_NPC( ch ) || !ch->act.test(ACT_CBOSS) )
   {
      send_to_char( "This command is for clan bosses only.\r\n", ch );
      return;
   }

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: ctoggle <clan number> <armourer/diplomat/treasurer/leader>\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPCs!\r\n", ch );
      return;
   }

   if( ch->clan != victim->clan )
   {
      send_to_char( "Only on members of YOUR clan!\r\n", ch );
      return;
   }

   if( !str_prefix( arg2, "armourer" ) )
      job = ACT_CARMORER;
   else if( !str_prefix( arg2, "diplomat" ) )
      job = ACT_CDIPLOMAT;
   else if( !str_prefix( arg2, "treasurer" ) )
      job = ACT_CTREASURER;
   else if( !str_prefix( arg2, "leader" ) )
      job = ACT_CLEADER;

   else
   {
      send_to_char( "That's not a legal clan job!!\r\n", ch );
      return;
   }

   victim->act.flip(job);

   switch ( job )
   {
      case ACT_CTREASURER:

         if( victim->act.test(ACT_CTREASURER) )
         {
            snprintf( arg1, MSL, "Job set as CLAN TREASURER.\r\n" );
            snprintf( arg2, MSL, "You have been set as a clan treasurer.\r\n" );
         }
         else
         {
            snprintf( arg1, MSL, "Job cleared: CLAN TREASURER.\r\n" );
            snprintf( arg2, MSL, "You are no longer a clan treasurer.\r\n" );
         }
         break;

      case ACT_CDIPLOMAT:

         if( victim->act.test(ACT_CDIPLOMAT) )
         {
            snprintf( arg1, MSL, "Job set as CLAN DIPLOMAT.\r\n" );
            snprintf( arg2, MSL, "You have been set as a clan diplomat.\r\n" );
         }
         else
         {
            snprintf( arg1, MSL, "Job cleared: CLAN DIPLOMAT.\r\n" );
            snprintf( arg2, MSL, "You are no longer a clan diplomat.\r\n" );
         }
         break;

      case ACT_CARMORER:

         if( victim->act.test(ACT_CARMORER) )
         {
            snprintf( arg1, MSL, "Job set as CLAN ARMOURER.\r\n" );
            snprintf( arg2, MSL, "You have been set as a clan armourer.\r\n" );
         }
         else
         {
            snprintf( arg1, MSL, "Job cleared: CLAN ARMOURER.\r\n" );
            snprintf( arg2, MSL, "You are no longer a clan armourer.\r\n" );
         }
         break;

      case ACT_CLEADER:

         if( victim->act.test(ACT_CLEADER) )
         {
            snprintf( arg1, MSL, "Job set as CLAN LEADER.\r\n" );
            snprintf( arg2, MSL, "You have been set as a clan leader.\r\n" );
         }
         else
         {
            snprintf( arg1, MSL, "Job cleared: CLAN LEADER.\r\n" );
            snprintf( arg2, MSL, "You are no longer a clan leader.\r\n" );
         }
         break;
   }  /* end of switch( job )  */

   send_to_char(arg1,ch);
   send_to_char(arg2,victim);

   return;
}




char *get_diplo_name( short value )
{

   char *name = '\0';

   if( value < -450 )
      name = "@@R  WAR  @@N";
   else if( value < -300 )
      name = "@@e HATRED@@N";
   else if( value < -150 )
      name = "@@dDISLIKE@@N";
   else if( value < 150 )
      name = "@@WNEUTRAL@@N";
   else if( value < 300 )
      name = "@@aRESPECT@@N";
   else if( value < 450 )
      name = "@@l TRUST @@N";
   else
      name = "@@B ALLY  @@N";

   return ( name );
}



DO_FUN(do_politics)
{
   short x, y;
   string str;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Not for Npcs.\r\n", ch );
      return;
   }

   send_to_char( "@@NCurrent Politics of " mudnamecolor "\r\n\r\n", ch );

   str +=  "        ";
   for( x = 1; x < MAX_CLAN; x++ )
   {
      str += " ";
      str += clan_table[x].clan_abbr;
      str += "  ";
   }
   str += "\r\n\r\n";

   send_to_char( str, ch );
   str.clear();

   for( x = 1; x < MAX_CLAN; x++ )
   {
      /*
       * clan symbol here 
       */
      char tmp[4] = {'\0'};
      str.clear();
      snprintf(tmp,4,"%d",x);

      str += tmp;
      str += " ";
      str += clan_table[x].clan_abbr;
      str += " ";

      for( y = 1; y < MAX_CLAN; y++ )
      {
         if( x != y )
         {
            str += " ";
            str += get_diplo_name(politics_data.diplomacy[x][y]);
         }
         else
            str += "        " ;
      }
      str += "\r\n\r\n";
      send_to_char( str, ch );
   }

   if( ch->act.test(ACT_CDIPLOMAT) )
      for( x = 1; x < MAX_CLAN; x++ )
      {
         if( politics_data.end_current_state[ch->clan][x] )
         {
            str.clear();
            str += clan_table[x].clan_name;
            str += " has requested an end to your current state of affairs.\r\n";
            send_to_char( str, ch );
         }
      }
   return;
}

DO_FUN(do_negotiate)
{
   char buf[MAX_STRING_LENGTH];
   char arg1[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];
   short target_clan;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );


   if( IS_NPC( ch ) || !ch->act.test(ACT_CDIPLOMAT) )
   {
      send_to_char( "This command is for clan diplomats only.\r\n", ch );
      return;
   }

   if( !is_number( arg1 ) || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: Negotiate <clan number> <peace/war/end>\r\n", ch );
      return;
   }

   target_clan = atoi( arg1 );
   if( ( target_clan < 1 ) || ( target_clan > ( MAX_CLAN - 1 ) ) )
   {
      send_to_char( "That is not a clan!\r\n", ch );
      return;
   }
   if( target_clan == ch->clan )
   {
      send_to_char( "Well, that will accomplish a lot..you must be of two minds about the whole thing!\r\n", ch );
      return;
   }

   if( politics_data.daily_negotiate_table[ch->clan][target_clan] )
   {
      snprintf( buf, MSL, "Your clan has already negotiated with %s today.\r\n", clan_table[target_clan].clan_name );
      send_to_char( buf, ch );
      return;
   }

   if( !str_prefix( arg2, "peace" ) )
   {
      if( politics_data.diplomacy[ch->clan][target_clan] < -450 )
      {
         if( politics_data.end_current_state[ch->clan][target_clan] &&
             politics_data.end_current_state[target_clan][ch->clan] )
         {
            politics_data.diplomacy[ch->clan][target_clan] = -425;
            politics_data.diplomacy[target_clan][ch->clan] = -425;
            send_to_char( "@@NYou have successfully negotiated an end to this dreaded @@eCLAN WAR@@N. Great Job!!\r\n", ch );
            snprintf( buf, MSL, "@@eCLAN:@@N The war between %s and %s has ended. They may no longer PKILL each other!!\r\n",
                     clan_table[ch->clan].clan_name, clan_table[target_clan].clan_name );
            info( buf, 1 );

            politics_data.end_current_state[ch->clan][target_clan] = FALSE;
            politics_data.end_current_state[target_clan][ch->clan] = FALSE;

            return;
         }
         else
         {
            snprintf( buf, MSL,
                     "@@NYou are currently at @@RWAR@@N with %s. Both clans must negotiate an end to the war first.\r\n",
                     clan_table[target_clan].clan_name );
            send_to_char( buf, ch );
            return;
         }
      }
      else
      {
         politics_data.diplomacy[ch->clan][target_clan] += 25;
         politics_data.diplomacy[target_clan][ch->clan] += 25;

         snprintf( buf, MSL, "You are requesting a more peaceful state of affairs with %s.", clan_table[target_clan].clan_name );
         send_to_char( buf, ch );
      }
   }



   else if( !str_prefix( arg2, "war" ) )
   {
      if( ( politics_data.diplomacy[ch->clan][target_clan] - 50 ) < -450 )

      {
         snprintf( buf, MSL, "@@NYou have started a @@eWAR@@N with %s! Watch out!.\r\n", clan_table[target_clan].clan_name );
         send_to_char( buf, ch );

         snprintf( buf, MSL, "@@eCLAN:@@N A war has started between %s and %s. They may now PKILL each other!!\r\n",
                  clan_table[ch->clan].clan_name, clan_table[target_clan].clan_name );
         info( buf, 1 );

         politics_data.diplomacy[ch->clan][target_clan] = -460;
         politics_data.diplomacy[target_clan][ch->clan] = -460;
         politics_data.end_current_state[ch->clan][target_clan] = FALSE;
         politics_data.end_current_state[target_clan][ch->clan] = FALSE;
         save_clan_table(  );
         return;
      }

      else
      {
         politics_data.diplomacy[ch->clan][target_clan] -= 50;
         politics_data.diplomacy[target_clan][ch->clan] -= 50;

         snprintf( buf, MSL, "You are requesting a more aggressive state of affairs with %s.", clan_table[target_clan].clan_name );
         send_to_char( buf, ch );
      }
   }
   else if( !str_prefix( arg2, "end" ) )
   {
      if( politics_data.diplomacy[ch->clan][target_clan] > 450 )
      {
         send_to_char( "You are negotiating an and to your alliance.\r\n", ch );
         politics_data.end_current_state[ch->clan][target_clan] = FALSE;
         politics_data.end_current_state[target_clan][ch->clan] = FALSE;
         politics_data.diplomacy[ch->clan][target_clan] = 100;
         politics_data.diplomacy[target_clan][ch->clan] = 100;

      }
      if( politics_data.diplomacy[ch->clan][target_clan] < -450 )
      {
         politics_data.end_current_state[ch->clan][target_clan] = TRUE;

         if( politics_data.end_current_state[ch->clan][target_clan]
             && politics_data.end_current_state[target_clan][ch->clan] )
         {
            send_to_char
               ( "Both clans have successfully negotiated and end to the war.  Negotiate peace to seal your treaty!\r\n",
                 ch );
            save_clan_table(  );
            return;
         }
         else
         {
            send_to_char( " You have requested an end to this dreaded war, but the other clan has not yet agreed.\r\n", ch );
            save_clan_table(  );
            return;
         }
      }
      else
      {
         send_to_char( "You must be either at war or in an alliance with a clan before you can END it.\r\n", ch );
         return;
      }
   }
   else
   {
      send_to_char( "That is not a legal diplomatic negotiation!\r\n", ch );
      return;
   }
   politics_data.daily_negotiate_table[ch->clan][target_clan] = TRUE;


/* save table to file here */
   save_clan_table(  );
   return;
}

/* These set of clan functions by [ -S- ] Stephen */

DO_FUN(do_leav)
{
   send_to_char( "If you want to LEAVE, spell it out!!\r\n", ch );
   return;
}




/* CSET is an immortal command, used to set a players clan.
 * it is also used by other functions.
 */

DO_FUN(do_cset)
{
   /*
    * This is basically do_mset, but with its guts ripped out,
    * and a slight change to what was left.
    * -- Stephen
    */

   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   int value;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   strcpy( arg2, argument );
   if( IS_NPC( ch ) )
      return;

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: cset <player> <clan No.>\r\n\r\n", ch );
      do_clan_list( ch, "" );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   value = is_number( arg2 ) ? atoi( arg2 ) : -1;

   if( value == -1 )
   {
      send_to_char( "Syntax: cset <player> <clan No.>\r\n\r\n", ch );
      do_clan_list( ch, "" );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( value < 0 || value >= MAX_CLAN )
   {
      snprintf( buf, MSL, "%d is not a valid value.\r\n", value );
      send_to_char( buf, ch );
      snprintf( buf, MSL, "Use a value between 0 and %d.\r\n\r\n", MAX_CLAN - 1 );
      send_to_char( buf, ch );
      do_clan_list( ch, "" );
      return;
   }


   victim->clan = value;
   snprintf( buf, MSL, "%s now belongs to clan %s.\r\n", victim->name.c_str(), clan_table[value].clan_name );
   send_to_char( buf, ch );

   snprintf( buf, MSL, "%s has cset %s into clan %s.", ch->name.c_str(), victim->name.c_str(), clan_table[value].clan_name );
   monitor_chan( buf, MONITOR_CLAN );

   return;
}

/* accept is a command available to players set as clan_leader.
 * after various checks, the victim's clan is set to the leader's.
 * note that the victim must NOT be in a clan to be accepted.
 */

DO_FUN(do_accept)
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;

   if( IS_NPC( ch ) )
      return;

   if( !ch->act.test(ACT_CLEADER) )
   {
      send_to_char( "You must be a clan leader to use this command!\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPCs!\r\n", ch );
      return;
   }

   if( IS_IMMORTAL( victim ) )
   {
      send_to_char( "You hear the gods laughing at you.  Nice try.\r\n", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "You want to accept yourself... Strange.\r\n", ch );
      return;
   }

   if( victim->clan != 0 )
   {
      snprintf( buf, MSL, "%s is already in a clan.  Maybe they should leave it first?\r\n", victim->name.c_str() );
      send_to_char( buf, ch );
      return;
   }

   if( victim->level < 20 )
   {
      snprintf( buf, MSL, "%s must be at least 20th level to enter a clan.\r\n", victim->name.c_str() );
      send_to_char( buf, ch );
      return;
   }
   victim->clan = ch->clan;
   act( "$N accepts you into $S clan!", victim, NULL, ch, TO_VICT );
   act( "You accept $N into your clan!", ch, NULL, victim, TO_VICT );

   snprintf( buf, MSL, "%s has accepted %s into clan %s.", ch->name.c_str(), victim->name.c_str(), clan_table[ch->clan].clan_name );
   monitor_chan( buf, MONITOR_CLAN );

   return;
}

/* cwhere is an adapted copy of the do_where command, except it only shows
 * clan members, and operates globally.
 */

DO_FUN(do_cwhere)
{


   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   DESCRIPTOR_DATA *d;
   bool found;
   if( IS_NPC( ch ) )
      return;

   if( ch->clan == 0 )
   {
      send_to_char( "You don't belong to a clan!\r\n", ch );
      return;
   }

   send_to_char( "Clan members visible to you:\r\n", ch );
   found = FALSE;
   for( d = first_desc; d != NULL; d = d->next )
   {
      if( d->connected == CON_PLAYING
          && ( victim = d->character ) != NULL
          && !IS_NPC( victim )
          && victim->in_room != NULL && ( ch->act.test(ACT_CBOSS) || can_see( ch, victim ) ) )
      {
         if( victim->clan != ch->clan )
            continue;
         if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( IS_SHIFTED( victim ) || IS_RAGED( victim ) ) )
            continue;

         found = TRUE;
         snprintf( buf, MSL, "%-28s %s ", victim->name.c_str(), victim->in_room->name );
         if( ch->act.test(ACT_CBOSS) )
         {
            if( victim->act.test(ACT_CLEADER) )
               strncat( buf, " L ", MSL );

            if( victim->act.test(ACT_CARMORER) )
               strncat( buf, " A ", MSL );

            if( victim->act.test(ACT_CTREASURER) )
               strncat( buf, " T ", MSL );

            if( victim->act.test(ACT_CDIPLOMAT) )
               strncat( buf, " D ", MSL );
         }
         strncat( buf, "\r\n", MSL );


         send_to_char( buf, ch );
      }
   }

   if( !found )
      send_to_char( "No other clan members were found.\r\n", ch );


   return;
}

/* leave allows a (non-leader) clan member to leave his/her clan.
 * their clan is set to 0.  Again, use do_cset to do the actual
 * stuff, and to generate the usage 'text'.  ??? ummm ???
 */

DO_FUN(do_leave)
{
   char buf[MAX_STRING_LENGTH];

   if( IS_NPC( ch ) )
      return;

   if( ch->clan == 0 )   /* No clan */
   {
      send_to_char( "You must be IN a clan before you can leave it!\r\n", ch );
      return;
   }

   snprintf( buf, MSL, "%s has left clan %s.", ch->name.c_str(), clan_table[ch->clan].clan_name );
   monitor_chan( buf, MONITOR_CLAN );

   ch->clan = 0;
   ch->act.reset(ACT_CDIPLOMAT | ACT_CTREASURER | ACT_CARMORER | ACT_CLEADER | ACT_CBOSS);
   send_to_char( "You leave your clan.  Let's hope they don't get mad!\r\n", ch );

   return;
}


DO_FUN(do_banish)
{
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];

   if( IS_NPC( ch ) )
      return;

   if( !ch->act.test(ACT_CLEADER) )
   {
      send_to_char( "Only Clan Leaders may use this command.\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "Banish WHO from your clan?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      send_to_char( "No such Person.\r\n", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "Dumb Idea!\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "You can't banish a NPC!\r\n", ch );
      return;
   }

   if( victim->clan != ch->clan )
   {
      send_to_char( "They're not in your clan!\r\n", ch );
      return;
   }

   if( victim->act.test(ACT_CBOSS) )
   {
      send_to_char( "Nice Try.", ch );
      return;
   }

   victim->clan = 0;
   victim->act.reset(ACT_CDIPLOMAT | ACT_CTREASURER | ACT_CARMORER | ACT_CLEADER | ACT_CBOSS);

   snprintf( buf, MSL, "%s has banished %s from clan %s.", ch->name.c_str(), victim->name.c_str(), clan_table[ch->clan].clan_name );
   monitor_chan( buf, MONITOR_CLAN );

   snprintf( buf, MSL, "%s banishes you from clan %s!\r\n", ch->name.c_str(), clan_table[ch->clan].clan_name );
   send_to_char( buf, victim );
   act( "$N has been banished.", ch, NULL, victim, TO_CHAR );
   return;
}

DO_FUN(do_make)
{
   char buf[MAX_STRING_LENGTH];
   char arg1[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];
   OBJ_INDEX_DATA *pObj;
   OBJ_DATA *obj;
   int cnt;
   int num;
   CHAR_DATA *target;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );


   if( IS_NPC( ch ) || !ch->act.test(ACT_CARMORER) )
   {
      send_to_char( "This command is for clan armourers only.\r\n", ch );
      return;
   }

   if( !is_number( arg1 ) || arg2[0] == '\0' )
   {
      snprintf( buf, MSL, "Syntax: Make [0-%d] [Target]\r\n", MAX_CLAN_EQ - 1 );
      send_to_char( buf, ch );
      send_to_char( "Items you can currently make:\r\n\r\n", ch );

      for( cnt = 0; cnt < MAX_CLAN_EQ; cnt++ )
      {
         if( clan_table[ch->clan].eq[cnt] != -1
             && ( ( pObj = get_obj_index( clan_table[ch->clan].eq[cnt] ) ) != NULL ) )
         {
            snprintf( buf, MSL, "[%2d] : %s.\r\n", cnt, pObj->short_descr );
            send_to_char( buf, ch );
         }
         else
         {
            snprintf( buf, MSL, "[%2d] : No Object Set.\r\n", cnt );
            send_to_char( buf, ch );
         }
      }
      return;
   }

   target = get_char_room( ch, arg2 );
   if( target == NULL )
   {
      send_to_char( "No one with that name is here with you.\r\n", ch );
      return;
   }

   if( IS_NPC( target ) )
   {
      send_to_char( "NOT on NPCs!\r\n", ch );
      return;
   }

   if( target->clan != ch->clan )
   {
      act( "$N isn't in your clan...", ch, NULL, target, TO_CHAR );
      return;
   }


   num = atoi( arg1 );
   if( num < 0 || num >= MAX_CLAN_EQ )
   {
      do_make( ch, "" );
      return;
   }

   if( clan_table[ch->clan].eq[num] == -1 )
   {
      send_to_char( "No object has been set for that slot.\r\n", ch );
      do_make( ch, "" );
      return;
   }

   if( ( pObj = get_obj_index( clan_table[ch->clan].eq[num] ) ) == NULL )
   {
      send_to_char( "Couldn't find that object to load...\r\n", ch );
      do_make( ch, "" );
      return;
   }

   if( !IS_OBJ_STAT(pObj,ITEM_EXTRA_CLAN_EQ) )
   {
      send_to_char( "Object exists, but not flagged as Clan Eq.\r\n", ch );
      return;
   }

   if( ch != target )
   {
      obj = create_object( pObj, target->level );
      act( "$n creates $p, and hands it to $N.", ch, obj, target, TO_NOTVICT );
      act( "You create $p, and hand it to $N.", ch, obj, target, TO_CHAR );
      act( "$N creates $p, and hands it to you.", target, obj, ch, TO_CHAR );
      obj_to_char( obj, target );
      snprintf( buf, MSL, "%s has made %s for %s.", ch->name.c_str(), obj->short_descr, target->name.c_str() );
   }
   else
   {
      obj = create_object( pObj, ch->level );
      act( "You create $p, and put it away.", ch, obj, NULL, TO_CHAR );
      act( "$n creates $p, and puts it away.", ch, obj, NULL, TO_ROOM );
      obj_to_char( obj, ch );
      snprintf( buf, MSL, "%s has made themself %s.", ch->name.c_str(), obj->short_descr );
   }
   monitor_chan( buf, MONITOR_CLAN );

   return;
}


DO_FUN(do_council)
{
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   char arg1[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];
   MEMBER_DATA *imember;
   MEMBER_DATA *member;
   short this_council = SUPER_NONE;
   CHAR_DATA *victim;


   if( ( IS_NPC( ch ) ) || ( !ch->act.test(ACT_COUNCIL) ) )
   {
      send_to_char( "You are not a council member!\r\n", ch );
      return;
   }

   if( IS_VAMP( ch ) )  /* add other super checks here later */
      this_council = SUPER_VAMP;
   GET_FREE( member, member_free );

   member->this_member = ch;
   member->next = NULL;
   member->prev = NULL;


   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' )
   {
      snprintf( buf, MSL, "%s", "" );
      snprintf( buf2, MSL, "%s", "" );
      snprintf( buf, MSL, "Members of the Council of %s\r\n\r\n", super_councils[this_council].council_name );

      for( imember = super_councils[this_council].first_member; imember != NULL; imember = imember->next )
      {
         snprintf( buf2, MSL, "%s\r\n", imember->this_member->name.c_str() );
         strncat( buf, buf2, MSL-1 );
      }
      send_to_char( buf, ch );

      return;
   }

   if( !str_prefix( arg1, "join" ) )
   {
      for( imember = super_councils[this_council].first_member; imember != NULL; imember = imember->next )
      {
         if( imember->this_member == ch )
         {
            send_to_char( "You have already joined the current Council!\r\n", ch );
            return;
         }
      }

      LINK( member, super_councils[this_council].first_member, super_councils[this_council].last_member, next, prev );
      snprintf( buf, MSL, "You have joined the Current Council of %s\r\n", super_councils[this_council].council_name );
      send_to_char( buf, ch );
      super_councils[this_council].council_time = 10;
      do_council( ch, "" );
      return;
   }

   if( !str_prefix( arg1, "leave" ) )
   {
      bool in_council = FALSE;

      for( imember = super_councils[this_council].first_member; imember != NULL; imember = imember->next )
      {
         if( imember->this_member == ch )
         {
            in_council = TRUE;
            break;
         }
      }

      if( in_council )
      {
         UNLINK( imember, super_councils[this_council].first_member, super_councils[this_council].last_member, next, prev );
         imember->this_member = NULL;
         imember->next = NULL;
         imember->prev = NULL;
         PUT_FREE( imember, member_free );
         send_to_char( "You have removed yourself from the current Council in Session!\r\n", ch );

      }
      else
         send_to_char( "You are not part of the Current Council!\r\n", ch );

      return;
   }

   if( !str_prefix( arg1, "accept" ) )
   {
      if( !super_councils[this_council].quorum )
      {
         send_to_char( "The council must have a quorum before this action may be taken.\r\n", ch );
         return;
      }
      else
      {
         if( ( victim = get_char_room( ch, arg2 ) ) != NULL )
         {
            if( IS_NPC( victim ) )
            {
               send_to_char( "Not on NPCS!!\r\n", ch );
               return;
            }
            else if( this_council == SUPER_VAMP && !IS_VAMP( victim ) )
            {
               send_to_char( "They are not a VAMPYRE!!\r\n", ch );
               return;
            }
            else
            {
               snprintf( buf, MSL, "%s is now a member of the Council of %s!\r\n", victim->name.c_str(),
                        super_councils[this_council].council_name );
               send_to_char( buf, ch );
               snprintf( buf, MSL, "You are now a member of the Council of %s!\r\n", super_councils[this_council].council_name );
               victim->act.set(ACT_COUNCIL);
            }
         }
         else
         {
            send_to_char( "They are not in this room!\r\n", ch );
         }
         return;
      }
   }

   if( !str_prefix( arg1, "outcast" ) )
   {

      if( !super_councils[this_council].quorum )
      {
         send_to_char( "The council must have a quorum before this action may be taken.\r\n", ch );
         return;
      }
      else
      {
         if( ( victim = get_char_world( ch, arg2 ) ) != NULL )
         {
            if( IS_NPC( victim ) )
            {
               send_to_char( "Not on NPCS!!\r\n", ch );
               return;
            }
            else if( this_council == SUPER_VAMP && !IS_VAMP( victim ) )
            {
               send_to_char( "They are not a VAMPYRE!!\r\n", ch );
               return;
            }
            else
            {
               snprintf( buf, MSL, "%s has been outcast from the @@eKindred@@N by the %s, and is now a @@dRENEGADE@@N!!!!\r\n",
                        victim->name.c_str(), super_councils[this_council].council_name );
               send_to_char( buf, ch );
               snprintf( buf, MSL,
                        "You have been @@ROUTCAST@@N from the @@eKindred@@N by the %s, and are now a @@dRenegade@@N!!!!!\r\n",
                        super_councils[this_council].council_name );
               send_to_char( buf, victim );
               victim->pcdata->super->bloodline = 0;
               do_save( victim, "auto" );
            }
         }
         else
         {
            send_to_char( "They are not here!!\r\n", ch );
         }
         return;
      }
   }

   send_to_char( "SYNTAX FOR COUNCIL:\r\n\r\n", ch );
   send_to_char( " COUNCIL <join/accept/outcast> <victim>\r\n", ch );
   return;
}
