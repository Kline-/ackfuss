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

#ifndef DEC_BUILDTAB_H
#include "h/buildtab.h"
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

#ifndef DEC_INTERP_H
#include "h/interp.h"
#endif

#ifndef DEC_MAGIC_H
#include "h/magic.h"
#endif

#ifndef DEC_MONEY_H
#include "h/money.h"
#endif

#ifndef DEC_OBJ_FUN_H
#include "h/obj_fun.h"
#endif

#ifndef DEC_SPELL_DAM_H
#include "h/spell_dam.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

/* Calculate mana cost */
int mana_cost( CHAR_DATA * ch, int sn )
{
   int best;
   int foo;
   int skill_lev;
   float cost, mincost;
   int p_class = 0;



   if( IS_NPC( ch ) )
   {
      best = ch->get_level("psuedo");
      for( foo = 0; foo < MAX_CLASS; foo++ )
         if( best >= skill_table[sn].skill_level[foo] )
         {
            p_class = foo;
         }

      if( ( skill_table[sn].flag1 == REMORT )
          && ( ( ( ch->act.test(ACT_PET ) ) || ( IS_AFFECTED( ch, AFF_CHARM ) ) ) && ( ch->rider == NULL ) ) )
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
            p_class = foo;
         }
   }
   if( skill_table[sn].flag1 == ADEPT )
   {
      best = -1;
      if( IS_ADEPT(ch) && ch->get_level("adept") >= skill_table[sn].skill_level[0] )
         best = ch->get_level("adept") * 4;
   }

   if( ( best == -1 ) && ( IS_NPC( ch ) ) )
   {
      if( ( ch->act.test(ACT_INTELLIGENT ) ) && ( sn == skill_lookup( "ethereal" ) ) )
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

   skill_lev = skill_table[sn].skill_level[p_class];

   if( skill_lev > best )
      cost = 1000;
   else
      cost = UMAX( skill_table[sn].min_mana, 100 / ( 2 + best - skill_lev ) );

   if( cost < mincost )
      mincost = cost;

   if( IS_VAMP( ch ) && ( skill_table[sn].flag2 == VAMP ) )
      mincost = skill_table[sn].min_mana;
   if( IS_NPC( ch ) && ch->act.test(ACT_INTELLIGENT ) )
      mincost = skill_table[sn].min_mana + ( 200 - ch->level );
   if( skill_table[sn].flag2 == WOLF )
   {
      if( IS_NPC( ch ) )
         return 5000;
      if( IS_WOLF( ch ) )
      {
         cost = UMAX( skill_table[sn].min_mana,
                      ( ( skill_table[sn].min_mana * skill_lev ) /
                        ( ( ch->pcdata->super->level == 0 ) ? 1 : ch->pcdata->super->level ) ) );
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
   return (int)mincost;
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
      char *sold;
      char *snew;
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
      for( iSyl = 0; ( length = strlen( syl_table[iSyl].sold ) ) != 0; iSyl++ )
      {
         if( !str_prefix( syl_table[iSyl].sold, pName ) )
         {
            strncat( buf, syl_table[iSyl].snew, MSL-1 );
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
         snprintf( msg, MSL, "$n closes $s eyes, and prays." );
         snprintf( msg2, MSL, "You close your eyes and pray.\r\n" );
         break;
      case TAR_CHAR_OFFENSIVE:
         snprintf( msg, MSL, "$n's eyes glow red for an instant!" );
         snprintf( msg2, MSL, "Your eyes glow red for an instant!\r\n" );
         break;
      case TAR_CHAR_DEFENSIVE:
         snprintf( msg, MSL, "$n falls silent as $e meditates." );
         snprintf( msg2, MSL, "You become silent as you meditate.\r\n" );
         break;
      case TAR_CHAR_SELF:
         snprintf( msg, MSL, "$n motions toward $mself." );
         snprintf( msg2, MSL, "You motion towards yourself.\r\n" );
         break;
      case TAR_OBJ_INV:
         snprintf( msg, MSL, "$n's hands briefly glow magically!" );
         snprintf( msg2, MSL, "Your hands briefly glow magically!\r\n" );
   }
   act( msg, ch, NULL, ch, TO_NOTVICT );
   send_to_char( msg2, ch );

   snprintf( buf2, MSL, "$n utters the words, '%s'.", buf );
   snprintf( buf, MSL, "$n utters the words, '%s'.", skill_table[sn].name );

   for( rch = ch->in_room->first_person; rch; rch = rch->next_in_room )
   {
      if( rch != ch )
         act( ch->p_class == rch->p_class ? buf : buf2, ch, NULL, rch, TO_VICT );
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
      ( ( victim->get_level("psuedo") > 60 ?
          victim->get_level("psuedo") * 2 / 3 :
          victim->get_level("psuedo") ) - level - URANGE( -40, victim->saving_throw, 40 ) );
   save -= wis_app[get_curr_wis( victim )].spell_save;
   if( ( IS_NPC( victim ) ) && ( victim->act.test(ACT_SOLO ) ) )
      save += 20;
   if( !IS_NPC( victim ) && ( IS_SET( race_table[victim->race].race_flags, RACE_MOD_RESIST_SPELL ) ) )
      save += 20;
   save = URANGE( 5, save, 95 );

   if( number_percent(  ) < save )
      saved = TRUE;

   snprintf( log_buf, (2 * MIL), "%s lvl %d wismod %d savemod %d save total %d against level %d, %s ).",
            victim->name.c_str(), victim->get_level("psuedo"),
            wis_app[get_curr_wis( victim )].spell_save,
            victim->saving_throw, save, level, ( saved ? "@@aSAVED@@N" : "@@eFAILED@@N" ) );
   monitor_chan( log_buf, MONITOR_MAGIC );

   return ( saved );

}



/*
 * The kludgy global is for spells who want more stuff from command line.
 */
char *target_name;

DO_FUN(do_cast)
{
 char tmp[100];
 char buf[MSL];
 char *color;
 int sn = -1;
 float mod_time;

 tmp[0] = '\0';
 one_argument(argument,tmp);

 if( IS_CASTING(ch) )
 {
  send_to_char("You are already casting a spell. Move or use @@Rstop@@N to cancel.\r\n",ch);
  return;
 }

 if( (sn = skill_lookup(tmp)) < 0 )
 {
  send_to_char("Wiggle swiggle biggle?\r\n",ch);
  return;
 }

 switch( skill_table[sn].target )
 {
  default:                 color = "@@N"; break;
  case TAR_IGNORE:         color = "@@p"; break;
  case TAR_CHAR_OFFENSIVE: color = "@@e"; break;
  case TAR_CHAR_DEFENSIVE: color = "@@l"; break;
  case TAR_CHAR_SELF:      color = "@@r"; break;
  case TAR_OBJ_INV:        color = "@@y"; break;
 }
 snprintf(buf,MSL,"$n's eyes begin %sglowing@@N as $e begins to utter magical incantations!",color);
 act(buf,ch,NULL,NULL,TO_ROOM);
 ch_printf(ch,"You begin casting %s%s@@N.\r\n",color,skill_table[sn].name);

 mod_time = cast_speed(ch,sn);

 free_string(ch->casting->arg);
 ch->casting->arg = str_dup(argument);
 ch->casting->time = mod_time;
 ch->casting->max_time = (mod_time * sysdata.max_pushback);
 cast_list.push_back(ch);

 return;
}

float cast_speed( CHAR_DATA *ch, int sn )
{
 float ret_val = skill_table[sn].beats;

 if( IS_SET(race_table[ch->race].race_flags,RACE_MOD_FAST_CAST) )
  ret_val *= 0.85;

 return ret_val;
}

DO_FUN(do_stop)
{
 if( !IS_CASTING(ch) )
 {
  send_to_char("You're not casting a spell right now!\r\n",ch);
  return;
 }
 send_to_char("You stop casting your spell.\r\n",ch);
 stop_casting(ch);
 return;
}

void stop_casting( CHAR_DATA *ch )
{
 free_string(ch->casting->arg);
 ch->casting->arg = &str_empty[0];
 ch->casting->time = 0;
 ch->casting->max_time = 0;
 ch->casting->pushback = 0;
 cast_list.remove(ch);
 return;
}

/* Note this is void cast, not do_cast. This will fire a spell from the casting timer,
 * do_cast will enqueue it to begin with. --Kline
 */
void cast( CHAR_DATA * ch, char *argument )
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
/* ZENFIX --ch's are surviving multiple kills per combat round */

   if( ( !IS_NPC( ch ) ) && ( ch->desc != NULL ) && ( ch->desc->connected == CON_SETTING_STATS ) )
      char_login = TRUE;


   /*
    * Modified to reference ch's highest ch->lvl[] value which
    * * apllies to the spell.  -S-
    */


   /*
    * -S- Mod: intelligent mobs can cast too.    NOT ANYMORE :P  Zen
    */
   /*
    * if ( IS_NPC(ch)
    * && ( !ch->act.test(ACT_INTELLIGENT ) )
    * || IS_AFFECTED( ch, AFF_CHARM ) ) )
    * return;
    */

   target_name = one_argument( argument, arg1 );
   one_argument( target_name, arg2 );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Cast which what where?\r\n", ch );
      return;
   }

   best = -1;  /* Default 'no way' value */

   if( ( sn = skill_lookup( arg1 ) ) == -1 )
   {
      send_to_char( "Wiggle swiggle biggle?\r\n", ch );
      return;
   }

   /*
    * Check if in a no-magic room 
    */
   if( !char_login && ch->in_room->room_flags.test(RFLAG_NO_MAGIC) && ( skill_table[sn].flag2 != WOLF ) && !IS_IMMORTAL(ch) )
   {
      send_to_char( "Some strange force prevents you casting the spell!\r\n", ch );
      return;
   }


   /*
    * Compute best level to use for spell, IF it meets requiements 
    */
   if( IS_NPC( ch ) )
   {
      best = UMIN( 90, ch->get_level("psuedo") );
      if( ( skill_table[sn].flag1 == REMORT )
          && ( ( ( ch->act.test(ACT_PET ) ) || ( IS_AFFECTED( ch, AFF_CHARM ) ) ) && ( ch->rider == NULL ) ) )
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
         best = ch->pcdata->super->level * 4;
   if( IS_ADEPT(ch) && ( skill_table[sn].flag1 == ADEPT ) )
      best = ch->get_level("adept") * 4;
   if( IS_NPC( ch ) && ( skill_table[sn].flag1 == ADEPT ) )
      best = -1;
   if( !IS_NPC( ch ) )
      if( skill_table[sn].flag2 == WOLF && IS_WOLF( ch ) )
         best = ch->pcdata->super->level * 4;


   if( best == 80 )
      best = 79;
   if( ( !IS_NPC( ch ) ) && ( is_name( skill_table[sn].name, race_table[ch->race].skill1 ) ) )
      best = 60;

   if( ( best == -1 )
       || ( ( skill_table[sn].flag2 == VAMP )
            && ( !IS_VAMP( ch ) ) ) || ( ( skill_table[sn].flag2 == WOLF ) && ( !IS_WOLF( ch ) ) ) )
   {
      send_to_char( "You can't do that.\r\n", ch );
      return;
   }

   if( ch->position < skill_table[sn].minimum_position )
   {
      send_to_char( "You can't concentrate enough.\r\n", ch );
      return;
   }
   if( IS_NPC( ch ) && skill_table[sn].flag2 == VAMP )
      return;
   if( IS_NPC( ch ) && skill_table[sn].flag2 == WOLF )
      return;

   mana = mana_cost( ch, sn );

   if( char_login )
      mana = 0;

   if( check_charm_aff(ch,CHARM_AFF_MAGE) )
    mana *= ((100 - get_charm_bonus(ch,CHARM_AFF_MAGE)) / 100);

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
               send_to_char( "Cast the spell on whom?\r\n", ch );
               return;
            }
         }
         else
         {
            if( ( victim = get_char_room( ch, arg2 ) ) == NULL )
            {
               send_to_char( "They aren't here.\r\n", ch );
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
		send_to_char( "You can't do that on a player.\r\n", ch );
		return;
	    }

	    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	    {
		send_to_char( "You can't do that on your own follower.\r\n",
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
               send_to_char( "They aren't here.\r\n", ch );
               return;
            }
         }

         vo = ( void * )victim;
         break;

      case TAR_CHAR_SELF:
         if( arg2[0] != '\0' && !is_name( arg2, const_cast<char *>(ch->name.c_str()) ) )
         {
            send_to_char( "You cannot cast this spell on another.\r\n", ch );
            return;
         }

         vo = ( void * )ch;
         break;

      case TAR_OBJ_INV:
         if( arg2[0] == '\0' )
         {
            send_to_char( "What should the spell be cast upon?\r\n", ch );
            return;
         }

         if( ( obj = get_obj_carry( ch, arg2 ) ) == NULL )
         {
            send_to_char( "You are not carrying that.\r\n", ch );
            return;
         }

         vo = ( void * )obj;
         break;
   }
   if( ( skill_table[sn].flag2 == NORM ) && ( is_affected( ch, skill_lookup( "mystical focus" ) ) ) )
   {
      float tmp = mana;
      tmp *= 2.5;
      mana = (int)tmp;
   }

   if( !IS_VAMP( ch ) && ( skill_table[sn].flag2 == VAMP ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }
   if( IS_VAMP( ch ) && ( skill_table[sn].flag2 == VAMP ) )
   {
      if( !IS_NPC( ch ) && ch->pcdata->super->energy < mana )
      {
         send_to_char( "@@NYou don't have enough @@eblood@@N to cast that spell!!!\r\n", ch );
         return;
      }
      else;
   }
   if( !IS_WOLF( ch ) && ( skill_table[sn].flag2 == WOLF ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }
   if( skill_table[sn].flag2 == WOLF && ( !IS_NPC( ch ) && IS_WOLF( ch ) ) && ch->pcdata->super->energy < mana )
   {
      send_to_char( "@@bYou aren't able to summon enough @@rRAGE@@N!!\r\n", ch );
      return;
   }
   else if( ch->mana < mana )
   {
      send_to_char( "You don't have enough mana.\r\n", ch );
      return;
   }

   if( str_cmp( skill_table[sn].name, "ventriloquate" ) )
      say_spell( ch, sn );

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
         send_to_char( "You lost your concentration.\r\n", ch );

         if( IS_VAMP( ch ) && ( skill_table[sn].flag2 == VAMP ) )
            ch->pcdata->super->energy -= mana / 2;
         else if( skill_table[sn].flag2 == WOLF && ( !IS_NPC( ch ) && IS_WOLF( ch ) ) )
            ch->pcdata->super->energy -= mana / 3;
         else
            ch->mana -= mana / 2;
         return;
      }
   }


   if( ( MAGIC_STANCE( ch ) )
       && ( skill_table[sn].target == TAR_CHAR_OFFENSIVE ) && ( number_range( 0, 99 ) < ch->get_level("psuedo") - 50 ) )
   {
      mana = mana * 2 / 3;
      multi_cast = TRUE;
   }
   if( ( sn != skill_lookup( "cure light" ) )
       && ( sn != skill_lookup( "cure serious" ) )
       && ( sn != skill_lookup( "cure critical" ) ) && ( sn != skill_lookup( "heal" ) ) )
   {
      snprintf( log_buf, (2 * MIL), "%s typed %s, Spell %s, room %s(%d), target %s",
               ch->name.c_str(), typed, skill_table[sn].name,
               ch->in_room->name, ch->in_room->vnum, ( victim != NULL ? victim->name.c_str() : obj != NULL ? obj->name : "NONE" ) );
      monitor_chan( log_buf, MONITOR_MAGIC );
   }
   if( ( *skill_table[sn].spell_fun ) ( sn, best, ch, vo, NULL ) )
   {
      if( ( skill_table[sn].flag2 == VAMP ) || ( skill_table[sn].flag2 == WOLF ) )
      {
         ch->pcdata->super->energy -= mana;
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
         if( ( ( victim = ch->fighting ) == NULL ) || ( victim->in_room == NULL ) )
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
            snprintf( log_buf, (2 * MIL), "%s typed %s, Spell %s, room %s(%d), target %s",
                     ch->name.c_str(), typed, skill_table[sn].name,
                     ch->in_room->name, ch->in_room->vnum,
                     ( victim != NULL ? victim->name.c_str() : obj != NULL ? obj->name : "NONE" ) );
            monitor_chan( log_buf, MONITOR_MAGIC );
         }
         if( ( *skill_table[sn].spell_fun ) ( sn, best, ch, vo, NULL ) )
         {
            if( ( skill_table[sn].flag2 == VAMP ) || ( skill_table[sn].flag2 == WOLF ) )
            {
               ch->pcdata->super->energy -= mana;
            }
            else
            {
               ch->mana -= mana; /* Only use mana if spell was called correctly */
            }
         }
      }
   }
   if( ( ch->stance == STANCE_MAGI )
       && ( skill_table[sn].target == TAR_CHAR_OFFENSIVE ) && ( number_range( 0, 99 ) < ch->get_level("psuedo") - 80 ) )
   {
      bool still_here = TRUE;
      mana = mana * 1 / 2;

      if( arg2[0] == '\0' )
      {
         if( ( ( victim = ch->fighting ) == NULL ) || ( victim->in_room == NULL ) )
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
            snprintf( log_buf, (2 * MIL), "%s typed %s, Spell %s, room %s(%d), target %s",
                     ch->name.c_str(), typed, skill_table[sn].name,
                     ch->in_room->name, ch->in_room->vnum,
                     ( victim != NULL ? victim->name.c_str() : obj != NULL ? obj->name : "NONE" ) );
            monitor_chan( log_buf, MONITOR_MAGIC );
         }
         if( ( *skill_table[sn].spell_fun ) ( sn, best, ch, vo, NULL ) )
         {
            if( ( skill_table[sn].flag2 == VAMP ) || ( skill_table[sn].flag2 == WOLF ) )
            {
               ch->pcdata->super->energy -= mana;
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
       && ( victim != NULL ) && ( ch != victim ) )
   {
      CHAR_DATA *vch;
      CHAR_DATA *vch_next;
      for( vch = ch->in_room->first_person; vch; vch = vch_next )
      {
         vch_next = vch->next_in_room;
         if( ( vch == victim ) && ( vch->fighting == NULL ) && ( vch->master != ch ) && ( vch != ch ) )
         {
            check_killer( ch, vch );
            one_hit( vch, ch, TYPE_UNDEFINED );  /* SRZ--fixed bad call--swap ch&v */
            break;
         }
      }
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
               send_to_char( "You can't do that.\r\n", ch );
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
               send_to_char( "You can't do that.\r\n", ch );
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
            one_hit( ch, victim, TYPE_UNDEFINED );  /* SRZ swapped ch& v */
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
   af.modifier = -20 - ch->get_level("psuedo") / 10;
   af.location = APPLY_AC;
   af.bitvector = 0;
   affect_to_char( victim, &af );
   send_to_char( "You feel someone protecting you.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}

bool spell_badbreath( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
/* --Stephen */
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   static const short dam_each[] = {
      0,
      3, 3, 4, 4, 5, 6, 6, 6, 6, 6,
      7, 7, 7, 7, 7, 8, 8, 8, 8, 8,
      9, 9, 9, 9, 9, 10, 10, 10, 10, 10,
      11, 11, 11, 11, 11, 12, 12, 12, 12, 12,
      13, 13, 13, 13, 13, 14, 14, 14, 14, 14
   };
   int dam;

   level = UMIN( level, (int)sizeof( dam_each ) / (int)sizeof( dam_each[0] ) - 1 );
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
   send_to_char( "Your skin turns to bark.\r\n", victim );
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
   send_to_char( "You feel righteous.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
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
   send_to_char( "You are blinded!\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}



bool spell_burning_hands( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   static const short dam_each[] = {
      0,
      0, 0, 0, 0, 14, 17, 20, 23, 26, 29,
      29, 29, 30, 30, 31, 31, 32, 32, 33, 33,
      34, 34, 35, 35, 36, 36, 37, 37, 38, 38,
      39, 39, 40, 40, 41, 41, 42, 42, 43, 43,
      44, 44, 45, 45, 46, 46, 47, 47, 48, 48
   };
   int dam;

   level = UMIN( level, (int)sizeof( dam_each ) / (int)sizeof( dam_each[0] ) - 1 );
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
   list<CHAR_DATA *>::iterator li;
   int dam;

   if( !IS_OUTSIDE( ch ) )
   {
      send_to_char( "You must be out of doors.\r\n", ch );
      return FALSE;
   }

   if( weather_info.sky < SKY_RAINING )
   {
      send_to_char( "You need bad weather.\r\n", ch );
      return FALSE;
   }

   dam = dice( level, 8 );


   if( obj == NULL )
   {
      act( "$n calls God's lightning to strike $s foes!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "God's lightning strikes your foes!\r\n", ch );
   }
   else
   {
      act( "$p summons lightning to strike $n's foes!", ch, obj, NULL, TO_ROOM );
      act( "$p summons lightning to strike your foes!", ch, obj, NULL, TO_CHAR );
   }
   for( li = ch->in_room->area->player_list.begin(); li != ch->in_room->area->player_list.end(); li++ )
   {
      vch = *li;
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

      if( IS_OUTSIDE( vch ) && IS_AWAKE( vch ) )
         send_to_char( "Lightning flashes in the sky.\r\n", vch );
   }

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
   send_to_char( "You feel different.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}



bool spell_charm_person( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( victim == ch )
   {
      send_to_char( "You like yourself even better!\r\n", ch );
      return FALSE;
   }

   if( !IS_NPC( victim ) )
   {
      send_to_char( "You're not that powerful.\r\n", ch );
      return FALSE;
   }

   if( IS_AFFECTED( victim, AFF_CHARM )
       || IS_AFFECTED( ch, AFF_CHARM ) || level - 5 < victim->level || saves_spell( level, victim ) )
      return TRUE;

   if( IS_VAMP( victim ) && ( IS_NPC( victim ) ) )
   {
      send_to_char( "Wow, it seems to be immune--imagine that!\r\n", ch );
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
      send_to_char( "Ok.\r\n", ch );
   victim->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   return TRUE;
}



bool spell_chill_touch( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   static const short dam_each[] = {
      0,
      0, 0, 6, 7, 8, 9, 12, 13, 13, 13,
      14, 14, 14, 15, 15, 15, 16, 16, 16, 17,
      17, 17, 18, 18, 18, 19, 19, 19, 20, 20,
      20, 21, 21, 21, 22, 22, 22, 23, 23, 23,
      24, 24, 24, 25, 25, 25, 26, 26, 26, 27
   };
   AFFECT_DATA af;
   int dam;

   level = UMIN( level, (int)sizeof( dam_each ) / (int)sizeof( dam_each[0] ) - 1 );
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
   static const short dam_each[] = {
      0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      30, 35, 40, 45, 50, 55, 55, 55, 56, 57,
      58, 58, 59, 60, 61, 61, 62, 63, 64, 64,
      65, 66, 67, 67, 68, 69, 70, 70, 71, 72,
      73, 73, 74, 75, 76, 76, 77, 78, 79, 79
   };
   int dam;

   level = UMIN( level, (int)sizeof( dam_each ) / (int)sizeof( dam_each[0] ) - 1 );
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
      send_to_char( "Do you want it to get better or worse?\r\n", ch );
      return FALSE;
   }
   send_to_char( "Ok.\r\n", ch );
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
      send_to_char( "It is unable to hold water.\r\n", ch );
      return FALSE;
   }

   if( ob->value[2] != LIQ_WATER && ob->value[1] != 0 )
   {
      send_to_char( "It contains some other liquid.\r\n", ch );
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
         snprintf( buf, MSL, "%s water", ob->name );
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
   send_to_char( "Your vision returns!\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}



bool spell_cure_critical( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int heal;

   heal = UMIN( 150, dice( 12, 8 ) + level );
   victim->hit = UMIN( victim->hit + heal, victim->max_hit );
   update_pos( victim );
   send_to_char( "You feel better!\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}



bool spell_cure_light( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int heal;

   heal = UMIN( 50, dice( 5, 8 ) + level );
   victim->hit = UMIN( victim->hit + heal, victim->max_hit );
   update_pos( victim );
   send_to_char( "You feel better!\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}



bool spell_cure_poison( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   if( is_affected( victim, gsn_poison ) )
   {
      affect_strip( victim, gsn_poison );
      act( "$N looks better.", ch, NULL, victim, TO_NOTVICT );
      send_to_char( "A warm feeling runs through your body.\r\n", victim );
      send_to_char( "Ok.\r\n", ch );
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
   send_to_char( "You feel better!\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}



bool spell_curse( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_CURSE ) )
   {
      send_to_char( "They are already weakened!\r\n", ch );
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

   send_to_char( "You feel unclean.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
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
   send_to_char( "Your eyes tingle.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
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
   send_to_char( "Your awareness improves.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
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
   send_to_char( "Your eyes tingle.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
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
   send_to_char( "Your eyes tingle.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
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
   send_to_char( "Your eyes tingle.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}


bool spell_detect_poison( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *ob = ( OBJ_DATA * ) vo;

   if( ob->item_type == ITEM_DRINK_CON || ob->item_type == ITEM_FOOD )
   {
      if( ob->value[3] != 0 )
         send_to_char( "You smell poisonous fumes.\r\n", ch );
      else
         send_to_char( "It looks very delicious.\r\n", ch );
   }
   else
   {
      send_to_char( "It doesn't look poisoned.\r\n", ch );
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
      send_to_char( "You are too @@rENRAGED @@NTo cast spells!\r\n", ch );
      return FALSE;
   }

   if( target_name[0] == '\0' && ch->fighting == NULL )
   {
      send_to_char( "Dispel who or what??\r\n", ch );
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

      chance += ( ( ch->get_level("psuedo") - victim->get_level("psuedo") ) );
      /*
       * Bonus/penalty for difference in levels. 
       */
      if( obj == NULL )
      {
         if( ch == victim )
         {
            send_to_char( "You gesture towards yourself.\r\n", ch );
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

         for( vch = ch->in_room->first_person; vch; vch = vch_next )
         {
            vch_next = vch->next_in_room;
            if( victim == vch && victim->fighting == NULL )
            {
               check_killer( ch, victim );
               one_hit( victim, ch, TYPE_UNDEFINED );
               break;
            }
         }
         return TRUE;
      }


      if( ( IS_AFFECTED( victim, AFF_CLOAK_ABSORPTION ) )
          && ( ch != victim ) && ( number_percent(  ) < ( victim->level - 40 ) ) )
      {

         act( "@@N$n's @@lcloak@@N glows brightly as $N's spell hits it, then fades@@N!!", victim, NULL, ch, TO_ROOM );
         act( "@@N$N's @@lcloak@@N glows brightly, and absorbs your spell@@N!!", ch, NULL, victim, TO_CHAR );
         act( "@@NYour @@lcloak@@N glows brightly, and absorbs $N's spell@@N!!!", victim, NULL, ch, TO_CHAR );

         for( vch = ch->in_room->first_person; vch; vch = vch_next )
         {
            vch_next = vch->next_in_room;
            if( victim == vch && victim->fighting == NULL )
            {
               check_killer( ch, victim );
               one_hit( victim, ch, TYPE_UNDEFINED );
               break;
            }
         }
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
            send_to_char( "@@NYour @@lcloak@@N is ripped from your body!\r\n", victim );
            REMOVE_BIT( victim->affected_by, AFF_CLOAK_ABSORPTION );
         }


         if( IS_AFFECTED( victim, AFF_CLOAK_REFLECTION ) && ( number_percent(  ) < chance ) )
         {
            chance = ( chance ) / 3;
            act( "@@NThe @@lc@@el@@ro@@ya@@ak@@N around $n is ripped to shreds!", victim, NULL, NULL, TO_ROOM );
            send_to_char( "@@NYour @@lc@@el@@ro@@ya@@ak@@N is ripped from your body!\r\n", victim );
            REMOVE_BIT( victim->affected_by, AFF_CLOAK_REFLECTION );
         }

         if( IS_AFFECTED( victim, AFF_SANCTUARY ) && ( number_percent(  ) < chance ) )
         {
            chance = ( 2 * chance ) / 3;
            act( "The white aura around $n fades.", victim, NULL, NULL, TO_ROOM );
            send_to_char( "The white aura around you fades.\r\n", victim );
            REMOVE_BIT( victim->affected_by, AFF_SANCTUARY );
         }
         if( IS_AFFECTED( victim, AFF_FLYING ) && ( number_percent(  ) < chance ) )
         {
            act( "$n suddenly drops to the ground!", victim, NULL, NULL, TO_ROOM );
            send_to_char( "You suddenly drop to the ground!\r\n", victim );
            REMOVE_BIT( victim->affected_by, AFF_SANCTUARY );
         }
         if( IS_AFFECTED( victim, AFF_INVISIBLE ) && ( number_percent(  ) < chance ) )
         {
            act( "$n flickers, and becomes visible.", victim, NULL, NULL, TO_ROOM );
            send_to_char( "You flicker, and become visible.\r\n", victim );
            REMOVE_BIT( victim->affected_by, AFF_INVISIBLE );
         }
         if( IS_AFFECTED( victim, AFF_FAERIE_FIRE ) && ( number_percent(  ) < chance ) )
         {
            act( "The pink glow around $n suddenly fades.", victim, NULL, NULL, TO_ROOM );
            send_to_char( "The pink glow around you suddenly fades.\r\n", ch );
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
	    send_to_char( "The red glow in your eyes fade.\r\n", victim );
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
            send_to_char( "@@NYour @@ecloak@@N is ripped from your body!\r\n", victim );
            REMOVE_BIT( victim->affected_by, AFF_CLOAK_FLAMING );
         }
      }

      for( vch = ch->in_room->first_person; vch; vch = vch_next )
      {
         vch_next = vch->next_in_room;
         if( victim == vch && victim->fighting == NULL )
         {
            check_killer( ch, victim );
            one_hit( victim, ch, TYPE_UNDEFINED );
            break;
         }
      }

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

      if( IS_OBJ_STAT(ob,ITEM_EXTRA_GLOW) )
      {
         ob->extra_flags.reset(ITEM_EXTRA_GLOW);
         act( "$p stops glowing.", ch, ob, NULL, TO_ROOM );
         act( "$p stops glowing.", ch, ob, NULL, TO_CHAR );
      }
      if( IS_OBJ_STAT(ob,ITEM_EXTRA_HUM) )
      {
         ob->extra_flags.reset(ITEM_EXTRA_HUM);
         act( "The hum surrounding $p fades.", ch, ob, NULL, TO_CHAR );
         act( "The hum surrounding $p fades.", ch, ob, NULL, TO_ROOM );
      }
      if( IS_OBJ_STAT(ob,ITEM_EXTRA_NO_DISARM) )
         ob->extra_flags.reset(ITEM_EXTRA_NO_DISARM);

      if( IS_OBJ_STAT(ob,ITEM_EXTRA_EVIL) )
      {
         ob->extra_flags.reset(ITEM_EXTRA_EVIL);
         act( "$p looks less evil.", ch, ob, NULL, TO_CHAR );
         act( "$p looks less evil.", ch, ob, NULL, TO_ROOM );
      }
      if( IS_OBJ_STAT(ob,ITEM_EXTRA_NO_DROP) )
         ob->extra_flags.reset(ITEM_EXTRA_NO_DROP);

      if( IS_OBJ_STAT(ob,ITEM_EXTRA_INVIS) )
      {
         ob->extra_flags.reset(ITEM_EXTRA_INVIS);
         act( "$p fades back into view.", ch, ob, NULL, TO_CHAR );
         act( "$p fades back into view.", ch, ob, NULL, TO_ROOM );
      }
      if( IS_OBJ_STAT(ob,ITEM_EXTRA_MAGIC) )
      {
         ob->extra_flags.reset(ITEM_EXTRA_MAGIC);
         act( "$p looks less magical.", ch, ob, NULL, TO_CHAR );
         act( "$p looks less magical.", ch, ob, NULL, TO_ROOM );
      }
      if( IS_OBJ_STAT(ob,ITEM_EXTRA_BLESS) )
      {
         ob->extra_flags.reset(ITEM_EXTRA_BLESS);
         act( "$p looks less Holy.", ch, ob, NULL, TO_CHAR );
         act( "$p looks less Holy.", ch, ob, NULL, TO_ROOM );
      }
      if( IS_OBJ_STAT(ob,ITEM_EXTRA_NO_REMOVE) )
         ob->extra_flags.reset(ITEM_EXTRA_NO_REMOVE);

      ob->first_apply = NULL;
      ob->last_apply = NULL;

      return TRUE;
   }
   send_to_char( "Dispel who or what??\r\n", ch );
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
   list<CHAR_DATA *>::iterator li;

   if( obj == NULL )
   {
      send_to_char( "The earth trembles beneath your feet!\r\n", ch );
      act( "$n makes the earth tremble and shiver.", ch, NULL, NULL, TO_ROOM );
   }
   else
   {
      act( "$p vibrates violently, making the earth tremble!", ch, obj, NULL, TO_CHAR );
      act( "$p vibrates violenty, making the earth around $n tremble!", ch, obj, NULL, TO_ROOM );
   }

   for( li = ch->in_room->area->player_list.begin(); li != ch->in_room->area->player_list.end(); li++ )
   {
      vch = *li;
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
            send_to_char( "You keep your footing.\r\n", vch );
         }
         continue;
      }

      send_to_char( "The earth trembles and shivers.\r\n", vch );
   }

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
      send_to_char( "Nothing Happens.\r\n", ch );
      return FALSE;
   }

   /*
    * Should change the first_apply thing to something else..
    * right now you can't enchant ANYTHING with ANY affect -- Alty 
    */
   if( ob->item_type != ITEM_WEAPON || IS_OBJ_STAT( ob, ITEM_EXTRA_MAGIC ) || ob->first_apply != NULL )
   {
      send_to_char( "Nothing happens.\r\n", ch );
      return TRUE;
   }

   paf = new AFFECT_DATA;
   paf->type = sn;
   paf->location = APPLY_HITROLL;
   paf->modifier = UMIN( ( level / 30 ) + 1, ob->level );
   LINK( paf, ob->first_apply, ob->last_apply, next, prev );

   paf = new AFFECT_DATA;
   paf->location = APPLY_DAMROLL;
   paf->modifier = UMIN( ( level / 40 ) + 1, ob->level );
   LINK( paf, ob->first_apply, ob->last_apply, next, prev );


   if( IS_GOOD( ch ) )
   {
      ob->extra_flags.set(ITEM_EXTRA_ANTI_EVIL);
      act( "$p glows white.", ch, ob, NULL, TO_CHAR );
   }
   else if( IS_EVIL( ch ) )
   {
      ob->extra_flags.set(ITEM_EXTRA_ANTI_GOOD);
      act( "$p glows black.", ch, ob, NULL, TO_CHAR );
   }
   else
   {
      ob->extra_flags.set(ITEM_EXTRA_ANTI_EVIL);
      ob->extra_flags.set(ITEM_EXTRA_ANTI_GOOD);
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
         send_to_char( "You are already encumbered!\r\n", ch );
      else
         act( "$N is already encumbered!\r\n", ch, NULL, victim, TO_CHAR );
      return TRUE;
   }
   af.type = sn;
   af.duration = 5 + ( level / 16 );
   af.modifier = +40 + ch->get_level("psuedo") / 5;
   af.location = APPLY_AC;
   af.bitvector = 0;
   affect_to_char( victim, &af );
   send_to_char( "You feel less protected.\r\n", victim );
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
      send_to_char( "This spell does not exist.\r\n", ch );
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
      int lose = number_range(level / 2,3 * level / 2);
      lose *= -1;
      victim->gain_exp(lose);
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
   static const short dam_each[] = {
      0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 30, 35, 40, 45, 50, 55,
      60, 65, 70, 75, 80, 82, 84, 86, 88, 90,
      92, 94, 96, 98, 100, 102, 104, 106, 108, 110,
      112, 114, 116, 118, 120, 122, 124, 126, 128, 130
   };
   int dam;

   level = UMIN( level, (int)sizeof( dam_each ) / (int)sizeof( dam_each[0] ) - 1 );
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
   send_to_char( "You are surrounded by a pink outline.\r\n", victim );
   act( "$n is surrounded by a pink outline.", victim, NULL, NULL, TO_ROOM );
   return TRUE;
}



bool spell_faerie_fog( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *ich;

   if( obj == NULL )
   {
      act( "$n conjures a cloud of purple smoke.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You conjure a cloud of purple smoke.\r\n", ch );
   }
   else
   {
      act( "A cloud of purple smoke flows from $p.", ch, obj, NULL, TO_ROOM );
      act( "A cloud of purple smoke flows from $p.", ch, obj, NULL, TO_ROOM );
   }
   for( ich = ch->in_room->first_person; ich != NULL; ich = ich->next_in_room )
   {
      if( !IS_NPC( ich ) && ich->act.test(ACT_WIZINVIS ) )
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
      send_to_char( "You are revealed!\r\n", ich );
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
   send_to_char( "Your feet rise off the ground.\r\n", victim );
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
   send_to_char( "You feel stronger.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
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
      send_to_char( "You feel very sick.\r\n", victim );
   }
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}



bool spell_heal( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   victim->hit = UMIN( victim->hit + 100 + dice( 30, level / 4 ), victim->max_hit );
   update_pos( victim );
   send_to_char( "A warm feeling fills your body.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   spell_cure_poison( sn, level, ch, vo, obj );

   return TRUE;
}


bool spell_influx( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
/* -- Stephen */
{
   CHAR_DATA *vch;


   if( obj == NULL )
   {
      send_to_char( "You fill the room with healing energy!\r\n", ch );
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

   dam = number_range( (int)(level / 2), (int)(level * 1.5) );
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

   snprintf( buf, MSL,
            "@@NObject '%s' is @@etype@@N %s, @@aextra flags@@N %s.\r\n@@mWorn@@N: %s, @@cWeight@@N: %d, @@lDurability@@N: %d/%d (%1.0f%%), @@yvalue@@N: %s @@N, @@rlevel@@N: %d.\r\n",
            ob->short_descr,
            item_type_name( ob ),
            extra_bit_name( ob->extra_flags ),
            wear_bit_name( ob->wear_flags ),
            ob->weight,
            ob->durability == 1 ? 0 : ob->durability, ob->max_durability,
            ob->durability == 1 ? 0 : (float)(((float)ob->durability / (float)ob->max_durability) * 100), cost_to_money( ob->cost ), ob->level );
   send_to_char( buf, ch );

   switch ( ob->item_type )
   {
      case ITEM_SCROLL:
      case ITEM_POTION:
         snprintf( buf, MSL, "Level %d spells of:", ob->value[0] );
         send_to_char( buf, ch );

         if( ob->value[1] > 0 && ob->value[1] < MAX_SKILL )
         {
            send_to_char( " '", ch );
            send_to_char( skill_table[ob->value[1]].name, ch );
            send_to_char( "'", ch );
         }

         if( ob->value[2] > 0 && ob->value[2] < MAX_SKILL )
         {
            send_to_char( " '", ch );
            send_to_char( skill_table[ob->value[2]].name, ch );
            send_to_char( "'", ch );
         }

         if( ob->value[3] > 0 && ob->value[3] < MAX_SKILL )
         {
            send_to_char( " '", ch );
            send_to_char( skill_table[ob->value[3]].name, ch );
            send_to_char( "'", ch );
         }

         send_to_char( ".\r\n", ch );
         break;

      case ITEM_WAND:
      case ITEM_STAFF:
         snprintf( buf, MSL, "Has %d(%d) charges of level %d", ob->value[1], ob->value[2], ob->value[0] );
         send_to_char( buf, ch );

         if( ob->value[3] >= 0 && ob->value[3] < MAX_SKILL )
         {
            send_to_char( " '", ch );
            send_to_char( skill_table[ob->value[3]].name, ch );
            send_to_char( "'", ch );
         }

         send_to_char( ".\r\n", ch );
         break;

      case ITEM_WEAPON:
        snprintf( buf, MSL, "Damage is %d to %d (average %d). Speed is %4.2f (%4.2f DPS).\r\n",
            ob->value[1], ob->value[2] , (ob->value[1] + ob->value[2]) / 2, ob->speed, ((ob->value[1] + ob->value[2])/2) / ob->speed);
         send_to_char(buf,ch);
         break;

      case ITEM_ARMOR:
         snprintf( buf, MSL, "Armor class is %d.\r\n", ob->value[0] );
         send_to_char( buf, ch );
         break;
   }

   for( paf = ob->first_apply; paf != NULL; paf = paf->next )
   {
      if( paf->location != APPLY_NONE && paf->modifier != 0 )
      {
         snprintf( buf, MSL, "Affects %s by %d.\r\n", affect_loc_name( paf->location ), paf->modifier );
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
   send_to_char( "Your eyes glow red.\r\n", victim );
   act( "$n's eyes glow red.", victim, NULL, NULL, TO_ROOM );
   return TRUE;
}

bool spell_invis( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim;
   OBJ_DATA *ob;
   AFFECT_DATA af;

   if( target_name[0] == '\0' )
   {
      send_to_char( "Make who or what invisible?\r\n", ch );
      return FALSE;
   }


   if( ( victim = get_char_room( ch, target_name ) ) != NULL )
   {
      if( IS_AFFECTED( victim, AFF_INVISIBLE ) || item_has_apply( victim, ITEM_APPLY_INV ) )
         return ( ch == victim ? FALSE : TRUE );

      act( "$n fades out of existence.", victim, NULL, NULL, TO_ROOM );
      af.type = sn;
      af.duration = 4 + ( level / 5 );
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = AFF_INVISIBLE;
      affect_to_char( victim, &af );
      send_to_char( "You fade out of existence.\r\n", victim );
      return TRUE;
   }
   else if( ( ob = get_obj_carry( ch, target_name ) ) != NULL )
   {
      if( !IS_OBJ_STAT(ob, ITEM_EXTRA_INVIS ) )
      {
         SET_BIT( ob->extra_flags, ITEM_EXTRA_INVIS );
         act( "$p shimmers out of visibility.", ch, ob, NULL, TO_CHAR );
         act( "$p shimmers out of visibility.", ch, ob, NULL, TO_ROOM );
         return TRUE;
      }
      else
      {
         act( "$p is already invisible!", ch, ob, NULL, TO_CHAR );
         return FALSE;
      }
   }
   else
   {
      send_to_char( "Couldn't find that person or object.\r\n", ch );
      return FALSE;
   }
   return FALSE;
}


bool spell_know_alignment( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   char *msg;
   int ap;

   ap = victim->alignment;

   if( ap > 700 )
      msg = "$N has an aura as white as the driven snow.";
   else if( ap > 350 )
      msg = "$N is of excellent moral character.";
   else if( ap > 100 )
      msg = "$N is often kind and thoughtful.";
   else if( ap > -100 )
      msg = "$N doesn't have a firm moral commitment.";
   else if( ap > -350 )
      msg = "$N lies to $S friends.";
   else if( ap > -700 )
      msg = "$N's slash DISEMBOWELS you!";
   else
      msg = "I'd rather just not say anything at all about $N.";

   act( msg, ch, NULL, victim, TO_CHAR );
   return TRUE;
}



bool spell_lightning_bolt( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   static const short dam_each[] = {
      0,
      0, 0, 0, 0, 0, 0, 0, 0, 25, 28,
      31, 34, 37, 40, 40, 41, 42, 42, 43, 44,
      44, 45, 46, 46, 47, 48, 48, 49, 50, 50,
      51, 52, 52, 53, 54, 54, 55, 56, 56, 57,
      58, 58, 59, 60, 60, 61, 62, 62, 63, 64
   };
   int dam;

   level = UMIN( level, (int)sizeof( dam_each ) / (int)sizeof( dam_each[0] ) - 1 );
   level = UMAX( 0, level );
   dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
   if( saves_spell( level, victim ) )
      dam /= 2;

   sp_damage( obj, ch, victim, dam, REALM_SHOCK, sn, TRUE );
   return TRUE;
}



bool spell_locate_object( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   char buf[MSL];
   OBJ_DATA *ob;
   OBJ_DATA *in_obj;
   bool found;
   list<OBJ_DATA *>::iterator li;

   found = FALSE;
   for( li = obj_list.begin(); li != obj_list.end(); li++ )
   {
      ob = *li;
      if( !can_see_obj( ch, ob ) || !is_name( target_name, ob->name )
          || IS_OBJ_STAT(ob,ITEM_EXTRA_RARE)
          || ( ob->item_type == ITEM_PIECE )
          || ( IS_OBJ_STAT(ob,ITEM_EXTRA_UNIQUE) ) || ( !str_prefix( target_name, "unique" ) ) )
         continue;



      for( in_obj = ob; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
         ;
      if( ( in_obj->carried_by != NULL )
          && ( IS_IMMORTAL( in_obj->carried_by )
               || ( !IS_NPC( in_obj->carried_by )
                    && IS_WOLF( in_obj->carried_by )
                    && ( IS_SHIFTED( in_obj->carried_by ) || ( IS_RAGED( in_obj->carried_by ) ) ) ) ) )
         break;

      if( in_obj->carried_by != NULL )
      {
         found = TRUE;
         snprintf( buf, MSL, "%s carried by %s.\r\n",
                  ob->short_descr, in_obj->carried_by->get_name(ch) );
      }
      else
      {
         found = TRUE;
         snprintf( buf, MSL, "%s in %s.\r\n", ob->short_descr, in_obj->in_room == NULL ? "somewhere" : in_obj->in_room->name );
      }

      buf[0] = UPPER( buf[0] );
      send_to_char( buf, ch );
   }

   if( !found )
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );

   return TRUE;
}



bool spell_magic_missile( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int cnt;
   int hits;
   static const short dam_each[] = {
      0,
      3, 3, 4, 4, 5, 6, 6, 6, 6, 6,
      7, 7, 7, 7, 7, 8, 8, 8, 8, 8,
      9, 9, 9, 9, 9, 10, 10, 10, 10, 10,
      11, 11, 11, 11, 11, 12, 12, 12, 12, 12,
      13, 13, 13, 13, 13, 14, 14, 14, 14, 14
   };
   int dam;

   level = UMIN( level, (int)sizeof( dam_each ) / (int)sizeof( dam_each[0] ) - 1 );
   level = UMAX( 0, level );
   dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
   if( saves_spell( level, victim ) )
      dam /= 2;
   cnt = 1 + ( level >= 30 ) + ( level >= 60 ) + ( level >= 80 );
   for( hits = 0; hits < cnt; hits++ )
   {
      sp_damage( obj, ch, victim, dam, REALM_IMPACT, sn, TRUE );

   }

   return TRUE;
}



bool spell_mass_invis( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   AFFECT_DATA af;
   CHAR_DATA *gch;

   for( gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room )
   {
      if( IS_AFFECTED( gch, AFF_INVISIBLE ) || item_has_apply( gch, ITEM_APPLY_INV ) )
         continue;
      act( "$n slowly fades out of existence.", gch, NULL, NULL, TO_ROOM );
      send_to_char( "You slowly fade out of existence.\r\n", gch );
      af.type = sn;
      af.duration = 4 + ( level / 5 );
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = AFF_INVISIBLE;
      affect_to_char( gch, &af );
   }
   send_to_char( "Ok.\r\n", ch );

   return TRUE;
}



bool spell_null( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   send_to_char( "That's not a spell!\r\n", ch );
   return FALSE;
}



bool spell_pass_door( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_PASS_DOOR ) || item_has_apply( victim, ITEM_APPLY_PASS_DOOR ) )
      return FALSE;
   af.type = sn;
   af.duration = 2 + ( level / 20 );
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.bitvector = AFF_PASS_DOOR;
   affect_to_char( victim, &af );
   act( "$n turns translucent.", victim, NULL, NULL, TO_ROOM );
   send_to_char( "You turn translucent.\r\n", victim );
   return TRUE;
}



bool spell_poison( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( saves_spell( level, victim ) )
      return TRUE;
   af.type = sn;
   af.duration = 12 + ( level / 10 );
   af.location = APPLY_STR;
   af.modifier = -2;
   af.bitvector = AFF_POISON;
   affect_join( victim, &af );
   send_to_char( "You feel very sick.\r\n", victim );
   act( "$n looks very sick.", victim, NULL, NULL, TO_ROOM );
   return TRUE;
}



bool spell_protection( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_PROTECT ) || item_has_apply( victim, ITEM_APPLY_PROT ) )
      return ( ch == victim ? FALSE : TRUE );
   af.type = sn;
   af.duration = 8 + ( level / 5 );
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.bitvector = AFF_PROTECT;
   affect_to_char( victim, &af );
   send_to_char( "You feel protected.\r\n", victim );
   act( "$n looks better protected.", victim, NULL, NULL, TO_ROOM );
   return TRUE;
}



bool spell_refresh( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   if( ch->carry_weight >= can_carry_w( ch ) )
   {
      send_to_char( "That's not gonna help, you are carrying too much wieght!\r\n", ch );
      return FALSE;
   }
   victim->move = UMIN( victim->move + level, victim->max_move );
   send_to_char( "You feel less tired.\r\n", victim );
   act( "$n looks less tired.", victim, NULL, NULL, TO_ROOM );
   return TRUE;
}



bool spell_remove_curse( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   if( is_affected( victim, gsn_curse ) )
   {
      affect_strip( victim, gsn_curse );
      send_to_char( "You feel better.\r\n", victim );
      act( "$n looks more Holy.", victim, NULL, NULL, TO_ROOM );
   }

   return TRUE;
}



bool spell_sanctuary( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_SANCTUARY ) || item_has_apply( victim, ITEM_APPLY_SANC ) )
      return ( ch == victim ? FALSE : TRUE );
   af.type = sn;
   af.duration = 5 + ( level / 20 );
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.bitvector = AFF_SANCTUARY;
   affect_to_char( victim, &af );
   act( "$n is surrounded by a white aura.", victim, NULL, NULL, TO_ROOM );
   send_to_char( "You are surrounded by a white aura.\r\n", victim );
   return TRUE;
}



bool spell_sense_evil( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
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
   send_to_char( "Your eyes tingle.\r\n", victim );
   act( "$n's eyes glow briefly.", victim, NULL, NULL, TO_ROOM );
   return TRUE;
}



bool spell_shield( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, sn ) )
      return FALSE;
   af.type = sn;
   af.duration = 4 + ( level / 5 );
   af.location = APPLY_AC;
   af.modifier = -20 - ch->get_level("psuedo") / 5;
   af.bitvector = 0;
   affect_to_char( victim, &af );
   act( "$n is surrounded by a force shield.", victim, NULL, NULL, TO_ROOM );
   send_to_char( "You are surrounded by a force shield.\r\n", victim );
   return TRUE;
}



bool spell_shocking_grasp( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   static const int dam_each[] = {
      0,
      0, 0, 0, 0, 0, 0, 20, 25, 29, 33,
      36, 39, 39, 39, 40, 40, 41, 41, 42, 42,
      43, 43, 44, 44, 45, 45, 46, 46, 47, 47,
      48, 48, 49, 49, 50, 50, 51, 51, 52, 52,
      53, 53, 54, 54, 55, 55, 56, 56, 57, 57
   };
   int dam;

   level = UMIN( level, (int)sizeof( dam_each ) / (int)sizeof( dam_each[0] ) - 1 );
   level = UMAX( 0, level );
   dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
   if( saves_spell( level, victim ) )
      dam /= 2;
   sp_damage( obj, ch, victim, dam, REALM_SHOCK, sn, TRUE );
   return TRUE;
}



bool spell_sleep( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_SLEEP ) || level < victim->level || saves_spell( level, victim ) )
      return TRUE;

   af.type = sn;
   af.duration = 4 + ( level / 15 );
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.bitvector = AFF_SLEEP;
   affect_join( victim, &af );

   if( IS_AWAKE( victim ) )
   {
      send_to_char( "You feel very sleepy ..... zzzzzz.\r\n", victim );

      /*
       * will stop the char from attackin' right away (4-24-96)
       * -Damane- 
       */
      if( victim->position == POS_FIGHTING )
         stop_fighting( victim, TRUE );
      do_sleep( victim, "" );
   }

   return TRUE;
}



bool spell_stone_skin( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( ch, sn ) || is_affected( ch, skill_lookup( "stone skin" ) ) )
      return FALSE;
   af.type = sn;
   af.duration = 5 + ( level / 12 );
   af.location = APPLY_AC;
   af.modifier = -40 - ch->get_level("psuedo") / 10;
   af.bitvector = 0;
   affect_to_char( victim, &af );
   act( "$n's skin turns to stone.", victim, NULL, NULL, TO_ROOM );
   send_to_char( "Your skin turns to stone.\r\n", victim );
   return TRUE;
}



bool spell_summon( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];

   if( ( victim = get_char_world( ch, target_name ) ) == NULL
       || victim == ch
       || victim->in_room == NULL
       || victim->in_room->room_flags.test(RFLAG_SAFE)
       || victim->in_room->room_flags.test(RFLAG_NO_RECALL)
       || !victim->in_room->area->flags.test(AFLAG_TELEPORT)
       || victim->level >= level + 10
       || victim->fighting != NULL
       || ( IS_NPC( victim ) && saves_spell( level, victim ) )
       || room_is_private( ch->in_room ) || ( ch->in_room->area->max_level < ( victim->level - 15 ) ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return TRUE;
   }
   if( ( victim->act.test(ACT_NO_SUMMON ) ) || ( IS_NPC( victim ) && ( victim->level > ( level - 21 ) ) ) )
   {
      send_to_char( "You seemed unable to snatch your victim!\r\n", ch );
      send_to_char( "You feel a slight tugging sensation.\r\n", victim );
      return TRUE;
   }

   act( "$n disappears suddenly.", victim, NULL, NULL, TO_ROOM );
   char_from_room( victim );
   char_to_room( victim, ch->in_room );
   act( "$n arrives suddenly.", victim, NULL, NULL, TO_ROOM );
   snprintf( buf, MSL, "%s has summoned you!!", ch->name.c_str() );
   send_to_char( buf, victim );
   do_look( victim, "auto" );
   return TRUE;
}



bool spell_teleport( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   ROOM_INDEX_DATA *pRoomIndex;

   if( deathmatch )
   {
      send_to_char( "Not during a @@eDeath Match@@N!!\r\n", ch );
      return FALSE;
   }
   if( victim->in_room == NULL
       || victim->in_room->room_flags.test(RFLAG_NO_RECALL)
       || ( !IS_NPC( ch ) && victim->fighting != NULL )
       || ( victim != ch && ( saves_spell( level, victim ) || saves_spell( level, victim ) ) ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return TRUE;
   }

   for( ;; )
   {
      pRoomIndex = get_room_index( number_range( 0, 32767 ) );
      if( pRoomIndex == NULL )
         continue;
      if( !pRoomIndex->room_flags.test(RFLAG_PRIVATE)
          && !pRoomIndex->room_flags.test(RFLAG_SOLITARY) && pRoomIndex->area->flags.test(AFLAG_TELEPORT) )
         break;
   }

   act( "$n slowly fades out of existence.", victim, NULL, NULL, TO_ROOM );
   char_from_room( victim );
   char_to_room( victim, pRoomIndex );
   act( "$n slowly fades into existence.", victim, NULL, NULL, TO_ROOM );
   do_look( victim, "auto" );
   return TRUE;

}



bool spell_ventriloquate( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   char buf1[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   char speaker[MAX_INPUT_LENGTH];
   CHAR_DATA *vch;

   target_name = one_argument( target_name, speaker );

   snprintf( buf1, MSL, "%s says '%s'.\r\n", speaker, target_name );
   snprintf( buf2, MSL, "Someone makes %s say '%s'.\r\n", speaker, target_name );
   buf1[0] = UPPER( buf1[0] );

   for( vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room )
   {
      if( !is_name( speaker, const_cast<char *>(vch->name.c_str()) ) )
         send_to_char( saves_spell( level, vch ) ? buf2 : buf1, vch );
   }

   return TRUE;
}

bool spell_weaken( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, sn ) || saves_spell( level, victim ) )
      return TRUE;
   af.type = sn;
   af.duration = 1 + ( level / 16 );
   af.location = APPLY_STR;
   af.modifier = -2;
   af.bitvector = 0;
   affect_to_char( victim, &af );
   send_to_char( "You feel weaker.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}



/*
 * For Ack! : Level 66 Priest spell - allow 100% recall.
 */
bool spell_word_of_recall( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   ROOM_INDEX_DATA *location;

   act( "$n requests Holy transport!", victim, NULL, NULL, TO_ROOM );
   if( IS_AFFECTED( victim, AFF_CURSE ) )
   {
      send_to_char( "The Gods do not transport cursed players!\r\n", ch );
      return FALSE;
   }

   if( IS_NPC( victim ) || victim->level < 6 )
      location = get_room_index( ROOM_VNUM_TEMPLE );
   else
      location = get_room_index( race_table[victim->race].recall );

   if( location == NULL )
   {
      send_to_char( "You are completely lost.\r\n", victim );
      return FALSE;
   }

   if( victim->in_room == location )
      return FALSE;

   if( victim->in_room->room_flags.test(RFLAG_NO_RECALL) )
   {
      send_to_char( "Some strange force prevents your transport.\r\n", victim );
      return TRUE;
   }

   if( victim->fighting != NULL )
      stop_fighting( victim, TRUE );

   act( "$n is engulfed by a stream of blue light!", victim, NULL, NULL, TO_ROOM );
   char_from_room( victim );
   char_to_room( victim, location );
   act( "$n arrives, carried by a blue stream of light!", victim, NULL, NULL, TO_ROOM );
   do_look( victim, "auto" );
   return TRUE;
}



/*
 * NPC spells.
 */
bool spell_acid_breath( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   /*
    * Fixed the nice bug that referenced the _caster's_ equipment,
    * * so that it checks the victim's instead *laugh* -S-
    */

   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   OBJ_DATA *obj_lose;
   OBJ_DATA *obj_next;
   int dam;
   int hpch;

   if( number_percent(  ) < 2 * level && !saves_spell( level, victim ) )
   {
      for( obj_lose = victim->first_carry; obj_lose != NULL; obj_lose = obj_next )
      {
         int iWear;

         obj_next = obj_lose->next_in_carry_list;

         if( number_bits( 2 ) != 0 )
            continue;

         switch ( obj_lose->item_type )
         {
            case ITEM_ARMOR:
               if( obj_lose->value[0] > 0 )
               {
                  act( "$p is pitted and etched!", victim, obj_lose, NULL, TO_CHAR );
                  if( ( iWear = obj_lose->wear_loc ) != WEAR_NONE )
                     victim->armor -= apply_ac( obj_lose, iWear );
                  obj_lose->value[0] -= 1;
                  obj_lose->cost = 0;
                  if( iWear != WEAR_NONE )
                     victim->armor += apply_ac( obj_lose, iWear );
               }
               break;

            case ITEM_CONTAINER:

            {
               OBJ_DATA *content;
               OBJ_DATA *content_next;
               act( "$p fumes and dissolves!", victim, obj_lose, NULL, TO_CHAR );
               for( content = obj_lose->first_in_carry_list; content; content = content_next )
               {
                  content_next = content->next_in_carry_list;
                  obj_from_obj( content );
                  obj_to_room( content, victim->in_room != NULL ? victim->in_room : get_room_index( ROOM_VNUM_LIMBO ) );
               }
               extract_obj( obj_lose );
               break;
            }
         }
      }

   }

   hpch = UMAX( 10, ch->hit );
   dam = number_range( hpch / 16 + 1, hpch / 8 );
   if( saves_spell( level, victim ) )
      dam /= 2;
   sp_damage( obj, ch, victim, dam, REALM_ACID | NO_REFLECT | NO_ABSORB, sn, TRUE );
   return TRUE;
}



bool spell_fire_breath( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   OBJ_DATA *obj_lose;
   OBJ_DATA *obj_next;
   int dam;
   int hpch;


   if( !IS_NPC( ch ) )
   {
      dam = number_range( (int)(ch->get_level("psuedo") * 1.2), (int)(ch->get_level("psuedo") * 1.6) );
      if( saves_spell( level, victim ) )
         dam /= 2;
      damage( ch, victim, dam, sn );
      return TRUE;
   }

   if( number_percent(  ) < 2 * level && !saves_spell( level, victim ) )
   {
      for( obj_lose = victim->first_carry; obj_lose != NULL; obj_lose = obj_next )
      {
         char *msg;

         obj_next = obj_lose->next_in_carry_list;
         if( number_bits( 2 ) != 0 )
            continue;

         switch ( obj_lose->item_type )
         {
            default:
               continue;
            case ITEM_CONTAINER:
               msg = "$p ignites and burns!";
               break;
            case ITEM_POTION:
               msg = "$p bubbles and boils!";
               break;
            case ITEM_SCROLL:
               msg = "$p crackles and burns!";
               break;
            case ITEM_STAFF:
               msg = "$p smokes and chars!";
               break;
            case ITEM_WAND:
               msg = "$p sparks and sputters!";
               break;
            case ITEM_FOOD:
               msg = "$p blackens and crisps!";
               break;
            case ITEM_PILL:
               msg = "$p melts and drips!";
               break;
         }

         act( msg, victim, obj_lose, NULL, TO_CHAR );
         if( obj_lose->item_type == ITEM_CONTAINER )
         {
            OBJ_DATA *content;
            OBJ_DATA *content_next;

            for( content = obj_lose->first_in_carry_list; content; content = content_next )
            {
               content_next = content->next_in_carry_list;
               obj_from_obj( content );
               obj_to_room( content, victim->in_room != NULL ? victim->in_room : get_room_index( ROOM_VNUM_LIMBO ) );
            }
            extract_obj( obj_lose );
         }
      }
   }

   hpch = UMAX( 10, ch->hit );
   dam = number_range( hpch / 16 + 1, hpch / 8 );
   if( saves_spell( level, victim ) )
      dam /= 2;
   sp_damage( obj, ch, victim, dam, REALM_FIRE | NO_REFLECT | NO_ABSORB, sn, TRUE );
   return TRUE;
}



bool spell_frost_breath( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   OBJ_DATA *obj_lose;
   OBJ_DATA *obj_next;
   int dam;
   int hpch;

   if( number_percent(  ) < 2 * level && !saves_spell( level, victim ) )
   {
      for( obj_lose = victim->first_carry; obj_lose != NULL; obj_lose = obj_next )
      {
         char *msg;

         obj_next = obj_lose->next_in_carry_list;
         if( number_bits( 2 ) != 0 )
            continue;

         switch ( obj_lose->item_type )
         {
            default:
               continue;
            case ITEM_CONTAINER:
            case ITEM_DRINK_CON:
            case ITEM_POTION:
               msg = "$p freezes and shatters!";
               break;
         }

         act( msg, victim, obj_lose, NULL, TO_CHAR );
         if( obj_lose->item_type == ITEM_CONTAINER )
         {
            OBJ_DATA *content;
            OBJ_DATA *content_next;

            for( content = obj_lose->first_in_carry_list; content; content = content_next )
            {
               content_next = content->next_in_carry_list;
               obj_from_obj( content );
               obj_to_room( content, victim->in_room != NULL ? victim->in_room : get_room_index( ROOM_VNUM_LIMBO ) );
            }
         }
      }
   }

   hpch = UMAX( 10, ch->hit );
   dam = number_range( hpch / 16 + 1, hpch / 8 );
   if( saves_spell( level, victim ) )
      dam /= 2;
   sp_damage( obj, ch, victim, dam, REALM_COLD | NO_REFLECT | NO_ABSORB, sn, TRUE );
   return TRUE;
}



bool spell_gas_breath( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int dam;
   int hpch;

   for( vch = ch->in_room->first_person; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
      if( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) )
      {
         hpch = UMAX( 10, ch->hit );
         dam = number_range( hpch / 16 + 1, hpch / 8 );
         if( saves_spell( level, vch ) )
            dam /= 2;
         sp_damage( obj, ch, vch, dam, REALM_GAS | NO_REFLECT | NO_ABSORB, sn, TRUE );
      }
   }
   return TRUE;
}



bool spell_lightning_breath( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;
   int hpch;

   hpch = UMAX( 10, ch->hit );
   dam = number_range( hpch / 16 + 1, hpch / 8 );
   if( saves_spell( level, victim ) )
      dam /= 2;
   sp_damage( obj, ch, victim, dam, REALM_SHOCK | NO_REFLECT | NO_ABSORB, sn, TRUE );
   return TRUE;
}


bool spell_hellspawn( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   /*
    * High level mag / psi spell. -S- 
    */
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;

   dam = number_range( level / 2, level * 5 / 2 );
   if( saves_spell( level, victim ) )
      dam /= 2;

   sp_damage( obj, ch, victim, dam, REALM_DRAIN, sn, TRUE );
   return TRUE;
}


bool spell_planergy( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   /*
    * Psi spell.  Work out which energy to summon, according to
    * * ch's level.  Each 9 levels, the Psi gets the use of a different
    * * spell, and more damage! ;)  (planergy = planar energy)
    * * --Stephen
    */


   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;
   int lvl;
   int plane;

   if( level <= 8 )
   {
      lvl = 4;
      plane = REALM_IMPACT;
   }
   else if( level <= 17 )
   {
      lvl = 13;
      plane = REALM_GAS;
   }
   else if( level <= 26 )
   {
      lvl = 22;
      plane = REALM_FIRE;
   }
   else if( level <= 35 )
   {
      lvl = 31;
      plane = REALM_LIGHT;
   }
   else if( level <= 44 )
   {
      lvl = 40;
      plane = REALM_SHOCK;
   }
   else if( level <= 53 )
   {
      lvl = 49;
      plane = REALM_SOUND;
   }
   else if( level <= 62 )
   {
      lvl = 58;
      plane = REALM_MIND;
   }
   else if( level <= 71 )
   {
      lvl = 67;
      plane = REALM_ACID;
   }
   else
   {
      lvl = 80;
      plane = REALM_POISON;
   }

   dam = dice( 6, lvl / 2 );

   if( saves_spell( level, victim ) )
      dam /= 2;
   SET_BIT( plane, NO_REFLECT );
   SET_BIT( plane, NO_ABSORB );

   sp_damage( obj, ch, victim, dam, plane, sn, TRUE );

   return TRUE;
}


bool spell_visit( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   /*
    * Psi spell.  Kinda like summon, but almost done in reverse.
    * * allows the Psi to try and transfer themself to the 'victim'
    * * --Stephen
    */

   CHAR_DATA *victim = ( CHAR_DATA * ) vo;

   if( deathmatch )
   {
      send_to_char( "Not during a @@eDeath Match@@N!!\r\n", ch );
      return FALSE;
   }
   if( ( victim = get_char_world( ch, target_name ) ) == NULL
       || victim == ch
       || IS_NPC( victim )
       || victim->in_room == NULL
       || victim->in_room->room_flags.test(RFLAG_PRIVATE)
       || victim->in_room->room_flags.test(RFLAG_SOLITARY)
       || victim->in_room->room_flags.test(RFLAG_SAFE)
       || ch->in_room->room_flags.test(RFLAG_NO_RECALL) || !victim->in_room->area->flags.test(AFLAG_TELEPORT) )
   {
      send_to_char( "You failed.\r\n", ch );
      return TRUE;
   }

   if( victim->act.test(ACT_NO_VISIT ) )
   {
      send_to_char( "You seem unable to visit your target!\r\n", ch );
      return TRUE;
   }

   /*
    * Check is ch screws up, and ends up in limbo... <grin> 
    */

   if( number_percent(  ) < 25 ) /* 25% chance */
   {
      send_to_char( "You get distracted, and appear in the middle of nowhere!\r\n", ch );
      act( "$n disappears suddenly.", ch, NULL, NULL, TO_ROOM );
      char_from_room( ch );
      char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );
      act( "$n arrives suddenly.", ch, NULL, NULL, TO_ROOM );
      do_look( ch, "auto" );
      return TRUE;
   }

   act( "$n disappears suddenly.", ch, NULL, NULL, TO_ROOM );
   char_from_room( ch );
   char_to_room( ch, victim->in_room );
   act( "$n arrives suddenly.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "You change locations!\r\n", ch );
   do_look( ch, "auto" );
   return TRUE;
}


bool spell_barrier( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   /*
    * Psi spell, like shield, but slightly better.  Good idea to make
    * * sure Psi's don't have access to shield as well as this... ;)
    * * -- Stephen
    */

   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, sn ) )
      return FALSE;

   af.type = sn;
   af.duration = 4 + ( level / 20 );
   af.location = APPLY_AC;
   af.modifier = -20 - ch->get_level("psuedo") / 10;
   af.bitvector = 0;
   affect_to_char( victim, &af );

   act( "$n is surrounded by a telekinetic barrier.", victim, NULL, NULL, TO_ROOM );
   send_to_char( "You are surrounded by a telekinetic barrier.\r\n", victim );
   return TRUE;
}


bool spell_static( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   /*
    * Psi Spell.
    * * Uses ch's movement, which is transfered to static electricity    
    * * which is then chanelled at the poor victim <laugh>
    * * --Stephen
    */

   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;
   int loss;

   send_to_char( "You transfer kinetic to static energy!\r\n", ch );

   loss = dice( 2, level ) + dice( 2, level );

   dam = number_range( 6, 8 ) * ( level / 3 );
   if( ( ch->move - loss ) <= 0 )
   {
      send_to_char( "You have no kinetic energy left!!\r\n", ch );
      return FALSE;
   }

   ch->move = UMAX( 1, ch->move - loss );

   if( saves_spell( level, victim ) )
      dam /= 2;

   act( "$n transfers kinetic to static energy.", ch, NULL, NULL, TO_ROOM );
   sp_damage( obj, ch, victim, dam, REALM_SHOCK | NO_REFLECT | NO_ABSORB, sn, TRUE );

   return TRUE;
}

bool spell_phobia( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   /*
    * Psi Spell.
    * * Conjures victims's phobia in their mind, does dam (small) or
    * * causes them to flee.
    * * --Stephen
    */

   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;

   if( ( victim->level ) > ( level + 8 ) )
   {
      send_to_char( "Your spell fails to take affect!\r\n", ch );
      return TRUE;
   }

   /*
    * figure out dam.... not too much as spell is meant to make vo flee 
    */

   dam = dice( 2, level / 8 );
   if( obj == NULL )
   {
      act( "$n projects nightmare images into $N's mind!", ch, NULL, victim, TO_NOTVICT );
      act( "You project nightmare images into $N's mind!", ch, NULL, victim, TO_CHAR );
      act( "$N projects nightmare images into your mind!", victim, NULL, ch, TO_CHAR );
   }
   else
   {
      act( "$p projects nightmare images into $n's mind!", victim, obj, NULL, TO_ROOM );
      act( "$p projects nightmare images into your mind!", victim, obj, NULL, TO_CHAR );
   }
   send_to_char( "Your worst phobia springs to life in your mind. Arrrggghhh!\r\n", victim );
   act( "$N suffers a mental phobia attack!", ch, NULL, victim, TO_NOTVICT );

   if( sp_damage( obj, ch, victim, dam, REALM_MIND, sn, TRUE ) )
   {
      if( !IS_NPC( victim ) && ( IS_WOLF( victim ) || IS_VAMP( victim ) ) )
         return TRUE;

      if( number_percent(  ) < 70 ) /* 70% chance */
      {
         act( "$N screams at $n in horror!!", ch, NULL, victim, TO_ROOM );
         act( "$N screams at you in horror!!", ch, NULL, victim, TO_CHAR );
         send_to_char( "You flip, and look for escape!!\r\n", victim );
         do_flee( victim, "" );
      }
   }

   return TRUE;
}

bool spell_mindflame( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   /*
    * Psi Multiple Attack - screws up all those affected 
    */
   CHAR_DATA *vch;
   list<CHAR_DATA *>::iterator li;

   if( obj == NULL )
   {
      send_to_char( "You initiate a mindflame attack!!\r\n", ch );
      act( "$n concentrates, and initiates a mindlame attack!", ch, NULL, NULL, TO_ROOM );
   }
   else
   {
      act( "$p glows, and initiates a mindflame attack!", ch, obj, NULL, TO_ROOM );
      act( "$p glows, and initiates a mindflame attack!", ch, obj, NULL, TO_CHAR );
   }

   for( li = ch->in_room->area->player_list.begin(); li != ch->in_room->area->player_list.end(); li++ )
   {
      vch = *li;
      if( vch->in_room == NULL )
         continue;
      if( vch->in_room == ch->in_room )
      {
         if( vch != ch && ( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) ) )
         {
            act( "$n rolls on the floor, clutching $s head in pain!", vch, NULL, NULL, TO_ROOM );
            send_to_char( "You roll on the floor, clutching your head in pain!\r\n", vch );
            sp_damage( obj, ch, vch, ( ch->get_level("psuedo") / 2 ) + dice( 6, 12 ),
                       REALM_MIND | NO_REFLECT | NO_ABSORB, sn, TRUE );
         }
         continue;
      }

      send_to_char( "You notice a slight burning feeling in your mind.\r\n", vch );
   }

   return TRUE;
}

bool spell_chain_lightning( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   /*
    * affects all in a room, can also hit caster <grin> 
    */
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int dam;

   /*
    * Work out starting damage. 
    */
   dam = dice( 10, ( ch->get_level("psuedo") / 2 ) );

   if( obj == NULL )
   {
      send_to_char( "Lightning flashes from your fingers!\r\n", ch );
      act( "$n unleashes lightning from $s fingers!", ch, NULL, NULL, TO_ROOM );
   }
   else
   {
      act( "A lightning bolt flashes from $p!", ch, obj, NULL, TO_ROOM );
      act( "A lightning bolt flashes from $p!", ch, obj, NULL, TO_CHAR );
   }

   for( vch = ch->in_room->first_person; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if( vch != ch && IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) )
      {
         send_to_char( "The lightning bolt strikes you!\r\n", vch );
         act( "The lightning bolt strikes $n!", vch, NULL, NULL, TO_ROOM );
         sp_damage( obj, ch, vch, dam, REALM_SHOCK | NO_REFLECT, sn, TRUE );
         dam = ( 4 * dam / 5 );
      }
   }



   /*
    * Now see if caster gets hit. 
    */

   if( saves_spell( level, ch ) )
   {
      act( "The lightning bolt hits the ground, and is GONE!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "Your lightning bolts hits the ground and is GONE!\r\n", ch );
   }
   else
   {
      act( "The lightning bolt strikes $n and vanishes!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "Your lightning returns, and hits you!\r\n", ch );
      dam = UMIN( level / 2, dam );
      sp_damage( obj, ch, ch, dam, REALM_SHOCK | NO_REFLECT | NO_ABSORB, sn, TRUE );
   }
   return TRUE;
}


bool spell_suffocate( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;

   dam = dice( level / 3, 2 );

   if( ( level + 5 ) > victim->level )
      dam += ( ( ( level + 5 ) - victim->level ) * 2 );
   else
      dam -= ( ( victim->level - level ) * 2 );

   act( "$n chokes and gags!", victim, NULL, NULL, TO_ROOM );
   send_to_char( "You feel your throat squeezed by an invisible force!\r\n", victim );

   sp_damage( obj, ch, victim, dam, REALM_IMPACT, sn, TRUE );  /* -1 = no dam message */
   return TRUE;
}

bool spell_enhance_weapon( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *ob = ( OBJ_DATA * ) vo;
   AFFECT_DATA *paf;

   /*
    * Quick way to stop imms (Bash?) enchanting weapons for players 
    */
   if( IS_IMMORTAL( ch ) && ch->level != 85 )
   {
      send_to_char( "Nothing Happens.\r\n", ch );
      return FALSE;
   }

   if( ob->item_type != ITEM_WEAPON || IS_OBJ_STAT( ob, ITEM_EXTRA_MAGIC ) || ob->first_apply != NULL )
      return TRUE;

   paf = new AFFECT_DATA;
   paf->type = sn;
   paf->duration = 3 + ( level / 4 );
   paf->location = APPLY_HITROLL;
   paf->modifier = 3;
   LINK( paf, ob->first_apply, ob->last_apply, next, prev );

   paf = new AFFECT_DATA;
   paf->duration = 3 + ( level / 4 );
   paf->location = APPLY_DAMROLL;
   paf->modifier = 2;
   LINK( paf, ob->first_apply, ob->last_apply, next, prev );

   act( "$p shines brightly.", ch, ob, NULL, TO_CHAR );
   act( "$p belonging to $n shines brightly.", ch, ob, NULL, TO_ROOM );

   return TRUE;
}

bool spell_bloody_tears( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;

   if( IS_AFFECTED( victim, AFF_BLIND ) || saves_spell( level, victim ) )
      return TRUE;

   act( "$n's eyes start bleeding!", victim, NULL, NULL, TO_ROOM );
   send_to_char( "Your eyes start bleeding!\r\n", victim );
   spell_blindness( skill_lookup( "blindness" ), level, ch, vo, obj );

   sp_damage( obj, ch, victim, ( level / 2 ), REALM_MIND | NO_REFLECT | NO_ABSORB, sn, FALSE ); /* -1 = no dam message */

   return TRUE;
}

bool spell_mind_bolt( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int cnt;
   int foo;
   int dam;


   cnt = ( level >= 12 ) + ( level >= 30 ) + ( level >= 60 ) + ( ch->level >= 75 );
   for( foo = 0; foo < cnt; foo++ )
   {
      if( number_range( 0, 99 ) < 30 )
         continue;
      dam = number_range( level / 4, level * 2 / 3 );
      if( saves_spell( level, victim ) )
         dam /= 2;

      sp_damage( obj, ch, victim, dam, REALM_MIND | NO_REFLECT | NO_ABSORB, sn, TRUE );   /* -1 = no dam message */

   }
   return TRUE;
}

bool spell_nerve_fire( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *vch;
   list<CHAR_DATA *>::iterator li;

   if( obj == NULL )
   {
      send_to_char( "You initiate a Nerve Fire attack!!\r\n", ch );
      act( "$n concentrates, and initiates a Nerve Fire attack!", ch, NULL, NULL, TO_ROOM );
   }
   else
   {
      act( "$p glows with the power of Nerve Fire!", ch, obj, NULL, TO_ROOM );
      act( "$p glows with the power of Nerve Fire!", ch, obj, NULL, TO_CHAR );
   }

   for( li = ch->in_room->area->player_list.begin(); li != ch->in_room->area->player_list.end(); li++ )
   {
      vch = *li;
      if( vch->in_room == NULL )
         continue;
      if( vch->in_room == ch->in_room )
      {
         if( vch != ch && ( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) ) )
         {
            sp_damage( obj, ch, vch, ( level ) + dice( 5, 20 ), REALM_MIND | NO_REFLECT | NO_ABSORB, sn, TRUE );
         }
         continue;
      }

      send_to_char( "You notice a slight burning feeling in your body.\r\n", vch );
   }

   return TRUE;
}

bool spell_fighting_trance( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, sn ) )
      return FALSE;

   af.type = sn;
   af.duration = 6 + ( level / 20 );
   af.location = APPLY_HITROLL;
   af.modifier = ch->get_level("psuedo") / 10;
   af.bitvector = 0;
   affect_to_char( victim, &af );

   af.location = APPLY_SAVING_SPELL;
   af.modifier = 0 - level / 8;
   affect_to_char( victim, &af );

   af.location = APPLY_DAMROLL;
   af.modifier = ch->get_level("psuedo") / 10;
   affect_to_char( victim, &af );

   af.location = APPLY_AC;
   af.modifier = -10 - ch->get_level("psuedo") / 10;
   affect_to_char( victim, &af );
   send_to_char( "You feel much stronger.\r\n", victim );
   act( "$n looks much stronger.", victim, NULL, NULL, TO_ROOM );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}

bool spell_phase( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_PASS_DOOR ) || item_has_apply( victim, ITEM_APPLY_PASS_DOOR ) )
      return FALSE;

   af.type = sn;
   af.duration = 3 + ( level / 20 );
   af.location = APPLY_AC;
   af.modifier = -10 - ch->get_level("psuedo") / 10;
   af.bitvector = AFF_PASS_DOOR;
   affect_to_char( victim, &af );
   send_to_char( "Your body switches phase.\r\n", victim );
   act( "$n's body switches phase.", victim, NULL, NULL, TO_ROOM );
   return TRUE;
}

bool spell_dimension_blade( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *ob = ( OBJ_DATA * ) vo;
   AFFECT_DATA *paf;

   /*
    * Quick way to stop imms (Bash?) enchanting weapons for players 
    */
   if( IS_IMMORTAL( ch ) && ch->level != 85 )
   {
      send_to_char( "Nothing Happens.\r\n", ch );
      return FALSE;
   }

   if( dice( 4, 5 ) == 20 )
   {
      act( "$p shatters into pieces!", ch, ob, NULL, TO_CHAR );
      act( "$p carried by $n shatters!", ch, ob, NULL, TO_ROOM );
      extract_obj( ob );
      return TRUE;
   }

   if( ob->item_type != ITEM_WEAPON || IS_OBJ_STAT( ob, ITEM_EXTRA_MAGIC ) || ob->first_apply != NULL )
      return TRUE;

   paf = new AFFECT_DATA;
   paf->type = sn;
   paf->location = APPLY_HITROLL;
   paf->modifier = 1 + ( level >= 50 ) + ( level >= 60 ) + ( level >= 70 );
   LINK( paf, ob->first_apply, ob->last_apply, next, prev );

   paf = new AFFECT_DATA;
   paf->location = APPLY_DAMROLL;
   paf->modifier = 1 + ( level >= 55 ) + ( level >= 70 );
   LINK( paf, ob->first_apply, ob->last_apply, next, prev );

   act( "Part of $p switches into a different plane.", ch, ob, NULL, TO_CHAR );
   act( "$n makes $p into a dimension blade.", ch, ob, NULL, TO_ROOM );
   return TRUE;
}

bool spell_produce_food( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   OBJ_DATA *mushroom;

   mushroom = create_object( get_obj_index( OBJ_VNUM_FOOD ), 0 );
   mushroom->value[0] = 5 + level;
   obj_to_room( mushroom, ch->in_room );
   act( "$p suddenly appears.", ch, mushroom, NULL, TO_ROOM );
   act( "$p suddenly appears.", ch, mushroom, NULL, TO_CHAR );
   return TRUE;
}

bool spell_animate( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *ob;
   OBJ_DATA *bits;
   CHAR_DATA *corpse;

   if( IS_NPC( ch ) )
      return FALSE;

   ob = get_obj_here( ch, target_name );

   if( ob == NULL )
   {
      send_to_char( "Couldn't find it.\r\n", ch );
      return FALSE;
   }

   if( ch->get_level("necromancer") < 70 )
   {
      send_to_char( " @@eDUE TO A ROLEPLAYING CHANGE, THIS SPELL IS NO LONGER AVAILABLE TO ANY CLASS\r\n", ch );
      send_to_char( " BUT a high level @@dNECRMOANCER@@N.  THE SKILL HAS BEEN REMOVED FROM YOU CHARACTER, \r\n", ch );
      send_to_char( " SORRY FOR THE INCONVIENENCE.  @@mTHANK YOU@@N :)\r\n", ch );
      ch->pcdata->learned[sn] = 0;
      return FALSE;
   }

   if( ob->item_type != ITEM_CORPSE_NPC )
   {
      send_to_char( "You can't animate that!\r\n", ch );
      return FALSE;
   }

   act( "$n lays $s hands onto the $p!", ch, ob, NULL, TO_ROOM );
   act( "You lay your hands upon the $p.", ch, ob, NULL, TO_CHAR );

   act( "Bright bolts of lightning fly out from $p!!", ch, ob, NULL, TO_ROOM );
   act( "Bright bolts of lightning fly out from $p!!", ch, ob, NULL, TO_CHAR );

   corpse = create_mobile( get_mob_index( MOB_VNUM_ZOMBIE ) );
   char_to_room( corpse, ch->in_room );
   act( "$n stands up, and stretches slowly.", corpse, NULL, NULL, TO_ROOM );

   if( ob->item_type == ITEM_CORPSE_NPC )
   {
      corpse->act.set(ACT_UNDEAD);
      act( "$n's eyes glow black!", corpse, NULL, NULL, TO_ROOM );
   }

   corpse->level = ob->level; /* Level of (N)PC before death */
   corpse->max_hit = dice( 5, level );
   corpse->hit = corpse->max_hit;
   corpse->max_move = dice( 10, level );
   corpse->move = corpse->max_move; /* Set Zombie's stats */

   for( ;; )
   {
      if( ob->first_in_carry_list == NULL )
         break;
      bits = ob->first_in_carry_list;
      obj_from_obj( bits );
      obj_to_char( bits, corpse );

   }

   extract_obj( ob );
   do_wear( corpse, "all" );  /* FIXME: better to check items, then wear... */
   corpse->act.set(ACT_PET);
   SET_BIT( corpse->affected_by, AFF_CHARM );
   corpse->npcdata->extract_timer = ch->get_level("psuedo") / 3;

   add_follower( corpse, ch );
   return TRUE;
}

bool spell_see_magic( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_DETECT_MAGIC ) || item_has_apply( victim, ITEM_APPLY_HIDE ) )
      return FALSE;
   af.type = sn;
   af.duration = 6 + ( level / 4 );
   af.modifier = 0;
   af.location = APPLY_NONE;
   af.bitvector = AFF_DETECT_MAGIC;
   affect_to_char( victim, &af );
   send_to_char( "Your eyes tingle.\r\n", victim );
   act( "$n's eyes glow briefly.", victim, NULL, NULL, TO_ROOM );
   return TRUE;
}

bool spell_detection( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   char buf[MSL];
   OBJ_DATA *ob;
   OBJ_DATA *in_obj;
   bool found;
   list<OBJ_DATA *>::iterator li;

   found = FALSE;
   for( li = obj_list.begin(); li != obj_list.end(); li++ )
   {
      ob = *li;
      if( !can_see_obj( ch, ob ) || !is_name( target_name, ob->name )
          || IS_OBJ_STAT(ob,ITEM_EXTRA_RARE)
          || ( ob->item_type == ITEM_PIECE )
          || ( IS_OBJ_STAT(ob,ITEM_EXTRA_UNIQUE) ) || ( !str_prefix( target_name, "unique" ) ) )
         continue;





      for( in_obj = ob; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
         ;
      if( ( in_obj->carried_by != NULL )
          && ( IS_IMMORTAL( in_obj->carried_by )
               || ( !IS_NPC( in_obj->carried_by )
                    && IS_WOLF( in_obj->carried_by )
                    && ( IS_SHIFTED( in_obj->carried_by ) || ( IS_RAGED( in_obj->carried_by ) ) ) ) ) )
         break;


      if( in_obj->carried_by != NULL )
      {
         found = TRUE;
         snprintf( buf, MSL, "%s carried by %s.\r\n",
                  ob->short_descr, in_obj->carried_by->get_name(ch) );
      }
      else
      {
         found = TRUE;
         snprintf( buf, MSL, "%s in %s.\r\n", ob->short_descr, in_obj->in_room == NULL ? "somewhere" : in_obj->in_room->name );
      }

      buf[0] = UPPER( buf[0] );
      send_to_char( buf, ch );
   }

   if( !found )
      send_to_char( "You fail to detect any such object.\r\n", ch );

   return TRUE;
}


bool spell_fire_blade( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *blade;

   if( get_obj_wear( ch, "fireblade" ) != NULL )
      return FALSE;  /* Only have one at a time.. */

   if( remove_obj( ch, WEAR_HOLD_HAND_L, TRUE ) )
   {
      blade = create_object( get_obj_index( OBJ_VNUM_FIREBLADE ), level );
      obj_to_char( blade, ch );
      equip_char( ch, blade, WEAR_HOLD_HAND_L );
      blade->timer = 2 + ( level / 20 );
      act( "A blazing Fire Blade appears in $n's hand!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "A blazing Fire Blade appears in your hand!\r\n", ch );
   }
   return TRUE;
}


bool spell_know_weakness( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   AFFECT_DATA af;

   if( is_affected( ch, sn ) )
      return FALSE;

   af.type = sn;
   af.duration = 2 + ( level >= 20 ) + ( level >= 40 ) + ( level >= 60 ) + ( level >= 80 );
   af.location = APPLY_HITROLL;
   af.modifier = 3;
   af.bitvector = 0;
   affect_to_char( ch, &af );
   send_to_char( "You are more aware of your enemy's weaknesses.\r\n", ch );
   return TRUE;
}

bool spell_know_critical( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   AFFECT_DATA af;

   if( is_affected( ch, sn ) )
      return FALSE;

   af.type = sn;
   af.duration = 2 + ( level >= 20 ) + ( level >= 40 ) + ( level >= 60 ) + ( level >= 80 );
   af.location = APPLY_DAMROLL;
   af.modifier = 3;
   af.bitvector = 0;
   affect_to_char( ch, &af );
   send_to_char( "You are more aware of critical damage points.\r\n", ch );
   return TRUE;
}

bool spell_calm( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *ppl;
   int chance;

   for( ppl = ch->in_room->first_person; ppl != NULL; ppl = ppl->next_in_room )
      if( IS_NPC( ppl ) && ppl != ch )
      {
         chance = ( ( IS_NPC( ch ) ? 50 : ch->pcdata->learned[sn] / 2 ) + ( 5 * ( level - ppl->level ) ) );
         if( number_percent(  ) < chance )
            stop_fighting( ppl, TRUE );
      }
   if( obj == NULL )
   {
      act( "$n emits a great feeling of calm around you.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You emit a great feeling of calm around you.\r\n", ch );
   }
   else
   {
      act( "$p glows with a clam light.", ch, obj, NULL, TO_ROOM );
      act( "$p glows with a clam light.", ch, obj, NULL, TO_CHAR );
   }
   return TRUE;
}


bool spell_hypnosis( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( victim == ch )
   {
      send_to_char( "You like yourself even better!\r\n", ch );
      return FALSE;
   }

   if( !IS_NPC( victim ) )
   {
      send_to_char( "You're not that powerful.\r\n", ch );
      return FALSE;
   }

   if( IS_AFFECTED( victim, AFF_CHARM )
       || IS_AFFECTED( ch, AFF_CHARM ) || level - 5 < victim->level || saves_spell( level, victim ) )
      return TRUE;
   if( IS_VAMP( victim ) && ( IS_NPC( victim ) ) )
   {
      send_to_char( "Wow, it seems to be immune--imagine that!\r\n", ch );
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
      send_to_char( "Ok.\r\n", ch );
   victim->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   return TRUE;
}

bool spell_mind_flail( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   int dam;
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;

   if( victim == ch )
      return FALSE;

   if( IS_NPC( victim ) && victim->act.test(ACT_NO_MIND ) )
      return TRUE;


   dam = 5 + ( 10 * ( ( level > 20 ) + ( level >= 40 ) + ( level >= 60 ) + ( level >= 75 ) ) );

   if( saves_spell( level, victim ) )
      dam /= 2;


   sp_damage( obj, ch, victim, dam, REALM_MIND | NO_REFLECT | NO_ABSORB, sn, TRUE );
   return TRUE;
}


bool spell_know_item( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *ob = ( OBJ_DATA * ) vo;
   char buf[MAX_STRING_LENGTH];
   AFFECT_DATA *paf;

   snprintf( buf, MSL,
            "Object '%s' is type %s, extra flags %s.\r\nWeight is %d.\r\n",
            ob->name, item_type_name( ob ), extra_bit_name( ob->extra_flags ), ob->weight );
   send_to_char( buf, ch );

   switch ( ob->item_type )
   {
      case ITEM_SCROLL:
      case ITEM_POTION:
         snprintf( buf, MSL, "Level %d spells of:", ob->value[0] );
         send_to_char( buf, ch );

         if( ob->value[1] > 0 && ob->value[1] < MAX_SKILL )
         {
            send_to_char( " '", ch );
            send_to_char( skill_table[ob->value[1]].name, ch );
            send_to_char( "'", ch );
         }

         if( ob->value[2] > 0 && ob->value[2] < MAX_SKILL )
         {
            send_to_char( " '", ch );
            send_to_char( skill_table[ob->value[2]].name, ch );
            send_to_char( "'", ch );
         }

         if( ob->value[3] > 0 && ob->value[3] < MAX_SKILL )
         {
            send_to_char( " '", ch );
            send_to_char( skill_table[ob->value[3]].name, ch );
            send_to_char( "'", ch );
         }

         send_to_char( ".\r\n", ch );
         break;

      case ITEM_WAND:
      case ITEM_STAFF:
         snprintf( buf, MSL, "Has %d(%d) charges of level %d", ob->value[1], ob->value[2], ob->value[0] );
         send_to_char( buf, ch );

         if( ob->value[3] >= 0 && ob->value[3] < MAX_SKILL )
         {
            send_to_char( " '", ch );
            send_to_char( skill_table[ob->value[3]].name, ch );
            send_to_char( "'", ch );
         }

         send_to_char( ".\r\n", ch );
         break;

      case ITEM_WEAPON:
         snprintf( buf, MSL, " Average damage is %d.\r\n", ( ob->value[1] + ob->value[2] ) / 2 );
         send_to_char( buf, ch );
         break;

      case ITEM_ARMOR:
         snprintf( buf, MSL, "Armor class is %d.\r\n", ob->value[0] );
         send_to_char( buf, ch );
         break;
   }

   for( paf = ob->first_apply; paf != NULL; paf = paf->next )
   {
      if( paf->location != APPLY_NONE && paf->modifier != 0 )
      {
         snprintf( buf, MSL, "Affects %s by %d.\r\n", affect_loc_name( paf->location ), paf->modifier );
         send_to_char( buf, ch );
      }
   }


   return TRUE;
}


bool spell_physic_thrust( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   int dam;
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;

   if( victim == ch )
      return FALSE;


   if( IS_NPC( victim ) && victim->act.test(ACT_NO_MIND ) )
      return TRUE;


   dam = 5 + ( 20 * ( level > 20 ) + ( level >= 40 ) + ( level >= 60 ) + ( level >= 75 ) );

   if( saves_spell( level, victim ) )
      dam /= 2;

   sp_damage( obj, ch, victim, dam, REALM_MIND | NO_REFLECT | NO_ABSORB, sn, TRUE );
   return TRUE;
}


bool spell_physic_crush( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   int dam;
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;

   if( victim == ch )
      return FALSE;

   if( IS_NPC( victim ) && victim->act.test(ACT_NO_MIND ) )
      return TRUE;


   dam = ch->get_level("psuedo") / 2 + ( 30 * ( level > 20 ) + ( level >= 40 ) + ( level >= 60 ) + ( level >= 75 ) );

   if( saves_spell( level, victim ) )
      dam /= 2;

   sp_damage( obj, ch, victim, dam, REALM_MIND | NO_REFLECT | NO_ABSORB, sn, TRUE );
   return TRUE;
}

bool spell_ego_whip( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   int dam;
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;

   if( victim == ch )
      return FALSE;

   if( IS_NPC( victim ) && victim->act.test(ACT_NO_MIND ) )
      return TRUE;


   dam = 50 + ( 40 * ( level > 20 ) + ( level >= 40 ) + ( level >= 60 ) + ( level >= 75 ) );

   if( saves_spell( level, victim ) )
      dam /= 2;

   sp_damage( obj, ch, victim, dam, REALM_MIND | NO_REFLECT | NO_ABSORB, sn, TRUE );
   return TRUE;
}


bool spell_night_vision( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_INFRARED ) || item_has_apply( victim, ITEM_APPLY_INFRA ) )
      return ( ch == victim ? FALSE : TRUE );
   act( "$n's eyes glow red.\r\n", ch, NULL, NULL, TO_ROOM );
   af.type = sn;
   af.duration = 4 + ( level / 3 );
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.bitvector = AFF_INFRARED;
   affect_to_char( victim, &af );
   send_to_char( "Your eyes glow red.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}

bool spell_stalker( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   /*
    * Fixed problem of stalker not finding victim, and attacking caster,
    * * thus giving an easy source of xp -S-
    */

   CHAR_DATA *victim;
   CHAR_DATA *stalker;

   if( target_name[0] == '\0' )
   {
      send_to_char( "Summon a stalker to hunt who?\r\n", ch );
      return FALSE;
   }


   victim = get_char_world( ch, target_name );

   if( victim == NULL )
   {
      send_to_char( "Target can't be found!\r\n", ch );
      return FALSE;
   }

   if( ch == victim )
   {
      send_to_char( "That WOULDN'T be wise!\r\n", ch );
      return FALSE;
   }
   act( "$n calls upon the dark powers to summon forth a Stalker!", ch, NULL, NULL, TO_ROOM );
   send_to_char( "You call upon the dark powers to summon forth a Stalker!\r\n", ch );

   stalker = create_mobile( get_mob_index( MOB_VNUM_STALKER ) );

   char_to_room( stalker, ch->in_room );
   act( "$n appears before you suddenly.", stalker, NULL, NULL, TO_ROOM );

   stalker->level = victim->level;
   stalker->max_hit = victim->max_hit;
   stalker->hit = stalker->max_hit;
   stalker->exp = victim->level * 10;  /* not much exp :P */

   if( set_hunt( stalker, ch, victim, NULL, HUNT_MERC, HUNT_CR ) )
      act( "$n sniffs the air in search of $s prey.", stalker, NULL, NULL, TO_ROOM );
   else
   {
      int slot;

      slot = skill_lookup( "hellspawn" );
      if( slot != 0 )  /* Check to be sure... should never == 0 */
         ( *skill_table[slot].spell_fun ) ( slot, stalker->level, stalker, ch, NULL );

      do_say( stalker, "How dare you waste my time!!" );
      act( "$n returns to the dark planes, vanishing suddenly!", stalker, NULL, NULL, TO_ROOM );
      /*
       * char_from_room( stalker );   
       */
      extract_char( stalker, TRUE );
   }
   return TRUE;
}

bool spell_mystic_armor( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( ch == victim )
   {
      send_to_char( "You are mystically armored, but it suddenly fades away!\r\n", ch );
      return FALSE;
   }

   if( is_affected( victim, sn ) )
      return TRUE;

   af.type = sn;
   af.duration = 4 + ( level / 3 );
   af.location = APPLY_AC;
   af.modifier = -10 - ch->get_level("psuedo") / 8;
   af.bitvector = 0;
   affect_to_char( victim, &af );

   act( "$n is surrounded by $N's mystic armor.", victim, NULL, ch, TO_ROOM );
   act( "$N is surrounded by your mystic armor.", ch, NULL, victim, TO_CHAR );
   act( "You are surrounded by $N's mystic armor.", victim, NULL, ch, TO_CHAR );
   return TRUE;
}


bool spell_flare( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_BLIND ) || saves_spell( level, victim ) )
      return TRUE;

   if( victim == ch )
      return FALSE;

   af.type = sn;
   af.location = APPLY_HITROLL;
   af.modifier = level / 4;
   af.duration = 1 + ( level / 4 );
   af.bitvector = AFF_BLIND;
   affect_to_char( victim, &af );
   act( "$n invokes the power of Ra to produce a solar flare which blinds $N!", ch, NULL, victim, TO_NOTVICT );
   act( "$N invokes the power of Ra to produce a solar flare.  You are BLINDED!", victim, NULL, ch, TO_CHAR );
   act( "You invoke the power of Ra to produce a solar flare, which blinds $N!", ch, NULL, victim, TO_CHAR );


   return TRUE;
}

bool spell_travel( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   /*
    * Transfer the player to Midgaard recall 
    */

   ROOM_INDEX_DATA *room;

   if( ch->fighting != NULL )
   {
      send_to_char( "You can't travel when fighting!\r\n", ch );
      return FALSE;
   }

   if( ( room = get_room_index( ROOM_VNUM_TEMPLE ) ) == NULL )
   {
      send_to_char( "It seems the Temple has vanished!\r\n", ch );
      return FALSE;
   }

   act( "$n is engulfed by a stream of green light!", ch, NULL, NULL, TO_ROOM );
   char_from_room( ch );
   char_to_room( ch, room );
   act( "$n arrives, carried by a stream of green light!", ch, NULL, NULL, TO_ROOM );
   do_look( ch, "auto" );
   return TRUE;
}

bool spell_window( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   /*
    * Create a magic window to look into another room 
    */
   OBJ_DATA *beacon;
   OBJ_DATA *ob;

   if( target_name[0] == '\0' )
   {
      send_to_char( "Form a window to whom?\r\n", ch );
      return FALSE;
   }

   beacon = get_obj_world( ch, target_name );

   if( beacon == NULL || beacon->item_type != ITEM_BEACON )
   {
      send_to_char( "Couldn't find target.  Sorry.\r\n", ch );
      return FALSE;
   }

   if( beacon->in_room == NULL )
   {
      send_to_char( "It seems that someone is carrying it.\r\n", ch );
      return FALSE;
   }

   if( str_cmp( beacon->owner, ch->name ) )
   {
      send_to_char( "That's not one of YOUR beacons!\r\n", ch );
      return FALSE;
   }

   act( "$n creates a magic window with a wave of $s hand.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "You create a magic window with a wave of your hand.\r\n", ch );

   ob = create_object( get_obj_index( OBJ_VNUM_WINDOW ), level );
   obj_to_room( ob, ch->in_room );
   ob->timer = 1 + ( level / 30 );
   ob->value[0] = beacon->in_room->vnum;
   ob->value[1] = 1;
   send_to_room( "The beacon suddenly vanishes!\r\n", beacon->in_room );
   extract_obj( beacon );
   return TRUE;
}

bool spell_portal( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   /*
    * Create a magic portal to another room 
    */
   OBJ_DATA *beacon;
   OBJ_DATA *ob;

   if( deathmatch )
   {
      send_to_char( "Not during a @@eDeath Match@@N!!!\r\n", ch );
      return FALSE;
   }
   if( target_name[0] == '\0' )
   {
      send_to_char( "Form a portal to what?\r\n", ch );
      return FALSE;
   }

   beacon = get_obj_world( ch, target_name );

   if( beacon == NULL || beacon->item_type != ITEM_BEACON )
   {
      send_to_char( "Couldn't find target.  Sorry.\r\n", ch );
      return FALSE;
   }

   if( beacon->in_room == NULL )
   {
      send_to_char( "It seems that someone is carrying it.\r\n", ch );
      return FALSE;
   }

   if( str_cmp( beacon->owner, ch->name ) )
   {
      send_to_char( "That's not one of YOUR beacons!\r\n", ch );
      return FALSE;
   }

   ob = create_object( get_obj_index( OBJ_VNUM_PORTAL ), level );
   obj_to_room( ob, ch->in_room );
   ob->timer = 1 + ( level / 30 );
   ob->value[0] = ( beacon->carried_by == NULL ? beacon->in_room->vnum : beacon->carried_by->in_room->vnum );
   ob->value[1] = 1;
   ob->value[2] = 1;

   act( "$n creates $p with a wave of $s hand.", ch, ob, NULL, TO_ROOM );
   act( "You create $p with a wave of your hand.", ch, ob, NULL, TO_CHAR );

   ob = create_object( get_obj_index( OBJ_VNUM_PORTAL ), level );
   obj_to_room( ob, beacon->in_room );
   ob->timer = 1 + ( level / 30 );
   ob->value[0] = ch->in_room->vnum;
   ob->value[1] = 1;
   ob->value[2] = 1;
   send_to_room( "The beacon suddenly vanishes!\r\n", beacon->in_room );
   send_to_room( "A glowing portal suddenly forms before you!\r\n", ob->in_room );
   extract_obj( beacon );
   return TRUE;
}

bool spell_beacon( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   /*
    * Create a beacon, ready for a portal to 'goto'.
    * * I owe someone thanks for this, but forgot the name... D'oh.
    * * How this works: (+ for portals, etc )
    * * a) Caster makes beacon: ownership set, obj->name = target_name
    * * b) Someone goes off with beacon, drops it
    * * c) Owner can then cast portal spell to it, beacon extracted
    * * -S- 
    */

   char buf[MAX_STRING_LENGTH];
   char arg[MAX_STRING_LENGTH];
   OBJ_DATA *ob;

   one_argument( target_name, arg );

   ob = get_obj_world( ch, arg );
   if( ob != NULL )
   {
      send_to_char( "There is already an object with that keyword.\r\n", ch );
      return FALSE;
   }

   ob = create_object( get_obj_index( OBJ_VNUM_BEACON ), level );
   snprintf( buf, MSL, "%s", arg );
   free_string( ob->name );
   ob->name = str_dup( arg );
   snprintf( buf, MSL, "%s", ch->name.c_str() );
   free_string( ob->owner );
   ob->owner = str_dup( buf );
   ob->timer = 20 + (level / 3);
   obj_to_room( ob, ch->in_room );
   act( "$n magically produces $p!", ch, ob, NULL, TO_ROOM );
   act( "You magically produce $p!", ch, ob, NULL, TO_CHAR );
   return TRUE;
}

bool spell_seal_room( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   ROOM_INDEX_DATA *room;
   ROOM_AFFECT_DATA raf;

   room = ch->in_room;

   if( room == NULL )
      return FALSE;

   if( IS_SET( room->affected_by, ROOM_BV_ENCAPS ) )
   {
      send_to_char( "This room is already sealed!\r\n", ch );
      return FALSE;
   }
   if( room->area->flags.test(AFLAG_NO_ROOM_AFF) )
   {
      send_to_char( "A strange @@eDeamon@@N appears, waggles it's finger at you, and shakes its(?) head!\r\n", ch );
      return FALSE;
   }
   if( deathmatch )
   {
      if( ch->hit < 150 )
      {
         send_to_char( "You don't have enough life force remaining!\r\n", ch );
         return FALSE;
      }
      else
      {
         send_to_char( "@@eYou feel some of your life force rip from your soul!@@N\r\n", ch );
         ch->hit -= 100;
      }
   }
   act( "$n spreads his hands into the air and ejects a web of energy!", ch, NULL, NULL, TO_ROOM );
   send_to_char( "You spread your hands into the air and eject a web of energy!\r\n", ch );

   raf.type = sn;
   raf.level = level;
   raf.duration = ( level / 20 ) + 2;
   raf.bitvector = ROOM_BV_ENCAPS;
   raf.caster = ch;
   affect_to_room( room, &raf );
   return TRUE;
}


bool spell_deflect_weapon( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, sn ) )
      return FALSE;
   af.type = sn;
   af.duration = (level + 2);
   af.modifier = -40 - ch->get_level("psuedo") / 5;
   af.location = APPLY_AC;
   af.bitvector = 0;
   affect_to_char( victim, &af );
   send_to_char( "You feel a mind shield form around you.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}

bool spell_black_hand( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;


   AFFECT_DATA af;




   act( "You summon a hand of death, and it chokes $N!", ch, NULL, victim, TO_CHAR );
   act( "A Black Hand grows from the shadows, and begins to choke $N!", ch, NULL, victim, TO_NOTVICT );
   act( "$n summons a Black Hand from the shadows, which begins to choke you!", ch, NULL, victim, TO_VICT );

   if( saves_spell( level, victim ) || IS_VAMP( victim ) || is_affected( victim, sn ) )
   {
      send_to_char( "The Black Hand dissolves back into the shadows!\r\n", victim );
      send_to_char( "The Black Hand dissolves back into the shadows!\r\n", ch );
      act( "The Black Hand dissolves back into the shadows!", ch, NULL, victim, TO_NOTVICT );
      return TRUE;
   }

   af.type = sn;
   af.duration = 3;
   af.location = APPLY_HIT;
   af.modifier = level;
   af.bitvector = 0;
   af.caster = ch;

   affect_to_char( victim, &af );

   act( "The Black Hand surrounds $N and begins to choke!", ch, NULL, victim, TO_ROOM );

   return TRUE;
}

bool spell_throw_needle( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;

   AFFECT_DATA af;

   if( !sp_damage( obj, ch, ( CHAR_DATA * ) vo, dice( 2, level ), REALM_POISON, sn, TRUE ) )
      return TRUE;

   if( !saves_spell( level, victim ) )
   {
      af.type = sn;
      af.duration = 12 + ( level / 10 );
      af.location = APPLY_STR;
      af.modifier = -3;
      af.bitvector = AFF_POISON;
      affect_join( victim, &af );
      send_to_char( "You feel very sick.\r\n", victim );
      act( "$n looks very sick.", victim, NULL, NULL, TO_ROOM );
   }

   return TRUE;




}

bool spell_morale( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   AFFECT_DATA af;
   CHAR_DATA *gch;

   for( gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room )
   {
      if( is_affected( gch, sn ) || !is_same_group( ch, gch ) )

         continue;
      act( "$n seems much more willing to fight.", gch, NULL, NULL, TO_ROOM );
      send_to_char( "You are inspired to fight better!\r\n", gch );
      af.type = sn;
      af.duration = 4 + ( level / 5 );
      af.location = APPLY_DAMROLL;
      af.modifier = ch->get_level("psuedo") / 10;
      af.bitvector = 0;
      affect_to_char( gch, &af );
   }
   send_to_char( "You inspire the troops!\r\n", ch );

   return TRUE;
}

bool spell_leadership( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   AFFECT_DATA af;
   CHAR_DATA *gch;

   for( gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room )
   {
      if( is_affected( gch, sn ) || !is_same_group( ch, gch ) )

         continue;
      act( "$n looks more courageous!", gch, NULL, NULL, TO_ROOM );
      send_to_char( "You fell courage wash over you!\r\n", gch );
      af.type = sn;
      af.duration = 4 + ( level / 5 );
      af.location = APPLY_HITROLL;
      af.modifier = ch->get_level("psuedo") / 10;
      af.bitvector = 0;
      affect_to_char( gch, &af );
   }
   send_to_char( "You inspire the troops!\r\n", ch );

   return TRUE;
}

bool spell_ice_bolt( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;

   dam = 150 + dice( level / 4, 12 );
   if( saves_spell( level, victim ) )
   {
      float tmp = dam;
      tmp /= 1.2;
      dam = (int)tmp;
   }
   sp_damage( obj, ch, victim, dam, REALM_COLD, sn, TRUE );
   return TRUE;
}

bool spell_waterelem( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *summoned;

   act( "$n calls upon the elemental forces of @@lwater@@N!", ch, obj, NULL, TO_ROOM );
   act( "You call upon the elemental forces of @@lwater@@N.", ch, obj, NULL, TO_CHAR );

   act( "A waterfall appears, and an elemental steps from the pool!!", ch, obj, NULL, TO_ROOM );
   act( "A waterfall appears, and an elemental steps from the pool!!", ch, obj, NULL, TO_CHAR );

   summoned = create_mobile( get_mob_index( MOB_VNUM_WATERELEM ) );
   char_to_room( summoned, ch->in_room );
   act( "$n floods from the pool, drawing all the water into it's body.", summoned, NULL, NULL, TO_ROOM );

   /*
    * don't think we need these  
    * summoned->level    = 40;  
    * summoned->max_hit  = dice( 8, 40 );
    * summoned->hit      = summoned->max_hit;
    * summoned->max_move = dice( 10, 40 );
    * summoned->move     = summoned->max_move;  
    * 
    */
   summoned->act.set(ACT_PET );
   SET_BIT( summoned->affected_by, AFF_CHARM );
   summoned->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   add_follower( summoned, ch );
   return TRUE;
}



bool spell_skeleton( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *summoned;

   act( "$n calls upon the @@dNegative Plane@@N!", ch, obj, NULL, TO_ROOM );
   act( "You call upon the @@dNegative Plane@@N.", ch, obj, NULL, TO_CHAR );

   act( "The ground opens beneath you, and a skeleton crawls out of the graveyard filth!!", ch, obj, NULL, TO_ROOM );
   act( "The ground opens beneath you, and a skeleton crawls out of the graveyard filth!!", ch, obj, NULL, TO_CHAR );

   summoned = create_mobile( get_mob_index( MOB_VNUM_SKELETON ) );
   char_to_room( summoned, ch->in_room );
   act( "$n Stands erect, and bow's towards it's master.", summoned, NULL, NULL, TO_ROOM );

   /*
    * summoned->level    = 40;
    * summoned->max_hit  = dice( 8, 40 );
    * summoned->hit      = summoned->max_hit;
    * summoned->max_move = dice( 10, 40 );
    * summoned->move     = summoned->max_move;  
    */

   summoned->act.set(ACT_PET );
   SET_BIT( summoned->affected_by, AFF_CHARM );
   summoned->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   add_follower( summoned, ch );
   return TRUE;
}

bool spell_poison_weapon( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *ob = ( OBJ_DATA * ) vo;
   AFFECT_DATA *paf;

   /*
    * Quick way to stop imms (Bash?) enchanting weapons for players 
    */
   if( IS_IMMORTAL( ch ) && ch->level != 85 )
   {
      send_to_char( "Nothing Happens.\r\n", ch );
      return FALSE;
   }

   if( ob->item_type != ITEM_WEAPON )
   {
      send_to_char( "That is not a weapon!.\r\n", ch );
      return FALSE;
   }

   /*
    * This is gonna fuck up a lot.. paf->type isnt saved for objects in
    * pfiles.. -- Alty 
    */
   for( paf = ob->first_apply; paf != NULL; paf = paf->next )
   {
      if( paf->type == sn )
      {
         send_to_char( "That weapon is already poisoned!.\r\n", ch );
         return FALSE;
      }
   }

   paf = new AFFECT_DATA;
   paf->type = sn;
   paf->location = APPLY_DAMROLL;
   paf->modifier = UMIN( ( level / 30 ) + 1, ob->level );
   LINK( paf, ob->first_apply, ob->last_apply, next, prev );

   return TRUE;
}

bool spell_fireshield( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   MAGIC_SHIELD *shield;
   AFFECT_DATA af;
   bool char_login = FALSE;

   if( ( !IS_NPC( ch ) ) && ( ch->desc != NULL ) && ( ch->desc->connected == CON_SETTING_STATS ) )
      char_login = TRUE;

/* check to see if victim is already flaming 
  if ( is_shielded( victim, FLAME_SHIELD ) )	
    return FALSE;
  
    for now, we are gonna allow only one shield up at a time  */
   if( ch->first_shield != NULL )
   {
      return FALSE;
   }

   af.type = sn;
   af.duration = ch->get_level("psuedo") / 10;
   if( char_login )
      af.duration /= 2;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char( ch, &af );

   shield = new MAGIC_SHIELD;
   shield->name = str_dup( "@@eFIRE@@N" );
   shield->type = FLAME_SHIELD;
   shield->harmful = TRUE;
   shield->attack_dam = number_range( ( level * 3 ), ( level * 5 ) );
   shield->percent = 10 + ( ch->get_level("psuedo") / 10 );
   shield->hits = 5000 + ( ch->get_level("psuedo") * 10 );
   shield->sn = sn;
   if( char_login )
      shield->hits /= ( number_range( 2, 10 ) );

   shield->absorb_message_room = str_dup( "@@N$n's @@eshield@@N flares and envelops $N in @@eflames@@N!!" );
   shield->absorb_message_victim = str_dup( "@@N$N's @@eshield@@N flares, and envelops you with @@eflame@@N!!" );
   shield->absorb_message_self = str_dup( "@@NYour @@eshield@@N flares, and envelops $N with @@eflame@@N!!!" );
   shield->wearoff_room = str_dup( "@@N$n's @@eshield@@N @@yFLAMES OUT@@N!!!!!" );
   shield->wearoff_self = str_dup( "@@NYour @@eshield@@N @@yFLAMES OUT@@N!!!!!" );

   LINK( shield, ch->first_shield, ch->last_shield, next, prev );

   send_to_char( "@@NYou @@eburst@@N into @@Rflames@@N!\r\n", ch );
   act( "@@N$n @@ebursts@@N into @@Rflames@@N!", ch, NULL, NULL, TO_ROOM );

   return TRUE;
}


bool spell_iceshield( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   MAGIC_SHIELD *shield;
   AFFECT_DATA af;
   bool char_login = FALSE;

   if( ( !IS_NPC( ch ) ) && ( ch->desc != NULL ) && ( ch->desc->connected == CON_SETTING_STATS ) )
      char_login = TRUE;

/* check to see if victim is already flaming 
  if ( is_shielded( victim, ICE_SHIELD ) )	
    return FALSE;
  
    for now, we are gonna allow only one shield up at a time  */
   if( ch->first_shield != NULL )
   {
      return FALSE;
   }

   af.type = sn;
   af.duration = ch->get_level("psuedo") / 15;
   if( char_login )
      af.duration /= 2;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char( ch, &af );

   shield = new MAGIC_SHIELD;
   shield->name = str_dup( "@@aICE@@N" );
   shield->type = ICE_SHIELD;
   shield->harmful = FALSE;
   shield->attack_dam = 0;
   shield->percent = 50 + ( ch->get_level("psuedo") / 6 );
   shield->hits = 15000 + ( ch->get_level("psuedo") * 50 );
   shield->sn = sn;
   if( char_login )
      shield->hits /= ( number_range( 2, 10 ) );

   shield->absorb_message_room = str_dup( "@@N$n's @@ashield@@N absorbs the blow@@N!!" );
   shield->absorb_message_victim = str_dup( "@@N$N's @@ashield@@N absorbs the blow@@N!!" );
   shield->absorb_message_self = str_dup( "@@NYour @@ashield@@N absorbs the blow@@N!!!" );
   shield->wearoff_room = str_dup( "@@N$n's @@ashield @@Nis @@rSHATTERED@@N!!!" );
   shield->wearoff_self = str_dup( "@@NYour @@ashield@@N is @@rSHATTERED@@N!!!" );


   LINK( shield, ch->first_shield, ch->last_shield, next, prev );

   send_to_char( "@@NYou are encased in @@aIce@@N!\r\n", ch );
   act( "@@N$n is encased in @@aIce@@N!!", ch, NULL, NULL, TO_ROOM );

   return TRUE;
}

bool spell_shockshield( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   MAGIC_SHIELD *shield;
   AFFECT_DATA af;
   bool char_login = FALSE;

   if( ( !IS_NPC( ch ) ) && ( ch->desc != NULL ) && ( ch->desc->connected == CON_SETTING_STATS ) )
      char_login = TRUE;

/* check to see if victim is already flaming 
  if ( is_shielded( victim, SHOCK_SHIELD ) )	
    return FALSE;
  
    for now, we are gonna allow only one shield up at a time  */
   if( ch->first_shield != NULL )
   {
      return FALSE;
   }

   af.type = sn;
   af.duration = ch->get_level("psuedo") / 7;
   if( char_login )
      af.duration /= 2;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char( ch, &af );

   shield = new MAGIC_SHIELD;
   shield->name = str_dup( "@@lSHOCK@@N" );
   shield->type = SHOCK_SHIELD;
   shield->harmful = TRUE;
   shield->attack_dam = number_range( ( level * 2 ), ( level * 4 ) );
   shield->percent = 25 + ch->get_level("psuedo") / 9;
   shield->hits = 7000 + ch->get_level("psuedo") * 20;
   shield->sn = sn;
   if( char_login )
      shield->hits /= ( number_range( 2, 10 ) );

   shield->absorb_message_room = str_dup( "@@N$n's @@lshield@@N @@ysparks,@@N and zaps $N@@N!!" );
   shield->absorb_message_victim = str_dup( "@@N$N's @@lshield@@N @@ysparks@@N, and zaps you@@N!!" );
   shield->absorb_message_self = str_dup( "@@NYour @@lshield@@N @@ysparks@@N, and zaps $N@@N!!!" );
   shield->wearoff_room = str_dup( "@@N$n's @@lshield@@N @@aDISSAPATES@@N!!!!!" );
   shield->wearoff_self = str_dup( "@@NYour @@lshield@@N @@aDISSAPATES@@N!!!!!" );


   LINK( shield, ch->first_shield, ch->last_shield, next, prev );

   send_to_char( "@@NYou are surrounded by an @@lelectric field@@N!!\r\n", ch );
   act( "@@N$n is surrounded in an @@lelectric field@@N!!", ch, NULL, NULL, TO_ROOM );

   return TRUE;
}

bool spell_shadowshield( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   MAGIC_SHIELD *shield;
   AFFECT_DATA af;
   bool char_login = FALSE;

   if( ( !IS_NPC( ch ) ) && ( ch->desc != NULL ) && ( ch->desc->connected == CON_SETTING_STATS ) )
      char_login = TRUE;

/* check to see if victim is already flaming 
  if ( is_shielded( victim, FLAME_SHIELD ) )	
    return FALSE;
  
    for now, we are gonna allow only one shield up at a time  */
   if( ch->first_shield != NULL )
   {
      return FALSE;
   }

   af.type = sn;
   af.duration = ch->get_level("psuedo") / 10;
   if( char_login )
      af.duration /= 2;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char( ch, &af );

   shield = new MAGIC_SHIELD;
   shield->name = str_dup( "@@dSHADOW@@N" );
   shield->type = FLAME_SHIELD;
   shield->harmful = TRUE;
   shield->attack_dam = number_range( (int)( level * 2 ), (int)( level * 4.5 ) );
   shield->percent = 20;
   shield->hits = 3000;
   shield->sn = sn;
   if( char_login )
      shield->hits /= ( number_range( 2, 10 ) );

   shield->absorb_message_room = str_dup( "@@N$n's @@eshield@@N flares and envelops $N in @@eflames@@N!!" );
   shield->absorb_message_victim = str_dup( "@@N$N's @@eshield@@N flares, and envelops you with @@eflame@@N!!" );
   shield->absorb_message_self = str_dup( "@@NYour @@eshield@@N flares, and envelops $N with @@eflame@@N!!!" );
   shield->wearoff_room = str_dup( "@@N$n's @@dshield@@N @@dDISSAPATES@@N!!!!!" );
   shield->wearoff_self = str_dup( "@@NYour @@dshield@@N @@dDISSAPATES@@N!!!!!" );


   LINK( shield, ch->first_shield, ch->last_shield, next, prev );

   send_to_char( "You burst into flames!\r\n", ch );
   act( "$n bursts into flames!", ch, NULL, NULL, TO_ROOM );

   return TRUE;
}

bool spell_thoughtshield( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   MAGIC_SHIELD *shield;
   AFFECT_DATA af;
   bool char_login = FALSE;

   if( ( !IS_NPC( ch ) ) && ( ch->desc != NULL ) && ( ch->desc->connected == CON_SETTING_STATS ) )
      char_login = TRUE;

/* check to see if victim is already flaming 
  if ( is_shielded( victim, FLAME_SHIELD ) )	
    return FALSE;
  
    for now, we are gonna allow only one shield up at a time  */
   if( ch->first_shield != NULL )
   {
      return FALSE;
   }

   af.type = sn;
   af.duration = ch->get_level("psuedo") / 10;
   if( char_login )
      af.duration /= 2;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char( ch, &af );

   shield = new MAGIC_SHIELD;
   shield->name = str_dup( "@@mTHOUGHT@@N" );
   shield->type = FLAME_SHIELD;
   shield->harmful = TRUE;
   shield->attack_dam = number_range( (int)( level * 2 ), (int)( level * 4.5 ) );
   shield->percent = 20;
   shield->hits = 3000;
   shield->sn = sn;
   if( char_login )
      shield->hits /= ( number_range( 2, 10 ) );

   shield->absorb_message_room = str_dup( "@@N$n's @@eshield@@N flares and envelops $N in @@eflames@@N!!" );
   shield->absorb_message_victim = str_dup( "@@N$N's @@eshield@@N flares, and envelops you with @@eflame@@N!!" );
   shield->absorb_message_self = str_dup( "@@NYour @@eshield@@N flares, and envelops $N with @@eflame@@N!!!" );
   shield->wearoff_room = str_dup( "@@N$n's @@dshield@@N @@dDISSAPATES@@N!!!!!" );
   shield->wearoff_self = str_dup( "@@NYour @@dshield@@N @@dDISSAPATES@@N!!!!!" );

   LINK( shield, ch->first_shield, ch->last_shield, next, prev );

   send_to_char( "You burst into flames!\r\n", ch );
   act( "$n bursts into flames!", ch, NULL, NULL, TO_ROOM );

   return TRUE;
}




bool spell_ethereal( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   /*
    * Ethereal travel :)
    * * 
    * * Zenithar
    */



   if( deathmatch )
   {
      send_to_char( "Not during a @@eDeath Match@@N!!\r\n", ch );
      return FALSE;
   }
   if( ch->in_room->room_flags.test(RFLAG_NO_RECALL)
       && ( !IS_NPC( ch ) && ( !ch->act.test(ACT_INTELLIGENT ) ) ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return FALSE;
   }


   /*
    * Check is ch screws up, and ends up in limbo... <grin> 
    */

   if( ( number_percent(  ) < 15 ) && ( !IS_NPC( ch ) ) )   /* 15% chance */
   {
      send_to_char( "@@NYou get distracted, and appear in the middle of @@dnowhere@@N!\r\n", ch );
      act( "@@N$n begins to disperse into @@lghostly @@Nparticles, then disappears!!", ch, NULL, NULL, TO_ROOM );
      char_from_room( ch );
      char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );
      if( ch->riding != NULL )
      {
         char_from_room( ch->riding );
         char_to_room( ch->riding, get_room_index( ROOM_VNUM_LIMBO ) );
      }

      act( "@@N$n coelesces from a @@lghostly@@N image into the room.", ch, NULL, NULL, TO_ROOM );
      do_look( ch, "auto" );
      return TRUE;
   }

   act( "@@N$n begins to disperse into @@lghostly @@Nparticles, then disappears!!", ch, NULL, NULL, TO_ROOM );
   char_from_room( ch );
   char_to_room( ch, get_room_index( ROOM_VNUM_ETHEREAL_PLANE ) );
   if( ch->riding != NULL )
   {
      char_from_room( ch->riding );
      char_to_room( ch->riding, get_room_index( ROOM_VNUM_ETHEREAL_PLANE ) );
   }

   act( "@@N$n coelesces from a @@lghostly@@N image into the room.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "@@NYou enter the @@lEthereal Plane@@N!\r\n", ch );
   do_look( ch, "auto" );
   return TRUE;
}

bool spell_fireelem( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *summoned;

   act( "$n calls upon the elemental forces of @@efire@@N!", ch, obj, NULL, TO_ROOM );
   act( "You call upon the elemental forces of @@efire@@N.", ch, obj, NULL, TO_CHAR );

   act( "@@NA @@ebonfire@@N appears, and an elemental steps from the flames!!", ch, obj, NULL, TO_ROOM );
   act( "@@NA @@ebonfire@@N appears, and an elemental steps from the flames!!", ch, obj, NULL, TO_CHAR );

   summoned = create_mobile( get_mob_index( MOB_VNUM_FIREELEM ) );
   char_to_room( summoned, ch->in_room );
   act( "$n leaps from the bonfire, drawing all the @@eflames@@N into it's body.", summoned, NULL, NULL, TO_ROOM );

   /*
    * don't think we need these  
    * summoned->level    = 40;  
    * summoned->max_hit  = dice( 8, 40 );
    * summoned->hit      = summoned->max_hit;
    * summoned->max_move = dice( 10, 40 );
    * summoned->move     = summoned->max_move;  
    * 
    */
   summoned->act.set(ACT_PET );
   SET_BIT( summoned->affected_by, AFF_CHARM );
   summoned->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   add_follower( summoned, ch );
   return TRUE;
}

bool spell_rune_fire( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   ROOM_INDEX_DATA *room;
   ROOM_AFFECT_DATA raf;

   room = ch->in_room;

   if( room == NULL )
      return FALSE;

   if( IS_SET( room->affected_by, ROOM_BV_FIRE_RUNE ) )
   {
      send_to_char( "@@NThere is already a @@eFire@@N Rune operating here!\r\n", ch );
      return FALSE;
   }

   if( room->area->flags.test(AFLAG_NO_ROOM_AFF) )
   {
      send_to_char( "A strange @@eDeamon@@N appears, waggles it's finger at you, and shakes its(?) head!\r\n", ch );
      return FALSE;
   }


   act( "$n draws a mystical @@eFire@@N Rune in the air.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "@@NYou draw a @@eFire@@N Rune in the air.\r\n", ch );

   raf.type = sn;
   raf.duration = ( level ) + number_range( 2, 20 );
   raf.level = level;
   raf.bitvector = ROOM_BV_FIRE_RUNE;
   raf.caster = ch;
   raf.modifier = ( level * 3 ) - number_range( 10, 50 );

   affect_to_room( room, &raf );
   return TRUE;
}

bool spell_rune_poison( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   ROOM_INDEX_DATA *room;
   ROOM_AFFECT_DATA raf;

   room = ch->in_room;

   if( room == NULL )
      return FALSE;

   if( IS_SET( room->affected_by, ROOM_BV_POISON_RUNE ) )
   {
      send_to_char( "@@NThere is already a @@dPoison@@N Rune operating here!\r\n", ch );
      return FALSE;
   }

   if( room->area->flags.test(AFLAG_NO_ROOM_AFF) )
   {
      send_to_char( "A strange @@eDeamon@@N appears, waggles it's finger at you, and shakes its(?) head!\r\n", ch );
      return FALSE;
   }


   act( "$n draws a mystical @@dPoison@@N Rune in the air.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "@@NYou draw a @@dPoison@@N Rune in the air.\r\n", ch );

   raf.type = sn;
   raf.duration = ( level ) + number_range( 2, 20 );
   raf.level = level;
   raf.bitvector = ROOM_BV_POISON_RUNE;
   raf.caster = ch;
   raf.modifier = 0;

   affect_to_room( room, &raf );
   return TRUE;
}

bool spell_rune_shock( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   ROOM_INDEX_DATA *room;
   ROOM_AFFECT_DATA raf;

   room = ch->in_room;

   if( room == NULL )
      return FALSE;

   if( IS_SET( room->affected_by, ROOM_BV_SHOCK_RUNE ) )
   {
      send_to_char( "@@NThere is already a @@lShock@@N Rune operating here!\r\n", ch );
      return FALSE;
   }
   if( room->area->flags.test(AFLAG_NO_ROOM_AFF) )
   {
      send_to_char( "A strange @@eDeamon@@N appears, waggles it's finger at you, and shakes its(?) head!\r\n", ch );
      return FALSE;
   }



   act( "$n draws a mystical @@lShock@@N Rune in the air.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "@@NYou draw a @@lShock@@N Rune in the air.\r\n", ch );

   raf.type = sn;
   raf.duration = ( level ) + number_range( 2, 20 );
   raf.level = level;
   raf.bitvector = ROOM_BV_SHOCK_RUNE;
   raf.caster = ch;
   raf.modifier = ( level * 3 ) - number_range( 10, 50 );

   affect_to_room( room, &raf );
   return TRUE;
}


bool spell_healing_light( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   ROOM_INDEX_DATA *room;
   ROOM_AFFECT_DATA raf;

   room = ch->in_room;

   if( room == NULL )
      return FALSE;

   if( IS_SET( room->affected_by, ROOM_BV_HEAL_REGEN ) )
   {
      send_to_char( "@@NThere is already a @@mHealing Light@@N operating here!\r\n", ch );
      return FALSE;
   }



   act( "@@NA majestic @@mHealing Light@@N fills the room.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "@@NYou fill the room with a majestic @@mHealing Light@@N.\r\n", ch );

   raf.type = sn;
   raf.duration = ( level / 8 ) + number_range( 2, 10 );
   raf.level = level;
   raf.bitvector = ROOM_BV_HEAL_REGEN;
   raf.caster = ch;
   raf.modifier = 0;

   affect_to_room( room, &raf );
   return TRUE;
}

bool spell_wither_shadow( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   ROOM_INDEX_DATA *room;
   ROOM_AFFECT_DATA raf;

   room = ch->in_room;

   if( room == NULL )
      return FALSE;

   if( IS_SET( room->affected_by, ROOM_BV_HEAL_STEAL ) )
   {
      send_to_char( "@@NThere is already a @@dWithering Shadow@@N operating here!\r\n", ch );
      return FALSE;
   }

   if( room->area->flags.test(AFLAG_NO_ROOM_AFF) )
   {
      send_to_char( "A strange @@eDeamon@@N appears, waggles it's finger at you, and shakes its(?) head!\r\n", ch );
      return FALSE;
   }


   act( "@@NA deadly @@dWithering Shadow@@N fills the room.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "@@NYou fill the room with a deadly @@dWithering Shadow@@N.\r\n", ch );

   raf.type = sn;
   raf.duration = ( level / 8 ) + number_range( 2, 10 );
   raf.level = level;
   raf.bitvector = ROOM_BV_HEAL_STEAL;
   raf.caster = ch;
   raf.modifier = 0;

   affect_to_room( room, &raf );
   return TRUE;
}


bool spell_mana_flare( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   ROOM_INDEX_DATA *room;
   ROOM_AFFECT_DATA raf;

   room = ch->in_room;

   if( room == NULL )
      return FALSE;

   if( IS_SET( room->affected_by, ROOM_BV_MANA_REGEN ) )
   {
      send_to_char( "@@NThere is already a @@eMana Flare@@N operating here!\r\n", ch );
      return FALSE;
   }



   act( "@@NA powerful @@eMana Flare@@N encompasses the room.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "@@NYou fill the room with a powerful @@eMana Flare@@N.\r\n", ch );

   raf.type = sn;
   raf.duration = ( level / 8 ) + number_range( 2, 10 );
   raf.level = level;
   raf.bitvector = ROOM_BV_MANA_REGEN;
   raf.caster = ch;
   raf.modifier = 0;

   affect_to_room( room, &raf );
   return TRUE;
}


bool spell_mana_drain( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   ROOM_INDEX_DATA *room;
   ROOM_AFFECT_DATA raf;

   room = ch->in_room;

   if( room == NULL )
      return FALSE;

   if( IS_SET( room->affected_by, ROOM_BV_MANA_STEAL ) )
   {
      send_to_char( "@@NThere is already a @@dMana Drain@@N operating here!\r\n", ch );
      return FALSE;
   }
   if( room->area->flags.test(AFLAG_NO_ROOM_AFF) )
   {
      send_to_char( "A strange @@eDeamon@@N appears, waggles it's finger at you, and shakes its(?) head!\r\n", ch );
      return FALSE;
   }



   act( "@@NA mind-sapping @@dMana Drain@@N fills the room.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "@@NYou fill the room with a mind-sapping @@dMana Drain@@N.\r\n", ch );

   raf.type = sn;
   raf.duration = ( level / 8 ) + number_range( 2, 10 );
   raf.level = level;
   raf.bitvector = ROOM_BV_MANA_STEAL;
   raf.caster = ch;
   raf.modifier = 0;

   affect_to_room( room, &raf );
   return TRUE;
}

bool spell_cage( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   ROOM_INDEX_DATA *room;
   ROOM_AFFECT_DATA raf;

   room = ch->in_room;

   if( room == NULL )
      return FALSE;

   if( IS_SET( room->affected_by, ROOM_BV_HOLD ) )
   {
      send_to_char( "@@NThere is already a @@rCage@@N operating here!\r\n", ch );
      return FALSE;
   }



   act( "@@NA paralysing @@rCage@@N surrounds the room.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "@@NYou surround the room with a paralyzing @@rCage@@N.\r\n", ch );

   raf.type = sn;
   raf.duration = ( level / 20 ) + number_range( 2, 10 );
   raf.level = level;
   raf.bitvector = ROOM_BV_HOLD;
   raf.caster = ch;
   raf.modifier = 0;

   affect_to_room( room, &raf );
   return TRUE;
}

bool spell_cloak_flaming( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   AFFECT_DATA af;


   if( is_affected( ch, sn ) )
      return FALSE;



   af.type = sn;
   af.duration = ch->level / 8;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = AFF_CLOAK_FLAMING;
   affect_to_char( ch, &af );

   return TRUE;
}


bool spell_cloak_reflect( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   AFFECT_DATA af;



   if( is_affected( ch, sn ) )
      return FALSE;


   af.type = sn;
   af.duration = ch->level / 8;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = AFF_CLOAK_REFLECTION;
   affect_to_char( ch, &af );

   return TRUE;
}

bool spell_cloak_absorb( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   AFFECT_DATA af;


   if( is_affected( ch, sn ) )
      return FALSE;



   af.type = sn;
   af.duration = ch->level / 8;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = AFF_CLOAK_ABSORPTION;
   affect_to_char( ch, &af );

   return TRUE;
}

bool spell_cloak_adept( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   AFFECT_DATA af;


   if( is_affected( ch, sn ) )
      return FALSE;

   if( IS_NPC( ch ) )
      return FALSE;


   af.type = sn;
   af.duration = ch->get_level("psuedo") / 5;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = AFF_CLOAK_ADEPT;
   affect_to_char( ch, &af );

   return TRUE;
}

bool spell_cloak_regen( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   AFFECT_DATA af;


   if( is_affected( ch, sn ) )
      return FALSE;



   af.type = sn;
   af.duration = ch->get_level("psuedo") / 10;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = AFF_CLOAK_REGEN;
   affect_to_char( ch, &af );

   return TRUE;
}


bool spell_room_dispel( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   ROOM_INDEX_DATA *room;
   ROOM_AFFECT_DATA *raf;
   ROOM_AFFECT_DATA *raf_next;
   int chance = 0;


   room = ch->in_room;

   if( room == NULL )
      return FALSE;

   if( IS_NPC( ch ) && ch->act.test(ACT_INTELLIGENT ) )
      chance = 1000;
   else
      chance = ch->level + 20;


   act( "$n gestures demonically at the magical spells around the room.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "@@NYou gesture demonically!\r\n", ch );


   for( raf = room->first_room_affect; raf != NULL; raf = raf_next )
   {
      raf_next = raf->next;
      if( number_percent(  ) < chance )
      {
         r_affect_remove( room, raf );
         chance = ( 2 * chance ) / 3;
      }
      else
         break;
   }

   return TRUE;
}


bool spell_throw_star( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;

   AFFECT_DATA af;

   if( !sp_damage( obj, ch, ( CHAR_DATA * ) vo,
                   dice( 5, level / 2 ) + dice( 5, level / 2 ) + level, REALM_POISON, sn, TRUE ) )
      return TRUE;

   if( !saves_spell( level, victim ) )
   {
      af.type = sn;
      af.duration = 12 + ( level / 10 );
      af.location = APPLY_DAMROLL;
      af.modifier = -level / 3;
      af.bitvector = AFF_POISON;
      affect_join( victim, &af );
      send_to_char( "You feel very sick.\r\n", victim );
      act( "$n looks very sick.", victim, NULL, NULL, TO_ROOM );
   }

   return TRUE;




}

bool spell_soul_net( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   ROOM_INDEX_DATA *room;
   ROOM_AFFECT_DATA raf;

   room = ch->in_room;

   if( room == NULL )
      return FALSE;

   if( IS_SET( room->affected_by, ROOM_BV_SOUL_NET ) )
   {
      send_to_char( "@@NThere is already a @@dSoul Net@@N operating here!\r\n", ch );
      return FALSE;
   }



   act( "@@NA demonic @@dSoul Net@@N enshrouds the room.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "@@NYou enshroud the room with a demonic @@dSoul Net@@N.\r\n", ch );

   raf.type = sn;
   raf.duration = ( level / 8 );
   raf.level = level;
   raf.bitvector = ROOM_BV_SOUL_NET;
   raf.caster = ch;
   raf.modifier = 0;

   affect_to_room( room, &raf );
   return TRUE;
}

bool spell_condense_soul( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *ob = ( OBJ_DATA * ) vo;

   if( IS_NPC( ch ) )
      return FALSE;

   if( ob->item_type != ITEM_SOUL )
   {
      send_to_char( "That is not a soul!.\r\n", ch );
      return FALSE;
   }

   if( ob->level < 80 )
   {
      send_to_char( "Bah!! That soul is to weak to use!!\r\n", ch );
      return FALSE;
   }
   extract_obj( ob );
   ob = create_object( get_obj_index( OBJ_VNUM_SOUL_POTION ), level );
   obj_to_char( ob, ch );

   act( "@@N$n gestures diabolically, and his captured soul condenses into a@@dSoul Potion@@N.", ch, NULL, NULL, TO_ROOM );
   send_to_char( "@@NYou condense the soul and some of your life force into a @@dSoul potion@@N.\r\n", ch );
   ch->max_hit -= 75;
   ch->pcdata->hp_from_gain -= 75;

   return TRUE;
}


bool spell_restoration( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   char buf[MAX_STRING_LENGTH];
   if( IS_NPC( ch ) )
      return FALSE;

   ch->hit = ch->max_hit;
   ch->mana = ch->max_mana;
   ch->move = ch->max_move;
   if( !IS_NPC( ch ) )
      ch->pcdata->super->energy = ch->pcdata->super->energy_max;

   send_to_char( "@@eTHe life force of tha captured soul restores you!\r\n", ch );
   snprintf( buf, MSL, " %s has used a restoration spell.\r\n", ch->name.c_str() );
   monitor_chan( buf, MONITOR_BAD );
   return TRUE;
}

bool spell_infuse( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   OBJ_DATA *ob = ( OBJ_DATA * ) vo;
   AFFECT_DATA *paf;
   OBJ_DATA *obj_soul;
   if( IS_NPC( ch ) )
      return FALSE;

   /*
    * Quick way to stop imms (Bash?) enchanting weapons for players
    */
   if( IS_IMMORTAL( ch ) && ch->level != 85 )
   {
      send_to_char( "Nothing Happens.\r\n", ch );
      return FALSE;
   }
   if( IS_NPC( ch ) )
      return FALSE;
   if( ( obj_soul = get_eq_char( ch, WEAR_HOLD_HAND_L ) ) == NULL )
      if( ( obj_soul = get_eq_char( ch, WEAR_HOLD_HAND_R ) ) == NULL )
      {
         send_to_char( "@@NYou must be holding a @@eSoul@@N to cast this spell!!\r\n", ch );
         return FALSE;
      }
   if( obj_soul->item_type != ITEM_SOUL )
   {
      send_to_char( "@@NYou are not holding a @@eSoul@@N!!\r\n", ch );
      return FALSE;
   }

   if( ob->item_type != ITEM_WEAPON )
   {
      send_to_char( "That is not a weapon!.\r\n", ch );
      return FALSE;
   }

   for( paf = ob->first_apply; paf != NULL; paf = paf->next )
   {
      if( paf->type == sn )
      {
         send_to_char( "That weapon is already infused with a soul!.\r\n", ch );
         return FALSE;
      }
   }
   if( ch->pcdata->hp_from_gain < 100 )
   {
      send_to_char( "You have exhausted your life force, and are unable to"
                    " control the necromantic forces necessary for this act.\r\n", ch );
      return FALSE;
   }
   paf = new AFFECT_DATA;
   paf->type = sn;
   paf->location = APPLY_DAMROLL;
   paf->modifier = ( obj_soul->level / 8 );
   LINK( paf, ob->first_apply, ob->last_apply, next, prev );

   paf = new AFFECT_DATA;
   paf->type = sn;
   paf->location = APPLY_AC;
   paf->modifier = -( obj_soul->level / 2 );
   LINK( paf, ob->first_apply, ob->last_apply, next, prev );

   paf = new AFFECT_DATA;
   paf->type = sn;
   paf->location = APPLY_MANA;
   paf->modifier = obj_soul->level / 3;
   LINK( paf, ob->first_apply, ob->last_apply, next, prev );


   paf = new AFFECT_DATA;
   paf->type = sn;
   paf->location = APPLY_HITROLL;
   paf->modifier = ( obj_soul->level / 8 );
   LINK( paf, ob->first_apply, ob->last_apply, next, prev );

   ob->extra_flags.set(ITEM_EXTRA_NO_DISARM);
   ob->extra_flags.set(ITEM_EXTRA_NO_DROP);
   ob->extra_flags.set(ITEM_EXTRA_NO_AUCTION);
   ob->extra_flags.set(ITEM_EXTRA_NO_SAC);
   ob->extra_flags.set(ITEM_EXTRA_NO_LOOT);
   ob->extra_flags.set(ITEM_EXTRA_ANTI_GOOD);
   ob->extra_flags.set(ITEM_EXTRA_NO_LOOT);

   ob->obj_fun = obj_fun_lookup( "objfun_infused_soul" );
   extract_obj( obj_soul );

   act( "@@N$n gestures diabolically, and his captured @@esoul@@N is infused into his $P.", ch, NULL, ob, TO_ROOM );
   send_to_char( "@@NYou condense the soul and some of your life force into your weapon.\r\n", ch );
   ch->max_hit -= 100;
   ch->pcdata->hp_from_gain -= 100;
   do_save( ch, "auto" );
   return TRUE;
}

bool spell_holy_light( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   CHAR_DATA *gch;

   for( gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room )
   {
      if( !is_same_group( ch, gch ) )

         continue;
      act( "$N is invigorated by the light shining off of $n!", ch, NULL, gch, TO_NOTVICT );
      send_to_char( "You feel a warm light invigorate you!\r\n", gch );
      gch->hit = UMIN( gch->max_hit, ( gch->hit + number_range( level * 2, level * 5 ) ) );
   }
   send_to_char( "You invigorate the troops!\r\n", ch );

   return TRUE;
}

bool spell_holy_armor( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, sn ) )
      return FALSE;
   af.type = sn;
   af.duration = 2 + ( level / 7 );
   af.modifier = -80 - ch->get_level("psuedo") / 5;
   af.location = APPLY_AC;
   af.bitvector = 0;
   affect_to_char( victim, &af );
   send_to_char( "Your armor is now blessed!.\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}

bool spell_divine_intervention( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int heal;

   heal = UMAX( 600, number_range( level * 4, level * 10 ) );
   victim->hit = UMIN( victim->hit + heal, victim->max_hit );
   update_pos( victim );
   send_to_char( "You feel the hand of your God invigorate your soul!\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}

bool spell_earthelem( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *summoned;

   act( "$n calls upon the elemental forces of @@bearth@@N!", ch, obj, NULL, TO_ROOM );
   act( "You call upon the elemental forces of @@bearth@@N.", ch, obj, NULL, TO_CHAR );

   act( "@@NA huge mound of @@bearth@@N appears, and an elemental steps from the boulders!!", ch, obj, NULL, TO_ROOM );
   act( "@@NA huge mound of @@bearth@@N appears, and an elemental steps from the boulders!!", ch, obj, NULL, TO_CHAR );

   summoned = create_mobile( get_mob_index( MOB_VNUM_EARTHELEM ) );
   char_to_room( summoned, ch->in_room );
   act( "@@N$n emerges from the mound, assuming the @@bearth@@N into it's body.", summoned, NULL, NULL, TO_ROOM );

   /*
    * don't think we need these  
    * summoned->level    = 40;  
    * summoned->max_hit  = dice( 8, 40 );
    * summoned->hit      = summoned->max_hit;
    * summoned->max_move = dice( 10, 40 );
    * summoned->move     = summoned->max_move;  
    * 
    */
   summoned->act.set(ACT_PET );
   SET_BIT( summoned->affected_by, AFF_CHARM );
   summoned->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   add_follower( summoned, ch );
   return TRUE;
}

bool spell_iron_golem( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *summoned;

   act( "@@N$n calls upon the @@aalchemical@@N forces of @@dmetal@@N!", ch, obj, NULL, TO_ROOM );
   act( "You call upon the @@aalchemical@@N forces of @@dmetal@@N.", ch, obj, NULL, TO_CHAR );

   act( "@@NA large slab of @@diron@@N appears, and a golem steps from the mass!!", ch, obj, NULL, TO_ROOM );
   act( "@@NA large slab of @@diron@@N appears, and a golem steps from the mass!!", ch, obj, NULL, TO_CHAR );

   summoned = create_mobile( get_mob_index( MOB_VNUM_IRON_GOLEM ) );
   char_to_room( summoned, ch->in_room );
   act( "@@N$n mutates from the @@dslab of iron@@N, drawing all the metal into it's body.", summoned, NULL, NULL, TO_ROOM );

   /*
    * don't think we need these  
    * summoned->level    = 40;  
    * summoned->max_hit  = dice( 8, 40 );
    * summoned->hit      = summoned->max_hit;
    * summoned->max_move = dice( 10, 40 );
    * summoned->move     = summoned->max_move;  
    * 
    */
   summoned->act.set(ACT_PET );
   SET_BIT( summoned->affected_by, AFF_CHARM );
   summoned->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   add_follower( summoned, ch );
   return TRUE;
}

bool spell_soul_thief( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *summoned;


   act( "$n calls upon the @@dNegative Plane@@N!", ch, obj, NULL, TO_ROOM );
   act( "You call upon the @@dNegative Plane@@N.", ch, obj, NULL, TO_CHAR );

   act( "@@NThe ground opens beneath you, and a @@lSoul @@eThief@@N crawls out of the graveyard filth!!", ch, obj, NULL,
        TO_ROOM );
   act( "@@NThe ground opens beneath you, and a @@lSoul @@eThief@@N crawls out of the graveyard filth!!", ch, obj, NULL,
        TO_CHAR );

   summoned = create_mobile( get_mob_index( MOB_VNUM_SOUL_THIEF ) );
   char_to_room( summoned, ch->in_room );
   act( "$n Stands erect, and bow's towards it's master.", summoned, NULL, NULL, TO_ROOM );
   /*
    * don't think we need these  
    * summoned->level    = 40;  
    * summoned->max_hit  = dice( 8, 40 );
    * summoned->hit      = summoned->max_hit;
    * summoned->max_move = dice( 10, 40 );
    * summoned->move     = summoned->max_move;  
    * 
    */
   summoned->act.set(ACT_PET );
   SET_BIT( summoned->affected_by, AFF_CHARM );
   summoned->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   add_follower( summoned, ch );
   return TRUE;
}

bool spell_holy_avenger( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *summoned;

   act( "$n calls upon the holy forces of @@Wlight@@N!", ch, obj, NULL, TO_ROOM );
   act( "You call upon the holy forces of @@Wlight@@N.", ch, obj, NULL, TO_CHAR );

   act( "@@NA shimmering halo appears, and a @@yHoly @@WAvenger@@N steps from the light!!", ch, obj, NULL, TO_ROOM );
   act( "@@NA shimmering halo appears, and a @@yHoly @@WAvenger@@N steps from the light!!", ch, obj, NULL, TO_CHAR );

   summoned = create_mobile( get_mob_index( MOB_VNUM_HOLY_AVENGER ) );
   char_to_room( summoned, ch->in_room );
   act( "$n steps from the light, drawing all the power into it's body.", summoned, NULL, NULL, TO_ROOM );

   /*
    * don't think we need these  
    * summoned->level    = 40;  
    * summoned->max_hit  = dice( 8, 40 );
    * summoned->hit      = summoned->max_hit;
    * summoned->max_move = dice( 10, 40 );
    * summoned->move     = summoned->max_move;  
    * 
    */
   summoned->act.set(ACT_PET );
   SET_BIT( summoned->affected_by, AFF_CHARM );
   summoned->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   add_follower( summoned, ch );
   return TRUE;
}

bool spell_diamond_golem( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *summoned;

   act( "@@N$n calls upon the @@aalchemical@@N forces of @@ylight@@N!", ch, obj, NULL, TO_ROOM );
   act( "@@NYou call upon the @@aalchemical@@N forces of @@ylight@@N.", ch, obj, NULL, TO_CHAR );

   act( "A huge gemstone appears, and a golem steps from the diamond!!", ch, obj, NULL, TO_ROOM );
   act( "A huge gemstone appears, and a golem steps from the diamond!!", ch, obj, NULL, TO_CHAR );

   summoned = create_mobile( get_mob_index( MOB_VNUM_DIAMOND_GOLEM ) );
   char_to_room( summoned, ch->in_room );
   act( "$n floods from the pool, drawing all the water into it's body.", summoned, NULL, NULL, TO_ROOM );

   /*
    * don't think we need these  
    * summoned->level    = 40;  
    * summoned->max_hit  = dice( 8, 40 );
    * summoned->hit      = summoned->max_hit;
    * summoned->max_move = dice( 10, 40 );
    * summoned->move     = summoned->max_move;  
    * 
    */
   summoned->act.set(ACT_PET );
   SET_BIT( summoned->affected_by, AFF_CHARM );
   summoned->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   add_follower( summoned, ch );
   return TRUE;
}

bool spell_summon_pegasus( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *summoned;

   act( "$n calls upon the holy forces of @@Wlight@@N!", ch, obj, NULL, TO_ROOM );
   act( "You call upon the holy forces of @@Wlight@@N.", ch, obj, NULL, TO_CHAR );

   act( "@@NA shimmering pyramid appears, and a @@mMagestic @@WPegasus@@N steps from the light!!", ch, obj, NULL, TO_ROOM );
   act( "@@NA shimmering pyramid appears, and a @@mMagestic @@WPagsus@@N steps from the light!!", ch, obj, NULL, TO_CHAR );

   summoned = create_mobile( get_mob_index( MOB_VNUM_PEGASUS ) );
   char_to_room( summoned, ch->in_room );
   act( "$n steps from the light, drawing all the power into it's body.", summoned, NULL, NULL, TO_ROOM );

   /*
    * don't think we need these  
    * summoned->level    = 40;  
    * summoned->max_hit  = dice( 8, 40 );
    * summoned->hit      = summoned->max_hit;
    * summoned->max_move = dice( 10, 40 );
    * summoned->move     = summoned->max_move;  
    * 
    */
   summoned->act.set(ACT_PET );
   SET_BIT( summoned->affected_by, AFF_CHARM );
   summoned->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   add_follower( summoned, ch );
   return TRUE;
}

bool spell_summon_nightmare( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *summoned;

   act( "$n calls upon the unholy forces of @@eEvil@@N!", ch, obj, NULL, TO_ROOM );
   act( "You call upon the unholy forces of @@eEvil@@N.", ch, obj, NULL, TO_CHAR );

   act( "@@NA flaming pit appears, and a @@eDemonic @@dNightmare@@N steps from the heat!!", ch, obj, NULL, TO_ROOM );
   act( "@@NA flaming pit appears, and a @@eDemonic @@dNightmare@@N steps from the heat!!", ch, obj, NULL, TO_CHAR );

   summoned = create_mobile( get_mob_index( MOB_VNUM_NIGHTMARE ) );
   char_to_room( summoned, ch->in_room );
   act( "$n steps from the flames, drawing all the power into it's body.", summoned, NULL, NULL, TO_ROOM );

   /*
    * don't think we need these  
    * summoned->level    = 40;  
    * summoned->max_hit  = dice( 8, 40 );
    * summoned->hit      = summoned->max_hit;
    * summoned->max_move = dice( 10, 40 );
    * summoned->move     = summoned->max_move;  
    * 
    */
   summoned->act.set(ACT_PET );
   SET_BIT( summoned->affected_by, AFF_CHARM );
   summoned->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   add_follower( summoned, ch );
   return TRUE;
}

bool spell_summon_beast( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *summoned;

   act( "$n calls upon the elemental forces of @@lAir@@N!", ch, obj, NULL, TO_ROOM );
   act( "You call upon the elemental forces of @@'Air@@N.", ch, obj, NULL, TO_CHAR );

   act( "@@NA whirlwind appears, and an @@lElemental @@aBeast@@N steps from the winds!!", ch, obj, NULL, TO_ROOM );
   act( "@@NA whirlwind appears, and an @@lElemental @@aBeast@@N steps from the winds!!", ch, obj, NULL, TO_CHAR );

   summoned = create_mobile( get_mob_index( MOB_VNUM_ELEM_BEAST ) );
   char_to_room( summoned, ch->in_room );
   act( "$n steps from the whirlwind, drawing all the power into it's body.", summoned, NULL, NULL, TO_ROOM );

   /*
    * don't think we need these  
    * summoned->level    = 40;  
    * summoned->max_hit  = dice( 8, 40 );
    * summoned->hit      = summoned->max_hit;
    * summoned->max_move = dice( 10, 40 );
    * summoned->move     = summoned->max_move;  
    * 
    */
   summoned->act.set(ACT_PET );
   SET_BIT( summoned->affected_by, AFF_CHARM );
   summoned->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   add_follower( summoned, ch );
   return TRUE;
}

bool spell_summon_devourer( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *summoned;

   act( "$n calls upon the psychic forces of the @@rMind@@N!", ch, obj, NULL, TO_ROOM );
   act( "You call upon the psychic forces of the @@rMind@@N!", ch, obj, NULL, TO_CHAR );

   act( "@@NA vortex appears, and an @@rIntellect @@bDevourer@@N steps from the circle!!", ch, obj, NULL, TO_ROOM );
   act( "@@NA vortex appears, and an @@rIntellect @@bDevourer@@N steps from the circle!!", ch, obj, NULL, TO_CHAR );

   summoned = create_mobile( get_mob_index( MOB_VNUM_INT_DEVOURER ) );
   char_to_room( summoned, ch->in_room );
   act( "$n steps from the vortex, drawing all the power into it's body.", summoned, NULL, NULL, TO_ROOM );

   /*
    * don't think we need these  
    * summoned->level    = 40;  
    * summoned->max_hit  = dice( 8, 40 );
    * summoned->hit      = summoned->max_hit;
    * summoned->max_move = dice( 10, 40 );
    * summoned->move     = summoned->max_move;  
    * 
    */
   summoned->act.set(ACT_PET );
   SET_BIT( summoned->affected_by, AFF_CHARM );
   summoned->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   add_follower( summoned, ch );
   return TRUE;
}

bool spell_summon_shadow( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *summoned;

   act( "$n calls upon the shadowy forces of @@dDarkness@@N!", ch, obj, NULL, TO_ROOM );
   act( "You call upon the shadowy forces of @@dDarkness@@N!", ch, obj, NULL, TO_CHAR );

   act( "@@NA tunnel of shadow appears, and a @@dShadow @@RHound@@N steps from the depths!!", ch, obj, NULL, TO_ROOM );
   act( "@@NA tunnel of shadow appears, and a @@dShadow @@RHound@@N steps from the depths!!", ch, obj, NULL, TO_CHAR );

   summoned = create_mobile( get_mob_index( MOB_VNUM_SHADOW_HOUND ) );
   char_to_room( summoned, ch->in_room );
   act( "$n steps from the tunnel, drawing all the power into it's body.", summoned, NULL, NULL, TO_ROOM );

   /*
    * don't think we need these  
    * summoned->level    = 40;  
    * summoned->max_hit  = dice( 8, 40 );
    * summoned->hit      = summoned->max_hit;
    * summoned->max_move = dice( 10, 40 );
    * summoned->move     = summoned->max_move;  
    * 
    */
   summoned->act.set(ACT_PET );
   SET_BIT( summoned->affected_by, AFF_CHARM );
   summoned->npcdata->extract_timer = ch->get_level("psuedo") / 3;
   add_follower( summoned, ch );
   return TRUE;
}

bool spell_lava_burst( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   float save_mod = 1.0;
   float dam;

   if( saves_spell( level, victim ) )
      save_mod = .75;

   if( number_range( 0, 100 ) < 50 )
   {
      OBJ_DATA *heated_item = NULL;
      OBJ_DATA *prev_carried = NULL;

      for( heated_item = victim->first_carry; heated_item != NULL; heated_item = heated_item->next_in_carry_list )
      {
         if( heated_item->wear_loc == WEAR_NONE )
            continue;
         prev_carried = heated_item;
         if( number_range( 0, 100 ) < 25 )
            continue;
         else
            break;

      }
      if( heated_item == NULL )
      {
         if( prev_carried != NULL )
         {
            SET_BIT( prev_carried->item_apply, ITEM_APPLY_HEATED );
            return TRUE;
         }
         else
         {
            return TRUE;
         }
      }
      else
      {
         SET_BIT( heated_item->item_apply, ITEM_APPLY_HEATED );
         return TRUE;
      }

   }

   dam = number_range( (int)(ch->get_level("psuedo") * 2), (int)(ch->get_level("psuedo") * 4));
   dam *= save_mod;

   sp_damage( obj, ch, ( CHAR_DATA * ) vo, (int)dam, REALM_FIRE, sn, TRUE );

   return TRUE;



}

bool spell_heat_armor( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;


   if( saves_spell( level, victim ) && ( number_range( 0, 99 ) < 50 ) )
      return TRUE;
   {
      OBJ_DATA *heated_item = NULL;
      OBJ_DATA *prev_carried = NULL;

      for( heated_item = victim->first_carry; heated_item != NULL; heated_item = heated_item->next_in_carry_list )
      {
         if( heated_item->wear_loc == WEAR_NONE )
            continue;
         prev_carried = heated_item;
         if( number_range( 0, 100 ) < 25 )
            continue;
         else
            break;

      }
      if( heated_item == NULL )
      {
         if( prev_carried != NULL )
         {
            if( ( !IS_NPC( victim ) && IS_WOLF( victim ) ) && ( !IS_RAGED( victim ) && !IS_SHIFTED( victim ) ) )

               SET_BIT( prev_carried->item_apply, ITEM_APPLY_HEATED );
            return TRUE;
         }
         else
         {
            return TRUE;
         }
      }
      else
      {
         if( ( !IS_NPC( victim ) && IS_WOLF( victim ) ) && ( !IS_RAGED( victim ) && !IS_SHIFTED( victim ) ) )

            SET_BIT( heated_item->item_apply, ITEM_APPLY_HEATED );
         return TRUE;
      }

   }


   return TRUE;



}

bool spell_retri_strike( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *vch;
   OBJ_DATA *staff_obj = NULL;
   list<CHAR_DATA *>::iterator li;

   if( ( staff_obj = get_eq_char( ch, WEAR_HOLD_HAND_R ) ) == NULL )
      if( ( staff_obj = get_eq_char( ch, WEAR_HOLD_HAND_L ) ) == NULL )
      {
         send_to_char( "You must be holding a @@rstaff@@N for this spell!\r\n", ch );
         return FALSE;
      }

   if( staff_obj->item_type != ITEM_STAFF )
   {
      send_to_char( "That is not a staff you are holding, you fool!!\r\n", ch );
      return FALSE;
   }


   if( obj == NULL )
   {
      send_to_char( "The earth trembles beneath your feet!\r\n", ch );
      act( "$n makes the earth tremble and shiver.", ch, NULL, NULL, TO_ROOM );
   }
   else
   {
      act( "$p vibrates violently, making the earth tremble!", ch, obj, NULL, TO_CHAR );
      act( "$p vibrates violenty, making the earth around $n tremble!", ch, obj, NULL, TO_ROOM );
   }

   for( li = ch->in_room->area->player_list.begin(); li != ch->in_room->area->player_list.end(); li++ )
   {
      vch = *li;
      if( vch->in_room == NULL )
         continue;
      if( vch->in_room == ch->in_room )
      {
         if( vch != ch )
         {
            OBJ_DATA *heated_item = NULL;
            OBJ_DATA *prev_carried = NULL;

            act( "@@W$n @@Wis @@eimmolated@@N by the fury released from the $p@@W, and falls to the ground!", vch, staff_obj,
                 NULL, TO_ROOM );
            act( "@@WYou are @@eimmolated@@N by the fury released from the $p@@W, and fall to the ground!", vch, staff_obj,
                 NULL, TO_CHAR );
            if( !sp_damage
                ( obj, ch, vch, number_range( staff_obj->level * 6, staff_obj->level * 10 ), REALM_LIGHT, sn, FALSE ) )
               continue;
            for( heated_item = vch->first_carry; heated_item != NULL; heated_item = heated_item->next_in_carry_list )
            {
               if( heated_item->wear_loc == WEAR_NONE )
                  continue;
               prev_carried = heated_item;
               if( number_range( 0, 100 ) < 25 )
                  continue;
               else
                  break;

            }
            if( heated_item == NULL )
            {
               if( prev_carried != NULL )
               {
                  if( ( !IS_NPC( vch ) && IS_WOLF( vch ) ) && ( !IS_RAGED( vch ) && !IS_SHIFTED( vch ) ) )
                     SET_BIT( prev_carried->item_apply, ITEM_APPLY_HEATED );

               }

            }
            else
            {
               if( ( !IS_NPC( vch ) && IS_WOLF( vch ) ) && ( !IS_RAGED( vch ) && !IS_SHIFTED( vch ) ) )
                  SET_BIT( heated_item->item_apply, ITEM_APPLY_HEATED );
            }
         }
         continue;
      }
      else
      {
         act( "@@W$n @@Wis protected by the fury of the @@W$p@@W.", vch, staff_obj, NULL, TO_ROOM );
         continue;
      }

      send_to_char( "@@WSuddenly, a @@ybright flash@@W sears your eyes, then is gone.@@N\r\n", vch );
   }

   extract_obj( staff_obj );
   return TRUE;
}

bool spell_creature_bond( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   CHAR_DATA *master;
   if( IS_NPC( ch ) )
   {
      send_to_char( "Not for Npcs.\r\n", ch );
      return FALSE;
   }

   if( ( !IS_NPC( victim ) ) || ( !IS_SET( victim->affected_by, AFF_CHARM ) ) )
   {
      send_to_char( "That is not a charmed creature!\r\n", ch );
      return FALSE;
   }
   master = victim->master ? victim->master : victim;
   if( ( master != NULL ) && ( ch->get_level("psuedo") < ( master->get_level("psuedo") - 20 ) ) )
   {
      send_to_char( "The current bond is too strong for you to overcome.\r\n", ch );
      return FALSE;
   }

   if( number_range( 0, 105 ) < ( level + ( ch->get_level("psuedo") - master->get_level("psuedo") ) ) )
   {
      if( saves_spell( level, victim ) )
      {
         do_say( victim, "How dare you!, I LOVE my master!" );
         one_hit( victim, ch, TYPE_UNDEFINED );
         return TRUE;
      }
      stop_follower( victim );
      victim->act.set(ACT_PET );
      SET_BIT( victim->affected_by, AFF_CHARM );
      victim->npcdata->extract_timer = ch->get_level("psuedo") / 3;
      add_follower( victim, ch );
   }
   else
   {
      do_say( victim, "How dare you!, I LOVE my master!" );
      one_hit( victim, ch, TYPE_UNDEFINED );
      return TRUE;
   }

   return TRUE;
}

bool spell_corrupt_bond( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   CHAR_DATA *master = NULL;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Not for Npcs.\r\n", ch );
      return FALSE;
   }

   if( ( !IS_NPC( victim ) ) || ( !IS_SET( victim->affected_by, AFF_CHARM ) ) )
   {
      send_to_char( "That is not a charmed creature!\r\n", ch );
      return FALSE;
   }
   master = victim->master ? victim->master : victim;
   if( ( master != NULL ) && ( ch->get_level("psuedo") < ( master->get_level("psuedo") - 20 ) ) )
   {
      send_to_char( "The current bond is too strong for you to corrupt.\r\n", ch );
      return FALSE;
   }

   if( number_range( 0, 105 ) < ( level + ( ch->get_level("psuedo") - master->get_level("psuedo") ) ) )
   {
      if( saves_spell( level, victim ) )
      {
         do_say( victim, "How dare you!, I LOVE my master!" );
         one_hit( victim, ch, TYPE_UNDEFINED );
         return TRUE;
      }

      stop_follower( victim );

      if( victim->in_room == master->in_room )
      {
         do_say( victim, "Now I shall have my revenge for being charmed!!!" );
         one_hit( victim, master, TYPE_UNDEFINED );
         return TRUE;
      }
      else
      {
         do_say( victim, "AARRGH!  I HATE being charmed! Now I shall have my revenge!" );
         victim->hunt_flags = HUNT_WORLD | HUNT_OPENDOOR | HUNT_PICKDOOR | HUNT_UNLOCKDOOR | HUNT_INFORM;
         set_hunt( victim, NULL, master, NULL, HUNT_WORLD | HUNT_OPENDOOR | HUNT_PICKDOOR | HUNT_UNLOCKDOOR | HUNT_INFORM,
                   HUNT_MERC | HUNT_CR );
         return TRUE;
      }
   }
   else
   {
      do_say( victim, "How dare you!, I LOVE my master!" );
      one_hit( victim, ch, TYPE_UNDEFINED );
      return TRUE;
   }

   return TRUE;
}

bool spell_fireblast( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;


   if( obj == NULL )
   {
      dam = ( ch->get_level("psuedo") / 2 )
         + dice( ( ch->get_level("psuedo") / 6 + ch->get_level("adept") / 2 ), 10 )
         + dice( ( ch->get_level("psuedo") / 6 + ch->get_level("adept") / 2 ), 10 );
      act( "@@gA blast of @@efire@@g flies from $n's hands!@@N", ch, NULL, NULL, TO_ROOM );
      send_to_char( "@@gA blast of @@efire@@g flies from your hands!@@N\r\n", ch );
   }
   else
   {
      dam = dice( level / 4, 15 );
      act( "@@gA blast of @@efire@@g flies from $p!@@N", ch, obj, NULL, TO_ROOM );
      act( "@@gA blast of @@efire@@g flies from $p!@@N", ch, obj, NULL, TO_CHAR );
   }
   if( saves_spell( level, victim ) )
      dam /= 2;
   act( "@@g$n is struck by the blast of @@efire@@g!!@@N", victim, NULL, NULL, TO_ROOM );
   send_to_char( "@@gYou are struck by the @@efire @@gblast!!@@N\r\n", victim );
   sp_damage( obj, ch, victim, dam, REALM_FIRE, sn, TRUE );
   return TRUE;
}

bool spell_shockstorm( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;


   if( obj == NULL )
   {
      dam = ( ch->get_level("psuedo") / 2 )
         + dice( ( ch->get_level("psuedo") / 5 + ch->get_level("adept") / 2 ), 10 )
         + dice( ( ch->get_level("psuedo") / 5 + ch->get_level("adept") / 2 ), 10 );
      act( "@@gA storm of @@lsparks@@g flies from $n's hands!@@N", ch, NULL, NULL, TO_ROOM );
      send_to_char( "@@gA storm of @@lsparks@@g flies from your hands!@@N\r\n", ch );
   }
   else
   {
      dam = dice( level / 4, 20 );
      act( "@@gA storm of @@lsparks@@g flies from $p!@@N", ch, obj, NULL, TO_ROOM );
      act( "@@gA storm of @@lsparks@@g flies from $p!@@N", ch, obj, NULL, TO_CHAR );
   }
   if( saves_spell( level, victim ) )
      dam /= 2;
   act( "@@g$n is struck by the storm of @@lsparks@@g!!@@N", victim, NULL, NULL, TO_ROOM );
   send_to_char( "@@gYou are struck by the storm of @@lsparks@@g!!@@N\r\n", victim );
   sp_damage( obj, ch, victim, dam, REALM_SHOCK, sn, TRUE );
   return TRUE;
}

bool spell_cone_cold( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;


   if( obj == NULL )
   {
      dam = ( ch->get_level("psuedo") / 2 )
         + dice( ( ch->get_level("psuedo") / 5 + ch->get_level("adept") / 2 ), 10 )
         + dice( ( ch->get_level("psuedo") / 5 + ch->get_level("adept") / 2 ), 10 );
      act( "@@gA cone of @@acold@@g bursts forth from $n's hands!@@N", ch, NULL, NULL, TO_ROOM );
      send_to_char( "@@gA cone of @@acold@@g bursts forth from your hands!@@N\r\n", ch );
   }
   else
   {
      dam = dice( level / 4, 20 );
      act( "@@gA cone of @@acold@@g bursts forth from $p!@@N", ch, obj, NULL, TO_ROOM );
      act( "@@gA cone of @@acold@@g bursts forth from $p!@@N", ch, obj, NULL, TO_CHAR );
   }
   if( saves_spell( level, victim ) )
      dam /= 2;
   act( "@@g$n is struck by the cone of @@acold@@g!!@@N", victim, NULL, NULL, TO_ROOM );
   send_to_char( "@@gYou are struck by the cone of @@acold@@g!!@@N\r\n", victim );
   sp_damage( obj, ch, victim, dam, REALM_COLD, sn, TRUE );
   return TRUE;
}

bool spell_holy_wrath( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;


   if( obj == NULL )
   {
      dam = ( ch->get_level("psuedo") / 2 )
         + dice( ( ch->get_level("psuedo") / 4 + ch->get_level("adept") ), 12 )
         + dice( ( ch->get_level("psuedo") / 4 + ch->get_level("adept") ), 12 );
      act( "@@gA coruscating sphere of @@ylight@@g bursts forth from $n's hands!@@N", ch, NULL, NULL, TO_ROOM );
      send_to_char( "@@gA coruscating sphere of @@ylight@@g bursts forth from your hands!@@N\r\n", ch );
   }
   else
   {
      dam = dice( level / 5, 20 );
      act( "@@gA coruscating sphere of @@ylight@@g bursts forth from $p!@@N", ch, obj, NULL, TO_ROOM );
      act( "@@gA coruscating sphere of @@ylight@@g bursts forth from $p!@@N", ch, obj, NULL, TO_CHAR );
   }
   if( saves_spell( level, victim ) )
      dam /= 2;
   act( "@@g$n is struck by the coruscating sphere of @@ylight@@g!!@@N", victim, NULL, NULL, TO_ROOM );
   send_to_char( "@@gYou are struck by the coruscating sphere of @@ylight@@g!!@@N\r\n", victim );
   sp_damage( obj, ch, victim, dam, REALM_HOLY, sn, TRUE );
   return TRUE;
}

bool spell_wraith_touch( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;
   float drain_mod = 1.0;

   if( obj == NULL )
   {
      dam = ( ch->get_level("psuedo") / 3 )
         + dice( ( ch->get_level("psuedo") / 6 + ch->get_level("adept") ), 8 )
         + dice( ( ch->get_level("psuedo") / 6 + ch->get_level("adept") ), 8 );
      act( "@@RA @@dwraithlike hand @@Rleaps forth from $n!@@N", ch, NULL, NULL, TO_ROOM );
      send_to_char( "@@RA @@dwraithlike hand @@Rleaps forth from your hands!@@N\r\n", ch );
   }
   else
   {
      dam = dice( level / 5, 20 );
      act( "@@RA @@dwraithlike hand @@Rleaps forth from $p!@@N", ch, obj, NULL, TO_ROOM );
      act( "@@RA @@dwraithlike hand @@Rleaps forth from $p!@@N", ch, obj, NULL, TO_CHAR );
   }
   if( saves_spell( level, victim ) )
      dam /= 2;
   act( "@@R$n is struck by the @@dwraithlike hand @@R!!@@N", victim, NULL, NULL, TO_ROOM );
   send_to_char( "@@RYou are struck by a @@dwraithlike hand @@R!!@@N\r\n", victim );
   drain_mod = ch->get_level("psuedo") * dam / 130;
   if( sp_damage( obj, ch, victim, dam, REALM_DRAIN | NO_REFLECT | NO_ABSORB, sn, TRUE ) )
      ch->hit = UMIN( ch->max_hit, (int)( ch->hit + drain_mod ) );
   return TRUE;
}

bool spell_thought_vise( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   int dam;


   if( obj == NULL )
   {
      dam = ( ch->get_level("psuedo") / 2 )
         + dice( ( ch->get_level("psuedo") / 3 + ch->get_level("adept") ), 10 )
         + dice( ( ch->get_level("psuedo") / 3 + ch->get_level("adept") ), 10 );
      act( "@@rA crushing weight brushes your mind, then is gone.@@N", ch, NULL, NULL, TO_ROOM );
      send_to_char( "@@rYou clench your mind, crushing the thoughts of your foe.\r\n@@N", ch );
   }
   else
   {
      dam = dice( level / 5, 20 );
      act( "@@rA crushing weight fills your mind.@@N", ch, obj, NULL, TO_CHAR );
   }
   if( saves_spell( level, victim ) )
      dam /= 2;
   act( "@@rA crushing weight brushes your mind, then is gone.@@N", victim, NULL, NULL, TO_ROOM );
   send_to_char( "@@rA crushing weight fills your mind.@@N", victim );
   sp_damage( obj, ch, victim, dam, REALM_MIND | NO_REFLECT | NO_ABSORB, sn, FALSE );
   return TRUE;
}

bool spell_black_curse( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( IS_AFFECTED( victim, AFF_CURSE ) )
   {
      send_to_char( "They are already weakened!\r\n", ch );
      return FALSE;
   }
   if( saves_spell( level, victim ) )
      return TRUE;
   if( saves_spell( level, victim ) )
      return TRUE;
   af.type = sn;
   af.duration = 2 * ( level / 8 );
   af.location = APPLY_HITROLL;
   af.modifier = -1 * ch->get_level("psuedo") / 12 * victim->get_level("psuedo") / 10;
   af.bitvector = AFF_CURSE;
   affect_to_char( victim, &af );

   af.location = APPLY_AC;
   af.modifier = 7 * ch->get_level("psuedo") / 12 * victim->get_level("psuedo") / 10;
   affect_to_char( victim, &af );

   send_to_char( "@@RA Cloud of @@dDespair@@R washes over you.@@N\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}


bool spell_cloak_misery( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   AFFECT_DATA af;


   if( is_affected( ch, sn ) )
      return FALSE;



   af.type = sn;
   af.duration = ch->get_level("psuedo") / 8;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char( ch, &af );

   return TRUE;
}


bool spell_poison_quinine( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, sn ) )
   {
      send_to_char( "They are already poisoned with quinine!\r\n", ch );
      return FALSE;
   }
   if( saves_spell( level, victim ) )
      if( saves_spell( level, victim ) )
         return TRUE;
   af.type = sn;
   af.duration = 2 * ( level / 8 );
   af.location = APPLY_HITROLL;
   af.modifier = -1 * ch->get_level("psuedo") / 20 * victim->get_level("psuedo") / 20;
   af.bitvector = AFF_POISON;
   affect_to_char( victim, &af );

   af.location = APPLY_AC;
   af.modifier = 7 * ch->get_level("psuedo") / 20 * victim->get_level("psuedo") / 20;
   affect_to_char( victim, &af );

   send_to_char( "@@You feel a dart pierce your neck.@@N\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}

bool spell_poison_arsenic( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;

   if( is_affected( victim, sn ) )
   {
      send_to_char( "They are already poisoned with arsenic!\r\n", ch );
      return FALSE;
   }
   if( saves_spell( level, victim ) )
      if( saves_spell( level, victim ) )
         return TRUE;
   af.type = sn;
   af.duration = 2 * ( level / 8 );
   af.location = APPLY_DAMROLL;
   af.modifier = -1 * ch->get_level("psuedo") / 20 * victim->get_level("psuedo") / 20;
   af.bitvector = AFF_POISON;
   affect_to_char( victim, &af );

   af.location = APPLY_AC;
   af.modifier = 5 * ch->get_level("psuedo") / 20 * victim->get_level("psuedo") / 20;
   affect_to_char( victim, &af );

   send_to_char( "@@You feel a dart pierce your neck.@@N\r\n", victim );
   if( ch != victim )
      send_to_char( "Ok.\r\n", ch );
   return TRUE;
}

bool spell_sonic_blast( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{
   CHAR_DATA *victim = ( CHAR_DATA * ) vo;
   AFFECT_DATA af;
   int dam;
   dam = dice( ch->get_level("psuedo") / 2, 3 ) + dice( ch->get_level("psuedo") / 2, 3 );
   if( saves_spell( level, victim ) )
      dam = dam * 2 / 3;
   if( sp_damage( obj, ch, victim, dam, REALM_SOUND | NO_REFLECT | NO_ABSORB, sn, TRUE ) )
   {
      if( is_affected( ch, sn ) )
         return TRUE;
      af.type = sn;
      af.duration = ( level / 20 );
      af.location = APPLY_DAMROLL;
      af.modifier = -1 * ch->get_level("psuedo") / 15 * victim->get_level("psuedo") / 15;
      af.bitvector = AFF_BLASTED;
      affect_to_char( victim, &af );

      af.type = sn;
      af.duration = ( level / 20 );
      af.location = APPLY_HITROLL;
      af.modifier = -1 * ch->get_level("psuedo") / 15 * victim->get_level("psuedo") / 15;
      af.bitvector = AFF_BLASTED;
      affect_to_char( victim, &af );

   }
   return TRUE;
}

bool spell_mystical_focus( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj )
{

   AFFECT_DATA af;


   if( is_affected( ch, sn ) )
      return FALSE;



   af.type = sn;
   af.duration = 1 + (ch->get_level("psuedo") / 7);
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char( ch, &af );

   return TRUE;
}

bool spell_redemption( int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj )
{
 CHAR_DATA *victim = (CHAR_DATA *) vo;

 if( ch == victim )
 {
  send_to_char("You can't redeem yourself!\r\n",ch);
  return false;
 }
 if( !IS_GHOST(victim) )
 {
  send_to_char("You can't redeem someone who isn't dead!\r\n",ch);
  return false;
 }

 act("A beam of holy light emanates from $n's hands, engulfing the body of $N!", ch, NULL, victim, TO_NOTVICT );
 act("A beam of holy light emanates from your hands, engulfing the body of $N!", ch, NULL, victim, TO_CHAR );
 act("A beam of holy light emanates from $n's hands, engulfing your body!", ch, NULL, victim, TO_VICT );

 resurrect(victim);
 victim->hit += number_range(level*2,level*6);
 victim->mana += number_range(level*2,level*6);
 victim->move += number_range(level*2,level*6);
 victim->gain_exp(victim->exp > 0 ? victim->exp / 4 : 1);
 send_to_char("You have gained insight from your death, and regaiend some of your lost experience!\r\n",victim);
 update_pos(victim);

 return true;
}

void cast_update( void )
{
 list<CHAR_DATA *>::iterator li;
 CHAR_DATA *ch;

 li = cast_list.begin();
 while( li != cast_list.end() )
 {
  ch = *li;
  if( ch == NULL )
  {
   monitor_chan("Removing a null ch from queue.",MONITOR_DEBUG);
   li = cast_list.erase(li);
  }

  /*
   * Speed based casting. Spells fire when done, and stop if
   * interrupted (movement, etc). It's also possible to take knockback. --Kline
   */
  if( ch->casting->time > 0 )
   ch->casting->time -= 0.01;
  if( ch->casting->time <= 0 )
  {
   cast(ch,ch->casting->arg);
   li = cast_list.erase(li);
  }
  else
   ++li;
 }

 return;
}
