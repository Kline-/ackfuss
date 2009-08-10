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

#ifndef DEC_ACT_OBJ_H
#include "h/act_obj.h"
#endif

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_BOARD_H
#include "h/board.h"
#endif

#ifndef DEC_BUILDTAB_H
#include "h/buildtab.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_CURSOR_H
#include "h/cursor.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_HUNT_H
#include "h/hunt.h"
#endif

#ifndef DEC_MACROS_H
#include "h/macros.h"
#endif

#ifndef DEC_MAGIC_H
#include "h/magic.h"
#endif

#ifndef DEC_MAPPER_H
#include "h/mapper.h"
#endif

#ifndef DEC_MONEY_H
#include "h/money.h"
#endif

#ifndef DEC_RULERS_H
#include "h/rulers.h"
#endif

#ifndef DEC_SAVE_H
#include "h/save.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

#ifndef DEC_TRIGGER_H
#include "h/trigger.h"
#endif

#ifndef DEC_UPDATE_H
#include "h/update.h"
#endif

char *format_obj_to_char( OBJ_DATA * obj, CHAR_DATA * ch, bool fShort )
{
   static char buf[MSL];
   static char buf2[MSL];

   snprintf( buf, MSL, "%s", color_string( ch, "objects" ) );

   if( IS_IMMORTAL(ch) && ch->position == POS_BUILDING ) /* Imms should see vnums, <3 builders :) --Kline */
   {
    snprintf(buf2,MSL,"(%d) ",obj->pIndexData->vnum);
    strncat(buf,buf2,MSL-1);
   }

   /* Check for mquest target */
   strncat(buf,display_obj_target(ch,obj),MSL-1);

   if( IS_OBJ_STAT( obj, ITEM_EXTRA_INVIS ) )
      strncat( buf, "(Invis) ", MSL-1 );
   if( ( IS_AFFECTED( ch, AFF_DETECT_EVIL ) || item_has_apply( ch, ITEM_APPLY_DET_EVIL ) ) && IS_OBJ_STAT( obj, ITEM_EXTRA_EVIL ) )
      strncat( buf, "(Red Aura) ", MSL-1 );
   if( ( IS_AFFECTED( ch, AFF_DETECT_MAGIC ) || item_has_apply( ch, ITEM_APPLY_DET_MAG ) ) && IS_OBJ_STAT( obj, ITEM_EXTRA_MAGIC ) )
      strncat( buf, "(Magical) ", MSL-1 );
   if( IS_OBJ_STAT( obj, ITEM_EXTRA_GLOW ) )
      strncat( buf, "(Glowing) ", MSL-1 );
   if( IS_OBJ_STAT( obj, ITEM_EXTRA_HUM ) )
      strncat( buf, "(Humming) ", MSL-1 );

   if( fShort )
   {
      if( obj->short_descr != NULL )
         strncat( buf, obj->short_descr, MSL-1 );
   }
   else
   {
      if( obj->long_descr != NULL )
         strncat( buf, obj->long_descr, MSL-1 );
   }
   strncat( buf, color_string( ch, "normal" ), MSL-1 );
   return buf;
}



/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char( OBJ_DATA * list, CHAR_DATA * ch, bool fShort, bool fShowNothing )
{
   char buf[MSL];
   char **prgpstrShow;
   int *prgnShow;
   char *pstrShow;
   OBJ_DATA *obj;
   int nShow;
   int iShow;
   int count;
   bool fCombine;

   if( ch->desc == NULL )
      return;
   buf[0] = '\0';


   /*
    * Alloc space for output lines.
    */
   count = 0;
   for( obj = list; obj != NULL; obj = obj->next_in_carry_list )
      count++;
   prgpstrShow = (char **)qgetmem( count * sizeof( char * ) );
   prgnShow = (int *)qgetmem( count * sizeof( int ) );
   nShow = 0;

   /*
    * Format the list of objects.
    */
   for( obj = list; obj != NULL; obj = obj->next_in_carry_list )
   {
      if( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
      {
         pstrShow = format_obj_to_char( obj, ch, fShort );
         fCombine = FALSE;

         if( IS_NPC( ch ) || ch->act.test(ACT_COMBINE) )
         {
            /*
             * Look for duplicates, case sensitive.
             * Matches tend to be near end so run loop backwords.
             */
            for( iShow = nShow - 1; iShow >= 0; iShow-- )
            {
               if( !strcmp( prgpstrShow[iShow], pstrShow ) )
               {
                  prgnShow[iShow]++;
                  fCombine = TRUE;
                  break;
               }
            }
         }

         /*
          * Couldn't combine, or didn't want to.
          */
         if( !fCombine )
         {
            prgpstrShow[nShow] = str_dup( pstrShow );
            prgnShow[nShow] = 1;
            nShow++;
         }
      }
   }

   /*
    * Output the formatted list.
    */
   for( iShow = 0; iShow < nShow; iShow++ )
   {
      if( IS_NPC( ch ) || ch->act.test(ACT_COMBINE) )
      {
         if( prgnShow[iShow] != 1 )
         {
            snprintf( buf, MSL, "(%2d) ", prgnShow[iShow] );
            send_to_char( buf, ch );
         }
         else
         {
            send_to_char( "     ", ch );
         }
      }
      send_to_char( prgpstrShow[iShow], ch );
      send_to_char( "\r\n", ch );
      free_string( prgpstrShow[iShow] );
   }

   if( fShowNothing && nShow == 0 )
   {
      if( IS_NPC( ch ) || ch->act.test(ACT_COMBINE) )
         send_to_char( "     ", ch );
      send_to_char( "Nothing.\r\n", ch );
   }

   /*
    * Clean up.
    */
   qdispose( prgpstrShow, count * sizeof( char * ) );
   qdispose( prgnShow, count * sizeof( int ) );

   return;
}

void show_room_list_to_char( OBJ_DATA * list, CHAR_DATA * ch, bool fShort, bool fShowNothing )
{
   char buf[MSL];
   char **prgpstrShow;
   int *prgnShow;
   char *pstrShow;
   OBJ_DATA *obj;
   int nShow;
   int iShow;
   int count;
   bool fCombine;

   if( ch->desc == NULL )
      return;
   buf[0] = '\0';


   /*
    * Alloc space for output lines.
    */
   count = 0;
   for( obj = list; obj != NULL; obj = obj->next_in_room )
      count++;
   prgpstrShow = (char **)qgetmem( count * sizeof( char * ) );
   prgnShow = (int *)qgetmem( count * sizeof( int ) );
   nShow = 0;

   /*
    * Format the list of objects.
    */
   for( obj = list; obj != NULL; obj = obj->next_in_room )
   {
      if( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) && str_cmp( obj->long_descr, "" ) )
      {
         pstrShow = format_obj_to_char( obj, ch, fShort );
         fCombine = FALSE;

         if( IS_NPC( ch ) || ch->act.test(ACT_COMBINE) )
         {
            /*
             * Look for duplicates, case sensitive.
             * Matches tend to be near end so run loop backwords.
             */
            for( iShow = nShow - 1; iShow >= 0; iShow-- )
            {
               if( !strcmp( prgpstrShow[iShow], pstrShow ) )
               {
                  prgnShow[iShow]++;
                  fCombine = TRUE;
                  break;
               }
            }
         }

         /*
          * Couldn't combine, or didn't want to.
          */
         if( !fCombine )
         {
            prgpstrShow[nShow] = str_dup( pstrShow );
            prgnShow[nShow] = 1;
            nShow++;
         }
      }
   }

   /*
    * Output the formatted list.
    */
   for( iShow = 0; iShow < nShow; iShow++ )
   {
      if( ( IS_NPC( ch ) || ch->act.test(ACT_COMBINE) ) && str_cmp( prgpstrShow[iShow], "" ) )
      {
         if( prgnShow[iShow] != 1 )
         {
            snprintf( buf, MSL, "(%2d) ", prgnShow[iShow] );
            send_to_char( buf, ch );
         }
         else
         {
            send_to_char( "     ", ch );
         }
      }
      send_to_char( prgpstrShow[iShow], ch );
      send_to_char( "\r\n", ch );
      free_string( prgpstrShow[iShow] );
   }

   if( fShowNothing && nShow == 0 )
   {
      if( IS_NPC( ch ) || ch->act.test(ACT_COMBINE) )
         send_to_char( "     ", ch );
      send_to_char( "Nothing.\r\n", ch );
   }

   /*
    * Clean up.
    */
   qdispose( prgpstrShow, count * sizeof( char * ) );
   qdispose( prgnShow, count * sizeof( int ) );

   return;
}




void show_char_to_char_0( CHAR_DATA * victim, CHAR_DATA * ch )
{
   char buf[MSL];
   char buf2[MSL];


   snprintf( buf, MSL, "%s", color_string( ch, "mobiles" ) );
   buf2[0] = '\0';

   /* Check for mquest target */
   strncat(buf,display_mob_target(ch,victim),MSL-1);

/* This is temporary....
 * Zen  WOLF
 *
 */
   if( IS_GHOST(victim) )
   {
    ch_printf(ch,"@@d(@@gGhost@@d) @@N%s floats here morbidly.\r\n", victim->get_name(ch) );
    return;
   }

   if( !IS_NPC( victim ) && IS_WOLF( victim ) )
   {
      if( IS_SHIFTED( victim ) )
      {
         send_to_char( "A large wolf scampers before you.@@N\r\n", ch );
         return;
      }
      else if( IS_RAGED( victim ) )
      {
         send_to_char( "A @@bWEREWOLF in @@rFULL RAGE stands before you!\r\n", ch );
         return;
      }
   }


   if( !IS_NPC( victim ) || IS_IMMORTAL(ch) )
   {
      /*
       * Then show what race they are (about time this added ;)
       * Imms should see mob races, too. <3 Builders! --Kline
       */
      if( IS_NPC(victim) && ch->position == POS_BUILDING )
       snprintf( buf2, MSL, "(%d) [%s] ",victim->npcdata->pIndexData->vnum, race_table[victim->race].race_name );
      else
       snprintf( buf2, MSL, "[%s] ", race_table[victim->race].race_name );
      strncat( buf, buf2, MSL-1 );
   }

   if( IS_NPC( victim ) )
   {
      if( victim->act.test(ACT_UNDEAD) && ( is_affected( ch, skill_lookup( "detect undead" ) ) || item_has_apply( ch, ITEM_APPLY_DET_UNDEAD ) ) )
         strncat( buf, "(UnDead) ", MSL-1 );

      if( victim->act.test(ACT_INTELLIGENT) )
         strncat( buf, "(Int) ", MSL-1 );

   }


   if( IS_AFFECTED( victim, AFF_INVISIBLE ) )
      strncat( buf, "(Invis) ", MSL-1 );
   if( IS_AFFECTED( victim, AFF_HIDE ) || item_has_apply( victim, ITEM_APPLY_HIDE ) )
      strncat( buf, "(Hide) ", MSL-1 );
   if( IS_AFFECTED( victim, AFF_CHARM ) )
      strncat( buf, "(Charm) ", MSL-1 );
   if( IS_AFFECTED( victim, AFF_PASS_DOOR ) || item_has_apply( victim, ITEM_APPLY_PASS_DOOR ) )
      strncat( buf, "(Clear) ", MSL-1 );
   if( IS_AFFECTED( victim, AFF_FAERIE_FIRE ) )
      strncat( buf, "(Pink) ", MSL-1 );
   if( IS_EVIL( victim ) && ( IS_AFFECTED( ch, AFF_DETECT_EVIL ) || item_has_apply( ch, ITEM_APPLY_DET_EVIL ) ) )
      strncat( buf, "(Red) ", MSL-1 );
   if( IS_AFFECTED( victim, AFF_SANCTUARY ) || item_has_apply( victim, ITEM_APPLY_SANC ) )
      strncat( buf, "(White) ", MSL-1 );
   if( !IS_NPC( victim ) && victim->act.test(ACT_KILLER) )
      strncat( buf, "(KILLER) ", MSL-1 );
   if( !IS_NPC( victim ) && victim->act.test(ACT_THIEF) )
      strncat( buf, "(THIEF) ", MSL-1 );

   /*
    * strncat to buf *linkdead* if not connected? 
    */
   if( !IS_NPC( victim ) )
      if( ( victim->desc ) != NULL && victim->desc->connected != CON_PLAYING )
         strncat( buf, "(LINKDEAD)", MSL-1 );

   if( !IS_NPC(victim) && victim->act.test(ACT_RULER) )
      strncat( buf, get_ruler_title( victim->pcdata->ruler_rank, victim->pcdata->login_sex ), MSL-1 );
   if( victim->position == POS_STANDING && victim->long_descr[0] != '\0' )
   {
      strncat( buf, victim->long_descr.c_str(), MSL-1 );
      strncat( buf, "\r\n", MSL-1 );
      strncat( buf, color_string( ch, "normal" ), MSL-1 );

      if( ( IS_AFFECTED( victim, AFF_CLOAK_FLAMING ) )
          || ( IS_AFFECTED( victim, AFF_CLOAK_ABSORPTION ) )
          || ( IS_AFFECTED( victim, AFF_CLOAK_REFLECTION ) ) || ( is_affected( victim, skill_lookup( "cloak:misery" ) ) ) )
      {
         strncat( buf, "  @@NCLOAK:", MSL-1 );
         if( IS_AFFECTED( victim, AFF_CLOAK_FLAMING ) )
            strncat( buf, " @@eFLAMING@@N", MSL-1 );
         if( IS_AFFECTED( victim, AFF_CLOAK_ABSORPTION ) )
            strncat( buf, " @@lABSORB@@N", MSL-1 );
         if( IS_AFFECTED( victim, AFF_CLOAK_REFLECTION ) )
            strncat( buf, " @@mREFLECT@@N", MSL-1 );
         if( is_affected( victim, skill_lookup( "cloak:misery" ) ) )
            strncat( buf, " @@RMISERY@@N", MSL-1 );
         strncat( buf, "\r\n", MSL-1 );
      }

      if( victim->first_shield != NULL )
      {
         MAGIC_SHIELD *this_shield;
         strncat( buf, "   @@WSHIELD: @@N", MSL-1 );
         for( this_shield = victim->first_shield; this_shield != NULL; this_shield = this_shield->next )
            strncat( buf, this_shield->name, MSL-1 );
         strncat( buf, "\r\n", MSL-1 );
      }

      send_to_char( buf, ch );
      return;
   }

   strncat( buf, victim->get_name(ch), MSL-1 );
   if( !ch->act.test(ACT_BRIEF) )
      strncat( buf, victim->get_title(), MSL-1 );

   switch ( victim->position )
   {
      case POS_DEAD:
         strncat( buf, " is DEAD!!", MSL-1 );
         break;
      case POS_MORTAL:
         strncat( buf, " is mortally wounded.", MSL-1 );
         break;
      case POS_INCAP:
         strncat( buf, " is incapacitated.", MSL-1 );
         break;
      case POS_STUNNED:
         strncat( buf, " is lying here stunned.", MSL-1 );
         break;
      case POS_SLEEPING:
         strncat( buf, " is sleeping here.", MSL-1 );
         break;
      case POS_RESTING:
         if( victim->sitting != NULL && victim->sitting->in_room == victim->in_room )
         {
            char sit[MSL];
            snprintf( sit, MSL, " is here, resting on %s.", victim->sitting->short_descr );
            strncat( buf, sit, MSL-1 );
         }
         else
            strncat( buf, " is resting here.", MSL-1 );
         break;
      case POS_STANDING:
      {
         strncat( buf, " is here", MSL-1 );
         if( !IS_NPC( victim ) && ( victim->stance > 0 ) && ( victim->stance != STANCE_AMBUSH ) )
         {
            char stance_buf[MSL];
            snprintf( stance_buf, MSL, " in the Stance of the %s.", stance_app[victim->stance].name );
            strncat( buf, stance_buf, MSL-1 );
         }
         else
            strncat( buf, ".", MSL-1 );
         break;
      }
      case POS_WRITING:
         strncat( buf, " is writing a note.", MSL-1 );
         break;
      case POS_BUILDING:
         strncat( buf, " is BUILDING!!", MSL-1 );
         break;
      case POS_FIGHTING:
         strncat( buf, " is here, fighting ", MSL-1 );
         if( victim->fighting == NULL )
            strncat( buf, "thin air??", MSL-1 );
         else if( victim->fighting == ch )
            strncat( buf, "YOU!", MSL-1 );
         else if( victim->in_room == victim->fighting->in_room )
         {
            strncat( buf, victim->fighting->get_name(ch), MSL-1 );
            strncat( buf, ".", MSL-1 );
         }
         else
            strncat( buf, "somone who left??", MSL-1 );
         break;
   }


   /*
    * buf[0] = UPPER(buf[0]); 
    */
   strncat( buf, color_string( ch, "normal" ), MSL-1 );
   strncat( buf, "\r\n", MSL-1 );

   if( ( IS_AFFECTED( victim, AFF_CLOAK_FLAMING ) )
       || ( IS_AFFECTED( victim, AFF_CLOAK_ADEPT ) )
       || ( IS_AFFECTED( victim, AFF_CLOAK_REGEN ) )
       || ( IS_AFFECTED( victim, AFF_CLOAK_ABSORPTION ) )
       || ( IS_AFFECTED( victim, AFF_CLOAK_REFLECTION ) ) || ( is_affected( victim, skill_lookup( "cloak:misery" ) ) ) )
   {
      strncat( buf, "  @@NCLOAK:", MSL-1 );
      if( IS_AFFECTED( victim, AFF_CLOAK_FLAMING ) )
         strncat( buf, " @@eFLAMING@@N", MSL-1 );
      if( IS_AFFECTED( victim, AFF_CLOAK_ABSORPTION ) )
         strncat( buf, " @@lABSORB@@N", MSL-1 );
      if( IS_AFFECTED( victim, AFF_CLOAK_REFLECTION ) )
         strncat( buf, " @@mREFLECT@@N", MSL-1 );
      if( IS_AFFECTED( victim, AFF_CLOAK_ADEPT ) )
         strncat( buf, " @@WADEPT@@N", MSL-1 );
      if( IS_AFFECTED( victim, AFF_CLOAK_REGEN ) )
         strncat( buf, " @@rREGEN@@N", MSL-1 );
      if( is_affected( victim, skill_lookup( "cloak:misery" ) ) )
         strncat( buf, " @@RMISERY@@N", MSL-1 );
      strncat( buf, "\r\n", MSL-1 );

   }
   if( victim->first_shield != NULL )
   {
      MAGIC_SHIELD *this_shield;
      strncat( buf, "   @@WSHIELD: @@N", MSL-1 );
      for( this_shield = victim->first_shield; this_shield != NULL; this_shield = this_shield->next )
         strncat( buf, this_shield->name, MSL-1 );
      strncat( buf, "\r\n", MSL-1 );
   }

   if( victim->riding != NULL )
   {
      snprintf( buf2, MSL, "  riding %s.\r\n", victim->riding->get_name() );
      strncat( buf, buf2, MSL-1 );
   }

   send_to_char( buf, ch );
   return;
}



void show_char_to_char_1( CHAR_DATA * victim, CHAR_DATA * ch )
{
   char buf[MSL];
   OBJ_DATA *obj;
   int iWear;
   int pct;
   bool found;
   buf[0] = '\0';
   extern const char *where_name[];


   if( can_see( victim, ch ) )
   {

      act( "$n looks at you.", ch, NULL, victim, TO_VICT );
      act( "$n looks at $N.", ch, NULL, victim, TO_NOTVICT );
   }

   if( victim->description[0] != '\0' )
   {
      send_to_char( tagline_format(victim->description.c_str(),ch), ch );
      send_to_char( "\r\n", ch);
   }
   else
   {
      act( "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );
   }

   if( victim->max_hit > 0 )
      pct = ( 100 * victim->hit ) / victim->max_hit;
   else
      pct = -1;

   strcpy( buf, victim->get_name(ch) );

   if( pct >= 100 )
      strncat( buf, " is in pristine condition.\r\n", MSL );
   else if( pct >= 90 )
      strncat( buf, " is slightly scratched.\r\n", MSL );
   else if( pct >= 80 )
      strncat( buf, " has some light bruising.\r\n", MSL );
   else if( pct >= 70 )
      strncat( buf, " has some shallow cuts.\r\n", MSL );
   else if( pct >= 60 )
      strncat( buf, " has several weeping wounds.\r\n", MSL );
   else if( pct >= 50 )
      strncat( buf, " looks like a traffic accident.\r\n", MSL );
   else if( pct >= 40 )
      strncat( buf, " is bleeding slowly into a puddle.\r\n", MSL );
   else if( pct >= 30 )
      strncat( buf, " is spraying blood all over.\r\n", MSL );
   else if( pct >= 20 )
      strncat( buf, " is having trouble living.\r\n", MSL );
   else if( pct >= 10 )
      strncat( buf, " looks ready to kick the bucket.\r\n", MSL );
   else
      strncat( buf, " is DYING.\r\n", MSL );

   buf[0] = UPPER( buf[0] );
   send_to_char( buf, ch );

   found = FALSE;

   if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( IS_SHIFTED( victim ) || IS_RAGED( victim ) ) )
   {
   }
   else
   {
      for( iWear = 0; iWear < MAX_WEAR; iWear++ )
      {
         if( ( obj = get_eq_char( victim, iWear ) ) != NULL && can_see_obj( ch, obj ) )
         {
            if( !found )
            {
               send_to_char( "\r\n", ch );
               act( "$N is using:", ch, NULL, victim, TO_CHAR );
               found = TRUE;
            }
            send_to_char( where_name[iWear], ch );
            send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
            send_to_char( "\r\n", ch );
         }
      }

      if( victim != ch && !IS_NPC( ch ) && number_percent(  ) < ch->pcdata->learned[gsn_peek] )
      {
         send_to_char( "\r\nYou peek at the inventory:\r\n", ch );
         show_list_to_char( victim->first_carry, ch, TRUE, TRUE );
      }
   }
   return;
}


void show_char_to_char( CHAR_DATA * list, CHAR_DATA * ch )
{
   CHAR_DATA *rch;

   for( rch = list; rch != NULL; rch = rch->next_in_room )
   {
      if( rch == ch )
         continue;

      if( !IS_NPC( rch ) && rch->act.test(ACT_WIZINVIS) && get_trust( ch ) < rch->pcdata->invis )
         continue;

      if( ( rch->rider != NULL ) && ( rch->rider != ch ) )
         continue;   /* show under the rider */

      if( can_see( ch, rch ) )
      {
         show_char_to_char_0( rch, ch );
      }
      else if( room_is_dark( ch->in_room )
               && ( IS_AFFECTED( rch, AFF_INFRARED ) || item_has_apply( rch, ITEM_APPLY_INFRA ) ) )
      {
         send_to_char( "You see glowing red eyes watching YOU!\r\n", ch );
      }
   }

   return;
}



bool check_blind( CHAR_DATA * ch )
{
   if( !IS_NPC( ch ) && ch->act.test(ACT_HOLYLIGHT) )
      return TRUE;

   if( IS_AFFECTED( ch, AFF_BLIND ) )
   {
      send_to_char( "You cannot see a thing!\r\n", ch );
      return FALSE;
   }

   return TRUE;
}

DO_FUN(do_look)
{
   char buf[MSL];
   char arg1[MSL];
   char arg2[MSL];
   char out[MSL];
   ROOM_INDEX_DATA *room;
   EXIT_DATA *pexit;
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char pdesc[MSL];
   int door;
   buf[0] = '\0';
   out[0] = '\0';

   if( !IS_NPC( ch ) && ch->desc == NULL )
      return;

   if( ch->position < POS_SLEEPING )
   {
      send_to_char( "You cannot see anything but stars!\r\n", ch );
      return;
   }

   if( ch->position == POS_SLEEPING )
   {
      send_to_char( "You are sleeping!\r\n", ch );
      return;
   }

   if( !check_blind( ch ) )
      return;

   if( !IS_NPC( ch ) && !ch->act.test(ACT_HOLYLIGHT) && room_is_dark( ch->in_room ) )
   {
      send_to_char( "It is pitch black ... \r\n", ch );
      show_char_to_char( ch->in_room->first_person, ch );
      return;
   }

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
   {
      /*
       * 'look' or 'look auto'
       */


      if( is_switched(ch) || ( !IS_NPC( ch ) && ch->act.test(ACT_MAPPER) ) )
      {
         send_to_char( "\r\n", ch );
         do_mapper( ch, "7" );
         send_to_char( "\r\n", ch );
      }
      else
      {
         snprintf( out, MSL, "%s%s%s\r\n", color_string( ch, "rooms" ), ch->in_room->name, color_string( ch, "normal" ) );

         send_to_char( out, ch );
         if( is_switched(ch) || ( !IS_NPC( ch ) && ch->act.test(ACT_AUTOEXIT) ) )
            do_exits( ch, "auto" );
         if( arg1[0] == '\0' || ( is_switched(ch) || ( !IS_NPC( ch ) && !ch->act.test(ACT_BRIEF) ) ) )
         {
            char *string_format( char *str, int *numlines, int width, int height, bool unjust );
            int wid = ( IS_NPC( ch ) ? 80 : ch->pcdata->term_columns );

            snprintf( out, MSL, "%s%s%s\r\n", color_string( ch, "rooms" ),
                     string_format( tagline_format(ch->in_room->description,ch), NULL, wid, 10000,
                                    !ch->act.test(ACT_JUSTIFY) ), color_string( ch, "normal" ) );
            send_to_char( out, ch );
         }
      }

      /*
       * Show any room-affects 
       */
      if( ( ch->in_room->affected_by != 0 )
          && ( ( is_affected( ch, skill_lookup( "detect magic" ) ) ) || ( item_has_apply( ch, ITEM_APPLY_DET_MAG ) ) ) )
      {
         snprintf( out, MSL, "%s", color_string( ch, "rooms" ) );
         if( IS_SET( ch->in_room->affected_by, ROOM_BV_SHADE ) )
            strncat( out, "@@NA menacing @@ddark shadow@@N hangs in the sky above you.\r\n", MSL-1 );
         if( IS_SET( ch->in_room->affected_by, ROOM_BV_ENCAPS ) )
            strncat( out, "@@NA barely visible @@renergy web@@N is blocking all exits here.\r\n", MSL-1 );
         if( IS_SET( ch->in_room->affected_by, ROOM_BV_FIRE_RUNE ) )
            strncat( out, "@@NA mystical @@eFire @@NRune@@N hangs in the air above you.\r\n", MSL-1 );
         if( IS_SET( ch->in_room->affected_by, ROOM_BV_SHOCK_RUNE ) )
            strncat( out, "@@NA mystical @@lShock@@N Rune@@N hangs in the air above you.\r\n", MSL-1 );
         if( IS_SET( ch->in_room->affected_by, ROOM_BV_POISON_RUNE ) )
            strncat( out, "@@NA mystical @@dPoison@@N Rune hangs in the air above you.\r\n", MSL-1 );
         if( IS_SET( ch->in_room->affected_by, ROOM_BV_HEAL_REGEN ) )
            strncat( out, "@@NA majestic @@mHealing Light@@N encompasses the room.\r\n", MSL-1 );
         if( IS_SET( ch->in_room->affected_by, ROOM_BV_HEAL_STEAL ) )
            strncat( out, "@@NA menacing @@dWithering shadow@@N enfolds the room.\r\n", MSL-1 );
         if( IS_SET( ch->in_room->affected_by, ROOM_BV_MANA_REGEN ) )
            strncat( out, "@@NA powerful @@eMana Flare@@N empowers the room.\r\n", MSL-1 );
         if( IS_SET( ch->in_room->affected_by, ROOM_BV_MANA_STEAL ) )
            strncat( out, "@@NA mind sapping @@dMana Drain@@N enfolds the room.\r\n", MSL-1 );
         if( IS_SET( ch->in_room->affected_by, ROOM_BV_HOLD ) )
            strncat( out, "@@NThe magical bars of a @@rCage@@N surround the room.\r\n", MSL-1 );
         if( IS_SET( ch->in_room->affected_by, ROOM_BV_SOUL_NET ) )
            strncat( out, "@@NA demonic @@dSoul Net@@N enshrouds the room.\r\n", MSL-1 );

         strncat( out, color_string( ch, "normal" ), MSL-1 );
         send_to_char( out, ch );
      }

      if( !IS_NPC( ch ) && ( IS_VAMP( ch ) || IS_WOLF( ch ) ) )
         if( !ch->in_room->mark_list.empty() )
         {
            list<MARK_DATA *>::iterator li;
            MARK_DATA *mk = NULL;
            short num_marks = 0;

            for( li = ch->in_room->mark_list.begin(); li != ch->in_room->mark_list.end(); li++ )
            {
             mk = *li;
               if( ( ( mk->type == WOLF ) && IS_WOLF( ch ) ) || ( ( mk->type == VAMP ) && IS_VAMP( ch ) ) )
                  num_marks++;
            }
            if( num_marks > 0 )
            {
               char marksbuf[MSL];

               if( IS_WOLF( ch ) )
               {
                  snprintf( marksbuf, MSL, "@@aThis room has @@W%d @@a%s@@W.@@N\r\n", num_marks,
                           ( num_marks > 1 ) ? "scents" : "scent" );
               }
               else if( IS_VAMP( ch ) )
               {
                  snprintf( marksbuf, MSL, "@@mThis room has @@W%d @@eBlood%s@@N.\r\n", num_marks,
                           ( num_marks > 1 ) ? "Signs" : "Sign" );
               }
               send_to_char( marksbuf, ch );
            }

         }



      show_room_list_to_char( ch->in_room->first_content, ch, FALSE, FALSE );
      show_char_to_char( ch->in_room->first_person, ch );
      {
         char money_show[MSL];
         snprintf( money_show, MSL, "%s lie in a pile.\r\n", money_string( ch->in_room->treasure ) );
         if( str_prefix( " lie", money_show ) )
            send_to_char( money_show, ch );
      }

      return;
   }


   if( !str_cmp( arg1, "i" ) || !str_cmp( arg1, "in" ) )
   {
      /*
       * 'look in' 
       */
      if( arg2[0] == '\0' )
      {
         send_to_char( "Look in what?\r\n", ch );
         return;
      }

      if( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
      {
         send_to_char( "You do not see that here.\r\n", ch );
         return;
      }

      switch ( obj->item_type )
      {
         default:
            send_to_char( "That is not a container.\r\n", ch );
            break;

         case ITEM_PORTAL:
            if( obj->value[1] == 0 )
            {
               act( "You don't seem to be able to look in $p.", ch, obj, NULL, TO_CHAR );
               return;
            }

            if( ( room = get_room_index( obj->value[0] ) ) == NULL )
            {
               send_to_char( "You see nothing but blackness!\r\n", ch );
               return;
            }

            act( "$n looks into $p.", ch, obj, NULL, TO_ROOM );

            if( !IS_NPC( ch ) && !ch->act.test(ACT_HOLYLIGHT) && room_is_dark( ch->in_room ) )
            {
               act( "$p comes out into a dark place.  You see nothing!\r\n", ch, obj, NULL, TO_CHAR );
               return;
            }

            ch->was_in_room = ch->in_room;
            char_from_room(ch);
            char_to_room(ch,room);
            do_look(ch,"auto");
            char_from_room(ch);
            char_to_room(ch,ch->was_in_room);
            ch->was_in_room = NULL;
            break;

         case ITEM_DRINK_CON:
            if( obj->value[1] <= 0 )
            {
               send_to_char( "It is empty.\r\n", ch );
               break;
            }

            snprintf( buf, MSL, "It's %s full of a %s liquid.\r\n",
                     obj->value[1] < obj->value[0] / 4
                     ? "less than" :
                     obj->value[1] < 3 * obj->value[0] / 4 ? "about" : "more than", liq_table[obj->value[2]].liq_color );

            send_to_char( buf, ch );
            break;
         case ITEM_SPELL_MATRIX:
         case ITEM_CONTAINER:
         case ITEM_CORPSE_NPC:
         case ITEM_CORPSE_PC:
            if( IS_SET( obj->value[1], CONT_CLOSED ) )
            {
               send_to_char( "It is closed.\r\n", ch );
               break;
            }

            act( "$p contains:", ch, obj, NULL, TO_CHAR );
            show_list_to_char( obj->first_in_carry_list, ch, TRUE, TRUE );
            {
               char money_show[MSL];
               snprintf( money_show, MSL, "%s lie within.\r\n", money_string( obj->money ) );
               if( str_prefix( " lie", money_show ) )
                  send_to_char( money_show, ch );
            }
            break;
      }
      return;
   }

   if( !str_cmp( arg1, "board" ) || !str_cmp( arg2, "board" ) )
   {
      /*
       * int bnum;
       */

      if( ( obj = get_obj_here( ch, "board" ) ) == NULL )
      {
         send_to_char( "You do not see that here.\r\n", ch );
         return;

      }

      show_contents( ch, obj );
      return;
   }

   if( ( victim = get_char_room( ch, arg1 ) ) != NULL )
   {
      show_char_to_char_1( victim, ch );
      return;
   }

   for( obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list )
   {
      if( can_see_obj( ch, obj ) )
      {
         snprintf( pdesc, MSL, "\r\n%s\r\n", get_extra_descr( arg1, obj->first_exdesc ) );
         if( str_cmp( pdesc, "\r\n(null)\r\n" ) )
         {
            send_to_char( pdesc, ch );
            act( "$L$n closely examines $p.", ch, obj, NULL, TO_ROOM );
            return;
         }

         snprintf( pdesc, MSL, "\r\n%s\r\n", get_extra_descr( arg1, obj->pIndexData->first_exdesc ) );

         if( str_cmp( pdesc, "\r\n(null)\r\n" ) )
         {
            send_to_char( pdesc, ch );
            act( "$L$n closely examines $p.", ch, obj, NULL, TO_ROOM );
            return;
         }
      }

      if( is_name( arg1, obj->name ) )
      {
         send_to_char( tagline_format(obj->long_descr,ch), ch );
         return;
      }
   }

   for( obj = ch->in_room->first_content; obj != NULL; obj = obj->next_in_room )
   {
      if( can_see_obj( ch, obj ) )
      {
         snprintf( pdesc, MSL, "\r\n%s\r\n", get_extra_descr( arg1, obj->first_exdesc ) );

         if( str_cmp( pdesc, "\r\n(null)\r\n" ) )
         {
            send_to_char( pdesc, ch );
            act( "$L$n closely examines $p.", ch, obj, NULL, TO_ROOM );
            return;
         }

         snprintf( pdesc, MSL, "\r\n%s\r\n", get_extra_descr( arg1, obj->pIndexData->first_exdesc ) );
         if( str_cmp( pdesc, "\r\n(null)\r\n" ) )
         {
            send_to_char( pdesc, ch );
            act( "$L$n closely examines $p.", ch, obj, NULL, TO_ROOM );
            return;
         }
      }

      if( is_name( arg1, obj->name ) )
      {
         snprintf( pdesc, MSL, "%s\r\n", obj->long_descr );
         send_to_char( pdesc, ch );
         act( "$L$n closely examines $p.", ch, obj, NULL, TO_ROOM );
         return;
      }
   }

   snprintf( pdesc, MSL, "\r\n%s\r\n", get_extra_descr( arg1, ch->in_room->first_exdesc ) );
   if( str_cmp( pdesc, "\r\n(null)\r\n" ) )
   {
      send_to_char( pdesc, ch );
      act( "$L$n closely examines the $t.", ch, arg1, NULL, TO_ROOM );
      return;
   }

   if( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) )
      door = 0;
   else if( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east" ) )
      door = 1;
   else if( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) )
      door = 2;
   else if( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west" ) )
      door = 3;
   else if( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up" ) )
      door = 4;
   else if( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down" ) )
      door = 5;
   else
   {
      send_to_char( "You do not see that here.\r\n", ch );
      return;
   }

   /*
    * 'look direction' 
    */
   if( ( pexit = ch->in_room->exit[door] ) == NULL )
   {
      send_to_char( "Nothing special there.\r\n", ch );
      return;
   }

   if( ( pexit->description != NULL )
       && ( pexit->description[0] != '\0' )
       && ( ( !str_cmp( pexit->keyword, "" ) )
            || ( ( str_cmp( pexit->keyword, "" ) ) && ( !str_cmp( pexit->keyword, arg1 ) ) ) ) )
      send_to_char( pexit->description, ch );
   else
      send_to_char( "Nothing special there.\r\n", ch );

   if( ( pexit->keyword != NULL )
       && ( pexit->keyword[0] != '\0' ) && ( pexit->keyword[0] != ' ' ) && ( !str_cmp( pexit->keyword, arg1 ) ) )
   {
      if( pexit->exit_info.test(EX_CLOSED) )
      {
         act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
      }
      else if( pexit->exit_info.test(EX_ISDOOR) )
      {
         act( "The $d is open.", ch, NULL, pexit->keyword, TO_CHAR );
      }
   }

   return;
}

DO_FUN(do_examine)
{
   char buf[MSL];
   char arg[MSL];
   OBJ_DATA *obj;
   buf[0] = '\0';

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Examine what?\r\n", ch );
      return;
   }

   do_look( ch, arg );

   if( ( obj = get_obj_here( ch, arg ) ) != NULL )
   {
      switch ( obj->item_type )
      {
         default:
            break;

         case ITEM_DRINK_CON:
         case ITEM_CONTAINER:
         case ITEM_CORPSE_NPC:
         case ITEM_CORPSE_PC:
            send_to_char( "When you look inside, you see:\r\n", ch );
            snprintf( buf, MSL, "in %s", arg );
            do_look( ch, buf );
      }
      if( ch->position > POS_RESTING )
         trigger_handler( ch, obj, TRIGGER_EXAMINE );
   }

   return;
}

/*
 * Thanks to Zrin for auto-exit part.
 */
DO_FUN(do_exits)
{
   extern char *const compass_name[];
   char buf[MSL];
   char buf2[MSL];
   EXIT_DATA *pexit;
   bool found;
   bool fAuto;
   bool fAutonr;
   int door;

   buf[0] = '\0';
   buf2[0] = '\0';


   fAuto = !str_cmp( argument, "auto" );
   fAutonr = !str_cmp( argument, "autonr" );

   if( !check_blind( ch ) )
      return;

   strcpy( buf, (fAuto || fAutonr ) ? "[Exits:" : "Obvious exits:\r\n" );

   found = FALSE;
   for( door = 0; door < MAX_DIR; door++ )
   {
      /*
       * check for players that are blind (irl) 
       */
      if( ch->act.test(ACT_BLIND_PLAYER) )
      {
         if( ( pexit = ch->in_room->exit[door] ) != NULL && pexit->to_room != NULL )
         {
            found = TRUE;
            if( pexit->exit_info.test(EX_CLOSED) )
            {
               snprintf( buf2, MSL, " closed %s", ( !str_cmp( pexit->keyword, "" ) ? compass_name[door] : pexit->keyword ) );
            }
            else
            {
               snprintf( buf2, MSL, " %s", ( !str_cmp( pexit->keyword, "" ) ? compass_name[door] : pexit->keyword ) );
            }
            strncat( buf, buf2, MSL-1 );
            continue;
         }
      }


      /*
       * Check for thieves with find_doors... 
       */
      if( !IS_NPC( ch ) )
         if( ( pexit = ch->in_room->exit[door] ) != NULL
             && pexit->to_room != NULL
             && pexit->exit_info.test(EX_CLOSED)
             && !pexit->exit_info.test(EX_NODETECT)
             && ( ch->pcdata->learned[gsn_find_doors] > number_percent(  ) ) && ( !str_cmp( pexit->keyword, "" ) ) )
         {
            found = TRUE;
            if( fAuto || fAutonr )
               snprintf( buf2, MSL, " (%s)", compass_name[door] );
            else
               snprintf( buf2, MSL, "%-5s - Door.\r\n", capitalize( compass_name[door] ) );

            strncat( buf, buf2, MSL-1 );
            continue;
         }

      if( ( pexit = ch->in_room->exit[door] ) != NULL
          && pexit->to_room != NULL && !pexit->exit_info.test(EX_CLOSED) && ( !str_cmp( pexit->keyword, "" ) ) )
      {
         found = TRUE;
         if( fAuto || fAutonr )
         {
            strncat( buf, " ", MSL-1 );
            strncat( buf, compass_name[door], MSL-1 );
         }
         else
         {
            snprintf( buf + strlen( buf ), MSL, "%-5s - %s\r\n",
                     capitalize( compass_name[door] ),
                     ( room_is_dark( pexit->to_room ) || pexit->to_room == ch->in_room ) ? "Too dark to tell" : pexit->to_room->name );
         }
      }
   }

   if( !found )
      strncat( buf, (fAuto || fAutonr) ? " none" : "None.\r\n", MSL-1 );

   if( fAuto )
      strncat( buf, "]\r\n", MSL-1 );

   if( fAutonr )
      strncat( buf, "]", MSL-1 );

   send_to_char( buf, ch );
   return;
}

DO_FUN(do_score)
{

   /*
    * Score text has been re-formatted to look a little nicer ;)
    * * Affected by now dealt with by command 'affected'
    * * Also now added color
    * *  -- Stephen
    */

   char buf[MSL];
   char buf2[MSL];
   int cnt;


   snprintf( buf, MSL, "@@y%s%s, Race: %s%s, Clan: %s\r\nAge: ",
            ch->get_name(),
            ch->get_title(),
            IS_NPC( ch ) ? "n/a" : race_table[ch->race].race_title,
            IS_VAMP( ch ) ? "@@e(Vampire)@@N" : IS_WOLF( ch ) ? "@@r(Werewolf)@@N" : "",
            IS_NPC( ch ) ? "n/a" : clan_table[ch->clan].clan_name );
   my_get_age( ch, buf );
   send_to_char( buf, ch );
   snprintf( buf, MSL, " (%d hours RL)\r\n", my_get_hours( ch ) );
   send_to_char( buf, ch );

   snprintf( buf, MSL, "@@c+===============================================================+\r\n" );
   send_to_char( buf, ch );

   snprintf( buf, MSL,
            "| @@y%4d/%4d @@WHit @@y%4d/%4d @@WMana @@y%4d/%4d @@WMovement @@y%3d @@WPractices@@c |\r\n",
            ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move, ch->max_move, IS_NPC(ch) ? 0 : ch->pcdata->practice );
   send_to_char( buf, ch );

   if( IS_NPC( ch ) )
   {
      snprintf( buf, MSL,
               "|     @@R[ @@WStr:  @@y%2d  @@WInt:  @@y%2d  @@WWis:  @@y%2d  @@WDex:  @@y%2d  @@WCon:  @@y%2d @@R]      @@c|\r\n",
               get_curr_str( ch ), get_curr_int( ch ), get_curr_wis( ch ), get_curr_dex( ch ), get_curr_con( ch ) );
      send_to_char( buf, ch );


   }
   else
   {
      snprintf( buf, MSL,
               "|   @@R[ @@WStr:@@y%2d/%2d  @@WInt:@@y%2d/%2d  @@WWis:@@y%2d/%2d  @@WDex:@@y%2d/%2d  @@WCon:@@y%2d/%2d @@R]   @@c|\r\n",
               get_curr_str( ch ), ch->pcdata->max_str,
               get_curr_int( ch ), ch->pcdata->max_int,
               get_curr_wis( ch ), ch->pcdata->max_wis,
               get_curr_dex( ch ), ch->pcdata->max_dex, get_curr_con( ch ), ch->pcdata->max_con );
      send_to_char( buf, ch );
   }

   if( !IS_NPC( ch ) )
   {

      buf2[0] = '\0';

      snprintf( buf2, MSL, "@@WClass Order:@@y " );
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      {
         snprintf( buf, MSL, "%s ", class_table[ch->pcdata->order[cnt]].who_name );
         strncat( buf2, buf, MSL-1 );
      }
      snprintf( buf, MSL, "@@c|%s @@c|\r\n", center_text( buf2, 62 ) );
      send_to_char( buf, ch );

      buf2[0] = '\0';
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      {
         snprintf( buf, MSL, "@@c%s", class_table[cnt].who_name );
         strncat( buf2, buf, MSL-1 );
         if( ch->lvl[cnt] != -1 )
            snprintf( buf, MSL, ":@@W%2d ", ch->lvl[cnt] );
         else
            snprintf( buf, MSL, "@@c:@@W%s ", " 0" );
         strncat( buf2, buf, MSL-1 );
      }
   }
   else
      snprintf( buf2, MSL, " @@WLevel: @@y%d", ch->level );

   snprintf( buf, MSL, "@@c|%s @@c|\r\n", center_text( buf2, 62 ) );
   send_to_char( buf, ch );
   buf2[0] = '\0';

   if( IS_REMORT( ch ) )
   {

      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      {
         if( ch->lvl2[cnt] != -1 )
         {
            snprintf( buf, MSL, "@@m%s:@@W%2d ", remort_table[cnt].who_name, ch->lvl2[cnt] );
            strncat( buf2, buf, MSL-1 );

         }
      }
      snprintf( buf, MSL, "@@c|%s @@c|\r\n", center_text( buf2, 62 ) );
      send_to_char( buf, ch );
   }

   if( IS_ADEPT(ch) )
   {
      snprintf( buf, MSL, "@@WADEPT@@N: %s ", get_adept_name( ch ) );
      buf2[0] = '\0';
      strncat( buf2, buf, MSL-1 );
      snprintf( buf, MSL, "@@c|%s @@c|\r\n", center_text( buf2, 62 ) );
      send_to_char( buf, ch );
   }


   snprintf( buf, MSL,
            "X========= @@WExps: @@y%9d @@c========= @@aQuest Points: @@y%4d @@c========X\r\n", ch->exp, IS_NPC(ch) ? 0 : ch->pcdata->quest_points );
   send_to_char( buf, ch );

   if( get_trust( ch ) != ch->level )
   {
      snprintf( buf, MSL, "X================= @@WYou are trusted at level @@y%2d @@c=================X\r\n", get_trust( ch ) );
      send_to_char( buf, ch );
   }





   snprintf( buf, MSL,
            "| @@WYou are carrying @@y%4d/%4d @@Witems, weight @@y%4.2f/%7d @@Wkg.  @@c|\r\n",
            ch->carry_number, can_carry_n( ch ), ch->carry_weight, can_carry_w( ch ) );
   send_to_char( buf, ch );


   snprintf( buf, MSL,
            "|       @@WAutoexit: @@y%s   @@WAutoloot: @@y%s  @@WAutosac: @@y%s       @@c|\r\n",
            ( !IS_NPC( ch ) && ch->act.test(ACT_AUTOEXIT) ) ? "*ON* " : "*OFF*",
            ( !IS_NPC( ch ) && ch->act.test(ACT_AUTOLOOT) ) ? "*ON* " : "*OFF*",
            ( !IS_NPC( ch ) && ch->act.test(ACT_AUTOSAC) ) ? "*ON* " : "*OFF*" );
   send_to_char( buf, ch );


   if( !IS_NPC( ch ) )
   {
      snprintf( buf, MSL, " @@WYou have killed a total of: @@y%d @@WNPCs, and @@y%d @@WPlayers.",
               ch->pcdata->records->mk, ch->pcdata->records->pk );
      snprintf( buf2, MSL, "@@c|%s @@c|\r\n", center_text( buf, 62 ) );
      send_to_char( buf2, ch );

      snprintf( buf, MSL, " @@WA total of @@y%d @@WNPCs and @@y%d @@WPlayers have killed you.",
               ch->pcdata->records->md, ch->pcdata->records->pd );
      snprintf( buf2, MSL, "@@c|%s @@c|\r\n", center_text( buf, 62 ) );
      send_to_char( buf2, ch );

      snprintf( buf, MSL, " @@WWimpy Set to @@y%d @@WHitPoints.  Page Length is @@y%d @@Wlines.", ch->wimpy, ch->pcdata->pagelen );
      snprintf( buf2, MSL, "@@c|%s @@c|\r\n", center_text( buf, 62 ) );
      send_to_char( buf2, ch );

      snprintf( buf, MSL, " @@WDrunk: @@y%3s   @@WThirsty: @@y%3s   @@WHungry: @@y%3s",
               ( ch->pcdata->condition[COND_DRUNK] > 10 ) ? "Yes" : "No",
               ( ch->pcdata->condition[COND_THIRST] == 0 ) ? "Yes" : "No",
               ( ch->pcdata->condition[COND_FULL] == 0 ) ? "Yes" : "No" );
      snprintf( buf2, MSL, "@@c|%s @@c|\r\n", center_text( buf, 62 ) );
      send_to_char( buf2, ch );

      if( IS_VAMP( ch ) )
      {
         snprintf( buf, MSL, "@@eBLOODLUST@@W: @@e%d@@W/@@e%d@@N", ch->pcdata->super->energy, ch->pcdata->super->energy_max );
         snprintf( buf2, MSL, "@@c|%s @@c|\r\n", center_text( buf, 62 ) );
         send_to_char( buf2, ch );
         snprintf( buf, MSL, "@@dKindred Rank:@@N %d  @@rGeneration:@@N %d   @@mKnowledge Avail:@@N %d",
                  ch->pcdata->super->level, ch->pcdata->super->generation, ch->pcdata->super->pracs );
         snprintf( buf2, MSL, "@@c|%s @@c|\r\n", center_text( buf, 62 ) );
         send_to_char( buf2, ch );
         snprintf( buf, MSL, "@@WFAMILY: %s", get_family_name( ch ) );
         snprintf( buf2, MSL, "@@c|%s @@c|\r\n", center_text( buf, 62 ) );
         send_to_char( buf2, ch );

      }
      else if( IS_WOLF( ch ) )
      {
         snprintf( buf, MSL, "@@eRAGE@@W: @@e%d@@W/@@e%d@@N", ch->pcdata->super->energy, ch->pcdata->super->energy_max );
         snprintf( buf2, MSL, "@@c|%s @@c|\r\n", center_text( buf, 62 ) );
         send_to_char( buf2, ch );
         snprintf( buf, MSL, "@@rTribe Rank@@W:@@N %d  @@yTribe Standing@@W:@@N %s   @@GKnowledge Avail:@@N %d",
                  ch->pcdata->super->level, get_tribe_standing_name( ch->pcdata->super->generation ), ch->pcdata->super->pracs );
         snprintf( buf2, MSL, "@@c|%s @@c|\r\n", center_text( buf, 62 ) );
         send_to_char( buf2, ch );
         snprintf( buf, MSL, "@@bTRIBE: %s", get_tribe_name( ch ) );
         snprintf( buf2, MSL, "@@c|%s @@c|\r\n", center_text( buf, 62 ) );
         send_to_char( buf2, ch );

      }

   }
   snprintf( buf, MSL, "%s", "@@N" );
   snprintf( buf2, MSL, "@@c|%s @@c|\r\n", center_text( buf, 62 ) );
   send_to_char( buf2, ch );
   snprintf( buf, MSL, "@@WYou have assumed the @@yStance @@Wof the@@N %s.", stance_app[ch->stance].name );
   snprintf( buf2, MSL, "@@c|%s @@c|\r\n", center_text( buf, 62 ) );
   send_to_char( buf2, ch );

   switch ( ch->position )
   {
      case POS_DEAD:
         snprintf( buf, MSL, " You are @@yDEAD!!" );
         break;
      case POS_MORTAL:
         snprintf( buf, MSL, " You are @@ymortally wounded." );
         break;
      case POS_INCAP:
         snprintf( buf, MSL, " You are @@yincapaciated." );
         break;
      case POS_STUNNED:
         snprintf( buf, MSL, " You are @@ystunned." );
         break;
      case POS_SLEEPING:
         snprintf( buf, MSL, " You are @@ysleeping." );
         break;
      case POS_RESTING:
         snprintf( buf, MSL, " You are @@yresting." );
         break;
      case POS_STANDING:
         snprintf( buf, MSL, " You are @@ystanding." );
         break;
      case POS_FIGHTING:
         snprintf( buf, MSL, " You are @@yfighting." );
         break;
   }
   snprintf( buf2, MSL, "|@@W%s @@c|\r\n", center_text( buf, 62 ) );
   send_to_char( buf2, ch );

   snprintf( buf, MSL, "@@WYou are " );
   if( GET_AC( ch ) >= 101 )
      strncat( buf, "@@yall skin and bones.", MSL );
   else if( GET_AC( ch ) >= 80 )
      strncat( buf, "@@ynaked.", MSL );
   else if( GET_AC( ch ) >= 0 )
      strncat( buf, "@@ybadly armored.", MSL );
   else if( GET_AC( ch ) >= -100 )
      strncat( buf, "@@yslightly armored.", MSL );
   else if( GET_AC( ch ) >= -300 )
      strncat( buf, "@@ysomewhat armored.", MSL );
   else if( GET_AC( ch ) >= -500 )
      strncat( buf, "@@yarmored.", MSL );
   else if( GET_AC( ch ) >= -800 )
      strncat( buf, "@@ywell armored.", MSL );
   else if( GET_AC( ch ) >= -1400 )
      strncat( buf, "@@ystrongly armored.", MSL );
   else if( GET_AC( ch ) >= -2000 )
      strncat( buf, "@@yheavily armored.", MSL );
   else if( GET_AC( ch ) >= -3000 )
      strncat( buf, "@@ysuperbly armored.", MSL );
   else if( GET_AC( ch ) >= -4500 )
      strncat( buf, "@@ydivinely armored.", MSL );
   else
      strncat( buf, "@@yalmost invincible!", MSL );


   snprintf( buf2, MSL, " @@WArmor Class: @@y%5d.   %s", GET_AC( ch ), buf );
   snprintf( buf, MSL, "@@c|%s @@c|\r\n", center_text( buf2, 62 ) );
   send_to_char( buf, ch );

   snprintf( buf, MSL, " @@WHitroll: @@y%-5d   @@WDamroll: @@y%-5d", GET_HITROLL( ch ), GET_DAMROLL( ch ) );
   snprintf( buf2, MSL, "@@c|%s @@c|\r\n", center_text( buf, 62 ) );
   send_to_char( buf2, ch );


   snprintf( buf, MSL, " @@WYou are " );

   if( ch->alignment > 900 )
      strncat( buf, "@@yangelic.", MSL );
   else if( ch->alignment > 700 )
      strncat( buf, "@@ysaintly.", MSL );
   else if( ch->alignment > 300 )
      strncat( buf, "@@ygood.", MSL );
   else if( ch->alignment > 100 )
      strncat( buf, "@@ykind.", MSL );
   else if( ch->alignment > -100 )
      strncat( buf, "@@yneutral.", MSL );
   else if( ch->alignment > -350 )
      strncat( buf, "@@ymean.", MSL );
   else if( ch->alignment > -700 )
      strncat( buf, "@@yevil.", MSL );
   else if( ch->alignment > -900 )
      strncat( buf, "@@ydemonic.", MSL );
   else
      strncat( buf, "@@ysatanic!", MSL );

   snprintf( buf2, MSL, " @@WAlignment: @@y%5d.   %s", ch->alignment, buf );
   snprintf( buf, MSL, "@@c|%s @@c|\r\n", center_text( buf2, 62 ) );
   send_to_char( buf, ch );

   if( !IS_NPC(ch) )
   {
    snprintf( buf, MSL, "%s", "" );
    snprintf( buf2, MSL, "@@c|%s @@c|\r\n", center_text( buf, 62 ) );
    send_to_char( buf2, ch );

    snprintf( buf2, MSL, " @@WPersonal Records" );
    snprintf( buf, MSL, "@@c|%s @@c|\r\n", center_text( buf2, 62 ) );
    send_to_char( buf, ch );

    if( sysdata.shownumbers )
    {
     snprintf( buf2, MSL, "@@WMDam: @@y%-4d @@W(@@y%-14s@@W)  PDam: @@y%-4d @@W(@@y%-14s@@W)",
      ch->pcdata->records->mdam_amt, ch->pcdata->records->mdam_gsn > 0 ? skill_table[ch->pcdata->records->mdam_gsn].name : "none",
      ch->pcdata->records->pdam_amt, ch->pcdata->records->pdam_gsn > 0 ? ch->pcdata->records->pdam_gsn >= TYPE_HIT ? "auto attack" : skill_table[ch->pcdata->records->pdam_gsn].name : "none" );
     snprintf( buf, MSL, "@@c|%s @@c|\r\n", center_text( buf2, 62 ) );
     send_to_char( buf, ch );
    }

    snprintf( buf2, MSL, "@@WCrusades: @@y%-4d  @@WQP: @@y%-4d  @@WTotal QP: @@y%-5d",
     ch->pcdata->records->crusade, ch->pcdata->records->qp, ch->pcdata->records->qp_tot );
    snprintf( buf, MSL, "@@c|%s @@c|\r\n", center_text( buf2, 62 ) );
    send_to_char( buf, ch );

    snprintf( buf2, MSL, "@@WYou have completed @@y%d @@Wmquests and failed @@y%d@@W.",
     ch->pcdata->records->mquest_c, ch->pcdata->records->mquest_f );
    snprintf( buf, MSL, "@@c|%s @@c|\r\n", center_text( buf2, 62 ) );
    send_to_char( buf, ch );

   }

   snprintf( buf, MSL, "+===============================================================+@@g\r\n" );
   send_to_char( buf, ch );


   return;
}

DO_FUN(do_affected)
{

   char buf[MSL];
   AFFECT_DATA *paf;
   buf[0] = '\0';

   send_to_char( "\r\n@@a********************** @@mYou are affected by: @@a*********************@@N\r\n", ch );


   if( ch->first_affect != NULL )
   {
      for( paf = ch->first_affect; paf != NULL; paf = paf->next )
      {
         snprintf( buf, MSL, "Spell: '%s'", skill_table[paf->type].name );
         send_to_char( buf, ch );

         if( ch->level >= 16 )
         {
            if( paf->location > APPLY_NONE )
            {
               snprintf( buf, MSL,
                        " modifies %s by %d for %d hours", affect_loc_name( paf->location ), paf->modifier, paf->duration );

            }
            else
               snprintf( buf, MSL, " lasts for %d hours", paf->duration );
            send_to_char( buf, ch );
         }

         send_to_char( ".\r\n", ch );
      }
      /*
       * Check to see if items have applies... 
       */
   }

   if( item_has_apply( ch, ITEM_APPLY_INFRA ) )
      send_to_char( "Spell: 'Infravision' permanent duration.\r\n", ch );
   if( item_has_apply( ch, ITEM_APPLY_INV ) )
      send_to_char( "Spell: 'Invisibilty' permanent duration.\r\n", ch );
   if( item_has_apply( ch, ITEM_APPLY_DET_INV ) )
      send_to_char( "Spell: 'Detect Invisibilty' permanent duration.\r\n", ch );
   if( item_has_apply( ch, ITEM_APPLY_SANC ) )
      send_to_char( "Spell: 'Sanctuary' permanent duration.\r\n", ch );
   if( item_has_apply( ch, ITEM_APPLY_SNEAK ) )
      send_to_char( "Skill: 'Sneak' permanent duration.\r\n", ch );
   if( item_has_apply( ch, ITEM_APPLY_HIDE ) )
      send_to_char( "Skill: 'Hide' permanent duration.\r\n", ch );
   if( item_has_apply( ch, ITEM_APPLY_PROT ) )
      send_to_char( "Spell: 'Protection' permanent duration.\r\n", ch );
   if( item_has_apply( ch, ITEM_APPLY_ENHANCED ) )
      send_to_char( "Skill: 'Enhanced Damage' permanent duration.\r\n", ch );
   if( item_has_apply( ch, ITEM_APPLY_DET_MAG ) )
      send_to_char( "Spell: 'Detect Magic' permanent duration.\r\n", ch );
   if( item_has_apply( ch, ITEM_APPLY_DET_HID ) )
      send_to_char( "Spell: 'Detect Hidden' permanent duration.\r\n", ch );
   if( item_has_apply( ch, ITEM_APPLY_DET_EVIL ) )
      send_to_char( "Spell: 'Detect Evil' permanent duration.\r\n", ch );
   if( item_has_apply( ch, ITEM_APPLY_PASS_DOOR ) )
      send_to_char( "Spell: 'Pass Door' permanent duration.\r\n", ch );
   if( item_has_apply( ch, ITEM_APPLY_DET_POISON ) )
      send_to_char( "Spell: 'Detect Posion' permanent duration.\r\n", ch );
   if( item_has_apply( ch, ITEM_APPLY_FLY ) )
      send_to_char( "Spell: 'Fly' permanent duration.\r\n", ch );
   if( item_has_apply( ch, ITEM_APPLY_KNOW_ALIGN ) )
      send_to_char( "Spell: 'Know Alignment' permanent duration.\r\n", ch );


   snprintf( buf, MSL, "*****************************************************************\r\n" );
   send_to_char( buf, ch );




   return;
}

char *const day_name[] = {
   "the Moon", "the Bull", "Deception", "Thunder", "Freedom",
   "the Great Gods", "the Sun"
};

/* 
 * Number of months reduced from 17 (!) to 8
 * This is to bring the time it takes a character to age on mud year
 * down from 200+ rl hrs to 64 rl hrs
 * (Player's age stats were screwed in base merc!)
 */

char *const month_name[] = {
   "the Frost Giant", "the Old Forces",
   "the Grand Struggle", "Futility",
   "the Dark Shades", "the Long Shadows",
   "the Ancient Darkness", "the Great Evil"
};

DO_FUN(do_time)
{
   extern char str_boot_time[];
   char buf[MSL];
   char *suf;
   int day;

   day = time_info.day + 1;

   if( day > 4 && day < 20 )
      suf = "th";
   else if( day % 10 == 1 )
      suf = "st";
   else if( day % 10 == 2 )
      suf = "nd";
   else if( day % 10 == 3 )
      suf = "rd";
   else
      suf = "th";

   snprintf( buf, MSL,
            "It is %d o'clock %s, Day of %s, %d%s the Month of %s.\r\nAck! started up at %s\rThe system time is %s\r",
            ( time_info.hour % 12 == 0 ) ? 12 : time_info.hour % 12,
            time_info.hour >= 12 ? "pm" : "am",
            day_name[day % 7], day, suf, month_name[time_info.month], str_boot_time, ( char * )ctime( &current_time ) );

   send_to_char( buf, ch );
   return;
}

DO_FUN(do_weather)
{
   char buf[MSL];
   char buf2[MSL];

   static char *const sky_look[4] = {
      "cloudless",
      "cloudy",
      "rainy",
      "lit by flashes of lightning"
   };
   buf[0] = '\0';
   buf2[0] = '\0';

   if( !IS_OUTSIDE( ch ) )
   {
      send_to_char( "You can't see the weather indoors.\r\n", ch );
      return;
   }

   if( IS_SET( ch->in_room->affected_by, ROOM_BV_SHADE ) )
   {
      send_to_char( "You see no weather - a dark shadow masks the sky.\r\n", ch );
      return;
   }

   snprintf( buf, MSL, "The sky is %s and %s.\r\n",
            sky_look[weather_info.sky],
            weather_info.change >= 0 ? "a warm southerly breeze blows" : "a cold northern gust blows" );
   switch ( weather_info.moon_loc )
   {
      case MOON_DOWN:
         strncat( buf, "The moon is not in the sky.\r\n", MSL-1 );
         break;
      case MOON_RISE:
         snprintf( buf2, MSL, "A %s @@yMoon@@N is just rising.\r\n", get_moon_phase_name(  ) );
         strncat( buf, buf2, MSL-1 );
         break;
      case MOON_LOW:
         snprintf( buf2, MSL, "A %s @@yMoon@@N is low on the horizon.\r\n", get_moon_phase_name(  ) );
         strncat( buf, buf2, MSL-1 );
         break;
      case MOON_PEAK:
         snprintf( buf2, MSL, "A %s @@yMoon@@N is high above you.\r\n", get_moon_phase_name(  ) );
         strncat( buf, buf2, MSL-1 );
         break;
      case MOON_FALL:
         snprintf( buf2, MSL, "A %s @@yMoon@@N is falling.\r\n", get_moon_phase_name(  ) );
         strncat( buf, buf2, MSL-1 );
         break;
      case MOON_SET:
         snprintf( buf2, MSL, "A %s @@yMoon@@N has just set.\r\n", get_moon_phase_name(  ) );
         strncat( buf, buf2, MSL-1 );
         break;
   }

   send_to_char( buf, ch );
   return;
}

DO_FUN(do_help)
{
 FILE *fp;
 char buf[MSL];
 char *farg = argument;
 bool found = FALSE;
 bool shelp = FALSE;

 farg = one_argument(farg,buf);

 if( argument[0] != '\0' )
 {
  if( !str_prefix(buf,"-find") )
  {
   send_to_char(find_helps(farg,IS_IMMORTAL(ch) ? true : false),ch);
   return;
  }
  if( !str_prefix(buf,"-search") )
  {
   send_to_char(grep_helps(farg,IS_IMMORTAL(ch) ? true : false),ch);
   return;
  }
 }

 buf[0] = '\0';
 argument = strlower(argument);
 smash_replace(argument," ","_");
 smash_replace(argument,":","_");

 if( !str_prefix("shelp_",argument) )
  shelp = TRUE;

 if( argument[0] == '\0' )
  snprintf(buf,MSL,"%s%s/%s",HELP_DIR,initial(HELP_INDEX),HELP_INDEX);
 else if( !str_cmp(argument,BHELP_INDEX) ) /* special case for the builder */
  snprintf(buf,MSL,"%s%s/%s",HELP_DIR,initial(BHELP_INDEX),BHELP_INDEX);
 else if( !str_cmp(argument,SHELP_INDEX) ) /* special case for shelp */
  snprintf(buf,MSL,"%s%s/%s",HELP_DIR,initial(SHELP_INDEX),SHELP_INDEX);
 else
  snprintf(buf,MSL,"%s%s/%s.%s",HELP_DIR,initial(argument),argument,IS_IMMORTAL(ch) ? HELP_IMM : HELP_MORT);

 if( (fp = file_open(buf,"r")) != NULL )
 {
  found = TRUE;
  while( fgets(buf,MSL,fp) )
   send_to_char(buf,ch);
 }
 file_close(fp);
 /* Search for a plural */
 snprintf(buf,MSL,"%s%s/%ss.%s",HELP_DIR,initial(argument),argument,IS_IMMORTAL(ch) ? HELP_IMM : HELP_MORT);
 if( (fp = file_open(buf,"r")) != NULL )
 {
  found = TRUE;
  while( fgets(buf,MSL,fp) )
   send_to_char(buf,ch);
  file_close(fp);
 }
 else if( !IS_IMMORTAL(ch) && !found )
 {
  if( !shelp )
  {
   send_to_char("No help on that word.\r\n",ch);
   snprintf(buf,MSL,"Missing help: %s attempted by %s.",argument,ch->name.c_str());
   monitor_chan(buf,MONITOR_HELPS);
  }
  else
  {
   send_to_char("No sHelp for that skill/spell.\r\n",ch);
   snprintf(buf,MSL,"Missing sHelp: %s attempted by %s.",argument,ch->name.c_str());
   monitor_chan(buf,MONITOR_HELPS);
  }
  file_close(fp);
  return;
 }

 if( IS_IMMORTAL(ch) )
 {
  snprintf(buf,MSL,"%s%s/%s.%s",HELP_DIR,initial(argument),argument,HELP_MORT);
  if( (fp = file_open(buf,"r")) != NULL )
  {
   if( found )
    send_to_char("\r\n",ch);
   found = TRUE;
   while( fgets(buf,MSL,fp) )
    send_to_char(buf,ch);
  }
  file_close(fp);
  /* Search for a plural */
  snprintf(buf,MSL,"%s%s/%ss.%s",HELP_DIR,initial(argument),argument,HELP_MORT);
  if( (fp = file_open(buf,"r")) != NULL )
  {
   if( found )
    send_to_char("\r\n",ch);
   found = TRUE;
   while( fgets(buf,MSL,fp) )
    send_to_char(buf,ch);
  }
  file_close(fp);
  if( !found )
  {
   if( !shelp )
   {
    send_to_char("No help on that word.\r\n",ch);
    snprintf(buf,MSL,"Missing help: %s attempted by %s.",argument,ch->name.c_str());
    monitor_chan(buf,MONITOR_HELPS);
   }
   else
   {
    send_to_char("No sHelp for that skill/spell.\r\n",ch);
    snprintf(buf,MSL,"Missing sHelp: %s attempted by %s.",argument,ch->name.c_str());
    monitor_chan(buf,MONITOR_HELPS);
   }
   file_close(fp);
   return;
  }
 }

 return;
}



/*
 * New 'who' command originally by Alander of Rivers of Mud.
 * Changed a lot since then though :P
 * List comes in 4 parts: Imms, Adepts, remorts then morts
 */

#define SHOW_IMMORT     0
#define SHOW_ADEPT	1
#define SHOW_REMORT     2
#define SHOW_MORTAL     3
#define SHOW_FINISH     4

DO_FUN(do_who)
{
   DESCRIPTOR_DATA *d;

   char buf[MSL * 10];
   char buf2[MSL * 4];
   char buf3[MSL * 4];
   char buf4[MSL * 4];
   char fgs[MSL * 4];
   char clan_job[MSL];
   int iClass;
   int iLevelLower;
   int iLevelUpper;
   int nNumber;
   int nMatch;
   bool rgfClass[MAX_CLASS];
   bool fClassRestrict;
   bool fImmortalOnly;
   bool fadeptonly = FALSE;
   bool fremortonly = FALSE;
   int cnt, slength, excess, nlength;
   int true_cnt = 0;
   static int max_players;
   int list;
   int number[3];
   bool idle = FALSE, invis = FALSE, wanted = FALSE;
   bool cangroup = FALSE;
   int stop_counter = 0;

   /*
    * Set default arguments.
    */
   buf[0] = '\0';
   buf2[0] = '\0';
   buf3[0] = '\0';
   buf4[0] = '\0';

   iLevelLower = 0;
   iLevelUpper = MAX_LEVEL;
   fClassRestrict = FALSE;
   fImmortalOnly = FALSE;
   for( iClass = 0; iClass < MAX_CLASS; iClass++ )
      rgfClass[iClass] = FALSE;
   /*
    * Parse arguments.
    */
   nNumber = 0;
   for( ;; )
   {
      char arg[MSL];

      argument = one_argument( argument, arg );
      if( arg[0] == '\0' )
         break;

      if( is_number( arg ) )
      {
         switch ( ++nNumber )
         {
            case 1:
               iLevelLower = atoi( arg );
               break;
            case 2:
               iLevelUpper = atoi( arg );
               break;
            default:
               send_to_char( "Only two level numbers allowed.\r\n", ch );
               return;
         }
      }
      else
      {
         if( strlen( arg ) < 3 )
         {
            send_to_char( "Classes must be longer than that.\r\n", ch );
            return;
         }
         /*
          * Look for classes to turn on.
          */
         if( !str_prefix( arg, "group" ) )
         {
            cangroup = TRUE;
            break;
         }
         if( !str_prefix( arg, "adept" ) )
         {
            fadeptonly = TRUE;
            break;
         }
         if( !str_prefix( arg, "remort" ) )
         {
            fremortonly = TRUE;
            break;
         }


         arg[3] = '\0';
         if( !str_cmp( arg, "imm" ) )
         {
            fImmortalOnly = TRUE;
         }

         else
         {
            fClassRestrict = TRUE;
            for( iClass = 0; iClass < MAX_CLASS; iClass++ )
            {
               if( !str_cmp( arg, class_table[iClass].who_name ) )
               {
                  rgfClass[iClass] = TRUE;
                  break;
               }
            }

            if( ( iClass == MAX_CLASS ) && ( !( cangroup ) ) )
            {
               send_to_char( "That's not a class.\r\n", ch );
               return;
            }
         }
      }
   }

   buf[0] = '\0';

   snprintf( buf, MSL, "WHO Listing: " mudnamecolor "\r\n" );
   strncat( buf,
                "@@R+-----------------------------------------------------------------------------+\r\n", MSL );
   strncat( buf,
                "| @@mSo Mo An Ki Ne@@R                                                              |\r\n", MSL );
   strncat( buf,
                "| @@bMa Cl Th Wa Ps @@eRace Clan  ABJPW    Player	Title		      @@R(flags) @@R|\r\n", MSL );
   strncat( buf,
                "|---------------------------------+-------------------------------------------|\r\n", MSL );

   for( list = SHOW_IMMORT; list < SHOW_FINISH; list++ )
   {
      number[list] = 0;

      for( d = first_desc; d != NULL; d = d->next )
      {
         CHAR_DATA *wch;

         if( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
            continue;

         wch = ( d->original != NULL ) ? d->original : d->character;
         if( ( list == SHOW_IMMORT && wch->level < LEVEL_HERO )
             || ( list == SHOW_REMORT && ( !IS_REMORT( wch ) || wch->level >= LEVEL_HERO ) )
             || ( list == SHOW_MORTAL && ( IS_REMORT( wch ) || wch->level >= LEVEL_HERO ) )
             || ( list == SHOW_ADEPT && ( !IS_ADEPT(wch) ) )
             || ( list == SHOW_ADEPT && ( wch->level >= LEVEL_HERO ) )
             || ( list == SHOW_REMORT && ( IS_ADEPT(wch) ) ) )
            continue;

         if( wch->level < iLevelLower
             || wch->level > iLevelUpper
             || ( fImmortalOnly && wch->level < LEVEL_HERO )
             || ( fadeptonly && ( ( !IS_ADEPT(wch) ) || wch->level >= LEVEL_HERO ) )
             || ( fClassRestrict && !rgfClass[wch->p_class] )
             || ( fremortonly && ( !IS_REMORT( wch ) || ( wch->level >= LEVEL_HERO ) || ( IS_ADEPT(wch) ) ) ) )
            continue;

/* Multiple grouping restriction checks  Zen */

         {
            bool ch_adept = FALSE, ch_dremort = FALSE, ch_sremort = FALSE, victim_adept = FALSE,
               victim_dremort = FALSE, victim_sremort = FALSE, legal_group = FALSE;

            if( IS_ADEPT(ch) )
               ch_adept = TRUE;
            if( IS_ADEPT(wch) )
               victim_adept = TRUE;

            if( ch->get_level("psuedo") > 97 )
               ch_dremort = TRUE;
            if( wch->get_level("psuedo") > 97 )
               victim_dremort = TRUE;

            if( ch->get_level("psuedo") > 80 )
               ch_sremort = TRUE;
            if( wch->get_level("psuedo") > 80 )
               victim_sremort = TRUE;


            if( ch_adept && victim_adept )
            {
               legal_group = TRUE;
            }
            else if( ( ch_adept && victim_dremort ) || ( victim_adept && ch_dremort ) )
            {
               if( abs( ch->get_level("psuedo") - wch->get_level("psuedo") ) < 9 )
                  legal_group = TRUE;
               else
                  legal_group = FALSE;

            }
            else if( ch_dremort || victim_dremort || ch_sremort || victim_sremort )
            {
               if( abs( ch->get_level("psuedo") - wch->get_level("psuedo") < 8 ) )
                  legal_group = TRUE;

            }

            else
            {
               if( abs( ch->get_level("psuedo") - wch->get_level("psuedo") ) < 21 )
                  legal_group = TRUE;
               /*
                * send_to_char( "No Remorts\r\n", ch );  
                */

            }

            if( IS_IMMORTAL( wch ) )
               legal_group = FALSE;

            if( !legal_group && cangroup )
               continue;

         }


         number[list]++;
      }
   }

   /*
    * Now show matching chars.
    */
   nMatch = 0;
   buf3[0] = '\0';
   buf4[0] = '\0';



   for( list = SHOW_IMMORT; list < SHOW_FINISH; list++ )
   {
      stop_counter = 0;
      if( number[list] == 0 )
         continue;


      switch ( list )
      {
         case SHOW_IMMORT:
            strncat( buf,
                         "@@R|---------------------------------|----------@@lImmortals@@R------------------------|@@g\r\n", MSL );
            break;

         case SHOW_ADEPT:
            strncat( buf,
                         "@@R|---------------------------------|------------@@WAdepts@@R-------------------------|@@g\r\n", MSL );
            break;

         case SHOW_REMORT:
            strncat( buf,
                         "@@R|---------------------------------|----------@@mRemortals@@R------------------------|@@g\r\n", MSL );
            break;
         case SHOW_MORTAL:
            strncat( buf,
                         "@@R|---------------------------------|-----------@@cMortals@@R-------------------------|@@g\r\n", MSL );

            break;
      }

      true_cnt = 0;


      for( d = first_desc; d != NULL; d = d->next )
      {
         CHAR_DATA *wch;
         char const *p_class;
         /*
          * Check for match against restrictions.
          * Don't use trust as that exposes trusted mortals.
          */
         true_cnt++;

         if( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
            continue;

         wch = ( d->original != NULL ) ? d->original : d->character;
         /*
          * Check to see if we're showing the correct character for
          * * each segment of the loop... 
          */
         if( ( list == SHOW_IMMORT && wch->level < LEVEL_HERO )
             || ( list == SHOW_REMORT && ( !IS_REMORT( wch ) || wch->level >= LEVEL_HERO ) )
             || ( list == SHOW_MORTAL && ( IS_REMORT( wch ) || wch->level >= LEVEL_HERO ) )
             || ( list == SHOW_ADEPT && ( !IS_ADEPT(wch) ) )
             || ( list == SHOW_ADEPT && ( wch->level >= LEVEL_HERO ) )
             || ( list == SHOW_REMORT && ( IS_ADEPT(wch) ) ) )
            continue;

         if( wch->level < iLevelLower
             || wch->level > iLevelUpper
             || ( fImmortalOnly && wch->level < LEVEL_HERO )
             || ( fadeptonly && ( ( !IS_ADEPT(wch) ) || wch->level >= LEVEL_HERO ) )
             || ( fClassRestrict && !rgfClass[wch->p_class] )
             || ( fremortonly && ( !IS_REMORT( wch ) || ( wch->level >= LEVEL_HERO ) || ( IS_ADEPT(wch) ) ) ) )

            continue;

/* Multiple grouping restriction checks  Zen */

         {
            bool ch_adept = FALSE, ch_dremort = FALSE, ch_sremort = FALSE, victim_adept = FALSE,
               victim_dremort = FALSE, victim_sremort = FALSE, legal_group = FALSE;

            if( IS_ADEPT(ch) )
               ch_adept = TRUE;
            if( IS_ADEPT(wch) )
               victim_adept = TRUE;

            if( ch->get_level("psuedo") > 97 )
               ch_dremort = TRUE;
            if( wch->get_level("psuedo") > 97 )
               victim_dremort = TRUE;

            if( ch->get_level("psuedo") > 80 )
               ch_sremort = TRUE;
            if( wch->get_level("psuedo") > 80 )
               victim_sremort = TRUE;


            if( ch_adept && victim_adept )
            {
               legal_group = TRUE;
            }
            else if( ( ch_adept && victim_dremort ) || ( victim_adept && ch_dremort ) )
            {
               if( abs( ch->get_level("psuedo") - wch->get_level("psuedo") ) < 9 )
                  legal_group = TRUE;

            }
            else if( ch_dremort || victim_dremort || ch_sremort || victim_sremort )
            {
               if( abs( ch->get_level("psuedo") - wch->get_level("psuedo") ) < 8 )
                  legal_group = TRUE;
               else
                  legal_group = FALSE;

            }

            else
            {
               if( abs( ch->get_level("psuedo") - wch->get_level("psuedo") ) < 21 )
                  legal_group = TRUE;
               /*
                * send_to_char( "No Remorts\r\n", ch );  
                */

            }
            if( IS_IMMORTAL( wch ) )
               legal_group = FALSE;
            if( !legal_group && cangroup )
               continue;

         }



         if( stop_counter > 45 )
            continue;
         stop_counter++;

         nMatch++;
         /*
          * Figure out what to print for class.
          */
         p_class = class_table[wch->p_class].who_name;

         if( str_cmp( wch->pcdata->who_name, "off" ) )
            p_class = wch->pcdata->who_name;
         else
         {
            switch ( wch->level )
            {
               default:
                  break;
               case MAX_LEVEL - 0:
                  p_class = "@@l-* CREATOR *-@@g ";
                  break;
               case MAX_LEVEL - 1:
                  p_class = "@@B-= SUPREME =-@@g ";
                  break;
               case MAX_LEVEL - 2:
                  p_class = "@@a--  DEITY  --@@g ";
                  break;
               case MAX_LEVEL - 3:
                  p_class = "@@c - IMMORTAL- @@g ";
                  break;
               case MAX_LEVEL - 4:
                  p_class = "@@W    ADEPT  @@N   ";
                  break;
            }

         }


         if( wch->act.test(ACT_AMBASSADOR) )
         {
            snprintf( buf3, MSL, "   AMBASSADOR  " );
         }
         else
         {
            if( wch->level >= ( MAX_LEVEL - 4 ) || str_cmp( wch->pcdata->who_name, "off" ) )
            {
               switch ( wch->level )
               {
                  case MAX_LEVEL - 0:
                     snprintf( buf3, MSL, "@@l %s@@g", p_class );
                     break;
                  case MAX_LEVEL - 1:
                     snprintf( buf3, MSL, "@@B %s@@g", p_class );
                     break;
                  case MAX_LEVEL - 2:
                     snprintf( buf3, MSL, "@@a %s@@g", p_class );
                     break;
                  case MAX_LEVEL - 3:
                     snprintf( buf3, MSL, "@@c %s@@g", p_class );
                     break;
                  default:
                     snprintf( buf3, MSL, "@@W %s@@g", p_class );
                     break;
               }
            }
            else
            {
               buf4[0] = '\0';
               buf3[0] = '\0';

               for( cnt = 0; cnt < MAX_CLASS; cnt++ )
               {
                  if( wch->lvl2[cnt] > 0 )
                  {
                     snprintf( buf4, MSL, "@@m%3d@@N", wch->lvl2[cnt] );

                  }
                  else
                  {
                     if( wch->lvl[cnt] <= 0 )
                        snprintf( buf4, MSL, "@@d%3d@@N", 0 );
                     else
                        snprintf( buf4, MSL, "@@b%3d@@N", wch->lvl[cnt] );
                  }

                  strncat( buf3, buf4, MSL );

               }
            }
         }
         /*
          * Work out what to show for 'flags' column.
          * PKOK, AFK, LEADER, WRITING, BUILDING 
          * ADDED: race and clan (Stephen)
          */
         clan_job[0] = '\0';
         if( wch->act.test(ACT_CBOSS) )
            strncat( clan_job, "*", MSL );
         else if( wch->act.test(ACT_CLEADER) )
            strncat( clan_job, "L", MSL );
         else if( wch->act.test(ACT_CARMORER) )
            strncat( clan_job, "!", MSL );
         else
            strncat( clan_job, " ", MSL );

         snprintf( fgs, MSL, "%3s %5s %s%s%s%s%s",
                  race_table[wch->race].race_name,
                  clan_table[wch->clan].clan_abbr,
                  wch->act.test(ACT_AFK) ? "A" : " ",
                  wch->position == POS_BUILDING ? "B" : " ",
                  clan_job,
                  wch->act.test(ACT_PKOK) ? "P" : " ", wch->position == POS_WRITING ? "W" : " " );
         /*
          * Oh look... another hack needed due to change in who format!
          */
         /*
          * Make sure that the title (and wanted/idle flag) will fit ok
          */
         /*
          * excess holds number of chars we have to lose ):
          */
         nlength = strlen( wch->name.c_str() );
         slength = 1 + my_strlen( wch->get_title() ) + nlength;

         excess = 0;

         idle = FALSE;
         wanted = FALSE;
         invis = FALSE;
         if( wch->act.test(ACT_WIZINVIS) )
         {
            excess += 6;
            invis = TRUE;
         }
         else
         {
            if( wch->act.test(ACT_KILLER) || wch->act.test(ACT_THIEF) )
            {
               excess += 8;
               wanted = TRUE;
            }

            if( wch->timer > 5 )
            {
               excess += 10;
               idle = TRUE;
            }
         }
         snprintf( buf4, MSL, "%s", wch->get_title() );

         if( slength + excess > 43 )
         {
            if( invis )
               snprintf( buf4, MSL, "%1.26s", wch->get_title() );
            else if( idle && !wanted )
               snprintf( buf4, MSL, "%1.24s", wch->get_title() );
            else if( !idle && wanted )
               snprintf( buf4, MSL, "%1.28s", wch->get_title() );
            else if( idle && wanted )
               snprintf( buf4, MSL, "%1.18s", wch->get_title() );
            else
               snprintf( buf4, MSL, "%1.31s", wch->get_title() );


         }
         else
         {
            for( cnt = slength + excess; cnt < 43; cnt++ )
            {
               strncat( buf4, " ", MSL );
            }
         }
         if( invis )
         {
            if( slength + excess <= 43 )
            {
               snprintf( buf + strlen( buf ), MSL, "@@R|%s%s%s  %s @@R| %s%s%s%s",
                        color_string( ch, "stats" ),
                        buf3,
                        color_string( ch, "stats" ),
                        fgs,
                        color_string( ch, "stats" ), wch->name.c_str(), buf4, wch->act.test(ACT_WIZINVIS) ? "(WIZI)" : "" );
               snprintf( buf + strlen( buf ), MSL, "@@R|@@g\r\n" );
            }
            else
            {
               snprintf( buf + strlen( buf ), MSL, "@@R|%s%s%s  %s @@R| %s%s%s%s",
                        color_string( ch, "stats" ),
                        buf3,
                        color_string( ch, "stats" ),
                        fgs,
                        color_string( ch, "stats" ), wch->name.c_str(), buf4, wch->act.test(ACT_WIZINVIS) ? " (WIZI) " : "" );
               snprintf( buf + strlen( buf ), MSL, "@@R|@@g\r\n" );
            }
         }
         else
         {
            snprintf( buf + strlen( buf ), MSL, "@@R|%s%s%s  %s @@R| %s%s%s%s",
                     color_string( ch, "stats" ),
                     buf3,
                     color_string( ch, "stats" ),
                     fgs,
                     color_string( ch, "stats" ),
                     wch->name.c_str(),
                     buf4, ( wch->act.test(ACT_KILLER) || wch->act.test(ACT_THIEF) ) ? "(WANTED)" : "" );



            if( wch->timer > 5 )
               snprintf( buf + strlen( buf ), MSL, "[IDLE:%2d] @@R|@@g\r\n", wch->timer );
            else
               snprintf( buf + strlen( buf ), MSL, "@@R|@@g\r\n" );
         }
      }

      strncat( buf,
                   "@@R|---------------------------------+-------------------------------------------|\r\n", MSL );
      send_to_char( buf, ch );
      buf[0] = '\0';
   }

   if( true_cnt > max_players )
      max_players = true_cnt;
   snprintf( buf4, MSL, "(%d Player%s)  KEY: (A)fk  (B)uilding  (*)Clan Boss  (P)kok  (W)riting", nMatch, nMatch == 1 ? "" : "s" );
   snprintf( buf2, MSL, "@@R|@@G %s @@R|\r\n", center_text( buf4, 75 ) );
   strncat( buf, buf2, MSL );
   snprintf( buf4, MSL, " (L) Clan Leader  (!) Clan Armourer " );
   snprintf( buf2, MSL, "@@R|@@G %s @@R|\r\n", center_text( buf4, 75 ) );
   strncat( buf, buf2, MSL );

   snprintf( buf4, MSL, "There have been a maximum of %d player%s logged on this session",
            max_players, max_players == 1 ? "" : "s" );
   snprintf( buf2, MSL, "@@R|@@G %s @@R|\r\n", center_text( buf4, 75 ) );
   strncat( buf, buf2, MSL );
   strncat( buf,
                "@@R+-----------------------------------------------------------------------------+\r\n", MSL );

/* Removed this... out is used once, to add a single color code... D'Oh! to me --Stimpy
    snprintf( out, "%s%s", buf, color_string( ch, "normal" ) );
*/

   send_to_char( buf, ch );
   send_to_char( color_string( ch, "normal" ), ch );

/* #endif  */

   return;
}

DO_FUN(do_inventory)
{
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "All your inventory has transformed into fur!!!@@N\r\n", ch );
      return;
   }

   send_to_char( "You are carrying:\r\n", ch );
   show_list_to_char( ch->first_carry, ch, TRUE, TRUE );
   return;
}

DO_FUN(do_equipment)
{
   do_wear( ch, "" );
   return;

}

DO_FUN(do_compare)
{
   char arg1[MSL];
   char arg2[MSL];
   OBJ_DATA *obj1;
   OBJ_DATA *obj2;
   int value1;
   int value2;
   char *msg;
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "All your inventory has transformed into fur!!!@@N\r\n", ch );
      return;
   }

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   if( arg1[0] == '\0' )
   {
      send_to_char( "Compare what to what?\r\n", ch );
      return;
   }

   if( ( obj1 = get_obj_carry( ch, arg1 ) ) == NULL )
   {
      send_to_char( "You do not have that item.\r\n", ch );
      return;
   }

   if( arg2[0] == '\0' )
   {
      for( obj2 = ch->first_carry; obj2 != NULL; obj2 = obj2->next_in_carry_list )
      {
         if( obj2->wear_loc != WEAR_NONE
             && can_see_obj( ch, obj2 )
             && obj1->item_type == obj2->item_type )
            break;
      }

      if( obj2 == NULL )
      {
         send_to_char( "You aren't wearing anything comparable.\r\n", ch );
         return;
      }
   }
   else
   {
      if( ( obj2 = get_obj_carry( ch, arg2 ) ) == NULL )
      {
         send_to_char( "You do not have that item.\r\n", ch );
         return;
      }
   }

   msg = NULL;
   value1 = 0;
   value2 = 0;

   if( obj1 == obj2 )
   {
      msg = "You compare $p to itself.  It looks about the same.";
   }
   else if( obj1->item_type != obj2->item_type )
   {
      msg = "You can't compare $p and $P.";
   }
   else
   {
      switch ( obj1->item_type )
      {
         default:
            msg = "You can't compare $p and $P.";
            break;

         case ITEM_ARMOR:
            value1 = obj1->value[0];
            value2 = obj2->value[0];
            break;

         case ITEM_WEAPON:
            value1 = obj1->value[1] + obj1->value[2];
            value2 = obj2->value[1] + obj2->value[2];
            break;
      }
   }

   if( msg == NULL )
   {
      if( value1 == value2 )
         msg = "$p and $P look about the same.";
      else if( value1 > value2 )
         msg = "$p looks better than $P.";
      else
         msg = "$p looks worse than $P.";
   }

   act( msg, ch, obj1, obj2, TO_CHAR );
   return;
}

DO_FUN(do_credits)
{
   do_help( ch, "diku" );
   do_help( ch, "ack" );
   return;
}

DO_FUN(do_where)
{
   char buf[MSL];
   char arg[MSL];
   CHAR_DATA *victim;
   list<CHAR_DATA *>::iterator li;
   DESCRIPTOR_DATA *d;
   bool found;
   buf[0] = '\0';

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Players near you:\r\n", ch );
      snprintf( buf, MSL, "In %s %s @@N:\r\n", ch->in_room->area->level_label, ch->in_room->area->name );
      send_to_char( buf, ch );
      found = FALSE;
      for( d = first_desc; d; d = d->next )
      {
         if( d->connected == CON_PLAYING
             && ( victim = d->character ) != NULL
             && !IS_NPC( victim )
             && victim->in_room != NULL
             && victim->in_room->area == ch->in_room->area
             && can_see( ch, victim ) && ( !IS_WOLF( victim ) || ( !IS_SHIFTED( victim ) && !IS_RAGED( victim ) ) ) )
         {
            found = TRUE;
            snprintf( buf, MSL, "%-28s %s\r\n", victim->name.c_str(), victim->in_room->name );
            send_to_char( buf, ch );
         }
      }
      if( !found )
         send_to_char( "None\r\n", ch );
   }
   else
   {
      found = FALSE;
      for( li = char_list.begin(); li != char_list.end(); li++ )
      {
         victim = *li;
         /*
          * &&   victim->in_room->area == ch->in_room->area
          * * taken out from below to allow global where use
          * * and then put back in... global where no fun at all. ;)
          * * -- Stephen
          */

         if( victim->in_room != NULL
             && ( !IS_AFFECTED( victim, AFF_HIDE ) && !item_has_apply( victim, ITEM_APPLY_HIDE ) )
             && ( victim->in_room->area == ch->in_room->area )
             && ( !IS_AFFECTED( victim, AFF_SNEAK ) && !item_has_apply( victim, ITEM_APPLY_SNEAK ) )
             && can_see( ch, victim ) && is_name( arg, const_cast<char *>(victim->name.c_str()) ) )
         {
            found = TRUE;
            snprintf( buf, MSL, "%-28s %s\r\n", victim->get_name(ch), victim->in_room->name );
            send_to_char( buf, ch );
            break;
         }
      }
      if( !found )
         act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );
   }

   return;
}

DO_FUN(do_consider)
{
   char arg[MSL];
   CHAR_DATA *victim;
   char *msg = '\0';
   char *buf = '\0';
   char *buf2 = '\0';
   char *buf3 = '\0';
   char *buf4 = '\0';
   float diff;
   int hpdiff;
   int hrdiff;
   int drdiff;
   int acdiff;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Consider killing whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They're not here.\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) )
      send_to_char( "Remember there are downfalls to PKilling!\r\n", ch );

   /*
    * Stephen - bypass class adjustment if victim == NPC 
    */
   /*
    * Also, only look at modifiers if victim == NPC 
    */

   diff = ( victim->get_level("psuedo") - ch->get_level("psuedo") );
   /*
    * Additions for difficulty. 
    */
   /*
    * Class:   
    */
   if( !IS_NPC( victim ) )
   {
      switch ( ch->p_class )
      {
         case 0:
            diff += 0.3;
            break;   /* Mage       */
         case 1:
            diff += 0.4;
            break;   /* Cleric     */
         case 2:
            diff += 0.1;
            break;   /* Thief      */
         case 3:
            diff += 0;
            break;   /* Warrior    */
         case 4:
            diff -= 0.4;
            break;   /* Psionicist */
         case 5:
            diff += 0.5;
            break;   /* Conjurer   */
         case 6:
            diff -= 0.2;
            break;   /* Templar    */
         case 7:
            diff += 0.2;
            break;   /* Ranger     */
      }
   }

   /*
    * Mod rolls. 
    */
   if( IS_NPC( victim ) )
   {
      diff += victim->npcdata->hr_mod / 4;
      diff += victim->npcdata->dr_mod / 4;
      diff -= victim->npcdata->ac_mod / 30;
   }

   if( diff >= 10 )
      msg = "Death will thank you for your gift.";
   if( diff <= 9 )
      msg = "$N laughs at you mercilessly.";
   if( diff <= 4 )
      msg = "$N says 'Do you feel lucky, punk?'.";
   if( diff <= 1 )
      msg = "The perfect match!";
   if( diff <= -2 )
      msg = "$N looks like an easy kill.";
   if( diff <= -5 )
      msg = "$N is no match for you.";
   if( diff <= -10 )
      msg = "You can kill $N naked and weaponless.";

   act( msg, ch, NULL, victim, TO_CHAR );
   msg = "";

   /*
    * additions by king@tinuviel.cs.wcu.edu 
    */
   hpdiff = ( ch->hit - victim->hit );

   if( ( ( diff >= 0 ) && ( hpdiff <= 0 ) ) || ( ( diff <= 0 ) && ( hpdiff >= 0 ) ) )
   {
      send_to_char( "Also,", ch );
   }
   else
   {
      send_to_char( "However,", ch );
   }

   if( hpdiff >= 101 )
      buf = " you are currently much healthier than $E.";
   if( hpdiff <= 100 )
      buf = " you are currently healthier than $E.";
   if( hpdiff <= 50 )
      buf = " you are currently slightly healthier than $E.";
   if( hpdiff <= 25 )
      buf = " you are a teensy bit healthier than $E.";
   if( hpdiff <= 0 )
      buf = " $E is a teensy bit healthier than you.";
   if( hpdiff <= -25 )
      buf = " $E is slightly healthier than you.";
   if( hpdiff <= -50 )
      buf = " $E is healthier than you.";
   if( hpdiff <= -100 )
      buf = " $E is much healthier than you.";

   act( buf, ch, NULL, victim, TO_CHAR );
   buf = "";

/* LLolth added the following code to make consider show more information */
/* root@vampyre.net */

   drdiff = ( GET_DAMROLL( ch ) - GET_DAMROLL( victim ) );
   if( drdiff >= 20 )
      buf2 = "You hit alot harder than $E.";
   if( drdiff <= 10 )
      buf2 = "You hit slightly harder than $E.";
   if( drdiff <= 5 )
      buf2 = "$E hits about the same as you.";
   if( drdiff <= -5 )
      buf2 = "$E hits about the same as you.";
   if( drdiff <= -10 )
      buf2 = "$E hits slightly harder than you.";
   if( drdiff <= -20 )
      buf2 = "$E hits alot harder than you.";
   act( buf2, ch, NULL, victim, TO_CHAR );
   buf = "";

   hrdiff = ( GET_HITROLL( ch ) - GET_HITROLL( victim ) );
   if( hrdiff >= 20 )
      buf3 = "You hit alot more often than $E.";
   if( hrdiff <= 10 )
      buf3 = "You hit slightly more often than $E.";
   if( hrdiff <= 5 )
      buf3 = "You and $E hit about equally as often.";
   if( hrdiff <= -10 )
      buf3 = "$E hits slightly more often than you.";
   if( hrdiff <= -20 )
      buf3 = "$E hits alot more often than you.";
   act( buf3, ch, NULL, victim, TO_CHAR );
   buf3 = "";

   acdiff = ( GET_AC( victim ) - GET_AC( ch ) );
   if( acdiff >= 100 )
      buf4 = "You are Armored like a tank compared to $E.";
   if( acdiff <= 75 )
      buf4 = "You are armored quite a bit better than $E is.";
   if( acdiff <= 50 )
      buf4 = "You are more heavily armored than $E is.";
   if( acdiff <= 25 )
      buf4 = "You are slightly more armored than $E is.";
   if( acdiff <= 5 )
      buf4 = "You and $E are armored pretty equally.";
   if( acdiff <= -6 )
      buf4 = "$E is slightly more armored than you are.";
   if( acdiff <= -50 )
      buf4 = "$E is more heavily armored than you are.";
   if( acdiff <= -75 )
      buf4 = "$E is armored quite a bit better than you are.";
   if( acdiff <= -100 )
      buf4 = "$E is armored like a tank compared to you.";

   act( buf4, ch, NULL, victim, TO_CHAR );
   buf = "";
   return;
}

DO_FUN(do_title)
{
   /*
    * Changed this to limit title length, and to remove and brackets. -S-
    */

   char buf[MSL];
   int cnt;
   bool changed;
   buf[0] = '\0';

   if( IS_NPC( ch ) )
      return;

   if( argument[0] == '\0' )
   {
      send_to_char( "Change your title to what?\r\n", ch );
      return;
   }
   changed = FALSE;

   for( cnt = 0; cnt < static_cast<int>(strlen( argument )); cnt++ )
   {
      if( argument[cnt] == '[' || argument[cnt] == ']' )
      {
         changed = TRUE;
         argument[cnt] = ( argument[cnt] == ']' ) ? '>' : '<';
      }
   }

   if( changed )
      send_to_char( "You used either [ or ] in your title.  They have been removed!\r\n", ch );

   /*
    * my_strlen handles color codes as zero length
    */
   if( my_strlen( argument ) > 32 )
   {
      send_to_char( "Title too long.  Please try again.\r\n", ch );
      return;
   }

   smash_tilde( argument );
   ch->set_title(argument);
   snprintf( buf, MSL, "You are now: %s%s.\r\n", ch->name.c_str(), ch->get_title() );
   send_to_char( buf, ch );
}

DO_FUN(do_description)
{
 char *farg = argument;

 if( !str_cmp(argument,"clear") )
 {
  ch->description.clear();
  return;
 }

 if( farg[0] != '\0' )
 {
  smash_tilde(farg);

  if( farg[0] == '+' )
  {
   *farg++; *farg++; /* Skip '+' and ' '. */
   if( strlen(ch->description.c_str()) + strlen(farg) >= MSL )
   {
    send_to_char("Description too long.\r\n",ch);
    return;
   }

   if( !ch->description.empty() )
   {
    ch->description += farg;
    ch->description += "\r\n";
   }
  }
  else
  {
   if( strlen(farg) >= MSL )
   {
    send_to_char("Description too long.\r\n",ch);
    return;
   }
   ch->description = farg;
   ch->description += "\r\n";
  }
 }

 send_to_char( "Your description is:\r\n", ch );
 send_to_char( !ch->description.empty() ? ch->description : "(None).\r\n", ch );

 return;
}

DO_FUN(do_report)
{
   char buf[MSL];

   snprintf( buf, MSL,
            "You report: %d/%d hp %d/%d mana %d/%d mv %d xp.\r\n",
            ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move, ch->max_move, ch->exp );

   send_to_char( buf, ch );

   snprintf( buf, MSL, "$n reports: %d/%d hp %d/%d mana %d/%d mv %d xp.",
            ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move, ch->max_move, ch->exp );

   act( buf, ch, NULL, NULL, TO_ROOM );

   return;
}

DO_FUN(do_practice)
{
   char buf[MSL];
   CHAR_DATA *mob;
   int cnt;
   int sn;
   int ack;
   int p_class;

   /*
    * Now need to check through ch->lvl[] to see if player's level in
    * * the required class is enough for him/her to be able to prac the
    * * skill/spell.  Eg if char is cle:10 and war:50, we don't want the
    * * player to be getting level 50 cleric spells, which would happen
    * * if ch->p_class was used here! -S-
    */
   buf[0] = '\0';

   if( IS_NPC( ch ) )
      return;

   if( ch->level < 3 )
   {
      send_to_char( "You must be third level to practice.  Go train instead!\r\n", ch );
      return;
   }
   /*
    * moved check for mob here. -S- 
    */

   for( mob = ch->in_room->first_person; mob != NULL; mob = mob->next_in_room )
   {
      if( IS_NPC( mob ) && mob->act.test(ACT_PRACTICE) )
         break;
   }


   if( argument[0] == '\0' )
   {
      int col;
      bool ok;
      col = 0;

      for( sn = 0; sn < MAX_SKILL; sn++ )
      {
         ok = FALSE;

         if( skill_table[sn].name == NULL )
            break;


         /*
          * Check ch->lvl[]
          */
         for( cnt = 0; cnt < MAX_CLASS; cnt++ )
            if( ( ( ( ch->lvl[cnt] >= skill_table[sn].skill_level[cnt] ) && ( skill_table[sn].flag1 == MORTAL ) )
                  || ( ( ch->lvl2[cnt] >= skill_table[sn].skill_level[cnt] ) && ( skill_table[sn].flag1 == REMORT ) ) )
                && ( skill_table[sn].flag2 != VAMP ) && ( skill_table[sn].flag2 != WOLF ) )
               ok = TRUE;

         if( ok || ch->pcdata->learned[sn] != 0 )
         {
            if( ch->pcdata->learned[sn] == 0 && mob == NULL )
            {
               /*
                * Not in prac room.  Only show what has been learnt 
                */
               continue;
            }
            if( ch->pcdata->learned[sn] > 0 )
            {
               snprintf( buf + strlen(buf), MSL, "@@W%25s-@@y%-7s@@g  ", skill_table[sn].name, learnt_name( ch->pcdata->learned[sn] ) );

               if( ++col % 3 == 0 )
               {
                  strncat( buf, "\r\n", MSL );
                  send_to_char(buf,ch);
                  buf[0] = '\0';
               }
            }
         }
      }

      if( col % 3 != 0 )
         strncat( buf, "\r\n", MSL );

      send_to_char( buf, ch );

      snprintf( buf, MSL, "\r\nYou have %d practice sessions left.\r\n", ch->pcdata->practice );
      send_to_char( buf, ch );
   }
   else
   {
      int adept;
      bool ok;

      if( !IS_AWAKE( ch ) )
      {
         send_to_char( "In your dreams, or what?\r\n", ch );
         return;
      }
      for( mob = ch->in_room->first_person; mob != NULL; mob = mob->next_in_room )
      {
         if( IS_NPC( mob ) && mob->act.test(ACT_PRACTICE) )
            break;
      }

      if( mob == NULL )
      {
         send_to_char( "You can't do that here.\r\n", ch );
         return;
      }

      if( ch->pcdata->practice <= 0 )
      {
         send_to_char( "You have no practice sessions left.\r\n", ch );
         return;
      }



      if( ( sn = skill_lookup( argument ) ) < 0 )
      {
         send_to_char( "You can't practice that.\r\n", ch );
         return;
      }
      p_class = 0;
      ack = -1;
      ok = FALSE;



      if( ( skill_table[sn].flag2 == VAMP ) || ( skill_table[sn].flag2 == WOLF ) )
      {
         send_to_char( "You can't practice that.\r\n", ch );
         return;
      }
      if( ( skill_table[sn].flag1 == ADEPT ) && IS_ADEPT(ch) && ( ch->get_level("adept") >= skill_table[sn].skill_level[0] ) )
      {
         p_class = 0;
         ok = TRUE;
      }
      else
      {
         for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         {
            if( ( ch->lvl[cnt] >= skill_table[sn].skill_level[cnt] ) && ( skill_table[sn].flag1 == MORTAL ) )
            {
               if( ch->lvl[cnt] > ack )
               {
                  ack = ch->lvl[cnt];
                  p_class = cnt;
               }

               ok = TRUE;
            }

            else if( ( ch->lvl2[cnt] >= skill_table[sn].skill_level[cnt] ) && ( skill_table[sn].flag1 == REMORT ) )
            {
               if( ch->lvl2[cnt] > ack )
               {
                  ack = ch->lvl2[cnt];
                  p_class = cnt;
               }
               ok = TRUE;
            }
         }
      }
      if( !ok )
      {
         send_to_char( "You can't practice that.\r\n", ch );
         return;
      }
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         if( ch->pcdata->order[cnt] == p_class )
            break;

      adept = IS_NPC( ch ) ? 100 : ( 90 - ( cnt * 4 ) );
      if( skill_table[sn].flag1 == ADEPT )
         adept = 95;

      if( ch->pcdata->learned[sn] >= adept )
      {
         snprintf( buf, MSL, "You are already know %s as well as is currently possible.\r\n", skill_table[sn].name );
         send_to_char( buf, ch );
      }
      else
      {
         ch->pcdata->practice--;
         ch->pcdata->learned[sn] += int_app[get_curr_int( ch )].learn;
         if( ch->pcdata->learned[sn] < adept )
         {
            act( "You practice $T.", ch, NULL, skill_table[sn].name, TO_CHAR );
            act( "$n practices $T.", ch, NULL, skill_table[sn].name, TO_ROOM );
         }
         else
         {
            ch->pcdata->learned[sn] = adept;
            act( "You are now a master of $T.", ch, NULL, skill_table[sn].name, TO_CHAR );
            act( "$n is now a master of $T.", ch, NULL, skill_table[sn].name, TO_ROOM );
         }
      }
   }
   return;
}

/*
 * 'Wimpy' originally by Dionysos.
 */
DO_FUN(do_wimpy)
{
   char buf[MSL];
   char arg[MSL];
   int wimpy;
   if( deathmatch )
   {
      send_to_char( "Not during a @@eDeath MAtch@@N!!!\r\n", ch );
      return;
   }
   buf[0] = '\0';

   one_argument( argument, arg );

   if( arg[0] == '\0' )
      wimpy = ch->max_hit / 5;
   else
      wimpy = atoi( arg );

   if( wimpy < 0 )
   {
      send_to_char( "Your courage exceeds your wisdom.\r\n", ch );
      return;
   }

   if( wimpy > ch->max_hit )
   {
      send_to_char( "Such cowardice ill becomes you.\r\n", ch );
      return;
   }

   ch->wimpy = wimpy;
   snprintf( buf, MSL, "Wimpy set to %d hit points.\r\n", wimpy );
   send_to_char( buf, ch );
   return;
}

DO_FUN(do_password)
{
   char arg1[MSL];
   char arg2[MSL];
   char *pArg;
   char *pwdnew;
   char *p;
   char cEnd;

   if( IS_NPC( ch ) )
      return;
   arg1[0] = '\0';
   arg2[0] = '\0';

   /*
    * Can't use one_argument here because it smashes case.
    * So we just steal all its code.  Bleagh.
    */
   pArg = arg1;
   while( isspace( *argument ) )
      argument++;

   cEnd = ' ';
   if( *argument == '\'' || *argument == '"' )
      cEnd = *argument++;

   while( *argument != '\0' )
   {
      if( *argument == cEnd )
      {
         argument++;
         break;
      }
      *pArg++ = *argument++;
   }
   *pArg = '\0';

   pArg = arg2;
   while( isspace( *argument ) )
      argument++;

   cEnd = ' ';
   if( *argument == '\'' || *argument == '"' )
      cEnd = *argument++;

   while( *argument != '\0' )
   {
      if( *argument == cEnd )
      {
         argument++;
         break;
      }
      *pArg++ = *argument++;
   }
   *pArg = '\0';

   if( ( ch->pcdata->pwd != '\0' ) && ( arg1[0] == '\0' || arg2[0] == '\0' ) )
   {
      send_to_char( "Syntax: password <old> <new>.\r\n", ch );
      return;
   }

   if( ( ch->pcdata->pwd != '\0' ) && ( strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) ) )
   {
      WAIT_STATE( ch, 1000 );
      send_to_char( "Wrong password.  Wait 10 seconds.\r\n", ch );
      return;
   }

   if( strlen( arg2 ) < 5 )
   {
      send_to_char( "New password must be at least five characters long.\r\n", ch );
      return;
   }

   /*
    * No tilde allowed because of player file format.
    */
   pwdnew = crypt( arg2, ch->name.c_str() );
   for( p = pwdnew; *p != '\0'; p++ )
   {
      if( *p == '~' )
      {
         send_to_char( "New password not acceptable, try again.\r\n", ch );
         return;
      }
   }

   free_string( ch->pcdata->pwd );
   ch->pcdata->pwd = str_dup( pwdnew );
   save_char_obj( ch );
   send_to_char( "Ok.\r\n", ch );
   return;
}

DO_FUN(do_socials)
{
   char buf[MSL];
   char out[MSL * 2];
   int iSocial;
   int col;
   buf[0] = '\0';

   col = 0;
   out[0] = '\0';

   for( iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++ )
   {
      snprintf( buf, MSL, "%-12s", social_table[iSocial].name );
      strncat( out, buf, MSL );
      if( ++col % 6 == 0 )
         strncat( out, "\r\n", MSL );
   }

   if( col % 6 != 0 )
      strncat( out, "\r\n", MSL );
   send_to_char( out, ch );
   return;
}

static char *const cmd_group_names[] = {
   "@@WMisc\r\n", "@@aCommunication\r\n", "@@mConfiguration\r\n", "@@eInformation\r\n", "@@rActions\r\n",
   "@@dItem Manipulation\r\n", "@@yImm\r\n"
};

DO_FUN(do_commands)
{
 short cnt = 0, i = 0, cmd = 0, show = 0, total = 0;
 string buf;
 char tmp[MSL];

 if( IS_NPC(ch) )
  return;

 total = IS_IMMORTAL(ch) ? C_TYPE_IMM+1 : C_TYPE_IMM;

 if( argument[0] != '\0' )
 {
  show = -1;
  if( !str_prefix(argument,"miscellaneous") )
   show = 0;
  else if( !str_prefix(argument,"communication") )
   show = 1;
  else if( !str_prefix(argument,"configuration") )
   show = 2;
  else if( !str_prefix(argument,"information") )
   show = 3;
  else if( !str_prefix(argument,"actions") )
   show = 4;
  else if( !str_prefix(argument,"objects") || !str_prefix(argument,"manipulation") || !str_prefix(argument,"items") )
   show = 5;
  else if( !str_prefix(argument,"imm") )
   show = 6;
 }
 else
  show = -1;

 if( show > -1 )
 {
  i = show;
  total = i + 1;
 }

 while( i < total )
 {
  if( i > 0 )
   buf += "\r\n";
  buf += cmd_group_names[i];

  for( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
  {
   if( cmd_table[cmd].level <= L_GOD && cmd_table[cmd].level <= get_trust(ch) )
   {
    if( show > -1 && cmd_table[cmd].type != show )
     continue;
    if( show == -1 && cmd_table[cmd].type != i )
     continue;
    if( cmd_table[cmd].level == CLAN_ONLY && ch->clan == 0 )
     continue;
    if( cmd_table[cmd].level == BOSS_ONLY && !ch->act.test(ACT_CLEADER) )
     continue;
    if( cmd_table[cmd].level == VAMP_ONLY && !IS_VAMP(ch) )
     continue;
    if( cmd_table[cmd].level == WOLF_ONLY && !IS_WOLF(ch) )
     continue;
    if( cmd_table[cmd].show == C_SHOW_NEVER )
     continue;
    if( cmd_table[cmd].show == C_SHOW_SKILL && ch->pcdata->learned[skill_lookup(cmd_table[cmd].name)] < 10 )
     continue;

    snprintf(tmp,MSL,"%-12s",cmd_table[cmd].name);
    buf += tmp;

    if( ++cnt % 6 == 0 )
     buf += "\r\n";
    else if( cnt % 6 != 0 && cmd_table[cmd].name[0] == '\0' )
      buf += "\r\n";
   }
  }
  i++;
  cnt = 0;
 }

 buf += "\r\n";
 send_to_char(buf,ch);
 return;
}



struct chan_type
{
   short bit;
   int min_level;
   char *name;
   char *on_string;
   char *off_string;
};

struct chan_type channels[] = {
   {CHANNEL_AUCTION, 0, "auction",
    "[ +AUCTION  ] You hear biddings.\r\n",
    "[ -auction  ] You don't hear biddings.\r\n"},
   {CHANNEL_GOSSIP, 0, "gossip",
    "[ +GOSSIP   ] You hear general gossip.\r\n",
    "[ -gossip   ] You don't hear general gossip.\r\n"},
   {CHANNEL_MUSIC, 0, "music",
    "[ +MUSIC    ] You hear people's music quotes.\r\n",
    "[ -music    ] You don't hear people's music quotes.\r\n"},
   {CHANNEL_IMMTALK, LEVEL_HERO, "immtalk",
    "[ +IMMTALK  ] You hear what other immortals have to say.\r\n",
    "[ -immtalk  ] You don't hear what other immortals have to say.\r\n"},
   {CHANNEL_NEWBIE, 0, "newbie",
    "[ +NEWBIE   ] You hear newbie's chit-chat.\r\n",
    "[ -newbie   ] You don't hear newbie's chit-chat.\r\n"},
   {CHANNEL_QUESTION, 0, "question",
    "[ +QUESTION ] You hear player's questions & answers.\r\n",
    "[ -question ] You don't hear player's questions & answers.\r\n"},
   {CHANNEL_SHOUT, 0, "shout",
    "[ +SHOUT    ] You hear people shouting.\r\n",
    "[ -shout    ] You don't hear people shouting.\r\n"},
   {CHANNEL_YELL, 0, "yell",
    "[ +YELL     ] You hear people yelling.\r\n",
    "[ -yell     ] You don't hear people yelling.\r\n"},
   {CHANNEL_FLAME, 0, "flame",
    "[ +FLAME    ] You hear players flaming each other.\r\n",
    "[ -flame    ] You don't hear players flaming each other.\r\n"},
   {CHANNEL_ZZZ, 0, "zzz",
    "[ +ZZZ      ] You hear sleeping players chatting.\r\n",
    "[ -zzz      ] You don't hear sleeping players chatting.\r\n"},
   {CHANNEL_RACE, 0, "race",
    "[ +RACE     ] You hear your race's chit-chat.\r\n",
    "[ -race     ] You don't hear your race's chit-chat.\r\n"},
   {CHANNEL_CLAN, 0, "clan",
    "[ +CLAN     ] You hear clan chit-chat.\r\n",
    "[ -clan     ] You don't hear clan chit-chat.\r\n"},
   {CHANNEL_NOTIFY, LEVEL_HERO, "notify",
    "[ +NOTIFY   ] You hear player information.\r\n",
    "[ -notify   ] You don't hear player information.\r\n"},
   {CHANNEL_INFO, 0, "info",
    "[ +INFO     ] You hear information about deaths, etc.\r\n",
    "[ -info     ] You don't hear information about deaths, etc.\r\n"},
   {CHANNEL_LOG, 85, "log",
    "[ +LOG      ] You receive LOG_FILE details.\r\n",
    "[ -log      ] You don't receive LOG_FILE details.\r\n"},
   {CHANNEL_CREATOR, 85, "creator",
    "[ +CREATOR  ] You hear Creator's discussing Mud matters.\r\n",
    "[ -creator  ] You don't hear Creators discussing Mud matters\r\n"},
   {CHANNEL_ALLCLAN, 85, "allclan",
    "[ +ALLCLAN  ] You hear ALL clan channels.\r\n",
    "[ -allclan  ] You don't hear ALL clan channels.\r\n"},
   {CHANNEL_ALLRACE, 85, "allrace",
    "[ +ALLRACE  ] You hear ALL race channels.\r\n",
    "[ -allrace  ] You don't hear ALL race channels.\r\n"},
   {CHANNEL_HERMIT, 0, "hermit",
    "[ +HERMIT   ] You are NOT ignoring all channels.\r\n",
    "[ -hermit   ] YOU ARE IGNORING ALL CHANNELS!\r\n"},
   {CHANNEL_BEEP, 0, "beep",
    "[ +BEEP     ] You accept 'beeps' from other players.\r\n",
    "[ -beep     ] You are ignoring 'beeps' from other players.\r\n"},
   {CHANNEL_FAMILY, 0, "vampire",
    "",
    ""},
   {CHANNEL_DIPLOMAT, 0, "diplomat",
    "[ +DIPLOMAT ] You hear diplomatic negotioations.\r\n",
    "[ -diplomat ] YOU do not hear diplomatic negotiations\r\n"},
   {CHANNEL_CRUSADE, 0, "crusade",
    "[ +CRUSADE  ] You hear quest information.\r\n",
    "[ -crusade  ] You are ignoring quest information.\r\n"},
   {CHANNEL_REMORTTALK, 0, "{",
    "[ +{        ] You hear remorts gossiping amongst themselves.\r\n",
    "[ -{        ] You are ignoring idle remort chatter.\r\n"},
   {CHANNEL_HOWL, 0, "howl",
    "",
    ""},
   {CHANNEL_ADEPT, 0, "adept",
    "[ +ADEPT    ] You hear Adepts plotting to take over the mud!\r\n",
    "[ -adept    ] You ignore those bored dudes and dudettes.\r\n"},
   {CHANNEL_OOC, 0, "ooc",
    "[ +OOC      ] You hear non-roleplaying chit-chat.\r\n",
    "[ -ooc      ] You ignore those boring non-roleplaying fools!\r\n"},
   {CHANNEL_QUEST, 0, "quest",
    "[ +QUEST    ] You hear roleplaying quests!\r\n",
    "[ -quest    ] You ignore roleplaying quests.\r\n"},
   {CHANNEL_GAME, 0, "game",
    "[ +GAME     ] You hear players participating in games.\r\n",
    "[ -game     ] You no longer hear people playing games.\r\n"},

   {0, 0, NULL, NULL}
};

DO_FUN(do_channels)
{
   char arg[MSL];
   char buffer[MSL];
   int a, trust;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      if( !IS_NPC( ch ) && ch->act.test(ACT_SILENCE) )
      {
         send_to_char( "You are silenced.\r\n", ch );
         return;
      }

      trust = get_trust( ch );
      buffer[0] = '\0';
      strncat( buffer, "Channels:\r\n", MSL-1 );

      for( a = 0; channels[a].bit != 0; a++ )
      {
         if( trust >= channels[a].min_level )
         {
            char colbuf[10];
            colbuf[0] = '\0';

            if( ch->deaf.test(channels[a].bit) )
            {
               if( !IS_NPC( ch ) )
               {
                  snprintf( colbuf, 10, "@@%c", ch->pcdata->dimcol );
                  strncat( buffer, colbuf, MSL-1 );
               }
               strncat( buffer, channels[a].off_string, MSL-1 );
            }
            else
            {
               if( !IS_NPC( ch ) )
               {
                  snprintf( colbuf, 10, "@@%c", ch->pcdata->hicol );
                  strncat( buffer, colbuf, MSL-1 );
               }
               strncat( buffer, channels[a].on_string, MSL-1 );
            }
         }

         strncat( buffer, "@@N", MSL-1 );
      }
      send_to_char( buffer, ch );
   }
   else
   {
      bool fClear;
      int bit;

      if( arg[0] == '+' )
         fClear = TRUE;
      else if( arg[0] == '-' )
         fClear = FALSE;
      else
      {
         send_to_char( "Channels -channel or +channel?\r\n", ch );
         return;
      }

      /*
       * Now check through table to set/unset channel... 
       */
      bit = 0;
      for( a = 0; channels[a].bit != 0; a++ )
      {
         if( channels[a].min_level > get_trust( ch ) )
            continue;
         if( !str_prefix( arg + 1, channels[a].name ) )
         {
            bit = channels[a].bit;
            break;
         }
      }
      if( bit == 0 )
      {
         send_to_char( "Set or clear which channel?\r\n", ch );
         return;
      }

      if( fClear )
         ch->deaf.reset(bit);
      else
         ch->deaf.set(bit);

      send_to_char( "Ok.\r\n", ch );
   }

   return;
}

/*
 * Contributed by Grodyn.
 */
DO_FUN(do_config)
{
   char arg[MSL];
   char buf[MSL];
   if( IS_NPC( ch ) )
      return;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "@@d[@@W Keyword   @@d]@@W Option@@N\r\n", ch );

      if( IS_IMMORTAL(ch) )
      {
       send_to_char( ch->act.test(ACT_AUTODIG)
                     ? "@@d[@@a+AUTODIG   @@d]@@a You can dig new zones by walking.@@N\r\n"
                     : "@@d[@@c-autodig   @@d]@@c You must manually dig new zones.@@N\r\n", ch );
      }

      send_to_char( ch->act.test(ACT_NO_SUMMON)
                    ? "@@d[@@a+NO_SUMMON @@d]@@a You may not be summoned.@@N\r\n"
                    : "@@d[@@c-no_summon @@d]@@c You may be summoned.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_NO_VISIT)
                    ? "@@d[@@a+NO_VISIT  @@d]@@a You may not be 'visited'.@@N\r\n"
                    : "@@d[@@c-no_visit  @@d]@@c You may be 'visited'.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_COLOR)
                    ? "@@d[@@a+COLOR     @@d]@@a You recieve 'ANSI' color.@@N\r\n"
                    : "@@d[@@c-color     @@d]@@c You don't receive 'ANSI' color.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_AUTOBRIEF)
                    ? "@@d[@@a+AUTOBRIEF @@d]@@a You automatically hide room descriptions when traveling.@@N\r\n"
                    : "@@d[@@c-autobrief @@d]@@c You don't automatically hide room descriptions when traveling.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_AUTOEXIT)
                    ? "@@d[@@a+AUTOEXIT  @@d]@@a You automatically see exits.@@N\r\n"
                    : "@@d[@@c-autoexit  @@d]@@c You don't automatically see exits.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_AUTOLOOT)
                    ? "@@d[@@a+AUTOLOOT  @@d]@@a You automatically loot corpses.@@N\r\n"
                    : "@@d[@@c-autoloot  @@d]@@c You don't automatically loot corpses.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_AUTOSAC)
                    ? "@@d[@@a+AUTOSAC   @@d]@@a You automatically sacrifice corpses.@@N\r\n"
                    : "@@d[@@c-autosac   @@d]@@c You don't automatically sacrifice corpses.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_BLANK)
                    ? "@@d[@@a+BLANK     @@d]@@a You have a blank line before your prompt.@@N\r\n"
                    : "@@d[@@c-blank     @@d]@@c You have no blank line before your prompt.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_BRIEF)
                    ? "@@d[@@a+BRIEF     @@d]@@a You see brief descriptions.\r\n"
                    : "@@d[@@c-brief     @@d]@@c You see long descriptions.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_COMBINE)
                    ? "@@d[@@a+COMBINE   @@d]@@a You see object lists in combined format.@@N\r\n"
                    : "@@d[@@c-combine   @@d]@@c You see object lists in single format.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_PROMPT)
                    ? "@@d[@@a+PROMPT    @@d]@@a You have a prompt.\r\n"
                    : "@@d[@@c-prompt    @@d]@@c You don't have a prompt.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_TELNET_GA)
                    ? "@@d[@@a+TELNET_GA @@d]@@a You receive a telnet GA sequence.@@N\r\n"
                    : "@@d[@@c-telnet_ga @@d]@@c You don't receive a telnet GA sequence.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_FULL_ANSI)
                    ? "@@d[@@a+FULL_ANSI @@d]@@a Your client supports FULL ANSI.@@N\r\n"
                    : "@@d[@@c-full_ansi @@d]@@c Your client does not support full ANSI (GMUD).@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_MAPPER)
                    ? "@@d[@@a+MAPPER    @@d]@@a You are viewing the ASCII display map!@@N\r\n"
                    : "@@d[@@c-MAPPER    @@d]@@c Your are not viewing the ASCII display map.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_JUSTIFY)
                    ? "@@d[@@a+JUSTIFY   @@d]@@a You are viewing rooms in space justified format.@@N\r\n"
                    : "@@d[@@c-justify   @@d]@@c Your are not viewing rooms space justified.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_NO_COOLDOWN)
                    ? "@@d[@@a+COOLDOWN  @@d]@@a You are not seeing cooldown messages.@@N\r\n"
                    : "@@d[@@c-cooldown  @@d]@@c You are seeing cooldown messages.@@N\r\n", ch );

      send_to_char( ch->act.test(ACT_WHITELIST)
                    ? "@@d[@@a+WHITELIST @@d]@@a You may only login from your whitelist domains.@@N\r\n"
                    : "@@d[@@c-whitelist @@d]@@c You may login from any domain.@@N\r\n", ch );

      send_to_char( !ch->act.test(ACT_NO_PRAY) ? "" : "@@d[@@a+NOPRAY   @@d]@@a You cannot use 'pray'.@@N\r\n", ch );

      send_to_char( !ch->act.test(ACT_SILENCE) ? "" : "@@d[@@a+SILENCE  @@d]@@a You are silenced.@@N\r\n", ch );

      send_to_char( !ch->act.test(ACT_NO_EMOTE) ? "" : "@@d[@@c-emote    @@d]@@c You can't emote.@@N\r\n", ch );

      send_to_char( !ch->act.test(ACT_NO_TELL) ? "" : "@@d[@@c-tell     @@d]@@c You can't use 'tell'.@@N\r\n", ch );
      snprintf( buf, MSL, "Terminal set to:  %i Rows, %i Columns.\r\n", ch->pcdata->term_rows, ch->pcdata->term_columns );
      send_to_char( buf, ch );
   }
   else
   {
      bool fSet;
      short bit = 0;
      if( arg[0] == '+' )
         fSet = TRUE;
      else if( arg[0] == '-' )
         fSet = FALSE;
      else
      {
         send_to_char( "Config -option or +option?\r\n", ch );
         return;
      }

      bit = table_lookup( tab_player_act, (arg + 1) );

      if( !str_prefix( "rows", arg + 1 ) )
      {
         char arg1[MSL];
         argument = one_argument( argument, arg1 );
         argument = one_argument( argument, arg1 );
         if( !is_number( arg1 ) )
         {
            send_to_char( "That is not a number.\r\n", ch );
            return;
         }
         ch->pcdata->term_rows = URANGE( 10, atoi( arg1 ), 80 );
         return;
      }
      if( bit == 0 )
      {
         send_to_char( "Config which option?\r\n", ch );
         return;
      }

      if( fSet )
      {
         ch->act.set(bit);

         if( bit == ACT_FULL_ANSI )
         {
            char scrollbuf[MSL];
            snprintf( scrollbuf, MSL, "%s%s%s%i;%ir%s%i;%iH",
                     CRS_RESET, CRS_CLS, CRS_CMD, 0, ch->pcdata->term_rows - 12, CRS_CMD, ch->pcdata->term_rows - 13, 0 );
            send_to_char( scrollbuf, ch );
         }
      }
      else
      {
         ch->act.reset(bit);

         if( bit == ACT_FULL_ANSI )
         {


            send_to_char( CRS_RESET, ch );
            send_to_char( "Done.\r\n", ch );

         }
      }

      send_to_char( "Ok.\r\n", ch );
   }

   return;
}

DO_FUN(do_wizlist)
{

   do_help( ch, "wiz_list" );
   return;

}

DO_FUN(do_race_list)
{
   int iRace;
   char buf[MSL];

   for( iRace = 0; iRace < MAX_RACE; iRace++ ) /* Lets display race info if people rlist <abbr> --Kline */
   {
    if( !str_cmp(argument,race_table[iRace].race_name) && (IS_IMMORTAL(ch) || race_table[iRace].player_allowed == TRUE) )
    {
     int iWear, cnt = 0;

     send_to_char("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n",ch);
     snprintf(buf,MSL,"[%3s] %9s (%s)\r\n",race_table[iRace].race_name,race_table[iRace].race_title,race_table[iRace].comment);
     send_to_char(buf,ch);
     send_to_char("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n",ch);
     snprintf(buf,MSL,"Max Stats     : Str [%d]  Int [%d]  Wis [%d]  Dex [%d]  Con [%d]\r\nRace Mods     : ",
      race_table[iRace].race_str,race_table[iRace].race_int,race_table[iRace].race_wis,race_table[iRace].race_dex,race_table[iRace].race_con);
     send_to_char(buf,ch);
     send_to_char(bit_table_lookup(tab_mob_race_mods_col,race_table[iRace].race_flags),ch);
     send_to_char("\r\nStrong Realms : ",ch);
     send_to_char(bit_table_lookup(tab_magic_realms_col,race_table[iRace].strong_realms),ch);
     send_to_char("\r\nWeak Realms   : ",ch);
     send_to_char(bit_table_lookup(tab_magic_realms_col,race_table[iRace].weak_realms),ch);
     send_to_char("\r\nResist Realms : ",ch);
     send_to_char(bit_table_lookup(tab_magic_realms_col,race_table[iRace].resist_realms),ch);
     send_to_char("\r\nSuscept Realms: ",ch);
     send_to_char(bit_table_lookup(tab_magic_realms_col,race_table[iRace].suscept_realms),ch);
     send_to_char("\r\nRacial Skills : ",ch);
     send_to_char(race_table[iRace].skill1,ch);
     for( iWear = 0; iWear < MAX_WEAR; iWear++ )
      if( race_table[iRace].wear_locs[iWear] == TRUE )
       cnt++;
     snprintf(buf,MSL,"\r\nWear Locations: %d\r\n",cnt);
     send_to_char(buf,ch);
     send_to_char(output_race_wear(iRace),ch);

     send_to_char("\r\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n",ch);
     return;
    }
   }

   send_to_char( "    Here follows a list of current races for " mudnamecolor ":\r\n", ch );
   send_to_char( "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n", ch );
   send_to_char( "\r\n", ch );
   if( IS_IMMORTAL( ch ) )
      send_to_char( "    No.   Room.    Abbr.    Name.    M/C   Classes: (Good->Bad)\r\n", ch );
   else
      send_to_char( "   Abbr.    Name.   M/C  Classes: (Good->Bad)\r\n", ch );

   for( iRace = 0; iRace < MAX_RACE; iRace++ )
   {
      if( IS_IMMORTAL( ch ) )
      {
         snprintf( buf, MSL, "   %3d   %5d    %5s     %9s %2d %s %5s %4s\r\n",
                  iRace, race_table[iRace].recall,
                  race_table[iRace].race_name, race_table[iRace].race_title,
                  race_table[iRace].classes, race_table[iRace].comment,
                  ( race_table[iRace].player_allowed == TRUE ? "@@aOpen@@N" : "@@eNPC @@N" ),
                  ( race_table[iRace].has_money == TRUE ? "@@aMoney@@N" : "@@eNONE@@N" ) );
         send_to_char( buf, ch );
      }

      else if( race_table[iRace].player_allowed == TRUE )
      {
         snprintf( buf, MSL, "   %5s    %9s    %2d %s\r\n",
                  race_table[iRace].race_name, race_table[iRace].race_title,
                  race_table[iRace].classes, race_table[iRace].comment );
         send_to_char( buf, ch );
      }


   }
   send_to_char( "\r\n", ch );
   send_to_char( "M/C = Number of classes available.\r\n", ch );
   send_to_char( "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n", ch );
   return;
}

DO_FUN(do_clan_list)
{
   int iClan;
   char buf[MSL];

   send_to_char( "\r\n    Here follows a list of current clans for " mudnamecolor ":\r\n", ch );
   send_to_char( "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n", ch );
   send_to_char( "\r\n", ch );
   if( IS_IMMORTAL( ch ) )
      send_to_char( "    No.   Room.   Abbr.     Leader    Name.\r\n", ch );
   else
      send_to_char( "   Abbr.          Leader     Name.\r\n", ch );

   for( iClan = 0; iClan < MAX_CLAN; iClan++ )
   {
      if( !IS_IMMORTAL( ch ) && iClan == 0 )
         continue;   /* Don't list 'none' as a clan :) */
      if( IS_IMMORTAL( ch ) )
         snprintf( buf, MSL, "   %3d   %5d    %s  %12s  %s\r\n",
                  iClan, clan_table[iClan].clan_room, clan_table[iClan].clan_abbr,
                  clan_table[iClan].leader, clan_table[iClan].clan_name );
      else
         snprintf( buf, MSL, "   %s    %12s  %s\r\n",
                  clan_table[iClan].clan_abbr, clan_table[iClan].leader, clan_table[iClan].clan_name );

      send_to_char( buf, ch );
   }
   send_to_char( "\r\n", ch );
   send_to_char( "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n", ch );
   return;
}

DO_FUN(do_spells)
{
   char buf[MSL];
   char buf1[MSL];
   int sn;
   int col;
   int cnt;
   bool ok;
   buf[0] = '\0';

   /*
    * if ( ( !IS_NPC( ch ) &&
    * !class_table[ch->p_class].fMana )
    * ||  IS_NPC ( ch ) )
    */
   if( IS_NPC( ch ) )
   {
      send_to_char( "You do not know how to cast spells!\r\n", ch );
      return;
   }

   buf1[0] = '\0';

   col = 0;
   for( sn = 0; sn < MAX_SKILL; sn++ )
   {
      ok = FALSE;

      if( skill_table[sn].name == NULL )
         break;
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         if( ( ch->lvl[cnt] >= skill_table[sn].skill_level[cnt] ) && ( skill_table[sn].skill_level[cnt] < LEVEL_HERO ) )
            ok = TRUE;

      if( ch->pcdata->learned[sn] == 0 )
         continue;

      if( skill_table[sn].slot == 0 )
         continue;

      /*
       * if ( skill_table[sn].skill_level[ch->p_class] > LEVEL_HERO )
       * continue;
       */

      snprintf( buf, MSL, "%20s (%0.2fs) %4dmp ", skill_table[sn].name, cast_speed( ch, sn ), mana_cost( ch, sn ) );
      strncat( buf1, buf, MSL-1 );
      if( ++col % 3 == 0 )
         strncat( buf1, "\r\n", MSL );
   }

   if( col % 3 != 0 )
      strncat( buf1, "\r\n", MSL );

   send_to_char( buf1, ch );
   return;

}

DO_FUN(do_slist)
{
   char buf[MSL];
   char buf1[MSL];
   int p_class;
   int foo;
   int sn;
   int col;
   int level;
   bool pSpell;
   bool any;
   bool remort_class;
   bool adept_class;
   buf[0] = '\0';
   buf1[0] = '\0';


   if( IS_NPC( ch ) )
   {
      send_to_char( "You do not need any stinking spells!\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "USAGE:  <abbrev>.  abbrev = 3-letter class abreviation.\r\n", ch );
      return;
   }

   any = FALSE;
   p_class = -1;
   remort_class = FALSE;
   adept_class = FALSE;
   for( foo = 0; foo < MAX_CLASS; foo++ )
      if( !str_cmp( class_table[foo].who_name, argument ) )
      {
         any = TRUE;
         p_class = foo;

      }
      else if( !str_cmp( remort_table[foo].who_name, argument ) )
      {
         any = TRUE;
         p_class = foo;
         remort_class = TRUE;
      }
      else if( !str_prefix( "ADEPT", argument ) )
      {
         any = TRUE;
         adept_class = TRUE;
         p_class = 0;
      }

   if( !any )
   {
      send_to_char( "That abbreviation not recognized!\r\n", ch );
      return;
   }

   buf1[0] = '\0';

   strncat( buf1, "ALL Spells available for the class ", MSL );
   if( adept_class )
   {
      strncat( buf1, "@@W", MSL );
      strncat( buf1, "Adept", MSL );
      strncat( buf1, "@@N\r\n\r\n", MSL );
   }
   else if( remort_class )
   {
      strncat( buf1, "@@m", MSL );
      strncat( buf1, remort_table[p_class].class_name, MSL-1 );
      strncat( buf1, "@@N\r\n\r\n", MSL );
   }
   else

   {
      strncat( buf1, "@@b", MSL );
      strncat( buf1, class_table[p_class].class_name, MSL-1 );
      strncat( buf1, "@@N\r\n\r\n", MSL );
   }

   strncat( buf1, "@@rLv          @@lSpells@@N\r\n\r\n", MSL );

   for( level = 1; level < LEVEL_IMMORTAL; level++ )
   {

      col = 0;
      pSpell = TRUE;

      for( sn = 0; sn < MAX_SKILL; sn++ )
      {
         if( skill_table[sn].name == NULL )
            break;
         if( ( skill_table[sn].skill_level[p_class] != level )
             || ( skill_table[sn].flag2 == VAMP ) || ( skill_table[sn].flag2 == WOLF ) )
            continue;
         if( ( adept_class ) && ( skill_table[sn].flag1 == ADEPT ) )
         {

            if( pSpell )
            {

               snprintf( buf, MSL, "@@r%2d:@@N", level );
               strncat( buf1, buf, MSL-1 );
               pSpell = FALSE;
            }

            if( ++col % 5 == 0 )
               strncat( buf1, "   ", MSL );
            if( ch->pcdata->learned[sn] > 0 )
            {
               snprintf( buf, MSL, "@@m%18s@@N", skill_table[sn].name );
               strncat( buf1, buf, MSL-1 );
            }
            else if( skill_table[sn].skill_level[p_class] >
                     ( adept_class ? ch->get_level("adept") : remort_class ? ch->lvl2[p_class] : ch->lvl[p_class] ) )
            {
               snprintf( buf, MSL, "@@d%18s@@N", skill_table[sn].name );
               strncat( buf1, buf, MSL-1 );
            }
            else
            {
               snprintf( buf, MSL, "@@a%18s@@N", skill_table[sn].name );
               strncat( buf1, buf, MSL-1 );
            }
            if( col % 4 == 0 )
               strncat( buf1, "\r\n", MSL );
         }
         else if( ( remort_class ) && ( !adept_class ) && ( skill_table[sn].flag1 == REMORT ) )
         {

            if( pSpell )
            {

               snprintf( buf, MSL, "@@r%2d:@@N", level );
               strncat( buf1, buf, MSL-1 );
               pSpell = FALSE;
            }

            if( ++col % 5 == 0 )
               strncat( buf1, "   ", MSL );
            if( ch->pcdata->learned[sn] > 0 )
            {
               snprintf( buf, MSL, "@@x@@m%18s@@N", skill_table[sn].name );
               strncat( buf1, buf, MSL-1 );
            }
            else if( skill_table[sn].skill_level[p_class] >
                     ( adept_class ? ch->get_level("adept") : remort_class ? ch->lvl2[p_class] : ch->lvl[p_class] ) )
            {
               snprintf( buf, MSL, "@@d%18s@@N", skill_table[sn].name );
               strncat( buf1, buf, MSL-1 );
            }
            else
            {
               snprintf( buf, MSL, "@@a%18s@@N", skill_table[sn].name );
               strncat( buf1, buf, MSL-1 );
            }
            if( col % 4 == 0 )
               strncat( buf1, "\r\n", MSL );
         }
         else if( ( !remort_class ) && ( !adept_class ) && ( skill_table[sn].flag1 == MORTAL ) )
         {
            if( pSpell )
            {
               snprintf( buf, MSL, "@@r%2d:@@N", level );
               strncat( buf1, buf, MSL-1 );
               pSpell = FALSE;
            }

            if( ++col % 5 == 0 )
               strncat( buf1, "   ", MSL );


            if( ch->pcdata->learned[sn] > 0 )
            {
               snprintf( buf, MSL, "@@x@@m%18s@@N", skill_table[sn].name );
               strncat( buf1, buf, MSL-1 );
            }
            else if( skill_table[sn].skill_level[p_class] >
                     ( adept_class ? ch->get_level("adept") : remort_class ? ch->lvl2[p_class] : ch->lvl[p_class] ) )
            {
               snprintf( buf, MSL, "@@d%18s@@N", skill_table[sn].name );
               strncat( buf1, buf, MSL-1 );
            }
            else
            {
               snprintf( buf, MSL, "@@a%18s@@N", skill_table[sn].name );
               strncat( buf1, buf, MSL-1 );
            }
            if( col % 4 == 0 )
               strncat( buf1, "\r\n", MSL );

         }
      }

      if( col % 4 != 0 )
         strncat( buf1, "\r\n", MSL );

   }

   send_to_char( buf1, ch );
   return;

}

/* by passing the conf command - Kahn */

DO_FUN(do_autoexit)
{

   ( ch->act.test(ACT_AUTOEXIT) ? do_config( ch, "-autoexit" ) : do_config( ch, "+autoexit" ) );

}

DO_FUN(do_autoloot)
{

   ( ch->act.test(ACT_AUTOLOOT) ? do_config( ch, "-autoloot" ) : do_config( ch, "+autoloot" ) );

}

DO_FUN(do_autosac)
{

   ( ch->act.test(ACT_AUTOSAC)  ? do_config( ch, "-autosac" ) : do_config( ch, "+autosac" ) );

}

DO_FUN(do_blank)
{

   ( ch->act.test(ACT_BLANK) ? do_config( ch, "-blank" ) : do_config( ch, "+blank" ) );

}

DO_FUN(do_brief)
{

   ( ch->act.test(ACT_BRIEF) ? do_config( ch, "-brief" ) : do_config( ch, "+brief" ) );

}

DO_FUN(do_combine)
{

   ( ch->act.test(ACT_COMBINE) ? do_config( ch, "-combine" ) : do_config( ch, "+combine" ) );

}

DO_FUN(do_autodig)
{

   ( ch->act.test(ACT_AUTODIG) ? do_config( ch, "-autodig" ) : do_config( ch, "+autodig" ) );

}

DO_FUN(do_autobrief)
{

   ( ch->act.test(ACT_AUTOBRIEF) ? do_config( ch, "-autobrief" ) : do_config( ch, "+autobrief" ) );

}

DO_FUN(do_pagelen)
{
   char buf[MSL];
   char arg[MSL];
   int lines;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
      lines = 20;
   else
      lines = atoi( arg );

   if( lines < 1 )
   {
      send_to_char( "Negative or Zero values for a page pause is not legal.\r\n", ch );
      return;
   }

   ch->pcdata->pagelen = lines;
   snprintf( buf, MSL, "Page pause set to %d lines.\r\n", lines );
   send_to_char( buf, ch );
   return;
}

DO_FUN(do_prompt)
{
 char *farg = argument;

 if( farg[0] == '\0' )
 {
  ch->act.test(ACT_PROMPT) ? do_config(ch,"-prompt") : do_config(ch,"+prompt");
  return;
 }
 if( !str_cmp(farg,"all") )
 {
  ch->prompt = DEFAULT_PROMPT;
  send_to_char("Ok.\r\n",ch);
  return;
 }
 if( strlen(farg) > 200 )
  farg[200] = '\0';

 smash_tilde(farg);
 ch->prompt = farg;
 send_to_char("Ok.\r\n",ch);

 return;
}

DO_FUN(do_diagnose)
{
   char buf[MSL];
   char arg[MSL];
   CHAR_DATA *victim;
   int pct;


   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Diagnose whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They're not here.\r\n", ch );
      return;
   }


   if( can_see( victim, ch ) )
   {
      act( "$n gives you the once-over.", ch, NULL, victim, TO_VICT );
      act( "$n gives $N the once-over.", ch, NULL, victim, TO_NOTVICT );
   }
   else
   {
      send_to_char( "They're not here.\r\n", ch );
      return;
   }

   if( victim->max_hit > 0 )
      pct = ( 100 * victim->hit ) / victim->max_hit;
   else
      pct = -1;

   strcpy( buf, victim->get_name(ch) );

   if( pct >= 100 )
      strncat( buf, " --  [5] 4  3  2  1   \r\n", MSL );
   else if( pct >= 80 )
      strncat( buf, " --   5 [4] 3  2  1   \r\n", MSL );
   else if( pct >= 60 )
      strncat( buf, " --   5  4 [3] 2  1   \r\n", MSL );
   else if( pct >= 40 )
      strncat( buf, " --   5  4  3 [2] 1   \r\n", MSL );
   else
      strncat( buf, " --   5  4  3  2 [1]  \r\n", MSL );

   buf[0] = UPPER( buf[0] );
   send_to_char( buf, ch );


   return;
}

DO_FUN(do_heal)
{
   /*
    * This function used when a player types heal when in a room with
    * * a mob with ACT_HEAL set.  Cost is based on the ch's level.
    * * -- Stephen
    */

   CHAR_DATA *mob;
   char buf[MSL];
   int mult;   /* Multiplier used to calculate costs. */
   char costbuf[MSL];
   char changebuf[MSL];
   char *give;
   char givebuf[MSL];
   buf[0] = '\0';

   /*
    * Check for mob with act->heal 
    */
   for( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
   {
      if( IS_NPC( mob ) &&  mob->act.test(ACT_HEAL) )
         break;
   }

   if( mob == NULL )
   {
      send_to_char( "You can't do that here.\r\n", ch );
      return;
   }

   mult = UMAX( 10, ch->get_level("psuedo") / 2 );

   if( argument[0] == '\0' )
   {
      /*
       * Work out costs of different spells. 
       */
      send_to_char( "Costs for spells ( approximate ):\r\n", ch );
      snprintf( costbuf, MSL, "%s", cost_to_money( ( mult * 100 ) ) );
      snprintf( buf, MSL, "Sanctuary:          %-*s.\r\n", ccode_len( costbuf, 40 ), costbuf );
      send_to_char( buf, ch );
      snprintf( costbuf, MSL, "%s", cost_to_money( ( mult * 50 ) ) );
      snprintf( buf, MSL, "Heal:               %-*s.\r\n", ccode_len( costbuf, 40 ), costbuf );
      send_to_char( buf, ch );
      snprintf( costbuf, MSL, "%s", cost_to_money( ( mult * 20 ) ) );
      snprintf( buf, MSL, "Invisibilty:        %-*s.\r\n", ccode_len( costbuf, 40 ), costbuf );
      send_to_char( buf, ch );
      snprintf( costbuf, MSL, "%s", cost_to_money( ( mult * 10 ) ) );
      snprintf( buf, MSL, "Detect Invisibilty: %-*s.\r\n", ccode_len( costbuf, 40 ), costbuf );
      send_to_char( buf, ch );
      snprintf( costbuf, MSL, "%s", cost_to_money( ( mult * 10 ) ) );
      snprintf( buf, MSL, "Refresh:            %-*s.\r\n", ccode_len( costbuf, 40 ), costbuf );
      send_to_char( buf, ch );
      snprintf( costbuf, MSL, "%s", cost_to_money( ( mult * 20 ) ) );
      snprintf( buf, MSL, "Night Vision:       %-*s.\r\n", ccode_len( costbuf, 40 ), costbuf );
      send_to_char( buf, ch );
      snprintf( costbuf, MSL, "%s", cost_to_money( ( mult * 200 ) ) );
      snprintf( buf, MSL, "Magical Dispel:     %-*s.\r\n", ccode_len( costbuf, 40 ), costbuf );
      send_to_char( buf, ch );
      snprintf( costbuf, MSL, "%s", cost_to_money( ( 2000 ) ) );
      snprintf( buf, MSL, "Mana:     %-*s.\r\n", ccode_len( costbuf, 40 ), costbuf );
      send_to_char( buf, ch );


      send_to_char( "Type HEAL [S|H|I|D|R|N|M|P]\r\n", ch );
      send_to_char( "Eg: 'HEAL H' will result in the heal spell being cast.\r\n", ch );
      send_to_char( "\r\n**ALL** Spells will be cast on the buyer ONLY.\r\n", ch );
      return;
   }

   switch ( UPPER( argument[0] ) )
   {
      case 'S':  /* Sanc */
         if( money_value( ch->money ) < ( mult * 100 ) )
         {
            send_to_char( "You don't have enough money...\r\n", ch );
            return;
         }
         act( "$N gestures towards $n.", ch, NULL, mob, TO_NOTVICT );
         act( "$N gestures towards you.", ch, NULL, mob, TO_CHAR );
         spell_sanctuary( skill_lookup( "sanc" ), mult, ch, ch, NULL );
         give = take_best_coins( ch->money, ( mult * 100 ) );
         give = one_argument( give, changebuf );
         snprintf( givebuf, MSL, "%s to %s", give, mob->name.c_str() );
         do_give( ch, givebuf );
         join_money( round_money( atoi( changebuf ), TRUE ), ch->money );
         send_to_char( "The healer hands you some change.\r\n", ch );
         break;
      case 'P':  /* mana */
         if( money_value( ch->money ) < 1000 )
         {
            send_to_char( "You don't have enough money...\r\n", ch );
            return;
         }
         act( "$N gestures towards $n.", ch, NULL, mob, TO_NOTVICT );
         act( "$N gestures towards you.", ch, NULL, mob, TO_CHAR );
         give = take_best_coins( ch->money, 1000 );
         ch->mana = UMIN( ch->max_mana, ch->mana + 50 );
         give = one_argument( give, changebuf );
         snprintf( givebuf, MSL, "%s to %s", give, mob->name.c_str() );
         do_give( ch, givebuf );
         join_money( round_money( atoi( changebuf ), TRUE ), ch->money );
         send_to_char( "The healer hands you some change.\r\n", ch );
         break;

      case 'H':  /* Heal */
         if( money_value( ch->money ) < ( mult * 50 ) )
         {
            send_to_char( "You don't have enough money...\r\n", ch );
            return;
         }
         act( "$N gestures towards $n.", ch, NULL, mob, TO_NOTVICT );
         act( "$N gestures towards you.", ch, NULL, mob, TO_CHAR );
         spell_heal( skill_lookup( "heal" ), mult, mob, ch, NULL );
         give = take_best_coins( ch->money, ( mult * 50 ) );
         give = one_argument( give, changebuf );
         snprintf( givebuf, MSL, "%s to %s", give, mob->name.c_str() );
         do_give( ch, givebuf );
         join_money( round_money( atoi( changebuf ), TRUE ), ch->money );
         send_to_char( "The healer hands you some change.\r\n", ch );
         break;
      case 'I':  /* invis */
         if( money_value( ch->money ) < ( mult * 20 ) )
         {
            send_to_char( "You don't have enough money...\r\n", ch );
            return;
         }
         act( "$N gestures towards $n.", ch, NULL, mob, TO_NOTVICT );
         act( "$N gestures towards you.", ch, NULL, mob, TO_CHAR );
         spell_invis( skill_lookup( "invis" ), mult, mob, ch, NULL );
         give = take_best_coins( ch->money, ( mult * 20 ) );
         give = one_argument( give, changebuf );
         snprintf( givebuf, MSL, "%s to %s", give, mob->name.c_str() );
         do_give( ch, givebuf );
         join_money( round_money( atoi( changebuf ), TRUE ), ch->money );
         send_to_char( "The healer hands you some change.\r\n", ch );
         break;
      case 'D':  /* detect invis */
         if( money_value( ch->money ) < ( mult * 10 ) )
         {
            send_to_char( "You don't have enough money...\r\n", ch );
            return;
         }
         act( "$N gestures towards $n.", ch, NULL, mob, TO_NOTVICT );
         act( "$N gestures towards you.", ch, NULL, mob, TO_CHAR );
         spell_detect_invis( skill_lookup( "detect invis" ), mult, mob, ch, NULL );
         give = take_best_coins( ch->money, ( mult * 10 ) );
         give = one_argument( give, changebuf );
         snprintf( givebuf, MSL, "%s to %s", give, mob->name.c_str() );
         do_give( ch, givebuf );
         join_money( round_money( atoi( changebuf ), TRUE ), ch->money );
         send_to_char( "The healer hands you some change.\r\n", ch );
         break;
      case 'R':  /* refresh */
         if( money_value( ch->money ) < ( mult * 10 ) )
         {
            send_to_char( "You don't have enough money...\r\n", ch );
            return;
         }
         act( "$N gestures towards $n.", ch, NULL, mob, TO_NOTVICT );
         act( "$N gestures towards you.", ch, NULL, mob, TO_CHAR );
         spell_refresh( skill_lookup( "refresh" ), mult, mob, ch, NULL );
         give = take_best_coins( ch->money, ( mult * 10 ) );
         give = one_argument( give, changebuf );
         snprintf( givebuf, MSL, "%s to %s", give, mob->name.c_str() );
         do_give( ch, givebuf );
         join_money( round_money( atoi( changebuf ), TRUE ), ch->money );
         send_to_char( "The healer hands you some change.\r\n", ch );
         break;
      case 'N':  /* Infra */
         if( money_value( ch->money ) < ( mult * 20 ) )
         {
            send_to_char( "You don't have enough money...\r\n", ch );
            return;
         }
         act( "$N gestures towards $n.", ch, NULL, mob, TO_NOTVICT );
         act( "$N gestures towards you.", ch, NULL, mob, TO_CHAR );
         spell_infravision( skill_lookup( "infra" ), mult, ch, ch, NULL );
         give = take_best_coins( ch->money, ( mult * 20 ) );
         give = one_argument( give, changebuf );
         snprintf( givebuf, MSL, "%s to %s", give, mob->name.c_str() );
         do_give( ch, givebuf );
         join_money( round_money( atoi( changebuf ), TRUE ), ch->money );
         send_to_char( "The healer hands you some change.\r\n", ch );
         break;
      case 'M':  /* dispel */
         if( money_value( ch->money ) < ( mult * 200 ) )
         {
            send_to_char( "You don't have enough money...\r\n", ch );
            return;
         }
         /*
          * No acts, as they are in spell_dispel_magic.  Doh.
          */
         spell_dispel_magic( skill_lookup( "dispel magic" ), mult * 5, mob, ch, NULL );
         give = take_best_coins( ch->money, ( mult * 200 ) );
         give = one_argument( give, changebuf );
         snprintf( givebuf, MSL, "%s to %s", give, mob->name.c_str() );
         do_give( ch, givebuf );
         join_money( round_money( atoi( changebuf ), TRUE ), ch->money );
         send_to_char( "The healer hands you some change.\r\n", ch );
         break;
      default:
         send_to_char( "Are you sure you're reading the instructions right??\r\n", ch );
         return;
   }
   return;
}

DO_FUN(do_gain)
{
   /*
    * Allow ch to gain a level in a chosen class.
    * * Only can be done at prac/train mob. -S-
    * * Now handles remort chars
    */

   CHAR_DATA *mob;
   char buf[MSL];
   int cost = 0;
   int cnt;
   int subpop;
   bool any;
   int c;   /* The class to gain in */
   int numclasses;   /* Current number of classes person has */
   int a;   /* Looping var */
   bool remort = FALSE;
   bool vamp = FALSE;
   bool adept = FALSE;
   bool wolf = FALSE;
   int vamp_cost = 0;
   short morts_at_seventy = 0;
   short remorts_at_seventy = 0;
   short morts_at_eighty = 0;
   short remorts_at_eighty = 0;
   short num_remorts = 0;
   bool allow_remort = FALSE;
   bool allow_adept = FALSE;

   buf[0] = '\0';




   if( IS_NPC( ch ) )
   {
      send_to_char( "Hahaha, not for NPCs.\r\n", ch );
      return;
   }

   /*
    * Check for mob with act->pac/train 
    */
   for( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
   {
      if( IS_NPC( mob ) && ( mob->act.test(ACT_TRAIN) || mob->act.test(ACT_PRACTICE) ) )
         break;
   }

   if( mob == NULL )
   {
      send_to_char( "You can't do that here.\r\n", ch );
      return;
   }
   for( cnt = 0; cnt < MAX_CLASS; cnt++ )
   {
      if( ch->lvl[cnt] >= 70 )
         morts_at_seventy++;
      if( ch->lvl[cnt] == 80 )
         morts_at_eighty++;
      if( ch->lvl2[cnt] >= 70 )
         remorts_at_seventy++;
      if( ch->lvl2[cnt] == 80 )
         remorts_at_eighty++;
      if( ch->lvl2[cnt] > -1 )
         num_remorts++;
   }
/* first case.. remort  */
   if( ( ( morts_at_seventy >= 2 )
         && ( IS_REMORT( ch ) == FALSE ) )
       || ( ( morts_at_eighty == 5 ) && ( remorts_at_seventy == 1 ) && ( num_remorts == 1 ) ) )
   {
      allow_remort = TRUE;
   }

/* second case..can adept */

   if( ( morts_at_eighty == 5 ) && ( remorts_at_eighty == 2 ) && ( !IS_ADEPT(ch) ) )
   {
      allow_adept = TRUE;
   }

   if( argument[0] == '\0' )
   {

      /*
       * Display details...
       */
      send_to_char( "You can gain levels in:\r\n", ch );
      any = FALSE;
      numclasses = 0;
      for( a = 0; a < MAX_CLASS; a++ )
         if( ch->lvl[a] >= 0 )
            numclasses++;

      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         if( ( ch->lvl[cnt] != -1 || numclasses < race_table[ch->race].classes ) && ch->lvl[cnt] < ( LEVEL_HERO - 1 ) )
         {
            any = TRUE;
            cost = exp_to_level( ch, cnt, ch->pcdata->order[cnt] );

            snprintf( buf, MSL, "%s : %d Exp.\r\n", class_table[cnt].who_name, cost );
            send_to_char( buf, ch );
         }

      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         if( ch->lvl2[cnt] != -1 && ch->lvl2[cnt] < ( LEVEL_HERO - 1 ) )
         {
            any = TRUE;
            cost = exp_to_level( ch, cnt, 5 );  /* 5 means remort */
            snprintf( buf, MSL, "%s : %d Exp.\r\n", remort_table[cnt].who_name, cost );
            send_to_char( buf, ch );
         }
      if( IS_ADEPT(ch) && ch->get_level("adept") < 20 )
      {
         any = TRUE;
         cost = exp_to_level_adept( ch );
         snprintf( buf, MSL, "@@WAdept@@N: %d Exp.\r\n", cost );
         send_to_char( buf, ch );
      }
      if( allow_remort )
      {
         any = TRUE;
         send_to_char( "You can @@mREMORT@@N!!! Type gain <first three letters of the class> you want.\r\n", ch );
      }
      if( allow_adept )
      {
         any = TRUE;
         send_to_char( "You can @@WADEPT@@N!!! Type gain adept!!\r\n", ch );
      }
      if( any )
         send_to_char( ".\r\n", ch );
      else
         send_to_char( "None.\r\n", ch );
      if( ( IS_VAMP( ch ) ) && ( ch->pcdata->super->level < ( MAX_VAMP_LEVEL - ( ch->pcdata->super->generation / 2 ) ) ) )
         if( IS_VAMP( ch ) )
            if( ch->pcdata->super->exp >= exp_to_level_vamp( ch->pcdata->super->level ) )
               send_to_char( "@@NYou may gain a @@dVAMPYRE@@N level!!!\r\n", ch );
      if( ( IS_WOLF( ch ) ) && ( ch->pcdata->super->level < ( ( MAX_WOLF_LEVEL + 2 ) - ( ch->pcdata->super->generation * 2 ) ) ) )
         if( IS_WOLF( ch ) )
            if( ch->pcdata->super->exp >= exp_to_level_wolf( ch->pcdata->super->level ) )
               send_to_char( "@@NYou may gain a @@bWerewolf@@N level!!!\r\n", ch );

      return;
   }

   /*
    * If an argument supplied, make sure it's valid :P
    */

   any = FALSE;
   c = -1;
   for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      if( !str_cmp( class_table[cnt].who_name, argument ) )
      {
         any = TRUE;
         c = cnt;
      }

   for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      if( ( !str_cmp( remort_table[cnt].who_name, argument ) ) && ( ( ch->lvl2[cnt] > 0 ) || ( allow_remort ) ) )
      {
         any = TRUE;
         remort = TRUE;
         c = cnt;
      }
   if( !str_prefix( "VAMPIRE", argument ) )
   {
      if( IS_VAMP( ch ) )
      {
         any = TRUE;
         vamp = TRUE;
      }
   }

   if( !str_prefix( "WEREWOLF", argument ) )
   {
      if( IS_WOLF( ch ) )
      {
         any = TRUE;
         wolf = TRUE;
      }

   }

   if( !str_prefix( "ADEPT", argument ) )
   {
      if( !IS_ADEPT(ch) && !allow_adept )
         return;

      any = TRUE;
      adept = TRUE;
   }

   if( !any )
   {
      send_to_char( "That's not a class!\r\n", ch );
      return;
   }

   /*
    * Ok, so now class should be valid.  Check if enough exp
    */
   if( wolf )
      vamp_cost = exp_to_level_wolf( ch->pcdata->super->level );
   else if( vamp )
      vamp_cost = exp_to_level_vamp( ch->pcdata->super->level );
   else if( remort )
      cost = exp_to_level( ch, c, 5 );
   else if( adept )
   {
      if( !IS_ADEPT(ch) )
         cost = 0;
      else
         cost = exp_to_level_adept( ch );
   }
   else
      cost = exp_to_level( ch, c, ( ch->pcdata->order[c] ) );

   if( vamp )
   {
      if( ch->pcdata->super->exp < vamp_cost )
      {
         send_to_char( "@@NYou have not yet mastered your current knowledge of the ways of the @@dKINDRED@@N!!\r\n", ch );
         return;
      }
   }
   else if( wolf )
   {
      if( ch->pcdata->super->exp < vamp_cost )
      {
         send_to_char( "@@NYour @@rtribe@@N does not consider you worthy!@@N!!\r\n", ch );
         return;
      }
   }

   else if( ch->exp < cost )
   {
      snprintf( buf, MSL, "Cost is %d Exp.  You only have %d (%d short).\r\n", cost, ch->exp, ( cost - ch->exp ) );
      send_to_char( buf, ch );
      return;
   }

   if( ( wolf ) && ( ch->pcdata->super->level < ( MAX_WOLF_LEVEL - ( ch->pcdata->super->generation * 2 ) ) ) )
   {
      c = ADVANCE_WOLF;
      send_to_char( "@@NYour @@rTribe@@N increases your standing@@N!!!\r\n", ch );
      ch->pcdata->super->exp -= vamp_cost;
      advance_level( ch, c, TRUE, remort );
      ch->pcdata->super->level += 1;
      do_save( ch, "auto" );
      return;
   }
   else if( wolf )
   {
      send_to_char( "@@NYour @@rTribe @@Ndenies your request@@N.\r\n", ch );
      return;
   }


   if( ( vamp ) && ( ch->pcdata->super->level < ( MAX_VAMP_LEVEL - ( ch->pcdata->super->generation / 2 ) ) ) )
   {
      c = ADVANCE_VAMP;
      send_to_char( "@@NYou gain more power in the ways of the @@dKindred@@N!!!\r\n", ch );
      ch->pcdata->super->exp -= vamp_cost;
      advance_level( ch, c, TRUE, remort );
      ch->pcdata->super->level += 1;
      do_save( ch, "auto" );
      return;
   }
   else if( vamp )
   {
      send_to_char( "@@NYou have reached the epitome of Rank in the ways of the @@eKindred@@N.\r\n", ch );
      return;
   }
   if( ( adept ) && ( ch->get_level("adept") < 20 ) )
   {
      c = ADVANCE_ADEPT;
      send_to_char( "@@WYou have reached another step on the stairway to Wisdom!!!@@N\r\n", ch );
      ch->exp -= cost;
      advance_level( ch, c, TRUE, FALSE );
      ch->pcdata->adept_level = UMAX( 1, ch->pcdata->adept_level + 1 );
      snprintf( buf, MSL, "%s @@W advances in the way of the Adept!!\r\n", ch->name.c_str() );
      info( buf, 1 );
      free_string( ch->pcdata->who_name );
      ch->pcdata->who_name = str_dup( get_adept_name( ch ) );
      do_save( ch, "auto" );
      if( ch->get_level("adept") == 1 )
         ch->exp /= 1000;
      return;
   }
   else if( adept )
   {
      send_to_char
         ( "@@aYou peer down upon all the hapless mortals, knowing that you have reached the final step upon the stairway of Wisdom.@@N\r\n",
           ch );
      return;
   }
   /*
    * Don't bother adapting for remort... dropped num classes yrs ago!
    */
   if( ch->lvl[c] < 0 )
   {
      /*
       * Check to see if max. num of classes has been reached.
       */
      numclasses = 0;

      for( a = 0; a < MAX_CLASS; a++ )
         if( ch->lvl[a] >= 0 )
            numclasses++;

      if( numclasses >= race_table[ch->race].classes )
      {
         /*
          * Already got max. number of classes
          */
         send_to_char( "Cannot level in that class, already have maximum number of classes.\r\n", ch );
         return;
      }

      ch->lvl[c] = 0;
   }

   /*
    * Ok,ok now we know ch has enough exps.  Do the advancement stuff
    */

   if( ( remort ? ch->lvl2[c] : ch->lvl[c] ) + 1 >= LEVEL_HERO )
   {
      send_to_char( "If you wish to advance this class, please ask an Immortal.\r\n", ch );
      return;
   }
   /*
    * Check to see if able to reach new level
    */
   if( remort )
      snprintf( buf, MSL, "You gain a %s level!\r\n", remort_table[c].class_name );
   else
      snprintf( buf, MSL, "You gain a %s level!\r\n", class_table[c].class_name );
   send_to_char( buf, ch );

   /*
    * Use info channel to inform of level gained!
    */

   if( remort )
      snprintf( buf, MSL, "%s advances in the way of the %s.", ch->name.c_str(), remort_table[c].class_name );
   else
      snprintf( buf, MSL, "%s advances in the way of the %s.", ch->name.c_str(), class_table[c].class_name );
   info( buf, 1 );

   ch->exp -= cost;

   advance_level( ch, c, TRUE, remort );
   if( remort )
      ch->lvl2[c] = UMAX( 1, ch->lvl2[c] + 1 );
   else
      ch->lvl[c] += 1;  /* Incr. the right class */


   /*
    * Maintain ch->level as max level of the lot
    */
   for( subpop = 0; subpop < MAX_CLASS; subpop++ )
   {
      if( ch->lvl[subpop] > ch->level )
         ch->level = ch->lvl[subpop];
      if( ch->lvl2[subpop] > ch->level )
         ch->level = ch->lvl2[subpop];
   }
   do_save( ch, "auto" );
   return;
}

DO_FUN(do_assassinate)
{
   char buf[MSL];
   int cost;
   int change = 0;
   char givebuf[MSL], changebuf[MSL];
   char *cost_string;
   CHAR_DATA *mob;
   CHAR_DATA *victim;
   buf[0] = '\0';


   /*
    * Check for mob with act->merc
    */
   for( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
   {
      if( IS_NPC( mob ) && ( mob->act.test(ACT_MERCENARY) ) )
         break;
   }

   if( mob == NULL )
   {
      send_to_char( "There doesn't seem to be a mercenary around!\r\n", ch );
      return;
   }

   if( mob->hunting != NULL || mob->hunt_obj != NULL )
   {
      act( "$N looks to be busy already!", ch, NULL, mob, TO_CHAR );
      return;
   }

   cost = ( mob->level * 10000 );

   if( argument[0] == '\0' )
   {
      snprintf( buf, MSL, "$N tells you, 'The cost is %d GP.'", cost );
      act( buf, ch, NULL, mob, TO_CHAR );
      return;
   }

   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      act( "$N tells you, 'I can't locate the victim.  Sorry.'", ch, NULL, mob, TO_CHAR );
      return;
   }

   if( victim == ch )
      return;


   if( IS_NPC( victim ) )
   {
      act( "$N tells you, 'I only go after REAL players, they're more fun!`", ch, NULL, mob, TO_CHAR );
      return;
   }

   if( victim->level < 12 )
   {
      act( "$N tells you, 'I'm not interested in small fry.'", ch, NULL, mob, TO_CHAR );
      return;
   }


   if( money_value(ch->money) < cost )
   {
      act( "$N tells you, 'You can't afford my services!'", ch, NULL, mob, TO_CHAR );
      return;
   }


   if( !set_hunt( mob, ch, victim, NULL, HUNT_WORLD | HUNT_MERC, HUNT_INFORM | HUNT_CR ) )
   {
      act( "$N tells you, 'I can't seem to find the target!'", ch, NULL, mob, TO_CHAR );
      return;
   }
/*   if ( !make_hunt( mob, victim ) )
   {
      act( "$N tells you, 'I can't seem to find the target!'", ch, NULL, mob, TO_CHAR );
      return;
   }*/

   cost_string = take_best_coins( ch->money, cost );
   cost_string = one_argument( cost_string, changebuf );
   change = is_number( changebuf ) ? atoi( changebuf ) : 0;
   snprintf( givebuf, MSL, "%s to %s", cost_string, mob->name.c_str() );
   do_give( ch, givebuf );

   if( change > 0 )
   {
      MONEY_TYPE *transaction;
      transaction = round_money( change, TRUE );
      join_money( transaction, ch->money );
   }

   act( "$n gives $N some gold coins.", ch, NULL, mob, TO_NOTVICT );
   act( "$n says '$N shall die by my hand!`", mob, NULL, victim, TO_ROOM );
   snprintf( buf, MSL, "%s employs the services of %s to assassinate %s!!", ch->name.c_str(), mob->get_name(), victim->name.c_str() );
   info( buf, 1 );
   return;
}

DO_FUN(do_alias)
{
   /*
    * Handle aliases - setting and clearing, as well as listing. 
    */

   int cnt;
   int alias_no;
   char buf[MSL];
   char arg1[MSL];
   char arg2[MSL];
   char arg3[MSL];

   buf[0] = '\0';


   if( IS_NPC( ch ) )
   {
      send_to_char( "Not a chance!\r\n", ch );
      return;
   }
   smash_tilde( argument );
   if( argument[0] == '\0' )
   {
      send_to_char( "Defined Aliases:\r\n", ch );

      for( cnt = 0; cnt < MAX_ALIASES; cnt++ )
      {
         snprintf( buf, MSL, "(%d) [Name:] %12s  [Aliases:] %s\r\n", cnt, ch->pcdata->alias_name[cnt], ch->pcdata->alias[cnt] );
         send_to_char( buf, ch );
      }
      send_to_char( "\r\nTo Set an Alias:\r\n", ch );
      send_to_char( "ALIAS <num> <name> <alias>\r\n", ch );
      send_to_char( "-enter 'CLEAR' as name to clear an alias.\r\n", ch );
      return;
   }

   argument = one_argument( argument, arg1 );   /* Number */
   argument = one_argument( argument, arg2 );   /* name   */
   strcpy( arg3, argument );  /* alias  */

   if( arg3[0] == '\0' && str_cmp( arg2, "clear" ) )
   {
      send_to_char( "\r\nTo Set an Alias:\r\n", ch );
      send_to_char( "ALIAS <num> <name> <alias>\r\n", ch );
      send_to_char( "-enter 'CLEAR' as name to clear an alias.\r\n", ch );
      return;
   }

   if( !is_number( arg1 ) )
   {
      send_to_char( "First argument must be an integer.\r\n", ch );
      return;
   }
   else
      alias_no = atoi( arg1 );

   if( alias_no < 0 || alias_no > ( MAX_ALIASES - 1 ) )
   {
      snprintf( buf, MSL, "Valid alias numbers are 0 to %d.\r\n", MAX_ALIASES );
      send_to_char( buf, ch );
      return;
   }

   if( !str_cmp( "clear", arg2 ) )
   {
      free_string( ch->pcdata->alias_name[alias_no] );
      free_string( ch->pcdata->alias[alias_no] );

      ch->pcdata->alias_name[alias_no] = str_dup( "<none>" );
      ch->pcdata->alias[alias_no] = str_dup( "<none>" );

      /*
       * Clear the alias (enter <none> for name and alias 
       */
      return;
   }

   /*
    * Hopefully, now just set the (new) alias... 
    */

   free_string( ch->pcdata->alias_name[alias_no] );
   free_string( ch->pcdata->alias[alias_no] );

   ch->pcdata->alias_name[alias_no] = str_dup( arg2 );
   ch->pcdata->alias[alias_no] = str_dup( arg3 );
   return;
}

DO_FUN(do_color)
{
   /*
    * Allow users to set which color they get certain texts in. -S- 
    */

   char buf[MSL];
   char arg1[MSL];
   char arg2[MSL];
   int col;
   int cnt;
   int ansi_number;
   int color_number;
   buf[0] = '\0';


   if( IS_NPC( ch ) )
      return;

   /*
    * First check to see if there is NO argument.  If so, display
    * * the current settings for players color. 
    * * In this context, 'color' means the type of text, eg "say" or "shout"
    * *                  'ansi'   means the actual color
    * *                  ->Confusing, right?
    * * -S-
    */

   if( IS_NPC( ch ) )
      return;
   col = 0;

   if( argument[0] == '\0' )
   {
      send_to_char( "@@yPresent color Configuration:@@g\r\n\r\n", ch );

      for( cnt = 0; cnt < MAX_COLOR; cnt++ )
      {
         snprintf( buf, MSL, "@@W%8s: %s%-12s@@N   ",
                  color_table[cnt].name, ansi_table[ch->pcdata->color[cnt]].value, ansi_table[ch->pcdata->color[cnt]].name );
         send_to_char( buf, ch );
         if( ++col % 3 == 0 )
            send_to_char( "\r\n", ch );
      }
      if( col % 3 != 0 )
         send_to_char( "\r\n", ch );

      send_to_char( "\r\n@@yAvailable colors:@@g\r\n", ch );

      col = 0;

      for( cnt = 0; cnt < MAX_ANSI; cnt++ )
      {
         snprintf( buf, MSL, "%s%-12s@@N  ",
                  ch->act.test(ACT_COLOR) ? ansi_table[cnt].value : "", ansi_table[cnt].name );
         send_to_char( buf, ch );
         if( ++col % 5 == 0 )
            send_to_char( "\r\n", ch );
      }
      if( col % 5 != 0 )
         send_to_char( "\r\n", ch );

      send_to_char( "\r\nUSAGE: color <name> <color>\r\n", ch );
      send_to_char( "Eg:     color say red\r\n", ch );
      send_to_char( "OR: color highlighted/dimmed <color> for emphasized or dimmed text.\r\n", ch );

      send_to_char( "NOTE: The 'stats' info covers who, score, etc.\r\n", ch );
      return;
   }

   argument = one_argument( argument, arg1 );   /* The name, eg 'say'   */
   argument = one_argument( argument, arg2 );   /* The color, eg 'red' */

   if( arg2[0] == '\0' )
   {
      do_color( ch, "" );  /* Generate message */
      return;
   }
   if( !str_prefix( arg1, "highlighted" ) )
      color_number = -2;
   else if( !str_prefix( arg1, "dimmed" ) )
      color_number = -3;
   else
   {
      /*
       * Check to see if the name is valid 
       */
      color_number = -1;
      for( cnt = 0; cnt < MAX_COLOR; cnt++ )
         if( !str_cmp( arg1, color_table[cnt].name ) )
            color_number = color_table[cnt].index;
   }

   if( color_number == -1 )
   {
      /*
       * list possible choices 
       */
      /*
       * do_color( ch, "help" ); 
       */
      return;
   }

   /*
    * color (the name) is ok.  Now find the ansi (the color) 
    */
   ansi_number = -1;
   for( cnt = 0; cnt < MAX_ANSI; cnt++ )
      if( !str_cmp( arg2, ansi_table[cnt].name ) )
         ansi_number = ansi_table[cnt].index;

   if( ansi_number == -1 )
   {
      /*
       * list possible choice 
       */
      /*
       * do_color( ch, "help" ); 
       */
      return;
   }

   /*
    * Ok now, we have color_number, which is the index to pcdata->color[]
    * * so we need to set the value of it to the color.  
    * * -S-
    */


   if( color_number == -2 )
   {
      ch->pcdata->hicol = ansi_table[ansi_number].letter;
      return;
   }
   else if( color_number == -3 )
   {
      ch->pcdata->dimcol = ansi_table[ansi_number].letter;
      return;
   }

   ch->pcdata->color[color_number] = ansi_number;
   send_to_char( "OK.\r\n", ch );
   return;
}

/* A simple, return the char sequence, function -S- */

char *color_string( CHAR_DATA * ch, char *argument )
{
   int cnt;
   int num;

   /*
    * if we don't want to send the string, return "" 
    */
   /*
    * argument should be the string to find, eg "say" 
    */

   if( IS_NPC( ch ) || argument[0] == '\0' )
      return ( "" );

   if( !ch->act.test(ACT_COLOR) )
      return ( "" );

   if( !str_cmp( argument, "normal" ) )
      return ( "\x1b[0m" );

   /*
    * By here, ch is a PC and wants color 
    */
   num = -1;
   for( cnt = 0; cnt < MAX_COLOR; cnt++ )
      if( !str_cmp( argument, color_table[cnt].name ) )
         num = cnt;

   if( num == -1 )   /* bug report? */
      return ( "" );

   return ( ansi_table[ch->pcdata->color[num]].value );
}

DO_FUN(do_worth)
{
   /*
    * Show details regarding cost to level each class, etc 
    */
   bool any;
   char buf[MSL];
   int numclasses;
   int a;
   int cost;
   int cnt;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Only for PCs.\r\n", ch );
      return;
   }

   send_to_char( "Costs in Exp for you to level:\r\n", ch );
   send_to_char( "Cost is shown first, followed by how much more exp you need.\r\n\r\n", ch );
   send_to_char( "CLASS NAME:        COST:    DIFFERENCE:\r\n\r\n", ch );

   if( IS_ADEPT(ch) )
   {

      cost = exp_to_level_adept( ch );
      snprintf( buf, MSL, " %-14s  %9d %9d.\r\n", get_adept_name( ch ), cost, UMAX( 0, cost - ch->exp ) );
      send_to_char( buf, ch );
      return;
   }

   any = FALSE;
   numclasses = 0;
   for( a = 0; a < MAX_CLASS; a++ )
      if( ch->lvl[a] >= 0 )
         numclasses++;

   for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      if( ( ch->lvl[cnt] != -1 || numclasses < race_table[ch->race].classes ) && ch->lvl[cnt] < LEVEL_HERO - 1 )
      {
         any = TRUE;
         cost = exp_to_level( ch, cnt, ch->pcdata->order[cnt] );

         snprintf( buf, MSL, "%-14s  %9d %9d.\r\n", class_table[cnt].who_name, cost, UMAX( 0, cost - ch->exp ) );
         send_to_char( buf, ch );
      }

   /*
    * Check for remort classes
    */
   for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      if( ch->lvl2[cnt] != -1 && ch->lvl2[cnt] < LEVEL_HERO - 1 )
      {
         any = TRUE;
         cost = exp_to_level( ch, cnt, 5 );  /* Pass 5 for remort */
         snprintf( buf, MSL, "%-14s  %9d %9d.\r\n", remort_table[cnt].who_name, cost, UMAX( 0, cost - ch->exp ) );
         send_to_char( buf, ch );
      }

   if( any )
      send_to_char( "\r\n", ch );
   else
      send_to_char( "None to show!!\r\n", ch );

   return;
}

DO_FUN(do_whois)
{
   /*
    * Show ch some details about the 'victim'
    * * Make sure ch can see victim!
    * * -S-
    */

   CHAR_DATA *victim;
   char buf[MSL];

   if( ( victim = get_char_world( ch, argument ) ) == NULL )
   {
      send_to_char( "No such player found.\r\n", ch );
      return;
   }

   if( !can_see( ch, victim ) )
   {
      send_to_char( "No such player found.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPCs.\r\n", ch );
      return;
   }

   /*
    * Ok, so now show the details!
    */
   snprintf( buf, MSL, "-=-=-=-=-=-=-=-=-=-=- %9s -=-=-=-=-=-=-=-=-=-=-\r\n", victim->name.c_str() );
   if( IS_IMMORTAL( victim ) )
   {
      snprintf( buf + strlen( buf ), MSL, " [ %3s ]\r\n", victim->pcdata->who_name );
   }
   else if( IS_ADEPT(victim) )
   {
      snprintf( buf + strlen( buf ), MSL, " %s \r\n", get_adept_name( victim ) );
   }
   else
   {
      snprintf( buf + strlen( buf ), MSL, "Levels: [ Mag:%2d  Cle:%2d  Thi:%2d  War:%2d  Psi:%2d ]\r\n",
               victim->get_level("mag") > 0 ? victim->get_level("mag") : 0,
               victim->get_level("cle") > 0 ? victim->get_level("cle") : 0,
               victim->get_level("thi") > 0 ? victim->get_level("thi") : 0,
               victim->get_level("war") > 0 ? victim->get_level("war") : 0,
               victim->get_level("psi") > 0 ? victim->get_level("psi") : 0 );

      if( IS_REMORT( victim ) )

         snprintf( buf + strlen( buf ), MSL, "Levels: [ Sor:%2d  Mon:%2d  Ass:%2d  Kni:%2d  Nec:%2d ]\r\n",
                  victim->get_level("sor") > 0 ? victim->get_level("sor") : 0,
                  victim->get_level("mon") > 0 ? victim->get_level("mon") : 0,
                  victim->get_level("ass") > 0 ? victim->get_level("ass") : 0,
                  victim->get_level("kni") > 0 ? victim->get_level("kni") : 0,
                  victim->get_level("nec") > 0 ? victim->get_level("nec") : 0 );
   }
   snprintf( buf + strlen( buf ), MSL, "Sex: %s.  Race: %s.  Clan: %s.\r\n",
            ( victim->sex == SEX_MALE ) ? "Male" :
            ( victim->sex == SEX_FEMALE ) ? "Female" : "None",
            race_table[victim->race].race_name, clan_table[victim->clan].clan_name );

   /*
    * if (victim->act != 0)
    * snprintf( buf+strlen(buf), "Player is %s\r\n",bit_table_lookup(tab_player_act,victim->act));
    * taken out to not show vamps :P 
    */
   if( victim->act.test(ACT_PKOK) )
      snprintf( buf + strlen( buf ), MSL, "Player is @@ePKOK@@N\r\n" );
   snprintf( buf + strlen( buf ), MSL, "Players Killed: %d.  Times killed by players: %d.\r\n",
            victim->pcdata->records->pk, victim->pcdata->records->pd );

   snprintf( buf + strlen( buf ), MSL, "Mobs killed: %d.  Times killed by mobs: %d.\r\n",
            victim->pcdata->records->mk, victim->pcdata->records->md );

   if( IS_IMMORTAL( victim ) )
   {
      snprintf( buf + strlen( buf ), MSL, "%s is an Immortal.\r\n", victim->name.c_str() );
   }
   /*
    * Description here, or email address? 
    */

   snprintf( buf + strlen( buf ), MSL, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n" );
   send_to_char( buf, ch );
   return;
}

DO_FUN(do_shelp)
{
 /*
  * Like help, except for spells and skills. 
  */
 int sn;
 char buf[MSL];
 buf[0] = '\0';

 if( argument[0] == '\0' )
 {
  do_help( ch, SHELP_INDEX );
  return;
 }

 if( ( sn = skill_lookup( argument ) ) < 0 )
 {
  snprintf( buf, MSL, "No sHelp found for argument:%s\r\n", argument );
  send_to_char( buf, ch );
  return;
 }
 snprintf( buf, MSL, "shelp_%s", skill_table[sn].name );
 do_help(ch,buf);

 return;
}

DO_FUN(do_afk)
{
   if( IS_NPC( ch ) )
      return;

   ch->act.flip(ACT_AFK);

   if( ch->act.test(ACT_AFK) )
      send_to_char( "AFK flag turned on.\r\n", ch );
   else
      send_to_char( "AFK flag turned off.\r\n", ch );

   return;
}

DO_FUN(do_colist)
{
   int col, n;
   char buf[MSL];

   if( IS_NPC( ch ) )
      return;

   send_to_char( "@@Wcolor Codes: @@yTo use color codes within a string, use the following\r\n", ch );
   send_to_char( "characters in sequence: @ @ <letter>.  Do not leave any spaces when you use\r\n", ch );
   send_to_char( "the codes.  (Spaces are left above so that the characters are not interpreted\r\n", ch );
   send_to_char( "as colors when you see this text!)@@g\r\n\r\n", ch );

   n = 0;

   for( col = 0; col < MAX_ANSI; col++ )
   {
      snprintf( buf, MSL, "%c - %s%-14s@@N    ",
               ansi_table[col].letter,
               ch->act.test(ACT_COLOR) ? ansi_table[col].value : "", ansi_table[col].name );
      send_to_char( buf, ch );
      if( ++n % 3 == 0 )
         send_to_char( "\r\n", ch );
   }
   send_to_char("z - @@zrandom color@@N      q - @@qrandom back color@@N\r\n",ch);
   if( n % 3 != 0 )
      send_to_char( "\r\n", ch );

   return;
}

DO_FUN(do_loot)
{

   char arg[MSL];
   OBJ_DATA *corpse;
   OBJ_DATA *obj = NULL;
   int counter, num;

   one_argument( argument, arg );

   if( IS_NPC( ch ) )
   {
      send_to_char( "NPC's cannot loot corpses.\r\n", ch );
      return;
   }

   if( arg[0] == '\0' )
   {
      send_to_char( "Loot what?\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "all" ) || !str_prefix( "all.", arg ) )
   {
      send_to_char( "You can't do that.\r\n", ch );
      return;
   }

   corpse = get_obj_room( ch, arg, ch->in_room->first_content );
   if( corpse == NULL )
   {
      act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
      return;
   }

   if( corpse->item_type == ITEM_CORPSE_NPC )
   {
      send_to_char( "Just go ahead and take it.\r\n", ch );
      return;
   }

   if( corpse->item_type != ITEM_CORPSE_PC )
   {
      send_to_char( "You cannot loot that.\r\n", ch );
      return;
   }
   /*
    * begin checking for lootability 
    */

   if( ( ch->clan == 0 ) && ( !ch->act.test(ACT_PKOK) ) && ( !IS_VAMP( ch ) && !IS_WOLF( ch ) ) )
   {
      send_to_char( "You cannot loot corpses.\r\n", ch );
      return;
   }

   if( corpse->value[3] == 0 )
   {
      send_to_char( "You cannot loot this corpse.\r\n", ch );
      return;
   }

   if( ( ch->clan == corpse->value[2] )
       || ( ( ch->act.test(ACT_PKOK) )
            && ( corpse->value[0] == 1 ) ) || ( ( IS_WOLF( ch ) || IS_VAMP( ch ) ) && ( corpse->value[0] == 1 ) ) )
   {
      counter = number_range( 1, 100 );

      if( counter >= 40 )
      {
         if( corpse->first_in_carry_list == NULL )
         {
            send_to_char( "There isn't anything in the corpse.\r\n", ch );
            return;
         }

         num = 0;
         for( obj = corpse->first_in_carry_list; obj != NULL; obj = obj->next_in_carry_list )
         {
            ++num;
         }

         counter = number_range( 1, num );

         obj = corpse->first_in_carry_list;
         for( num = 1; num < counter; ++num )
         {
            obj = obj->next_in_carry_list;
         }

         if( !IS_OBJ_STAT(obj,ITEM_EXTRA_NO_LOOT) )
         {
            get_obj( ch, obj, corpse );

            /*
             * just incase... 
             */
            if( ch->level > 1 )
            {
               do_save( ch, "auto" );
            }
         }
         else
         {
            send_to_char( "You failed to loot the corpse.\r\n", ch );
            return;
         }

         corpse->value[3] = corpse->value[3] - 1;
         return;
      }
      else
      {
         send_to_char( "You failed to loot the corpse.\r\n", ch );
         return;
      }
   }

   send_to_char( "You cannot loot this corpse.\r\n", ch );
   return;
}

char *output_race_wear( int iRace )
{
 static char buf[MSL];

 buf[0] = '\0';
 
 strncat(buf,race_table[iRace].wear_locs[0]  ? " @@aLight@@N"      : " @@dLight@@N"      ,MSL);
 strncat(buf,race_table[iRace].wear_locs[1]  ? " @@aFloating@@N"   : " @@dFloating@@N"   ,MSL);
 strncat(buf,race_table[iRace].wear_locs[2]  ? " @@aAura@@N"       : " @@dAura@@N"       ,MSL);
 strncat(buf,race_table[iRace].wear_locs[3]  ? " @@aHorns@@N"      : " @@dHorns@@N"      ,MSL);
 strncat(buf,race_table[iRace].wear_locs[4]  ? " @@aHead@@N"       : " @@dHead@@N"       ,MSL);
 strncat(buf,race_table[iRace].wear_locs[5]  ? " @@aFace@@N"       : " @@dFace@@N"       ,MSL);
 strncat(buf,race_table[iRace].wear_locs[6]  ? " @@aBeak@@N"       : " @@dBeak@@N"       ,MSL);
 strncat(buf,race_table[iRace].wear_locs[7]  ? " @@aEar@@N"        : " @@dEar@@N"        ,MSL);
 strncat(buf,race_table[iRace].wear_locs[8]  ? " @@aEar@@N"        : " @@dEar@@N"        ,MSL);
 strncat(buf,race_table[iRace].wear_locs[9]  ? " @@aNeck@@N"       : " @@dNeck@@N"       ,MSL);
 strncat(buf,race_table[iRace].wear_locs[10] ? " @@aNeck@@N\r\n"   : " @@dNeck@@N\r\n"   ,MSL);
 strncat(buf,race_table[iRace].wear_locs[11] ? " @@aWings@@N"      : " @@dWings@@N"      ,MSL);
 strncat(buf,race_table[iRace].wear_locs[12] ? " @@aShoulders@@N"  : " @@dShoulders@@N"  ,MSL);
 strncat(buf,race_table[iRace].wear_locs[13] ? " @@aArms@@N"       : " @@dArms@@N"       ,MSL);
 strncat(buf,race_table[iRace].wear_locs[14] ? " @@aWrist@@N"      : " @@dWrist@@N"      ,MSL);
 strncat(buf,race_table[iRace].wear_locs[15] ? " @@aWrist@@N"      : " @@dWrist@@N"      ,MSL);
 strncat(buf,race_table[iRace].wear_locs[16] ? " @@aHands@@N"      : " @@dHands@@N"      ,MSL);
 strncat(buf,race_table[iRace].wear_locs[17] ? " @@aFinger@@N"     : " @@dFinger@@N"     ,MSL);
 strncat(buf,race_table[iRace].wear_locs[18] ? " @@aFinger@@N"     : " @@dFinger@@N"     ,MSL);
 strncat(buf,race_table[iRace].wear_locs[19] ? " @@aClaws@@N\r\n"  : " @@dClaws@@N\r\n"  ,MSL);
 strncat(buf,race_table[iRace].wear_locs[20] ? " @@aLeft Hand@@N"  : " @@dLeft Hand@@N"  ,MSL);
 strncat(buf,race_table[iRace].wear_locs[21] ? " @@aRight Hand@@N" : " @@dRight Hand@@N" ,MSL);
 strncat(buf,race_table[iRace].wear_locs[22] ? " @@aCape@@N"       : " @@dCape@@N"       ,MSL);
 strncat(buf,race_table[iRace].wear_locs[23] ? " @@aWaist@@N"      : " @@dWaist@@N"      ,MSL);
 strncat(buf,race_table[iRace].wear_locs[24] ? " @@aBody@@N"       : " @@dBody@@N"       ,MSL);
 strncat(buf,race_table[iRace].wear_locs[25] ? " @@aTail@@N"       : " @@dTail@@N"       ,MSL);
 strncat(buf,race_table[iRace].wear_locs[26] ? " @@aLegs@@N"       : " @@dLegs@@N"       ,MSL);
 strncat(buf,race_table[iRace].wear_locs[27] ? " @@aFeet@@N"       : " @@dFeet@@N"       ,MSL);
 strncat(buf,race_table[iRace].wear_locs[28] ? " @@aHooves@@N"     : " @@dHooves@@N"     ,MSL);

 return buf;
}

DO_FUN(do_safe)
{
 if( ch->in_room != NULL && ch->in_room->room_flags.test(RFLAG_SAFE) )
  send_to_char("@@GYou are safe from attacks here.@@N\r\n",ch);
 else
  send_to_char("@@RYou are not safe from attacks here.@@N\r\n",ch);
 return;
}

void area_message( AREA_DATA *area, const char *message )
{
 list<CHAR_DATA *>::iterator li;
 CHAR_DATA *ch;

 if( area == NULL )
  return;

 for( li = area->player_list.begin(); li != area->player_list.end(); li++ )
 {
  ch = *li;
  send_to_char(message,ch);
 }

 return;
}

DO_FUN(do_whitelist)
{
 char *farg;
 char arg1[MSL] = {'\0'}, arg2[MSL] = {'\0'};
 short i = 0;

 farg = argument;
 farg = one_argument(farg,arg1);
 farg = one_argument(farg,arg2);

 if( arg1[0] == '\0' )
 {
  ch_printf(ch,"You are limited to [%2d] whitelist sites. Whitelist status: [%14s]\r\n",MAX_HOSTS,ch->act.test(ACT_WHITELIST) ? "@@rACTIVE@@N" : "@@eINACTIVE@@N" );
  send_to_char("---------------------------------------------------------------------\r\n",ch);
  for( i = 0; i < MAX_HOSTS; i++ )
   ch_printf(ch,"  [%2d] %s\r\n",i,ch->pcdata->whitelist[i]);
  return;
 }
 if( !str_prefix(arg1,"add") )
 {
  for( i = 0; i < MAX_HOSTS; i++ )
  {
   if( ch->pcdata->whitelist[i] == &str_empty[0] )
   {
    if( arg2[0] == '\0' || strlen(arg2) < 8 )
    {
     send_to_char("Host must be at least 8 characters long.\r\n",ch);
     return;
    }
    ch->pcdata->whitelist[i] = str_dup(arg2);
    send_to_char("Whitelist updated.\r\n",ch);
    return;
   }
  }
  send_to_char("Your whitelist is full. Please delete an entry first.\r\n",ch);
  return;
 }
 else if( !str_prefix(arg1,"del") )
 {
  if( arg2[0] == '\0' )
  {
   send_to_char("You must supply a host to delete.\r\n",ch);
   return;
  }
  for( i = 0; i < MAX_HOSTS; i++ )
  {
   if( !str_cmp(arg2,ch->pcdata->whitelist[i]) )
   {
    ch_printf(ch,"Host %s has been removed from your whitelist.\r\n",ch->pcdata->whitelist[i]);
    free_string(ch->pcdata->whitelist[i]);
    ch->pcdata->whitelist[i] = &str_empty[0];
    return;
   }
  }
  send_to_char("That host was not found. Did you spell it exactly as it is listed in your whitelist?\r\n",ch);
  return;
 }
 else
  do_whitelist(ch);

 return;
}

DO_FUN(do_logins)
{
 if( IS_NPC(ch) )
  return;

 ch_printf(ch,"Last [%2d] successful logins:\r\n",MAX_HOSTS);
 for( short i = 0; i < MAX_HOSTS; i++ )
  ch_printf(ch,"  [%2d] %s\r\n",i,ch->pcdata->host[i]);

 return;
}
