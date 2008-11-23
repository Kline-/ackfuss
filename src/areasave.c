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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_AREASAVE_H
#include "h/areasave.h"
#endif

#ifndef DEC_BUILD_H
#include "h/build.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

/* Way this works:
	Mud reads in area files, stores details in data lists.
	Edit rooms, objects, resets.
	type savearea.
	Sets bool saving_area to true.
	Incrementally saves an area, using data lists.
*/


#define SAVEQUEUESIZE 50
#define NOT_SAVING 0
#define START_SAVING 1
#define AM_SAVING 2
#define BUILD_OK -1
#define BUILD_CANTSAVE 1
#define BUILD_TOOMANY  2

#define BUILD_SEC_AREA     1
#define BUILD_SEC_ROOMS    2
#define BUILD_SEC_MOBILES  3
#define BUILD_SEC_MOBPROGS 4
#define BUILD_SEC_OBJECTS  5
#define BUILD_SEC_SHOPS    6
#define BUILD_SEC_RESETS   7
#define BUILD_SEC_SPECIALS 8
#define BUILD_SEC_OBJFUNS  9 /* -S- Mod */
#define BUILD_SEC_END      10
#define AREA_VERSION       22

struct save_queue_type
{
   AREA_DATA *area;
   CHAR_DATA *ch;
   int loops;
} SaveQ[SAVEQUEUESIZE];

/* Semi-local vars. */
int saving_area = 0;

/* local */
int ToBeSaved = 0;
int CurrentSaving = -1;
AREA_DATA *CurSaveArea = NULL;
CHAR_DATA *CurSaveChar = NULL;
int CurLoops = 1;
int Section;
BUILD_DATA_LIST *Pointer;
RESET_DATA *ResetPointer;
FILE *SaveFile;
FILE *Envy;
int AreasModified = 0;

void do_savearea( CHAR_DATA * ch, char *argument )
{
   AREA_DATA *SaveArea;
   int loops;

   if( ch == NULL )
   {
      SaveArea = ( AREA_DATA * ) argument;
      loops = 10;
   }
   else
   {
      if( ch->in_room == NULL )
      {
         send_to_char( "Do not know what room you are in!!, cannot save.\n", ch );
         return;
      }

      SaveArea = ( ch->in_room )->area;
      if( SaveArea == NULL )
      {
         send_to_char( "Do not know what area you are in!!, cannot save.\n", ch );
         return;
      }

      if( *argument != '\0' )
      {
         loops = atoi( argument );
         if( loops < 1 )
            loops = 1;
      }
      else
         loops = 1;
   }

   if( ToBeSaved == CurrentSaving )
   {
      send_to_char( "Too many areas in queue, please try later.\n", ch );
      return;
   }

   SaveQ[ToBeSaved].area = SaveArea;
   SaveQ[ToBeSaved].ch = ch;
   SaveQ[ToBeSaved].loops = loops;
   ToBeSaved = ( ToBeSaved + 1 ) % SAVEQUEUESIZE;

   if( saving_area == NOT_SAVING )
      saving_area = START_SAVING;
   else
      send_to_char( "Save is queued, please wait. \n", ch );

   build_save(  );
   return;
}

void build_save(  )
{
   int a;
   char filename[255];
   char buf[MAX_STRING_LENGTH];

   for( a = 0; a < CurLoops && saving_area > 0; a++ )
   {

      if( saving_area == START_SAVING )
      {
         CurrentSaving = ( CurrentSaving + 1 ) % SAVEQUEUESIZE;
         CurSaveArea = SaveQ[CurrentSaving].area;
         CurSaveChar = SaveQ[CurrentSaving].ch;
         CurLoops = SaveQ[CurrentSaving].loops;
         send_to_char( "Starting Save.\n", CurSaveChar );

         snprintf( filename, 255, "%s.new", CurSaveArea->filename );
         SaveFile = file_open( filename, "w" );
         if( SaveFile == NULL )
         {
            if( CurrentSaving == ToBeSaved )
               saving_area = NOT_SAVING;
            send_to_char( "Can not open file for saving.\n", CurSaveChar );
            file_close(SaveFile);
            return;
         }
         /*
          * Open second file for saving in envy format 
          */

         snprintf( buf, MSL, "Starting to save %s", CurSaveArea->filename );
         monitor_chan( buf, MONITOR_AREA_SAVING );


         Section = 1;
         saving_area = AM_SAVING;
         Pointer = NULL;
         ResetPointer = NULL;
      }

      switch ( Section )
      {
         case BUILD_SEC_AREA:
            build_save_area(  );
            break;
         case BUILD_SEC_ROOMS:
            build_save_rooms(  );
            break;
         case BUILD_SEC_MOBILES:
            build_save_mobs(  );
            break;
         case BUILD_SEC_MOBPROGS:
            build_save_mobprogs(  );
            break;
         case BUILD_SEC_OBJECTS:
            build_save_objects(  );
            break;
         case BUILD_SEC_SHOPS:
            build_save_shops(  );
            break;
         case BUILD_SEC_RESETS:
            build_save_resets(  );
            break;
         case BUILD_SEC_SPECIALS:
            build_save_specs(  );
            break;
         case BUILD_SEC_OBJFUNS:
            build_save_objfuns(  );
            break;
         case BUILD_SEC_END:
            build_save_end(  );
            break;
      }
   }
   return;
}



void build_save_area(  )
{
 short i = 0;

 fprintf( SaveFile, "#AREA\n" );
 fprintf( SaveFile, "Revision  %d\n", AREA_VERSION );  /* Must be first for sanity checks --Kline */
 fprintf( SaveFile, "CanRead   %s~\n", CurSaveArea->can_read );
 fprintf( SaveFile, "CanWrite  %s~\n", CurSaveArea->can_write );

 fprintf( SaveFile, "Flags     " );
 for( i = 0; i < MAX_BITSET; i++ )
  if( CurSaveArea->flags.test(i) )
   fprintf( SaveFile, "%d ", i );
 fprintf( SaveFile, "EOL\n" );

 fprintf( SaveFile, "Keyword   %s~\n", CurSaveArea->keyword );
 fprintf( SaveFile, "LevLabel  %s~\n", CurSaveArea->level_label );
 fprintf( SaveFile, "LevRange  %d %d\n", CurSaveArea->min_level, CurSaveArea->max_level );
 fprintf( SaveFile, "Name      %s~\n", CurSaveArea->name );
 fprintf( SaveFile, "Number    %d\n", CurSaveArea->area_num );
 fprintf( SaveFile, "Owner     %s~\n", CurSaveArea->owner );
 fprintf( SaveFile, "ResetMsg  %s~\n", CurSaveArea->reset_msg );
 fprintf( SaveFile, "ResetRate %d\n", CurSaveArea->reset_rate );
 fprintf( SaveFile, "VnumRange %d %d\n", CurSaveArea->min_vnum, CurSaveArea->max_vnum );
 fprintf( SaveFile, "End\n" );

 Section++;
}

void build_save_mobs(  )
{
   MOB_INDEX_DATA *pMobIndex;
   MPROG_DATA *mprg;
   short i = 0;

   if( Pointer == NULL )   /* Start */
   {
      if( CurSaveArea->first_area_mobile == NULL )
      {
         Section++;
         return;
      }
      send_to_char( "Saving mobs.\n", CurSaveChar );
      fprintf( SaveFile, "\n#MOBILE\n" );
      Pointer = CurSaveArea->first_area_mobile;
   }

   pMobIndex = (MOB_INDEX_DATA *)Pointer->data;

   fprintf( SaveFile, "Vnum      %d\n", pMobIndex->vnum );  /* Must be first for sanity checks --Kline */
   fprintf( SaveFile, "AcMod     %d\n", pMobIndex->ac_mod );

   fprintf( SaveFile, "Act       " );
   for( i = 0; i < MAX_BITSET; i++ )
    if( pMobIndex->act.test(i) )
     fprintf( SaveFile, "%d ", i );
   fprintf( SaveFile, "EOL\n" );

   fprintf( SaveFile, "Affected  %d\n", pMobIndex->affected_by );
   fprintf( SaveFile, "Alignment %d\n", pMobIndex->alignment );
   fprintf( SaveFile, "Cast      %d\n", pMobIndex->cast );
   fprintf( SaveFile, "Clan      %d\n", pMobIndex->clan );
   fprintf( SaveFile, "Class     %d\n", pMobIndex->p_class );
   fprintf( SaveFile, "Def       %d\n", pMobIndex->def );
   fprintf( SaveFile, "Desc      %s~\n", pMobIndex->description );
   fprintf( SaveFile, "DrMod     %d\n", pMobIndex->dr_mod );
   fprintf( SaveFile, "HrMod     %d\n", pMobIndex->hr_mod );
   fprintf( SaveFile, "Level     %d\n", pMobIndex->level );
   fprintf( SaveFile, "LongDesc  %s~\n", pMobIndex->long_descr );
   fprintf( SaveFile, "PCast     %d\n", pMobIndex->power_cast );
   fprintf( SaveFile, "PlrName   %s~\n", pMobIndex->player_name );
   fprintf( SaveFile, "Position  %d\n", pMobIndex->position );
   fprintf( SaveFile, "PSkills   %d\n", pMobIndex->power_skills );
   fprintf( SaveFile, "Race      %d\n", pMobIndex->race );
   fprintf( SaveFile, "RaceMods  %d\n", pMobIndex->race_mods );
   fprintf( SaveFile, "Resist    %d\n", pMobIndex->resist );
   fprintf( SaveFile, "Sex       %d\n", pMobIndex->sex );
   fprintf( SaveFile, "ShortDesc %s~\n", pMobIndex->short_descr );
   fprintf( SaveFile, "Skills    %d\n", pMobIndex->skills );
   fprintf( SaveFile, "SMagic    %d\n", pMobIndex->strong_magic );
   fprintf( SaveFile, "Suscept   %d\n", pMobIndex->suscept );
   fprintf( SaveFile, "WMagic    %d\n", pMobIndex->weak_magic );
   fprintf( SaveFile, "End\n" );

   mprg = pMobIndex->first_mprog;
   while( mprg )
   {
      fprintf( SaveFile, "#MOBPROG\n" );
      fprintf( SaveFile, "ArgList %s~\n", mprg->arglist );
      fprintf( SaveFile, "ComList %s~\n", mprg->comlist );
      fprintf( SaveFile, "Type    %d\n", mprg->type );
      fprintf( SaveFile, "End\n" );
      mprg = mprg->next;
   }

   Pointer = Pointer->next;
   if( Pointer == NULL ) /* End */
    Section++;

   return;
}

void build_save_mobprogs(  )
{
   MOB_INDEX_DATA *pMobIndex;
   MOBPROG_ITEM *pItem;

   if( Pointer == NULL )   /* Start */
   {
      if( CurSaveArea->first_area_mobprog == NULL )
      {
         Section++;
         return;
      }
      send_to_char( "Saving mobprogs.\n", CurSaveChar );
      fprintf( SaveFile, "#MOBPROGS\n" );

      Pointer = CurSaveArea->first_area_mobprog;
   }

   pItem = (MOBPROG_ITEM *)Pointer->data;
   pMobIndex = pItem->mob;

   fprintf( SaveFile, "M %i %s\n", pMobIndex->vnum, pItem->filename );

   Pointer = Pointer->next;
   if( Pointer == NULL )   /* End */
   {
      fprintf( SaveFile, "S\n" );
      Section++;
   }
   return;
}

void build_save_objects(  )
{
   OBJ_INDEX_DATA *pObjIndex;
   AFFECT_DATA *pAf;
   EXTRA_DESCR_DATA *pEd;
   int val0, val1, val2, val3;

   if( Pointer == NULL )   /* Start */
   {
      if( CurSaveArea->first_area_object == NULL )
      {
         Section++;
         return;
      }
      send_to_char( "Saving objects.\n", CurSaveChar );
      fprintf( SaveFile, "\n#OBJECT\n" );
      Pointer = CurSaveArea->first_area_object;
   }

   pObjIndex = (OBJ_INDEX_DATA *)Pointer->data;

   fprintf( SaveFile, "Vnum       %d\n", pObjIndex->vnum );  /* Must be first for sanity checks --Kline */
   fprintf( SaveFile, "Durability %d\n", pObjIndex->max_durability );
   fprintf( SaveFile, "Level      %d\n", pObjIndex->level );
   fprintf( SaveFile, "LongDesc   %s~\n", pObjIndex->long_descr );
   fprintf( SaveFile, "Name       %s~\n", pObjIndex->name );
   fprintf( SaveFile, "ShortDesc  %s~\n", pObjIndex->short_descr );
   fprintf( SaveFile, "Speed      %0.2f\n", pObjIndex->speed );
   fprintf( SaveFile, "Type       %d\n", pObjIndex->item_type );
   fprintf( SaveFile, "End\n" );
/*
   fprintf( SaveFile, "%i %i %i\n", pObject->extra_flags, pObject->wear_flags, pObject->item_apply );


    * Check for pills, potions, scrolls, staffs and wands.  

   val0 = pObject->value[0];
   val1 = pObject->value[1];
   val2 = pObject->value[2];
   val3 = pObject->value[3];
   switch ( pObject->item_type )
   {
      case ITEM_PILL:
      case ITEM_POTION:
      case ITEM_SCROLL:
         val1 = val1 < 0 ? -1 : skill_table[val1].slot;
         val2 = val2 < 0 ? -1 : skill_table[val2].slot;
         val3 = val3 < 0 ? -1 : skill_table[val3].slot;
         break;

      case ITEM_STAFF:
      case ITEM_WAND:
         val3 = val3 < 0 ? -1 : skill_table[val3].slot;
         break;
   }
   fprintf( SaveFile, "%i %i %i %i %i %i %i %i %i %i\n", val0, val1, val2, val3,
            pObject->value[4], pObject->value[5], pObject->value[6], pObject->value[7],
            pObject->value[8], pObject->value[9] );
   fprintf( SaveFile, "%i\n", pObject->weight );


   pAf = pObject->first_apply;
   while( pAf )
   {
      fprintf( SaveFile, "A\n" );
      fprintf( SaveFile, "%i %i\n", pAf->location, pAf->modifier );
      pAf = pAf->next;
   }

   pEd = pObject->first_exdesc;
   while( pEd )
   {
      fprintf( SaveFile, "E\n" );
      fprintf( SaveFile, "%s~\n", pEd->keyword );
      fprintf( SaveFile, "%s~\n", pEd->description );
      pEd = pEd->next;
   }
*/
   Pointer = Pointer->next;
   if( Pointer == NULL ) /* End */
    Section++;

   return;
}

void build_save_rooms(  )
{
   ROOM_INDEX_DATA *pRoomIndex;
   EXTRA_DESCR_DATA *pEd;
   short d, i;
   EXIT_DATA *pexit;

   if( Pointer == NULL )   /* Start */
   {
      if( CurSaveArea->first_area_room == NULL )
      {
         Section++;
         return;
      }
      send_to_char( "Saving rooms.\n", CurSaveChar );
      Pointer = CurSaveArea->first_area_room;
   }

   pRoomIndex = (ROOM_INDEX_DATA *)Pointer->data;

   fprintf( SaveFile, "\n#ROOM\n" );
   fprintf( SaveFile, "Vnum  %d\n", pRoomIndex->vnum );  /* Must be first for sanity checks --Kline */
   fprintf( SaveFile, "Desc  %s~\n", pRoomIndex->description );

   fprintf( SaveFile, "Flags " );
   for( i = 0; i < MAX_BITSET; i++ )
    if( pRoomIndex->room_flags.test(i) )
     fprintf( SaveFile, "%d ", i );
   fprintf( SaveFile, "EOL\n" );

   fprintf( SaveFile, "Name  %s~\n", pRoomIndex->name );
   fprintf( SaveFile, "Sect  %d\n", pRoomIndex->sector_type );
   fprintf( SaveFile, "End\n" );

   /*
    * Now do doors. 
    */
   for( d = 0; d < 6; d++ )
   {
      if( pRoomIndex->exit[d] )
      {
         fprintf( SaveFile, "#DOOR\n" );
         fprintf( SaveFile, "Dir     %d\n", d );  /* Must be first for sanity checks --Kline */
         pexit = pRoomIndex->exit[d];
         fprintf( SaveFile, "Desc    %s~\n", pexit->description );

         fprintf( SaveFile, "Flags   " );
         for( i = 0; i < MAX_BITSET; i++ )
          if( pexit->exit_info.test(i) && i != EX_LOCKED && i != EX_CLOSED )
           fprintf( SaveFile, "%d ", i );
         fprintf( SaveFile, "EOL\n" );

         fprintf( SaveFile, "Key     %d\n", pexit->key );
         fprintf( SaveFile, "Keyword %s~\n", pexit->keyword );
         fprintf( SaveFile, "Vnum    %d\n", pexit->vnum );
         fprintf( SaveFile, "End\n" );
      }
   }

   /*
    * Now do extra descripts.. 
    */

   pEd = pRoomIndex->first_exdesc;
   while( pEd )
   {
      fprintf( SaveFile, "#REXTRA\n" );
      fprintf( SaveFile, "Desc       %s~\n", pEd->description );
      fprintf( SaveFile, "Keyword    %s~\n", pEd->keyword );
      fprintf( SaveFile, "End\n" );
      pEd = pEd->next;
   }

   Pointer = Pointer->next;
   if( Pointer == NULL ) /* End */
    Section++;

   return;
}

void build_save_shops(  )
{
   SHOP_DATA *pShop;
   int iTrade;

   if( Pointer == NULL )   /* Start */
   {
      if( CurSaveArea->first_area_shop == NULL )
      {
         Section++;
         return;
      }
      send_to_char( "Saving shops.\n", CurSaveChar );
      fprintf( SaveFile, "#SHOPS\n" );
      Pointer = CurSaveArea->first_area_shop;
   }

   pShop = (SHOP_DATA *)Pointer->data;
   fprintf( SaveFile, "%i ", pShop->keeper );
   for( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
      fprintf( SaveFile, "%i ", pShop->buy_type[iTrade] );
   fprintf( SaveFile, "%i %i %i %i\n", pShop->profit_buy, pShop->profit_sell, pShop->open_hour, pShop->close_hour );


   Pointer = Pointer->next;
   if( Pointer == NULL )   /* End */
   {
      fprintf( SaveFile, "0\n" );
      Section++;
   }
   return;
}

void build_save_specs(  )
{
   MOB_INDEX_DATA *pMob;

   if( Pointer == NULL )   /* Start */
   {
      if( CurSaveArea->first_area_specfunc == NULL )
      {
         Section++;
         return;
      }
      send_to_char( "Saving specials.\n", CurSaveChar );
      fprintf( SaveFile, "#SPECIALS\n" );
      Pointer = CurSaveArea->first_area_specfunc;
   }

   pMob = (MOB_INDEX_DATA *)Pointer->data;

   fprintf( SaveFile, "M %i ", pMob->vnum );
   fprintf( SaveFile, "%s\n", rev_spec_lookup( pMob->spec_fun ) );


   Pointer = Pointer->next;
   if( Pointer == NULL )   /* End */
   {
      fprintf( SaveFile, "S\n" );
      Section++;
   }
   return;
}

void build_save_objfuns(  )
{
   OBJ_INDEX_DATA *pObj;

   if( Pointer == NULL )   /* Start */
   {
      if( CurSaveArea->first_area_objfunc == NULL )
      {
         Section++;
         return;
      }
      send_to_char( "Saving objfuns.\n", CurSaveChar );
      fprintf( SaveFile, "#OBJFUNS\n" );
      Pointer = CurSaveArea->first_area_objfunc;
   }

   pObj = (OBJ_INDEX_DATA *)Pointer->data;

   fprintf( SaveFile, "O %i ", pObj->vnum );
   fprintf( SaveFile, "%s\n", rev_obj_fun_lookup( pObj->obj_fun ) );

   Pointer = Pointer->next;
   if( Pointer == NULL )   /* End */
   {
      fprintf( SaveFile, "S\n" );
      Section++;
   }
   return;
}

void build_save_resets(  )
{
   if( ResetPointer == NULL ) /* Start */
   {
      if( CurSaveArea->first_reset == NULL )
      {
         Section++;
         return;
      }
      send_to_char( "Saving resets.\n", CurSaveChar );
      fprintf( SaveFile, "#RESETS\n" );
      ResetPointer = CurSaveArea->first_reset;
   }

   fprintf( SaveFile, "%c %i %i %i ", ResetPointer->command, ResetPointer->ifflag, ResetPointer->arg1, ResetPointer->arg2 );
   if( ResetPointer->command == 'G' || ResetPointer->command == 'R' )
      fprintf( SaveFile, "%s\n", ResetPointer->notes );
   else
      fprintf( SaveFile, "%i %s\n", ResetPointer->arg3, ResetPointer->notes );

   ResetPointer = ResetPointer->next;
   if( ResetPointer == NULL ) /* End */
   {
      fprintf( SaveFile, "S\n" );
      Section++;
   }
   return;
}

void build_save_end(  )
{
   char filename[255];
   char buf[MAX_STRING_LENGTH];

   snprintf( buf, MSL, "Finished saving %s", CurSaveArea->filename );
   monitor_chan( buf, MONITOR_AREA_SAVING );

   fprintf( SaveFile, "#$\n" );
   send_to_char( "Finished saving.\n", CurSaveChar );
   file_close( SaveFile );
   /*
    * Save backup 
    */
   snprintf( filename, 255, "%s.old", CurSaveArea->filename );
   rename( CurSaveArea->filename, filename );
   /*
    * And rename .new to area filename 
    */
   snprintf( filename, 255, "%s.new", CurSaveArea->filename );
   rename( filename, CurSaveArea->filename );

   Section = 0;
   if( ToBeSaved == ( CurrentSaving + 1 ) % SAVEQUEUESIZE )
      saving_area = NOT_SAVING;
   else
      saving_area = START_SAVING;
}


void build_save_flush(  )
{
   AREA_DATA *pArea;

   if( AreasModified == 0 )
      return;

   for( pArea = first_area; pArea != NULL; pArea = pArea->next )
   {
      if( pArea->modified )
      {
         pArea->modified = 0;
         do_savearea( NULL, ( char * )pArea );
      }
   }

   AreasModified = 0;
}

void area_modified( AREA_DATA * pArea )
{
   pArea->modified = 1;
   AreasModified = 1;
}
