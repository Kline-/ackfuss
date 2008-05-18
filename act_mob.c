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

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "globals.h"

#ifndef DEC_ACT_MOB_H
#include "act_mob.h"
#endif

bool able_to_level( CHAR_DATA * ch )
{
   bool in_need = FALSE;

   if( ch->intell_exp > ( 5 * exp_for_mobile( ch->level, ch ) ) )
      in_need = TRUE;

   return in_need;
}

void gain_level( CHAR_DATA * ch )
{
   int cost;
   char buf[MAX_STRING_LENGTH];

   cost = 5 * exp_for_mobile( ch->level, ch );
   if( ch->intell_exp < cost )
      return;

   ch->intell_exp -= cost;
   ch->level = UMIN( 140, ch->level++ );

   sprintf( buf, "%s gains a level!", ch->short_descr );
   info( buf, 1 );
   return;
}

int find_spell( CHAR_DATA * ch, int type )
{
   int sn;
   int bar;
   int level;
   int spell = -1;
   int spell_level = -1;

   for( sn = 0; sn < MAX_SKILL; sn++ )
   {
      if( skill_table[sn].name == NULL )
         break;
      if( skill_table[sn].slot == 0 )
         continue;
      if( skill_table[sn].target != type )
         continue;

      level = -1;
      for( bar = 0; bar < MAX_CLASS; bar++ )
         if( skill_table[sn].skill_level[bar] > level && ch->level >= skill_table[sn].skill_level[bar] )
            level = skill_table[sn].skill_level[bar];

      if( level == -1 ) /* not high enough to use */
         continue;

      if( level > spell_level && mana_cost( ch, sn ) < ch->mana )
      {
         spell = sn;
         spell_level = level;
      }
   }

   return spell;
}

void mob_group_follow( CHAR_DATA * ch, CHAR_DATA * target )
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *vch;
   int num;


   if( ( ch == NULL ) || ( target == NULL ) )
   {
      sprintf( buf, "%s", "Null ch and/or target in mob_group_follow, exiting." );
      monitor_chan( buf, MONITOR_MOB );
      return;
   }
   sprintf( buf, "Ok guys, let's all follow %s.", target->short_descr );
   do_say( ch, buf );

   for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
   {
      if( ( ch != vch ) && ( AI_MOB( vch ) ) && ( is_same_group( ch, vch ) ) )
      {
         if( vch->position != POS_STANDING )
            do_stand( vch, "" );

         num = number_percent(  );

//  WAIT_STATE( vch, 12 );

         if( num > 85 )
            do_say( vch, "Ok boss.  Whatever you say." );
         else if( num > 70 )
            do_say( vch, "Alright!  More people, more power!" );
         else if( num > 55 )
            do_say( vch, "Whoo Hooo!" );
         else if( num > 35 )
            do_say( vch, "Sure thing." );
         else if( num > 29 )
         {
            if( num > 32 )
               sprintf( buf, "Man I don't want to join %s's group!", target->short_descr );
            else
               sprintf( buf, "I hate big groups." );
            do_say( vch, buf );
            do_follow( vch, vch->name );
            do_say( vch, "I'm outta here." );
            do_recall( vch, "" );
            continue;
         }

         if( !can_see( vch, target ) )
         {
            vch->master = target;
            vch->leader = NULL;
         }
         else
            do_follow( vch, target->name );
         do_group( target, "all" );

      }
   }
   return;
}


void get_mob_group( CHAR_DATA * ch, CHAR_DATA * target )
{
   CHAR_DATA *vch;
   bool ch_is_leader = FALSE;
   bool tar_is_leader = FALSE;
   bool is_hunting = FALSE;
   bool ch_is_higher = FALSE;
   char buf[MAX_STRING_LENGTH];
   int number_of_tar_group = 1;
   int number_of_ch_group = 1;

   ch_is_leader = is_group_leader( ch );
   tar_is_leader = is_group_leader( target );

   if( tar_is_leader == TRUE )
   {
      for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
      {
         if( !AI_MOB( vch ) )
         {
            continue;
         }
         if( vch != target && is_same_group( vch, target ) )
         {
            number_of_tar_group = number_of_tar_group + 1;
            continue;
         }
         if( vch != ch && is_same_group( ch, vch ) )
         {
            number_of_ch_group = number_of_ch_group + 1;
            continue;
         }
      }
   }

   do_say( ch, "Hello there.  What are you up to?" );

//   WAIT_STATE( target, 24 );

   if( target->hunting != NULL )
   {
      is_hunting = TRUE;
      if( tar_is_leader == TRUE )
      {
         sprintf( buf, "We're planning on killing %s.", target->hunting->short_descr );
      }
      else
      {
         sprintf( buf, "I'm planning on killing %s.", target->hunting->short_descr );
      }
      do_say( target, buf );
   }
   else
   {
      do_say( target, "Nothing.  Just hanging around." );
   }

   WAIT_STATE( ch, 24 );

   /*
    * check to see which of the two is higher. the higher mob will lead 
    */
   if( get_psuedo_level( ch ) >= get_psuedo_level( target ) )
      ch_is_higher = TRUE;

   /*
    * if ch is higher in levels and victim is hunting, then say 
    * * appropriate line. 
    */
   if( ( ch_is_higher == FALSE ) && ( is_hunting == TRUE ) )
   {
      do_say( ch, "Oh really?  Cool!  Need any help?" );
   }
   else if( ( ch_is_higher == FALSE ) && ( is_hunting == FALSE ) )
   {
      do_say( ch, "Great!  Since you're not doing anything, wanna group?" );
   }
   else if( ( ch_is_higher == TRUE ) && ( is_hunting == TRUE ) )
   {
      if( ch_is_leader == TRUE )
      {
         sprintf( buf, "Want to help us kill %s instead?", ch->hunting->short_descr );
         do_say( ch, buf );
      }
      else if( ch_is_leader == FALSE )
      {
         sprintf( buf, "Want to help me kill %s instead?", ch->hunting->short_descr );
         do_say( ch, buf );
      }
   }
   else if( ( ch_is_higher == TRUE ) && ( ch->hunting == FALSE ) )
   {
      do_say( ch, "Want to group?" );
   }
   WAIT_STATE( target, 24 );

   if( ch_is_higher == TRUE )
   {
      do_say( target, "Ok sure!  Thanks for asking." );

      if( tar_is_leader == TRUE )
      {
         mob_group_follow( target, ch );
      }
      do_follow( target, ch->name );
      do_group( ch, target->name );
   }
   else
   {
      do_say( target, "Ok, why not!?  Follow me." );

      WAIT_STATE( ch, 24 );

      do_say( ch, "Cool!" );

      if( ch_is_leader == TRUE )
      {
         mob_group_follow( ch, target );
      }
      do_follow( ch, target->name );
      do_group( target, ch->name );
   }
   return;
}



/* returns false if mob needed to cast a room affect spell */
bool ready_heal_room( CHAR_DATA * ch )
{
   bool ready = TRUE;

   if( ( !IS_SET( ch->in_room->affected_by, ROOM_BV_HEAL_REGEN ) ) && ( ch->hit < ch->max_hit * 75 / 100 ) )
   {
      if( ch->mana >= mana_cost( ch, skill_lookup( "healing light" ) ) )
      {
         ready = FALSE;
         do_cast( ch, "'healing light'" );
         return ready;
      }
   }

   if( ( !IS_SET( ch->in_room->affected_by, ROOM_BV_MANA_REGEN ) ) && ( ch->mana < ch->max_mana * 75 / 100 ) )
   {
      if( ch->mana >= mana_cost( ch, skill_lookup( "mana flare" ) ) )
      {
         ready = FALSE;
         do_cast( ch, "'mana flare'" );
         return ready;
      }
   }

   /*
    * don't cast seal room in midgaard 
    */

// ZEN FIX this to check for bad room spells allowed in area

//   if (  ( ch->in_room->vnum > ROOM_VNUM_MID_TOP
//      ||   ch->in_room->vnum < ROOM_VNUM_MID_BOTTOM )   
//      && ( !IS_SET( ch->in_room->affected_by, ROOM_BV_ENCAPS ) )  )
//   {
//      if ( ch->mana >= mana_cost( ch, skill_lookup ( "seal room" ) )  )
//      {
//        ready = FALSE;
//        do_cast ( ch, "'seal room'" );
//        return ready;
//      }
//   }

   return ready;
}

/* checks to see if mob needs to stand up for any reason, if so then stand. */
void need_to_stand( CHAR_DATA * ch )
{
   int current_state;
   CHAR_DATA *vch;

   current_state = ch->position;

   /*
    * if someone in your group is fighting, get up 
    */
   for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
   {
      if( ( is_same_group( ch, vch ) ) && ( vch->position == POS_FIGHTING ) )
      {
         get_up( ch, current_state );
         return;
      }
   }

   /*
    * if your leader is up and ready to move, get up 
    */
   if( ( ch->leader != NULL )
       && ( ch->in_room == ch->leader->in_room )
       && ( ch->leader->position == POS_STANDING )
       && ( ch->leader->hit >= ch->leader->max_hit * 85 / 100 ) && ( ch->leader->mana >= ch->leader->max_mana * 85 / 100 ) )
   {
      get_up( ch, current_state );
      return;
   }


   /*
    * Do you need heal? if so, can you heal? 
    */
   if( ch->hit < ch->max_hit * 85 / 100 )
   {
      if( ( ch->mana >= mana_cost( ch, skill_lookup( "heal" ) ) )
          || ( ch->mana >= mana_cost( ch, skill_lookup( "cure critical" ) ) )
          || ( ch->mana >= mana_cost( ch, skill_lookup( "cure serious" ) ) ) )
      {
         get_up( ch, current_state );
         return;
      }

   }

   /*
    * if there is an int mob in the room stand so that you can group with 
    * * it 
    */
   if( ch->leader == NULL )
   {
      for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
      {
         if( ( AI_MOB( vch ) )
             && ( vch->leader == NULL )
             && ( !is_same_group( vch, ch ) )
             && ( IS_SET( vch->act, ACT_INTELLIGENT ) )
             && ( vch != ch )
             && ( ( get_psuedo_level( vch ) - get_psuedo_level( ch ) <= 20 )
                  && ( get_psuedo_level( vch ) - get_psuedo_level( ch ) >= -20 ) ) )
         {
            get_up( ch, current_state );
            return;
         }
      }
   }

   /*
    * if you're ready to move, stand 
    */
   if( ( ch->hit >= ch->max_hit * 85 / 100 ) && ( ch->mana >= ch->max_mana * 85 / 100 ) )
      get_up( ch, current_state );

   return;
}

void get_up( CHAR_DATA * ch, int current_state )
{

   if( current_state == POS_SLEEPING )
      do_wake( ch, "" );
   else if( current_state == POS_RESTING )
      do_stand( ch, "" );

   return;
}


void mob_regen_check( CHAR_DATA * ch, CHAR_DATA * target, bool need_flee )
{
   char buf[MAX_STRING_LENGTH];

   if( target == NULL )
      target = ch;

   if( ch->mana >= mana_cost( ch, skill_lookup( "heal" ) ) )
      sprintf( buf, "'heal' %s", target->name );
   else if( ch->mana >= mana_cost( ch, skill_lookup( "cure critical" ) ) )
      sprintf( buf, "'cure critical' %s", target->name );
   else if( ch->mana >= mana_cost( ch, skill_lookup( "cure serious" ) ) )
      sprintf( buf, "'cure serious' %s", target->name );
   else if( need_flee == TRUE )
   {
      do_flee( ch, "" );
      return;
   }
   else if( need_flee == FALSE )
      return;

   do_cast( ch, buf );
   return;
}

void mob_is_fighting( CHAR_DATA * ch )
{
   CHAR_DATA *vch;
   CHAR_DATA *target = NULL;
   bool is_being_attacked = FALSE;
   bool need_flee = FALSE;

   /*
    * check to see if you are the one being attacked 
    */
   for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
   {
      if( vch->fighting == ch )
      {
         is_being_attacked = TRUE;
         break;
      }
   }

   /*
    * if you have a leader and he/she is present, they should rescue you if 
    * * you are being attacked, else you should check on them in case they 
    * * need heals 
    */
   if( ( ch->leader != NULL ) && ( ch->leader->in_room == ch->in_room ) )

   {
      target = ch->leader;

      if( is_being_attacked == TRUE && target != ch )
         do_rescue( target, ch->name );
      else if( target->hit < target->max_hit * 50 / 100 )
         mob_regen_check( ch, target, need_flee );
      else
      {
         for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
         {
            if( ( is_same_group( ch, vch ) ) && ( vch->hit < vch->max_hit * 20 / 100 ) )
            {
               mob_regen_check( ch, vch, need_flee );
               return;
            }
         }
      }

      return;
   }


   /*
    * if flow reaches here, you are the tank 
    */
   /*
    * either heal yourself or flee 
    */
   if( ch->hit < ch->max_hit * 50 / 100 )
   {
      if( ch->hit < ch->max_hit * 20 / 100 )
         need_flee = TRUE;
      mob_regen_check( ch, target, need_flee );
   }

   return;
}

void power_up_mob( CHAR_DATA * ch )
{
   int num_percent;

   /*
    * you can only have one cloak spell 
    */
   if( !IS_AFFECTED( ch, AFF_CLOAK_ABSORPTION )
       && !IS_AFFECTED( ch, AFF_CLOAK_REFLECTION ) && !IS_AFFECTED( ch, AFF_CLOAK_FLAMING ) )
   {
      num_percent = number_percent(  );

      if( num_percent > 75 )
      {
         if( ch->mana >= mana_cost( ch, skill_lookup( "cloak:flaming" ) ) )
            do_cast( ch, "'cloak:flaming'" );
         return;
      }

      if( num_percent > 50 )
      {
         if( ch->mana >= mana_cost( ch, skill_lookup( "cloak:reflection" ) ) )
            do_cast( ch, "cloak:reflection" );
         return;
      }

      if( num_percent > 25 )
      {
         if( ch->mana >= mana_cost( ch, skill_lookup( "cloak:absorption" ) ) )
            do_cast( ch, "cloak:absorption" );
         return;
      }
   }

   /*
    * usually i only have mobs do one thing per round but what they hell 
    * * let the kids have their fun :) 
    */
   if( !IS_AFFECTED( ch, AFF_SANCTUARY ) )
      if( ch->mana >= mana_cost( ch, skill_lookup( "sanctuary" ) ) )
         do_cast( ch, "sanctuary" );
   if( !IS_AFFECTED( ch, AFF_PROTECT ) )
      if( ch->mana >= mana_cost( ch, skill_lookup( "protection" ) ) )
         do_cast( ch, "protection" );
   if( !IS_AFFECTED( ch, skill_lookup( "bless" ) ) )
      if( ch->mana >= mana_cost( ch, skill_lookup( "bless" ) ) )
         do_cast( ch, "bless" );
   if( !IS_AFFECTED( ch, skill_lookup( "stone skin" ) ) )
      if( ch->mana >= mana_cost( ch, skill_lookup( "stone skin" ) ) )
         do_cast( ch, "stone" );
   return;
}

void mob_is_standing( CHAR_DATA * ch )
{
   sh_int dir;
   CHAR_DATA *vch;
   CHAR_DATA *tch;
   bool ready = TRUE;
   bool prey_still_exist = FALSE;
   int number_got_up = 0;
   int number_of_group = 1;
   int number_of_other_group = 1;

   /*
    * get a light source 
    */
   if( ch->in_room->light <= 0 )
   {
      if( ch->mana >= mana_cost( ch, skill_lookup( "continual light" ) ) )
      {
         do_cast( ch, "'continual light'" );
         do_get( ch, "all" );
         do_wear( ch, "all" );
         return;
      }
   }

   if( ( IS_AFFECTED( ch, AFF_POISON ) ) || ( IS_AFFECTED( ch, AFF_BLIND ) ) )
   {
      if( IS_AFFECTED( ch, AFF_POISON ) )
         if( ch->mana >= mana_cost( ch, skill_lookup( "cure poison" ) ) )
            do_cast( ch, "'cure poison'" );
      if( IS_AFFECTED( ch, AFF_BLIND ) )
         if( ch->mana >= mana_cost( ch, skill_lookup( "cure blindness" ) ) )
            do_cast( ch, "'cure blindness'" );
      return;
   }

   /*
    * is anyone in group being attacked? if so, assist! 
    */
   /*
    * -leaders will be forced to rescue in the 'mob_is_fighting' function
    * * already so no need to check for it here 
    */
   for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
   {
      if( ( is_same_group( ch, vch ) ) && ( vch->fighting != NULL ) && ( vch != ch ) )
      {
         do_assist( ch, "" );
         return;
      }
   }
   if( ch->leader == NULL )
   {
      for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
      {
         if( is_same_group( ch, vch ) && ( ch != vch ) )
         {
            number_of_group = number_of_group + 1;
         }
      }
      if( number_of_group < 4 )
      {
         for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
         {
            if( vch->leader != NULL )
               continue;

            if( ( vch != ch ) && ( IS_NPC( vch ) )
                && ( IS_SET( vch->act, ACT_INTELLIGENT ) )
                && ( !is_same_group( ch, vch ) )
                && ( vch->position == POS_STANDING )
                && ( ( get_psuedo_level( vch ) - get_psuedo_level( ch ) <= 20
                       && get_psuedo_level( vch ) - get_psuedo_level( ch ) >= -20 )
                     || ( get_psuedo_level( ch ) - get_psuedo_level( vch ) <= 20
                          && get_psuedo_level( ch ) - get_psuedo_level( vch ) >= -20 ) )
                && ( can_see( vch, ch ) ) && ( can_see( ch, vch ) ) )

            {
               if( vch->leader == NULL )
               {
                  for( tch = vch->in_room->first_person; tch != NULL; tch = tch->next_in_room )
                  {
                     if( is_same_group( tch, vch ) && ( tch != vch ) )
                     {
                        number_of_other_group = number_of_other_group + 1;
                     }
                  }
               }

               if( number_of_group + number_of_other_group <= 4 )
               {
                  get_mob_group( ch, vch );
                  return;
               }
            }
         }
      }
   }

   /*
    * do you need to heal? 
    */
   if( ch->hit < ch->max_hit * 85 / 100 )
   {
      if( ( ch->mana >= mana_cost( ch, skill_lookup( "heal" ) ) )
          || ( ch->mana >= mana_cost( ch, skill_lookup( "cure critical" ) ) )
          || ( ch->mana >= mana_cost( ch, skill_lookup( "cure serious" ) ) ) )

         mob_regen_check( ch, NULL, FALSE );

      /*
       * if leader is ready to move, just keep standing 
       */
      if( ( ch->leader != NULL )
          && ( ch->leader->in_room == ch->in_room )
          && ( ch->leader->position == POS_STANDING )
          && ( ch->leader->mana >= ch->leader->max_mana * 85 / 100 )
          && ( ch->leader->hit >= ch->leader->max_hit * 85 / 100 ) )
         return;
      else
      {
         if( ch->mana >= ch->max_mana * 75 / 100 )
            ready = ready_heal_room( ch );

         if( ready == TRUE )
            do_sleep( ch, "" );

         return;
      }
   }


   if( ch->mana < ch->max_mana * 85 / 100 )
   {
      do_sleep( ch, "" );
      return;
   }


   /*
    * do you need to level? if you have a group leader, have the leader 
    * * find a the trainer. if you are the leader just go and find the
    * * trainer 
    */

// ZEN FIX Have them recall then hunt the room

   if( able_to_level( ch ) )
   {
      char_from_room( ch );
      char_to_room( ch, get_room_index( 3758 ) );
      gain_level( ch );
      return;
/*
      dir = h_find_dir ( get_room_index(ch->in_room->vnum), 
			 get_room_index(ROOM_VNUM_INT_HEAL),
			 ch->hunt_flags );

      if (  dir == -1  )
	 gain_level ( ch );
      else 
      { 
	 if (  ( ch->leader != NULL )
	    && ( ch->leader->in_room == ch->in_room )  )
	 {
	    hunt_move ( ch->leader, dir );
	    end_hunt ( ch->leader );
	    return;
	 }
	 else
	 {
	    hunt_move ( ch, dir );
	    return;
	 }
      }  */
   }

   /*
    * if you're leader and you don't need to gain level, does anyone else 
    * * in the group? 
    */
   /*
    * actually, the above function will force the leader to find a trainer 
    * * already.  but since i don't want the leader to select a new target 
    * * until the group gains the needed level, i'll put this check here 
    */
   for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
   {
      if( ( is_same_group( vch, ch ) )
          && ( vch->in_room == ch->in_room ) && ( vch->leader == ch ) && ( able_to_level( vch ) ) )
      {
         dir = h_find_dir( get_room_index( ch->in_room->vnum ), get_room_index( ROOM_VNUM_INT_HEAL ), ch->hunt_flags );
         hunt_move( ch, dir );
         return;
      }
   }

   /*
    * if noone needs to heal or gain level, then let's hunt! 
    */
   /*
    * by the way, only leaders will hunt. followers will just follow and 
    * * assist when needed 
    */
   if( ( ch->leader != NULL ) && ( ch->leader->in_room == ch->in_room ) && ( ch->hunting != NULL ) )
   {
      end_hunt( ch );
   }
   else if( ( ch->leader != NULL ) && ( ch->leader->in_room == ch->in_room ) && ( ch->hunting == NULL ) )
   {
      return;
   }
   else if( is_group_leader( ch ) )
   {
      for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
      {
         if( ( vch != ch ) && ( is_same_group( vch, ch ) ) && ( vch->position != POS_STANDING ) )
         {
            get_up( vch, vch->position );
            number_got_up = number_got_up + 1;
         }
      }
      if( number_got_up != 0 )
         return;
   }
   else
   {
      if( ch->hunting != NULL )
      {
         for( vch = first_char; vch != NULL; vch = vch->next )
         {
            if( vch == ch->hunting )
            {
               prey_still_exist = TRUE;
               return;
            }
         }

         if( prey_still_exist == FALSE )
         {
            ch->hunting = NULL;
         }
      }
      if( ch->hunting == NULL && ch->leader == NULL )
      {
         select_target( ch );
         return;
      }
   }

   /*
    * power_up_mob ( ch ); 
    */

   /*
    * if (  ch->leader != NULL 
    * && ch->in_room != ch->leader->in_room )
    * {
    * do_follow( ch, ch->name );
    * }  
    */

   return;

}


bool valid_target( CHAR_DATA * ch, CHAR_DATA * victim, int l )
{
   /*
    * Return TRUE if victim is a valid target for ch to kill. 
    */

   /*
    * don't hunt people you can't even see 
    */
   if( !can_see( ch, victim ) )
      return FALSE;

   /*
    * Don't attack group members or self! 
    */
   if( is_same_group( ch, victim ) )
      return FALSE;

   /*
    * Don't attack other int mobs! 
    */
   if( IS_NPC( victim ) && IS_SET( victim->act, ACT_INTELLIGENT ) )
      return FALSE;

   /*
    * Don't attack players.... except for have spec_vamp_hunter 
    */
   if( ( !IS_NPC( victim ) ) && ( ch->spec_fun != spec_lookup( "spec_vamp_hunter" ) ) )
      return FALSE;

   /*
    * don't attack fairy godmother 
    */
   if( IS_NPC( victim ) )
      if( victim->pIndexData->vnum == 1026 )
         return FALSE;


   /*
    * if IS vamp_hunter, make sure target is a player vamp 
    */

   if( ( IS_VAMP( victim ) ) && ( !IS_NPC( victim ) ) && ( ch->spec_fun != spec_lookup( "spec_vamp_hunter" ) ) )
      return FALSE;

   /*
    * don't attack NPC VAMPS (they can't die ) 
    */
   if( ( IS_VAMP( victim ) ) && ( IS_NPC( victim ) ) )
      return FALSE;

   /*
    * Only kill victims of similar level 
    */
   if( ( ( get_psuedo_level( victim ) - get_psuedo_level( ch ) ) > -7 )
       || ( ( get_psuedo_level( ch ) - get_psuedo_level( victim ) ) > 12 ) )
      return FALSE;

//   if ( ( IS_GOOD( ch )    && IS_GOOD( victim    ) )
//   || (   IS_EVIL( ch )    && IS_EVIL( victim    ) )
//   || (   IS_NEUTRAL( ch ) && IS_NEUTRAL( victim ) ) )
//      return FALSE;

   if( ( ch->spec_fun == spec_lookup( "spec_vamp_hunter" ) ) && ( IS_NPC( victim ) ) && ( number_percent(  ) < 20 ) )
      return FALSE;

   if( ( IS_SET( victim->in_room->room_flags, ROOM_SAFE ) ) || ( IS_SET( victim->act, ACT_SOLO ) )
       /*
        * || ( IS_SET( victim->in_room->room_flags, ROOM_JAIL_CELL ) ) 
        */  )
      return FALSE;

   return TRUE;
}


void select_target( CHAR_DATA * ch )
{
   /*
    * Find a new target for the group to go after 
    */
   int average_level;
//   int        tmp   = 0;
   CHAR_DATA *vch;
   CHAR_DATA *victim = NULL;
   char buf[MAX_STRING_LENGTH];
   int force_index = 0;
   bool alone = TRUE;
   bool mob_is_leader = FALSE;
   sh_int attempts;

   /*
    * mobs were doing ethereal travel too much... i've now lowered it to 
    * * 15% of the time and only if they are not hunting 
    */

   mob_is_leader = is_group_leader( ch );
   if( ( number_percent(  ) < 15 ) && ( ch->hunting == NULL ) && ( ch->in_room->vnum != ROOM_VNUM_ETHEREAL_PLANE ) )

/* was victim == NULL, that's always true at this point.. Zen */

   {
      if( mob_is_leader == TRUE )
      {
         for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
         {
            if( ( is_same_group( ch, vch ) ) == TRUE )
            {
               if( vch->mana < mana_cost( vch, skill_lookup( "ethereal travel" ) ) )
               {
                  return;
               }
            }
         }
      }
      if( ch->mana < mana_cost( ch, skill_lookup( "ethereal travel" ) ) )
         return;

      do_say( ch, "This place is boring! I am gonna go somewhere else!" );
      for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
      {
         if( ( is_same_group( vch, ch ) ) && ( ch != vch ) )
         {
            do_say( vch, "Yeah, it is--we're outta here!" );
            do_cast( vch, "ethereal" );
         }
      }
      do_cast( ch, "ethereal" );
   }
   else
   {
      /*
       * keeps checking until you've found a valid target 
       */
      attempts = 0;
      while( ( victim == NULL ) && ( attempts < 15 ) )
      {
// ZEN FIX set average level based on level of ngroup
         attempts++;
         average_level = get_psuedo_level( ch );

         force_index = number_range( 1, 1200 );
/* we currently have about 1300 mobs..this should get a random enough sample */

         for( vch = first_char; vch != NULL; vch = vch->next )
         {
            if( victim != NULL )
               break;
            force_index--;
            if( force_index > 0 )
               continue;

            if( valid_target( ch, vch, average_level ) )
            {
               /*
                * Trick used in  something else... 
                */
               if( number_range( 0, 1 ) == 0 )
               {
                  victim = vch;
               }
               if( victim == NULL ) /* screwed up somehow */
               {
                  continue;
               }
               if( !IS_NPC( victim ) )
               {
                  for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
                  {
                     if( is_same_group( ch, vch ) )
                     {
                        alone = FALSE;
                        break;
                     }
                  }
                  if( alone == FALSE )
                  {
                     sprintf( buf, "%s We're coming for you!", victim->name );
                     do_tell( ch, buf );
                  }
                  else
                  {
                     sprintf( buf, "%s I'm coming for you!", victim->name );
                     do_tell( ch, buf );
                  }
               }
            }
         }
      }

      if( set_hunt( ch, NULL, victim, NULL, HUNT_WORLD | HUNT_PICKDOOR | HUNT_CR, HUNT_MERC ) )
      {
         sprintf( buf, "Right!  %s is our new target!!", victim->short_descr );
         do_say( ch, buf );
      }

      return;
   }
   return;

}

static const char *group_state_table[] = {
   "@@eFleeing@@N",
   "@@rFighting@@N",
   "@@eCritical Healing@@N",
   "@@aReforming@@N",
   "@@eCritical Mana@@N",
   "@@lHunting@@N",
   "@@cNormal Healing@@N",
   "@@cNormal Mana@@N",
   "@@WLevelling@@N",
   "@@yGetting EQ@@N",
   "@@pIdling@@N",
   "@@eLost@@N"
};

/* i have condensed this function to just three states: MOB_FIGHTING, 
 * MOB_RESTING/ MOB_SLEEPING, and MOB_STANDING. each of these three states 
 * will call it's appropriate function. */
void int_group_handler( NPC_GROUP_DATA * ngroup )
{
   CHAR_DATA *follower = NULL;
   CHAR_DATA *leader = ngroup->leader;
   DL_LIST *follower_ptr;
//  sh_int followers_want = GRP_STATE_NO_CHANGE;
   sh_int leader_wants = GRP_STATE_NO_CHANGE;
   sh_int group_count = 1; // start with leader
   char monbuf[MSL];

   if( leader == NULL )
   {
      monitor_chan( "No Leader in NPC_GROUP", MONITOR_MOB );
      return;
   }

// check for followers needs
   for( follower_ptr = ngroup->first_follower; follower_ptr; follower_ptr = follower_ptr->next )
   {
      // check for needing healing, levelling
      follower = follower_ptr->this_one;
      group_count++;
      continue;
   }

// check for leader's needs

   if( leader->hit < leader->max_hit * 25 / 100 )
   {
      leader_wants = GRP_STATE_CRIT_HEAL;
   }
   else if( leader->mana < leader->max_mana * 25 / 100 )
   {
      leader_wants = GRP_STATE_CRIT_MANA;
   }
   else if( leader->hit < leader->max_hit * 60 / 100 )
   {
      leader_wants = GRP_STATE_NORM_HEAL;
   }
   else if( leader->mana < leader->max_mana * 50 / 100 )
   {
      leader_wants = GRP_STATE_NORM_MANA;
   }
   else if( able_to_level( leader ) )
   {
      leader_wants = GRP_STATE_LEVELING;
   }
   sprintf( monbuf, "NPC Group Handler, Leader is %s, state is %s", ngroup->leader->name, group_state_table[ngroup->state] );
   monitor_chan( monbuf, MONITOR_MOB );

   switch ( ngroup->state )
   {


      case GRP_STATE_CRIT_HEAL:
      case GRP_STATE_CRIT_MANA:
      case GRP_STATE_NORM_HEAL:
      case GRP_STATE_NORM_MANA:
      {
         bool everyone_ready = TRUE;
         bool room_ready = FALSE;
//      ready_heal_room( leader );
         if( ( leader->mana < leader->max_mana * 85 / 100 ) || ( leader->hit < leader->max_hit * 85 / 100 ) )
         {
            everyone_ready = FALSE;
            if( ( ( room_ready = ready_heal_room( leader ) ) == TRUE ) || ( leader->mana < leader->max_mana * 20 / 100 ) )
            {
               do_sleep( leader, "" );
            }
         }
         else
         {
            do_stand( leader, "" );
         }
         for( follower_ptr = ngroup->first_follower; follower_ptr; follower_ptr = follower_ptr->next )
         {
            follower = follower_ptr->this_one;
            if( ( follower->mana < follower->max_mana * 75 / 100 ) || ( follower->hit < follower->max_hit * 75 / 100 ) )
            {
               everyone_ready = FALSE;
               do_sleep( follower, "" );
            }
            else
            {
               do_stand( follower, "" );
            }
         }
         if( everyone_ready == TRUE )
         {
            ngroup->state = GRP_STATE_IDLE;
         }
         break;
      }
      case GRP_STATE_FIGHT:
      {  // violence_update will handle
         if( ( leader_wants < GRP_STATE_HUNTING ) || ( leader->fighting == NULL ) )
         {
            bool someone_still_fighting = FALSE;

            ngroup->state = GRP_STATE_FLEE;
            for( follower_ptr = ngroup->first_follower; follower_ptr; follower_ptr = follower_ptr->next )
            {
               follower = follower_ptr->this_one;
               if( follower->fighting != NULL )
               {
                  do_flee( follower, "" );
                  someone_still_fighting = TRUE;
               }
            }
            if( someone_still_fighting == FALSE )
            {
               ngroup->state = GRP_STATE_REFORM;
            }
            if( leader->fighting != NULL )
            {
               do_flee( leader, "" );
            }
         }

         break;
      }

      case GRP_STATE_FLEE:
      {
         bool someone_still_fighting = FALSE;
         for( follower_ptr = ngroup->first_follower; follower_ptr; follower_ptr = follower_ptr->next )
         {
            follower = follower_ptr->this_one;
            if( follower->fighting != NULL )
            {
               do_flee( follower, "" );
               someone_still_fighting = TRUE;
            }
         }
         if( leader->fighting != NULL )
         {
            do_flee( leader, "" );
            someone_still_fighting = TRUE;
         }
         if( someone_still_fighting == FALSE )
         {
            ngroup->state = GRP_STATE_REFORM;
         }

         break;
      }
      case GRP_STATE_IDLE:
      {
//      check_re_equip( leader );
//      check_rewield( leader );
         if( leader_wants < GRP_STATE_NO_CHANGE )
         {
            ngroup->state = leader_wants;
            break;
         }
         else if( number_percent(  ) < 40 )
         {
            select_target( ngroup->leader );
            ngroup->state = GRP_STATE_HUNTING;
            break;
         }
      }
      case GRP_STATE_HUNTING:
      {  // poll followers later
         sh_int move_dir;

         if( leader->fighting != NULL )
         {
            ngroup->state = GRP_STATE_FIGHT;
            break;
         }
         if( leader->hunting == NULL )
         {
//        sprintf( monbuf, "Leader %s not hunting anything in GRP_STATE_HUNTING",
//              leader->name );
//        monitor_chan( monbuf, MONITOR_MOB );
            select_target( ngroup->leader );
            break;
         }
         if( leader->in_room == leader->hunting->in_room )
         {
            ngroup->state = GRP_STATE_FIGHT;
            multi_hit( leader, leader->hunting, TYPE_UNDEFINED );
            break;
         }
         move_dir = h_find_dir( leader->in_room, leader->hunting->in_room,
                                HUNT_WORLD | HUNT_OPENDOOR | HUNT_UNLOCKDOOR | HUNT_PICKDOOR );
         if( move_dir < 0 )   // can't get there from here
         {
            ngroup->state = GRP_STATE_LOST;
            break;
         }
         hunt_move( leader, move_dir );
         break;
      }
      case GRP_STATE_LEVELING:
      {
         char_from_room( leader );
         char_to_room( leader, get_room_index( 3758 ) );
         if( able_to_level( leader ) )
         {
            gain_level( leader );
         }
         for( follower_ptr = ngroup->first_follower; follower_ptr; follower_ptr = follower_ptr->next )
         {
            follower = follower_ptr->this_one;
            if( able_to_level( follower ) )
            {
               gain_level( follower );
            }
         }
         ngroup->state = GRP_STATE_IDLE;
         break;
      }
      case GRP_STATE_REFORM:
      {
         bool all_are_here = TRUE;
         for( follower_ptr = ngroup->first_follower; follower_ptr; follower_ptr = follower_ptr->next )
         {
            follower = follower_ptr->this_one;
            if( follower->in_room != leader->in_room )
            {
               sh_int move_dir;
               all_are_here = FALSE;
               move_dir = h_find_dir( follower->in_room, leader->in_room,
                                      HUNT_WORLD | HUNT_OPENDOOR | HUNT_UNLOCKDOOR | HUNT_PICKDOOR );
               if( move_dir < 0 )   // can't get there from here
               {
                  ngroup->state = GRP_STATE_LOST;
                  break;
               }
               hunt_move( follower, move_dir );
            }
         }
         if( all_are_here == TRUE )
         {
            ngroup->state = GRP_STATE_IDLE;
         }
         break;
      }

   }
}


void int_handler( CHAR_DATA * ch )
{
   int current_state;

   current_state = ch->position;

   /*
    * if you're fighting check on your condition 
    */
   if( current_state == POS_FIGHTING )
   {
      mob_is_fighting( ch );
      return;
   }

   /*
    * if you are resting or sleeping, check to see if you need to stand up 
    */
   if( current_state != POS_STANDING )
   {
      need_to_stand( ch );
      return;
   }

   /*
    * cheat so that players cannot get easy xp from almost dead int mobs 
    */
   /*
    * not sure where to place this yet -- will look at this later 
    */
   if( ch->hit < 100 )
   {
      ch->hit = ch->max_hit * 50 / 100;
      ch->mana = ch->max_mana * 50 / 100;
   }

   /*
    * by now you should be standing 
    */
   mob_is_standing( ch );
   return;
}

/* i've never learned cases before so i'm pretty much leaving this one 
 * alone, except for taking out the one_arguement() function */
void int_combat_handler( CHAR_DATA * ch, CHAR_DATA * victim )
{
   /*
    * Called from fight.c during combat to enable mobs to use spells and
    * skills.  ACT_INTELLIGENT mobs can call cast() now.
    * --Stephen   
    */

   char arg[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *vch;
   int sn;
   int counter = 1;

   if( number_percent(  ) < 65 )
      return;

   for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
   {
      if( vch == victim )
      {
         sprintf( buf, "%d.%s", counter, vch->name );
         break;
      }

      counter = counter + 1;
   }

   switch ( number_range( 0, 5 ) )  /* Pick a skill or a spell */
   {
      case 0:
      case 1:
      case 2:
      case 3:
         /*
          * Use a skill 
          */
         switch ( number_range( 0, 5 ) )
         {
            case 0:
               sprintf( arg, "frenzy" );
               break;
            case 1:
               sprintf( arg, "punch %s", buf );
               break;
            case 2:
               sprintf( arg, "knee %s", buf );
               break;
            case 3:
               sprintf( arg, "headbutt %s", buf );
               break;
            case 4:
               sprintf( arg, "punch %s", buf );
               break;
            case 5:
               sprintf( arg, "dirt %s", buf );
               break;
         }
         interpret( ch, arg );
         do_say( ch, buf );
         break;
      default:
         sn = find_spell( ch, TAR_CHAR_OFFENSIVE );
         if( ( sn != -1 ) && ( ch->mana > mana_cost( ch, sn ) ) )
         {
            sprintf( arg, "cast '%s' %s", skill_table[sn].name, buf );
            interpret( ch, arg );
         }
   }
   return;
}
