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

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
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

#ifndef DEC_RULERS_H
#include "h/rulers.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

void save_brands(  )
{

   FILE *fp;
   char brand_file_name[MAX_STRING_LENGTH];
   DL_LIST *brand;
   BRAND_DATA *this_brand;

   snprintf( brand_file_name, MSL, "%s", BRANDS_FILE );

   if( ( fp = file_open( brand_file_name, "w" ) ) == NULL )
   {
      bug( "Save brands list: file_open", 0 );
      perror( "failed open of brands.lst in save_brands" );
   }
   else
   {
      for( brand = first_brand; brand != NULL; brand = brand->next )
      {
         this_brand = (BRAND_DATA *)brand->this_one;
         fprintf( fp, "#BRAND~\n" );
         fprintf( fp, "%s~\n", this_brand->branded );
         fprintf( fp, "%s~\n", this_brand->branded_by );
         fprintf( fp, "%s~\n", this_brand->dt_stamp );
         fprintf( fp, "%s~\n", this_brand->message );
         fprintf( fp, "%s~\n", this_brand->priority );

      }
      fprintf( fp, "#END~\n\n" );
   }

   file_close(fp);

   return;

}

void load_brands( void )
{

   FILE *brandsfp;
   char brands_file_name[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   BRAND_DATA *this_brand;
   DL_LIST *brand_member;


   snprintf( brands_file_name, MSL, "%s", BRANDS_FILE );


   snprintf( buf, MSL, "Loading %s",BRANDS_FILE);
   log_f("%s",buf);

   if( ( brandsfp = file_open( brands_file_name, "r" ) ) == NULL )
   {
      bug( "Load brands Table: file_open", 0 );
      perror( "failed open of brands_table.dat in load_brands_table" );
   }
   else
   {
      for( ;; )
      {

         char *word;


         word = fread_string( brandsfp );
         if( !str_cmp( word, "#BRAND" ) )
         {
            this_brand = new BRAND_DATA;
            GET_FREE( brand_member, dl_list_free );
            this_brand->branded = fread_string( brandsfp );
            this_brand->branded_by = fread_string( brandsfp );
            this_brand->dt_stamp = fread_string( brandsfp );
            this_brand->message = fread_string( brandsfp );
            this_brand->priority = fread_string( brandsfp );

            free_string( word );

            brand_member->this_one = this_brand;
            brand_member->next = NULL;
            brand_member->prev = NULL;
            LINK( brand_member, first_brand, last_brand, next, prev );

         }
         else if( !str_cmp( word, "#END" ) )
         {
            free_string( word );
            break;
         }
         else
         {
            free_string( word );
            monitor_chan( "Load_brands: bad section.", MONITOR_BAD );
            break;
         }
      }

      file_close( brandsfp );

      log_f("Done.");

   }
}

void do_qpspend( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
/*    char arg3 [MAX_INPUT_LENGTH];  */
   char buf[MAX_STRING_LENGTH];
   char brandbuf[MSL];
   char catbuf[MSL];

   snprintf( brandbuf, MSL, "%s", "" );
   snprintf( catbuf, MSL, "%s", "" );

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
/*    argument = one_argument( argument, arg2 );  */

   if( IS_NPC( ch ) )
   {
      send_to_char( "Not for NPC's.\r\n", ch );
      return;
   }

   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax: qpspend enter | exit | assist <message> \r\n", ch );
      send_to_char( "or:     qpspend show | buy | clear | recall | home | corpse | rescue\r\n", ch );
      send_to_char( "or:     qpspend set name |short | long <message>\r\n", ch );
      send_to_char( "To set your enter or exit message, for example, type:\r\n", ch );
      send_to_char( "qpspend enter waddles in from\r\n", ch );
      send_to_char( "qpspend show to see a test, then qpspend buy\r\n", ch );
      return;
   }

   if( !str_cmp( arg1, "enter" ) )
   {
      if( str_cmp( ch->pcdata->pedit_state, "walk" ) )
      {
         free_string( ch->pcdata->pedit_string[0] );
         ch->pcdata->pedit_string[0] = str_dup( "none" );
         free_string( ch->pcdata->pedit_state );
         ch->pcdata->pedit_state = str_dup( "walk" );
      }
      free_string( ch->pcdata->pedit_string[0] );
      ch->pcdata->pedit_string[0] = str_dup( argument );
      return;
   }
   if( !str_cmp( arg1, "exit" ) )
   {
      if( str_cmp( ch->pcdata->pedit_state, "walk" ) )
      {
         free_string( ch->pcdata->pedit_string[0] );
         ch->pcdata->pedit_string[0] = str_dup( "none" );
         free_string( ch->pcdata->pedit_state );
         ch->pcdata->pedit_state = str_dup( "walk" );
      }

      free_string( ch->pcdata->pedit_string[1] );
      ch->pcdata->pedit_string[1] = str_dup( argument );
      return;
   }
   if( !str_cmp( arg1, "assist" ) )
   {
      if( str_cmp( ch->pcdata->pedit_state, "assist" ) )
      {
         free_string( ch->pcdata->pedit_string[0] );
         ch->pcdata->pedit_string[0] = str_dup( "none" );
         free_string( ch->pcdata->pedit_string[1] );
         ch->pcdata->pedit_string[1] = str_dup( "none" );
         free_string( ch->pcdata->pedit_state );
         ch->pcdata->pedit_state = str_dup( "assist" );
      }
      free_string( ch->pcdata->pedit_string[0] );
      if( !is_name( "*name*", argument ) )
      {
         send_to_char( "You must include *name* by itself somewhere in your assist message!\r\n", ch );
         ch->pcdata->pedit_string[0] = str_dup( "none" );
         return;
      }
      if( str_cmp( "assist", ch->pcdata->pedit_state ) )
      {
         free_string( ch->pcdata->pedit_state );
         ch->pcdata->pedit_state = str_dup( "assist" );
      }
      ch->pcdata->pedit_string[0] = str_dup( argument );
      return;
   }

   if( !str_cmp( arg1, "show" ) )
   {
      if( !str_cmp( ch->pcdata->pedit_state, "walk" ) )
      {
         char move_buf[MSL];
         char test_string[MSL];
         short qp_cost = 0;

         if( !str_cmp( ch->pcdata->pedit_string[0], "none" ) )
         {
            snprintf( test_string, MSL, "%s", ch->pcdata->room_enter );
         }
         else
         {
            snprintf( test_string, MSL, "%s", ch->pcdata->pedit_string[0] );
            qp_cost++;
         }
         snprintf( move_buf, MSL, "$L%s$n %s $T.", get_ruler_title( ch->pcdata->ruler_rank, ch->pcdata->login_sex ), test_string );
         act( move_buf, ch, NULL, rev_name[1], TO_CHAR );
         if( !str_cmp( ch->pcdata->pedit_string[1], "none" ) )
         {
            snprintf( test_string, MSL, "%s", ch->pcdata->room_exit );
         }
         else
         {
            snprintf( test_string, MSL, "%s", ch->pcdata->pedit_string[1] );
            qp_cost++;
         }


         snprintf( move_buf, MSL, "$L%s$n %s $T.", get_ruler_title( ch->pcdata->ruler_rank, ch->pcdata->login_sex ), test_string );
         act( move_buf, ch, NULL, dir_name[1], TO_CHAR );

         snprintf( buf, MSL, "Purchase cost is %d qps.\r\n", qp_cost );
         send_to_char( buf, ch );
         return;
      }
      else if( !str_cmp( ch->pcdata->pedit_state, "assist" ) )
      {
         snprintf( buf, MSL, "Current message:\r\n  %s\r\n",
                  ( !str_cmp( ch->pcdata->pedit_string[0], "none" ) ?
                    ch->pcdata->assist_msg : ch->pcdata->pedit_string[0] ) );
         send_to_char( buf, ch );
      }
      return;
   }

   if( !str_cmp( arg1, "buy" ) )
   {
      short qp_cost = 0;
      char *strtime;
      short i;

      if( !str_cmp( ch->pcdata->pedit_state, "walk" ) )
      {
         if( str_cmp( ch->pcdata->pedit_string[0], "none" ) )
            qp_cost++;
         if( str_cmp( ch->pcdata->pedit_string[1], "none" ) )
            qp_cost++;
         if( ch->pcdata->quest_points < qp_cost )
         {
            send_to_char( "You don't have enough quest points!\r\n", ch );
            for( i = 0; i < 5; i++ )
            {
               if( str_cmp( ch->pcdata->pedit_string[i], "none" ) )
               {
                  free_string( ch->pcdata->pedit_string[i] );
                  ch->pcdata->pedit_string[i] = str_dup( "none" );
               }
            }
            return;
         }
         else if( qp_cost == 0 )
         {
            send_to_char( "No changes.\r\n", ch );
            return;
         }
         else
         {
            if( str_cmp( ch->pcdata->pedit_string[0], "none" ) )
            {
               free_string( ch->pcdata->room_enter );
               ch->pcdata->room_enter = str_dup( ch->pcdata->pedit_string[0] );
               free_string( ch->pcdata->pedit_string[0] );
               ch->pcdata->pedit_string[0] = str_dup( "none" );
               snprintf( catbuf, MSL, "Enter message changed to %s\r\n", ch->pcdata->room_enter );
               strncat( brandbuf, catbuf, MSL-1 );
            }
            if( str_cmp( ch->pcdata->pedit_string[1], "none" ) )
            {
               free_string( ch->pcdata->room_exit );
               ch->pcdata->room_exit = str_dup( ch->pcdata->pedit_string[1] );
               free_string( ch->pcdata->pedit_string[1] );
               ch->pcdata->pedit_string[1] = str_dup( "none" );
               snprintf( catbuf, MSL, "Exit message changed to %s\r\n", ch->pcdata->room_exit );
               strncat( brandbuf, catbuf, MSL-1 );
            }
            free_string( ch->pcdata->pedit_string[0] );
            ch->pcdata->pedit_string[0] = str_dup( "none" );
            free_string( ch->pcdata->pedit_string[1] );
            ch->pcdata->pedit_string[1] = str_dup( "none" );
            free_string( ch->pcdata->pedit_state );
            ch->pcdata->pedit_state = str_dup( "none" );
            ch->pcdata->quest_points -= qp_cost;
            do_save( ch, "auto" );
            {
               BRAND_DATA *brand;
               DL_LIST *brand_member;

               brand = new BRAND_DATA;
               GET_FREE( brand_member, dl_list_free );
               brand->branded = str_dup( ch->name.c_str() );
               brand->branded_by = str_dup( "@@rSystem@@N" );
               brand->priority = str_dup( "normal" );
               brand->message = str_dup( brandbuf );
               strtime = ctime( &current_time );
               strtime[strlen( strtime ) - 1] = '\0';
               brand->dt_stamp = str_dup( strtime );
               brand_member->next = NULL;
               brand_member->prev = NULL;
               brand_member->this_one = brand;
               LINK( brand_member, first_brand, last_brand, next, prev );
               save_brands(  );
               send_to_char( "Your messages have been updated, and logged for inspection by an Immortal.\r\n", ch );
            }
            return;
         }
      }
      else if( !str_cmp( ch->pcdata->pedit_state, "assist" ) )
      {
         char assistbuf[MSL];
         char *parse = ch->pcdata->pedit_string[0];
         char word1[MSL];
         assistbuf[0] = '\0';
         if( ch->pcdata->quest_points < 3 )
         {
            send_to_char( "You do not have enough quest points!\r\n", ch );
            for( i = 0; i < 5; i++ )
            {
               if( str_cmp( ch->pcdata->pedit_string[i], "none" ) )
               {
                  free_string( ch->pcdata->pedit_string[i] );
                  ch->pcdata->pedit_string[i] = str_dup( "none" );
               }
               free_string( ch->pcdata->pedit_state );
               ch->pcdata->pedit_state = str_dup( "none" );
            }
            return;
         }
         for( ;; )
         {
            parse = one_word( parse, word1 );
            if( word1[0] == '\0' )
               break;
            if( !str_cmp( "*name*", word1 ) )
            {
               strncat( assistbuf, "$N ", MSL-1 );
               continue;
            }
            else
            {
               snprintf( catbuf, MSL, "%s ", word1 );
               strncat( assistbuf, catbuf, MSL-1 );
            }
         }
         free_string( ch->pcdata->assist_msg );
         ch->pcdata->assist_msg = str_dup( assistbuf );
         for( i = 0; i < 5; i++ )
         {
            if( str_cmp( ch->pcdata->pedit_string[i], "none" ) )
            {
               free_string( ch->pcdata->pedit_string[i] );
               ch->pcdata->pedit_string[i] = str_dup( "none" );
            }
         }
         free_string( ch->pcdata->pedit_state );
         ch->pcdata->pedit_state = str_dup( "none" );
         send_to_char( "Done!\r\n", ch );
         ch->pcdata->quest_points -= 3;
         do_save( ch, "auto" );
         {
            BRAND_DATA *brand;
            DL_LIST *brand_member;
            brand = new BRAND_DATA;
            GET_FREE( brand_member, dl_list_free );
            brand->branded = str_dup( ch->name.c_str() );
            brand->branded_by = str_dup( "@@rSystem@@N" );
            brand->priority = str_dup( "normal" );
            snprintf( brandbuf, MSL, "Assist message changed to %s\r\n", ch->pcdata->assist_msg );
            brand->message = str_dup( brandbuf );
            strtime = ctime( &current_time );
            strtime[strlen( strtime ) - 1] = '\0';
            brand->dt_stamp = str_dup( strtime );
            brand_member->next = NULL;
            brand_member->prev = NULL;
            brand_member->this_one = brand;
            LINK( brand_member, first_brand, last_brand, next, prev );
            save_brands(  );
            send_to_char( "Your messages have been updated, and logged for inspection by an Immortal.\r\n", ch );
         }
         return;
      }
      return;
   }
   if( !str_cmp( arg1, "rescue" ) )
   {
      if( ch->in_room->vnum == ROOM_VNUM_JAIL )
      {
         send_to_char( "No one can rescue you from here.\r\n", ch );
         return;
      }

      if( ch->pcdata->quest_points < 10 )
      {
         send_to_char( "You don't have enough quest points. Cost is 10 qp.\r\n", ch );
         return;
      }
      else
      {
         ROOM_INDEX_DATA *location;
         if( ( location = get_room_index( ch->pcdata->recall_vnum ) ) == NULL )
            location = get_room_index( ROOM_VNUM_TEMPLE );
         act( "@@mKline @@Wappears before you, and takes you to your Happy Place!", ch, NULL, NULL, TO_CHAR );
         act( "@@mKline @@Wappears before you, and takes $n to $s Happy Place!", ch, NULL, NULL, TO_ROOM );
         char_from_room( ch );
         char_to_room( ch, location );
         do_look( ch, "" );
         act( "$n steps into the room from a @@apulsating @@mvortex@@N.", ch, NULL, NULL, TO_ROOM );
         ch->pcdata->quest_points -= 10;
         do_save( ch, "auto" );
         return;
      }
   }
   if( !str_cmp( arg1, "home" ) )
   {
      if( ch->pcdata->quest_points < 50 )
      {
         send_to_char( "You don't have enough quest points. Cost is 50 qp.\r\n", ch );
         return;
      }
      else
      {
         if( ch->in_room->sector_type != SECT_RECALL_OK )
         {
            send_to_char( "This is not a legal location to call your home.\r\n", ch );
            return;
         }
         else if( ( ch->in_room == NULL ) || ( ch->in_room->vnum < 3 ) )
         {
            send_to_char( "You are LOST!\r\n", ch );
            return;
         }
         else
         {

            ch->pcdata->recall_vnum = ch->in_room->vnum;
            ch->pcdata->quest_points -= 50;
            do_save( ch, "auto" );
            send_to_char( "You know call this room your home, and will recall here!\r\n", ch );
            return;
         }
      }
   }

   if( !str_cmp( arg1, "corpse" ) )
   {
      if( ch->pcdata->quest_points < 10 )
      {
         send_to_char( "You don't have enough quest points. Cost is 10 qp.\r\n", ch );
         return;
      }
      else if( ( ch->in_room == NULL ) || ( ch->in_room->vnum < 3 ) )
      {
         send_to_char( "You are completly LOST!!\r\n", ch );
         return;
      }
      else
      {
         OBJ_DATA *obj;
         bool found = FALSE;
         list<OBJ_DATA *>::iterator li;

         for( li = obj_list.begin(); li != obj_list.end(); li++ )
         {
            obj = *li;
            if( ( ( obj->pIndexData->vnum ) == OBJ_VNUM_CORPSE_PC ) && ( !str_cmp( ch->name, obj->owner ) ) && ( !( obj->in_room == ch->in_room ) ) ) /*don't work! */
            {
               found = TRUE;
               obj_from_room( obj );
               obj_to_room( obj, ch->in_room );
               act( "Got the blighter!", ch, NULL, NULL, TO_CHAR );
            }
         }
         /*
          * act used to enable mobiles to check for CR triggers... 
          */

         if( !found )
         {
            act( "Couldn't find it.", ch, NULL, NULL, TO_CHAR );
         }
         ch->pcdata->quest_points -= 10;
         do_save( ch, "auto" );
         return;
      }
   }


   if( !str_cmp( arg1, "set" ) )
   {
      if( !is_name( arg2, "unique rename" ) )
      {
         send_to_char( "That is not a currently implemented editing state.\r\n", ch );
         return;
      }
      free_string( ch->pcdata->pedit_state );
      ch->pcdata->pedit_state = str_dup( arg2 );
      return;
   }
   if( !str_cmp( arg1, "clear" ) )
   {
      short i;
      for( i = 0; i < 5; i++ )
      {
         free_string( ch->pcdata->pedit_string[i] );
         ch->pcdata->pedit_string[i] = str_dup( "none" );
      }
      return;
   }

}


void do_immbrand( CHAR_DATA * ch, char *argument )
{
   DL_LIST *brand_list;
   DL_LIST *this_brand;
   BRAND_DATA *brand;
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH * 7];
   char arg[MAX_INPUT_LENGTH];
   int vnum = 0;
   int anum = 0;

   if( IS_NPC( ch ) )
      return;


   argument = one_argument( argument, arg );
   smash_tilde( argument );

   if( arg[0] == '\0' )
   {
      do_immbrand( ch, "read" );
      return;
   }

   if( !str_cmp( arg, "list" ) )
   {
      vnum = 0;
      buf1[0] = '\0';
      for( brand_list = first_brand; brand_list; brand_list = brand_list->next )
      {
         brand = (BRAND_DATA *)brand_list->this_one;
         snprintf( buf, MSL, "[%3d] @@r%s@@W: @@GBrander@@W: %s  @@a%s @@ePriority: %s@@N\r\n",
                  vnum, brand->branded, brand->branded_by, brand->dt_stamp, brand->priority );
         strncat( buf1, buf, MSL );
         vnum++;
         if( vnum > 100 )
         {
            strncat( buf1, "---More Follow---\r\n", MSL );
            break;
         }
      }

      if( vnum == 0 )
         send_to_char( "There are no outstanding brands.\r\n", ch );
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
      if( is_number( argument ) )
      {
         anum = atoi( argument );
      }
      else
      {
         send_to_char( "Read which brand?\r\n", ch );
         return;
      }

      vnum = 0;
      buf1[0] = '\0';
      for( brand_list = first_brand; brand_list; brand_list = brand_list->next )
      {
         if( vnum++ == anum )
         {
            brand = (BRAND_DATA *)brand_list->this_one;
            snprintf( buf, MSL, "[%3d] @@r%s@@W: @@GBrander@@W: %s  @@a%s @@ePriority: %s@@N\r\n",
                     anum, brand->branded, brand->branded_by, brand->dt_stamp, brand->priority );
            strncat( buf1, buf, MSL );
            strncat( buf1, brand->message, MSL );
            send_to_char( buf1, ch );
            return;
         }
         else
            continue;
         send_to_char( "No such brand.\r\n", ch );
         return;
      }
   }

   if( !str_cmp( arg, "write" ) || !str_cmp( arg, "edit" ) )
   {
      if( ch->pcdata->current_brand == NULL )
         ch->pcdata->current_brand = new BRAND_DATA;

      build_strdup( &ch->pcdata->current_brand->message, "$edit", TRUE, FALSE, ch );
      return;
   }



   if( !str_cmp( arg, "player" ) )
   {
      if( ch->pcdata->current_brand == NULL )
         ch->pcdata->current_brand = new BRAND_DATA;

      free_string( ch->pcdata->current_brand->branded );
      ch->pcdata->current_brand->branded = str_dup( argument );
      send_to_char( "Ok.\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "priority" ) )
   {
      if( ch->pcdata->current_brand == NULL )
         ch->pcdata->current_brand = new BRAND_DATA;

      free_string( ch->pcdata->current_brand->priority );
      ch->pcdata->current_brand->priority = str_dup( argument );
      send_to_char( "Ok.\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "clear" ) )
   {
      if( ch->pcdata->current_brand )
      {
         delete ch->pcdata->current_brand;
         ch->pcdata->current_brand = NULL;
      }
      save_brands(  );
      send_to_char( "Ok.\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "show" ) )
   {
      if( !ch->pcdata->current_brand )
      {
         send_to_char( "You have no brand in progress.\r\n", ch );
         return;
      }
      buf1[0] = '\0';
      snprintf( buf, MSL, "[%3d] %s: Brander: %s  Date: %s Priority: %s\r\n",
               vnum,
               ch->pcdata->current_brand->branded,
               ch->pcdata->current_brand->branded_by, ch->pcdata->current_brand->dt_stamp, ch->pcdata->current_brand->priority );
      strncat( buf1, buf, MSL );
      strncat( buf1, ch->pcdata->current_brand->message, MSL );
      send_to_char( buf1, ch );
      return;
   }

   if( !str_cmp( arg, "post" ) )
   {

      char *strtime;

      if( !ch->pcdata->current_brand )
      {
         send_to_char( "You have no brand in progress.\r\n", ch );
         return;
      }

      if( !str_cmp( ch->pcdata->current_brand->branded, "" ) )
      {
         send_to_char( "You need to provide a player name .\r\n", ch );
         return;
      }

      if( !str_cmp( ch->pcdata->current_brand->message, "" ) )
      {
         send_to_char( "You need to provide a message.\r\n", ch );
         return;
      }

      strtime = ctime( &current_time );
      strtime[strlen( strtime ) - 1] = '\0';
      free_string( ch->pcdata->current_brand->dt_stamp );
      ch->pcdata->current_brand->dt_stamp = str_dup( strtime );
      free_string( ch->pcdata->current_brand->branded_by );
      ch->pcdata->current_brand->branded_by = str_dup( ch->name.c_str() );
      GET_FREE( this_brand, dl_list_free );
      this_brand->next = NULL;
      this_brand->prev = NULL;
      this_brand->this_one = ch->pcdata->current_brand;
      LINK( this_brand, first_brand, last_brand, next, prev );
      ch->pcdata->current_brand = NULL;
      save_brands(  );
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
      for( brand_list = first_brand; brand_list; brand_list = brand_list->next )
      {
         if( vnum++ == anum )
         {
            break;
         }
      }
      if( brand_list != NULL )
      {
         UNLINK( brand_list, first_brand, last_brand, next, prev );
         brand = (BRAND_DATA *)brand_list->this_one;
         delete brand;
         brand_list->this_one = NULL;
         PUT_FREE( brand_list, dl_list_free );
         save_brands(  );
         return;
      }

      send_to_char( "No such brand.\r\n", ch );
      return;
   }

   send_to_char( "Huh?  Type 'help letter' for usage.\r\n", ch );
   return;
}

void do_statraise(CHAR_DATA *ch, char *argument)
{
 int cost = 5;
 int statcap = 25;
 char buf[MSL];

 /* Note, you'll need to edit your stat tables in const.c
  * to give bonuses past 25 for each stat, based on whatever
  * limit you set in this function --Kline
  */

 if( IS_NPC(ch) ) /* Not on mobs */
  return;

 if( argument[0] == '\0' ) /* No arg supplied, show the syntax */
 {
  send_to_char("Syntax is: statraise <str/int/wis/dex/con>\r\n",ch);
  return;
 }
 if( !str_prefix(argument,"str") ) /* Matches on 's', 'st', 'str' */
 {
  if( ch->pcdata->max_str >= statcap )
  {
   snprintf(buf,MSL,"You can not raise your Str any further. The limit is %d.\r\n",statcap);
   send_to_char(buf,ch);
   return;
  }
  cost *= ch->pcdata->max_str; /* Take base cost (5) mult by current value */
  if( ch->pcdata->quest_points < cost )
  {
   snprintf(buf,MSL,"It costs %d QP to raise your Str, but you only have %d.\r\n",cost,ch->pcdata->quest_points);
   send_to_char(buf,ch);
   return;
  }
  ch->pcdata->max_str++; /* Raise the actual cap */
  ch->pcdata->perm_str++; /* Give them a free stat train */
  ch->pcdata->quest_points -= cost;
  snprintf(buf,MSL,"You have spent %d QP to raise your Str! It is now %d and you have %d QP remaining.\r\n",cost,ch->pcdata->max_str,ch->pcdata->quest_points);
  send_to_char(buf,ch);
  return;
 }
 else if( !str_prefix(argument,"int") ) /* Matches on 'i', 'in', 'int' */
 {
  if( ch->pcdata->max_int >= statcap )
  {
   snprintf(buf,MSL,"You can not raise your Int any further. The limit is %d.\r\n",statcap);
   send_to_char(buf,ch);
   return;
  }
  cost *= ch->pcdata->max_int; /* Take base cost (5) mult by current value */
  if( ch->pcdata->quest_points < cost )
  {
   snprintf(buf,MSL,"It costs %d QP to raise your Int, but you only have %d.\r\n",cost,ch->pcdata->quest_points);
   send_to_char(buf,ch);
   return;
  }
  ch->pcdata->max_int++; /* Raise the actual cap */
  ch->pcdata->perm_int++; /* Give them a free stat train */
  ch->pcdata->quest_points -= cost;
  snprintf(buf,MSL,"You have spent %d QP to raise your Int! It is now %d and you have %d QP remaining.\r\n",cost,ch->pcdata->max_int,ch->pcdata->quest_points);
  send_to_char(buf,ch);
  return;
 }
 else if( !str_prefix(argument,"wis") ) /* Matches on 'w', 'wi', 'wis' */
 {
  if( ch->pcdata->max_wis >= statcap )
  {
   snprintf(buf,MSL,"You can not raise your Wis any further. The limit is %d.\r\n",statcap);
   send_to_char(buf,ch);
   return;
  }
  cost *= ch->pcdata->max_wis; /* Take base cost (5) mult by current value */
  if( ch->pcdata->quest_points < cost )
  {
   snprintf(buf,MSL,"It costs %d QP to raise your Wis, but you only have %d.\r\n",cost,ch->pcdata->quest_points);
   send_to_char(buf,ch);
   return;
  }
  ch->pcdata->max_wis++; /* Raise the actual cap */
  ch->pcdata->perm_wis++; /* Give them a free stat train */
  ch->pcdata->quest_points -= cost;
  snprintf(buf,MSL,"You have spent %d QP to raise your Wis! It is now %d and you have %d QP remaining.\r\n",cost,ch->pcdata->max_wis,ch->pcdata->quest_points);
  send_to_char(buf,ch);
  return;
 }
 else if( !str_prefix(argument,"dex") ) /* Matches on 'd', 'de', 'dex' */
 {
  if( ch->pcdata->max_dex >= statcap )
  {
   snprintf(buf,MSL,"You can not raise your Dex any further. The limit is %d.\r\n",statcap);
   send_to_char(buf,ch);
   return;
  }
  cost *= ch->pcdata->max_dex; /* Take base cost (5) mult by current value */
  if( ch->pcdata->quest_points < cost )
  {
   snprintf(buf,MSL,"It costs %d QP to raise your Dex, but you only have %d.\r\n",cost,ch->pcdata->quest_points);
   send_to_char(buf,ch);
   return;
  }
  ch->pcdata->max_dex++; /* Raise the actual cap */
  ch->pcdata->perm_dex++; /* Give them a free stat train */
  ch->pcdata->quest_points -= cost;
  snprintf(buf,MSL,"You have spent %d QP to raise your Dex! It is now %d and you have %d QP remaining.\r\n",cost,ch->pcdata->max_dex,ch->pcdata->quest_points);
  send_to_char(buf,ch);
  return;
 }
 else if( !str_prefix(argument,"con") ) /* Matches on 'c', 'co', 'con' */
 {
  if( ch->pcdata->max_con >= statcap )
  {
   snprintf(buf,MSL,"You can not raise your Con any further. The limit is %d.\r\n",statcap);
   send_to_char(buf,ch);
   return;
  }
  cost *= ch->pcdata->max_con; /* Take base cost (5) mult by current value */
  if( ch->pcdata->quest_points < cost )
  {
   snprintf(buf,MSL,"It costs %d QP to raise your Con, but you only have %d.\r\n",cost,ch->pcdata->quest_points);
   send_to_char(buf,ch);
   return;
  }
  ch->pcdata->max_con++; /* Raise the actual cap */
  ch->pcdata->perm_con++; /* Give them a free stat train */
  ch->pcdata->quest_points -= cost;
  snprintf(buf,MSL,"You have spent %d QP to raise your Con! It is now %d and you have %d QP remaining.\r\n",cost,ch->pcdata->max_con,ch->pcdata->quest_points);
  send_to_char(buf,ch);
  return;
 }
 else /* Invalid argument, display syntax */
 {
  send_to_char("Syntax is: statraise <str/int/wis/dex/con>\r\n",ch);
  return;
 }
}
