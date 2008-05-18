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
/* This file deals with adding/deleting and manipulating areas
   as a whole, also checking on permissions and deals with area bank. */


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#ifndef DEC_MONEY_H
#include "money.h"
#endif

/* Variables declared in db.c, which we need */

extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
extern OBJ_INDEX_DATA *obj_index_hash[MAX_KEY_HASH];
extern ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];
extern char *string_hash[MAX_KEY_HASH];

extern char *string_space;
extern char *top_string;
extern char str_empty[1];

extern AREA_DATA *area_used[MAX_AREAS];

extern int top_affect;
extern int top_area;
extern int top_ed;
extern int top_exit;
extern int top_help;
extern int top_mob_index;
extern int top_obj_index;
extern int top_reset;
extern int top_room;
extern int top_shop;

#define                 MAX_STRING      1048576
#define                 MAX_PERM_BLOCK  131072
extern int nAllocString;
extern int sAllocString;
extern int nAllocPerm;
extern int sAllocPerm;
extern int fBootDb;

/* Some build.c functions : */
void build_strdup( char **dest, char *src, bool freesrc, CHAR_DATA * ch );
char *build_simpstrdup( char * );
ROOM_INDEX_DATA *new_room( AREA_DATA * pArea, sh_int vnum, sh_int sector );



int build_canread( AREA_DATA * Area, CHAR_DATA * ch, int showerror )
{
   if( get_trust( ch ) >= MAX_LEVEL - 1 )
      return 1;

   if( Area->can_read != NULL )
      if( is_name( "all", Area->can_read )
          || is_name( ch->name, Area->can_read ) || ( is_name( "gods", Area->can_read ) && IS_IMMORTAL( ch ) ) )
         return 1;

   if( showerror == AREA_SHOWERROR )
      send_to_char( "You are not allowed to use this area.\n\r", ch );

   return 0;
}

int build_canwrite( AREA_DATA * Area, CHAR_DATA * ch, int showerror )
{
   if( get_trust( ch ) >= MAX_LEVEL - 1 )
      return 1;

   if( Area->can_write != NULL )
      if( is_name( "all", Area->can_write )
          || is_name( ch->name, Area->can_write ) || ( is_name( "gods", Area->can_write ) && IS_IMMORTAL( ch ) ) )
         return 1;

   if( showerror == AREA_SHOWERROR )
      send_to_char( "You are not allowed to edit this area.\n\r", ch );

   return 0;
}

void build_save_area_list( void )
{
   AREA_DATA *pArea;
   FILE *fpArea;

   fpArea = fopen( "area.lst.new", "w" );

   if( fpArea == NULL )
   {
      bug( "Could not open area.lst.new for saving.", 0 );
      return;
   }

   for( pArea = first_area; pArea != NULL; pArea = pArea->next )
   {
      fprintf( fpArea, "%s\n\r", pArea->filename );
   }

   fprintf( fpArea, "$\n\r" );

   fclose( fpArea );

   /*
    * Save backup 
    */
   rename( "area.lst", "area.lst.old" );
   rename( "area.lst.new", "area.lst" );
}

void build_save_area_gold( void )
{
   AREA_DATA *pArea;
   FILE *fpArea;

   fpArea = fopen( "area.gld.new", "w" );

   if( fpArea == NULL )
   {
      bug( "Could not open area.gld.new for saving.", 0 );
      return;
   }

   for( pArea = first_area; pArea != NULL; pArea = pArea->next )
   {
      fprintf( fpArea, "%i %i\n\r", pArea->area_num, pArea->gold );
   }

   fprintf( fpArea, "-1\n\r" );

   fclose( fpArea );

   /*
    * Save backup 
    */
   rename( "area.gld", "area.gld.old" );
   rename( "area.gld.new", "area.gld" );
}

void build_makearea( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];


   char buf[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *pRoomIndex;
   int vnum = 0;
   int svnum;
   int envnum = 0;
   int mvnum = 0;
   int a;
   int iHash;
   int rooms;

   BUILD_DATA_LIST *pList;
   AREA_DATA *pArea = NULL;
   AREA_DATA *fpadd = NULL;
   AREA_DATA *lpadd = NULL;
   FILE *fpArea;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );


   strcpy( argument, arg2 );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "\n\rSyntax: makearea filename numrooms\n\r", ch );
      return;
   }
   rooms = atoi( arg2 );
   if( rooms < 3 || rooms > 300 )
   {

      send_to_char( "rooms between 3 and 300..:)\r\n", ch );
      sprintf( buf, "%d\r\n", rooms );
      send_to_char( buf, ch );
      return;

   }


   for( pArea = first_area; pArea; pArea = pArea->next )
   {

      fpadd = pArea;
      svnum = pArea->min_vnum;
      a = svnum - envnum - 1;
/*	sprintf(buf,"dif %d for rooms %d, %d-%d\r\n",a,rooms,pArea->min_vnum,pArea->max_vnum);
	send_to_char(buf,ch);
        sprintf(buf,"%s\r\n",pArea->name); 
      	send_to_char(buf,ch); */
      if( ( rooms <= a && buf != NULL ) )
      {
         send_to_char( "found one!\r\n", ch );
         vnum = envnum + 1;
         mvnum = vnum + rooms - 1;
         sprintf( buf, "\r\n Your start vnum :%d and ending vnum: %d :\r\n", vnum, mvnum );
         send_to_char( buf, ch );
         fpadd = pArea;
         break;
      }

      envnum = pArea->max_vnum;
      lpadd = pArea;
   }


   if( vnum == mvnum )
   {
      send_to_char( "Can't create a room with 0 rooms.\n\r", ch );
      return;
   }

   if( get_room_index( vnum ) != NULL )
   {
      send_to_char( "There is already a room with that vnum.\n\r", ch );
      return;
   }

   fpArea = fopen( arg1, "r" );
   if( fpArea != NULL )
   {
      send_to_char( "There is already a file with that name.\n\r", ch );
      fclose( fpArea );
      return;
   }

   fpArea = fopen( arg1, "w" );
   if( fpArea == NULL )
   {
      send_to_char( "Invalid filename, would not be able to save.\n\r", ch );
      return;
   }
   fclose( fpArea );

   /*
    * Find a unique area number 
    */
   for( a = 0; a < MAX_AREAS; a++ )
      if( area_used[a] == NULL )
         break;
   if( a == MAX_AREAS )
   {
      send_to_char( "Maximum number of areas already.\n\r", ch );
      return;
   }



   /*
    * set the new areanum, and resort the other area indexes. 
    * for(pArea=first_area;pArea;pArea=pArea->next)
    * {   
    * if (pArea->area_num==a)
    * b=TRUE;
    * 
    * if(b==TRUE)
    * pArea->area_num++;
    * 
    * 
    * }
    * 
    * for(pArea=first_area;pArea;pArea=pArea->next)
    * {  
    * sprintf(buf,"area#%d, area name: %s\r\n",pArea->area_num,pArea->name);
    * send_to_char(buf,ch);
    * 
    * }
    * 
    * Add area 
    */


   GET_FREE( pArea, area_free );
   pArea->area_num = a;
   pArea->first_reset = NULL;
   pArea->last_reset = NULL;
   pArea->name = str_dup( "New area." );
   pArea->level_label = str_dup( "{?? ??}" );
   pArea->keyword = str_dup( " none " );
   pArea->reset_msg = str_dup( "You here the screams of the Dead within your head." );
   pArea->reset_rate = 15;
   pArea->min_level = 1;
   pArea->max_level = 1;
   pArea->age = 15;
   pArea->offset = 0;
   pArea->nplayer = 0;
   pArea->modified = 1;
   pArea->filename = str_dup( arg1 );
   pArea->min_vnum = vnum;
   pArea->max_vnum = mvnum;
   pArea->owner = str_dup( ch->name );
   pArea->can_read = str_dup( ch->name );
   pArea->can_write = str_dup( ch->name );
   pArea->gold = 0;
   pArea->flags = AREA_NOSHOW;   /* don't list on 'areas' -S- */
   pArea->first_area_room = NULL;
   pArea->last_area_room = NULL;
   pArea->first_area_help_text = NULL;
   pArea->last_area_help_text = NULL;
   pArea->first_area_object = NULL;
   pArea->last_area_object = NULL;
   pArea->first_area_mobile = NULL;
   pArea->last_area_mobile = NULL;
   pArea->first_area_shop = NULL;
   pArea->last_area_shop = NULL;
   pArea->first_area_specfunc = NULL;
   pArea->last_area_specfunc = NULL;
   pArea->first_area_mobprog = NULL;
   pArea->last_area_mobprog = NULL;


   area_used[pArea->area_num] = pArea;

   LINK_BEFORE( pArea, fpadd, first_area, last_area, next, prev );

   top_area++;

   /*
    * Now add it to area.lst 
    */
   build_save_area_list(  );

   /*
    * Now add room 
    */
   pRoomIndex = new_room( pArea, vnum, SECT_INSIDE );
   /*
    * Add room to hash table 
    */


   iHash = vnum % MAX_KEY_HASH;
   SING_TOPLINK( pRoomIndex, room_index_hash[iHash], next );

   /*
    * Add room into area list. 
    */
   GET_FREE( pList, build_free );
   pList->data = pRoomIndex;
   LINK( pList, pArea->first_area_room, pArea->last_area_room, next, prev );
   top_room++;

   send_to_char( "Ok.\n\r", ch );
}

void build_addarea( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *pRoomIndex;
   int vnum;
   int a;
   int iHash;
   int door;
   BUILD_DATA_LIST *pList;
   AREA_DATA *pArea;
   FILE *fpArea;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   strcpy( arg3, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "\n\rSyntax: addarea filename vnum\n\r", ch );
      return;
   }

   vnum = is_number( arg2 ) ? atoi( arg2 ) : -1;

   if( vnum < 0 || vnum > MAX_VNUM )
   {
      send_to_char( "Vnum must be between 0 and 32767.\n\r", ch );
      return;
   }

   if( get_room_index( vnum ) != NULL )
   {
      send_to_char( "There is already a room with that vnum.\n\r", ch );
      return;
   }

   fpArea = fopen( arg1, "r" );
   if( fpArea != NULL )
   {
      send_to_char( "There is already a file with that name.\n\r", ch );
      fclose( fpArea );
      return;
   }

   fpArea = fopen( arg1, "w" );
   if( fpArea == NULL )
   {
      send_to_char( "Invalid filename, would not be able to save.\n\r", ch );
      return;
   }
   fclose( fpArea );

   /*
    * Find a unique area number 
    */
   for( a = 0; a < MAX_AREAS; a++ )
      if( area_used[a] == NULL )
         break;
   if( a == MAX_AREAS )
   {
      send_to_char( "Maximum number of areas already.\n\r", ch );
      return;
   }

   /*
    * Add area 
    */

   GET_FREE( pArea, area_free );
   pArea->area_num = a;
   pArea->first_reset = NULL;
   pArea->last_reset = NULL;
   pArea->name = str_dup( "New area." );
   pArea->level_label = str_dup( "{?? ??}" );
   pArea->keyword = str_dup( " none " );
   pArea->reset_msg = str_dup( "You here the screams of the Dead within your head." );
   pArea->reset_rate = 15;
   pArea->age = 15;
   pArea->offset = 0;
   pArea->nplayer = 0;
   pArea->modified = 1;
   pArea->filename = str_dup( arg1 );
   pArea->min_vnum = 0;
   pArea->max_vnum = MAX_VNUM;
   pArea->owner = NULL;
   pArea->can_read = NULL;
   pArea->can_write = NULL;
   pArea->gold = 0;
   pArea->flags = AREA_NOSHOW;   /* don't list on 'areas' -S- */
   pArea->first_area_room = NULL;
   pArea->last_area_room = NULL;
   pArea->first_area_help_text = NULL;
   pArea->last_area_help_text = NULL;
   pArea->first_area_object = NULL;
   pArea->last_area_object = NULL;
   pArea->first_area_mobile = NULL;
   pArea->last_area_mobile = NULL;
   pArea->first_area_shop = NULL;
   pArea->last_area_shop = NULL;
   pArea->first_area_specfunc = NULL;
   pArea->last_area_specfunc = NULL;
   pArea->first_area_mobprog = NULL;
   pArea->last_area_mobprog = NULL;


   area_used[pArea->area_num] = pArea;

   LINK( pArea, first_area, last_area, next, prev );

   top_area++;

   /*
    * Now add it to area.lst 
    */
   build_save_area_list(  );

   /*
    * Now add room 
    */
   GET_FREE( pRoomIndex, rid_free );
   pRoomIndex->first_person = NULL;
   pRoomIndex->last_person = NULL;
   pRoomIndex->first_content = NULL;
   pRoomIndex->last_content = NULL;
   pRoomIndex->first_exdesc = NULL;
   pRoomIndex->last_exdesc = NULL;
   pRoomIndex->area = pArea;
   pRoomIndex->vnum = vnum;
   pRoomIndex->name = str_dup( "New room" );
   pRoomIndex->description = str_dup( "No description" );
   pRoomIndex->room_flags = 0;
   pRoomIndex->sector_type = SECT_INSIDE;
   pRoomIndex->light = 0;
   for( door = 0; door <= 5; door++ )
      pRoomIndex->exit[door] = NULL;
   pRoomIndex->first_room_reset = NULL;
   pRoomIndex->last_room_reset = NULL;

   /*
    * Add room to hash table 
    */
   iHash = vnum % MAX_KEY_HASH;
   SING_TOPLINK( pRoomIndex, room_index_hash[iHash], next );

   /*
    * Add room into area list. 
    */
   GET_FREE( pList, build_free );
   pList->data = pRoomIndex;
   LINK( pList, pArea->first_area_room, pArea->last_area_room, next, prev );
   top_room++;

   send_to_char( "Ok.\n\r", ch );
}


void do_change_gold( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   int value;
   AREA_DATA *pArea;

   argument = one_argument( argument, arg1 );

   pArea = ch->in_room->area;

   if( !build_canwrite( pArea, ch, AREA_SHOWERROR ) )
      return;


   if( !is_number( arg1 ) )
   {
      send_to_char( "Must be a number.\n\r", ch );
      return;
   }

   value = atoi( arg1 );
   if( value > ch->gold )
   {
      send_to_char( "You cannot put in more gold than you have!\n\r", ch );
      return;
   }

   if( value < -pArea->gold )
   {
      send_to_char( "You cannot take out more gold than there is here!\n\r", ch );
      return;
   }

   pArea->gold = pArea->gold + value;
   ch->gold = ch->gold - value;

   build_save_area_gold(  );
   return;
}

void build_setarea( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char name[MAX_INPUT_LENGTH];
   char buffer[MAX_INPUT_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   char *argn, *oldperm;
   AREA_DATA *pArea;
   int num;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );

   if( ( str_cmp( arg1, "title" ) ) && ( str_cmp( arg1, "level_label" ) ) && ( str_cmp( arg1, "message" ) ) )
      argument = one_argument( argument, arg2 );
   else
      strcpy( arg2, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: setarea <arguments> \n\r", ch );
      send_to_char( "\n\r", ch );
      send_to_char( "Arguments being one of:\n\r", ch );
      send_to_char( "      owner       <name>  \n\r", ch );
      send_to_char( "      read        [-]<name>  \n\r", ch );
      send_to_char( "      write       [-]<name>  \n\r", ch );
      send_to_char( "      gold        <amount>\n\r", ch );
      send_to_char( "      title       <string>\n\r", ch );
      send_to_char( "      payarea     Yes/No  \n\r", ch );
      send_to_char( "      min         <min_vnum>\n\r", ch );
      send_to_char( "      max         <max_vnum>\n\r", ch );
      send_to_char( "      teleport    Yes/No  \n\r", ch );
      send_to_char( "      building    Yes/No  \n\r", ch );
      send_to_char( "      show        Yes/No  \n\r", ch );
      send_to_char( "	     room_spells On/Off \n\r", ch );
      send_to_char( "      min_level   <level>\n\r", ch );
      send_to_char( "      max_level   <level>\n\r", ch );
      send_to_char( "      level_label <label>\n\r", ch );
      send_to_char( "	     keyword     <keyword> \n\r", ch );
      send_to_char( "	     repop_rate  ticks( lower is faster ) \n\r", ch );
      send_to_char( "	     message <message> \n\r", ch );

      send_to_char( "@@Y      offset  <vnum>@@g\n\r", ch );
      return;
   }

   pArea = ch->in_room->area;

   area_modified( pArea );

   /*
    * Set something.
    */
   if( !str_cmp( arg1, "read" ) )
   {
      num = 1;
      argn = arg2;
      if( argn[0] == '-' )
      {
         num = 0;
         argn++;
      }

      if( num == 1 )
      {
         if( pArea->can_read == NULL )
         {
            pArea->can_read = str_dup( argn );
            return;
         }
         if( !is_name( argn, pArea->can_read ) )
         {
            sprintf( buffer, "%s %s", pArea->can_read, argn );
            free_string( pArea->can_read );
            pArea->can_read = str_dup( buffer );
         }
      }
      else
      {

         if( is_name( argn, pArea->can_read ) )
         {
            buffer[0] = '\0';

            strcpy( buf2, pArea->can_read );
            oldperm = buf2;

            oldperm = one_argument( oldperm, name );
            while( name[0] != '\0' )
            {
               if( str_cmp( name, argn ) )   /* i.e. not the same */
               {
                  strcat( buffer, name );
                  strcat( buffer, " " );
               }
               oldperm = one_argument( oldperm, name );
            }
            free_string( pArea->can_read );
            pArea->can_read = str_dup( buffer );
         }
      }
      return;
   }

   if( !str_cmp( arg1, "write" ) )
   {
      num = 1;
      argn = arg2;
      if( argn[0] == '-' )
      {
         num = 0;
         argn++;
      }

      if( num == 1 )
      {
         if( pArea->can_write == NULL )
         {
            pArea->can_write = str_dup( argn );
            return;
         }
         if( !is_name( argn, pArea->can_write ) )
         {
            sprintf( buffer, "%s %s", pArea->can_write, argn );
            free_string( pArea->can_write );
            pArea->can_write = str_dup( buffer );
         }
      }
      else if( is_name( argn, pArea->can_write ) )
      {
         buffer[0] = '\0';

         strcpy( buf2, pArea->can_write );
         oldperm = buf2;

         oldperm = one_argument( oldperm, name );
         while( name[0] != '\0' )
         {
            if( str_cmp( name, argn ) )   /* i.e. not the same */
            {
               strcat( buffer, name );
               strcat( buffer, " " );
            }
            oldperm = one_argument( oldperm, name );
         }
         free_string( pArea->can_write );
         pArea->can_write = str_dup( buffer );
      }
      return;
   }

   if( !str_cmp( arg1, "owner" ) )
   {
      build_strdup( &pArea->owner, arg2, TRUE, ch );
      return;
   }

   if( !str_cmp( arg1, "title" ) )
   {
      build_strdup( &pArea->name, argument, TRUE, ch );
      return;
   }
   if( !str_cmp( arg1, "level_label" ) )
   {
      build_strdup( &pArea->level_label, argument, TRUE, ch );
      return;
   }
   if( !str_cmp( arg1, "keyword" ) )
   {
      build_strdup( &pArea->keyword, arg2, TRUE, ch );
      return;
   }
   if( !str_cmp( arg1, "message" ) )
   {
      build_strdup( &pArea->reset_msg, arg2, TRUE, ch );
      return;
   }

   if( !str_cmp( arg1, "payarea" ) )
   {
      if( !str_cmp( arg2, "yes" ) )
      {
         SET_BIT( pArea->flags, AREA_PAYAREA );
         return;
      }

      if( !str_cmp( arg2, "no" ) )
      {
         REMOVE_BIT( pArea->flags, AREA_PAYAREA );
         return;
      }
   }

   if( !str_cmp( arg1, "teleport" ) )
   {
      if( !str_cmp( arg2, "yes" ) )
      {
         SET_BIT( pArea->flags, AREA_TELEPORT );
         return;
      }

      if( !str_cmp( arg2, "no" ) )
      {
         REMOVE_BIT( pArea->flags, AREA_TELEPORT );
         return;
      }
   }
   if( !str_cmp( arg1, "room_spells" ) )
   {
      if( !str_cmp( arg2, "on" ) )
      {
         REMOVE_BIT( pArea->flags, AREA_NO_ROOM_AFF );
         return;
      }

      if( !str_cmp( arg2, "off" ) )
      {
         SET_BIT( pArea->flags, AREA_NO_ROOM_AFF );
         return;
      }
   }
   if( !str_cmp( arg1, "building" ) )
   {
      if( !str_cmp( arg2, "yes" ) )
      {
         SET_BIT( pArea->flags, AREA_BUILDING );
         return;
      }

      if( !str_cmp( arg2, "no" ) )
      {
         REMOVE_BIT( pArea->flags, AREA_BUILDING );
         return;
      }
   }

   if( !str_cmp( arg1, "show" ) )
   {
      if( !str_cmp( arg2, "no" ) )
      {
         SET_BIT( pArea->flags, AREA_NOSHOW );
         send_to_char( "The area title will not be shown on the area list.\n\r", ch );
         return;
      }
      if( !str_cmp( arg2, "yes" ) )
      {
         REMOVE_BIT( pArea->flags, AREA_NOSHOW );
         send_to_char( "The area title will be shown on the area list.\n\r", ch );
         return;
      }
   }

   if( !str_cmp( arg1, "offset" ) )
   {
      if( is_number( arg2 ) )
      {
         pArea->offset = atoi( arg2 );
         return;
      }
      return;
   }

   if( !str_cmp( arg1, "gold" ) )
   {
      if( is_number( arg2 ) )
      {
         pArea->gold = atoi( arg2 );
         return;
      }
   }
   if( !str_cmp( arg1, "repop_rate" ) )
   {
      if( is_number( arg2 ) )
      {
         pArea->reset_rate = atoi( arg2 );
         return;
      }
   }

   if( !str_cmp( arg1, "min" ) )
   {
      if( is_number( arg2 ) )
      {
         pArea->min_vnum = atoi( arg2 );
         return;
      }
   }
   if( !str_cmp( arg1, "min_level" ) )
   {
      if( is_number( arg2 ) )
      {
         pArea->min_level = atoi( arg2 );
         return;
      }
   }
   if( !str_cmp( arg1, "max_level" ) )
   {
      if( is_number( arg2 ) )
      {
         pArea->max_level = atoi( arg2 );
         return;
      }
   }

   if( !str_cmp( arg1, "max" ) )
   {
      if( is_number( arg2 ) )
      {
         pArea->max_vnum = atoi( arg2 );
         return;
      }
   }
   /*
    * Generate usage message.
    */
   build_setarea( ch, "" );
   return;
}

void build_findarea( CHAR_DATA * ch, char *argument )
{
/*    extern int top_room_index; Unused Var */
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   AREA_DATA *pArea;
   ROOM_INDEX_DATA *pRoomIndex = NULL;
   int nMatch;
   bool fAll;
   bool found;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Find what area?\n\r", ch );
      return;
   }


   buf1[0] = '\0';
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   for( pArea = first_area; pArea != NULL; pArea = pArea->next )
   {
      nMatch++;
      if( ( fAll || is_name( arg, pArea->name ) ) && build_canread( pArea, ch, 0 ) )
      {
         found = TRUE;
         if( pArea->first_area_room != NULL )
            pRoomIndex = pArea->first_area_room->data;
         sprintf( buf, "[%5d] %s\n\r", pArea->first_area_room != NULL ? pRoomIndex->vnum : 0, pArea->name );
         strcat( buf1, buf );
      }
   }

   if( !found )
   {
      send_to_char( "No area like that.\n\r", ch );
      return;
   }

   send_to_char( buf1, ch );
   return;
}


void build_showarea( CHAR_DATA * ch, char *argument )
{
   AREA_DATA *pArea;
   char buf[MAX_STRING_LENGTH];
   char buffer[MAX_INPUT_LENGTH];

   pArea = ch->in_room->area;

   if( !build_canread( pArea, ch, 1 ) )
      return;

   buf[0] = '\0';

   sprintf( buffer, "\n\rTitle: %s\n\r", pArea->name );
   strcat( buf, buffer );
   sprintf( buffer, "Keyword: %s\n\r", pArea->keyword );
   strcat( buf, buffer );
   sprintf( buffer, "Level Label: %s\n\r", pArea->level_label );
   strcat( buf, buffer );
   sprintf( buffer, "Repop Rate: %i\n\r", pArea->reset_rate );
   strcat( buf, buffer );
   sprintf( buffer, "Reset Message: %s\n\r", pArea->reset_msg );
   strcat( buf, buffer );

   if( get_trust( ch ) >= MAX_LEVEL - 1 )
   {
      sprintf( buffer, "filename: %s\n\r", pArea->filename );
      strcat( buf, buffer );
   }

   sprintf( buffer, "OFFSET: %d\n\r", pArea->offset );
   strcat( buf, buffer );

   sprintf( buffer, "Owner: %s\n\rCan Read: %s\n\rCan Write: %s\n\r", pArea->owner, pArea->can_read, pArea->can_write );
   strcat( buf, buffer );

   sprintf( buffer, "Min Vnum: %5d    Max Vnum: %5d      Gold: %i\n\r", pArea->min_vnum, pArea->max_vnum, pArea->gold );
   strcat( buf, buffer );
   sprintf( buffer, "Min Level: %5d    Max Level: %5d \n\r", pArea->min_level, pArea->max_level );
   strcat( buf, buffer );

   if( IS_SET( pArea->flags, AREA_PAYAREA ) )
      strcat( buf, "This is a pay area.\n\r" );
   if( !IS_SET( pArea->flags, AREA_TELEPORT ) )
      strcat( buf, "You cannot teleport into here.\n\r" );
   if( IS_SET( pArea->flags, AREA_BUILDING ) )
      strcat( buf, "Area currently being built.\n\r" );

   if( IS_SET( pArea->flags, AREA_NOSHOW ) )
      strcat( buf, "Area title will not be shown on area list.\n\r" );
   else
      strcat( buf, "Area title will show on area list.\n\r" );
   if( IS_SET( pArea->flags, AREA_NO_ROOM_AFF ) )
      strcat( buf, "Bad Room Affect spells are not allowed.\n\r" );
   else
      strcat( buf, "Bad Room Affect spells may be used.\n\r" );

   send_to_char( buf, ch );
   return;
}

void build_arealist( CHAR_DATA * ch, char *argument )
{
   /*
    * -S- : Show list of areas, vnum range and owners. 
    */
   char buf[MAX_STRING_LENGTH];
   char msg[MAX_STRING_LENGTH];
   AREA_DATA *pArea;
   sh_int stop_counter = 0;

   buf[0] = '\0';
   sprintf( msg, "%s", "Areas of " mudnamecolor ":\n\r" );
   for( pArea = first_area; pArea != NULL; pArea = pArea->next )
   {
      sprintf( buf, "%12s [%5d to %5d] %s\n\r", capitalize( pArea->owner ), pArea->min_vnum, pArea->max_vnum, pArea->name );
      stop_counter++;
      if( stop_counter > 40 )
      {
         safe_strcat( MSL, msg, buf );
         stop_counter = 0;
         send_to_char( msg, ch );
         msg[0] = '\0';
      }
      else
      {
         safe_strcat( MAX_STRING_LENGTH, msg, buf );
      }
   }

   send_to_char( msg, ch );
   return;
}
