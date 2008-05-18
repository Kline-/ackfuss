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

/*
 *  This program goes through each area. It checks all vnums to see if
 *  they are in the allowed range. If not it moves them, and updates all
 *  references.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "hash.h"



void swap_global_hash( char Tp, void *Ptr, int old_vnum, int new_vnum )
{
   ROOM_INDEX_DATA *pRoomIndex, *prevRoomIndex;
   OBJ_INDEX_DATA *pObjIndex, *prevObjIndex;
   MOB_INDEX_DATA *pMobIndex, *prevMobIndex;

   extern ROOM_INDEX_DATA *room_index_hash[];
   extern OBJ_INDEX_DATA *obj_index_hash[];
   extern MOB_INDEX_DATA *mob_index_hash[];

   int iHash;


   switch ( Tp )
   {
      case 'R':
         /*
          * Delete old hash table entry 
          */

         iHash = old_vnum % MAX_KEY_HASH;
         prevRoomIndex = NULL;
         for( pRoomIndex = room_index_hash[iHash]; pRoomIndex != NULL; pRoomIndex = pRoomIndex->next )
         {
            if( pRoomIndex == ( ROOM_INDEX_DATA * ) Ptr )
               break;
            prevRoomIndex = pRoomIndex;
         }

         if( pRoomIndex != NULL )
         {
            if( prevRoomIndex == NULL )
               room_index_hash[iHash] = pRoomIndex->next;
            else
               prevRoomIndex->next = pRoomIndex->next;
         }

         /*
          * Add another 
          */

         iHash = new_vnum % MAX_KEY_HASH;
         pRoomIndex->next = room_index_hash[iHash];
         room_index_hash[iHash] = pRoomIndex;

         break;

      case 'M':
         /*
          * Delete old hash table entry 
          */

         iHash = old_vnum % MAX_KEY_HASH;
         prevMobIndex = NULL;
         for( pMobIndex = mob_index_hash[iHash]; pMobIndex != NULL; pMobIndex = pMobIndex->next )
         {
            if( pMobIndex == ( MOB_INDEX_DATA * ) Ptr )
               break;
            prevMobIndex = pMobIndex;
         }

         if( pMobIndex != NULL )
         {
            if( prevMobIndex == NULL )
               mob_index_hash[iHash] = pMobIndex->next;
            else
               prevMobIndex->next = pMobIndex->next;
         }

         /*
          * Add another 
          */

         iHash = new_vnum % MAX_KEY_HASH;
         pMobIndex->next = mob_index_hash[iHash];
         mob_index_hash[iHash] = pMobIndex;

         break;

      case 'O':
         /*
          * Delete old hash table entry 
          */

         iHash = old_vnum % MAX_KEY_HASH;
         prevObjIndex = NULL;
         for( pObjIndex = obj_index_hash[iHash]; pObjIndex != NULL; pObjIndex = pObjIndex->next )
         {
            if( pObjIndex == ( OBJ_INDEX_DATA * ) Ptr )
               break;
            prevObjIndex = pObjIndex;
         }

         if( pObjIndex != NULL )
         {
            if( prevObjIndex == NULL )
               obj_index_hash[iHash] = pObjIndex->next;
            else
               prevObjIndex->next = pObjIndex->next;
         }

         /*
          * Add another 
          */

         iHash = new_vnum % MAX_KEY_HASH;
         pObjIndex->next = obj_index_hash[iHash];
         obj_index_hash[iHash] = pObjIndex;

         break;

   }
   return;

}


void do_check_areas( CHAR_DATA * ch, char *argument )
{
   hash_table *room_hash;
   hash_table *obj_hash;
   hash_table *mob_hash;
   int min_vnum, max_vnum;
   int new_vnum, old_vnum;
   char buffer[MAX_INPUT_LENGTH];
   FILE *out_file;
   int a;
   RESET_DATA *pReset;

   AREA_DATA *CurArea;
   BUILD_DATA_LIST *pList;
   ROOM_INDEX_DATA *pRoomIndex;
   OBJ_INDEX_DATA *pObjIndex;
   MOB_INDEX_DATA *pMobIndex;

   /*
    * Create hash tables for rooms, mobiles, objects 
    */

   room_hash = create_hash_table( MAX_KEY_HASH );
   obj_hash = create_hash_table( MAX_KEY_HASH );
   mob_hash = create_hash_table( MAX_KEY_HASH );

   out_file = fopen( "area_changes.txt", "a" );

   for( CurArea = first_area; CurArea != NULL; CurArea = CurArea->next )
   {
      min_vnum = CurArea->min_vnum;
      max_vnum = CurArea->max_vnum;
      fprintf( out_file, "%s:\n", CurArea->name );

      /*
       * Go through rooms 
       */
      for( pList = CurArea->first_area_room; pList != NULL; pList = pList->next )
      {
         pRoomIndex = pList->data;
         if( pRoomIndex->vnum < min_vnum || pRoomIndex->vnum > max_vnum )
         {
            old_vnum = pRoomIndex->vnum;
            /*
             * Find a free slot 
             */
            for( new_vnum = min_vnum; new_vnum <= max_vnum; new_vnum++ )
               if( get_room_index( new_vnum ) == NULL )
                  break;

            if( new_vnum > max_vnum )
            {
               sprintf( buffer, "Not enough vnums in area %s\n\r", CurArea->name );
               send_to_char( buffer, ch );
            }
            else
            {
               fprintf( out_file, "Room: [%5i] -> [%5i] %s\n", old_vnum, new_vnum, pRoomIndex->name );
               /*
                * Delete from room hashing table, and put new vnum in. 
                */
               add_hash_entry( room_hash, old_vnum, ( void * )new_vnum );
               swap_global_hash( 'R', pRoomIndex, old_vnum, new_vnum );
               pRoomIndex->vnum = new_vnum;
               area_modified( CurArea );

            }
         }
      }

      /*
       * Go through objs 
       */
      for( pList = CurArea->first_area_object; pList != NULL; pList = pList->next )
      {
         pObjIndex = pList->data;
         if( pObjIndex->vnum < min_vnum || pObjIndex->vnum > max_vnum )
         {
            old_vnum = pObjIndex->vnum;
            /*
             * Find a free slot 
             */
            for( new_vnum = min_vnum; new_vnum <= max_vnum; new_vnum++ )
               if( get_obj_index( new_vnum ) == NULL )
                  break;

            if( new_vnum > max_vnum )
            {
               sprintf( buffer, "Not enough vnums in area %s\n\r", CurArea->name );
               send_to_char( buffer, ch );
            }
            else
            {
               fprintf( out_file, "Obj: [%5i] -> [%5i] %s\n", old_vnum, new_vnum, pObjIndex->short_descr );
               /*
                * Delete from obj hashing table, and put new vnum in. 
                */
               add_hash_entry( obj_hash, old_vnum, ( void * )new_vnum );
               swap_global_hash( 'O', pObjIndex, old_vnum, new_vnum );
               pObjIndex->vnum = new_vnum;
               area_modified( CurArea );

            }
         }
      }

      /*
       * Go through mobs 
       */
      for( pList = CurArea->first_area_mobile; pList != NULL; pList = pList->next )
      {
         pMobIndex = pList->data;
         if( pMobIndex->vnum < min_vnum || pMobIndex->vnum > max_vnum )
         {
            old_vnum = pMobIndex->vnum;
            /*
             * Find a free slot 
             */
            for( new_vnum = min_vnum; new_vnum <= max_vnum; new_vnum++ )
               if( get_mob_index( new_vnum ) == NULL )
                  break;

            if( new_vnum > max_vnum )
            {
               sprintf( buffer, "Not enough vnums in area %s\n\r", CurArea->name );
               send_to_char( buffer, ch );
            }
            else
            {
               fprintf( out_file, "Mob: [%5i] -> [%5i] %s\n", old_vnum, new_vnum, pMobIndex->short_descr );
               /*
                * Delete from mob hashing table, and put new vnum in. 
                */
               add_hash_entry( mob_hash, old_vnum, ( void * )new_vnum );
               swap_global_hash( 'M', pMobIndex, old_vnum, new_vnum );
               pMobIndex->vnum = new_vnum;
               area_modified( CurArea );
               /*
                * Check for shops 
                */
               if( pMobIndex->pShop != NULL )
                  pMobIndex->pShop->keeper = new_vnum;

            }
         }
      }
   }

   fclose( out_file );

   /*
    * Now go through all things referencing the changes 
    * * 
    * *      Resets     
    * *      exit->key
    * *      
    */

   for( CurArea = first_area; CurArea != NULL; CurArea = CurArea->next )
   {
      for( pList = CurArea->first_area_room; pList != NULL; pList = pList->next )
      {
         /*
          * Check keys and exits
          */
         pRoomIndex = pList->data;

         for( a = 0; a <= 5; a++ )
         {
            if( pRoomIndex->exit[a] != NULL )
            {
               if( pRoomIndex->exit[a]->key != 0
                   && ( new_vnum = ( int )get_hash_entry( obj_hash, pRoomIndex->exit[a]->key ) ) != 0 )
               {
                  pRoomIndex->exit[a]->key = new_vnum;
                  area_modified( CurArea );
               }
               if( ( new_vnum = ( int )get_hash_entry( room_hash, pRoomIndex->exit[a]->vnum ) ) != 0 )
               {
                  pRoomIndex->exit[a]->vnum = new_vnum;
                  area_modified( CurArea );
               }
            }

         }
      }

      /*
       * Scan through resets 
       */
      for( pReset = CurArea->first_reset; pReset != NULL; pReset = pReset->next )
      {
         switch ( pReset->command )
         {
            case 'M':
               if( ( new_vnum = ( int )get_hash_entry( mob_hash, pReset->arg1 ) ) != 0 )
               {
                  area_modified( CurArea );
                  pReset->arg1 = new_vnum;
               }
               if( ( new_vnum = ( int )get_hash_entry( room_hash, pReset->arg3 ) ) != 0 )
               {
                  area_modified( CurArea );
                  pReset->arg3 = new_vnum;
               }
               break;

            case 'O':
               if( ( new_vnum = ( int )get_hash_entry( obj_hash, pReset->arg1 ) ) != 0 )
               {
                  area_modified( CurArea );
                  pReset->arg1 = new_vnum;
               }
               if( ( new_vnum = ( int )get_hash_entry( room_hash, pReset->arg3 ) ) != 0 )
               {
                  area_modified( CurArea );
                  pReset->arg3 = new_vnum;
               }
               break;

            case 'P':
               if( ( new_vnum = ( int )get_hash_entry( obj_hash, pReset->arg1 ) ) != 0 )
               {
                  area_modified( CurArea );
                  pReset->arg1 = new_vnum;
               }
               if( ( new_vnum = ( int )get_hash_entry( obj_hash, pReset->arg3 ) ) != 0 )
               {
                  area_modified( CurArea );
                  pReset->arg3 = new_vnum;
               }
               break;

            case 'G':
            case 'E':
               if( ( new_vnum = ( int )get_hash_entry( obj_hash, pReset->arg1 ) ) != 0 )
               {
                  area_modified( CurArea );
                  pReset->arg1 = new_vnum;
               }
               break;

            case 'D':
            case 'R':
               if( ( new_vnum = ( int )get_hash_entry( room_hash, pReset->arg1 ) ) != 0 )
               {
                  area_modified( CurArea );
                  pReset->arg1 = new_vnum;
               }
               break;
         }

      }
   }

   /*
    * FINISHED 
    */

   delete_hash_table( room_hash );
   delete_hash_table( obj_hash );
   delete_hash_table( mob_hash );

   return;
}

void do_check_area( CHAR_DATA * ch, char *argument )
{
   send_to_char( "You must say check_areas in full.\n\r", ch );
   return;
}
