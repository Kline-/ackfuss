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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include "tables.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif


/*
 * Local functions.
 */
void say_spell args( ( CHAR_DATA * ch, int sn ) );


/* Calculate mana cost */
int mana_cost( CHAR_DATA * ch, int sn )
{
   int best;
   int foo;
   int skill_lev;
   int cost, mincost;
   int class = 0;



   if( IS_NPC( ch ) )
   {
      best = get_psuedo_level( ch );
      for( foo = 0; foo < MAX_CLASS; foo++ )
         if( best >= skill_table[sn].skill_level[foo] )
         {
            class = foo;
         }

      if( ( skill_table[sn].flag1 == REMORT )
          && ( ( ( IS_SET( ch->act, ACT_PET ) ) || ( IS_AFFECTED( ch, AFF_CHARM ) ) ) && ( ch->rider == NULL ) ) )
         best = -1;

      if( skill_table[sn].flag1 == ADEPT )
         best = -1;

   }
   else
   {
      best = -1;
      for( foo = 0; foo < MAX_CLASS; foo++ )
         if( ch->lvl[foo] >= skill_table[sn].skill_level[foo] && ch->lvl[foo] > best )
         {
            best = ch->lvl[foo];
            class = foo;
         }
   }
   if( skill_table[sn].flag1 == ADEPT )
   {
      best = -1;
      if( ch->adept_level >= skill_table[sn].skill_level[0] )
         best = ch->adept_level * 4;
   }

   if( ( best == -1 ) && ( IS_NPC( ch ) ) )
   {
      if( ( IS_SET( ch->act, ACT_INTELLIGENT ) ) && ( sn == skill_lookup( "ethereal" ) ) )
      {
         return 150;
      }
      else
      {
         return 1000;
      }
   }
   else if( ( best == -1 ) && ( ( !IS_NPC( ch ) ) && ( !is_name( skill_table[sn].name, race_table[ch->race].skill1 ) ) ) )
      return ( 1000 );



   mincost = 1000;

   skill_lev = skill_table[sn].skill_level[class];

   if( skill_lev > best )
      cost = 1000;
   else
      cost = UMAX( skill_table[sn].min_mana, 100 / ( 2 + best - skill_lev ) );

   if( cost < mincost )
      mincost = cost;

   if( IS_VAMP( ch ) && ( skill_table[sn].flag2 == VAMP ) )
      mincost = skill_table[sn].min_mana;
   if( IS_NPC( ch ) && IS_SET( ch->act, ACT_INTELLIGENT ) )
      mincost = skill_table[sn].min_mana + ( 200 - ch->level );
   if( skill_table[sn].flag2 == WOLF )
   {
      if( IS_NPC( ch ) )
         return 5000;
      if( IS_WOLF( ch ) )
      {
         cost = UMAX( skill_table[sn].min_mana,
                      ( ( skill_table[sn].min_mana * skill_lev ) /
                        ( ( ch->pcdata->vamp_level == 0 ) ? 1 : ch->pcdata->vamp_level ) ) );
      }
   }
   if( IS_NPC( ch ) )
      mincost /= 2;
   if( !IS_NPC( ch ) && ( MAGIC_STANCE( ch ) ) )
      mincost = mincost - ( mincost * .3 );
   if( ( !IS_NPC( ch ) ) && ( is_name( skill_table[sn].name, race_table[ch->race].skill1 ) ) )
      mincost = 10;
   if( !IS_NPC( ch ) && ( skill_table[sn].flag2 == NORM ) )
   {
      if( IS_SET( race_table[ch->race].race_flags, RACE_MOD_NO_MAGIC ) )
         mincost *= 1.75;
      else if( IS_SET( race_table[ch->race].race_flags, RACE_MOD_WEAK_MAGIC ) )
         mincost *= 1.25;
      else if( IS_SET( race_table[ch->race].race_flags, RACE_MOD_STRONG_MAGIC ) )
         mincost *= .60;
   }
   return mincost;
}

/*
 * Lookup a skill by name.
 */
int skill_lookup( const char *name )
{
   int sn;

   for( sn = 0; sn < MAX_SKILL; sn++ )
   {
      if( skill_table[sn].name == NULL )
         break;
      if( LOWER( name[0] ) == LOWER( skill_table[sn].name[0] ) && !str_prefix( name, skill_table[sn].name ) )
         return sn;
   }

   return -1;
}



/*
 * Lookup a skill by slot number.
 * Used for object loading.
 */
int slot_lookup( int slot )
{
   extern bool fBootDb;
   int sn;

   if( slot <= 0 )
      return -1;

   for( sn = 0; sn < MAX_SKILL; sn++ )
   {
      if( slot == skill_table[sn].slot )
         return sn;
   }

   if( fBootDb )
   {
      bug( "Slot_lookup: bad slot %d.", slot );
      abort(  );
   }

   return -1;
}



/*
 * Utter mystical words for an sn.
 */
void say_spell( CHAR_DATA * ch, int sn )
{
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   char msg[MAX_STRING_LENGTH];
   char msg2[MAX_STRING_LENGTH];
   CHAR_DATA *rch;
   char *pName;
   int iSyl;
   int length;

   struct syl_type
   {
      char *old;
      char *new;
   };

   static const struct syl_type syl_table[] = {
      {" ", " "},
      {"ar", "abra"},
      {"au", "kada"},
      {"bless", "fido"},
      {"blind", "nose"},
      {"bur", "mosa"},
      {"cu", "judi"},
      {"de", "oculo"},
      {"en", "unso"},
      {"light", "dies"},
      {"lo", "hi"},
      {"mor", "zak"},
      {"move", "sido"},
      {"ness", "lacri"},
      {"ning", "illa"},
      {"per", "duda"},
      {"ra", "gru"},
      {"re", "candus"},
      {"son", "sabru"},
      {"tect", "infra"},
      {"tri", "cula"},
      {"ven", "nofo"},
      {"a", "a"}, {"b", "b"}, {"c", "q"}, {"d", "e"},
      {"e", "z"}, {"f", "y"}, {"g", "o"}, {"h", "p"},
      {"i", "u"}, {"j", "y"}, {"k", "t"}, {"l", "r"},
      {"m", "w"}, {"n", "i"}, {"o", "a"}, {"p", "s"},
      {"q", "d"}, {"r", "f"}, {"s", "g"}, {"t", "h"},
      {"u", "j"}, {"v", "z"}, {"w", "x"}, {"x", "n"},
      {"y", "l"}, {"z", "k"},
      {"", ""}
   };

   buf[0] = '\0';
   for( pName = skill_table[sn].name; *pName != '\0'; pName += length )
   {
      for( iSyl = 0; ( length = strlen( syl_table[iSyl].old ) ) != 0; iSyl++ )
      {
         if( !str_prefix( syl_table[iSyl].old, pName ) )
         {
            strcat( buf, syl_table[iSyl].new );
            break;
         }
      }

      if( length == 0 )
         length = 1;
   }
   /*
    * Give some other message, according to TAR type and if !NPC 
    */
   switch ( skill_table[sn].target )
   {
      case TAR_IGNORE:
         sprintf( msg, "$n closes $s eyes, and prays." );
         sprintf( msg2, "You close your eyes and pray.\n\r" );
         break;
      case TAR_CHAR_OFFENSIVE:
         sprintf( msg, "$n's eyes glow red for an instant!" );
         sprintf( msg2, "Your eyes glow red for an instant!\n\r" );
         break;
      case TAR_CHAR_DEFENSIVE:
         sprintf( msg, "$n falls silent as $e meditates." );
         sprintf( msg2, "You become silent as you meditate.\n\r" );
         break;
      case TAR_CHAR_SELF:
         sprintf( msg, "$n motions toward $mself." );
         sprintf( msg2, "You motion towards yourself.\n\r" );
         break;
      case TAR_OBJ_INV:
         sprintf( msg, "$n's hands briefly glow magically!" );
         sprintf( msg2, "Your hands briefly glow magically!\n\r" );
   }
   act( msg, ch, NULL, ch, TO_NOTVICT );
   send_to_char( msg2, ch );

   sprintf( buf2, "$n utters the words, '%s'.", buf );
   sprintf( buf, "$n utters the words, '%s'.", skill_table[sn].name );

   for( rch = ch->in_room->first_person; rch; rch = rch->next_in_room )
   {
      if( rch != ch )
         act( ch->class == rch->class ? buf : buf2, ch, NULL, rch, TO_VICT );
   }

   return;
}



/*
 * Compute a saving throw.
 * Negative apply's make saving throw better.
 */
bool saves_spell( int level, CHAR_DATA * victim )
{
   int save;
   bool saved = FALSE;
   save = 40 +
      ( ( get_psuedo_level( victim ) > 60 ?
          get_psuedo_level( victim ) * 2 / 3 :
          get_psuedo_level( victim ) ) - level - URANGE( -40, victim->saving_throw, 40 ) );
   save -= wis_app[get_curr_wis( victim )].spell_save;
   if( ( IS_NPC( victim ) ) && ( IS_SET( victim->act, ACT_SOLO ) ) )
      save += 20;
   if( !IS_NPC( victim ) && ( IS_SET( race_table[victim->race].race_flags, RACE_MOD_RESIST_SPELL ) ) )
      save += 20;
   save = URANGE( 5, save, 95 );

   if( number_percent(  ) < save )
      saved = TRUE;
#if 0
   sprintf( log_buf, "%s lvl %d wismod %d savemod %d save total %d against level %d, %s ).",
            victim->name, get_psuedo_level( victim ),
            wis_app[get_curr_wis( victim )].spell_save,
            victim->saving_throw, save, level, ( saved ? "@@aSAVED@@N" : "@@eFAILED@@N" ) );
   monitor_chan( log_buf, MONITOR_MAGIC );
#endif

   return ( saved );

}



/*
 * The kludgy global is for spells who want more stuff from command line.
 */
char *target_name;

void do_cast( CHAR_DATA * ch, char *argument )
{
   char *typed = argument;
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   void *vo;
   int mana;
   int sn;
   int best;   /* The best class to use for the job :P */
   int cnt;
   bool char_login = FALSE;
   int cast_chance = 0;
   bool multi_cast = FALSE;
   if( ch->is_free == TRUE )
      return;
/* ZENFIX --ch's are surviving multiple kills per combat round */

   if( ( !IS_NPC( ch ) ) && ( ch->desc != NULL ) && ( ch->desc->connected == CON_SETTING_STATS ) )
      char_login = TRUE;


   /*
    * Modified to reference ch's highest ch->lvl[] value which
    * * apllies to the spell.  -S-
    */


   /*
    * Only MOBprogrammed mobs not charmed can cast spells
    * like PC's - 
    * -S- Mod: intelligent mobs can cast too.    NOT ANYMORE :P  Zen
    */
   /*
    * if ( IS_NPC(ch)
    * && ( !IS_SET( ch->act, ACT_INTELLIGENT ) )
    * && ( !ch->pIndexData->progtypes
    * || IS_AFFECTED( ch, AFF_CHARM ) ) )
    * return;
    */

   target_name = one_argument( argument, arg1 );
   one_argument( target_name, arg2 );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Cast which what where?\n\r", ch );
      return;
   }

   best = -1;  /* Default 'no way' value */

   if( ( sn = skill_lookup( arg1 ) ) == -1 )
   {
      send_to_char( "Wiggle swiggle biggle?\n\r", ch );
      return;
   }

   /*
    * Check if in a no-magic room 
    */
   if( !char_login && IS_SET( ch->in_room->room_flags, ROOM_NO_MAGIC ) && ( skill_table[sn].flag2 != WOLF ) )
   {
      send_to_char( "Some strange force prevents you casting the spell!\n\r", ch );
      return;
   }


   /*
    * Compute best level to use for spell, IF it meets requiements 
    */
   if( IS_NPC( ch ) )
   {
      best = UMIN( 90, get_psuedo_level( ch ) );
      if( ( skill_table[sn].flag1 == REMORT )
          && ( ( ( IS_SET( ch->act, ACT_PET ) ) || ( IS_AFFECTED( ch, AFF_CHARM ) ) ) && ( ch->rider == NULL ) ) )
         best = -1;



      if( skill_table[sn].flag1 == ADEPT )
         best = -1;
      if( ( skill_table[sn].flag2 == VAMP ) || ( skill_table[sn].flag2 == WOLF ) )
         best = -1;

   }
   else
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      {
         if( ( ( ch->lvl[cnt] >= skill_table[sn].skill_level[cnt] ) && ( skill_table[sn].flag1 == MORTAL ) )
             && best < ch->lvl[cnt] )
            best = ch->lvl[cnt];
         if( ( ( ch->lvl2[cnt] >= skill_table[sn].skill_level[cnt] ) && ( skill_table[sn].flag1 == REMORT ) )
             && best < ch->lvl2[cnt] )
            best = ch->lvl[cnt];
      }
   if( !IS_NPC( ch ) )
      if( ( IS_VAMP( ch ) ) && ( skill_table[sn].flag2 == VAMP ) )
         best = ch->pcdata->vamp_level * 4;
   if( ( ch->adept_level > 0 ) && ( skill_table[sn].flag1 == ADEPT ) )
      best = ch->adept_level * 4;
   if( IS_NPC( ch ) && ( skill_table[sn].flag1 == ADEPT ) )
      best = -1;
   if( !IS_NPC( ch ) )
      if( skill_table[sn].flag2 == WOLF && IS_WOLF( ch ) )
         best = ch->pcdata->vamp_level * 4;


   if( best == 80 )
      best = 79;
   if( ( !IS_NPC( ch ) ) && ( is_name( skill_table[sn].name, race_table[ch->race].skill1 ) ) )
      best = 60;

   if( ( best == -1 )
       || ( ( skill_table[sn].flag2 == VAMP )
            && ( !IS_VAMP( ch ) ) ) || ( ( skill_table[sn].flag2 == WOLF ) && ( !IS_WOLF( ch ) ) ) )
   {
      send_to_char( "You can't do that.\n\r", ch );
      return;
   }

   if( ch->position < skill_table[sn].minimum_position )
   {
      send_to_char( "You can't concentrate enough.\n\r", ch );
      return;
   }
   if( IS_NPC( ch ) && skill_table[sn].flag2 == VAMP )
      return;
   if( IS_NPC( ch ) && skill_table[sn].flag2 == WOLF )
      return;

   mana = mana_cost( ch, sn );

   if( char_login )
      mana = 0;

   /*
    * Locate targets.
    */
   victim = NULL;
   obj = NULL;
   vo = NULL;

   switch ( skill_table[sn].target )
   {
      default:
         bug( "Do_cast: bad target for sn %d.", sn );
         return;

      case TAR_IGNORE:
         break;

      case TAR_CHAR_OFFENSIVE:
         if( arg2[0] == '\0' )
         {
            if( ( victim = ch->fighting ) == NULL )
            {
               send_to_char( "Cast the spell on whom?\n\r", ch );
               return;
            }
         }
         else
         {
            if( ( victim = get_char_room( ch, arg2 ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }
         }
         if( IS_AFFECTED( ch, AFF_CHARM ) && !IS_NPC( victim ) )
         {
            check_social( ch, "sulk", "" );
            return;
         }
         check_killer( ch, victim );

/*      if ( !IS_NPC(ch) )
	{
	    if ( !IS_NPC(victim) && ch != victim )
	    {
		send_to_char( "You can't do that on a player.\n\r", ch );
		return;
	    }

	    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	    {
		send_to_char( "You can't do that on your own follower.\n\r",
		    ch );
		return;
	    }
	}
*/
         vo = ( void * )victim;
         break;

      case TAR_CHAR_DEFENSIVE:
         if( arg2[0] == '\0' )
         {
            victim = ch;
         }
         else
         {
            if( ( victim = get_char_room( ch, arg2 ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }
         }

         vo = ( void * )victim;
         break;

      case TAR_CHAR_SELF:
         if( arg2[0] != '\0' && !is_name( arg2, ch->name ) )
         {
            send_to_char( "You cannot cast this spell on another.\n\r", ch );
            return;
         }

         vo = ( void * )ch;
         break;

      case TAR_OBJ_INV:
         if( arg2[0] == '\0' )
         {
            send_to_char( "What should the spell be cast upon?\n\r", ch );
            return;
         }

         if( ( obj = get_obj_carry( ch, arg2 ) ) == NULL )
         {
            send_to_char( "You are not carrying that.\n\r", ch );
            return;
         }

         vo = ( void * )obj;
         break;
   }
   if( ( skill_table[sn].flag2 == NORM ) && ( is_affected( ch, skill_lookup( "mystical focus" ) ) ) )
   {
      mana *= 2.5;
   }

   if( !IS_VAMP( ch ) && ( skill_table[sn].flag2 == VAMP ) )
   {
      send_to_char( "Huh?\n\r", ch );
      return;
   }
   if( IS_VAMP( ch ) && ( skill_table[sn].flag2 == VAMP ) )
   {
      if( !IS_NPC( ch ) && ch->pcdata->bloodlust < mana )
      {
         send_to_char( "@@NYou don't have enough @@eblood@@N to cast that spell!!!\n\r", ch );
         return;
      }
      else;
   }
   if( !IS_WOLF( ch ) && ( skill_table[sn].flag2 == WOLF ) )
   {
      send_to_char( "Huh?\n\r", ch );
      return;
   }
   if( skill_table[sn].flag2 == WOLF && ( !IS_NPC( ch ) && IS_WOLF( ch ) ) && ch->pcdata->bloodlust < mana )
   {
      send_to_char( "@@bYou aren't able to summon enough @@rRAGE@@N!!\n\r", ch );
      return;
   }
   else if( ch->mana < mana )
   {
      send_to_char( "You don't have enough mana.\n\r", ch );
      return;
   }

   if( str_cmp( skill_table[sn].name, "ventriloquate" ) )
      say_spell( ch, sn );

   WAIT_STATE( ch, skill_table[sn].beats );
   cast_chance = ( ( IS_NPC( ch ) ? ch->level : ch->pcdata->learned[sn] ) + ( int_app[get_curr_int( ch )].spell_mod ) );

   if( !IS_NPC( ch ) && ( skill_table[sn].flag2 == NORM ) )
   {
      if( IS_SET( race_table[ch->race].race_flags, RACE_MOD_NO_MAGIC ) )
         cast_chance += -20;
      else if( IS_SET( race_table[ch->race].race_flags, RACE_MOD_WEAK_MAGIC ) )
         cast_chance += -10;
      else if( IS_SET( race_table[ch->race].race_flags, RACE_MOD_STRONG_MAGIC ) )
         cast_chance += 15;
   }
   if( cast_chance < 10 )
      cast_chance += number_range( 3, 7 );

   if( !char_login && !IS_NPC( ch ) && number_percent(  ) > cast_chance )
   {
      bool good_cast = FALSE;
      if( MAGIC_STANCE( ch ) )
      {
         if( number_percent(  ) > cast_chance )
         {
            good_cast = FALSE;
         }
         else
         {
            good_cast = TRUE;
         }
      }
      if( ( !good_cast ) && ( !IS_NPC( ch ) ) && ( ch->stance == STANCE_MAGI ) )
      {
         if( number_percent(  ) > cast_chance )
         {
            good_cast = FALSE;
         }
         else
         {
            good_cast = TRUE;
         }
      }
      if( !good_cast )
      {
         send_to_char( "You lost your concentration.\n\r", ch );

         if( IS_VAMP( ch ) && ( skill_table[sn].flag2 == VAMP ) )
            ch->pcdata->bloodlust -= mana / 2;
         else if( skill_table[sn].flag2 == WOLF && ( !IS_NPC( ch ) && IS_WOLF( ch ) ) )
            ch->pcdata->bloodlust -= mana / 3;
         else
            ch->mana -= mana / 2;
         return;
      }
   }


   if( ( MAGIC_STANCE( ch ) )
       && ( skill_table[sn].target == TAR_CHAR_OFFENSIVE ) && ( number_range( 0, 99 ) < get_psuedo_level( ch ) - 50 ) )
   {
      mana = mana * 2 / 3;
      multi_cast = TRUE;
   }
   if( ( sn != skill_lookup( "cure light" ) )
       && ( sn != skill_lookup( "cure serious" ) )
       && ( sn != skill_lookup( "cure critical" ) ) && ( sn != skill_lookup( "heal" ) ) )
   {
      sprintf( log_buf, "%s typed %s, Spell %s, room %s(%d), target %s",
               ch->name, typed, skill_table[sn].name,
               ch->in_room->name, ch->in_room->vnum, ( victim != NULL ? victim->name : obj != NULL ? obj->name : "NONE" ) );
      monitor_chan( log_buf, MONITOR_MAGIC );
   }
   if( ( *skill_table[sn].spell_fun ) ( sn, best, ch, vo, NULL ) )
   {
      if( ( skill_table[sn].flag2 == VAMP ) || ( skill_table[sn].flag2 == WOLF ) )
      {
         ch->pcdata->bloodlust -= mana;
      }
      else
      {
         ch->mana -= mana; /* Only use mana if spell was called correctly */
      }
   }

   if( multi_cast )
   {
      bool still_here = TRUE;

      if( arg2[0] == '\0' )
      {
         if( ( ( victim = ch->fighting ) == NULL ) || ( victim->in_room == NULL ) || ( victim->is_free == TRUE ) )
         {
            ch->fighting = NULL;
            still_here = FALSE;
         }
      }
      else
      {
         if( ( victim = get_char_room( ch, arg2 ) ) == NULL )
         {
            still_here = FALSE;
         }
      }
      if( still_here )
      {
         vo = ( void * )victim;
         if( ( sn != skill_lookup( "cure light" ) )
             && ( sn != skill_lookup( "cure serious" ) )
             && ( sn != skill_lookup( "cure critical" ) ) && ( sn != skill_lookup( "heal" ) ) )
         {
            sprintf( log_buf, "%s typed %s, Spell %s, room %s(%d), target %s",
                     ch->name, typed, skill_table[sn].name,
                     ch->in_room->name, ch->in_room->vnum,
                     ( victim != NULL ? victim->name : obj != NULL ? obj->name : "NONE" ) );
            monitor_chan( log_buf, MONITOR_MAGIC );
         }
         if( ( *skill_table[sn].spell_fun ) ( sn, best, ch, vo, NULL ) )
         {
            if( ( skill_table[sn].flag2 == VAMP ) || ( skill_table[sn].flag2 == WOLF ) )
            {
               ch->pcdata->bloodlust -= mana;
            }
            else
            {
               ch->mana -= mana; /* Only use mana if spell was called correctly */
            }
         }
      }
   }
   if( ( ch->stance == STANCE_MAGI )
       && ( skill_table[sn].target == TAR_CHAR_OFFENSIVE ) && ( number_range( 0, 99 ) < get_psuedo_level( ch ) - 80 ) )
   {
      bool still_here = TRUE;
      mana = mana * 1 / 2;

      if( arg2[0] == '\0' )
      {
         if( ( ( victim = ch->fighting ) == NULL ) || ( victim->in_room == NULL ) || ( victim->is_free == TRUE ) )
         {
            ch->fighting = NULL;
            still_here = FALSE;
         }
      }
      else
      {
         if( ( victim = get_char_room( ch, arg2 ) ) == NULL )
         {
            still_here = FALSE;
         }
      }
      if( still_here )
      {
         vo = ( void * )victim;
         if( ( sn != skill_lookup( "cure light" ) )
             && ( sn != skill_lookup( "cure serious" ) )
             && ( sn != skill_lookup( "cure critical" ) ) && ( sn != skill_lookup( "heal" ) ) )
         {
            sprintf( log_buf, "%s typed %s, Spell %s, room %s(%d), target %s",
                     ch->name, typed, skill_table[sn].name,
                     ch->in_room->name, ch->in_room->vnum,
                     ( victim != NULL ? victim->name : obj != NULL ? obj->name : "NONE" ) );
            monitor_chan( log_buf, MONITOR_MAGIC );
         }
         if( ( *skill_table[sn].spell_fun ) ( sn, best, ch, vo, NULL ) )
         {
            if( ( skill_table[sn].flag2 == VAMP ) || ( skill_table[sn].flag2 == WOLF ) )
            {
               ch->pcdata->bloodlust -= mana;
            }
            else
            {
               ch->mana -= mana; /* Only use mana if spell was called correctly */
            }
         }
      }
   }


   if( ( skill_table[sn].target == TAR_CHAR_OFFENSIVE )
       && ( ch != NULL )
       && ( victim != NULL ) && ( ch != victim ) && ( ch->is_free == FALSE ) && ( victim->is_free == FALSE ) )
   {
      CHAR_DATA *vch;
      CHAR_DATA *vch_next;
      CREF( vch_next, CHAR_NEXTROOM );
      for( vch = ch->in_room->first_person; vch; vch = vch_next )
      {
         vch_next = vch->next_in_room;
         if( ( vch == victim ) && ( vch->fighting == NULL ) && ( vch->master != ch ) && ( vch != ch ) )
         {
            check_killer( ch, vch );
            multi_hit( vch, ch, TYPE_UNDEFINED );  /* SRZ--fixed bad call--swap ch&v */
            break;
         }
      }
      CUREF( vch_next );
   }

   return;
}



/*
 * Cast spells at targets using a magical object.
 */
void obj_cast_spell( int sn, int level, CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * obj )
{
   void *vo;

   if( sn <= 0 )
      return;

   if( sn >= MAX_SKILL || skill_table[sn].spell_fun == 0 )
   {
      bug( "Obj_cast_spell: bad sn %d.", sn );
      return;
   }

   switch ( skill_table[sn].target )
   {
      default:
         bug( "Obj_cast_spell: bad target for sn %d.", sn );
         return;

      case TAR_IGNORE:
         vo = NULL;
         break;

      case TAR_CHAR_OFFENSIVE:
         if( victim == NULL )
            if( ch != NULL )
               victim = ch->fighting;
         if( victim == NULL )
         {
            if( ch != NULL )
               send_to_char( "You can't do that.\n\r", ch );
            return;
         }
         vo = ( void * )victim;
         break;

      case TAR_CHAR_DEFENSIVE:
         if( victim == NULL )
            if( ch != NULL )
               victim = ch;
         if( victim != NULL )
            vo = ( void * )victim;
         else
            return;
         break;

      case TAR_CHAR_SELF:
         if( ch != NULL )
            vo = ( void * )ch;
         else
            return;
         break;

      case TAR_OBJ_INV:
         if( obj == NULL )
         {
            if( ch != NULL )
               send_to_char( "You can't do that.\n\r", ch );
            return;
         }
         vo = ( void * )obj;
         break;
   }

   target_name = "";

   ( *skill_table[sn].spell_fun ) ( sn, level, ch, vo, obj );

   if( skill_table[sn].target == TAR_CHAR_OFFENSIVE && ( victim != NULL ) && victim->master != ch )
   {
      CHAR_DATA *vch;
      CHAR_DATA *vch_next;

      for( vch = ch->in_room->first_person; vch; vch = vch_next )
      {
         vch_next = vch->next_in_room;
         if( victim == vch && victim->fighting == NULL )
         {
            multi_hit( ch, victim, TYPE_UNDEFINED );  /* SRZ swapped ch& v */
            break;
         }
      }
   }

   return;
}



/* Spell functions. */

bool spell_acid_blast( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;

   dam = dice( level, 6 );
   if( saves_spell( level, victim ) )
      dam /= 2;
   sp_damage( obj, ch, victim, dam, REALM_ACID, sn, TRUE );
   return TRUE;
}



bool spell_armor( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, sn ) )
      return FALSE;
   af.type = sn;
   af.duration = 24;
   af.modifier = -20 - get_psuedo_level( ch ) / 10;
   af.location = APPLY_AC;
   af.bitvector = 0;
   affect_to_char( victim, &af );
   send_to_char( "You feel someone protecting you.\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}

bool spell_badbreath( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
/* --Stephen */
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   static const sh_int dam_each[] = {
      0,
      3, 3, 4, 4, 5, 6, 6, 6, 6, 6,
      7, 7, 7, 7, 7, 8, 8, 8, 8, 8,
      9, 9, 9, 9, 9, 10, 10, 10, 10, 10,
      11, 11, 11, 11, 11, 12, 12, 12, 12, 12,
      13, 13, 13, 13, 13, 14, 14, 14, 14, 14
   };
   int dam;

   level = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
   level = UMAX( 0, level );
   dam = number_range( dam_each[level] / 2, dam_each[level] * 2 ) + ( level / 4 );
   if( saves_spell( level, victim ) )
   {
      dam /= 2;
   }
   sp_damage( obj, ch, victim, dam, REALM_GAS, sn, TRUE );
   return TRUE;
}

bool spell_bark_skin( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( ch, sn ) || is_affected( ch, skill_lookup( "stone skin" ) ) )
      return FALSE;
   af.type = sn;
   af.duration = 3 + ( level / 12 );
   af.location = APPLY_AC;
   af.modifier = -10;
   af.bitvector = 0;
   affect_to_char( victim, &af );
   act( "$n's skin turns to bark.", victim, NULL, NULL, TO_ROOM );
   send_to_char( "Your skin turns to bark.\n\r", victim );
   return TRUE;
}

bool spell_bless( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( victim->position == POS_FIGHTING || is_affected( victim, sn ) )
      return ( ch == victim ? FALSE : TRUE );
   af.type = sn;
   af.duration = 6 + ( level / 6 );
   af.location = APPLY_HITROLL;
   af.modifier = level / 8;
   af.bitvector = 0;
   affect_to_char( victim, &af );

   af.location = APPLY_SAVING_SPELL;
   af.modifier = 0 - level / 8;
   affect_to_char( victim, &af );
   send_to_char( "You feel righteous.\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_blindness( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( !sp_damage( obj, ch, victim, 0, REALM_LIGHT, sn, FALSE ) )
      return TRUE;

   if( IS_AFFECTED( victim, AFF_BLIND ) || saves_spell( level, victim ) )
      return TRUE;

   af.type = sn;
   af.location = APPLY_HITROLL;
   af.modifier = -4;
   af.duration = 1 + ( level / 4 );
   af.bitvector = AFF_BLIND;
   affect_to_char( victim, &af );
   send_to_char( "You are blinded!\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_burning_hands( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   static const sh_int dam_each[] = {
      0,
      0, 0, 0, 0, 14, 17, 20, 23, 26, 29,
      29, 29, 30, 30, 31, 31, 32, 32, 33, 33,
      34, 34, 35, 35, 36, 36, 37, 37, 38, 38,
      39, 39, 40, 40, 41, 41, 42, 42, 43, 43,
      44, 44, 45, 45, 46, 46, 47, 47, 48, 48
   };
   int dam;

   level = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
   level = UMAX( 0, level );
   dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
   if( saves_spell( level, victim ) )
      dam /= 2;

   sp_damage( obj, ch, victim, dam, REALM_FIRE, sn, TRUE );
   return TRUE;
}



bool spell_call_lightning( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int dam;

   if( !IS_OUTSIDE( ch ) )
   {
      send_to_char( "You must be out of doors.\n\r", ch );
      return FALSE;
   }

   if( weather_info.sky < SKY_RAINING )
   {
      send_to_char( "You need bad weather.\n\r", ch );
      return FALSE;
   }

   dam = dice( level, 8 );


   if( obj == NULL )
   {
      act( "$n calls God's lightning to strike $s foes!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "God's lightning strikes your foes!\n\r", ch );
   }
   else
   {
      act( "$p summons lightning to strike $n's foes!", ch, obj, NULL, TO_ROOM );
      act( "$p summons lightning to strike your foes!", ch, obj, NULL, TO_CHAR );
   }
   CREF( vch_next, CHAR_NEXTROOM );
   for( vch = first_char; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next;
      if( vch->in_room == NULL )
         continue;
      if( vch->in_room == ch->in_room )
      {
         if( vch != ch && ( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) ) )
         {
            sp_damage( obj, ch, vch, ( saves_spell( level, vch ) ? dam / 2 : dam ),
                       REALM_SHOCK | NO_REFLECT | NO_ABSORB, sn, TRUE );
         }
         continue;
      }

      if( vch->in_room->area == ch->in_room->area && IS_OUTSIDE( vch ) && IS_AWAKE( vch ) )
         send_to_char( "Lightning flashes in the sky.\n\r", vch );
   }
   CUREF( vch_next );
   return TRUE;
}




bool spell_cause_light( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   sp_damage( obj, ch, ( CHAR_DATA * ) vo, dice( 2, 10 ) + level / 3, REALM_DRAIN, sn, TRUE );
   return TRUE;
}



bool spell_cause_critical( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   sp_damage( obj, ch, ( CHAR_DATA * ) vo, dice( 10, 30 ) + level - 6, REALM_DRAIN, sn, TRUE );
   return TRUE;
}



bool spell_cause_serious( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   sp_damage( obj, ch, ( CHAR_DATA * ) vo, dice( 8, 20 ) + level / 2, REALM_DRAIN, sn, TRUE );
   return TRUE;
}



bool spell_change_sex( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, sn ) || saves_spell( level, victim ) )
      return FALSE;
   af.type = sn;
   af.duration = 5 + ( level / 10 );
   af.location = APPLY_SEX;
   do
   {
      af.modifier = number_range( 0, 2 ) - victim->sex;
   }
   while( af.modifier == 0 );
   af.bitvector = 0;
   affect_to_char( victim, &af );
   send_to_char( "You feel different.\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_charm_person( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( victim == ch )
   {
      send_to_char( "You like yourself even better!\n\r", ch );
      return FALSE;
   }

   if( !IS_NPC( victim ) )
   {
      send_to_char( "You're not that powerful.\n\r", ch );
      return FALSE;
   }

   if( IS_AFFECTED( victim, AFF_CHARM )
       || IS_AFFECTED( ch, AFF_CHARM ) || level - 5 < victim->level || saves_spell( level, victim ) )
      return TRUE;

   if( IS_VAMP( victim ) && ( IS_NPC( victim ) ) )
   {
      send_to_char( "Wow, it seems to be immune--imagine that!\n\r", ch );
      return TRUE;
   }


   if( victim->master )
      stop_follower( victim );
   add_follower( victim, ch );
   af.type = sn;
   af.duration = 3 + ( level / 8 );
   af.location = 0;
   af.modifier = 0;
   af.bitvector = AFF_CHARM;
   affect_to_char( victim, &af );
   act( "Isn't $n just so nice?", ch, NULL, victim, TO_VICT );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   victim->extract_timer = get_psuedo_level( ch ) / 3;
   return TRUE;
}



bool spell_chill_touch( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   static const sh_int dam_each[] = {
      0,
      0, 0, 6, 7, 8, 9, 12, 13, 13, 13,
      14, 14, 14, 15, 15, 15, 16, 16, 16, 17,
      17, 17, 18, 18, 18, 19, 19, 19, 20, 20,
      20, 21, 21, 21, 22, 22, 22, 23, 23, 23,
      24, 24, 24, 25, 25, 25, 26, 26, 26, 27
   };
   AFFECT_DATA af;
   int dam;

   level = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
   level = UMAX( 0, level );
   dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
   if( !saves_spell( level, victim ) )
   {
      if( sp_damage( obj, ch, victim, dam, REALM_COLD, sn, TRUE ) )
      {
         af.type = sn;
         af.duration = 3;
         af.location = APPLY_STR;
         af.modifier = -1;
         af.bitvector = 0;
         affect_join( victim, &af );
      }
   }
   else
   {
      sp_damage( obj, ch, victim, dam / 2, REALM_COLD, sn, TRUE );
   }

   return TRUE;
}



bool spell_color_spray( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   static const sh_int dam_each[] = {
      0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      30, 35, 40, 45, 50, 55, 55, 55, 56, 57,
      58, 58, 59, 60, 61, 61, 62, 63, 64, 64,
      65, 66, 67, 67, 68, 69, 70, 70, 71, 72,
      73, 73, 74, 75, 76, 76, 77, 78, 79, 79
   };
   int dam;

   level = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
   level = UMAX( 0, level );
   dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
   if( saves_spell( level, victim ) )
      dam /= 2;

   sp_damage( obj, ch, victim, dam, REALM_MIND, sn, TRUE );
   return TRUE;
}



bool spell_continual_light( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *light;

   light = create_object( get_obj_index( OBJ_VNUM_LIGHT_BALL ), 0 );
   obj_to_room( light, ch->in_room );
   if( obj == NULL )
   {
      act( "$n twiddles $s thumbs and $p appears.", ch, light, NULL, TO_ROOM );
      act( "You twiddle your thumbs and $p appears.", ch, light, NULL, TO_CHAR );
   }
   else
      act( "$p suddenly appears before you!", NULL, light, NULL, TO_ROOM );
   return TRUE;
}



bool spell_control_weather( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   if( !str_cmp( target_name, "better" ) )
      weather_info.change += dice( level / 3, 4 );
   else if( !str_cmp( target_name, "worse" ) )
      weather_info.change -= dice( level / 3, 4 );
   else
   {
      send_to_char( "Do you want it to get better or worse?\n\r", ch );
      return FALSE;
   }
   send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_create_food( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *mushroom;

   mushroom = create_object( get_obj_index( OBJ_VNUM_MUSHROOM ), 0 );
   mushroom->value[0] = 5 + level;
   obj_to_room( mushroom, ch->in_room );
   act( "$p suddenly appears.", ch, mushroom, NULL, TO_ROOM );
   act( "$p suddenly appears.", ch, mushroom, NULL, TO_CHAR );
   return TRUE;
}



bool spell_create_spring( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *spring;

   spring = create_object( get_obj_index( OBJ_VNUM_SPRING ), 0 );
   spring->timer = level;
   obj_to_room( spring, ch->in_room );
   act( "$p flows from the ground.", ch, spring, NULL, TO_CHAR );
   act( "$p flows from the ground.", ch, spring, NULL, TO_ROOM );
   return TRUE;
}



bool spell_create_water( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *ob = ( OBJ_DATA * ) vo;
   int water;

   if( ob->item_type != ITEM_DRINK_CON )
   {
      send_to_char( "It is unable to hold water.\n\r", ch );
      return FALSE;
   }

   if( ob->value[2] != LIQ_WATER && ob->value[1] != 0 )
   {
      send_to_char( "It contains some other liquid.\n\r", ch );
      return FALSE;
   }

   water = UMIN( level * ( weather_info.sky >= SKY_RAINING ? 4 : 2 ), ob->value[0] - ob->value[1] );

   if( water > 0 )
   {
      ob->value[2] = LIQ_WATER;
      ob->value[1] += water;
      if( !is_name( "water", ob->name ) )
      {
         char buf[MAX_STRING_LENGTH];
         sprintf( buf, "%s water", ob->name );
         free_string( ob->name );
         ob->name = str_dup( buf );
      }
      act( "$p is filled.", ch, ob, NULL, TO_CHAR );
   }

   return TRUE;
}



bool spell_cure_blindness( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   if( !is_affected( victim, gsn_blindness ) )
      return ( ch == victim ? FALSE : TRUE );
   affect_strip( victim, gsn_blindness );
   send_to_char( "Your vision returns!\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_cure_critical( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int heal;

   heal = UMIN( 150, dice( 12, 8 ) + level );
   victim->hit = UMIN( victim->hit + heal, victim->max_hit );
   update_pos( victim );
   send_to_char( "You feel better!\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_cure_light( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int heal;

   heal = UMIN( 50, dice( 5, 8 ) + level );
   victim->hit = UMIN( victim->hit + heal, victim->max_hit );
   update_pos( victim );
   send_to_char( "You feel better!\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_cure_poison( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   if( is_affected( victim, gsn_poison ) )
   {
      affect_strip( victim, gsn_poison );
      act( "$N looks better.", ch, NULL, victim, TO_NOTVICT );
      send_to_char( "A warm feeling runs through your body.\n\r", victim );
      send_to_char( "Ok.\n\r", ch );
   }
   return TRUE;
}



bool spell_cure_serious( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int heal;

   heal = UMIN( 200, dice( 6, 8 ) + level );
   victim->hit = UMIN( victim->hit + heal, victim->max_hit );
   update_pos( victim );
   send_to_char( "You feel better!\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_curse( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_CURSE ) )
   {
      send_to_char( "They are already weakened!\n\r", ch );
      return FALSE;
   }
   if( saves_spell( level, victim ) )
      return TRUE;
   af.type = sn;
   af.duration = 2 * ( level / 8 );
   af.location = APPLY_HITROLL;
   af.modifier = -1;
   af.bitvector = AFF_CURSE;
   affect_to_char( victim, &af );

   af.location = APPLY_SAVING_SPELL;
   af.modifier = 1;
   affect_to_char( victim, &af );

   send_to_char( "You feel unclean.\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_detect_evil( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_DETECT_EVIL ) )
      return FALSE;
   af.type = sn;
   af.duration = 5 + ( level / 10 );
   af.modifier = 0;
   af.location = APPLY_NONE;
   af.bitvector = AFF_DETECT_EVIL;
   affect_to_char( victim, &af );
   send_to_char( "Your eyes tingle.\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_detect_hidden( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_DETECT_HIDDEN ) )
      return FALSE;
   af.type = sn;
   af.duration = 4 + ( level / 8 );
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.bitvector = AFF_DETECT_HIDDEN;
   affect_to_char( victim, &af );
   send_to_char( "Your awareness improves.\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_detect_invis( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_DETECT_INVIS ) )
      return FALSE;
   af.type = sn;
   af.duration = 6 + ( level / 8 );
   af.modifier = 0;
   af.location = APPLY_NONE;
   af.bitvector = AFF_DETECT_INVIS;
   affect_to_char( victim, &af );
   send_to_char( "Your eyes tingle.\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_detect_magic( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_DETECT_MAGIC ) || item_has_apply( victim, ITEM_APPLY_DET_MAG ) )
      return FALSE;
   af.type = sn;
   af.duration = 6 + ( level / 4 );
   af.modifier = 0;
   af.location = APPLY_NONE;
   af.bitvector = AFF_DETECT_MAGIC;
   affect_to_char( victim, &af );
   send_to_char( "Your eyes tingle.\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_detect_undead( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, skill_lookup( "detect undead" ) ) || item_has_apply( victim, ITEM_APPLY_DET_UNDEAD ) )
      return FALSE;

   af.type = sn;
   af.duration = 5 + ( level / 6 );
   af.modifier = 0;
   af.location = APPLY_NONE;
   af.bitvector = 0;
   affect_to_char( victim, &af );
   send_to_char( "Your eyes tingle.\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}


bool spell_detect_poison( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *ob = ( OBJ_DATA * ) vo;

   if( ob->item_type == ITEM_DRINK_CON || ob->item_type == ITEM_FOOD )
   {
      if( ob->value[3] != 0 )
         send_to_char( "You smell poisonous fumes.\n\r", ch );
      else
         send_to_char( "It looks very delicious.\n\r", ch );
   }
   else
   {
      send_to_char( "It doesn't look poisoned.\n\r", ch );
   }

   return TRUE;
}

bool spell_dispel_magic( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   /*
    * Remove certain affects from victim.  Chance will be 100% if
    * * ch == victim.  Otherwise, variable chance of success, depending
    * * on if a) victim is NPC b) Difference in levels.
    * * Rewritten yet AGAIN to check chance for each dispel... also works
    * * on objects as well :)
    * * Stephen
    */
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   CHAR_DATA *victim;
   AFFECT_DATA *paf;
   AFFECT_DATA *paf_next;
   OBJ_DATA *ob;
   int chance;
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You are too @@rENRAGED @@NTo cast spells!\n\r", ch );
      return FALSE;
   }

   if( target_name[0] == '\0' && ch->fighting == NULL )
   {
      send_to_char( "Dispel who or what??\n\r", ch );
      return FALSE;
   }
   if( target_name[0] == '\0' && ch->fighting != NULL )
      target_name = "enemy";

   if( ( victim = get_char_room( ch, target_name ) ) != NULL )
   {
      if( IS_NPC( victim ) )
         chance = 100;
      else
         chance = 75;

      if( ch == victim )
         chance = 100;

      chance += ( ( get_psuedo_level( ch ) - get_psuedo_level( victim ) ) );
      /*
       * Bonus/penalty for difference in levels. 
       */
      if( obj == NULL )
      {
         if( ch == victim )
         {
            send_to_char( "You gesture towards yourself.\n\r", ch );
            act( "$n gestures towards $mself.", ch, NULL, NULL, TO_ROOM );
         }
         else
         {
            act( "You gesture towards $N.", ch, NULL, victim, TO_CHAR );
            act( "$n gestures towards $N.", ch, NULL, victim, TO_NOTVICT );
            act( "$N gestures towards you.", victim, NULL, ch, TO_CHAR );
         }
      }
      else
      {
         act( "$p glows brightly at $n.", victim, obj, NULL, TO_ROOM );
         act( "$p glows brightly towards you.", victim, obj, NULL, TO_CHAR );
      }
      /*
       * So now, player should have 'rolled' less than chance, so check
       * * and remove affects, with some messages too.
       */





      if( ( IS_AFFECTED( victim, AFF_CLOAK_REFLECTION ) )
          && ( ch != victim ) && ( number_percent(  ) < ( victim->level - 50 ) ) )
      {

         act( "@@N$n's @@lc@@el@@ro@@ya@@ak@@N glows brightly as $N's spell hits it@@N!!", victim, NULL, ch, TO_ROOM );
         act( "@@N$N's @@lc@@el@@ro@@ya@@ak@@N glows brightly@@N!!", ch, NULL, victim, TO_CHAR );
         act( "@@NYour @@lc@@el@@ro@@ya@@ak@@N glows brightly@@N!!!", victim, NULL, ch, TO_CHAR );
         CREF( vch_next, CHAR_NEXTROOM );
         for( vch = ch->in_room->first_person; vch; vch = vch_next )
         {
            vch_next = vch->next_in_room;
            if( victim == vch && victim->fighting == NULL )
            {
               check_killer( ch, victim );
               multi_hit( victim, ch, TYPE_UNDEFINED );
               break;
            }
         }
         CUREF( vch_next );
         return TRUE;
      }


      if( ( IS_AFFECTED( victim, AFF_CLOAK_ABSORPTION ) )
          && ( ch != victim ) && ( number_percent(  ) < ( victim->level - 40 ) ) )
      {

         act( "@@N$n's @@lcloak@@N glows brightly as $N's spell hits it, then fades@@N!!", victim, NULL, ch, TO_ROOM );
         act( "@@N$N's @@lcloak@@N glows brightly, and absorbs your spell@@N!!", ch, NULL, victim, TO_CHAR );
         act( "@@NYour @@lcloak@@N glows brightly, and absorbs $N's spell@@N!!!", victim, NULL, ch, TO_CHAR );
         CREF( vch_next, CHAR_NEXTROOM );

         for( vch = ch->in_room->first_person; vch; vch = vch_next )
         {
            vch_next = vch->next_in_room;
            if( victim == vch && victim->fighting == NULL )
            {
               check_killer( ch, victim );
               multi_hit( victim, ch, TYPE_UNDEFINED );
               break;
            }
         }
         CUREF( vch_next );

         return TRUE;
      }

      for( paf = victim->first_affect; paf != NULL; paf = paf_next )
      {
         paf_next = paf->next;
         if( paf->type == skill_lookup( "Enraged" ) )
            continue;
         if( ( obj ) || ( victim == ch->fighting ) )
         {
            switch ( paf->bitvector )
            {
               case AFF_BLIND:
               case AFF_FAERIE_FIRE:
               case AFF_CURSE:
               case AFF_POISON:
               case AFF_SLEEP:
               case AFF_CHARM:
               case AFF_HOLD:
               case AFF_PARALYSIS:
                  continue;
                  break;
               default:
                  break;
            }
         }
         if( number_percent(  ) < chance )
         {
            affect_remove( victim, paf );
            chance = ( 2 * chance ) / 3;
         }
         else
            break;
      }

      if( IS_NPC( victim ) )
      {
         if( IS_AFFECTED( victim, AFF_CLOAK_ABSORPTION ) && ( number_percent(  ) < chance ) )
         {
            chance = ( chance ) / 3;
            act( "@@NThe @@lcloak@@N around $n is ripped to shreds!", victim, NULL, victim, TO_ROOM );
            send_to_char( "@@NYour @@lcloak@@N is ripped from your body!\n\r", victim );
            REMOVE_BIT( victim->affected_by, AFF_CLOAK_ABSORPTION );
         }


         if( IS_AFFECTED( victim, AFF_CLOAK_REFLECTION ) && ( number_percent(  ) < chance ) )
         {
            chance = ( chance ) / 3;
            act( "@@NThe @@lc@@el@@ro@@ya@@ak@@N around $n is ripped to shreds!", victim, NULL, NULL, TO_ROOM );
            send_to_char( "@@NYour @@lc@@el@@ro@@ya@@ak@@N is ripped from your body!\n\r", victim );
            REMOVE_BIT( victim->affected_by, AFF_CLOAK_REFLECTION );
         }

         if( IS_AFFECTED( victim, AFF_SANCTUARY ) && ( number_percent(  ) < chance ) )
         {
            chance = ( 2 * chance ) / 3;
            act( "The white aura around $n fades.", victim, NULL, NULL, TO_ROOM );
            send_to_char( "The white aura around you fades.\n\r", victim );
            REMOVE_BIT( victim->affected_by, AFF_SANCTUARY );
         }
         if( IS_AFFECTED( victim, AFF_FLYING ) && ( number_percent(  ) < chance ) )
         {
            act( "$n suddenly drops to the ground!", victim, NULL, NULL, TO_ROOM );
            send_to_char( "You suddenly drop to the ground!\n\r", victim );
            REMOVE_BIT( victim->affected_by, AFF_SANCTUARY );
         }
         if( IS_AFFECTED( victim, AFF_INVISIBLE ) && ( number_percent(  ) < chance ) )
         {
            act( "$n flickers, and becomes visible.", victim, NULL, NULL, TO_ROOM );
            send_to_char( "You flicker, and become visible.\n\r", victim );
            REMOVE_BIT( victim->affected_by, AFF_INVISIBLE );
         }
         if( IS_AFFECTED( victim, AFF_FAERIE_FIRE ) && ( number_percent(  ) < chance ) )
         {
            act( "The pink glow around $n suddenly fades.", victim, NULL, NULL, TO_ROOM );
            send_to_char( "The pink glow around you suddenly fades.\n\r", ch );
            REMOVE_BIT( victim->affected_by, AFF_FAERIE_FIRE );
         }
         if( IS_AFFECTED( victim, AFF_DETECT_INVIS ) && ( number_percent(  ) < chance ) )
            REMOVE_BIT( victim->affected_by, AFF_DETECT_INVIS );

         if( IS_AFFECTED( victim, AFF_DETECT_EVIL ) && ( number_percent(  ) < chance ) )
            REMOVE_BIT( victim->affected_by, AFF_DETECT_EVIL );

         if( IS_AFFECTED( victim, AFF_DETECT_MAGIC ) && ( number_percent(  ) < chance ) )
            REMOVE_BIT( victim->affected_by, AFF_DETECT_MAGIC );

         if( IS_AFFECTED( victim, AFF_DETECT_HIDDEN ) && ( number_percent(  ) < chance ) )
            REMOVE_BIT( victim->affected_by, AFF_DETECT_HIDDEN );

/*	 if ( IS_AFFECTED( victim, AFF_INFRARED ) )
	 {
	    act( "The red glow in $n's eyes fades quickly.", victim, NULL, NULL, TO_ROOM );
	    send_to_char( "The red glow in your eyes fade.\n\r", victim );
	    REMOVE_BIT( victim->affected_by, AFF_INFRARED );
	 }
      
	 if ( IS_AFFECTED( victim, AFF_SNEAK ) )
	    REMOVE_BIT( victim->affected_by, AFF_SNEAK );

	 if ( IS_AFFECTED( victim, AFF_HIDE ) )
	    REMOVE_BIT( victim->affected_by, AFF_HIDE );    */


         if( IS_AFFECTED( victim, AFF_CLOAK_FLAMING ) && ( number_percent(  ) < chance ) )
         {
            chance = ( chance ) / 3;

            act( "@@NThe @@ecloak@@N around $n is ripped to shreds!", victim, NULL, NULL, TO_ROOM );
            send_to_char( "@@NYour @@ecloak@@N is ripped from your body!\n\r", victim );
            REMOVE_BIT( victim->affected_by, AFF_CLOAK_FLAMING );
         }



      }




      CREF( vch_next, CHAR_NEXTROOM );
      for( vch = ch->in_room->first_person; vch; vch = vch_next )
      {
         vch_next = vch->next_in_room;
         if( victim == vch && victim->fighting == NULL )
         {
            check_killer( ch, victim );
            multi_hit( victim, ch, TYPE_UNDEFINED );
            break;
         }
      }
      CUREF( vch_next );

      return TRUE;
   }
   /*
    * This won't work in conjunction with identify! 
    */
   if( ( ob = get_obj_carry( ch, target_name ) ) != NULL )
   {
      /*
       * NOTE: Must also remove ALL affects, otherwise players
       * * will be able to enchant, dispel, enchant.... -S-
       * * No they wouldnt, enchant has a check for obj->first_apply.
       * * -- Alty
       */

      if( IS_SET( ob->extra_flags, ITEM_GLOW ) )
      {
         REMOVE_BIT( ob->extra_flags, ITEM_GLOW );
         act( "$p stops glowing.", ch, ob, NULL, TO_ROOM );
         act( "$p stops glowing.", ch, ob, NULL, TO_CHAR );
      }
      if( IS_SET( ob->extra_flags, ITEM_HUM ) )
      {
         REMOVE_BIT( ob->extra_flags, ITEM_HUM );
         act( "The hum surrounding $p fades.", ch, ob, NULL, TO_CHAR );
         act( "The hum surrounding $p fades.", ch, ob, NULL, TO_ROOM );
      }
      if( IS_SET( ob->extra_flags, ITEM_DARK ) )
      {
         REMOVE_BIT( ob->extra_flags, ITEM_DARK );
         act( "$p looks brighter.", ch, ob, NULL, TO_CHAR );
         act( "$p looks brighter.", ch, ob, NULL, TO_ROOM );
      }
      if( IS_SET( ob->extra_flags, ITEM_EVIL ) )
      {
         REMOVE_BIT( ob->extra_flags, ITEM_EVIL );
         act( "$p looks less evil.", ch, ob, NULL, TO_CHAR );
         act( "$p looks less evil.", ch, ob, NULL, TO_ROOM );
      }
      if( IS_SET( ob->extra_flags, ITEM_NODROP ) )
         REMOVE_BIT( ob->extra_flags, ITEM_NODROP );

      if( IS_SET( ob->extra_flags, ITEM_INVIS ) )
      {
         REMOVE_BIT( ob->extra_flags, ITEM_INVIS );
         act( "$p fades back into view.", ch, ob, NULL, TO_CHAR );
         act( "$p fades back into view.", ch, ob, NULL, TO_ROOM );
      }
      if( IS_SET( ob->extra_flags, ITEM_MAGIC ) )
      {
         REMOVE_BIT( ob->extra_flags, ITEM_MAGIC );
         act( "$p looks less magical.", ch, ob, NULL, TO_CHAR );
         act( "$p looks less magical.", ch, ob, NULL, TO_ROOM );
      }
      if( IS_SET( ob->extra_flags, ITEM_BLESS ) )
      {
         REMOVE_BIT( ob->extra_flags, ITEM_BLESS );
         act( "$p looks less Holy.", ch, ob, NULL, TO_CHAR );
         act( "$p looks less Holy.", ch, ob, NULL, TO_ROOM );
      }
      if( IS_SET( ob->extra_flags, ITEM_NOREMOVE ) )
         REMOVE_BIT( ob->extra_flags, ITEM_NOREMOVE );

      ob->first_apply = NULL;
      ob->last_apply = NULL;

      return TRUE;
   }
   send_to_char( "Dispel who or what??\n\r", ch );
   return FALSE;
}


bool spell_dispel_evil( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;

   if( !IS_NPC( ch ) && IS_EVIL( ch ) )
      victim = ch;

   if( IS_GOOD( victim ) )
   {
      act( "God protects $N.", ch, NULL, victim, TO_ROOM );
      return TRUE;
   }

   if( IS_NEUTRAL( victim ) )
   {
      act( "$N does not seem to be affected.", ch, NULL, victim, TO_CHAR );
      return TRUE;
   }

   dam = dice( level, 4 );
   if( saves_spell( level, victim ) )
      dam /= 2;
   damage( ch, victim, dam, sn );
   return TRUE;
}



bool spell_earthquake( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;

   if( obj == NULL )
   {
      send_to_char( "The earth trembles beneath your feet!\n\r", ch );
      act( "$n makes the earth tremble and shiver.", ch, NULL, NULL, TO_ROOM );
   }
   else
   {
      act( "$p vibrates violently, making the earth tremble!", ch, obj, NULL, TO_CHAR );
      act( "$p vibrates violenty, making the earth around $n tremble!", ch, obj, NULL, TO_ROOM );
   }
   CREF( vch_next, CHAR_NEXT );
   for( vch = first_char; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next;
      if( vch->in_room == NULL )
         continue;
      if( vch->in_room == ch->in_room )
      {
         if( vch != ch && ( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) ) )
         {
            act( "$n loses $s footing, and falls to the ground!", vch, NULL, NULL, TO_ROOM );
            send_to_char( "You lose your footing, and fall to the ground!", vch );
            damage( ch, vch, level + dice( 20, 10 ), -1 );
         }
         else
         {
            act( "$n keeps $s footing, and stays where $e is.", vch, NULL, NULL, TO_ROOM );
            send_to_char( "You keep your footing.\n\r", vch );
         }
         continue;
      }

      if( vch->in_room->area == ch->in_room->area )
         send_to_char( "The earth trembles and shivers.\n\r", vch );
   }
   CUREF( vch_next );
   return TRUE;
}



bool spell_enchant_weapon( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *ob = ( OBJ_DATA * ) vo;
   AFFECT_DATA *paf;

   /*
    * Quick way to stop imms (Bash?) enchanting weapons for players 
    */
   if( IS_IMMORTAL( ch ) && ch->level != 85 )
   {
      send_to_char( "Nothing Happens.\n\r", ch );
      return FALSE;
   }

   /*
    * Should change the first_apply thing to something else..
    * right now you can't enchant ANYTHING with ANY affect -- Alty 
    */
   if( ob->item_type != ITEM_WEAPON || IS_OBJ_STAT( ob, ITEM_MAGIC ) || ob->first_apply != NULL )
   {
      send_to_char( "Nothing happens.\n\r", ch );
      return TRUE;
   }

   GET_FREE( paf, affect_free );
   paf->type = sn;
   paf->duration = -1;
   paf->location = APPLY_HITROLL;
   paf->modifier = UMIN( ( level / 30 ) + 1, ob->level );
   paf->bitvector = 0;
   LINK( paf, ob->first_apply, ob->last_apply, next, prev );

   GET_FREE( paf, affect_free );
   paf->type = -1;
   paf->duration = -1;
   paf->location = APPLY_DAMROLL;
   paf->modifier = UMIN( ( level / 40 ) + 1, ob->level );
   paf->bitvector = 0;
   LINK( paf, ob->first_apply, ob->last_apply, next, prev );


   if( IS_GOOD( ch ) )
   {
      SET_BIT( ob->extra_flags, ITEM_ANTI_EVIL );
      act( "$p glows white.", ch, ob, NULL, TO_CHAR );
   }
   else if( IS_EVIL( ch ) )
   {
      SET_BIT( ob->extra_flags, ITEM_ANTI_GOOD );
      act( "$p glows black.", ch, ob, NULL, TO_CHAR );
   }
   else
   {
      SET_BIT( ob->extra_flags, ITEM_ANTI_EVIL );
      SET_BIT( ob->extra_flags, ITEM_ANTI_GOOD );
      act( "$p glows yellow.", ch, ob, NULL, TO_CHAR );
   }

   return TRUE;
}

bool spell_encumber( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
/* Increases victim's ac.  Tweaked copy of armor function
 * --Stephen
 */
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, sn ) )
   {
      if( victim == ch )
         send_to_char( "You are already encumbered!\n\r", ch );
      else
         act( "$N is already encumbered!\n\r", ch, NULL, victim, TO_CHAR );
      return TRUE;
   }
   af.type = sn;
   af.duration = 5 + ( level / 16 );
   af.modifier = +40 + get_psuedo_level( ch ) / 5;
   af.location = APPLY_AC;
   af.bitvector = 0;
   affect_to_char( victim, &af );
   send_to_char( "You feel less protected.\n\r", victim );
   if( ch != victim )
      act( "$N looks less protected.", ch, NULL, victim, TO_CHAR );
   return TRUE;
}



/*
 * Drain XP, MANA, HP.
 * Caster gains HP.
 */
bool spell_energy_drain( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;

   if( !IS_VAMP( ch ) )
   {
      send_to_char( "This spell does not exist.\n\r", ch );
      return FALSE;
   }

   if( saves_spell( level, victim ) )
      return TRUE;

   ch->alignment = UMAX( -1000, ch->alignment - 200 );
   if( victim->level <= 2 )
   {
      dam = ch->hit + 1;
   }
   else
   {
      gain_exp( victim, 0 - number_range( level / 2, 3 * level / 2 ) );
      victim->mana /= 4;
      victim->move /= 4;
      dam = dice( level / 15, level );
      ch->hit = UMIN( ch->max_hit, ch->hit + dam );
   }

   sp_damage( obj, ch, victim, dam, REALM_DRAIN, sn, TRUE );

   return TRUE;
}



bool spell_fireball( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   static const sh_int dam_each[] = {
      0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 30, 35, 40, 45, 50, 55,
      60, 65, 70, 75, 80, 82, 84, 86, 88, 90,
      92, 94, 96, 98, 100, 102, 104, 106, 108, 110,
      112, 114, 116, 118, 120, 122, 124, 126, 128, 130
   };
   int dam;

   level = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
   level = UMAX( 0, level );
   dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
   if( saves_spell( level, victim ) )
      dam /= 2;
   sp_damage( obj, ch, victim, dam, REALM_FIRE, sn, TRUE );
   return TRUE;
}



bool spell_flamestrike( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;

   dam = dice( 8, level );
   if( saves_spell( level, victim ) )
      dam /= 2;

   sp_damage( obj, ch, victim, dam, REALM_FIRE, sn, TRUE );
   return TRUE;
}



bool spell_faerie_fire( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_FAERIE_FIRE ) )
      return ( ch == victim ? FALSE : TRUE );
   af.type = sn;
   af.duration = 5 + ( level / 5 );
   af.location = APPLY_AC;
   af.modifier = 2 * level;
   af.bitvector = AFF_FAERIE_FIRE;
   affect_to_char( victim, &af );
   send_to_char( "You are surrounded by a pink outline.\n\r", victim );
   act( "$n is surrounded by a pink outline.", victim, NULL, NULL, TO_ROOM );
   return TRUE;
}



bool spell_faerie_fog( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *ich;

   if( obj == NULL )
   {
      act( "$n conjures a cloud of purple smoke.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You conjure a cloud of purple smoke.\n\r", ch );
   }
   else
   {
      act( "A cloud of purple smoke flows from $p.", ch, obj, NULL, TO_ROOM );
      act( "A cloud of purple smoke flows from $p.", ch, obj, NULL, TO_ROOM );
   }
   for( ich = ch->in_room->first_person; ich != NULL; ich = ich->next_in_room )
   {
      if( !IS_NPC( ich ) && IS_SET( ich->act, PLR_WIZINVIS ) )
         continue;

      if( ich == ch || saves_spell( level, ich ) )
         continue;

      affect_strip( ich, gsn_invis );
      affect_strip( ich, gsn_mass_invis );
      affect_strip( ich, gsn_sneak );
      REMOVE_BIT( ich->affected_by, AFF_HIDE );
      REMOVE_BIT( ich->affected_by, AFF_INVISIBLE );
      REMOVE_BIT( ich->affected_by, AFF_SNEAK );
      act( "$n is revealed!", ich, NULL, NULL, TO_ROOM );
      send_to_char( "You are revealed!\n\r", ich );
   }

   return TRUE;
}



bool spell_fly( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_FLYING ) )
      return ( ch == victim ? FALSE : TRUE );
   af.type = sn;
   af.duration = 3 + ( level / 6 );
   af.location = 0;
   af.modifier = 0;
   af.bitvector = AFF_FLYING;
   affect_to_char( victim, &af );
   send_to_char( "Your feet rise off the ground.\n\r", victim );
   act( "$n's feet rise off the ground.", victim, NULL, NULL, TO_ROOM );
   return TRUE;
}



bool spell_gate( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   char_to_room( create_mobile( get_mob_index( MOB_VNUM_VAMPIRE ) ), ch->in_room );
   return TRUE;
}



/*
 * Spell for mega1.are from Glop/Erkenbrand.
 */
bool spell_general_purpose( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;

   dam = number_range( 25, 100 );
   if( saves_spell( level, victim ) )
      dam /= 2;
   if( obj != NULL )
   {
      act( "A round of general purpose ammo from $p strikes $n!", victim, obj, NULL, TO_ROOM );
      act( "A round of general purpose ammo from $p strikes you!", victim, obj, NULL, TO_CHAR );
   }
   else
   {
      act( "$n's general purpose ammo strikes $N!", ch, NULL, victim, TO_NOTVICT );
      act( "$n's general purpose ammo strikes you!", ch, NULL, victim, TO_VICT );
      act( "You strike $N with your general purpose ammo!", ch, NULL, victim, TO_CHAR );
   }

   damage( ch, victim, dam, -1 );
   return TRUE;
}



bool spell_giant_strength( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, sn ) )
      return FALSE;
   af.type = sn;
   af.duration = 2 + ( level / 4 );
   af.location = APPLY_STR;
   af.modifier = 2 + ( level >= 50 ) + ( level >= 65 );
   af.bitvector = 0;
   affect_to_char( victim, &af );
   send_to_char( "You feel stronger.\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_harm( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;
   AFFECT_DATA af;

   dam = UMIN( 180, victim->hit - dice( 1, 4 ) );
   if( saves_spell( level, victim ) || ( saves_spell( level, victim ) ) )
      dam = UMIN( 100, dam / 4 );
   if( sp_damage( obj, ch, victim, dam, REALM_DRAIN, sn, TRUE ) )
   {
      af.type = skill_lookup( "poison" );
      af.duration = 12 + ( level / 10 );
      af.location = APPLY_STR;
      af.modifier = -2;
      af.bitvector = AFF_POISON;
      affect_join( victim, &af );
      send_to_char( "You feel very sick.\n\r", victim );
   }
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   return TRUE;
}



bool spell_heal( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   victim->hit = UMIN( victim->hit + 100 + dice( 30, level / 4 ), victim->max_hit );
   update_pos( victim );
   send_to_char( "A warm feeling fills your body.\n\r", victim );
   if( ch != victim )
      send_to_char( "Ok.\n\r", ch );
   spell_cure_poison( sn, level, ch, vo, obj );

   return TRUE;
}


bool spell_influx( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
/* -- Stephen */
{
   CHAR_DATA *vch;


   if( obj == NULL )
   {
      send_to_char( "You fill the room with healing energy!\n\r", ch );
      act( "$n spreads $s arms and heals the room!", ch, NULL, NULL, TO_ROOM );
   }
   else
   {
      act( "$p glows blindingly bright in $n's hand!", ch, obj, NULL, TO_ROOM );
      act( "$p glows blindingly in your hand!", ch, obj, NULL, TO_CHAR );
   }
   /*
    * need to do this on everyone! 
    */
   /*
    * for loop taken from spell_earthquake 
    */
   /*
    * Dont need to do on everyone.. earthquake does because it needs to
    * send messages to people in other rooms as well.. (whole area sees
    * "the earth trembles and shivers").. -- Alty 
    */

   for( vch = ch->in_room->first_person; vch; vch = vch->next_in_room )
   {
      vch->hit = UMIN( vch->hit + 40 + dice( 20, level / 10 ), vch->max_hit );
      update_pos( vch );
   }

/*    for ( vch=first_char; vch != NULL; vch = vch->next )
    {
	
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    CHAR_DATA *victim = (CHAR_DATA *) vo;
	    victim->hit = UMIN( victim->hit + 40, victim->max_hit );
	    update_pos( victim );
	}
    }*/
   return TRUE;
}




/*
 * Spell for mega1.are from Glop/Erkenbrand.
 */
bool spell_high_explosive( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;

   if( obj != NULL )
   {
      act( "A high explosive charge from $p engulfs $n!", victim, obj, NULL, TO_ROOM );
      act( "A high explosive charge from $p engulfs you!", victim, obj, NULL, TO_CHAR );
   }
   else
   {
      act( "$n's high explosive charge engulfs $N!", ch, NULL, victim, TO_NOTVICT );
      act( "Your high explosive charge engulfs $N!", ch, NULL, victim, TO_CHAR );
      act( "$n's high explosive charge engulfs you!", ch, NULL, victim, TO_VICT );
   }

   dam = number_range( 30, 120 );
   if( saves_spell( level, victim ) )
      dam /= 2;
   damage( ch, victim, dam, sn );
   return TRUE;
}

bool spell_laserbolt( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;

   dam = number_range( level / 2, level * 1.5 );
   if( saves_spell( level, victim ) )
      dam /= 2;

   sp_damage( obj, ch, victim, dam, REALM_LIGHT, sn, TRUE );
   return TRUE;
}

bool spell_identify( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *ob = ( OBJ_DATA * ) vo;
   char buf[MAX_STRING_LENGTH];
   AFFECT_DATA *paf;

   sprintf( buf,
            "@@NObject '%s' is @@etype@@N %s, @@aextra flags@@N %s.\n\r@@mWorn@@N: %s, @@cWeight@@N: %d, @@yvalue@@N: %s @@N, @@rlevel@@N: %d.\n\r",
            ob->short_descr,
            item_type_name( ob ),
            extra_bit_name( ob->extra_flags ),
            bit_table_lookup( tab_wear_flags, ob->wear_flags ), ob->weight, cost_to_money( ob->cost ), ob->level );
   send_to_char( buf, ch );

   switch ( ob->item_type )
   {
      case ITEM_SCROLL:
      case ITEM_POTION:
         sprintf( buf, "Level %d spells of:", ob->value[0] );
         send_to_char( buf, ch );

         if( ob->value[1] >= 0 && ob->value[1] < MAX_SKILL )
         {
            send_to_char( " '", ch );
            send_to_char( skill_table[ob->value[1]].name, ch );
            send_to_char( "'", ch );
         }

         if( ob->value[2] >= 0 && ob->value[2] < MAX_SKILL )
         {
            send_to_char( " '", ch );
            send_to_char( skill_table[ob->value[2]].name, ch );
            send_to_char( "'", ch );
         }

         if( ob->value[3] >= 0 && ob->value[3] < MAX_SKILL )
         {
            send_to_char( " '", ch );
            send_to_char( skill_table[ob->value[3]].name, ch );
            send_to_char( "'", ch );
         }

         send_to_char( ".\n\r", ch );
         break;

      case ITEM_WAND:
      case ITEM_STAFF:
         sprintf( buf, "Has %d(%d) charges of level %d", ob->value[1], ob->value[2], ob->value[0] );
         send_to_char( buf, ch );

         if( ob->value[3] >= 0 && ob->value[3] < MAX_SKILL )
         {
            send_to_char( " '", ch );
            send_to_char( skill_table[ob->value[3]].name, ch );
            send_to_char( "'", ch );
         }

         send_to_char( ".\n\r", ch );
         break;

      case ITEM_WEAPON:
         sprintf( buf, "Damage is %d to %d (average %d).\n\r",
                  ob->value[1], ob->value[2], ( ob->value[1] + ob->value[2] ) / 2 );
         send_to_char( buf, ch );
         break;

      case ITEM_ARMOR:
         sprintf( buf, "Armor class is %d.\n\r", ob->value[0] );
         send_to_char( buf, ch );
         break;
   }

   for( paf = ob->first_apply; paf != NULL; paf = paf->next )
   {
      if( paf->location != APPLY_NONE && paf->modifier != 0 )
      {
         sprintf( buf, "Affects %s by %d.\n\r", affect_loc_name( paf->location ), paf->modifier );
         send_to_char( buf, ch );
      }
   }

   return TRUE;
}



bool spell_infravision( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_INFRARED ) || item_has_apply( victim, ITEM_APPLY_INFRA ) )
      return ( ch == victim ? FALSE : TRUE );
   af.type = sn;
   af.duration = 4 + ( level / 3 );
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.bitvector = AFF_INFRARED;
   affect_to_char( victim, &af );
   send_to_char( "Your eyes glow red.\n\r", victim );
   act( "$n's eyes glow red.", victim, NULL, NULL, TO_ROOM );
   return TRUE;
}
