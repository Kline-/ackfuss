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


void save_brands(  )
{

   FILE *fp;
   char brand_file_name[MAX_STRING_LENGTH];
   DL_LIST *brand;
   BRAND_DATA *this_brand;


   fclose( fpReserve );
   sprintf( brand_file_name, "%s", BRANDS_FILE );

   if( ( fp = fopen( brand_file_name, "w" ) ) == NULL )
   {
      bug( "Save brands list: fopen", 0 );
      perror( "failed open of brands.lst in save_brands" );
   }
   else
   {
      for( brand = first_brand; brand != NULL; brand = brand->next )
      {
         this_brand = brand->this_one;
         fprintf( fp, "#BRAND~\n" );
         fprintf( fp, "%s~\n", this_brand->branded );
         fprintf( fp, "%s~\n", this_brand->branded_by );
         fprintf( fp, "%s~\n", this_brand->dt_stamp );
         fprintf( fp, "%s~\n", this_brand->message );
         fprintf( fp, "%s~\n", this_brand->priority );

      }
      fprintf( fp, "#END~\n\n" );
   }


   fflush( fp );
   fclose( fp );


   fpReserve = fopen( NULL_FILE, "r" );
   return;

}

void load_brands( void )
{

   FILE *brandsfp;
   char brands_file_name[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   BRAND_DATA *this_brand;
   DL_LIST *brand_member;


   sprintf( brands_file_name, "%s", BRANDS_FILE );


   sprintf( buf, "Loading %s\n\r", brands_file_name );
   monitor_chan( buf, MONITOR_CLAN );

   if( ( brandsfp = fopen( brands_file_name, "r" ) ) == NULL )
   {
      bug( "Load brands Table: fopen", 0 );
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
            GET_FREE( this_brand, brand_data_free );
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

      fclose( brandsfp );

      sprintf( buf, "Done Loading %s\n\r", brands_file_name );
      monitor_chan( buf, MONITOR_CLAN );

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

   sprintf( brandbuf, "%s", "" );
   sprintf( catbuf, "%s", "" );

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
/*    argument = one_argument( argument, arg2 );  */

   if( IS_NPC( ch ) )
   {
      send_to_char( "Not for NPC's.\n\r", ch );
      return;
   }

   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax: qpspend enter | exit | assist <message> \n\r", ch );
      send_to_char( "or:     qpspend show | buy | clear | recall | home | corpse | rescue\n\r", ch );
      send_to_char( "or:     qpspend set name |short | long <message>\n\r", ch );
      send_to_char( "To set your enter or exit message, for example, type:\n\r", ch );
      send_to_char( "qpspend enter waddles in from\n\r", ch );
      send_to_char( "qpspend show to see a test, then qpspend buy\n\r", ch );
      return;
   }
#if 0
   if( !str_cmp( arg1, "state" ) )
   {
      argument = one_argument( argument, arg2 );
      if( !is_name( arg2, "unique rename" ) )
      {
         send_to_char( "That is not a currently implemented editing state.\n\r", ch );
         return;
      }
      free_string( ch->pcdata->pedit_state );
      ch->pcdata->pedit_state = str_dup( arg2 );
      return;
   }
#endif


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
         send_to_char( "You must include *name* by itself somewhere in your assist message!\n\r", ch );
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
         sh_int qp_cost = 0;

         if( !str_cmp( ch->pcdata->pedit_string[0], "none" ) )
         {
            sprintf( test_string, ch->pcdata->room_enter );
         }
         else
         {
            sprintf( test_string, ch->pcdata->pedit_string[0] );
            qp_cost++;
         }
         sprintf( move_buf, "$L%s$n %s $T.", get_ruler_title( ch->pcdata->ruler_rank, ch->login_sex ), test_string );
         act( move_buf, ch, NULL, rev_name[1], TO_CHAR );
         if( !str_cmp( ch->pcdata->pedit_string[1], "none" ) )
         {
            sprintf( test_string, ch->pcdata->room_exit );
         }
         else
         {
            sprintf( test_string, ch->pcdata->pedit_string[1] );
            qp_cost++;
         }


         sprintf( move_buf, "$L%s$n %s $T.", get_ruler_title( ch->pcdata->ruler_rank, ch->login_sex ), test_string );
         act( move_buf, ch, NULL, dir_name[1], TO_CHAR );

         sprintf( buf, "Purchase cost is %d qps.\n\r", qp_cost );
         send_to_char( buf, ch );
         return;
      }
      else if( !str_cmp( ch->pcdata->pedit_state, "assist" ) )
      {
         sprintf( buf, "Current message:\n\r  %s\n\r",
                  ( !str_cmp( ch->pcdata->pedit_string[0], "none" ) ?
                    ch->pcdata->assist_msg : ch->pcdata->pedit_string[0] ) );
         send_to_char( buf, ch );
      }
      return;
   }

   if( !str_cmp( arg1, "buy" ) )
   {
      sh_int qp_cost = 0;
      char *strtime;
      sh_int i;

      if( !str_cmp( ch->pcdata->pedit_state, "walk" ) )
      {
         if( str_cmp( ch->pcdata->pedit_string[0], "none" ) )
            qp_cost++;
         if( str_cmp( ch->pcdata->pedit_string[1], "none" ) )
            qp_cost++;
         if( ch->quest_points < qp_cost )
         {
            send_to_char( "You don't have enough quest points!\n\r", ch );
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
            send_to_char( "No changes.\n\r", ch );
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
               sprintf( catbuf, "Enter message changed to %s\n\r", ch->pcdata->room_enter );
               safe_strcat( MSL, brandbuf, catbuf );
            }
            if( str_cmp( ch->pcdata->pedit_string[1], "none" ) )
            {
               free_string( ch->pcdata->room_exit );
               ch->pcdata->room_exit = str_dup( ch->pcdata->pedit_string[1] );
               free_string( ch->pcdata->pedit_string[1] );
               ch->pcdata->pedit_string[1] = str_dup( "none" );
               sprintf( catbuf, "Exit message changed to %s\n\r", ch->pcdata->room_exit );
               safe_strcat( MSL, brandbuf, catbuf );
            }
            free_string( ch->pcdata->pedit_string[0] );
            ch->pcdata->pedit_string[0] = str_dup( "none" );
            free_string( ch->pcdata->pedit_string[1] );
            ch->pcdata->pedit_string[1] = str_dup( "none" );
            free_string( ch->pcdata->pedit_state );
            ch->pcdata->pedit_state = str_dup( "none" );
            ch->quest_points -= qp_cost;
            do_save( ch, "" );
            {
               BRAND_DATA *brand;
               DL_LIST *brand_member;

               GET_FREE( brand, brand_data_free );
               GET_FREE( brand_member, dl_list_free );
               brand->branded = str_dup( ch->name );
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
               send_to_char( "Your messages have been updated, and logged for inspection by an Immortal.\n\r", ch );
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
         if( ch->quest_points < 3 )
         {
            send_to_char( "You do not have enough quest points!\n\r", ch );
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
               safe_strcat( MSL, assistbuf, "$N " );
               continue;
            }
            else
            {
               sprintf( catbuf, "%s ", word1 );
               safe_strcat( MSL, assistbuf, catbuf );
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
         send_to_char( "Done!\n\r", ch );
         ch->quest_points -= 3;
         do_save( ch, "" );
         {
            BRAND_DATA *brand;
            DL_LIST *brand_member;
            GET_FREE( brand, brand_data_free );
            GET_FREE( brand_member, dl_list_free );
            brand->branded = str_dup( ch->name );
            brand->branded_by = str_dup( "@@rSystem@@N" );
            brand->priority = str_dup( "normal" );
            sprintf( brandbuf, "Assist message changed to %s\n\r", ch->pcdata->assist_msg );
            brand->message = str_dup( brandbuf );
            strtime = ctime( &current_time );
            strtime[strlen( strtime ) - 1] = '\0';
            brand->dt_stamp = str_dup( strtime );
            brand_member->next = NULL;
            brand_member->prev = NULL;
            brand_member->this_one = brand;
            LINK( brand_member, first_brand, last_brand, next, prev );
            save_brands(  );
            send_to_char( "Your messages have been updated, and logged for inspection by an Immortal.\n\r", ch );
         }
         return;
      }
      return;
   }
   if( !str_cmp( arg1, "rescue" ) )
   {
      if( ch->in_room->vnum == ROOM_VNUM_JAIL )
      {
         send_to_char( "No one can rescue you from here.\n\r", ch );
         return;
      }

      if( ch->quest_points < 10 )
      {
         send_to_char( "You don't have enough quest points. Cost is 10 qp.\n\r", ch );
         return;
      }
      else
      {
         ROOM_INDEX_DATA *location;
         if( ( location = get_room_index( ch->pcdata->recall_vnum ) ) == NULL )
            location = get_room_index( 3001 );
         act( "@@mZenithar @@Wappears before you, and takes you to your Happy Place!", ch, NULL, NULL, TO_CHAR );
         act( "@@mZenithar @@Wappears before you, and takes $n to $s Happy Place!", ch, NULL, NULL, TO_ROOM );
         char_from_room( ch );
         char_to_room( ch, location );
         do_look( ch, "" );
         act( "$n steps into the room from a @@apulsating @@mvortex@@N.", ch, NULL, NULL, TO_ROOM );
         ch->quest_points -= 10;
         do_save( ch, "" );
         return;
      }
   }
   if( !str_cmp( arg1, "home" ) )
   {
      if( ch->quest_points < 50 )
      {
         send_to_char( "You don't have enough quest points. Cost is 50 qp.\n\r", ch );
         return;
      }
      else
      {
         if( ch->in_room->sector_type != SECT_RECALL_OK )
         {
            send_to_char( "This is not a legal location to call your home.\n\r", ch );
            return;
         }
         else if( ( ch->in_room == NULL ) || ( ch->in_room->vnum < 3 ) )
         {
            send_to_char( "You are LOST!\n\r", ch );
            return;
         }
         else
         {

            ch->pcdata->recall_vnum = ch->in_room->vnum;
            ch->quest_points -= 50;
            do_save( ch, "" );
            send_to_char( "You know call this room your home, and will recall here!\n\r", ch );
            return;
         }
      }
   }

   if( !str_cmp( arg1, "corpse" ) )
   {
      if( ch->quest_points < 10 )
      {
         send_to_char( "You don't have enough quest points. Cost is 10 qp.\n\r", ch );
         return;
      }
      else if( ( ch->in_room == NULL ) || ( ch->in_room->vnum < 3 ) )
      {
         send_to_char( "You are completly LOST!!\n\r", ch );
         return;
      }
      else
      {
         OBJ_DATA *obj;
         bool found = FALSE;

         for( obj = first_obj; obj != NULL; obj = obj->next )
         {
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
         ch->quest_points -= 10;
         do_save( ch, "" );
         return;
      }
   }


   if( !str_cmp( arg1, "set" ) )
   {
      if( !is_name( arg2, "unique rename" ) )
      {
         send_to_char( "That is not a currently implemented editing state.\n\r", ch );
         return;
      }
      free_string( ch->pcdata->pedit_state );
      ch->pcdata->pedit_state = str_dup( arg2 );
      return;
   }
   if( !str_cmp( arg1, "clear" ) )
   {
      sh_int i;
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
         brand = brand_list->this_one;
         sprintf( buf, "[%3d] @@r%s@@W: @@GBrander@@W: %s  @@a%s @@ePriority: %s@@N\n\r",
                  vnum, brand->branded, brand->branded_by, brand->dt_stamp, brand->priority );
         safe_strcat( MSL, buf1, buf );
         vnum++;
         if( vnum > 100 )
         {
            safe_strcat( MSL, buf1, "---More Follow---\n\r" );
            break;
         }
      }

      if( vnum == 0 )
         send_to_char( "There are no outstanding brands.\n\r", ch );
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
         send_to_char( "Read which brand?\n\r", ch );
         return;
      }

      vnum = 0;
      buf1[0] = '\0';
      for( brand_list = first_brand; brand_list; brand_list = brand_list->next )
      {
         if( vnum++ == anum )
         {
            brand = brand_list->this_one;
            sprintf( buf, "[%3d] @@r%s@@W: @@GBrander@@W: %s  @@a%s @@ePriority: %s@@N\n\r",
                     anum, brand->branded, brand->branded_by, brand->dt_stamp, brand->priority );
            safe_strcat( MSL, buf1, buf );
            safe_strcat( MSL, buf1, brand->message );
            send_to_char( buf1, ch );
            return;
         }
         else
            continue;
         send_to_char( "No such brand.\n\r", ch );
         return;
      }
   }

   if( !str_cmp( arg, "write" ) || !str_cmp( arg, "edit" ) )
   {
      if( ch->current_brand == NULL )
      {
         GET_FREE( ch->current_brand, brand_data_free );
         ch->current_brand->branded = str_dup( "" );
         ch->current_brand->branded_by = str_dup( "" );
         ch->current_brand->message = str_dup( "" );
         ch->current_brand->dt_stamp = str_dup( "" );
         ch->current_brand->priority = str_dup( "" );
      }

      build_strdup( &ch->current_brand->message, "$edit", TRUE, ch );
      return;
   }



   if( !str_cmp( arg, "player" ) )
   {
      if( ch->current_brand == NULL )
      {
         GET_FREE( ch->current_brand, brand_data_free );
         ch->current_brand->branded = str_dup( "" );
         ch->current_brand->branded_by = str_dup( "" );
         ch->current_brand->message = str_dup( "" );
         ch->current_brand->dt_stamp = str_dup( "" );
         ch->current_brand->priority = str_dup( "" );
      }

      free_string( ch->current_brand->branded );
      ch->current_brand->branded = str_dup( argument );
      send_to_char( "Ok.\n\r", ch );
      return;
   }

   if( !str_cmp( arg, "priority" ) )
   {
      if( ch->current_brand == NULL )
      {
         GET_FREE( ch->current_brand, brand_data_free );
         ch->current_brand->branded = str_dup( "" );
         ch->current_brand->branded_by = str_dup( "" );
         ch->current_brand->message = str_dup( "" );
         ch->current_brand->dt_stamp = str_dup( "" );
         ch->current_brand->priority = str_dup( "" );
      }
      free_string( ch->current_brand->priority );
      ch->current_brand->priority = str_dup( argument );
      send_to_char( "Ok.\n\r", ch );
      return;
   }

   if( !str_cmp( arg, "clear" ) )
   {
      if( ch->current_brand )
      {
         PUT_FREE( ch->current_brand, brand_data_free );
         ch->current_brand = NULL;
      }
      save_brands(  );
      send_to_char( "Ok.\n\r", ch );
      return;
   }

   if( !str_cmp( arg, "show" ) )
   {
      if( !ch->current_brand )
      {
         send_to_char( "You have no brand in progress.\n\r", ch );
         return;
      }
      buf1[0] = '\0';
      sprintf( buf, "[%3d] %s: Brander: %s  Date: %s Priority: %s\n\r",
               vnum,
               ch->current_brand->branded,
               ch->current_brand->branded_by, ch->current_brand->dt_stamp, ch->current_brand->priority );
      strcat( buf1, buf );
      strcat( buf1, ch->current_brand->message );
      send_to_char( buf1, ch );
      return;
   }

   if( !str_cmp( arg, "post" ) )
   {

      char *strtime;

      if( !ch->current_brand )
      {
         send_to_char( "You have no brand in progress.\n\r", ch );
         return;
      }

      if( !str_cmp( ch->current_brand->branded, "" ) )
      {
         send_to_char( "You need to provide a player name .\n\r", ch );
         return;
      }

      if( !str_cmp( ch->current_brand->message, "" ) )
      {
         send_to_char( "You need to provide a message.\n\r", ch );
         return;
      }

      strtime = ctime( &current_time );
      strtime[strlen( strtime ) - 1] = '\0';
      free_string( ch->current_brand->dt_stamp );
      ch->current_brand->dt_stamp = str_dup( strtime );
      free_string( ch->current_brand->branded_by );
      ch->current_brand->branded_by = str_dup( ch->name );
      GET_FREE( this_brand, dl_list_free );
      this_brand->next = NULL;
      this_brand->prev = NULL;
      this_brand->this_one = ch->current_brand;
      LINK( this_brand, first_brand, last_brand, next, prev );
      ch->current_brand = NULL;
      save_brands(  );
      send_to_char( "Ok.\n\r", ch );
      return;
   }

   if( !str_cmp( arg, "remove" ) )
   {
      if( !is_number( argument ) )
      {
         send_to_char( "Remove which letter number?\n\r", ch );
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
         brand = brand_list->this_one;
         PUT_FREE( brand, brand_data_free );
         brand_list->this_one = NULL;
         PUT_FREE( brand_list, dl_list_free );
         save_brands(  );
         return;
      }

      send_to_char( "No such brand.\n\r", ch );
      return;
   }

   send_to_char( "Huh?  Type 'help letter' for usage.\n\r", ch );
   return;
}
