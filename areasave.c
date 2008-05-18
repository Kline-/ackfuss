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


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"



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
#define BUILD_SEC_HELP     2
#define BUILD_SEC_ROOMS    3
#define BUILD_SEC_MOBILES  4
#define BUILD_SEC_MOBPROGS 5
#define BUILD_SEC_OBJECTS  6
#define BUILD_SEC_SHOPS    7
#define BUILD_SEC_RESETS   8
#define BUILD_SEC_SPECIALS 9
#define BUILD_SEC_OBJFUNS  10 /* -S- Mod */
#define BUILD_SEC_END      11
#define AREA_VERSION  16


struct save_queue_type
{
   AREA_DATA *area;
   CHAR_DATA *ch;
   int loops;
} SaveQ[SAVEQUEUESIZE];

/* Semi-local vars. */
int saving_area = 0;

/* local */
int offset;
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

/* Local functions */
/* void build_save(); proto in merc.h */
void build_save_area( void );
void build_save_help( void );
void build_save_mobs( void );
void build_save_mobprogs( void );
void build_save_objects( void );
void build_save_rooms( void );
void build_save_shops( void );
void build_save_resets( void );
void build_save_specs( void );
void build_save_objfuns( void );
void build_save_end( void );
char *mprog_type_to_name( int );
void vuild_save_flush( void );
/*  int convert(int lev); */
/* Convert levels from ack -> envy! */

/* int  convert(int lev)	*/
/*  { 		*/
/*   return( lev - ( lev/5 ) ); 	*/
/* }   */

void do_savearea( CHAR_DATA * ch, char *argument )
{
   AREA_DATA *SaveArea;
   int loops;
/*     char first_arg[MAX_INPUT_LENGTH]; unused? */


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

         sprintf( filename, "%s.new", CurSaveArea->filename );
         SaveFile = fopen( filename, "w" );
         if( SaveFile == NULL )
         {
            if( CurrentSaving == ToBeSaved )
               saving_area = NOT_SAVING;
            send_to_char( "Can not open file for saving.\n", CurSaveChar );
            return;
         }
         /*
          * Open second file for saving in envy format 
          */

         sprintf( buf, "Starting to save %s", CurSaveArea->filename );
         monitor_chan( buf, MONITOR_AREA_SAVING );


         Section = 1;
         offset = CurSaveArea->offset;
         saving_area = AM_SAVING;
         Pointer = NULL;
         ResetPointer = NULL;
      }

      switch ( Section )
      {
         case BUILD_SEC_AREA:
            build_save_area(  );
            break;
         case BUILD_SEC_HELP:
            build_save_help(  );
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
   fprintf( SaveFile, "#AREA\n" );
   fprintf( SaveFile, "%s~\n", CurSaveArea->name );
   fprintf( SaveFile, "Q %i\n", AREA_VERSION );
   fprintf( SaveFile, "K %s~\n", CurSaveArea->keyword );
   fprintf( SaveFile, "L %s~\n", CurSaveArea->level_label );
   fprintf( SaveFile, "N %i\n", CurSaveArea->area_num );
   fprintf( SaveFile, "I %i %i\n", CurSaveArea->min_level, CurSaveArea->max_level );
   fprintf( SaveFile, "V %i %i\n", CurSaveArea->min_vnum, CurSaveArea->max_vnum );
   fprintf( SaveFile, "X %i\n", CurSaveArea->offset );
   fprintf( SaveFile, "F %i\n", CurSaveArea->reset_rate );
   fprintf( SaveFile, "U %s~\n", CurSaveArea->reset_msg );
   if( CurSaveArea->owner != NULL )
      fprintf( SaveFile, "O %s~\n", CurSaveArea->owner );
   if( CurSaveArea->can_read != NULL )
      fprintf( SaveFile, "R %s~\n", CurSaveArea->can_read );
   if( CurSaveArea->can_write != NULL )
      fprintf( SaveFile, "W %s~\n", CurSaveArea->can_write );
   if( IS_SET( CurSaveArea->flags, AREA_PAYAREA ) )
      fprintf( SaveFile, "P This is a pay area.\n" );
   if( IS_SET( CurSaveArea->flags, AREA_TELEPORT ) )
      fprintf( SaveFile, "T You can teleport into here\n" );
   if( IS_SET( CurSaveArea->flags, AREA_BUILDING ) )
      fprintf( SaveFile, "B Currently building area.\n" );
   if( IS_SET( CurSaveArea->flags, AREA_NOSHOW ) )
      fprintf( SaveFile, "S Title not shown on area list.\n" );
   if( IS_SET( CurSaveArea->flags, AREA_NO_ROOM_AFF ) )
      fprintf( SaveFile, "M No bad room spells allowed.\n" );


/*     fprintf( Envy, "#AREA\n" );                      remove save bug */
/*     fprintf( Envy, "%s~\n", CurSaveArea->name );                     */

   Section++;
}

void build_save_help(  )
{
   HELP_DATA *pHelp;

   if( Pointer == NULL )   /* Start */
   {
      if( CurSaveArea->first_area_help_text == NULL )
      {
         Section++;
         return;
      }
      send_to_char( "Saving help section.\n", CurSaveChar );
      fprintf( SaveFile, "#HELPS\n" );
      Pointer = CurSaveArea->first_area_help_text;
   }

   pHelp = Pointer->data;
   fprintf( SaveFile, "%i %s~\n", pHelp->level, pHelp->keyword );
   if( isspace( pHelp->text[0] ) )
      fprintf( SaveFile, ".%s~\n", pHelp->text );
   else
      fprintf( SaveFile, "%s~\n", pHelp->text );

   Pointer = Pointer->next;
   if( Pointer == NULL )   /* End */
   {
      fprintf( SaveFile, "0 $~\n" );
      Section++;
   }
   /*
    * No saving helps for envy format 
    */

   return;
}

void build_save_mobs(  )
{
   MOB_INDEX_DATA *pMobIndex;
   MPROG_DATA *mprg;
   int finish_progs;

   if( Pointer == NULL )   /* Start */
   {
      if( CurSaveArea->first_area_mobile == NULL )
      {
         Section++;
         return;
      }
      send_to_char( "Saving mobs.\n", CurSaveChar );
      fprintf( SaveFile, "#MOBILES\n" );
      Pointer = CurSaveArea->first_area_mobile;
   }

   pMobIndex = Pointer->data;
   fprintf( SaveFile, "#%i\n", pMobIndex->vnum );
   fprintf( SaveFile, "%s~\n", pMobIndex->player_name );
   fprintf( SaveFile, "%s~\n", pMobIndex->short_descr );
   fprintf( SaveFile, "%s~\n", pMobIndex->long_descr );
   fprintf( SaveFile, "%s~\n", pMobIndex->description );
   fprintf( SaveFile, "%i %i %i S\n", pMobIndex->act, pMobIndex->affected_by, pMobIndex->alignment );
   fprintf( SaveFile, "%i %i\n", pMobIndex->level, pMobIndex->sex );
   fprintf( SaveFile, "%i %i %i\n", pMobIndex->ac_mod, pMobIndex->hr_mod, pMobIndex->dr_mod );


   /*
    * Write out new details - clan, class, race and skills
    * * The '!' signifies new section to load_mobiles() in db.c
    */


   fprintf( SaveFile, "! %i %i %i %i %i %i %i\n",
            pMobIndex->class,
            pMobIndex->clan, pMobIndex->race, pMobIndex->position, pMobIndex->skills, pMobIndex->cast, pMobIndex->def );
   fprintf( SaveFile, "| %i %i %i %i %i %i %i\n",
            pMobIndex->strong_magic,
            pMobIndex->weak_magic,
            pMobIndex->race_mods, pMobIndex->power_skills, pMobIndex->power_cast, pMobIndex->resist, pMobIndex->suscept );

   mprg = pMobIndex->first_mprog;
   finish_progs = 0;
   while( mprg )
   {
      if( mprg->filename == NULL )
      {
         fprintf( SaveFile, ">%s ", mprog_type_to_name( mprg->type ) );
         fprintf( SaveFile, "%s~\n", mprg->arglist );
         fprintf( SaveFile, "%s~\n", mprg->comlist );
         finish_progs = 1;
      }
      mprg = mprg->next;
   }
   if( finish_progs )
   {
      fprintf( SaveFile, "|\n" );
   }


   Pointer = Pointer->next;
   if( Pointer == NULL )   /* End */
   {
      fprintf( SaveFile, "#0\n" );
      Section++;
   }
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

   pItem = Pointer->data;
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
   OBJ_INDEX_DATA *pObject;
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
      fprintf( SaveFile, "#OBJECTS\n" );
      Pointer = CurSaveArea->first_area_object;
   }

   pObject = Pointer->data;

   fprintf( SaveFile, "#%i\n", pObject->vnum );
   fprintf( SaveFile, "%s~\n", pObject->name );
   fprintf( SaveFile, "%s~\n", pObject->short_descr );
   fprintf( SaveFile, "%s~\n", pObject->description );
   fprintf( SaveFile, "%i %i %i %i\n", pObject->item_type, pObject->extra_flags, pObject->wear_flags, pObject->item_apply );

   /*
    * Check for pills, potions, scrolls, staffs and wands.  
    */
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

   if( ( pObject->level > 1 ) && ( pObject->level < 130 ) )
   {
      fprintf( SaveFile, "L\n" );
      fprintf( SaveFile, "%d\n", pObject->level );
   }
   else
   {
      fprintf( SaveFile, "L\n" );
      fprintf( SaveFile, "%d\n", 1 );
   }

   /*
    * Now for Envy... taken from my OLC :P 
    */

   Pointer = Pointer->next;
   if( Pointer == NULL )   /* End */
   {
      fprintf( SaveFile, "#0\n" );
      Section++;
   }

   return;
}

void build_save_rooms(  )
{
   ROOM_INDEX_DATA *pRoomIndex;
   EXTRA_DESCR_DATA *pEd;
   int d;
   EXIT_DATA *pexit;
   int locks = 0;

   if( Pointer == NULL )   /* Start */
   {
      if( CurSaveArea->first_area_room == NULL )
      {
         Section++;
         return;
      }
      send_to_char( "Saving rooms.\n", CurSaveChar );
      fprintf( SaveFile, "#ROOMS\n" );
      Pointer = CurSaveArea->first_area_room;
   }

   pRoomIndex = Pointer->data;

   fprintf( SaveFile, "#%i\n", pRoomIndex->vnum );
   fprintf( SaveFile, "%s~\n", pRoomIndex->name );
   fprintf( SaveFile, "%s~\n", pRoomIndex->description );
   fprintf( SaveFile, "%i %i\n", pRoomIndex->room_flags, pRoomIndex->sector_type );

   /*
    * Now do doors. 
    */
   for( d = 0; d < 6; d++ )
   {
      if( pRoomIndex->exit[d] )
      {

         fprintf( SaveFile, "D%i\n", d );
         pexit = pRoomIndex->exit[d];
         fprintf( SaveFile, "%s~\n", pexit->description );
         fprintf( SaveFile, "%s~\n", pexit->keyword );
         /*
          * Deal with locks 
          */
         /*
          * -S- Mod: Filter out EX_LOCKED and EX_CLOSED and save exit_info 
          */
         locks = pexit->exit_info;
         if( IS_SET( locks, EX_CLOSED ) )
            REMOVE_BIT( locks, EX_CLOSED );
         if( IS_SET( locks, EX_LOCKED ) )
            REMOVE_BIT( locks, EX_LOCKED );

#if 0
         /*
          * BUG: before, an isdoor OR pickproof flag alone would give locks=1 
          */
         if( pexit->exit_info & EX_ISDOOR )
            locks++;
         if( pexit->exit_info & EX_PICKPROOF )
            locks++;
#endif

         fprintf( SaveFile, "%i %i %i\n", locks, pexit->key, pexit->vnum );
      }
   }

   /*
    * Now do extra descripts.. 
    */

   pEd = pRoomIndex->first_exdesc;
   while( pEd )
   {
      fprintf( SaveFile, "E\n" );
      fprintf( SaveFile, "%s~\n", pEd->keyword );
      fprintf( SaveFile, "%s~\n", pEd->description );
      pEd = pEd->next;
   }

   /*
    * End of one room 
    */
   fprintf( SaveFile, "S\n" );

   Pointer = Pointer->next;
   if( Pointer == NULL )   /* End */
   {
      fprintf( SaveFile, "#0\n" );
      Section++;
   }
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

   pShop = Pointer->data;
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

   pMob = Pointer->data;

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

   pObj = Pointer->data;

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

   sprintf( buf, "Finished saving %s", CurSaveArea->filename );
   monitor_chan( buf, MONITOR_AREA_SAVING );

   fprintf( SaveFile, "#$\n" );
   send_to_char( "Finished saving.\n", CurSaveChar );
   fclose( SaveFile );
   /*
    * Save backup 
    */
   sprintf( filename, "%s.old", CurSaveArea->filename );
   rename( CurSaveArea->filename, filename );
   /*
    * And rename .new to area filename 
    */
   sprintf( filename, "%s.new", CurSaveArea->filename );
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
