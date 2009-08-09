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

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
#endif

#ifndef DEC_ACT_INFO_H
#include "h/act_info.h"
#endif

#ifndef DEC_ACT_MOVE_H
#include "h/act_move.h"
#endif

#ifndef DEC_ACT_OBJ_H
#include "h/act_obj.h"
#endif

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_AREASAVE_H
#include "h/areasave.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_FIGHT_H
#include "h/fight.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_HUNT_H
#include "h/hunt.h"
#endif

#ifdef IMC
void imc_delete_reminfo(REMOTEINFO * p);
void imc_request_keepalive(void);
#endif

#ifndef DEC_INTERP_H
#include "h/interp.h"
#endif

#ifndef DEC_LUASCRIPT_H
#include "h/luascript.h"
#endif

#ifndef DEC_MAGIC_H
#include "h/magic.h"
#endif

#ifndef DEC_MONEY_H
#include "h/money.h"
#endif

#ifndef DEC_MUDINFO_H
#include "h/mudinfo.h"
#endif

#ifndef DEC_QUEST_H
#include "h/quest.h"
#endif

#ifndef DEC_SAVE_H
#include "h/save.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

#ifndef DEC_UPDATE_H
#include "h/update.h"
#endif

extern POL_DATA politics_data;
extern OBJ_DATA *quest_object;
extern CHAR_DATA *quest_mob;
extern COUNCIL_DATA super_councils[MAX_SUPER];
extern int ssm_dup_count;
extern int ssm_loops;
extern int ssm_recent_loops;
extern OBJ_DATA *auction_item;
extern CHAR_DATA *auction_owner;
extern CHAR_DATA *auction_bidder;
extern int auction_bid;
extern int auction_reserve;
extern int auction_stage;
extern int saving_area;
extern bool auction_flop;
extern bool auto_quest;
extern bool quest;
extern int quest_timer;
extern int quest_wait;

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
   char *strtime;
   char buf[MSL];

   ssm_dup_count = 0;
   ssm_loops = 0;

   last_checkpoint = get_user_seconds(  );
   if( abort_threshold == BOOT_DB_ABORT_THRESHOLD )
   {
      abort_threshold = RUNNING_ABORT_THRESHOLD;
      strtime = ctime( &current_time );
      strtime[strlen( strtime ) - 1] = '\0';
      snprintf( buf, MSL, "Used %d user CPU seconds.", last_checkpoint );
      fprintf( stderr, "%s :: %s\n", strtime, buf );
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

      /*
       * spec: log usage values
       */
      log_f( "current usage: %d, last checkpoint: %d", usage_now, last_checkpoint );
      log_f( "SSM dups: %d, loops: %d, recent: %d", ssm_dup_count, ssm_loops, ssm_recent_loops );

      snprintf( buf, MSL, "%s\r\n", szFrozenMessage );
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
void advance_level( CHAR_DATA * ch, int p_class, bool show, bool remort )
{

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
   if( p_class == ADVANCE_WOLF )
   {
      add_bloodlust = ( number_range( 1, ( ( MAX_WOLF_LEVEL / 2 ) - ch->pcdata->super->generation ) ) ) +
         ( ( ( MAX_WOLF_LEVEL / 2 ) - ch->pcdata->super->generation ) / 2 );
      add_prac = number_range( 1, UMAX( 2, ( ( MAX_WOLF_LEVEL / 2 ) - ch->pcdata->super->generation ) ) );
      add_max_skills = add_prac;

      ch->pcdata->super->energy_max += add_bloodlust;
      ch->pcdata->super->pracs += add_prac;
      ch->pcdata->super->skills_max += add_max_skills;
      snprintf( buf, MSL, "@@NYou gain: %d @@rRage Ability@@N, and %d @@bWerewolf Practices. .@@N\r\n", add_bloodlust, add_prac );


      send_to_char( buf, ch );
      return;
   }


   if( ( p_class == 16 ) )
   {

      add_bloodlust = UMAX( ( ( MAX_VAMP_LEVEL / 2 ) - ( ch->pcdata->super->generation / 2 ) ), 1 );
      add_prac = number_range( 1, UMAX( 2, ( ( MAX_VAMP_LEVEL / 2 ) - ( ch->pcdata->super->generation ) ) ) );
      add_max_skills = number_range( 1, UMAX( 2, ( ( MAX_VAMP_LEVEL / 2 ) - ( ch->pcdata->super->generation ) ) ) );

      ch->pcdata->super->energy_max += add_bloodlust;
      ch->pcdata->super->pracs += add_prac;
      ch->pcdata->super->skills_max += add_max_skills;


      snprintf( buf, MSL, "You gain: %d @@eBloodlust@@N, and %d Vampyre Practices. .\r\n", add_bloodlust, add_prac );


      send_to_char( buf, ch );
      return;
   }

   if( ( p_class == 32 ) )
   {

      add_hp = con_app[get_curr_con( ch )].hitp + number_range( 10, 50 );
      add_mana = number_range( 10, ( 3 * get_curr_int( ch ) + get_curr_wis( ch ) ) / 4 );
   }

   else if( remort )
   {
      add_hp = con_app[get_curr_con( ch )].hitp + number_range( remort_table[p_class].hp_min, remort_table[p_class].hp_max );
      add_mana = remort_table[p_class].fMana ? number_range( 2, ( 2 * get_curr_int( ch ) + get_curr_wis( ch ) ) / 16 ) : 0;

   }
   else
   {
      add_hp = con_app[get_curr_con( ch )].hitp + number_range( class_table[p_class].hp_min, class_table[p_class].hp_max );

      add_mana = class_table[p_class].fMana ? number_range( 2, ( 2 * get_curr_int( ch ) + get_curr_wis( ch ) ) / 16 ) : 0;
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
   ch->pcdata->practice += add_prac;

   if( !IS_NPC( ch ) )
    ch->act.reset(ACT_BOUGHT_PET);

   snprintf( buf, MSL, "You gain: %d Hit Points, %d Mana, %d Movement and %d pracs.\r\n", add_hp, add_mana, add_move, add_prac );

   if( show )
      send_to_char( buf, ch );

   return;
}

/*
 * Regeneration stuff.
 */
int hit_gain( CHAR_DATA * ch )
{
   float gain;
   if( ch == NULL )
      return 0;

   if( IS_NPC( ch ) && !ch->act.test(ACT_INTELLIGENT) )

      gain = ( 5 + ch->level / 30 );

   gain = ( 5 + ch->level / 20 );

   if( ch->in_room->room_flags.test(RFLAG_REGEN) )
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

      if( IS_VAMP( ch ) && ch->pcdata->super->energy < 3 )
         gain = 0;
      else if( IS_VAMP( ch ) && ch->pcdata->super->energy < 8 )
         gain /= 2;
      if( IS_VAMP( ch ) && ch->pcdata->super->energy == -10 )
         gain = ( 5 + ch->level / 25 );


   }



   if( IS_AFFECTED( ch, AFF_POISON ) )
      gain /= 4;

   if( ch->in_room->room_flags.test(RFLAG_COLD) || ch->in_room->room_flags.test(RFLAG_HOT) )
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

   if( check_charm_aff(ch,CHARM_AFF_REGEN) )
    gain *= ((100 + get_charm_bonus(ch,CHARM_AFF_REGEN)) / 100);

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

   return UMIN( static_cast<int>(gain), ch->max_hit - ch->hit );
}



int mana_gain( CHAR_DATA * ch )
{
   float gain;
   if( ch == NULL )
      return 0;
   if( IS_NPC( ch ) && !ch->act.test(ACT_INTELLIGENT) )
   {
      gain = ( 1 + ch->level / 30 );
   }
   else
   {
      gain = ( 5 + ch->level / 20 );

      if( ch->in_room->room_flags.test(RFLAG_REGEN) )
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
         if( IS_VAMP( ch ) && ch->pcdata->super->energy < 3 )
            gain = 0;
         else if( IS_VAMP( ch ) && ch->pcdata->super->energy < 8 )
            gain /= 2;
         if( IS_VAMP( ch ) && ch->pcdata->super->energy == -10 )
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

   if( check_charm_aff(ch,CHARM_AFF_REGEN) )
    gain *= ((100 + get_charm_bonus(ch,CHARM_AFF_REGEN)) / 100);

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
   return UMIN( static_cast<int>(gain), ch->max_mana - ch->mana );
}



int move_gain( CHAR_DATA * ch )
{
   float gain;

   if( IS_NPC( ch ) )
   {
      gain = ch->level;
   }
   else
   {
      gain = ( 10 + ch->level / 4 );

      if( ch->in_room->room_flags.test(RFLAG_REGEN) )
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

      if( IS_VAMP( ch ) && ch->pcdata->super->energy < 3 )
         gain = 0;
      else if( IS_VAMP( ch ) && ch->pcdata->super->energy < 8 )
         gain /= 2;
      if( IS_VAMP( ch ) && ch->pcdata->super->energy == -10 )
         gain = ( 5 + ch->level / 25 );


   }

   if( IS_AFFECTED( ch, AFF_POISON ) )
      gain /= 4;

   if( check_charm_aff(ch,CHARM_AFF_REGEN) )
    gain *= ((100 + get_charm_bonus(ch,CHARM_AFF_REGEN)) / 100);

   return UMIN( static_cast<int>(gain), ch->max_move - ch->move );
}

void rage_gain( CHAR_DATA * ch )
{

   short rage_gain = 0;
   short current_rage = 0;

   if( IS_NPC( ch ) || !IS_WOLF( ch ) )
      return;


   if( IS_RAGED( ch ) )
      current_rage = ch->pcdata->super->energy_max;
   else if( IS_SHIFTED( ch ) )
      current_rage = UMAX( 1, ( ch->pcdata->super->energy_max / 5 ) );
   else
      current_rage = UMAX( 1, ( ch->pcdata->super->energy_max / 10 ) );

   rage_gain = number_range( 1, ( MAX_WOLF_LEVEL / 2 - ch->pcdata->super->generation ) );

   if( ch->pcdata->super->energy >= current_rage )
      ch->pcdata->super->energy = UMIN( ( ch->pcdata->super->energy + rage_gain ), current_rage );
   else
      ch->pcdata->super->energy = UMIN( current_rage, ( ch->pcdata->super->energy + rage_gain ) );

}

void bloodlust_gain( CHAR_DATA * ch, int value )
{
   /*
    * Kinda like condition_gain, but handles vampires -S-
    */

   int condition;

   if( value == 0 )
      return;

   condition = ch->pcdata->super->energy;

   /*
    * in case vamp bites off more than he can chew ;)
    * -Damane- 4/26/96
    */

   if( ( ch->pcdata->super->energy + value ) > ch->pcdata->super->energy_max )
      ch->pcdata->super->energy = ch->pcdata->super->energy_max;
   else
      ch->pcdata->super->energy += value;

   if( ch->pcdata->super->energy > ch->pcdata->super->energy_max )
      ch->pcdata->super->energy = ch->pcdata->super->energy_max;

   if( ch->position == POS_BUILDING || ch->position == POS_WRITING )
      return;
   if( ( ch->pcdata->super->energy < 0 ) && ( ch->pcdata->super->energy != -10 ) )
      ch->pcdata->super->energy = 0;
   if( ch->pcdata->super->energy < 2 )
      send_to_char( "Your body burns with the need for blood!\r\n", ch );
   else if( ch->pcdata->super->energy < 7 )
      send_to_char( "You start to feel weaker... more blood needed!\r\n", ch );
   else if( ch->pcdata->super->energy < 10 )
      send_to_char( "You find yourself missing the taste of blood.\r\n", ch );
   return;
}

void condition_gain( CHAR_DATA * ch, int iCond, int value )
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
            send_to_char( "You are hungry.\r\n", ch );
            break;

         case COND_THIRST:
            send_to_char( "You are thirsty.\r\n", ch );
            break;

         case COND_DRUNK:
            if( condition != 0 )
               send_to_char( "You are sober.\r\n", ch );
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
   EXIT_DATA *pexit;
   int door;
   CHAR_DATA *quitter;
   list<CHAR_DATA *>::iterator li;

   /*
    * Examine all mobs.
    */

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      ch = *li;

      if( ch == NULL || !IS_NPC( ch ) || ch->in_room == NULL || IS_AFFECTED( ch, AFF_CHARM ) )
         continue;

      /*
       * Examine call for special procedure
       */
      if( IS_NPC(ch) && ch->npcdata->spec_fun != 0 )
      {
         if( ( *ch->npcdata->spec_fun ) ( ch ) )
            continue;
      }

      /*
       * Added by Aeria for mob hunting continuously
       */
      if( !ch->searching.empty() )
      {
         if( ( quitter = get_char_world( ch, const_cast<char *>(ch->searching.c_str()) ) ) != NULL )
            set_hunt( ch, NULL, quitter, NULL, HUNT_WORLD | HUNT_OPENDOOR | HUNT_PICKDOOR | HUNT_UNLOCKDOOR | HUNT_INFORM,
                      HUNT_CR | HUNT_MERC );

      }


      /*
       * DISABLED
       */
      /*
       * Intelligent mob?
       */
/*	if ( ch->act.test(ACT_INTELLIGENT) )
	   int_handler( ch ); Disabled for now, for bugs.  */

      /*
       * That's all for sleeping / busy monster
       */
      if( ch->position < POS_STANDING )
         continue;

      /*
       * Check for rewield, and re-equip (specials not used anymore)
       */
      if( ch->act.test(ACT_RE_WIELD) )
         if( check_rewield( ch ) )
            continue;

      if( ch->act.test(ACT_RE_EQUIP) )
         if( check_reequip( ch ) )
            continue;

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
       * Scavenge
       */
      if( ch->act.test(ACT_SCAVENGER) && ch->in_room->first_content != NULL && number_bits( 2 ) == 0 )
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
      if( !ch->act.test(ACT_SENTINEL)
          && ch->leader == NULL
          && ( door = number_bits( 5 ) ) <= 5
          && ( pexit = ch->in_room->exit[door] ) != NULL
          && pexit->to_room != NULL
          && !pexit->exit_info.test(EX_CLOSED)
          && !pexit->to_room->room_flags.test(RFLAG_NO_MOB)
          && ( !ch->act.test(ACT_STAY_AREA) || pexit->to_room->area == ch->in_room->area ) )
      {
         move_char( ch, door, FALSE );
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
          && !pexit->exit_info.test(EX_CLOSED) && !pexit->to_room->room_flags.test(RFLAG_NO_MOB) )
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
            move_char( ch, door, FALSE );
      }

   }
   return;
}


void clean_donate_rooms( void )
{
   ROOM_INDEX_DATA *room = NULL;
   short room_count, looper, chance;
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
         for( object = room->first_content; object; object = obj_next )
         {
            obj_next = object->next_in_room;
            if( count_obj_room( object->pIndexData, room->first_content ) > 5 )
               chance = 50;
            else
               chance = 10;

            if(  IS_OBJ_STAT(object,ITEM_EXTRA_RARE) || object == quest_object )
               chance = 0;
            if( number_range( 0, 99 ) < chance )
               extract_obj( object );
         }
      }
   }
   if( ( room = get_room_index( ROOM_VNUM_WEAPON_DONATE ) ) != NULL )
   {
      for( room_count = 0, object = room->first_content; object; room_count++, object = object->next_in_room );
      if( room_count > 150 )
      {
         for( object = room->first_content; object; object = obj_next )
         {
            obj_next = object->next_in_room;
            if( count_obj_room( object->pIndexData, room->first_content ) > 5 )
               chance = 50;
            else
               chance = 10;
            if(  IS_OBJ_STAT(object,ITEM_EXTRA_RARE) || object == quest_object )
               chance = 0;
            if( number_range( 0, 99 ) < chance )
               extract_obj( object );
         }
      }
   }
   if( ( room = get_room_index( ROOM_VNUM_ARMOR_DONATE ) ) != NULL )
   {
      for( room_count = 0, object = room->first_content; object; room_count++, object = object->next_in_room );
      if( room_count > 150 )
      {
         for( object = room->first_content; object; object = obj_next )
         {
            obj_next = object->next_in_room;
            if( count_obj_room( object->pIndexData, room->first_content ) > 5 )
               chance = 50;
            else
               chance = 10;
            if(  IS_OBJ_STAT(object,ITEM_EXTRA_RARE) || object == quest_object )
               chance = 0;
            if( number_range( 0, 99 ) < chance )
               extract_obj( object );
         }
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
            for( object = room->first_content; object; object = obj_next )
            {
               obj_next = object->next_in_room;
               if( count_obj_room( object->pIndexData, room->first_content ) > 5 )
                  chance = 50;
               else
                  chance = 10;
               if(  IS_OBJ_STAT(object,ITEM_EXTRA_RARE) || object == quest_object )
                  chance = 0;
               if( number_range( 0, 99 ) < chance )
                  extract_obj( object );
            }
         }
      }
   }
}

void weather_update( void )
{
   char buf[MSL];
   char buf2[MSL];
   DESCRIPTOR_DATA *d;
   int diff;
   short x, y;
#ifdef IMC
   REMOTEINFO *r, *rnext;
#endif
   buf[0] = '\0';
   buf2[0] = '\0';

   switch ( ++time_info.hour )
   {
      case 5:
         weather_info.sunlight = SUN_LIGHT;
         strncat( buf, "The sky shows signs of daybreak.\r\n", MSL );
         break;

      case 6:
         weather_info.sunlight = SUN_RISE;
         strncat( buf, "The sun rises in the east.\r\n", MSL );
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
         strncat( buf, "The sun slowly disappears in the west.\r\n", MSL );
         for( x = 1; x < MAX_CLAN; x++ )
            for( y = 1; y < MAX_CLAN; y++ )
               politics_data.daily_negotiate_table[x][y] = FALSE;
         break;

      case 20:
         weather_info.sunlight = SUN_DARK;
         strncat( buf, "The night has begun.\r\n", MSL );
         break;

      case 24:
         time_info.hour = 0;
         time_info.day++;
         for( x = 1; x < MAX_CLAN; x++ )
            for( y = 1; y < MAX_CLAN; y++ )
               politics_data.daily_negotiate_table[x][y] = FALSE;
         clean_donate_rooms(  );
         save_mudinfo();
#ifdef IMC
         if( this_imcmud->autoconnect ) /* Don't attempt a re-connect if we don't want to. --Kline */
         {
          monitor_chan("Mud list is being refreshed.",MONITOR_IMC);
          for( r = first_rinfo; r; r = rnext )
          {
           rnext = r->next;
           imc_delete_reminfo( r );
          }
          imc_request_keepalive(  );
         }
#endif
         break;
   }
   switch ( time_info.moon++ )
   {
      case 5:
         weather_info.moon_loc = MOON_RISE;
         snprintf( buf2, MSL, "@@NA %s @@yMoon @@Nhas risen.\r\n", get_moon_phase_name(  ) );
         strncat( buf, buf2, MSL-1 );
         break;
      case 10:
         weather_info.moon_loc = MOON_LOW;
         snprintf( buf2, MSL, "@@NThe %s @@yMoon @@Nrides low on the horizon.\r\n", get_moon_phase_name(  ) );
         strncat( buf, buf2, MSL-1 );
         break;
      case 15:
         weather_info.moon_loc = MOON_PEAK;
         snprintf( buf2, MSL, "@@NThe %s @@yMoon @@Nreaches it's zenith.\r\n", get_moon_phase_name(  ) );
         strncat( buf, buf2, MSL-1 );
         break;
      case 20:
         weather_info.moon_loc = MOON_FALL;
         snprintf( buf2, MSL, "@@NThe %s @@yMoon @@Nfalls.\r\n", get_moon_phase_name(  ) );
         strncat( buf, buf2, MSL-1 );
         break;
      case 25:
         weather_info.moon_loc = MOON_SET;
         snprintf( buf2, MSL, "@@NThe %s @@yMoon @@Nis setting.\r\n", get_moon_phase_name(  ) );
         strncat( buf, buf2, MSL-1 );
         break;
      case 30:
         weather_info.moon_loc = MOON_DOWN;
         snprintf( buf2, MSL, "@@NThe %s @@yMoon @@Nhas left the sky.\r\n", get_moon_phase_name(  ) );
         strncat( buf, buf2, MSL-1 );
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
            strncat( buf, "The sky is getting cloudy.\r\n", MSL );
            weather_info.sky = SKY_CLOUDY;
         }
         break;

      case SKY_CLOUDY:
         if( weather_info.mmhg < 970 || ( weather_info.mmhg < 990 && number_bits( 2 ) == 0 ) )
         {
            strncat( buf, "It starts to rain.\r\n", MSL );
            weather_info.sky = SKY_RAINING;
         }

         if( weather_info.mmhg > 1030 && number_bits( 2 ) == 0 )
         {
            strncat( buf, "The clouds disappear.\r\n", MSL );
            weather_info.sky = SKY_CLOUDLESS;
         }
         break;

      case SKY_RAINING:
         if( weather_info.mmhg < 970 && number_bits( 2 ) == 0 )
         {
            strncat( buf, "Lightning flashes in the sky.\r\n", MSL );
            weather_info.sky = SKY_LIGHTNING;
         }

         if( weather_info.mmhg > 1030 || ( weather_info.mmhg > 1010 && number_bits( 2 ) == 0 ) )
         {
            strncat( buf, "The rain stopped.\r\n", MSL );
            weather_info.sky = SKY_CLOUDY;
         }
         break;

      case SKY_LIGHTNING:
         if( weather_info.mmhg > 1010 || ( weather_info.mmhg > 990 && number_bits( 2 ) == 0 ) )
         {
            strncat( buf, "The lightning has stopped.\r\n", MSL );
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
   list<CHAR_DATA *>::iterator li;
/* Update super_councils info  */

   {

      MEMBER_DATA *imember;
      short count = 0;
      short council_index;

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
                  send_to_char( "The current council is disbanded.\r\n", imember->this_member );
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
                  send_to_char( "The Council is in Session!\r\n", imember->this_member );
            }
         }
      }
   }

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      ch = *li;
      if( ch == NULL )
         continue;
      if( ch->position >= POS_STUNNED && !IS_SET( ch->affected_by, AFF_VAMP_HEALING ) && !IS_GHOST(ch) )
      {
         if( ( ch->hit < ch->max_hit ) && ( !IS_SET( ch->in_room->affected_by, ROOM_BV_NONE ) ) )
            ch->hit += ch->position == POS_FIGHTING ? (hit_gain( ch ) / 2) : hit_gain( ch );
         ch->hit = UMAX( 25, ch->hit );

         if( ( ch->mana < ch->max_mana ) && ( !IS_SET( ch->in_room->affected_by, ROOM_BV_NONE ) ) )
            ch->mana += ch->position == POS_FIGHTING ? (mana_gain( ch ) / 2) : mana_gain( ch );

         if( ( ch->move < ch->max_move ) && ( ch->carry_weight < can_carry_w( ch ) ) )
            ch->move += ch->position == POS_FIGHTING ? (move_gain( ch ) / 2) : move_gain( ch );
         else if( ch->carry_weight >= can_carry_w( ch ) )
         {
            send_to_char( "You are carrying so much weight that you are @@eEXHAUSTED@@N!!\r\n", ch );
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
   CHAR_DATA *ch_save;
   CHAR_DATA *ch_quit;
   list<CHAR_DATA *>::iterator li;
   time_t save_time;

   save_time = current_time;
   ch_save = NULL;
   ch_quit = NULL;

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      AFFECT_DATA *paf;
      AFFECT_DATA *paf_next;

      ch = *li;
      if( ch == NULL )
         continue;

      if( !IS_NPC(ch) && IS_GHOST(ch) )
      {
       if( ch->pcdata->death_cnt > 0 )
        ch->pcdata->death_cnt--;
       if( ch->pcdata->death_cnt == 0 )
       {
        char_from_room(ch);
        char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
        send_to_char("The gods have taken pity upon your ghostly plight...\r\n",ch);
        resurrect(ch);
        do_mpcr(ch,const_cast<char *>(ch->name.c_str()));
        do_look(ch,"auto");
       }
      }

      if( !IS_NPC( ch ) && IS_WOLF( ch ) )
         rage_gain( ch );

      /*
       * Find dude with oldest save time.
       */
      if( !IS_NPC( ch )
          && ( ch->desc == NULL || ch->desc->connected == CON_PLAYING ) && ch->level >= 2 && ch->pcdata->save_time < save_time )
      {
         ch_save = ch;
         save_time = ch->pcdata->save_time;
      }

      if( ( IS_NPC( ch ) ) && ( ch->hit < -15 ) )
         raw_kill( ch, "" );

      if( ch->sitting != NULL && ch->sitting->in_room != ch->in_room )
      {
         ch->sitting->value[1]--;
         ch->sitting = NULL;
      }

      /* Update mquest status */
      update_mquest_wait_time(ch);

      if( ch->position >= POS_STUNNED )
      {
         /*
          * -S- mod.
          */
         if( ch->position != POS_WRITING && ch->position != POS_BUILDING )
         {
            if( ch->in_room->room_flags.test(RFLAG_HOT) )
               send_to_char( "You feel your skin burning.\r\n", ch );
            else if( ch->in_room->room_flags.test(RFLAG_COLD) )
               send_to_char( "You feel your skin freezing.\r\n", ch );
         }


      }

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
                     snprintf( bug_buf, (2 * MIL),
                              "ERROR in expiring item %s(%s %d): item has a replace vnum set (%d), but that is not a valid item.",
                              obj->name, obj->pIndexData->area->keyword, obj->pIndexData->vnum, obj->value[6] );
                     monitor_chan( bug_buf, MONITOR_OBJ );
                     log_f( "%s", bug_buf );
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
               send_to_char( "You disappear into the void.\r\n", ch );
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
            bloodlust_gain( ch, 0 - number_range( 1, 2 ) );
            check_vamp( ch );
         }

         if( !check_charm_aff(ch,CHARM_AFF_HUNGERLESS) )
          condition_gain( ch, COND_THIRST, 0 - number_range( 1, 2 ) );
         if( ch->pcdata->condition[COND_THIRST] <= 10 )
            ch->pcdata->condition[COND_THIRST] = 10;
         condition_gain( ch, COND_DRUNK, 0 - number_range( 1, 2 ) );
         if( !IS_VAMP( ch ) )
         {
          if( !check_charm_aff(ch,CHARM_AFF_HUNGERLESS) )
            condition_gain( ch, COND_FULL, 0 - number_range( 1, 2 ) );

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
                  send_to_char( "You feel the blood leach sapping your strength.\r\n", ch );
                  act( "You feel a surge of blood, coming from your blood leach on $N.", paf->caster, NULL, ch, TO_CHAR );
                  paf->caster->pcdata->super->energy += ( 10 - paf->caster->pcdata->super->generation );
                  if( paf->caster->pcdata->super->energy > paf->caster->pcdata->super->energy_max )
                     paf->caster->pcdata->super->energy = paf->caster->pcdata->super->energy_max;
                  damage( ch, ch, paf->caster->pcdata->super->level * 20, TYPE_UNDEFINED );
               }
            }
            if( paf->type == skill_lookup( "black hand" ) )
            {
               if( paf->caster != NULL && !IS_NPC( paf->caster ) )
               {
                  send_to_char( "You feel the Black Hand choking you.\r\n", ch );
                  ch->hit -= paf->modifier;
               }
            }
            if( ( paf->type == skill_lookup( "adrenaline bonus" ) ) && ( ch->fighting == NULL ) && ( ch->hit > 10 ) )
            {
               ch->hit = UMAX( 10, ( ch->hit - ( paf->duration * 30 ) ) );
               ch->move = UMAX( 10, ( ch->move - ( paf->duration * 80 ) ) );
               send_to_char( "@@NYou feel the affects of your @@eadrenaline rush@@N wear off, leaving you exhausted.\r\n",
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
                  send_to_char( "\r\n", ch );
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
         if( !ch->target.empty() && number_bits( 4 ) == 0 ) /* And all was forgiven ... --Kline */
          ch->target.clear();

         if( ch->npcdata->extract_timer > 0 )
            ch->npcdata->extract_timer--;
         if( ch->npcdata->extract_timer == 0 )
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
               if( number_range( 0, 99 ) < ch->master->get_level("psuedo") - 25 )
               {
                  CHAR_DATA *this_master;
                  this_master = ch->master;
                  do_say( ch, "Whaa?? Where am I? How did I get here?" );
                  do_scan( ch, "" );
                  check_social( ch, "growl", const_cast<char *>(ch->master->name.c_str()) );
                  do_say( ch, "How dare you order me around!!!" );
                  stop_follower( ch );
                  one_hit( ch, this_master, TYPE_UNDEFINED );
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
         send_to_char( "You shiver and suffer.\r\n", ch );
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

   /*
    * Autosave and autoquit.
    * Check that these chars still exist.
    */
   if( ch_save != NULL )
    save_char_obj(ch_save);
   if( ch_quit != NULL )
   {
    send_to_char("Idle for too long. Bye bye!\r\n",ch_quit);
    do_quit(ch_quit,"");
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

      dam *= 40 - ch->pcdata->super->level;

      /*
       * So dam ranges from 2 (lightning;no bloodlust)
       * * to 200 (sunny;complete bloodlust)
       * * And that's each tick!
       */

      act( "$n's skin burns with it's contact with daylight!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "Your skin burns with it's contact with daylight!", ch );
      if( ch->pcdata->super->energy <= -5 )
         return;
      damage( ch, ch, dam, -1 );
   }
   return;
}



/* Check for objfuns.... this is probably performance sensitive too. */
void objfun_update( void )
{
   OBJ_DATA *obj;
   list<OBJ_DATA *>::iterator li;

   for( li = obj_list.begin(); li != obj_list.end(); li++ )
   {
      obj = *li;
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
   }

   return;
}


/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update( void )
{
   OBJ_DATA *obj;
   list<OBJ_DATA *>::iterator li;

   /*
    * Repeatedly remove object from front of list, add to tail, and process
    * until the marker is at the head of the list.  That will indicate all
    * objects have been processed.
    */
   disable_timer_abort = FALSE;
   for( li = obj_list.begin(); li != obj_list.end(); li++ )
   {
      CHAR_DATA *rch;
      char *message;

      obj = *li;

      monitor_chan(obj->name,MONITOR_OBJ);

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
            snprintf( bug_buf, (2 * MIL),
                     "ERROR in expiring item %s(%s %d): item has a replace vnum set (%d), but that is not a valid item.",
                     obj->name, obj->pIndexData->area->keyword, obj->pIndexData->vnum, obj->value[6] );
            monitor_chan( bug_buf, MONITOR_OBJ );
            log_f( "%s", bug_buf );
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
 /*
  * for each aggressive mob
  *  for each mortal PC in room
  *   aggress on some random PC
  *
  * Less intensive this way ;)
  * --Kline
  *
  */
void aggr_update( void )
{
 CHAR_DATA *ch = NULL, *vch = NULL, *vch_next = NULL, *victim = NULL;
 list<CHAR_DATA *>::iterator li;
 vector<CHAR_DATA *> vict_list;

 for( li = aggro_list.begin(); li != aggro_list.end(); li++ )
 {
  ch = *li;
  vict_list.clear();

  if( ch == NULL || ch->in_room == NULL || ch->in_room->area->player_list.empty() )
   continue;

  if( ch->fighting != NULL || ch->hunting != NULL || IS_AFFECTED(ch,AFF_CHARM) || !IS_AWAKE(ch) || ch->in_room->room_flags.test(RFLAG_SAFE) || ch == quest_mob )
   continue;

  for( vch = ch->in_room->first_person; vch != NULL; vch = vch_next )
  {
   vch_next = vch->next_in_room;

   if( (IS_NPC(vch) && !vch->act.test(ACT_INTELLIGENT)) || vch->level >= LEVEL_IMMORTAL || (ch->act.test(ACT_WIMPY) && IS_AWAKE(vch)) || !can_see(ch,vch) || (IS_UNDEAD(ch) && IS_VAMP(vch)) )
    continue;

   if( (IS_AFFECTED(vch,AFF_SNEAK) || item_has_apply(vch,ITEM_APPLY_SNEAK)) && (number_percent() < 50 + (2 * (vch->get_level("psuedo") - ch->get_level("psuedo")))) )
    continue;

   vict_list.push_back(vch);
  }

 /*
  * We have a 'ch' NPC aggressor and a vict_list of player characters / intelligent NPCs.
  * Now make the aggressor fight a random target in the room, giving each an equal chance
  * of selection.
  */
  if( vict_list.empty() )
   continue;

  random_shuffle(vict_list.begin(),vict_list.end());
  victim = *vict_list.begin();
  aggro_attack(ch,victim);
 }
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
   list<AREA_DATA *>::iterator li;
   list<MARK_DATA *>::iterator mi;

   for( li = area_list.begin(); li != area_list.end(); li++ )
   {
      area = *li;
      for( thing = area->first_area_room; thing != NULL; thing = thing->next )
      {
         room = (ROOM_INDEX_DATA *)thing->data;

         /*
          * if ( room->first_room_affect == NULL )
          * continue;
          */

         if( !room->mark_list.empty() )
         {
          for( mi = room->mark_list.begin(); mi != room->mark_list.end(); mi++ )
          {
           static_cast<MARK_DATA *>(*mi)->duration--;
           if( static_cast<MARK_DATA *>(*mi)->duration <= 0 )
            mi = room->mark_list.erase(mi);
          }
          save_marks();
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
                  }
                  r_affect_remove( room, raf );
               }
            }
         }
      }
   }
   return;
}

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
   static int pulse_combat;
   static int pulse_point;
   static int pulse_auction;

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

   if( --pulse_combat <= 0 )
   {
    pulse_combat = PULSE_COMBAT;
    combat_update( );
    cast_update( );
    cooldown_update( );
    lua_update( );
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
         snprintf( buf, MSL, "Great!  %s!  Just what i've always wanted!", weapon->short_descr );
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

bool check_reequip( CHAR_DATA * ch )
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

         if( !CAN_WEAR(obj,ITEM_TAKE) )
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
            snprintf( buf, MSL, "Great!  %s!  Just what i've always wanted!", armor->short_descr );
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
            snprintf( buf, MSL, "Great!  %s!  Just what i've always wanted!", light->short_descr );
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
   char buf[MAX_STRING_LENGTH];
   list<CHAR_DATA *>::iterator li;
   CHAR_DATA *ach;
   bool good_seller = false, good_buyer = false;

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
            snprintf( buf, MSL,
                     "@@N%s (level:%d, valued at %s) has been offered for auction.  A @@e10%% fee@@N will be charged, the higher of the reserve price or highest bid.",
                     auction_item->short_descr, auction_item->level, cost_to_money( auction_item->cost ) );
         }
         else
         {
            snprintf( buf, MSL, "%s has bid %s for %s.", auction_bidder->name.c_str(),
                     cost_to_money( auction_bid ), auction_item->short_descr );
         }
         break;
      case 1:
         if( auction_bidder == NULL )
            snprintf( buf, MSL, "Last chance to bid for %s.", auction_item->short_descr );
         else
            snprintf( buf, MSL, "Last bid for %s was %s.  Any more offers?",
                     auction_item->short_descr, cost_to_money( auction_bid ) );
         break;
      case 2:
         if( auction_bidder == NULL )
         {
            auction( "No bidders.  Auction Ended." );

            for( li = char_list.begin(); li != char_list.end(); li++ )
            {
               ach = *li;
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
               bid = UMIN( money_value( auction_owner->money ), static_cast<int>(auction_reserve * .1) );
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
         snprintf( buf, MSL, "%s - Going Once!", auction_item->short_descr );
         break;
      case 3:
         snprintf( buf, MSL, "%s - Going TWICE!", auction_item->short_descr );
         break;
      case 4:
         if( auction_bid < auction_reserve )
         {
            for( li = char_list.begin(); li != char_list.end(); li++ )
            {
               ach = *li;
               if( auction_owner == ach )
                  good_seller = TRUE;
               if( auction_bidder == ach )
                  good_buyer = TRUE;
            }

            snprintf( buf, MSL, "%s - CANCELLED.  Reserve price not matched.", auction_item->short_descr );
            if( good_seller )
            {
               int bid;
               char changebuf[MSL];
               char *change;
               bid = UMIN( money_value( auction_owner->money ), static_cast<int>(auction_reserve * .1) );
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

            for( li = char_list.begin(); li != char_list.end(); li++ )
            {
               ach = *li;
               if( auction_owner == ach )
                  good_seller = TRUE;
               if( auction_bidder == ach )
                  good_buyer = TRUE;
            }

            if( good_buyer )
            {
               snprintf( buf, MSL, "%s - SOLD! to %s.", auction_item->short_descr, auction_bidder->name.c_str() );

               obj_to_char( auction_item, auction_bidder );
            }
            else
            {
               snprintf( buf, MSL, "%s - SOLD!, but the buyer has left us.  Oh Well!!!", auction_item->short_descr );
               extract_obj( auction_item );
            }
            if( good_seller )
               join_money( round_money( static_cast<int>(auction_bid - ( auction_bid * .1 )), TRUE ), auction_owner->money );

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

void quest_update(  )
{
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

void lua_update( )
{
 LUA_DATA *lua;
 list<LUA_DATA *>::iterator li;
 string str;
 string arg;

 for( li = lua_list.begin(); li != lua_list.end(); li++ )
 {
  lua = *li;
  str = "";
  arg = "";

  str += "wait.update";
  arg += "";
  //call_lua(lua,str,arg);
 }

 return;
}
