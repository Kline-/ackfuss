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
#include <string.h>
#include <time.h>
#include "globals.h"

#ifndef DEC_MONEY_H
#include "money.h"
#endif

#ifndef DEC_ACT_MOB_H
#include "act_mob.h"
#endif

extern OBJ_DATA *quest_object;
extern bool quest;
extern CHAR_DATA *quest_mob;
extern CHAR_DATA *quest_target;
extern COUNCIL_DATA super_councils[MAX_SUPER];


/*
 * Local functions.
 */
void affect_modify args( ( CHAR_DATA * ch, AFFECT_DATA * paf, bool fAdd ) );

/*
 * Updated pointer referencing, curtesy of Spectrum, from Beyond the Veil
 *
 */



struct obj_ref_type *obj_ref_list;

void obj_reference( struct obj_ref_type *ref )
{
   if( ref->inuse )
   {
      bugf( "Reused obj_reference!" );
      abort(  );
   }

   ref->inuse = TRUE;
   ref->next = obj_ref_list;
   obj_ref_list = ref;
}

void obj_unreference( OBJ_DATA ** var )
{
   struct obj_ref_type *p, *last;

   for( p = obj_ref_list, last = NULL; p && p->var != var; last = p, p = p->next )
      ;

   if( !p )
   {
      bugf( "obj_unreference: var not found" );
      return;
   }

   p->inuse = FALSE;

   if( !last )
      obj_ref_list = obj_ref_list->next;
   else
      last->next = p->next;
}

struct char_ref_type *char_ref_list;

void char_reference( struct char_ref_type *ref )
{
   if( ref->inuse )
   {
      bugf( "Reused char_reference!" );
      abort(  );
   }

   ref->inuse = TRUE;
   ref->next = char_ref_list;
   char_ref_list = ref;
}

void char_unreference( CHAR_DATA ** var )
{
   struct char_ref_type *p, *last;

   for( p = char_ref_list, last = NULL; p && p->var != var; last = p, p = p->next )
      ;

   if( !p )
   {
      bugf( "char_unreference: var not found" );
      return;
   }

   p->inuse = FALSE;

   if( !last )
      char_ref_list = char_ref_list->next;
   else
      last->next = p->next;
}



/*
 * Retrieve a character's trusted level for permission checking.
 */
int get_trust( CHAR_DATA * ch )
{
   if( ch->desc != NULL && ch->desc->original != NULL )
      ch = ch->desc->original;

   if( !IS_NPC( ch ) && IS_SET( ch->pcdata->pflags, PFLAG_AMBAS ) )
      return ( LEVEL_HERO + 1 );

   if( ch->trust != 0 )
      return ch->trust;

   if( IS_NPC( ch ) && ch->level >= LEVEL_HERO )
      return LEVEL_HERO - 1;
   else
      return ch->level;
}


/* 
 * Replacement for retrieving a character's age
 * Each tick = 1 mud hr.  (spaced at 1 minute rl)
 * 24 mud hrs = 1 mud day
 * 20 mud days = 1 mud month
 * 8 mud months = 1 mud year
 * Therefore, 24*20*8 = 3840 ticks/mins.
 * Returns a string with age info in for use by score, mst, etc
 */

void my_get_age( CHAR_DATA * ch, char *buf )
{
   int days, years, months;
   int base, ticks;

   /*
    * Base = time in seconds ch has been playing... 
    */
   base = ch->played + ( int )( current_time - ch->logon );

   ticks = base / 60;   /* 1 tick = 60 seconds */

   days = ( ticks / 24 ) % 20;
   months = ( ticks / 480 ) % 8;
   years = 17 + ( ticks / 3840 );

   sprintf( buf + strlen( buf ), "%d years, %d months and %d days", years, months, days );
   return;
}

/* Simple function to return number of hours a character has played */
int my_get_hours( CHAR_DATA * ch )
{
   int secs;
   int hrs;

   secs = ch->played + ( int )( current_time - ch->logon );
   hrs = ( secs / 3600 );

   return hrs;
}




/*
 * Retrieve a character's age.
 */
int get_age( CHAR_DATA * ch )
{
   return 17 + ( ch->played + ( int )( current_time - ch->logon ) ) / 14400;

   /*
    * 12240 assumes 30 second hours, 24 hours a day, 20 day - Kahn 
    */
}


/*
 * Retrieve character's current strength.
 */
int get_curr_str( CHAR_DATA * ch )
{
   int max;

   if( IS_NPC( ch ) )
   {
      return ( 13 + ( ch->level / 16 ) );
   }

   max = ch->pcdata->max_str;

   return URANGE( 3, ch->pcdata->perm_str + ch->pcdata->mod_str, max );
}



/*
 * Retrieve character's current intelligence.
 */
int get_curr_int( CHAR_DATA * ch )
{
   int max;

   if( IS_NPC( ch ) )
   {
      return ( 15 + number_fuzzy( ( ch->level / 20 ) ) );
   }

   max = ch->pcdata->max_int;

   return URANGE( 3, ch->pcdata->perm_int + ch->pcdata->mod_int, max );
}



/*
 * Retrieve character's current wisdom.
 */
int get_curr_wis( CHAR_DATA * ch )
{
   int max;

   if( IS_NPC( ch ) )
   {
      return ( 15 + number_fuzzy( ( ch->level / 20 ) ) );
   }

   max = ch->pcdata->max_wis;

   return URANGE( 3, ch->pcdata->perm_wis + ch->pcdata->mod_wis, max );
}



/*
 * Retrieve character's current dexterity.
 */
int get_curr_dex( CHAR_DATA * ch )
{
   int max;

   if( IS_NPC( ch ) )
   {
      return ( 16 + number_fuzzy( ( ch->level / 25 ) ) );
   }

   max = ch->pcdata->max_dex;

   return URANGE( 3, ch->pcdata->perm_dex + ch->pcdata->mod_dex, max );
}



/*
 * Retrieve character's current constitution.
 */
int get_curr_con( CHAR_DATA * ch )
{
   int max;

   if( IS_NPC( ch ) )
   {
      return ( 15 + number_fuzzy( ( ch->level / 12 ) ) );
   }

   max = ch->pcdata->max_con;

   return URANGE( 3, ch->pcdata->perm_con + ch->pcdata->mod_con, max );
}



/*
 * Retrieve a character's carry capacity.
 */
int can_carry_n( CHAR_DATA * ch )
{
   if( !IS_NPC( ch ) && ch->level >= LEVEL_IMMORTAL )
      return 500;

   /*
    * if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )
    * return 0;  
    */

   return MAX_WEAR + 2 * get_curr_dex( ch ) / 2;
}



/*
 * Retrieve a character's carry capacity.
 */
int can_carry_w( CHAR_DATA * ch )
{
   if( !IS_NPC( ch ) && ch->level >= LEVEL_IMMORTAL )
      return 9999999;

/*    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )
	return 0;   */

   return str_app[get_curr_str( ch )].carry;
}


/*
 * Apply or remove an affect to a character.
 */
void affect_modify( CHAR_DATA * ch, AFFECT_DATA * paf, bool fAdd )
{
   char buf[MAX_STRING_LENGTH];
   OBJ_DATA *wield;
   int mod;

   mod = paf->modifier;

   if( fAdd )
   {
      SET_BIT( ch->affected_by, paf->bitvector );
   }
   else
   {
      REMOVE_BIT( ch->affected_by, paf->bitvector );
      mod = 0 - mod;
   }

   if( IS_NPC( ch ) )
   {
      switch ( paf->location )
      {
         default:
            return;
         case APPLY_MANA:
            ch->max_mana += mod;
            break;
         case APPLY_HIT:
            ch->max_hit += mod;
            break;
         case APPLY_MOVE:
            ch->max_move += mod;
            break;
         case APPLY_AC:
            ch->armor += mod;
            break;
         case APPLY_HITROLL:
            ch->hitroll += mod;
            break;
         case APPLY_DAMROLL:
            ch->damroll += mod;
            break;
         case APPLY_SAVING_PARA:
            ch->saving_throw += mod;
            break;
         case APPLY_SAVING_ROD:
            ch->saving_throw += mod;
            break;
         case APPLY_SAVING_PETRI:
            ch->saving_throw += mod;
            break;
         case APPLY_SAVING_BREATH:
            ch->saving_throw += mod;
            break;
         case APPLY_SAVING_SPELL:
            ch->saving_throw += mod;
            break;
      }
      return;
   }

   if( paf->type == skill_lookup( "Enraged" ) )
      REMOVE_BIT( ch->pcdata->pflags, PFLAG_RAGED );

   switch ( paf->location )
   {
      default:
         bug( "Affect_modify: unknown location %d.", paf->location );
         sprintf( buf, "Affect_modify: called for %s - unknown location %d.", ch->name, paf->location );
         monitor_chan( buf, MONITOR_OBJ );
         return;

      case APPLY_NONE:
         break;
      case APPLY_STR:
         ch->pcdata->mod_str += mod;
         break;
      case APPLY_DEX:
         ch->pcdata->mod_dex += mod;
         break;
      case APPLY_INT:
         ch->pcdata->mod_int += mod;
         break;
      case APPLY_WIS:
         ch->pcdata->mod_wis += mod;
         break;
      case APPLY_CON:
         ch->pcdata->mod_con += mod;
         break;
      case APPLY_SEX:
         ch->sex += mod;
         break;
      case APPLY_CLASS:
         break;
      case APPLY_LEVEL:
         break;
      case APPLY_AGE:
         break;
      case APPLY_HEIGHT:
         break;
      case APPLY_WEIGHT:
         break;
      case APPLY_MANA:
         ch->max_mana += mod;
         break;
      case APPLY_HIT:
         ch->max_hit += mod;
         break;
      case APPLY_MOVE:
         ch->max_move += mod;
         break;
      case APPLY_GOLD:
         break;
      case APPLY_EXP:
         break;
      case APPLY_AC:
         ch->armor += mod;
         break;
      case APPLY_HITROLL:
         ch->hitroll += mod;
         break;
      case APPLY_DAMROLL:
         ch->damroll += mod;
         break;
      case APPLY_SAVING_PARA:
         ch->saving_throw += mod;
         break;
      case APPLY_SAVING_ROD:
         ch->saving_throw += mod;
         break;
      case APPLY_SAVING_PETRI:
         ch->saving_throw += mod;
         break;
      case APPLY_SAVING_BREATH:
         ch->saving_throw += mod;
         break;
      case APPLY_SAVING_SPELL:
         ch->saving_throw += mod;
         break;
   }

   /*
    * Check for weapon wielding.
    * Guard against recursion (for weapons with affects).
    */
   if( ( ch->is_quitting == FALSE ) && ( ch->desc != NULL ) && ( ch->desc->connected != CON_SETTING_STATS ) )
   {
      sh_int i;
      for( i = 0; i < MAX_WEAR; i++ )
      {
         if( ( ( wield = get_eq_char( ch, i ) ) != NULL )
             && ( get_obj_weight( wield ) > str_app[get_curr_str( ch )].wield ) )
         {
            static int depth;

            if( depth == 0 )
            {
               depth++;
               act( "You stop using $p since it is too heavy.", ch, wield, NULL, TO_CHAR );
               act( "$n stops using $p. since it is too heavy", ch, wield, NULL, TO_ROOM );
               unequip_char( ch, wield );
/*	    obj_to_room( wield, ch->in_room );  */
               depth--;
            }
         }
      }
   }
   return;
}


void mark_to_room( int this_room_vnum, MARK_DATA * mark )
{
   /*
    * this assumes that the mark_data is good, and has
    * * been constructed properly. It will link it to the room 
    * * and the main list. Caller must get the struct memory. 
    * *
    */
   MARK_LIST_MEMBER *mlist;
   MARK_LIST_MEMBER *rlist;
   ROOM_INDEX_DATA *this_room;

   if( ( this_room = get_room_index( this_room_vnum ) ) == NULL )
   {
      PUT_FREE( mark, mark_free );
      return;
   }


   GET_FREE( mlist, mark_list_free );
   GET_FREE( rlist, mark_list_free );

   mlist->mark = mark;
   rlist->mark = mark;

   LINK( mlist, first_mark_list, last_mark_list, next, prev );
   LINK( rlist, this_room->first_mark_list, this_room->last_mark_list, next, prev );
   if( !booting_up )
      save_marks(  );

   return;
}

void mark_from_room( int this_room_vnum, MARK_DATA * mark )
{

   MARK_LIST_MEMBER *mlist;
   MARK_LIST_MEMBER *rlist;
   ROOM_INDEX_DATA *this_room;

   this_room = get_room_index( this_room_vnum );


   for( rlist = this_room->first_mark_list; rlist != NULL; rlist = rlist->next )
   {
      if( rlist->mark == mark )
      {
         UNLINK( rlist, this_room->first_mark_list, this_room->last_mark_list, next, prev );
         break;
      }
   }
   for( mlist = first_mark_list; mlist != NULL; mlist = mlist->next )
   {
      if( mlist->mark == mark )
      {
         UNLINK( mlist, first_mark_list, last_mark_list, next, prev );
         break;
      }
   }

   PUT_FREE( mark, mark_free );
   PUT_FREE( rlist, mark_list_free );
   PUT_FREE( mlist, mark_list_free );
   if( !booting_up )
      save_marks(  );

   return;
}


/* Give an affect to a room */
void affect_to_room( ROOM_INDEX_DATA * room, ROOM_AFFECT_DATA * raf )
{
   ROOM_AFFECT_DATA *raf_new;
   char buf[MAX_STRING_LENGTH];

   GET_FREE( raf_new, raffect_free );
/* Ramias... Don't copy uninitialized fields: next, prev, is_free */
/*
   *raf_new = *raf;
*/
   raf_new->duration = raf->duration;
   raf_new->level = raf->level;
   raf_new->type = raf->type;
   raf_new->bitvector = raf->bitvector;
   raf_new->applies_spell = raf->applies_spell;
   raf_new->modifier = raf->modifier;
   raf_new->location = raf->location;
   raf_new->caster = raf->caster;

   LINK( raf_new, room->first_room_affect, room->last_room_affect, next, prev );

   SET_BIT( room->affected_by, raf->bitvector );

   sprintf( buf, "@@e%s@@N has cast @@d%s@@N in @@Narea: @@r%s@@N, @@Nroom: @@r%d@@N.",
            raf->caster->name, raffect_bit_name( raf->bitvector ), room->area->name, room->vnum );
   monitor_chan( buf, MONITOR_GEN_MORT );


   return;
}

/* Remove an affect from a room */
void r_affect_remove( ROOM_INDEX_DATA * room, ROOM_AFFECT_DATA * raf )
{
   if( room->first_room_affect == NULL )
   {
      char buf[MAX_STRING_LENGTH];
      sprintf( buf, "r_affect_remove: no affect to remove from room %d.", room->vnum );
      monitor_chan( buf, MONITOR_ROOM );

      bug( "R_affect_remove: no affect for room: %d.", room->vnum );
      return;
   }

   REMOVE_BIT( room->affected_by, raf->bitvector );

   UNLINK( raf, room->first_room_affect, room->last_room_affect, next, prev );
   PUT_FREE( raf, raffect_free );
   return;
}



/*
 * Give an affect to a char.
 */
void affect_to_char( CHAR_DATA * ch, AFFECT_DATA * paf )
{
   AFFECT_DATA *paf_new;

   GET_FREE( paf_new, affect_free );
/* Ramias... Don't copy uninitialized fields: next, prev, is_free */
/*
    *paf_new = *paf;
*/
   paf_new->type = paf->type;
   paf_new->duration = paf->duration;
   paf_new->location = paf->location;
   paf_new->modifier = paf->modifier;
   paf_new->bitvector = paf->bitvector;
   paf_new->caster = paf->caster;
   paf_new->level = paf->level;
   LINK( paf_new, ch->first_affect, ch->last_affect, next, prev );

   affect_modify( ch, paf_new, TRUE );

   if( paf_new->type == skill_lookup( "Enraged" ) )
      if( !IS_NPC( ch ) && IS_WOLF( ch ) )
         SET_BIT( ch->pcdata->pflags, PFLAG_RAGED );

   return;
}



/*
 * Remove an affect from a char.
 */
void affect_remove( CHAR_DATA * ch, AFFECT_DATA * paf )
{
   sh_int shield_type;

   if( ch->first_affect == NULL )
   {
      char buf[MAX_STRING_LENGTH];
      sprintf( buf, "affect_remove: %s did not have aff %d to remove.",
               IS_NPC( ch ) ? ch->short_descr : ch->name, paf->type );
      monitor_chan( buf, MONITOR_MOB );

      bug( "Affect_remove: no affect.", 0 );
      return;
   }

   affect_modify( ch, paf, FALSE );
   shield_type = SHIELD_NONE;

   if( paf->type == skill_lookup( "fireshield" ) )
      shield_type = FLAME_SHIELD;
   else if( paf->type == skill_lookup( "iceshield" ) )
      shield_type = ICE_SHIELD;
   else if( paf->type == skill_lookup( "shockshield" ) )
      shield_type = SHOCK_SHIELD;
   else if( paf->type == skill_lookup( "shadowshield" ) )
      shield_type = SHADOW_SHIELD;
   else if( paf->type == skill_lookup( "thoughtshield" ) )
      shield_type = PSI_SHIELD;
   if( shield_type > SHIELD_NONE )
   {
      MAGIC_SHIELD *this_shield;

      for( this_shield = ch->first_shield; this_shield != NULL; this_shield = this_shield->next )
         if( this_shield->type == shield_type )
            break;
      if( this_shield != NULL )
      {
         char buf1[MSL];
         char buf2[MSL];

         sprintf( buf1, this_shield->wearoff_room );
         sprintf( buf2, this_shield->wearoff_self );
         act( buf1, ch, NULL, NULL, TO_ROOM );
         act( buf2, ch, NULL, NULL, TO_CHAR );

         UNLINK( this_shield, ch->first_shield, ch->last_shield, next, prev );
         PUT_FREE( this_shield, shield_free );
      }
   }

   UNLINK( paf, ch->first_affect, ch->last_affect, next, prev );
   PUT_FREE( paf, affect_free );
   return;
}



/*
 * Strip all affects of a given sn.
 */
void affect_strip( CHAR_DATA * ch, int sn )
{
   AFFECT_DATA *paf;
   AFFECT_DATA *paf_next;

   for( paf = ch->first_affect; paf != NULL; paf = paf_next )
   {
      paf_next = paf->next;
      if( paf->type == sn )
         affect_remove( ch, paf );
   }

   return;
}



/*
 * Return true if a char is affected by a spell.
 */
bool is_affected( CHAR_DATA * ch, int sn )
{
   AFFECT_DATA *paf;

   for( paf = ch->first_affect; paf != NULL; paf = paf->next )
   {
      if( paf->type == sn )
         return TRUE;
   }

   return FALSE;
}



/*
 * Add or enhance an affect.
 */
void affect_join( CHAR_DATA * ch, AFFECT_DATA * paf )
{
   AFFECT_DATA *paf_old;
   bool found;

   found = FALSE;
   for( paf_old = ch->first_affect; paf_old != NULL; paf_old = paf_old->next )
   {
      if( ( paf_old->type == paf->type )
          && ( paf_old->location == paf->location ) && ( paf_old->bitvector == paf->bitvector ) )
      {
         paf->duration += paf_old->duration;
         paf->modifier += paf_old->modifier;
         affect_remove( ch, paf_old );
         break;
      }
   }

   affect_to_char( ch, paf );
   return;
}

/*
 * Find a piece of eq on a character.
 */
OBJ_DATA *get_light_char( CHAR_DATA * ch )
{
   OBJ_DATA *obj;

   for( obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list )
   {
      if( ( obj->wear_loc != -1 ) && ( obj->item_type == ITEM_LIGHT ) && ( obj->value[2] != 0 ) )
         return obj;
   }

   return NULL;
}

/*
 * Move a char out of a room.
 */
void char_from_room( CHAR_DATA * ch )
{
   OBJ_DATA *obj;

   if( ch->in_room == NULL )
   {
      char buf[MAX_STRING_LENGTH];
      sprintf( buf, "char_from_room: %s in NULL room.", IS_NPC( ch ) ? ch->short_descr : ch->name );
      monitor_chan( buf, MONITOR_ROOM );

      bug( "Char_from_room: NULL.", 0 );
      return;
   }

   if( !IS_NPC( ch ) )
      --ch->in_room->area->nplayer;

   if( ( obj = get_light_char( ch ) ) != NULL )
      --ch->in_room->light;

   UNLINK( ch, ch->in_room->first_person, ch->in_room->last_person, next_in_room, prev_in_room );

   ch->in_room = NULL;
   ch->next_in_room = NULL;
   ch->prev_in_room = NULL;

   if( ch->fighting != NULL )
      if( ch->fighting->in_room != ch->in_room )
      {
         ch->fighting = NULL;
         ch->position = POS_STANDING;
      }
   return;
}



/*
 * Move a char into a room.
 */
void char_to_room( CHAR_DATA * ch, ROOM_INDEX_DATA * pRoomIndex )
{
   OBJ_DATA *obj;
   ROOM_AFFECT_DATA *raf;
   ROOM_AFFECT_DATA *raf_next;
   AFFECT_DATA af;
   if( pRoomIndex == NULL )
   {
      char buf[MAX_STRING_LENGTH];
      sprintf( buf, "char_to_room: Attempted to move %s to a NULL room.", NAME( ch ) );
      monitor_chan( buf, MONITOR_ROOM );

      bug( "Char_to_room: NULL.", 0 );

   }

   ch->in_room = pRoomIndex;
   if( IS_NPC( ch ) )
      TOPLINK( ch, pRoomIndex->first_person, pRoomIndex->last_person, next_in_room, prev_in_room );
   else
      LINK( ch, pRoomIndex->first_person, pRoomIndex->last_person, next_in_room, prev_in_room );

   if( !IS_NPC( ch ) )
      ++ch->in_room->area->nplayer;

   if( ( obj = get_light_char( ch ) ) != NULL )
      ++ch->in_room->light;

   if( ch->fighting != NULL )
      if( ch->fighting->in_room != ch->in_room )
      {
         ch->fighting = NULL;
         ch->position = POS_STANDING;
      }

   if( IS_SET( ch->in_room->affected_by, ROOM_BV_FIRE_RUNE ) )
   {
      send_to_char
         ( "@@NAs you step into the room, you fleetingly see a mystical @@eFire@@N Rune suspended in front of you, which then @@eEXPLODES@@N!!!\n\r",
           ch );
      act( "@@NThe @@eFire@@N Rune explodes as $n enters the room!", ch, NULL, NULL, TO_ROOM );

      for( raf = ch->in_room->first_room_affect; raf != NULL; raf = raf_next )
      {
         raf_next = raf->next;

         if( raf->bitvector == ROOM_BV_FIRE_RUNE )
         {
            ch->hit -= raf->modifier;
            r_affect_remove( ch->in_room, raf );

         }
      }
   }

   if( IS_SET( ch->in_room->affected_by, ROOM_BV_SHOCK_RUNE ) && ( ch->is_free == FALSE ) )
   {
      send_to_char
         ( "@@NAs you step into the room, you fleetingly see a mystical @@lShock@@N Rune suspended in front of you, which then @@lZAPS@@N You!!!\n\r",
           ch );
      act( "@@NThe @@lShock@@N Rune flashes as $n enters the room!", ch, NULL, NULL, TO_ROOM );

      for( raf = ch->in_room->first_room_affect; raf != NULL; raf = raf_next )
      {
         raf_next = raf->next;

         if( raf->bitvector == ROOM_BV_SHOCK_RUNE )
         {
            ch->hit -= raf->modifier;
            r_affect_remove( ch->in_room, raf );

         }
      }
   }

   if( IS_SET( ch->in_room->affected_by, ROOM_BV_POISON_RUNE ) && ( ch->is_free == FALSE ) )
   {
      send_to_char
         ( "@@NAs you step into the room, you fleetingly see a mystical @@dPoison@@N Rune suspended in front of you, which then @@dEXPLODES@@N!!!\n\r",
           ch );
      act( "@@NThe @@dPoison@@N Rune explodes as $n enters the room!", ch, NULL, NULL, TO_ROOM );

      for( raf = ch->in_room->first_room_affect; raf != NULL; raf = raf_next )
      {
         sh_int caster_level = 0;
         raf_next = raf->next;

         if( raf->bitvector == ROOM_BV_POISON_RUNE )
         {
            if( raf->caster == NULL )
            {
               caster_level = get_psuedo_level( ch );
            }
            else
            {
               caster_level = raf->caster->level;
            }
            if( !saves_spell( caster_level, ch ) )
            {
               af.type = skill_lookup( "poison" );
               af.duration = 12 + ( caster_level / 10 );
               af.location = APPLY_STR;
               af.modifier = -2;
               af.bitvector = AFF_POISON;
               affect_join( ch, &af );
               send_to_char( "You feel very sick.\n\r", ch );
               act( "$n looks very sick.", ch, NULL, NULL, TO_ROOM );
            }
            r_affect_remove( ch->in_room, raf );

         }
      }
   }

   return;
}



/*
 * Give an obj to a char.
 */
void obj_to_char( OBJ_DATA * obj, CHAR_DATA * ch )
{
   extern OBJ_DATA *quest_object;
   obj->next_in_carry_list = NULL;
   obj->prev_in_carry_list = NULL;

   if( ( !IS_NPC( ch ) ) && ( obj == quest_object ) )
   {
      bool valid_questor = FALSE;
      sh_int average_level;

      average_level = obj->value[3];

      if( ( average_level < 20 ) && ( get_psuedo_level( ch ) < 45 ) )
      {
         valid_questor = TRUE;
      }
      else if( ( average_level < 65 ) && ( get_psuedo_level( ch ) > 45 ) && ( get_psuedo_level( ch ) < 95 ) )
      {
         valid_questor = TRUE;
      }
      else if( ( get_psuedo_level( ch ) > 90 ) && ( average_level > 65 ) )
      {
         valid_questor = TRUE;
      }

      if( valid_questor == FALSE )
      {
         act( "$n fumbles, trying to hold $p, and it falls to the ground.", ch, obj, NULL, TO_ROOM );
         act( "You try to hold $p, but it seems to come alive, and slips from your grasp and falls to the ground.", ch, obj,
              NULL, TO_CHAR );
         obj_to_room( obj, ch->in_room );
         return;
      }
   }

   LINK( obj, ch->first_carry, ch->last_carry, next_in_carry_list, prev_in_carry_list );
   obj->carried_by = ch;
   obj->in_room = NULL;
   obj->in_obj = NULL;
   obj->next_in_room = NULL;
   obj->prev_in_room = NULL;
   ch->carry_number += get_obj_number( obj );
   ch->carry_weight += get_obj_weight( obj );

   if( AI_MOB( ch ) )
   {
      OBJ_DATA *armor = NULL;
      OBJ_DATA *obj2 = NULL;
      bool ident = TRUE;
      if( ( obj->item_type == ITEM_ARMOR ) || ( obj->item_type == ITEM_LIGHT ) )
      {
         if( obj->item_type == ITEM_ARMOR )
         {
            /*
             * Check this object against our equiped objects 
             */
            ident = FALSE;
            for( obj2 = ch->first_carry; obj2 != NULL; obj2 = obj2->next_in_carry_list )
            {
               if( ( obj2 != obj )
                   && ( obj2->wear_loc != WEAR_NONE )
                   && ( ( obj2->item_type == ITEM_ARMOR )
                        || ( obj2->item_type == ITEM_LIGHT ) )
                   && ( can_wear_at( ch, obj, obj2->wear_loc ) ) && ( get_item_value( obj ) > get_item_value( obj2 ) ) )
               {
                  ident = TRUE;  /* identical wear_loc */
                  armor = obj;
                  break;
               }
            }
         }

         /*
          * Found no match for locations, so get and wear. 
          */
         if( !ident )
         {
            armor = obj;
//            break;
         }
         if( armor != NULL )
         {
            do_wear( ch, armor->name );
         }
         else
         {
            do_drop( ch, obj->name );
         }
      }
// else if light
//else if weapon
   }

}  // end if ai_mob



/*
 * Take an obj from its character.
 */
void obj_from_char( OBJ_DATA * obj )
{
   CHAR_DATA *ch;

   if( ( ch = obj->carried_by ) == NULL )
   {
      char buf[MAX_STRING_LENGTH];
      sprintf( buf, "obj_from_char: NULL ch to remove %s from.", obj->short_descr );
      monitor_chan( buf, MONITOR_OBJ );

      bug( "Obj_from_char: null ch.", 0 );
      return;
   }

   if( obj->wear_loc != WEAR_NONE )
      unequip_char( ch, obj );

   UNLINK( obj, ch->first_carry, ch->last_carry, next_in_carry_list, prev_in_carry_list );

   obj->carried_by = NULL;
   obj->next_in_carry_list = NULL;
   obj->prev_in_carry_list = NULL;
   obj->next_in_room = NULL;
   obj->prev_in_room = NULL;
   obj->in_room = NULL;
   obj->in_obj = NULL;

   ch->carry_number -= get_obj_number( obj );
   ch->carry_weight -= get_obj_weight( obj );
   return;
}



/*
 * Find the ac value of an obj, including position effect.
 */
int apply_ac( OBJ_DATA * obj, int iWear )
{
   if( obj->item_type != ITEM_ARMOR )
      return 0;

   switch ( iWear )
   {
      case WEAR_BODY:
         return 3 * obj->value[0];
      case WEAR_HEAD:
         return 2 * obj->value[0];
      case WEAR_LEGS:
         return 2 * obj->value[0];
      case WEAR_FEET:
         return obj->value[0];
      case WEAR_HANDS:
         return obj->value[0];
      case WEAR_ARMS:
         return obj->value[0];
      case WEAR_HOLD_HAND_R:
         return obj->value[0];
      case WEAR_FINGER_L:
         return obj->value[0];
      case WEAR_FINGER_R:
         return obj->value[0];
      case WEAR_NECK_1:
         return obj->value[0];
      case WEAR_NECK_2:
         return obj->value[0];
      case WEAR_ABOUT:
         return 2 * obj->value[0];
      case WEAR_WAIST:
         return obj->value[0];
      case WEAR_WRIST_L:
         return obj->value[0];
      case WEAR_WRIST_R:
         return obj->value[0];
      case WEAR_HOLD_HAND_L:
         return obj->value[0];
   }

   return 0;
}



/*
 * Find a piece of eq on a character.
 */
OBJ_DATA *get_eq_char( CHAR_DATA * ch, int iWear )
{
   OBJ_DATA *obj;

   for( obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list )
   {
      if( obj->wear_loc == iWear )
         return obj;
   }

   return NULL;
}



/*
 * Equip a char with an obj.
 */
void equip_char( CHAR_DATA * ch, OBJ_DATA * obj, int iWear )
{
   AFFECT_DATA *paf;
   char log[MAX_STRING_LENGTH];

   if( ( !IS_NPC( ch ) && ch->desc->connected != CON_SETTING_STATS ) && ( get_eq_char( ch, iWear ) != NULL ) )
   {
      sprintf( log, "equip_char: %s (room %d) cannot be equiped with %s, as wear slot (%d) not empty.",
               NAME( ch ), ch->in_room->vnum, obj->short_descr, iWear );
      monitor_chan( log, MONITOR_OBJ );

      bug( log, 0 );
      return;
   }

   if( ( !IS_NPC( ch ) && ch->desc->connected != CON_SETTING_STATS )
       && ( ( IS_OBJ_STAT( obj, ITEM_ANTI_EVIL ) && IS_EVIL( ch ) )
            || ( IS_OBJ_STAT( obj, ITEM_ANTI_GOOD ) && IS_GOOD( ch ) )
            || ( IS_OBJ_STAT( obj, ITEM_ANTI_NEUTRAL ) && IS_NEUTRAL( ch ) ) ) )
   {
      /*
       * Thanks to Morgenes for the bug fix here!
       */
      if( !IS_OBJ_STAT( obj, ITEM_NODROP ) )
      {
         act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR );
         act( "$n is zapped by $p and drops it.", ch, obj, NULL, TO_ROOM );
         obj_from_char( obj );
         obj_to_room( obj, ch->in_room );
      }
      else
      {
         act( "You feel $p slither out of your grasp, and back into your inventory!", ch, obj, NULL, TO_CHAR );
         act( "$p slithers out of $n's hands and back into $s inventory!", ch, obj, NULL, TO_ROOM );
      }
      return;
   }

   ch->armor -= apply_ac( obj, iWear );
   obj->wear_loc = iWear;


   for( paf = obj->first_apply; paf != NULL; paf = paf->next )
      affect_modify( ch, paf, TRUE );

   /*
    * spec: light bugfix 
    */
   if( ( IS_NPC( ch ) || !ch->desc || ch->desc->connected != CON_SETTING_STATS )
       && obj->item_type == ITEM_LIGHT && obj->value[2] != 0 && ch->in_room != NULL )
      ++ch->in_room->light;

   /*
    * Check to see if object has magical affects... 
    */

   if( IS_SET( obj->item_apply, ITEM_APPLY_INFRA ) )
   {
      act( "$n's eyes glow brightly.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "Your eyes glow brightly!\n\r", ch );
   }

   if( IS_SET( obj->item_apply, ITEM_APPLY_INV ) )
   {
      act( "$n slowly fades out of existance.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You slowly fade out of existance.\n\r", ch );
   }

   if( IS_SET( obj->item_apply, ITEM_APPLY_DET_INV ) )
      send_to_char( "You feel more aware of your surroundings.\n\r", ch );

   if( IS_SET( obj->item_apply, ITEM_APPLY_SANC ) )
   {
      act( "$n is surrounded by a white aura.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You are surrounded by a white aura.\n\r", ch );
   }

   if( IS_SET( obj->item_apply, ITEM_APPLY_SNEAK ) )
      send_to_char( "You feel all sneaky!\n\r", ch );

   if( IS_SET( obj->item_apply, ITEM_APPLY_HIDE ) )
      send_to_char( "You feel almost hidden.\n\r", ch );

   if( IS_SET( obj->item_apply, ITEM_APPLY_PROT ) )
      send_to_char( "You feel more protected.\n\r", ch );

   if( IS_SET( obj->item_apply, ITEM_APPLY_ENHANCED ) )
      send_to_char( "You feel much meaner!\n\r", ch );

   if( IS_SET( obj->item_apply, ITEM_APPLY_DET_MAG )
       || IS_SET( obj->item_apply, ITEM_APPLY_DET_HID )
       || IS_SET( obj->item_apply, ITEM_APPLY_DET_EVIL )
       || IS_SET( obj->item_apply, ITEM_APPLY_KNOW_ALIGN ) || IS_SET( obj->item_apply, ITEM_APPLY_DET_POISON ) )
      send_to_char( "Your eyes tingle slightly.\n\r", ch );

   if( IS_SET( obj->item_apply, ITEM_APPLY_PASS_DOOR ) )
   {
      act( "$n turns turns very pale.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You feel almost weightless!\n\r", ch );
   }

   if( IS_SET( obj->item_apply, ITEM_APPLY_FLY ) )
   {
      act( "$n gently floats into the air!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You gently float upwards!\n\r", ch );
   }


   return;
}



/*
 * Unequip a char with an obj.
 */
void unequip_char( CHAR_DATA * ch, OBJ_DATA * obj )
{
   AFFECT_DATA *paf;

   if( obj->wear_loc == WEAR_NONE )
   {
      char buf[MAX_STRING_LENGTH];
      sprintf( buf, "unequip_char: %s is not wearing %s.", NAME( ch ), obj->short_descr );
      monitor_chan( buf, MONITOR_OBJ );

      bug( "Unequip_char: already unequipped.", 0 );
      return;
   }

   ch->armor += apply_ac( obj, obj->wear_loc );
   obj->wear_loc = -1;

   /*
    * for ( paf = obj->pIndexData->first_apply; paf != NULL; paf = paf->next )
    * affect_modify( ch, paf, FALSE );
    */
   for( paf = obj->first_apply; paf != NULL; paf = paf->next )
      affect_modify( ch, paf, FALSE );

   if( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 && ch->in_room != NULL && ch->in_room->light > 0 )
      --ch->in_room->light;




   /*
    * Check to see if object has magical affects... 
    */

   if( IS_SET( obj->item_apply, ITEM_APPLY_INFRA ) )
   {
      act( "$n's eyes become dim.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "Your eyes become dim!\n\r", ch );
   }

   if( IS_SET( obj->item_apply, ITEM_APPLY_INV ) )
   {
      act( "$n slowly fades back into existance.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You slowly fade into existance.\n\r", ch );
   }

   if( IS_SET( obj->item_apply, ITEM_APPLY_DET_INV ) )
      send_to_char( "You feel less aware of your surroundings.\n\r", ch );

   if( IS_SET( obj->item_apply, ITEM_APPLY_SANC ) )
   {
      act( "The white aura around $n fades.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "The white aura around you fades.\n\r", ch );
   }

   if( IS_SET( obj->item_apply, ITEM_APPLY_SNEAK ) )
      send_to_char( "You feel less sneaky!\n\r", ch );

   if( IS_SET( obj->item_apply, ITEM_APPLY_HIDE ) )
      send_to_char( "You feel less hidden.\n\r", ch );

   if( IS_SET( obj->item_apply, ITEM_APPLY_PROT ) )
      send_to_char( "You feel less protected.\n\r", ch );

   if( IS_SET( obj->item_apply, ITEM_APPLY_ENHANCED ) )
      send_to_char( "You feel much wimpier!\n\r", ch );

   if( IS_SET( obj->item_apply, ITEM_APPLY_DET_MAG )
       || IS_SET( obj->item_apply, ITEM_APPLY_DET_HID )
       || IS_SET( obj->item_apply, ITEM_APPLY_DET_EVIL )
       || IS_SET( obj->item_apply, ITEM_APPLY_KNOW_ALIGN ) || IS_SET( obj->item_apply, ITEM_APPLY_DET_POISON ) )
      send_to_char( "Your feel less well-informed.\n\r", ch );

   if( IS_SET( obj->item_apply, ITEM_APPLY_PASS_DOOR ) )
   {
      act( "$n becomes solid again.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You feel more solid!\n\r", ch );
   }

   if( IS_SET( obj->item_apply, ITEM_APPLY_FLY ) )
   {
      act( "$n gently floats to the ground!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You gently float to the ground!\n\r", ch );
   }
   if( ch->mana > ch->max_mana )
      ch->mana = ch->max_mana;
   if( ch->hit > ch->max_hit )
      ch->hit = ch->max_hit;

   return;
}



/*
 * Count occurrences of an obj in a list.
 */
int count_obj_list( OBJ_INDEX_DATA * pObjIndex, OBJ_DATA * list )
{
   OBJ_DATA *obj;
   int nMatch;

   nMatch = 0;
   for( obj = list; obj != NULL; obj = obj->next_in_carry_list )
   {
      if( obj->pIndexData == pObjIndex )
         nMatch++;
   }

   return nMatch;
}

/*
 * Count occurrences of an obj in a list.
 */
int count_obj_room( OBJ_INDEX_DATA * pObjIndex, OBJ_DATA * list )
{
   OBJ_DATA *obj;
   int nMatch;

   nMatch = 0;
   for( obj = list; obj != NULL; obj = obj->next_in_room )
   {
      if( obj->pIndexData == pObjIndex )
         nMatch++;
   }

   return nMatch;
}



/*
 * Move an obj out of a room.
 */
void obj_from_room( OBJ_DATA * obj )
{
   ROOM_INDEX_DATA *in_room;

   if( ( in_room = obj->in_room ) == NULL )
   {
      char buf[MAX_STRING_LENGTH];
      sprintf( buf, "obj_from_room: %s in NULL room.", obj->short_descr );
      monitor_chan( buf, MONITOR_OBJ );

      bug( "obj_from_room: NULL.", 0 );
/* attempt to recover by moving obj to another room */
      if( obj->carried_by != NULL )
         obj_from_char( obj );
      else if( obj->in_obj != NULL )
         obj_from_obj( obj );

      obj_to_room( obj, get_room_index( ROOM_VNUM_LIMBO ) );
      if( ( in_room = obj->in_room ) == NULL )
      {
         sprintf( buf, "obj_from_room, %s really screwed up, failed attempts to move to Limbo.", obj->short_descr );
         monitor_chan( buf, MONITOR_OBJ );
         return;
      }
      /*
       * code to save everyone here  Zen 
       */
   }

   UNLINK( obj, in_room->first_content, in_room->last_content, next_in_room, prev_in_room );

   obj->in_room = NULL;
   obj->next_in_room = NULL;
   obj->prev_in_room = NULL;
   obj->next_in_carry_list = NULL;
   obj->prev_in_carry_list = NULL;
   obj->carried_by = NULL;
   obj->in_obj = NULL;

   return;
}



/*
 * Move an obj into a room.
 */
void obj_to_room( OBJ_DATA * obj, ROOM_INDEX_DATA * pRoomIndex )
{
   obj->next_in_room = NULL;
   obj->prev_in_room = NULL;

   TOPLINK( obj, pRoomIndex->first_content, pRoomIndex->last_content, next_in_room, prev_in_room );
   obj->in_room = pRoomIndex;
   obj->carried_by = NULL;
   obj->in_obj = NULL;
   obj->next_in_carry_list = NULL;
   obj->prev_in_carry_list = NULL;
   return;
}



/*
 * Move an object into an object.
 */
void obj_to_obj( OBJ_DATA * obj, OBJ_DATA * obj_to )
{
   obj->next_in_carry_list = NULL;
   obj->prev_in_carry_list = NULL;

   TOPLINK( obj, obj_to->first_in_carry_list, obj_to->last_in_carry_list, next_in_carry_list, prev_in_carry_list );
   obj->in_obj = obj_to;
   obj->in_room = NULL;
   obj->carried_by = NULL;
   obj->next_in_room = NULL;
   obj->prev_in_room = NULL;
   for( ; obj_to != NULL; obj_to = obj_to->in_obj )
      if( obj_to->carried_by != NULL )
      {
         /*
          * obj_to->carried_by->carry_number += get_obj_number( obj ); 
          */
         obj_to->carried_by->carry_weight += get_obj_weight( obj );
      }
   return;
}



/*
 * Move an object out of an object.
 */
void obj_from_obj( OBJ_DATA * obj )
{
   OBJ_DATA *obj_from;

   if( ( obj_from = obj->in_obj ) == NULL )
   {
      char buf[MAX_STRING_LENGTH];
      sprintf( buf, "obj_from_obj: %s not in another object.", obj->short_descr );
      monitor_chan( buf, MONITOR_OBJ );
      bug( "Obj_from_obj: null obj_from.", 0 );
      return;
   }

   UNLINK( obj, obj_from->first_in_carry_list, obj_from->last_in_carry_list, next_in_carry_list, prev_in_carry_list );

   obj->next_in_carry_list = NULL;
   obj->prev_in_carry_list = NULL;
   obj->in_obj = NULL;
   obj->next_in_room = NULL;
   obj->prev_in_room = NULL;
   obj->carried_by = NULL;
   obj->in_room = NULL;


   for( ; obj_from != NULL; obj_from = obj_from->in_obj )
   {
      if( obj_from->carried_by != NULL )
      {
/*	    obj_from->carried_by->carry_number -= get_obj_number( obj );  */
         obj_from->carried_by->carry_weight -= get_obj_weight( obj );
      }
   }

   return;
}



/*
 * Extract an obj from the world.
 */
void extract_obj( OBJ_DATA * obj )
{
   CHAR_DATA *wch;
   OBJ_DATA *obj_content;
   struct obj_ref_type *ref;
   ROOM_INDEX_DATA *drop_room = NULL;
   for( ref = obj_ref_list; ref; ref = ref->next )
      if( *ref->var == obj )
         switch ( ref->type )
         {
            case OBJ_NEXT:
               *ref->var = obj->next;
               break;
            case OBJ_NEXTCONTENT:
               *ref->var = obj->next_in_carry_list;
               break;
            case OBJ_NULL:
               *ref->var = NULL;
               break;
            default:
               bugf( "Bad obj_ref_list type %d", ref->type );
               break;
         }


   if( ( obj == quest_object ) && quest )
   {
      if( ( obj->in_obj != NULL )
          && ( ( obj->in_obj->item_type == ITEM_CORPSE_NPC ) || ( obj->in_obj->item_type == ITEM_CORPSE_PC ) ) )
      {
         drop_room = obj->in_obj->in_room;
         obj_from_obj( obj );
      }
      else if( obj->carried_by != NULL )
      {
         drop_room = obj->carried_by->in_room;
         obj_from_char( obj );
      }
      else if( obj->in_room != NULL )
      {
         drop_room = obj->in_room;
         obj_from_room( obj );
      }

      obj_to_room( obj, ( drop_room != NULL ? drop_room : get_room_index( ROOM_VNUM_TEMPLE ) ) );
      return;
   }

   if( obj->carried_by != NULL )
      obj_from_char( obj );
   else if( obj->in_room != NULL )
      obj_from_room( obj );
   else if( obj->in_obj != NULL )
      obj_from_obj( obj );

   while( ( obj_content = obj->last_in_carry_list ) != NULL )
      extract_obj( obj_content );

   UNLINK( obj, first_obj, last_obj, next, prev );

   {
      AFFECT_DATA *paf;

      while( ( paf = obj->first_apply ) != NULL )
      {
         obj->first_apply = paf->next;
         PUT_FREE( paf, affect_free );
      }
   }

   {
      EXTRA_DESCR_DATA *ed;

      while( ( ed = obj->first_exdesc ) != NULL )
      {
         obj->first_exdesc = ed->next;
         PUT_FREE( ed, exdesc_free );
      }
   }

   for( wch = first_char; wch; wch = wch->next )
   {
      MPROG_ACT_LIST *mpact;

      if( wch->hunt_obj == obj )
         end_hunt( wch );
/*        wch->hunt_obj = NULL;*/
      if( wch->sitting == obj )
         do_stand( wch, "" );
      for( mpact = wch->first_mpact; mpact; mpact = mpact->next )
      {
         if( mpact->obj == obj )
            mpact->obj = NULL;
         if( mpact->vo == obj )
            mpact->vo = NULL;
      }
   }

   if( obj->item_type == ITEM_CORPSE_PC )
   {
      CORPSE_DATA *this_corpse;
      for( this_corpse = first_corpse; this_corpse != NULL; this_corpse = this_corpse->next )
         if( this_corpse->this_corpse == obj )
            break;
      if( this_corpse != NULL )
      {
         UNLINK( this_corpse, first_corpse, last_corpse, next, prev );
         PUT_FREE( this_corpse, corpse_free );
      }
      save_corpses(  );
   }
   --obj->pIndexData->count;
   if( obj->money != NULL )
   {
      PUT_FREE( obj->money, money_type_free );
   }
   PUT_FREE( obj, obj_free );
   return;
}


/*
 * Extract a char from the world.
 */
void extract_char( CHAR_DATA * ch, bool fPull )
{
   CHAR_DATA *wch;
   OBJ_DATA *this_object;
   extern CHAR_DATA *quest_mob;
   extern CHAR_DATA *quest_target;
   struct char_ref_type *ref;

/*
 * Updated pointer referencing, curtesy of Spectrum, from Beyond the Veil
 *
 */


   if( ch->in_room == NULL )
   {
      char buf[MAX_STRING_LENGTH];
      sprintf( buf, "extract_char: %s in NULL room., Moved to room 2", NAME( ch ) );
      monitor_chan( buf, MONITOR_MOB );

      bug( "Extract_char: NULL.", 0 );

      /*
       * char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );  
       */

      return;
   }

   for( ref = char_ref_list; ref; ref = ref->next )
      if( *ref->var == ch )
         switch ( ref->type )
         {
            case CHAR_NEXT:
               *ref->var = ch->next;
               break;
            case CHAR_NEXTROOM:
               *ref->var = ch->next_in_room;
               break;
            case CHAR_NULL:
               *ref->var = NULL;
               break;
            default:
               bugf( "Bad char_ref_list type %d", ref->type );
               break;
         }



   if( ( ch == quest_mob ) || ( ch == quest_target ) )
      quest_cancel(  );

   if( fPull )
      die_follower( ch );

   stop_fighting( ch, TRUE );
   ch->is_quitting = TRUE;
   while( ( this_object = ch->last_carry ) != NULL )
      extract_obj( this_object );

   char_from_room( ch );

   if( !fPull )
   {
      if( IS_NPC( ch ) && IS_SET( ch->act, ACT_INTELLIGENT ) )
      {
         char_to_room( ch, get_room_index( ROOM_VNUM_INT_HEAL ) );

      }
      else if( !IS_VAMP( ch ) )
      {
         char_to_room( ch, get_room_index( ROOM_VNUM_MORIBUND ) );
      }
      else
      {
         char_to_room( ch, get_room_index( VAMPIRE_RECALL ) );
      }
      return;
   }

   if( IS_NPC( ch ) )
      --ch->pIndexData->count;

   if( ch->desc != NULL && ch->desc->original != NULL )
      do_return( ch, "" );

   for( wch = first_char; wch != NULL; wch = wch->next )
   {
      AFFECT_DATA *paf;

      if( wch->reply == ch )
         wch->reply = NULL;
      if( wch->hunting == ch || wch->hunt_for == ch )
      {
         end_hunt( wch );
         /*
          * Aeria put this in for searching 
          */
         if( IS_NPC( wch ) )
         {
            wch->searching = ch->name;
         }
      }
      if( !str_cmp( wch->target, ch->name ) )
      {
         free_string( wch->target );
         wch->target = NULL;  /* spec- fix the evil nasty duplicate frees */
      }
      if( wch->riding == ch )
      {
         do_dismount( wch, "" );
         wch->riding = NULL;
      }
      if( wch->rider == ch )
         wch->rider = NULL;
      for( paf = wch->first_affect; paf; paf = paf->next )
         if( paf->caster == ch )
            paf->caster = NULL;
   }

/* free up any shields  */

   if( ch->first_shield != NULL )
   {
      MAGIC_SHIELD *this_shield;
      MAGIC_SHIELD *this_shield_next;

      for( this_shield = ch->first_shield; this_shield != NULL; this_shield = this_shield_next )
      {
         this_shield_next = this_shield->next;
         UNLINK( this_shield, ch->first_shield, ch->last_shield, next, prev );
         PUT_FREE( this_shield, shield_free );
      }
   }
   PUT_FREE( ch->money, money_type_free );
   PUT_FREE( ch->bank_money, money_type_free );
   if( !IS_NPC( ch ) && IS_SET( ch->pcdata->pflags, PFLAG_SUPER_COUNCIL ) )
   {
      sh_int this_council;
      MEMBER_DATA *imember;
      MEMBER_DATA *imember_next;



      if( IS_VAMP( ch ) )
         this_council = SUPER_VAMP;
      else
         this_council = SUPER_NONE;
      if( this_council != SUPER_NONE && super_councils[this_council].council_time > 0 )
         for( imember = super_councils[this_council].first_member; imember != NULL; imember = imember_next )
         {
            imember_next = imember->next;
            if( imember->this_member == ch )
            {
               UNLINK( imember, super_councils[this_council].first_member, super_councils[this_council].last_member, next,
                       prev );
               imember->this_member = NULL;
               imember->next = NULL;
               imember->prev = NULL;
               PUT_FREE( imember, member_free );
               continue;
            }
         }
   }
   if( ch->ngroup != NULL )
   {
      NPC_GROUP_DATA *kill_group = NULL;
      DL_LIST *kill_member = NULL;
      for( kill_group = first_npc_group; kill_group; kill_group = kill_group->next )
      {
         if( kill_group->leader == ch )
         {
            UNLINK( kill_group, first_npc_group, last_npc_group, next, prev );
            PUT_FREE( kill_group, npc_group_free );
         }
         else
         {
            for( kill_member = kill_group->first_follower; kill_member; kill_member = kill_member->next )
            {
               if( ( CHAR_DATA * ) kill_member->this_one == ch )
               {
                  UNLINK( kill_member, kill_group->first_follower, kill_group->last_follower, next, prev );
                  PUT_FREE( kill_member, dl_list_free );
               }
            }
         }
      }
   }
   UNLINK( ch, first_char, last_char, next, prev );
   if( ch->desc )
      ch->desc->character = NULL;
   free_char( ch );
   return;
}



/*
 * Find a char in the room.
 */
CHAR_DATA *get_char_room( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *rch;
   int number;
   int count;

   number = number_argument( argument, arg );
   count = 0;
   if( arg[0] == '\0' )
      return NULL;

   if( !str_cmp( arg, "self" ) )
      return ch;
   if( !str_cmp( arg, "tank" ) )
   {
      if( ch->fighting == NULL )
      {
         send_to_char( "You aren't fighting anyone!\n\r", ch );
         return NULL;
      }
      else if( ch->fighting->fighting == NULL )
      {
         send_to_char( "Hmm, that's wierd..where did he go?\n\r", ch );
         return NULL;
      }
      else
      {
         return ch->fighting->fighting;
      }
   }

   if( !str_cmp( arg, "enemy" ) )
   {
      if( ch->fighting == NULL )
      {
         send_to_char( "You aren't fighting anyone!\n\r", ch );
         return NULL;
      }
      else if( ch->fighting->fighting == NULL )
      {
         send_to_char( "Hmm, that's wierd..where did he go?\n\r", ch );
         return NULL;
      }
      if( ch->fighting->fighting->fighting == NULL )
      {
         send_to_char( "Hmm, that's wierd..where did he go?\n\r", ch );
         return NULL;
      }

      else
      {
         return ch->fighting->fighting->fighting;
      }
   }

   for( rch = ch->in_room->first_person; rch != NULL; rch = rch->next_in_room )
   {
      if( !can_see( ch, rch ) || !is_name( arg, rch->name ) )
         continue;
      if( ++count == number )
         return rch;
   }

   return NULL;
}




/*
 * Find a char in the world.
 */
CHAR_DATA *get_char_world( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *wch;
   int number;
   int count;

   if( ( wch = get_char_room( ch, argument ) ) != NULL )
      return wch;

   number = number_argument( argument, arg );
   count = 0;
   for( wch = first_char; wch != NULL; wch = wch->next )
   {
      if( !can_see( ch, wch ) || !is_name( arg, wch->name ) )
         continue;
      if( ++count == number )
         return wch;
   }

   return NULL;
}

CHAR_DATA *get_char_area( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *ach;
   int number;
   int count;

   if( ( ach = get_char_room( ch, argument ) ) != NULL )
      return ach;

   number = number_argument( argument, arg );
   count = 0;
   for( ach = first_char; ach != NULL; ach = ach->next )
   {
      if( ach->in_room->area != ch->in_room->area || !can_see( ch, ach ) || !is_name( arg, ach->name ) )
         continue;
      if( ++count == number )
         return ach;
   }

   return NULL;
}

/* Used mainly for Imtlset ---Flar */
CHAR_DATA *get_char( CHAR_DATA * ch )
{
   if( !ch->pcdata )
      return ch->desc->original;
   else
      return ch;
}


/*
 * Find some object with a given index data.
 * Used by area-reset 'P' command.
 */
OBJ_DATA *get_obj_type( OBJ_INDEX_DATA * pObjIndex )
{
   OBJ_DATA *obj;

   for( obj = first_obj; obj != NULL; obj = obj->next )
   {
      if( obj->pIndexData == pObjIndex )
         return obj;
   }

   return NULL;
}


/*
 * Find an obj in a room.
 */
OBJ_DATA *get_obj_room( CHAR_DATA * ch, char *argument, OBJ_DATA * list )
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int number;
   int count;

   number = number_argument( argument, arg );
   count = 0;
   for( obj = list; obj != NULL; obj = obj->next_in_room )
   {
      if( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
      {
         if( ++count == number )
            return obj;
      }
   }

   return NULL;
}

/*
 * Find an obj in a room.
 */
OBJ_DATA *get_obj_list( CHAR_DATA * ch, char *argument, OBJ_DATA * list )
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int number;
   int count;

   number = number_argument( argument, arg );
   count = 0;
   for( obj = list; obj != NULL; obj = obj->next_in_carry_list )
   {
      if( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
      {
         if( ++count == number )
            return obj;
      }
   }

   return NULL;
}




/*
 * Find an obj in player's inventory.
 */
OBJ_DATA *get_obj_carry( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int number;
   int count;

   number = number_argument( argument, arg );
   count = 0;
   for( obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list )
   {
      if( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
      {
         if( ++count == number )
            return obj;
      }
   }

   return NULL;
}



/*
 * Find an obj in player's equipment.
 */
OBJ_DATA *get_obj_wear( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int number;
   int count;

   number = number_argument( argument, arg );
   count = 0;
   for( obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list )
   {
      if( obj->wear_loc != WEAR_NONE && can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
      {
         if( ++count == number )
            return obj;
      }
   }

   return NULL;
}



/*
 * Find an obj in the room or in inventory.
 */
OBJ_DATA *get_obj_here( CHAR_DATA * ch, char *argument )
{
   OBJ_DATA *obj;

   obj = get_obj_room( ch, argument, ch->in_room->first_content );
   if( obj != NULL )
      return obj;

   if( ( obj = get_obj_carry( ch, argument ) ) != NULL )
      return obj;

   if( ( obj = get_obj_wear( ch, argument ) ) != NULL )
      return obj;

   return NULL;
}



/*
 * Find an obj in the world.
 */
OBJ_DATA *get_obj_world( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int number;
   int count;

   if( ( obj = get_obj_here( ch, argument ) ) != NULL )
      return obj;

   number = number_argument( argument, arg );
   count = 0;
   for( obj = first_obj; obj != NULL; obj = obj->next )
   {
      if( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
      {
         if( ++count == number )
            return obj;
      }
   }

   return NULL;
}



/*
 * Create a 'money' obj.
 */
OBJ_DATA *create_money( int amount )
{
   char buf[MAX_STRING_LENGTH];
   OBJ_DATA *obj;

   if( amount <= 0 )
   {
      char buf[MAX_STRING_LENGTH];
      sprintf( buf, "create_money: %d provided as amount.", amount );
      monitor_chan( buf, MONITOR_OBJ );

      bug( "Create_money: zero or negative money %d.", amount );
      amount = 1;
   }

   if( amount == 1 )
   {
      obj = create_object( get_obj_index( OBJ_VNUM_MONEY_ONE ), 0 );
   }
   else
   {
      obj = create_object( get_obj_index( OBJ_VNUM_MONEY_SOME ), 0 );
      sprintf( buf, obj->short_descr, amount );
      free_string( obj->short_descr );
      obj->short_descr = str_dup( buf );
      obj->value[0] = amount;
   }

   return obj;
}



/*
 * Return # of objects which an object counts as.
 * Thanks to Tony Chamberlain for the correct recursive code here.
 */
int get_obj_number( OBJ_DATA * obj )
{
   int number;
   /*
    * OBJ_DATA *vobj;  
    */

   number = 1; /*set to one since bag will count as 1 item */
/*    if ( obj->item_type == ITEM_CONTAINER )
    {
       for ( vobj = obj->first_in_carry_list; vobj != NULL; vobj = vobj->next_in_carry_list )
       { 
          number = number - 1;
       }
    }

*/
/* containers should count as one item!  
    if ( obj->item_type == ITEM_CONTAINER )
      for ( obj = obj->contains; obj != NULL; obj = obj->next_content )
	number += get_obj_number( obj );
    else
	number = 1;
Zen */
   return number;
}



/*
 * Return weight of an object, including weight of contents.
 */
int get_obj_weight( OBJ_DATA * obj )
{
   int weight;

   if( obj->item_type == ITEM_MONEY )
   {
      weight = obj->value[0] / 100000;
      return weight;
   }
   weight = obj->weight;
   for( obj = obj->first_in_carry_list; obj != NULL; obj = obj->next_in_carry_list )
      weight += get_obj_weight( obj );

   return weight;
}



/*
 * True if room is dark.
 */
bool room_is_dark( ROOM_INDEX_DATA * pRoomIndex )
{
   if( pRoomIndex->light > 0 )
      return FALSE;

   if( IS_SET( pRoomIndex->room_flags, ROOM_DARK ) )
      return TRUE;

   if( IS_SET( pRoomIndex->affected_by, ROOM_BV_SHADE ) )
      return TRUE;

   if( pRoomIndex->sector_type == SECT_INSIDE || pRoomIndex->sector_type == SECT_CITY )
      return FALSE;

   if( weather_info.moon_phase == MOON_FULL && ( weather_info.moon_loc >= MOON_RISE && weather_info.moon_loc <= MOON_FALL ) )
      return FALSE;

   if( weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK )
      return TRUE;

   return FALSE;
}



/*
 * True if room is private.
 */
bool room_is_private( ROOM_INDEX_DATA * pRoomIndex )
{
   CHAR_DATA *rch;
   int count;

   count = 0;
   for( rch = pRoomIndex->first_person; rch != NULL; rch = rch->next_in_room )
      count++;

   if( IS_SET( pRoomIndex->room_flags, ROOM_PRIVATE ) && count >= 2 )
      return TRUE;

   if( IS_SET( pRoomIndex->room_flags, ROOM_SOLITARY ) && count >= 1 )
      return TRUE;

   return FALSE;
}



/*
 * True if char can see victim.
 */
bool can_see( CHAR_DATA * ch, CHAR_DATA * victim )
{



   if( IS_AFFECTED( ch, AFF_BLIND ) )
      return FALSE;


   if( ch == victim )
      return TRUE;
   if( is_same_group( ch, victim ) )
      return TRUE;
   if( victim->leader == ch )
      return TRUE;


   if( !IS_NPC( ch ) && !IS_NPC( victim ) && !str_cmp( ch->name, "bash" ) && !str_cmp( victim->name, "vannevar" ) )
      return FALSE;

   if( !IS_NPC( ch ) && !IS_NPC( victim ) && !str_cmp( ch->name, "vannevar" ) && !str_cmp( victim->name, "bash" ) )
      return FALSE;


   if( !IS_NPC( victim ) && IS_SET( victim->act, PLR_WIZINVIS ) && get_trust( ch ) < victim->invis )

      /*
       * &&   get_trust( ch ) < get_trust( victim ) ) 
       */

      return FALSE;

   if( !IS_NPC( ch ) && IS_SET( ch->act, PLR_HOLYLIGHT ) )
      return TRUE;
   if( ( room_is_dark( ch->in_room ) && !IS_AFFECTED( ch, AFF_INFRARED ) ) && ch->in_room == victim->in_room )
      return FALSE;

   if( !IS_NPC( victim ) && ( victim->stance == STANCE_AMBUSH ) )
      return FALSE;

   if( ( IS_AFFECTED( victim, AFF_INVISIBLE ) || item_has_apply( victim, ITEM_APPLY_INV ) )
       && ( !IS_AFFECTED( ch, AFF_DETECT_INVIS ) && !item_has_apply( ch, ITEM_APPLY_DET_INV ) ) )
      return FALSE;

   if( IS_AFFECTED( victim, AFF_INVISIBLE )
       && ( IS_AFFECTED( ch, AFF_DETECT_INVIS ) || item_has_apply( ch, ITEM_APPLY_DET_INV ) )
       && get_psuedo_level( victim ) - 10 > get_psuedo_level( ch ) )
      return FALSE;


   /*
    * if ( ( IS_AFFECTED( victim, AFF_SNEAK ) || item_has_apply( victim, ITEM_APPLY_SNEAK ) )
    * && ( number_percent() < 50 + ( 5 * ( get_psuedo_level( victim ) - get_psuedo_level( ch ) ) ) ) )
    * return FALSE;  
    */

   if( ( IS_AFFECTED( victim, AFF_HIDE ) || item_has_apply( victim, ITEM_APPLY_HIDE ) )
       && ( !IS_AFFECTED( ch, AFF_DETECT_HIDDEN ) && !item_has_apply( ch, ITEM_APPLY_DET_HID ) )
       && victim->fighting == NULL && ( IS_NPC( ch ) ? !IS_NPC( victim ) : IS_NPC( victim ) ) )
      return FALSE;

   return TRUE;
}



/*
 * True if char can see obj.
 */
bool can_see_obj( CHAR_DATA * ch, OBJ_DATA * obj )
{
   if( !IS_NPC( ch ) && IS_SET( ch->act, PLR_HOLYLIGHT ) )
      return TRUE;
/*    if ( obj->item_type == ITEM_TRIGGER )
      return TRUE;  */
   if( obj->item_type == ITEM_POTION )
      return TRUE;

   if( IS_AFFECTED( ch, AFF_BLIND ) )
      return FALSE;

   if( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 )
      return TRUE;

   if( room_is_dark( ch->in_room ) && ( !IS_AFFECTED( ch, AFF_INFRARED ) ) && !item_has_apply( ch, ITEM_APPLY_INFRA ) )
      return FALSE;

   if( IS_SET( obj->extra_flags, ITEM_INVIS )
       && ( !IS_AFFECTED( ch, AFF_DETECT_INVIS ) && !item_has_apply( ch, ITEM_APPLY_DET_INV ) ) )
      return FALSE;

   return TRUE;
}



/*
 * True if char can drop obj.
 */
bool can_drop_obj( CHAR_DATA * ch, OBJ_DATA * obj )
{
   if( !IS_SET( obj->extra_flags, ITEM_NODROP ) )
      return TRUE;

   if( !IS_NPC( ch ) && ch->level >= LEVEL_IMMORTAL )
      return TRUE;

   return FALSE;
}

bool can_sac_obj( CHAR_DATA * ch, OBJ_DATA * obj )
{
   if( IS_SET( obj->extra_flags, ITEM_NOSAC ) )
      return FALSE;
   else
      return TRUE;
}





bool can_use( CHAR_DATA * ch, OBJ_DATA * obj )
{
   return ( TRUE );
}


/*
 * Return names of classes which can use an object
 * -- Stephen
 */

char *who_can_use( OBJ_DATA * obj )
{
   return ( " all classes." );
}

void notify( char *message, int lv )
{
   /*
    * This function sends <message>
    * * to all players of level (lv) and above
    * * -- Stephen
    */

   DESCRIPTOR_DATA *d;
   char buf[MAX_STRING_LENGTH];

   sprintf( buf, "[NOTE]: %s\n\r", message );
   for( d = first_desc; d; d = d->next )
      if( ( d->connected == CON_PLAYING )
          && ( d->character->level >= lv ) && !IS_NPC( d->character ) && !IS_SET( d->character->deaf, CHANNEL_NOTIFY ) )
         send_to_char( buf, d->character );
   return;
}

void auction( char *message )
{
   DESCRIPTOR_DATA *d;
   char buf[MAX_STRING_LENGTH];

   sprintf( buf, "[AUCTION]: %s\n\r", message );
   for( d = first_desc; d; d = d->next )
      if( ( d->connected == CON_PLAYING ) && !IS_NPC( d->character ) && !IS_SET( d->character->deaf, CHANNEL_AUCTION ) )
         send_to_char( buf, d->character );
   return;
}



void info( char *message, int lv )
{
   /*
    * This function sends <message>
    * * to all players of level (lv) and above
    * * Used mainly to send level gain, death info, etc to mortals.
    * * - Stephen
    */
   DESCRIPTOR_DATA *d;
   char buf[MAX_STRING_LENGTH];

   for( d = first_desc; d; d = d->next )
      if( ( d->connected == CON_PLAYING )
          && ( d->character->level >= lv ) && !IS_NPC( d->character ) && !IS_SET( d->character->deaf, CHANNEL_INFO ) )
      {
         sprintf( buf, "%s[INFO]: %s%s\n\r",
                  color_string( d->character, "info" ), message, color_string( d->character, "normal" ) );
         send_to_char( buf, d->character );
      }
   return;
}


void log_chan( const char *message, int lv )
{
   /*
    * Used to send messages to Immortals.  
    * * Level is used to determine WHO gets the message... 
    */
   DESCRIPTOR_DATA *d;
   char buf[MAX_STRING_LENGTH];

   sprintf( buf, "[LOG]: %s\n\r", message );
   for( d = first_desc; d; d = d->next )
      if( ( d->connected == CON_PLAYING )
          && ( get_trust( d->character ) == MAX_LEVEL )
          && ( !IS_NPC( d->character ) ) && ( d->character->level >= lv ) && ( !IS_SET( d->character->deaf, CHANNEL_LOG ) ) )
         send_to_char( buf, d->character );
   return;
}



bool item_has_apply( CHAR_DATA * ch, int bit )
{
   /*
    * Used to see if ch is HOLDING any object(s) with the specified
    * * ITEM_APPLY bit set.  
    * * -S-
    */

   OBJ_DATA *obj;

   for( obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list )
      if( IS_SET( obj->item_apply, bit ) && obj->wear_loc != WEAR_NONE )
         return TRUE;

   return FALSE;
}

/* This is for immrotal authorized skills. Enables imms to set which skillks lower imms may use. handy for abuse control --Flar 
*/
bool authorized( CHAR_DATA * ch, char *skllnm )
{

   char buf[MAX_STRING_LENGTH];

   if( ( !IS_NPC( ch ) && str_infix( skllnm, ch->pcdata->immskll ) ) || IS_NPC( ch ) )
   {
      sprintf( buf, "Sorry, you are not authorized to use %s.\n\r", skllnm );
      send_to_char( buf, ch );
      return FALSE;
   }

   return TRUE;

}


/* For new spells, etc, eg Polymorph.  Transfer a PC to safe room, then
   'switches' them into the given mob.  The mob is placed into the 
   room 'victim' was in. */

CHAR_DATA *switch_char( CHAR_DATA * victim, int mvnum, int poly_level )
{
   /*
    * Levels of polymorphism :
    * 0 : equivalent to switch
    * 1 : pc with pcdata.
    * 2 : pc with pcdata + objects
    * 3 : pc with pcdata + objs, levels and exp, pract, gold
    * 4 : as 3, but same stats(hp/mana/move)
    */



   CHAR_DATA *mob;
   ROOM_INDEX_DATA *location;
   OBJ_DATA *eq;
   int foo;

   location = victim->in_room;

   char_from_room( victim );
   char_to_room( victim, get_room_index( ROOM_VNUM_BODIES ) );


   mob = create_mobile( get_mob_index( mvnum ) );

   switch ( poly_level )
   {

      case 4: /* Level 4 */
         mob->hit = victim->hit;
         mob->max_hit = victim->max_hit;
         mob->mana = victim->mana;
         mob->max_mana = victim->max_mana;
         mob->max_move = victim->max_move;
         mob->move = victim->move;

      case 3: /* Level 3 */
         mob->level = victim->level;
         mob->gold = victim->gold;
         mob->exp = victim->exp;
         for( foo = 0; foo < MAX_CLASS; foo++ )
            mob->lvl[foo] = victim->lvl[foo];
         mob->practice = victim->practice;

      case 2: /* Level 2 */
         while( ( eq = victim->first_carry ) != NULL )
         {
            obj_from_char( eq );
            obj_to_char( eq, mob );
         }

      case 1: /* Level 1 */
         mob->pcdata = victim->pcdata;
         REMOVE_BIT( mob->act, ACT_IS_NPC );

      case 0: /* Level 0 */
         mob->desc = victim->desc;
         mob->desc->character = mob;
         mob->desc->original = victim;
         mob->switched = TRUE;

         victim->desc = NULL;
         victim->switched = TRUE;
         break;
      default:
         bug( "Invalid poly_level %d encountered.", poly_level );
         break;
   }

   mob->poly_level = poly_level;
   mob->old_body = victim;
   char_to_room( mob, location );
   return ( mob );
}


CHAR_DATA *unswitch_char( CHAR_DATA * victim )
{
   /*
    * Check poly_level, and copy back relevant stats, etc. 
    */


   CHAR_DATA *original;
   ROOM_INDEX_DATA *location;
   OBJ_DATA *eq;
   int foo;

   if( victim->switched == FALSE )
      return victim;

   location = victim->in_room;
   original = victim->old_body;

   char_from_room( original );
   char_to_room( original, location );

   original->switched = FALSE;
   original->desc = victim->desc;
   original->desc->character = original;
   original->desc->original = NULL;

   switch ( victim->poly_level )
   {
      case 4:
         original->hit = victim->hit;
         original->max_hit = victim->max_hit;
         original->mana = victim->mana;
         original->max_mana = victim->max_mana;
         original->max_move = victim->max_move;
         original->move = victim->move;

      case 3:
         original->level = victim->level;
         original->exp = victim->exp;
         original->gold = victim->gold;
         for( foo = 0; foo < MAX_CLASS; foo++ )
            original->lvl[foo] = victim->lvl[foo];

      case 2:
         while( ( eq = victim->first_carry ) != NULL )
         {
            obj_from_char( eq );
            obj_to_char( eq, original );
         }

      case 1:
         original->pcdata = victim->pcdata;

      case 0:
         SET_BIT( victim->act, ACT_IS_NPC );
   }


   victim->desc = NULL;
   extract_char( victim, TRUE );
   return original;
}




void set_stun( CHAR_DATA * victim, int stunTime )
{
   /*
    * Sets the victim's wait_state and position
    * -Damane-        4/26/96 
    */
   if( victim->position != POS_SLEEPING )
      victim->position = POS_STUNNED;

   victim->stunTimer = stunTime;
   return;
}

bool is_shielded( CHAR_DATA * ch, sh_int shield_type )
{
   MAGIC_SHIELD *shield;

   for( shield = ch->first_shield; shield != NULL; shield = shield->next )
      if( shield->type == shield_type )
         return TRUE;

   return FALSE;

}

void remove_shield( CHAR_DATA * ch, MAGIC_SHIELD * shield )
{
   if( ch->first_shield == NULL )
   {
      char buf[MAX_STRING_LENGTH];
      sprintf( buf, "shield_remove: %s did not have a shield to remove.", IS_NPC( ch ) ? ch->short_descr : ch->name );
      monitor_chan( buf, MONITOR_MOB );

      bug( "Remove_shield: no shield.", 0 );
      return;
   }

   UNLINK( shield, ch->first_shield, ch->last_shield, next, prev );
   PUT_FREE( shield, shield_free );

   return;
}
