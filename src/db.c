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
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3.1        *
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

#ifndef DEC_ACT_MOB_H
#include "h/act_mob.h"
#endif

#ifndef DEC_ACT_OBJ_H
#include "h/act_obj.h"
#endif

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
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

#ifndef DEC_HASH_H
#include "h/hash.h"
#endif

#ifndef DEC_INTERP_H
#include "h/interp.h"
#endif

#ifndef DEC_LUASCRIPT_H
#include "h/luascript.h"
#endif

#ifndef DEC_MACROS_H
#include "h/macros.h"
#endif

#ifndef DEC_MONEY_H
#include "h/money.h"
#endif

#ifndef DEC_MUDINFO_H
#include "h/mudinfo.h"
#endif

#ifndef DEC_OBJ_FUN_H
#include "h/obj_fun.h"
#endif

#ifndef DEC_RULERS_H
#include "h/rulers.h"
#endif

#ifndef DEC_SAVE_H
#include "h/save.h"
#endif

#ifndef DEC_SOCIAL_EDIT_H
#include "h/social_edit.h"
#endif

#ifndef DEC_SPEC_FUN_H
#include "h/spec_fun.h"
#endif

#ifndef DEC_SPENDQP_H
#include "h/spendqp.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

#ifndef DEC_SYSDATA_H
#include "h/sysdata.h"
#endif

#if !defined(macintosh)
extern int _filbuf args( ( FILE * ) );
#endif

/*
 * Globals.
 */
char bug_buf[2 * MAX_INPUT_LENGTH];
char log_buf[2 * MAX_INPUT_LENGTH];
char testerbuf[MSL];
KILL_DATA kill_table[MAX_LEVEL];
TIME_INFO_DATA time_info;
WEATHER_DATA weather_info;
SYS_DATA_TYPE sysdata;
MUDINFO mudinfo;
CHAN_HISTORY chan_history;

bool booting_up;
bool area_resetting_global;
bool mem_log;
short gsn_martial_arts;
short gsn_stake;
short gsn_dualwield;
short gsn_stun;
short gsn_feed;
short gsn_knee;
short gsn_shadow;
short gsn_appraise;
short gsn_climb;
short gsn_find_doors;
short gsn_bash;
short gsn_smash;
short gsn_notrip;
short gsn_nodisarm;
short gsn_trip;
short gsn_dirt;
short gsn_shield_block;
short gsn_berserk;

short gsn_circle;
short gsn_backstab;
short gsn_dodge;
short gsn_hide;
short gsn_hunt;
short gsn_peek;
short gsn_pick_lock;
short gsn_sneak;
short gsn_warcry;
short gsn_steal;

short gsn_punch;
short gsn_headbutt;

short gsn_disarm;
short gsn_enhanced_damage;
short gsn_kick;
short gsn_parry;
short gsn_rescue;
short gsn_enhanced_reflexes;
short gsn_sleight_of_hand;
short gsn_crushing_blow;
short gsn_combat_prowess;
short gsn_quickstrike;
short gsn_blindness;
short gsn_charm_person;
short gsn_curse;
short gsn_invis;
short gsn_mass_invis;
short gsn_poison;
short gsn_sleep;
short gsn_disguise;
short gsn_instruct;
short gsn_adrenaline;
short gsn_frenzy;
short gsn_emotion_control;
short gsn_target;
short gsn_charge;
short gsn_unit_tactics;
short gsn_imprint;
short gsn_scent;
short gsn_mount;
short gsn_scout;
short gsn_decapitate;
short gsn_potency;
short gsn_thaumatergy;

extern bool auto_quest;
extern COUNCIL_DATA super_councils[MAX_SUPER];


/*
 * Locals.
 */
MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
OBJ_INDEX_DATA *obj_index_hash[MAX_KEY_HASH];
ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];
char *string_hash[MAX_KEY_HASH];

AREA_DATA *area_used[MAX_AREAS];
AREA_DATA *area_load;
ROOM_INDEX_DATA *room_load;
MOB_INDEX_DATA *mob_load;
OBJ_INDEX_DATA *obj_load;

int fp_open;
int fp_close;

int nAllocPerm;
int sAllocPerm;

/*
 * Semi-locals.
 */
bool fBootDb;
FILE *fpArea;
char strArea[MAX_STRING_LENGTH];
int area_revision = -1;

#define SHOW_AREA \
  if (!previous_bug) \
  { \
    bugf ("      In %s.", pArea->filename); \
    previous_bug=1; \
  }

/*
 * Big mama top level function.
 */
void boot_db( void )
{
   int a;

   /*
    * Init some data space stuff.
    */

   init_string_space(  );
   fBootDb = TRUE;


   send_to_descrips( "Initialising Ack! Mud.  Please Wait....\r\n" );

   /*
    * Init random number generator.
    */
   {
      init_mm(  );
   }

   load_sysdata();   /* Must be first, can possibly alter things to follow! --Kline */

   /*
    * Set time and weather.
    */
   {
      long lhour, lday, lmonth;

      lhour = ( current_time - 650336715 ) / ( PULSE_TICK / PULSE_PER_SECOND );
      time_info.hour = lhour % 24;
      lday = lhour / 24;
      time_info.day = lday % 35;
      lmonth = lday / 35;
      time_info.month = lmonth % 17;
      time_info.year = lmonth / 17;
      time_info.moon = 0;
      weather_info.moon_loc = MOON_DOWN;
      weather_info.moon_phase = number_range( MOON_NEW, MOON_FULL );


      if( time_info.hour < 5 )
         weather_info.sunlight = SUN_DARK;
      else if( time_info.hour < 6 )
         weather_info.sunlight = SUN_RISE;
      else if( time_info.hour < 19 )
         weather_info.sunlight = SUN_LIGHT;
      else if( time_info.hour < 20 )
         weather_info.sunlight = SUN_SET;
      else
         weather_info.sunlight = SUN_DARK;

      weather_info.change = 0;
      weather_info.mmhg = 960;
      if( time_info.month >= 7 && time_info.month <= 12 )
         weather_info.mmhg += number_range( 1, 50 );
      else
         weather_info.mmhg += number_range( 1, 80 );

      if( weather_info.mmhg <= 980 )
         weather_info.sky = SKY_LIGHTNING;
      else if( weather_info.mmhg <= 1000 )
         weather_info.sky = SKY_RAINING;
      else if( weather_info.mmhg <= 1020 )
         weather_info.sky = SKY_CLOUDY;
      else
         weather_info.sky = SKY_CLOUDLESS;

   }

   /*
    * Initialize Supernatural Councils
    */

   {
      short index;
      char buf[MAX_STRING_LENGTH];
      log_f( "Initializing supernatural councils..." );
      for( index = SUPER_NONE; index < MAX_SUPER; index++ )
      {
         super_councils[index].first_member = NULL;
         super_councils[index].last_member = NULL;
         super_councils[index].quorum = FALSE;
         super_councils[index].council_time = 0;
         switch ( index )
         {
            case SUPER_NONE:
               snprintf( buf, MSL, "%s", "ORDINARIES" );
               break;
            case SUPER_VAMP:
               snprintf( buf, MSL, "%s", "@@rJUSTICARS@@N" );
               break;

            default:
               snprintf( buf, MSL, "%s", "NONE" );
               break;
         }
         super_councils[index].council_name = str_dup( buf );

      }
      log_f("Done.");
   }

   /*
    * Assign gsn's for skills which have them.
    */
   {
      int sn;

      for( sn = 0; sn < MAX_SKILL; sn++ )
      {
         if( skill_table[sn].pgsn != 0 )
            *skill_table[sn].pgsn = sn;
      }
   }

   /*
    * Clear list of used areas 
    */
   for( a = 0; a < MAX_AREAS; a++ )
   {
      area_used[a] = NULL;
   }


/*
 *   Read in clan data table
 */

   {
      extern POL_DATA politics_data;

      FILE *clanfp;
      char clan_file_name[MAX_STRING_LENGTH];
      short x, y;
      char buf[MAX_STRING_LENGTH];

      snprintf( clan_file_name, MSL, "%s", CLAN_FILE );


      snprintf( buf, MSL, "Loading %s", clan_file_name );
      log_f("%s",buf);


      if( ( clanfp = file_open( clan_file_name, "r" ) ) == NULL )
      {
         log_f( "failed open of clan_table.dat in load_clan_table" );
      }
      else
      {
         fpArea = clanfp;
         snprintf( strArea, MSL, "%s", clan_file_name );

         for( x = 1; x < MAX_CLAN; x++ )
         {
            for( y = 1; y < MAX_CLAN; y++ )
            {
               politics_data.diplomacy[x][y] = fread_number( clanfp );
            }
         }

         for( x = 1; x < MAX_CLAN; x++ )
         {
            politics_data.treasury[x] = fread_number( clanfp );
         }

         for( x = 1; x < MAX_CLAN; x++ )
         {
            for( y = 1; y < MAX_CLAN; y++ )
            {
               politics_data.end_current_state[x][y] = fread_number( clanfp );
            }
         }


         file_close( clanfp );
      }
      fpArea = NULL;
      log_f("Done.");

   }

   /*
    * Start loading up data files!
    */

   load_areas();
   load_social_table();
   load_notes();
   load_marks();
   load_bans();
   load_rulers();
   load_brands();
   load_corpses();
   load_disabled();
   load_mudinfo();
   if( mudinfo.first_boot == 0 ) /* First game boot, record it! */
    mudinfo.first_boot = current_time;
   /*
    * Fix up exits.
    * MAG Mod: Check resets. (Allows loading objects of later areas.)
    * Declare db booting over.
    * Reset all areas once.
    * Load up the notes file.
    */
   {
      log_f( "Fixing exits..." );
      fix_exits(  );
      log_f( "Done." );
      fBootDb = FALSE;
      log_f( "Checking resets..." );
      check_resets(  );
      log_f( "Done.");
      log_f( "Updating areas..." );
      area_update(  );
      log_f("Done.");
      booting_up = FALSE;
   }
   boot_done();
   auto_quest = TRUE;
   return;
}

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )  if ( !str_cmp( word, literal ) ) { field  = value; fMatch = true;  break;}
#define SKEY( literal, field, value )  if ( !str_cmp( word, literal ) ) { if (field!=NULL) free_string(field);field  = value; fMatch = true;  break;}
/*
 * Read in all the area files.
 */
void load_areas( void )
{
   FILE *fpList;
   log_f( "Loading area files..." );

   if( ( fpList = file_open( AREA_LIST, "r" ) ) == NULL )
   {
      perror( AREA_LIST );
      log_f( "Unable to open area.lst, aborting bootup." );
      kill( getpid(  ), SIGQUIT );
   }

   for( ;; )
   {
      strcpy( strArea, fread_word( fpList ) );
      if( strArea[0] == '$' )
         break;

      if( strArea[0] == '-' )
      {
         fpArea = stdin;
      }
      else
      {
         string file;

         file += AREA_DIR;
         file += strArea;

         if( ( fpArea = file_open( file.c_str(), "r" ) ) == NULL )
         {
            log_string( strArea );
            kill( getpid(  ), SIGQUIT );
         }
      }

      for( ;; )
      {
         char *word;

         if( fread_letter( fpArea ) != '#' )
         {
            bug( "Boot_db: # not found.", 0 );
            kill( getpid(  ), SIGQUIT );
         }

         word = fread_word( fpArea );

         if( word[0] == '$' )
            break;

         if( !str_cmp( word, "AREA" ) )
            load_area( fpArea );
         else if( !str_cmp( word, "DOOR" ) )
            load_door( fpArea );
         else if( !str_cmp( word, "MOBILE" ) )
            load_mobile( fpArea );
         else if( !str_cmp( word, "OAFFECT" ) )
            load_oaffect( fpArea );
         else if( !str_cmp( word, "OBJECT" ) )
            load_object( fpArea );
         else if( !str_cmp( word, "OEXTRA" ) )
            load_oextra( fpArea );
         else if( !str_cmp( word, "RESETS" ) )
            load_resets( fpArea );
         else if( !str_cmp( word, "REXTRA" ) )
            load_rextra( fpArea );
         else if( !str_cmp( word, "ROOM" ) )
            load_room( fpArea );
         else if( !str_cmp( word, "SHOP" ) )
            load_shop( fpArea );
         else
         {
            bug( "Boot_db: bad section name.", 0 );
            exit( 1 );
         }
      }

      if( fpArea != stdin )
         file_close( fpArea );
      fpArea = NULL;
   }
   file_close( fpList );
   log_f("Done.");
   return;
}

/*
 * Snarf an 'area' header line.
 */
void load_area( FILE * fp )
{
   AREA_DATA *pArea;
   const char *word;
   bool fMatch = false;
   int a;

   pArea = new AREA_DATA;
   pArea->filename = str_dup( strArea );
   area_revision = -1;

   for( ;; )
   {
      word = fread_word( fp );
      fMatch = false;

      if( !str_cmp(word,"End") )
       break;

      switch ( UPPER( word[0] ) )
      {
         case '*':
            fMatch = TRUE;
            fread_to_eol( fp );
            break;
         case 'C':
            SKEY("CanRead", pArea->can_read, fread_string( fp ) );
            SKEY("CanWrite", pArea->can_write, fread_string( fp ) );
            break;
         case 'F':
            if( !str_cmp( word, "Flags" ) )
            {
             const char *tmp = fread_word(fp);
             while( str_cmp(tmp,"EOL") )
             {
              pArea->flags.set(atoi(tmp));
              tmp = fread_word(fp);
             }
             fMatch = TRUE;
             break;
            }
            break;
         case 'K':
            SKEY("Keyword", pArea->keyword, fread_string( fp ) );
            break;
         case 'L':
            SKEY("LevLabel", pArea->level_label, fread_string( fp ) );
            if( !str_cmp(word,"LevRange") )
            {
             pArea->min_level = fread_number(fp);
             pArea->max_level = fread_number(fp);
             fMatch = true;
             break;
            }
            break;
         case 'N':
            SKEY("Name", pArea->name, fread_string( fp ) );
            KEY("Number", pArea->area_num, fread_number( fp ) );
            break;
         case 'O':
            SKEY("Owner", pArea->owner, fread_string( fp ) );
            break;
         case 'R':
            KEY("ResetMsg", pArea->reset_msg, fread_string( fp ) );
            KEY("ResetRate", pArea->reset_rate, fread_number( fp ) );
            KEY("Revision", area_revision, fread_number( fp ) );
            break;
         case 'V':
            if( !str_cmp(word,"VnumRange") )
            {
             pArea->min_vnum = fread_number(fp);
             pArea->max_vnum = fread_number(fp);
             fMatch = true;
             break;
            }
            break;
      }
   }

   if( !fMatch )
   {
      snprintf( log_buf, (2 * MIL), "Loading in area :%s, no match for ( %s ).", pArea->name, word );
      monitor_chan( log_buf, MONITOR_BAD );
      fread_to_eol( fp );
   }

   if( pArea->area_num == 0 )
   {
      /*
       * Find a unique area number 
       */
      for( a = 0; a < MAX_AREAS; a++ )
         if( area_used[a] == NULL )
            break;
      pArea->area_num = a;
   }

   area_used[pArea->area_num] = pArea;
   area_load = pArea;

   return;
}

void load_door( FILE *fp )
{
 EXIT_DATA *pExit;
 short dir = 0;
 const char *word;
 bool fMatch = false;

 if( room_load == NULL )
 {
  bug( "Load_door: no #ROOM seen yet.", 0 );
  hang( "Loading doors in db.c" );
 }

 pExit = new EXIT_DATA;

 for( ;; )
 {
  word = fread_word( fp );
  fMatch = false;

  if( !str_cmp(word,"End") )
   break;

  switch( UPPER(word[0]) )
  {
   case '*':
    fMatch = true;
    fread_to_eol(fp);
    break;

   case 'D':
    SKEY("Desc", pExit->description, fread_string(fp));
    if( !str_cmp(word,"Dir") )
    {
     dir = fread_number(fp);

     if( dir < 0 || dir > MAX_DIR )
     {
       bug( "Load_door: dir %d has bad door number.", dir );
       hang( "Loading doors in db.c" );
     }

     fMatch = true;
     break;
    }
    break;

   case 'F':
    if( !str_cmp(word,"Flags") )
    {
     const char *tmp;

     tmp = fread_word(fp);

     while( str_cmp(tmp,"EOL") )
     {
      pExit->exit_info.set(atoi(tmp));
      tmp = fread_word(fp);
     }

     fMatch = true;
     break;
    }
    break;

   case 'K':
    KEY("Key", pExit->key, fread_number(fp));
    SKEY("Keyword", pExit->keyword, fread_string(fp));
    break;

   case 'V':
    KEY("Vnum", pExit->vnum, fread_number(fp));
    break;
  }
 }

 if( !fMatch )
 {
  snprintf( log_buf, (2 * MIL), "Loading in door :%s (%s), no match for ( %s ).", area_load->name, room_load->name, word );
  monitor_chan( log_buf, MONITOR_BAD );
  fread_to_eol( fp );
 }

 room_load->exit[dir] = pExit;

 return;
}

void load_corpses( void )
{

   FILE *corpsefp;
   char corpse_file_name[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];


   snprintf( corpse_file_name, MSL, "%s", CORPSE_FILE );


   snprintf( buf, MSL, "Loading %s", CORPSE_FILE);
   log_f( "%s", buf );



   if( ( corpsefp = file_open( corpse_file_name, "r" ) ) == NULL )
   {
      log_f( "Load corpse Table: file_open" );
      perror( "failed open of corpse_table.dat in load_corpse_table" );
   }

   else
   {
      fpArea = corpsefp;
      snprintf( strArea, MSL, "%s", corpse_file_name );

      for( ;; )

      {
         char letter;
         char *word;

         letter = fread_letter( corpsefp );
         if( letter == '*' )
         {
            fread_to_eol( corpsefp );
            continue;
         }

         if( letter != '#' )
         {
            log_f( "Load_char_obj: # not found." );
            break;
         }

         word = fread_word( corpsefp );
         if( !str_cmp( word, "OBJECT" ) )
            fread_corpse( corpsefp );
         else if( !str_cmp( word, "END" ) )
            break;
         else
         {
            log_f( "Load_char_obj: bad section." );
            break;
         }
      }
   }
   file_close( corpsefp );
   fpArea = NULL;
   log_f("Done.");

}

/*
 * Load marked rooms
 *
 */

void load_marks( void )
{
 FILE *fp;

 snprintf(log_buf,(2 * MIL),"Loading %s",MARKS_FILE);
 log_f("%s",log_buf);

 if( (fp = file_open(MARKS_FILE,"r")) == NULL )
 {
  file_close(fp);
  log_f("No marks file to read.");
  return;
 }

 for( ;; )
 {
  MARK_DATA *mark;
  list<MARK_DATA *>::iterator li;
  char letter;

  do
  {
   letter = getc(fp);

   if( feof(fp) )
   {
    file_close(fp);
    log_f("Done.");
    return;
   }
  }
  while( isspace(letter) );

  ungetc(letter,fp);

  mark = new MARK_DATA;

  if( str_cmp(fread_word(fp),"Room") )
   break;
  mark->room_vnum = fread_number(fp);
  if( get_room_index(mark->room_vnum) == NULL )
  {
   bug("NULL room %d loading marks!",mark->room_vnum);
   delete mark;
   file_close(fp);
   log_f("Done.");
   return;
  }

  if( str_cmp(fread_word(fp),"Author") )
   break;
  mark->author = fread_string(fp);
  if( str_cmp(fread_word(fp),"Duration") )
   break;
  mark->duration = fread_number(fp);
  if( str_cmp(fread_word(fp),"Message") )
   break;
  mark->message = fread_string(fp);
  if( str_cmp(fread_word(fp),"Type") )
   break;
  mark->type = fread_number(fp);
  get_room_index(mark->room_vnum)->mark_list.push_back(mark);
 }

 bug("Load_marks: bad key word.",0);
 hang("Loading marks in db.c");
 return;
}

void load_bans( void )
{

   FILE *bansfp;
   char bans_file_name[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];


   snprintf( bans_file_name, MSL, "%s", BANS_FILE );
   snprintf( buf, MSL, "Loading %s",BANS_FILE);
   log_f( "%s", buf );



   if( ( bansfp = file_open( bans_file_name, "r" ) ) == NULL )
   {
      log_f( "Load bans Table: file_open" );
      perror( "failed open of bans_table.dat in load_bans_table" );
   }
   else
   {
      fpArea = bansfp;
      snprintf( strArea, MSL, "%s", bans_file_name );

      for( ;; )
      {
         char *word;
         BAN_DATA *pban;
         word = fread_string( bansfp );
         if( !str_cmp( word, "#BAN" ) )
         {
            short get_bool = FALSE;

            pban = new BAN_DATA;
            get_bool = fread_number( bansfp );
            if( get_bool == 1 )
               pban->newbie = TRUE;
            else
               pban->newbie = FALSE;
            pban->name = fread_string( bansfp );
            pban->banned_by = fread_string( bansfp );
            free_string( word );
         }
         else if( !str_cmp( word, "#END" ) )
         {
            free_string( word );
            break;
         }
         else
         {
            free_string( word );
            log_f( "Load_bans: bad section." );
            break;
         }
      }

      file_close( bansfp );
      fpArea = NULL;
      log_f("Done.");

   }
}

/*
 * Snarf a mob section.
 */
void load_mobile( FILE * fp )
{
   MOB_INDEX_DATA *pMobIndex;
   BUILD_DATA_LIST *pList;
   char buf[MSL];
   const char *word;
   const char *tmp;
   bool fMatch = false;
   int vnum = 0, iHash = 0;

   if( area_load == NULL )
   {
    bug( "Load_mobile: no #AREA seen yet.", 0 );
    hang( "Loading Mobiles in db.c" );
   }

   pMobIndex = new MOB_INDEX_DATA;
   pMobIndex->area = area_load;

   for( ;; )
   {

      word = fread_word( fp );
      fMatch = false;

      if( !str_cmp(word,"End") )
         break;

      switch( UPPER( word[0] ) )
      {
         case '*':
            fMatch = true;
            fread_to_eol( fp );
            break;

         case 'A':
            KEY("AcMod",pMobIndex->ac_mod,fread_number(fp));
            if( !str_cmp(word,"Act") )
            {
             tmp = fread_word(fp);

             while( str_cmp(tmp,"EOL") )
             {
              pMobIndex->act.set(atoi(tmp));
              tmp = fread_word(fp);
             }
             fMatch = true;
             break;
            }
            KEY("Affected",pMobIndex->affected_by,fread_number(fp));
            KEY("Alignment",pMobIndex->alignment,fread_number(fp));
            break;

         case 'C':
            KEY("Cast",pMobIndex->cast,fread_number(fp));
            KEY("Clan",pMobIndex->clan,fread_number(fp));
            KEY("Class",pMobIndex->p_class,fread_number(fp));
            break;

         case 'D':
            KEY("Def",pMobIndex->def,fread_number(fp));
            SKEY("Desc",pMobIndex->description,fread_string(fp));
            KEY("DrMod",pMobIndex->dr_mod,fread_number(fp));
            break;

         case 'H':
            KEY("HrMod",pMobIndex->hr_mod,fread_number(fp));
            break;

         case 'L':
            KEY("Level",pMobIndex->level,fread_number(fp));
            SKEY("LongDesc",pMobIndex->long_descr,fread_string(fp));
            break;

         case 'P':
            SKEY("PlrName",pMobIndex->player_name,fread_string(fp));
            KEY("Position",pMobIndex->position,fread_number(fp));
            break;

         case 'R':
            KEY("Race",pMobIndex->race,fread_number(fp));
            KEY("RaceMods", pMobIndex->race_mods,fread_number(fp));
            KEY("Resist",pMobIndex->resist,fread_number(fp));
            break;

         case 'S':
            SKEY("ScriptName",pMobIndex->script_name,fread_string(fp));
            KEY("Sex",pMobIndex->sex,fread_number(fp));
            SKEY("ShortDesc",pMobIndex->short_descr,fread_string(fp));
            KEY("Skills",pMobIndex->skills,fread_number(fp));
            KEY("SMagic",pMobIndex->strong_magic,fread_number(fp));
            if( !str_cmp(word,"SpecFun") )
            {
             tmp = fread_word(fp);
             if( str_cmp(tmp,"(null)") )
             {
              pMobIndex->spec_fun = spec_lookup(tmp);

              if( pMobIndex->spec_fun == NULL )
              {
               snprintf(buf,MSL,"Load_mobile: spec_fun invalid for mob %d in %s.",pMobIndex->vnum,area_load->filename);
               log_string(buf);
              }
             }
             fMatch = true;
             break;
            }
            KEY("Suscept",pMobIndex->suscept,fread_number(fp));
            break;

         case 'V':
            if( !str_cmp(word,"Vnum") )
            {
             vnum = fread_number(fp);
             if( vnum < area_load->min_vnum || vnum > area_load->max_vnum )
             {
              snprintf(buf,MSL,"Load_mobile: vnum %d out of bounds for %s.",vnum,area_load->filename);
              log_string(buf);
             }
             fBootDb = FALSE;
             if( get_mob_index( vnum ) != NULL )
             {
              bug( "Load_mobile: vnum %d duplicated.", vnum );
              hang( "Loading Mobiles in db.c" );
             }
             fBootDb = TRUE;
             pMobIndex->vnum = vnum;
             fMatch = true;
             break;
            }
            break;

         case 'W':
            KEY("WMagic",pMobIndex->weak_magic,fread_number(fp));
            break;

      }
   }

   if( !fMatch )
   {
    snprintf( log_buf, (2 * MIL), "Loading in mob :%s (%s), no match for ( %s ).", area_load->name, pMobIndex->short_descr, word );
    monitor_chan( log_buf, MONITOR_BAD );
    fread_to_eol( fp );
   }

   iHash = vnum % MAX_KEY_HASH;
   SING_TOPLINK( pMobIndex, mob_index_hash[iHash], next );
   pList = new BUILD_DATA_LIST;
   pList->data = pMobIndex;
   LINK( pList, area_load->first_area_mobile, area_load->last_area_mobile, next, prev );

   mob_load = pMobIndex;
   kill_table[URANGE( 0, pMobIndex->level, MAX_LEVEL - 1 )].number++;

   return;
}

void load_oaffect( FILE * fp )
{
 AFFECT_DATA *pAf;
 const char *word;
 bool fMatch = false;

 if( obj_load == NULL )
 {
  bug( "Load_oaffect: no #OBJECT seen yet.", 0 );
  hang( "Loading Oaffects in db.c" );
 }

 pAf = new AFFECT_DATA;
 affect_list.push_back(pAf); //Done manually due to how the "affects list" could be shared in the ctor --Kline

 for( ;; )
 {
  word = fread_word( fp );
  fMatch = false;

  if( !str_cmp(word,"End") )
   break;

  switch( UPPER(word[0]) )
  {
   case '*':
    fMatch = true;
    fread_to_eol(fp);
    break;

   case 'L':
    KEY("Location", pAf->location, fread_number(fp));
    break;

   case 'M':
    KEY("Modifier", pAf->modifier, fread_number(fp));
    break;
  }
 }

 if( !fMatch )
 {
  snprintf( log_buf, (2 * MIL), "Loading in oaffect :%s (%s), no match for ( %s ).", area_load->name, obj_load->name, word );
  monitor_chan( log_buf, MONITOR_BAD );
  fread_to_eol( fp );
 }

 LINK( pAf, obj_load->first_apply, obj_load->last_apply, next, prev );

 return;
}

/*
 * Snarf an obj section.
 */
void load_object( FILE * fp )
{
   OBJ_INDEX_DATA *pObjIndex;
   BUILD_DATA_LIST *pList;
   const char *word;
   const char *tmp;
   bool fMatch = false;
   int vnum = 0, iHash = 0;
   char buf[MSL];

   if( area_load == NULL )
   {
    bug( "Load_object: no #AREA seen yet.", 0 );
    hang( "Loading Objects in db.c" );
   }

   pObjIndex = new OBJ_INDEX_DATA;
   pObjIndex->area = area_load;

   for( ;; )
   {

      word = fread_word( fp );
      fMatch = false;

      if( !str_cmp(word,"End") )
         break;

      switch( UPPER( word[0] ) )
      {
         case '*':
            fMatch = true;
            fread_to_eol( fp );
            break;

         case 'D':
            KEY("Durability",pObjIndex->max_durability,fread_number(fp));
            break;

         case 'E':
            if( !str_cmp(word,"ExtraFlags") )
            {
             tmp = fread_word(fp);

             while( str_cmp(tmp,"EOL") )
             {
              pObjIndex->extra_flags.set(atoi(tmp));
              tmp = fread_word(fp);
             }
             fMatch = true;
             break;
            }
            break;

         case 'I':
            KEY("ItemApply",pObjIndex->item_apply,fread_number(fp));
            break;

         case 'L':
            KEY("Level",pObjIndex->level,fread_number(fp));
            SKEY("LongDesc",pObjIndex->long_descr,fread_string(fp));
            break;

         case 'N':
            SKEY("Name",pObjIndex->name,fread_string(fp));
            break;

         case 'O':
            if( !str_cmp(word,"ObjFun") )
            {
             tmp = fread_word(fp);
             if( str_cmp(tmp,"(null)") )
             {
              pObjIndex->obj_fun = obj_fun_lookup(tmp);

              if( pObjIndex->obj_fun == NULL )
              {
               snprintf(buf,MSL,"Load_object: obj_fun invalid for item %d in %s.",pObjIndex->vnum,area_load->filename);
               log_string(buf);
              }
             }
             fMatch = true;
             break;
            }
            break;

         case 'T':
            KEY("Type",pObjIndex->item_type,fread_number(fp));
            break;

         case 'S':
            SKEY("ScriptName",pObjIndex->script_name,fread_string(fp));
            SKEY("ShortDesc",pObjIndex->short_descr,fread_string(fp));
            KEY("Speed",pObjIndex->speed,fread_float(fp));
            break;

         case 'V':
            if( !str_cmp(word,"Values") )
            {
             for( short i = 0; i < MAX_OBJ_VALUE; i++ )
              pObjIndex->value[i] = fread_number(fp);
             fMatch = true;
             break;
            }
            if( !str_cmp(word,"Vnum") )
            {
             vnum = fread_number(fp);
             if( vnum < area_load->min_vnum || vnum > area_load->max_vnum )
             {
              snprintf(buf,MSL,"Load_object: vnum %d out of bounds for %s.",vnum,area_load->filename);
              log_string(buf);
             }
             fBootDb = FALSE;
             if( get_obj_index( vnum ) != NULL )
             {
              bug( "Load_object: vnum %d duplicated.", vnum );
              hang( "Loading Objects in db.c" );
             }
             fBootDb = TRUE;
             pObjIndex->vnum = vnum;
             fMatch = true;
             break;
            }
            break;

         case 'W':
            if( !str_cmp(word,"WearFlags") )
            {
             tmp = fread_word(fp);

             while( str_cmp(tmp,"EOL") )
             {
              pObjIndex->wear_flags.set(atoi(tmp));
              tmp = fread_word(fp);
             }
             fMatch = true;
             break;
            }
            KEY("Weight",pObjIndex->weight,fread_number(fp));
            break;
      }
   }

   if( !fMatch )
   {
    snprintf( log_buf, (2 * MIL), "Loading in obj :%s (%s), no match for ( %s ).", area_load->name, pObjIndex->short_descr, word );
    monitor_chan( log_buf, MONITOR_BAD );
    fread_to_eol( fp );
   }

   iHash = vnum % MAX_KEY_HASH;
   SING_TOPLINK( pObjIndex, obj_index_hash[iHash], next );
   pList = new BUILD_DATA_LIST;
   pList->data = pObjIndex;
   LINK( pList, area_load->first_area_object, area_load->last_area_object, next, prev );

   obj_load = pObjIndex;

   return;
}

void load_oextra( FILE * fp )
{
 EXTRA_DESCR_DATA *pEd;
 const char *word;
 bool fMatch = false;

 if( obj_load == NULL )
 {
  bug( "Load_oextra: no #OBJECT seen yet.", 0 );
  hang( "Loading Oextras in db.c" );
 }

 pEd = new EXTRA_DESCR_DATA;

 for( ;; )
 {
  word = fread_word( fp );
  fMatch = false;

  if( !str_cmp(word,"End") )
   break;

  switch( UPPER(word[0]) )
  {
   case '*':
    fMatch = true;
    fread_to_eol(fp);
    break;

   case 'D':
    SKEY("Desc", pEd->description, fread_string(fp));
    break;

   case 'K':
    SKEY("Keyword", pEd->keyword, fread_string(fp));
    break;
  }
 }

 if( !fMatch )
 {
  snprintf( log_buf, (2 * MIL), "Loading in oextra :%s (%s), no match for ( %s ).", area_load->name, obj_load->name, word );
  monitor_chan( log_buf, MONITOR_BAD );
  fread_to_eol( fp );
 }

 LINK( pEd, obj_load->first_exdesc, obj_load->last_exdesc, next, prev );

 return;
}

/*
 * Snarf a reset section.
 */

/* Spec: it isn't obvious how this works, so I'll add a comment here.
 *
 * load_resets only allows resets that are placed in current rooms - this is
 * the reset_ok code here. It does _not_ check the validity of room and object
 * vnums. It handles linking the resets into the appropriate room-reset-list
 * for OLC.
 *
 * check_resets is called after all areas are loaded. It checks that all
 * resets point to valid mobs and object and spits errors as necessary. It
 * does _not_ touch the room lists any more!
 */
void load_resets( FILE * fp )
{
   RESET_DATA *pReset = NULL;
   BUILD_DATA_LIST *pList = NULL;
   ROOM_INDEX_DATA *last_mob_room;
   ROOM_INDEX_DATA *last_obj_room;

   last_mob_room = NULL;
   last_obj_room = NULL;

   if( area_load == NULL )
   {
      bug( "Load_resets: no #AREA seen yet.", 0 );
      hang( "Loading Resets in db.c" );
   }

   for( ;; )
   {
      ROOM_INDEX_DATA *pRoomIndex;
      /*
       * EXIT_DATA *pexit; unused? 
       */
      char letter;
      /*
       * Temp values to hold reset, while we see if valid
       */
      char Tcommand;
      int Targ1;
      int Targ2;
      int Targ3;
      bool reset_ok;

      reset_ok = FALSE;

      if( ( letter = fread_letter( fp ) ) == 'S' )
         break;

      if( letter == '*' )
      {
         fread_to_eol( fp );
         continue;
      }


      Tcommand = letter;
      /*
       * MAG Mod 
       */

      Targ1 = fread_number( fp );
      Targ2 = fread_number( fp );
      Targ3 = ( letter == 'G' || letter == 'R' ) ? 0 : fread_number( fp );

      pRoomIndex = NULL;   /* Set it to check for room numbers */
      /*
       * Validate parameters.
       * We're calling the index functions for the side effect.
       */
      switch ( letter )
      {
         default:
            bug( "Load_resets: bad command '%c'.", letter );
            reset_ok = FALSE;
            break;

         case 'A':
            reset_ok = FALSE;
            break;   /* obsolete reset, discard */
         case 'M':
            if( ( pRoomIndex = get_room_index( Targ3 ) ) != NULL )
            {
               reset_ok = TRUE;
               last_mob_room = pRoomIndex;
            }
            break;
         case 'O':
            if( ( pRoomIndex = get_room_index( Targ3 ) ) != NULL )
            {
               reset_ok = TRUE;
               last_obj_room = pRoomIndex;
            }
            break;
         case 'P':
            if ( last_obj_room )
            {
               pRoomIndex = last_obj_room;
               reset_ok = TRUE;
            } 
            break;
         case 'G':
         case 'E':
            if( last_mob_room )
            {
               pRoomIndex = last_mob_room;
               reset_ok = TRUE;
            }
            break;

         case 'D':
            if( ( pRoomIndex = get_room_index( Targ1 ) ) != NULL )
            {
               reset_ok = TRUE;
            }
            break;
         case 'R':
            if( ( pRoomIndex = get_room_index( Targ1 ) ) != NULL )
            {
               reset_ok = TRUE;
            }
            break;

      }

      if( reset_ok ) /* i.e. only add if reset is valid. */
      {

         pReset = new RESET_DATA;
         pReset->command = Tcommand;
         pReset->arg1 = Targ1;
         pReset->arg2 = Targ2;
         pReset->arg3 = Targ3;
         pReset->notes = fsave_to_eol( fp );

         LINK( pReset, area_load->first_reset, area_load->last_reset, next, prev );
         pList = new BUILD_DATA_LIST;
         pList->data = pReset;
         LINK( pList, pRoomIndex->first_room_reset, pRoomIndex->last_room_reset, next, prev );

      }
   }

   return;
}

void load_rextra( FILE * fp )
{
 EXTRA_DESCR_DATA *pEd;
 const char *word;
 bool fMatch = false;

 if( room_load == NULL )
 {
  bug( "Load_rextra: no #ROOM seen yet.", 0 );
  hang( "Loading Rextras in db.c" );
 }

 pEd = new EXTRA_DESCR_DATA;

 for( ;; )
 {
  word = fread_word( fp );
  fMatch = false;

  if( !str_cmp(word,"End") )
   break;

  switch( UPPER(word[0]) )
  {
   case '*':
    fMatch = true;
    fread_to_eol(fp);
    break;

   case 'D':
    SKEY("Desc", pEd->description, fread_string(fp));
    break;

   case 'K':
    SKEY("Keyword", pEd->keyword, fread_string(fp));
    break;
  }
 }

 if( !fMatch )
 {
  snprintf( log_buf, (2 * MIL), "Loading in rextra :%s (%s), no match for ( %s ).", area_load->name, room_load->name, word );
  monitor_chan( log_buf, MONITOR_BAD );
  fread_to_eol( fp );
 }

 LINK( pEd, room_load->first_exdesc, room_load->last_exdesc, next, prev );

 return;
}

/*
 * Snarf a room section.
 */
void load_room( FILE * fp )
{
   ROOM_INDEX_DATA *pRoomIndex;;
   BUILD_DATA_LIST *pList;
   char buf[MSL];
   const char *tmp;
   const char *word;
   bool fMatch = false;
   int vnum = 0, iHash;

   if( area_load == NULL )
   {
      bug( "Load_room: no #AREA seen yet.", 0 );
      hang( "Loading Rooms in db.c" );
   }

   pRoomIndex = new ROOM_INDEX_DATA;
   pRoomIndex->area = area_load;

   for( ;; )
   {
      word = fread_word( fp );
      fMatch = false;

      if( !str_cmp(word,"End") )
       break;

      switch( UPPER( word[0] ) )
      {
         case '*':
            fMatch = true;
            fread_to_eol( fp );
            break;

         case 'D':
            SKEY("Desc", pRoomIndex->description, fread_string( fp ) );
            break;

         case 'F':
            if( !str_cmp(word,"Flags") )
            {
             tmp = fread_word(fp);

             while( str_cmp(tmp,"EOL") )
             {
              pRoomIndex->room_flags.set(atoi(tmp));
              tmp = fread_word(fp);
             }
             fMatch = true;
             break;
            }
            break;

         case 'N':
            SKEY("Name", pRoomIndex->name, fread_string( fp ) );
            break;

         case 'S':
            if( !str_cmp( word, "ScriptName" ) )
            {
             free_string(pRoomIndex->script_name);
             pRoomIndex->script_name = fread_string(fp);
             if( pRoomIndex->script_name != &str_empty[0] ) /* Room has a script attached */
             {
              pRoomIndex->lua = new LUA_DATA;
              init_lua(pRoomIndex);
             }
             fMatch = true;
             break;
            }
            KEY("Sect", pRoomIndex->sector_type, fread_number( fp ) );
            break;

         case 'V':
            if( !str_cmp(word,"Vnum") )
            {
             vnum = fread_number(fp);
             if( vnum < area_load->min_vnum || vnum > area_load->max_vnum )
             {
              snprintf(buf,MSL,"Load_room: vnum %d out of bounds for %s.",vnum,area_load->filename);
              log_string(buf);
             }
             fBootDb = FALSE;
             if( get_room_index( vnum ) != NULL )
             {
              bug( "Load_room: vnum %d duplicated.", vnum );
              hang( "Loading Rooms in db.c" );
             }
             fBootDb = TRUE;
             pRoomIndex->vnum = vnum;
             fMatch = true;
             break;
            }
            break;
      }
   }

   if( !fMatch )
   {
    snprintf( log_buf, (2 * MIL), "Loading in room :%s (%s), no match for ( %s ).", area_load->name, pRoomIndex->name, word );
    monitor_chan( log_buf, MONITOR_BAD );
    fread_to_eol( fp );
   }

   if( pRoomIndex->sector_type == SECT_NULL )
      pRoomIndex->sector_type = SECT_INSIDE;


   iHash = vnum % MAX_KEY_HASH;
   SING_TOPLINK( pRoomIndex, room_index_hash[iHash], next );
   pList = new BUILD_DATA_LIST;
   pList->data = pRoomIndex;
   LINK( pList, area_load->first_area_room, area_load->last_area_room, next, prev );

   room_load = pRoomIndex;

   return;
}

/*
 * Snarf a shop section.
 */
void load_shop( FILE * fp )
{
   SHOP_DATA *pShop;
   BUILD_DATA_LIST *pList;
   MOB_INDEX_DATA *pMobIndex;
   int iTrade, keeper;
   const char *word;
   char buf[MSL];
   bool fMatch = false;

   if( area_load == NULL )
   {
      bug( "Load_shop: no #AREA seen yet.", 0 );
      hang( "Loading Shops in db.c" );
   }

   pShop = new SHOP_DATA;

   for( ;; )
   {
      word = fread_word( fp );
      fMatch = false;

      if( !str_cmp(word,"End") )
       break;

      switch( UPPER( word[0] ) )
      {
         case '*':
            fMatch = true;
            fread_to_eol( fp );
            break;

         case 'B':
            if( !str_cmp(word,"BuyType") )
            {
             for( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
              pShop->buy_type[iTrade] = fread_number( fp );

             fMatch = true;
             break;
            }
            break;

         case 'H':
            KEY("HourClose",pShop->close_hour,fread_number(fp));
            KEY("HourOpen",pShop->open_hour,fread_number(fp));
            break;

         case 'K':
            if( !str_cmp(word,"Keeper") )
            {
             keeper = fread_number(fp);
             if( keeper < area_load->min_vnum || keeper > area_load->max_vnum )
             {
              snprintf(buf,MSL,"Load_shop: keeper %d out of bounds for %s.",keeper,area_load->filename);
              log_string(buf);
             }
             if( get_mob_index(keeper) == NULL )
             {
              snprintf(buf,MSL,"Load_shop: keeper %d not found in %s.",keeper,area_load->filename);
              log_string(buf);
             }
             pShop->keeper = keeper;
             fMatch = true;
             break;
            }
            break;

         case 'P':
            KEY("ProfBuy",pShop->profit_buy,fread_number(fp));
            KEY("ProfSell",pShop->profit_sell,fread_number(fp));
            break;
      }
   }

   if( !fMatch )
   {
    snprintf( log_buf, (2 * MIL), "Loading in shop :%s (%s), no match for ( %s ).", area_load->name, get_mob_index(pShop->keeper)->short_descr, word );
    monitor_chan( log_buf, MONITOR_BAD );
    fread_to_eol( fp );
   }

   pMobIndex = get_mob_index( pShop->keeper );
   pMobIndex->pShop = pShop;
   pList = new BUILD_DATA_LIST;
   pList->data = pShop;
   LINK( pList, area_load->first_area_shop, area_load->last_area_shop, next, prev );

   return;
}

/*
 * Snarf notes file.
 */
void load_notes( void )
{
   FILE *fp;

   snprintf(log_buf,(2 * MIL),"Loading %s",NOTE_FILE);
   log_f("%s",log_buf);

   if( ( fp = file_open( NOTE_FILE, "r" ) ) == NULL )
   {
      log_f( "No note file to read." );
      return;
   }

   for( ;; )
   {
      NOTE_DATA *pnote;
      char letter;
      do
      {
         letter = getc( fp );
         if( feof( fp ) )
         {
            file_close( fp );
            log_f("Done.");
            return;
         }
      }
      while( isspace( letter ) );
      ungetc( letter, fp );

      pnote = new NOTE_DATA;

      if( str_cmp( fread_word( fp ), "sender" ) )
         break;
      pnote->sender = fread_string( fp );

      if( str_cmp( fread_word( fp ), "date" ) )
         break;
      pnote->date = fread_string( fp );

      if( str_cmp( fread_word( fp ), "stamp" ) )
         break;
      pnote->date_stamp = fread_number( fp );

      if( str_cmp( fread_word( fp ), "to" ) )
         break;
      pnote->to_list = fread_string( fp );

      if( str_cmp( fread_word( fp ), "subject" ) )
         break;
      pnote->subject = fread_string( fp );

      if( str_cmp( fread_word( fp ), "text" ) )
         break;
      pnote->text = fread_string( fp );
   }

   strcpy( strArea, NOTE_FILE );
   fpArea = fp;
   bug( "Load_notes: bad key word.", 0 );
   hang( "Loading Notes in db.c" );
   return;
}



/*
 * Translate all room exits from virtual to real.
 * Has to be done after all rooms are read in.
 * Check for bad reverse exits.
 */
void fix_exits( void )
{

   char buf[MAX_STRING_LENGTH];
   ROOM_INDEX_DATA *pRoomIndex;
   EXIT_DATA *pexit;
   int iHash;
   int door;

   for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
   {
      for( pRoomIndex = room_index_hash[iHash]; pRoomIndex != NULL; pRoomIndex = pRoomIndex->next )
      {
         bool fexit;

         fexit = FALSE;
         for( door = 0; door < MAX_DIR; door++ )
         {
            if( ( pexit = pRoomIndex->exit[door] ) != NULL )
            {
               if( ( pexit->vnum <= 0 ) || ( get_room_index( pexit->vnum ) == NULL ) )
               {
                  snprintf( buf, MSL, "Bad exit vnum %d in room %d", pexit->vnum, pRoomIndex->vnum );
                  bug( buf, 0 );
                  delete pexit;
                  pRoomIndex->exit[door] = NULL;
/*			pexit->to_room = NULL;   */
               }

               else
               {
                  fexit = TRUE;
                  pexit->to_room = get_room_index( pexit->vnum );
               }
            }
         }

         if( !fexit )
            pRoomIndex->room_flags.set(RFLAG_NO_MOB);
      }
   }
   return;
}

void load_disabled( void )
{
 FILE *fp;
 DISABLED_DATA *p;
 const char *word;
 short i;

 snprintf( log_buf, (2 * MIL), "Loading %s", DISABLED_FILE);
 log_f( "%s", log_buf );

 if( (fp = file_open(DISABLED_FILE,"r")) == NULL )
 {
  log_f("Done.");
  file_close(fp);
  return;
 }

 for( ;; )
 {
  word = feof(fp) ? "End" : fread_word(fp);
 
  if( !str_cmp(word,"End") )
  {
   file_close(fp);
   log_f("Done.");
   return;
  }

  for( i = 0; cmd_table[i].name[0] != '\0'; i++ )
   if( !str_cmp(cmd_table[i].name,word) )
    break;

  if( cmd_table[i].name[0] == '\0' ) // Old command now removed?
  {
   snprintf(log_buf,(2 * MIL),"Skipping unknown command (%s) in disabled commands.",word);
   log_f("%",log_buf);
   fread_number(fp); // Level
   fread_word(fp); // Disabled by
  }
  else
  {
   p = new DISABLED_DATA;
   p->command = &cmd_table[i];
   p->level = fread_number(fp);
   p->disabled_by = str_dup(fread_word(fp));
  }
 }

 log_f("Done.");
 file_close(fp);

 return;
}

void save_disabled( void )
{
 FILE *fp;
 DISABLED_DATA *p;
 list<DISABLED_DATA *>::iterator li;

 if( disabled_list.empty() )
 {
  unlink(DISABLED_FILE);
  return;
 }

 if( (fp = file_open(DISABLED_FILE,"w")) == NULL )
 {
  bug("Couldn't open " DISABLED_FILE " for writing.",0);
  file_close(fp);
  return;
 }

 for( li = disabled_list.begin(); li != disabled_list.end(); li++ )
 {
  p = *li;
  fprintf(fp,"%s %d %s\n",p->command->name,p->level,p->disabled_by);
 }

 fprintf(fp,"End\n\n");
 file_close(fp);

 return;
}

/* Spec: see the comments on load_resets about what check_resets does */
#define VALID_RESET 0
#define INVAL_ROOM  1
#define INVAL_OBJ   2
#define INVAL_MOB   3
#define INVAL_GEN   4
void check_resets( void )
{
   AREA_DATA *pArea;
   RESET_DATA *pReset;
   RESET_DATA *nextReset;
   ROOM_INDEX_DATA *last_mob_room;
   list<AREA_DATA *>::iterator li;
   int previous_bug = 0;

   for( li = area_list.begin(); li != area_list.end(); li++ )
   {
      pArea = *li;
      last_mob_room = NULL;

      for( pReset = pArea->first_reset; pReset; pReset = nextReset )
      {
         ROOM_INDEX_DATA *pRoomIndex = NULL;
         EXIT_DATA *pexit;
         int ValReset = VALID_RESET;

         nextReset = pReset->next;

         switch ( pReset->command )
         {
            default:
               SHOW_AREA;
               bug( "Load_resets: bad command '%c'.", pReset->command );
               ValReset = INVAL_GEN;
               break;
            case 'A':  /* obsolete.. */
               if( !get_room_index( pReset->arg1 ) )
                  ValReset = INVAL_ROOM;
               break;
            case 'M':
               if( !get_mob_index( pReset->arg1 ) )
                  ValReset = INVAL_MOB;
               if( !( last_mob_room = get_room_index( pReset->arg3 ) ) )
                  ValReset = INVAL_ROOM;
               break;
            case 'O':
               if( !get_obj_index( pReset->arg1 ) )
                  ValReset = INVAL_OBJ;
               if( !get_room_index( pReset->arg3 ) )
                  ValReset = INVAL_ROOM;
               break;
            case 'P':  /* obsolete.. */
               if( !get_obj_index( pReset->arg1 ) || !get_obj_index( pReset->arg3 ) )
                  ValReset = INVAL_OBJ;
               break;
            case 'G':
            case 'E':
               if( !get_obj_index( pReset->arg1 ) )
                  ValReset = INVAL_OBJ;
               if( !last_mob_room )
                  ValReset = INVAL_ROOM;
               break;
            case 'D':
               if( !( pRoomIndex = get_room_index( pReset->arg1 ) ) )
               {
                  ValReset = INVAL_ROOM;
                  break;
               }
               if( pReset->arg2 < 0 || pReset->arg2 > 5 ||
                   !( pexit = pRoomIndex->exit[pReset->arg2] ) || !pexit->exit_info.test(EX_ISDOOR) )
               {
                  SHOW_AREA;
                  bug( "Check_resets: 'D': exit %d not door.", pReset->arg2 );
                  ValReset = INVAL_GEN;
               }

               if( pReset->arg3 < 0 || pReset->arg3 > 2 )
               {
                  SHOW_AREA;
                  bug( "Check_resets: 'D': bad 'locks': %d.", pReset->arg3 );
                  ValReset = INVAL_GEN;
               }
               break;
            case 'R':
               if( !( pRoomIndex = get_room_index( pReset->arg1 ) ) )
               {
                  ValReset = INVAL_ROOM;
                  break;
               }
               if( pReset->arg2 < 0 || pReset->arg2 > 5 )
               {
                  SHOW_AREA;
                  bug( "Check_resets: 'R': bad exit %d.", pReset->arg2 );
                  ValReset = INVAL_GEN;
               }
               break;
         }
         if( ValReset != VALID_RESET )
         {
            char buf[128];

            switch ( ValReset )
            {
               case INVAL_ROOM:
                  snprintf( buf, 128, "Invalid room for reset: " );
                  break;
               case INVAL_OBJ:
                  snprintf( buf, 128, "Invalid obj  for reset: " );
                  break;
               case INVAL_MOB:
                  snprintf( buf, 128, "Invalid mob  for reset: " );
                  break;
               case INVAL_GEN:
               default:
                  snprintf( buf, 128, "Invalid arg  for reset: " );
                  break;
            }
            SHOW_AREA;
            snprintf( buf + strlen( buf ), 128, "%c %d %d %d.", pReset->command,
                     pReset->arg1, pReset->arg2, pReset->arg3 );
            bug( buf, 0 );
            {
               int room = 0;
               ROOM_INDEX_DATA *reset_room;
               RESET_DATA *criminal;
               BUILD_DATA_LIST *guilty_reset;
               room = pReset->arg1;
               reset_room = get_room_index( room );
               for( guilty_reset = reset_room->first_room_reset; guilty_reset; guilty_reset = guilty_reset->next )
               {

                  criminal = (RESET_DATA *)guilty_reset->data;
                  if( criminal == pReset )
                  {
                     bug( "Found the reset", 0 );
                     break;
                  }
               }
               if( guilty_reset == NULL )
               {
                  snprintf( bug_buf, (2 * MIL), "Uhoh, couldn't find that damn reset!" );
                  monitor_chan( bug_buf, MONITOR_AREA_BUGS );
                  bug( "Couldn't find the reset.", 0 );
               }
               else
               {
                  UNLINK( guilty_reset, reset_room->first_room_reset, reset_room->last_room_reset, next, prev );
                  guilty_reset->data = NULL;
                  build_dat_list.remove(guilty_reset);
                  delete guilty_reset;
               }
            }
            UNLINK( pReset, pArea->first_reset, pArea->last_reset, next, prev );
            delete pReset;
         }
      }
   }
   return;
}



/*
 * Repopulate areas periodically.
 */
void area_update( void )
{
   AREA_DATA *pArea;
   list<AREA_DATA *>::iterator li;

   for( li = area_list.begin(); li != area_list.end(); li++ )
   {
      CHAR_DATA *pch;
      list<CHAR_DATA *>::iterator pi;

      pArea = *li;
      pArea->age++;
      /*
       * Check for PC's.
       */
      if( !pArea->player_list.empty() && ( pArea->age >= ( pArea->reset_rate - 1 ) ) )
      {
         for( pi = pArea->player_list.begin(); pi != pArea->player_list.end(); pi++ )
         {
            pch = *pi;
            if( !IS_NPC( pch ) && IS_AWAKE( pch ) && pch->in_room != NULL && str_cmp(pArea->reset_msg,"off") )
            {
               char reset_buf[MSL];
               snprintf( reset_buf, MSL, "%s\r\n", pArea->reset_msg );
               send_to_char( reset_buf, pch );
            }
         }
      }


      /*
       * Check age and reset.
       * Note: Mud School resets every 3 minutes (not 15).
       */
      if( ( pArea->player_list.empty() && ( pArea->age >= 5 ) ) || ( pArea->age >= pArea->reset_rate ) )
      {
         reset_area( pArea );
         pArea->age = UMIN( pArea->reset_rate - 1, number_range( 0, 3 ) );

      }
   }

   return;
}



/*
 * Reset one area.
 */

void reset_area( AREA_DATA * pArea )
{
   RESET_DATA *pReset;
   CHAR_DATA *mob;
   char buf[MAX_STRING_LENGTH];
   bool last;
   bool just_loaded = FALSE;
   int level;
   int previous_bug = 0;


   snprintf( buf, MSL, "Area Reset called for %s.", pArea->name );
   monitor_chan( buf, MONITOR_AREA_UPDATE );
   area_resetting_global = TRUE;
   mob = NULL;
   last = TRUE;
   level = 0;
   for( pReset = pArea->first_reset; pReset != NULL; pReset = pReset->next )
   {
      ROOM_INDEX_DATA *pRoomIndex;
      MOB_INDEX_DATA *pMobIndex;
      OBJ_INDEX_DATA *pObjIndex;
      OBJ_INDEX_DATA *pObjToIndex;
      EXIT_DATA *pexit;
      OBJ_DATA *obj;
      OBJ_DATA *obj_to;

      switch ( pReset->command )
      {
         default:
            bug( "Reset_area: bad command %c.", pReset->command );
            break;

         case 'A':
            break;


         case 'M':
            just_loaded = FALSE;
            if( ( pMobIndex = get_mob_index( pReset->arg1 ) ) == NULL )
            {
               SHOW_AREA;
               bug( "Reset_area: 'M': bad vnum %d.", pReset->arg1 );
               continue;
            }

            if( ( pRoomIndex = get_room_index( pReset->arg3 ) ) == NULL )
            {
               SHOW_AREA;
               bug( "Reset_area: 'R': bad vnum %d.", pReset->arg3 );
               continue;
            }

            level = pMobIndex->level;
            if( pReset->count >= pReset->arg2 )
            {
               last = FALSE;
               break;
            }

            mob = create_mobile( pMobIndex );
            mob->npcdata->reset = pReset;
            pReset->count++;
            just_loaded = TRUE;

            /*
             * Check for pet shop.
             */
            {
               ROOM_INDEX_DATA *pRoomIndexPrev;
               pRoomIndexPrev = get_room_index( pRoomIndex->vnum - 1 );
               if( pRoomIndexPrev != NULL && pRoomIndexPrev->room_flags.test(RFLAG_PET_SHOP) )
                  mob->act.set(ACT_PET);
            }

            if( room_is_dark( pRoomIndex ) )
               SET_BIT( mob->affected_by, AFF_INFRARED );

            if( mob->in_room != NULL )
               char_to_room( mob, mob->in_room );
            else
               char_to_room( mob, pRoomIndex );

            level = URANGE( 0, mob->level - 2, LEVEL_HERO );
            last = TRUE;
            break;

         case 'O':
            if( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )
            {
               SHOW_AREA;
               bug( "Reset_area: 'O': bad vnum %d.", pReset->arg1 );
               continue;
            }

            if( ( pRoomIndex = get_room_index( pReset->arg3 ) ) == NULL )
            {
               SHOW_AREA;
               bug( "Reset_area: 'R': bad vnum %d.", pReset->arg3 );
               continue;
            }
            if( ( count_obj_room( pObjIndex, pRoomIndex->first_content ) >= pReset->arg2 )
                || ( ( count_obj_room( pObjIndex, pRoomIndex->first_content ) > 0 )
                     && ( ( pObjIndex->item_type == ITEM_BOARD )
                          || ( pObjIndex->item_type == ITEM_FOUNTAIN )
                          || ( pObjIndex->item_type == ITEM_FURNITURE )
                          || ( pObjIndex->item_type == ITEM_PORTAL )
                          || ( pObjIndex->item_type == ITEM_PIECE ) || ( IS_OBJ_STAT(pObjIndex,ITEM_EXTRA_RARE) ) ) ) )
            {
               last = FALSE;
               break;
            }

            level = pObjIndex->level;
            obj = create_object( pObjIndex, level > 1 ? number_fuzzy( level ) : level );
            obj->reset = pReset;
            pReset->count++;

            obj_to_room( obj, pRoomIndex );

            if( ( IS_OBJ_STAT(obj,ITEM_EXTRA_RARE) ) && !( ( number_percent(  ) < 2 ) && ( number_percent(  ) < 8 ) ) )
            {
               extract_obj( obj );
            }
            else if( ( pRoomIndex->room_flags.test(RFLAG_NO_REPOP) ) && ( pRoomIndex->first_person != NULL ) )
            {
               extract_obj( obj );
            }
            last = TRUE;
            break;

         case 'P':
            if( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )
            {
               SHOW_AREA;
               bug( "Reset_area: 'P': bad vnum %d.", pReset->arg1 );
               continue;
            }

            if( ( pObjToIndex = get_obj_index( pReset->arg3 ) ) == NULL )
            {
               SHOW_AREA;
               bug( "Reset_area: 'P': bad vnum %d.", pReset->arg3 );
               continue;
            }

            if( ( obj_to = get_obj_type( pObjToIndex ) ) == NULL
                || count_obj_list( pObjIndex, obj_to->first_in_carry_list ) >= pReset->arg2 )
            {
               last = FALSE;
               break;
            }

            level = pObjIndex->level;
            obj = create_object( pObjIndex, obj_to->level > 1 ? number_fuzzy( obj_to->level ) : obj_to->level );
            obj->reset = pReset;
            pReset->count++;

            obj_to_obj( obj, obj_to );

            last = TRUE;
            break;

         case 'G':
         case 'E':
            if( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )
            {
               SHOW_AREA;
               bug( "Reset_area: 'E' or 'G': bad vnum %d.", pReset->arg1 );
               continue;
            }

            if( !just_loaded )
               break;

            if( mob == NULL )
            {
               SHOW_AREA;
               bug( "Reset_area: 'E' or 'G': null mob for vnum %d.", pReset->arg1 );
               last = FALSE;
               break;
            }

            if( mob->npcdata->pIndexData->pShop != NULL )
            {
               int olevel;

               switch ( pObjIndex->item_type )
               {
                  default:
                     olevel = 0;
                     break;
                  case ITEM_PILL:
                     olevel = number_range( 0, 10 );
                     break;
                  case ITEM_POTION:
                     olevel = number_range( 0, 10 );
                     break;
                  case ITEM_SCROLL:
                     olevel = number_range( 5, 15 );
                     break;
                  case ITEM_WAND:
                     olevel = number_range( 10, 20 );
                     break;
                  case ITEM_STAFF:
                     olevel = number_range( 15, 25 );
                     break;
                  case ITEM_ARMOR:
                     olevel = number_range( 5, 15 );
                     break;
                  case ITEM_WEAPON:
                     olevel = number_range( 5, 15 );
                     break;
               }

               obj = create_object( pObjIndex, olevel > 1 ? number_fuzzy( olevel ) : olevel );
               obj->reset = pReset;
               pReset->count++;
               obj->extra_flags.set(ITEM_EXTRA_INVENTORY);
            }
            else
            {
               bool has_obj = FALSE;
               OBJ_DATA *carried;
               for( carried = mob->first_carry; carried; carried = carried->next_in_carry_list )
               {
                  if( carried->pIndexData == pObjIndex )
                  {
                     has_obj = TRUE;
                     break;
                  }
               }
               if( has_obj )
                  break;

               level = pObjIndex->level;
               obj = create_object( pObjIndex, level > 1 ? number_fuzzy( level ) : level );
               obj->reset = pReset;
               pReset->count++;
            }
            obj_to_char( obj, mob );
            if( pReset->command == 'E' )
            {
               char objname[MSL];
               one_argument( obj->name, objname );
               do_wear( mob, objname );
            }
            last = TRUE;
            break;

         case 'D':
            if( ( pRoomIndex = get_room_index( pReset->arg1 ) ) == NULL )
            {
               bug( "Reset_area: 'D': bad vnum %d.", pReset->arg1 );
               continue;
            }

            if( ( pexit = pRoomIndex->exit[pReset->arg2] ) == NULL )
               break;

            switch ( pReset->arg3 )
            {
               case 0:
                  pexit->exit_info.reset(EX_CLOSED);
                  pexit->exit_info.reset(EX_LOCKED);
                  break;

               case 1:
                  pexit->exit_info.set(EX_CLOSED);
                  pexit->exit_info.reset(EX_LOCKED);
                  break;

               case 2:
                  pexit->exit_info.set(EX_CLOSED);
                  pexit->exit_info.set(EX_LOCKED);
                  break;
            }

            last = TRUE;
            break;

         case 'R':
            if( ( pRoomIndex = get_room_index( pReset->arg1 ) ) == NULL )
            {
               SHOW_AREA;
               bug( "Reset_area: 'R': bad vnum %d.", pReset->arg1 );
               continue;
            }

            {
               int d0;
               int d1;

               for( d0 = 0; d0 < pReset->arg2 - 1; d0++ )
               {
                  d1 = number_range( d0, pReset->arg2 - 1 );
                  pexit = pRoomIndex->exit[d0];
                  pRoomIndex->exit[d0] = pRoomIndex->exit[d1];
                  pRoomIndex->exit[d1] = pexit;
               }
            }
            break;
      }
   }
   area_resetting_global = FALSE;
   return;
}

/*
 * Create an instance of a mobile.
 */
CHAR_DATA *create_mobile( MOB_INDEX_DATA * pMobIndex )
{
   CHAR_DATA *mob;
   char buf[255];
   float hold = 0;

   if( pMobIndex == NULL )
   {
      bug( "Create_mobile: NULL pMobIndex.", 0 );
      hang( "Create Mobile in db.c" );
   }

   if( pMobIndex->act.test(ACT_INTELLIGENT) )
   {
      /*
       * Only load one with the same name 
       */
#if !defined(machintosh) && !defined(MSDOS)
      snprintf( buf, 255, "%s n%i", pMobIndex->player_name, 1 );
#else
      snprintf( buf, 255, "n%i %s", 1, pMobIndex->player_name );
#endif

   }

   mob = new CHAR_DATA;
   mob->npcdata = new NPC_DATA;

   mob->npc = true;
   mob->npcdata->pIndexData = pMobIndex;

   if( pMobIndex->act.test(ACT_AGGRESSIVE) )
    aggro_list.push_back(mob);

   if( pMobIndex->act.test(ACT_INTELLIGENT) )
      mob->name = str_dup( buf );
   else
      mob->name = str_dup( pMobIndex->player_name );

   if( pMobIndex->script_name != &str_empty[0] ) /* Mob has a script attached */
   {
    mob->lua = new LUA_DATA;
    init_lua(mob);
   }

   mob->npcdata->short_descr = str_dup( pMobIndex->short_descr );
   mob->long_descr = str_dup( pMobIndex->long_descr );
   mob->description = pMobIndex->description;
   mob->npcdata->spec_fun = pMobIndex->spec_fun;
   mob->prompt = str_dup(DEFAULT_PROMPT);
   mob->level = pMobIndex->level;
   mob->act = pMobIndex->act;
   mob->affected_by = pMobIndex->affected_by;
   mob->alignment = pMobIndex->alignment;
   mob->sex = pMobIndex->sex;
   mob->npcdata->ac_mod = pMobIndex->ac_mod;
   mob->npcdata->hr_mod = pMobIndex->hr_mod;
   mob->npcdata->dr_mod = pMobIndex->dr_mod;

   mob->armor = interpolate( mob->level / 2, 100, -100 );
   hold = mob->armor;
   hold *= sysdata.mob_ac;
   mob->armor = (int)hold;

   mob->hitroll += (mob->get_level("psuedo") / 4);
   hold = mob->hitroll;
   hold *= sysdata.mob_hr;
   mob->hitroll = (int)hold;
   mob->damroll += (mob->get_level("psuedo") / 4);
   hold = mob->damroll;
   hold *= sysdata.mob_dr;
   mob->damroll = (int)hold;

   mob->max_hit = mob->level * 15 + number_range( mob->level * mob->level / 2, mob->level * mob->level / 1 );
   hold = mob->max_hit;
   hold *= sysdata.mob_hp;
   mob->max_hit = (int)hold;
   mob->hit = mob->max_hit;

   mob->exp = exp_for_mobile( mob->level, mob );

   /*
    * mana for mobs... 
    */
   mob->max_mana = mob->level * 25;
   hold = mob->max_mana;
   hold *= sysdata.mob_mp;
   mob->max_mana = (int)hold;
   mob->mana = mob->max_mana;

   /*
    * move for mobs...
    */
   mob->max_move = mob->level * 25;
   hold = mob->max_move;
   hold *= sysdata.mob_mv;
   mob->max_move = (int)hold;
   mob->move = mob->max_move;

   mob->npcdata->skills = pMobIndex->skills;
   mob->npcdata->cast = pMobIndex->cast;
   mob->npcdata->def = pMobIndex->def;
   mob->p_class = pMobIndex->p_class;
   mob->clan = pMobIndex->clan;
   mob->npcdata->strong_magic = pMobIndex->strong_magic;
   mob->npcdata->weak_magic = pMobIndex->weak_magic;
   mob->npcdata->resist = pMobIndex->resist;
   mob->npcdata->suscept = pMobIndex->suscept;
   mob->race_mods = pMobIndex->race_mods;
   mob->race = pMobIndex->race;
   mob->position = POS_STANDING;
   mob->saving_throw = (mob->get_level("psuedo") / 10);
   hold = mob->saving_throw;
   hold *= sysdata.mob_svs;
   mob->saving_throw = (int)hold;

//  Create group data for mob

   if( AI_MOB( mob ) )
   {
      NPC_GROUP_DATA *ngroup;
      ngroup = new NPC_GROUP_DATA;
      ngroup->next = NULL;
      ngroup->prev = NULL;
      ngroup->state = GRP_STATE_IDLE;
      ngroup->first_follower = NULL;
      ngroup->last_follower = NULL;
      ngroup->leader = mob;
      ngroup->seek_room = NULL;
      ngroup->enemies = str_dup( "none" );
      ngroup->last_fighting = str_dup( "none" );
      ngroup->wants = str_dup( "none" );
      ngroup->needs = str_dup( "none" );
      mob->npcdata->ngroup = ngroup;
      LINK( ngroup, first_npc_group, last_npc_group, next, prev );
   }
   return mob;
}



/*
 * Create an instance of an object.
 */
OBJ_DATA *create_object( OBJ_INDEX_DATA * pObjIndex, int level )
{
   OBJ_DATA *obj;
   AFFECT_DATA *af;
   AFFECT_DATA *new_af;
   int new_cost;
   int looper;

   if( level < 0 )
      level = 1;

   if( pObjIndex == NULL )
   {
      bug( "Create_object: NULL pObjIndex.", 0 );
      hang( "Creating Objext in db.c" );
   }

   obj = new OBJ_DATA;
   obj->pIndexData = pObjIndex;
   obj->in_room = NULL;

   if( pObjIndex->script_name != &str_empty[0] ) /* Obj has a script attached */
   {
    obj->script_name = str_dup(pObjIndex->script_name);
    obj->lua = new LUA_DATA;
    init_lua(obj);
   }

   if( pObjIndex->level < 3 )
   {
      if( pObjIndex->level == 2 )
      {
         obj->level = 1;
      }
      else
      {
         obj->level = level;
      }
   }
   else
   {
      obj->level = pObjIndex->level;
   }

   obj->wear_loc = -1;

   obj->name = str_dup( pObjIndex->name );
   obj->short_descr = str_dup( pObjIndex->short_descr );
   obj->long_descr = str_dup( pObjIndex->long_descr );
   obj->item_type = pObjIndex->item_type;
   obj->extra_flags = pObjIndex->extra_flags;
   obj->wear_flags = pObjIndex->wear_flags;
   obj->item_apply = pObjIndex->item_apply;
   obj->obj_fun = pObjIndex->obj_fun;
   obj->weight = pObjIndex->weight;
   if( (obj->speed = pObjIndex->speed) < 0.01 || (obj->speed = pObjIndex->speed) > 4.00 )
    obj->speed = number_speed();
   if( (obj->max_durability = pObjIndex->max_durability) < 1 )
    obj->max_durability = number_range(2,100);
   obj->durability = obj->max_durability;

   /*
    * We want to copy pObjIndex->affected to obj->index 
    */
   for( af = pObjIndex->first_apply; af != NULL; af = af->next )
   {
      /*
       * Create new_af, or use a free_affect 
       */
      new_af = new AFFECT_DATA;

      new_af->location = af->location;
      new_af->modifier = af->modifier;
      LINK( new_af, obj->first_apply, obj->last_apply, next, prev );
   }
   for( looper = 0; looper < 10; looper++ )
   {
      obj->value[looper] = pObjIndex->value[looper];
   }
   new_cost = get_item_value( obj );
   if( IS_OBJ_STAT(obj,ITEM_EXTRA_REMORT) )
      new_cost *= 2;
   if( IS_OBJ_STAT(obj,ITEM_EXTRA_ADEPT) )
      new_cost *= 3;
   if( IS_OBJ_STAT(obj,ITEM_EXTRA_RARE) )
      new_cost *= 10;
   obj->cost = new_cost;

   if( obj->item_type == ITEM_FOOD )
      obj->cost /= 200;
   obj->cost = UMAX( 10, obj->cost );

   /*
    * Mess with object properties.
    */
   switch ( obj->item_type )
   {
      default:
         bugf( "create_object: vnum %d bad type %d.", pObjIndex->vnum, obj->item_type );
         break;
      case ITEM_CHARM:
      case ITEM_ANVIL:
      case ITEM_COOKING_FIRE:
      case ITEM_ALCHEMY_LAB:
      case ITEM_FORGE:
      case ITEM_LOOM:
      case ITEM_SPELL_MATRIX:
      case ITEM_ENCHANTMENT:
      case ITEM_LIGHT:
      case ITEM_BEACON:
      case ITEM_TREASURE:
      case ITEM_FURNITURE:
      case ITEM_TRASH:
      case ITEM_CLUTCH:
         break;
      case ITEM_CONTAINER:
         obj->wear_flags.set(ITEM_TAKE);
      case ITEM_DRINK_CON:
      case ITEM_BOARD:
      case ITEM_KEY:
      case ITEM_FOOD:
      case ITEM_BOAT:
      case ITEM_CORPSE_NPC:
      case ITEM_CORPSE_PC:
      case ITEM_FOUNTAIN:
      case ITEM_PORTAL:
      case ITEM_PIECE:
      case ITEM_SOUL:
      case ITEM_TRIGGER:
      case ITEM_STAKE:
      case ITEM_QUEST:
      case ITEM_WRAPPED_PRESENT:
         break;

      case ITEM_SCROLL:
         obj->value[0] = number_fuzzy( obj->value[0] );
         break;

      case ITEM_WAND:
      case ITEM_STAFF:
         obj->value[0] = number_fuzzy( obj->value[0] );
         obj->value[1] = number_fuzzy( obj->value[1] );
         obj->value[2] = obj->value[1];
         break;

      case ITEM_WEAPON:
         obj->value[1] = number_fuzzy( obj->value[1] );
         obj->value[2] = number_fuzzy( obj->value[2] );
         break;

      case ITEM_ARMOR:
         obj->value[0] = number_fuzzy( level / 12 + 4 );
         break;

      case ITEM_POTION:
      case ITEM_PILL:
         obj->value[0] = number_fuzzy( number_fuzzy( obj->value[0] ) );
         break;

      case ITEM_MONEY:
         obj->value[0] = obj->cost;
         break;
   }

   if( pObjIndex->value[7] > 0 )
      obj->timer = pObjIndex->value[7];

   return obj;
}


/*
 * Get an extra description from a list.
 */
char *get_extra_descr( const char *name, EXTRA_DESCR_DATA * ed )
{
   for( ; ed != NULL; ed = ed->next )
   {
      if( is_name( name, ed->keyword ) )
         return ed->description;
   }
   return NULL;
}



/*
 * Translates mob virtual number to its mob index struct.
 * Hash table lookup.
 */
MOB_INDEX_DATA *get_mob_index( int vnum )
{
   MOB_INDEX_DATA *pMobIndex;

   for( pMobIndex = mob_index_hash[vnum % MAX_KEY_HASH]; pMobIndex != NULL; pMobIndex = pMobIndex->next )
   {
      if( pMobIndex->vnum == vnum )
         return pMobIndex;
   }

   if( fBootDb )
   {
      bug( "Get_mob_index: bad vnum %d.", vnum );
      hang( "Get mob index in db.c" );
   }

   return NULL;
}



/*
 * Translates mob virtual number to its obj index struct.
 * Hash table lookup.
 */
OBJ_INDEX_DATA *get_obj_index( int vnum )
{
   OBJ_INDEX_DATA *pObjIndex;

   for( pObjIndex = obj_index_hash[vnum % MAX_KEY_HASH]; pObjIndex != NULL; pObjIndex = pObjIndex->next )
   {
      if( pObjIndex->vnum == vnum )
         return pObjIndex;
   }

   if( fBootDb )
   {
      bug( "Get_obj_index: bad vnum %d.", vnum );
      hang( "Get Object Index in db.c" );
   }

   return NULL;
}



/*
 * Translates mob virtual number to its room index struct.
 * Hash table lookup.
 */
ROOM_INDEX_DATA *get_room_index( int vnum )
{
   ROOM_INDEX_DATA *pRoomIndex;

   for( pRoomIndex = room_index_hash[vnum % MAX_KEY_HASH]; pRoomIndex != NULL; pRoomIndex = pRoomIndex->next )
   {
      if( pRoomIndex->vnum == vnum )
         return pRoomIndex;
   }

   if( fBootDb )
   {
      bug( "Get_room_index: bad vnum %d.", vnum );
/*	exit( 1 );   */
   }

   return NULL;
}



/*
 * Read a letter from a file.
 */
char fread_letter( FILE * fp )
{
   char c;

   do
   {
      c = getc( fp );
   }
   while( isspace( c ) );

   return c;
}

/*
 * Read a float number from a file. Turn the result into a float value.
 * Taken from AFKMud by Samson
 */
float fread_float( FILE *fp )
{
 float number;
 bool sign, decimal;
 char c;
 int place = 0;
 
 do
 {
  if( feof(fp) )
  {
   bug("fread_float: EOF encountered on read.",0);
   if( fBootDb )
   {
    bug( "Corrupt file somewhere.",0);
    exit(1);
   }
   return 0;
  }
  c = getc( fp );
 }
 while( isspace(c) );
 
 number = 0;
 
 sign   = FALSE;
 decimal = FALSE;

 if( c == '+' )
  c = getc( fp );
 else if( c == '-' )
 {
  sign = TRUE;
  c = getc( fp );
 }
 
 if( !isdigit(c) )
 {
  bug( "Fread_float: bad format. (%c)", c );
  if( fBootDb )
   exit( 1 );
  return 0;
 }
  
 while( 1 )
 {
  if( c == '.' || isdigit(c) )
  {
   if( c == '.' )
   {
    decimal = TRUE;
    c = getc( fp );
   }

   if( feof(fp) )
   {
    bug("fread_float: EOF encountered on read.",0);
    if( fBootDb )
     exit( 1 );
    return number;
   }
   if( !decimal )
    number = number * 10 + c - '0';
   else
   {
    place++;
    number += pow( 10.0, ( -1 * place ) ) * ( c - '0' );
   }
   c      = getc( fp );
  }
  else break;
 }

 if( sign )
  number = 0 - number;

 if( c == '|' )
  number += fread_float( fp );
 else if( c != ' ' )
  ungetc( c, fp );

 return number;
}

/*
 * Read a number from a file.
 */
int fread_number( FILE * fp )
{
   int number;
   bool sign;
   char c;

   do
   {
      c = getc( fp );
   }
   while( isspace( c ) );

   number = 0;

   sign = FALSE;
   if( c == '+' )
   {
      c = getc( fp );
   }
   else if( c == '-' )
   {
      sign = TRUE;
      c = getc( fp );
   }

   if( !isdigit( c ) )
   {
      char error_buf[MSL];
      snprintf( error_buf, MSL, "%c", c );
      bug_string( "Fread_number: looking for a digit, found a %s.", error_buf );
      hang( "Error in fread_number" );
   }

   while( isdigit( c ) )
   {
      number = number * 10 + c - '0';
      c = getc( fp );
   }

   if( sign )
      number = 0 - number;

   if( c == '|' )
      number += fread_number( fp );
   else if( c != ' ' )
      ungetc( c, fp );

   return number;
}


/*
 * Read to end of line (for comments).
 */

/* Spec: fixed to handle EOF more gracefully */

void fread_to_eol( FILE * fp )
{
   char c;

   do
   {
      c = getc( fp );
   }
   while( c != '\n' && c != '\r' && c != EOF );

   while( c == '\n' || c == '\r' )
   {
      c = getc( fp );
   }

   if( c == EOF )
   {
      bugf( "fread_to_eol: EOF" );
      return;
   }

   ungetc( c, fp );
   return;
}


/* Same as above, but returns the rest of the line */

/* Spec: fixed to handle EOF more gracefully */

char *fsave_to_eol( FILE * fp )
{
   char string[MAX_INPUT_LENGTH + 1];
   int a;
   char c;

   a = 0;
   do
   {
      string[a++] = getc( fp );

   }
   while( a < MAX_INPUT_LENGTH && string[a - 1] != '\n' && string[a - 1] != '\r' && string[a - 1] != EOF );

   if( string[a - 1] == EOF )
   {
      bugf( "fsave_to_eol: EOF" );
      string[a - 1] = 0;
      return str_dup( string );
   }

   if( a == MAX_INPUT_LENGTH )
      fread_to_eol( fp );
   else
   {
      c = string[a - 1];
      while( c == '\n' || c == '\r' )
         c = getc( fp );

      if( c == EOF )
         bugf( "fsave_to_eol: EOF" );
      else
         ungetc( c, fp );
   }
   string[a] = '\0';

   return str_dup( string );
}







/*
 * Read one word (into static buffer).
 */
char *fread_word( FILE * fp )
{
   static char word[MAX_INPUT_LENGTH];
   char *pword;
   char cEnd;

   do
   {
      if( feof( fp ) )
      {
         bug( "fread_word: EOF encountered on read.\r\n", 0 );
         if( fBootDb )
            kill( getpid(  ), SIGQUIT );
         strcpy( word, "" );
         return word;
      }
      cEnd = getc( fp );
   }
   while( isspace( cEnd ) );

   if( cEnd == '\'' || cEnd == '"' )
   {
      pword = word;
   }
   else
   {
      word[0] = cEnd;
      pword = word + 1;
      cEnd = ' ';
   }

   for( ; pword < word + MAX_INPUT_LENGTH; pword++ )
   {
      if( feof( fp ) )
      {
         bug( "fread_word: EOF encountered on read.\r\n", 0 );
         if( fBootDb )
            kill( getpid(  ), SIGQUIT );
         *pword = '\0';
         return word;
      }
      *pword = getc( fp );
      if( cEnd == ' ' ? isspace( *pword ) : *pword == cEnd )
      {
         if( cEnd == ' ' )
            ungetc( *pword, fp );
         *pword = '\0';
         return word;
      }
   }

   bug( "Fread_word: word too long", 0 );
   kill( getpid(  ), SIGQUIT );
   return NULL;
}



void *_getmem( int size, const char *caller, int log )
{
   void *mem;

   if( !( mem = malloc( size ) ) )
   {
      fprintf( stderr, "Out of memory.\n" );
      raise( SIGSEGV );
   }
   memset( mem, 0, size );

   if( log && mem_log )
      log_f( "getmem(%d)=%p from %s", size, mem, caller );

   return mem;
}

DO_FUN(do_areas)
{
   char buf[MAX_STRING_LENGTH];
   char msg[MAX_STRING_LENGTH];
   char arg1[MSL];
   short foo;
   AREA_DATA *pArea;
   list<AREA_DATA *>::iterator li;
   bool fall = FALSE;

   argument = one_argument( argument, arg1 );

   if( !str_cmp( arg1, "all" ) )
      fall = TRUE;
   snprintf( buf, MSL, "@@W" mudnamecolor " AREA LISTING\r\n" );
   strncat( buf, "+-------+------------+------------------------------------------------+\r\n", MSL );
   strncat( buf, "| @@yLevel@@W |            |                                                |\r\n", MSL );
   strncat( buf, "| @@yrange@@W |   @@yAuthor@@W   |      @@yName of Area@@W                              |\r\n", MSL );
   strncat( buf, "+-------+------------+------------------------------------------------+\r\n", MSL );

   foo = 0;
   for( li = area_list.begin(); li != area_list.end(); li++ )
   {
      pArea = *li;
      if( pArea->flags.test(AFLAG_NOSHOW) || pArea->flags.test(AFLAG_BUILDING) )
         continue;   /* for non-finished areas - don't show */
      if( ( !fall )
          && ( ( pArea->min_level > ( ch->get_level("psuedo") ) ) || ( pArea->max_level < ( ch->get_level("psuedo") ) ) ) )
         continue;

      foo++;
      snprintf( msg, MSL, " %s %12s          %s\r\n", pArea->level_label, capitalize( pArea->owner ), pArea->name );
      strncat( buf, msg, MSL-1 );
   }
   snprintf( msg, MSL, "@@R%d Areas listed.\r\n@@N Type areas all to list the entire " mudnamecolor " realm.\r\n@@N", foo );
   strncat( buf, msg, MSL-1 );
   send_to_char( buf, ch );
   return;
}

void perm_update(  )
{
   /*
    * output perm usage to log file
    */
   FILE *po;
   char *strtime;
   po = file_open( "perm.out", "a" );


   strtime = ctime( &current_time );
   strtime[strlen( strtime ) - 1] = '\0';

   fprintf( po, "%s :: Perms   %5d blocks  of %7d bytes.\r\n", strtime, nAllocPerm, sAllocPerm );
   file_close( po );
   return;
}


DO_FUN(do_memory)
{
   char buf[MAX_STRING_LENGTH];

   if( !str_cmp( argument, "defrag" ) )
   {
      send_to_char( "Defragmenting SSM heap.", ch );
      log_f( "SSM: %s called defrag_heap.", ch->name.c_str() );
      defrag_heap(  );
      return;
   }

   if( !str_cmp( argument, "log" ) )
   {
      if( get_trust( ch ) < MAX_LEVEL )
      {
         send_to_char( "Not at your level.\r\n", ch );
         return;
      }

      if( mem_log )
      {
         mem_log = FALSE;
         send_to_char( "Memory logging is now OFF.\r\n", ch );
         log_f( "%s turned off memory logging", ch->name.c_str() );
         return;
      }
      else
      {
         mem_log = TRUE;
         send_to_char( "Memory logging is now ON.. remember to turn it off!\r\n", ch );
         log_f( "%s turned on memory logging", ch->name.c_str() );
         return;
      }
   }

   

   snprintf( buf, MSL, "Affects %5d\r\n", static_cast<int>(affect_list.size()) );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Areas   %5d\r\n", static_cast<int>(area_list.size()) );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "ExDes   %5d\r\n", static_cast<int>(exdesc_list.size()) );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Exits   %5d\r\n", static_cast<int>(exit_list.size()) );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Helps   %5d\r\n", count_helps() );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Lua     %5d\r\n", static_cast<int>(lua_list.size()) );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Mobs    %5d\r\n", static_cast<int>(mob_index_list.size()) );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Objs    %5d\r\n", static_cast<int>(obj_index_list.size()) );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Resets  %5d\r\n", static_cast<int>(reset_list.size()) );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Rooms   %5d\r\n", static_cast<int>(room_index_list.size()) );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Shops   %5d\r\n", static_cast<int>(shop_list.size()) );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Shared String Info:\r\n" );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Strings           %5ld strings of %7ld bytes (max %ld).\r\n", nAllocString, sAllocString, MAX_STRING );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Overflow Strings  %5ld strings of %7ld bytes.\r\n", nOverFlowString, sOverFlowString );
   send_to_char( buf, ch );
   if( Full )
   {
      send_to_char( "Shared String Heap is full, increase MAX_STRING.\r\n", ch );
      snprintf( buf, MSL, "Overflow high-water-mark is %ld bytes.\r\n", hwOverFlow );
      send_to_char( buf, ch );
   }

   snprintf( buf, MSL, "Perms             %5d blocks  of %7d bytes.\r\n", nAllocPerm, sAllocPerm );
   send_to_char( buf, ch );

   snprintf( buf, MSL, "Freelist Info: Gets: %5d Puts:   %5d\r\n",free_get,free_put);
   send_to_char( buf, ch );

   snprintf( buf, MSL, "File Streams: Opens: %5d Closes: %5d\r\n",fp_open,fp_close);
   send_to_char( buf, ch );

   return;
}


DO_FUN(do_status)
{
   char buf[MAX_STRING_LENGTH];

   send_to_char( "             Status Report for Ack! Mud:\r\n", ch );
   send_to_char( "             ---------------------------\r\n\r\n", ch );
   do_time( ch, "" );

   send_to_char( "\r\n", ch );
   send_to_char( "The following counts are for *distinct* mobs/objs/rooms, not a count\r\n", ch );
   send_to_char( "of how many are actually in the game at this time.\r\n", ch );
   snprintf( buf, MSL, "Areas   %5d\r\n", static_cast<int>(area_list.size()) );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Helps   %5d\r\n", count_helps() );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Mobs    %5d\r\n", static_cast<int>(mob_index_list.size()) );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Objs    %5d\r\n", static_cast<int>(obj_index_list.size()) );
   send_to_char( buf, ch );
   snprintf( buf, MSL, "Rooms   %5d\r\n", static_cast<int>(room_index_list.size()) );
   send_to_char( buf, ch );

   return;
}




/*
 * Generate a random float for weapon speed.
 */
float number_speed(void)
{
 float s1, s2, s3, value = 0.00;

 s1 = number_range(1,4);
 s2 = ((number_range(0,4) + 0.00) / 10);
 s3 = ((number_range(0,4) + 0.00) / 100);
 value = s1+s2+s3;

 if (value > 4.00)
  value = 4.00;

 return value;
}

/*
 * Stick a little fuzz on a number.
 */
int number_fuzzy( int number )
{
   switch ( number_bits( 2 ) )
   {
      case 0:
         number -= 1;
         break;
      case 3:
         number += 1;
         break;
   }

   return UMAX( 1, number );
}



/*
 * Generate a random number.
 */
int number_range( int from, int to )
{
   int power;
   int number;

   if( ( to = to - from + 1 ) <= 1 )
      return from;

   for( power = 2; power < to; power <<= 1 )
      ;

   while( ( number = number_mm(  ) & ( power - 1 ) ) >= to )
      ;

   return from + number;
}



/*
 * Generate a percentile roll.
 */
int number_percent( void )
{
   int percent;

   while( ( percent = number_mm(  ) & ( 128 - 1 ) ) > 99 )
      ;

   return 1 + percent;
}



/*
 * Generate a random door.
 */
int number_door( void )
{
   int door;

   while( ( door = number_mm(  ) & ( 8 - 1 ) ) > 5 )
      ;

   return door;
}



int number_bits( int width )
{
   return number_mm(  ) & ( ( 1 << width ) - 1 );
}



/*
 * I've gotten too many bad reports on OS-supplied random number generators.
 * This is the Mitchell-Moore algorithm from Knuth Volume II.
 * Best to leave the constants alone unless you've read Knuth.
 * -- Furey
 */
static int rgiState[2 + 55];

void init_mm(  )
{
   int *piState;
   int iState;

   piState = &rgiState[2];

   piState[-2] = 55 - 55;
   piState[-1] = 55 - 24;

   piState[0] = ( ( int )current_time ) & ( ( 1 << 30 ) - 1 );
   piState[1] = 1;
   for( iState = 2; iState < 55; iState++ )
   {
      piState[iState] = ( piState[iState - 1] + piState[iState - 2] ) & ( ( 1 << 30 ) - 1 );
   }
   return;
}



int number_mm( void )
{
   int *piState;
   int iState1;
   int iState2;
   int iRand;

   piState = &rgiState[2];
   iState1 = piState[-2];
   iState2 = piState[-1];
   iRand = ( piState[iState1] + piState[iState2] ) & ( ( 1 << 30 ) - 1 );
   piState[iState1] = iRand;
   if( ++iState1 == 55 )
      iState1 = 0;
   if( ++iState2 == 55 )
      iState2 = 0;
   piState[-2] = iState1;
   piState[-1] = iState2;
   return iRand >> 6;
}



/*
 * Roll some dice.
 */
int dice( int number, int size )
{
   int idice;
   int sum;

   switch ( size )
   {
      case 0:
         return 0;
      case 1:
         return number;
   }

   for( idice = 0, sum = 0; idice < number; idice++ )
      sum += number_range( 1, size );

   return sum;
}



/*
 * Simple linear interpolation.
 */
int interpolate( int level, int value_00, int value_32 )
{
   return value_00 + level * ( value_32 - value_00 ) / 32;
}




/*
 * Append a string to a file.
 */
void append_file( CHAR_DATA * ch, char *file, char *str )
{
   FILE *fp;
   char buf[MSL];

   if( IS_NPC( ch ) || str[0] == '\0' )
      return;

   if( ( fp = file_open( file, "a" ) ) == NULL )
   {
      file_close(fp);
      perror( file );
      send_to_char( "Could not open the file!\r\n", ch );
   }
   else
   {
      snprintf( buf, MSL, "[%5d] %s: %s\n", ch->in_room ? ch->in_room->vnum : 0, ch->name.c_str(), str );
      fprintf(fp,"%s",buf);

      buf[strlen(buf)-1] = '\0'; /* Strip newline */

      if( !str_cmp(file,TYPO_FILE) )
       monitor_chan(buf,MONITOR_TYPO);
      else if( !str_cmp(file,IDEA_FILE) )
       monitor_chan(buf,MONITOR_IDEA);
      else if( !str_cmp(file,BUG_FILE) )
       monitor_chan(buf,MONITOR_BUG);

      file_close( fp );
   }

   return;
}

/*
 * Nice little functions that limit the amount of typing you have to do with    
 * each and every log File entry and bug report.---Flar
 */
void bugf( char *fmt, ... )
{
   char buf[MSL];
   va_list args;
   va_start( args, fmt );
   vsnprintf( buf, sizeof( buf ), fmt, args );
   va_end( args );

   bug( buf, 0 );
}

void log_f( char *fmt, ... )
{
   char buf[2 * MSL];
   va_list args;
   va_start( args, fmt );
   vsnprintf( buf, sizeof( buf ), fmt, args );
   va_end( args );

   log_string( buf );
}



/*
 * Reports a bug.
 */
void bug( const char *str, int param )
{
   char buf[MAX_STRING_LENGTH];
   FILE *fp;

   if( fpArea != NULL )
   {
      int iLine;
      int iChar;

      if( fpArea == stdin )
      {
         iLine = 0;
      }
      else
      {
         iChar = ftell( fpArea );
         fseek( fpArea, 0, 0 );
         for( iLine = 0; ftell( fpArea ) < iChar; iLine++ )
         {
            while( getc( fpArea ) != '\n' )
               ;
         }
         fseek( fpArea, iChar, 0 );
      }

      snprintf( buf, MSL, "[*****] FILE: %s LINE: %d", strArea, iLine );
      log_string( buf );

      if( ( fp = file_open( SHUTDOWN_FILE, "a" ) ) != NULL )
      {
         fprintf( fp, "[*****] %s\n", buf );
         file_close( fp );
      }
   }

   if( param > 0 )
    snprintf( buf, MSL, "[*****] BUG: %s %d", str, param );
   else
    snprintf( buf, MSL, "[*****] BUG: %s", str );
   log_string( buf );

   if( ( fp = file_open( BUG_FILE, "a" ) ) != NULL )
   {
      fprintf( fp, "%s\n", buf );
      file_close( fp );
   }

   return;
}

void bug_string( const char *str, const char *str2 )
{
   char buf[MAX_STRING_LENGTH];
   FILE *fp;

   if( fpArea != NULL )
   {
      int iLine;
      int iChar;

      if( fpArea == stdin )
      {
         iLine = 0;
      }
      else
      {
         iChar = ftell( fpArea );
         fseek( fpArea, 0, 0 );
         for( iLine = 0; ftell( fpArea ) < iChar; iLine++ )
         {
            while( getc( fpArea ) != '\n' )
               ;
         }
         fseek( fpArea, iChar, 0 );
      }

      snprintf( buf, MSL, "[*****] FILE: %s LINE: %d", strArea, iLine );
      log_string( buf );

      if( ( fp = file_open( SHUTDOWN_FILE, "a" ) ) != NULL )
      {
         fprintf( fp, "[*****] %s\n", buf );
         file_close( fp );
      }
   }

   snprintf( buf, MSL, "[*****] BUG: %s %s", str, str2 );
   log_string( buf );

   if( ( fp = file_open( BUG_FILE, "a" ) ) != NULL )
   {
      fprintf( fp, "%s\n", buf );
      file_close( fp );
   }

   return;
}



/*
 * Writes a string to the log.
 */
void log_string( const char *str )
{
   char *strtime;

   strtime = ctime( &current_time );
   strtime[strlen( strtime ) - 1] = '\0';
   fprintf( stderr, "%s :: %s\n", strtime, str );
   monitor_chan(str,MONITOR_LOG);
   return;
}

void message_update( void )
{
   RESET_DATA *pReset;
   AREA_DATA *pArea;
   ROOM_INDEX_DATA *pRoom;
   CHAR_DATA *ch;
   list<AREA_DATA *>::iterator li;

   for( li = area_list.begin(); li != area_list.end(); li++ )
   {
      pArea = *li;
      for( pReset = pArea->first_reset; pReset != NULL; pReset = pReset->next )
      {
         if( pReset->command != 'A' )
            continue;

         if( ( pRoom = get_room_index( pReset->arg1 ) ) == NULL )
         {
            bug( "message_update, no room %d found.", pReset->arg1 );
            continue;
         }

         for( ch = pRoom->first_person; ch != NULL; ch = ch->next_in_room )
            if( ch->level >= pReset->arg2 && ch->level <= pReset->arg3 )
               send_to_char( pReset->notes, ch );
      }
   }
   return;
}

bool char_exists( char *argument )
{
 FILE *fp;
 char buf[MAX_STRING_LENGTH];
 bool found = FALSE;

 snprintf( buf, MSL, "%s%s%s%s", PLAYER_DIR, initial( argument ), "/", capitalize( argument ) );

 if( ( fp = file_open( buf, "r" ) ) != NULL )
  found = TRUE;

 file_close( fp );
 
 return found;
}

int bv_log( int n )
{
 int result = 0;

 while( n > 0 )
 {
  n >>= 1;
  result++;
 }

 return result;
}

void check_chistory( CHAR_DATA *ch, int channel )
{
 short x,y = 0;
 bool found = FALSE;
 char buf[MAX_STRING_LENGTH];

 x = (bv_log(channel)-1);

 for( y = 0; y < MAX_HISTORY; y++ )
  if( chan_history.message[x][y][0] != '\0' )
  {
   if( IS_IMMORTAL(ch) )
   {
    found = TRUE;
    snprintf(buf,MSL,"[%s",ctime(&chan_history.time[x][y]));
    buf[(strlen(buf)-1)] = '\0';                  /* I realize how ugly this chunk looks but it was         */
    strncat(buf,"] ",MSL);                        /* necessary to get around ctime adding a newline --Kline */
    strncat(buf,chan_history.message[x][y],MSL-1);
    send_to_char(buf,ch);
   }
   else if( ch->logon <= chan_history.time[x][y] )
   {
    switch( channel )
    {
     default: found = TRUE; send_to_char(chan_history.message[x][y],ch); break;
     case CHANNEL_DIPLOMAT:
      if( ch->act.test(ACT_CDIPLOMAT) )
      {
       found = TRUE;
       send_to_char(chan_history.message[x][y],ch);
      }
      break;
     case CHANNEL_REMORTTALK:
      if( IS_REMORT(ch) )
      {
       found = TRUE;
       send_to_char(chan_history.message[x][y],ch);
      }
      break;
     case CHANNEL_YELL:
      if( !str_cmp(ch->in_room->area->name,chan_history.aname[x][y]) )
      {
       found = TRUE;
       send_to_char(chan_history.message[x][y],ch);
      }
      break;
     case CHANNEL_CLAN:
      if( ch->clan == chan_history.cbit[x][y] )
      {
       send_to_char(buf,ch);
       found = TRUE;
       send_to_char(chan_history.message[x][y],ch);
      }
      break;
     case CHANNEL_RACE:
      if( ch->race == chan_history.cbit[x][y] )
      {
       found = TRUE;
       send_to_char(chan_history.message[x][y],ch);
      }
      break;
     case CHANNEL_FAMILY:
      if( IS_VAMP(ch) && ch->pcdata->super->bloodline == chan_history.cbit[x][y] )
      {
       found = TRUE;
       send_to_char(chan_history.message[x][y],ch);
      }
      break;
     case CHANNEL_HOWL:
      if( IS_WOLF(ch) && ch->pcdata->super->bloodline == chan_history.cbit[x][y] )
      {
       found = TRUE;
       send_to_char(chan_history.message[x][y],ch);
      }
     break;
    }
   }
  }

 if( !found )
  send_to_char("No history available for that channel yet.\r\n",ch);

 return;
}

void update_chistory( CHAR_DATA *ch, char *argument, int channel )
{
 short x,y = 0;

 x = (bv_log(channel)-1);

 for( y = 0; y < MAX_HISTORY; y++ )
 {
  if( chan_history.message[x][y] == '\0' )
  {
   snprintf(chan_history.message[x][y],MSL,"%s: %s@@N\r\n",ch->get_name(),argument);
   chan_history.time[x][y] = current_time;
   snprintf(chan_history.aname[x][y],128,"none");
   chan_history.cbit[x][y] = -1;

   switch( channel )
   {
    default: break;
    case CHANNEL_YELL:   snprintf(chan_history.aname[x][y],128,"%s",ch->in_room->area->name);      break;
    case CHANNEL_CLAN:   chan_history.cbit[x][y] = ch->clan;                                       break;
    case CHANNEL_RACE:   chan_history.cbit[x][y] = ch->race;                                       break;
    case CHANNEL_FAMILY: if( !IS_NPC(ch) ) chan_history.cbit[x][y] = ch->pcdata->super->bloodline; break;
    case CHANNEL_HOWL:   if( !IS_NPC(ch) ) chan_history.cbit[x][y] = ch->pcdata->super->bloodline; break;
   }
   break;
  }
  if( y == (MAX_HISTORY -1) )
  {
   int i = 0;

   for( i = 1; i < MAX_HISTORY; i++ )
   {
    snprintf(chan_history.message[x][(i-1)],MSL,"%s",chan_history.message[x][i]);
    chan_history.time[x][(i-1)] = chan_history.time[x][i];
    snprintf(chan_history.aname[x][(i-1)],128,"%s",chan_history.aname[x][i]);
    chan_history.cbit[x][(i-1)] = chan_history.cbit[x][i];
   }

   chan_history.message[x][y][0] = '\0';
   chan_history.time[x][y] = 0;
   chan_history.aname[x][y][0] = '\0';
   chan_history.cbit[x][y] = 0;

   snprintf(chan_history.message[x][y],MSL,"%s: %s@@N\r\n",ch->get_name(),argument);
   chan_history.time[x][y] = current_time;
   snprintf(chan_history.aname[x][y],128,"none");
   chan_history.cbit[x][y] = -1;

   switch( channel )
   {
    default: break;
    case CHANNEL_YELL:   snprintf(chan_history.aname[x][y],128,"%s",ch->in_room->area->name);      break;
    case CHANNEL_CLAN:   chan_history.cbit[x][y] = ch->clan;                                       break;
    case CHANNEL_RACE:   chan_history.cbit[x][y] = ch->race;                                       break;
    case CHANNEL_FAMILY: if( !IS_NPC(ch) ) chan_history.cbit[x][y] = ch->pcdata->super->bloodline; break;
    case CHANNEL_HOWL:   if( !IS_NPC(ch) ) chan_history.cbit[x][y] = ch->pcdata->super->bloodline; break;
   }
  }
 }
 return;
}

int count_helps( void )
{
 char buf[MSL];
 char tmp[MSL];

 snprintf(tmp,MSL,"expr `ls -1 -R %s | wc -l` - 26",HELP_DIR);
 snprintf(buf,MSL,"%s",_popen(tmp));

 return atoi(buf);
}

int count_skills( void )
{
 int cnt = 0;

 while( skill_table[cnt].name != NULL )
  cnt++;

 return cnt;
}

char *find_helps( const char *search, bool imm )
{
 static char ret[MSL];
 char tmp[MSL] = {'\0'};
 char split[MSL] = {'\0'};
 string str;
 size_t pos;

 if( !isalpha(*search) )
  return "You can only search for letters.\r\n";

 if( strlen(search) < 2 )
  return "Searches must be at least two letters.\r\n";

 if( imm )
  snprintf(split,MSL,"find %s -maxdepth 2 -iname \\*%s\\* -printf '%%f '",HELP_DIR,search);
 else
  snprintf(split,MSL,"find %s -maxdepth 2 -iname \\*%s\\*.%s -printf '%%f '",HELP_DIR,search,HELP_MORT);
 snprintf(tmp,MSL,"%s",_popen(split));

 str = tmp;

 if( str.empty() )
  return "Nothing found.\r\n";

 snprintf(tmp,MSL,".%s",HELP_MORT);
 while( (pos = str.find(tmp)) < str.size() )
  str.replace(pos,strlen(tmp),"");

 if( imm )
 {
  snprintf(tmp,MSL,".%s",HELP_IMM);
  while( (pos = str.find(tmp)) < str.size() )
   str.replace(pos,strlen(tmp),"");
 }

 snprintf(ret,MSL,"Found the following possible matches:\r\n%s\r\n",str.c_str());
 return ret;
}

char *grep_helps( const char *search, bool imm )
{
 static char ret[MSL];
 char tmp[MSL] = {'\0'};
 char split[MSL] = {'\0'};
 string str;
 size_t pos;

 if( !isalpha(*search) )
  return "You can only search for letters.\r\n";

 if( strlen(search) < 2 )
  return "Searches must be at least two letters.\r\n";

 if( imm )
  snprintf(split,MSL,"grep -i -l -R '%s' %s*/* | cut -c %d-",search,HELP_DIR,static_cast<int>(strlen(HELP_DIR)+3));
 else
  snprintf(split,MSL,"grep -i -l -R '%s' %s*/*.%s | cut -c %d-",search,HELP_DIR,HELP_MORT,static_cast<int>(strlen(HELP_DIR)+3));
 snprintf(tmp,MSL,"%s",_popen(split));

 str = tmp;

 if( str.empty() )
  return "Nothing found.\r\n";

 snprintf(tmp,MSL,".%s",HELP_MORT);
 while( (pos = str.find(tmp)) < str.size() )
  str.replace(pos,strlen(tmp),"");

 if( imm )
 {
  snprintf(tmp,MSL,".%s",HELP_IMM);
  while( (pos = str.find(tmp)) < str.size() )
   str.replace(pos,strlen(tmp),"");
 }

 snprintf(ret,MSL,"Found the following possible matches:\r\n%s",str.c_str());
 return ret;
}

char *_popen( const char *search )
{
 static char ret[MSL];
 char tmp[MSL];
 FILE *fp;

 ret[0] = '\0';
 tmp[0] = '\0';
 if( fpReserve != NULL )
 {
  fclose(fpReserve);
  fpReserve = NULL;
 }

 if( (fp = popen(search,"r")) == NULL )
  snprintf(ret,MSL,"%s","");
 else
 {
  while( fgets(tmp,MSL,fp) )
  {
   if( ret[0] == '\0' )
    snprintf(ret,MSL,"%s",tmp);
   else
    strncat(ret,tmp,MSL);
  }
 }

 fp_open++;
 fp_close++;

 fclose(fp);
 if( fpReserve == NULL )
 {
  fpReserve = fopen(NULL_FILE,"r");
 }

 return ret;
}

FILE *file_open( const char *file, const char *opt )
{
 FILE *fp;

 if( fpReserve != NULL )
 {
  fclose(fpReserve);
  fpReserve = NULL;
 }
 fp = fopen(file,opt);

 return fp;
}

void file_close( FILE *file )
{
 if( file != NULL )
 {
  fflush(file);
  fclose(file);
 }
 if( fpReserve == NULL )
 {
  fpReserve = fopen(NULL_FILE,"r");
 }

 return;
}

void clear_lists( void )
{
 DESCRIPTOR_DATA *d, *d_next;
 extern hash_table *hash_changed_vnums;

 for( d = first_desc; d != NULL; d = d_next )
 {
  d_next = d->next;
  delete d;
 }

 for_each( affect_list.begin(),     affect_list.end(),     DeleteObject() );
 for_each( area_list.begin(),       area_list.end(),       DeleteObject() );
 for_each( ban_list.begin(),        ban_list.end(),        DeleteObject() );
 for_each( board_list.begin(),      board_list.end(),      DeleteObject() );
 for_each( build_dat_list.begin(),  build_dat_list.end(),  DeleteObject() );
 for_each( char_list.begin(),       char_list.end(),       DeleteObject() );
 for_each( disabled_list.begin(),   disabled_list.end(),   DeleteObject() );
 for_each( exdesc_list.begin(),     exdesc_list.end(),     DeleteObject() );
 for_each( exit_list.begin(),       exit_list.end(),       DeleteObject() );
 for_each( file_list.begin(),       file_list.end(),       DeleteObject() );
 for_each( mob_index_list.begin(),  mob_index_list.end(),  DeleteObject() );
 for_each( note_list.begin(),       note_list.end(),       DeleteObject() );
 for_each( obj_list.begin(),        obj_list.end(),        DeleteObject() );
 for_each( obj_index_list.begin(),  obj_index_list.end(),  DeleteObject() );
 for_each( reset_list.begin(),      reset_list.end(),      DeleteObject() );
 for_each( room_index_list.begin(), room_index_list.end(), DeleteObject() );
 for_each( ruler_list.begin(),      ruler_list.end(),      DeleteObject() );
 for_each( shop_list.begin(),       shop_list.end(),       DeleteObject() );

 comlog(NULL,true,0,NULL);
 fclose(fpReserve);
 delete_hash_table(hash_changed_vnums);
 free(string_space);
 free(social_table);
#ifdef IMC
 free_imcdata(true);
#endif
}
