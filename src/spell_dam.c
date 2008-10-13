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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"

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

#ifndef DEC_MAGIC_H
#include "magic.h"
#endif

/*
 * This file should contain:
 *	o advanced damage engine
 */


struct sp_dam_str_type sp_dam_str[] = {
/* NONE      */ {REALM_NONE, "@@g", "@@g", "@@W", "", "%s%s%s",
                 "blast", "BLAST", "blasts", "BLASTS"},

/* Fire      */ {REALM_FIRE, "@@R", "@@W", "@@y", "@@2", "%s<%s^%s>",
                 "burn", "INCINERATE", "burns", "INCINERATES"},
/* Shock     */ {REALM_SHOCK, "@@l", "@@W", "@@y", "@@1", "%s~%s-%s~",
                 "shock", "FRY", "shocks", "FRIES"},
/* light     */ {REALM_LIGHT, "@@W", "@@y", "@@c", "@@7", "%s*%s*%s*",
                 "zap", "ZAP", "zaps", "ZAPS"},
/* gas       */ {REALM_GAS, "@@W", "@@y", "@@c", "@@7", "%s(%sO%s)",
                 "choke", "ENVELOP", "chokes", "ENVELOPS"},
/* poison    */ {REALM_POISON, "@@e", "@@r", "@@e", "@@4", "%s{%s^%s}",
                 "weaken", "POISON", "weakens", "POISONS"},
/* cold      */ {REALM_COLD, "@@l", "@@W", "@@y", "@@1", "%s[%s~%s]",
                 "chill", "FREEZE", "chills", "FREEZES"},
/* sound     */ {REALM_SOUND, "@@l", "@@W", "@@y", "@@1", "%s~%s\\/%s~",
                 "blast", "BLAST", "blasts", "BLASTS"},
/* acid      */ {REALM_ACID, "@@G", "@@W", "@@r", "@@3", "%s-%sX%s-",
                 "etch", "DISSOLVE", "etches", "DISSOLVES"},
/* drain     */ {REALM_DRAIN, "@@d", "@@R@@i", "@@2@@e", "@@R@@i", "%s>%s-%s<",
                 "taint", "DRAIN", "taints", "DRAINS"},
/* impact    */ {REALM_IMPACT, "@@b", "@@W", "@@y", "@@4", "%s.%s^%s.",
                 "strike", "SLAM", "strikes", "SLAMS"},
/* mind      */ {REALM_MIND, "@@m", "@@W", "@@p", "@@5", "%s/%sV%s\\",
                 "zap", "BLAST", "zaps", "BLASTS"},
/* holy      */ {REALM_HOLY, "@@l", "@@W", "@@y", "@@1", "%s~%s\\/%s~",
                 "holy", "HOLY", "holies", "HOLIES"}
};

void sp_death_message( CHAR_DATA * ch, CHAR_DATA * victim, int realm )
{
   /*
    * Used to display assorted death messages, based on dt 
    * * max_dt == number of entries in attack table in dam_message,
    * * so we know if kill was unarmed, armed, or through spells or skills.
    * * Stephen
    */

   char buf1[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   char buf3[MAX_STRING_LENGTH];
   int vnum;
   int looper;
   /*
    * For debugging purposes  
    * xprintf( buf, "dt: %d max_dt: %d\n\r", dt, max_dt );
    * notify( buf, 1 );  
    */

   vnum = 0;   /* Used to load up body parts.... eewwwwww! */

   for( looper = 1; looper < 32; looper++ )
   {
      if( IS_SET( realm, ( 1 << looper ) ) )
         break;
   }
   switch ( looper + 1 )
   {
      default:   /* bad realm, use non-specific */
         xprintf( buf1, "$n destroy $N!" );
         xprintf( buf2, "You destroy $N!" );
         xprintf( buf3, "$n destroys you." );
         break;
      case RE_FIRE:
      {
         if( HAS_BODY( victim ) )
         {
            switch ( number_range( 0, 4 ) )
            {
               case 0:
                  xprintf( buf1, "$n cremates $N's body to a crisp!" );
                  xprintf( buf2, "You cremate $N's body to a crispy @@eRED@@N cinder!" );
                  xprintf( buf3, "$n cremates your remains." );
                  break;
               case 1:
                  xprintf( buf1, "$n fries $N's guts; smoke envelops $S corpse!" );
                  xprintf( buf2, "You fry $N's guts; smoke bites your eyes!" );
                  xprintf( buf3, "$n slowly fries your entrails; smoke covers your remains!" );
                  break;
               case 2:
                  xprintf( buf1, "$n leaves gigantic burns on $N's remains. Blood and pus spray out wildly!" );
                  xprintf( buf2, "You leave gigantic burns on $N's remains. Blood and pus spray out wildly!" );
                  xprintf( buf3, "$n leaves gigantic burns on your body. Blood and pus spray out wildly!" );
                  break;
               case 3:
                  xprintf( buf1, "$n burns $N's skin off, leaving a bloody mess!" );
                  xprintf( buf2, "You burn $N's skin off, leaving a bloody mess!" );
                  xprintf( buf3, "$n burns your skin off, leaving a bloody mess!" );
                  break;
               case 4:
                  xprintf( buf1, "$n leaves $N burning in flames. Blood spills all over the floor!" );
                  xprintf( buf2, "You leave $N burning in flames. Blood spills all over the floor!" );
                  xprintf( buf3, "$n leaves you burning in flames. Your skin breaks and blood spills on the floor!" );
                  break;
            }

         }
         else
         {
            xprintf( buf1, "$n sets $N aura on fire!" );
            xprintf( buf2, "You set $N on fire!" );
            xprintf( buf3, "$n sets you on fire!" );
            break;
         }
         break;
      }
      case RE_SHOCK:
      {
         if( HAS_BODY( victim ) )
         {
            switch ( number_range( 0, 2 ) )
            {
               case 0:
                  xprintf( buf1, "$n electrifies $N's to a crisp." );
                  xprintf( buf2, "You electrify $N's body to a crisp!" );
                  xprintf( buf3, "$n electrifies your remains" );
                  break;
               case 1:
                  xprintf( buf1, "$n fries $N. Smoke envelops $S corpse!" );
                  xprintf( buf2, "You fry $N. Smoke bites your eyes!" );
                  xprintf( buf3, "$n sends electricity through your body. You feel your skin burn." );
                  break;
               case 2:
                  xprintf( buf1, "$n lightning shocks $N's body and $S eye balls fall out!" );
                  xprintf( buf2, "You send lightning through $N's body and $S eye balls fall out!" );
                  xprintf( buf3, "$n sends lightning though your body. Your eye sockets pop and your eye balls fall out!" );
                  break;
            }
         }
         else
         {
            xprintf( buf1, "$n electricutes $N." );
            xprintf( buf2, "You electricute $N." );
            xprintf( buf3, "$n electricutes you." );
            break;
         }
         break;
      }
      case RE_LIGHT:
      {
         if( HAS_BODY( victim ) )
         {
            xprintf( buf1, "$n makes $N's corpse glow with unholy light!" );
            xprintf( buf2, "You penetrate $N's body with light!" );
            xprintf( buf3, "$n penetrates your remains with light!" );
            break;
         }
         else
         {
            xprintf( buf1, "$n makes $N glow with unholy light!" );
            xprintf( buf2, "You penetrate $N with light!" );
            xprintf( buf3, "$n penetrates you with light!" );
            break;
         }
         break;
      }
      case RE_GAS:
      {
         if( HAS_BODY( victim ) )
         {
            xprintf( buf1, "$n gasses $N and watches $M choke." );
            xprintf( buf2, "You gas $N and watch $M die." );
            xprintf( buf3, "$n gasses you and watches you die." );
            break;
         }
         else
         {
            xprintf( buf1, "$n gasses $N and watches $M choke." );
            xprintf( buf2, "You gas $N and watch $M die." );
            xprintf( buf3, "$n gasses you and watches you die." );
            break;
         }
         break;
      }
      case RE_POISON:
      {
         if( HAS_BODY( victim ) )
         {
            switch ( number_range( 0, 1 ) )
            {
               case 0:
                  xprintf( buf1, "$n poisons $N ;$S body begins to twist in agony!" );
                  xprintf( buf2, "You poison $N and watch $S body begin twisting in agony!" );
                  xprintf( buf3, "$n poisons you -- you twist in agony." );
                  break;

               case 1:
                  xprintf( buf1, "$n sends a poisonous cloud into $N ;$S mouth starts dripping with blood!" );
                  xprintf( buf2, "You send a poisonous cloud into $N ;$S mouth starts dripping with blood!" );
                  xprintf( buf3, "$n bursts your lungs with his poisonous cloud, blood begins flowing from inside." );
                  break;
            }

         }
         else
         {
            xprintf( buf1, "$n poisons $N ;$S soul begins to twist in agony!" );
            xprintf( buf2, "You poison $N and watch $S soul begin twisting in agony!" );
            xprintf( buf3, "$n poisons you -- you twist in agony." );
            break;
         }
         break;
      }
      case RE_COLD:
      {
         if( HAS_BODY( victim ) )
         {
            switch ( number_range( 0, 0 ) )
            {
               case 0:
                  xprintf( buf1, "$n turns $N into a block of ice." );
                  xprintf( buf2, "You turn $N into a block of ice." );
                  xprintf( buf3, "$n turns you into ice!!" );
                  break;
            }
         }
         else
         {
            xprintf( buf1, "$n freezes $N to death!" );
            xprintf( buf2, "You freeze $N to death!" );
            xprintf( buf3, "$n freezes you to death!" );
            break;
         }
         break;
      }
      case RE_SOUND:
      {
         if( HAS_BODY( victim ) )
         {
            xprintf( buf1, "$n explodes $N's remains with deafening sound!" );
            xprintf( buf2, "You explode $N's remains with deafening sound!" );
            xprintf( buf3, "$n explodes your remains with deafening sound!" );
            break;
         }
         else
         {
            xprintf( buf1, "$n shatters $N with deafening sound!" );
            xprintf( buf2, "You shatter $N with deafening sound!" );
            xprintf( buf3, "$n shatters you with deafening sound!" );
            break;
         }
         break;
      }
      case RE_ACID:
      {
         if( HAS_BODY( victim ) )
         {
            switch ( number_range( 0, 1 ) )
            {
               case 0:
                  xprintf( buf1, "$n sprays acid on $N; $S flesh falls apart!" );
                  xprintf( buf2, "You spray acid on $N; $S flesh falls apart!" );
                  xprintf( buf3, "$n sprays acid on you. Your flesh falls apart!" );
                  break;
               case 1:
                  xprintf( buf1, "$n dissolves $N's corpse into mushy paste." );
                  xprintf( buf2, "You dissolve $N's corpse into mushy paste." );
                  xprintf( buf3, "$n dissolves you into mushy paste!" );
                  break;
            }
         }
         else
         {
            xprintf( buf1, "$n etches $N to death!" );
            xprintf( buf2, "You etch $N to death!" );
            xprintf( buf3, "$n etches you to death!" );
            break;
         }
         break;
      }
      case RE_DRAIN:
      {
         if( HAS_BODY( victim ) )
         {
            xprintf( buf1, "$n drains $N of $S life energy." );
            xprintf( buf2, "You drains $N of $S life energy." );
            xprintf( buf3, "$n drains you of your life energy." );
            break;
         }
         else
         {
            xprintf( buf1, "$n darkens $N soul!" );
            xprintf( buf2, "You darken $N soul!" );
            xprintf( buf3, "$n darkens your soul!" );
            break;
         }
         break;
      }
      case RE_IMPACT:
      {
         if( HAS_BODY( victim ) )
         {
            xprintf( buf1, "$n smashes $N's body into messy flesh." );
            xprintf( buf2, "You smash $N into messy flesh." );
            xprintf( buf3, "$n smashes you into messy flesh!" );
            break;
         }
         else
         {
            xprintf( buf1, "$n crushes $N to death!" );
            xprintf( buf2, "You crush $N to death!" );
            xprintf( buf3, "$n crushes you to death!" );
            break;
         }
         break;
      }
      case RE_MIND:
      {
         if( HAS_MIND( victim ) )
         {
            xprintf( buf1, "$n blasts $N with psionic power!" );
            xprintf( buf2, "You blast $N with psionic power!" );
            xprintf( buf3, "$n blasts you with psionic power!" );
            break;
         }
         else
         {
            xprintf( buf1, "$n blasts $N with psionic power!" );
            xprintf( buf2, "You blast $N with psionic power!" );
            xprintf( buf3, "$n blasts you with psionic power!" );
            break;
         }
         break;
      }
      case RE_HOLY:
      {
         if( HAS_MIND( victim ) )
         {
            xprintf( buf1, "$n steals $N's soul with divine power!" );
            xprintf( buf2, "You steal $N's soul with divine power!" );
            xprintf( buf3, "$n steals your soul with divine power!" );
            break;
         }
         else
         {
            xprintf( buf1, "$n steals $N's soul with divine power!" );
            xprintf( buf2, "You steal $N's soul with divine power!" );
            xprintf( buf3, "$n steals your soul with divine power!" );
            break;
         }
         break;
      }
   }  /* end switch */

   act( buf1, ch, NULL, victim, TO_NOTVICT );
   act( buf2, ch, NULL, victim, TO_CHAR );
   act( buf3, ch, NULL, victim, TO_VICT );

   /*
    * Load up object, if needed. 
    */
   if( vnum != 0 )
   {
      char buf[MAX_STRING_LENGTH];
      OBJ_DATA *obj;
      char *name;

      name = IS_NPC( ch ) ? ch->short_descr : ch->name;
      obj = create_object( get_obj_index( vnum ), 0 );
      obj->timer = number_range( 4, 7 );

      xprintf( buf, obj->short_descr, name );
      free_string( obj->short_descr );
      obj->short_descr = str_dup( buf );

      xprintf( buf, obj->description, name );
      free_string( obj->description );
      obj->description = str_dup( buf );

      obj_to_room( obj, ch->in_room );
   }



   return;
}



/* 
 * assumes a base damage, after initial save checks.
 * will modify damage as it sees fit for weaknesses and strengths.
 *
 */






void sp_dam_message( OBJ_DATA * obj, CHAR_DATA * ch, CHAR_DATA * victim, int dam, int realm, int dt )
{

   char buf1[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH], buf3[MAX_STRING_LENGTH];
   const char *attack;
   char punct;
   int rtype, dam_range;
   bool dead = FALSE;
   char symbuf[MSL];
   char catsymbuf[MSL];
   char outercol[MSL];
   char innercol[MSL];
   bool stress = FALSE;

   if( dam >= victim->hit )
      dead = TRUE;
   if( sysdata.shownumbers == TRUE )
   {
      xprintf_2( testerbuf, " @@l(@@W%i@@l)@@N", dam );
   }
   else
      xprintf_2( testerbuf, "%s", "" );

   for( rtype = 0; rtype < MAX_REALM; rtype++ )
   {
      if( sp_dam_str[rtype].realm == realm )
         break;
   }
   if( rtype == MAX_REALM )
      rtype = 0;



/* notes:
  2000+ stress color    
  1000+ normal color
  0+ no backcolor
  500+ stress verb
  0+ normal verb
  each 50 is a symbol 
*/

   if( dam > 2000 )
   {
      dam_range = dam - 2000;
   }
   else if( dam > 1000 )
   {
      dam_range = dam - 1000;
   }
   else
   {
      dam_range = dam;
   }
   dam_range /= 2;
   if( dam_range > 250 )
   {
      stress = TRUE;
      dam_range -= 250;
   }
   punct = ( !stress ) ? '.' : '!';
   xprintf( outercol, "%s%s",
            ( dam > 250 ? sp_dam_str[rtype].backcol : "" ),
            ( dam > 250 ? sp_dam_str[rtype].invertcol : sp_dam_str[rtype].basecol ) );
   xprintf( innercol, "%s%s",
            ( dam > 250 ? sp_dam_str[rtype].backcol : "" ),
            ( dam > 500 ? sp_dam_str[rtype].stresscol :
              ( dam > 250 ? sp_dam_str[rtype].invertcol : sp_dam_str[rtype].basecol ) ) );

   catsymbuf[0] = '\0';
   xprintf( symbuf, "%s", " " );
   for( ; dam_range > 0; dam_range -= 50 )
   {
      xprintf( catsymbuf, sp_dam_str[rtype].formatter, outercol, innercol, outercol );
      xcat( symbuf, catsymbuf );
   }
   xcat( symbuf, " " );



   if( dt >= 0 && dt < MAX_SKILL )
      attack = skill_table[dt].noun_damage;
   else
   {
      bug( "Dam_message: bad dt %d.", dt );
      rtype = 0;
      attack = "!!!!!";
   }
   if( obj == NULL )
   {
      xprintf( buf1, "%s$n %s%s%s@@N $N %swith $s %s%c@@N%s",
               sp_dam_str[rtype].basecol,
               symbuf,
               ( stress ? sp_dam_str[rtype].vp_stress : sp_dam_str[rtype].vp ),
               symbuf, sp_dam_str[rtype].basecol, attack, punct, testerbuf );
      xprintf( buf2, "%sYou %s%s%s@@N $N %swith your %s%c@@N%s",
               sp_dam_str[rtype].basecol,
               symbuf,
               ( stress ? sp_dam_str[rtype].vs_stress : sp_dam_str[rtype].vs ),
               symbuf, sp_dam_str[rtype].basecol, attack, punct, testerbuf );
/*	   if ( *str == '\'' )
	    xprintf( buf3, "%s$n %s%s your%s%s $s %s%c@@N",col,col, vp,col, str+2, attack, punct);
	   else
*/
      xprintf( buf3, "%s$n %s%s%s %syou with $s %s%c@@N%s",
               sp_dam_str[rtype].basecol,
               symbuf,
               ( stress ? sp_dam_str[rtype].vp_stress : sp_dam_str[rtype].vp ),
               symbuf, sp_dam_str[rtype].basecol, attack, punct, testerbuf );
   }
   else if( obj != NULL )
   {
      xprintf( buf1, "%s$p %s%s%s@@N $N %swith a %s%c@@N",
               sp_dam_str[rtype].basecol,
               symbuf,
               ( stress ? sp_dam_str[rtype].vp_stress : sp_dam_str[rtype].vp ),
               symbuf, sp_dam_str[rtype].basecol, attack, punct );
#if 0
      xprintf( buf2, "%sYou %s%s%s@@N $N %swith your %s%c@@N%s",
               sp_dam_str[rtype].basecol,
               symbuf,
               ( stress ? sp_dam_str[rtype].vs_stress : sp_dam_str[rtype].vs ),
               symbuf, sp_dam_str[rtype].basecol, attack, punct, testerbuf );
#endif
/*	   if ( *str == '\'' )
	    xprintf( buf3, "%s$n %s%s your%s%s $s %s%c@@N",col,col, vp,col, str+2, attack, punct);
	   else
*/
      xprintf( buf3, "%s$p %s%s%s %syou with $s %s%c@@N",
               sp_dam_str[rtype].basecol,
               symbuf,
               ( stress ? sp_dam_str[rtype].vp_stress : sp_dam_str[rtype].vp ),
               symbuf, sp_dam_str[rtype].basecol, attack, punct );
   }
   else
   {
      xprintf_2( log_buf, "%s", "Error in casting spell , sp_dam called with NULL obj and ch." );
      monitor_chan( log_buf, MONITOR_DEBUG );
      return;
   }
   if( obj == NULL )
   {
      act( buf1, ch, NULL, victim, TO_NOTVICT );
   }
   else
   {
      act( buf1, NULL, ( void * )obj, victim, TO_NOTVICT );
   }
   if( is_set( ch->act, ACT_BLIND_PLAYER ) )
   {
      if( dam < victim->max_hit / 30 )
         act( "You glance $K", ch, NULL, victim, TO_CHAR );
      else if( dam < victim->max_hit / 20 )
         act( "You hit $K", ch, NULL, victim, TO_CHAR );
      else if( dam < victim->max_hit / 10 )
         act( "You nail $K", ch, NULL, victim, TO_CHAR );
      else
         act( "You thwack $K", ch, NULL, victim, TO_CHAR );
   }
   else if( obj == NULL )
   {
      act( buf2, ch, NULL, victim, TO_CHAR );
   }
   if( is_set( victim->act, ACT_BLIND_PLAYER ) )
   {
      if( dam < victim->max_hit / 30 )
         act( "$k glances you", ch, NULL, victim, TO_VICT );
      else if( dam < victim->max_hit / 20 )
         act( "$k hits you", ch, NULL, victim, TO_VICT );
      else if( dam < victim->max_hit / 10 )
         act( "$k nails you", ch, NULL, victim, TO_VICT );
      else
         act( "$k thwacks you", ch, NULL, victim, TO_VICT );
   }
   else if( obj == NULL )
   {
      act( buf3, ch, NULL, victim, TO_VICT );
   }
   else
   {
      act( buf3, NULL, ( void * )obj, victim, TO_VICT );
   }
   if( dead )
   {
      sp_death_message( ch, victim, realm );
   }
   return;
}



bool sp_damage( OBJ_DATA * obj, CHAR_DATA * ch, CHAR_DATA * victim, int dam, int type, int sn, bool show_msg )
{
   int ch_strong, ch_weak, ch_race, ch_suscept, ch_resist, vi_strong, vi_weak, vi_race, vi_suscept, vi_resist;
   float dam_modifier = 1.0;
   float tmp = 0;
   bool can_reflect = TRUE;
   bool can_absorb = TRUE;

/*   char buf[MAX_STRING_LENGTH];   this is unused now -- uni */


   if( ( victim->is_free == TRUE ) )
      return FALSE;

/*
 *  First, check caster's strengths and weaknesses.
 *
 */

   if( IS_SET( type, NO_REFLECT ) )
   {
      REMOVE_BIT( type, NO_REFLECT );
      can_reflect = FALSE;
   }
   if( IS_SET( type, NO_ABSORB ) )
   {
      REMOVE_BIT( type, NO_ABSORB );
      can_absorb = FALSE;
   }
   if( obj == NULL )
   {
      if( ( can_reflect )
          && ( skill_table[sn].target == TAR_CHAR_OFFENSIVE )
          && ( IS_AFFECTED( victim, AFF_CLOAK_REFLECTION ) )
          && ( ch != victim ) && ( number_percent(  ) < ( get_psuedo_level( victim ) - 70 ) ) )
      {

         act( "@@N$n's @@lc@@el@@ro@@ya@@ak@@N glows brightly as $Nn's spell hits it, and the spell is reflected@@N!!", ch,
              victim, NULL, TO_ROOM );
         act( "@@N$N's @@lc@@el@@ro@@ya@@ak@@N glows brightly, and reflects your spell back on you@@N!!", ch, NULL, victim,
              TO_CHAR );
         act( "@@NYour @@lc@@el@@ro@@ya@@ak@@N glows brightly, and reflects the spell back on $N@@N!!!", victim, NULL, ch,
              TO_CHAR );
         ( *skill_table[sn].spell_fun ) ( sn, 60, ch, ( void * )ch, NULL );
         return FALSE;

      }


      else if( ( can_reflect )
               && ( skill_table[sn].target == TAR_CHAR_OFFENSIVE )
               && ( IS_AFFECTED( victim, AFF_CLOAK_ABSORPTION ) )
               && ( ch != victim ) && ( number_percent(  ) < ( get_psuedo_level( victim ) - 55 ) ) )
      {
         int mana;
         mana = mana_cost( ch, sn );
         victim->mana = UMIN( victim->max_mana, victim->mana + mana );

         act( "@@N$n's @@lcloak@@N glows brightly as $N's spell hits it, then fades@@N!!", victim, NULL, ch, TO_ROOM );
         act( "@@N$N's @@lcloak@@N glows brightly, and absorbs your spell@@N!!", ch, NULL, victim, TO_CHAR );
         act( "@@NYour @@lcloak@@N glows brightly, and absorbs $N's spell@@N!!!", victim, NULL, ch, TO_CHAR );
         return FALSE;
      }




      ch_strong = ( IS_NPC( ch ) ?
                    ( ( ( ch->race > 0 )
                        && ( ch->race < MAX_RACE ) ) ?
                      race_table[ch->race].strong_realms : ch->strong_magic ) : race_table[ch->race].strong_realms );
      ch_resist = ( IS_NPC( ch ) ?
                    ( ( ( ch->race > 0 )
                        && ( ch->race < MAX_RACE ) ) ?
                      race_table[ch->race].resist_realms : ch->resist ) : race_table[ch->race].resist_realms );
      ch_weak = ( IS_NPC( ch ) ?
                  ( ( ( ch->race > 0 )
                      && ( ch->race < MAX_RACE ) ) ?
                    race_table[ch->race].weak_realms : ch->weak_magic ) : race_table[ch->race].weak_realms );
      ch_suscept = ( IS_NPC( ch ) ?
                     ( ( ( ch->race > 0 )
                         && ( ch->race < MAX_RACE ) ) ?
                       race_table[ch->race].suscept_realms : ch->suscept ) : race_table[ch->race].suscept_realms );
      ch_race = ( IS_NPC( ch ) ?
                  ( ( ( ch->race > 0 )
                      && ( ch->race < MAX_RACE ) ) ?
                    race_table[ch->race].race_flags : ch->race_mods ) : race_table[ch->race].race_flags );

      if( IS_SET( ch_strong, type ) )
      {
         dam_modifier += .35;
      }
      else if( IS_SET( ch_weak, type ) )
      {
         dam_modifier -= .35;
      }

      if( IS_SET( ch_race, RACE_MOD_STRONG_MAGIC ) )
      {
         dam_modifier += .25;
      }
      else if( IS_SET( ch_race, RACE_MOD_WEAK_MAGIC ) )
      {
         dam_modifier -= .25;
      }
      else if( IS_SET( ch_race, RACE_MOD_NO_MAGIC ) )
      {
         dam_modifier -= .50;
      }

      if( ch->stance == STANCE_CASTER )
         dam_modifier += .10;
      else if( ch->stance == STANCE_WIZARD )
         dam_modifier += .25;
      else if( ch->stance == STANCE_MAGI )
         dam_modifier += .30;

      if( ( !IS_NPC( ch ) ) && ( !IS_SET( type, REALM_MIND ) ) )
      {
         if( ch->pcdata->learned[gsn_potency] > 0 )
         {
            dam_modifier += ( get_curr_int( ch ) * ch->pcdata->learned[gsn_potency] / 5000 );
         }

         if( ch->pcdata->learned[gsn_thaumatergy] > 0 )
         {
            dam_modifier += ( get_curr_int( ch ) * ch->pcdata->learned[gsn_thaumatergy] / 2500 );
         }
      }
      if( is_affected( ch, skill_lookup( "mystical focus" ) ) )
      {
         dam_modifier += .5;
      }
   }  /* obj == NULL */
   else if( obj->carried_by != NULL )
   {
      ch = obj->carried_by;
   }
   else
   {
      xprintf_2( log_buf, "Error, object %s casting spell, but not carried by anyone.", obj->short_descr );
      monitor_chan( log_buf, MONITOR_DEBUG );
      return FALSE;
   }

/* 
 *  Next, the victim
 *
 */
   vi_strong = ( IS_NPC( victim ) ?
                 ( ( ( victim->race > 0 )
                     && ( victim->race < MAX_RACE ) ) ?
                   race_table[victim->race].strong_realms :
                   victim->strong_magic ) : race_table[victim->race].strong_realms );
   vi_resist = ( IS_NPC( victim ) ?
                 ( ( ( victim->race > 0 )
                     && ( victim->race < MAX_RACE ) ) ?
                   race_table[victim->race].resist_realms : victim->resist ) : race_table[victim->race].resist_realms );
   vi_weak = ( IS_NPC( victim ) ?
               ( ( ( victim->race > 0 )
                   && ( victim->race < MAX_RACE ) ) ?
                 race_table[victim->race].weak_realms : victim->weak_magic ) : race_table[victim->race].weak_realms );
   vi_suscept = ( IS_NPC( victim ) ?
                  ( ( ( victim->race > 0 )
                      && ( victim->race < MAX_RACE ) ) ?
                    race_table[victim->race].suscept_realms : victim->suscept ) : race_table[victim->race].suscept_realms );
   vi_race = ( IS_NPC( victim ) ?
               ( ( ( victim->race > 0 )
                   && ( victim->race < MAX_RACE ) ) ?
                 race_table[victim->race].race_flags : victim->race_mods ) : race_table[victim->race].race_flags );

   if( IS_SET( vi_suscept, type ) )
   {
      dam_modifier += .45;
   }
   else if( IS_SET( vi_resist, type ) )
   {
      dam_modifier -= .45;
   }

   else if( IS_SET( vi_race, RACE_MOD_NO_MAGIC ) )
   {
      dam_modifier -= .25;
   }

   if( MAGIC_STANCE( ch ) )
      dam_modifier += .15;

   if( ( IS_SET( type, REALM_MIND ) ) && ( !HAS_MIND( victim ) ) )
      dam_modifier = 0.0;
   else if( ( ( IS_SET( type, REALM_IMPACT ) )
              || ( IS_SET( type, REALM_ACID ) ) || ( IS_SET( type, REALM_GAS ) ) ) && ( !HAS_BODY( victim ) ) )
      dam_modifier = 0.0;

   if( ( IS_SET( type, REALM_POISON ) ) && ( IS_SET( vi_race, RACE_MOD_IMMUNE_POISON ) ) )
      dam_modifier = 0.0;

   if( ( IS_SET( type, REALM_DRAIN ) ) && ( IS_UNDEAD( victim ) ) )
      dam_modifier = 0.0;

   tmp = dam;
   tmp *= dam_modifier;
   dam = (int)tmp;
   dam += number_range((int)(dam * -0.10),(int)(dam * 0.10)); /* Lets add a little randomness to things. --Kline */

   if( victim != ch )
   {
      /*
       * Certain attacks are forbidden.
       * Most other attacks are returned.
       */
      if( is_safe( ch, victim ) )
         return FALSE;
      if( victim != ch->fighting )
         check_killer( ch, victim );

      if( victim->position > POS_STUNNED )
      {
         if( victim->fighting == NULL )
            set_fighting( victim, ch, FALSE );
         victim->position = POS_FIGHTING;
      }

      if( victim->position > POS_STUNNED )
      {
         if( ch->fighting == NULL )
         {
            set_fighting( ch, victim, TRUE );
         }


         /*
          * If victim is charmed, ch might attack victim's master.
          */
         if( IS_NPC( ch )
             && IS_NPC( victim )
             && IS_AFFECTED( victim, AFF_CHARM )
             && victim->master != NULL && victim->master->in_room == ch->in_room && number_bits( 3 ) == 0 )
         {
            stop_fighting( ch, FALSE );
            one_hit( ch, victim->master, TYPE_UNDEFINED );
         }
      }

      /*
       * More charm stuff.
       */
      if( victim->master == ch )
         stop_follower( victim );

      /*
       * Inviso attacks ... not.
       */
      if( IS_AFFECTED( ch, AFF_INVISIBLE ) )
      {
         affect_strip( ch, gsn_invis );
         affect_strip( ch, gsn_mass_invis );
         REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
         act( "$n shimmers into existence.", ch, NULL, NULL, TO_ROOM );
      }

      /*
       * Damage modifiers.
       */

      if( dam < 0 )
         dam = 0;

     /*
      * Stop up any residual loopholes.
      */
     if( dam > sysdata.damcap )
     {
        char buf[MAX_STRING_LENGTH];
        xprintf( buf, "Spell: %d damage by %s, spell %s", dam,
                 ( obj == NULL ) ? ( IS_NPC( ch ) ? ch->short_descr : ch->name ) : obj->short_descr, skill_table[sn].name );
        if( ch->level < 82 )
         monitor_chan( buf, MONITOR_MAGIC );
        log_f( buf );
        dam = sysdata.damcap;
     }


      if( ( show_msg ) && ( dam >= 0 ) )
         sp_dam_message( obj, ch, victim, dam, type, sn );

   }

   /*
    * Hurt the victim.
    * Inform the victim of his new state.
    */
   victim->hit -= dam;

   if( !IS_NPC(ch) )
   {
    if( dam > ch->pcdata->records->mdam_amt )
    {
     send_to_char("@@yYou've broken your magical damage record!@@N\n\r",ch);
     ch->pcdata->records->mdam_amt = dam;
     ch->pcdata->records->mdam_gsn = sn;
    }
   }

   if( !IS_NPC( victim ) )
      check_adrenaline( victim, dam );

   if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( dam > 350 ) )
      do_rage( victim, "FORCE" );

   update_pos( victim );

   if( ( IS_NPC( victim ) || !IS_VAMP( victim ) ) && !( deathmatch ) )
   {
      switch ( victim->position )
      {
         case POS_MORTAL:
            act( "$n is mortally wounded, and will die soon, if not aided.", victim, NULL, NULL, TO_ROOM );
            send_to_char( "You are mortally wounded, and will die soon, if not aided.\n\r", victim );
            break;

         case POS_INCAP:
            act( "$n is incapacitated and will slowly die, if not aided.", victim, NULL, NULL, TO_ROOM );
            send_to_char( "You are incapacitated and will slowly die, if not aided.\n\r", victim );
            break;

         case POS_STUNNED:
            act( "$n is too stunned to do anything!", victim, NULL, NULL, TO_ROOM );
            send_to_char( "You are too stunned to do anything!\n\r", victim );
            break;

         case POS_DEAD:
            act( "$n is DEAD!!", victim, 0, 0, TO_ROOM );
            send_to_char( "You have been KILLED!!\n\r\n\r", victim );
            break;

         default:
            if( dam > victim->max_hit / 4 )
               send_to_char( "That really did HURT!\n\r", victim );
            if( victim->hit < victim->max_hit / 4 )
               send_to_char( "You sure are BLEEDING!\n\r", victim );
            break;
      }
   }  /* end of if statement */
   /*
    * Sleep spells and extremely wounded folks.
    */
   if( !IS_AWAKE( victim ) )
      stop_fighting( victim, FALSE );

   /*
    * Payoff for killing things.
    */



   if( victim->position == POS_DEAD && ( IS_NPC( victim ) || !IS_VAMP( victim ) || ( deathmatch ) ) )
   {
      group_gain( ch, victim );

      /*
       * Sort out kill counts..... 
       */
      if( !IS_NPC( ch ) )
      {
         if( !IS_NPC( victim ) )
            ch->pcdata->records->pk++;
         else
            ch->pcdata->records->mk++;
      }

      if( !IS_NPC( victim ) )
      {
         if( !IS_NPC( ch ) )
            victim->pcdata->records->pd++;
         else
            victim->pcdata->records->md++;
      }

      if( !IS_NPC( victim ) || is_set( victim->act, ACT_INTELLIGENT ) )
      {


         xprintf_2( log_buf, "%s killed by %s at %d",
                  ( IS_NPC( victim ) ? victim->short_descr : victim->name ),
                  ( IS_NPC( ch ) ? ch->short_descr : ch->name ), victim->in_room->vnum );
         log_string( log_buf );

         notify( log_buf, 82 );

         /*
          * As level gain is no longer automatic, a dead char loses
          * * 1/2 their gained exp.  -S- 
          * * Fixed my bug here too, hehe!
          */

         if( victim->exp > 0 )
         {
            gain_exp( victim, ( 0 - ( victim->exp / 2 ) ) );
            victim->exp = UMAX( victim->exp, 0 );
         }

      }

      if( IS_NPC( ch ) )
         raw_kill( victim, "" );
      else
      {
         char name_buf[MAX_STRING_LENGTH];
         xprintf( name_buf, "%s", ch->name );
         raw_kill( victim, name_buf );
      }

      if( deathmatch && !IS_NPC( victim ) )
         do_quit( victim, "" );

      if( IS_NPC( ch ) && IS_NPC( victim ) && is_set( ch->act, ACT_INTELLIGENT ) )
      {
         do_get( ch, "all corpse" );
         do_sacrifice( ch, "corpse" );
      }

      if( !IS_NPC( ch ) && IS_NPC( victim ) )
      {
         if( is_set( ch->act, ACT_AUTOLOOT ) )
            do_get( ch, "all corpse" );
         else
            do_look( ch, "in corpse" );

         if( is_set( ch->act, ACT_AUTOSAC ) )
            do_sacrifice( ch, "corpse" );
      }

      return FALSE;
   }

   if( victim == ch )
      return TRUE;

   /*
    * Take care of link dead people.
    */
   if( !IS_NPC( victim ) && victim->desc == NULL )
   {
      if( number_range( 0, victim->wait ) == 0 )
      {
         do_recall( victim, "" );
         return TRUE;
      }
   }

   /*
    * Wimp out?
    */
   if( IS_NPC( victim ) && dam > 0 )
   {
      if( ( is_set( victim->act, ACT_WIMPY ) && number_bits( 1 ) == 0
            && victim->hit < victim->max_hit / 2 )
          || ( IS_AFFECTED( victim, AFF_CHARM ) && victim->master != NULL && victim->master->in_room != victim->in_room ) )
         do_flee( victim, "" );
   }

   if( !IS_NPC( victim ) && victim->hit > 0 && victim->hit <= victim->wimpy && victim->wait == 0 )
      do_flee( victim, "" );

   tail_chain(  );
   return TRUE;

}
