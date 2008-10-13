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
#include <string.h>
#include <time.h>
#include "globals.h"
#include "hash.h"

#ifndef DEC_ACT_OBJ_H
#include "h/act_obj.h"
#endif

#ifndef DEC_MONEY_H
#include "money.h"
#endif

#ifndef DEC_MQUEST_H
#include "mquest.h"
#endif

#if !defined(macintosh)
extern int _filbuf args( ( FILE * ) );
#endif



/* SAVE_REVISION number defines what has changed:
   0 -> 1:
     Went multi-char then lost 3 classes.
     Need to convert pre-multi-char to Ver.1 multi-char.
     Need to loose 3 classes off Ver.0 multi-char.        
   1 -> 2:  
     Changed exp system - need to void old player's exp 
     added sentence integer into ch structure - old ver's set to 0
     (Sentence is unused, btw)
   2 -> 3:
     Player-selectable order of class abilities.
     Need to convert Ver.1- racial class order to player class order.
     Spells/Skills re-ordered, changed, so Ver.1- lose spells, get pracs.
   3 -> 4:
     Needed to fix a bug.  I screwed up.  
   4 -> 5:
      Arrggghhh.
   5 -> 6:
      Redid exp.. set all exp to 0.
   6 -> 7:
      Reduced number of clans.
   7 ->8:
      Multiple changes for ackmud 4.3
      race wear slots

     */


#define SAVE_REVISION 15
char *cap_nocol( const char *str )
{
   static char strcap[MAX_STRING_LENGTH];
   int i;

   for( i = 0; str[i] != '\0'; i++ )
      strcap[i] = LOWER( str[i] );
   strcap[i] = '\0';
   strcap[0] = UPPER( strcap[0] );
   return strcap;
}

/*
 * Array of containers read for proper re-nesting of objects.
 */
#define MAX_NEST        100
static OBJ_DATA *rgObjNest[MAX_NEST];


extern bool deathmatch;  /* Deathmatch happening?        */


/*
 * Local functions.
 */
void fwrite_char args( ( CHAR_DATA * ch, FILE * fp ) );
void fwrite_obj args( ( CHAR_DATA * ch, OBJ_DATA * obj, FILE * fp, int iNest ) );
void fread_char args( ( CHAR_DATA * ch, FILE * fp ) );
void fread_obj args( ( CHAR_DATA * ch, FILE * fp ) );

void abort_wrapper( void );

/* Courtesy of Yaz of 4th Realm */
char *initial( const char *str )
{
   static char strint[MAX_STRING_LENGTH];

   strint[0] = LOWER( str[0] );
   return strint;

}

/*
 * Save a character and inventory.
 * Would be cool to save NPC's too for quest purposes,
 *   some of the infrastructure is provided.
 */
int loop_counter;
void save_char_obj( CHAR_DATA * ch )
{
   char strsave[MAX_INPUT_LENGTH];
   char tempstrsave[MAX_INPUT_LENGTH]; /* Hold temp filename here.. */
   char buf[MAX_INPUT_LENGTH];   /* hold misc stuff here.. */
   FILE *fp;
   char *nmptr, *bufptr;

   if( deathmatch )
   {
      /*
       * Then a deathmatch is in progress... 
       */
      /*
       * And therefore don't save... 
       */
      return;
   }

   if( IS_NPC( ch ) || ch->level < 2 )
      return;


   if( !IS_NPC( ch ) && ch->desc != NULL && ch->desc->original != NULL )
      ch = ch->desc->original;

   ch->save_time = current_time;


   /*
    * player files parsed directories by Yaz 4th Realm 
    */
#if !defined(machintosh) && !defined(MSDOS)
   if( IS_NPC( ch ) )   /* convert spaces to . */
   {
      for( nmptr = ch->name, bufptr = buf; *nmptr != 0; nmptr++ )
      {
         if( *nmptr == ' ' )
            *( bufptr++ ) = '.';
         else
            *( bufptr++ ) = *nmptr;
      }
      *( bufptr ) = *nmptr;
   }
   else
      strcpy( buf, ch->name );
   xprintf( strsave, "%s%s%s%s", PLAYER_DIR, initial( buf ), "/", cap_nocol( buf ) );
#else
   /*
    * Convert npc names to dos compat name.... yuk 
    */
   if( IS_NPC( ch ) )
   {
      for( nmptr = ch->name, bufptr = buf; *nmptr != 0; nmptr++ )
      {
         if( *nmptr != ' ' && *nmptr != '.' )
            *( bufptr++ ) = *nmptr;
         if( bufptr - buf == 8 )
            break;
      }
      *( bufptr ) = 0;
   }
   else
      strcpy( buf, ch->name );

   xprintf( strsave, "%s%s", IS_NPC( ch ) ? NPC_DIR : PLAYER_DIR, cap_nocol( buf ) );
#endif
   /*
    * Tack on a .temp to strsave, use as tempstrsave 
    */

   xprintf( tempstrsave, "%s.temp", strsave );

   if( ( fp = file_open( tempstrsave, "w" ) ) == NULL )
   {
      monitor_chan( "Save_char_obj: file_open", MONITOR_BAD );
      perror( strsave );
   }
   else
   {
      fwrite_char( ch, fp );
      loop_counter = 0;
      if( ch->first_carry != NULL )
         fwrite_obj( ch, ch->first_carry, fp, 0 );
      fprintf( fp, "#END\n" );
   }
   file_close(fp);

   /*
    * Now make temp file the actual pfile... 
    */

   rename( tempstrsave, strsave );
   /*
    * THAT easy?? 
    */

   return;
}



/*
 * Write the char.
 */
void fwrite_char( CHAR_DATA * ch, FILE * fp )
{
   /*
    * UUURRRGGGGHHHHHH!  When writing out ch->lvl[x] no loop used,
    * * instead, the values are just done 0,1,2,etc.. yuck.  -S- 
    */

   AFFECT_DATA *paf;
   int cnt;
   int sn;
   int foo;
   char time_buf[MSL];

   xprintf(time_buf,ctime(&current_time)); /* ctime adding a newline annoyed me --Kline */
   time_buf[strlen(time_buf)-1] = '\0';

   /*
    * Really cool fix for m/c prob.. *laugh* 
    */
   for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      if( ch->lvl[cnt] < 0 || ch->lvl[cnt] == 0 )
         ch->lvl[cnt] = -1;

   fprintf( fp, "#%s\n", IS_NPC( ch ) ? "MOB" : "PLAYER" );

   fprintf( fp, "Revision       %d\n", SAVE_REVISION );
   fprintf( fp, "Name           %s~\n", ch->name );
   fprintf( fp, "Deaf           %s\n", save_bitmask( ch->deaf ) );
   fprintf( fp, "ShortDescr     %s~\n", ch->short_descr );
   fprintf( fp, "LongDescr      %s~\n", ch->long_descr_orig );
   fprintf( fp, "Description    %s~\n", ch->description );
   fprintf( fp, "Prompt         %s~\n", ch->prompt );
   fprintf( fp, "Sex            %d\n", ch->sex );
   fprintf( fp, "LoginSex       %d\n", ch->login_sex );
   fprintf( fp, "Class          %d\n", ch->p_class );
   fprintf( fp, "Race           %d\n", ch->race );
   fprintf( fp, "Level          %d\n", ch->level );
   fprintf( fp, "Sentence       %d\n", ch->sentence );
   fprintf( fp, "Invis          %d\n", ch->invis );
   fprintf( fp, "Incog          %d\n", ch->incog );

   fprintf( fp, "m/c            " );
   for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      fprintf( fp, "%2d ", ch->lvl[cnt] );
   fprintf( fp, "\n" );

   fprintf( fp, "Remort         " );
   for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      fprintf( fp, "%2d ", ch->lvl2[cnt] );
   fprintf( fp, "\n" );

   fprintf( fp, "Adeptlevel     %d\n", ch->adept_level );
   fprintf( fp, "Trust          %d\n", ch->trust );
   fprintf( fp, "Wizbit         %d\n", ch->wizbit );
   fprintf( fp, "Played         %d\n", ch->played + ( int )( current_time - ch->logon ) );
   fprintf( fp, "Note           %ld\n", ch->last_note );
   fprintf( fp, "Room           %d\n",
            ( ch->in_room == get_room_index( ROOM_VNUM_LIMBO )
              && ch->was_in_room != NULL ) ? ch->was_in_room->vnum : ch->in_room->vnum );

   fprintf( fp, "HpManaMove     %d %d %d %d %d %d\n", ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move, ch->max_move );

   fprintf( fp, "Money          %d ", MAX_CURRENCY );
   for( foo = 0; foo < MAX_CURRENCY; foo++ )
      fprintf( fp, "%d ", ch->money->cash_unit[foo] );
   fprintf( fp, "\n" );

   fprintf( fp, "Monitor        %s\n", save_bitmask( ch->pcdata->monitor ) );

   fprintf( fp, "BankMoney      %d ", MAX_CURRENCY );
   for( foo = 0; foo < MAX_CURRENCY; foo++ )
      fprintf( fp, "%d ", ch->bank_money->cash_unit[foo] );
   fprintf( fp, "\n" );

   fprintf( fp, "Exp            %d\n", ch->exp );
   fprintf( fp, "Act            %s\n", save_bitmask( ch->act ) );
   fprintf( fp, "AffectedBy     %d\n", ch->affected_by );
   /*
    * Bug fix from Alander 
    */
   fprintf( fp, "Position       %d\n", ch->position == POS_FIGHTING ? POS_STANDING : ch->position );

   fprintf( fp, "Practice       %d\n", ch->practice );
   fprintf( fp, "SavingThrow    %d\n", ch->saving_throw );
   fprintf( fp, "Alignment      %d\n", ch->alignment );
   fprintf( fp, "Hitroll        %d\n", ch->hitroll );
   fprintf( fp, "Damroll        %d\n", ch->damroll );
   fprintf( fp, "Armor          %d\n", ch->armor );
   fprintf( fp, "Wimpy          %d\n", ch->wimpy );

   if( IS_NPC( ch ) )
   {
      fprintf( fp, "Vnum           %d\n", ch->pIndexData->vnum );
   }
   else
   {
      fprintf( fp, "Clan           %d\n", ch->pcdata->clan );
      fprintf( fp, "Order          %d %d %d %d %d\n",
               ch->pcdata->order[0], ch->pcdata->order[1], ch->pcdata->order[2],
               ch->pcdata->order[3], ch->pcdata->order[4] );
      fprintf( fp, "Password       %s~\n", ch->pcdata->pwd );
      fprintf( fp, "LoadMsg        %s~\n", ch->pcdata->load_msg );
      fprintf( fp, "Bamfin         %s~\n", ch->pcdata->bamfin );
      fprintf( fp, "Bamfout        %s~\n", ch->pcdata->bamfout );
      fprintf( fp, "Roomenter      %s~\n", ch->pcdata->room_enter );
      fprintf( fp, "Roomexit       %s~\n", ch->pcdata->room_exit );
      fprintf( fp, "Title          %s~\n", ch->pcdata->title );
      /*
       * We add a '*' to preserve leading spaces... strip * on load 
       */
      fprintf( fp, "Whoname        *%s~\n", ch->pcdata->who_name );
      fprintf( fp, "Host           %s~\n", ch->pcdata->host );
      fprintf( fp, "Failures       %d\n", ch->pcdata->failures );
      fprintf( fp, "LastLogin      %s~\n", time_buf );
      fprintf( fp, "HiCol          %c~\n", ch->pcdata->hicol );
      fprintf( fp, "DimCol         %c~\n", ch->pcdata->dimcol );
      fprintf( fp, "TermRows       %i\n", ch->pcdata->term_rows );
      fprintf( fp, "TermColumns    %i\n", ch->pcdata->term_columns );
      fprintf( fp, "Email          %s~\n", ch->pcdata->email_address );
      fprintf( fp, "EmailValid     %i\n", ch->pcdata->valid_email );
      fprintf( fp, "AssistMsg      %s~\n", ch->pcdata->assist_msg );

      for( cnt = 0; cnt < MAX_ALIASES; cnt++ )
      {
         fprintf( fp, "Alias_Name%d    %s~\n", cnt, ch->pcdata->alias_name[cnt] );
         fprintf( fp, "Alias%d         %s~\n", cnt, ch->pcdata->alias[cnt] );
      }

      fprintf( fp, "Colors         " );
       for( foo = 0; foo < MAX_COLOR; foo++ )
        fprintf( fp, "%d ", ch->pcdata->color[foo] );
       fprintf( fp, "\n" );

      fprintf( fp, "AttrPerm       %d %d %d %d %d\n",
               ch->pcdata->perm_str,
               ch->pcdata->perm_int, ch->pcdata->perm_wis, ch->pcdata->perm_dex, ch->pcdata->perm_con );

      fprintf( fp, "AttrMax        %d %d %d %d %d\n",
               ch->pcdata->max_str, ch->pcdata->max_int, ch->pcdata->max_wis, ch->pcdata->max_dex, ch->pcdata->max_con );

      fprintf( fp, "QuestAmount    " );
       for( foo = 0; foo < QUEST_MAX_DATA; foo++ )
        fprintf( fp, "%d ", ch->pcdata->quest_info->amount[foo] );
       fprintf( fp, "\n" );
      fprintf( fp, "QuestComplete  %d\n", ch->pcdata->quest_info->quest_complete );
      fprintf( fp, "QuestHint      " );
       for( foo = 0; foo < QUEST_MAX_DATA; foo++ )
        fprintf( fp, "%d ", ch->pcdata->quest_info->quest_hint[foo] );
       fprintf( fp, "\n" );
      fprintf( fp, "QuestIsQuest   %d\n", ch->pcdata->quest_info->is_questing );
      fprintf( fp, "QuestItem      " );
       for( foo = 0; foo < QUEST_MAX_DATA; foo++ )
        fprintf( fp, "%d ", ch->pcdata->quest_info->quest_item_vnum[foo] );
       fprintf( fp, "\n" );
      fprintf( fp, "QuestMob       " );
       for( foo = 0; foo < QUEST_MAX_DATA; foo++ )
        fprintf( fp, "%d ", ch->pcdata->quest_info->quest_mob_vnum[foo] );
       fprintf( fp, "\n" );
      fprintf( fp, "QuestReward    " );
       for( foo = 0; foo < QUEST_MAX_REWARD; foo++ )
        fprintf( fp, "%d ", ch->pcdata->quest_info->quest_reward[foo] );
       fprintf( fp, "\n" );
      fprintf( fp, "QuestTimeLeft  %d\n", ch->pcdata->quest_info->time_left );
      fprintf( fp, "QuestType      %d\n", ch->pcdata->quest_info->quest_type );
      fprintf( fp, "QuestWaitTime  %d\n", ch->pcdata->quest_info->wait_time );

      fprintf( fp, "RecCrusade     %d\n", ch->pcdata->records->crusade );
      fprintf( fp, "RecMdamAmt     %d\n", ch->pcdata->records->mdam_amt );
      fprintf( fp, "RecMdamGsn     %d\n", ch->pcdata->records->mdam_gsn );
      fprintf( fp, "RecPdamAmt     %d\n", ch->pcdata->records->pdam_amt );
      fprintf( fp, "RecPdamGsn     %d\n", ch->pcdata->records->pdam_gsn );
      fprintf( fp, "RecPD          %d\n", ch->pcdata->records->pd );
      fprintf( fp, "RecPK          %d\n", ch->pcdata->records->pk );
      fprintf( fp, "RecMD          %d\n", ch->pcdata->records->md );
      fprintf( fp, "RecMK          %d\n", ch->pcdata->records->mk );
      fprintf( fp, "RecMquestC     %d\n", ch->pcdata->records->mquest_c );
      fprintf( fp, "RecMquestF     %d\n", ch->pcdata->records->mquest_f );
      fprintf( fp, "RecQP          %d\n", ch->pcdata->records->qp );
      fprintf( fp, "RecQpTot       %d\n", ch->pcdata->records->qp_tot );

      fprintf( fp, "SupBloodline   %d\n", ch->pcdata->super->bloodline );
      fprintf( fp, "SupEnergy      %d\n", ch->pcdata->super->energy );
      fprintf( fp, "SupEnergyMax   %d\n", ch->pcdata->super->energy_max );
      fprintf( fp, "SupExp         %d\n", ch->pcdata->super->exp );
      fprintf( fp, "SupGeneration  %d\n", ch->pcdata->super->generation );
      fprintf( fp, "SupLevel       %d\n", ch->pcdata->super->level );
      fprintf( fp, "SupPracs       %d\n", ch->pcdata->super->pracs );
      fprintf( fp, "SupSkillLearn  %d\n", ch->pcdata->super->skills_learned );
      fprintf( fp, "SupSkillMax    %d\n", ch->pcdata->super->skills_max );

      fprintf( fp, "QuestPoints    %d\n", ch->pcdata->quest_points );
      fprintf( fp, "RecallVnum     %d\n", ch->pcdata->recall_vnum );
      fprintf( fp, "GainMana       %d\n", ch->pcdata->mana_from_gain );
      fprintf( fp, "GainHp         %d\n", ch->pcdata->hp_from_gain );
      fprintf( fp, "GainMove       %d\n", ch->pcdata->move_from_gain );
      fprintf( fp, "RulerRank      %d\n", ch->pcdata->ruler_rank );
      fprintf( fp, "Condition      %d %d %d\n", ch->pcdata->condition[0], ch->pcdata->condition[1], ch->pcdata->condition[2] );
      fprintf( fp, "Pagelen        %d\n", ch->pcdata->pagelen );

      for( sn = 0; sn < MAX_SKILL; sn++ )
      {
         if( skill_table[sn].name != NULL && ch->pcdata->learned[sn] > 0 )
         {
            fprintf( fp, "Skill          %d '%s'\n", ch->pcdata->learned[sn], skill_table[sn].name );
         }
      }
#ifdef IMC
   imc_savechar( ch, fp );
#endif
#ifdef I3
   i3save_char( ch, fp );
#endif
   }

   for( paf = ch->first_affect; paf != NULL; paf = paf->next )
   {
      fprintf( fp, "Affect %3d %3d %3d %3d %10d\n", paf->type, paf->duration, paf->modifier, paf->location, paf->bitvector );
   }

   fprintf( fp, "End\n\n" );
   return;
}

/*
 * Write an object and its contents.
 */
void fwrite_obj( CHAR_DATA * ch, OBJ_DATA * obj, FILE * fp, int iNest )
{
   EXTRA_DESCR_DATA *ed;
   AFFECT_DATA *paf;
   short foo;
   /*
    * Slick recursion to write lists backwards,
    *   so loading them will load in forwards order.
    */
   loop_counter++;
   if( loop_counter > 650 )
   {
      abort_wrapper(  );
   }
   if( obj->next_in_carry_list != NULL )
      fwrite_obj( ch, obj->next_in_carry_list, fp, iNest );

   /*
    * Castrate storage characters.
    */

   /*
    * Also bypass no-save objects -S- 
    */

   if( get_psuedo_level( ch ) + 5 < ( obj->level )
       || obj->item_type == ITEM_KEY || obj->item_type == ITEM_BEACON || IS_SET( obj->extra_flags, ITEM_NOSAVE ) )
      return;

   fprintf( fp, "#OBJECT\n" );
   fprintf( fp, "Nest         %d\n", iNest );
   fprintf( fp, "Name         %s~\n", obj->name );
   fprintf( fp, "ShortDescr   %s~\n", obj->short_descr );
   fprintf( fp, "Description  %s~\n", obj->description );
   fprintf( fp, "Durability   %d %d\n", obj->durability, obj->max_durability );
   fprintf( fp, "Vnum         %d\n", obj->pIndexData->vnum );
   fprintf( fp, "ExtraFlags   %d\n", obj->extra_flags );
   fprintf( fp, "WearFlags    %d\n", obj->wear_flags );
   fprintf( fp, "WearLoc      %d\n", obj->wear_loc );
   fprintf( fp, "Money %d ", MAX_CURRENCY );
   for( foo = 0; foo < MAX_CURRENCY; foo++ )
      fprintf( fp, "%d ", obj->money->cash_unit[foo] );
   fprintf( fp, "\n" );
   if( obj->obj_fun != NULL )
      fprintf( fp, "Objfun       %s~\n", rev_obj_fun_lookup( obj->obj_fun ) );

   fprintf( fp, "ClassFlags   %d\n", obj->item_apply );
   /*
    * ClassFlags still used to save fucking with pfiles 
    */
   fprintf( fp, "ItemType     %d\n", obj->item_type );
   fprintf( fp, "Weight       %d\n", obj->weight );
   fprintf( fp, "Level        %d\n", obj->level );
   fprintf( fp, "Timer        %d\n", obj->timer );
   fprintf( fp, "Cost         %d\n", obj->cost );
   fprintf( fp, "Values       %d %d %d %d %d %d %d %d %d %d\n",
            obj->value[0], obj->value[1], obj->value[2], obj->value[3],
            obj->value[4], obj->value[5], obj->value[6], obj->value[7], obj->value[8], obj->value[9] );
   fprintf( fp, "Speed        %4.2f\n", obj->speed );

   switch ( obj->item_type )
   {
      case ITEM_POTION:
      case ITEM_SCROLL:
         if( obj->value[1] > 0 )
         {
            fprintf( fp, "Spell 1      '%s'\n", skill_table[obj->value[1]].name );
         }

         if( obj->value[2] > 0 )
         {
            fprintf( fp, "Spell 2      '%s'\n", skill_table[obj->value[2]].name );
         }

         if( obj->value[3] > 0 )
         {
            fprintf( fp, "Spell 3      '%s'\n", skill_table[obj->value[3]].name );
         }

         break;

      case ITEM_PILL:
      case ITEM_STAFF:
      case ITEM_WAND:
         if( obj->value[3] > 0 )
         {
            fprintf( fp, "Spell 3      '%s'\n", skill_table[obj->value[3]].name );
         }

         break;
   }

   for( paf = obj->first_apply; paf != NULL; paf = paf->next )
   {
      fprintf( fp, "Affect       %d %d %d %d %d\n", paf->type, paf->duration, paf->modifier, paf->location, paf->bitvector );
   }

   for( ed = obj->first_exdesc; ed != NULL; ed = ed->next )
   {
      fprintf( fp, "ExtraDescr   %s~ %s~\n", ed->keyword, ed->description );
   }

   fprintf( fp, "End\n\n" );

   if( obj->first_in_carry_list != NULL )
      fwrite_obj( ch, obj->first_in_carry_list, fp, iNest + 1 );

   return;
}

/* so the stack doesn't get hosed */
void abort_wrapper( void )
{
   abort(  );
};

hash_table *hash_changed_vnums = NULL;

/* Nasty hack for db.c to get back address of ch */
CHAR_DATA *loaded_mob_addr;

/*
 * Load a char and inventory into a new ch structure.
 */

int cur_revision = 0;

bool load_char_obj( DESCRIPTOR_DATA * d, char *name, bool system_call )
{
   int cnt;
   static BITMASK bitmask_zero;
   static PC_DATA pcdata_zero;
   static QUEST_INFO quest_info_zero;
   static RECORD_DATA record_zero;
   static SUPER_DATA super_zero;
   char strsave[MAX_INPUT_LENGTH];
   char tempstrsave[MAX_INPUT_LENGTH];
   char *bufptr, *nmptr;
   CHAR_DATA *ch;
   char buf[MAX_STRING_LENGTH];
   FILE *fp;
   bool found;
   bool is_npc;
   int oldvnum, newvnum;
   int foo;

   if( hash_changed_vnums == NULL )
   {
      /*
       * Initialise hash table for changed vnums, and read in file. 
       */
      hash_changed_vnums = create_hash_table( 1024 );

      if( ( fp = file_open( "area_changes.txt", "r" ) ) != NULL )  /* -- Alty */
      {
         while( !feof( fp ) )
         {
            if( str_cmp( fread_word( fp ), "Obj:" )
                || fread_letter( fp ) != '['
                || ( oldvnum = fread_number( fp ) ) == 0
                || fread_letter( fp ) != ']'
                || str_cmp( fread_word( fp ), "->" )
                || fread_letter( fp ) != '[' || ( newvnum = fread_number( fp ) ) == 0 || fread_letter( fp ) != ']' )
               fread_to_eol( fp );
            else
            {
               fread_to_eol( fp );
               add_hash_entry( hash_changed_vnums, oldvnum, ( void * )newvnum );
            }
         }
         file_close( fp );
      }
   }

   if( ( d == NULL ) /* load npc */
       && ( !system_call ) )
      is_npc = TRUE;
   else
      is_npc = FALSE;

   GET_FREE( ch, char_free );
   clear_char( ch );

   if( !is_npc )
   {
      GET_FREE( ch->pcdata, pcd_free );
      *ch->pcdata = pcdata_zero;
      GET_FREE( ch->pcdata->monitor, bitmask_free );
      *ch->pcdata->monitor = bitmask_zero;
      GET_FREE( ch->pcdata->quest_info, quest_info_free );
      *ch->pcdata->quest_info = quest_info_zero;
      GET_FREE( ch->pcdata->records, record_free );
      *ch->pcdata->records = record_zero;
      GET_FREE( ch->pcdata->super, super_free );
      *ch->pcdata->super = super_zero;

      d->character = ch;

      ch->pcdata->host = str_dup( "Unknown!" );
      ch->pcdata->lastlogin = str_dup( "Unknown!" );
      ch->pcdata->who_name = str_dup( "off" );
      ch->pcdata->pwd = str_dup( "" );
      ch->pcdata->load_msg = str_dup( "" );
      ch->pcdata->bamfin = str_dup( "" );
      ch->pcdata->bamfout = str_dup( "" );
      ch->pcdata->room_enter = str_dup( "" );
      ch->pcdata->room_exit = str_dup( "" );
      ch->pcdata->title = str_dup( "" );
      ch->pcdata->perm_str = 13;
      ch->pcdata->perm_int = 13;
      ch->pcdata->perm_wis = 13;
      ch->pcdata->perm_dex = 13;
      ch->pcdata->perm_con = 13;

      for( foo = 0; foo < QUEST_MAX_DATA; foo++ )
       ch->pcdata->quest_info->amount[foo] = -1;
      ch->pcdata->quest_info->is_questing = FALSE;
      ch->pcdata->quest_info->quest_complete = FALSE;
      for( foo = 0; foo < QUEST_MAX_DATA; foo++ )
       ch->pcdata->quest_info->quest_hint[foo] = FALSE;
      for( foo = 0; foo < QUEST_MAX_DATA; foo++ )
       ch->pcdata->quest_info->quest_item_vnum[foo] = -1;
      for( foo = 0; foo < QUEST_MAX_DATA; foo++ )
       ch->pcdata->quest_info->quest_mob_vnum[foo] = -1;
      for( foo = 0; foo < QUEST_MAX_REWARD; foo++ )
       ch->pcdata->quest_info->quest_reward[foo] = 0;
      ch->pcdata->quest_info->quest_type = 0;
      ch->pcdata->quest_info->time_left = 0;
      ch->pcdata->quest_info->wait_time = 0;

      ch->pcdata->records->crusade = 0;
      ch->pcdata->records->mdam_amt = 0;
      ch->pcdata->records->mdam_gsn = 0;
      ch->pcdata->records->pdam_amt = 0;
      ch->pcdata->records->pdam_gsn = 0;
      ch->pcdata->records->pd = 0;
      ch->pcdata->records->pk = 0;
      ch->pcdata->records->md = 0;
      ch->pcdata->records->mk = 0;
      ch->pcdata->records->mquest_c = 0;
      ch->pcdata->records->mquest_f = 0;
      ch->pcdata->records->qp = 0;
      ch->pcdata->records->qp_tot = 0;

      ch->pcdata->super->energy = 24;

      ch->pcdata->condition[COND_THIRST] = 48;
      ch->pcdata->pagelen = 20;
      ch->pcdata->condition[COND_FULL] = 48;
      ch->pcdata->recall_vnum = 3001;
      ch->pcdata->mana_from_gain = -1;
      ch->pcdata->hp_from_gain = -1;
      ch->pcdata->move_from_gain = -1;
      ch->pcdata->hicol = 'y';
      ch->pcdata->dimcol = 'b';
      ch->pcdata->ruler_rank = 0;
      for( foo = 0; foo < 5; foo++ )
         ch->pcdata->pedit_string[foo] = str_dup( "none" );
      ch->pcdata->pedit_state = str_dup( "none" );
      ch->pcdata->term_rows = 25;
      ch->pcdata->term_columns = 80;
      ch->pcdata->valid_email = FALSE;
      ch->pcdata->email_address = str_dup( "not set" );
      ch->pcdata->assist_msg = str_dup( "'@@eBANZAI!!@@N $N must be assisted!!@@N'" );
      ch->pcdata->quest_points = 0;
      for( foo = 0; foo < MAX_CLASS; foo++ )
         ch->lvl2[foo] = -1;
      ch->adept_level = -1;
      for( cnt = 0; cnt < MAX_ALIASES; cnt++ )
      {
         ch->pcdata->alias_name[cnt] = str_dup( "<none>" );
         ch->pcdata->alias[cnt] = str_dup( "<none>" );
      }
#ifdef IMC
      imc_initchar( ch );
#endif
#ifdef I3
      i3init_char( ch );
#endif
   }
   else
   {
      /*
       * is NPC 
       */
      ch->pcdata = NULL;
      loaded_mob_addr = ch;
   }

   ch->stunTimer = 0;
   ch->first_shield = NULL;
   ch->last_shield = NULL;
   ch->switched = FALSE;
   ch->old_body = NULL;
   ch->desc = d;
   if( ch->name != NULL )
      free_string( ch->name );
   ch->name = str_dup( name );
   ch->prompt = str_dup( "" );
   ch->old_prompt = str_dup( "" );
   ch->prompt = str_dup( "@@g<@@d[@@W%x@@d] [@@e%h@@RH @@l%m@@BM @@r%v@@GV@@d]@@g>@@N" );
   ch->last_note = 0;
   if( is_npc )
      ch->npc = TRUE;
   else
      ch->npc = FALSE;
   ch->sex = SEX_NEUTRAL;
   ch->login_sex = -1;
   ch->current_brand = NULL;
   ch->stance = 0;
   ch->stance_ac_mod = 0;
   ch->stance_dr_mod = 0;
   ch->stance_hr_mod = 0;
   ch->carry_weight = 0.0;
   ch->carry_number = 0;
   ch->ngroup = NULL;

   {
      MONEY_TYPE *money;
      GET_FREE( money, money_type_free );
#ifdef DEBUG_MONEY
      {
         char testbuf[MSL];
         xprintf( testbuf, "loading player money, %s", ch->name );
         if( money->money_key != NULL )
            free_string( money->money_key );
         money->money_key = str_dup( testbuf );
      }
#endif
      for( cnt = 0; cnt < MAX_CURRENCY; cnt++ )
         money->cash_unit[cnt] = 0;
      ch->money = money;
      GET_FREE( money, money_type_free );
#ifdef DEBUG_MONEY
      {
         char testbuf[MSL];
         xprintf( testbuf, "loading player bank, %s", ch->name );
         if( money->money_key != NULL )
            free_string( money->money_key );
         money->money_key = str_dup( testbuf );
      }
#endif
      for( cnt = 0; cnt < MAX_CURRENCY; cnt++ )
         money->cash_unit[cnt] = 0;
      ch->bank_money = money;
   }

   found = FALSE;

   /*
    * parsed player file directories by Yaz of 4th Realm 
    */
   /*
    * decompress if .gz file exists - Thx Alander 
    */

#if !defined(machintosh) && !defined(MSDOS)
   if( is_npc )   /* convert spaces to . */
   {
      for( nmptr = name, bufptr = buf; *nmptr != 0; nmptr++ )
      {
         if( *nmptr == ' ' )
            *( bufptr++ ) = '.';
         else
            *( bufptr++ ) = *nmptr;
      }
      *( bufptr ) = *nmptr;
   }
   else
      strcpy( buf, name );
   xprintf( strsave, "%s%s%s%s", is_npc ? NPC_DIR : PLAYER_DIR, initial( buf ), "/", cap_nocol( buf ) );
#else
   /*
    * Convert npc names to dos compat name.... yuk 
    */
   if( is_npc )
   {
      for( nmptr = ch->name, bufptr = buf; *nmptr != 0; nmptr++ )
      {
         if( *nmptr != ' ' && *nmptr != '.' )
            *( bufptr++ ) = *nmptr;
         if( bufptr - buf == 8 )
            break;
      }
      *( bufptr ) = 0;
   }
   else
      strcpy( buf, name );

   xprintf( strsave, "%s%s", is_npc ? NPC_DIR : PLAYER_DIR, cap_nocol( buf ) );
#endif



#if !defined(macintosh) && !defined(MSDOS)
   xprintf( tempstrsave, "%s%s", strsave, ".gz" );
   if( ( fp = file_open( tempstrsave, "r" ) ) != NULL )
   {
      char buf[MAX_STRING_LENGTH];
      file_close( fp );
      xprintf( buf, "gzip -dfq %s", tempstrsave );
      system( buf );
   }
#endif

   if( ( fp = file_open( strsave, "r" ) ) != NULL )
   {
      int iNest;

      for( iNest = 0; iNest < MAX_NEST; iNest++ )
         rgObjNest[iNest] = NULL;

      found = TRUE;
      for( ;; )
      {
         char letter;
         char *word;

         letter = fread_letter( fp );
         if( letter == '*' )
         {
            fread_to_eol( fp );
            continue;
         }

         if( letter != '#' )
         {
            monitor_chan( "Load_char_obj: # not found.", MONITOR_BAD );
            break;
         }

         word = fread_word( fp );
         if( !str_cmp( word, "PLAYER" ) )
            fread_char( ch, fp );
         else if( !str_cmp( word, "MOB" ) )
            fread_char( ch, fp );
         else if( !str_cmp( word, "OBJECT" ) )
            fread_obj( ch, fp );
         else if( !str_cmp( word, "END" ) )
            break;
         else
         {
            monitor_chan( "Load_char_obj: bad section.", MONITOR_BAD );
            break;
         }
      }
      file_close( fp );
   }

   if( !found && is_npc )
   {
      /*
       * return memory for char back to system. 
       */
      free_char( ch );
   }
   return found;
}



/*
 * Read in a char.
 */

#if defined(KEY)
#undef KEY
#endif

/* BKEY is to load a bitmask field, so it works a bit differently. --Kline */
#define BKEY( literal, field, value )  if ( !str_cmp( word, literal ) ) { load_bitmask(field,value); fMatch = TRUE; break;}
#define KEY( literal, field, value )  if ( !str_cmp( word, literal ) ) { field  = value; fMatch = TRUE;  break;}
#define SKEY( literal, field, value )  if ( !str_cmp( word, literal ) ) { if (field!=NULL) free_string(field);field  = value; fMatch = TRUE;  break;}

void fread_char( CHAR_DATA * ch, FILE * fp )
{
   const char *word;
   bool fMatch;
   int cnt;
   /*
    * Save revision control: 
    */


   /*
    * Ugly fix for pfiles with no balance value 
    */
   ch->balance = 0;
   /*
    * Another fix for m/c levels.. this is getting to be a habit... 
    */

   for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      ch->lvl[cnt] = -1;   /* -1 means no-use of that class */


   for( ;; )
   {
      word = feof( fp ) ? "End" : fread_word( fp );
      fMatch = FALSE;

      switch ( UPPER( word[0] ) )
      {
         case '*':
            fMatch = TRUE;
            fread_to_eol( fp );
            break;

         case 'A':
            BKEY( "Act", ch->act, fp );
            KEY( "AffectedBy", ch->affected_by, fread_number( fp ) );
            KEY( "Alignment", ch->alignment, fread_number( fp ) );
            KEY( "Armor", ch->armor, fread_number( fp ) );
            KEY( "Adeptlevel", ch->adept_level, fread_number( fp ) );
            if( !IS_NPC( ch ) )
            {
               SKEY( "AssistMsg", ch->pcdata->assist_msg, fread_string( fp ) );

               SKEY( "Alias_Name0", ch->pcdata->alias_name[0], fread_string( fp ) );
               SKEY( "Alias_Name1", ch->pcdata->alias_name[1], fread_string( fp ) );
               SKEY( "Alias_Name2", ch->pcdata->alias_name[2], fread_string( fp ) );
               SKEY( "Alias_Name3", ch->pcdata->alias_name[3], fread_string( fp ) );
               SKEY( "Alias_Name4", ch->pcdata->alias_name[4], fread_string( fp ) );
               SKEY( "Alias_Name5", ch->pcdata->alias_name[5], fread_string( fp ) );

               SKEY( "Alias0", ch->pcdata->alias[0], fread_string( fp ) );
               SKEY( "Alias1", ch->pcdata->alias[1], fread_string( fp ) );
               SKEY( "Alias2", ch->pcdata->alias[2], fread_string( fp ) );
               SKEY( "Alias3", ch->pcdata->alias[3], fread_string( fp ) );
               SKEY( "Alias4", ch->pcdata->alias[4], fread_string( fp ) );
               SKEY( "Alias5", ch->pcdata->alias[5], fread_string( fp ) );
            }


            if( !str_cmp( word, "Affect" ) )
            {
               AFFECT_DATA *paf;

               GET_FREE( paf, affect_free );
               paf->type = fread_number( fp );
               paf->duration = fread_number( fp );
               paf->modifier = fread_number( fp );
               paf->location = fread_number( fp );
               paf->bitvector = fread_number( fp );
               paf->caster = NULL;
               if( paf->type != -1 )
                  LINK( paf, ch->first_saved_aff, ch->last_saved_aff, next, prev );
               else
                  PUT_FREE( paf, affect_free );
               fMatch = TRUE;
               break;
            }

            if( !IS_NPC( ch ) )
            {
               if( !str_cmp( word, "AttrMax" ) )
               {
                  ch->pcdata->max_str = fread_number( fp );
                  ch->pcdata->max_int = fread_number( fp );
                  ch->pcdata->max_wis = fread_number( fp );
                  ch->pcdata->max_dex = fread_number( fp );
                  ch->pcdata->max_con = fread_number( fp );
                  fMatch = TRUE;
                  break;
               }

               if( !str_cmp( word, "AttrPerm" ) )
               {
                  ch->pcdata->perm_str = fread_number( fp );
                  ch->pcdata->perm_int = fread_number( fp );
                  ch->pcdata->perm_wis = fread_number( fp );
                  ch->pcdata->perm_dex = fread_number( fp );
                  ch->pcdata->perm_con = fread_number( fp );
                  fMatch = TRUE;
                  break;
               }
            }
            break;

         case 'B':
/*	    KEY( "Balance",     ch->balance,            fread_number( fp ) );  */
            if( !str_cmp( word, "Balance" ) )
            {
               join_money( round_money( fread_number( fp ), TRUE ), ch->bank_money );
               fMatch = TRUE;
               break;
            }
            if( !IS_NPC( ch ) )
            {
               SKEY( "Bamfin", ch->pcdata->bamfin, fread_string( fp ) );
               SKEY( "Bamfout", ch->pcdata->bamfout, fread_string( fp ) );
            }
            if( !str_cmp( word, "BankMoney" ) )
            {
               MONEY_TYPE *transfer;
               int num_coins;
               GET_FREE( transfer, money_type_free );
#ifdef DEBUG_MONEY
               {
                  char testbuf[MSL];
                  xprintf( testbuf, "reading player money, %s", ch->name );
                  transfer->money_key = str_dup( testbuf );
               }
#endif
               num_coins = fread_number( fp );
               for( cnt = 0; cnt < num_coins; cnt++ )
                  transfer->cash_unit[( cnt < MAX_CURRENCY ? cnt : MAX_CURRENCY - 1 )] = fread_number( fp );
               join_money( transfer, ch->bank_money );
               fMatch = TRUE;
               break;
            }
            break;

         case 'C':
            if( !IS_NPC( ch ) )
            {
               KEY( "Clan", ch->pcdata->clan, fread_number( fp ) );
            }
            KEY( "Class", ch->p_class, fread_number( fp ) );
            if( !str_cmp( word, "Colors" ) && !IS_NPC( ch ) )
            {
               int foo;
               for( foo = 0; foo < MAX_COLOR; foo++ )
                  ch->pcdata->color[foo] = fread_number( fp );
               fMatch = TRUE;
               break;
            }

            if( !str_cmp( word, "Condition" ) && !IS_NPC( ch ) )
            {
               ch->pcdata->condition[0] = fread_number( fp );
               ch->pcdata->condition[1] = fread_number( fp );
               ch->pcdata->condition[2] = fread_number( fp );
               fMatch = TRUE;
               break;
            }
            break;

         case 'D':
            KEY( "Damroll", ch->damroll, fread_number( fp ) );
            BKEY( "Deaf", ch->deaf, fp );
            SKEY( "Description", ch->description, fread_string( fp ) );

            if( !str_cmp( word, "DimCol" ) && !IS_NPC( ch ) )
            {
               char *temp;
               temp = fread_string( fp );
               ch->pcdata->dimcol = temp[0];
/*              fread_to_eol( fp );   */
               free_string( temp );
               fMatch = TRUE;
               break;
            }


            break;

         case 'E':
            if( !str_cmp( word, "End" ) )
            {
               if( ch->login_sex < 0 )
                  ch->login_sex = ch->sex;
               return;
            }
            KEY( "Exp", ch->exp, fread_number( fp ) );
            if( !IS_NPC( ch ) )
            {
               KEY( "EmailValid", ch->pcdata->valid_email, fread_number( fp ) );
               SKEY( "Email", ch->pcdata->email_address, fread_string( fp ) );
            }
            break;

         case 'F':
            if( !IS_NPC( ch ) )
            {
               KEY( "Failures", ch->pcdata->failures, fread_number( fp ) );
            }
            break;

         case 'G':
            if( !IS_NPC( ch ) )
            {
               KEY( "GainMana", ch->pcdata->mana_from_gain, fread_number( fp ) );
               KEY( "GainHp", ch->pcdata->hp_from_gain, fread_number( fp ) );
               KEY( "GainMove", ch->pcdata->move_from_gain, fread_number( fp ) );
            }
/*	    KEY( "Gold",        ch->gold,               fread_number( fp ) );  */

            if( !str_cmp( word, "Gold" ) )
            {
               join_money( round_money( fread_number( fp ), TRUE ), ch->money );
               fMatch = TRUE;
               break;
            }
            break;

         case 'H':
            KEY( "Hitroll", ch->hitroll, fread_number( fp ) );
            if( !IS_NPC( ch ) )
	    {
               SKEY( "Host", ch->pcdata->host, fread_string( fp ) );
            }

            if( !str_cmp( word, "HiCol" ) && !IS_NPC( ch ) )
            {
               char *temp;
               temp = fread_string( fp );
               ch->pcdata->hicol = temp[0];
/*              fread_to_eol( fp );   */
               free_string( temp );
               fMatch = TRUE;
               break;
            }

            if( !str_cmp( word, "HpManaMove" ) )
            {
               ch->hit = fread_number( fp );
               ch->max_hit = fread_number( fp );
               ch->mana = fread_number( fp );
               ch->max_mana = fread_number( fp );
               ch->move = fread_number( fp );
               ch->max_move = fread_number( fp );
               fMatch = TRUE;
               break;
            }
            break;

         case 'I':
            KEY( "Incog", ch->incog, fread_number( fp ) );
            KEY( "Invis", ch->invis, fread_number( fp ) );
#ifdef IMC
            if( ( fMatch = imc_loadchar( ch, fp, word ) ) )
               break;
#endif
#ifdef I3
            if( ( fMatch = i3load_char( ch, fp, word ) ) )
               break;
#endif
            break;



         case 'L':

            KEY( "Level", ch->level, fread_number( fp ) );
            SKEY( "LongDescr", ch->long_descr, fread_string( fp ) );
            KEY( "LoginSex", ch->login_sex, fread_number( fp ) );

            if( !IS_NPC( ch ) )
            {
               SKEY( "LastLogin", ch->pcdata->lastlogin, fread_string( fp ) );
               SKEY( "LoadMsg", ch->pcdata->load_msg, fread_string( fp ) );
            }
            break;

         case 'M':
            if( !IS_NPC( ch ) )
            {
               BKEY( "Monitor", ch->pcdata->monitor, fp );
            }
            if( !str_cmp( word, "Money" ) )
            {
               MONEY_TYPE *transfer;
               int num_coins;
               GET_FREE( transfer, money_type_free );
#ifdef DEBUG_MONEY
               {
                  char testbuf[MSL];
                  xprintf( testbuf, "reading player money, %s", ch->name );
                  transfer->money_key = str_dup( testbuf );
               }
#endif
               num_coins = fread_number( fp );
               for( cnt = 0; cnt < num_coins; cnt++ )
                  transfer->cash_unit[( cnt < MAX_CURRENCY ? cnt : MAX_CURRENCY - 1 )] = fread_number( fp );
               join_money( transfer, ch->money );
               ch->carry_weight += money_weight( transfer );
               fMatch = TRUE;
               break;
            }

            if( !str_cmp( word, "m/c" ) )
            {
               switch ( cur_revision )
               {
                  default:
                     for( cnt = 0; cnt < MAX_CLASS; cnt++ )
                        ch->lvl[cnt] = fread_number( fp );
                     break;
               }
               fMatch = TRUE;
            }
            break;

         case 'N':
            if( !str_cmp( word, "Name" ) )
            {
               /*
                * Name already set externally.
                */
               fread_to_eol( fp );
               fMatch = TRUE;
               break;
            }
            KEY( "Note", ch->last_note, fread_number( fp ) );
            break;

         case 'O':
            if( !str_cmp( word, "Order" ) && !IS_NPC( ch ) )
            {
               for( cnt = 0; cnt < MAX_CLASS; cnt++ )
                  ch->pcdata->order[cnt] = fread_number( fp );
               fMatch = TRUE;
               break;
            }
            break;

         case 'P':
            if( !IS_NPC( ch ) )
            {
               KEY( "Pagelen", ch->pcdata->pagelen, fread_number( fp ) );
               SKEY( "Password", ch->pcdata->pwd, fread_string( fp ) );
            }
            KEY( "Played", ch->played, fread_number( fp ) );
            KEY( "Position", ch->position, fread_number( fp ) );
            KEY( "Practice", ch->practice, fread_number( fp ) );
            SKEY( "Prompt", ch->prompt, fread_string( fp ) );
            break;

         case 'Q':
            if( !IS_NPC( ch ) )
            {
               if( !str_cmp( word, "QuestAmount" ) )
               {
                  for( cnt = 0; cnt < QUEST_MAX_DATA; cnt++ )
                     ch->pcdata->quest_info->amount[cnt] = fread_number( fp );
                  fMatch = TRUE;
                  break;
               }
               KEY( "QuestComplete", ch->pcdata->quest_info->quest_complete, fread_number( fp ) );
               if( !str_cmp( word, "QuestHint" ) )
               {
                  for( cnt = 0; cnt < QUEST_MAX_DATA; cnt++ )
                     ch->pcdata->quest_info->quest_hint[cnt] = fread_number( fp );
                  fMatch = TRUE;
                  break;
               }
               KEY( "QuestIsQuest", ch->pcdata->quest_info->is_questing, fread_number( fp ) );
               if( !str_cmp( word, "QuestItem" ) )
               {
                  for( cnt = 0; cnt < QUEST_MAX_DATA; cnt++ )
                     ch->pcdata->quest_info->quest_item_vnum[cnt] = fread_number( fp );
                  fMatch = TRUE;
                  break;
               }
               if( !str_cmp( word, "QuestMob" ) )
               {
                  for( cnt = 0; cnt < QUEST_MAX_DATA; cnt++ )
                     ch->pcdata->quest_info->quest_mob_vnum[cnt] = fread_number( fp );
                  fMatch = TRUE;
                  break;
               }
               KEY( "QuestPoints", ch->pcdata->quest_points, fread_number( fp ) );
               if( !str_cmp( word, "QuestReward" ) )
               {
                  for( cnt = 0; cnt < QUEST_MAX_REWARD; cnt++ )
                     ch->pcdata->quest_info->quest_reward[cnt] = fread_number( fp );
                  fMatch = TRUE;
                  break;
               }
               KEY( "QuestTimeLeft", ch->pcdata->quest_info->time_left, fread_number( fp ) );
               KEY( "QuestType", ch->pcdata->quest_info->quest_type, fread_number( fp ) );
               KEY( "QuestWaitTime", ch->pcdata->quest_info->wait_time, fread_number( fp ) );
            }
            break;

         case 'R':
            KEY( "Race", ch->race, fread_number( fp ) );
            if( !IS_NPC( ch ) )
            {
               KEY( "RecCrusade", ch->pcdata->records->crusade, fread_number( fp ) );
               KEY( "RecMdamAmt", ch->pcdata->records->mdam_amt, fread_number( fp ) );
               KEY( "RecMdamGsn", ch->pcdata->records->mdam_gsn, fread_number( fp ) );
               KEY( "RecPdamAmt", ch->pcdata->records->pdam_amt, fread_number( fp ) );
               KEY( "RecPdamGsn", ch->pcdata->records->pdam_gsn, fread_number( fp ) );
               KEY( "RecPD", ch->pcdata->records->pd, fread_number( fp ) );
               KEY( "RecPK", ch->pcdata->records->pk, fread_number( fp ) );
               KEY( "RecMD", ch->pcdata->records->md, fread_number( fp ) );
               KEY( "RecMK", ch->pcdata->records->mk, fread_number( fp ) );
               KEY( "RecMquestC", ch->pcdata->records->mquest_c, fread_number( fp ) );
               KEY( "RecMquestF", ch->pcdata->records->mquest_f, fread_number( fp ) );
               KEY( "RecQP", ch->pcdata->records->qp, fread_number( fp ) );
               KEY( "RecQpTot", ch->pcdata->records->qp_tot, fread_number( fp ) );
            }
            KEY( "Revision", cur_revision, fread_number( fp ) );
            if( !IS_NPC( ch ) )
            {
               SKEY( "Roomenter", ch->pcdata->room_enter, fread_string( fp ) );
               SKEY( "Roomexit", ch->pcdata->room_exit, fread_string( fp ) );
               KEY( "RulerRank", ch->pcdata->ruler_rank, fread_number( fp ) );
            }

            if( !str_cmp( word, "Remort" ) )
            {
               for( cnt = 0; cnt < MAX_CLASS; cnt++ )
                  ch->lvl2[cnt] = fread_number( fp );
               fMatch = TRUE;
               break;
            }

            if( !str_cmp( word, "Room" ) )
            {
               ch->in_room = get_room_index( fread_number( fp ) );
               if( ch->in_room == NULL )
                  ch->in_room = get_room_index( ROOM_VNUM_LIMBO );
               fMatch = TRUE;
               break;
            }
            if( !IS_NPC( ch ) )
            {
               KEY( "RecallVnum", ch->pcdata->recall_vnum, fread_number( fp ) );
            }

            break;

         case 'S':
            KEY( "SavingThrow", ch->saving_throw, fread_number( fp ) );
            KEY( "Sentence", ch->sentence, fread_number( fp ) );
            KEY( "Sex", ch->sex, fread_number( fp ) );
            SKEY( "ShortDescr", ch->short_descr, fread_string( fp ) );

            if( !str_cmp( word, "Skill" ) && !IS_NPC( ch ) )
            {
               int sn;
               int value;
               char *skill_word;
               value = fread_number( fp );
               skill_word = fread_word( fp );
               sn = skill_lookup( skill_word );
               if( sn < 0 )
               {
                  xprintf_2( log_buf, "Loading pfile %s, unknown skill %s.", ch->name, skill_word );
                  monitor_chan( log_buf, MONITOR_BAD );
               }
               else
                  ch->pcdata->learned[sn] = value;
               fMatch = TRUE;
            }

            if( !IS_NPC( ch ) )
            {
               KEY( "SupBloodline", ch->pcdata->super->bloodline, fread_number( fp ) );
               KEY( "SupEnergy", ch->pcdata->super->energy, fread_number( fp ) );
               KEY( "SupEnergyMax", ch->pcdata->super->energy_max, fread_number( fp ) );
               KEY( "SupExp", ch->pcdata->super->exp, fread_number( fp ) );
               KEY( "SupGeneration", ch->pcdata->super->generation, fread_number( fp ) );
               KEY( "SupLevel", ch->pcdata->super->level, fread_number( fp ) );
               KEY( "SupPracs", ch->pcdata->super->pracs, fread_number( fp ) );
               KEY( "SupSkillLearn", ch->pcdata->super->skills_learned, fread_number( fp ) );
               KEY( "SupSkillMax", ch->pcdata->super->skills_max, fread_number( fp ) );
            }
            break;

         case 'T':
            KEY( "Trust", ch->trust, fread_number( fp ) );
            if( !IS_NPC( ch ) )
            {
               KEY( "TermRows", ch->pcdata->term_rows, fread_number( fp ) );
               KEY( "TermColumns", ch->pcdata->term_columns, fread_number( fp ) );
               SKEY( "Title", ch->pcdata->title, fread_string( fp ) );
            }
            break;

         case 'V':
            if( !str_cmp( word, "Vnum" ) && IS_NPC( ch ) )
            {
               ch->pIndexData = get_mob_index( fread_number( fp ) );
               fMatch = TRUE;
               break;
            }
            break;

         case 'W':
            KEY( "Wimpy", ch->wimpy, fread_number( fp ) );
            KEY( "Wizbit", ch->wizbit, fread_number( fp ) );
            if( !str_cmp( word, "Whoname" ) && !IS_NPC( ch ) )
            {
               char buf[MSL];

               if( ch->pcdata->who_name != NULL )
                  free_string( ch->pcdata->who_name );
               ch->pcdata->who_name = fread_string( fp );
               xprintf( buf, "%s", ch->pcdata->who_name + 1 );

               free_string( ch->pcdata->who_name );
               ch->pcdata->who_name = str_dup( buf );
               fMatch = TRUE;
               break;
            }

            break;
      }

      /*
       * Make sure old chars have this field - Kahn 
       */
      if( !IS_NPC( ch ) )
      {
         if( !ch->pcdata->pagelen )
            ch->pcdata->pagelen = 20;
         if( !ch->prompt || *ch->prompt == '\0' )
            ch->prompt = str_dup( "<%h %m %mv> " );
      }

      ch->long_descr_orig = str_dup( ch->long_descr );
      if( !fMatch )
      {
         xprintf_2( log_buf, "Loading in pfile :%s, no match for ( %s ).", ch->name, word );
         monitor_chan( log_buf, MONITOR_BAD );
         fread_to_eol( fp );
      }
   }


}


#define TEMP_VNUM 3090

extern int top_obj_index;

void fread_obj( CHAR_DATA * ch, FILE * fp )
{
   static OBJ_DATA obj_zero;
   OBJ_DATA *obj;
   const char *word;
   int iNest;
   bool fMatch;
   bool fNest;
   bool fVnum;
   int Temp_Obj = 0, OldVnum = 0;

   GET_FREE( obj, obj_free );
   *obj = obj_zero;
   obj->name = str_dup( "" );
   obj->short_descr = str_dup( "" );
   obj->description = str_dup( "" );
   {
      MONEY_TYPE *money;
      short cnt;
      GET_FREE( money, money_type_free );
#ifdef DEBUG_MONEY
      {
         char testbuf[MSL];
         xprintf( testbuf, "loading obj money, %s", obj->name );
         money->money_key = str_dup( testbuf );
      }
#endif
      for( cnt = 0; cnt < MAX_CURRENCY; cnt++ )
         money->cash_unit[cnt] = 0;
      obj->money = money;
   }
   fNest = FALSE;
   fVnum = TRUE;
   iNest = 0;

   for( ;; )
   {
      word = feof( fp ) ? "End" : fread_word( fp );
      fMatch = FALSE;

      switch ( UPPER( word[0] ) )
      {
         case '*':
            fMatch = TRUE;
            fread_to_eol( fp );
            break;

         case 'A':
            if( !str_cmp( word, "Affect" ) )
            {
               AFFECT_DATA *paf;

               GET_FREE( paf, affect_free );
               paf->type = fread_number( fp );
               paf->duration = fread_number( fp );
               paf->modifier = fread_number( fp );
               paf->location = fread_number( fp );
               paf->bitvector = fread_number( fp );
               LINK( paf, obj->first_apply, obj->last_apply, next, prev );
               fMatch = TRUE;
               break;
            }
            break;

         case 'C':
            KEY( "Cost", obj->cost, fread_number( fp ) );
            KEY( "ClassFlags", obj->item_apply, fread_number( fp ) );
            break;

         case 'D':
            SKEY( "Description", obj->description, fread_string( fp ) );
            if( !str_cmp( word, "Durability" ) )
            {
             obj->durability = fread_number( fp );
             obj->max_durability = fread_number( fp );
             fMatch = TRUE;
             break;
            }
            break;

         case 'E':
            KEY( "ExtraFlags", obj->extra_flags, fread_number( fp ) );

            if( !str_cmp( word, "ExtraDescr" ) )
            {
               EXTRA_DESCR_DATA *ed;

               GET_FREE( ed, exdesc_free );
               ed->keyword = fread_string( fp );
               ed->description = fread_string( fp );
               LINK( ed, obj->first_exdesc, obj->last_exdesc, next, prev );
               fMatch = TRUE;
            }

            if( !str_cmp( word, "End" ) )
            {
               if( !fNest || !fVnum )
               {
                  AFFECT_DATA *paf;
                  EXTRA_DESCR_DATA *ed;

                  monitor_chan( "Fread_obj: incomplete object.", MONITOR_BAD );
                  while( ( paf = obj->first_apply ) != NULL )
                  {
                     obj->first_apply = paf->next;
                     PUT_FREE( paf, affect_free );
                  }
                  while( ( ed = obj->first_exdesc ) != NULL )
                  {
                     obj->first_exdesc = ed->next;
                     PUT_FREE( ed, exdesc_free );
                  }
                  PUT_FREE( obj, obj_free );
                  return;
               }
               else
               {
                  LINK( obj, first_obj, last_obj, next, prev );
                  obj->pIndexData->count++;

                  if( Temp_Obj )
                  {
                     int newvnum;
                     OBJ_INDEX_DATA *pObjIndex;
                     int nMatch = 0;
                     int vnum;

                     /*
                      * One of three things:
                      * Obj Vnum was deleted
                      * Obj Vnum was moved
                      * Obj Vnum was previously deleted 
                      */
                     newvnum = TEMP_VNUM;

                     if( OldVnum != TEMP_VNUM )
                     {
                        /*
                         * Check on move table 
                         */
                        if( ( newvnum = ( int )get_hash_entry( hash_changed_vnums, OldVnum ) ) != 0 )
                        {
                           obj->pIndexData = get_obj_index( newvnum );
                           if( obj->pIndexData == NULL )
                           {
                              obj->pIndexData = get_obj_index( TEMP_VNUM );
                              newvnum = TEMP_VNUM;
                           }
                        }
                     }

                     if( newvnum == TEMP_VNUM )
                     {
                        /*
                         * Scan through objects, trying to find a matching description 
                         */
                        for( vnum = 0; nMatch < top_obj_index; vnum++ )
                        {
                           if( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
                           {
                              nMatch++;
                              if( !str_cmp( obj->short_descr, pObjIndex->short_descr ) )
                              {
                                 obj->pIndexData = pObjIndex;
                                 break;
                              }
                           }
                        }
                     }

                  }

                  if( iNest == 0 || rgObjNest[iNest] == NULL )
                     obj_to_char( obj, ch );
                  else  /*
                         * if ( rgObjNest[iNest-1] == obj )
                         * obj_to_char( obj, ch );
                         * else */
                     obj_to_obj( obj, rgObjNest[iNest - 1] );
                  return;
               }
            }
            break;

         case 'I':
            KEY( "ItemType", obj->item_type, fread_number( fp ) );
            break;

         case 'L':
            KEY( "Level", obj->level, fread_number( fp ) );
            break;
         case 'M':
            if( !str_cmp( word, "Money" ) )
            {
               MONEY_TYPE *transfer;
               int num_coins, cnt;
               GET_FREE( transfer, money_type_free );
#ifdef DEBUG_MONEY
               {
                  char testbuf[MSL];
                  xprintf( testbuf, "loading obj money, %s", obj->name );
                  transfer->money_key = str_dup( testbuf );
               }
#endif
               num_coins = fread_number( fp );
               for( cnt = 0; cnt < num_coins; cnt++ )
                  transfer->cash_unit[( cnt < MAX_CURRENCY ? cnt : MAX_CURRENCY - 1 )] = fread_number( fp );
               join_money( transfer, obj->money );
               fMatch = TRUE;
               break;
            }
         case 'N':
            SKEY( "Name", obj->name, fread_string( fp ) );

            if( !str_cmp( word, "Nest" ) )
            {
               iNest = fread_number( fp );
               if( iNest < 0 || iNest >= MAX_NEST )
               {
                  monitor_chan( "Fread_obj: bad nest.", MONITOR_BAD );
               }
               else
               {
                  rgObjNest[iNest] = obj;
                  fNest = TRUE;
               }
               fMatch = TRUE;
            }
            break;

         case 'O':
/*	   KEY( "Objfun", obj->obj_fun,  obj_fun_lookup( fread_string( fp ) ) );  */
            if( !str_cmp( word, "Objfun" ) )
            {
               char *dumpme;
               dumpme = fread_string( fp );
               obj->obj_fun = obj_fun_lookup( dumpme );
               free_string( dumpme );
               fMatch = TRUE;
            }
            break;

         case 'S':
            SKEY( "ShortDescr", obj->short_descr, fread_string( fp ) );
            KEY( "Speed", obj->speed, fread_float( fp ) );

            if( !str_cmp( word, "Spell" ) )
            {
               int iValue;
               int sn;

               iValue = fread_number( fp );
               sn = skill_lookup( fread_word( fp ) );
               if( iValue < 0 || iValue > 3 )
               {
                  monitor_chan( "Fread_obj: bad iValue ", MONITOR_BAD );
               }
               else if( sn < 0 )
               {
                  monitor_chan( "Fread_obj: unknown skill.", MONITOR_BAD );
               }
               else
               {
                  obj->value[iValue] = sn;
               }
               fMatch = TRUE;
               break;
            }

            break;

         case 'T':
            KEY( "Timer", obj->timer, fread_number( fp ) );
            break;

         case 'V':
            if( !str_cmp( word, "Values" ) )
            {
               int looper;

               if( cur_revision < UPGRADE_REVISION )
               {
                  obj->value[0] = fread_number( fp );
                  obj->value[1] = fread_number( fp );
                  obj->value[2] = fread_number( fp );
                  obj->value[3] = fread_number( fp );
                  for( looper = 4; looper < 10; obj->value[looper] = 0, looper++ );
               }
               else
               {
                  for( looper = 0; looper < 10; obj->value[looper] = fread_number( fp ), looper++ );
               }
               fMatch = TRUE;
               break;
            }

            if( !str_cmp( word, "Vnum" ) )
            {
               int vnum;

               vnum = fread_number( fp );


               if( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL || vnum == TEMP_VNUM )
               {
                  /*
                   * Set flag saying that object is temporary 
                   */
                  Temp_Obj = 1;
                  OldVnum = vnum;
                  vnum = TEMP_VNUM;
                  obj->pIndexData = get_obj_index( vnum );
               }
               /*
                * bug( "Fread_obj: bad vnum %d.", vnum ); This killed it. 
                */
               else
                  fVnum = TRUE;
               fMatch = TRUE;
               break;
            }
            break;

         case 'W':
            if( !str_cmp( word, "WearLoc" ) )
            {
               if( cur_revision < UPGRADE_REVISION )
               {
                  int temp_loc;
                  temp_loc = fread_number( fp );
                  obj->wear_loc = -1;
                  fMatch = TRUE;
                  break;
               }
               else
               {
                  KEY( "WearLoc", obj->wear_loc, fread_number( fp ) );
               }
            }
            KEY( "WearFlags", obj->wear_flags, fread_number( fp ) );
            KEY( "Weight", obj->weight, fread_number( fp ) );
            break;

      }

      if( !fMatch )
      {
         monitor_chan( "Fread_obj: no match.", MONITOR_BAD );
         fread_to_eol( fp );
      }
   }
}
void fread_corpse( FILE * fp )
{
   static OBJ_DATA obj_zero;
   OBJ_DATA *obj;
   const char *word;
   int iNest;
   bool fMatch;
   bool fNest;
   bool fVnum;
   int Temp_Obj = 0, OldVnum = 0;
   int this_room_vnum;

   GET_FREE( obj, obj_free );
   *obj = obj_zero;
   obj->name = str_dup( "" );
   obj->short_descr = str_dup( "" );
   obj->description = str_dup( "" );
   {
      MONEY_TYPE *money;
      short cnt;
      GET_FREE( money, money_type_free );
#ifdef DEBUG_MONEY
      {
         char testbuf[MSL];
         xprintf( testbuf, "loading obj money, %s", obj->name );
         money->money_key = str_dup( testbuf );
      }
#endif
      for( cnt = 0; cnt < MAX_CURRENCY; cnt++ )
         money->cash_unit[cnt] = 0;
      obj->money = money;
   }
   fNest = FALSE;
   fVnum = TRUE;
   iNest = 0;
   this_room_vnum = 0;

   for( ;; )
   {
      word = feof( fp ) ? "End" : fread_word( fp );
      fMatch = FALSE;

      switch ( UPPER( word[0] ) )
      {
         case '*':
            fMatch = TRUE;
            fread_to_eol( fp );
            break;

         case 'A':
            if( !str_cmp( word, "Affect" ) )
            {
               AFFECT_DATA *paf;

               GET_FREE( paf, affect_free );
               paf->type = fread_number( fp );
               paf->duration = fread_number( fp );
               paf->modifier = fread_number( fp );
               paf->location = fread_number( fp );
               paf->bitvector = fread_number( fp );
               LINK( paf, obj->first_apply, obj->last_apply, next, prev );
               fMatch = TRUE;
               break;
            }
            break;

         case 'C':
            KEY( "Cost", obj->cost, fread_number( fp ) );
            KEY( "ClassFlags", obj->item_apply, fread_number( fp ) );
            break;

         case 'D':
            SKEY( "Description", obj->description, fread_string( fp ) );
            break;

         case 'E':
            KEY( "ExtraFlags", obj->extra_flags, fread_number( fp ) );

            if( !str_cmp( word, "ExtraDescr" ) )
            {
               EXTRA_DESCR_DATA *ed;

               GET_FREE( ed, exdesc_free );
               ed->keyword = fread_string( fp );
               ed->description = fread_string( fp );
               LINK( ed, obj->first_exdesc, obj->last_exdesc, next, prev );
               fMatch = TRUE;
            }

            if( !str_cmp( word, "End" ) )
            {
               if( !fNest || !fVnum )
               {
                  AFFECT_DATA *paf;
                  EXTRA_DESCR_DATA *ed;

                  monitor_chan( "Fread_obj: incomplete object.", MONITOR_BAD );
                  while( ( paf = obj->first_apply ) != NULL )
                  {
                     obj->first_apply = paf->next;
                     PUT_FREE( paf, affect_free );
                  }
                  while( ( ed = obj->first_exdesc ) != NULL )
                  {
                     obj->first_exdesc = ed->next;
                     PUT_FREE( ed, exdesc_free );
                  }
                  PUT_FREE( obj, obj_free );
                  return;
               }
               else
               {
                  LINK( obj, first_obj, last_obj, next, prev );
                  obj->pIndexData->count++;

                  if( Temp_Obj )
                  {
                     int newvnum;
                     OBJ_INDEX_DATA *pObjIndex;
                     int nMatch = 0;
                     int vnum;

                     /*
                      * One of three things:
                      * Obj Vnum was deleted
                      * Obj Vnum was moved
                      * Obj Vnum was previously deleted 
                      */
                     newvnum = TEMP_VNUM;

                     if( OldVnum != TEMP_VNUM )
                     {
                        /*
                         * Check on move table 
                         */
                        if( ( newvnum = ( int )get_hash_entry( hash_changed_vnums, OldVnum ) ) != 0 )
                        {
                           obj->pIndexData = get_obj_index( newvnum );
                           if( obj->pIndexData == NULL )
                           {
                              obj->pIndexData = get_obj_index( TEMP_VNUM );
                              newvnum = TEMP_VNUM;
                           }
                        }
                     }

                     if( newvnum == TEMP_VNUM )
                     {
                        /*
                         * Scan through objects, trying to find a matching description 
                         */
                        for( vnum = 0; nMatch < top_obj_index; vnum++ )
                        {
                           if( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
                           {
                              nMatch++;
                              if( !str_cmp( obj->short_descr, pObjIndex->short_descr ) )
                              {
                                 obj->pIndexData = pObjIndex;
                                 break;
                              }
                           }
                        }
                     }

                  }

                  if( iNest == 0 || rgObjNest[iNest] == NULL )
                     obj_to_room( obj, get_room_index( this_room_vnum ) );
                  else
                     obj_to_obj( obj, rgObjNest[iNest - 1] );
                  return;
               }
            }
            break;

         case 'I':
            KEY( "ItemType", obj->item_type, fread_number( fp ) );
            break;

         case 'L':
            KEY( "Level", obj->level, fread_number( fp ) );
            break;
         case 'M':
            if( !str_cmp( word, "Money" ) )
            {
               MONEY_TYPE *transfer;
               int num_coins, cnt;
               GET_FREE( transfer, money_type_free );
#ifdef DEBUG_MONEY
               {
                  char testbuf[MSL];
                  xprintf( testbuf, "loading obj money, %s", obj->name );
                  transfer->money_key = str_dup( testbuf );
               }
#endif
               num_coins = fread_number( fp );
               for( cnt = 0; cnt < num_coins; cnt++ )
                  transfer->cash_unit[( cnt < MAX_CURRENCY ? cnt : MAX_CURRENCY - 1 )] = fread_number( fp );
               join_money( transfer, obj->money );
               fMatch = TRUE;
               break;
            }
         case 'N':
            KEY( "Name", obj->name, fread_string( fp ) );

            if( !str_cmp( word, "Nest" ) )
            {
               iNest = fread_number( fp );
               if( iNest < 0 || iNest >= MAX_NEST )
               {
                  monitor_chan( "Fread_obj: bad nest.", MONITOR_BAD );
               }
               else
               {
                  rgObjNest[iNest] = obj;
                  fNest = TRUE;
               }
               fMatch = TRUE;
            }
            break;

         case 'O':
/*	   KEY( "Objfun", obj->obj_fun,  obj_fun_lookup( fread_string( fp ) ) );  */
            if( !str_cmp( word, "Objfun" ) )
            {
               char *dumpme;
               dumpme = fread_string( fp );
               obj->obj_fun = obj_fun_lookup( dumpme );
               free_string( dumpme );
               fMatch = TRUE;
            }
            break;

         case 'S':
            SKEY( "ShortDescr", obj->short_descr, fread_string( fp ) );
            KEY( "Speed", obj->speed, fread_float( fp ) );

            if( !str_cmp( word, "Spell" ) )
            {
               int iValue;
               int sn;

               iValue = fread_number( fp );
               sn = skill_lookup( fread_word( fp ) );
               if( iValue < 0 || iValue > 3 )
               {
                  monitor_chan( "Fread_obj: bad iValue ", MONITOR_BAD );
               }
               else if( sn < 0 )
               {
                  monitor_chan( "Fread_obj: unknown skill.", MONITOR_BAD );
               }
               else
               {
                  obj->value[iValue] = sn;
               }
               fMatch = TRUE;
               break;
            }

            break;

         case 'T':
            KEY( "Timer", obj->timer, fread_number( fp ) );
            break;

         case 'V':
            if( !str_cmp( word, "Values" ) )
            {
               obj->value[0] = fread_number( fp );
               obj->value[1] = fread_number( fp );
               obj->value[2] = fread_number( fp );
               obj->value[3] = fread_number( fp );
               fMatch = TRUE;
               break;
            }

            if( !str_cmp( word, "Vnum" ) )
            {
               int vnum;

               vnum = fread_number( fp );


               if( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL || vnum == TEMP_VNUM )
               {
                  /*
                   * Set flag saying that object is temporary 
                   */
                  Temp_Obj = 1;
                  OldVnum = vnum;
                  vnum = TEMP_VNUM;
                  obj->pIndexData = get_obj_index( vnum );
               }
               /*
                * bug( "Fread_obj: bad vnum %d.", vnum ); This killed it. 
                */
               else
                  fVnum = TRUE;
               fMatch = TRUE;
               break;
            }
            break;

         case 'W':
            KEY( "WearFlags", obj->wear_flags, fread_number( fp ) );
            KEY( "WearLoc", obj->wear_loc, fread_number( fp ) );
            KEY( "Weight", obj->weight, fread_number( fp ) );
            KEY( "WhereVnum", this_room_vnum, fread_number( fp ) );
            break;

      }

      if( !fMatch )
      {
         monitor_chan( "Fread_obj: no match.", MONITOR_BAD );
         fread_to_eol( fp );
      }
   }
}


void fwrite_corpse( OBJ_DATA * obj, FILE * fp, int iNest )
{
   EXTRA_DESCR_DATA *ed;
   AFFECT_DATA *paf;
   int where_vnum = 3300;
   short foo;
   /*
    * Slick recursion to write lists backwards,
    *   so loading them will load in forwards order.
    */
   if( obj->next_in_carry_list != NULL )
      fwrite_corpse( obj->next_in_carry_list, fp, iNest );

   if( obj->in_obj != NULL )
      where_vnum = 3300;
   if( obj->in_room != NULL )
      where_vnum = obj->in_room->vnum;

   if( obj->in_room == NULL && obj->in_obj == NULL )
      obj->in_room = get_room_index( ROOM_VNUM_LIMBO );


   fprintf( fp, "#OBJECT\n" );
   fprintf( fp, "WhereVnum    %d\n", where_vnum );

   fprintf( fp, "Nest         %d\n", iNest );
   fprintf( fp, "Name         %s~\n", obj->name );
   fprintf( fp, "ShortDescr   %s~\n", obj->short_descr );
   fprintf( fp, "Description  %s~\n", obj->description );
   fprintf( fp, "Vnum         %d\n", obj->pIndexData->vnum );
   fprintf( fp, "ExtraFlags   %d\n", obj->extra_flags );
   fprintf( fp, "WearFlags    %d\n", obj->wear_flags );
   fprintf( fp, "WearLoc      %d\n", obj->wear_loc );
   fprintf( fp, "Money %d ", MAX_CURRENCY );
   for( foo = 0; foo < MAX_CURRENCY; foo++ )
      fprintf( fp, "%d ", obj->money->cash_unit[foo] );
   fprintf( fp, "\n" );
   if( obj->obj_fun != NULL )
      fprintf( fp, "Objfun       %s~\n", rev_obj_fun_lookup( obj->obj_fun ) );

   fprintf( fp, "ClassFlags   %d\n", obj->item_apply );
   /*
    * ClassFlags still used to save fucking with pfiles 
    */
   fprintf( fp, "ItemType     %d\n", obj->item_type );
   fprintf( fp, "Weight       %d\n", obj->weight );
   fprintf( fp, "Level        %d\n", obj->level );
   fprintf( fp, "Timer        %d\n", obj->timer );
   fprintf( fp, "Cost         %d\n", obj->cost );
   fprintf( fp, "Values       %d %d %d %d\n", obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
   fprintf( fp, "Speed        %4.2f\n", obj->speed);

   switch ( obj->item_type )
   {
      case ITEM_POTION:
      case ITEM_SCROLL:
         if( obj->value[1] > 0 )
         {
            fprintf( fp, "Spell 1      '%s'\n", skill_table[obj->value[1]].name );
         }

         if( obj->value[2] > 0 )
         {
            fprintf( fp, "Spell 2      '%s'\n", skill_table[obj->value[2]].name );
         }

         if( obj->value[3] > 0 )
         {
            fprintf( fp, "Spell 3      '%s'\n", skill_table[obj->value[3]].name );
         }

         break;

      case ITEM_PILL:
      case ITEM_STAFF:
      case ITEM_WAND:
         if( obj->value[3] > 0 )
         {
            fprintf( fp, "Spell 3      '%s'\n", skill_table[obj->value[3]].name );
         }

         break;
   }

   for( paf = obj->first_apply; paf != NULL; paf = paf->next )
   {
      fprintf( fp, "Affect       %d %d %d %d %d\n", paf->type, paf->duration, paf->modifier, paf->location, paf->bitvector );
   }

   for( ed = obj->first_exdesc; ed != NULL; ed = ed->next )
   {
      fprintf( fp, "ExtraDescr   %s~ %s~\n", ed->keyword, ed->description );
   }

   fprintf( fp, "End\n\n" );

   if( obj->first_in_carry_list != NULL )
      fwrite_corpse( obj->first_in_carry_list, fp, iNest + 1 );

   return;
}




void save_corpses(  )
{

   FILE *fp;
   char corpse_file_name[MAX_STRING_LENGTH];
   CORPSE_DATA *this_corpse;

   xprintf( corpse_file_name, "%s", CORPSE_FILE );

   if( ( fp = file_open( corpse_file_name, "w" ) ) == NULL )
   {
      bug( "Save Corpses: file_open", 0 );
      perror( "failed open of corpse_file in save_corpses" );
   }
   else
   {
      for( this_corpse = first_corpse; this_corpse != NULL; this_corpse = this_corpse->next )
      {
         fwrite_corpse( this_corpse->this_corpse, fp, 0 );
      }
      fprintf( fp, "#END\n\n" );

      file_close(fp);
   }
   return;

}

void save_marks(  )
{

   FILE *fp;
   char mark_file_name[MAX_STRING_LENGTH];
   MARK_LIST_MEMBER *mark_list;

   xprintf( mark_file_name, "%s", MARKS_FILE );

   if( ( fp = file_open( mark_file_name, "w" ) ) == NULL )
   {
      bug( "Save Mark list: file_open", 0 );
      perror( "failed open of roommarks.lst in save_marks" );
   }
   else
   {
      for( mark_list = first_mark_list; mark_list != NULL; mark_list = mark_list->next )
      {
         fprintf( fp, "#MARK~\n" );
         fprintf( fp, "%d\n", mark_list->mark->room_vnum );
         fprintf( fp, "%s~\n", mark_list->mark->message );
         fprintf( fp, "%s~\n", mark_list->mark->author );
         fprintf( fp, "%d\n", mark_list->mark->duration );
         fprintf( fp, "%d\n", mark_list->mark->type );
      }
      fprintf( fp, "#END~\n\n" );
   }


   file_close(fp);
   return;

}

void save_bans(  )
{

   FILE *fp;
   char ban_file_name[MAX_STRING_LENGTH];
   BAN_DATA *pban;

   xprintf( ban_file_name, "%s", BANS_FILE );

   if( ( fp = file_open( ban_file_name, "w" ) ) == NULL )
   {
      bug( "Save ban list: file_open", 0 );
      perror( "failed open of bans.lst in save_ban" );
   }
   else
   {
      for( pban = first_ban; pban != NULL; pban = pban->next )
      {
         fprintf( fp, "#BAN~\n" );
         fprintf( fp, "%d\n", ( pban->newbie ? 1 : 0 ) );
         fprintf( fp, "%s~\n", pban->name );
         fprintf( fp, "%s~\n", pban->banned_by );
      }
      fprintf( fp, "#END~\n\n" );
   }


   file_close(fp);
   return;

}
