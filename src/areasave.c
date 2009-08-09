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

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_OBJ_FUN_H
#include "h/obj_fun.h"
#endif

#ifndef DEC_SPEC_FUN_H
#include "h/spec_fun.h"
#endif

/* Way this works:
	Mud reads in area files, stores details in data lists.
	Edit rooms, objects, resets.
	type savearea.
	Sets bool saving_area to true.
	Incrementally saves an area, using data lists.
*/

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

DO_FUN(do_savearea)
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

         snprintf( filename, 255, "%s%s.new", AREA_DIR, CurSaveArea->filename );
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
         case BUILD_SEC_OBJECTS:
            build_save_objects(  );
            break;
         case BUILD_SEC_SHOPS:
            build_save_shops(  );
            break;
         case BUILD_SEC_RESETS:
            build_save_resets(  );
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
   short i = 0;

   if( Pointer == NULL )   /* Start */
   {
      if( CurSaveArea->first_area_mobile == NULL )
      {
         Section++;
         return;
      }
      send_to_char( "Saving mobs.\n", CurSaveChar );
      Pointer = CurSaveArea->first_area_mobile;
   }

   pMobIndex = (MOB_INDEX_DATA *)Pointer->data;

   fprintf( SaveFile, "\n#MOBILE\n" );
   fprintf( SaveFile, "Vnum      %d\n", pMobIndex->vnum );  /* Must be first for sanity checks --Kline */
   fprintf( SaveFile, "AcMod     %d\n", pMobIndex->ac_mod );

   fprintf( SaveFile, "Act       " );
   for( i = 0; i < MAX_BITSET; i++ )
    if( pMobIndex->act.test(i) )
     fprintf( SaveFile, "%d ", i );
   fprintf( SaveFile, "EOL\n" );

   fprintf( SaveFile, "Affected   %d\n", pMobIndex->affected_by );
   fprintf( SaveFile, "Alignment  %d\n", pMobIndex->alignment );
   fprintf( SaveFile, "Cast       %d\n", pMobIndex->cast );
   fprintf( SaveFile, "Clan       %d\n", pMobIndex->clan );
   fprintf( SaveFile, "Class      %d\n", pMobIndex->p_class );
   fprintf( SaveFile, "Def        %d\n", pMobIndex->def );
   fprintf( SaveFile, "Desc       %s~\n", pMobIndex->description );
   fprintf( SaveFile, "DrMod      %d\n", pMobIndex->dr_mod );
   fprintf( SaveFile, "HrMod      %d\n", pMobIndex->hr_mod );
   fprintf( SaveFile, "Level      %d\n", pMobIndex->level );
   fprintf( SaveFile, "LongDesc   %s~\n", pMobIndex->long_descr );
   fprintf( SaveFile, "PlrName    %s~\n", pMobIndex->player_name );
   fprintf( SaveFile, "Position   %d\n", pMobIndex->position );
   fprintf( SaveFile, "Race       %d\n", pMobIndex->race );
   fprintf( SaveFile, "RaceMods   %d\n", pMobIndex->race_mods );
   fprintf( SaveFile, "Resist     %d\n", pMobIndex->resist );
   fprintf( SaveFile, "ScriptName %s~\n", pMobIndex->script_name );
   fprintf( SaveFile, "Sex        %d\n", pMobIndex->sex );
   fprintf( SaveFile, "ShortDesc  %s~\n", pMobIndex->short_descr );
   fprintf( SaveFile, "Skills     %d\n", pMobIndex->skills );
   fprintf( SaveFile, "SMagic     %d\n", pMobIndex->strong_magic );
   fprintf( SaveFile, "SpecFun    %s\n", rev_spec_lookup(pMobIndex->spec_fun) );
   fprintf( SaveFile, "Suscept    %d\n", pMobIndex->suscept );
   fprintf( SaveFile, "WMagic     %d\n", pMobIndex->weak_magic );
   fprintf( SaveFile, "End\n" );

   Pointer = Pointer->next;
   if( Pointer == NULL ) /* End */
    Section++;

   return;
}

void build_save_objects(  )
{
   OBJ_INDEX_DATA *pObjIndex;
   AFFECT_DATA *pAf;
   EXTRA_DESCR_DATA *pEd;

   if( Pointer == NULL )   /* Start */
   {
      if( CurSaveArea->first_area_object == NULL )
      {
         Section++;
         return;
      }
      send_to_char( "Saving objects.\n", CurSaveChar );
      Pointer = CurSaveArea->first_area_object;
   }

   pObjIndex = (OBJ_INDEX_DATA *)Pointer->data;

   fprintf( SaveFile, "\n#OBJECT\n" );
   fprintf( SaveFile, "Vnum       %d\n", pObjIndex->vnum );  /* Must be first for sanity checks --Kline */
   fprintf( SaveFile, "Durability %d\n", pObjIndex->max_durability );

   fprintf( SaveFile, "ExtraFlags " );
   for( short i = 0; i < MAX_BITSET; i++ )
    if( pObjIndex->extra_flags.test(i) )
     fprintf( SaveFile, "%d ", i );
   fprintf( SaveFile, "EOL\n" );

   fprintf( SaveFile, "ItemApply  %d\n", pObjIndex->item_apply );
   fprintf( SaveFile, "Level      %d\n", pObjIndex->level );
   fprintf( SaveFile, "LongDesc   %s~\n", pObjIndex->long_descr );
   fprintf( SaveFile, "Name       %s~\n", pObjIndex->name );
   fprintf( SaveFile, "ObjFun     %s\n", rev_obj_fun_lookup( pObjIndex->obj_fun ) );
   fprintf( SaveFile, "ScriptName %s~\n", pObjIndex->script_name );
   fprintf( SaveFile, "ShortDesc  %s~\n", pObjIndex->short_descr );
   fprintf( SaveFile, "Speed      %0.2f\n", pObjIndex->speed );
   fprintf( SaveFile, "Type       %d\n", pObjIndex->item_type );

   fprintf( SaveFile, "Values     " );
   for( short i = 0; i < MAX_OBJ_VALUE; i++ )
    fprintf( SaveFile, "%d ", pObjIndex->value[i] );
   fprintf( SaveFile, "\n" );

   fprintf( SaveFile, "WearFlags  " );
   for( short i = 0; i < MAX_BITSET; i++ )
    if( pObjIndex->wear_flags.test(i) )
     fprintf( SaveFile, "%d ", i );
   fprintf( SaveFile, "EOL\n" );

   fprintf( SaveFile, "Weight     %d\n", pObjIndex->weight );

   fprintf( SaveFile, "End\n" );

   pAf = pObjIndex->first_apply;
   while( pAf )
   {
      fprintf( SaveFile, "#OAFFECT\n" );
      fprintf( SaveFile, "Location   %d\n", pAf->location );
      fprintf( SaveFile, "Modifier   %d\n", pAf->modifier );
      fprintf( SaveFile, "End\n" );
      pAf = pAf->next;
   }

   pEd = pObjIndex->first_exdesc;
   while( pEd )
   {
      fprintf( SaveFile, "#OEXTRA\n" );
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
   fprintf( SaveFile, "Vnum       %d\n", pRoomIndex->vnum );  /* Must be first for sanity checks --Kline */
   fprintf( SaveFile, "Desc       %s~\n", pRoomIndex->description );

   fprintf( SaveFile, "Flags      " );
   for( i = 0; i < MAX_BITSET; i++ )
    if( pRoomIndex->room_flags.test(i) )
     fprintf( SaveFile, "%d ", i );
   fprintf( SaveFile, "EOL\n" );

   fprintf( SaveFile, "Name       %s~\n", pRoomIndex->name );
   fprintf( SaveFile, "ScriptName %s~\n", pRoomIndex->script_name );
   fprintf( SaveFile, "Sect       %d\n", pRoomIndex->sector_type );
   fprintf( SaveFile, "End\n" );

   /*
    * Now do doors. 
    */
   for( d = 0; d < 6; d++ )
   {
      if( pRoomIndex->exit[d] )
      {
         fprintf( SaveFile, "#DOOR\n" );
         fprintf( SaveFile, "Dir        %d\n", d );  /* Must be first for sanity checks --Kline */
         pexit = pRoomIndex->exit[d];
         fprintf( SaveFile, "Desc       %s~\n", pexit->description );

         fprintf( SaveFile, "Flags      " );
         for( i = 0; i < MAX_BITSET; i++ )
          if( pexit->exit_info.test(i) && i != EX_LOCKED && i != EX_CLOSED )
           fprintf( SaveFile, "%d ", i );
         fprintf( SaveFile, "EOL\n" );

         fprintf( SaveFile, "Key        %d\n", pexit->key );
         fprintf( SaveFile, "Keyword    %s~\n", pexit->keyword );
         fprintf( SaveFile, "Vnum       %d\n", pexit->vnum );
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
      fprintf( SaveFile, "Desc          %s~\n", pEd->description );
      fprintf( SaveFile, "Keyword       %s~\n", pEd->keyword );
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
      Pointer = CurSaveArea->first_area_shop;
   }

   pShop = (SHOP_DATA *)Pointer->data;

   fprintf( SaveFile, "\n#SHOP\n" );
   fprintf( SaveFile, "Keeper    %d\n", pShop->keeper ); /* Must be first for sanity checks --Kline */

   fprintf( SaveFile, "BuyType   " );
   for( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
      fprintf( SaveFile, "%d ", pShop->buy_type[iTrade] );
   fprintf( SaveFile, "\n" );

   fprintf( SaveFile, "HourClose %d\n", pShop->close_hour );
   fprintf( SaveFile, "HourOpen  %d\n", pShop->open_hour );
   fprintf( SaveFile, "ProfBuy   %d\n", pShop->profit_buy );
   fprintf( SaveFile, "ProfSell  %d\n", pShop->profit_sell );
   fprintf( SaveFile, "End\n" );

   Pointer = Pointer->next;
   if( Pointer == NULL )   /* End */
      Section++;

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

   fprintf( SaveFile, "%c %i %i ", ResetPointer->command, ResetPointer->arg1, ResetPointer->arg2 );
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
   snprintf( filename, 255, "%s%s.old", AREA_DIR, CurSaveArea->filename );
   snprintf( buf, MSL, "%s%s", AREA_DIR, CurSaveArea->filename );
   rename( buf, filename );
   /*
    * And rename .new to area filename 
    */
   snprintf( filename, 255, "%s%s.new", AREA_DIR, CurSaveArea->filename );
   snprintf( buf, MSL, "%s%s", AREA_DIR, CurSaveArea->filename );
   rename( filename, buf );

   Section = 0;
   if( ToBeSaved == ( CurrentSaving + 1 ) % SAVEQUEUESIZE )
      saving_area = NOT_SAVING;
   else
      saving_area = START_SAVING;
}


void build_save_flush(  )
{
   list<AREA_DATA *>::iterator i;
   AREA_DATA *pArea;

   if( AreasModified == 0 )
      return;

   for( i = area_list.begin(); i != area_list.end(); i++ )
   {
      pArea = *i;
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
