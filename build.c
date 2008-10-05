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
#include "tables.h"
#ifndef DEC_MONEY_H
#include "money.h"
#endif


/* This program provides the interpreting of building commands */

/* The tables are now in buildtab.c: (This file was getting a bit big.)
     Mob act flags       :    tab_mob_act         : bitmask
     Player act flags    :    tab_player_act      : bitmask
     Mob affected by     :    tab_affected_by     : bit_vector
     Object item type    :    tab_item_types      : number
     Object extra flags  :    tab_obj_flags       : bit_vector
     Object wear flags   :    tab_wear_flags      : bit_vector
     Object affect types :    tab_obj_aff         : number
     Class types         :    tab_class           : bit_vector
     Wear locations      :    tab_wear_loc        : number
     Room flags          :    tab_room_flags      : bit_vector
     Sector types        :    tab_sector_types    : number
     Door types          :    tab_door_types      : bit_vector
     Door states         :    tab_door_states     : number

*/
#define                 MAX_STRING      2097152

/*
 * Directions.
 */
const char *sDirs[] = { "North", "East ", "South", "West ", "Up   ", "Down " };
const char *cDirs = "NESWUD";
int RevDirs[] = { 2, 3, 0, 1, 5, 4 };

/*
 * Sex.
 * Used in #MOBILES.
 */
#define NEUTRAL                   0
#define MALE                      1
#define FEMALE                    2

/* Interp. vars et al.
 *
 */

/*
 * Command logging types.
 */
#define LOG_NORMAL      0
#define LOG_ALWAYS      1
#define LOG_NEVER       2

/*
 * God Levels
 */
#define L_GOD           MAX_LEVEL
#define L_SUP           L_GOD - 1
#define L_DEI           L_SUP - 1
#define L_ANG           L_DEI - 1
#define L_HER           L_ANG - 1


/*
 * Log-all switch.
 */
extern bool fLogAll;

/* -S- Additions */
DECLARE_DO_FUN( build_set_medit );
DECLARE_DO_FUN( build_set_oedit );
DECLARE_DO_FUN( build_set_redit );
DECLARE_DO_FUN( build_set_nedit );
DECLARE_DO_FUN( build_setvnum );
DECLARE_DO_FUN( build_list );
DECLARE_DO_FUN( build_set );
DECLARE_DO_FUN( build_listvalues );
DECLARE_DO_FUN( build_listweapons );
DECLARE_DO_FUN( build_listliquids );
DECLARE_DO_FUN( build_listspells );
DECLARE_DO_FUN( build_urooms );
DECLARE_DO_FUN( build_uobjs );
DECLARE_DO_FUN( build_umobs );
DECLARE_DO_FUN( build_findhelp );
DECLARE_DO_FUN( build_commands );
DECLARE_DO_FUN( build_clone );


/* build_functions */
DECLARE_DO_FUN( build_showmob );
DECLARE_DO_FUN( build_showroom );
DECLARE_DO_FUN( build_showobj );
DECLARE_DO_FUN( build_findmob );
DECLARE_DO_FUN( build_findmobroom );
DECLARE_DO_FUN( build_findroom );
DECLARE_DO_FUN( build_findobject );
DECLARE_DO_FUN( build_help );
DECLARE_DO_FUN( build_helpedit );
DECLARE_DO_FUN( build_setmob );
DECLARE_DO_FUN( build_setroom );
DECLARE_DO_FUN( build_setobject );
DECLARE_DO_FUN( build_stop );
DECLARE_DO_FUN( build_dig );
DECLARE_DO_FUN( build_addobject );
DECLARE_DO_FUN( build_addmob );
DECLARE_DO_FUN( build_delwarn );
DECLARE_DO_FUN( build_delroom );
DECLARE_DO_FUN( build_delmob );
DECLARE_DO_FUN( build_delobject );
DECLARE_DO_FUN( build_delhelp );
DECLARE_DO_FUN( build_showresets );
DECLARE_DO_FUN( build_addreset );
DECLARE_DO_FUN( build_delreset );
DECLARE_DO_FUN( build_forcereset );
DECLARE_DO_FUN( build_addhelp );

/* Functions in buildare.c: */
DECLARE_DO_FUN( build_showarea );
DECLARE_DO_FUN( build_findarea );
DECLARE_DO_FUN( build_addarea );
DECLARE_DO_FUN( build_setarea );
DECLARE_DO_FUN( build_makearea );
DECLARE_DO_FUN( build_sysdata );

/* Commands */
const struct cmd_type build_cmd_table[] = {
   /*
    * Common movement commands.
    */

   {"north", do_north, POS_STANDING, 0, LOG_NORMAL},
   {"east", do_east, POS_STANDING, 0, LOG_NORMAL},
   {"south", do_south, POS_STANDING, 0, LOG_NORMAL},
   {"west", do_west, POS_STANDING, 0, LOG_NORMAL},
   {"up", do_up, POS_STANDING, 0, LOG_NORMAL},
   {"down", do_down, POS_STANDING, 0, LOG_NORMAL},
   {"goto", do_goto, POS_STANDING, 0, LOG_NORMAL},
   /*
    * Building commands.
    */

   {"look", do_look, POS_STANDING, 0, LOG_NORMAL},
   {"list", build_list, POS_STANDING, 0, LOG_NORMAL},
   {"x", build_list, POS_STANDING, 0, LOG_NORMAL},
   {"commands", build_commands, POS_STANDING, 0, LOG_NORMAL},
   {"showarea", build_showarea, POS_STANDING, 0, LOG_NORMAL},
   {"showmob", build_showmob, POS_STANDING, 0, LOG_NORMAL},
   {"showroom", build_showroom, POS_STANDING, 0, LOG_NORMAL},
   {"showobject", build_showobj, POS_STANDING, 0, LOG_NORMAL},
   {"showresets", build_showresets, POS_STANDING, 0, LOG_NORMAL},
   {"findarea", build_findarea, POS_STANDING, 0, LOG_NORMAL},
   {"findmob", build_findmob, POS_STANDING, 0, LOG_NORMAL},
   {"findmobroom", build_findmobroom, POS_STANDING, 0, LOG_NORMAL},
   {"findroom", build_findroom, POS_STANDING, 0, LOG_NORMAL},
   {"findobject", build_findobject, POS_STANDING, 0, LOG_NORMAL},
   {"help", build_help, POS_STANDING, 0, LOG_NORMAL},
   {"helpedit", build_helpedit, POS_STANDING, 0, LOG_NORMAL},
   {"set", build_set, POS_STANDING, 0, LOG_NORMAL},
   {"setarea", build_setarea, POS_STANDING, MAX_LEVEL - 1, LOG_NORMAL},
   {"stop", build_stop, POS_STANDING, 0, LOG_ALWAYS},
   {"dig", build_dig, POS_STANDING, 0, LOG_NORMAL},
   {"makearea", build_makearea, POS_STANDING, L_SUP, LOG_NORMAL},
   {"addobject", build_addobject, POS_STANDING, 0, LOG_NORMAL},
   {"addmob", build_addmob, POS_STANDING, 0, LOG_NORMAL},
   {"addreset", build_addreset, POS_STANDING, 0, LOG_NORMAL},
   {"delroo", build_delwarn, POS_STANDING, 0, LOG_NORMAL},
   {"delobjec", build_delwarn, POS_STANDING, 0, LOG_NORMAL},
   {"delmobil", build_delwarn, POS_STANDING, 0, LOG_NORMAL},
   {"delroom", build_delroom, POS_STANDING, 0, LOG_NORMAL},
   {"delobject", build_delobject, POS_STANDING, 0, LOG_NORMAL},
   {"delreset", build_delreset, POS_STANDING, 0, LOG_NORMAL},
   {"delmobile", build_delmob, POS_STANDING, 0, LOG_NORMAL},
   {"delhel", build_delwarn, POS_STANDING, MAX_LEVEL, LOG_NORMAL},
   {"delhelp", build_delhelp, POS_STANDING, MAX_LEVEL, LOG_NORMAL},
   {"forcereset", build_forcereset, POS_STANDING, 0, LOG_NORMAL},
   {"findhelp", build_findhelp, POS_STANDING, 0, LOG_NORMAL},
   {"addhelp", build_addhelp, POS_STANDING, 0, LOG_NORMAL},
   {"redit", build_set_redit, POS_STANDING, 0, LOG_NORMAL},
   {"oedit", build_set_oedit, POS_STANDING, 0, LOG_NORMAL},
   {"medit", build_set_medit, POS_STANDING, 0, LOG_NORMAL},
   {"nedit", build_set_nedit, POS_STANDING, 0, LOG_NORMAL},
   {"setvnum", build_setvnum, POS_STANDING, 0, LOG_NORMAL},
   {"vset", build_setvnum, POS_STANDING, 0, LOG_NORMAL},
   {"uobjs", build_uobjs, POS_STANDING, 0, LOG_NORMAL},
   {"urooms", build_urooms, POS_STANDING, 0, LOG_NORMAL},
   {"umobs", build_umobs, POS_STANDING, 0, LOG_NORMAL},
   {"values", build_listvalues, POS_STANDING, 0, LOG_NORMAL},
   {"liquids", build_listliquids, POS_STANDING, 0, LOG_NORMAL},
   {"weapons", build_listweapons, POS_STANDING, 0, LOG_NORMAL},
   {"spells", build_listspells, POS_STANDING, 0, LOG_NORMAL},
   {"check_area", do_check_area, POS_STANDING, MAX_LEVEL, LOG_NORMAL},
   {"check_areas", do_check_areas, POS_STANDING, MAX_LEVEL, LOG_ALWAYS},
   {"clone", build_clone, POS_STANDING, 0, LOG_NORMAL},
   {"sysdata", build_sysdata, POS_STANDING, MAX_LEVEL, LOG_ALWAYS},
   /*
    * End of list.
    */
   {"", 0, POS_DEAD, 0, LOG_NORMAL}

};



/* Building memory management
 * Using the linked list approach for various sized bits.
 * Store array of sizes, pointing to linked list.
 */

#define MAX_MEM_SIZES 20

int build_freesize[MAX_MEM_SIZES];
void *build_freepointer[MAX_MEM_SIZES];
int build_numsizes = 0;

/* String function */
/* moved build_strdup to merc.h - Stephen */
char *build_simpstrdup( char * );
void build_editstr( char **dest, char *src, CHAR_DATA * ch );
void build_finishedstr( char *orig, char **dest, CHAR_DATA * ch, bool saved );


/* Variables declared in db.c, which we need */

extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
extern OBJ_INDEX_DATA *obj_index_hash[MAX_KEY_HASH];
extern ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];
extern char *string_hash[MAX_KEY_HASH];

extern char *string_space;
extern char *top_string;
extern char str_empty[1];

extern int top_affect;
extern int top_area;
extern int top_ed;
extern int top_exit;
extern int top_mob_index;
extern int top_obj_index;
extern int top_reset;
extern int top_room;
extern int top_shop;

extern AREA_DATA *area_last;
extern AREA_DATA *area_first;
extern SHOP_DATA *first_shop;
extern SHOP_DATA *last_shop;
#define                 MAX_PERM_BLOCK  131072
extern int nAllocString;
extern int sAllocString;
extern int nAllocPerm;
extern int sAllocPerm;
/* extern int                     fBootDb;   */


/* local functions */
char *build_docount( int * );
char *reset_to_text( BUILD_DATA_LIST **, int * );



ROOM_INDEX_DATA *new_room( AREA_DATA * pArea, sh_int vnum, sh_int sector )
{
   ROOM_INDEX_DATA *pRoomIndex;
   sh_int door, cnt;
   MONEY_TYPE *room_treasure;
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
   pRoomIndex->sector_type = sector;
   pRoomIndex->light = 0;
   for( door = 0; door < MAX_DIR; door++ )
      pRoomIndex->exit[door] = NULL;
   pRoomIndex->first_room_reset = NULL;
   pRoomIndex->last_room_reset = NULL;
   GET_FREE( room_treasure, money_type_free );
#ifdef DEBUG_MONEY
   {
      char testbuf[MSL];
      xprintf( testbuf, "loading rooms, vnum %d", pRoomIndex->vnum );
      room_treasure->money_key = str_dup( testbuf );
   }
#endif
   for( cnt = 0; cnt < MAX_CURRENCY; cnt++ )
      room_treasure->cash_unit[cnt] = 0;
   pRoomIndex->treasure = room_treasure;


   return pRoomIndex;
}
void build_interpret( CHAR_DATA * ch, char *argument )
{
   char command[MAX_INPUT_LENGTH];
   char logline[MAX_INPUT_LENGTH];
   char buffer[MAX_INPUT_LENGTH];
   int cmd;
/*    int trust; Unused var*/
   bool found;

   /*
    * Strip leading spaces.
    */
   while( isspace( *argument ) )
      argument++;
   if( argument[0] == '\0' )
      return;

   /*
    * Grab the command word.
    * Special parsing so ' can be a command,
    *   also no spaces needed after punctuation.
    */
   strcpy( logline, argument );
   if( !isalpha( argument[0] ) && !isdigit( argument[0] ) )
   {
      command[0] = argument[0];
      command[1] = '\0';
      argument++;
      while( isspace( *argument ) )
         argument++;
   }
   else
   {
      argument = one_argument( argument, command );
   }


   /*
    * Look for command in command table.
    */
   found = FALSE;
   for( cmd = 0; build_cmd_table[cmd].name[0] != '\0'; cmd++ )
   {
      if( command[0] == build_cmd_table[cmd].name[0]
          && !str_prefix( command, build_cmd_table[cmd].name ) && get_trust( ch ) >= build_cmd_table[cmd].level )
      {
         found = TRUE;
         break;
      }
   }

   /*
    * Log and snoop.
    */
   if( build_cmd_table[cmd].log == LOG_NEVER )
      strcpy( logline, "XXXXXXXX XXXXXXXX XXXXXXXX" );

   if( ( !IS_NPC( ch ) && is_set( ch->act, ACT_LOG ) ) || fLogAll || build_cmd_table[cmd].log == LOG_ALWAYS )
   {
      xprintf_2( log_buf, "Log %s: %s", ch->name, logline );
      log_string( log_buf );
      monitor_chan( log_buf, MONITOR_BUILD );
   }

   if( ch->desc != NULL && ch->desc->snoop_by != NULL )
   {
      write_to_buffer( ch->desc->snoop_by, "% ", 2 );
      write_to_buffer( ch->desc->snoop_by, logline, 0 );
      write_to_buffer( ch->desc->snoop_by, "\n\r", 2 );
   }

   if( !found )
   {
      xprintf( buffer, "%s %s", command, argument );
      build_set( ch, buffer );
      return;
   }


   /*
    * Dispatch the command.
    */
   ( *build_cmd_table[cmd].do_fun ) ( ch, argument );

   /*
    * make sure that if in Redit, vnum is at new room. 
    */
   if( ch->act_build == ACT_BUILD_REDIT && ch->build_vnum != ch->in_room->vnum )
   {
      ch->build_vnum = ch->in_room->vnum;
      send_to_char( "Redit: Vnum changed to new room.\n\r", ch );
   }

   tail_chain(  );
   return;
}

/* -S- Addition: */
void build_commands( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char out[MAX_STRING_LENGTH];
   int cmd;
   int col = 0;

   xprintf( out, "Building / Editing Commands Available to You:\n\r" );

   for( cmd = 0; build_cmd_table[cmd].name[0] != '\0'; cmd++ )
   {
      if( build_cmd_table[cmd].level > get_trust( ch ) )
         continue;
      xprintf( buf, "%-20s    ", build_cmd_table[cmd].name );
      xcat( out, buf );
      if( ++col % 3 == 0 )
         xcat( out, "\n\r" );
   }
   xcat( out, "\n\r" );
   send_to_char( out, ch );
   return;
}



void build_showmob( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   MOB_INDEX_DATA *pMob;
/*    RESET_DATA     *pReset; Unused var */
   SHOP_DATA *pShop;
   bool is_shopkeeper = 0;
   int iTrade;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );


   if( arg1[0] == '\0' )
   {
      send_to_char( "showmob <vnum>\n\r", ch );
      return;
   }

   if( !is_number( arg1 ) )
   {
      send_to_char( "must be a vnum.\n\r", ch );
      return;
   }

   if( ( pMob = get_mob_index( atoi( arg1 ) ) ) == NULL )
   {
      send_to_char( "Cannot find a mob with that vnum.\n\r", ch );
      return;
   }

   if( !build_canread( pMob->area, ch, 1 ) )
      return;

   buf1[0] = '\0';

   xprintf( buf, "@@WName: @@y%s.    @@WClass: @@y%s.\n\r", pMob->player_name, tab_mob_class[pMob->class].text );
   xcat( buf1, buf );

   xprintf( buf, "@@WVnum: @@y%d.  @@WSex: @@y%s.  @@WRace:@@y %s ",
            pMob->vnum,
            pMob->sex == SEX_MALE ? "male" :
            pMob->sex == SEX_FEMALE ? "female" : "neutral",
            ( ( pMob->race > -1 ) && ( pMob->race < MAX_RACE ) ) ? race_table[pMob->race].race_title : "Illegal Race" );
   xcat( buf1, buf );

   xprintf( buf, " @@WLv: @@y%d.    @@WAlign: @@y%d.\n\r", pMob->level, pMob->alignment );
   xcat( buf1, buf );

   xprintf( buf, "@@WModifiers: AC: @@y%d.  @@WHitroll: @@y%d.  @@WDamroll: @@y%d.\n\r",
            pMob->ac_mod, pMob->hr_mod, pMob->dr_mod );
   xcat( buf1, buf );

   xprintf( buf, "@@WMob Flags:@@y\n\r%s", bm_show_values( tab_mob_act, pMob->act ) );
   xcat( buf1, buf );

   xprintf( buf, "@@WAffected by:@@y\n\r%s", show_values( tab_affected_by, pMob->affected_by, TRUE ) );
   xcat( buf1, buf );

   xprintf( buf, "@@WSkill Flags:@@y %s\n\r", bit_table_lookup( tab_mob_skill, pMob->skills ) );
   xcat( buf1, buf );
   xprintf( buf, "@@WPSkill Flags:@@y %s\n\r", bit_table_lookup( tab_mob_powerskill, pMob->power_skills ) );
   xcat( buf1, buf );
   xprintf( buf, "@@WCast Flags:@@y %s\n\r", bit_table_lookup( tab_mob_cast, pMob->cast ) );
   xcat( buf1, buf );
   xprintf( buf, "@@WPCast Flags:@@y %s\n\r", bit_table_lookup( tab_mob_powercast, pMob->power_cast ) );
   xcat( buf1, buf );
   xprintf( buf, "@@WDef Flags:@@y %s\n\r", bit_table_lookup( tab_mob_def, pMob->def ) );
   xcat( buf1, buf );
   xprintf( buf, "@@WStrong Magic Realms:@@y %s\n\r", bit_table_lookup( tab_magic_realms, pMob->strong_magic ) );
   xcat( buf1, buf );
   xprintf( buf, "@@WWeak Magic Realms:@@y %s\n\r", bit_table_lookup( tab_magic_realms, pMob->weak_magic ) );
   xcat( buf1, buf );
   xprintf( buf, "@@WResist Magic Realms:@@y %s\n\r", bit_table_lookup( tab_magic_realms, pMob->resist ) );
   xcat( buf1, buf );
   xprintf( buf, "@@WSuscept Magic Realms:@@y %s\n\r", bit_table_lookup( tab_magic_realms, pMob->suscept ) );
   xcat( buf1, buf );
   xprintf( buf, "@@WRace Mods:@@y %s\n\r", bit_table_lookup( tab_mob_race_mods, pMob->race_mods ) );
   xcat( buf1, buf );

   xprintf( buf, "@@WShort description: @@y%s.\n\r@@WLong  description: @@y%s",
            pMob->short_descr, pMob->long_descr[0] != '\0' ? pMob->long_descr : "(none).\n\r" );
   xcat( buf1, buf );

   if( pMob->spec_fun != 0 )
   {
      xprintf( buf, "@@WMobile has spec fun: @@y%s\n\r", rev_spec_lookup( pMob->spec_fun ) );
      xcat( buf1, buf );
   }

   if( ( pShop = pMob->pShop ) != 0 )
   {
      is_shopkeeper = 1;
      xcat( buf1, "@@WMobile is a shopkeeper, will buy @@y" );
      for( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
      {
         if( pShop->buy_type[iTrade] > 0 )
         {
            xcat( buf1, tab_item_types[pShop->buy_type[iTrade] - 1].text );
            xcat( buf1, " " );
         }
      }
      xcat( buf1, "\n\r" );
      xprintf( buf, "@@WOpens at @@y%i @@Whrs, shuts at @@y%i @@Whours, profbuy:@@y%i, @@Wprofsell:@@y%i.\n\r",
               pShop->open_hour, pShop->close_hour, pShop->profit_buy, pShop->profit_sell );
      xcat( buf1, buf );

   }
   xcat( buf1, "@@g" );
   send_to_char( buf1, ch );
   return;
}

void build_showobj( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   AFFECT_DATA *paf;
   OBJ_INDEX_DATA *obj;
   int cnt;
   char *foo;
   int fubar;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Show what object?\n\r", ch );
      return;
   }

   if( !is_number( arg ) )
   {
      send_to_char( "must be a vnum.\n\r", ch );
      return;
   }

   buf1[0] = '\0';

   if( ( obj = get_obj_index( atoi( arg ) ) ) == NULL )
   {
      send_to_char( "Object vnum not found.\n\r", ch );
      return;
   }

   if( !build_canread( obj->area, ch, 1 ) )
      return;

   xprintf( buf, "@@WName: @@y%s  @@WLevel: @@y%d.\n\r", obj->name, obj->level );
   xcat( buf1, buf );

   xprintf( buf, "@@WVnum: @@y%d.  @@WType: @@y%s.\n\r", obj->vnum, tab_item_types[( obj->item_type ) - 1].text );
   xcat( buf1, buf );

   xprintf( buf, "@@WShort description: @@y%s.\n\r@@WLong description: @@y%s\n\r", obj->short_descr, obj->description );
   xcat( buf1, buf );

   /*
    * xprintf( buf, "@@WItem type: @@y%s.\n\r",rev_table_lookup(tab_item_types,obj->item_type));
    * xcat( buf1, buf);
    */

   xprintf( buf, "@@WWear bits: @@y%s\n\r@@WExtra bits: @@y%s.\n\r",
            bit_table_lookup( tab_wear_flags, obj->wear_flags ), extra_bit_name( obj->extra_flags ) );
   /*
    * bit_table_lookup(tab_extra_flags, obj->extra_flags ) );
    * I think another bit_table_lookup is better!  Zen 
    */

   xcat( buf1, buf );

   xprintf( buf, "@@WItem_applies: @@y%s.\n\r", bit_table_lookup( tab_item_apply, obj->item_apply ) );
   xcat( buf1, buf );

   xprintf( buf, "@@WWeight: @@y%d.\n\r", obj->weight );
   xcat( buf1, buf );

   xprintf( buf, "@@WSpeed: @@y%4.2f\n\r", obj->speed );
   xcat( buf1, buf );

   xprintf( buf, "@@WDurability: @@y%d.\n\r", obj->max_durability );
   xcat( buf1, buf );

   xcat( buf1, "@@WObject Values:\n\r" );

   for( cnt = 0; cnt < 10; cnt++ )
   {
      xprintf( buf, "@@W[Value%d : @@y%6d@@W] %s",
               cnt, obj->value[cnt], rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) );
      xcat( buf1, buf );
      if( is_name( "Spell", rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) ) )
      {
         fubar = obj->value[cnt];
         if( fubar < 0 || fubar > MAX_SKILL )
            xprintf( buf, "               @@R(INVALID!)@@g\n\r" );
         else
            xprintf( buf, "               @@y(%s)@@g\n\r", skill_table[fubar].name );

      }
      else if( is_name( "Liquid", rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) ) )
      {
         foo = str_dup( rev_table_lookup( tab_drink_types, obj->value[cnt] ) );
         if( foo[0] == '\0' )
            xprintf( buf, "                  @@R(INVALID!)@@g\n\r" );
         else
            xprintf( buf, "                  @@y(%s)@@g\n\r", foo );
      }
      else if( is_name( "Weapon", rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) ) )
      {
         foo = rev_table_lookup( tab_weapon_types, obj->value[cnt] );
         if( foo[0] == '\0' )
            xprintf( buf, "                  @@R(INVALID!)@@g\n\r" );
         else
            xprintf( buf, "                  @@y(%s)@@g\n\r", foo );
      }
      else
         xprintf( buf, "@@g\n\r" );
      xcat( buf1, buf );
   }
   if( obj->obj_fun != NULL )
   {
      xprintf( buf, "@@WObject has objfun: @@y%s.@@g\n\r", rev_obj_fun_lookup( obj->obj_fun ) );
      xcat( buf1, buf );
   }

   if( obj->first_exdesc != NULL )
   {
      EXTRA_DESCR_DATA *ed;

      xcat( buf1, "@@WExtra description keywords: '@@y" );

      for( ed = obj->first_exdesc; ed != NULL; ed = ed->next )
      {
         xcat( buf1, ed->keyword );
         if( ed->next != NULL )
            xcat( buf1, " " );
      }

      xcat( buf1, "'@@g.\n\r" );
   }

   for( paf = obj->first_apply; paf != NULL; paf = paf->next )
   {
      xprintf( buf, "@@WAffects @@y%s @@Wby @@y%d@@g.\n\r", affect_loc_name( paf->location ), paf->modifier );
      xcat( buf1, buf );
   }

   send_to_char( buf1, ch );
   return;
}

#define DISPLAY_BRIEFDOORS  1
#define DISPLAY_RESETS      2
#define DISPLAY_FULLDOORS   4
#define DISPLAY_DESC        8

void build_showroom( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg1[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *location;
   BUILD_DATA_LIST *Pointer;
   int door;
   int display;

   argument = one_argument( argument, arg1 );

   location = ch->in_room;

   if( !build_canread( location->area, ch, 1 ) )
      return;

   display = 0;
   if( arg1[0] == '\0' )
   {
      display = DISPLAY_BRIEFDOORS | DISPLAY_RESETS | DISPLAY_DESC;
      xcat( buf1, "Can also use showroom brief/doors/resets/desc/all.\n\r" );
   }
   else if( !is_name( arg1, "brief doors resets desc all" ) )
   {
      send_to_char( "Syntax: Showroom brief/doors/resets/desc/all.\n\r", ch );
      return;
   }
   if( !str_cmp( arg1, "brief" ) )
      display = DISPLAY_BRIEFDOORS;

   if( !str_cmp( arg1, "doors" ) )
      display = DISPLAY_FULLDOORS;

   if( !str_cmp( arg1, "resets" ) )
      display = DISPLAY_RESETS;

   if( !str_cmp( arg1, "desc" ) )
      display = DISPLAY_DESC;

   if( !str_cmp( arg1, "all" ) )
      display = DISPLAY_FULLDOORS | DISPLAY_RESETS | DISPLAY_DESC;

   buf1[0] = '\0';

   xprintf( buf, "@@WName: @@y%s.\n\r@@WArea: @@y%s.\n\r", location->name, location->area->name );
   xcat( buf1, buf );

   xprintf( buf,
            "@@WVnum: @@y%d.  @@WSector:\n\r @@y%s",
            location->vnum, show_values( tab_sector_types, location->sector_type, FALSE ) );
   xcat( buf1, buf );

   xprintf( buf, "@@WFlags:\n\r@@y%s", show_values( tab_room_flags, location->room_flags, TRUE ) );
   xcat( buf1, buf );

   if( ( display & DISPLAY_DESC ) )
   {
      xprintf( buf, "@@WDescr: \n\r@@y%s", location->description );
      xcat( buf1, buf );
   }

   if( location->first_exdesc != NULL )
   {
      EXTRA_DESCR_DATA *ed;

      xcat( buf1, "@@WExtra description keywords:@@y '" );
      for( ed = location->first_exdesc; ed; ed = ed->next )
      {
         xcat( buf1, ed->keyword );
         if( ed->next != NULL )
            xcat( buf1, " " );
      }
      xcat( buf1, "'.\n\r" );
   }

   /*
    * Exits
    */
   if( ( display & ( DISPLAY_BRIEFDOORS | DISPLAY_FULLDOORS ) ) )
      for( door = 0; door < MAX_DIR; door++ )
      {
         EXIT_DATA *pexit;
         OBJ_INDEX_DATA *pKeyObj;

         if( ( pexit = location->exit[door] ) != NULL )
         {
            if( ( display & DISPLAY_FULLDOORS ) )
            {
               if( pexit->key > 0 )
                  pKeyObj = get_obj_index( pexit->key );
               else
                  pKeyObj = NULL;
               xprintf( buf, "@@WExit: @@y%7s @@W: To @@y%5i %s\n\r", sDirs[door],
                        pexit->to_room != NULL ? pexit->to_room->vnum : 0,
                        pexit->to_room != NULL ? pexit->to_room->name : "" );
               xcat( buf1, buf );
               xprintf( buf, "             @@WKey: @@y%5i %s,  @@WExit Type:@@y %s\n\r",
                        pKeyObj != NULL ? pKeyObj->vnum : 0,
                        pKeyObj != NULL ? pKeyObj->name : "None", bit_table_lookup( tab_door_types, pexit->exit_info ) );
               xcat( buf1, buf );
               if( pexit->keyword != NULL && pexit->keyword[0] != '\0' )
               {
                  xprintf( buf, "@@WKeyword(s): @@y%s.  ", pexit->keyword );
                  xcat( buf1, buf );
               }
               xprintf( buf, "@@WDesc: @@y%s", pexit->description[0] != '\0' ? pexit->description : "(none).\n\r" );
               xcat( buf1, buf );
            }
            else
            {
               if( pexit->key > 0 )
                  xcat( buf1, "@@yK" );
               /*
                * New way of showing doors... move to relevant place if it works 
                */

               xprintf( buf, "%s: (%s)  @@WTo: @@y(%i) %s, %s\n\r", sDirs[door],
                        pexit->keyword,
                        pexit->to_room != NULL ? pexit->to_room->vnum : 0,
                        pexit->to_room != NULL ? pexit->to_room->name : "Unknown",
                        bit_table_lookup( tab_door_types, pexit->exit_info ) );
               xcat( buf1, buf );
            }
         }
      }


   /*
    * Show resets for room 
    */
   if( ( display & DISPLAY_RESETS ) && location->first_room_reset != NULL )
   {
      xcat( buf1, "@@WResets:\n\r" );
      for( Pointer = location->first_room_reset; Pointer; )
         xcat( buf1, reset_to_text( &Pointer, NULL ) );
   }
   xcat( buf1, "@@g" );
   send_to_char( buf1, ch );
   return;
}

void build_showresets( CHAR_DATA * ch, char *argument )
{
   char buf1[MAX_STRING_LENGTH];
   ROOM_INDEX_DATA *location;
   BUILD_DATA_LIST *Pointer;
   int count;

   count = 0;
   buf1[0] = '\0';
   location = ch->in_room;

   if( !build_canread( location->area, ch, 1 ) )
      return;

   xcat( buf1, "Room Resets:\n\r" );
   for( Pointer = location->first_room_reset; Pointer != NULL; )
      xcat( buf1, reset_to_text( &Pointer, &count ) );

   send_to_char( buf1, ch );
}

/* spec: rewrote to catch errors and count how many resets it handled */

char *reset_to_text( BUILD_DATA_LIST ** pList, int *pcount )
{
   char buf[MAX_STRING_LENGTH];
   static char buf1[MAX_STRING_LENGTH];
   MOB_INDEX_DATA *pMob;
   OBJ_INDEX_DATA *pObj;
   RESET_DATA *pReset;

   buf[0] = '\0';
   buf1[0] = '\0';
   pReset = ( *pList )->data;

   xcat( buf1, build_docount( pcount ) );
   if( ( *pList )->is_free )  /* sanity check */
      xcat( buf1, " **LIST IS FREE** " );  /* Bad Thing! */
   if( pReset->is_free )
      xcat( buf1, " **RESET IS FREE** " );

   ( *pList ) = ( *pList )->next;

   switch ( pReset->command )
   {
      default:
         xprintf( buf, "  stray '%c' reset: %d %d %d.\n\r", pReset->command, pReset->arg1, pReset->arg2, pReset->arg3 );
         xcat( buf1, buf );
         break;
      case 'G':
         pObj = get_obj_index( pReset->arg1 );
         if( pObj )
            xprintf( buf, "  stray 'give' reset: object [%d] %s.\n\r", pReset->arg1, pObj->name );
         else
            xprintf( buf, "  stray 'give' reset: object %d (unknown).\n\r", pReset->arg1 );
         xcat( buf1, buf );
         break;
      case 'E':
         pObj = get_obj_index( pReset->arg1 );
         if( pObj )
            xprintf( buf, "  stray 'equip' reset: object [%d] %s, on %s.\n\r",
                     pReset->arg1, pObj->name, tab_wear_loc[( pReset->arg3 )].text );
         else
            xprintf( buf, "  stray 'equip' reset: object [%d] (unknown), on %s.\n\r",
                     pReset->arg1, tab_wear_loc[( pReset->arg3 )].text );
         xcat( buf1, buf );
         break;
      case 'A':  /* AutoMessage for room */
         xprintf( buf1, "Message: (%d-%d) %s\n\r", pReset->arg2, pReset->arg3, pReset->notes );
         xcat( buf1, buf );
         break;
      case 'M':  /* Load mob */
         pMob = get_mob_index( pReset->arg1 );
         if( pMob != NULL )
            xprintf( buf, " [%d] %s (limit of %d).\n\r", pMob->vnum, pMob->player_name, pReset->arg2 );
         else
            xprintf( buf, " [%d] (unknown) (limit of %d).\n\r", pReset->arg1, pReset->arg2 );
         xcat( buf1, buf );

         /*
          * scan for give and equip commands for this mob 
          */

         while( *pList )
         {
            pReset = ( *pList )->data;

            if( pReset->command != 'G' && pReset->command != 'E' )
               break;

            ( *pList ) = ( *pList )->next;
            xcat( buf1, build_docount( pcount ) );

            if( pReset->command == 'G' )
            {
               pObj = get_obj_index( pReset->arg1 );
               if( pObj != NULL )
               {
                  if( pMob->pShop != NULL )
                     xcat( buf1, "  sells " );
                  else
                     xcat( buf1, "  with " );
                  xprintf( buf, "[%d] %s.\n\r", pObj->vnum, pObj->name );
                  xcat( buf1, buf );
               }
               else
               {
                  xprintf( buf, "[%d] unknown object in give reset!\n\r", pReset->arg1 );
                  xcat( buf1, buf );
               }
            }
            else if( pReset->command == 'E' )
            {
               pObj = get_obj_index( pReset->arg1 );
               if( pObj != NULL )
                  xprintf( buf, "  equiped with [%d] %s, on %s.\n\r", pObj->vnum,
                           pObj->name, tab_wear_loc[( pReset->arg3 )].text );
               else
                  xprintf( buf, "[%d] unknown object equipped on %s.\n\r", pReset->arg1, tab_wear_loc[pReset->arg3].text );
               xcat( buf1, buf );
            }
         }
         break;
      case 'O':  /* Load object to room */
         pObj = get_obj_index( pReset->arg1 );
         if( pObj != NULL )
            xprintf( buf, " [%d] %s no more than %d in room.\n\r", pObj->vnum, pObj->name, pReset->arg2 );
         else
            xprintf( buf, " [%d] unknown object reset!\n\r", pReset->arg1 );
         xcat( buf1, buf );
         break;
      case 'D':  /* close/lock doors */
         buf[0] = '\0';
         switch ( pReset->arg3 )
         {
            default:
               xprintf( buf, " Illegal door state %d for door %d\n\r", pReset->arg1, pReset->arg2 );
            case 0:
               xprintf( buf, " Open door %s.\n\r", sDirs[pReset->arg2] );
               break;
            case 1:
               xprintf( buf, " Close door %s.\n\r", sDirs[pReset->arg2] );
               break;
            case 2:
               xprintf( buf, " Close and lock door %s.\n\r", sDirs[pReset->arg2] );
               break;
         }
         xcat( buf1, buf );
         break;
      case 'R':  /* randomise exits */
         xprintf( buf, " Randomize doors up to number %d.\n\r", pReset->arg2 );
         xcat( buf1, buf );
         break;
   }

   return buf1;
}

char *build_docount( int *pcount )
{
   static char buf[20];

   buf[0] = '\0';
   if( pcount != NULL )
      xprintf( buf, "%d) ", ++*pcount );
   else
      buf[0] = '\0';
   return buf;
}


void build_findmob( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   MOB_INDEX_DATA *pMobIndex;
   BUILD_DATA_LIST *Pointer;
   AREA_DATA *Area;
   int nMatch;
   bool fAll;
   bool found;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "find which mob?\n\r", ch );
      return;
   }

   if( !( Area = ch->in_room->area ) )
   {
      send_to_char( "Don't know what area you're in.\n\r", ch );
      return;
   }

   if( !build_canread( Area, ch, 1 ) )
      return;

   if( !( Pointer = Area->first_area_mobile ) )
   {
      send_to_char( "No mobiles in this area.\n\r", ch );
      return;
   }

   buf1[0] = '\0';
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   for( ; Pointer != NULL; Pointer = Pointer->next )
   {
      nMatch++;
      pMobIndex = Pointer->data;
      if( fAll || is_name( arg, pMobIndex->player_name ) )
      {
         found = TRUE;
         xprintf( buf, "[%5d] %s\n\r", pMobIndex->vnum, capitalize( pMobIndex->short_descr ) );
         xcat( buf1, buf );
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in this area.\n\r", ch );
      return;
   }

   send_to_char( buf1, ch );
   return;
}

void build_findmobroom( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *pRoom;
   RESET_DATA *pReset;
   AREA_DATA *Area;
   int vnum, found;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "find which mob vnum?\n\r", ch );
      return;
   }

   if( !is_number( arg ) )
   {
      send_to_char( "syntax: findmobroom vnum.\n\r", ch );
      return;
   }

   vnum = atoi( arg );
   found = 0;
   buf1[0] = '\0';

   if( !( Area = ch->in_room->area ) )
   {
      send_to_char( "Don't know what area you're in.\n\r", ch );
      return;
   }

   if( !build_canread( Area, ch, 1 ) )
      return;

   for( pReset = Area->first_reset; pReset != NULL; pReset = pReset->next )
   {
      if( pReset->command == 'M' && pReset->arg1 == vnum )
      {
         found = TRUE;
         pRoom = get_room_index( pReset->arg3 );
         if( pRoom != NULL )
            xprintf( buf, "[%5d] %s\n\r", pRoom->vnum, capitalize( pRoom->name ) );
         else
            xprintf( buf, "[%5d] Unknown\n\r", pReset->arg3 );
         xcat( buf1, buf );
      }
   }

   if( !found )
   {
      send_to_char( "Mob not found in any room.\n\r", ch );
      return;
   }

   send_to_char( buf1, ch );
   return;
}

void build_findobject( CHAR_DATA * ch, char *argument )
{
/*    extern int top_obj_index; Unused Var */
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   OBJ_INDEX_DATA *pObjIndex;
   BUILD_DATA_LIST *pList;
   AREA_DATA *Area;
   int nMatch;
   bool fAll;
   bool found;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Find what object?\n\r", ch );
      return;
   }

   if( !( Area = ch->in_room->area ) )
   {
      send_to_char( "Don't know what area you're in.\n\r", ch );
      return;
   }

   if( !build_canread( Area, ch, 1 ) )
      return;


   if( !( pList = Area->first_area_object ) )
   {
      send_to_char( "No objects in this area.\n\r", ch );
      return;
   }

   buf1[0] = '\0';
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   for( ; pList != NULL; pList = pList->next )
   {
      pObjIndex = pList->data;
      nMatch++;
      if( fAll || is_name( arg, pObjIndex->name ) )
      {
         found = TRUE;
         xprintf( buf, "[%5d] %s\n\r", pObjIndex->vnum, capitalize( pObjIndex->short_descr ) );
         xcat( buf1, buf );
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in the area.\n\r", ch );
      return;
   }

   send_to_char( buf1, ch );
   return;
}

void build_findroom( CHAR_DATA * ch, char *argument )
{
/*    extern int top_room_index; Unused Var */
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *pRoomIndex;
   BUILD_DATA_LIST *pList;
   AREA_DATA *Area;
   int nMatch;
   bool fAll;
   bool found;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Find what room?\n\r", ch );
      return;
   }

   if( !( Area = ch->in_room->area ) )
   {
      send_to_char( "Don't know what area you're in.\n\r", ch );
      return;
   }

   if( !build_canread( Area, ch, 1 ) )
      return;


   if( !( pList = Area->first_area_room ) )
   {
      send_to_char( "No rooms in this area.\n\r", ch );
      return;
   }

   buf1[0] = '\0';
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   for( ; pList != NULL; pList = pList->next )
   {
      pRoomIndex = pList->data;
      nMatch++;
      if( fAll || is_name( arg, pRoomIndex->name ) )
      {
         found = TRUE;
         xprintf( buf, "[%5d] %s\n\r", pRoomIndex->vnum, capitalize( pRoomIndex->name ) );
         xcat( buf1, buf );
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in the area.\n\r", ch );
      return;
   }

   send_to_char( buf1, ch );
   return;
}

/* -S- : More 'intelligent' way to handle things perhaps? */
void build_setmob( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   char arg4[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
/*    char buf2 [MAX_STRING_LENGTH]; unused */
/*    char buffer[MAX_STRING_LENGTH]; unused */
/*    char name[MAX_INPUT_LENGTH]; unused */
   MOB_INDEX_DATA *pMob;
/*    char *argn, *oldperm; unused */
   int value, num;
   int lvalue; /* lookup value */
   AREA_DATA *pArea;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );   /* vnum */
   argument = one_argument( argument, arg2 );
   strcpy( arg3, argument );

   /** Changes to old setmob: *********************************
   Check through the tables, to see if the
   builder has entered the name of an entry.  If so, toggle
   entry( new way of showing bits set is now used )
   Otherwise, check for other values being set, or huh?  :P
   ************************************************************/

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: [set] <field>  <value>\n\r", ch );
      send_to_char( "or:     [set] <string> <value>\n\r", ch );
      send_to_char( "or:     [set] edit <string>\n\r", ch );
      send_to_char( "or:     [set] shop <shopspec> <value>\n\r", ch );
      send_to_char( "\n\r", ch );
      send_to_char( "Field being one of:\n\r", ch );
      send_to_char( "  sex level align  aff act\n\r", ch );
      send_to_char( "  class clan position race\n\r", ch );
      send_to_char( "  skill cast def pskill pcast rmod\n\r", ch );
      send_to_char( "  hr_mod dr_mod ac_mod\n\r", ch );
      send_to_char( "String being one of:\n\r", ch );
      send_to_char( "  name short long desc spec\n\r", ch );
      send_to_char( "Use [set] spec - to clear spec_fun\n\r", ch );
      send_to_char( "Shopspec being one of:\n\r", ch );
      send_to_char( "  trade0 - trade4 profbuy profsell openhour\n\r", ch );
      send_to_char( "  closehour clear\n\r", ch );
      return;
   }

   if( !is_number( arg1 ) )
   {
      send_to_char( "Must be a vnum for first arg.\n\r", ch );
      return;
   }

   if( ( pMob = get_mob_index( atoi( arg1 ) ) ) == NULL )
   {
      send_to_char( "Cannot find mob with that vnum.\n\r", ch );
      return;
   }

   pArea = pMob->area;

   if( !build_canwrite( pArea, ch, 1 ) )
      return;


   /*
    * Snarf the value (which need not be numeric).
    */
   value = is_number( arg3 ) ? atoi( arg3 ) : -1;

   /*
    * Check for act flags 
    */
   lvalue = table_lookup( tab_mob_act, arg2 );
   if( lvalue != 0 )
   {
      /*
       * Then we've found a value 
       */
      if( is_set( pMob->act, lvalue ) )
         remove_bit( pMob->act, lvalue );
      else
         set_bit( pMob->act, lvalue );
      send_to_char( "OK.  Act Flag toggled.\n\r", ch );
      area_modified( pArea );
      return;
   }

   /*
    * Check for affected_by flags 
    */
   lvalue = table_lookup( tab_affected_by, arg2 );
   if( lvalue != 0 )
   {
      /*
       * Then we've found a value 
       */
      if( IS_SET( pMob->affected_by, lvalue ) )
         REMOVE_BIT( pMob->affected_by, lvalue );
      else
         SET_BIT( pMob->affected_by, lvalue );
      send_to_char( "OK.  Affected_by Flag toggled.\n\r", ch );
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "skills" ) )
   {
      lvalue = table_lookup( tab_mob_skill, arg3 );
      if( lvalue == 0 )
      {
         xprintf( buf, "You can toggle the following flags:\n\r" );
         table_printout( tab_mob_skill, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      /*
       * Toggle flag 
       */
      if( IS_SET( pMob->skills, lvalue ) )
         REMOVE_BIT( pMob->skills, lvalue );
      else
         SET_BIT( pMob->skills, lvalue );
      send_to_char( "Ok.\n\r", ch );
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "cast" ) )
   {
      lvalue = table_lookup( tab_mob_cast, arg3 );
      if( lvalue == 0 )
      {
         xprintf( buf, "You can toggle the following flags:\n\r" );
         table_printout( tab_mob_cast, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      /*
       * Toggle flag 
       */
      if( IS_SET( pMob->cast, lvalue ) )
         REMOVE_BIT( pMob->cast, lvalue );
      else
         SET_BIT( pMob->cast, lvalue );
      send_to_char( "Ok.\n\r", ch );
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "def" ) )
   {
      lvalue = table_lookup( tab_mob_def, arg3 );
      if( lvalue == 0 )
      {
         xprintf( buf, "You can toggle the following flags:\n\r" );
         table_printout( tab_mob_def, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      /*
       * Toggle flag 
       */
      if( IS_SET( pMob->def, lvalue ) )
         REMOVE_BIT( pMob->def, lvalue );
      else
         SET_BIT( pMob->def, lvalue );
      send_to_char( "Ok.\n\r", ch );
      area_modified( pArea );
      return;
   }
   if( !str_cmp( arg2, "pcast" ) )
   {
      lvalue = table_lookup( tab_mob_powercast, arg3 );
      if( lvalue == 0 )
      {
         xprintf( buf, "You can toggle the following flags:\n\r" );
         table_printout( tab_mob_powercast, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      /*
       * Toggle flag 
       */
      if( IS_SET( pMob->power_cast, lvalue ) )
         REMOVE_BIT( pMob->power_cast, lvalue );
      else
         SET_BIT( pMob->power_cast, lvalue );
      send_to_char( "Ok.\n\r", ch );
      area_modified( pArea );
      return;
   }
   if( !str_cmp( arg2, "pskill" ) )
   {
      lvalue = table_lookup( tab_mob_powerskill, arg3 );
      if( lvalue == 0 )
      {
         xprintf( buf, "You can toggle the following flags:\n\r" );
         table_printout( tab_mob_powerskill, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      /*
       * Toggle flag 
       */
      if( IS_SET( pMob->power_skills, lvalue ) )
         REMOVE_BIT( pMob->power_skills, lvalue );
      else
         SET_BIT( pMob->power_skills, lvalue );
      send_to_char( "Ok.\n\r", ch );
      area_modified( pArea );
      return;
   }
   if( !str_cmp( arg2, "strong" ) )
   {
      lvalue = table_lookup( tab_magic_realms, arg3 );
      if( lvalue == 0 )
      {
         xprintf( buf, "You can toggle the following flags:\n\r" );
         table_printout( tab_magic_realms, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      /*
       * Toggle flag 
       */
      if( IS_SET( pMob->strong_magic, lvalue ) )
         REMOVE_BIT( pMob->strong_magic, lvalue );
      else
         SET_BIT( pMob->strong_magic, lvalue );
      send_to_char( "Ok.\n\r", ch );
      area_modified( pArea );
      return;
   }
   if( !str_cmp( arg2, "rmod" ) )
   {
      lvalue = table_lookup( tab_mob_race_mods, arg3 );
      if( lvalue == 0 )
      {
         xprintf( buf, "You can toggle the following flags:\n\r" );
         table_printout( tab_mob_race_mods, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      /*
       * Toggle flag 
       */
      if( IS_SET( pMob->race_mods, lvalue ) )
         REMOVE_BIT( pMob->race_mods, lvalue );
      else
         SET_BIT( pMob->race_mods, lvalue );
      send_to_char( "Ok.\n\r", ch );
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "weak" ) )
   {
      lvalue = table_lookup( tab_magic_realms, arg3 );
      if( lvalue == 0 )
      {
         xprintf( buf, "You can toggle the following flags:\n\r" );
         table_printout( tab_magic_realms, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      /*
       * Toggle flag 
       */
      if( IS_SET( pMob->weak_magic, lvalue ) )
         REMOVE_BIT( pMob->weak_magic, lvalue );
      else
         SET_BIT( pMob->weak_magic, lvalue );
      send_to_char( "Ok.\n\r", ch );
      area_modified( pArea );
      return;
   }
   if( !str_cmp( arg2, "resist" ) )
   {
      lvalue = table_lookup( tab_magic_realms, arg3 );
      if( lvalue == 0 )
      {
         xprintf( buf, "You can toggle the following flags:\n\r" );
         table_printout( tab_magic_realms, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      /*
       * Toggle flag 
       */
      if( IS_SET( pMob->resist, lvalue ) )
         REMOVE_BIT( pMob->resist, lvalue );
      else
         SET_BIT( pMob->resist, lvalue );
      send_to_char( "Ok.\n\r", ch );
      area_modified( pArea );
      return;
   }
   if( !str_cmp( arg2, "suscept" ) )
   {
      lvalue = table_lookup( tab_magic_realms, arg3 );
      if( lvalue == 0 )
      {
         xprintf( buf, "You can toggle the following flags:\n\r" );
         table_printout( tab_magic_realms, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      /*
       * Toggle flag 
       */
      if( IS_SET( pMob->suscept, lvalue ) )
         REMOVE_BIT( pMob->suscept, lvalue );
      else
         SET_BIT( pMob->suscept, lvalue );
      send_to_char( "Ok.\n\r", ch );
      area_modified( pArea );
      return;
   }
   if( !str_cmp( arg2, "rmod" ) )
   {
      lvalue = table_lookup( tab_mob_race_mods, arg3 );
      if( lvalue == 0 )
      {
         xprintf( buf, "You can toggle the following flags:\n\r" );
         table_printout( tab_mob_race_mods, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      /*
       * Toggle flag 
       */
      if( IS_SET( pMob->race_mods, lvalue ) )
         REMOVE_BIT( pMob->race_mods, lvalue );
      else
         SET_BIT( pMob->race_mods, lvalue );
      send_to_char( "Ok.\n\r", ch );
      area_modified( pArea );
      return;
   }
   if( !str_cmp( arg2, "class" ) )
   {
      lvalue = table_lookup( tab_mob_class, arg3 );
      if( lvalue == -1 )
      {
         xprintf( buf, "You can use the following classes:\n\r" );
         table_printout( tab_mob_class, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      pMob->class = lvalue;
      send_to_char( "Ok.\n\r", ch );
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "race" ) )
   {
      lvalue = is_number( arg3 ) ? atoi( arg3 ) : -1;
      if( ( lvalue < -1 ) || ( lvalue > MAX_RACE ) )
      {
         int looper;
         char catbuf[MSL];
         xprintf( buf, "You can use the following races:\n\r" );
         for( looper = 0; looper < MAX_RACE; looper++ )
         {
            xprintf( catbuf, "[%i]:%s ", looper, race_table[looper].race_title );
            xcat( buf, catbuf );
         }
         xcat( buf, "\n\r" );
         send_to_char( buf, ch );
         return;
      }
      pMob->race = lvalue;
      send_to_char( "Ok.\n\r", ch );
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "position" ) )
   {
      if( !str_prefix( arg3, "standing" ) )
         value = POS_STANDING;
      else if( !str_prefix( arg3, "resting" ) )
         value = POS_RESTING;
      else if( !str_prefix( arg3, "sleeping" ) )
         value = POS_SLEEPING;
      else
      {
         send_to_char( "position must be standing, resting or sleeping.\n\r", ch );
         return;
      }
      pMob->position = value;
      area_modified( pArea );
      send_to_char( "Ok.\n\r", ch );
      return;
   }

   if( !str_cmp( arg2, "sex" ) )
   {
      value = -1;
      if( arg3[0] == 'm' && !str_prefix( arg3, "male" ) )
         value = SEX_MALE;
      else if( arg3[0] == 'f' && !str_prefix( arg3, "female" ) )
         value = SEX_FEMALE;
      else if( arg3[0] == 'n' && !str_prefix( arg3, "neutral" ) )
         value = SEX_NEUTRAL;

      if( value == -1 )
      {
         send_to_char( "Must be male, female or neutral.\n\r", ch );
         return;
      }
      pMob->sex = value;
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "level" ) )
   {
      if( value < 1 || value > 140 )
      {
         send_to_char( "Level range is 1 to 140.\n\r", ch );
         return;
      }
      pMob->level = value;
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "align" ) )
   {
      if( value < -1000 || value > 1000 )
      {
         send_to_char( "Alignment range is -1000 to 1000.\n\r", ch );
         return;
      }
      pMob->alignment = value;
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "ac_mod" ) )
   {
      if( value < -2500 || value > 500 )
      {
         send_to_char( "ac_mod range is -2500 to 500.\n\r", ch );
         return;
      }
      pMob->ac_mod = value;
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "hr_mod" ) )
   {
      if( value < -10 || value > 1000 )
      {
         send_to_char( "hr_mod range is -10 to 1000.\n\r", ch );
         return;
      }
      pMob->hr_mod = value;
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "dr_mod" ) )
   {
      if( value < -10 || value > 1000 )
      {
         send_to_char( "dr_mod range is -10 to 1000.\n\r", ch );
         return;
      }
      pMob->dr_mod = value;
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "edit" ) )
   {
      if( is_name( arg3, "desc name short long" ) )
         xprintf( buf, "%s %s $edit", arg1, arg3 );
      else
         xprintf( buf, " " );
      build_setmob( ch, buf );
      return;
   }


   if( !str_cmp( arg2, "name" ) )
   {
      build_strdup( &pMob->player_name, arg3, TRUE, FALSE, ch );
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "short" ) )
   {
      build_strdup( &pMob->short_descr, arg3, TRUE, FALSE, ch );
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "long" ) )
   {
      build_strdup( &pMob->long_descr, arg3, TRUE, TRUE, ch );
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "desc" ) )
   {
      build_strdup( &pMob->description, arg3, TRUE, FALSE, ch );
      area_modified( pArea );
      return;
   }


   if( !str_cmp( arg2, "spec" ) )
   {
      BUILD_DATA_LIST *pList;

      if( arg3[0] == '-' )
      {
         if( pMob->spec_fun == NULL )
            return;

         pMob->spec_fun = NULL;

         for( pList = pArea->first_area_specfunc; pList != NULL; pList = pList->next )
         {
            if( ( MOB_INDEX_DATA * ) pList->data == pMob )
               break;
         }

         if( pList != NULL )
         {
            UNLINK( pList, pArea->first_area_specfunc, pArea->last_area_specfunc, next, prev );
            PUT_FREE( pList, build_free );
         }

         return;
      }

      if( ( pMob->spec_fun = spec_lookup( arg3 ) ) == 0 )
      {
         xprintf( buf, "Valid spec funs are :\n\r" );
         print_spec_lookup( buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }

      /*
       * Add to area linked list 
       */
      /*
       * First see if there is one for this char already 
       */
      for( pList = pArea->first_area_specfunc; pList != NULL; pList = pList->next )
         if( ( MOB_INDEX_DATA * ) pList->data == pMob )
            break;

      if( pList == NULL )
      {
         GET_FREE( pList, build_free );
         pList->data = pMob;
         LINK( pList, pArea->first_area_specfunc, pArea->last_area_specfunc, next, prev );
      }

      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "shop" ) )
   {
      SHOP_DATA *pShop;
      BUILD_DATA_LIST *pList;
      int iTrade;

      argument = one_argument( argument, arg3 );
      strcpy( arg4, argument );
      pArea = pMob->area;
      pShop = pMob->pShop;


      if( !str_cmp( arg3, "clear" ) )
      {
         if( pShop != NULL )
         {
            UNLINK( pShop, first_shop, last_shop, next, prev );

            top_shop--; /* Reduce shop count */

            for( pList = pArea->first_area_shop; pList != NULL; pList = pList->next )
            {
               if( pShop == pList->data )
                  break;
            }

            if( pList != NULL )
            {
               UNLINK( pList, pArea->first_area_shop, pArea->last_area_shop, next, prev );
               PUT_FREE( pList, build_free );
            }

            pMob->pShop = NULL;  /* Take away link from mob */
            PUT_FREE( pShop, shop_free ); /* Free the memory it occupies */
         }
         area_modified( pArea );
         return;
      }

      if( ( str_prefix( "trade", arg3 ) && !is_name( arg3, "profbuy profsell openhour closehour" ) ) )
      {
         /*
          * Usage message 
          */
         build_setmob( ch, "" );
         return;
      }

      area_modified( pArea );

      if( pShop == NULL )
      {
         GET_FREE( pShop, shop_free );
         if( pShop == NULL )  /* Couldn't alloc space. */
         {
            send_to_char( "Out of memory, please reboot ASAP.\n\r", ch );
            return;
         }
         GET_FREE( pList, build_free );
         if( pList == NULL )
         {
            send_to_char( "Out of memory, please reboot ASAP.\n\r", ch );
            return;
         }

         pShop->keeper = pMob->vnum;
         pMob->pShop = pShop; /* Add link to mob */
         pList->data = pShop; /* Add to area list */
         LINK( pList, pArea->first_area_shop, pArea->last_area_shop, next, prev );

         LINK( pShop, first_shop, last_shop, next, prev );
         top_shop++; /* Increment number of shops */
         for( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
            pShop->buy_type[iTrade] = 0;
         pShop->profit_buy = 100;
         pShop->profit_sell = 100;
         pShop->open_hour = 0;
         pShop->close_hour = 23;
      }


      if( !str_prefix( "trade", arg3 ) )
      {
         num = arg3[5] - '0';
         if( num < 0 || num > MAX_TRADE )
         {
            xprintf( buf, "trade must be between 0 and %i\n\r", MAX_TRADE );
            send_to_char( buf, ch );
            return;
         }
         value = table_lookup( tab_item_types, arg4 );
         if( value == 0 )
         {
            xprintf( buf, "Item type must be one of the following:\n\r" );
            table_printout( tab_item_types, buf + strlen( buf ) );
            send_to_char( buf, ch );
         }
         pShop->buy_type[num] = value;
         return;
      }

      value = is_number( arg4 ) ? atoi( arg4 ) : -1;

      if( !str_cmp( "profbuy", arg3 ) )
      {
         if( value < 0 )
         {
            send_to_char( "Profbuy must be a number more than 0.\n\r", ch );
            return;
         }
         if( value < 100 )
         {
            send_to_char( "WARNING: Profbuy should be over 100.\n\r", ch );
            return;
         }

         pShop->profit_buy = value;
         return;
      }

      if( !str_cmp( "profsell", arg3 ) )
      {
         if( value < 0 )
         {
            send_to_char( "Profsell must be a number more than 0.\n\r", ch );
            return;
         }
         if( value > 100 )
         {
            send_to_char( "WARNING: Profsell should be over 100.\n\r", ch );
            return;
         }
         pShop->profit_sell = value;
         return;
      }

      if( !str_cmp( "openhour", arg3 ) )
      {
         if( value < 0 || value > 24 )
         {
            send_to_char( "Openhour must be a number between 0 and 24.\n\r", ch );
            return;
         }

         pShop->open_hour = value;
         return;
      }

      if( !str_cmp( "closehour", arg3 ) )
      {
         if( value < 0 || value > 24 )
         {
            send_to_char( "closehour must be a number between 0 and 24.\n\r", ch );
            return;
         }

         pShop->close_hour = value;
         return;
      }
   }


   /*
    * Generate usage message.
    */
   build_setmob( ch, "" );
   return;
}

/* spec: nuke any stray resets for a changed exit */

void nuke_exit_resets( ROOM_INDEX_DATA * pRoomIndex, int door )
{
   BUILD_DATA_LIST *pList, *pListNext;
   RESET_DATA *pReset;
   AREA_DATA *pArea = pRoomIndex->area;

   for( pList = pRoomIndex->first_room_reset; pList; pList = pListNext )
   {
      pListNext = pList->next;
      pReset = pList->data;

      if( pReset->command == 'D' && pReset->arg2 == door )
      {
         /*
          * nuke this reset 
          */
         UNLINK( pList, pRoomIndex->first_room_reset, pRoomIndex->last_room_reset, next, prev );
         PUT_FREE( pList, build_free );

         UNLINK( pReset, pArea->first_reset, pArea->last_reset, next, prev );
         PUT_FREE( pReset, reset_free );
      }
   }
}

void build_setroom( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   char arg4[MAX_INPUT_LENGTH];
   char arg5[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   char *argn;
   ROOM_INDEX_DATA *location;
   int value, num;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   strcpy( arg2, argument );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax: [set] <arguments> \n\r", ch );
      send_to_char( "    Or: [set] <string> value\n\r", ch );
      send_to_char( "    Or: [set] edit <string>\n\r", ch );
      send_to_char( "\n\r", ch );
      send_to_char( "Arguments being one of:\n\r", ch );
      send_to_char( "      flags  +/-<ascii-flag> \n\r", ch );
      send_to_char( "      sector <ascii-sector-type>\n\r", ch );
      send_to_char( "      door <door-number> to <vnum> [onesided]\n\r", ch );
      send_to_char( "                         keyword <string>\n\r", ch );
      send_to_char( "                         desc <string>\n\r", ch );
      send_to_char( "                         key <vnum> [onesided]\n\r", ch );
      send_to_char( "                         type <ascii-type> [onesided]\n\r", ch );
      send_to_char( "                         clear [onesided]\n\r", ch );
      send_to_char( "      extra <keyword> <string>\n\r", ch );
      send_to_char( "      extra -<keyword>\n\r", ch );
      send_to_char( "String being one of:\n\r", ch );
      send_to_char( "      name desc\n\r", ch );
      return;
   }

   location = get_room_index( ch->build_vnum );

   if( location == NULL )
   {
      send_to_char( "Invalid vnum selected!\n\r", ch );
      return;
   }

   if( !build_canwrite( location->area, ch, 1 ) )
      return;


   area_modified( location->area );


   /*
    * Check for flags first, then other values... 
    */

   value = table_lookup( tab_room_flags, arg1 );
   if( value != 0 )
   {
      /*
       * Flag found 
       */
      if( IS_SET( location->room_flags, value ) )
         REMOVE_BIT( location->room_flags, value );
      else
         SET_BIT( location->room_flags, value );
      xprintf( buf, "@@WRoom flag @@y%s@@W toggled to @@y%s@@W.@@g\n\r",
               rev_table_lookup( tab_room_flags, value ), IS_SET( location->room_flags, value ) ? "ON" : "OFF" );
      send_to_char( buf, ch );
      return;
   }

   value = table_lookup( tab_sector_types, arg1 );
   if( value >= 1 )
   {
      /*
       * Sector found 
       */
      location->sector_type = value;
      xprintf( buf, "@@WSector type set to @@y%s@@W.@@g\n\r", rev_table_lookup( tab_sector_types, value ) );
      send_to_char( buf, ch );
      return;
   }
   if( !str_cmp( arg1, "edit" ) && is_name( arg2, "name desc" ) )
   {
      xprintf( buf, "%s $edit", arg2 );
      build_setroom( ch, buf );
      return;
   }

   if( !str_cmp( arg1, "name" ) )
   {
      build_strdup( &location->name, arg2, TRUE, FALSE, ch );
      return;
   }

   if( !str_cmp( arg1, "desc" ) )
   {
      build_strdup( &location->description, arg2, TRUE, FALSE, ch );
      return;
   }

   if( !str_cmp( arg1, "extra" ) )
   {

      EXTRA_DESCR_DATA *ed;
      bool found;


      argument = one_argument( argument, arg2 );
      strcpy( arg3, argument );

      argn = arg2;
      if( argn[0] == '-' )
      {
         argn++;
         found = FALSE;

         for( ed = location->first_exdesc; ed != NULL; ed = ed->next )
         {
            if( is_name( argn, ed->keyword ) )
            {
               found = TRUE;
               break;
            }
         }

         if( !found )
         {
            send_to_char( "Keyword not found.\n\r", ch );
            return;
         }

         /*
          * Delete description 
          */
         UNLINK( ed, location->first_exdesc, location->last_exdesc, next, prev );
         PUT_FREE( ed, exdesc_free );
         top_ed--;

         return;
      }

      found = FALSE;
      for( ed = location->first_exdesc; ed != NULL; ed = ed->next )
         if( is_name( arg2, ed->keyword ) )
         {
            found = TRUE;
            break;
         }

      if( !found )
      {
         GET_FREE( ed, exdesc_free );

         build_strdup( &ed->keyword, arg2, FALSE, FALSE, ch );
         build_strdup( &ed->description, arg3, FALSE, FALSE, ch );
         LINK( ed, location->first_exdesc, location->last_exdesc, next, prev );
         top_ed++;
         return;
      }

      build_strdup( &ed->description, arg3, TRUE, FALSE, ch );
      return;

   }

   /*
    * spec: remove resets referring to this door if we clear the 'door'
    * *       state
    */

   if( !str_cmp( arg1, "door" ) )
   {
      EXIT_DATA *pExit;
      int door;
      char *temp;
      ROOM_INDEX_DATA *pDestRoom;
      EXIT_DATA *pDestExit;


      argument = one_argument( argument, arg2 );
      argument = one_argument( argument, arg3 );
      strcpy( arg4, argument );

      if( is_number( arg2 ) )
         door = atoi( arg2 );
      else
      {
         if( ( temp = index( cDirs, UPPER( arg2[0] ) ) ) != NULL )
            door = temp - cDirs;
         else
            door = -1;
      }

      if( door < 0 || door > MAX_DIR )
      {
         send_to_char( "Door number must be a number between 0 and 5, or NSEWUD\n\r", ch );
         return;
      }

      pExit = location->exit[door];

      if( !str_cmp( arg3, "clear" ) )
      {
         ROOM_INDEX_DATA *pDestRoom;

         if( pExit == NULL )
            return;

         pDestRoom = get_room_index( pExit->vnum );
         if( pDestRoom == NULL )
            return;
         if( !build_canwrite( pDestRoom->area, ch, 0 ) )
         {
            send_to_char( "You cannot change this exit.", ch );
            return;
         }

         /*
          * Delete exit 
          */
         PUT_FREE( pExit, exit_free );
         location->exit[door] = NULL;
         top_exit--;

         /*
          * spec: nuke any resets for it 
          */
         nuke_exit_resets( location, door );

         /*
          * Delete corresponding exit on other side 
          */
         if( str_cmp( arg4, "onesided" ) && pDestRoom != NULL )
         {
            pExit = pDestRoom->exit[RevDirs[door]];
            if( pExit != NULL )
               if( pExit->vnum == location->vnum )
               {
                  PUT_FREE( pExit, exit_free );
                  pDestRoom->exit[RevDirs[door]] = NULL;
                  top_exit--;

                  /*
                   * spec: nuke any resets for it 
                   */
                  nuke_exit_resets( pDestRoom, RevDirs[door] );

                  area_modified( pDestRoom->area );
               }
               else
                  send_to_char( "Note: Door in exit room does not point to this door, so not deleted.", ch );
            else
               send_to_char( "Note: There is no corresponding exit in the room pointed to by this exit.", ch );
         }
      }

      if( !is_name( arg3, "to key desc keyword type" ) )
      {
         build_setroom( ch, "" );
         return;
      }

      if( pExit == NULL )
      {
         GET_FREE( pExit, exit_free );
         pExit->to_room = NULL;
         pExit->vnum = 0;
         pExit->description = &str_empty[0];
         pExit->keyword = &str_empty[0];
         pExit->exit_info = 0;
         pExit->key = -1;
         top_exit++;
         location->exit[door] = pExit;
      }

      pDestRoom = pExit->to_room;   /* Setup pDestRoom to point to dest. room */
      if( pDestRoom != NULL )
      {
         pDestExit = pDestRoom->exit[RevDirs[door]];  /* Setup pDestExit to exit from DestRoom to here. */
         if( pDestExit != NULL && pDestExit->to_room != location )
            pDestExit = NULL;
      }
      else
         pDestExit = NULL;


      if( !str_cmp( arg3, "to" ) )
      {
         ROOM_INDEX_DATA *pRoom;

         argument = one_argument( argument, arg4 );
         argument = one_argument( argument, arg5 );

         if( !is_number( arg4 ) )
         {
            send_to_char( "Room number must be a vnum\n\r", ch );
            return;
         }
         value = atoi( arg4 );
         if( !( pRoom = get_room_index( value ) ) )
         {
            send_to_char( "No such vnum.\n\r", ch );
            return;
         }

         if( !build_canwrite( pRoom->area, ch, 0 ) )
         {
            send_to_char( "You cannot connect to this room.\n\r", ch );
            return;
         }

         if( pDestRoom != NULL && !build_canwrite( pDestRoom->area, ch, 0 ) )
         {
            send_to_char( "You cannot change the destination of this exit.", ch );
            return;
         }

         if( str_cmp( arg5, "onesided" ) )   /* If NOT onesided, do checks */
         {
            if( pDestRoom != NULL && pDestExit != NULL ) /* If already connected */
            {
               PUT_FREE( pDestExit, exit_free );
               pDestRoom->exit[RevDirs[door]] = NULL;
               top_exit--;

               /*
                * spec: nuke exit resets 
                */

               nuke_exit_resets( pDestRoom, RevDirs[door] );
            }


            if( !( pDestExit = pRoom->exit[RevDirs[door]] ) )  /* If exit hasn't been made on other side */
            {
               /*
                * Create door coming other way. 
                */
               GET_FREE( pDestExit, exit_free );
               pDestExit->to_room = location;
               pDestExit->vnum = location->vnum;
               pDestExit->description = &str_empty[0];
               build_strdup( &pDestExit->keyword, pExit->keyword, FALSE, FALSE, ch );
               pDestExit->exit_info = pExit->exit_info;
               pDestExit->key = pExit->exit_info;
               top_exit++;
               pRoom->exit[RevDirs[door]] = pDestExit;
               area_modified( pRoom->area );
            }

            if( pDestExit->to_room != location )
            {
               xprintf( buf, "The door in room %i going %s points to room [%i] %s.\n\r",
                        value,
                        sDirs[RevDirs[door]], pDestExit->vnum, pDestExit->to_room != NULL ? pDestExit->to_room->name : "" );
               send_to_char( buf, ch );
            }
         }
         pExit->vnum = value;
         pExit->to_room = pRoom;
         return;
      }

      if( !str_cmp( arg3, "keyword" ) )
      {
         build_strdup( &pExit->keyword, arg4, TRUE, FALSE, ch );
         return;
      }

      if( !str_cmp( arg3, "desc" ) )
      {
         build_strdup( &pExit->description, arg4, TRUE, FALSE, ch );
         return;
      }

      if( !str_cmp( arg3, "key" ) )
      {
         OBJ_INDEX_DATA *pObject;

         argument = one_argument( argument, arg4 );
         argument = one_argument( argument, arg5 );

         if( arg4[0] == '\0' )
         {
            pExit->key = -1;
            return;
         }

         if( !is_number( arg4 ) )
         {
            send_to_char( "Object number must be a vnum, or blank.\n\r", ch );
            return;
         }
         if( !( pObject = get_obj_index( atoi( arg4 ) ) ) )
         {
            send_to_char( "No such vnum.\n\r", ch );
            return;
         }

         pExit->key = pObject->vnum;
         if( pDestExit && str_cmp( arg5, "onesided" ) )
            pDestExit->key = pObject->vnum;

         return;
      }

      if( !str_cmp( arg3, "type" ) )
      {

         argument = one_argument( argument, arg4 );
         argument = one_argument( argument, arg5 );

         /*
          * Make SURE that closed/locked flags are NOT set. -S- 
          */
         /*
          * Resets set these bits, nothing else :P 
          */

         argn = arg4;
         num = 1;
         if( argn[0] == '-' )
         {
            num = 0;
            argn++;
         }
         if( argn[0] == '+' )
         {
            num = 1;
            argn++;
         }

         value = table_lookup( tab_door_types, argn );
         if( value == 0 )
         {
            xprintf( buf, "Possible door types are +/-:\n\r" );
            table_printout( tab_door_types, buf + strlen( buf ) );
            send_to_char( buf, ch );
            return;
         }

         if( value == EX_CLOSED || value == EX_LOCKED )
         {
            send_to_char( "To CLOSE or LOCK a door, use a reset.\n\r", ch );
            return;
         }


         if( num == 1 )
         {
            if( pDestExit && str_cmp( arg5, "onesided" ) )
            {
               SET_BIT( pDestExit->exit_info, value );
               area_modified( pDestRoom->area );
            }
            SET_BIT( pExit->exit_info, value );
         }
         else
         {
            if( pDestExit && str_cmp( arg5, "onesided" ) )
            {
               REMOVE_BIT( pDestExit->exit_info, value );
               if( IS_SET( value, EX_ISDOOR ) )
                  nuke_exit_resets( pDestRoom, RevDirs[door] );
               area_modified( pDestRoom->area );
            }
            REMOVE_BIT( pExit->exit_info, value );
            if( IS_SET( value, EX_ISDOOR ) )
               nuke_exit_resets( location, door );
         }
         return;
      }
   }

   /*
    * Generate usage message.
    */
   build_setroom( ch, "" );
   return;
}


void build_setobject( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   char *argn;
   OBJ_INDEX_DATA *pObj;
   AREA_DATA *pArea;
   int value, num;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   strcpy( arg3, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: [set] <field>  <value>\n\r", ch );
      send_to_char( "or:     [set] <string> <value>\n\r", ch );
      send_to_char( "or:     aobj [<slot>]\n\r", ch );
      send_to_char( "\n\r", ch );
      send_to_char( "Field being one of:\n\r", ch );
      send_to_char( "  value0 value1 value2 value3 speed\n\r", ch );
      send_to_char( "  level extra wear weight aff type durability\n\r", ch );
      send_to_char( "\n\r", ch );
      send_to_char( "String being one of:\n\r", ch );
      send_to_char( "  name short long ed objfun\n\r", ch );
      send_to_char( "Use [set] objfun - to clear objfun.\n\r", ch );
      return;
   }

   if( ( pObj = get_obj_index( atoi( arg1 ) ) ) == NULL )
   {
      send_to_char( "Vnum not found.\n\r", ch );
      return;
   }

   pArea = pObj->area;
   if( !build_canwrite( pArea, ch, 1 ) )
      return;

   /*
    * Check for extra flag: clan_eq 
    */
   if( IS_SET( pObj->extra_flags, ITEM_CLAN_EQ ) && get_trust( ch ) != 85 )
   {
      send_to_char( "Only a Creator can set Clan-Eq.\n\r", ch );
      return;
   }




   area_modified( pObj->area );

   /*
    * Snarf the value (which need not be numeric).
    */
   value = atol( arg3 );

   /*
    * Set something.
    */
   if( !str_prefix( "aobj", arg2 ) )
   {
    float mult = 0.0;
    sh_int ac = 0, dr = 0, hp = 0, hr = 0, mp = 0, mv = 0, svs = 0;

    if( pObj->level < 1 ) /* Should never happen anyhow, but sanity checks are never bad */
    {
     send_to_char("You can only auto-object items that are at least level 1.\n\r",ch);
     return;
    }

    mult = (float)pObj->level / 120.0000;
    ac = sysdata.build_obj_ac * mult;
    dr = sysdata.build_obj_dr * mult;
    hp = sysdata.build_obj_hp * mult;
    hr = sysdata.build_obj_hr * mult;
    mp = sysdata.build_obj_mp * mult;
    mv = sysdata.build_obj_mv * mult;
    svs = sysdata.build_obj_svs * mult;

    if( arg3[0] != '\0' ) /* Use modifiers based on wear slot */
    {
     sh_int i = 0;
     bool found = FALSE;

     for( i = 0; tab_auto_obj[i].name != NULL; i++ )
     {
      if( !str_prefix(arg3,tab_auto_obj[i].name) )
      {
       found = TRUE;
       ac *= tab_auto_obj[i].ac;
       dr *= tab_auto_obj[i].dr;
       hp *= tab_auto_obj[i].hp;
       hr *= tab_auto_obj[i].hr;
       mp *= tab_auto_obj[i].mp;
       mv *= tab_auto_obj[i].mv;
       svs *= tab_auto_obj[i].svs;
       break;
      }
     }
     if( found )
     {
      xprintf(buf,"Values modified based on object type: %s.\n\r",tab_auto_obj[i].name);
      send_to_char(buf,ch);
     }
     else
     {
      xprintf(buf,"Invalid object type. Valid object types:");
      for( i = 0; tab_auto_obj[i].name != NULL; i++ )
       xcat(buf," %s",tab_auto_obj[i].name);
      xcat(buf,".\n\r");
      send_to_char(buf,ch);
     }
    }

    xprintf(buf,"%d aff %sac %d",pObj->vnum,ac == 0 ? "-" : "",ac);
    build_setobject(ch,buf);
    xprintf(buf,"%d aff %sdamroll %d",pObj->vnum,dr == 0 ? "-" : "",dr);
    build_setobject(ch,buf);
    xprintf(buf,"%d aff %shit %d",pObj->vnum,hp == 0 ? "-" : "",hp);
    build_setobject(ch,buf);
    xprintf(buf,"%d aff %shitroll %d",pObj->vnum,hr == 0 ? "-" : "",hr);
    build_setobject(ch,buf);
    xprintf(buf,"%d aff %smana %d",pObj->vnum,mp == 0 ? "-" : "",mp);
    build_setobject(ch,buf);
    xprintf(buf,"%d aff %smove %d",pObj->vnum,mv == 0 ? "-" : "",mv);
    build_setobject(ch,buf);
    xprintf(buf,"%d aff %ssaving_spell %d",pObj->vnum,svs == 0 ? "-" : "",svs);
    build_setobject(ch,buf);

    xprintf(buf,"Auto-object complete based on item level %d. Stats are %0.2f%% of max.\n\r",pObj->level,(mult * 100));
    send_to_char(buf,ch);

    return;
   }

   if( !str_prefix( "value", arg2 ) )
   {
      num = arg2[5] - '0';
      if( num < 0 || num > 9 )
      {
         send_to_char( "Value0-9.\n\r", ch );
         return;
      }
      pObj->value[num] = value;
      return;
   }
   if( !str_cmp( arg2, "level" ) )
   {
      if( value < 1 || value > 120 )
      {
         send_to_char( "item level is 1 to 120.\n\r", ch );
         return;
      }
      pObj->level = value;
      return;
   }

   if( !str_cmp( arg2, "speed" ) )
   {
      if( atof(arg3) < 0.01 || atof(arg3) > 4 )
      {
         send_to_char("item speed is 0.01 to 4.00.\n\r", ch );
         return;
      }
      pObj->speed = atof(arg3);
      return;
   }

   if( UPPER( arg2[0] ) == 'V' && arg2[2] == '\0' )
   {
      num = arg2[1] - '0';
      if( num < 0 || num > 9 )
      {
         send_to_char( "v0-v9.\n\r", ch );
         return;
      }
      pObj->value[num] = value;
      return;
   }

   if( !str_cmp( arg2, "objfun" ) )
   {
      BUILD_DATA_LIST *pList;

      if( arg3[0] == '-' )
      {
         if( pObj->obj_fun == NULL )
            return;

         pObj->obj_fun = NULL;

         for( pList = pArea->first_area_objfunc; pList != NULL; pList = pList->next )
         {
            if( ( OBJ_INDEX_DATA * ) pList->data == pObj )
               break;
         }

         if( pList != NULL )
         {
            UNLINK( pList, pArea->first_area_objfunc, pArea->last_area_objfunc, next, prev );
            PUT_FREE( pList, build_free );
         }

         return;
      }

      if( ( pObj->obj_fun = obj_fun_lookup( arg3 ) ) == 0 )
      {
         xprintf( buf, "Valid obj funs are :\n\r" );
         print_obj_fun_lookup( buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }

      /*
       * Add to area linked list 
       */
      /*
       * First see if there is one for this char already 
       */
      for( pList = pArea->first_area_objfunc; pList != NULL; pList = pList->next )
         if( ( OBJ_INDEX_DATA * ) pList->data == pObj )
            break;

      if( pList == NULL )
      {
         GET_FREE( pList, build_free );
         pList->data = pObj;
         LINK( pList, pArea->first_area_objfunc, pArea->last_area_objfunc, next, prev );
      }

      area_modified( pArea );
      return;
   }



   if( !str_cmp( arg2, "extra" ) )
   {
      num = 1;
      argn = arg3;
      if( argn[0] == '+' )
      {
         num = 1;
         argn++;
      }
      if( argn[0] == '-' )
      {
         num = 0;
         argn++;
      }
      value = table_lookup( tab_obj_flags, argn );
      if( value == 0 )
      {
         xprintf( buf, "Values for extra flags are +/- :\n\r" );
         table_printout( tab_obj_flags, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      if( num == 1 )
         SET_BIT( pObj->extra_flags, value );
      else
         REMOVE_BIT( pObj->extra_flags, value );
      return;
   }



   if( !str_cmp( arg2, "magic" ) )
   {
      num = 1;
      argn = arg3;
      if( argn[0] == '+' )
         argn++;
      if( argn[0] == '-' )
      {
         num = 0;
         argn++;
      }
      value = table_lookup( tab_item_apply, argn );

      if( value == 0 )
      {
         xprintf( buf, "Values for magical applies are +/- :\n\r" );
         table_printout( tab_item_apply, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      if( num == 1 )
         SET_BIT( pObj->item_apply, value );
      else
         REMOVE_BIT( pObj->item_apply, value );

      return;
   }

   if( !str_cmp( arg2, "wear" ) )
   {
      num = 1;
      argn = arg3;
      if( argn[0] == '+' )
      {
         num = 1;
         argn++;
      }
      if( argn[0] == '-' )
      {
         num = 0;
         argn++;
      }
      value = table_lookup( tab_wear_flags, argn );

      if( value == 0 )
      {
         xprintf( buf, "Values for wear flags are +/- :\n\r" );
         table_printout( tab_wear_flags, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      if( num == 1 )
         SET_BIT( pObj->wear_flags, value );
      else
         REMOVE_BIT( pObj->wear_flags, value );

      return;

   }

   if( !str_cmp( arg2, "type" ) )
   {
      value = table_lookup( tab_item_types, arg3 );
      if( value == 0 )
      {
         xprintf( buf, "Values for item types are :\n\r" );
         table_printout( tab_item_types, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }

      if( value == ITEM_CLAN_EQ && get_trust( ch ) != 85 )
      {
         send_to_char( "Only a CREATOR may set this flag.\n\r", ch );
         return;
      }


      pObj->item_type = value;
      return;
   }


   if( !str_cmp( arg2, "weight" ) )
   {
      pObj->weight = value;
      return;
   }


   if( !str_cmp( arg2, "name" ) )
   {
      build_strdup( &pObj->name, arg3, TRUE, FALSE, ch );
      return;
   }

   if( !str_cmp( arg2, "short" ) )
   {
      build_strdup( &pObj->short_descr, arg3, TRUE, FALSE, ch );
      return;
   }

   if( !str_cmp( arg2, "long" ) )
   {
      build_strdup( &pObj->description, arg3, TRUE, TRUE, ch );
      return;
   }

   if( !str_cmp( arg2, "ed" ) )
   {
      EXTRA_DESCR_DATA *ed;
      int found;

      argument = one_argument( argument, arg3 );
      if( argument == NULL )
      {
         send_to_char( "Syntax: setobject <object> ed [-]<keyword> <string>\n\r", ch );
         return;
      }

      if( arg3[0] == '-' )
      {
         /*
          * Find and delete keyword 
          */
         argn = arg3 + 1;
         found = FALSE;

         for( ed = pObj->first_exdesc; ed != NULL; ed = ed->next )
         {
            if( is_name( argn, ed->keyword ) )
            {
               found = TRUE;
               break;
            }
         }

         if( !found )
         {
            send_to_char( "Keyword not found.\n\r", ch );
            return;
         }

         UNLINK( ed, pObj->first_exdesc, pObj->last_exdesc, next, prev );
         PUT_FREE( ed, exdesc_free );

         return;
      }
      found = FALSE;
      for( ed = pObj->first_exdesc; ed != NULL; ed = ed->next )
      {
         if( is_name( arg3, ed->keyword ) )
         {
            found = TRUE;
            break;
         }
      }
      if( found )
      {
         build_strdup( &ed->description, argument, TRUE, FALSE, ch );
         return;
      }


      GET_FREE( ed, exdesc_free );
      build_strdup( &ed->keyword, arg3, FALSE, FALSE, ch );
      build_strdup( &ed->description, argument, FALSE, FALSE, ch );
      LINK( ed, pObj->first_exdesc, pObj->last_exdesc, next, prev );
      return;
   }




   if( !str_cmp( arg2, "aff" ) )
   {
      AFFECT_DATA *paf;
      int found, location, neg;

      argument = one_argument( argument, arg3 );
      if( argument == NULL )
      {
         send_to_char( "Syntax: setobject <object> aff [-]<location> <mod>\n\r", ch );
         return;
      }

      argn = arg3;
      neg = 0;
      if( argn[0] == '-' )
      {
         neg = 1;
         argn++;
      }

      location = table_lookup( tab_obj_aff, argn );
      if( location == 0 )
      {
         xprintf( buf, "Location can be one of the following [-]:\n\r" );
         table_printout( tab_obj_aff, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }

      found = FALSE;

      for( paf = pObj->first_apply; paf != NULL; paf = paf->next )
      {
         if( paf->location == location )
         {
            found = TRUE;
            break;
         }
      }

      if( neg == 1 )
      {
         if( !found )
         {  /* Commenting this to squelch auto-object. It's not that important to generate a message. --Kline
            send_to_char( "Location not found.\n\r", ch ); */
            return;
         }

         UNLINK( paf, pObj->first_apply, pObj->last_apply, next, prev );
         PUT_FREE( paf, affect_free ); /* Put on free list */
         return;
      }


      if( !found )
      {
         GET_FREE( paf, affect_free );
         paf->location = location;
         paf->modifier = atoi( argument );
         LINK( paf, pObj->first_apply, pObj->last_apply, next, prev );
      }
      else
         paf->modifier = atoi( argument );
      return;
   }

   if( !str_prefix(arg2,"durability") )
   {
    if( !is_number(arg3) )
    {
     send_to_char("Durability must be a number.\n\r",ch);
     return;
    }
    if( atoi(arg3) < 2 )
    {
     send_to_char("Value must be at least 2. An object with only 1 durability is considered 'broken' and un-usable.\n\r",ch);
     return;
    }
    pObj->max_durability = atoi(arg3);
    pObj->durability = pObj->max_durability;
   }

   /*
    * Generate usage message.
    */
   build_setobject( ch, "" );
   return;
}



void build_dig( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   char buffer[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *pRoomIndex;
   ROOM_INDEX_DATA *pCurRoom;
   int vnum, dir;
   char *temp;
   int iHash;
   EXIT_DATA *pExit;
   BUILD_DATA_LIST *pList;
   AREA_DATA *pArea;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   strcpy( arg3, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: dig <dir> <vnum> [onesided]\n\r", ch );
      return;
   }

   if( !( temp = index( cDirs, UPPER( arg1[0] ) ) ) )
   {
      send_to_char( "Direction must be one of NSEWUP\n\r", ch );
      return;
   }
   dir = temp - cDirs;

   if( ch->in_room->exit[dir] != NULL )
   {
      send_to_char( "There is already an exit in that direction\n.", ch );
      return;
   }

   vnum = is_number( arg2 ) ? atoi( arg2 ) : -1;

   if( vnum < 0 || vnum > MAX_VNUM )
   {
      send_to_char( "Vnum must be between 0 and 16777216.\n\r", ch );
      return;
   }

   if( get_room_index( vnum ) != NULL )
   {
      send_to_char( "There is already a room with that vnum.\n\r", ch );
      return;
   }

   pCurRoom = ch->in_room;
   pArea = pCurRoom->area;

   if( !build_canwrite( pArea, ch, 1 ) )
      return;

   if( vnum < pArea->min_vnum || vnum > pArea->max_vnum )
   {
      xprintf( buffer, "Vnum must be between %i and %i.\n\r", pArea->min_vnum, pArea->max_vnum );
      send_to_char( buffer, ch );
      return;
   }

   /*
    * Create room 
    */
   pRoomIndex = new_room( pArea, vnum, ch->in_room->sector_type );

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
   LINK( pList, pCurRoom->area->first_area_room, pCurRoom->area->last_area_room, next, prev );
   top_room++;

   /*
    * Create door 
    */
   GET_FREE( pExit, exit_free );
   pExit->to_room = pRoomIndex;
   pExit->vnum = vnum;
   pExit->description = &str_empty[0];
   pExit->keyword = &str_empty[0];
   pExit->exit_info = 0;
   pExit->key = -1;
   top_exit++;
   pCurRoom->exit[dir] = pExit;

   if( str_cmp( arg3, "onesided" ) )   /* If NOT onesided */
   {
      GET_FREE( pExit, exit_free );
      pExit->to_room = pCurRoom;
      pExit->vnum = pCurRoom->vnum;
      pExit->description = &str_empty[0];
      pExit->keyword = &str_empty[0];
      pExit->exit_info = 0;
      pExit->key = -1;
      top_exit++;
      pRoomIndex->exit[RevDirs[dir]] = pExit;
   }

   area_modified(pArea);
   return;
}


void build_stop( CHAR_DATA * ch, char *argument )
{
   ch->position = POS_STANDING;
   send_to_char( "Building stopped.\n\r", ch );
}

void do_build( CHAR_DATA * ch, char *argument )
{
   if( !is_set( ch->act, ACT_BUILDER ) )
   {
      send_to_char( "You aren't allowed to build!\n\r", ch );
      return;
   }

   ch->position = POS_BUILDING;
   do_help( ch, "build_bmotd" ); /* motd for builders -S- */
   send_to_char( "Building commands are now operative. Type stop to stop building.\n\r", ch );
}

void build_addmob( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buffer[MAX_INPUT_LENGTH];
   MOB_INDEX_DATA *pMobIndex;
/*    ROOM_INDEX_DATA *room; unused */
   AREA_DATA *pArea;
   BUILD_DATA_LIST *pList;
   int vnum;
   int iHash;
   static BITMASK bitmask_zero;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   strcpy( arg2, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: addmob <vnum> <name>\n\r", ch );
      return;
   }

   vnum = is_number( arg1 ) ? atoi( arg1 ) : -1;

   if( vnum < 0 || vnum > MAX_VNUM )
   {
      send_to_char( "Vnum must be between 0 and 16777216.\n\r", ch );
      return;
   }

   if( get_mob_index( vnum ) != NULL )
   {
      send_to_char( "There is already a mob with that vnum.\n\r", ch );
      return;
   }

   pArea = ch->in_room->area; /* bugger.  this might need fixing!! */

   if( !build_canwrite( pArea, ch, 1 ) )
      return;

   if( vnum < pArea->min_vnum || vnum > pArea->max_vnum )
   {
      xprintf( buffer, "Vnum must be between %i and %i.\n\r", pArea->min_vnum, pArea->max_vnum );
      send_to_char( buffer, ch );
      return;
   }

   ch->build_vnum = vnum;
   ch->act_build = ACT_BUILD_MEDIT;


   GET_FREE( pMobIndex, mid_free );
   GET_FREE( pMobIndex->act, bitmask_free );
   *pMobIndex->act = bitmask_zero;
   pMobIndex->vnum = vnum;
   pMobIndex->area = pArea;
   pMobIndex->player_name = str_dup( arg2 );
   pMobIndex->short_descr = &str_empty[0];
   pMobIndex->long_descr = &str_empty[0];
   pMobIndex->description = &str_empty[0];

   pMobIndex->affected_by = 0;
   pMobIndex->pShop = NULL;
   pMobIndex->alignment = 0;
   pMobIndex->level = 1;

   pMobIndex->sex = SEX_MALE;

   pMobIndex->ac_mod = 0;
   pMobIndex->hr_mod = 0;
   pMobIndex->dr_mod = 0;

   pMobIndex->spec_fun = NULL;
   pMobIndex->pShop = NULL;
   pMobIndex->count = 0;
   pMobIndex->killed = 0;
   pMobIndex->target = NULL;
   pMobIndex->first_mprog = NULL;
   pMobIndex->last_mprog = NULL;
   pMobIndex->progtypes = 0;


   iHash = vnum % MAX_KEY_HASH;
   SING_TOPLINK( pMobIndex, mob_index_hash[iHash], next );
   GET_FREE( pList, build_free );
   pList->data = pMobIndex;
   LINK( pList, pArea->first_area_mobile, pArea->last_area_mobile, next, prev );

   top_mob_index++;
   kill_table[URANGE( 0, pMobIndex->level, MAX_LEVEL - 1 )].number++;
   return;
}


void build_addobject( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buffer[MAX_INPUT_LENGTH];
   OBJ_INDEX_DATA *pObjIndex;
   AREA_DATA *pArea;
   BUILD_DATA_LIST *pList;
   int vnum;
   int iHash;
   int looper;
   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   strcpy( arg2, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: addobject <vnum> <name>\n\r", ch );
      return;
   }

   vnum = is_number( arg1 ) ? atoi( arg1 ) : -1;

   if( vnum < 0 || vnum > MAX_VNUM )
   {
      send_to_char( "Vnum must be between 0 and 16777216.\n\r", ch );
      return;
   }

   if( get_obj_index( vnum ) != NULL )
   {
      send_to_char( "There is already an object with that vnum.\n\r", ch );
      return;
   }


   pArea = ch->in_room->area;

   if( !build_canwrite( pArea, ch, 1 ) )
      return;

   if( vnum < pArea->min_vnum || vnum > pArea->max_vnum )
   {
      xprintf( buffer, "Vnum must be between %i and %i.\n\r", pArea->min_vnum, pArea->max_vnum );
      send_to_char( buffer, ch );
      return;
   }

   ch->build_vnum = vnum;
   ch->act_build = ACT_BUILD_OEDIT;


   GET_FREE( pObjIndex, oid_free );
   pObjIndex->vnum = vnum;
   pObjIndex->area = pArea;
   pObjIndex->name = str_dup( arg2 );
   pObjIndex->short_descr = &str_empty[0];
   pObjIndex->description = &str_empty[0];
   pObjIndex->owner = &str_empty[0];
   pObjIndex->level = 1;
   pObjIndex->item_type = 1;
   pObjIndex->extra_flags = 0;
   pObjIndex->wear_flags = 0;
   pObjIndex->item_apply = 1;
   for( looper = 0; looper < 10; looper++ )
      pObjIndex->value[looper] = 0;
   pObjIndex->weight = 1;
   pObjIndex->cost = 0;

   pObjIndex->first_exdesc = NULL;
   pObjIndex->last_exdesc = NULL;
   pObjIndex->first_apply = NULL;
   pObjIndex->last_apply = NULL;


   iHash = vnum % MAX_KEY_HASH;
   SING_TOPLINK( pObjIndex, obj_index_hash[iHash], next );

   GET_FREE( pList, build_free );
   pList->data = pObjIndex;
   LINK( pList, pArea->first_area_object, pArea->last_area_object, next, prev );

   top_obj_index++;

   return;
}

void build_addreset( CHAR_DATA * ch, char *argument )
{
   RESET_DATA *pReset;
   RESET_DATA *pMobReset;
   BUILD_DATA_LIST *pList;
   BUILD_DATA_LIST *pMobList;
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   char arg4[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];
   char command;
   int rarg1, rarg2, rarg3;
   char *rauto = "";
   ROOM_INDEX_DATA *pRoomIndex;
   int vnum, num;
   int found;
   char *temp;
   AREA_DATA *pArea;
   MOB_INDEX_DATA *pMob;
   OBJ_INDEX_DATA *pObj;
   EXIT_DATA *pExit;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   argument = one_argument( argument, arg3 );
   argument = one_argument( argument, arg4 );

   if( arg1[0] == '\0' || ( arg2[0] == '\0' && strcmp( arg1, "message" ) ) )
   {
      send_to_char( "addreset mob   <vnum> <limit>\n\r", ch );
      send_to_char( "         obj   <vnum> <max number in room>\n\r", ch );
      send_to_char( "         equip <n.mob-vnum> <obj-vnum> <location>\n\r", ch );
      send_to_char( "         give  <n.mob-vnum> <obj-vnum>\n\r", ch );
      send_to_char( "         door  <dir> <state>\n\r", ch );
      send_to_char( "         rand  <num-dirs>\n\r", ch );
      send_to_char( "         message <min_lev> <max_lev> <text>\n\r", ch );
      return;
   }

   if( !is_name( arg1, "mob obj equip message give door rand" ) )
   {
      build_addreset( ch, "" );
      return;
   }

   command = '*';
   rarg1 = 0;
   rarg2 = 0;
   rarg3 = 0;

   pRoomIndex = ch->in_room;
   pArea = pRoomIndex->area;
   pMobReset = NULL;
   pMobList = NULL;

   if( !build_canwrite( pArea, ch, 1 ) )
      return;

   if( !str_cmp( arg1, "mob" ) )
   {
      vnum = is_number( arg2 ) ? atoi( arg2 ) : -1;
      if( vnum < 0 || vnum > MAX_VNUM )
      {
         xprintf( buf, "Vnum must be between 0 and %i.\n\r", MAX_VNUM );
         send_to_char( buf, ch );
         return;
      }
      if( !( pMob = get_mob_index( vnum ) ) )
      {
         send_to_char( "Mob not found.\n\r", ch );
         return;
      }

      if( !build_canread( pMob->area, ch, 0 ) )
      {
         send_to_char( "You cannot use that mob.\n\r", ch );
         return;
      }

      num = is_number( arg3 ) ? atoi( arg3 ) : -1;
      if( num < 0 )
      {
         send_to_char( "Limit must be a number more than 0.\n\r", ch );
         return;
      }

      command = 'M';
      rarg1 = vnum;
      rarg2 = num;
      rarg3 = pRoomIndex->vnum;
   }

   if( !str_cmp( arg1, "message" ) )
   {
      if( !is_number( arg2 ) || !is_number( arg3 ) )
      {
         send_to_char( "Must specify TWO levels: min and max before message.\n\r", ch );
         return;
      }


      rarg1 = pRoomIndex->vnum;
      rarg2 = atoi( arg2 );
      rarg3 = atoi( arg3 );
      rauto = str_dup( arg4 );
      command = 'A';
   }

   if( !str_cmp( arg1, "obj" ) )
   {
      vnum = is_number( arg2 ) ? atoi( arg2 ) : -1;
      if( vnum < 0 || vnum > MAX_VNUM )
      {
         xprintf( buf, "Vnum must be between 0 and %i.\n\r", MAX_VNUM );
         send_to_char( buf, ch );
         return;
      }
      if( !( pObj = get_obj_index( vnum ) ) )
      {
         send_to_char( "Object not found.\n\r", ch );
         return;
      }

      if( !build_canread( pObj->area, ch, 0 ) )
      {
         send_to_char( "You cannot use that object.\n\r", ch );
         return;
      }


      if( IS_SET( pObj->extra_flags, ITEM_CLAN_EQ ) && !IS_IMMORTAL( ch ) )
      {
         send_to_char( "You can't use that object for a reset.\n\r", ch );
         return;
      }


      command = 'O';
      rarg1 = vnum;
      rarg2 = ( is_number( arg3 ) ? atoi( arg3 ) : 1 );
      rarg3 = pRoomIndex->vnum;
   }

   if( !str_cmp( arg1, "equip" ) )
   {
      num = number_argument( arg2, buf );
      if( num == 0 )
      {
         send_to_char( "Count portion of vnum cannot be 0. Indicate which instance of the mob, or -1 for all instances.\n\r",
                       ch );
         return;
      }
      vnum = is_number( buf ) ? atoi( buf ) : -1;

      found = num;
      for( pMobList = pRoomIndex->first_room_reset; pMobList != NULL; pMobList = pMobList->next )
      {
         pMobReset = pMobList->data;
         if( pMobReset->command == 'M' && pMobReset->arg1 == vnum )
         {
            found--;
            if( found <= 0 )
               break;
         }
      }

      if( found == num )
      {
         send_to_char( "None of that mob loaded in the room resets.\n\r", ch );
         return;
      }
      if( num > 0 && found > 0 )
      {
         send_to_char( "Not enough of that mob loaded in the room resets.\n\r", ch );
         return;
      }

      vnum = is_number( arg3 ) ? atoi( arg3 ) : -1;
      if( !( pObj = get_obj_index( vnum ) ) )
      {
         send_to_char( "Object not found.\n\r", ch );
         return;
      }


      if( IS_SET( pObj->extra_flags, ITEM_CLAN_EQ ) && !IS_IMMORTAL( ch ) )
      {
         send_to_char( "You can't use that object for a reset.\n\r", ch );
         return;
      }


      if( !build_canread( pObj->area, ch, 0 ) )
      {
         send_to_char( "You cannot use that object.\n\r", ch );
         return;
      }

      rarg1 = vnum;

      num = table_lookup( tab_wear_loc, arg4 );
      if( num == -2 )
      {
         xprintf( buf, "Location must be one of the following:\n\r" );
         table_printout( tab_wear_loc, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      rarg2 = 0;
      rarg3 = num;
      command = 'E';
   }


   if( !str_cmp( arg1, "give" ) )
   {
      num = number_argument( arg2, buf );
      if( num == 0 )
      {
         send_to_char( "Count portion of vnum cannot be 0. Indicate which instance of the mob, or -1 for all instances.\n\r",
                       ch );
         return;
      }
      vnum = is_number( buf ) ? atoi( buf ) : -1;

      found = num;
      for( pMobList = pRoomIndex->first_room_reset; pMobList != NULL; pMobList = pMobList->next )
      {
         pMobReset = pMobList->data;
         if( pMobReset->command == 'M' && pMobReset->arg1 == vnum )
         {
            found--;
            if( found <= 0 )
               break;
         }
      }

      if( found == num )
      {
         send_to_char( "None of that mob loaded in the room resets.\n\r", ch );
         return;
      }
      if( num > 0 && found > 0 )
      {
         send_to_char( "Not enough of that mob loaded in the room resets.\n\r", ch );
         return;
      }

      vnum = is_number( arg3 ) ? atoi( arg3 ) : -1;
      if( !( pObj = get_obj_index( vnum ) ) )
      {
         send_to_char( "Object not found.\n\r", ch );
         return;
      }


      if( IS_SET( pObj->extra_flags, ITEM_CLAN_EQ ) && !IS_IMMORTAL( ch ) )
      {
         send_to_char( "You can't use that object for a reset.\n\r", ch );
         return;
      }

      if( !build_canread( pObj->area, ch, 0 ) )
      {
         send_to_char( "You cannot use that object.\n\r", ch );
         return;
      }

      rarg1 = vnum;
      rarg2 = 0;
      rarg3 = 0;
      command = 'G';
   }

   if( !str_cmp( arg1, "door" ) )
   {
      temp = index( cDirs, UPPER( arg2[0] ) );
      if( !temp )
      {
         send_to_char( "Direction must be one of NSEWUP.\n\r", ch );
         return;
      }

      rarg1 = pRoomIndex->vnum;
      rarg2 = temp - cDirs;

      if( ( pExit = pRoomIndex->exit[rarg2] ) == NULL || !IS_SET( pExit->exit_info, EX_ISDOOR ) )
      {
         send_to_char( "That exit isn't a door.\n\r", ch );
         return;
      }

      num = table_lookup( tab_door_states, arg3 );
      if( num < 0 )
      {
         xprintf( buf, "Door state can be one of the following:\n\r" );
         table_printout( tab_door_states, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      rarg3 = num;
      command = 'D';
   }

   if( !str_cmp( arg1, "rand" ) )
   {
      num = is_number( arg2 ) ? atoi( arg2 ) : -1;
      if( num < 1 || num > 6 )
      {
         send_to_char( "Number of doors must be from 1 to 6.\n\r", ch );
         return;
      }
      rarg1 = pRoomIndex->vnum;
      rarg2 = num;
      command = 'R';
   }

   if( command == '*' )
   {
      bug( "build_addreset: Shouldn't have come this far.\n\r", 0 );
      build_addreset( ch, "" );
      return;
   }

   /*
    * Now create new pReset 
    */
   GET_FREE( pReset, reset_free );
   pReset->command = command;
   pReset->arg1 = rarg1;
   pReset->arg2 = rarg2;
   pReset->arg3 = rarg3;
   if( command == 'A' )
      pReset->notes = rauto;

   GET_FREE( pList, build_free );
   pList->data = pReset;

   if( pMobReset != NULL )
   {
      pReset->next = pMobReset->next;
      pMobReset->next = pReset;
      pReset->prev = pMobReset;
      if( pReset->next )
         pReset->next->prev = pReset;
      else
         pRoomIndex->area->last_reset = pReset;
      pList->next = pMobList->next;
      pMobList->next = pList;
      pList->prev = pMobList;
      if( pList->next )
         pList->next->prev = pList;
      else
         pRoomIndex->last_room_reset = pList;
   }
   else
   {
      pArea = pRoomIndex->area;
      LINK( pReset, pArea->first_reset, pArea->last_reset, next, prev );
      LINK( pList, pRoomIndex->first_room_reset, pRoomIndex->last_room_reset, next, prev );
   }
   area_modified( pArea );
   return;
}

/* spec: rewrote the internals of this, it might work somewhat more reliably
 * now :) .. please, someone put the reset lists out of their misery.. */

void build_delreset( CHAR_DATA * ch, char *argument )
{
   RESET_DATA *pReset;
   BUILD_DATA_LIST *pList;
   BUILD_DATA_LIST *pNextList;
   char arg1[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *pRoomIndex;

   int found;
   AREA_DATA *pArea;


   smash_tilde( argument );
   argument = one_argument( argument, arg1 );

   if( arg1[0] == '\0' || !is_number( arg1 ) )
   {
      send_to_char( "delreset <reset-number>  (type showresets for numbers)\n\r", ch );
      return;
   }

   found = atoi( arg1 );

   if( found <= 0 )
   {
      send_to_char( "Reset number must be more than 0.\n\r", ch );
      return;
   }

   pReset = NULL;

   pRoomIndex = ch->in_room;
   pArea = pRoomIndex->area;

   if( !build_canwrite( pArea, ch, 1 ) )
      return;

   for( pList = pRoomIndex->first_room_reset; pList; pList = pList->next )
      if( --found == 0 )
         break;

   if( found > 0 || !pList )
   {
      send_to_char( "Could not find reset.\n\r", ch );
      return;
   }

   pReset = pList->data;

   if( !pReset )
   {
      bugf( "delreset: pList returned null in search for reset number %d", atoi( arg1 ) );
      send_to_char( "bug: Could not do deletion.\n\r", ch );
      return;
   }

   pArea = pRoomIndex->area;

   /*
    * Special for MOB resets, also take out following resets. 
    */
   if( pReset->command == 'M' )
   {
      pNextList = pList->next;

      /*
       * free the mob reset 
       */

      UNLINK( pList, pRoomIndex->first_room_reset, pRoomIndex->last_room_reset, next, prev );
      PUT_FREE( pList, build_free );

      UNLINK( pReset, pArea->first_reset, pArea->last_reset, next, prev );
      PUT_FREE( pReset, reset_free );

      /*
       * free any give/equip resets following it 
       */
      for( pList = pNextList; pList; pList = pNextList )
      {
         pNextList = pList->next;

         if( pReset->command != 'E' && pReset->command != 'G' )
            break;

         UNLINK( pList, pRoomIndex->first_room_reset, pRoomIndex->last_room_reset, next, prev );
         PUT_FREE( pList, build_free );

         UNLINK( pReset, pArea->first_reset, pArea->last_reset, next, prev );
         PUT_FREE( pReset, reset_free );
      }
   }
   else
   {
      /*
       * Just get rid of the one reset 
       */
      pNextList = pList->next;

      UNLINK( pList, pRoomIndex->first_room_reset, pRoomIndex->last_room_reset, next, prev );
      PUT_FREE( pList, build_free );

      UNLINK( pReset, pArea->first_reset, pArea->last_reset, next, prev );
      PUT_FREE( pReset, reset_free );
   }

   send_to_char( "Done.\n\r", ch );
   return;
}


void build_delwarn( CHAR_DATA * ch, char *argument )
{
   send_to_char( "You must spell out delroom, delobject, delmobile, or delhelp in full.\n\r", ch );
   return;
}

void build_delhelp( CHAR_DATA *ch, char *argument )
{
 FILE *fp;
 char arg1[MSL];
 char arg2[MSL];
 bool mort = FALSE;

 argument = strlower(argument);
 argument = one_argument(argument,arg1);
 argument = one_argument(argument,arg2);

 if( arg1[0] == '\0' )
 {
  send_to_char("Syntax: delhelp <mort/imm> <keyword> ok.\n\r",ch);
  return;
 }
 if( !str_prefix(arg1,"mort") )
  mort = TRUE;
 else if( !str_prefix(arg1,"imm") )
  mort = FALSE;
 else
 {
  build_delhelp(ch,"");
  return;
 }

 xprintf(arg1,"%s%s/%s.%s",HELP_DIR,initial(arg2),arg2,mort ? HELP_MORT : HELP_IMM);
 if( (fp = file_open(arg1,"r")) == NULL )
 {
  send_to_char("Couldn't find that keyword.\n\r",ch);
  file_close(fp);
  return;
 }
 else if( (fp = file_open(arg1,"r")) != NULL )
 {
  if( str_cmp(argument,"ok") )
  {
   build_delhelp(ch,"");
   file_close(fp);
   return;
  }
  else
  {
   file_close(fp);
   unlink(arg1);
   send_to_char("Done.\n\r",ch);
   return;
  }
 }
 else
 {
  send_to_char("An error has occured. Contact a shell owner.\n\r",ch);
  return;
 }

 return;
}

void build_delroom( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *pRoomIndex;
   AREA_DATA *pArea;
   BUILD_DATA_LIST *pList;
   int vnum;
   static int old_vnum = 0;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   strcpy( arg2, argument );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax: delroom <vnum>\n\r", ch );
      return;
   }

   if( is_number( arg1 ) )
   {
      vnum = atoi( arg1 );
      if( !( pRoomIndex = get_room_index( vnum ) ) )
      {
         send_to_char( "Vnum not found.\n\r", ch );
         return;
      }

      if( !build_canwrite( pRoomIndex->area, ch, 1 ) )
         return;

      if( ( pRoomIndex->first_person ) != NULL )
      {
         send_to_char( "You cannot delete the room while people are in it!.\n\r", ch );
         return;
      }
      old_vnum = vnum;

      xprintf( buf, "Are you sure you want to delete room [%d] %s?\n\r", vnum, pRoomIndex->name );
      xcat( buf, "Type delroom ok if you are sure.\n\r" );
      send_to_char( buf, ch );
      return;
   }

   if( str_cmp( arg1, "ok" ) )   /* arg1 is NOT ok. */
   {
      /*
       * Usage message 
       */
      build_delroom( ch, "" );
      return;
   }

   if( old_vnum == 0 )
   {
      send_to_char( "First specify a room number.\n\r", ch );
      return;
   }


   /*
    * Now do deletion *gulp* 
    */
   vnum = old_vnum;
   old_vnum = 0;
   pRoomIndex = get_room_index( vnum );
   pArea = pRoomIndex->area;

   {
      /*
       * 'purge' room 
       */
      CHAR_DATA *victim;
      CHAR_DATA *vnext;
      OBJ_DATA *obj;
      OBJ_DATA *obj_next;

      for( victim = pRoomIndex->first_person; victim != NULL; victim = vnext )
      {
         vnext = victim->next_in_room;
         if( IS_NPC( victim ) && victim != ch )
            extract_char( victim, TRUE );
      }

      for( obj = pRoomIndex->first_content; obj != NULL; obj = obj_next )
      {
         obj_next = obj->next_in_room;
         extract_obj( obj );
      }
   }

   if( ( pRoomIndex->first_person ) != NULL )
   {
      send_to_char( "You cannot delete the room while people are in it!.\n\r", ch );
      old_vnum = 0;
      return;
   }

   /*
    * Scan other rooms for exits pointing to this one, and delete exits. 
    */
   /*
    * Delete all exits from this room too 
    */
   {
      ROOM_INDEX_DATA *pSrchRoom;
      EXIT_DATA *pExit;
      int door;

      for( pList = pArea->first_area_room; pList != NULL; pList = pList->next )
      {
         pSrchRoom = pList->data;

         for( door = 0; door < MAX_DIR; door++ )
         {
            if( ( pExit = pSrchRoom->exit[door] ) != NULL && ( pExit->to_room == pRoomIndex || pSrchRoom == pRoomIndex ) )
            {
               /*
                * Get rid of exit. 
                */
               PUT_FREE( pExit, exit_free );
               pSrchRoom->exit[door] = NULL;
               top_exit--;

               /*
                * spec: do the resets thing 
                */

               nuke_exit_resets( pSrchRoom, door );
            }
         }
      }
   }

   /*
    * Remove room from area list 
    */
   for( pList = pArea->first_area_room; pList; pList = pList->next )
      if( pList->data == pRoomIndex )
         break;
   if( pList )
   {
      UNLINK( pList, pArea->first_area_room, pArea->last_area_room, next, prev );
      PUT_FREE( pList, build_free );
   }

   /*
    * Remove room from vnum hash table 
    */
   {
      int iHash;

      iHash = vnum % MAX_KEY_HASH;
      SING_UNLINK( pRoomIndex, room_index_hash[iHash], next, ROOM_INDEX_DATA );
   }

   /*
    * wipe off resets referencing room. 
    */
   {
      BUILD_DATA_LIST *pNext;
      RESET_DATA *pReset;
      RESET_DATA *pNextReset;
      int iLastMobRoom;
      int found;

      /*
       * First get rid of data lists referencing the resets we want to kill. 
       */
      for( pList = pRoomIndex->first_room_reset; pList != NULL; pList = pNext )
      {
         pNext = pList->next;
         PUT_FREE( pList, build_free );
      }

      /*
       * Now kill resets from area reset list, and free memory 
       */
      iLastMobRoom = 0;

      for( pReset = pArea->first_reset; pReset != NULL; pReset = pNextReset )
      {
         pNextReset = pReset->next;
         found = 0;

         switch ( pReset->command )
         {
            case 'M':
               iLastMobRoom = pReset->arg3;
               if( pReset->arg3 == vnum )
                  found = 1;
               break;
            case 'O':
               if( pReset->arg3 == vnum )
                  found = 1;
               break;
            case 'D':
            case 'R':
               if( pReset->arg1 == vnum )
                  found = 1;
               break;
            case 'G':
            case 'E':
               if( iLastMobRoom == vnum )
                  found = 1;
               break;
         }

         if( found )
         {
            UNLINK( pReset, pArea->first_reset, pArea->last_reset, next, prev );
            PUT_FREE( pReset, reset_free );
            top_reset--;
         }
      }
   }


   /*
    * Now get rid of exits and free memory 
    */
   {
      EXIT_DATA *pExit;
      int door;

      for( door = 0; door < MAX_DIR; door++ )
      {
         if( ( pExit = pRoomIndex->exit[door] ) != NULL )
         {
            PUT_FREE( pExit, exit_free );
            pRoomIndex->exit[door] = NULL;
            top_exit--;

            /*
             * spec: resets aren't a problem, since they will get freed anyway 
             */
         }
      }
   }

   /*
    * Get rid of extra descriptions 
    */
   {
      EXTRA_DESCR_DATA *pNext;
      EXTRA_DESCR_DATA *pEd;

      for( pEd = pRoomIndex->first_exdesc; pEd != NULL; pEd = pNext )
      {
         pNext = pEd->next;
         PUT_FREE( pEd, exdesc_free );
      }
   }
   PUT_FREE( pRoomIndex->treasure, money_type_free );
   PUT_FREE( pRoomIndex, rid_free );

   top_room--;


   send_to_char( "Done.\n\r", ch );
   return;
}


int old_ovnum = 0;

void build_delobject( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];
   CHAR_DATA *vch;
   OBJ_INDEX_DATA *pObjIndex;
   AREA_DATA *pArea;
   BUILD_DATA_LIST *pList;
   int vnum;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   strcpy( arg2, argument );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax: delobject <vnum>\n\r", ch );
      return;
   }

   if( is_number( arg1 ) )
   {
      vnum = atoi( arg1 );
      if( !( pObjIndex = get_obj_index( vnum ) ) )
      {
         send_to_char( "Vnum not found.\n\r", ch );
      }

      if( !build_canwrite( pObjIndex->area, ch, 1 ) )
         return;

      old_ovnum = vnum;

      xprintf( buf, "Are you sure you want to delete object: [%d] %s?\n\r", vnum, pObjIndex->name );
      xcat( buf, "Type delobject ok if you are sure.\n\r" );
      send_to_char( buf, ch );
      return;
   }

   if( str_cmp( arg1, "ok" ) )   /* arg1 is NOT ok. */
   {
      /*
       * Usage message 
       */
      build_delobject( ch, "" );
      return;
   }

   if( old_ovnum == 0 )
   {
      send_to_char( "First specify a object number.\n\r", ch );
      return;
   }

   /*
    * make sure that NO one else has build_vnum set to this room!! 
    */
   for( vch = first_char; vch != NULL; vch = vch->next )
      if( vch->build_vnum == old_ovnum )
         vch->build_vnum = -1;


   /*
    * Now do deletion *gulp* 
    */
   vnum = old_ovnum;
   old_ovnum = 0;
   pObjIndex = get_obj_index( vnum );
   pArea = pObjIndex->area;

   /*
    * Things to get rid of: Resets, Affect, Extra descs, struct 
    */
   /*
    * Remove room from area list 
    */
   for( pList = pArea->first_area_object; pList; pList = pList->next )
      if( pList->data == pObjIndex )
         break;
   {
      UNLINK( pList, pArea->first_area_object, pArea->last_area_object, next, prev );
      PUT_FREE( pList, build_free );
   }



   /*
    * Remove object from vnum hash table 
    */
   {
      int iHash;

      iHash = vnum % MAX_KEY_HASH;
      SING_UNLINK( pObjIndex, obj_index_hash[iHash], next, OBJ_INDEX_DATA );
   }

   /*
    * wipe off resets referencing Obj. 
    */
   {
      /*
       * BUILD_DATA_LIST * pNext; Unused var 
       */
      RESET_DATA *pReset;
      RESET_DATA *pNextReset;
      int found;

      /*
       * Now kill resets from area reset list, and free memory 
       */
      for( pReset = pArea->first_reset; pReset != NULL; pReset = pNextReset )
      {
         pNextReset = pReset->next;
         found = 0;

         switch ( pReset->command )
         {
            case 'O':
               if( pReset->arg1 == vnum )
                  found = 1;
               break;
            case 'P':
               if( pReset->arg1 == vnum || pReset->arg3 == vnum )
                  found = 1;
               break;
            case 'G':
            case 'E':
               if( pReset->arg1 == vnum )
                  found = 1;
               break;
         }

         if( found )
         {
            UNLINK( pReset, pArea->first_reset, pArea->last_reset, next, prev );
            PUT_FREE( pReset, reset_free );
            top_reset--;
         }
      }
   }

   /*
    * Get rid of extra descriptions 
    */
   {
      EXTRA_DESCR_DATA *pNext;
      EXTRA_DESCR_DATA *pEd;

      for( pEd = pObjIndex->first_exdesc; pEd != NULL; pEd = pNext )
      {
         pNext = pEd->next;
         PUT_FREE( pEd, exdesc_free );
      }
   }

   /*
    * Get rid of affects 
    */
   {
      AFFECT_DATA *paf;
      AFFECT_DATA *pNext;

      for( paf = pObjIndex->first_apply; paf != NULL; paf = pNext )
      {
         pNext = paf->next;
         PUT_FREE( paf, affect_free );
      }
   }

   /*
    * Now delete structure 
    */
   PUT_FREE( pObjIndex, oid_free );

   top_obj_index--;

   send_to_char( "Done.\n\r", ch );
   return;
}


int old_mob_vnum = 0;

void build_delmob( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];
   CHAR_DATA *vch;
   MOB_INDEX_DATA *pMobIndex;
   AREA_DATA *pArea;
   BUILD_DATA_LIST *pList;
   int vnum;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   strcpy( arg2, argument );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax: delmobile <vnum>\n\r", ch );
      return;
   }

   if( is_number( arg1 ) )
   {
      vnum = atoi( arg1 );
      if( !( pMobIndex = get_mob_index( vnum ) ) )
      {
         send_to_char( "Vnum not found.\n\r", ch );
      }

      if( !build_canwrite( pMobIndex->area, ch, 1 ) )
         return;

      old_mob_vnum = vnum;

      xprintf( buf, "Are you sure you want to delete mobile: [%d] %s?\n\r", vnum, pMobIndex->player_name );
      xcat( buf, "Type delmobile ok if you are sure.\n\r" );
      send_to_char( buf, ch );
      return;
   }

   if( str_cmp( arg1, "ok" ) )   /* arg1 is NOT ok. */
   {
      /*
       * Usage message 
       */
      build_delmob( ch, "" );
      return;
   }

   if( old_mob_vnum == 0 )
   {
      send_to_char( "First specify a mobile number.\n\r", ch );
      return;
   }

   /*
    * make sure that NO one else has build_vnum set to this mob!! 
    */
   for( vch = first_char; vch != NULL; vch = vch->next )
      if( vch->build_vnum == old_ovnum )
         vch->build_vnum = -1;


   /*
    * Now do deletion *gulp* 
    */
   vnum = old_mob_vnum;
   old_mob_vnum = 0;
   pMobIndex = get_mob_index( vnum );
   pArea = pMobIndex->area;



   /*
    * Things to get rid of: Resets, Affect, Extra descs, struct 
    */
   /*
    * Take mobile out of area list 
    */
   for( pList = pArea->first_area_mobile; pList; pList = pList->next )
      if( pList->data == pMobIndex )
         break;
   if( pList )
   {
      UNLINK( pList, pArea->first_area_mobile, pArea->last_area_mobile, next, prev );
      PUT_FREE( pList, build_free );
   }



   /*
    * Get rid of mobile from world 
    */
   {
      CHAR_DATA *wch;
      CHAR_DATA *wchnext;
      for( wch = first_char; wch != NULL; wch = wchnext )
      {
         wchnext = wch->next;
         if( wch->pIndexData == pMobIndex )
            extract_char( wch, TRUE );
      }
   }

   /*
    * Remove mobile from vnum hash table 
    */
   {
      int iHash;

      iHash = vnum % MAX_KEY_HASH;
      SING_UNLINK( pMobIndex, mob_index_hash[iHash], next, MOB_INDEX_DATA );
   }

   /*
    * wipe off resets referencing Mob. 
    */
   {
      RESET_DATA *pReset;
      RESET_DATA *pNextReset;
      int found;
      int iLastMob;

      /*
       * Now kill resets from area reset list, and free memory 
       */
      iLastMob = 0;

      for( pReset = pArea->first_reset; pReset != NULL; pReset = pNextReset )
      {
         pNextReset = pReset->next;
         found = 0;

         switch ( pReset->command )
         {
            case 'M':
               iLastMob = pReset->arg1;
               if( pReset->arg1 == vnum )
                  found = 1;
               break;
            case 'G':
            case 'E':
               if( iLastMob == vnum )
                  found = 1;
               break;
         }

         if( found )
         {
            UNLINK( pReset, pArea->first_reset, pArea->last_reset, next, prev );
            PUT_FREE( pReset, reset_free );
            top_reset--;
         }
      }
   }

   /*
    * Get rid of shop 
    */
   if( pMobIndex->pShop != NULL )
   {
      SHOP_DATA *pShop;

      pShop = pMobIndex->pShop;

      /*
       * Take out of pList 
       */
      UNLINK( pList, pArea->first_area_shop, pArea->last_area_shop, next, prev );

      PUT_FREE( pList, build_free );

      /*
       * Take out of pShop linked list 
       */
      UNLINK( pShop, first_shop, last_shop, next, prev );

      /*
       * Now free shop structure 
       */
      PUT_FREE( pShop, shop_free );
   }


   /*
    * Get rid of mobprogs 
    */
   {
      MPROG_DATA *pMobProg;
      MPROG_DATA *pNextMobProg;

      for( pMobProg = pMobIndex->first_mprog; pMobProg != NULL; pMobProg = pNextMobProg )
      {
         pNextMobProg = pMobProg->next;
         PUT_FREE( pMobProg, mprog_free );
      }
   }

   /*
    * Now delete structure 
    */
   PUT_FREE( pMobIndex->act, bitmask_free );
   PUT_FREE( pMobIndex, mid_free );

   top_mob_index--;

   send_to_char( "Done.\n\r", ch );
   return;
}

void build_help( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];

   if( argument[0] != '\0' )  /* If an argument supplied... */
   {
      xprintf( buf, "build_%s", argument );  /* Format text to send */
      do_help( ch, buf );  /* Try and find BUILD_<helpname> in helps */
      return;
   }

   do_help( ch, BHELP_INDEX );
   return;
}


void reset_area( AREA_DATA * );

void build_forcereset( CHAR_DATA * ch, char *argument )
{
   AREA_DATA *pArea;

   pArea = ch->in_room->area;

   if( !build_canwrite( pArea, ch, 0 ) )
   {
      send_to_char( "You cannot reset this area.\n\r", ch );
      return;
   }

   reset_area( pArea );
   build_save_flush(  );

   send_to_char( "Done.\n\r", ch );
   return;
}

char *build_simpstrdup( char *buf )
{
   char *rvalue;

   build_strdup( &rvalue, buf, FALSE, FALSE, NULL );
   return rvalue;
}

#define STRING_FILE_DIR "temp/"

/* spec- rewritten to work correctly with SSM */

void build_strdup( char **dest, char *src, bool freesrc, bool newline, CHAR_DATA * ch )
{
   /*
    * Does the same as fread_string plus more, if there is enough memory. 
    */
   FILE *infile;
   char *filechar;
   char filename[255];
   char *old_destp;
   char *out;
   char buf[MSL];

   if( src[0] == '$' )  /* Special functions */
   {
      src++;
      if( src[0] != '$' )
      {
         /*
          * Check for edit, new, clear 
          */
         if( is_name( src, "edit new clear" ) && ch != NULL )
         {
            if( !str_cmp( src, "clear" ) )
            {
               if( freesrc && ( *dest ) != NULL )
                  free_string( *dest );
               *dest = &str_empty[0];
               return;
            }

            if( ( !str_cmp( src, "edit" ) ) && freesrc )
            {
               old_destp = *dest;
               build_editstr( dest, *dest, ch );
               if( ( old_destp ) != NULL )
                  free_string( old_destp );
               return;
            }

            /*
             * If clear, or freesrc is FALSE, start with a blank sheet. 
             */
            build_editstr( dest, "", ch );
            return;
         }

         if( freesrc && ( *dest ) != NULL )
            free_string( *dest );

         /*
          * Read in a file 
          */
         filename[0] = '\0';
         xcat( filename, STRING_FILE_DIR );
         xcat( filename, src );
         infile = file_open( filename, "r" );
         if( !infile )
            filechar = str_dup( "Could not open file.\n\r" );
         else
         {
/*       fBootDb=1; */
            /*
             * spec- we can't do the fBootDb thing, since SSM has already
             * * freed the hash table
             */
            filechar = fread_string( infile );
/*       fBootDb=0; */
         }
         *dest = filechar;
         return;
      }
   }


   if( freesrc && ( *dest ) != NULL )
      free_string( *dest );


   /*
    * spec- call str_dup to do most of the work 
    */

   /*
    * SSM still uses a single string block, so we're relatively safe here 
    */
   if( src >= string_space && src < top_string )
   {
      *dest = str_dup( src );
      return;
   }

   /*
    * spec- rewrite of the ugly fread_string close, to use str_dup 
    */

   /*
    * do literal \n -> LF etc. conversions to a buffer, then str_dup it 
    */
   /*
    * we assume here that the src string is <MSL (should be safe to do so) 
    */

   out = buf;

   while( *src )
   {
      switch ( *src )
      {
         default:
            *out++ = *src++;
            break;
         case '\n':
            *out++ = '\n';
            *out++ = '\r';
            src++;
            break;
         case '\r':
            src++;
            break;
         case '\\':
            switch ( *++src )
            {
               case 'n':
                  *out++ = '\n';
                  break;
               case 'r':
                  *out++ = '\r';
                  break;
               default:
                  *out++ = '\\';
                  *out++ = *src;
                  break;
            }
            if( *src )  /* don't overrun here.. */
               src++;
      }
   }

   if( newline ) /* Tack a newline on at the end --Kline */
   {
    *out++ = '\n';
    *out++ = '\r';
   }

   *out = 0;
   *dest = str_dup( buf );
}

void build_editstr( char **dest, char *src, CHAR_DATA * ch )
{
   /*
    * Starts a character editing. write_start sets *dest to the buffer
    */
   char *orig;

   orig = str_dup( src );

   send_to_char( "Editing string. Type .help for help.\n\r", ch );
   write_start( dest, build_finishedstr, orig, ch );

   if( *dest != &str_empty[0] )
   {
      send_to_char( src, ch );
      xcat_2( *dest, src );   /* Add src string into the buffer */
   }
   else
   {
      free_string( orig );
   }
   return;
}

void build_finishedstr( char *orig, char **dest, CHAR_DATA * ch, bool saved )
{
   if( !saved )
   {
      *dest = str_dup( orig );
   }

   free_string( orig );
   return;
}


void build_set_oedit( CHAR_DATA * ch, char *argument )
{
   OBJ_INDEX_DATA *obj;

   ch->act_build = ACT_BUILD_OEDIT;
   if( is_number( argument ) )
   {
      ch->build_vnum = atoi( argument );
      if( ( obj = get_obj_index( ch->build_vnum ) ) == NULL )
         ch->build_vnum = -1;
   }
   else
      ch->build_vnum = -1;

   send_to_char( ch->build_vnum == -1 ? "No vnum set.  Use setvnum.\n\r" : "Vnum now set. ", ch );
   return;
}

void build_set_redit( CHAR_DATA * ch, char *argument )
{
   ch->build_vnum = ch->in_room->vnum;

   ch->act_build = ACT_BUILD_REDIT;

   send_to_char( "Now in Redit mode.  Vnum set to current room.\n\r", ch );
   return;
}

void build_set_medit( CHAR_DATA * ch, char *argument )
{
   MOB_INDEX_DATA *mob;

   ch->act_build = ACT_BUILD_MEDIT;
   if( is_number( argument ) )
   {
      ch->build_vnum = atoi( argument );
      if( ( mob = get_mob_index( ch->build_vnum ) ) == NULL )
         ch->build_vnum = -1;
   }
   else
      ch->build_vnum = -1;

   send_to_char( ch->build_vnum == -1 ? "No vnum set.  Use setvnum.\n\r" : "Vnum now set. ", ch );
   return;
}

void build_set_nedit( CHAR_DATA * ch, char *argument )
{
   ch->act_build = ACT_BUILD_NOWT;
   ch->build_vnum = -1;

   send_to_char( "You are no longer in ANY editing mode.\n\r", ch );
   return;
}




void build_setvnum( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   int vnum;
   OBJ_INDEX_DATA *obj;
/*   ROOM_INDEX_DATA *room; unused */
   MOB_INDEX_DATA *mob;
   bool found;
   sh_int inc = 0;


   if( argument[0] == '\0' )
   {
      send_to_char( "USAGE: setvnum <vnum>, or v n(dec)/m(inc)\n\r", ch );
      return;
   }

   if( !is_number( argument ) )
   {
      if( !str_cmp( argument, "m" ) )
         inc = 1;
      else if( !str_cmp( argument, "n" ) )
         inc = -1;
      else
      {
         send_to_char( "Argument must be numeric. [vnum]\n\r", ch );
         return;
      }
   }
   if( is_number( argument ) )
      vnum = atoi( argument );
   else
   {

      vnum = ch->build_vnum + inc;
   }

   xprintf( buf, "Current vnum now set to: %d.\n\r", vnum );
   found = TRUE;

   switch ( ch->act_build )
   {
      case ACT_BUILD_OEDIT:
         if( ( obj = get_obj_index( vnum ) ) == NULL )
         {
            xprintf( buf2, "No object with that vnum exists.  Use addobject first.\n\r" );
            found = FALSE;
         }
         else
            xprintf( buf2, "Object exists: %s\n\r", obj->short_descr );

         break;

      case ACT_BUILD_REDIT:
         /*
          * do_goto( ch, itoa( vnum ) );   
          */
         ch->build_vnum = ch->in_room->vnum;
         send_to_char( "Vnum set to current room vnum.\n\r", ch );
         break;

      case ACT_BUILD_MEDIT:
         if( ( mob = get_mob_index( vnum ) ) == NULL )
         {
            xprintf( buf2, "No mobile with that vnum exists.  Use addmob first.\n\r" );
            found = FALSE;
         }
         else
            xprintf( buf2, "Mobile exists: %s\n\r", mob->short_descr );

         break;

      default:
         xprintf( buf2, "Please set your editing mode first!!\n\r" );

   }

   send_to_char( buf, ch );
   send_to_char( buf2, ch );

   if( found )
   {
      ch->build_vnum = vnum;
      send_to_char( "New vnum IS set.\n\r", ch );
      build_list( ch, "" );
   }
   else
   {
      send_to_char( "New vnum NOT set - still at old value.\n\r", ch );
   }

   return;
}

void build_list( CHAR_DATA * ch, char *argument )
{
   /*
    * do show obj|mob|room according to ch->act_build -S- 
    */

   char buf[MAX_STRING_LENGTH];
   bool found;

   found = FALSE;

   if( argument[0] == '\0' || ( ch->act_build == ACT_BUILD_REDIT && is_name( argument, "brief doors resets desc all" ) ) )
   {
      switch ( ch->act_build )
      {
         case ACT_BUILD_NOWT:
            send_to_char( "Not in any editing mode.  Nothing to show!\n\r", ch );
            break;
         case ACT_BUILD_OEDIT:
            if( ch->build_vnum == -1 )
               send_to_char( "No vnum has been selected!\n\r", ch );
            else
            {
               xprintf( buf, "%d", ch->build_vnum );
               build_showobj( ch, buf );
            }
            break;
         case ACT_BUILD_REDIT:
            if( ch->build_vnum == -1 )
               send_to_char( "No vnum has been selected!\n\r", ch );
            else
            {
               build_showroom( ch, argument );
            }
            break;
         case ACT_BUILD_MEDIT:
            if( ch->build_vnum == -1 )
               send_to_char( "No vnum has been selected!\n\r", ch );
            else
            {
               xprintf( buf, "%d", ch->build_vnum );
               build_showmob( ch, buf );
            }
            break;
      }
      return;
   }


   if( ch->act_build == ACT_BUILD_NOWT )
   {
      send_to_char( "You must be in an editing mode first!\n\r", ch );
      return;
   }

   /*
    * Ok, now arg is valid.  See if it applic. to edit mode 
    */

   if( !strcmp( argument, "flags" ) )
   {
      found = TRUE;
      switch ( ch->act_build )
      {
         case ACT_BUILD_REDIT:
            xprintf( buf, "Valid room flags are :\n\r" );
            wide_table_printout( tab_room_flags, buf + strlen( buf ) );
            send_to_char( buf, ch );
            break;
         case ACT_BUILD_MEDIT:
            xprintf( buf, "Valid mob flags are :\n\r" );
            wide_table_printout( tab_mob_act, buf + strlen( buf ) );
            send_to_char( buf, ch );
            break;
         case ACT_BUILD_OEDIT:
            xprintf( buf, "Valid object flags are :\n\r" );
            wide_table_printout( tab_obj_flags, buf + strlen( buf ) );
            send_to_char( buf, ch );
            break;
      }
   }

   if( !strcmp( argument, "aff" ) )
   {
      switch ( ch->act_build )
      {
         case ACT_BUILD_OEDIT:
            xprintf( buf, "Valid object affects are :\n\r" );
            wide_table_printout( tab_obj_aff, buf + strlen( buf ) );
            send_to_char( buf, ch );
            found = TRUE;
            break;
         case ACT_BUILD_MEDIT:
            xprintf( buf, "Valid mob affects are :\n\r" );
            wide_table_printout( tab_affected_by, buf + strlen( buf ) );
            send_to_char( buf, ch );
            found = TRUE;
            break;
         default:
            send_to_char( "Only valid when in Oedit or Medit modes.\n\r", ch );
      }
   }

   if( !strcmp( argument, "types" ) )
   {
      if( ch->act_build != ACT_BUILD_OEDIT )
      {
         send_to_char( "Only valid when in Oedit mode.\n\r", ch );
         return;
      }
      found = TRUE;
      xprintf( buf, "Valid object types are :\n\r" );
      wide_table_printout( tab_item_types, buf + strlen( buf ) );
      send_to_char( buf, ch );
   }

   if( !strcmp( argument, "wear" ) )
   {
      if( ch->act_build != ACT_BUILD_OEDIT )
      {
         send_to_char( "Only valid when in Oedit mode.\n\r", ch );
         return;
      }
      found = TRUE;
      xprintf( buf, "Valid object wear flags are :\n\r" );
      wide_table_printout( tab_wear_flags, buf + strlen( buf ) );
      send_to_char( buf, ch );
   }

   if( !strcmp( argument, "loc" ) )
   {
      if( ch->act_build != ACT_BUILD_OEDIT )
      {
         send_to_char( "Only valid when in Oedit mode.\n\r", ch );
         return;
      }
      found = TRUE;
      xprintf( buf, "Valid object wear locations are :\n\r" );
      wide_table_printout( tab_wear_loc, buf + strlen( buf ) );
      send_to_char( buf, ch );
   }

   if( !strcmp( argument, "sec" ) )
   {
      if( ch->act_build != ACT_BUILD_REDIT )
      {
         send_to_char( "Only valid when in Redit mode.\n\r", ch );
         return;
      }
      found = TRUE;
      xprintf( buf, "Valid room sector types are :\n\r" );
      wide_table_printout( tab_sector_types, buf + strlen( buf ) );
      send_to_char( buf, ch );
   }

   if( !strcmp( argument, "exit" ) )
   {
      if( ch->act_build != ACT_BUILD_REDIT )
      {
         send_to_char( "Only valid when in Redit mode.", ch );
         return;
      }
      found = TRUE;
      xprintf( buf, "Valid room door types are :\n\r" );
      wide_table_printout( tab_door_types, buf + strlen( buf ) );
      send_to_char( buf, ch );
      xprintf( buf, "Valid room door states are :\n\r" );
      wide_table_printout( tab_door_states, buf + strlen( buf ) );
      send_to_char( buf, ch );
   }


   if( !found )
   {
      send_to_char( "You may display the following values:\n\r\n\r", ch );
      send_to_char( "Edit Mode:      Values:\n\r----------      -------\n\r", ch );
      send_to_char( "  Redit         FLAGS - room flags.\n\r", ch );
      send_to_char( "                SEC   - room sector types.\n\r", ch );
      send_to_char( "                EXIT  - door states / types.\n\r", ch );
      send_to_char( "  Medit         FLAGS - mob flags.\n\r", ch );
      send_to_char( "                AFF   - mob affected_by values.\n\r", ch );
      send_to_char( "  Oedit         TYPES - object types.\n\r", ch );
      send_to_char( "                FLAGS - object flags.\n\r", ch );
      send_to_char( "                WEAR  - object wear flags.\n\r", ch );
      send_to_char( "                LOC   - object wear locations.\n\r", ch );
      send_to_char( "                AFF   - object affected_by values.\n\r", ch );
      return;
   }
   return;
}

void build_set( CHAR_DATA * ch, char *argument )
{
   /*
    * Call setroom/mob/obj with argument, and vnum, etc. 
    */
   char buf[MAX_STRING_LENGTH];

   switch ( ch->act_build )
   {
      case ACT_BUILD_OEDIT:
         if( ch->build_vnum == -1 )
         {
            send_to_char( "No vnum is set!!\n\r", ch );
            return;
         }
         xprintf( buf, "%d %s", ch->build_vnum, argument );
         build_setobject( ch, buf );
         break;
      case ACT_BUILD_REDIT:
         if( ch->build_vnum == -1 )
         {
            send_to_char( "No vnum is set!!\n\r", ch );
            return;
         }
         build_setroom( ch, argument );
         break;
      case ACT_BUILD_MEDIT:
         if( ch->build_vnum == -1 )
         {
            send_to_char( "No vnum is set!!\n\r", ch );
            return;
         }
         xprintf( buf, "%d %s", ch->build_vnum, argument );
         build_setmob( ch, buf );
         break;
      default:
         send_to_char( "You are not in any editing mode!\n\r", ch );
   }
   return;
}

void build_listvalues( CHAR_DATA * ch, char *argument )
{
   /*
    * Lookup what the 4 values mean for the given object type -S- 
    */
   int value;
   int foo;
   char buf[MAX_STRING_LENGTH];


   if( argument[0] == '\0' )
   {
      send_to_char( "USAGE: values <item-type-name>\n\r", ch );
      send_to_char( "Eg: values staff\n\r", ch );
      return;
   }

   value = table_lookup( tab_item_types, argument );
   if( value == 0 )
   {
      xprintf( buf, "Valid object types are :\n\r" );
      wide_table_printout( tab_item_types, buf + strlen( buf ) );
      send_to_char( buf, ch );
      return;
   }

   send_to_char( "Details for value0,...,value3 are:\n\r", ch );
   value *= 10;
   for( foo = 0; foo < 4; foo++ )
   {
      xprintf( buf, "@@W[Value@@y%d@@W] : @@y%s@@g\n\r", foo, rev_table_lookup( tab_value_meanings, value + foo ) );
      send_to_char( buf, ch );
   }
   return;
}

void build_listweapons( CHAR_DATA * ch, char *argument )
{
   /*
    * list weapon types, along with values 
    */
   char buf[MAX_STRING_LENGTH];
   int foo;

   /*
    * Need values as well, so rehash table_printout 
    */
   for( foo = 0; tab_weapon_types[foo].text != NULL; foo++ )
   {
      xprintf( buf, "@@W%2ld - @@y%10s.   ", tab_weapon_types[foo].value, tab_weapon_types[foo].text );
      send_to_char( buf, ch );
      if( ( foo + 1 ) % 2 == 0 )
         send_to_char( "\n\r", ch );
   }
   send_to_char( "@@g\n\r", ch );
   return;
}

void build_listliquids( CHAR_DATA * ch, char *argument )
{
   /*
    * list liquid types, along with values 
    */
   char buf[MAX_STRING_LENGTH];
   int foo;

   /*
    * Need values as well, so rehash table_printout 
    */
   for( foo = 0; tab_drink_types[foo].text != NULL; foo++ )
   {
      xprintf( buf, "%2ld - %12s.   ", tab_drink_types[foo].value, tab_drink_types[foo].text );
      send_to_char( buf, ch );
      if( ( foo + 1 ) % 2 == 0 )
         send_to_char( "\n\r", ch );
   }
   send_to_char( "\n\r", ch );
   return;
}

void build_listspells( CHAR_DATA * ch, char *argument )
{
   /*
    * List spells -S- 
    */
   int sn;
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   int type;
   bool fall;

   if( argument[0] == '\0' )
   {
      send_to_char( "USAGE: spells < target_type >\n\r", ch );
      send_to_char( "target type being one of:  ign - picks own target\n\r", ch );
      send_to_char( "                           off - offensive\n\r", ch );
      send_to_char( "                           def - defensive\n\r", ch );
      send_to_char( "                           slf - personal-only (self)\n\r", ch );
      send_to_char( "                           obj - object-only\n\r", ch );
      send_to_char( "                           all - list ALL spells\n\r", ch );
      return;
   }

   fall = FALSE;
   type = -1;
   buf2[0] = '\0';

   if( !strcmp( argument, "all" ) )
      fall = TRUE;
   else if( !strcmp( argument, "ign" ) )
      type = TAR_IGNORE;
   else if( !strcmp( argument, "off" ) )
      type = TAR_CHAR_OFFENSIVE;
   else if( !strcmp( argument, "def" ) )
      type = TAR_CHAR_DEFENSIVE;
   else if( !strcmp( argument, "slf" ) )
      type = TAR_CHAR_SELF;
   else if( !strcmp( argument, "obj" ) )
      type = TAR_OBJ_INV;
   else
   {
      send_to_char( "Option not recognized.\n\r", ch );
      build_listspells( ch, "" );   /* usage message */
      return;
   }

   xprintf( buf2, "List of spells for option %s:\n\r", argument );

   for( sn = 0; sn < MAX_SKILL; sn++ )
   {
      if( skill_table[sn].name == NULL )
         break;
      if( skill_table[sn].slot == 0 )
         continue;

      if( skill_table[sn].target == type || fall )
      {
         xprintf( buf, "@@W[Spell No: %4d] @@y%s@@g\n\r", sn, skill_table[sn].name );
         xcat( buf2, buf );
      }
   }
   send_to_char( buf2, ch );
   return;
}

int get_dir( char dir )
{
   char *temp;
   if( dir == '\0' || ( temp = strchr( cDirs, dir ) ) == NULL )
      return -1;

   return temp - cDirs;
}

void build_urooms( CHAR_DATA * ch, char *argument )
{
   /*
    * List vnum usage for area... 
    */
   int curvnum;
   char buf[MAX_STRING_LENGTH];
   char free[MAX_STRING_LENGTH];
   char used[MAX_STRING_LENGTH];
   AREA_DATA *area;
   int last = 0;  /* 0 = start, 1 = used, 2 = free */
   int foo = 0;   /* holds start of free/used vnums, so no 3001-3001 */

   area = ch->in_room->area;
   /*
    * Rooms 
    */
   xprintf( free, "(Free) " );
   xprintf( used, "(Used) " );
   for( curvnum = area->min_vnum; curvnum < area->max_vnum; curvnum++ )
   {
      if( get_room_index( curvnum ) != NULL )
      {
         switch ( last )
         {
            case 0:
               xprintf( buf, "%d", curvnum );
               xcat( used, buf );
               foo = curvnum;
               last = 1;
               break;
            case 1:
               break;
            case 2:
               if( foo != curvnum - 1 )
               {
                  xprintf( buf, "-%d", curvnum - 1 );
                  xcat( free, buf );
               }
               xprintf( buf, " %d", curvnum );
               xcat( used, buf );
               foo = curvnum;
               last = 1;
         }
      }
      else
      {
         switch ( last )
         {
            case 0:
               xprintf( buf, "%d", curvnum );
               xcat( free, buf );
               foo = curvnum;
               last = 2;
               break;
            case 1:
               if( foo != curvnum - 1 )
               {
                  xprintf( buf, "-%d", curvnum - 1 );
                  xcat( used, buf );
               }
               xprintf( buf, " %d", curvnum );
               xcat( free, buf );
               last = 2;
               foo = curvnum;
               break;
            case 2:
               break;
         }
      }
   }

   curvnum = area->max_vnum;
   if( get_room_index( curvnum ) != NULL )
   {
      switch ( last )
      {
         case 1:
            if( foo != ( curvnum - 1 ) )
               xprintf( buf, "-%d.", curvnum );
            else
               xprintf( buf, " %d.", curvnum );
            xcat( used, buf );
            break;
         case 2:
            if( foo != curvnum - 1 )
            {
               xprintf( buf, "-%d.", curvnum - 1 );
               xcat( used, buf );
            }
            xprintf( buf, " %d.", curvnum );
            xcat( free, buf );
            break;
      }
   }
   else
   {
      switch ( last )
      {
         case 1:
            if( foo != curvnum - 1 )
            {
               xprintf( buf, "-%d.", curvnum - 1 );
               xcat( used, buf );
            }
            xprintf( buf, " %d.", curvnum );
            xcat( free, buf );
            break;
         case 2:
            if( foo != curvnum - 1 )
               xprintf( buf, "-%d.", curvnum );
            else
               xprintf( buf, " %d.", curvnum );
            xcat( free, buf );
            break;
      }
   }

   xprintf( buf, "Room vnum usage summary:\n\r\n\r%s\n\r\n\r%s\n\r", used, free );
   send_to_char( buf, ch );
   return;
}



void build_uobjs( CHAR_DATA * ch, char *argument )
{
   /*
    * List vnum usage for area... 
    */
   int curvnum;
   char buf[MAX_STRING_LENGTH];
   char free[MAX_STRING_LENGTH];
   char used[MAX_STRING_LENGTH];
   AREA_DATA *area;
   int last = 0;  /* 0 = start, 1 = used, 2 = free */
   int foo = 0;   /* holds start of free/used vnums, so no 3001-3001 */

   area = ch->in_room->area;
   /*
    * Rooms 
    */
   xprintf( free, "(Free) " );
   xprintf( used, "(Used) " );
   for( curvnum = area->min_vnum; curvnum < area->max_vnum; curvnum++ )
   {
      if( get_obj_index( curvnum ) != NULL )
      {
         switch ( last )
         {
            case 0:
               xprintf( buf, "%d", curvnum );
               xcat( used, buf );
               foo = curvnum;
               last = 1;
               break;
            case 1:
               break;
            case 2:
               if( foo != curvnum - 1 )
               {
                  xprintf( buf, "-%d", curvnum - 1 );
                  xcat( free, buf );
               }
               xprintf( buf, " %d", curvnum );
               xcat( used, buf );
               foo = curvnum;
               last = 1;
         }
      }
      else
      {
         switch ( last )
         {
            case 0:
               xprintf( buf, "%d", curvnum );
               xcat( free, buf );
               foo = curvnum;
               last = 2;
               break;
            case 1:
               if( foo != curvnum - 1 )
               {
                  xprintf( buf, "-%d", curvnum - 1 );
                  xcat( used, buf );
               }
               xprintf( buf, " %d", curvnum );
               xcat( free, buf );
               last = 2;
               foo = curvnum;
               break;
            case 2:
               break;
         }
      }
   }

   curvnum = area->max_vnum;
   if( get_obj_index( curvnum ) != NULL )
   {
      switch ( last )
      {
         case 1:
            if( foo != ( curvnum - 1 ) )
               xprintf( buf, "-%d.", curvnum );
            else
               xprintf( buf, " %d.", curvnum );
            xcat( used, buf );
            break;
         case 2:
            if( foo != curvnum - 1 )
            {
               xprintf( buf, "-%d.", curvnum - 1 );
               xcat( used, buf );
            }
            xprintf( buf, " %d.", curvnum );
            xcat( free, buf );
            break;
      }
   }
   else
   {
      switch ( last )
      {
         case 1:
            if( foo != curvnum - 1 )
            {
               xprintf( buf, "-%d.", curvnum - 1 );
               xcat( used, buf );
            }
            xprintf( buf, " %d.", curvnum );
            xcat( free, buf );
            break;
         case 2:
            if( foo != curvnum - 1 )
               xprintf( buf, "-%d.", curvnum );
            else
               xprintf( buf, " %d.", curvnum );
            xcat( free, buf );
            break;
      }
   }

   xprintf( buf, "Object vnum usage summary:\n\r\n\r%s\n\r\n\r%s\n\r", used, free );
   send_to_char( buf, ch );
   return;
}


void build_umobs( CHAR_DATA * ch, char *argument )
{
   /*
    * List vnum usage for area... 
    */
   int curvnum;
   char buf[MAX_STRING_LENGTH];
   char free[MAX_STRING_LENGTH];
   char used[MAX_STRING_LENGTH];
   AREA_DATA *area;
   int last = 0;  /* 0 = start, 1 = used, 2 = free */
   int foo = 0;   /* holds start of free/used vnums, so no 3001-3001 */

   area = ch->in_room->area;
   xprintf( free, "(Free) " );
   xprintf( used, "(Used) " );
   for( curvnum = area->min_vnum; curvnum < area->max_vnum; curvnum++ )
   {
      if( get_mob_index( curvnum ) != NULL )
      {
         switch ( last )
         {
            case 0:
               xprintf( buf, "%d", curvnum );
               xcat( used, buf );
               foo = curvnum;
               last = 1;
               break;
            case 1:
               break;
            case 2:
               if( foo != curvnum - 1 )
               {
                  xprintf( buf, "-%d", curvnum - 1 );
                  xcat( free, buf );
               }
               xprintf( buf, " %d", curvnum );
               xcat( used, buf );
               foo = curvnum;
               last = 1;
         }
      }
      else
      {
         switch ( last )
         {
            case 0:
               xprintf( buf, "%d", curvnum );
               xcat( free, buf );
               foo = curvnum;
               last = 2;
               break;
            case 1:
               if( foo != curvnum - 1 )
               {
                  xprintf( buf, "-%d", curvnum - 1 );
                  xcat( used, buf );
               }
               xprintf( buf, " %d", curvnum );
               xcat( free, buf );
               last = 2;
               foo = curvnum;
               break;
            case 2:
               break;
         }
      }
   }

   curvnum = area->max_vnum;
   if( get_mob_index( curvnum ) != NULL )
   {
      switch ( last )
      {
         case 1:
            if( foo != ( curvnum - 1 ) )
               xprintf( buf, "-%d.", curvnum );
            else
               xprintf( buf, " %d.", curvnum );
            xcat( used, buf );
            break;
         case 2:
            if( foo != curvnum - 1 )
            {
               xprintf( buf, "-%d.", curvnum - 1 );
               xcat( used, buf );
            }
            xprintf( buf, " %d.", curvnum );
            xcat( free, buf );
            break;
      }
   }
   else
   {
      switch ( last )
      {
         case 1:
            if( foo != curvnum - 1 )
            {
               xprintf( buf, "-%d.", curvnum - 1 );
               xcat( used, buf );
            }
            xprintf( buf, " %d.", curvnum );
            xcat( free, buf );
            break;
         case 2:
            if( foo != curvnum - 1 )
               xprintf( buf, "-%d.", curvnum );
            else
               xprintf( buf, " %d.", curvnum );
            xcat( free, buf );
            break;
      }
   }

   xprintf( buf, "Mobile vnum usage summary:\n\r\n\r%s\n\r\n\r%s\n\r", used, free );
   send_to_char( buf, ch );
   return;
}



/** Help Editor 
    We want to be able to edit ANY help, so 3.bank etc should work,
    in case we have helps with the same keyword(s).
 **/
void build_findhelp( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_STRING_LENGTH];
   int cnt = 0;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Usage: Findhelp <keyword>\n\r", ch );
      return;
   }

   if( cnt == 0 )
      send_to_char( "Couldn't find that keyword.\n\r", ch );
   return;
}




void build_helpedit( CHAR_DATA * ch, char *argument )
{
 char arg[MAX_STRING_LENGTH];
 bool mort = FALSE;
 FILE *fp;

 argument = strlower(argument);
 argument = one_argument(argument,arg);
 smash_replace(argument," ","_");

 if( argument[0] == '\0' )
 {
  send_to_char("Usage: HELPEDIT <mort/imm> <keyword>.\n\r",ch);
  return;
 }
 if( !str_prefix(arg,"mort") )
  mort = TRUE;
 else if( !str_prefix(arg,"imm") )
  mort = FALSE;
 else
 {
  build_helpedit(ch,"");
  return;
 }

 xprintf(arg,"%s%s/%s.%s",HELP_DIR,initial(argument),argument,mort ? HELP_MORT : HELP_IMM);
 if( (fp = file_open(arg,"r")) == NULL )
 {
  send_to_char("Couldn't find that keyword.\n\r",ch);
  file_close(fp);
  return;
 }
 else if( (fp = file_open(arg,"r")) != NULL )
 {
  char buf1[MSL];
  char buf2[MSL];

  buf1[0] = '\0';
  buf2[0] = '\0';
  ch->pcdata->header = str_dup(arg);

  while( fgets(buf1,MSL,fp) )
  {
   if( buf2[0] == '\0' )
    xprintf(buf2,buf1);
   else
    xcat(buf2,buf1);
  }
  ch->pcdata->message = str_dup(buf2);

  build_strdup(&ch->pcdata->message,"$edit",TRUE,FALSE,ch);
  file_close(fp);
 }
 else
 {
  send_to_char("An error has occured. Contact a shell owner.\n\r",ch);
  return;
 }

 return;
}

void build_addhelp( CHAR_DATA * ch, char *argument )
{
 char arg[MAX_STRING_LENGTH];
 bool mort = FALSE;
 FILE *fp;

 argument = strlower(argument);
 argument = one_argument(argument,arg);
 smash_replace(argument," ","_");

 if( argument[0] == '\0' )
 {
  send_to_char("Usage: ADDHELP <mort/imm> <keyword>.\n\r",ch);
  return;
 }
 if( !str_prefix(arg,"mort") )
  mort = TRUE;
 else if( !str_prefix(arg,"imm") )
  mort = FALSE;
 else
 {
  build_addhelp(ch,"");
  return;
 }

 xprintf(arg,"%s%s/%s.%s",HELP_DIR,initial(argument),argument,mort ? HELP_MORT : HELP_IMM);
 if( (fp = file_open(arg,"r")) != NULL )
 {
  send_to_char("Help already exists. Use HELPEDIT <keyword> to edit it.\n\r",ch);
  file_close(fp);
  return;
 }
 else if( (fp = file_open(arg,"w")) != NULL )
 {
  send_to_char("Help added. Use HELPEDIT <keyword> to edit it.\n\r",ch);
  file_close(fp);
  return;
 }
 else
 {
  send_to_char("An error has occured. Contact a shell owner.\n\r",ch);
  return;
 }

 return;
}

/* NOTE--NEED TO MAKE SURE WE GET MOTD, TOO--I THINK IT WIL BE OKAY ZEN */


void build_clone( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];

   /*
    * Allow builder to clone a room/mob/object -
    * Takes existing r/m/o and makes new copy with relevant details
    * copied across...
    */

   if( argument[0] == '\0' )  /* Show help info */
   {
      send_to_char( "Usage: CLONE <type> <vnum>\n\r", ch );
      send_to_char( "Where <type> is one of: mob  room  obj\n\r", ch );
      send_to_char( "      <vnum> is the vnum of the type you want to clone.\n\r", ch );
      send_to_char( "[The item will be copied onto the exisiting item you are editing]\n\r", ch );
      return;
   }

   argument = one_argument( argument, arg1 );   /* room/mob/obj     */
   argument = one_argument( argument, arg2 );   /* vnum to clone to */

   /*
    * Check arguments 
    */
   if( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) || !is_name( arg1, "room obj mob" ) )
   {
      build_clone( ch, "" );
      return;
   }

   if( !str_cmp( arg1, "room" ) )
   {
      ROOM_INDEX_DATA *room;
      ROOM_INDEX_DATA *in_room = ch->in_room;
      /*
       * EXTRA_DESCR_DATA *ed 
       */

      if( ch->act_build != ACT_BUILD_REDIT )
      {
         send_to_char( "You must be in REDIT mode to clone a room.\n\r", ch );
         return;
      }

      if( ( room = get_room_index( atoi( arg2 ) ) ) == NULL )
      {
         send_to_char( "Sorry, no such room with that vnum.\n\r", ch );
         return;
      }

      if( in_room == NULL )
      {
         send_to_char( "Don't know what room you're in!!\n\r", ch );
         return;
      }

      /*
       * Check builder can read room, and write to in_room 
       */
      if( !build_canread( room->area, ch, 0 ) )
      {
         send_to_char( "Sorry, you don't have authorization to read that room.\n\r", ch );
         return;
      }
      if( !build_canwrite( in_room->area, ch, 0 ) )
      {
         send_to_char( "Sorry, you don't have authorization to write into this room.\n\r", ch );
         return;
      }

      /*
       * Copy details across..... 
       */
      if( in_room->name != NULL )
         free_string( in_room->name );
      in_room->name = str_dup( room->name );
      if( in_room->description != NULL )
         free_string( in_room->description );
      in_room->description = str_dup( room->description );
      in_room->sector_type = room->sector_type;
      in_room->room_flags = room->room_flags;

      /*
       * FIXME:
       * * Copy extra descriptions
       */


      send_to_char( "Room cloned.\n\r", ch );
      return;
   }

   if( !str_cmp( arg1, "obj" ) )
   {
      OBJ_INDEX_DATA *obj;
      OBJ_INDEX_DATA *this_obj;
      int looper;
      if( ch->act_build != ACT_BUILD_OEDIT )
      {
         send_to_char( "You must be in OEDIT mode to clone an object.\n\r", ch );
         return;
      }

      if( ( this_obj = get_obj_index( ch->build_vnum ) ) == NULL )
      {
         send_to_char( "You must select a valid object in OEDIT before you clone.\n\r", ch );
         return;
      }

      if( ( obj = get_obj_index( atoi( arg2 ) ) ) == NULL )
      {
         send_to_char( "That object does not exist to be cloned.\n\r", ch );
         return;
      }

      if( !build_canread( obj->area, ch, 0 ) )
      {
         send_to_char( "Sorry, you don't have authorization to read that object.\n\r", ch );
         return;
      }
      if( !build_canwrite( this_obj->area, ch, 0 ) )
      {
         send_to_char( "Sorry, you don't have authorization to write to this object.\n\r", ch );
         return;
      }

      /*
       * Copy details across... 
       */
      if( this_obj->name != NULL )
         free_string( this_obj->name );
      this_obj->name = str_dup( obj->name );
      this_obj->level = obj->level;
      if( this_obj->short_descr != NULL )
         free_string( this_obj->short_descr );
      this_obj->short_descr = str_dup( obj->short_descr );
      if( this_obj->description != NULL )
         free_string( this_obj->description );
      this_obj->description = str_dup( obj->description );
      this_obj->item_type = obj->item_type;
      this_obj->extra_flags = obj->extra_flags;
      this_obj->wear_flags = obj->wear_flags;
      this_obj->item_apply = obj->item_apply;
      for( looper = 0; looper < 10; looper++ )
         this_obj->value[looper] = obj->value[looper];

      this_obj->weight = obj->weight;

      /*
       * FIXME:
       * * Copy extra descriptions
       * * handle obj_funs
       */

      send_to_char( "Object cloned.\n\r", ch );
      return;
   }

   if( !str_cmp( arg1, "mob" ) )
   {
      MOB_INDEX_DATA *mob;
      MOB_INDEX_DATA *this_mob;

      if( ch->act_build != ACT_BUILD_MEDIT )
      {
         send_to_char( "You must be in MEDIT mode to clone mobiles.\n\r", ch );
         return;
      }

      if( ( this_mob = get_mob_index( ch->build_vnum ) ) == NULL )
      {
         send_to_char( "You must select a valid mobile in MEDIT before you clone mobiles.\n\r", ch );
         return;
      }

      if( ( mob = get_mob_index( atoi( arg2 ) ) ) == NULL )
      {
         send_to_char( "That mobile does not exist to be cloned.\n\r", ch );
         return;
      }

      if( !build_canread( mob->area, ch, 0 ) )
      {
         send_to_char( "Sorry, you do not have authorization to read that mobile.\n\r", ch );
         return;
      }
      if( !build_canwrite( this_mob->area, ch, 0 ) )
      {
         send_to_char( "Sorry, you do not have authorization to write to this mobile.\n\r", ch );
         return;
      }

      /*
       * Copy details across... 
       */
      if( this_mob->player_name != NULL )
         free_string( this_mob->player_name );
      this_mob->player_name = str_dup( mob->player_name );
      if( this_mob->short_descr != NULL )
         free_string( this_mob->short_descr );
      this_mob->short_descr = str_dup( mob->short_descr );
      if( this_mob->long_descr != NULL )
         free_string( this_mob->long_descr );
      this_mob->long_descr = str_dup( mob->long_descr );
      if( this_mob->description != NULL )
         free_string( this_mob->description );
      this_mob->description = str_dup( mob->description );

      this_mob->act = mob->act;
      this_mob->affected_by = mob->affected_by;
      this_mob->alignment = mob->alignment;
      this_mob->level = mob->level;
      this_mob->sex = mob->sex;
      this_mob->ac_mod = mob->ac_mod;
      this_mob->hr_mod = mob->hr_mod;
      this_mob->dr_mod = mob->dr_mod;
      this_mob->cast = mob->cast;
      this_mob->def = mob->def;
      this_mob->skills = mob->skills;

      /*
       * FIXME:
       * * Copy shop details (if any) across
       * * handle spec_fun
       * * ignore mob_progs?
       */

      send_to_char( "Mobile cloned.\n\r", ch );
      return;
   }
   return;
}

void check_autodig( CHAR_DATA *ch, int dir )
{
 if( ch->position == POS_BUILDING && ch->act_build == ACT_BUILD_REDIT && is_set(ch->act,ACT_AUTODIG) )
 {
  AREA_DATA *pArea;
  EXIT_DATA *pExit;
  int vnum;
  bool found = FALSE;
  char buf[20];
  char exit[6];

  pExit = ch->in_room->exit[dir];
  pArea = ch->in_room->area;
  vnum = pArea->min_vnum;

  if( pExit != NULL ) /* Don't try to dig out an existing exit --Kline */
   return;

  while( !found )
  {
   vnum++;

   if( get_room_index(vnum) )
    found = FALSE;
   else
    found = TRUE;
  }

  if( vnum > pArea->max_vnum )
  {
   send_to_char("AUTODIG: No more free vnums in area.\n\r",ch);
   return;
  }

  switch( dir )
  {
   case DIR_NORTH: xprintf(exit,"north"); break;
   case DIR_SOUTH: xprintf(exit,"south"); break;
   case DIR_EAST:  xprintf(exit,"east");  break;
   case DIR_WEST:  xprintf(exit,"west");  break;
   case DIR_UP:    xprintf(exit,"up");    break;
   case DIR_DOWN:  xprintf(exit,"down");  break;
  }

  xprintf(buf,"%s %d",exit,vnum);
  build_dig(ch,buf);
 }
}

void build_sysdata( CHAR_DATA *ch, char *argument )
{
 char outbuf[MSL];
 char catbuf[MSL];
 char arg1[MSL];
 char arg2[MSL];

 argument = one_argument(argument,arg1);
 argument = one_argument(argument,arg2);

 if( arg1[0] == '\0' )
 {
  send_to_char("Syntax for sysdata:\n\r",ch);
  send_to_char("  sysdata help | show | <option> <value>\n\r",ch);
  send_to_char("  options: obj[ac | dr | hp | hr | mp | mv | svs]\n\r",ch);
  return;
 }

 if( !str_prefix(arg1,"help") )
 {
  build_help(ch,"sysdata");
  return;
 }

 if( !str_prefix(arg1,"show") )
 {
  xprintf(outbuf,"%s","  Builder system data for " mudnamecolor ":\n\r");
  xprintf(catbuf,"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r");
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Option         ]       [Value          ]\n\r");
  xcat(outbuf,catbuf);
  xprintf(catbuf,"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r");
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Obj AC         ]       [%15d]\n\r",sysdata.build_obj_ac);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Obj DR         ]       [%15d]\n\r",sysdata.build_obj_dr);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Obj HP         ]       [%15d]\n\r",sysdata.build_obj_hp);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Obj HR         ]       [%15d]\n\r",sysdata.build_obj_hr);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Obj MP         ]       [%15d]\n\r",sysdata.build_obj_mp);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Obj MV         ]       [%15d]\n\r",sysdata.build_obj_mv);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Obj SVS        ]       [%15d]\n\r",sysdata.build_obj_svs);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r");
  xcat(outbuf,catbuf);
  send_to_char(outbuf,ch);
  return;
 }

 if( !str_cmp(arg1,"objac") )
  sysdata.build_obj_ac = atoi(arg2) != 0 ? atoi(arg2) : 1;
 else if( !str_cmp(arg1,"objdr") )
  sysdata.build_obj_dr = atoi(arg2) != 0 ? atoi(arg2) : 1;
 else if( !str_cmp(arg1,"objhp") )
  sysdata.build_obj_hp = atoi(arg2) != 0 ? atoi(arg2) : 1;
 else if( !str_cmp(arg1,"objhr") )
  sysdata.build_obj_hr = atoi(arg2) != 0 ? atoi(arg2) : 1;
 else if( !str_cmp(arg1,"objmp") )
  sysdata.build_obj_mp = atoi(arg2) != 0 ? atoi(arg2) : 1;
 else if( !str_cmp(arg1,"objmv") )
  sysdata.build_obj_mv = atoi(arg2) != 0 ? atoi(arg2) : 1;
 else if( !str_cmp(arg1,"objsvs") )
  sysdata.build_obj_svs = atoi(arg2) != 0 ? atoi(arg2) : 1;
 else
 {
  build_sysdata(ch,"");
  return;
 }

 save_sysdata( );
 build_sysdata(ch,"show");

 return;
}
