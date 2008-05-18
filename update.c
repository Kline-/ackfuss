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



#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include <signal.h>
#ifndef DEC_MONEY_H
#include "money.h"
#endif

extern POL_DATA politics_data;
extern OBJ_DATA *quest_object;

extern COUNCIL_DATA super_councils[MAX_SUPER];

/*
 * Local functions.
 */
int hit_gain args( ( CHAR_DATA * ch ) );
int mana_gain args( ( CHAR_DATA * ch ) );
int move_gain args( ( CHAR_DATA * ch ) );
void mobile_update args( ( void ) );
void weather_update args( ( void ) );
void char_update args( ( void ) );
void gain_update args( ( void ) );
void obj_update args( ( void ) );
void aggr_update args( ( void ) );
void objfun_update args( ( void ) );
void auction_update args( ( void ) );
void rooms_update args( ( void ) );
void remember_attack args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void quest_update args( ( void ) );


int abort_threshold = BOOT_DB_ABORT_THRESHOLD;
bool disable_timer_abort = FALSE;
int last_checkpoint;


int get_user_seconds(  )
{
   struct rusage rus;
   getrusage( RUSAGE_SELF, &rus );
   return rus.ru_utime.tv_sec;
}

/* Update the checkpoint */
void alarm_update(  )
{
   extern int ssm_dup_count;
   extern int ssm_loops;

   ssm_dup_count = 0;
   ssm_loops = 0;

   last_checkpoint = get_user_seconds(  );
   if( abort_threshold == BOOT_DB_ABORT_THRESHOLD )
   {
      abort_threshold = RUNNING_ABORT_THRESHOLD;
      fprintf( stderr, "Used %d user CPU seconds.\n", last_checkpoint );
   }
}

#ifndef WIN32
/* Set the virtual (CPU time) timer to the standard setting, ALARM_FREQUENCY */

void reset_itimer(  )
{
   struct itimerval itimer;
   itimer.it_interval.tv_usec = 0;  /* miliseconds */
   itimer.it_interval.tv_sec = ALARM_FREQUENCY;
   itimer.it_value.tv_usec = 0;
   itimer.it_value.tv_sec = ALARM_FREQUENCY;

   /*
    * start the timer - in that many CPU seconds, alarm_handler will be called 
    */
   if( setitimer( ITIMER_VIRTUAL, &itimer, NULL ) < 0 )
   {
      perror( "reset_itimer:setitimer" );
      exit( 1 );
   }
}
#endif

const char *szFrozenMessage = "Alarm_handler: Not checkpointed recently, aborting!\n";

/* Signal handler for alarm - suggested for use in MUDs by Fusion */
void alarm_handler( int signo )
{
   int usage_now = get_user_seconds(  );

   /*
    * Has there gone abort_threshold CPU seconds without alarm_update? 
    */
   if( !disable_timer_abort && ( usage_now - last_checkpoint > abort_threshold ) )
   {
      /*
       * For the log file 
       */
      char buf[MAX_STRING_LENGTH];
      extern int ssm_dup_count;
      extern int ssm_loops;
      extern int ssm_recent_loops;

      /*
       * spec: log usage values 
       */
      log_f( "current usage: %d, last checkpoint: %d", usage_now, last_checkpoint );
      log_f( "SSM dups: %d, loops: %d, recent: %d", ssm_dup_count, ssm_loops, ssm_recent_loops );

      sprintf( buf, "%s\n\r", szFrozenMessage );
      bug( buf, 0 );
      raise( SIGABRT ); /* kill ourselves on return */
   }

   /*
    * The timer resets to the values specified in it_interval 
    * * automatically.
    * *
    * * Spec: additionally, SIGABRT is blocked in this handler, and will
    * * only be delivered on return. This should ensure a good core.
    */
}

#ifndef WIN32
/* Install signal alarm handler */
void init_alarm_handler(  )
{
   struct sigaction sa;

   sa.sa_handler = alarm_handler;
   sa.sa_flags = SA_RESTART;  /* Restart interrupted system calls */
   sigemptyset( &sa.sa_mask );
   sigaddset( &sa.sa_mask, SIGABRT );  /* block abort() in the handler
                                        * so we can get a good coredump */

   if( sigaction( SIGVTALRM, &sa, NULL ) < 0 )  /* setup handler for virtual timer */
   {
      perror( "init_alarm_handler:sigaction" );
      exit( 1 );
   }
   last_checkpoint = get_user_seconds(  );
   reset_itimer(  ); /* start timer */
}
#endif

/*
 * Advancement stuff.
 */
void advance_level( CHAR_DATA * ch, int class, bool show, bool remort )
{

   /*
    * class used instead of ch->class.  -S- 
    */
   /*
    * show added to allow no display of gain ( when using setclass ) 
    */
   /*
    * remort indicates remortal class or normal mortal class 
    */

   char buf[MAX_STRING_LENGTH];
   int add_hp;
   int add_mana;
   int add_move;
   int add_prac;
   int add_bloodlust, add_max_skills;

   /*
    * title no longer changed..... 
    */
   if( class == ADVANCE_WOLF )
   {
      add_bloodlust = ( number_range( 1, ( ( MAX_WOLF_LEVEL / 2 ) - ch->pcdata->generation ) ) ) +
         ( ( ( MAX_WOLF_LEVEL / 2 ) - ch->pcdata->generation ) / 2 );
      add_prac = number_range( 1, UMAX( 2, ( ( MAX_WOLF_LEVEL / 2 ) - ch->pcdata->generation ) ) );
      add_max_skills = add_prac;

      ch->pcdata->bloodlust_max += add_bloodlust;
      ch->pcdata->vamp_pracs += add_prac;
      ch->pcdata->vamp_skill_max += add_max_skills;
      sprintf( buf, "@@NYou gain: %d @@rRage Ability@@N, and %d @@bWerewolf Practices. .@@N\n\r", add_bloodlust, add_prac );


      send_to_char( buf, ch );
      return;
   }


   if( ( class == 16 ) )
   {

      add_bloodlust = UMAX( ( ( MAX_VAMP_LEVEL / 2 ) - ( ch->pcdata->generation / 2 ) ), 1 );
      add_prac = number_range( 1, UMAX( 2, ( ( MAX_VAMP_LEVEL / 2 ) - ( ch->pcdata->generation ) ) ) );
      add_max_skills = number_range( 1, UMAX( 2, ( ( MAX_VAMP_LEVEL / 2 ) - ( ch->pcdata->generation ) ) ) );

      ch->pcdata->bloodlust_max += add_bloodlust;
      ch->pcdata->vamp_pracs += add_prac;
      ch->pcdata->vamp_skill_max += add_max_skills;


      sprintf( buf, "You gain: %d @@eBloodlust@@N, and %d Vampyre Practices. .\n\r", add_bloodlust, add_prac );


      send_to_char( buf, ch );
      return;
   }

   if( ( class == 32 ) )
   {

      add_hp = con_app[get_curr_con( ch )].hitp + number_range( 10, 50 );
      add_mana = number_range( 10, ( 3 * get_curr_int( ch ) + get_curr_wis( ch ) ) / 4 );
   }

   else if( remort )
   {
      add_hp = con_app[get_curr_con( ch )].hitp + number_range( remort_table[class].hp_min, remort_table[class].hp_max );
      add_mana = remort_table[class].fMana ? number_range( 2, ( 2 * get_curr_int( ch ) + get_curr_wis( ch ) ) / 16 ) : 0;

   }
   else
   {
      add_hp = con_app[get_curr_con( ch )].hitp + number_range( class_table[class].hp_min, class_table[class].hp_max );

      add_mana = class_table[class].fMana ? number_range( 2, ( 2 * get_curr_int( ch ) + get_curr_wis( ch ) ) / 16 ) : 0;
   }
   add_move = number_range( 2, ( get_curr_con( ch ) + get_curr_dex( ch ) ) / 5 );
   add_prac = ( wis_app[get_curr_wis( ch )].practice / 2 ) + number_range( 1, 3 );



   add_hp = UMAX( 1, add_hp );


   add_mana = UMAX( 0, add_mana );
   add_move = UMAX( 7, add_move );

   ch->pcdata->mana_from_gain += add_mana;
   ch->pcdata->hp_from_gain += add_hp;
   ch->pcdata->move_from_gain += add_move;

   ch->max_hit += add_hp;
   ch->max_mana += add_mana;
   ch->max_move += add_move;
   ch->practice += add_prac;

   if( !IS_NPC( ch ) )
      REMOVE_BIT( ch->act, PLR_BOUGHT_PET );

   sprintf( buf, "You gain: %d Hit Points, %d Mana, %d Movement and %d pracs.\n\r", add_hp, add_mana, add_move, add_prac );

   if( show )
      send_to_char( buf, ch );

   return;
}



void gain_exp( CHAR_DATA * ch, long_int gain )
{
   /*
    * Not much happens here, as no-longer auto-level... -S- 
    */

   /*
    * -S- Mod:  mobs CAN gain exp as well as players 
    */

   if( ( IS_NPC( ch ) ) && !( IS_SET( ch->act, ACT_INTELLIGENT ) ) )
      return;

   if( IS_IMMORTAL( ch ) )
      return;


   /*
    * Changed exp system AGAIN old 'cap' was screwy!! -S- 
    */

   ch->exp += gain;

   return;
}



/*
 * Regeneration stuff.
 */
int hit_gain( CHAR_DATA * ch )
{
   int gain;
   if( ch->is_free != FALSE )
      return 0;

   if( IS_NPC( ch ) && !IS_SET( ch->act, ACT_INTELLIGENT ) )

      gain = ( 5 + ch->level / 30 );

   gain = ( 5 + ch->level / 20 );

   if( IS_SET( ch->in_room->room_flags, ROOM_REGEN ) )
      gain *= 2;

   switch ( ch->position )
   {
      case POS_SLEEPING:
         gain += get_curr_con( ch ) / 2;
         break;
      case POS_RESTING:
         gain += get_curr_con( ch ) / 4;
         break;
   }
   if( !IS_NPC( ch ) )
   {
      if( ch->pcdata->condition[COND_FULL] == 0 )
         gain /= 2;

      if( ch->pcdata->condition[COND_THIRST] == 0 )
         gain /= 2;

      if( IS_VAMP( ch ) && ch->pcdata->bloodlust < 3 )
         gain = 0;
      else if( IS_VAMP( ch ) && ch->pcdata->bloodlust < 8 )
         gain /= 2;
      if( IS_VAMP( ch ) && ch->pcdata->bloodlust == -10 )
         gain = ( 5 + ch->level / 25 );


   }



   if( IS_AFFECTED( ch, AFF_POISON ) )
      gain /= 4;

   if( IS_SET( ch->in_room->room_flags, ROOM_COLD ) || ( IS_SET( ch->in_room->room_flags, ROOM_HOT ) ) )
      gain *= -2;

   if( IS_SET( ch->in_room->affected_by, ROOM_BV_HEAL_REGEN ) )
   {
      if( gain < 0 )
         gain *= -2;
      else
         gain *= 2;
   }

   if( IS_AFFECTED( ch, AFF_CLOAK_REGEN ) )
   {
      if( gain < 0 )
         gain *= -2;
      else
         gain *= 2;
   }

   if( IS_SET( ch->in_room->affected_by, ROOM_BV_HEAL_STEAL ) )
      if( gain > 0 )
         gain *= -1;
   if( !IS_NPC( ch ) && ( gain > 0 ) )
   {
      if( IS_SET( race_table[ch->race].race_flags, RACE_MOD_FAST_HEAL ) )
         gain = gain * 1.5;
      else if( IS_SET( race_table[ch->race].race_flags, RACE_MOD_SLOW_HEAL ) )
         gain = gain * .75;
   }
   if( !IS_NPC( ch ) && ( gain > 0 ) )
   {
      if( ( IS_SET( race_table[ch->race].race_flags, RACE_MOD_WOODLAND ) ) && ( ch->in_room != NULL ) )
      {
         if( ( ch->in_room->sector_type == SECT_FIELD ) || ( ch->in_room->sector_type == SECT_FOREST ) )
            gain = gain * 1.3;
         else if( ( ch->in_room->sector_type == SECT_CITY ) || ( ch->in_room->sector_type == SECT_INSIDE ) )
            gain = gain * .8;
      }
      else if( ( IS_SET( race_table[ch->race].race_flags, RACE_MOD_DARKNESS ) ) && ( ch->in_room != NULL ) )
      {
         if( ( ch->in_room->sector_type == SECT_FIELD )
             || ( ch->in_room->sector_type == SECT_HILLS )
             || ( ch->in_room->sector_type == SECT_AIR ) || ( ch->in_room->sector_type == SECT_DESERT ) )
            gain = gain * .8;
         else if( ( ch->in_room->sector_type == SECT_CITY ) || ( ch->in_room->sector_type == SECT_INSIDE ) )
            gain = gain * 1.3;
      }

   }

   return UMIN( gain, ch->max_hit - ch->hit );
}



int mana_gain( CHAR_DATA * ch )
{
   int gain;
   if( ch->is_free != FALSE )
      return 0;
   if( IS_NPC( ch ) && !IS_SET( ch->act, ACT_INTELLIGENT ) )
   {
      gain = ( 1 + ch->level / 30 );
   }
   else
   {
      gain = ( 5 + ch->level / 20 );

      if( IS_SET( ch->in_room->room_flags, ROOM_REGEN ) )
         gain *= 2;

      switch ( ch->position )
      {
         case POS_SLEEPING:
            gain += get_curr_int( ch );
            break;
         case POS_RESTING:
            gain += get_curr_int( ch ) / 2;
            break;
      }
      if( !IS_NPC( ch ) )
      {
         if( ch->pcdata->condition[COND_FULL] == 0 )
            gain /= 2;

         if( ch->pcdata->condition[COND_THIRST] == 0 )
            gain /= 2;
         if( IS_VAMP( ch ) && ch->pcdata->bloodlust < 3 )
            gain = 0;
         else if( IS_VAMP( ch ) && ch->pcdata->bloodlust < 8 )
            gain /= 2;
         if( IS_VAMP( ch ) && ch->pcdata->bloodlust == -10 )
            gain = ( 5 + ch->level / 25 );

         if( IS_WOLF( ch ) && IS_RAGED( ch ) )
            gain = 0;
      }

      if( IS_SET( ch->in_room->affected_by, ROOM_BV_MANA_REGEN ) )
      {
         if( gain < 0 )
            gain *= -2;
         else
            gain *= 2;
      }
      if( IS_SET( ch->in_room->affected_by, ROOM_BV_MANA_STEAL ) )
         if( gain > 0 )
            gain *= -1;

   }

   if( IS_AFFECTED( ch, AFF_POISON ) )
      gain /= 4;

   if( !IS_NPC( ch ) && ( gain > 0 ) )
   {
      if( IS_SET( race_table[ch->race].race_flags, RACE_MOD_NO_MAGIC ) )
         gain = gain * .5;
      else if( IS_SET( race_table[ch->race].race_flags, RACE_MOD_WEAK_MAGIC ) )
         gain = gain * .75;
      else if( IS_SET( race_table[ch->race].race_flags, RACE_MOD_STRONG_MAGIC ) )
         gain = gain * 1.40;

   }
   if( !IS_NPC( ch ) && ( gain > 0 ) )
   {
      if( ( IS_SET( race_table[ch->race].race_flags, RACE_MOD_WOODLAND ) ) && ( ch->in_room != NULL ) )
      {
         if( ( ch->in_room->sector_type == SECT_FIELD ) || ( ch->in_room->sector_type == SECT_FOREST ) )
            gain = gain * 1.3;
         else if( ( ch->in_room->sector_type == SECT_CITY ) || ( ch->in_room->sector_type == SECT_INSIDE ) )
            gain = gain * .8;
      }
      else if( ( IS_SET( race_table[ch->race].race_flags, RACE_MOD_DARKNESS ) ) && ( ch->in_room != NULL ) )
      {
         if( ( ch->in_room->sector_type == SECT_FIELD )
             || ( ch->in_room->sector_type == SECT_HILLS )
             || ( ch->in_room->sector_type == SECT_AIR ) || ( ch->in_room->sector_type == SECT_DESERT ) )
            gain = gain * .8;
         else if( ( ch->in_room->sector_type == SECT_CITY ) || ( ch->in_room->sector_type == SECT_INSIDE ) )
            gain = gain * 1.3;
      }
   }
   if( gain > 0 )
   {
      gain = gain * int_app[get_curr_int( ch )].mana_regen / 10;
      if( MAGIC_STANCE( ch ) )
         gain = gain * int_app[get_curr_int( ch )].mana_regen / 10;
   }
   return UMIN( gain, ch->max_mana - ch->mana );
}



int move_gain( CHAR_DATA * ch )
{
   int gain;

   if( IS_NPC( ch ) )
   {
      gain = ch->level;
   }
   else
   {
      gain = ( 10 + ch->level / 4 );

      if( IS_SET( ch->in_room->room_flags, ROOM_REGEN ) )
         gain *= 2;

      switch ( ch->position )
      {
         case POS_SLEEPING:
            gain += get_curr_dex( ch ) / 2;
            break;
         case POS_RESTING:
            gain += get_curr_dex( ch ) / 4;
            break;
      }

      if( ch->pcdata->condition[COND_FULL] == 0 )
         gain /= 2;

      if( ch->pcdata->condition[COND_THIRST] == 0 )
         gain /= 2;

      if( IS_VAMP( ch ) && ch->pcdata->bloodlust < 3 )
         gain = 0;
      else if( IS_VAMP( ch ) && ch->pcdata->bloodlust < 8 )
         gain /= 2;
      if( IS_VAMP( ch ) && ch->pcdata->bloodlust == -10 )
         gain = ( 5 + ch->level / 25 );


   }

   if( IS_AFFECTED( ch, AFF_POISON ) )
      gain /= 4;


   return UMIN( gain, ch->max_move - ch->move );
}

void gain_rage( CHAR_DATA * ch )
{

   sh_int rage_gain = 0;
   sh_int current_rage = 0;

   if( IS_NPC( ch ) || !IS_WOLF( ch ) )
      return;


   if( IS_RAGED( ch ) )
      current_rage = ch->pcdata->bloodlust_max;
   else if( IS_SHIFTED( ch ) )
      current_rage = UMAX( 1, ( ch->pcdata->bloodlust_max / 5 ) );
   else
      current_rage = UMAX( 1, ( ch->pcdata->bloodlust_max / 10 ) );

   rage_gain = number_range( 1, ( MAX_WOLF_LEVEL / 2 - ch->pcdata->generation ) );

   if( ch->pcdata->bloodlust >= current_rage )
      ch->pcdata->bloodlust = UMIN( ( ch->pcdata->bloodlust + rage_gain ), current_rage );
   else
      ch->pcdata->bloodlust = UMIN( current_rage, ( ch->pcdata->bloodlust + rage_gain ) );

}

void gain_bloodlust( CHAR_DATA * ch, int value )
{
   /*
    * Kinda like gain_condition, but handles vampires -S- 
    */

   int condition;

   if( value == 0 )
      return;

   condition = ch->pcdata->bloodlust;

   /*
    * in case vamp bites off more than he can chew ;)
    * -Damane- 4/26/96 
    */

   if( ( ch->pcdata->bloodlust + value ) > ch->pcdata->bloodlust_max )
      ch->pcdata->bloodlust = ch->pcdata->bloodlust_max;
   else
      ch->pcdata->bloodlust += value;

   if( ch->pcdata->bloodlust > ch->pcdata->bloodlust_max )
      ch->pcdata->bloodlust = ch->pcdata->bloodlust_max;

   if( ch->position == POS_BUILDING || ch->position == POS_WRITING )
      return;
   if( ( ch->pcdata->bloodlust < 0 ) && ( ch->pcdata->bloodlust != -10 ) )
      ch->pcdata->bloodlust = 0;
   if( ch->pcdata->bloodlust < 2 )
      send_to_char( "Your body burns with the need for blood!\n\r", ch );
   else if( ch->pcdata->bloodlust < 7 )
      send_to_char( "You start to feel weaker... more blood needed!\n\r", ch );
   else if( ch->pcdata->bloodlust < 10 )
      send_to_char( "You find yourself missing the taste of blood.\n\r", ch );
   return;
}

void gain_condition( CHAR_DATA * ch, int iCond, int value )
{
   int condition;

   if( value == 0 || IS_NPC( ch ) || ch->level >= LEVEL_HERO )
      return;

   condition = ch->pcdata->condition[iCond];
   ch->pcdata->condition[iCond] = URANGE( 0, condition + value, 48 );


   if( ch->position == POS_BUILDING || ch->position == POS_WRITING )
      return;


   if( ch->pcdata->condition[iCond] == 0 )
   {
      switch ( iCond )
      {
         case COND_FULL:
            send_to_char( "You are hungry.\n\r", ch );
            break;

         case COND_THIRST:
            send_to_char( "You are thirsty.\n\r", ch );
            break;

         case COND_DRUNK:
            if( condition != 0 )
               send_to_char( "You are sober.\n\r", ch );
            break;
      }
   }

   return;
}



/*
 * Mob autonomous action.
 * This function takes 25% to 35% of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update( void )
{
   CHAR_DATA *ch;
   CHAR_DATA *ch_next;
   CHAR_DATA *target;
   EXIT_DATA *pexit;
   int door;
   CHAR_DATA *quitter;

   /*
    * Examine all mobs. 
    */
   CREF( ch_next, CHAR_NEXT );

   for( ch = first_char; ch != NULL; ch = ch_next )
   {
      ch_next = ch->next;

      if( !IS_NPC( ch ) || ch->in_room == NULL || IS_AFFECTED( ch, AFF_CHARM ) )
         continue;

      /*
       * Examine call for special procedure 
       */
      if( ch->spec_fun != 0 )
      {
         if( ( *ch->spec_fun ) ( ch ) )
            continue;
      }

      /*
       * Added by Aeria for mob hunting continuously 
       */
      if( ch->searching != NULL )
      {
         if( ( quitter = get_char_world( ch, ch->searching ) ) != NULL )
            set_hunt( ch, NULL, quitter, NULL, HUNT_WORLD | HUNT_OPENDOOR | HUNT_PICKDOOR | HUNT_UNLOCKDOOR | HUNT_INFORM,
                      HUNT_CR | HUNT_MERC );

      }


      /*
       * DISABLED 
       */
      /*
       * Intelligent mob? 
       */
/*	if ( IS_SET( ch->act, ACT_INTELLIGENT ) )
	   int_handler( ch ); Disabled for now, for bugs.  */

      /*
       * That's all for sleeping / busy monster 
       */
      if( ch->position < POS_STANDING )
         continue;

      /*
       * Check for rewield, and re-equip (specials not used anymore) 
       */
      if( IS_SET( ch->act, ACT_REWIELD ) )
         if( check_rewield( ch ) )
            continue;

      if( IS_SET( ch->act, ACT_RE_EQUIP ) )
         if( check_re_equip( ch ) )
            continue;


      /*
       * Check for remember victims 
       */
      if( ch->target != NULL && ( target = get_char_room( ch, ch->target ) ) != NULL )
      {
         remember_attack( ch, target );
         continue;
      }


      /*
       * Check to see if mob is moving somewhere 
       */
/*	if ( mob_hunt(ch) )
	  continue;
	if ( IS_SET( ch->act_hunt, ACT_HUNT_MOVE ) 
	&& ch->move_to != NO_VNUM )
	{
	   hunt_move( ch );
	   continue;
	}
	*/


      /*
       * MOBprogram random trigger 
       */
      if( ch->in_room->area->nplayer > 0 )
      {
         mprog_random_trigger( ch );
         /*
          * If ch dies or changes
          * position due to it's random
          * trigger continue - Kahn 
          */
         if( ch->position < POS_STANDING )
            continue;
      }



      /*
       * Scavenge 
       */
      if( IS_SET( ch->act, ACT_SCAVENGER ) && ch->in_room->first_content != NULL && number_bits( 2 ) == 0 )
      {
         OBJ_DATA *obj;
         OBJ_DATA *obj_best;
         int max;

         max = 1;
         obj_best = 0;
         for( obj = ch->in_room->first_content; obj; obj = obj->next_in_room )
         {
            if( CAN_WEAR( obj, ITEM_TAKE ) && obj->cost > max )
            {
               obj_best = obj;
               max = obj->cost;
            }
         }

         if( obj_best )
         {
            obj_from_room( obj_best );
            obj_to_char( obj_best, ch );
            act( "$n gets $p.", ch, obj_best, NULL, TO_ROOM );
         }
      }

      /*
       * Wander 
       */
      if( !IS_SET( ch->act, ACT_SENTINEL )
          && ch->leader == NULL
          && ( door = number_bits( 5 ) ) <= 5
          && ( pexit = ch->in_room->exit[door] ) != NULL
          && pexit->to_room != NULL
          && !IS_SET( pexit->exit_info, EX_CLOSED )
          && !IS_SET( pexit->to_room->room_flags, ROOM_NO_MOB )
          && ( !IS_SET( ch->act, ACT_STAY_AREA ) || pexit->to_room->area == ch->in_room->area ) )
      {
         move_char( ch, door );
         /*
          * If ch changes position due
          * to it's or someother mob's
          * movement via MOBProgs,
          * continue - Kahn 
          */
         if( ch->position < POS_STANDING )
            continue;
      }

      /*
       * Flee 
       */
      if( ch->hit < ( ch->max_hit / 2 )
          && ( door = number_bits( 3 ) ) <= 5
          && ( pexit = ch->in_room->exit[door] ) != NULL
          && pexit->to_room != NULL
          && !IS_SET( pexit->exit_info, EX_CLOSED ) && !IS_SET( pexit->to_room->room_flags, ROOM_NO_MOB ) )
      {
         CHAR_DATA *rch;
         bool found;

         found = FALSE;
         for( rch = pexit->to_room->first_person; rch != NULL; rch = rch->next_in_room )
         {
            if( !IS_NPC( rch ) )
            {
               found = TRUE;
               break;
            }
         }
         if( !found )
            move_char( ch, door );
      }

   }
   CUREF( ch_next );
   return;
}



/*
 * Update the weather.
 */



void clean_donate_rooms( void )
{
   ROOM_INDEX_DATA *room = NULL;
   sh_int room_count, looper, chance;
   int room_vnum;
   OBJ_DATA *object, *obj_next;


   /*
    * normal donate rooms 
    */
   if( ( room = get_room_index( ROOM_VNUM_MISC_DONATE ) ) != NULL )
   {
      for( room_count = 0, object = room->first_content; object; room_count++, object = object->next_in_room );
      if( room_count > 150 )
      {
         OREF( obj_next, OBJ_NEXT );
         for( object = room->first_content; object; object = obj_next )
         {
            obj_next = object->next_in_room;
            if( count_obj_room( object->pIndexData, room->first_content ) > 5 )
               chance = 50;
            else
               chance = 10;

            if( ( IS_SET( object->extra_flags, ITEM_RARE ) ) || ( object == quest_object ) )
               chance = 0;
            if( number_range( 0, 99 ) < chance )
               extract_obj( object );
         }
         OUREF( obj_next );
      }
   }
   if( ( room = get_room_index( ROOM_VNUM_WEAPON_DONATE ) ) != NULL )
   {
      for( room_count = 0, object = room->first_content; object; room_count++, object = object->next_in_room );
      if( room_count > 150 )
      {
         OREF( obj_next, OBJ_NEXT );
         for( object = room->first_content; object; object = obj_next )
         {
            obj_next = object->next_in_room;
            if( count_obj_room( object->pIndexData, room->first_content ) > 5 )
               chance = 50;
            else
               chance = 10;
            if( ( IS_SET( object->extra_flags, ITEM_RARE ) ) || ( object == quest_object ) )
               chance = 0;
            if( number_range( 0, 99 ) < chance )
               extract_obj( object );
         }
         OUREF( obj_next );
      }
   }
   if( ( room = get_room_index( ROOM_VNUM_ARMOR_DONATE ) ) != NULL )
   {
      for( room_count = 0, object = room->first_content; object; room_count++, object = object->next_in_room );
      if( room_count > 150 )
      {
         OREF( obj_next, OBJ_NEXT );
         for( object = room->first_content; object; object = obj_next )
         {
            obj_next = object->next_in_room;
            if( count_obj_room( object->pIndexData, room->first_content ) > 5 )
               chance = 50;
            else
               chance = 10;
            if( ( IS_SET( object->extra_flags, ITEM_RARE ) ) || ( object == quest_object ) )
               chance = 0;
            if( number_range( 0, 99 ) < chance )
               extract_obj( object );
         }
         OUREF( obj_next );
      }
   }

   /*
    * clan donate rooms 
    */
   for( looper = 1; looper < MAX_CLAN; looper++ )
   {
      if( ( room_vnum = clan_table[looper].donat_room ) == 0 )
         continue;
      if( ( room = get_room_index( room_vnum ) ) != NULL )
      {
         for( room_count = 0, object = room->first_content; object; room_count++, object = object->next_in_room );
         if( room_count > 150 )
         {
            OREF( obj_next, OBJ_NEXT );
            for( object = room->first_content; object; object = obj_next )
            {
               obj_next = object->next_in_room;
               if( count_obj_room( object->pIndexData, room->first_content ) > 5 )
                  chance = 50;
               else
                  chance = 10;
               if( ( IS_SET( object->extra_flags, ITEM_RARE ) ) || ( object == quest_object ) )
                  chance = 0;
               if( number_range( 0, 99 ) < chance )
                  extract_obj( object );
            }
            OUREF( obj_next );
         }
      }
   }
}
void weather_update( void )
{
   char buf[MAX_STRING_LENGTH];
   char buf2[MSL];
   DESCRIPTOR_DATA *d;
   int diff;
   sh_int x, y;

   buf[0] = '\0';
   buf2[0] = '\0';

   switch ( ++time_info.hour )
   {
      case 5:
         weather_info.sunlight = SUN_LIGHT;
         strcat( buf, "The sky shows signs of daybreak.\n\r" );
         break;

      case 6:
         weather_info.sunlight = SUN_RISE;
         strcat( buf, "The sun rises in the east.\n\r" );
         for( x = 1; x < MAX_CLAN; x++ )
            for( y = 1; y < MAX_CLAN; y++ )
               politics_data.daily_negotiate_table[x][y] = FALSE;
         break;
      case 12:

         for( x = 1; x < MAX_CLAN; x++ )
            for( y = 1; y < MAX_CLAN; y++ )
               politics_data.daily_negotiate_table[x][y] = FALSE;
         break;


      case 19:
         weather_info.sunlight = SUN_SET;
         strcat( buf, "The sun slowly disappears in the west.\n\r" );
         for( x = 1; x < MAX_CLAN; x++ )
            for( y = 1; y < MAX_CLAN; y++ )
               politics_data.daily_negotiate_table[x][y] = FALSE;
         break;

      case 20:
         weather_info.sunlight = SUN_DARK;
         strcat( buf, "The night has begun.\n\r" );
         break;

      case 24:
         time_info.hour = 0;
         time_info.day++;
         for( x = 1; x < MAX_CLAN; x++ )
            for( y = 1; y < MAX_CLAN; y++ )
               politics_data.daily_negotiate_table[x][y] = FALSE;
         clean_donate_rooms(  );
         break;
   }
   switch ( time_info.moon++ )
   {
      case 5:
         weather_info.moon_loc = MOON_RISE;
         sprintf( buf2, "@@NA %s @@yMoon @@Nhas risen.\n\r", get_moon_phase_name(  ) );
         safe_strcat( MSL, buf, buf2 );
         break;
      case 10:
         weather_info.moon_loc = MOON_LOW;
         sprintf( buf2, "@@NThe %s @@yMoon @@Nrides low on the horizon.\n\r", get_moon_phase_name(  ) );
         safe_strcat( MSL, buf, buf2 );
         break;
      case 15:
         weather_info.moon_loc = MOON_PEAK;
         sprintf( buf2, "@@NThe %s @@yMoon @@Nreaches it's zenith.\n\r", get_moon_phase_name(  ) );
         safe_strcat( MSL, buf, buf2 );
         break;
      case 20:
         weather_info.moon_loc = MOON_FALL;
         sprintf( buf2, "@@NThe %s @@yMoon @@Nfalls.\n\r", get_moon_phase_name(  ) );
         safe_strcat( MSL, buf, buf2 );
         break;
      case 25:
         weather_info.moon_loc = MOON_SET;
         sprintf( buf2, "@@NThe %s @@yMoon @@Nis setting.\n\r", get_moon_phase_name(  ) );
         safe_strcat( MSL, buf, buf2 );
         break;
      case 30:
         weather_info.moon_loc = MOON_DOWN;
         sprintf( buf2, "@@NThe %s @@yMoon @@Nhas left the sky.\n\r", get_moon_phase_name(  ) );
         safe_strcat( MSL, buf, buf2 );
         break;

      default:
         break;
   }

   if( time_info.moon >= 50 )
   {
      time_info.moon = 0;
      weather_info.moon_loc = MOON_DOWN;
   }

   if( time_info.day >= 20 )  /* now 20 days = 1 month */
   {
      time_info.day = 0;
      time_info.month++;
   }

   if( time_info.month >= 8 ) /* 8 months a year */
   {
      time_info.month = 0;
      time_info.year++;
   }
   if( ( ( time_info.day ) % 4 ) == 0 )
   {
      if( !weather_info.phase_changed )
         weather_info.moon_phase++;
      if( weather_info.moon_phase > MOON_WAN_CRE )
         weather_info.moon_phase = MOON_NEW;
      weather_info.phase_changed = TRUE;
   }
   else
      weather_info.phase_changed = FALSE;

   /*
    * Weather change.
    */
   if( time_info.month >= 9 && time_info.month <= 16 )
      diff = weather_info.mmhg > 985 ? -2 : 2;
   else
      diff = weather_info.mmhg > 1015 ? -2 : 2;

   weather_info.change += diff * dice( 1, 4 ) + dice( 2, 6 ) - dice( 2, 6 );
   weather_info.change = UMAX( weather_info.change, -12 );
   weather_info.change = UMIN( weather_info.change, 12 );

   weather_info.mmhg += weather_info.change;
   weather_info.mmhg = UMAX( weather_info.mmhg, 960 );
   weather_info.mmhg = UMIN( weather_info.mmhg, 1040 );

   switch ( weather_info.sky )
   {
      default:
         bug( "Weather_update: bad sky %d.", weather_info.sky );
         weather_info.sky = SKY_CLOUDLESS;
         break;

      case SKY_CLOUDLESS:
         if( weather_info.mmhg < 990 || ( weather_info.mmhg < 1010 && number_bits( 2 ) == 0 ) )
         {
            strcat( buf, "The sky is getting cloudy.\n\r" );
            weather_info.sky = SKY_CLOUDY;
         }
         break;

      case SKY_CLOUDY:
         if( weather_info.mmhg < 970 || ( weather_info.mmhg < 990 && number_bits( 2 ) == 0 ) )
         {
            strcat( buf, "It starts to rain.\n\r" );
            weather_info.sky = SKY_RAINING;
         }

         if( weather_info.mmhg > 1030 && number_bits( 2 ) == 0 )
         {
            strcat( buf, "The clouds disappear.\n\r" );
            weather_info.sky = SKY_CLOUDLESS;
         }
         break;

      case SKY_RAINING:
         if( weather_info.mmhg < 970 && number_bits( 2 ) == 0 )
         {
            strcat( buf, "Lightning flashes in the sky.\n\r" );
            weather_info.sky = SKY_LIGHTNING;
         }

         if( weather_info.mmhg > 1030 || ( weather_info.mmhg > 1010 && number_bits( 2 ) == 0 ) )
         {
            strcat( buf, "The rain stopped.\n\r" );
            weather_info.sky = SKY_CLOUDY;
         }
         break;

      case SKY_LIGHTNING:
         if( weather_info.mmhg > 1010 || ( weather_info.mmhg > 990 && number_bits( 2 ) == 0 ) )
         {
            strcat( buf, "The lightning has stopped.\n\r" );
            weather_info.sky = SKY_RAINING;
            break;
         }
         break;
   }

   if( buf[0] != '\0' )
   {
      for( d = first_desc; d != NULL; d = d->next )
      {
         if( d->connected == CON_PLAYING
             && IS_OUTSIDE( d->character ) && ( d->character->position != POS_WRITING ) && IS_AWAKE( d->character ) )
            send_to_char( buf, d->character );
      }
   }

   return;
}



/* New update loop to handle gains for players => smaller 'ticks' for
   hp/mana/move gain, normal 'ticks' for objects, affects, weather, etc */
void gain_update( void )
{
   CHAR_DATA *ch;

/* send wholist to web page :) Zen  !-!-! ONLY RUN THIS ON ONE PORT OF YOUR SERVER !-! */
#if defined(SOE) && !defined(SOETEST) && !defined(SOEBLD)
   list_who_to_output(  );
#endif
/* Update super_councils info  */

   {

      MEMBER_DATA *imember;
      sh_int count = 0;
      sh_int council_index;

      for( council_index = 1; council_index < MAX_SUPER; council_index++ )
      {
         if( super_councils[council_index].council_time > 0 )
         {
            super_councils[council_index].council_time--;


            if( super_councils[council_index].council_time == 1 )
            {
               MEMBER_DATA *imember_next;
               for( imember = super_councils[council_index].first_member; imember != NULL; imember = imember_next )
               {
                  imember_next = imember->next;
                  send_to_char( "The current council is disbanded.\n\r", imember->this_member );
                  UNLINK( imember, super_councils[council_index].first_member, super_councils[council_index].last_member,
                          next, prev );
                  imember->this_member = NULL;
                  imember->next = NULL;
                  imember->prev = NULL;
                  PUT_FREE( imember, member_free );
               }
               super_councils[council_index].council_time = 0;
               super_councils[council_index].quorum = FALSE;
            }
         }
         if( !super_councils[council_index].quorum )
         {
            super_councils[council_index].quorum = FALSE;
            for( imember = super_councils[council_index].first_member; imember != NULL; imember = imember->next )
            {
               count++;
            }
            if( count >= QUORUM_NUMBER )
            {
               super_councils[council_index].quorum = TRUE;
               super_councils[council_index].council_time = 10;
               for( imember = super_councils[council_index].first_member; imember != NULL; imember = imember->next )
                  send_to_char( "The Council is in Session!\n\r", imember->this_member );
            }
         }
      }
   }






   for( ch = first_char; ch != NULL; ch = ch->next )
   {
      if( ch->is_free != FALSE )
         continue;
      if( ch->position >= POS_STUNNED && !IS_SET( ch->affected_by, AFF_VAMP_HEALING ) )
      {
         if( ( ch->hit < ch->max_hit ) && ( !IS_SET( ch->in_room->affected_by, ROOM_BV_NONE ) ) )
            ch->hit += hit_gain( ch );
         ch->hit = UMAX( 25, ch->hit );

         if( ( ch->mana < ch->max_mana ) && ( !IS_SET( ch->in_room->affected_by, ROOM_BV_NONE ) ) )
            ch->mana += mana_gain( ch );

         if( ( ch->move < ch->max_move ) && ( ch->carry_weight < can_carry_w( ch ) ) )
            ch->move += move_gain( ch );
         else if( ch->carry_weight >= can_carry_w( ch ) )
         {
            send_to_char( "You are carrying so much wieght that you are @@eEXHAUSTED@@N!!\n\r", ch );
            ch->move = 0;
         }

      }

      if( ch->position == POS_STUNNED || IS_SET( ch->affected_by, AFF_VAMP_HEALING ) )
         update_pos( ch );
   }
   return;
}



/*
 * Update all chars, including mobs.
 * This function is performance sensitive.
 */
void char_update( void )
{
   CHAR_DATA *ch;
   CHAR_DATA *ch_next;
   CHAR_DATA *ch_save;
   CHAR_DATA *ch_quit;
   time_t save_time;

   save_time = current_time;
   ch_save = NULL;
   ch_quit = NULL;

   CREF( ch_next, CHAR_NEXT );
   for( ch = first_char; ch != NULL; ch = ch_next )
   {
      AFFECT_DATA *paf;
      AFFECT_DATA *paf_next;


      ch_next = ch->next;
      if( ch->is_free != FALSE )
         continue;

      if( !IS_NPC( ch ) && IS_WOLF( ch ) )
         gain_rage( ch );

      /*
       * Find dude with oldest save time.
       */
      if( !IS_NPC( ch )
          && ( ch->desc == NULL || ch->desc->connected == CON_PLAYING ) && ch->level >= 2 && ch->save_time < save_time )
      {
         ch_save = ch;
         save_time = ch->save_time;
      }

      if( ( !IS_NPC( ch ) ) && ( ch->pcdata->has_exp_fix == 0 ) && ( ch->level > 1 ) )
      {
         send_to_char( "@@eWE HAVE CHANGED THE EXP SYSTEM. YOUR EXP HAS BEEN RESET TO 0.@@N\n\r", ch );
         send_to_char
            ( "@@ePLEASE CONACT AN IMMORTAL, AND YOU WILL BE ADVANCED ONE CLASS, UNLESS YOU ARE A NEW CHARACTER.@@N\n\r",
              ch );
         send_to_char( "@@mTHANK YOU!!!!@@N  NOTE: DO NOT ATTEMPT TO ABUSE THIS--WE ARE KEEPING TRACK!\n\r", ch );
         ch->pcdata->has_exp_fix = 1;
         ch->exp = 0;
      }
      else if( !IS_NPC( ch ) )
         ch->pcdata->has_exp_fix = 1;

      if( ( IS_NPC( ch ) ) && ( ch->hit < -15 ) )
         raw_kill( ch, "" );

      if( ch->sitting != NULL && ch->sitting->in_room != ch->in_room )
      {
         ch->sitting->value[1]--;
         ch->sitting = NULL;
      }

      if( ch->position >= POS_STUNNED )
      {
         /*
          * -S- mod. 
          */
         if( ch->position != POS_WRITING && ch->position != POS_BUILDING )
         {
            if( IS_SET( ch->in_room->room_flags, ROOM_HOT ) )
               send_to_char( "You feel your skin burning.\n\r", ch );
            else if( IS_SET( ch->in_room->room_flags, ROOM_COLD ) )
               send_to_char( "You feel your skin freezing.\n\r", ch );
         }


      }

      /*
       * if ( ch->stunTimer > 0 )
       * {
       * ch->position = POS_STUNNED;
       * ch->stunTimer -= 1;
       * }
       * else
       * {
       * ch->stunTimer = 0;
       * ch->position = POS_STANDING;
       * 
       * }  
       */



      if( ( !IS_NPC( ch ) && ch->level < LEVEL_IMMORTAL ) )
      {

         OBJ_DATA *obj;

         if( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) != NULL && obj->item_type == ITEM_LIGHT && obj->value[2] > 0 )
         {
            if( --obj->value[2] == 0 && ch->in_room != NULL )
            {
               --ch->in_room->light;
               act( "$p goes out.", ch, obj, NULL, TO_ROOM );
               act( "$p goes out.", ch, obj, NULL, TO_CHAR );
               if( obj->value[7] > 0 )
               {
                  OBJ_INDEX_DATA *new_index;
                  OBJ_DATA *replacer;
                  if( ( new_index = get_obj_index( obj->value[6] ) ) == NULL )
                  {
                     sprintf( bug_buf,
                              "ERROR in expiring item %s(%s %d): item has a replace vnum set (%d), but that is not a valid item.",
                              obj->name, obj->pIndexData->area->keyword, obj->pIndexData->vnum, obj->value[6] );
                     monitor_chan( bug_buf, MONITOR_OBJ );
                     log_f( bug_buf );
                  }
                  else
                  {
                     replacer = create_object( new_index, obj->level );
                     if( obj->carried_by != NULL )
                        obj_to_char( replacer, obj->carried_by );
                     else if( obj->in_room != NULL )
                        obj_to_room( replacer, obj->in_room );
                     else if( obj->in_obj != NULL )
                        obj_to_obj( replacer, obj->in_obj );
                  }
               }
               extract_obj( obj );
            }

         }

         if( ++ch->timer >= 12 )
         {
            if( ch->was_in_room == NULL && ch->in_room != NULL )
            {
               ch->was_in_room = ch->in_room;
               if( ch->fighting != NULL )
                  stop_fighting( ch, TRUE );
               act( "$n disappears into the void.", ch, NULL, NULL, TO_ROOM );
               send_to_char( "You disappear into the void.\n\r", ch );
               save_char_obj( ch );
               char_from_room( ch );
               char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );
            }
         }

         if( ch->timer > 30 )
            ch_quit = ch;

         /*
          * Move this inside the if loop below to stop imms getting bloodlust 
          */
         if( ( IS_VAMP( ch ) ) && ( !IS_NPC( ch ) ) )
         {
            gain_bloodlust( ch, 0 - number_range( 1, 2 ) );
            check_vamp( ch );
         }

         gain_condition( ch, COND_THIRST, 0 - number_range( 1, 2 ) );
         if( ch->pcdata->condition[COND_THIRST] <= 10 )
            ch->pcdata->condition[COND_THIRST] = 10;
         gain_condition( ch, COND_DRUNK, 0 - number_range( 1, 2 ) );
         if( !IS_VAMP( ch ) )
         {
            gain_condition( ch, COND_FULL, 0 - number_range( 1, 2 ) );

         }
      }

      for( paf = ch->first_affect; paf != NULL; paf = paf_next )
      {
         paf_next = paf->next;
         if( paf->duration > 0 )
         {
            paf->duration--;

            /*
             * We need a check here for spells that keep working... 
             */
            if( paf->type == skill_lookup( "blood leach" ) )
            {
               if( paf->caster != NULL && !IS_NPC( paf->caster ) )
               {
                  send_to_char( "You feel the blood leach sapping your strength.\n\r", ch );
                  act( "You feel a surge of blood, coming from your blood leach on $N.", paf->caster, NULL, ch, TO_CHAR );
                  paf->caster->pcdata->bloodlust += ( 10 - paf->caster->pcdata->generation );
                  if( paf->caster->pcdata->bloodlust > paf->caster->pcdata->bloodlust_max )
                     paf->caster->pcdata->bloodlust = paf->caster->pcdata->bloodlust_max;
                  damage( ch, ch, paf->caster->pcdata->vamp_level * 20, TYPE_UNDEFINED );
               }
            }
            if( paf->type == skill_lookup( "black hand" ) )
            {
               if( paf->caster != NULL && !IS_NPC( paf->caster ) )
               {
                  send_to_char( "You feel the Black Hand choking you.\n\r", ch );
                  ch->hit -= paf->modifier;
               }
            }
            if( ( paf->type == skill_lookup( "adrenaline bonus" ) ) && ( ch->fighting == NULL ) && ( ch->hit > 10 ) )
            {
               ch->hit = UMAX( 10, ( ch->hit - ( paf->duration * 30 ) ) );
               ch->move = UMAX( 10, ( ch->move - ( paf->duration * 80 ) ) );
               send_to_char( "@@NYou feel the affects of your @@eadrenaline rush@@N wear off, leaving you exhausted.\n\r",
                             ch );
               affect_remove( ch, paf );
            }
         }
         else if( paf->duration < 0 )
            ;
         else
         {
            if( paf_next == NULL || paf_next->type != paf->type || paf_next->duration > 0 )
            {
               if( paf->type > 0 && skill_table[paf->type].msg_off )
               {
                  send_to_char( skill_table[paf->type].msg_off, ch );
                  send_to_char( "\n\r", ch );
               }
               if( paf->type > 0 && skill_table[paf->type].room_off )
                  act( skill_table[paf->type].room_off, ch, NULL, NULL, TO_ROOM );
            }

            affect_remove( ch, paf );
         }
      }

      /*
       * Careful with the damages here,
       *   MUST NOT refer to ch after damage taken,
       *   as it may be lethal damage (on NPC).
       */
      if( IS_NPC( ch ) )
      {
         if( ch->target != NULL && number_bits( 4 ) == 0 )
         {
            free_string( ch->target );
            ch->target = NULL;
         }
         if( ch->extract_timer > 0 )
         {
            ch->extract_timer--;
         }
         else if( ch->extract_timer == 0 )
         {
/*            if ( IS_SET( ch->affected_by, AFF_CHARM ) )
            {  */
            if( ( ch->master == NULL ) || ( ch->master->in_room == NULL ) || ( ch->in_room != ch->master->in_room ) )
            {
               if( ch->in_room != NULL )
               {
                  do_say( ch, "Whaa?? Where am I? How did I get here?" );
                  do_say( ch, "AHHH!!! Help me!!!! I'm MELTING......" );
               }
               extract_char( ch, TRUE );
               continue;
            }
            else
            {
               if( number_range( 0, 99 ) < get_psuedo_level( ch->master ) - 25 )
               {
                  CHAR_DATA *this_master;
                  this_master = ch->master;
                  do_say( ch, "Whaa?? Where am I? How did I get here?" );
                  do_scan( ch, "" );
                  check_social( ch, "growl", ch->master->name );
                  do_say( ch, "How dare you order me around!!!" );
                  stop_follower( ch );
                  multi_hit( ch, this_master, TYPE_UNDEFINED );
                  continue;
               }
            }
            /*
             * }  
             */
         }
      }

      if( IS_AFFECTED( ch, AFF_POISON ) )
      {
         act( "$n shivers and suffers.", ch, NULL, NULL, TO_ROOM );
         send_to_char( "You shiver and suffer.\n\r", ch );
         damage( ch, ch, number_range( 2, 8 ), gsn_poison );
      }
      else if( ch->position == POS_INCAP && !IS_VAMP( ch ) )
      {
         damage( ch, ch, number_range( 1, 4 ), TYPE_UNDEFINED );
      }
      else if( ch->position == POS_MORTAL && !IS_VAMP( ch ) )
      {
         damage( ch, ch, number_range( 2, 3 ), TYPE_UNDEFINED );
      }
      else if( ch->position == POS_DEAD && !IS_VAMP( ch ) )
      {
         damage( ch, ch, number_range( 5, 10 ), TYPE_UNDEFINED );
      }
      else if( ch->hit < -10 && !IS_VAMP( ch ) )
      {
         damage( ch, ch, number_range( 5, 10 ), TYPE_UNDEFINED );
      }

   }
   CUREF( ch_next );

   /*
    * Autosave and autoquit.
    * Check that these chars still exist.
    */
   if( ch_save != NULL || ch_quit != NULL )
   {
      CREF( ch_next, CHAR_NEXT );

      for( ch = first_char; ch != NULL; ch = ch_next )
      {
         ch_next = ch->next;
         if( ch == ch_save )
            save_char_obj( ch );
         if( ch == ch_quit )
         {
            send_to_char( "Idle for too long.  Bye Bye!\n\r", ch );
            do_quit( ch, "" );
         }
      }
      CUREF( ch_next );

   }

   return;
}

void check_vamp( CHAR_DATA * ch )
{
   /*
    * If vampire is outside, then (s)he suffers damage 
    */

   if( is_affected( ch, skill_lookup( "cloak:darkness" ) ) )
      return;

   if( IS_OUTSIDE( ch ) && !IS_SET( ch->in_room->affected_by, ROOM_BV_SHADE ) && time_info.hour > 5 && time_info.hour < 19 )
   {
      /*
       * Oh dear 
       */
      int dam;

      switch ( weather_info.sky )
      {
         case SKY_CLOUDLESS:
            dam = 4;
            break;
         case SKY_CLOUDY:
            dam = 3;
            break;
         case SKY_RAINING:
            dam = 3;
            break;
         default:
            dam = 1;
            break;
      }
      /*
       * Take bloodlust into account when calculating dam! 
       */

      dam *= 40 - ch->pcdata->vamp_level;

      /*
       * So dam ranges from 2 (lightning;no bloodlust)
       * * to 200 (sunny;complete bloodlust)
       * * And that's each tick!
       */

      act( "$n's skin burns with it's contact with daylight!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "Your skin burns with it's contact with daylight!", ch );
      if( ch->pcdata->bloodlust <= -5 )
         return;
      damage( ch, ch, dam, -1 );
   }
   return;
}



/* Check for objfuns.... this is probably performance sensitive too. */
void objfun_update( void )
{
   OBJ_DATA *obj;

   for( obj = first_obj; obj != NULL; obj = obj->next )
      if( obj->obj_fun != NULL )
      {
         if( obj->carried_by != NULL )
         {
            if( !IS_NPC( obj->carried_by ) && IS_WOLF( obj->carried_by )
                && ( IS_SHIFTED( obj->carried_by ) || IS_RAGED( obj->carried_by ) ) )
            {
               continue;
            }
         }
         ( *obj->obj_fun ) ( obj, obj->carried_by );
      }

   return;
}


/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update( void )
{

   OBJ_DATA *marker;
   OBJ_DATA *obj;
   extern OBJ_DATA *auction_item;

   /*
    * Create dummy object and add to end of list.  This object is
    * only a marker, and will not actually be processed by this
    * routine.
    */
   GET_FREE( marker, obj_free );
   LINK( marker, first_obj, last_obj, next, prev );

   /*
    * Repeatedly remove object from front of list, add to tail, and process
    * until the marker is at the head of the list.  That will indicate all
    * objects have been processed.
    */
   disable_timer_abort = FALSE;
   while( ( obj = first_obj ) != marker )
   {

      CHAR_DATA *rch;
      char *message;

      UNLINK( obj, first_obj, last_obj, next, prev );
      LINK( obj, first_obj, last_obj, next, prev );

      if( obj == auction_item )
         continue;
      if( IS_SET( obj->item_apply, ITEM_APPLY_HEATED ) && number_range( 0, 100 ) < 25 )
      {
         REMOVE_BIT( obj->item_apply, ITEM_APPLY_HEATED );
         if( obj->carried_by != NULL )
         {
            act( "Your $p @@acools off@@N!!", obj->carried_by, obj, NULL, TO_CHAR );
         }
         else if( obj->in_room != NULL && ( rch = obj->in_room->first_person ) != NULL )
         {
            act( "$p @@acools off@@N!!", rch, obj, NULL, TO_ROOM );
            act( "$p @@acools off@@N!!", rch, obj, NULL, TO_CHAR );
         }

      }


      if( obj->timer <= 0 || --obj->timer > 0 )
         continue;
      switch ( obj->item_type )
      {
         default:
            message = "$p vanishes.";
            break;
         case ITEM_FOUNTAIN:
            message = "$p dries up.";
            break;
         case ITEM_CORPSE_NPC:
            message = "$p decays into dust.";
            break;
         case ITEM_CORPSE_PC:
            message = "$p vapourises, and goes to heaven.";
            break;
         case ITEM_PORTAL:
            message = "$p implodes suddenly.";
            break;
         case ITEM_FOOD:
            message = "$p decomposes.";
            break;
      }

      if( obj->carried_by != NULL )
      {
         act( message, obj->carried_by, obj, NULL, TO_CHAR );
      }
      else if( obj->in_room != NULL && ( rch = obj->in_room->first_person ) != NULL )
      {
         act( message, rch, obj, NULL, TO_ROOM );
         act( message, rch, obj, NULL, TO_CHAR );
      }
/*
      if ( obj->in_room == NULL )
        continue; 
      if ( obj->item_type == ITEM_CORPSE_NPC )
        continue;
*/
      if( obj->value[7] > 0 )
      {
         OBJ_INDEX_DATA *new_index;
         OBJ_DATA *replacer;
         if( ( new_index = get_obj_index( obj->value[6] ) ) == NULL )
         {
            sprintf( bug_buf,
                     "ERROR in expiring item %s(%s %d): item has a replace vnum set (%d), but that is not a valid item.",
                     obj->name, obj->pIndexData->area->keyword, obj->pIndexData->vnum, obj->value[6] );
            monitor_chan( bug_buf, MONITOR_OBJ );
            log_f( bug_buf );
         }
         else
         {
            replacer = create_object( new_index, obj->level );
            if( obj->carried_by != NULL )
               obj_to_char( replacer, obj->carried_by );
            else if( obj->in_room != NULL )
               obj_to_room( replacer, obj->in_room );
            else if( obj->in_obj != NULL )
               obj_to_obj( replacer, obj->in_obj );
         }
      }
      extract_obj( obj );
   }


   /*
    * All objects have been processed.  Remove the marker object and
    * put it back on the free list.
    */
   UNLINK( marker, first_obj, last_obj, next, prev );
   PUT_FREE( marker, obj_free );

   disable_timer_abort = FALSE;
   return;
}



/*
 * Aggress.
 *
 * for each mortal PC
 *     for each mob in room
 *         aggress on some random PC
 *
 * This function takes 25% to 35% of ALL Merc cpu time.
 * Unfortunately, checking on each PC move is too tricky,
 *   because we don't the mob to just attack the first PC
 *   who leads the party into the room.
 *
 * -- Furey
 */
void aggr_update( void )
{

   /*
    * Check to see if ch has encountered a mob with ACT_REMEMBER set,
    * * and with victim->target == ch->name...    tbc ;)
    * * -- Stephen
    */

   CHAR_DATA *wch;
   CHAR_DATA *wch_next;
   CHAR_DATA *ch;
   CHAR_DATA *ch_next;
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   CHAR_DATA *victim;
   OBJ_DATA *wield;

   CREF( wch_next, CHAR_NEXT );

   for( wch = first_char; wch != NULL; wch = wch_next )
   {
      wch_next = wch->next;
      if( wch->is_free != FALSE )
         continue;
      if( IS_NPC( wch ) && wch->mpactnum > 0 && wch->in_room->area->nplayer > 0 )
      {
         MPROG_ACT_LIST *mpact;

         while( ( mpact = wch->first_mpact ) != NULL )
         {
            mprog_wordlist_check( mpact->buf, wch, mpact->ch, mpact->obj, mpact->vo, ACT_PROG );
            /*
             * Lets hope this check works until something better is in place.
             * * -- Alty 
             */
            if( wch->hit < -10 )
               break;
            wch->first_mpact = mpact->next;
            PUT_FREE( mpact, mpact_free );
         }
         if( wch->hit < -10 )
            continue;
         wch->mpactnum = 0;
         wch->first_mpact = NULL;
         wch->last_mpact = NULL;
      }

      if( ( IS_NPC( wch ) ) || wch->level >= LEVEL_IMMORTAL || wch->in_room == NULL )
         continue;
      CREF( ch_next, CHAR_NEXTROOM );
      for( ch = wch->in_room->first_person; ch != NULL; ch = ch_next )
      {
         int count;

         ch_next = ch->next_in_room;

         if( !IS_NPC( ch )
             || !IS_SET( ch->act, ACT_AGGRESSIVE )
             || ch->fighting != NULL
             || ch->hunting != NULL
             || IS_AFFECTED( ch, AFF_CHARM )
             || !IS_AWAKE( ch ) || ( IS_SET( ch->act, ACT_WIMPY ) && IS_AWAKE( wch ) ) || !can_see( ch, wch ) )
            continue;


         if( ( IS_AFFECTED( wch, AFF_SNEAK ) || item_has_apply( wch, ITEM_APPLY_SNEAK ) )
             && ( number_percent(  ) < 50 + ( 2 * ( get_psuedo_level( wch ) - get_psuedo_level( ch ) ) ) ) )
            continue;
         /*
          * Ok we have a 'wch' player character and a 'ch' npc aggressor.
          * MAG - wch can be an intelligent NPC.
          * Now make the aggressor fight a RANDOM pc victim in the room,
          *   giving each 'vch' an equal chance of selection.
          */
         count = 0;
         victim = NULL;
         CREF( vch_next, CHAR_NEXTROOM );
         for( vch = wch->in_room->first_person; vch != NULL; vch = vch_next )
         {
            vch_next = vch->next_in_room;

            if( ( !IS_NPC( vch ) || IS_SET( vch->act, ACT_INTELLIGENT ) )
                && vch->level < LEVEL_IMMORTAL
                && ( !IS_SET( ch->act, ACT_WIMPY ) || !IS_AWAKE( vch ) )
                && can_see( ch, vch ) && ( !( IS_UNDEAD( ch ) && IS_VAMP( vch ) ) ) )
            {
               if( number_range( 0, count ) == 0 )
                  victim = vch;
               count++;
            }
         }
         CUREF( vch_next );
         if( victim == NULL )
         {
            /*
             * bug( "Aggr_update: null victim.", count );    
             */
            continue;
         }
         if( IS_SET( victim->in_room->room_flags, ROOM_SAFE ) )
            continue;

         act( "$n growls at $N!", victim, NULL, ch, TO_NOTVICT );
         act( "$N growls at you!  Uh-oh!!", victim, NULL, ch, TO_CHAR );
         act( "You growl at $N.  Get $M!!", ch, NULL, victim, TO_CHAR );

         wield = get_eq_char( ch, WEAR_HOLD_HAND_L );
         if( wield != NULL && wield->item_type == ITEM_WEAPON && wield->value[3] == 11 && victim->fighting == NULL )
            do_backstab( ch, victim->name );
         else
            multi_hit( ch, victim, TYPE_UNDEFINED );
      }
      CUREF( ch_next );
   }
   CUREF( wch_next );
   return;
}

/*
 * Check ALL rooms for affects... the ratio of affects to rooms should
 * be relatively low, so this shouldn't hit performance too much.
 * -S-
 */

void rooms_update( void )
{
   ROOM_INDEX_DATA *room;
   AREA_DATA *area;
   BUILD_DATA_LIST *thing;
   ROOM_AFFECT_DATA *raf;
   ROOM_AFFECT_DATA *raf_next;
   MARK_LIST_MEMBER *this_mark;
   MARK_LIST_MEMBER *next_mark;


   for( area = first_area; area != NULL; area = area->next )
   {
      for( thing = area->first_area_room; thing != NULL; thing = thing->next )
      {
         room = thing->data;

         /*
          * if ( room->first_room_affect == NULL )
          * continue;   
          */

         for( this_mark = room->first_mark_list; this_mark != NULL; this_mark = next_mark )
         {
            next_mark = this_mark->next;
            if( this_mark->mark->duration > 0 )
               this_mark->mark->duration--;
            else if( this_mark->mark->duration < 0 )
               ;
            else
            {
               mark_from_room( room->vnum, this_mark->mark );
            }
         }



         for( raf = room->first_room_affect; raf != NULL; raf = raf_next )
         {
            raf_next = raf->next;

            if( raf->duration > 0 )
               raf->duration--;
            else if( raf->duration < 0 )
               ;
            else
            {
               if( raf_next == NULL || raf_next->type != raf->type || raf_next->duration > 0 )
               {
                  if( raf->type > 0 && skill_table[raf->type].msg_off )
                  {
                     send_to_room( skill_table[raf->type].msg_off, room );
                     send_to_room( "\n\r", room );
                  }
                  r_affect_remove( room, raf );
               }
            }
         }
      }
   }
   return;
}


extern void build_save_flush( void );

/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */
void update_handler( void )
{
   static int pulse_message;
   static int objfun_check;
   static int pulse_area;
   static int pulse_rooms;
   static int pulse_mobile;
   static int pulse_gain;
   static int pulse_violence;
   static int pulse_point;
   static int pulse_auction;
   extern int saving_area;
   extern bool auction_flop;

   if( saving_area )
      build_save(  );   /* For incremental area saving */


   if( --pulse_area <= 0 )
   {
      pulse_area = number_range( PULSE_AREA / 2, 3 * PULSE_AREA / 2 );
      area_update(  );
      build_save_flush(  );
   }

   if( --pulse_rooms <= 0 )
   {
      pulse_rooms = PULSE_ROOMS;
      rooms_update(  );
   }

   if( --pulse_message <= 0 )
   {
      pulse_message = PULSE_MESSAGE;
      message_update(  );
   }

   if( auction_flop )
   {
      pulse_auction = PULSE_AUCTION;
      auction_flop = FALSE;
   }

   if( --pulse_auction <= 0 )
   {
      pulse_auction = PULSE_AUCTION;
      auction_update(  );
   }

   if( --objfun_check <= 0 )
   {
      objfun_check = PULSE_OBJFUN;
      objfun_update(  );
   }


   if( --pulse_violence <= 0 )
   {
      alarm_update(  );
      pulse_violence = PULSE_VIOLENCE;
      violence_update(  );
   }

   if( --pulse_mobile <= 0 )
   {
      pulse_mobile = PULSE_MOBILE;
      mobile_update(  );
   }

   if( --pulse_gain <= 0 )
   {
      gain_update(  );
      pulse_gain = PULSE_PER_SECOND * number_range( 5, 8 );
   }

   if( --pulse_point <= 0 )
   {
      pulse_point = PULSE_TICK;
      weather_update(  );
      char_update(  );
      obj_update(  );
      quest_update(  );

      /*
       * This will log the number of perms being used...
       * * fgrep the log file to get results...
       */

      /*
       * perm_update( ); 
       */
   }

   aggr_update(  );
   tail_chain(  );
   return;
}

bool check_rewield( CHAR_DATA * ch )
{
   OBJ_DATA *obj;
   OBJ_DATA *weapon = NULL;
   int dam;
   int chance;
   bool pickup;
   char buf[MAX_STRING_LENGTH];


   pickup = TRUE;
   dam = 0;

   chance = ( ch->fighting == NULL ? 35 : 60 );

   if( number_percent(  ) < chance )
   {
      for( obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list )
         if( obj->item_type == ITEM_WEAPON && dam < obj->value[2] )
         {
            dam = obj->value[2];
            pickup = FALSE;
            weapon = obj;
         }


      /*
       * Then check inventory and room for any weapons 
       */
      for( obj = ch->in_room->first_content; obj != NULL; obj = obj->next_in_room )
      {
         if( obj->item_type == ITEM_WEAPON )
         {
            if( obj->value[2] > dam )
            {
               dam = obj->value[2];
               weapon = obj;
               pickup = TRUE;
            }
         }
      }



      if( weapon == NULL )
         return FALSE;

      if( weapon->wear_loc == WEAR_HOLD_HAND_L )
         return FALSE;


      if( pickup )
      {
         sprintf( buf, "Great!  %s!  Just what i've always wanted!", weapon->short_descr );
         do_say( ch, buf );
      }

      if( weapon != NULL )
      {
         /*
          * Now make the mob get the weapon 
          */
         if( pickup )
            get_obj( ch, weapon, NULL );

         do_wear( ch, weapon->name );

         /*
          * Check is mob wielded weapon ok... 
          */
         if( weapon->wear_loc == WEAR_NONE && weapon != quest_object )
         {
            act( "$n sniffs sadly.  'Baah!  It's no good to me!'", ch, NULL, NULL, TO_ROOM );
            extract_obj( weapon );
            act( "$n sacrifices $p.", ch, weapon, NULL, TO_ROOM );
         }
         return TRUE;
      }

   }



   return FALSE;
}

bool check_re_equip( CHAR_DATA * ch )
{
   OBJ_DATA *obj;
   OBJ_DATA *obj2;
   OBJ_DATA *armor = NULL;
   OBJ_DATA *light = NULL;
   int ac;
   int chance;
   bool pickup;
   bool ident;
   int best;
   char buf[MAX_STRING_LENGTH];
   int objnum;



   best = -1;
   pickup = TRUE;
   ac = 0;

   chance = ( ch->fighting == NULL ? 35 : 60 );
   if( number_percent(  ) < chance )
   {
      /*
       * Check each armor in room against ch's equipment 
       */

      ident = FALSE;
      for( obj = ch->in_room->first_content; obj != NULL; obj = obj->next_in_room )
      {
         if( !can_see_obj( ch, obj ) )
            continue;

         if( ( obj->wear_flags & ~ITEM_TAKE ) == 0 )
            continue;   /* Check to see if item cannot be worn */
         if( obj->item_type == ITEM_PIECE )
            continue;
         if( obj->item_type == ITEM_ARMOR )
         {
            /*
             * Check this object against our equiped objects 
             */
            ident = FALSE;
            for( obj2 = ch->first_carry; obj2 != NULL; obj2 = obj2->next_in_carry_list )
            {
               /*
                * Only scan against worn objects. 
                * * If obj2 is being worn in a position that obj can be worn in,
                * * and obj2->value[0] is better, then choose it.
                */

               if( obj2->wear_loc != WEAR_NONE
                   && obj2->item_type == ITEM_ARMOR
                   && can_wear_at( ch, obj, obj2->wear_loc ) && obj->value[0] > obj2->value[0] )
               {
                  ident = TRUE;  /* identical wear_loc */
                  armor = obj;
                  break;
               }
            }

            /*
             * Found no match for locations, so get and wear. 
             */
            if( !ident )
            {
               armor = obj;
               break;
            }
         }

         if( obj->item_type == ITEM_LIGHT && ( get_eq_char( ch, WEAR_LIGHT ) == NULL ) )
         {
            light = obj;
            break;
         }
      }


      /*
       * MAG Modification. Only check one item each time, against currently
       * worn object. 
       */

      /*
       * Check one inv item against worn eq, incase we've picked up some nicer
       * stuff 
       */

      objnum = number_percent(  ) * ch->carry_number / 100;
      for( obj = ch->first_carry; obj != NULL && objnum > 0; obj = obj->next_in_carry_list )
         objnum--;


      if( obj != NULL && obj->wear_loc == WEAR_NONE && obj->item_type == ITEM_ARMOR )
      {
         ident = FALSE;
         for( obj2 = ch->first_carry; obj2 != NULL; obj2 = obj2->next_in_carry_list )
         {
            if( obj2->wear_loc != WEAR_NONE && can_wear_at( ch, obj, obj2->wear_loc ) && obj->value[0] > obj2->value[0] )
            {
               ident = TRUE;
               armor = obj;
               break;
            }
         }
         if( !ident )
         {
            armor = obj;
         }
      }

      if( obj != NULL && obj->item_type == ITEM_LIGHT && ( get_eq_char( ch, WEAR_LIGHT ) == NULL ) )
      {
         light = obj;
      }
   }


   if( armor != NULL )
   {
      if( armor->carried_by != ch )
      {
         /*
          * Pick up off ground 
          */
         if( pickup )
         {
            sprintf( buf, "Great!  %s!  Just what i've always wanted!", armor->short_descr );
            do_say( ch, buf );
         }

         /*
          * Now make the mob get the armor 
          */
         if( pickup )
            get_obj( ch, armor, NULL );
      }

      do_wear( ch, armor->name );

      /*
       * Check is mob wielded weapon ok... 
       */
      if( armor->wear_loc == WEAR_NONE && armor != quest_object )
      {
         act( "$n sniffs sadly.  'Baah!  It's no good to me!'", ch, NULL, NULL, TO_ROOM );
         extract_obj( armor );
         act( "$n sacrifices $p.", ch, armor, NULL, TO_ROOM );
      }
      return TRUE;
   }

   if( light != NULL )
   {
      if( light->carried_by != ch )
      {
         /*
          * Pick up off ground 
          */
         if( pickup )
         {
            sprintf( buf, "Great!  %s!  Just what i've always wanted!", light->short_descr );
            do_say( ch, buf );
         }

         /*
          * Now make the mob get the light 
          */
         if( pickup )
            get_obj( ch, light, NULL );
      }

      do_wear( ch, light->name );

      /*
       * Check is mob wielded weapon ok... 
       */
      if( light->wear_loc == WEAR_NONE && light != quest_object )
      {
         act( "$n sniffs sadly.  'Baah!  It's no good to me!'", ch, NULL, NULL, TO_ROOM );
         extract_obj( light );
         act( "$n sacrifices $p.", ch, light, NULL, TO_ROOM );
      }
      return TRUE;
   }

   return FALSE;
}

void auction_update( void )
{
   extern OBJ_DATA *auction_item;
   extern CHAR_DATA *auction_owner;
   extern CHAR_DATA *auction_bidder;
   extern int auction_bid;
   extern int auction_reserve;
   extern int auction_stage;
   char buf[MAX_STRING_LENGTH];

   CHAR_DATA *ach;
   bool good_seller = FALSE, good_buyer = FALSE;

   /*
    * Stages: 0) No/New bid.  
    * 1) Waiting.  (If no bid here, then give up next time)
    * 2) Going once.  
    * 3) Going Twice.  
    * 4) GONE! 
    */

   if( auction_item == NULL )
      return;

   switch ( auction_stage )
   {
      case 0:
         if( auction_bidder == NULL )
         {
            sprintf( buf,
                     "@@N%s (level:%d, valued at %s) has been offered for auction.  A @@e10%% fee@@N will be charged, the higher of the reserve price or highest bid.",
                     auction_item->short_descr, auction_item->level, cost_to_money( auction_item->cost ) );
         }
         else
         {
            sprintf( buf, "%s has bid %s for %s.", auction_bidder->name,
                     cost_to_money( auction_bid ), auction_item->short_descr );
         }
         break;
      case 1:
         if( auction_bidder == NULL )
            sprintf( buf, "Last chance to bid for %s.", auction_item->short_descr );
         else
            sprintf( buf, "Last bid for %s was %s.  Any more offers?",
                     auction_item->short_descr, cost_to_money( auction_bid ) );
         break;
      case 2:
         if( auction_bidder == NULL )
         {
            auction( "No bidders.  Auction Ended." );

            for( ach = first_char; ach != NULL; ach = ach->next )
            {
               if( auction_owner == ach )
                  good_seller = TRUE;
               if( auction_bidder == ach )
                  good_buyer = TRUE;
            }

            if( good_seller )
            {
               int bid;
               char changebuf[MSL];
               char *change;
               bid = UMIN( money_value( auction_owner->money ), auction_reserve * .1 );
               change = take_best_coins( auction_owner->money, bid );
               change = one_argument( change, changebuf );
               money_to_value( auction_owner, change );
               join_money( round_money( atoi( change ), TRUE ), auction_owner->money );
               obj_to_char( auction_item, auction_owner );
            }
            else
            {
               auction( "Oh, well..guess they didn't want it anymore, since they LEFT!!  Well, it's mine now! " );
               extract_obj( auction_item );
            }
            auction_item = NULL;
            return;
         }
         sprintf( buf, "%s - Going Once!", auction_item->short_descr );
         break;
      case 3:
         sprintf( buf, "%s - Going TWICE!", auction_item->short_descr );
         break;
      case 4:
         if( auction_bid < auction_reserve )
         {
            for( ach = first_char; ach != NULL; ach = ach->next )
            {
               if( auction_owner == ach )
                  good_seller = TRUE;
               if( auction_bidder == ach )
                  good_buyer = TRUE;
            }

            sprintf( buf, "%s - CANCELLED.  Reserve price not matched.", auction_item->short_descr );
            if( good_seller )
            {
               int bid;
               char changebuf[MSL];
               char *change;
               bid = UMIN( money_value( auction_owner->money ), auction_reserve * .1 );
               change = take_best_coins( auction_owner->money, bid );
               change = one_argument( change, changebuf );
               money_to_value( auction_owner, change );
               join_money( round_money( atoi( change ), TRUE ), auction_owner->money );
               obj_to_char( auction_item, auction_owner );
            }
            else
               extract_obj( auction_item );
            if( good_buyer )
               join_money( round_money( auction_bid, TRUE ), auction_bidder->money );

         }
         else
         {

            for( ach = first_char; ach != NULL; ach = ach->next )
            {
               if( auction_owner == ach )
                  good_seller = TRUE;
               if( auction_bidder == ach )
                  good_buyer = TRUE;
            }

            if( good_buyer )
            {
               sprintf( buf, "%s - SOLD! to %s.", auction_item->short_descr, auction_bidder->name );

               obj_to_char( auction_item, auction_bidder );
            }
            else
            {
               sprintf( buf, "%s - SOLD!, but the buyer has left us.  Oh Well!!!", auction_item->short_descr );
               extract_obj( auction_item );
            }
            if( good_seller )
               join_money( round_money( auction_bid - ( auction_bid * .1 ), TRUE ), auction_owner->money );

         }

         auction_stage = 0;
         auction_bidder = NULL;
         auction_owner = NULL;
         auction_item = NULL;
         auction_reserve = 0;
         auction_bid = 0;
         break;
   }
   auction( buf );
   auction_stage++;
   return;
}

void remember_attack( CHAR_DATA * ch, CHAR_DATA * victim )
{
   /*
    * Called when an NPC ch encounters a PC victim, that tried to
    * * kill it previously.
    * * --Stephen
    */

   char buf[MAX_STRING_LENGTH];

   /*
    * Pick a random response for ch to give, before attacking 
    */

   switch ( number_range( 0, 7 ) )
   {
      case 0:
         sprintf( buf, "%s returns!  I shall have my revenge at last!", victim->name );
         do_yell( ch, buf );
         break;
      case 1:
         sprintf( buf, "%s You should never have returned.  Ye shall DIE!", victim->name );
         do_whisper( ch, buf );
         break;
      case 2:
         act( "$n looks at $N, remembering $S attack", ch, NULL, victim, TO_ROOM );
         act( "$n looks at you, remembering your attack", ch, NULL, victim, TO_VICT );
         act( "You look at $N, remembering $S attack.", ch, NULL, victim, TO_CHAR );
         do_say( ch, "I SHALL HAVE MY REVENGE!!!" );
         break;
      case 3:
         sprintf( buf, "%s has wronged me, and now I will seek my revenge!", victim->name );
         do_gossip( ch, buf );
         sprintf( buf, "Prepare to die, %s.", victim->name );
         do_say( ch, buf );
         break;
      case 4:
         sprintf( buf, "So, %s.  You have returned.  Let us finish our fight this time!", victim->name );
         do_say( ch, buf );
         break;
      case 5:
         sprintf( buf, "Only cowards flee from me, %s!", victim->name );
         do_say( ch, buf );
         break;
      case 6:
         act( "$n looks at $N, and recognizes $M!!", ch, NULL, victim, TO_ROOM );
         act( "$n looks at you, and recognizes you!!", ch, NULL, victim, TO_VICT );
         act( "You look at $N, and recognize $M!", ch, NULL, victim, TO_CHAR );
         sprintf( buf, "There can only be one winner, %s.", victim->name );
         do_say( ch, buf );
         break;
   }

   /*
    * Check if has intelligence, and call correct attack? 
    */

   multi_hit( ch, victim, TYPE_UNDEFINED );
   /*
    * spec- plug leak here 
    */
   if( ch->target )
   {
      free_string( ch->target );
      ch->target = NULL;
   }
   return;
}

void quest_update(  )
{
   extern bool auto_quest;
   extern bool quest;
   extern CHAR_DATA *quest_mob;
   extern OBJ_DATA *quest_object;
   extern int quest_timer;
   extern int quest_wait;

   if( !quest && !auto_quest )
      return;

   if( quest )
   {
      /*
       * Make sure the mobile and obj still exist! 
       */
      if( quest_mob == NULL || quest_object == NULL )
      {
         quest_cancel(  );
         return;
      }

      quest_inform(  );
      if( quest_timer > 15 )
         quest_cancel(  );
      return;
   }

   if( !quest )
   {
      if( quest_wait > 0 )
      {
         quest_wait--;
         return;
      }

      if( auto_quest )
         generate_auto_quest(  );
   }
}
