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

#ifndef DEC_ACT_INFO_H
#include "h/act_info.h"
#endif

#ifndef DEC_ACT_MOVE_H
#include "h/act_move.h"
#endif

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_AREACHK_H
#include "h/areachk.h"
#endif

#ifndef DEC_AREASAVE_H
#include "h/areasave.h"
#endif

#ifndef DEC_BUILD_H
#include "h/build.h"
#endif

#ifndef DEC_BUILDARE_H
#include "h/buildare.h"
#endif

#ifndef DEC_BUILDTAB_H
#include "h/buildtab.h"
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

#ifndef DEC_LUASCRIPT_H
#include "h/luascript.h"
#endif

#ifndef DEC_MONEY_H
#include "h/money.h"
#endif

#ifndef DEC_OBJ_FUN_H
#include "h/obj_fun.h"
#endif

#ifndef DEC_SAVE_H
#include "h/save.h"
#endif

#ifndef DEC_SPEC_FUN_H
#include "h/spec_fun.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

#ifndef DEC_SYSDATA_H
#include "h/sysdata.h"
#endif

#ifndef DEC_WRITE_H
#include "h/write.h"
#endif

/* This program provides the interpreting of building commands */

/* The tables are now in buildtab.c: (This file was getting a bit big.)
     Mob act flags       :    tab_mob_act         : bitset
     Player act flags    :    tab_player_act      : bitset
     Mob affected by     :    tab_affected_by     : bit_vector
     Object item type    :    tab_item_types      : number
     Object extra flags  :    tab_obj_flags       : bitset
     Object wear flags   :    tab_wear_flags      : bitset
     Object affect types :    tab_obj_aff         : number
     Class types         :    tab_class           : bit_vector
     Wear locations      :    tab_wear_loc        : number
     Room flags          :    tab_room_flags      : bitset
     Sector types        :    tab_sector_types    : number
     Door types          :    tab_door_types      : bitset
     Door states         :    tab_door_states     : number

*/

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
   {"addroom", build_addroom, POS_STANDING, 0, LOG_NORMAL},
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

/* Variables declared in db.c, which we need */
extern char *string_hash[MAX_KEY_HASH];

extern char *string_space;
extern char *top_string;
extern char str_empty[1];

extern AREA_DATA *area_last;
extern AREA_DATA *area_first;
#define                 MAX_PERM_BLOCK  131072
extern long int nAllocString;
extern long int sAllocString;
extern int nAllocPerm;
extern int sAllocPerm;

DO_FUN(build_interpret)
{
   char command[MSL];
   char logline[MSL];
   char buffer[MSL];
   int cmd;
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

   if( ( !IS_NPC( ch ) && ch->act.test(ACT_LOG) ) || fLogAll || build_cmd_table[cmd].log == LOG_ALWAYS )
   {
      snprintf( log_buf, (2 * MIL),  "Log %s: %s", ch->name.c_str(), logline );
      log_string( log_buf );
      monitor_chan( log_buf, MONITOR_BUILD );
   }

   if( ch->desc != NULL && ch->desc->snoop_by != NULL )
   {
      write_to_buffer( ch->desc->snoop_by, "% ", 2 );
      write_to_buffer( ch->desc->snoop_by, logline );
      write_to_buffer( ch->desc->snoop_by, "\r\n", 2 );
   }

   if( !found )
   {
      snprintf( buffer, MSL, "%s %s", command, argument );
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
   if( ch->pcdata->build_mode == BUILD_MODE_REDIT && ch->pcdata->build_vnum != ch->in_room->vnum )
   {
      ch->pcdata->build_vnum = ch->in_room->vnum;
      send_to_char( "Redit: Vnum changed to new room.\r\n", ch );
   }
   return;
}

/* -S- Addition: */
DO_FUN(build_commands)
{
   char buf[MSL];
   char out[MSL];
   int cmd;
   int col = 0;

   snprintf( out, MSL, "Building / Editing Commands Available to You:\r\n" );

   for( cmd = 0; build_cmd_table[cmd].name[0] != '\0'; cmd++ )
   {
      if( build_cmd_table[cmd].level > get_trust( ch ) )
         continue;
      snprintf( buf, MSL, "%-20s    ", build_cmd_table[cmd].name );
      strncat( out, buf, MSL-1 );
      if( ++col % 3 == 0 )
         strncat( out, "\r\n", MSL );
   }
   strncat( out, "\r\n", MSL );
   send_to_char( out, ch );
   return;
}



DO_FUN(build_showmob)
{
   char buf[MSL];
   char buf1[MSL];
   char arg1[MSL];
   char arg2[MSL];
   MOB_INDEX_DATA *pMob;
/*    RESET_DATA     *pReset; Unused var */
   SHOP_DATA *pShop;
   bool is_shopkeeper = 0;
   int iTrade;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );


   if( arg1[0] == '\0' )
   {
      send_to_char( "showmob <vnum>\r\n", ch );
      return;
   }

   if( !is_number( arg1 ) )
   {
      send_to_char( "must be a vnum.\r\n", ch );
      return;
   }

   if( ( pMob = get_mob_index( atoi( arg1 ) ) ) == NULL )
   {
      send_to_char( "Cannot find a mob with that vnum.\r\n", ch );
      return;
   }

   if( !build_canread( pMob->area, ch, 1 ) )
      return;

   buf1[0] = '\0';

   snprintf( buf, MSL, "@@WName: @@y%s.    @@WClass: @@y%s.\r\n", pMob->player_name, tab_mob_class[pMob->p_class].text );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "@@WVnum: @@y%d.  @@WSex: @@y%s.  @@WRace:@@y %s ",
            pMob->vnum,
            pMob->sex == SEX_MALE ? "male" :
            pMob->sex == SEX_FEMALE ? "female" : "neutral",
            ( ( pMob->race > -1 ) && ( pMob->race < MAX_RACE ) ) ? race_table[pMob->race].race_title : "Illegal Race" );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, " @@WLv: @@y%d.    @@WAlign: @@y%d.\r\n", pMob->level, pMob->alignment );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "@@WModifiers: AC: @@y%d.  @@WHitroll: @@y%d.  @@WDamroll: @@y%d.\r\n",
            pMob->ac_mod, pMob->hr_mod, pMob->dr_mod );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "@@WMob Flags:@@y\r\n%s", bs_show_values( tab_mob_act, pMob->act ) );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "@@WAffected by:@@y\r\n%s", show_values( tab_affected_by, pMob->affected_by, TRUE ) );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "@@WSkill Flags:@@y %s\r\n", bit_table_lookup( tab_mob_skill, pMob->skills ) );
   strncat( buf1, buf, MSL-1 );
   snprintf( buf, MSL, "@@WCast Flags:@@y %s\r\n", bit_table_lookup( tab_mob_cast, pMob->cast ) );
   strncat( buf1, buf, MSL-1 );
   snprintf( buf, MSL, "@@WDef Flags:@@y %s\r\n", bit_table_lookup( tab_mob_def, pMob->def ) );
   strncat( buf1, buf, MSL-1 );
   snprintf( buf, MSL, "@@WStrong Magic Realms:@@y %s\r\n", bit_table_lookup( tab_magic_realms, pMob->strong_magic ) );
   strncat( buf1, buf, MSL-1 );
   snprintf( buf, MSL, "@@WWeak Magic Realms:@@y %s\r\n", bit_table_lookup( tab_magic_realms, pMob->weak_magic ) );
   strncat( buf1, buf, MSL-1 );
   snprintf( buf, MSL, "@@WResist Magic Realms:@@y %s\r\n", bit_table_lookup( tab_magic_realms, pMob->resist ) );
   strncat( buf1, buf, MSL-1 );
   snprintf( buf, MSL, "@@WSuscept Magic Realms:@@y %s\r\n", bit_table_lookup( tab_magic_realms, pMob->suscept ) );
   strncat( buf1, buf, MSL-1 );
   snprintf( buf, MSL, "@@WRace Mods:@@y %s\r\n", bit_table_lookup( tab_mob_race_mods, pMob->race_mods ) );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "@@WShort description: @@y%s.\r\n@@WLong description: @@y%s\r\n",
            pMob->short_descr, pMob->long_descr[0] != '\0' ? pMob->long_descr : "(none)." );
   strncat( buf1, buf, MSL-1 );

   if( pMob->spec_fun != 0 )
   {
      snprintf( buf, MSL, "@@WMobile has spec fun: @@y%s\r\n", rev_spec_lookup( pMob->spec_fun ) );
      strncat( buf1, buf, MSL-1 );
   }

   if( pMob->script_name != &str_empty[0] )
   {
      snprintf( buf, MSL, "@@WMobile has Lua script: @@y%s\r\n", pMob->script_name );
      strncat( buf1, buf, MSL-1 );
   }

   if( ( pShop = pMob->pShop ) != 0 )
   {
      is_shopkeeper = 1;
      strncat( buf1, "@@WMobile is a shopkeeper, will buy @@y", MSL );
      for( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
      {
         if( pShop->buy_type[iTrade] > 0 )
         {
            strncat( buf1, tab_item_types[pShop->buy_type[iTrade] - 1].text, MSL-1 );
            strncat( buf1, " ", MSL );
         }
      }
      strncat( buf1, "\r\n", MSL );
      snprintf( buf, MSL, "@@WOpens at @@y%i @@Whrs, shuts at @@y%i @@Whours, profbuy:@@y%i, @@Wprofsell:@@y%i.\r\n",
               pShop->open_hour, pShop->close_hour, pShop->profit_buy, pShop->profit_sell );
      strncat( buf1, buf, MSL-1 );

   }
   strncat( buf1, "@@g", MSL );
   send_to_char( buf1, ch );
   return;
}

DO_FUN(build_showobj)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   AFFECT_DATA *paf;
   OBJ_INDEX_DATA *obj;
   int cnt;
   char *foo;
   int fubar;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Show what object?\r\n", ch );
      return;
   }

   if( !is_number( arg ) )
   {
      send_to_char( "must be a vnum.\r\n", ch );
      return;
   }

   buf1[0] = '\0';

   if( ( obj = get_obj_index( atoi( arg ) ) ) == NULL )
   {
      send_to_char( "Object vnum not found.\r\n", ch );
      return;
   }

   if( !build_canread( obj->area, ch, 1 ) )
      return;

   snprintf( buf, MSL, "@@WName: @@y%s  @@WLevel: @@y%d.\r\n", obj->name, obj->level );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "@@WVnum: @@y%d.  @@WType: @@y%s.\r\n", obj->vnum, tab_item_types[( obj->item_type ) - 1].text );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "@@WShort description: @@y%s.\r\n@@WLong description: @@y%s\r\n", obj->short_descr, obj->long_descr );
   strncat( buf1, buf, MSL-1 );

   /*
    * snprintf( buf, MSL, "@@WItem type: @@y%s.\r\n",rev_table_lookup(tab_item_types,obj->item_type));
    * strncat( buf1, buf, MSL);
    */

   snprintf( buf, MSL, "@@WWear bits:\r\n@@y%s\r\n@@WExtra bits: @@y%s\r\n",
            bs_show_values( tab_wear_flags, obj->wear_flags ), extra_bit_name( obj->extra_flags ) );
   /*
    * bit_table_lookup(tab_extra_flags, obj->extra_flags ) );
    * I think another bit_table_lookup is better!  Zen 
    */

   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "@@WItem_applies: @@y%s.\r\n", bit_table_lookup( tab_item_apply, obj->item_apply ) );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "@@WWeight: @@y%d.\r\n", obj->weight );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "@@WSpeed: @@y%4.2f\r\n", obj->speed );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "@@WDurability: @@y%d.\r\n", obj->max_durability );
   strncat( buf1, buf, MSL-1 );

   strncat( buf1, "@@WObject Values:\r\n", MSL );

   for( cnt = 0; cnt < 10; cnt++ )
   {
      snprintf( buf, MSL, "@@W[Value%d : @@y%6d@@W] %s",
               cnt, obj->value[cnt], rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) );
      strncat( buf1, buf, MSL-1 );
      if( is_name( "Spell", rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) ) )
      {
         fubar = obj->value[cnt];
         if( fubar < 0 || fubar > MAX_SKILL )
            snprintf( buf, MSL, "               @@R(INVALID!)@@g\r\n" );
         else
            snprintf( buf, MSL, "               @@y(%s)@@g\r\n", skill_table[fubar].name );

      }
      else if( is_name( "Liquid", rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) ) )
      {
         foo = str_dup( rev_table_lookup( tab_drink_types, obj->value[cnt] ) );
         if( foo[0] == '\0' )
            snprintf( buf, MSL, "                  @@R(INVALID!)@@g\r\n" );
         else
            snprintf( buf, MSL, "                  @@y(%s)@@g\r\n", foo );
      }
      else if( is_name( "Weapon", rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) ) )
      {
         foo = rev_table_lookup( tab_weapon_types, obj->value[cnt] );
         if( foo[0] == '\0' )
            snprintf( buf, MSL, "                  @@R(INVALID!)@@g\r\n" );
         else
            snprintf( buf, MSL, "                  @@y(%s)@@g\r\n", foo );
      }
      else if( is_name( "Charm", rev_table_lookup( tab_value_meanings, ( obj->item_type * 10 ) + cnt ) ) )
      {
         foo = rev_table_lookup( tab_charm_types, obj->value[cnt] );
         if( foo[0] == '\0' )
            snprintf( buf, MSL, "                  @@R(INVALID!)@@g\r\n" );
         else
            snprintf( buf, MSL, "                  @@y(%s)@@g\r\n", foo );
      }
      else
         snprintf( buf, MSL, "@@g\r\n" );
      strncat( buf1, buf, MSL-1 );
   }
   if( obj->obj_fun != NULL )
   {
      snprintf( buf, MSL, "@@WObject has objfun: @@y%s.@@g\r\n", rev_obj_fun_lookup( obj->obj_fun ) );
      strncat( buf1, buf, MSL-1 );
   }

   if( obj->script_name != &str_empty[0] )
   {
      snprintf( buf, MSL, "@@WObject has Lua script: @@y%s\r\n", obj->script_name );
      strncat( buf1, buf, MSL-1 );
   }

   if( obj->first_exdesc != NULL )
   {
      EXTRA_DESCR_DATA *ed;

      strncat( buf1, "@@WExtra description keywords: '@@y", MSL );

      for( ed = obj->first_exdesc; ed != NULL; ed = ed->next )
      {
         strncat( buf1, ed->keyword, MSL-1 );
         if( ed->next != NULL )
            strncat( buf1, " ", MSL );
      }

      strncat( buf1, "'@@g.\r\n", MSL );
   }

   for( paf = obj->first_apply; paf != NULL; paf = paf->next )
   {
      snprintf( buf, MSL, "@@WAffects @@y%s @@Wby @@y%d@@g.\r\n", affect_loc_name( paf->location ), paf->modifier );
      strncat( buf1, buf, MSL-1 );
   }

   send_to_char( buf1, ch );
   return;
}

#define DISPLAY_BRIEFDOORS  1
#define DISPLAY_RESETS      2
#define DISPLAY_FULLDOORS   4
#define DISPLAY_DESC        8

DO_FUN(build_showroom)
{
   char buf[MSL];
   char buf1[MSL];
   char arg1[MSL];
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
      strncat( buf1, "Can also use showroom brief/doors/resets/desc/all.\r\n", MSL );
   }
   else if( !is_name( arg1, "brief doors resets desc all" ) )
   {
      send_to_char( "Syntax: Showroom brief/doors/resets/desc/all.\r\n", ch );
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

   snprintf( buf, MSL, "@@WName: @@y%s.\r\n@@WArea: @@y%s.\r\n", location->name, location->area->name );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL,
            "@@WVnum: @@y%d.  @@WSector:\r\n @@y%s",
            location->vnum, show_values( tab_sector_types, location->sector_type, FALSE ) );
   strncat( buf1, buf, MSL-1 );

   snprintf( buf, MSL, "@@WFlags:\r\n@@y%s", bs_show_values( tab_room_flags, location->room_flags ) );
   strncat( buf1, buf, MSL-1 );

   if( location->script_name != &str_empty[0] )
   {
      snprintf( buf, MSL, "@@WRoom has Lua script: @@y%s\r\n", location->script_name );
      strncat( buf1, buf, MSL-1 );
   }

   if( ( display & DISPLAY_DESC ) )
   {
      snprintf( buf, MSL, "\r\n@@WDescr: @@y%s@@N\r\n", location->description[0] != '\0' ? location->description : "(none)." );
      strncat( buf1, buf, MSL-1 );
   }

   if( location->first_exdesc != NULL )
   {
      EXTRA_DESCR_DATA *ed;

      strncat( buf1, "@@WExtra description keywords:@@y '", MSL );
      for( ed = location->first_exdesc; ed; ed = ed->next )
      {
         strncat( buf1, ed->keyword, MSL-1 );
         if( ed->next != NULL )
            strncat( buf1, " ", MSL );
      }
      strncat( buf1, "'.\r\n", MSL );
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
               snprintf( buf, MSL, "@@WExit: @@y%7s @@W: To @@y%5i %s@@N\r\n", sDirs[door],
                        pexit->to_room != NULL ? pexit->to_room->vnum : 0,
                        pexit->to_room != NULL ? pexit->to_room->name : "" );
               strncat( buf1, buf, MSL-1 );
               snprintf( buf, MSL, "             @@WKey: @@y%5i %s@@N\r\n@@WExit Type:@@y %s@@N\r\n",
                        pKeyObj != NULL ? pKeyObj->vnum : 0,
                        pKeyObj != NULL ? pKeyObj->name : "None", bs_show_values( tab_door_types, pexit->exit_info ) );
               strncat( buf1, buf, MSL-1 );
               if( pexit->keyword != NULL && pexit->keyword[0] != '\0' )
               {
                  snprintf( buf, MSL, "@@WKeyword(s): @@y%s.  ", pexit->keyword );
                  strncat( buf1, buf, MSL-1 );
               }
               snprintf( buf, MSL, "@@WDesc: @@y%s", pexit->description[0] != '\0' ? pexit->description : "(none).\r\n" );
               strncat( buf1, buf, MSL-1 );
            }
            else
            {
               if( pexit->key > 0 )
                  strncat( buf1, "@@yK", MSL );
               /*
                * New way of showing doors... move to relevant place if it works 
                */

               snprintf( buf, MSL, "\r\n%s: (%s)  @@WTo: @@y(%i) %s\r\n@@WExit flags:@@N %s\r\n", sDirs[door],
                        pexit->keyword,
                        pexit->to_room != NULL ? pexit->to_room->vnum : 0,
                        pexit->to_room != NULL ? pexit->to_room->name : "Unknown",
                        exit_bit_name( pexit->exit_info ));
               strncat( buf1, buf, MSL-1 );
            }
         }
      }


   /*
    * Show resets for room
    */
   if( ( display & DISPLAY_RESETS ) && location->first_room_reset != NULL )
   {
      strncat( buf1, "@@WResets:\r\n", MSL );
      for( Pointer = location->first_room_reset; Pointer; )
         strncat( buf1, reset_to_text( &Pointer, NULL ), MSL-1 );
   }
   strncat( buf1, "@@g", MSL );
   send_to_char( buf1, ch );
   return;
}

DO_FUN(build_showresets)
{
   char buf1[MSL];
   ROOM_INDEX_DATA *location;
   BUILD_DATA_LIST *Pointer;
   int count;

   count = 0;
   buf1[0] = '\0';
   location = ch->in_room;

   if( !build_canread( location->area, ch, 1 ) )
      return;

   strncat( buf1, "Room Resets:\r\n", MSL );
   for( Pointer = location->first_room_reset; Pointer != NULL; )
      strncat( buf1, reset_to_text( &Pointer, &count ), MSL-1 );

   send_to_char( buf1, ch );
}

/* spec: rewrote to catch errors and count how many resets it handled */

char *reset_to_text( BUILD_DATA_LIST ** pList, int *pcount )
{
   char buf[MSL];
   static char buf1[MSL];
   MOB_INDEX_DATA *pMob;
   OBJ_INDEX_DATA *pObj, *to_obj;
   RESET_DATA *pReset;

   buf[0] = '\0';
   buf1[0] = '\0';
   pReset = (RESET_DATA *)( *pList )->data;

   strncat( buf1, build_docount( pcount ), MSL );
   if( ( *pList )->is_free )  /* sanity check */
      strncat( buf1, " **LIST IS FREE** ", MSL );  /* Bad Thing! */
   if( pReset->is_free )
      strncat( buf1, " **RESET IS FREE** ", MSL );

   ( *pList ) = ( *pList )->next;

   switch ( pReset->command )
   {
      default:
         snprintf( buf, MSL, "  stray '%c' reset: %d %d %d.\r\n", pReset->command, pReset->arg1, pReset->arg2, pReset->arg3 );
         strncat( buf1, buf, MSL-1 );
         break;
      case 'G':
         pObj = get_obj_index( pReset->arg1 );
         if( pObj )
            snprintf( buf, MSL, "  stray 'give' reset: object [%d] %s.\r\n", pReset->arg1, pObj->name );
         else
            snprintf( buf, MSL, "  stray 'give' reset: object %d (unknown).\r\n", pReset->arg1 );
         strncat( buf1, buf, MSL-1 );
         break;
      case 'E':
         pObj = get_obj_index( pReset->arg1 );
         if( pObj )
            snprintf( buf, MSL, "  stray 'equip' reset: object [%d] %s, on %s.\r\n",
                     pReset->arg1, pObj->name, tab_wear_loc[( pReset->arg3 )].text );
         else
            snprintf( buf, MSL, "  stray 'equip' reset: object [%d] (unknown), on %s.\r\n",
                     pReset->arg1, tab_wear_loc[( pReset->arg3 )].text );
         strncat( buf1, buf, MSL-1 );
         break;
      case 'A':  /* AutoMessage for room */
         snprintf( buf1, MSL, "Message: (%d-%d) %s\r\n", pReset->arg2, pReset->arg3, pReset->notes );
         strncat( buf1, buf, MSL-1 );
         break;
      case 'M':  /* Load mob */
         pMob = get_mob_index( pReset->arg1 );
         if( pMob != NULL )
            snprintf( buf, MSL, " [%d] %s (limit of %d).\r\n", pMob->vnum, pMob->player_name, pReset->arg2 );
         else
            snprintf( buf, MSL, " [%d] (unknown) (limit of %d).\r\n", pReset->arg1, pReset->arg2 );
         strncat( buf1, buf, MSL-1 );

         /*
          * scan for give and equip commands for this mob 
          */

         while( *pList )
         {
            pReset = (RESET_DATA *)( *pList )->data;

            if( pReset->command != 'G' && pReset->command != 'E' )
               break;

            ( *pList ) = ( *pList )->next;
            strncat( buf1, build_docount( pcount ), MSL );

            if( pReset->command == 'G' )
            {
               pObj = get_obj_index( pReset->arg1 );
               if( pObj != NULL )
               {
                  if( pMob->pShop != NULL )
                     strncat( buf1, "  sells ", MSL );
                  else
                     strncat( buf1, "  with ", MSL );
                  snprintf( buf, MSL, "[%d] %s.\r\n", pObj->vnum, pObj->name );
                  strncat( buf1, buf, MSL-1 );
               }
               else
               {
                  snprintf( buf, MSL, "[%d] unknown object in give reset!\r\n", pReset->arg1 );
                  strncat( buf1, buf, MSL-1 );
               }
            }
            else if( pReset->command == 'E' )
            {
               pObj = get_obj_index( pReset->arg1 );
               if( pObj != NULL )
                  snprintf( buf, MSL, "  equiped with [%d] %s, on %s.\r\n", pObj->vnum,
                           pObj->name, tab_wear_loc[( pReset->arg3 )].text );
               else
                  snprintf( buf, MSL, "[%d] unknown object equipped on %s.\r\n", pReset->arg1, tab_wear_loc[pReset->arg3].text );
               strncat( buf1, buf, MSL-1 );
            }
         }
         break;
      case 'O':  /* Load object to room */
         pObj = get_obj_index( pReset->arg1 );
         if( pObj != NULL )
            snprintf( buf, MSL, " [%d] %s no more than %d in room.\r\n", pObj->vnum, pObj->name, pReset->arg2 );
         else
            snprintf( buf, MSL, " [%d] unknown object reset!\r\n", pReset->arg1 );
         strncat( buf1, buf, MSL-1 );
         break;
      case 'P':
         pObj = get_obj_index( pReset->arg1 );
         to_obj = get_obj_index( pReset->arg3 );
         if( pObj )
         {
            if( to_obj )
               snprintf( buf, MSL, " object [%d] %s inside object [%d] %s. (limit %d)\r\n", pReset->arg1, pObj->name, pReset->arg3, to_obj->name, pReset->arg2 );
            else
               snprintf( buf, MSL, " object [%d] %s inside object [%d] (unknown). (limit %d)\r\n", pReset->arg1, pObj->name, pReset->arg3, pReset->arg2 );
         }
         else
            snprintf( buf, MSL, " object [%d] (unknown) inside object [%d] (unknown). (limit %d)\r\n", pReset->arg1, pReset->arg3, pReset->arg2 );
         strncat( buf1, buf, MSL-1 );
         break;
      case 'D':  /* close/lock doors */
         buf[0] = '\0';
         switch ( pReset->arg3 )
         {
            default:
               snprintf( buf, MSL, " Illegal door state %d for door %d\r\n", pReset->arg1, pReset->arg2 );
            case 0:
               snprintf( buf, MSL, " Open door %s.\r\n", sDirs[pReset->arg2] );
               break;
            case 1:
               snprintf( buf, MSL, " Close door %s.\r\n", sDirs[pReset->arg2] );
               break;
            case 2:
               snprintf( buf, MSL, " Close and lock door %s.\r\n", sDirs[pReset->arg2] );
               break;
         }
         strncat( buf1, buf, MSL-1 );
         break;
      case 'R':  /* randomise exits */
         snprintf( buf, MSL, " Randomize doors up to number %d.\r\n", pReset->arg2 );
         strncat( buf1, buf, MSL-1 );
         break;
   }

   return buf1;
}

char *build_docount( int *pcount )
{
   static char buf[20];

   buf[0] = '\0';
   if( pcount != NULL )
      snprintf( buf, 20, "%d) ", ++*pcount );
   else
      buf[0] = '\0';
   return buf;
}


DO_FUN(build_findmob)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   MOB_INDEX_DATA *pMobIndex;
   BUILD_DATA_LIST *Pointer;
   AREA_DATA *Area;
   int nMatch;
   bool fAll;
   bool found;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "find which mob?\r\n", ch );
      return;
   }

   if( !( Area = ch->in_room->area ) )
   {
      send_to_char( "Don't know what area you're in.\r\n", ch );
      return;
   }

   if( !build_canread( Area, ch, 1 ) )
      return;

   if( !( Pointer = Area->first_area_mobile ) )
   {
      send_to_char( "No mobiles in this area.\r\n", ch );
      return;
   }

   buf1[0] = '\0';
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   for( ; Pointer != NULL; Pointer = Pointer->next )
   {
      nMatch++;
      pMobIndex = (MOB_INDEX_DATA *)Pointer->data;
      if( fAll || is_name( arg, pMobIndex->player_name ) )
      {
         found = TRUE;
         snprintf( buf, MSL, "[%5d] %s\r\n", pMobIndex->vnum, capitalize( pMobIndex->short_descr ) );
         strncat( buf1, buf, MSL-1 );
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in this area.\r\n", ch );
      return;
   }

   send_to_char( buf1, ch );
   return;
}

DO_FUN(build_findmobroom)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   ROOM_INDEX_DATA *pRoom;
   RESET_DATA *pReset;
   AREA_DATA *Area;
   int vnum, found;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "find which mob vnum?\r\n", ch );
      return;
   }

   if( !is_number( arg ) )
   {
      send_to_char( "syntax: findmobroom vnum.\r\n", ch );
      return;
   }

   vnum = atoi( arg );
   found = 0;
   buf1[0] = '\0';

   if( !( Area = ch->in_room->area ) )
   {
      send_to_char( "Don't know what area you're in.\r\n", ch );
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
            snprintf( buf, MSL, "[%5d] %s\r\n", pRoom->vnum, capitalize( pRoom->name ) );
         else
            snprintf( buf, MSL, "[%5d] Unknown\r\n", pReset->arg3 );
         strncat( buf1, buf, MSL-1 );
      }
   }

   if( !found )
   {
      send_to_char( "Mob not found in any room.\r\n", ch );
      return;
   }

   send_to_char( buf1, ch );
   return;
}

DO_FUN(build_findobject)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   OBJ_INDEX_DATA *pObjIndex;
   BUILD_DATA_LIST *pList;
   AREA_DATA *Area;
   int nMatch;
   bool fAll;
   bool found;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Find what object?\r\n", ch );
      return;
   }

   if( !( Area = ch->in_room->area ) )
   {
      send_to_char( "Don't know what area you're in.\r\n", ch );
      return;
   }

   if( !build_canread( Area, ch, 1 ) )
      return;


   if( !( pList = Area->first_area_object ) )
   {
      send_to_char( "No objects in this area.\r\n", ch );
      return;
   }

   buf1[0] = '\0';
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   for( ; pList != NULL; pList = pList->next )
   {
      pObjIndex = (OBJ_INDEX_DATA *)pList->data;
      nMatch++;
      if( fAll || is_name( arg, pObjIndex->name ) )
      {
         found = TRUE;
         snprintf( buf, MSL, "[%5d] %s\r\n", pObjIndex->vnum, capitalize( pObjIndex->short_descr ) );
         strncat( buf1, buf, MSL-1 );
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in the area.\r\n", ch );
      return;
   }

   send_to_char( buf1, ch );
   return;
}

DO_FUN(build_findroom)
{
   char buf[MSL];
   char buf1[MSL];
   char arg[MSL];
   ROOM_INDEX_DATA *pRoomIndex;
   BUILD_DATA_LIST *pList;
   AREA_DATA *Area;
   int nMatch;
   bool fAll;
   bool found;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Find what room?\r\n", ch );
      return;
   }

   if( !( Area = ch->in_room->area ) )
   {
      send_to_char( "Don't know what area you're in.\r\n", ch );
      return;
   }

   if( !build_canread( Area, ch, 1 ) )
      return;


   if( !( pList = Area->first_area_room ) )
   {
      send_to_char( "No rooms in this area.\r\n", ch );
      return;
   }

   buf1[0] = '\0';
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   for( ; pList != NULL; pList = pList->next )
   {
      pRoomIndex = (ROOM_INDEX_DATA *)pList->data;
      nMatch++;
      if( fAll || is_name( arg, pRoomIndex->name ) )
      {
         found = TRUE;
         snprintf( buf, MSL, "[%5d] %s\r\n", pRoomIndex->vnum, capitalize( pRoomIndex->name ) );
         strncat( buf1, buf, MSL-1 );
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in the area.\r\n", ch );
      return;
   }

   send_to_char( buf1, ch );
   return;
}

/* -S- : More 'intelligent' way to handle things perhaps? */
DO_FUN(build_setmob)
{
   char arg1[MSL];
   char arg2[MSL];
   char arg3[MSL];
   char arg4[MSL];
   char buf[MSL];
/*    char buf2 [MSL]; unused */
/*    char buffer[MSL]; unused */
/*    char name[MSL]; unused */
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
      send_to_char( "Syntax: [set] <field>  <value>\r\n", ch );
      send_to_char( "or:     [set] <string> <value>\r\n", ch );
      send_to_char( "or:     [set] edit <string>\r\n", ch );
      send_to_char( "or:     [set] shop <shopspec> <value>\r\n", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "Field being one of:\r\n", ch );
      send_to_char( "  sex level align  aff act\r\n", ch );
      send_to_char( "  class clan position race\r\n", ch );
      send_to_char( "  skill cast def rmod\r\n", ch );
      send_to_char( "  hr_mod dr_mod ac_mod\r\n", ch );
      send_to_char( "String being one of:\r\n", ch );
      send_to_char( "  name short long desc spec script\r\n", ch );
      send_to_char( "Use [set] spec - to clear spec_fun\r\n", ch );
      send_to_char( "Use [set] script - to clear script\r\n", ch );
      send_to_char( "Shopspec being one of:\r\n", ch );
      send_to_char( "  trade0 - trade4 profbuy profsell openhour\r\n", ch );
      send_to_char( "  closehour clear\r\n", ch );
      return;
   }

   if( !is_number( arg1 ) )
   {
      send_to_char( "Must be a vnum for first arg.\r\n", ch );
      return;
   }

   if( ( pMob = get_mob_index( atoi( arg1 ) ) ) == NULL )
   {
      send_to_char( "Cannot find mob with that vnum.\r\n", ch );
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
      pMob->act.flip(lvalue);
      send_to_char( "OK.  Act Flag toggled.\r\n", ch );
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
      send_to_char( "OK.  Affected_by Flag toggled.\r\n", ch );
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "skills" ) )
   {
      lvalue = table_lookup( tab_mob_skill, arg3 );
      if( lvalue == 0 )
      {
         snprintf( buf, MSL, "You can toggle the following flags:\r\n" );
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
      send_to_char( "Ok.\r\n", ch );
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "cast" ) )
   {
      lvalue = table_lookup( tab_mob_cast, arg3 );
      if( lvalue == 0 )
      {
         snprintf( buf, MSL, "You can toggle the following flags:\r\n" );
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
      send_to_char( "Ok.\r\n", ch );
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "def" ) )
   {
      lvalue = table_lookup( tab_mob_def, arg3 );
      if( lvalue == 0 )
      {
         snprintf( buf, MSL, "You can toggle the following flags:\r\n" );
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
      send_to_char( "Ok.\r\n", ch );
      area_modified( pArea );
      return;
   }
   if( !str_cmp( arg2, "strong" ) )
   {
      lvalue = table_lookup( tab_magic_realms, arg3 );
      if( lvalue == 0 )
      {
         snprintf( buf, MSL, "You can toggle the following flags:\r\n" );
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
      send_to_char( "Ok.\r\n", ch );
      area_modified( pArea );
      return;
   }
   if( !str_cmp( arg2, "rmod" ) )
   {
      lvalue = table_lookup( tab_mob_race_mods, arg3 );
      if( lvalue == 0 )
      {
         snprintf( buf, MSL, "You can toggle the following flags:\r\n" );
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
      send_to_char( "Ok.\r\n", ch );
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "weak" ) )
   {
      lvalue = table_lookup( tab_magic_realms, arg3 );
      if( lvalue == 0 )
      {
         snprintf( buf, MSL, "You can toggle the following flags:\r\n" );
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
      send_to_char( "Ok.\r\n", ch );
      area_modified( pArea );
      return;
   }
   if( !str_cmp( arg2, "resist" ) )
   {
      lvalue = table_lookup( tab_magic_realms, arg3 );
      if( lvalue == 0 )
      {
         snprintf( buf, MSL, "You can toggle the following flags:\r\n" );
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
      send_to_char( "Ok.\r\n", ch );
      area_modified( pArea );
      return;
   }
   if( !str_cmp( arg2, "suscept" ) )
   {
      lvalue = table_lookup( tab_magic_realms, arg3 );
      if( lvalue == 0 )
      {
         snprintf( buf, MSL, "You can toggle the following flags:\r\n" );
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
      send_to_char( "Ok.\r\n", ch );
      area_modified( pArea );
      return;
   }
   if( !str_cmp( arg2, "rmod" ) )
   {
      lvalue = table_lookup( tab_mob_race_mods, arg3 );
      if( lvalue == 0 )
      {
         snprintf( buf, MSL, "You can toggle the following flags:\r\n" );
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
      send_to_char( "Ok.\r\n", ch );
      area_modified( pArea );
      return;
   }
   if( !str_cmp( arg2, "class" ) )
   {
      lvalue = table_lookup( tab_mob_class, arg3 );
      if( lvalue == -1 )
      {
         snprintf( buf, MSL, "You can use the following classes:\r\n" );
         table_printout( tab_mob_class, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      pMob->p_class = lvalue;
      send_to_char( "Ok.\r\n", ch );
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
         snprintf( buf, MSL, "You can use the following races:\r\n" );
         for( looper = 0; looper < MAX_RACE; looper++ )
         {
            snprintf( catbuf, MSL, "[%i]:%s ", looper, race_table[looper].race_title );
            strncat( buf, catbuf, MSL-1 );
         }
         strncat( buf, "\r\n", MSL );
         send_to_char( buf, ch );
         return;
      }
      pMob->race = lvalue;
      send_to_char( "Ok.\r\n", ch );
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
         send_to_char( "position must be standing, resting or sleeping.\r\n", ch );
         return;
      }
      pMob->position = value;
      area_modified( pArea );
      send_to_char( "Ok.\r\n", ch );
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
         send_to_char( "Must be male, female or neutral.\r\n", ch );
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
         send_to_char( "Level range is 1 to 140.\r\n", ch );
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
         send_to_char( "Alignment range is -1000 to 1000.\r\n", ch );
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
         send_to_char( "ac_mod range is -2500 to 500.\r\n", ch );
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
         send_to_char( "hr_mod range is -10 to 1000.\r\n", ch );
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
         send_to_char( "dr_mod range is -10 to 1000.\r\n", ch );
         return;
      }
      pMob->dr_mod = value;
      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "edit" ) )
   {
      if( is_name( arg3, "desc name short long" ) )
         snprintf( buf, MSL, "%s %s $edit", arg1, arg3 );
      else
         snprintf( buf, MSL, " " );
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
      build_strdup( &pMob->long_descr, arg3, TRUE, FALSE, ch );
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
      if( arg3[0] == '-' )
      {
         if( pMob->spec_fun == NULL )
            return;

         pMob->spec_fun = NULL;
         area_modified( pArea );
         return;
      }

      if( ( pMob->spec_fun = spec_lookup( arg3 ) ) == 0 )
      {
         snprintf( buf, MSL, "Valid spec funs are :\r\n" );
         print_spec_lookup( buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }

      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "script" ) )
   {
      if( arg3[0] == '-' )
      {
       free_string(pMob->script_name);
       pMob->script_name = &str_empty[0];
       area_modified( pArea );
       return;
      }

      if( strlen(arg3) >= 3 )
      {
       pMob->script_name = str_dup(arg3);
       area_modified( pArea );
       return;
      }
      else
      {
       send_to_char("Script must be at least 3 characters.\r\n",ch);
       return;
      }
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
            for( pList = pArea->first_area_shop; pList != NULL; pList = pList->next )
            {
               if( pShop == pList->data )
                  break;
            }

            if( pList != NULL )
            {
               UNLINK( pList, pArea->first_area_shop, pArea->last_area_shop, next, prev );
               build_dat_list.remove(pList);
               delete pList;
            }

            pMob->pShop = NULL;  /* Take away link from mob */
            shop_list.remove(pShop);
            delete pShop; /* Free the memory it occupies */
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
         pShop = new SHOP_DATA;
         if( pShop == NULL )  /* Couldn't alloc space. */
         {
            send_to_char( "Out of memory, please reboot ASAP.\r\n", ch );
            return;
         }
         pList = new BUILD_DATA_LIST;
         if( pList == NULL )
         {
            send_to_char( "Out of memory, please reboot ASAP.\r\n", ch );
            return;
         }

         pShop->keeper = pMob->vnum;
         pMob->pShop = pShop; /* Add link to mob */
         pList->data = pShop; /* Add to area list */
         LINK( pList, pArea->first_area_shop, pArea->last_area_shop, next, prev );

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
            snprintf( buf, MSL, "trade must be between 0 and %i\r\n", MAX_TRADE );
            send_to_char( buf, ch );
            return;
         }
         value = table_lookup( tab_item_types, arg4 );
         if( value == 0 )
         {
            snprintf( buf, MSL, "Item type must be one of the following:\r\n" );
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
            send_to_char( "Profbuy must be a number more than 0.\r\n", ch );
            return;
         }
         if( value < 100 )
         {
            send_to_char( "WARNING: Profbuy should be over 100.\r\n", ch );
            return;
         }

         pShop->profit_buy = value;
         return;
      }

      if( !str_cmp( "profsell", arg3 ) )
      {
         if( value < 0 )
         {
            send_to_char( "Profsell must be a number more than 0.\r\n", ch );
            return;
         }
         if( value > 100 )
         {
            send_to_char( "WARNING: Profsell should be over 100.\r\n", ch );
            return;
         }
         pShop->profit_sell = value;
         return;
      }

      if( !str_cmp( "openhour", arg3 ) )
      {
         if( value < 0 || value > 24 )
         {
            send_to_char( "Openhour must be a number between 0 and 24.\r\n", ch );
            return;
         }

         pShop->open_hour = value;
         return;
      }

      if( !str_cmp( "closehour", arg3 ) )
      {
         if( value < 0 || value > 24 )
         {
            send_to_char( "closehour must be a number between 0 and 24.\r\n", ch );
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
      pReset = (RESET_DATA *)pList->data;

      if( pReset->command == 'D' && pReset->arg2 == door )
      {
         /*
          * nuke this reset
          */
         UNLINK( pList, pRoomIndex->first_room_reset, pRoomIndex->last_room_reset, next, prev );
         build_dat_list.remove(pList);
         delete pList;

         UNLINK( pReset, pArea->first_reset, pArea->last_reset, next, prev );
         delete pReset;
      }
   }
}

DO_FUN(build_setroom)
{
   char arg1[MSL];
   char arg2[MSL];
   char arg3[MSL];
   char arg4[MSL];
   char arg5[MSL];
   char buf[MSL];
   char *argn;
   ROOM_INDEX_DATA *location;
   int value, num;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   strcpy( arg2, argument );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax: [set] <arguments> \r\n", ch );
      send_to_char( "    Or: [set] <string> value\r\n", ch );
      send_to_char( "    Or: [set] edit <string>\r\n", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "Arguments being one of:\r\n", ch );
      send_to_char( "      flags  +/-<ascii-flag> \r\n", ch );
      send_to_char( "      sector <ascii-sector-type>\r\n", ch );
      send_to_char( "      door <door-number> to <vnum> [onesided]\r\n", ch );
      send_to_char( "                         keyword <string>\r\n", ch );
      send_to_char( "                         desc <string>\r\n", ch );
      send_to_char( "                         key <vnum> [onesided]\r\n", ch );
      send_to_char( "                         type <ascii-type> [onesided]\r\n", ch );
      send_to_char( "                         clear [onesided]\r\n", ch );
      send_to_char( "      ed <keyword> <string>\r\n", ch );
      send_to_char( "String being one of:\r\n", ch );
      send_to_char( "      name desc script\r\n", ch );
      send_to_char( "Use [set] ed - to clear extra keyword\r\n", ch );
      send_to_char( "Use [set] script - to clear script\r\n", ch );
      return;
   }

   location = get_room_index( ch->pcdata->build_vnum );

   if( location == NULL )
   {
      send_to_char( "Invalid vnum selected!\r\n", ch );
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
      location->room_flags.flip(value);
      snprintf( buf, MSL, "@@WRoom flag @@y%s@@W toggled to @@y%s@@W.@@g\r\n",
               rev_table_lookup( tab_room_flags, value ), location->room_flags.test(value) ? "ON" : "OFF" );
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
      snprintf( buf, MSL, "@@WSector type set to @@y%s@@W.@@g\r\n", rev_table_lookup( tab_sector_types, value ) );
      send_to_char( buf, ch );
      return;
   }
   if( !str_cmp( arg1, "edit" ) && is_name( arg2, "name desc" ) )
   {
      snprintf( buf, MSL, "%s $edit", arg2 );
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

   if( !str_cmp( arg1, "ed" ) )
   {

      EXTRA_DESCR_DATA *ed;
      bool found;

      argument = one_argument( argument, arg2 );
      strcpy( arg3, argument );

      if( strlen(arg2) < 1 || strlen(arg3) < 1 )
      {
        send_to_char("Extra descrs must have a keyword and description.\r\n",ch );
        return;
      }

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
            send_to_char( "Keyword not found.\r\n", ch );
            return;
         }

         /*
          * Delete description 
          */
         UNLINK( ed, location->first_exdesc, location->last_exdesc, next, prev );
         exdesc_list.remove(ed);
         delete ed;

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
         ed = new EXTRA_DESCR_DATA;

         build_strdup( &ed->keyword, arg2, FALSE, FALSE, ch );
         build_strdup( &ed->description, arg3, FALSE, FALSE, ch );
         LINK( ed, location->first_exdesc, location->last_exdesc, next, prev );
         return;
      }

      build_strdup( &ed->description, arg3, TRUE, FALSE, ch );
      return;

   }

   if( !str_cmp( arg1, "script" ) )
   {
      if( arg2[0] == '-' )
      {
       delete location->lua;
       free_string(location->script_name);
       location->script_name = &str_empty[0];
       return;
      }

      if( strlen(arg2) >= 3 )
      {
       location->lua = new LUA_DATA;
       init_lua(location);
       location->script_name = str_dup(arg2);
       return;
      }
      else
      {
       send_to_char("Script must be at least 3 characters.\r\n",ch);
       return;
      }
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
         send_to_char( "Door number must be a number between 0 and 5, or NSEWUD\r\n", ch );
         return;
      }

      pExit = location->exit[door];

      if( !str_cmp( arg3, "clear" ) )
      {
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
         exit_list.remove(pExit);
         delete pExit;
         location->exit[door] = NULL;

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
                  exit_list.remove(pExit);
                  delete pExit;
                  pDestRoom->exit[RevDirs[door]] = NULL;

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
         pExit = new EXIT_DATA;
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
            send_to_char( "Room number must be a vnum\r\n", ch );
            return;
         }
         value = atoi( arg4 );
         if( !( pRoom = get_room_index( value ) ) )
         {
            send_to_char( "No such vnum.\r\n", ch );
            return;
         }

         if( !build_canwrite( pRoom->area, ch, 0 ) )
         {
            send_to_char( "You cannot connect to this room.\r\n", ch );
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
               exit_list.remove(pDestExit);
               delete pDestExit;
               pDestRoom->exit[RevDirs[door]] = NULL;

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
               pDestExit = new EXIT_DATA;
               pDestExit->to_room = location;
               pDestExit->vnum = location->vnum;
               build_strdup( &pDestExit->keyword, pExit->keyword, FALSE, FALSE, ch );
               pDestExit->exit_info = pExit->exit_info;
               pDestExit->key = pExit->vnum;
               pRoom->exit[RevDirs[door]] = pDestExit;
               area_modified( pRoom->area );
            }

            if( pDestExit->to_room != location )
            {
               snprintf( buf, MSL, "The door in room %i going %s points to room [%i] %s.\r\n",
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
            send_to_char( "Object number must be a vnum, or blank.\r\n", ch );
            return;
         }
         if( !( pObject = get_obj_index( atoi( arg4 ) ) ) )
         {
            send_to_char( "No such vnum.\r\n", ch );
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
            snprintf( buf, MSL, "Possible door types are +/-:\r\n" );
            table_printout( tab_door_types, buf + strlen( buf ) );
            send_to_char( buf, ch );
            return;
         }

         if( value == EX_CLOSED || value == EX_LOCKED )
         {
            send_to_char( "To CLOSE or LOCK a door, use a reset.\r\n", ch );
            return;
         }


         if( num == 1 )
         {
            if( pDestExit && str_cmp( arg5, "onesided" ) )
            {
               pDestExit->exit_info.set(value);
               area_modified( pDestRoom->area );
            }
            pExit->exit_info.set(value);
         }
         else
         {
            if( pDestExit && str_cmp( arg5, "onesided" ) )
            {
               pDestExit->exit_info.reset(value);
               if( value == EX_ISDOOR )
                  nuke_exit_resets( pDestRoom, RevDirs[door] );
               area_modified( pDestRoom->area );
            }
            pExit->exit_info.reset(value);
            if( value == EX_ISDOOR )
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


DO_FUN(build_setobject)
{
   char arg1[MSL];
   char arg2[MSL];
   char arg3[MSL];
   char buf[MSL];
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
      send_to_char( "Syntax: [set] <field>  <value>\r\n", ch );
      send_to_char( "or:     [set] <string> <value>\r\n", ch );
      send_to_char( "or:     aobj [<slot>]\r\n", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "Field being one of:\r\n", ch );
      send_to_char( "  value0 value1 value2 value3 speed magic\r\n", ch );
      send_to_char( "  level extra wear weight aff type durability\r\n", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "String being one of:\r\n", ch );
      send_to_char( "  name short long ed objfun script\r\n", ch );
      send_to_char( "Use [set] objfun - to clear objfun.\r\n", ch );
      send_to_char( "Use [set] script - to clear script\r\n", ch );
      return;
   }

   if( ( pObj = get_obj_index( atoi( arg1 ) ) ) == NULL )
   {
      send_to_char( "Vnum not found.\r\n", ch );
      return;
   }

   pArea = pObj->area;
   if( !build_canwrite( pArea, ch, 1 ) )
      return;

   /*
    * Check for extra flag: clan_eq 
    */
   if( IS_OBJ_STAT(pObj,ITEM_EXTRA_CLAN_EQ) && get_trust( ch ) != 85 )
   {
      send_to_char( "Only a Creator can set Clan-Eq.\r\n", ch );
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
    float ac = 0, dr = 0, hp = 0, hr = 0, mp = 0, mv = 0, svs = 0;

    if( pObj->level < 1 ) /* Should never happen anyhow, but sanity checks are never bad */
    {
     send_to_char("You can only auto-object items that are at least level 1.\r\n",ch);
     return;
    }

    mult = (float)pObj->level / 120.0000;

    if( IS_OBJ_STAT(pObj,ITEM_EXTRA_RARE) )
    {
     mult += 0.15; /* Rares are 15% better by default */
     send_to_char("Applying 15% bonus for rare flag.\r\n",ch);
    }

    ac = sysdata.build_obj_ac * mult;
    dr = sysdata.build_obj_dr * mult;
    hp = sysdata.build_obj_hp * mult;
    hr = sysdata.build_obj_hr * mult;
    mp = sysdata.build_obj_mp * mult;
    mv = sysdata.build_obj_mv * mult;
    svs = sysdata.build_obj_svs * mult;

    if( arg3[0] != '\0' ) /* Use modifiers based on wear slot */
    {
     short i = 0;
     bool found = FALSE;

     for( i = 0; tab_auto_obj[i].name != NULL; i++ )
     {
      if( !str_prefix(arg3,"list") )
      {
       snprintf(buf,MSL,"Valid object types:");
       for( i = 0; tab_auto_obj[i].name != NULL; i++ )
       {
        char tmp[MSL];
        snprintf(tmp,MSL," %s",tab_auto_obj[i].name);
        strncat(buf,tmp,MSL-1);
       }
       strncat(buf,".\r\n",MSL);
       send_to_char(buf,ch);
       return;
      }
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
      snprintf(buf,MSL,"Values modified based on object type: %s.\r\n",tab_auto_obj[i].name);
      send_to_char(buf,ch);
     }
     else
     {
      snprintf(buf,MSL,"Invalid object type. Valid object types:");
      for( i = 0; tab_auto_obj[i].name != NULL; i++ )
      {
       char tmp[MSL];
       snprintf(tmp,MSL," %s",tab_auto_obj[i].name);
       strncat(buf,tmp,MSL-1);
      }
      strncat(buf,".\r\n",MSL);
      send_to_char(buf,ch);
     }
    }

    snprintf(buf,MSL,"%d aff %sac %.0f",pObj->vnum,ac == 0 ? "-" : "",ac);
    build_setobject(ch,buf);
    snprintf(buf,MSL,"%d aff %sdamroll %.0f",pObj->vnum,dr == 0 ? "-" : "",dr);
    build_setobject(ch,buf);
    snprintf(buf,MSL,"%d aff %shit %.0f",pObj->vnum,hp == 0 ? "-" : "",hp);
    build_setobject(ch,buf);
    snprintf(buf,MSL,"%d aff %shitroll %.0f",pObj->vnum,hr == 0 ? "-" : "",hr);
    build_setobject(ch,buf);
    snprintf(buf,MSL,"%d aff %smana %.0f",pObj->vnum,mp == 0 ? "-" : "",mp);
    build_setobject(ch,buf);
    snprintf(buf,MSL,"%d aff %smove %.0f",pObj->vnum,mv == 0 ? "-" : "",mv);
    build_setobject(ch,buf);
    snprintf(buf,MSL,"%d aff %ssaving_spell %.0f",pObj->vnum,svs == 0 ? "-" : "",svs);
    build_setobject(ch,buf);

    snprintf(buf,MSL,"Auto-object complete based on item level %d. Stats are %0.2f%% of max.\r\n",pObj->level,(mult * 100));
    send_to_char(buf,ch);

    return;
   }

   if( !str_prefix( "value", arg2 ) )
   {
      num = arg2[5] - '0';
      if( num < 0 || num > 9 )
      {
         send_to_char( "Value0-9.\r\n", ch );
         return;
      }
      pObj->value[num] = value;
      return;
   }
   if( !str_cmp( arg2, "level" ) )
   {
      if( value < 1 || value > 120 )
      {
         send_to_char( "item level is 1 to 120.\r\n", ch );
         return;
      }
      if( pObj->item_type == ITEM_WEAPON )
      {
       pObj->value[1] = number_range((value - 10),value);
       pObj->value[2] = number_range((value - 5),(value +5));
       if( pObj->value[1] < 1 )
        pObj->value[1] = 1;
       if( pObj->value[2] < 1 )
        pObj->value[2] = 1;
      }

      pObj->max_durability = value * 5;
      pObj->durability = pObj->max_durability;
      pObj->level = value;
      return;
   }

   if( !str_cmp( arg2, "speed" ) )
   {
      if( atof(arg3) < 0.01 || atof(arg3) > 4 )
      {
         send_to_char("item speed is 0.01 to 4.00.\r\n", ch );
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
         send_to_char( "v0-v9.\r\n", ch );
         return;
      }
      pObj->value[num] = value;
      return;
   }

   if( !str_cmp( arg2, "objfun" ) )
   {
      if( arg3[0] == '-' )
      {
         if( pObj->obj_fun == NULL )
            return;

         pObj->obj_fun = NULL;
         area_modified( pArea );

         return;
      }

      if( ( pObj->obj_fun = obj_fun_lookup( arg3 ) ) == 0 )
      {
         snprintf( buf, MSL, "Valid obj funs are :\r\n" );
         print_obj_fun_lookup( buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }

      area_modified( pArea );
      return;
   }

   if( !str_cmp( arg2, "script" ) )
   {
      if( arg3[0] == '-' )
      {
       free_string(pObj->script_name);
       pObj->script_name = &str_empty[0];
       area_modified( pArea );
       return;
      }

      if( strlen(arg3) >= 3 )
      {
       pObj->script_name = str_dup(arg3);
       area_modified( pArea );
       return;
      }
      else
      {
       send_to_char("Script must be at least 3 characters.\r\n",ch);
       return;
      }
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
         snprintf( buf, MSL, "Values for extra flags are +/- :\r\n" );
         table_printout( tab_obj_flags, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      if( num == 1 )
         pObj->extra_flags.set(value);
      else
         pObj->extra_flags.reset(value);
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
         snprintf( buf, MSL, "Values for magical applies are +/- :\r\n" );
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
         snprintf( buf, MSL, "Values for wear flags are +/- :\r\n" );
         table_printout( tab_wear_flags, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }
      if( num == 1 )
         pObj->wear_flags.set(value);
      else
         pObj->wear_flags.reset(value);

      return;

   }

   if( !str_cmp( arg2, "type" ) )
   {
      value = table_lookup( tab_item_types, arg3 );
      if( value == 0 )
      {
         snprintf( buf, MSL, "Values for item types are :\r\n" );
         table_printout( tab_item_types, buf + strlen( buf ) );
         send_to_char( buf, ch );
         return;
      }

      if( value == ITEM_EXTRA_CLAN_EQ && get_trust( ch ) != 85 )
      {
         send_to_char( "Only a CREATOR may set this flag.\r\n", ch );
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
      build_strdup( &pObj->long_descr, arg3, TRUE, FALSE, ch );
      return;
   }

   if( !str_cmp( arg2, "ed" ) )
   {
      EXTRA_DESCR_DATA *ed;
      int found;

      argument = one_argument( argument, arg3 );

      if( strlen(argument) < 1 || strlen(arg3) < 1 )
      {
        send_to_char("Extra descrs must have a keyword and description.\r\n",ch );
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
            send_to_char( "Keyword not found.\r\n", ch );
            return;
         }

         UNLINK( ed, pObj->first_exdesc, pObj->last_exdesc, next, prev );
         exdesc_list.remove(ed);
         delete ed;

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


      ed = new EXTRA_DESCR_DATA;
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
         send_to_char( "Syntax: setobject <object> aff [-]<location> <mod>\r\n", ch );
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
         snprintf( buf, MSL, "Location can be one of the following [-]:\r\n" );
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
            send_to_char( "Location not found.\r\n", ch ); */
            return;
         }

         UNLINK( paf, pObj->first_apply, pObj->last_apply, next, prev );
         delete paf;
         return;
      }


      if( !found )
      {
         paf = new AFFECT_DATA;
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
     send_to_char("Durability must be a number.\r\n",ch);
     return;
    }
    if( atoi(arg3) < 2 )
    {
     send_to_char("Value must be at least 2. An object with only 1 durability is considered 'broken' and un-usable.\r\n",ch);
     return;
    }
    pObj->max_durability = atoi(arg3);
    pObj->durability = pObj->max_durability;
    return;
   }

   /*
    * Generate usage message.
    */
   build_setobject( ch, "" );
   return;
}



DO_FUN(build_dig)
{
   char arg1[MSL];
   char arg2[MSL];
   char arg3[MSL];
   char buffer[MSL];
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
      send_to_char( "Syntax: dig <dir> <vnum> [onesided]\r\n", ch );
      return;
   }

   if( !( temp = index( cDirs, UPPER( arg1[0] ) ) ) )
   {
      send_to_char( "Direction must be one of NSEWUP\r\n", ch );
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
      send_to_char( "Vnum must be between 0 and 16777216.\r\n", ch );
      return;
   }

   if( get_room_index( vnum ) != NULL )
   {
      send_to_char( "There is already a room with that vnum.\r\n", ch );
      return;
   }

   pCurRoom = ch->in_room;
   pArea = pCurRoom->area;

   if( !build_canwrite( pArea, ch, 1 ) )
      return;

   if( vnum < pArea->min_vnum || vnum > pArea->max_vnum )
   {
      snprintf( buffer, MSL,  "Vnum must be between %i and %i.\r\n", pArea->min_vnum, pArea->max_vnum );
      send_to_char( buffer, ch );
      return;
   }

   /*
    * Create room 
    */
   pRoomIndex = new ROOM_INDEX_DATA;
   pRoomIndex->area = pArea;
   pRoomIndex->vnum = vnum;
   pRoomIndex->sector_type = ch->in_room->sector_type;

   /*
    * Add room to hash table 
    */

   iHash = vnum % MAX_KEY_HASH;
   SING_TOPLINK( pRoomIndex, room_index_hash[iHash], next );

   /*
    * Add room into area list. 
    */
   pList = new BUILD_DATA_LIST;
   pList->data = pRoomIndex;
   LINK( pList, pCurRoom->area->first_area_room, pCurRoom->area->last_area_room, next, prev );

   /*
    * Create door 
    */
   pExit = new EXIT_DATA;
   pExit->to_room = pRoomIndex;
   pExit->vnum = vnum;
   pCurRoom->exit[dir] = pExit;

   if( str_cmp( arg3, "onesided" ) )   /* If NOT onesided */
   {
      pExit = new EXIT_DATA;
      pExit->to_room = pCurRoom;
      pExit->vnum = pCurRoom->vnum;
      pRoomIndex->exit[RevDirs[dir]] = pExit;
   }

   area_modified(pArea);
   return;
}


DO_FUN(build_stop)
{
   ch->position = POS_STANDING;
   send_to_char( "Building stopped.\r\n", ch );
}

DO_FUN(do_build)
{
   if( IS_NPC(ch) ) /* Just...no. --Kline */
    return;

   if( !ch->act.test(ACT_BUILDER) )
   {
      send_to_char( "You aren't allowed to build!\r\n", ch );
      return;
   }

   ch->position = POS_BUILDING;
   do_help( ch, "build_bmotd" ); /* motd for builders -S- */
   send_to_char( "Building commands are now operative. Type stop to stop building.\r\n", ch );
}

DO_FUN(build_addmob)
{
   char arg1[MSL];
   char arg2[MSL];
   char buffer[MSL];
   MOB_INDEX_DATA *pMobIndex;
/*    ROOM_INDEX_DATA *room; unused */
   AREA_DATA *pArea;
   BUILD_DATA_LIST *pList;
   int vnum;
   int iHash;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   strcpy( arg2, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: addmob <vnum> <name>\r\n", ch );
      return;
   }

   vnum = is_number( arg1 ) ? atoi( arg1 ) : -1;

   if( vnum < 0 || vnum > MAX_VNUM )
   {
      send_to_char( "Vnum must be between 0 and 16777216.\r\n", ch );
      return;
   }

   if( get_mob_index( vnum ) != NULL )
   {
      send_to_char( "There is already a mob with that vnum.\r\n", ch );
      return;
   }

   pArea = ch->in_room->area; /* bugger.  this might need fixing!! */

   if( !build_canwrite( pArea, ch, 1 ) )
      return;

   if( vnum < pArea->min_vnum || vnum > pArea->max_vnum )
   {
      snprintf( buffer, MSL,  "Vnum must be between %i and %i.\r\n", pArea->min_vnum, pArea->max_vnum );
      send_to_char( buffer, ch );
      return;
   }

   ch->pcdata->build_vnum = vnum;
   ch->pcdata->build_mode = BUILD_MODE_MEDIT;


   pMobIndex = new MOB_INDEX_DATA;
   pMobIndex->vnum = vnum;
   pMobIndex->area = pArea;
   pMobIndex->player_name = str_dup( arg2 );

   iHash = vnum % MAX_KEY_HASH;
   SING_TOPLINK( pMobIndex, mob_index_hash[iHash], next );
   pList = new BUILD_DATA_LIST;
   pList->data = pMobIndex;
   LINK( pList, pArea->first_area_mobile, pArea->last_area_mobile, next, prev );
   kill_table[URANGE( 0, pMobIndex->level, MAX_LEVEL - 1 )].number++;

   return;
}


DO_FUN(build_addobject)
{
   char arg1[MSL];
   char arg2[MSL];
   char buffer[MSL];
   OBJ_INDEX_DATA *pObjIndex;
   AREA_DATA *pArea;
   BUILD_DATA_LIST *pList;
   int vnum;
   int iHash;
   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   strcpy( arg2, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: addobject <vnum> <name>\r\n", ch );
      return;
   }

   vnum = is_number( arg1 ) ? atoi( arg1 ) : -1;

   if( vnum < 0 || vnum > MAX_VNUM )
   {
      send_to_char( "Vnum must be between 0 and 16777216.\r\n", ch );
      return;
   }

   if( get_obj_index( vnum ) != NULL )
   {
      send_to_char( "There is already an object with that vnum.\r\n", ch );
      return;
   }


   pArea = ch->in_room->area;

   if( !build_canwrite( pArea, ch, 1 ) )
      return;

   if( vnum < pArea->min_vnum || vnum > pArea->max_vnum )
   {
      snprintf( buffer, MSL,  "Vnum must be between %i and %i.\r\n", pArea->min_vnum, pArea->max_vnum );
      send_to_char( buffer, ch );
      return;
   }

   ch->pcdata->build_vnum = vnum;
   ch->pcdata->build_mode = BUILD_MODE_OEDIT;

   pObjIndex = new OBJ_INDEX_DATA;
   pObjIndex->vnum = vnum;
   pObjIndex->area = pArea;
   pObjIndex->name = str_dup(arg2);

   iHash = vnum % MAX_KEY_HASH;
   SING_TOPLINK( pObjIndex, obj_index_hash[iHash], next );
   pList = new BUILD_DATA_LIST;
   pList->data = pObjIndex;
   LINK( pList, pArea->first_area_object, pArea->last_area_object, next, prev );

   return;
}

DO_FUN(build_addroom)
{
 ROOM_INDEX_DATA *pRoomIndex;
 AREA_DATA *pArea;
 BUILD_DATA_LIST *pList;
 int vnum = 0, iHash = 0;
 ROOM_INDEX_DATA *pCurRoom;

 smash_tilde( argument );

 if( argument[0] == '\0' )
 {
    send_to_char( "Syntax: addroom <vnum>\r\n", ch );
    return;
 }

 vnum = is_number( argument ) ? atoi( argument ) : -1;

 if( vnum < 0 || vnum > MAX_VNUM )
 {
    send_to_char( "Vnum must be between 0 and 16777216.\r\n", ch );
    return;
 }

 if( get_room_index( vnum ) != NULL )
 {
    send_to_char( "There is already a room with that vnum.\r\n", ch );
    return;
 }

 pCurRoom = ch->in_room;
 pArea = pCurRoom->area;

 if( !build_canwrite( pArea, ch, 1 ) )
    return;

 if( vnum < pArea->min_vnum || vnum > pArea->max_vnum )
 {
    snprintf( argument, MSL,  "Vnum must be between %i and %i.\r\n", pArea->min_vnum, pArea->max_vnum );
    send_to_char( argument, ch );
    return;
 }

 /*
  * Create room
  */
 pRoomIndex = new ROOM_INDEX_DATA;
 pRoomIndex->area = pArea;
 pRoomIndex->vnum = vnum;
 pRoomIndex->sector_type = ch->in_room->sector_type;

 ch->pcdata->build_vnum = vnum;
 ch->pcdata->build_mode = BUILD_MODE_REDIT;

 /*
  * Add room to hash table
  */

 iHash = vnum % MAX_KEY_HASH;
 SING_TOPLINK( pRoomIndex, room_index_hash[iHash], next );

 /*
  * Add room into area list.
  */
 pList = new BUILD_DATA_LIST;
 pList->data = pRoomIndex;
 LINK( pList, pCurRoom->area->first_area_room, pCurRoom->area->last_area_room, next, prev );
}

DO_FUN(build_addreset)
{
   RESET_DATA *pReset;
   RESET_DATA *pMobReset;
   RESET_DATA *pObjReset;
   BUILD_DATA_LIST *pList;
   BUILD_DATA_LIST *pMobList;
   BUILD_DATA_LIST *pObjList;
   char arg1[MSL];
   char arg2[MSL];
   char arg3[MSL];
   char arg4[MSL];
   char buf[MSL];
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
      send_to_char( "addreset mob     <vnum> <limit>\r\n", ch );
      send_to_char( "         obj     <vnum> <max number in room>\r\n", ch );
      send_to_char( "         equip   <n.mob-vnum> <obj-vnum> <location>\r\n", ch );
      send_to_char( "         give    <n.mob-vnum> <obj-vnum>\r\n", ch );
      send_to_char( "         within  <n.obj-vnum> <obj-vnum> <max num>\r\n", ch);
      send_to_char( "         door    <dir> <state>\r\n", ch );
      send_to_char( "         rand    <num-dirs>\r\n", ch );
      send_to_char( "         message <min_lev> <max_lev> <text>\r\n", ch );
      return;
   }

   if( !is_name( arg1, "mob obj equip message give within door rand" ) )
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
         snprintf( buf, MSL, "Vnum must be between 0 and %i.\r\n", MAX_VNUM );
         send_to_char( buf, ch );
         return;
      }
      if( !( pMob = get_mob_index( vnum ) ) )
      {
         send_to_char( "Mob not found.\r\n", ch );
         return;
      }

      if( !build_canread( pMob->area, ch, 0 ) )
      {
         send_to_char( "You cannot use that mob.\r\n", ch );
         return;
      }

      num = is_number( arg3 ) ? atoi( arg3 ) : -1;
      if( num < 0 )
      {
         send_to_char( "Limit must be a number more than 0.\r\n", ch );
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
         send_to_char( "Must specify TWO levels: min and max before message.\r\n", ch );
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
         snprintf( buf, MSL, "Vnum must be between 0 and %i.\r\n", MAX_VNUM );
         send_to_char( buf, ch );
         return;
      }
      if( !( pObj = get_obj_index( vnum ) ) )
      {
         send_to_char( "Object not found.\r\n", ch );
         return;
      }

      if( !build_canread( pObj->area, ch, 0 ) )
      {
         send_to_char( "You cannot use that object.\r\n", ch );
         return;
      }


      if( IS_OBJ_STAT(pObj,ITEM_EXTRA_CLAN_EQ) && !IS_IMMORTAL( ch ) )
      {
         send_to_char( "You can't use that object for a reset.\r\n", ch );
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
         send_to_char( "Count portion of vnum cannot be 0. Indicate which instance of the mob, or -1 for all instances.\r\n",
                       ch );
         return;
      }
      vnum = is_number( buf ) ? atoi( buf ) : -1;

      found = num;
      for( pMobList = pRoomIndex->first_room_reset; pMobList != NULL; pMobList = pMobList->next )
      {
         pMobReset = (RESET_DATA *)pMobList->data;
         if( pMobReset->command == 'M' && pMobReset->arg1 == vnum )
         {
            found--;
            if( found <= 0 )
               break;
         }
      }

      if( found == num )
      {
         send_to_char( "None of that mob loaded in the room resets.\r\n", ch );
         return;
      }
      if( num > 0 && found > 0 )
      {
         send_to_char( "Not enough of that mob loaded in the room resets.\r\n", ch );
         return;
      }

      vnum = is_number( arg3 ) ? atoi( arg3 ) : -1;
      if( !( pObj = get_obj_index( vnum ) ) )
      {
         send_to_char( "Object not found.\r\n", ch );
         return;
      }


      if( IS_OBJ_STAT(pObj,ITEM_EXTRA_CLAN_EQ) && !IS_IMMORTAL( ch ) )
      {
         send_to_char( "You can't use that object for a reset.\r\n", ch );
         return;
      }


      if( !build_canread( pObj->area, ch, 0 ) )
      {
         send_to_char( "You cannot use that object.\r\n", ch );
         return;
      }

      rarg1 = vnum;

      num = table_lookup( tab_wear_loc, arg4 );
      if( num == -2 )
      {
         snprintf( buf, MSL, "Location must be one of the following:\r\n" );
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
         send_to_char( "Count portion of vnum cannot be 0. Indicate which instance of the mob, or -1 for all instances.\r\n",
                       ch );
         return;
      }
      vnum = is_number( buf ) ? atoi( buf ) : -1;

      found = num;
      for( pMobList = pRoomIndex->first_room_reset; pMobList != NULL; pMobList = pMobList->next )
      {
         pMobReset = (RESET_DATA *)pMobList->data;
         if( pMobReset->command == 'M' && pMobReset->arg1 == vnum )
         {
            found--;
            if( found <= 0 )
               break;
         }
      }

      if( found == num )
      {
         send_to_char( "None of that mob loaded in the room resets.\r\n", ch );
         return;
      }
      if( num > 0 && found > 0 )
      {
         send_to_char( "Not enough of that mob loaded in the room resets.\r\n", ch );
         return;
      }

      vnum = is_number( arg3 ) ? atoi( arg3 ) : -1;
      if( !( pObj = get_obj_index( vnum ) ) )
      {
         send_to_char( "Object not found.\r\n", ch );
         return;
      }


      if( IS_OBJ_STAT(pObj,ITEM_EXTRA_CLAN_EQ) && !IS_IMMORTAL( ch ) )
      {
         send_to_char( "You can't use that object for a reset.\r\n", ch );
         return;
      }

      if( !build_canread( pObj->area, ch, 0 ) )
      {
         send_to_char( "You cannot use that object.\r\n", ch );
         return;
      }

      rarg1 = vnum;
      rarg2 = 0;
      rarg3 = 0;
      command = 'G';
   }

   if( !str_cmp( arg1, "within" ) )
   {
      int vnum1;

      num = number_argument( arg2, buf );
      if( num == 0 )
      {
         send_to_char( "Count portion of vnum cannot be 0. Indicate which instance of the obj, or -1 for all instances.\r\n",
                       ch );
         return;
      }
      vnum = is_number( buf ) ? atoi( buf ) : -1;

      found = num;
      for( pObjList = pRoomIndex->first_room_reset; pObjList != NULL; pObjList = pObjList->next )
      {
         pObjReset = (RESET_DATA *)pObjList->data;
         if( pObjReset->command == 'O' && pObjReset->arg1 == vnum )
         {
            found--;
            if( found <= 0 )
               break;
         }
      }

      if( found == num )
      {
         send_to_char( "None of that obj loaded in the room resets.\r\n", ch );
         return;
      }
      if( num > 0 && found > 0 )
      {
         send_to_char( "Not enough of that obj loaded in the room resets.\r\n", ch );
         return;
      }

      vnum = is_number( arg3 ) ? atoi( arg3 ) : -1;
      if( !( pObj = get_obj_index( vnum ) ) )
      {
         send_to_char( "Object not found.\r\n", ch );
         return;
      }

      if( IS_OBJ_STAT(pObj,ITEM_EXTRA_CLAN_EQ) && !IS_IMMORTAL( ch ) )
      {
         send_to_char( "You can't use that object for a reset.\r\n", ch );
         return;
      }

      vnum1 = is_number( arg2 ) ? atoi( arg2 ) : -1;
      if( !( pObj = get_obj_index( vnum1 ) ) )
      {
         send_to_char( "Object not found.\r\n", ch );
         return;
      }

      if( IS_OBJ_STAT(pObj,ITEM_EXTRA_CLAN_EQ) && !IS_IMMORTAL( ch ) )
      {
         send_to_char( "You can't use that object for a reset.\r\n", ch );
         return;
      }

      if( !build_canread( pObj->area, ch, 0 ) )
      {
         send_to_char( "You cannot use that object.\r\n", ch );
         return;
      }

      rarg1 = vnum;
      rarg2 = is_number( arg4 ) ? atoi ( arg4 ) : 1;
      rarg3 = vnum1;
      command = 'P';
   }

   if( !str_cmp( arg1, "door" ) )
   {
      temp = index( cDirs, UPPER( arg2[0] ) );
      if( !temp )
      {
         send_to_char( "Direction must be one of NSEWUP.\r\n", ch );
         return;
      }

      rarg1 = pRoomIndex->vnum;
      rarg2 = temp - cDirs;

      if( ( pExit = pRoomIndex->exit[rarg2] ) == NULL || !pExit->exit_info.test(EX_ISDOOR) )
      {
         send_to_char( "That exit isn't a door.\r\n", ch );
         return;
      }

      num = table_lookup( tab_door_states, arg3 );
      if( num < 0 )
      {
         snprintf( buf, MSL, "Door state can be one of the following:\r\n" );
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
         send_to_char( "Number of doors must be from 1 to 6.\r\n", ch );
         return;
      }
      rarg1 = pRoomIndex->vnum;
      rarg2 = num;
      command = 'R';
   }

   if( command == '*' )
   {
      bug( "build_addreset: Shouldn't have come this far.\r\n", 0 );
      build_addreset( ch, "" );
      return;
   }

   /*
    * Now create new pReset 
    */
   pReset = new RESET_DATA;
   pReset->command = command;
   pReset->arg1 = rarg1;
   pReset->arg2 = rarg2;
   pReset->arg3 = rarg3;
   if( command == 'A' )
      pReset->notes = rauto;

   pList = new BUILD_DATA_LIST;
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

DO_FUN(build_delreset)
{
   RESET_DATA *pReset;
   BUILD_DATA_LIST *pList;
   BUILD_DATA_LIST *pNextList;
   char arg1[MSL];
   ROOM_INDEX_DATA *pRoomIndex;

   int found;
   AREA_DATA *pArea;


   smash_tilde( argument );
   argument = one_argument( argument, arg1 );

   if( arg1[0] == '\0' || !is_number( arg1 ) )
   {
      send_to_char( "delreset <reset-number>  (type showresets for numbers)\r\n", ch );
      return;
   }

   found = atoi( arg1 );

   if( found <= 0 )
   {
      send_to_char( "Reset number must be more than 0.\r\n", ch );
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
      send_to_char( "Could not find reset.\r\n", ch );
      return;
   }

   pReset = (RESET_DATA *)pList->data;

   if( !pReset )
   {
      bugf( "delreset: pList returned null in search for reset number %d", atoi( arg1 ) );
      send_to_char( "bug: Could not do deletion.\r\n", ch );
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
      build_dat_list.remove(pList);
      delete pList;

      UNLINK( pReset, pArea->first_reset, pArea->last_reset, next, prev );
      delete pReset;

      /*
       * free any give/equip resets following it 
       */
      for( pList = pNextList; pList; pList = pNextList )
      {
         pNextList = pList->next;

         if( pReset->command != 'E' && pReset->command != 'G' )
            break;

         UNLINK( pList, pRoomIndex->first_room_reset, pRoomIndex->last_room_reset, next, prev );
         build_dat_list.remove(pList);
         delete pList;

         UNLINK( pReset, pArea->first_reset, pArea->last_reset, next, prev );
         delete pReset;
      }
   }
   else
   {
      /*
       * Just get rid of the one reset
       */
      pNextList = pList->next;

      UNLINK( pList, pRoomIndex->first_room_reset, pRoomIndex->last_room_reset, next, prev );
      build_dat_list.remove(pList);
      delete pList;

      UNLINK( pReset, pArea->first_reset, pArea->last_reset, next, prev );
      delete pReset;
   }

   send_to_char( "Done.\r\n", ch );
   return;
}


DO_FUN(build_delwarn)
{
   send_to_char( "You must spell out delroom, delobject, delmobile, or delhelp in full.\r\n", ch );
   return;
}

DO_FUN(build_delhelp)
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
  send_to_char("Syntax: delhelp <mort/imm> <keyword> ok.\r\n",ch);
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

 snprintf(arg1,MSL,"%s%s/%s.%s",HELP_DIR,initial(arg2),arg2,mort ? HELP_MORT : HELP_IMM);
 if( (fp = file_open(arg1,"r")) == NULL )
 {
  send_to_char("Couldn't find that keyword.\r\n",ch);
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
   send_to_char("Done.\r\n",ch);
   return;
  }
 }
 else
 {
  send_to_char("An error has occured. Contact a shell owner.\r\n",ch);
  return;
 }

 return;
}

DO_FUN(build_delroom)
{
   char arg1[MSL];
   char arg2[MSL];
   char buf[MSL];
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
      send_to_char( "Syntax: delroom <vnum>\r\n", ch );
      return;
   }

   if( is_number( arg1 ) )
   {
      vnum = atoi( arg1 );
      if( !( pRoomIndex = get_room_index( vnum ) ) )
      {
         send_to_char( "Vnum not found.\r\n", ch );
         return;
      }

      if( !build_canwrite( pRoomIndex->area, ch, 1 ) )
         return;

      if( ( pRoomIndex->first_person ) != NULL )
      {
         send_to_char( "You cannot delete the room while people are in it!.\r\n", ch );
         return;
      }
      old_vnum = vnum;

      snprintf( buf, MSL, "Are you sure you want to delete room [%d] %s?\r\n", vnum, pRoomIndex->name );
      strncat( buf, "Type delroom ok if you are sure.\r\n", MSL );
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
      send_to_char( "First specify a room number.\r\n", ch );
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
      send_to_char( "You cannot delete the room while people are in it!.\r\n", ch );
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
         pSrchRoom = (ROOM_INDEX_DATA *)pList->data;

         for( door = 0; door < MAX_DIR; door++ )
         {
            if( ( pExit = pSrchRoom->exit[door] ) != NULL && ( pExit->to_room == pRoomIndex || pSrchRoom == pRoomIndex ) )
            {
               /*
                * Get rid of exit. 
                */
               exit_list.remove(pExit);
               delete pExit;
               pSrchRoom->exit[door] = NULL;

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
      build_dat_list.remove(pList);
      delete pList;
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
         build_dat_list.remove(pList);
         delete pList;
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
            reset_list.remove(pReset);
            delete pReset;
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
            exit_list.remove(pExit);
            delete pExit;
            pRoomIndex->exit[door] = NULL;

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
         exdesc_list.remove(pEd);
         delete pEd;
      }
   }
   room_index_list.remove(pRoomIndex);
   delete pRoomIndex;

   send_to_char( "Done.\r\n", ch );
   return;
}


int old_ovnum = 0;

DO_FUN(build_delobject)
{
   char arg1[MSL];
   char arg2[MSL];
   char buf[MSL];
   CHAR_DATA *vch;
   OBJ_INDEX_DATA *pObjIndex;
   AREA_DATA *pArea;
   BUILD_DATA_LIST *pList;
   list<CHAR_DATA *>::iterator li;
   int vnum;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   strcpy( arg2, argument );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax: delobject <vnum>\r\n", ch );
      return;
   }

   if( is_number( arg1 ) )
   {
      vnum = atoi( arg1 );
      if( !( pObjIndex = get_obj_index( vnum ) ) )
      {
         send_to_char( "Vnum not found.\r\n", ch );
      }

      if( !build_canwrite( pObjIndex->area, ch, 1 ) )
         return;

      old_ovnum = vnum;

      snprintf( buf, MSL, "Are you sure you want to delete object: [%d] %s?\r\n", vnum, pObjIndex->name );
      strncat( buf, "Type delobject ok if you are sure.\r\n", MSL );
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
      send_to_char( "First specify a object number.\r\n", ch );
      return;
   }

   /*
    * make sure that NO one else has pcdata->build_vnum set to this room!! 
    */
   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      vch = *li;
      if( vch->pcdata->build_vnum == old_ovnum )
         vch->pcdata->build_vnum = 0;
   }


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
      build_dat_list.remove(pList);
      delete pList;
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
            reset_list.remove(pReset);
            delete pReset;
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
         exdesc_list.remove(pEd);
         delete pEd;
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
         delete paf;
      }
   }

   /*
    * Now delete structure
    */
   obj_index_list.remove(pObjIndex);
   delete pObjIndex;

   send_to_char( "Done.\r\n", ch );
   return;
}


int old_mob_vnum = 0;

DO_FUN(build_delmob)
{
   char arg1[MSL];
   char arg2[MSL];
   char buf[MSL];
   CHAR_DATA *vch;
   MOB_INDEX_DATA *pMobIndex;
   AREA_DATA *pArea;
   BUILD_DATA_LIST *pList;
   list<CHAR_DATA *>::iterator li;
   int vnum;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   strcpy( arg2, argument );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax: delmobile <vnum>\r\n", ch );
      return;
   }

   if( is_number( arg1 ) )
   {
      vnum = atoi( arg1 );
      if( !( pMobIndex = get_mob_index( vnum ) ) )
      {
         send_to_char( "Vnum not found.\r\n", ch );
      }

      if( !build_canwrite( pMobIndex->area, ch, 1 ) )
         return;

      old_mob_vnum = vnum;

      snprintf( buf, MSL, "Are you sure you want to delete mobile: [%d] %s?\r\n", vnum, pMobIndex->player_name );
      strncat( buf, "Type delmobile ok if you are sure.\r\n", MSL );
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
      send_to_char( "First specify a mobile number.\r\n", ch );
      return;
   }

   /*
    * make sure that NO one else has pcdata->build_vnum set to this mob!! 
    */
   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      vch = *li;
      if( vch->pcdata->build_vnum == old_ovnum )
         vch->pcdata->build_vnum = 0;
   }


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
      build_dat_list.remove(pList);
      delete pList;
   }



   /*
    * Get rid of mobile from world 
    */
   {
      CHAR_DATA *wch;

      for( li = char_list.begin(); li != char_list.end(); li++ )
      {
         wch = *li;
         if( wch->npcdata->pIndexData == pMobIndex )
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
            reset_list.remove(pReset);
            delete pReset;
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
      build_dat_list.remove(pList);
      delete pList;

      /*
       * Now free shop structure
       */
      shop_list.remove(pShop);
      delete pShop;
   }

   /*
    * Now delete structure
    */
   mob_index_list.remove(pMobIndex);
   delete pMobIndex;

   send_to_char( "Done.\r\n", ch );
   return;
}

DO_FUN(build_help)
{
   char buf[MSL];

   if( argument[0] != '\0' )  /* If an argument supplied... */
   {
      snprintf( buf, MSL, "build_%s", argument );  /* Format text to send */
      do_help( ch, buf );  /* Try and find BUILD_<helpname> in helps */
      return;
   }

   do_help( ch, BHELP_INDEX );
   return;
}

DO_FUN(build_forcereset)
{
   AREA_DATA *pArea;

   pArea = ch->in_room->area;

   if( !build_canwrite( pArea, ch, 0 ) )
   {
      send_to_char( "You cannot reset this area.\r\n", ch );
      return;
   }

   reset_area( pArea );
   build_save_flush(  );

   send_to_char( "Done.\r\n", ch );
   return;
}

char *build_simpstrdup( char *buf )
{
   char *rvalue;

   build_strdup( &rvalue, buf, FALSE, FALSE, NULL );
   return rvalue;
}

#define STRING_FILE_DIR "temp/"

/* New build strdup to handle string objects as we move away from SSM --Kline */
void build_strdup( string dest, char *src, bool newline, CHAR_DATA *ch )
{
 if( src[0] == '$' ) /* Special functions */
 {
  src++;

  if( src[0] != '$' )
  {
   if( is_name(src,"edit new clear") && ch != NULL )
   {
    if( !str_cmp(src,"clear") )
    {
     dest.clear();
     return;
    }
   }
  }
 }

 return;
}

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
         strncat( filename, STRING_FILE_DIR, 254 );
         strncat( filename, src, 254 );
         infile = file_open( filename, "r" );
         if( !infile )
            filechar = str_dup( "Could not open file.\r\n" );
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

   send_to_char( "Editing string. Type .help for help.\r\n", ch );
   write_start( dest, (RET_FUN *)build_finishedstr, orig, ch );

   if( *dest != &str_empty[0] )
   {
      send_to_char( src, ch );
      strncat( *dest, src, MSL );   /* Add src string into the buffer */
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


DO_FUN(build_set_oedit)
{
   OBJ_INDEX_DATA *obj;

   ch->pcdata->build_mode = BUILD_MODE_OEDIT;
   if( is_number( argument ) )
   {
      ch->pcdata->build_vnum = atoi( argument );
      if( ( obj = get_obj_index( ch->pcdata->build_vnum ) ) == NULL )
         ch->pcdata->build_vnum = 0;
   }
   else
      ch->pcdata->build_vnum = 0;

   send_to_char( ch->pcdata->build_vnum < 1 ? "No vnum set.  Use setvnum.\r\n" : "Vnum now set. ", ch );
   return;
}

DO_FUN(build_set_redit)
{
   ch->pcdata->build_vnum = ch->in_room->vnum;

   ch->pcdata->build_mode = BUILD_MODE_REDIT;

   send_to_char( "Now in Redit mode.  Vnum set to current room.\r\n", ch );
   return;
}

DO_FUN(build_set_medit)
{
   MOB_INDEX_DATA *mob;

   ch->pcdata->build_mode = BUILD_MODE_MEDIT;
   if( is_number( argument ) )
   {
      ch->pcdata->build_vnum = atoi( argument );
      if( ( mob = get_mob_index( ch->pcdata->build_vnum ) ) == NULL )
         ch->pcdata->build_vnum = 0;
   }
   else
      ch->pcdata->build_vnum = 0;

   send_to_char( ch->pcdata->build_vnum < 1 ? "No vnum set.  Use setvnum.\r\n" : "Vnum now set. ", ch );
   return;
}

DO_FUN(build_set_nedit)
{
   ch->pcdata->build_mode = BUILD_MODE_NONE;
   ch->pcdata->build_vnum = 0;

   send_to_char( "You are no longer in ANY editing mode.\r\n", ch );
   return;
}




DO_FUN(build_setvnum)
{
   char buf[MSL];
   char buf2[MSL];
   int vnum;
   OBJ_INDEX_DATA *obj;
/*   ROOM_INDEX_DATA *room; unused */
   MOB_INDEX_DATA *mob;
   bool found;
   short inc = 0;


   if( argument[0] == '\0' )
   {
      send_to_char( "USAGE: setvnum <vnum>, or v n(dec)/m(inc)\r\n", ch );
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
         send_to_char( "Argument must be numeric. [vnum]\r\n", ch );
         return;
      }
   }
   if( is_number( argument ) )
      vnum = atoi( argument );
   else
   {

      vnum = ch->pcdata->build_vnum + inc;
   }

   snprintf( buf, MSL, "Current vnum now set to: %d.\r\n", vnum );
   found = TRUE;

   switch ( ch->pcdata->build_mode )
   {
      case BUILD_MODE_OEDIT:
         if( ( obj = get_obj_index( vnum ) ) == NULL )
         {
            snprintf( buf2, MSL, "No object with that vnum exists.  Use addobject first.\r\n" );
            found = FALSE;
         }
         else
            snprintf( buf2, MSL, "Object exists: %s\r\n", obj->short_descr );

         break;

      case BUILD_MODE_REDIT:
         /*
          * do_goto( ch, itoa( vnum ) );   
          */
         ch->pcdata->build_vnum = ch->in_room->vnum;
         send_to_char( "Vnum set to current room vnum.\r\n", ch );
         break;

      case BUILD_MODE_MEDIT:
         if( ( mob = get_mob_index( vnum ) ) == NULL )
         {
            snprintf( buf2, MSL, "No mobile with that vnum exists.  Use addmob first.\r\n" );
            found = FALSE;
         }
         else
            snprintf( buf2, MSL, "Mobile exists: %s\r\n", mob->short_descr );

         break;

      default:
         snprintf( buf2, MSL, "Please set your editing mode first!!\r\n" );

   }

   send_to_char( buf, ch );
   send_to_char( buf2, ch );

   if( found )
   {
      ch->pcdata->build_vnum = vnum;
      send_to_char( "New vnum IS set.\r\n", ch );
      build_list( ch, "" );
   }
   else
   {
      send_to_char( "New vnum NOT set - still at old value.\r\n", ch );
   }

   return;
}

DO_FUN(build_list)
{
   /*
    * do show obj|mob|room according to ch->pcdata->build_mode -S-
    */

   char buf[MSL];
   bool found;

   found = FALSE;

   if( argument[0] == '\0' || ( ch->pcdata->build_mode == BUILD_MODE_REDIT && is_name( argument, "brief doors resets desc all" ) ) )
   {
      switch ( ch->pcdata->build_mode )
      {
         case BUILD_MODE_NONE:
            send_to_char( "Not in any editing mode.  Nothing to show!\r\n", ch );
            break;
         case BUILD_MODE_OEDIT:
            if( ch->pcdata->build_vnum < 1 )
               send_to_char( "No vnum has been selected!\r\n", ch );
            else
            {
               snprintf( buf, MSL, "%d", ch->pcdata->build_vnum );
               build_showobj( ch, buf );
            }
            break;
         case BUILD_MODE_REDIT:
            if( ch->pcdata->build_vnum < 1 )
               send_to_char( "No vnum has been selected!\r\n", ch );
            else
            {
               build_showroom( ch, argument );
            }
            break;
         case BUILD_MODE_MEDIT:
            if( ch->pcdata->build_vnum < 1 )
               send_to_char( "No vnum has been selected!\r\n", ch );
            else
            {
               snprintf( buf, MSL, "%d", ch->pcdata->build_vnum );
               build_showmob( ch, buf );
            }
            break;
      }
      return;
   }


   if( ch->pcdata->build_mode == BUILD_MODE_NONE )
   {
      send_to_char( "You must be in an editing mode first!\r\n", ch );
      return;
   }

   /*
    * Ok, now arg is valid.  See if it applic. to edit mode
    */

   if( !strcmp( argument, "flags" ) )
   {
      found = TRUE;
      switch ( ch->pcdata->build_mode )
      {
         case BUILD_MODE_REDIT:
            snprintf( buf, MSL, "Valid room flags are :\r\n" );
            wide_table_printout( tab_room_flags, buf + strlen( buf ) );
            send_to_char( buf, ch );
            break;
         case BUILD_MODE_MEDIT:
            snprintf( buf, MSL, "Valid mob flags are :\r\n" );
            wide_table_printout( tab_mob_act, buf + strlen( buf ) );
            send_to_char( buf, ch );
            break;
         case BUILD_MODE_OEDIT:
            snprintf( buf, MSL, "Valid object flags are :\r\n" );
            wide_table_printout( tab_obj_flags, buf + strlen( buf ) );
            send_to_char( buf, ch );
            break;
      }
   }

   if( !strcmp( argument, "aff" ) )
   {
      switch ( ch->pcdata->build_mode )
      {
         case BUILD_MODE_OEDIT:
            snprintf( buf, MSL, "Valid object affects are :\r\n" );
            wide_table_printout( tab_obj_aff, buf + strlen( buf ) );
            send_to_char( buf, ch );
            found = TRUE;
            break;
         case BUILD_MODE_MEDIT:
            snprintf( buf, MSL, "Valid mob affects are :\r\n" );
            wide_table_printout( tab_affected_by, buf + strlen( buf ) );
            send_to_char( buf, ch );
            found = TRUE;
            break;
         default:
            send_to_char( "Only valid when in Oedit or Medit modes.\r\n", ch );
      }
   }

   if( !strcmp( argument, "types" ) )
   {
      if( ch->pcdata->build_mode != BUILD_MODE_OEDIT )
      {
         send_to_char( "Only valid when in Oedit mode.\r\n", ch );
         return;
      }
      found = TRUE;
      snprintf( buf, MSL, "Valid object types are :\r\n" );
      wide_table_printout( tab_item_types, buf + strlen( buf ) );
      send_to_char( buf, ch );
   }

   if( !strcmp( argument, "wear" ) )
   {
      if( ch->pcdata->build_mode != BUILD_MODE_OEDIT )
      {
         send_to_char( "Only valid when in Oedit mode.\r\n", ch );
         return;
      }
      found = TRUE;
      snprintf( buf, MSL, "Valid object wear flags are :\r\n" );
      wide_table_printout( tab_wear_flags, buf + strlen( buf ) );
      send_to_char( buf, ch );
   }

   if( !strcmp( argument, "loc" ) )
   {
      if( ch->pcdata->build_mode != BUILD_MODE_OEDIT )
      {
         send_to_char( "Only valid when in Oedit mode.\r\n", ch );
         return;
      }
      found = TRUE;
      snprintf( buf, MSL, "Valid object wear locations are :\r\n" );
      wide_table_printout( tab_wear_loc, buf + strlen( buf ) );
      send_to_char( buf, ch );
   }

   if( !strcmp( argument, "sec" ) )
   {
      if( ch->pcdata->build_mode != BUILD_MODE_REDIT )
      {
         send_to_char( "Only valid when in Redit mode.\r\n", ch );
         return;
      }
      found = TRUE;
      snprintf( buf, MSL, "Valid room sector types are :\r\n" );
      wide_table_printout( tab_sector_types, buf + strlen( buf ) );
      send_to_char( buf, ch );
   }

   if( !strcmp( argument, "exit" ) )
   {
      if( ch->pcdata->build_mode != BUILD_MODE_REDIT )
      {
         send_to_char( "Only valid when in Redit mode.", ch );
         return;
      }
      found = TRUE;
      snprintf( buf, MSL, "Valid room door types are :\r\n" );
      wide_table_printout( tab_door_types, buf + strlen( buf ) );
      send_to_char( buf, ch );
      snprintf( buf, MSL, "Valid room door states are :\r\n" );
      wide_table_printout( tab_door_states, buf + strlen( buf ) );
      send_to_char( buf, ch );
   }


   if( !found )
   {
      send_to_char( "You may display the following values:\r\n\r\n", ch );
      send_to_char( "Edit Mode:      Values:\r\n----------      -------\r\n", ch );
      send_to_char( "  Redit         FLAGS - room flags.\r\n", ch );
      send_to_char( "                SEC   - room sector types.\r\n", ch );
      send_to_char( "                EXIT  - door states / types.\r\n", ch );
      send_to_char( "  Medit         FLAGS - mob flags.\r\n", ch );
      send_to_char( "                AFF   - mob affected_by values.\r\n", ch );
      send_to_char( "  Oedit         TYPES - object types.\r\n", ch );
      send_to_char( "                FLAGS - object flags.\r\n", ch );
      send_to_char( "                WEAR  - object wear flags.\r\n", ch );
      send_to_char( "                LOC   - object wear locations.\r\n", ch );
      send_to_char( "                AFF   - object affected_by values.\r\n", ch );
      return;
   }
   return;
}

DO_FUN(build_set)
{
   /*
    * Call setroom/mob/obj with argument, and vnum, etc.
    */
   char buf[MSL];

   switch ( ch->pcdata->build_mode )
   {
      case BUILD_MODE_OEDIT:
         if( ch->pcdata->build_vnum < 1 )
         {
            send_to_char( "No vnum is set!!\r\n", ch );
            return;
         }
         snprintf( buf, MSL, "%d %s", ch->pcdata->build_vnum, argument );
         build_setobject( ch, buf );
         break;
      case BUILD_MODE_REDIT:
         if( ch->pcdata->build_vnum < 1 )
         {
            send_to_char( "No vnum is set!!\r\n", ch );
            return;
         }
         build_setroom( ch, argument );
         break;
      case BUILD_MODE_MEDIT:
         if( ch->pcdata->build_vnum < 1 )
         {
            send_to_char( "No vnum is set!!\r\n", ch );
            return;
         }
         snprintf( buf, MSL, "%d %s", ch->pcdata->build_vnum, argument );
         build_setmob( ch, buf );
         break;
      default:
         send_to_char( "You are not in any editing mode!\r\n", ch );
   }
   return;
}

DO_FUN(build_listvalues)
{
   /*
    * Lookup what the 4 values mean for the given object type -S-
    */
   int value;
   int foo;
   char buf[MSL];


   if( argument[0] == '\0' )
   {
      send_to_char( "USAGE: values <item-type-name>\r\n", ch );
      send_to_char( "Eg: values staff\r\n", ch );
      return;
   }

   value = table_lookup( tab_item_types, argument );
   if( value == 0 )
   {
      snprintf( buf, MSL, "Valid object types are :\r\n" );
      wide_table_printout( tab_item_types, buf + strlen( buf ) );
      send_to_char( buf, ch );
      return;
   }

   send_to_char( "Details for value0,...,value3 are:\r\n", ch );
   value *= 10;
   for( foo = 0; foo < 4; foo++ )
   {
      snprintf( buf, MSL, "@@W[Value@@y%d@@W] : @@y%s@@g\r\n", foo, rev_table_lookup( tab_value_meanings, value + foo ) );
      send_to_char( buf, ch );
   }
   return;
}

DO_FUN(build_listweapons)
{
   /*
    * list weapon types, along with values
    */
   char buf[MSL];
   int foo;

   /*
    * Need values as well, so rehash table_printout
    */
   for( foo = 0; tab_weapon_types[foo].text != NULL; foo++ )
   {
      snprintf( buf, MSL, "@@W%2ld - @@y%10s.   ", tab_weapon_types[foo].value, tab_weapon_types[foo].text );
      send_to_char( buf, ch );
      if( ( foo + 1 ) % 2 == 0 )
         send_to_char( "\r\n", ch );
   }
   send_to_char( "@@g\r\n", ch );
   return;
}

DO_FUN(build_listliquids)
{
   /*
    * list liquid types, along with values
    */
   char buf[MSL];
   int foo;

   /*
    * Need values as well, so rehash table_printout
    */
   for( foo = 0; tab_drink_types[foo].text != NULL; foo++ )
   {
      snprintf( buf, MSL, "%2ld - %12s.   ", tab_drink_types[foo].value, tab_drink_types[foo].text );
      send_to_char( buf, ch );
      if( ( foo + 1 ) % 2 == 0 )
         send_to_char( "\r\n", ch );
   }
   send_to_char( "\r\n", ch );
   return;
}

DO_FUN(build_listspells)
{
   /*
    * List spells -S-
    */
   int sn;
   char buf[MSL];
   char buf2[MSL];
   int type;
   bool fall;

   if( argument[0] == '\0' )
   {
      send_to_char( "USAGE: spells < target_type >\r\n", ch );
      send_to_char( "target type being one of:  ign - picks own target\r\n", ch );
      send_to_char( "                           off - offensive\r\n", ch );
      send_to_char( "                           def - defensive\r\n", ch );
      send_to_char( "                           slf - personal-only (self)\r\n", ch );
      send_to_char( "                           obj - object-only\r\n", ch );
      send_to_char( "                           all - list ALL spells\r\n", ch );
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
      send_to_char( "Option not recognized.\r\n", ch );
      build_listspells( ch, "" );   /* usage message */
      return;
   }

   snprintf( buf2, MSL, "List of spells for option %s:\r\n", argument );

   for( sn = 0; sn < MAX_SKILL; sn++ )
   {
      if( skill_table[sn].name == NULL )
         break;
      if( skill_table[sn].slot == 0 )
         continue;

      if( skill_table[sn].target == type || fall )
      {
         snprintf( buf, MSL, "@@W[Spell No: %4d] @@y%s@@g\r\n", sn, skill_table[sn].name );
         strncat( buf2, buf, MSL-1 );
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

DO_FUN(build_urooms)
{
   /*
    * List vnum usage for area... 
    */
   int curvnum;
   char buf[MSL];
   char free[MSL];
   char used[MSL];
   AREA_DATA *area;
   int last = 0;  /* 0 = start, 1 = used, 2 = free */
   int foo = 0;   /* holds start of free/used vnums, so no 3001-3001 */

   area = ch->in_room->area;
   /*
    * Rooms 
    */
   snprintf( free, MSL, "(Free) " );
   snprintf( used, MSL, "(Used) " );
   for( curvnum = area->min_vnum; curvnum < area->max_vnum; curvnum++ )
   {
      if( get_room_index( curvnum ) != NULL )
      {
         switch ( last )
         {
            case 0:
               snprintf( buf, MSL, "%d", curvnum );
               strncat( used, buf, MSL-1 );
               foo = curvnum;
               last = 1;
               break;
            case 1:
               break;
            case 2:
               if( foo != curvnum - 1 )
               {
                  snprintf( buf, MSL, "-%d", curvnum - 1 );
                  strncat( free, buf, MSL-1 );
               }
               snprintf( buf, MSL, " %d", curvnum );
               strncat( used, buf, MSL-1 );
               foo = curvnum;
               last = 1;
         }
      }
      else
      {
         switch ( last )
         {
            case 0:
               snprintf( buf, MSL, "%d", curvnum );
               strncat( free, buf, MSL-1 );
               foo = curvnum;
               last = 2;
               break;
            case 1:
               if( foo != curvnum - 1 )
               {
                  snprintf( buf, MSL, "-%d", curvnum - 1 );
                  strncat( used, buf, MSL-1 );
               }
               snprintf( buf, MSL, " %d", curvnum );
               strncat( free, buf, MSL-1 );
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
               snprintf( buf, MSL, "-%d.", curvnum );
            else
               snprintf( buf, MSL, " %d.", curvnum );
            strncat( used, buf, MSL-1 );
            break;
         case 2:
            if( foo != curvnum - 1 )
            {
               snprintf( buf, MSL, "-%d.", curvnum - 1 );
               strncat( used, buf, MSL-1 );
            }
            snprintf( buf, MSL, " %d.", curvnum );
            strncat( free, buf, MSL-1 );
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
               snprintf( buf, MSL, "-%d.", curvnum - 1 );
               strncat( used, buf, MSL-1 );
            }
            snprintf( buf, MSL, " %d.", curvnum );
            strncat( free, buf, MSL-1 );
            break;
         case 2:
            if( foo != curvnum - 1 )
               snprintf( buf, MSL, "-%d.", curvnum );
            else
               snprintf( buf, MSL, " %d.", curvnum );
            strncat( free, buf, MSL-1 );
            break;
      }
   }

   snprintf( buf, MSL, "Room vnum usage summary:\r\n\r\n%s\r\n\r\n%s\r\n", used, free );
   send_to_char( buf, ch );
   return;
}



DO_FUN(build_uobjs)
{
   /*
    * List vnum usage for area... 
    */
   int curvnum;
   char buf[MSL];
   char free[MSL];
   char used[MSL];
   AREA_DATA *area;
   int last = 0;  /* 0 = start, 1 = used, 2 = free */
   int foo = 0;   /* holds start of free/used vnums, so no 3001-3001 */

   area = ch->in_room->area;
   /*
    * Rooms
    */
   snprintf( free, MSL, "(Free) " );
   snprintf( used, MSL, "(Used) " );
   for( curvnum = area->min_vnum; curvnum < area->max_vnum; curvnum++ )
   {
      if( get_obj_index( curvnum ) != NULL )
      {
         switch ( last )
         {
            case 0:
               snprintf( buf, MSL, "%d", curvnum );
               strncat( used, buf, MSL-1 );
               foo = curvnum;
               last = 1;
               break;
            case 1:
               break;
            case 2:
               if( foo != curvnum - 1 )
               {
                  snprintf( buf, MSL, "-%d", curvnum - 1 );
                  strncat( free, buf, MSL-1 );
               }
               snprintf( buf, MSL, " %d", curvnum );
               strncat( used, buf, MSL-1 );
               foo = curvnum;
               last = 1;
         }
      }
      else
      {
         switch ( last )
         {
            case 0:
               snprintf( buf, MSL, "%d", curvnum );
               strncat( free, buf, MSL-1 );
               foo = curvnum;
               last = 2;
               break;
            case 1:
               if( foo != curvnum - 1 )
               {
                  snprintf( buf, MSL, "-%d", curvnum - 1 );
                  strncat( used, buf, MSL-1 );
               }
               snprintf( buf, MSL, " %d", curvnum );
               strncat( free, buf, MSL-1 );
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
               snprintf( buf, MSL, "-%d.", curvnum );
            else
               snprintf( buf, MSL, " %d.", curvnum );
            strncat( used, buf, MSL-1 );
            break;
         case 2:
            if( foo != curvnum - 1 )
            {
               snprintf( buf, MSL, "-%d.", curvnum - 1 );
               strncat( used, buf, MSL-1 );
            }
            snprintf( buf, MSL, " %d.", curvnum );
            strncat( free, buf, MSL-1 );
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
               snprintf( buf, MSL, "-%d.", curvnum - 1 );
               strncat( used, buf, MSL-1 );
            }
            snprintf( buf, MSL, " %d.", curvnum );
            strncat( free, buf, MSL-1 );
            break;
         case 2:
            if( foo != curvnum - 1 )
               snprintf( buf, MSL, "-%d.", curvnum );
            else
               snprintf( buf, MSL, " %d.", curvnum );
            strncat( free, buf, MSL-1 );
            break;
      }
   }

   snprintf( buf, MSL, "Object vnum usage summary:\r\n\r\n%s\r\n\r\n%s\r\n", used, free );
   send_to_char( buf, ch );
   return;
}


DO_FUN(build_umobs)
{
   /*
    * List vnum usage for area...
    */
   int curvnum;
   char buf[MSL];
   char free[MSL];
   char used[MSL];
   AREA_DATA *area;
   int last = 0;  /* 0 = start, 1 = used, 2 = free */
   int foo = 0;   /* holds start of free/used vnums, so no 3001-3001 */

   area = ch->in_room->area;
   snprintf( free, MSL, "(Free) " );
   snprintf( used, MSL, "(Used) " );
   for( curvnum = area->min_vnum; curvnum < area->max_vnum; curvnum++ )
   {
      if( get_mob_index( curvnum ) != NULL )
      {
         switch ( last )
         {
            case 0:
               snprintf( buf, MSL, "%d", curvnum );
               strncat( used, buf, MSL-1 );
               foo = curvnum;
               last = 1;
               break;
            case 1:
               break;
            case 2:
               if( foo != curvnum - 1 )
               {
                  snprintf( buf, MSL, "-%d", curvnum - 1 );
                  strncat( free, buf, MSL-1 );
               }
               snprintf( buf, MSL, " %d", curvnum );
               strncat( used, buf, MSL-1 );
               foo = curvnum;
               last = 1;
         }
      }
      else
      {
         switch ( last )
         {
            case 0:
               snprintf( buf, MSL, "%d", curvnum );
               strncat( free, buf, MSL-1 );
               foo = curvnum;
               last = 2;
               break;
            case 1:
               if( foo != curvnum - 1 )
               {
                  snprintf( buf, MSL, "-%d", curvnum - 1 );
                  strncat( used, buf, MSL-1 );
               }
               snprintf( buf, MSL, " %d", curvnum );
               strncat( free, buf, MSL-1 );
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
               snprintf( buf, MSL, "-%d.", curvnum );
            else
               snprintf( buf, MSL, " %d.", curvnum );
            strncat( used, buf, MSL-1 );
            break;
         case 2:
            if( foo != curvnum - 1 )
            {
               snprintf( buf, MSL, "-%d.", curvnum - 1 );
               strncat( used, buf, MSL-1 );
            }
            snprintf( buf, MSL, " %d.", curvnum );
            strncat( free, buf, MSL-1 );
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
               snprintf( buf, MSL, "-%d.", curvnum - 1 );
               strncat( used, buf, MSL-1 );
            }
            snprintf( buf, MSL, " %d.", curvnum );
            strncat( free, buf, MSL-1 );
            break;
         case 2:
            if( foo != curvnum - 1 )
               snprintf( buf, MSL, "-%d.", curvnum );
            else
               snprintf( buf, MSL, " %d.", curvnum );
            strncat( free, buf, MSL-1 );
            break;
      }
   }

   snprintf( buf, MSL, "Mobile vnum usage summary:\r\n\r\n%s\r\n\r\n%s\r\n", used, free );
   send_to_char( buf, ch );
   return;
}



/** Help Editor
    We want to be able to edit ANY help, so 3.bank etc should work,
    in case we have helps with the same keyword(s).
 **/
DO_FUN(build_findhelp)
{
   char arg[MSL];
   int cnt = 0;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Usage: Findhelp <keyword>\r\n", ch );
      return;
   }

   if( cnt == 0 )
      send_to_char( "Couldn't find that keyword.\r\n", ch );
   return;
}




DO_FUN(build_helpedit)
{
 char arg[MSL];
 bool mort = FALSE;
 FILE *fp;

 argument = strlower(argument);
 argument = one_argument(argument,arg);
 smash_replace(argument," ","_");

 if( argument[0] == '\0' )
 {
  send_to_char("Usage: HELPEDIT <mort/imm> <keyword>.\r\n",ch);
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

 snprintf(arg,MSL,"%s%s/%s.%s",HELP_DIR,initial(argument),argument,mort ? HELP_MORT : HELP_IMM);
 if( (fp = file_open(arg,"r")) == NULL )
 {
  send_to_char("Couldn't find that keyword.\r\n",ch);
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
    snprintf(buf2,MSL,"%s",buf1);
   else
    strncat(buf2,buf1,MSL-1);
  }
  ch->pcdata->message = str_dup(buf2);

  build_strdup(&ch->pcdata->message,"$edit",TRUE,FALSE,ch);
  file_close(fp);
 }
 else
 {
  send_to_char("An error has occured. Contact a shell owner.\r\n",ch);
  return;
 }

 return;
}

DO_FUN(build_addhelp)
{
 char arg[MSL];
 bool mort = FALSE;
 FILE *fp;

 argument = strlower(argument);
 argument = one_argument(argument,arg);
 smash_replace(argument," ","_");

 if( argument[0] == '\0' )
 {
  send_to_char("Usage: ADDHELP <mort/imm> <keyword>.\r\n",ch);
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

 snprintf(arg,MSL,"%s%s/%s.%s",HELP_DIR,initial(argument),argument,mort ? HELP_MORT : HELP_IMM);
 if( (fp = file_open(arg,"r")) != NULL )
 {
  send_to_char("Help already exists. Use HELPEDIT <keyword> to edit it.\r\n",ch);
  file_close(fp);
  return;
 }
 else if( (fp = file_open(arg,"w")) != NULL )
 {
  send_to_char("Help added. Use HELPEDIT <keyword> to edit it.\r\n",ch);
  file_close(fp);
  return;
 }
 else
 {
  send_to_char("An error has occured. Contact a shell owner.\r\n",ch);
  return;
 }

 return;
}


DO_FUN(build_clone)
{
   char arg1[MSL];
   char arg2[MSL];

   /*
    * Allow builder to clone a room/mob/object -
    * Takes existing r/m/o and makes new copy with relevant details
    * copied across...
    */

   if( argument[0] == '\0' )  /* Show help info */
   {
      send_to_char( "Usage: CLONE <type> <vnum>\r\n", ch );
      send_to_char( "Where <type> is one of: mob  room  obj\r\n", ch );
      send_to_char( "      <vnum> is the vnum of the type you want to clone.\r\n", ch );
      send_to_char( "[The item will be copied onto the exisiting item you are editing]\r\n", ch );
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
      EXTRA_DESCR_DATA *pEd, *pEd_new;

      if( ch->pcdata->build_mode != BUILD_MODE_REDIT )
      {
         send_to_char( "You must be in REDIT mode to clone a room.\r\n", ch );
         return;
      }

      if( ( room = get_room_index( atoi( arg2 ) ) ) == NULL )
      {
         send_to_char( "Sorry, no such room with that vnum.\r\n", ch );
         return;
      }

      if( in_room == NULL )
      {
         send_to_char( "Don't know what room you're in!!\r\n", ch );
         return;
      }

      /*
       * Check builder can read room, and write to in_room 
       */
      if( !build_canread( room->area, ch, 0 ) )
      {
         send_to_char( "Sorry, you don't have authorization to read that room.\r\n", ch );
         return;
      }
      if( !build_canwrite( in_room->area, ch, 0 ) )
      {
         send_to_char( "Sorry, you don't have authorization to write into this room.\r\n", ch );
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

      for( pEd = room->first_exdesc; pEd != NULL; pEd = pEd->next )
      {
       pEd_new = new EXTRA_DESCR_DATA;
       pEd_new->keyword = str_dup(pEd->keyword);
       pEd_new->description = str_dup(pEd->description);
       LINK( pEd_new, in_room->first_exdesc, in_room->last_exdesc, next, prev );
      }

      send_to_char( "Room cloned.\r\n", ch );
      return;
   }

   if( !str_cmp( arg1, "obj" ) )
   {
      OBJ_INDEX_DATA *obj;
      OBJ_INDEX_DATA *this_obj;
      EXTRA_DESCR_DATA *pEd, *pEd_new;
      int looper;

      if( ch->pcdata->build_mode != BUILD_MODE_OEDIT )
      {
         send_to_char( "You must be in OEDIT mode to clone an object.\r\n", ch );
         return;
      }

      if( ( this_obj = get_obj_index( ch->pcdata->build_vnum ) ) == NULL )
      {
         send_to_char( "You must select a valid object in OEDIT before you clone.\r\n", ch );
         return;
      }

      if( ( obj = get_obj_index( atoi( arg2 ) ) ) == NULL )
      {
         send_to_char( "That object does not exist to be cloned.\r\n", ch );
         return;
      }

      if( !build_canread( obj->area, ch, 0 ) )
      {
         send_to_char( "Sorry, you don't have authorization to read that object.\r\n", ch );
         return;
      }
      if( !build_canwrite( this_obj->area, ch, 0 ) )
      {
         send_to_char( "Sorry, you don't have authorization to write to this object.\r\n", ch );
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
      if( this_obj->long_descr != NULL )
         free_string( this_obj->long_descr );
      this_obj->long_descr = str_dup( obj->long_descr );
      this_obj->item_type = obj->item_type;
      this_obj->extra_flags = obj->extra_flags;
      this_obj->wear_flags = obj->wear_flags;
      this_obj->item_apply = obj->item_apply;
      for( looper = 0; looper < MAX_OBJ_VALUE; looper++ )
         this_obj->value[looper] = obj->value[looper];

      this_obj->weight = obj->weight;
      this_obj->speed = obj->speed;
      this_obj->durability = obj->durability;
      this_obj->max_durability = obj->max_durability;
      this_obj->obj_fun = obj->obj_fun;

      for( pEd = obj->first_exdesc; pEd != NULL; pEd = pEd->next )
      {
       pEd_new = new EXTRA_DESCR_DATA;
       pEd_new->keyword = str_dup(pEd->keyword);
       pEd_new->description = str_dup(pEd->description);
       LINK( pEd_new, this_obj->first_exdesc, this_obj->last_exdesc, next, prev );
      }

      send_to_char( "Object cloned.\r\n", ch );
      return;
   }

   if( !str_cmp( arg1, "mob" ) )
   {
      MOB_INDEX_DATA *mob;
      MOB_INDEX_DATA *this_mob;
      SHOP_DATA *pShop, *pShop_new;
      BUILD_DATA_LIST *pList, *pList_new;

      if( ch->pcdata->build_mode != BUILD_MODE_MEDIT )
      {
         send_to_char( "You must be in MEDIT mode to clone mobiles.\r\n", ch );
         return;
      }

      if( ( this_mob = get_mob_index( ch->pcdata->build_vnum ) ) == NULL )
      {
         send_to_char( "You must select a valid mobile in MEDIT before you clone mobiles.\r\n", ch );
         return;
      }

      if( ( mob = get_mob_index( atoi( arg2 ) ) ) == NULL )
      {
         send_to_char( "That mobile does not exist to be cloned.\r\n", ch );
         return;
      }

      if( !build_canread( mob->area, ch, 0 ) )
      {
         send_to_char( "Sorry, you do not have authorization to read that mobile.\r\n", ch );
         return;
      }
      if( !build_canwrite( this_mob->area, ch, 0 ) )
      {
         send_to_char( "Sorry, you do not have authorization to write to this mobile.\r\n", ch );
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
      this_mob->p_class = mob->p_class;
      this_mob->race = mob->race;
      this_mob->race_mods = mob->race_mods;
      this_mob->resist = mob->resist;
      this_mob->strong_magic = mob->strong_magic;
      this_mob->suscept = mob->suscept;
      this_mob->weak_magic = mob->weak_magic;
      this_mob->spec_fun = mob->spec_fun;

      for( pList = mob->area->first_area_shop; pList != NULL; pList = pList->next )
      {
       pShop = (SHOP_DATA *)pList->data;
       if( pShop->keeper != mob->vnum )
        continue;

       pShop_new = new SHOP_DATA;
       pList_new = new BUILD_DATA_LIST;

       for( short i = 0; i < MAX_TRADE; i++ )
        pShop_new->buy_type[i] = pShop->buy_type[i];
       pShop_new->close_hour = pShop->close_hour;
       pShop_new->keeper = this_mob->vnum;
       pShop_new->open_hour = pShop->open_hour;
       pShop_new->profit_buy = pShop->profit_buy;
       pShop_new->profit_sell = pShop->profit_sell;

       this_mob->pShop = pShop_new;

       pList_new->data = pShop_new;
       LINK( pList_new, this_mob->area->first_area_shop, this_mob->area->last_area_shop, next, prev );
      }

      send_to_char( "Mobile cloned.\r\n", ch );
      return;
   }
   return;
}

void check_autodig( CHAR_DATA *ch, int dir )
{
 if( ch->position == POS_BUILDING && ch->pcdata->build_mode == BUILD_MODE_REDIT && ch->act.test(ACT_AUTODIG) )
 {
  AREA_DATA *pArea;
  EXIT_DATA *pExit;
  int vnum;
  bool found = FALSE;
  char buf[MSL];
  char exit[MSL];

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
   send_to_char("AUTODIG: No more free vnums in area.\r\n",ch);
   return;
  }

  switch( dir )
  {
   case DIR_NORTH: snprintf(exit,6,"north"); break;
   case DIR_SOUTH: snprintf(exit,6,"south"); break;
   case DIR_EAST:  snprintf(exit,6,"east");  break;
   case DIR_WEST:  snprintf(exit,6,"west");  break;
   case DIR_UP:    snprintf(exit,6,"up");    break;
   case DIR_DOWN:  snprintf(exit,6,"down");  break;
  }

  snprintf(buf,MSL,"%s %d",exit,vnum);
  build_dig(ch,buf);
 }
}

DO_FUN(build_sysdata)
{
 char outbuf[MSL];
 char catbuf[MSL];
 char arg1[MSL];
 char arg2[MSL];

 argument = one_argument(argument,arg1);
 argument = one_argument(argument,arg2);

 if( arg1[0] == '\0' )
 {
  send_to_char("Syntax for sysdata:\r\n",ch);
  send_to_char("  sysdata help | show | <option> <value>\r\n",ch);
  send_to_char("  options: obj[ac | dr | hp | hr | mp | mv | svs]\r\n",ch);
  return;
 }

 if( !str_prefix(arg1,"help") )
 {
  build_help(ch,"sysdata");
  return;
 }

 if( !str_prefix(arg1,"show") )
 {
  snprintf(outbuf,MSL,"%s","  Builder system data for " mudnamecolor ":\r\n");
  snprintf(catbuf,MSL,"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n");
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Option         ]       [Value          ]\r\n");
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n");
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Obj AC         ]       [%15d]\r\n",sysdata.build_obj_ac);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Obj DR         ]       [%15d]\r\n",sysdata.build_obj_dr);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Obj HP         ]       [%15d]\r\n",sysdata.build_obj_hp);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Obj HR         ]       [%15d]\r\n",sysdata.build_obj_hr);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Obj MP         ]       [%15d]\r\n",sysdata.build_obj_mp);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Obj MV         ]       [%15d]\r\n",sysdata.build_obj_mv);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Obj SVS        ]       [%15d]\r\n",sysdata.build_obj_svs);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n");
  strncat(outbuf,catbuf,MSL-1);
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
