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

#ifndef DEC_MACROS_H
#include "h/macros.h"
#endif

#ifndef DEC_MAGIC_H
#include "h/magic.h"
#endif

#ifndef DEC_MONEY_H
#include "h/money.h"
#endif

#ifndef DEC_QUEST_H
#include "h/quest.h"
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

#ifndef DEC_TRIGGER_H
#include "h/trigger.h"
#endif

#ifndef DEC_UPDATE_H
#include "h/update.h"
#endif

extern OBJ_DATA *quest_object;
extern OBJ_DATA *auction_item;
extern CHAR_DATA *auction_owner;
extern CHAR_DATA *auction_bidder;
extern int auction_bid;
extern int auction_stage;
extern bool auction_flop;

void get_obj( CHAR_DATA * ch, OBJ_DATA * obj, OBJ_DATA * container )
{

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   if( !CAN_WEAR( obj, ITEM_TAKE ) || obj->item_type == ITEM_CORPSE_PC || obj->extra_flags.test(ITEM_EXTRA_NO_TAKE) )
   {
/*	send_to_char( "You can't take that.\r\n", ch );  */
      return;
   }

   if( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
   {
      act( "$d: you can't carry that many items.", ch, NULL, obj->name, TO_CHAR );
      return;
   }

   if( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
   {
      act( "$d: you can't carry that much weight.", ch, NULL, obj->name, TO_CHAR );
      return;
   }

   if( container != NULL )
   {
      act( "You get $p from $P.", ch, obj, container, TO_CHAR );
      act( "$n gets $p from $P.", ch, obj, container, TO_ROOM );
      obj_from_obj( obj );
   }
   else
   {
      act( "You get $p.", ch, obj, container, TO_CHAR );
      act( "$n gets $p.", ch, obj, container, TO_ROOM );
      obj_from_room( obj );
   }

   if( obj->item_type == ITEM_MONEY )
   {
      join_money(ch->money,round_money(obj->value[0],true));
      extract_obj( obj );
   }
   else
   {
      obj_to_char( obj, ch );
      trigger_handler( ch, obj, TRIGGER_GET );
   }

   return;
}



DO_FUN(do_get)
{

   char container_name[MSL];
   char object_list[MSL];
   char one_object[MSL];
   char object_number[MSL];
   char victim_name[MSL];
   char actbuf[MSL];

   OBJ_DATA *obj;
   OBJ_DATA *obj_next;
   OBJ_DATA *container = NULL;
   CHAR_DATA *victim;
   bool found = FALSE;
   bool get_ok = FALSE;
   bool found_money = FALSE;
   victim_name[0] = '\0';
   object_list[0] = '\0';

/* hack to take care of players used to old system */
   if( ( !is_name( "from", argument ) ) && ( is_name( "corpse", argument ) ) )
   {
      snprintf( container_name, MSL, "%s", "corpse" );
      if( is_name( "all", argument ) )
         snprintf( object_list, MSL, "1 %s", "all" );
      else if( ( is_name( "coins", argument ) ) || ( is_name( "coin", argument ) ) || ( is_name( "money", argument ) ) )
         snprintf( object_list, MSL, "%s", "coinsonly" );
   }
   else
   {
      pre_parse( argument, victim_name, container_name, object_list );
   }

   /*
    * Get type. 
    */
   if( object_list[0] == '\0' )
   {
      send_to_char( "Get what _from_ what?\r\n", ch );
      return;
   }
   if( ( container_name[0] != '\0' ) && ( container = get_obj_here( ch, container_name ) ) == NULL )
   {
      act( "I see no $T here.", ch, NULL, container_name, TO_CHAR );
      return;
   }

   if( ( container != NULL ) && ( container->item_type == ITEM_CORPSE_PC ) )
   {
      char name[MSL];
      CHAR_DATA *gch;
      list<CHAR_DATA *>::iterator li;
      char *pd;
      if( IS_NPC( ch ) )
      {
         send_to_char( "You can't do that.\r\n", ch );
         return;
      }
      pd = container->short_descr;
      pd = one_argument( pd, name );
      pd = one_argument( pd, name );
      pd = one_argument( pd, name );
      if( str_cmp( name, ch->name ) && !IS_IMMORTAL( ch ) )
      {
         bool fGroup = FALSE;
         victim = NULL;
         for( li = char_list.begin(); li != char_list.end(); li++ )
         {
            gch = *li;
            if( !IS_NPC( gch ) && !str_cmp( name, gch->name ) )
            {
               victim = gch;
               break;
            }
         }

         /*
          * if (victim !=NULL && is_same_group(ch, gch))
          * fGroup = TRUE;   
          */

         if( !fGroup )
         {
            if( victim == NULL
                || !victim->act.test(ACT_PKOK) || !ch->act.test(ACT_PKOK) )
               send_to_char( "You can't do that.\r\n", ch );
            return;
         }
      }
   }
   if( container != NULL )
   {
      if( IS_SET( container->value[1], CONT_CLOSED ) )
      {
         act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );
         return;
      }
      if( ( !str_prefix( "1 all", object_list ) ) || ( !str_cmp( "coinsonly", object_list ) ) )
      {
         /*
          * 'get all container' or 'get all.obj container' 
          */
         found = FALSE;
         if( str_cmp( "coinsonly", object_list ) )
            for( obj = container->first_in_carry_list; obj != NULL; obj = obj_next )
            {
               obj_next = obj->next_in_carry_list;
               if( can_see_obj( ch, obj ) )
               {
                  found = TRUE;
                  get_obj( ch, obj, container );
               }
            }
         found_money = get_money_obj( ch, "all", container );
         if( !found && !found_money )
         {
            act( "There is nothing at all in the $T.", ch, NULL, container_name, TO_CHAR );
         }
         else
         {
            if( container->item_type == ITEM_CORPSE_PC )
               save_corpses(  );
         }
         if( ch->level > 1 )
            do_save( ch, "auto" );
         return;
      }
      else
      {
         char *parse;
         parse = object_list;
         snprintf( bug_buf, (2 * MIL), "Get %s, object_list: %s, container %s", argument, object_list, container_name );
         monitor_chan( bug_buf, MONITOR_DEBUG );

         for( ;; )   /* 'get obj corpse' */
         {
            short looper;

            parse = one_argument( parse, object_number );
            if( object_number[0] == '\0' )
               break;
            parse = one_argument( parse, one_object );
            if( one_object[0] == '\0' )
               break;
            snprintf( bug_buf, (2 * MIL), "Looking for %s %s", object_number, one_object );
            monitor_chan( bug_buf, MONITOR_DEBUG );
            for( looper = 0; looper < ( is_number( object_number ) ? atoi( object_number ) : 0 ); looper++ )
            {
               obj = get_obj_list( ch, one_object, container->first_in_carry_list );
               if( ( obj == NULL ) || ( !can_see_obj( ch, obj ) ) )
               {
                  snprintf( actbuf, MSL, "I see no more %s in the %s.", one_object, container_name );
                  act( actbuf, ch, NULL, NULL, TO_CHAR );
                  break;
               }
               get_obj( ch, obj, container );
            }
         }
         if( ch->level > 1 )
            do_save( ch, "auto" );


         return;
      }
   }
   else  /* getting from room */
   {
      if( !str_prefix( "1 all", object_list ) )
      {  /* 'get all' or 'get all.obj' */
         found = FALSE;
         get_ok = get_money_room( ch, "all" );
         if( get_ok )
            found = TRUE;
         for( obj = ch->in_room->first_content; obj != NULL; obj = obj_next )
         {
            obj_next = obj->next_in_room;
            if( can_see_obj( ch, obj ) )
            {
               found = TRUE;
               get_obj( ch, obj, NULL );
            }
         }
         if( found && ch->level > 1 )
            do_save( ch, "auto" );

         if( !found && !get_ok )
         {
            send_to_char( "I see nothing here.\r\n", ch );
         }
         return;
      }
      else
      {
         bool found_one_obj = FALSE;
         char *parse;
         parse = object_list;

         for( ;; )
         {
            short looper;
            snprintf( bug_buf, (2 * MIL), "In get thing room, parse is %s.", parse );
            monitor_chan( bug_buf, MONITOR_DEBUG );
            parse = one_argument( parse, object_number );
            if( object_number[0] == '\0' )
               break;
            parse = one_argument( parse, one_object );
            if( one_object[0] == '\0' )
               break;
            snprintf( bug_buf, (2 * MIL), "Looking for %s %s.", object_number, one_object );
            monitor_chan( bug_buf, MONITOR_DEBUG );
            for( looper = 0; looper < atoi( object_number ); looper++ )
            {
               obj = get_obj_room( ch, one_object, ch->in_room->first_content );
               if( obj == NULL )
               {
                  if( found_one_obj )
                  {
                     snprintf( actbuf, MSL, "There isn't another %s here.", one_object );
                     act( actbuf, ch, NULL, NULL, TO_CHAR );
                     break;
                  }
                  else if( is_name( one_object, CURRENCY_NAMES ) )
                  {
                     char moneybuf[MSL];
                     snprintf( moneybuf, MSL, "%s %s", object_number, one_object );
                     get_ok = get_money_room( ch, moneybuf );
                     if( get_ok )
                     {
                        snprintf( actbuf, MSL, "$n gets %s %s.", object_number, one_object );
                        act( actbuf, ch, NULL, NULL, TO_ROOM );
                        snprintf( actbuf, MSL, "You get %s %s.", object_number, one_object );
                        act( actbuf, ch, NULL, NULL, TO_CHAR );
                        break;
                     }
                     else
                     {
                        snprintf( actbuf, MSL, "There aren't %s %s here!", object_number, one_object );
                        act( actbuf, ch, NULL, NULL, TO_CHAR );
                        break;
                     }
                  }
               }

               else if( !can_see_obj( ch, obj ) )
               {
                  snprintf( actbuf, MSL, "I see no more %s here.", one_object );
                  act( actbuf, ch, NULL, NULL, TO_CHAR );
                  break;
               }
               else
               {
                  get_obj( ch, obj, NULL );
                  found_one_obj = TRUE;
               }
            }
         }
         if( ( found == FALSE ) && ( found_one_obj == FALSE ) && ( found_money == FALSE ) && ( get_ok == FALSE ) )
         {
            send_to_char( "I don't see what your looking for in this room.\r\n", ch );
            return;
         }
         if( ch->level > 1 )
            do_save( ch, "auto" );


         return;
      }
   }
   if( ch->level > 1 )
      do_save( ch, "auto" );


   return;
}




DO_FUN(do_put)
{
   char object_list[MAX_INPUT_LENGTH];
   char container_name[MSL];
   char victim_name[MSL];
   char actbuf[MSL];
   OBJ_DATA *container_obj;
   OBJ_DATA *obj;


   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }
   pre_parse( argument, victim_name, container_name, object_list );


   if( object_list[0] == '\0' )
   {
      send_to_char( "Drop what?\r\n", ch );
      return;
   }
   if( container_name[0] == '\0' )
   {
      send_to_char( "What container?\r\n", ch );
      return;
   }
   else if( ( container_obj = get_obj_here( ch, container_name ) ) == NULL )
   {
      act( "I see no $T here.", ch, NULL, container_name, TO_CHAR );
      return;
   }
   if( ( container_obj->item_type != ITEM_CONTAINER ) && ( container_obj->item_type != ITEM_SPELL_MATRIX ) )
   {
      send_to_char( "That's not a container.\r\n", ch );
      return;
   }
   if( IS_SET( container_obj->value[1], CONT_CLOSED ) )
   {
      act( "The $d is closed.", ch, NULL, container_obj->name, TO_CHAR );
      return;
   }



   if( !str_prefix( "1 all", object_list ) || !str_prefix( "all.", object_list ) )
   {
      send_to_char( "You must list the items you wish to put into your container!\r\n", ch );
      return;
   }
   else
   {
      char *parse = object_list;
      char object_number[MSL];
      char one_object[MSL];
      for( ;; )
      {
         short looper;
         snprintf( bug_buf, (2 * MIL), "In put thing container, parse is %s.", parse );
         monitor_chan( bug_buf, MONITOR_DEBUG );
         parse = one_argument( parse, object_number );
         if( object_number[0] == '\0' )
            break;
         parse = one_argument( parse, one_object );
         if( one_object[0] == '\0' )
            break;
         snprintf( bug_buf, (2 * MIL), "Looking for %s %s.", object_number, one_object );
         monitor_chan( bug_buf, MONITOR_DEBUG );
         for( looper = 0; looper < atoi( object_number ); looper++ )
         {
            obj = get_obj_carry( ch, one_object );
            if( obj == NULL )
            {
               if( is_name( one_object, CURRENCY_NAMES ) )
               {
                  send_to_char( "You cannot put money into containers, yet!\r\n", ch );
                  return;
               }
               break;
            }
            if( !can_drop_obj( ch, obj ) )
            {
               act( "You can't let go of $d.", ch, NULL, obj->name, TO_CHAR );

               if( looper < 10 )
               {
                  continue;
               }
               else
               {
                  break;
               }
            }
            if( obj == container_obj )
            {
               send_to_char( "You can't fold it into itself.\r\n", ch );
               continue;
            }
            if( get_obj_weight( obj ) + get_obj_weight( container_obj ) > container_obj->value[0] )
            {
               act( "$p won't fit into $P.", ch, obj, container_obj, TO_CHAR );
               continue;
            }
            if( container_obj->item_type == ITEM_SPELL_MATRIX )
            {
               if( ( obj->item_type == ITEM_WEAPON )
                   || ( obj->item_type == ITEM_LIGHT ) || ( obj->item_type == ITEM_ARMOR ) )
               {
                  OBJ_DATA *one_obj;
                  bool has_item = FALSE;
                  for( one_obj = container_obj->first_in_carry_list; one_obj != NULL; one_obj = one_obj->next_in_carry_list )
                  {
                     if( one_obj->item_type != ITEM_ENCHANTMENT )
                     {
                        has_item = TRUE;
                        break;
                     }
                  }
                  if( has_item )
                  {
                     send_to_char( "Spell Matrix containers may hold only 1 non-enchantment item.\r\n", ch );
                     return;
                  }
               }
               else if( obj->item_type != ITEM_ENCHANTMENT )
               {
                  send_to_char( "Spell Matrix containers may hold only 1 non-enchantment item.\r\n", ch );
                  return;
               }
            }

            obj_from_char( obj );
            obj_to_obj( obj, container_obj );
            snprintf( actbuf, MSL, "$n puts $p into $P%s.", ( ( container_obj->carried_by != NULL ) ? "" : " ( in the room )" ) );
            act( actbuf, ch, obj, container_obj, TO_ROOM );
            snprintf( actbuf, MSL, "You put $p into $P%s.", ( ( container_obj->carried_by != NULL ) ? "" : " ( in the room )" ) );

            act( actbuf, ch, obj, container_obj, TO_CHAR );
            /*
             * trigger_handler( ch, obj, TRIGGER_DROP );   
             */
         }
      }
      if( ch->level > 2 )
         do_save( ch, "auto" );
      return;
   }

   return;
}




DO_FUN(do_drop)
{
   char arg[MAX_INPUT_LENGTH];
   char container_name[MSL];
   char victim_name[MSL];
   char object_list[MSL];
   OBJ_DATA *obj;
   OBJ_DATA *obj_next;
   bool found = FALSE;
   char object_number[MSL];
   char one_object[MSL];
   char actbuf[MSL];

   victim_name[0] = '\0';
   pre_parse( argument, victim_name, container_name, object_list );


   if( object_list[0] == '\0' )
   {
      send_to_char( "Drop what?\r\n", ch );
      return;
   }

   if( str_prefix( "1 all", object_list ) && str_prefix( "all.", arg ) )
   {
      char *parse;
      parse = object_list;
      for( ;; )
      {
         short looper;
         snprintf( bug_buf, (2 * MIL), "In drop thing room, parse is %s.", parse );
         monitor_chan( bug_buf, MONITOR_DEBUG );
         parse = one_argument( parse, object_number );
         if( object_number[0] == '\0' )
            break;
         parse = one_argument( parse, one_object );
         if( one_object[0] == '\0' )
            break;
         snprintf( bug_buf, (2 * MIL), "Looking for %s %s.", object_number, one_object );
         monitor_chan( bug_buf, MONITOR_DEBUG );
         for( looper = 0; looper < atoi( object_number ); looper++ )
         {
            obj = get_obj_carry( ch, one_object );
            if( obj == NULL )
            {
               if( is_name( one_object, CURRENCY_NAMES ) )
               {
                  char moneybuf[MSL];
                  snprintf( moneybuf, MSL, "%s %s", object_number, one_object );
                  snprintf( actbuf, MSL, "$n drops %s %s.", object_number, one_object );
                  act( actbuf, ch, NULL, NULL, TO_ROOM );
                  snprintf( actbuf, MSL, "You drop %s %s.", object_number, one_object );
                  act( actbuf, ch, NULL, NULL, TO_CHAR );
                  drop_money( ch, moneybuf );
               }
               break;
            }
            if( !can_drop_obj( ch, obj ) )
            {
               send_to_char( "You can't let go of it.\r\n", ch );
               if( looper < 10 )
               {
                  continue;
               }
               else
               {
                  break;
               }
            }
            obj_from_char( obj );
            obj_to_room( obj, ch->in_room );
            act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
            act( "You drop $p.", ch, obj, NULL, TO_CHAR );
            /*
             * trigger_handler( ch, obj, TRIGGER_DROP );   
             */
         }
      }
      if( ch->level > 1 )
         do_save( ch, "auto" );
      return;
   }

   /*
    * 'drop all' or 'drop all.obj' 
    */
   found = FALSE;
   for( obj = ch->first_carry; obj != NULL; obj = obj_next )
   {
      obj_next = obj->next_in_carry_list;

      if( can_see_obj( ch, obj ) && obj->wear_loc == WEAR_NONE && can_drop_obj( ch, obj ) )
      {
         found = TRUE;
         obj_from_char( obj );
         obj_to_room( obj, ch->in_room );
         act( "You drop $p.", ch, obj, NULL, TO_CHAR );
         act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
         /*
          * trigger_handler( ch, obj, TRIGGER_DROP );   
          */
      }
   }

   if( !found )
   {
      act( "You are not carrying anything.", ch, NULL, arg, TO_CHAR );
   }

   if( found && ch->level > 1 )
      do_save( ch, "auto" );
   return;
}





DO_FUN(do_give)
{
   char victim_name[MSL];
   char container_name[MSL];
   char object_list[MSL];
   char one_object[MSL];
   char object_number[MSL];
   char buf[MAX_STRING_LENGTH];
   char actbuf[MSL];
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   extern bool quest;
   extern bool auto_quest;
   extern CHAR_DATA *quest_mob;

   container_name[0] = '\0';
   pre_parse( argument, victim_name, container_name, object_list );

   if( object_list[0] == '\0' || victim_name[0] == '\0' )
   {
      send_to_char( "Give what to whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, victim_name ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_GHOST(victim) )
   {
    send_to_char("You can't seem to trade with the dead...\r\n",ch);
    return;
   }

   if( str_prefix( "1 all", object_list ) && str_prefix( "all.", object_list ) )
   {
      char *parse;
      parse = object_list;
      for( ;; )
      {
         short looper;
         snprintf( bug_buf, (2 * MIL), "In give thing, parse is %s.", parse );
         monitor_chan( bug_buf, MONITOR_DEBUG );
         parse = one_argument( parse, object_number );
         if( object_number[0] == '\0' )
            break;
         parse = one_argument( parse, one_object );
         if( one_object[0] == '\0' )
            break;
         snprintf( bug_buf, (2 * MIL), "Looking for %s %s.", object_number, one_object );
         monitor_chan( bug_buf, MONITOR_DEBUG );
         for( looper = 0; looper < atoi( object_number ); looper++ )
         {
            obj = get_obj_carry( ch, one_object );
            if( obj == NULL )
            {
               if( is_name( one_object, CURRENCY_NAMES ) )
               {
                  char moneybuf[MSL];
                  bool give_ok = FALSE;
                  snprintf( moneybuf, MSL, "%s %s", object_number, one_object );
                  give_ok = give_money( ch, victim, moneybuf );
                  if( give_ok )
                  {
                     snprintf( actbuf, MSL, "$n gives %s %s to $N.", object_number, one_object );
                     act( actbuf, ch, NULL, victim, TO_ROOM );
                     snprintf( actbuf, MSL, "$N gives you %s %s.", object_number, one_object );
                     act( actbuf, victim, NULL, ch, TO_CHAR );
                     snprintf( actbuf, MSL, "You give %s %s to $N.", object_number, one_object );
                     act( actbuf, ch, NULL, victim, TO_CHAR );
                  }
               }
               break;
            }
            if( obj->wear_loc != WEAR_NONE )
            {
               send_to_char( "You must remove it first.\r\n", ch );
               continue;
            }
            if( !can_drop_obj( ch, obj ) )
            {
               send_to_char( "You can't let go of it.\r\n", ch );
               if( looper < 10 )
               {
                  continue;
               }
               else
               {
                  break;
               }
            }
            if( victim->carry_number + get_obj_number( obj ) > can_carry_n( victim ) )
            {
               act( "$N has $S hands full.", ch, NULL, victim, TO_CHAR );
               continue;
            }

            if( victim->carry_weight + get_obj_weight( obj ) > can_carry_w( victim ) )
            {
               act( "$N can't carry that much weight.", ch, NULL, victim, TO_CHAR );
               continue;
            }

            if( !can_see_obj( victim, obj ) )
            {
               act( "$N can't see it.", ch, NULL, victim, TO_CHAR );
               continue;
            }
            obj_from_char( obj );
            obj_to_char( obj, victim );
            act( "$n gives $p to $N.", ch, obj, victim, TO_NOTVICT );
            act( "You give $p to $N.", ch, obj, victim, TO_CHAR );
            act( "$n gives you $p.", ch, obj, victim, TO_VICT );
            /*
             * trigger_handler( ch, obj, TRIGGER_DROP );
             */

            if( ( quest || auto_quest ) && IS_NPC( victim ) && victim == quest_mob && obj == quest_object )
            {
               /*
                * Then ch has recovered the quest object!!!!!
                */
               snprintf( buf, MSL, "Oh! %s you found %s for me!  Thank You!", ch->get_name(), obj->short_descr );
               do_say( victim, buf );
               interpret( victim, "hop" );

               if( !IS_NPC( ch ) )
               {
                  snprintf( buf, MSL, "%s I shall reward you well for recovering this for me!", ch->get_name() );
                  do_tell( victim, buf );
                  crusade_reward(ch);
               }
               quest_complete( ch );
            }
         }
      }
   }
   if( ch->level > 1 )
      do_save( victim, "auto" );


   return;

}


DO_FUN(do_fill)
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   OBJ_DATA *fountain;
   bool found;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Fill what?\r\n", ch );
      return;
   }

   if( ( obj = get_obj_carry( ch, arg ) ) == NULL )
   {
      send_to_char( "You do not have that item.\r\n", ch );
      return;
   }

   found = FALSE;
   for( fountain = ch->in_room->first_content; fountain != NULL; fountain = fountain->next_in_room )
   {
      if( fountain->item_type == ITEM_FOUNTAIN )
      {
         found = TRUE;
         break;
      }
   }

   if( !found )
   {
      send_to_char( "There is no fountain here!\r\n", ch );
      return;
   }

   if( obj->item_type != ITEM_DRINK_CON )
   {
      send_to_char( "You can't fill that.\r\n", ch );
      return;
   }

   if( obj->value[1] != 0 && obj->value[2] != 0 )
   {
      send_to_char( "There is already another liquid in it.\r\n", ch );
      return;
   }

   if( obj->value[1] >= obj->value[0] )
   {
      send_to_char( "Your container is full.\r\n", ch );
      return;
   }

   act( "You fill $p.", ch, obj, NULL, TO_CHAR );
   obj->value[2] = fountain->value[0];
   obj->value[1] = obj->value[0];
   return;
}



DO_FUN(do_drink)
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int amount;
   int liquid;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      for( obj = ch->in_room->first_content; obj; obj = obj->next_in_room )
      {
         if( obj->item_type == ITEM_FOUNTAIN )
            break;
      }

      if( obj == NULL )
      {
         send_to_char( "Drink what?\r\n", ch );
         return;
      }
   }
   else
   {
      if( ( obj = get_obj_here( ch, arg ) ) == NULL )
      {
         send_to_char( "You can't find it.\r\n", ch );
         return;
      }
   }

   if( ch->get_level("psuedo") < obj->level && !IS_IMMORTAL(ch) )
   {
      send_to_char( "You are not knowledgeable enough to drink such a liquid.\n\r", ch );
      return;
   }

   if( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 40 )
   {
      send_to_char( "You do not feel thirsty.\r\n", ch );
      return;
   }



   if( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 10 )
   {
      send_to_char( "You fail to reach your mouth.  *Hic*\r\n", ch );
      return;
   }

   switch ( obj->item_type )
   {
      default:
         send_to_char( "You can't drink from that.\r\n", ch );
         break;

      case ITEM_FOUNTAIN:
         if( ( liquid = obj->value[0] ) >= LIQ_MAX )
         {
            bug( "Do_drink: bad liquid number %d.", liquid );
            liquid = obj->value[0] = 0;
         }

         act( "$n drinks $T from $p.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
         act( "You drink $T from $p.", ch, obj, liq_table[liquid].liq_name, TO_CHAR );

         amount = number_range( 1, 3 );

         if( liquid == LIQUID_BLOOD && IS_VAMP( ch ) && !IS_NPC( ch ) )
         {
            send_to_char( "Ahhhh! Blood!\r\n", ch );
            bloodlust_gain( ch, amount + 2 );
         }


         condition_gain( ch, COND_DRUNK, amount * liq_table[liquid].liq_affect[COND_DRUNK] );
         condition_gain( ch, COND_FULL, amount * liq_table[liquid].liq_affect[COND_FULL] );
         condition_gain( ch, COND_THIRST, amount * liq_table[liquid].liq_affect[COND_THIRST] );

         if( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 10 )
            send_to_char( "You feel drunk.\r\n", ch );
         if( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL] > 40 )
            send_to_char( "You are full.\r\n", ch );
         if( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 40 )
            send_to_char( "You do not feel thirsty.\r\n", ch );

         if( obj->value[1] != 0 )
         {
            /*
             * The shit was poisoned! 
             */
            AFFECT_DATA af;

            act( "$n chokes and gags.", ch, 0, 0, TO_ROOM );
            send_to_char( "You choke and gag.\r\n", ch );

            af.type = gsn_poison;
            af.duration = 2;
            af.location = APPLY_NONE;
            af.modifier = 0;
            af.bitvector = AFF_POISON;
            affect_join( ch, &af );
         }
         break;

      case ITEM_DRINK_CON:
         if( obj->value[1] <= 0 )
         {
            send_to_char( "It is already empty.\r\n", ch );
            return;
         }

         if( ( liquid = obj->value[2] ) >= LIQ_MAX )
         {
            bug( "Do_drink: bad liquid number %d.", liquid );
            liquid = obj->value[2] = 0;
         }

         act( "$n drinks $T from $p.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
         act( "You drink $T from $p.", ch, obj, liq_table[liquid].liq_name, TO_CHAR );

         amount = number_range( 3, 10 );
         amount = UMIN( amount, obj->value[1] );

         if( liquid == LIQUID_BLOOD && IS_VAMP( ch ) && !IS_NPC( ch ) )
         {
            send_to_char( "@@eAhh, only if it was fresh, would taste much better!@@N\r\n", ch );
            bloodlust_gain( ch, 2 );
         }

         condition_gain( ch, COND_DRUNK, amount * liq_table[liquid].liq_affect[COND_DRUNK] );
         condition_gain( ch, COND_FULL, amount * liq_table[liquid].liq_affect[COND_FULL] );
         condition_gain( ch, COND_THIRST, amount * liq_table[liquid].liq_affect[COND_THIRST] );

         if( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 10 )
            send_to_char( "You feel drunk.\r\n", ch );
         if( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL] > 40 )
            send_to_char( "You are full.\r\n", ch );
         if( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 40 )
            send_to_char( "You do not feel thirsty.\r\n", ch );

         if( obj->value[3] != 0 )
         {
            /*
             * The shit was poisoned ! 
             */
            AFFECT_DATA af;

            act( "$n chokes and gags.", ch, NULL, NULL, TO_ROOM );
            send_to_char( "You choke and gag.\r\n", ch );
            af.type = gsn_poison;
            af.duration = 3 * amount;
            af.location = APPLY_NONE;
            af.modifier = 0;
            af.bitvector = AFF_POISON;
            affect_join( ch, &af );
         }

         obj->value[1] -= amount;
         if( obj->value[1] <= 0 )
         {
            obj->value[1] = 0;
/*
	    send_to_char( "The empty container vanishes.\r\n", ch );
	    extract_obj( obj );  */
         }
         break;
   }

   return;
}



DO_FUN(do_eat)
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Eat what?\r\n", ch );
      return;
   }

   if( ( obj = get_obj_carry( ch, arg ) ) == NULL )
   {
      send_to_char( "You do not have that item.\r\n", ch );
      return;
   }

   if( ch->get_level("psuedo") < obj->level && !IS_IMMORTAL(ch) )
   {
      send_to_char( "You are not knowledgeable enough to eat such a food.\n\r", ch );
      return;
   }

   if( !IS_IMMORTAL( ch ) )
   {
      if( obj->item_type != ITEM_FOOD && obj->item_type != ITEM_PILL )
      {
         send_to_char( "That's not edible.\r\n", ch );
         return;
      }

      if( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL] > 40 )
      {
         send_to_char( "You are too full to eat more.\r\n", ch );
         return;
      }
   }

   act( "$n eats $p.", ch, obj, NULL, TO_ROOM );
   act( "You eat $p.", ch, obj, NULL, TO_CHAR );

   switch ( obj->item_type )
   {

      case ITEM_FOOD:
         if( !IS_NPC( ch ) )
         {
            int condition;

            condition = ch->pcdata->condition[COND_FULL];
            condition_gain( ch, COND_FULL, obj->value[0] );
            if( condition == 0 && ch->pcdata->condition[COND_FULL] > 0 )
               send_to_char( "You are no longer hungry.\r\n", ch );
            else if( ch->pcdata->condition[COND_FULL] > 40 )
               send_to_char( "You are full.\r\n", ch );
         }

         if( obj->value[3] != 0 )
         {
            /*
             * The shit was poisoned! 
             */
            AFFECT_DATA af;

            act( "$n chokes and gags.", ch, 0, 0, TO_ROOM );
            send_to_char( "You choke and gag.\r\n", ch );

            af.type = gsn_poison;
            af.duration = 2 * obj->value[0];
            af.location = APPLY_NONE;
            af.modifier = 0;
            af.bitvector = AFF_POISON;
            affect_join( ch, &af );
         }
         break;

      case ITEM_PILL:
         obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
         obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
         obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );
         break;
   }

   extract_obj( obj );
   return;
}



/*
 * Remove an object.
 */
bool remove_obj( CHAR_DATA * ch, int iWear, bool fReplace )
{
   OBJ_DATA *obj;
   AFFECT_DATA *aff;
   short hp_mod = 0;
   if( ( obj = get_eq_char( ch, iWear ) ) == NULL )
      return TRUE;

   if( !fReplace )
      return FALSE;

   if( IS_OBJ_STAT(obj,ITEM_EXTRA_NO_REMOVE) )
   {
      act( "You can't remove $p.", ch, obj, NULL, TO_CHAR );
      return FALSE;
   }

// check for hitpoints low, and hp enhancing eq
   for( aff = obj->first_apply; aff; aff = aff->next )
   {
      if( aff->location == APPLY_HIT )
         hp_mod += aff->modifier;
   }

   if( ( hp_mod >= ch->max_hit ) || ( hp_mod >= ch->hit ) )
   {
      send_to_char( "That is the only thing keeping you alive!\r\n", ch );
      return FALSE;
   }
   unequip_char( ch, obj );
   act( "$n stops using $p.", ch, obj, NULL, TO_ROOM );
   act( "You stop using $p.", ch, obj, NULL, TO_CHAR );
   /*
    * trigger_handler( ch, obj, TRIGGER_REMOVE );  
    */
   return TRUE;
}


/* MAG
   can_wear_at(ch,obj,location) returns true if the char can wear the object
   at the location.
   */

bool can_wear_at( CHAR_DATA * ch, OBJ_DATA * obj, int location )
{
   int loc_flag;

   switch ( location )
   {
      case WEAR_NONE:
         loc_flag = 0;
         break;
      case WEAR_LIGHT:
         loc_flag = ITEM_WEAR_HOLD_HAND;
         break;
      case WEAR_FINGER_L:
      case WEAR_FINGER_R:
         loc_flag = ITEM_WEAR_FINGER;
         break;
      case WEAR_NECK_1:
      case WEAR_NECK_2:
         loc_flag = ITEM_WEAR_NECK;
         break;
      case WEAR_BODY:
         loc_flag = ITEM_WEAR_BODY;
         break;
      case WEAR_HEAD:
         loc_flag = ITEM_WEAR_HEAD;
         break;
      case WEAR_LEGS:
         loc_flag = ITEM_WEAR_LEGS;
         break;
      case WEAR_FEET:
         loc_flag = ITEM_WEAR_FEET;
         break;
      case WEAR_HANDS:
         loc_flag = ITEM_WEAR_HANDS;
         break;
      case WEAR_ARMS:
         loc_flag = ITEM_WEAR_ARMS;
         break;
      case WEAR_TAIL:
         loc_flag = ITEM_WEAR_TAIL;
         break;
      case WEAR_ABOUT:
         loc_flag = ITEM_WEAR_ABOUT;
         break;
      case WEAR_WAIST:
         loc_flag = ITEM_WEAR_WAIST;
         break;
      case WEAR_WRIST_L:
      case WEAR_WRIST_R:
         loc_flag = ITEM_WEAR_WRIST;
         break;
      case WEAR_HORNS:
         loc_flag = ITEM_WEAR_HORNS;
         break;
      case WEAR_BEAK:
         loc_flag = ITEM_WEAR_BEAK;
         break;
      case WEAR_FACE:
         loc_flag = ITEM_WEAR_FACE;
         break;
      case WEAR_EAR_L:
      case WEAR_EAR_R:
         loc_flag = ITEM_WEAR_EAR;
         break;
      case WEAR_HOOVES:
         loc_flag = ITEM_WEAR_HOOVES;
         break;
      case WEAR_AURA:
         loc_flag = ITEM_WEAR_AURA;
         break;
      case WEAR_HALO:
         loc_flag = ITEM_WEAR_HALO;
         break;
      case WEAR_WINGS:
         loc_flag = ITEM_WEAR_WINGS;
         break;
      case WEAR_SHOULDERS:
         loc_flag = ITEM_WEAR_SHOULDERS;
         break;
      case WEAR_CLAWS:
         loc_flag = ITEM_WEAR_CLAWS;
         break;
      case WEAR_HOLD_HAND_L:
      case WEAR_HOLD_HAND_R:
         loc_flag = ITEM_WEAR_HOLD_HAND;
         break;

      default:
         loc_flag = 0;
         break;
   }

   if( race_table[URANGE( 0, ch->race, MAX_RACE )].wear_locs[location] == FALSE )
   {
      act( "You don't have anywhere to wear $p!", ch, obj, NULL, TO_CHAR );
      act( "$L$n fumbles about with $p, looking for a place to wear it.", ch, obj, NULL, TO_ROOM );
      return FALSE;
   }

   if( !obj->wear_flags.test(loc_flag) && ( obj->wear_loc == -1 ) )
   {
      send_to_char( "That item can't be worn there!\r\n", ch );
      return FALSE;
   }


   return TRUE;
}


/*
 * Wear one object.
 * Optional replacement of existing objects.
 * Big repetitive code, ick.
 */
void wear_obj( CHAR_DATA * ch, OBJ_DATA * obj, bool fReplace )
{
   char buf[MSL];

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   if( ch->get_level("psuedo") < obj->level )
   {
      snprintf( buf, MSL, "You must be level %d to use this object.\r\n", obj->level );
      send_to_char( buf, ch );
      act( "$n tries to use $p, but is too inexperienced.", ch, obj, NULL, TO_ROOM );
      return;
   }

   if( ( ch->get_level("psuedo") < obj->level )
       && ( IS_OBJ_STAT( obj, ITEM_EXTRA_VAMP ) ) && ( IS_VAMP( ch ) ) && ( !IS_NPC( ch ) ) )
   {
      snprintf( buf, MSL, "You must be level %d to use this object.\r\n", obj->level );
      send_to_char( buf, ch );
      act( "$n tries to use $p, but is too inexperienced.", ch, obj, NULL, TO_ROOM );
      return;
   }

   if( ( IS_OBJ_STAT( obj, ITEM_EXTRA_VAMP ) ) && ( !IS_VAMP( ch ) ) && ( !IS_NPC( ch ) ) )
   {
      snprintf( buf, MSL, "You must be a vampire to use this object." );
      send_to_char( buf, ch );
      act( "$n tries to use $p but is stopped by some unknown force.", ch, obj, NULL, TO_ROOM );
      return;
   }

   if( ( ch->get_level("maxremortal") < obj->level ) && ( IS_OBJ_STAT( obj, ITEM_EXTRA_REMORT ) ) && ( !IS_NPC( ch ) ) )
   {
      snprintf( buf, MSL, "You must be level %d in a remort class to use this object.\r\n", obj->level );
      send_to_char( buf, ch );
      act( "$n tries to use $p, but is too inexperienced.", ch, obj, NULL, TO_ROOM );
      return;
   }

   if( obj->durability <= 1 )
   {
    act("$p is broken and in need of repair.",ch,obj,NULL,TO_CHAR);
    act("$n tries to use $p but is is broken and in need of repair.",ch,obj,NULL,TO_ROOM);
    return;
   }

   if( obj->item_type == ITEM_TRIGGER && obj->value[0] == 6 )
   {
      /*
       * NOTE: you can't actually GET or WEAR a trigger item that triggers on get/wear. ZEN  
       */
      trigger_handler( ch, obj, TRIGGER_GET );
      return;
   }

   if( ( get_obj_weight( obj ) ) > str_app[get_curr_str( ch )].wield )
   {
      send_to_char( "You are not of sufficient strength to use it effectively.\r\n", ch );
      act( "$L$n nearly drops a $p.", ch, obj, NULL, TO_ROOM );
      return;
   }

   if( ( obj->item_type == ITEM_LIGHT ) && ( can_wear_at( ch, obj, WEAR_LIGHT ) ) )
   {
      if( !remove_obj( ch, WEAR_LIGHT, fReplace ) )
         return;
      act( "$L$n lights a $p.", ch, obj, NULL, TO_ROOM );
      act( "You light $p and hold it.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_LIGHT );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_FINGER ) ) && ( can_wear_at( ch, obj, WEAR_FINGER_L ) ) )
   {
      if( get_eq_char( ch, WEAR_FINGER_L ) != NULL
          && get_eq_char( ch, WEAR_FINGER_R ) != NULL
          && !remove_obj( ch, WEAR_FINGER_L, fReplace ) && !remove_obj( ch, WEAR_FINGER_R, fReplace ) )
         return;

      if( get_eq_char( ch, WEAR_FINGER_L ) == NULL )
      {
         act( "$n slips $p onto $s left finger.", ch, obj, NULL, TO_ROOM );
         act( "You slip $p onto your left finger.", ch, obj, NULL, TO_CHAR );
         equip_char( ch, obj, WEAR_FINGER_L );
         return;
      }

      if( get_eq_char( ch, WEAR_FINGER_R ) == NULL )
      {
         act( "$n slips $p onto $s right finger.", ch, obj, NULL, TO_ROOM );
         act( "You slip $p onto your right finger.", ch, obj, NULL, TO_CHAR );
         equip_char( ch, obj, WEAR_FINGER_R );
         return;
      }
      send_to_char( "You already wear two rings.\r\n", ch );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_NECK ) ) && ( can_wear_at( ch, obj, WEAR_NECK_1 ) ) )
   {
      if( get_eq_char( ch, WEAR_NECK_1 ) != NULL
          && get_eq_char( ch, WEAR_NECK_2 ) != NULL
          && !remove_obj( ch, WEAR_NECK_1, fReplace ) && !remove_obj( ch, WEAR_NECK_2, fReplace ) )
         return;

      if( get_eq_char( ch, WEAR_NECK_1 ) == NULL )
      {
         act( "$n fastens $p around $s neck.", ch, obj, NULL, TO_ROOM );
         act( "You fasten $p around your neck.", ch, obj, NULL, TO_CHAR );
         equip_char( ch, obj, WEAR_NECK_1 );
         return;
      }

      if( get_eq_char( ch, WEAR_NECK_2 ) == NULL )
      {
         act( "$n fastens $p around $s neck.", ch, obj, NULL, TO_ROOM );
         act( "You fasten $p around your neck.", ch, obj, NULL, TO_CHAR );
         equip_char( ch, obj, WEAR_NECK_2 );
         return;
      }

      bug( "Wear_obj: no free neck.", 0 );
      send_to_char( "You already wear two neck items.\r\n", ch );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_BODY ) ) && ( can_wear_at( ch, obj, WEAR_BODY ) ) )
   {
      if( !remove_obj( ch, WEAR_BODY, fReplace ) )
         return;
      act( "$n pulls $p down over $s body.", ch, obj, NULL, TO_ROOM );
      act( "You pull $p down over your body.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_BODY );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_HEAD ) ) && ( can_wear_at( ch, obj, WEAR_HEAD ) ) )
   {
      if( !remove_obj( ch, WEAR_HEAD, fReplace ) )
         return;
      act( "$n places $p onto $s head.", ch, obj, NULL, TO_ROOM );
      act( "You place $p onto your head.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_HEAD );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_LEGS ) ) && ( can_wear_at( ch, obj, WEAR_LEGS ) ) )
   {
      if( !remove_obj( ch, WEAR_LEGS, fReplace ) )
         return;
      act( "$n pushes $s legs into $p.", ch, obj, NULL, TO_ROOM );
      act( "You push you legs into $p.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_LEGS );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_FEET ) ) && ( can_wear_at( ch, obj, WEAR_FEET ) ) )
   {
      if( !remove_obj( ch, WEAR_FEET, fReplace ) )
         return;
      act( "$n slides $s feet into $p.", ch, obj, NULL, TO_ROOM );
      act( "You slide your feet into $p.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_FEET );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_HANDS ) ) && ( can_wear_at( ch, obj, WEAR_HANDS ) ) )
   {
      if( !remove_obj( ch, WEAR_HANDS, fReplace ) )
         return;
      act( "$n pulls $p over $s hands.", ch, obj, NULL, TO_ROOM );
      act( "You pull $p over your hands.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_HANDS );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_ARMS ) ) && ( can_wear_at( ch, obj, WEAR_ARMS ) ) )
   {
      if( !remove_obj( ch, WEAR_ARMS, fReplace ) )
         return;
      act( "$n pushes $p onto $s arms.", ch, obj, NULL, TO_ROOM );
      act( "You push $p onto your arms.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_ARMS );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_ABOUT ) ) && ( can_wear_at( ch, obj, WEAR_ABOUT ) ) )
   {
      if( !remove_obj( ch, WEAR_ABOUT, fReplace ) )
         return;
      act( "$n fixes $p around $s body.", ch, obj, NULL, TO_ROOM );
      act( "You fix $p around your body.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_ABOUT );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_WAIST ) ) && ( can_wear_at( ch, obj, WEAR_WAIST ) ) )
   {
      if( !remove_obj( ch, WEAR_WAIST, fReplace ) )
         return;
      act( "$n secures $p about $s waist.", ch, obj, NULL, TO_ROOM );
      act( "You secure $p about your waist.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_WAIST );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_WRIST ) ) && ( can_wear_at( ch, obj, WEAR_WRIST_L ) ) )
   {
      if( get_eq_char( ch, WEAR_WRIST_L ) != NULL
          && get_eq_char( ch, WEAR_WRIST_R ) != NULL
          && !remove_obj( ch, WEAR_WRIST_L, fReplace ) && !remove_obj( ch, WEAR_WRIST_R, fReplace ) )
         return;

      if( get_eq_char( ch, WEAR_WRIST_L ) == NULL )
      {
         act( "$n slides $p onto $s left wrist.", ch, obj, NULL, TO_ROOM );
         act( "You slide $p onto your left wrist.", ch, obj, NULL, TO_CHAR );
         equip_char( ch, obj, WEAR_WRIST_L );
         return;
      }

      if( get_eq_char( ch, WEAR_WRIST_R ) == NULL )
      {
         act( "$n slides $p onto $s right wrist.", ch, obj, NULL, TO_ROOM );
         act( "You slide $p onto your right wrist.", ch, obj, NULL, TO_CHAR );
         equip_char( ch, obj, WEAR_WRIST_R );
         return;
      }

      bug( "Wear_obj: no free wrist.", 0 );
      send_to_char( "You already wear two wrist items.\r\n", ch );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_FACE ) ) && ( can_wear_at( ch, obj, WEAR_FACE ) ) )
   {
      if( !remove_obj( ch, WEAR_FACE, fReplace ) )
         return;

      act( "$n wears $p on $s face.", ch, obj, NULL, TO_ROOM );
      act( "You wear $p on your face.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_FACE );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_EAR ) ) && ( can_wear_at( ch, obj, WEAR_EAR_L ) ) )
   {
      if( get_eq_char( ch, WEAR_EAR_L ) != NULL && get_eq_char( ch, WEAR_EAR_R ) != NULL
          && !remove_obj( ch, WEAR_EAR_L, fReplace ) && !remove_obj( ch, WEAR_EAR_R, fReplace ) )
         return;
      if( get_eq_char( ch, WEAR_EAR_L ) == NULL )
      {
         act( "$n clips $p on $s left ear.", ch, obj, NULL, TO_ROOM );
         act( "You clip $p on your left ear.", ch, obj, NULL, TO_CHAR );
         equip_char( ch, obj, WEAR_EAR_L );
         return;
      }

      if( get_eq_char( ch, WEAR_EAR_R ) == NULL )
      {
         act( "$n clips $p on $s right ear.", ch, obj, NULL, TO_ROOM );
         act( "You clip $p on your right ear.", ch, obj, NULL, TO_CHAR );
         equip_char( ch, obj, WEAR_EAR_R );
         return;
      }

      bug( "Wear_obj: no free ear.", 0 );
      send_to_char( "You already wear two earrings.\r\n", ch );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_HALO ) ) && ( can_wear_at( ch, obj, WEAR_HALO ) ) )
   {
      if( !remove_obj( ch, WEAR_HALO, fReplace ) )
         return;

      act( "$n places $p over $s head.", ch, obj, NULL, TO_ROOM );
      act( "You place $p over your head.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_HALO );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_AURA ) ) && ( can_wear_at( ch, obj, WEAR_AURA ) ) )
   {
      if( !remove_obj( ch, WEAR_AURA, fReplace ) )
         return;

      act( "$n wraps $p around $s soul.", ch, obj, NULL, TO_ROOM );
      act( "You wrap $p around your soul.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_AURA );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_HORNS ) ) && ( can_wear_at( ch, obj, WEAR_HORNS ) ) )
   {
      if( !remove_obj( ch, WEAR_HORNS, fReplace ) )
         return;

      act( "$n slides $p over $s horns.", ch, obj, NULL, TO_ROOM );
      act( "You slide $p over your horns.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_HORNS );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_BEAK ) ) && ( can_wear_at( ch, obj, WEAR_BEAK ) ) )
   {
      if( !remove_obj( ch, WEAR_BEAK, fReplace ) )
         return;

      act( "$n slides $p over $s beak.", ch, obj, NULL, TO_ROOM );
      act( "You slide $p over your beak.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_BEAK );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_WINGS ) ) && ( can_wear_at( ch, obj, WEAR_WINGS ) ) )
   {
      if( !remove_obj( ch, WEAR_WINGS, fReplace ) )
         return;

      act( "$n slides $p over $s wings.", ch, obj, NULL, TO_ROOM );
      act( "You slide $p over your wings.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_WINGS );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_SHOULDERS ) ) && ( can_wear_at( ch, obj, WEAR_SHOULDERS ) ) )
   {
      if( !remove_obj( ch, WEAR_SHOULDERS, fReplace ) )
         return;

      act( "$n slides $p onto $s shoulders.", ch, obj, NULL, TO_ROOM );
      act( "You slide $p onto your shoulders.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_SHOULDERS );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_CLAWS ) ) && ( can_wear_at( ch, obj, WEAR_CLAWS ) ) )
   {
      if( !remove_obj( ch, WEAR_CLAWS, fReplace ) )
         return;

      act( "$n slides $p over $s claws.", ch, obj, NULL, TO_ROOM );
      act( "You slide $p over your claws.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_CLAWS );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_TAIL ) ) && ( can_wear_at( ch, obj, WEAR_TAIL ) ) )
   {
      if( !remove_obj( ch, WEAR_TAIL, fReplace ) )
         return;

      act( "$n slides $p over $s tail.", ch, obj, NULL, TO_ROOM );
      act( "You slide $p over your tail.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_TAIL );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_HOOVES ) ) && ( can_wear_at( ch, obj, WEAR_HOOVES ) ) )
   {
      if( !remove_obj( ch, WEAR_HOOVES, fReplace ) )
         return;

      act( "$n slides $p over $s hooves.", ch, obj, NULL, TO_ROOM );
      act( "You slide $p over your hooves.", ch, obj, NULL, TO_CHAR );
      equip_char( ch, obj, WEAR_HOOVES );
      return;
   }

   if( ( CAN_WEAR( obj, ITEM_WEAR_HOLD_HAND ) ) && ( can_wear_at( ch, obj, WEAR_HOLD_HAND_L ) ) )
   {
      if( get_eq_char( ch, WEAR_HOLD_HAND_L ) != NULL
          && get_eq_char( ch, WEAR_HOLD_HAND_R ) != NULL
          && !remove_obj( ch, WEAR_HOLD_HAND_L, fReplace ) && !remove_obj( ch, WEAR_HOLD_HAND_R, fReplace ) )
         return;

      if( get_eq_char( ch, WEAR_HOLD_HAND_L ) == NULL )
      {
         if( ( IS_WEAPON( obj ) )
             && ( IS_WEAPON( get_eq_char( ch, WEAR_HOLD_HAND_R ) ) )
             && ( ( IS_NPC( ch ) ) || ( ch->pcdata->learned[gsn_dualwield] < 10 ) ) )
         {
            act( "You are not capable of wielding two weapons!", ch, obj, NULL, TO_CHAR );
            return;
         }
         act( "$n holds $p in $s left hand.", ch, obj, NULL, TO_ROOM );
         act( "You hold $p in your left hand.", ch, obj, NULL, TO_CHAR );
         equip_char( ch, obj, WEAR_HOLD_HAND_L );
         return;
      }

      if( get_eq_char( ch, WEAR_HOLD_HAND_R ) == NULL )
      {
         if( ( IS_WEAPON( obj ) )
             && ( IS_WEAPON( get_eq_char( ch, WEAR_HOLD_HAND_L ) ) )
             && ( ( IS_NPC( ch ) ) || ( ch->pcdata->learned[gsn_dualwield] < 10 ) ) )
         {
            act( "You are not capable of wielding two weapons!", ch, obj, NULL, TO_CHAR );
            return;
         }
         act( "$n holds $p in $s right hand.", ch, obj, NULL, TO_ROOM );
         act( "You hold $p in your right hand.", ch, obj, NULL, TO_CHAR );
         equip_char( ch, obj, WEAR_HOLD_HAND_R );
         return;
      }
      bug( "Wear_obj: no free hand.", 0 );
      send_to_char( "You already hold two items.\r\n", ch );
      return;
   }

   if( fReplace )
   {
      send_to_char( "Well, that didn't work!\r\n", ch );
      return;
   }
}

DO_FUN(do_wear)
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   short num_unique = 0;
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }
   {
      OBJ_DATA *obj_next;
      for( obj = ch->first_carry; obj != NULL; obj = obj_next )
      {
         obj_next = obj->next_in_carry_list;
         if( obj->wear_loc != WEAR_NONE &&  IS_OBJ_STAT(obj,ITEM_EXTRA_UNIQUE) )
            num_unique++;
      }
   }

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      short location;
      char outbuf[MSL];
      char catbuf[MSL];
      char colbuf[MSL], eqbuf[MSL];
      OBJ_DATA *worn;
      extern const char *where_name[];
      snprintf( outbuf, MSL, "%s", "Wear slots for your race:\r\n" );
      for( location = 0; location < MAX_WEAR; location++ )
      {
         if( race_table[ch->race].wear_locs[location] == TRUE )
         {
            if( ( worn = get_eq_char( ch, location ) ) != NULL )
            {
               snprintf( colbuf, MSL, "%s", "@@!" );
               snprintf( eqbuf, MSL, "%s", format_obj_to_char( worn, ch, TRUE ) );
            }
            else
            {
               snprintf( colbuf, MSL, "%s", "@@." );
               snprintf( eqbuf, MSL, "%s", "@@dNothing@@N" );
            }
            snprintf( catbuf, MSL, "%s%25s@@N %-*s\r\n", colbuf, where_name[location], ccode_len( eqbuf, 40 ), eqbuf );
            strncat( outbuf, catbuf, MSL-1 );
         }
      }
      send_to_char( outbuf, ch );
      return;
   }

   if( !str_cmp( arg, "all" ) )
   {
      OBJ_DATA *obj_next;
      for( obj = ch->first_carry; obj != NULL; obj = obj_next )
      {
         obj_next = obj->next_in_carry_list;
         if( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
         {
            if( ( num_unique > 4 ) && (  IS_OBJ_STAT(obj,ITEM_EXTRA_UNIQUE) ) )
               send_to_char( "You may only wear 5 unique items at one time.\r\n", ch );
            else
               wear_obj( ch, obj, FALSE );
         }
         if(  IS_OBJ_STAT(obj,ITEM_EXTRA_UNIQUE) )
            num_unique++;
      }

      return;
   }
   else
   {
      if( ( obj = get_obj_carry( ch, arg ) ) == NULL )
      {
         send_to_char( "You do not have that item.\r\n", ch );
         return;
      }
      if( num_unique > 4 && IS_OBJ_STAT(obj,ITEM_EXTRA_UNIQUE) )
      {
         send_to_char( " You may only wear 5 unique items at one time.\r\n", ch );
      }
      else
         wear_obj( ch, obj, TRUE );
   }

   return;
}



void remove_all( CHAR_DATA * ch )
{
   int counter = 0;
   while( counter < MAX_WEAR )
   {
      remove_obj( ch, counter, TRUE );
      counter = counter + 1;
   }

   return;
}

DO_FUN(do_remove)
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Remove what?\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "all" ) )
   {
      remove_all( ch );
      return;
   }

   if( ( obj = get_obj_wear( ch, arg ) ) == NULL )
   {
      send_to_char( "You do not have that item.\r\n", ch );
      return;
   }

   remove_obj( ch, obj->wear_loc, TRUE );
   return;
}



DO_FUN(do_sacrifice)
{
   char arg[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   char monbuf[MSL];
   int gp;
   OBJ_DATA *obj;
   double align_change = 0;
   short align_direction = 0;
   bool change_align = FALSE;
   bool paying_fine = FALSE;
   double obj_value =0 ;
   argument = one_argument( argument, arg );
   argument = one_argument( argument, arg2 );
   if( arg[0] == '\0' || !str_cmp( arg, ch->name ) )
   {
      act( "@@N$n offers $mself to @@e" sacgodname "@@N, who graciously declines.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "@@e" sacgodname "@@N appreciates your offer and may accept it later.\r\n", ch );
      return;
   }

   if( arg2[0] != '\0' )
   {
      if( !str_prefix( arg2, goodgodname ) )
      {
         align_direction = 1;
         change_align = TRUE;
      }
      else if( !str_prefix( arg2, evilgodname ) )
      {
         align_direction = -1;
         change_align = TRUE;
      }
      else if( !str_prefix( arg2, neutralgodname ) )
      {
         align_direction = 0;
         change_align = TRUE;
      }
      else if( !str_prefix( arg2, "judge" ) )
      {
         align_direction = 0;
         paying_fine = TRUE;
      }
   }

   obj = get_obj_room( ch, arg, ch->in_room->first_content );
   if( obj == NULL )
   {
      send_to_char( "You can't find it.\r\n", ch );
      return;
   }

   if( !can_sac_obj( ch, obj ) )
   {
      act( "$p is not an acceptable sacrifice.", ch, obj, 0, TO_CHAR );
      return;
   }

   if( obj == quest_object )
   {
      send_to_char( "Sorry, there is currently a retrieval quest running for that item.\r\n", ch );
      return;
   }
   if( !CAN_WEAR( obj, ITEM_TAKE ) && ( obj->item_type != ITEM_CORPSE_NPC ) )
   {
      act( "$p is not an acceptable sacrifice.", ch, obj, 0, TO_CHAR );
      return;
   }

   /*
    * Checks for payment of fines.
    */
   if( paying_fine )
   {
      if( IS_NPC(ch) )
      {
       send_to_char("NPCs don't have fines.\r\n",ch);
       return;
      }
      short plevel = ch->get_level("psuedo");

      if( ch->pcdata->sentence <= 0 )
      {

         send_to_char( "You have no fines outstanding.\r\n", ch );
         return;
      }
      else if( !CAN_WEAR( obj, ITEM_TAKE ) || plevel < obj->level || plevel > ( obj->level + 30 ) )
      {

         act( "The judge will not accept $p as payment.", ch, obj, 0, TO_CHAR );
         return;
      }
      else if(  IS_OBJ_STAT(obj,ITEM_EXTRA_CLAN_EQ) )
      {

         send_to_char( "You cannot use clan equipment for payment of fines.\r\n", ch );
         return;
      }
      obj_value = get_item_value( obj );
      if( plevel > 110 )
         obj_value *= .2;
      else if( plevel > 100 )
         obj_value *= .4;
      else if( plevel > 80 )
         obj_value *= .6;
      if( ( obj->item_type == ITEM_FOOD ) || ( obj->item_type == ITEM_BEACON ) || ( obj->item_type == ITEM_SOUL ) )
         obj_value = 0;
      ch->pcdata->sentence -= (int)obj_value;
      if( ch->pcdata->sentence > 0 )
      {

         snprintf( buf, MSL, "%s: Sentence reduced to %d ( - %.0f ) by sacrificing %s.", ch->name.c_str(),
                  ch->pcdata->sentence, obj_value, obj->short_descr );
         monitor_chan( buf, MONITOR_OBJ );
         act( "The judge accepts $p as partial payment of your fine.", ch, obj, 0, TO_CHAR );
      }
      else
      {

         ch->pcdata->sentence = 0;
         ch->act.reset(ACT_KILLER | ACT_THIEF);
         send_to_char( "Your debt to society has been paid!  Please more careful in the future.\r\n", ch );
         snprintf( monbuf, MSL, "%s has had a WANTED flag removed by the judge.\r\n", ch->name.c_str() );
         monitor_chan( monbuf, MONITOR_GEN_MORT );
      }

      extract_obj( obj );
      /*
       * Force a save, to prevent cheaters claiming reimb rights.
       */
      do_save( ch, "auto" );
      return;
   }

   if( obj->item_type == ITEM_CORPSE_NPC )
   {
      gp = UMAX( 1, obj->level / 5 );
      change_align = FALSE;
      paying_fine = FALSE;
   }
   else
   {
      gp = 1;
   }

   if( change_align )
   {

      align_change = obj->level;
      if(  IS_OBJ_STAT(obj,ITEM_EXTRA_REMORT) )
         align_change *= 1.5;
      if( align_direction == 1 )
      {
         if(  IS_OBJ_STAT(obj,ITEM_EXTRA_ANTI_GOOD) )
            align_change *= 1.3;
         if(  IS_OBJ_STAT(obj,ITEM_EXTRA_ANTI_EVIL) )
            align_change *= -.3;
         if(  IS_OBJ_STAT(obj,ITEM_EXTRA_ANTI_NEUTRAL) )
            align_change *= 1.1;
         snprintf( buf, MSL, "@@a" goodgodname "@@N gives you %s for your sacrifice.\r\n", cost_to_money( gp ) );
         send_to_char( buf, ch );
         act( "@@N$n sacrifices $p to @@a" goodgodname "@@N.", ch, obj, NULL, TO_ROOM );
      }

      if( align_direction == -1 )
      {
         if(  IS_OBJ_STAT(obj,ITEM_EXTRA_ANTI_GOOD) )
            align_change *= -.3;
         if(  IS_OBJ_STAT(obj,ITEM_EXTRA_ANTI_EVIL) )
            align_change *= 1.5;
         if(  IS_OBJ_STAT(obj,ITEM_EXTRA_ANTI_NEUTRAL) )
            align_change *= 1.25;
         snprintf( buf, MSL, "@@e" evilgodname "@@N gives you %s for your sacrifice.\r\n", cost_to_money( gp ) );
         send_to_char( buf, ch );
         act( "@@N$n sacrifices $p to @@e" evilgodname "@@N.", ch, obj, NULL, TO_ROOM );
      }
      if( align_direction == 0 )
      {
         if( ch->alignment > 200 )
            align_direction = -1;
         else if( ch->alignment < -200 )
            align_direction = 1;
         if(  IS_OBJ_STAT(obj,ITEM_EXTRA_ANTI_GOOD) )
            align_change *= 1.5;
         if(  IS_OBJ_STAT(obj,ITEM_EXTRA_ANTI_EVIL) )
            align_change *= 1.5;
         if(  IS_OBJ_STAT(obj,ITEM_EXTRA_ANTI_NEUTRAL) )
            align_change *= -.3;
         snprintf( buf, MSL, "@@l" neutralgodname "@@N gives you %s for your sacrifice.\r\n", cost_to_money( gp ) );
         send_to_char( buf, ch );
         act( "@@N$n sacrifices $p to @@l" neutralgodname "@@N.", ch, obj, NULL, TO_ROOM );
      }
      if( obj->item_type == ITEM_BEACON || obj->item_type == ITEM_LIGHT
          || obj->item_type == ITEM_PORTAL || obj->item_type == ITEM_FOOD )
         align_change /= 10;
      ch->alignment = URANGE( -1000, ( ch->alignment += align_direction * (short)align_change ), 1000 );
   }

   if( !change_align )
   {
      snprintf( buf, MSL, "@@N" sacgodname "@@N gives you %s for your sacrifice.\r\n", cost_to_money( gp ) );
      send_to_char( buf, ch );
      act( "@@N$n sacrifices $p to @@N" sacgodname "@@N.", ch, obj, NULL, TO_ROOM );
   }

   join_money( round_money( gp, TRUE ), ch->money );
   extract_obj( obj );
   return;
}



DO_FUN(do_quaff)
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Quaff what?\r\n", ch );
      return;
   }

   if( ( obj = get_obj_carry( ch, arg ) ) == NULL )
   {
      send_to_char( "You do not have that potion.\r\n", ch );
      return;
   }

   if( obj->item_type != ITEM_POTION )
   {
      send_to_char( "You can quaff only potions.\r\n", ch );
      return;
   }

   if( ch->get_level("psuedo") < obj->level && !IS_IMMORTAL(ch) )
   {
      send_to_char( "You are not knowledgeable enough to use such a potion.\n\r", ch );
      return;
   }

   act( "$n quaffs $p.", ch, obj, NULL, TO_ROOM );
   act( "You quaff $p.", ch, obj, NULL, TO_CHAR );
   obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
   obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
   obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );
   extract_obj( obj );
   return;
}



DO_FUN(do_recite)
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *scroll;
   OBJ_DATA *obj;
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   if( ( scroll = get_obj_carry( ch, arg1 ) ) == NULL )
   {
      send_to_char( "You do not have that scroll.\r\n", ch );
      return;
   }

   if( scroll->item_type != ITEM_SCROLL )
   {
      send_to_char( "You can recite only scrolls.\r\n", ch );
      return;
   }

   obj = NULL;
   if( arg2[0] == '\0' )
   {
      victim = ch;
   }
   else
   {
      if( ( victim = get_char_room( ch, arg2 ) ) == NULL && ( obj = get_obj_here( ch, arg2 ) ) == NULL )
      {
         send_to_char( "You can't find it.\r\n", ch );
         return;
      }
   }

   if( ch->get_level("psuedo") < obj->level && !IS_IMMORTAL(ch) )
   {
      send_to_char( "You are not knowledgeable enough to use such a scroll.\n\r", ch );
      return;
   }

   act( "$n recites $p.", ch, scroll, NULL, TO_ROOM );
   act( "You recite $p.", ch, scroll, NULL, TO_CHAR );
   obj_cast_spell( scroll->value[1], scroll->value[0], ch, victim, obj );
   obj_cast_spell( scroll->value[2], scroll->value[0], ch, victim, obj );
   obj_cast_spell( scroll->value[3], scroll->value[0], ch, victim, obj );
   extract_obj( scroll );
   return;
}



DO_FUN(do_brandish)
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   OBJ_DATA *staff;
   int sn;

   if( ch->check_cooldown(COOLDOWN_OFF) )
    return;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   if( ( ( staff = get_eq_char( ch, WEAR_HOLD_HAND_L ) ) == NULL ) || ( staff->item_type != ITEM_STAFF ) )
      if( ( ( staff = get_eq_char( ch, WEAR_HOLD_HAND_R ) ) == NULL ) || ( staff->item_type != ITEM_STAFF ) )
      {
         send_to_char( "You are not holding a staff!\r\n", ch );
         return;
      }

   if( ( sn = staff->value[3] ) < 0 || sn >= MAX_SKILL || skill_table[sn].spell_fun == 0 )
   {
      bugf( "Do_brandish: bad sn %d on object vnum %d", sn, staff->pIndexData->vnum );
      return;
   }

   ch->set_cooldown(COOLDOWN_OFF,2.25);
   if( staff->value[2] > 0 )
   {
      act( "$n brandishes $p.", ch, staff, NULL, TO_ROOM );
      act( "You brandish $p.", ch, staff, NULL, TO_CHAR );

      for( vch = ch->in_room->first_person; vch; vch = vch_next )
      {
         vch_next = vch->next_in_room;
         switch ( skill_table[sn].target )
         {
            default:
               bug( "Do_brandish: bad target for sn %d.", sn );
               return;
            case TAR_IGNORE:
               if( vch != ch )
                  continue;
               break;
            case TAR_CHAR_OFFENSIVE:
               if( IS_NPC( ch ) ? IS_NPC( vch ) : !IS_NPC( vch ) ? TRUE : vch->master != NULL ? TRUE : FALSE )
                  continue;
               break;
            case TAR_CHAR_DEFENSIVE:
               if( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) )
                  continue;
               break;
            case TAR_CHAR_SELF:
               if( vch != ch )
                  continue;
               break;
         }

         obj_cast_spell( staff->value[3], staff->value[0], ch, vch, staff );
      }
   }

   if( --staff->value[2] <= 0 )
   {
      act( "$n's $p blazes bright and is gone.", ch, staff, NULL, TO_ROOM );
      act( "Your $p blazes bright and is gone.", ch, staff, NULL, TO_CHAR );
      extract_obj( staff );
   }

   return;
}



DO_FUN(do_zap)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *wand;
   OBJ_DATA *obj;

   if( ch->check_cooldown(COOLDOWN_OFF) )
    return;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   one_argument( argument, arg );
   if( arg[0] == '\0' && ch->fighting == NULL )
   {
      send_to_char( "Zap whom or what?\r\n", ch );
      return;
   }

   if( ( ( wand = get_eq_char( ch, WEAR_HOLD_HAND_L ) ) == NULL ) || ( wand->item_type != ITEM_WAND ) )
      if( ( ( wand = get_eq_char( ch, WEAR_HOLD_HAND_R ) ) == NULL ) || ( wand->item_type != ITEM_WAND ) )
      {
         send_to_char( "You are not holding a wand!\r\n", ch );
         return;
      }



   obj = NULL;
   if( arg[0] == '\0' )
   {
      if( ch->fighting != NULL )
      {
         victim = ch->fighting;
      }
      else
      {
         send_to_char( "Zap whom or what?\r\n", ch );
         return;
      }
   }
   else
   {
      if( ( victim = get_char_room( ch, arg ) ) == NULL && ( obj = get_obj_here( ch, arg ) ) == NULL )
      {
         send_to_char( "You can't find it.\r\n", ch );
         return;
      }
   }

   ch->set_cooldown(COOLDOWN_OFF,2.25);
   if( wand->value[2] > 0 )
   {
      if( victim != NULL )
      {
         act( "$n zaps $N with $p.", ch, wand, victim, TO_ROOM );
         act( "You zap $N with $p.", ch, wand, victim, TO_CHAR );
      }
      else
      {
         act( "$n zaps $P with $p.", ch, wand, obj, TO_ROOM );
         act( "You zap $P with $p.", ch, wand, obj, TO_CHAR );
      }

      obj_cast_spell( wand->value[3], wand->value[0], ch, victim, obj );
   }

   if( --wand->value[2] <= 0 )
   {
      act( "$n's $p explodes into fragments.", ch, wand, NULL, TO_ROOM );
      act( "Your $p explodes into fragments.", ch, wand, NULL, TO_CHAR );
      extract_obj( wand );
   }

   return;
}



DO_FUN(do_steal)
{
   char buf[MAX_STRING_LENGTH];
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   int chance = 0;
   /*
    * do not allow steal in LIMBO 
    */

   if( ch->check_cooldown("steal") )
    return;

   if( ch->in_room == get_room_index( ROOM_VNUM_LIMBO ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Steal what from whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg2 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "That's pointless.\r\n", ch );
      return;
   }

   if( IS_GHOST(victim) )
   {
    send_to_char("You can't seem to steal from the dead...\r\n",ch);
    return;
   }

   ch->set_cooldown("steal");
   chance = IS_NPC( ch ) ? ( ch->get_level("psuedo") / 4 )
      : ( ch->pcdata->learned[gsn_steal] / 3 + ( get_curr_dex( ch ) / 2 ) );
   chance = chance - ( ( victim->get_level("psuedo") - ch->get_level("psuedo") ) / 2 );
   if( IS_ADEPT(victim) && !IS_ADEPT(ch) )
      chance = chance - 25;
   if( !IS_NPC( ch ) )
      chance += ch->get_level("assassin") / 4;
   if( ch->get_level("psuedo") > ( victim->get_level("psuedo") + 30 ) )
   {
      send_to_char( "Coward!!! Trying to steal from the weak..\r\n", ch );
      return;
   }
   if( ( victim->position == POS_FIGHTING ) || ( number_percent(  ) > chance ) )
   {
      /*
       * Failure.
       */
      send_to_char( "Oops.\r\n", ch );
      act( "$n tried to steal from you.\r\n", ch, NULL, victim, TO_VICT );
      act( "$n tried to steal from $N.\r\n", ch, NULL, victim, TO_NOTVICT );
      if( !IS_NPC( ch ) && !IS_NPC( victim )
          && ch->act.test(ACT_PKOK) && victim->act.test(ACT_PKOK) )
         return;
      snprintf( buf, MSL, "%s is a bloody thief!", ch->name.c_str() );
      do_yell( victim, buf );
      if( !IS_NPC( ch ) )
      {
         if( IS_NPC( victim ) && ( victim->npcdata->pIndexData->pShop == NULL ) )
         {
            act( "$n says 'I'll get you for that, $N!'", victim, NULL, ch, TO_ROOM );
            one_hit( victim, ch, TYPE_UNDEFINED );
         }
         else
         {
            int diff = 0;
            diff = ( abs( ch->get_level("psuedo") - victim->get_level("psuedo") ) + 10 ) * 20;
            ch->act.set(ACT_THIEF);
            send_to_char( "*** You are now a THIEF!! ***\r\n", ch );
            ch->pcdata->sentence += diff;
            save_char_obj( ch );
            log_string( buf );
         }
      }
      return;
   }

   if( ( obj = get_obj_carry( victim, arg1 ) ) == NULL )
   {
      send_to_char( "You can't find it.\r\n", ch );
      return;
   }

   if( !can_drop_obj( ch, obj )
       ||  IS_OBJ_STAT(obj,ITEM_EXTRA_INVENTORY) || obj->level > ch->level || ( obj->wear_loc > -1 ) )
   {
      send_to_char( "You can't pry it away.\r\n", ch );
      return;
   }

   if( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
   {
      send_to_char( "You have your hands full.\r\n", ch );
      return;
   }

   if( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
   {
      send_to_char( "You can't carry that much weight.\r\n", ch );
      return;
   }

   obj_from_char( obj );
   obj_to_char( obj, ch );
   send_to_char( "Yes!  Got it!!!\r\n", ch );
   return;
}



/*
 * Shopping commands.
 */
CHAR_DATA *find_keeper( CHAR_DATA * ch )
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *keeper;
   SHOP_DATA *pShop;
   pShop = NULL;
   for( keeper = ch->in_room->first_person; keeper; keeper = keeper->next_in_room )
   {
      if( IS_NPC( keeper ) && ( pShop = keeper->npcdata->pIndexData->pShop ) != NULL )
         break;
   }

   if( pShop == NULL || IS_AFFECTED( keeper, AFF_CHARM ) )
   {
      send_to_char( "There does not appear to be a shop here.\r\n",ch);
      return NULL;
   }

   /*
    * Undesirables.
    */

   if( !IS_NPC( ch ) && ch->act.test(ACT_KILLER) )
   {
      do_say( keeper, "Killers are not welcome!" );
      snprintf( buf, MSL, "%s the KILLER is over here!", ch->name.c_str() );
      do_shout( keeper, buf );
      check_guards( ch );
      return NULL;
   }

   if( !IS_NPC( ch ) && ch->act.test(ACT_THIEF) )
   {
      do_say( keeper, "Thieves are not welcome!" );
      snprintf( buf, MSL, "%s the THIEF is over here!", ch->name.c_str() );
      do_shout( keeper, buf );
      check_guards( ch );
      return NULL;
   }



   /*
    * Shop hours.
    */
   if( time_info.hour < pShop->open_hour )
   {
      do_say( keeper, "Sorry, come back later." );
      return NULL;
   }

   if( time_info.hour > pShop->close_hour )
   {
      do_say( keeper, "Sorry, come back tomorrow." );
      return NULL;
   }

   /*
    * Invisible or hidden people.
    */
   if( !can_see( keeper, ch ) )
   {
      do_say( keeper, "I don't trade with folks I can't see." );
      return NULL;
   }
   if( !can_see( ch, keeper ) )
   {
      send_to_char( "You don't seem to see anyone around.\r\n", ch );
      return NULL;
   }
   return keeper;
}


/* Called when a shopkeeper spots a wanted player...
 * calls local mobs with spec_policeman set to hunt player
 * --Stephen
 */

void check_guards( CHAR_DATA * ch )
{
   CHAR_DATA *guard;
   list<CHAR_DATA *>::iterator li;
   char buf[MAX_STRING_LENGTH];

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      guard = *li;
      if( IS_NPC( guard )
          && ( guard->in_room->area == ch->in_room->area )
          && guard->npcdata->spec_fun != 0
          && !str_cmp( "spec_policeman", rev_spec_lookup( guard->npcdata->spec_fun ) ) && guard->hunting == NULL )
      {
         if( set_hunt( guard, NULL, ch, NULL, 0, HUNT_INFORM | HUNT_MERC | HUNT_CR ) )
         {
/*	     if ( make_hunt( guard, ch ) )
	     {
	       REMOVE_BIT( guard->act_hunt, ACT_HUNT_INFORM );*/
            switch ( number_range( 0, 7 ) )
            {
               case 0:
                  do_yell( guard, "I'm coming!" );
                  break;
               case 1:
                  do_yell( guard, "On my way!" );
                  break;
               case 2:
                  do_yell( guard, "Watch out buster, here i come!" );
                  break;
               case 3:
                  do_yell( guard, "Come make my day punk!" );
                  interpret( guard, "grin" );
                  break;
               case 4:
                  snprintf( buf, MSL, "leaves in search of %s.", ch->name.c_str() );
                  do_emote( guard, buf );
                  break;
               case 5:
                  interpret( guard, "wave" );
                  snprintf( buf, MSL, "Laters... i'm off to get %s.", ch->name.c_str() );
                  do_say( guard, buf );
                  break;
               case 6:
                  interpret( guard, "bounce" );
                  do_say( guard, "Yes!  Someone to go kill!" );
                  break;
               case 7:
                  snprintf( buf, MSL, "%s Watch it mate... i'm after you!", ch->name.c_str() );
                  do_tell( guard, buf );
                  break;
            }
         }
      }
   }
   return;
}



int get_cost( CHAR_DATA * keeper, OBJ_DATA * obj, bool fBuy )
{
   SHOP_DATA *pShop;
   int cost;
   if( obj == NULL || ( pShop = keeper->npcdata->pIndexData->pShop ) == NULL )
      return 0;
   if( fBuy )
   {
      cost = obj->cost;
      if( obj->item_type == ITEM_CHARM )
      {
       cost += 1000;
       cost += (obj->level * 100);
      }
      cost *= pShop->profit_buy / 100;
   }
   else
   {
      OBJ_DATA *obj2;
      int itype;
      cost = 0;
      for( itype = 0; itype < MAX_TRADE; itype++ )
      {
         if( obj->item_type == pShop->buy_type[itype] )
         {
            cost = obj->cost * pShop->profit_sell / 100;
            break;
         }
      }

      for( obj2 = keeper->first_carry; obj2; obj2 = obj2->next_in_carry_list )
      {
         if( obj->pIndexData == obj2->pIndexData )
            cost /= 2;
      }
   }

   if( obj->item_type == ITEM_STAFF || obj->item_type == ITEM_WAND )
      cost = cost * obj->value[2] / obj->value[1];
   return cost;
}



DO_FUN(do_buy)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *keeper;
   argument = one_argument( argument, arg );

   if( ( keeper = find_keeper( ch ) ) == NULL )
    return;

   if( arg[0] == '\0' )
   {
      send_to_char( "Buy what?\r\n", ch );
      return;
   }
   if( IS_VAMP( ch ) && !IS_VAMP( keeper ) )
   {
      do_say( keeper, "I don't serve VAMPIRES!! Be gone evil one!" );
      return;
   }
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      do_say( keeper, "I don't deal with @@bBeasts@@N Be gone!!!@@N\r\n" );
      return;
   }

   if( ch->in_room->room_flags.test(RFLAG_PET_SHOP) )
   {
      char buf[MAX_STRING_LENGTH];
      CHAR_DATA *pet;
      ROOM_INDEX_DATA *pRoomIndexNext;
      ROOM_INDEX_DATA *in_room;
      int change = 0;
      char *cost_string;
      char givebuf[MSL], changebuf[MSL];
      if( IS_NPC( ch ) )
         return;
      pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
      if( pRoomIndexNext == NULL )
      {
         bug( "Do_buy: bad pet shop at vnum %d.", ch->in_room->vnum );
         send_to_char( "Sorry, you can't buy that here.\r\n", ch );
         return;
      }

      in_room = ch->in_room;
      ch->in_room = pRoomIndexNext;
      pet = get_char_room( ch, arg );
      ch->in_room = in_room;
      if( pet == NULL || !pet->act.test(ACT_PET) )
      {
         send_to_char( "Sorry, you can't buy that here.\r\n", ch );
         return;
      }

      if( ch->act.test(ACT_BOUGHT_PET) )
      {
         send_to_char( "You already bought one pet this level.\r\n", ch );
         return;
      }

      if( money_value( ch->money ) < 10 * pet->level * pet->level )
      {
         send_to_char( "You can't afford it.\r\n", ch );
         return;
      }

      if( ch->level < pet->level )
      {
         send_to_char( "You're not ready for this pet.\r\n", ch );
         return;
      }
      cost_string = take_best_coins( ch->money, 10 * pet->level * pet->level );
      cost_string = one_argument( cost_string, changebuf );
      change = is_number( changebuf ) ? atoi( changebuf ) : 0;
      snprintf( givebuf, MSL, "%s to %s", cost_string, keeper->name.c_str() );
      do_give( ch, givebuf );
      if( change > 0 )
      {
         MONEY_TYPE *transaction;
         do_say( keeper, "And here is your change!" );
         transaction = round_money( change, TRUE );
         snprintf( givebuf, MSL, "$N gives you %s.", money_string( transaction ) );
         act( givebuf, ch, NULL, keeper, TO_CHAR );
         snprintf( givebuf, MSL, "$N gives $n %s.", money_string( transaction ) );
         act( givebuf, ch, NULL, keeper, TO_ROOM );
         join_money( transaction, ch->money );
      }
      else
      {
         do_say( keeper, "Thank you very much for your business!" );
      }

      pet = create_mobile( pet->npcdata->pIndexData );
      ch->act.set(ACT_BOUGHT_PET);
      pet->act.set(ACT_PET);
      SET_BIT( pet->affected_by, AFF_CHARM );
      argument = one_argument( argument, arg );
      if( arg[0] != '\0' )
      {
         snprintf( buf, MSL, "%s %s", pet->name.c_str(), arg );
         pet->name = buf;
      }

      snprintf( buf, MSL, "%sA neck tag says 'I belong to %s'.\r\n", pet->description.c_str(), ch->name.c_str() );
      pet->description = buf;
      char_to_room( pet, ch->in_room );
      add_follower( pet, ch );
      send_to_char( "Enjoy your pet.\r\n", ch );
      act( "$n bought $N as a pet.", ch, NULL, pet, TO_ROOM );
      return;
   }
   else
   {

      OBJ_DATA *obj;
      int cost;
      int change = 0;
      char givebuf[MSL], changebuf[MSL];
      char *cost_string;
      obj = get_obj_carry( keeper, arg );
      cost = get_cost( keeper, obj, TRUE );
      if( cost <= 0 || !can_see_obj( ch, obj ) )
      {
         act( "$n tells you 'I don't sell that -- try 'list''.", keeper, NULL, ch, TO_VICT );
         ch->reply = keeper;
         return;
      }

      if( money_value( ch->money ) < cost )
      {
         act( "$n tells you 'You can't afford to buy $p'.", keeper, obj, ch, TO_VICT );
         ch->reply = keeper;
         return;
      }

      if( obj->level > ch->get_level("psuedo") )
      {
         act( "$n tells you 'You can't use $p yet'.", keeper, obj, ch, TO_VICT );
         ch->reply = keeper;
         return;
      }

      if( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
      {
         send_to_char( "You can't carry that many items.\r\n", ch );
         return;
      }

      if( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
      {
         send_to_char( "You can't carry that much weight.\r\n", ch );
         return;
      }

      cost_string = take_best_coins( ch->money, cost );
      cost_string = one_argument( cost_string, changebuf );
      change = is_number( changebuf ) ? atoi( changebuf ) : 0;
      snprintf( givebuf, MSL, "%s to %s", cost_string, keeper->name.c_str() );
      do_give( ch, givebuf );
      act( "$n buys $p.", ch, obj, NULL, TO_ROOM );
      act( "You buy $p.", ch, obj, NULL, TO_CHAR );
      if( change > 0 )
      {
         MONEY_TYPE *transaction;
         do_say( keeper, "And here is your change!" );
         transaction = round_money( change, TRUE );
         snprintf( givebuf, MSL, "$N gives you %s.", money_string( transaction ) );
         act( givebuf, ch, NULL, keeper, TO_CHAR );
         snprintf( givebuf, MSL, "$N gives $n %s.", money_string( transaction ) );
         act( givebuf, ch, NULL, keeper, TO_ROOM );
         join_money( transaction, ch->money );
      }
      else
      {
         do_say( keeper, "Thank you very much for your business!" );
      }

      if(  IS_OBJ_STAT(obj,ITEM_EXTRA_INVENTORY) )
         obj = create_object( obj->pIndexData, obj->level );
      else
         obj_from_char( obj );
      obj_to_char( obj, ch );
      return;
   }
}



DO_FUN(do_list)
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   int stopcounter = 0;
   bool found;
   MONEY_TYPE *rounded_cost;
   char costbuf[MSL];
   CHAR_DATA *keeper;
   int cost;

   buf1[0] = '\0';
   found = false;

   if( ( keeper = find_keeper( ch ) ) == NULL )
    return;

   if( IS_VAMP( ch ) && !IS_VAMP( keeper ) )
   {
      do_say( keeper, "I don't serve VAMPIRES!! Be gone evil one!" );
      return;
   }
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      do_say( keeper, "I don't deal with @@bBeasts@@N Be gone!!!@@N\r\n" );
      return;
   }


   if( ch->in_room->room_flags.test(RFLAG_PET_SHOP) )
   {
      ROOM_INDEX_DATA *pRoomIndexNext;
      CHAR_DATA *pet;

      pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
      if( pRoomIndexNext == NULL )
      {
         bug( "Do_list: bad pet shop at vnum %d.", ch->in_room->vnum );
         send_to_char( "You can't do that here.\r\n", ch );
         return;
      }

      found = FALSE;
      for( pet = pRoomIndexNext->first_person; pet; pet = pet->next_in_room )
      {
         if( pet->act.test(ACT_PET) )
         {
            if( !found )
            {
               found = TRUE;
               strncat( buf1,
                            "\r\n@@g[@@yLevel@@g]       @@yPet@@g                           @@yPrice  ( Approximate )@@N \r\n", MSL );
            }
            stopcounter++;
            rounded_cost = round_money_off( 10 * pet->level * pet->level, 1 );
            snprintf( costbuf, MSL, "%s", money_string( rounded_cost ) );
            snprintf( buf, MSL, "[ @@W%3d@@g]  @@c%-*s@@g  @@W%-*s@@N \r\n", pet->level, ccode_len( pet->npcdata->short_descr, 30 ),
                     capitalize( pet->npcdata->short_descr ), ccode_len( costbuf, 35 ), costbuf );
            delete rounded_cost;
            strncat( buf1, buf, MSL-1 );
            if( stopcounter > 45 )
            {
               send_to_char( buf1, ch );
               snprintf( buf1, MSL, "%s", "" );
               stopcounter = 0;
            }
         }
      }
      if( !found )
         send_to_char( "Sorry, we're out of pets right now.\r\n", ch );
      send_to_char( buf1, ch );
      return;
   }
   else
   {
      char arg[MAX_INPUT_LENGTH];
      OBJ_DATA *obj;
      one_argument( argument, arg );
      for( obj = keeper->first_carry; obj; obj = obj->next_in_carry_list )
      {
         if( obj->wear_loc == WEAR_NONE
             && can_see_obj( ch, obj )
             && ( cost = get_cost( keeper, obj, TRUE ) ) > 0 && ( arg[0] == '\0' || is_name( arg, obj->name ) ) )
         {
            if( !found )
            {
               found = TRUE;
               strncat( buf1,
                            "\r\n@@g[@@yLvl@@g]       @@yItem@@g                           @@yPrice  ( Approximate )@@N \r\n", MSL );
            }
            stopcounter++;
            rounded_cost = round_money_off( cost, 1 );
            snprintf( costbuf, MSL, "%s", money_string( rounded_cost ) );
            snprintf( buf, MSL, "@@g[%s%3d@@g]  @@c%-*s@@g  @@W%-*s@@N \r\n", ( IS_OBJ_STAT(obj,ITEM_EXTRA_REMORT) ? "@@m" : "@@a" ),
                     obj->level, ccode_len( obj->short_descr, 30 ), capitalize( obj->short_descr ), ccode_len( costbuf, 30 ),
                     costbuf );
            delete rounded_cost;
            strncat( buf1, buf, MSL-1 );
            if( stopcounter > 45 )
            {
               send_to_char( buf1, ch );
               snprintf( buf1, MSL, "%s", "" );
               stopcounter = 0;
            }

         }
      }

      if( !found )
      {
         if( arg[0] == '\0' )
            send_to_char( "You can't buy anything here.\r\n", ch );
         else
            send_to_char( "You can't buy that here.\r\n", ch );
         return;
      }

      send_to_char( buf1, ch );
      send_to_char( "@@mMagenta@@N Level numbers indicate remort items.\r\nYou may also type list <keyword>.\r\n", ch );
      return;
   }
}



DO_FUN(do_sell)
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *keeper;
   OBJ_DATA *obj;
   int cost;
   int cur_money;
   char moneybuf[MSL];
   bool good_trade = FALSE;
   MONEY_TYPE *transfer;
   one_argument( argument, arg );

   if( ( keeper = find_keeper( ch ) ) == NULL )
    return;

   if( arg[0] == '\0' )
   {
      send_to_char( "Sell what?\r\n", ch );
      return;
   }
   if( IS_VAMP( ch ) && !IS_VAMP( keeper ) )
   {
      do_say( keeper, "I don't serve VAMPIRES!! Be gone evil one!" );
      return;
   }
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      do_say( keeper, "I don't deal with @@bBeasts@@N Be gone!!!@@N\r\n" );
      return;
   }


   if( ( obj = get_obj_carry( ch, arg ) ) == NULL )
   {
      act( "$n tells you 'You don't have that item'.", keeper, NULL, ch, TO_VICT );
      ch->reply = keeper;
      return;
   }

   if( !can_drop_obj( ch, obj ) )
   {
      send_to_char( "You can't let go of it.\r\n", ch );
      return;
   }

   if( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
   {
      act( "$n looks uninterested in $p.", keeper, obj, ch, TO_VICT );
      return;
   }

   if(  IS_OBJ_STAT(obj,ITEM_EXTRA_CLAN_EQ) )
   {
      act( "$n looks uncertain about buying clan equipment.", keeper, NULL, NULL, TO_VICT );
      return;
   }

   if( count_obj_list( obj->pIndexData, keeper->first_carry ) > 4 )
   {
      send_to_char( "Sorry, I don't need any more of those.\r\n", ch );
      return;
   }

   cur_money = money_value( keeper->money );
   if( cur_money < cost )
   {
      snprintf( buf, MSL, "Sorry, but I can't afford that!" );
      do_say( keeper, buf );
      return;
   }
   cur_money -= cost;
   delete keeper->money;
   keeper->money = round_money( cur_money, TRUE );
   transfer = round_money( cost, TRUE );
   snprintf( buf, MSL, "You sell $p for %s.", money_string( transfer ) );
   if( ( ch->carry_weight + money_weight( transfer ) ) > can_carry_w( ch ) )
   {
      snprintf( buf, MSL, "%s cannot carry that much weight!\r\n", keeper->get_name(ch) );
      send_to_char( buf, keeper );
      send_to_char( "You cannot carry that much money with your current load!\r\n", ch );
      join_money( transfer, keeper->money );
      return;
   }
   keeper->carry_weight -= money_weight( transfer );
   ch->carry_weight += money_weight( transfer );
   join_money( transfer, ch->money );
   snprintf( moneybuf, MSL, "%s", unit_string( transfer ) );
   good_trade = TRUE;
   snprintf( log_buf, (2 * MIL), "Selling for %s, Trade %s", moneybuf, good_trade ? "@@aAccepted@@N" : "@@eRejected@@N" );
   monitor_chan( log_buf, MONITOR_DEBUG );
   if( good_trade )
   {
      act( "$n sells $p.", ch, obj, NULL, TO_ROOM );
      act( buf, ch, obj, NULL, TO_CHAR );
      if( obj->item_type == ITEM_TRASH )
      {
         extract_obj( obj );
      }
      else
      {
         obj_from_char( obj );
         obj_to_char( obj, keeper );
      }
   }

   return;
}



DO_FUN(do_value)
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *keeper;
   OBJ_DATA *obj;
   int cost;
   MONEY_TYPE *moneycost;
   one_argument( argument, arg );

   if( ( keeper = find_keeper( ch ) ) == NULL )
    return;

   if( arg[0] == '\0' )
   {
      send_to_char( "Value what?\r\n", ch );
      return;
   }
   if( IS_VAMP( ch ) && !IS_VAMP( keeper ) )
   {
      do_say( keeper, "I don't serve VAMPIRES!! Be gone evil one!" );
      return;
   }
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      do_say( keeper, "I don't deal with @@bBeasts@@N Be gone!!!@@N\r\n" );
      return;
   }


   if( ( obj = get_obj_carry( ch, arg ) ) == NULL )
   {
      act( "$n tells you 'You don't have that item'.", keeper, NULL, ch, TO_VICT );
      ch->reply = keeper;
      return;
   }

   if( !can_drop_obj( ch, obj ) )
   {
      send_to_char( "You can't let go of it.\r\n", ch );
      return;
   }

   if( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
   {
      act( "$n looks uninterested in $p.", keeper, obj, ch, TO_VICT );
      return;
   }
   moneycost = round_money( cost, TRUE );
   snprintf( buf, MSL, "$n tells you 'I'll give you %s for $p'.", money_string( moneycost ) );
   act( buf, keeper, obj, ch, TO_VICT );
   ch->reply = keeper;
   delete moneycost;
   return;
}

DO_FUN(do_donate)
/*  This function removes *one* object from a player,
 *  and instead of dropping it in the curent room, it is 
 *  placed in the donation room, vnum 3206
 *  (I have donation room one east of temple)
 *  Eg: donate excalibur 
 *  Donate all was not implemented to avoid unwanted eq loss :)
 *  Money may NOT be denoted - risk of too much lying around!!
 *  Three donation rooms now, used to hold armor, weapons and misc.
 *  --  Stephen
 */
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int place_to_put_it = ROOM_VNUM_MISC_DONATE;
   ROOM_INDEX_DATA *room = NULL;
   argument = one_argument( argument, arg );
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   if( arg[0] == '\0' )
   {
      send_to_char( "Donate what?\r\n", ch );
      return;
   }

   /*
    * Donating coins is NOT allowed! 
    */

   if( is_number( arg ) )
   {
      send_to_char( "Sorry, that is not possible - yet?!\r\n", ch );
      return;
   }

   /*
    * Donate all will not be allowed! 
    */

   if( !str_cmp( arg, "all" ) || !str_prefix( "all.", arg ) )
   {
      send_to_char( "Donating *all* sounds dodgy to me!\r\n", ch );
      return;
   }

   /*
    * Check to see if item in inventory :) 
    */

   if( ( obj = get_obj_carry( ch, arg ) ) == NULL )
   {
      send_to_char( "You do not have that item to donate.\r\n", ch );
      return;
   }

   /*
    * Check to see if item *can* be dropped! 
    */

   if( !can_drop_obj( ch, obj ) )
   {
      send_to_char( "You can't let go of it.\r\n", ch );
      return;
   }
   if( obj->item_type == ITEM_BEACON )
   {
      send_to_char( " @@eNice try:P@@N\r\n", ch );
      return;
   }

   /*
    * Remove object from inventory and put into donation room 
    */

   if( obj->item_type == ITEM_WEAPON )
      place_to_put_it = ROOM_VNUM_WEAPON_DONATE;
   if( obj->item_type == ITEM_ARMOR )
      place_to_put_it = ROOM_VNUM_ARMOR_DONATE;
   if( ( room = get_room_index( place_to_put_it ) ) == NULL )
   {
      snprintf( log_buf, (2 * MIL), "No valid donation room: %d", place_to_put_it );
      log_f( "%s", log_buf );
      send_to_char( "System Error.  Contact an Administrator.\r\n", ch );
      return;
   }
   act( "$n donates $p, how kind.", ch, obj, NULL, TO_ROOM );
   act( "You donate $p, thank you.", ch, obj, NULL, TO_CHAR );
   if( count_obj_room( obj->pIndexData, room->first_content ) < 10 )
   {
      obj_from_char( obj );
      obj_to_room( obj, room );
   }
   else
   {
      extract_obj( obj );
   }
   do_save( ch, "auto" );
}

DO_FUN(do_adapt)
{
   /*
    * Take one piece of eq, the ch's level, and reset the eq's stats
    * * and level req so that they can use it, but the eq won't be as
    * * good as it was previously...  Only do if ch->level > eq->level!
    * * Added: descrips changed to reflect adaptation.
    * * -- Stephen
    */

   char arg[MSL];
   char buf[MSL];
   OBJ_DATA *obj;
   AFFECT_DATA *paf;
   CHAR_DATA *mob;
   bool changed;  /* was the eq changed?? */
   long diff;  /* ratio to change stuff by */
   int pen; /* additional penalty */
   int cost;
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   cost = ( ch->level * 250 );
   argument = one_argument( argument, arg );
   /*
    * Check for mob with act->adapt
    */
   for( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
   {
      if( IS_NPC( mob ) && mob->act.test(ACT_ADAPT) )
         break;
   }

   if( mob == NULL )
   {
      send_to_char( "You can't do that here.\r\n", ch );
      return;
   }

   if( arg[0] == '\0' )
   {
      snprintf( buf, MSL, "The cost for you to have a weapon adapted is: %d GP.\r\n", cost );
      send_to_char( buf, ch );
      send_to_char( "Usuage: ADAPT <weapon>.  The weapon must be in your inventory.\r\n", ch );
      return;
   }



   if( !str_cmp( arg, "all" ) || !str_prefix( "all.", arg ) )
   {
      send_to_char( "Only one weapon can be adapted at a time.....\r\n", ch );
      return;
   }

   /*
    * Check to see if item in inventory :)
    */

   if( ( obj = get_obj_carry( ch, arg ) ) == NULL )
   {
      send_to_char( "You do not have that item to be adapted.\r\n", ch );
      return;
   }

   diff = ( obj->level / ch->level );
   pen = ( obj->level - ch->level );
   if( diff < 1 || pen == 0 )
   {
      send_to_char( "It doesn't need adapting!\r\n", ch );
      return;
   }

   if( pen == 1 )
   {
      send_to_char( "Just wait until you gain another level!\r\n", ch );
      return;
   }
   pen *= 2;   /* Double difference in levels as penalty. */
   if( money_value(ch->money) < cost )
   {
      send_to_char( "You can't afford to have it adapted!\r\n", ch );
      return;
   }

   /*
    * Now, see if the piece of eq CAN be adapted..... 
    */

   switch ( obj->item_type )
   {
      default:
         changed = FALSE;
         break;
      case ITEM_WEAPON:
         obj->value[1] /= diff;  /* Reduce the damage the weapon does */
         obj->value[1] = UMAX( 0, obj->value[1] - pen );
         obj->value[2] /= diff;
         obj->value[2] = UMAX( 0, obj->value[2] - pen );
         changed = TRUE;
         if( obj->value[2] == 0 )   /* Then weapon is screwed */
         {
            send_to_char( "The weapon crumbles into dust...It was broken!\r\n", ch );
            extract_obj( obj );
            changed = FALSE;
         }
         break;
   }

   if( !changed )
   {
      send_to_char( "You can only have *weapons* adapted!\r\n", ch );
      return;
   }

   /*
    * Tone down any affects that apply to the object, if applicable 
    */

   for( paf = obj->first_apply; paf != NULL; paf = paf->next )
   {
      if( paf->location != APPLY_NONE && paf->modifier != 0 )
      {
         if( paf->modifier > 0 )
            paf->modifier = UMAX( 0, ( paf->modifier / ( diff * 2 ) - pen ) );
         else
            paf->modifier = UMIN( 0, ( paf->modifier / ( diff * 2 ) - pen ) );
      }
   }

   char *coststring;
   char change[MSL], mbuf[MSL];
   change[0] = '\0';
   coststring = take_best_coins(ch->money,cost); /* Gives a string of the coins we need to steal. */
   coststring = one_argument(coststring,change); /* First arg is change, skip it. Rest is what we take. */
   for( short mn = 0; mn < MAX_CURRENCY; mn++ )
   {
    char m_number[MSL], m_name[MSL];
    coststring = one_argument(coststring,m_number);
    if( m_number[0] == '\0' )
     break;
    coststring = one_argument(coststring,m_name);
    if( m_name[0] == '\0' )
     break;
    if( m_number > 0 )
     snprintf(mbuf,MSL,"%s %s",m_number,m_name);
   }
   money_to_value(ch,mbuf);

   obj->level = ch->level; /* Allow ch to use the eq... */
   snprintf( buf, MSL, "%s <adapted>", obj->short_descr );
   free_string( obj->short_descr );
   obj->short_descr = str_dup( buf );
   snprintf( buf, MSL, "<adapted> %s", obj->long_descr );
   free_string( obj->long_descr );
   obj->long_descr = str_dup( buf );
   send_to_char( "Your weapon is now adapted.\r\n", ch );
   return;
}


DO_FUN(do_cdonate)
/*  This function removes *one* object from a player,
 *  and instead of dropping it in the curent room, it is 
 *  placed in the donation room.
 *  (I have donation room one east of temple)
 *  Eg: donate excalibur 
 *  Donate all was not implemented to avoid unwanted eq loss :)
 *  Money may NOT be denoted - risk of too much lying around!!
 *  Three donation rooms now, used to hold armor, weapons and misc.
 *  --  Stephen
 * This just a copy of donate, except it works for clans -S-
 */
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int place_to_put_it;
   ROOM_INDEX_DATA *room;
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   argument = one_argument( argument, arg );
   if( IS_NPC( ch ) )
   {
      send_to_char( "Mobs don't have clans.\r\n", ch );
      return;
   }

   if( ch->clan == 0 )
   {
      send_to_char( "You must be in a clan to use this command!\r\n", ch );
      return;
   }

   if( arg[0] == '\0' )
   {
      send_to_char( "Donate what?\r\n", ch );
      return;
   }

   /*
    * Donating coins is NOT allowed! 
    */

   if( is_number( arg ) )
   {
      send_to_char( "Sorry, that is not possible - yet?!\r\n", ch );
      return;
   }

   /*
    * Donate all will not be allowed! 
    */

   if( !str_cmp( arg, "all" ) || !str_prefix( "all.", arg ) )
   {
      send_to_char( "Donating *all* sounds dodgy to me!\r\n", ch );
      return;
   }

   /*
    * Check to see if item in inventory :) 
    */

   if( ( obj = get_obj_carry( ch, arg ) ) == NULL )
   {
      send_to_char( "You do not have that item to donate.\r\n", ch );
      return;
   }

   /*
    * Check to see if item *can* be dropped! 
    */

   if( !can_drop_obj( ch, obj ) )
   {
      send_to_char( "You can't let go of it.\r\n", ch );
      return;
   }
   if( obj->item_type == ITEM_BEACON )
   {
      send_to_char( " @@eNice try:P@@N\r\n", ch );
      return;
   }

   /*
    * Remove object from inventory and put into donation room 
    */

   place_to_put_it = clan_table[ch->clan].donat_room;
   if( ( room = get_room_index( place_to_put_it ) ) == NULL )
   {
      snprintf( log_buf, (2 * MIL), "Cdonate: no valid room for vnum %d", place_to_put_it );
      log_f( "%s", log_buf );
      send_to_char( "Invalid Clan Donation Room.  Please contact an Administrator.\r\n", ch );
      return;
   }
   act( "$n clan-donates $p, how kind.", ch, obj, NULL, TO_ROOM );
   act( "You clan-donate $p, thank you.", ch, obj, NULL, TO_CHAR );
   if( count_obj_room( obj->pIndexData, room->first_content ) < 10 )
   {
      obj_from_char( obj );
      obj_to_room( obj, room );
   }
   else
   {
      extract_obj( obj );
   }
   do_save( ch, "auto" );
   return;
}


DO_FUN(do_appraise)
{
   OBJ_DATA *obj;
   char buf[MAX_STRING_LENGTH];
   int av;
   int gold;
   int ac;
   int hold;
   int foo;
   int lv;
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "Appraise what?\r\n", ch );
      return;
   }

   if( ( obj = get_obj_here( ch, argument ) ) == NULL )
   {
      send_to_char( "It would help if you tried to appraise an object!\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_appraise] == 0 )
   {
      send_to_char( "You don't know how to appraise items!\r\n", ch );
      return;
   }


   switch ( obj->item_type )
   {
      case ITEM_WEAPON:
         /*
          * Try and make output a little interesting 
          */

         av = ( obj->value[1] + obj->value[2] ) / 2;
         if( av < 5 )
            snprintf( buf, MSL, "$p couldn't cut a blade of grass." );
         else if( av < 10 )
            snprintf( buf, MSL, "$p might hurt a small animal." );
         else if( av < 15 )
            snprintf( buf, MSL, "$p is ok, but it's not worth stealing!" );
         else if( av < 20 )
            snprintf( buf, MSL, "$p looks good, maybe you should get one?" );
         else if( av < 30 )
            snprintf( buf, MSL, "$p appears to be pretty powerful!" );
         else if( av < 50 )
            snprintf( buf, MSL, "$p could fell trees in one swoop!" );
         else
            snprintf( buf, MSL, "$p could kill the very Gods!" );
         act( buf, ch, obj, NULL, TO_CHAR );
         break;
      case ITEM_ARMOR:

         ac = obj->value[0];
         if( ac < 0 )
            snprintf( buf, MSL, "$p couldn't protect a paper bag!" );
         else if( ac < 3 )
            snprintf( buf, MSL, "$p looks quite strong!" );
         else if( ac < 5 )
            snprintf( buf, MSL, "$p could help protect you well." );
         else if( ac < 10 )
            snprintf( buf, MSL, "$p would guard well against attack." );
         else if( ac < 50 )
            snprintf( buf, MSL, "$p could protect a God!" );
         else
            snprintf( buf, MSL, "$p looks like divine armor!" );
         act( buf, ch, obj, NULL, TO_CHAR );
         break;
      case ITEM_SCROLL:
      case ITEM_WAND:
      case ITEM_STAFF:
      case ITEM_PILL:
         lv = obj->value[0];
         if( lv < 10 )
            snprintf( buf, MSL, "$p doesn't look at all powerful." );
         else if( lv < 20 )
            snprintf( buf, MSL, "$p looks like it has a little power." );
         else if( lv < 40 )
            snprintf( buf, MSL, "$p appears to be quite powerful." );
         else if( lv < 60 )
            snprintf( buf, MSL, "$p almost bristles with power." );
         else if( lv < 80 )
            snprintf( buf, MSL, "$p crackles with pure energy!" );
         else
            snprintf( buf, MSL, "$p looks like it was divinely created!!" );
         act( buf, ch, obj, NULL, TO_CHAR );
         break;
      case ITEM_CONTAINER:
         hold = obj->value[0];
         if( hold < 10 )
            snprintf( buf, MSL, "$p couldn't hold an apple!" );
         else if( hold < 30 )
            snprintf( buf, MSL, "$p could hold a few items." );
         else if( hold < 50 )
            snprintf( buf, MSL, "$p could hold quite a few objects." );
         else if( hold < 100 )
            snprintf( buf, MSL, "$p looks like it could hold a LOT of stuff!" );
         else
            snprintf( buf, MSL, "$p looks like the creation of the Gods!" );
         act( buf, ch, obj, NULL, TO_CHAR );
         break;
      case ITEM_FURNITURE:
         act( "$p looks like furniture.", ch, obj, NULL, TO_CHAR );
         break;
      case ITEM_TRASH:
         act( "$p doesn't appear to do anything at all, really.", ch, obj, NULL, TO_CHAR );
         break;
      case ITEM_DRINK_CON:
         snprintf( buf, MSL, "$p looks like it has some %s in it.", liq_table[obj->value[2]].liq_name );
         act( buf, ch, obj, NULL, TO_CHAR );
         if( obj->value[3] != 0 )
            send_to_char( "It looks poisoned!\r\n", ch );
         break;
      case ITEM_KEY:
         act( "$p looks like it unlocks something.", ch, obj, NULL, TO_CHAR );
         break;
      case ITEM_FOOD:
         foo = obj->value[0];
         if( foo < 5 )
            snprintf( buf, MSL, "$p couldn't feed an ant!" );
         else if( foo < 10 )
            snprintf( buf, MSL, "$p would fill a very small stomach." );
         else if( foo < 20 )
            snprintf( buf, MSL, "$p looks quite filling." );
         else if( foo < 40 )
            snprintf( buf, MSL, "$p could fulfill most hungers." );
         else
            snprintf( buf, MSL, "$p looks VERY filling!" );
         act( buf, ch, obj, NULL, TO_CHAR );
         if( obj->value[3] != 0 )
            send_to_char( "It looks poisoned!\r\n", ch );
         break;
      case ITEM_MONEY:
         act( "$p is MONEY!!!", ch, obj, NULL, TO_CHAR );
         break;
      case ITEM_BOAT:
         act( "$p looks like a sturdy means of travelling across water.", ch, obj, NULL, TO_CHAR );
         break;
      case ITEM_CORPSE_PC:
      case ITEM_CORPSE_NPC:
         act( "$p is a corpse.  Anything worth stealing in it??", ch, obj, NULL, TO_CHAR );
         break;
      case ITEM_FOUNTAIN:
         act( "$p looks like a thirst-quenching fountain.", ch, obj, NULL, TO_CHAR );
         break;
      case ITEM_BOARD:
         act( "$p looks good for leaving messages on.", ch, obj, NULL, TO_CHAR );
         break;
      default:
         act( "You don't see anything special about $p.", ch, obj, NULL, TO_CHAR );
         break;
   }


   /*
    * Now show rough idea of the cost, etc 
    */

   if( obj->first_apply != NULL )
      act( "You are able to sense a strange power in $p.", ch, obj, NULL, TO_CHAR );
   gold = ( obj->cost );
   if( gold < 1000 )
      snprintf( buf, MSL, "$p doesn't look at all valuable." );
   else if( gold < 10000 )
      snprintf( buf, MSL, "$p looks like it has some value to it." );
   else if( gold < 50000 )
      snprintf( buf, MSL, "$p might fetch an average price." );
   else if( gold < 100000 )
      snprintf( buf, MSL, "$p would do well at auction." );
   else if( gold < 500000 )
      snprintf( buf, MSL, "$p looks VERY valuable." );
   else
      snprintf( buf, MSL, "$p looks priceless!!" );
   act( buf, ch, obj, NULL, TO_CHAR );
   return;
}

DO_FUN(do_bid)
{
   int amount;
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   if( IS_NPC( ch ) )
   {
      send_to_char( "Only players may use the auction system.\r\n", ch );
      return;
   }

   if( auction_item == NULL )
   {
      send_to_char( "Nothing is being auctioned right now!\r\n", ch );
      return;
   }

   if( ( amount = money_to_cost( argument ) ) < 0 )
   {
      send_to_char( "You must bid with real money!\r\n", ch );
      return;
   }


   if( amount <= auction_bid )
   {
      send_to_char( "That doesn't beat the latest bid.\r\n", ch );
      return;
   }

   if( ( amount > money_value( ch->money ) ) || ( money_to_value( ch, argument ) == -1 ) )
// This fix provided by Coldmyst
   {
      send_to_char( "You don't have that much money.\r\n", ch );
      return;
   }

   if( auction_bidder == ch )
   {
      send_to_char( "You made the last bid, silly!\r\n", ch );
      return;
   }

   if( auction_owner == ch )
   {
      send_to_char( "You can't bid for your own item!!\r\n", ch );
      return;
   }

   /*
    * Ok, so now set the variables to reflect the new bidder
    */
   /*
    * refund gold to last bidder, take gold from new one
    */

   if( auction_bidder != NULL )
      join_money( round_money( auction_bid, TRUE ), auction_bidder->money );
   auction_bidder = ch;
   auction_bid = amount;
   auction_stage = 0;
   auction_update(  );
   auction_flop = TRUE;
   return;
}

DO_FUN(do_auction)
{
   int auction_reserve = 0;
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_STRING_LENGTH];
   double reserve;
   void *vo = NULL;
   if( IS_NPC( ch ) )
      return;
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   argument = one_argument( argument, arg );
   if( ( IS_IMMORTAL( ch ) ) && ( ( !str_cmp( arg, "stop" ) ) || ( !str_cmp( arg, "take" ) ) ) )
      if( auction_item != NULL )
      {
         CHAR_DATA *ach;
         list<CHAR_DATA *>::iterator li;
         bool good_seller = FALSE;
         bool good_buyer = FALSE;
         for( li = char_list.begin(); li != char_list.end(); li++ )
         {
            ach = *li;
            if( auction_owner == ach )
               good_seller = TRUE;
            if( auction_bidder == ach )
               good_buyer = TRUE;
         }

         snprintf( buf, MSL, "The auction of %s has been stopped by an @@mImmortal@@N.\r\n", auction_item->short_descr );
         if( !str_cmp( arg, "take" ) )
         {
            char buf2[MSL];
            snprintf( buf2, MSL, " The item has been taken by %s.\r\n", ch->name.c_str() );
            strncat( buf, buf2, MSL-1 );
         }
         do_echo( ch, buf );
         if( good_seller )
         {
            if( !str_cmp( arg, "take" ) )
            {
               obj_to_char( auction_item, ch );
               send_to_char( "Your item has been claimed by the gods.\r\n", auction_owner );
            }
            else
            {
               obj_to_char( auction_item, auction_owner );
               send_to_char( "Your item has been returned.\r\n ", auction_owner );
            }

         }
         else
         {

            obj_to_char( auction_item, ch );
         }

         if( good_buyer )
         {
            send_to_char( "Your bid has been returned.\r\n", auction_bidder );
            join_money( round_money( auction_bid, TRUE ), auction_bidder->money );
         }


         auction_stage = 0;
         auction_bidder = NULL;
         auction_owner = NULL;
         auction_item = NULL;
         auction_reserve = 0;
         auction_bid = 0;
         return;
      }



   if( auction_item != NULL )
   {
      char lastbidbuf[MSL];
      snprintf( buf, MSL, "The current object being auctioned is: %s\r\n", auction_item->short_descr );
      send_to_char( buf, ch );
      snprintf( buf, MSL, "Item was offered for sale by %s.\r\n", auction_owner->name.c_str() );
      send_to_char( buf, ch );
      snprintf( lastbidbuf, MSL, "%s", cost_to_money( auction_bid ) );
      snprintf( buf, MSL, "The estimated value is %s, and last bid was for %s.\r\n",
               cost_to_money( ( int )auction_item->cost ), lastbidbuf );
      send_to_char( buf, ch );
      snprintf( buf, MSL, "The reserve price is @@W%s @@N.\r\n", cost_to_money( auction_reserve ) );
      send_to_char( buf, ch );
      vo = ( void * )auction_item;
      spell_identify( 0, LEVEL_HERO - 1, ch, vo, auction_item );
      return;
   }



   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "USAGE:  SYNTAX <object>  [reserve_price]\r\n", ch );
      return;
   }


   if( argument[0] != '\0' )
      reserve = money_to_cost( argument );
   else
      reserve = 0;
   if( reserve * .1 > money_value( ch->money ) )
   {
      send_to_char( "You don't have enough gold to cover the auction fee at that reserve price!\r\n", ch );
      return;
   }



   if( ( auction_item = get_obj_carry( ch, arg ) ) == NULL )
   {
      send_to_char( "You're not carrying that item!\r\n", ch );
      return;
   }

   if(  IS_OBJ_STAT(auction_item,ITEM_EXTRA_NO_AUCTION) )
   {
      send_to_char( "You can't auction that.  Sorry!\r\n", ch );
      auction_item = NULL;
      return;
   }

   switch ( auction_item->item_type )
   {
      case ITEM_CORPSE_PC:
      case ITEM_CORPSE_NPC:
         send_to_char( "You can't auction corpses.\r\n", ch );
         auction_item = NULL;
         return;
         break;
      case ITEM_TRASH:
         send_to_char( "Looks like trash.  Forget it!\r\n", ch );
         auction_item = NULL;
         return;
         break;
      case ITEM_DRINK_CON:
      case ITEM_FOOD:
         send_to_char( "Food or Drink can't be auctioned.\r\n", ch );
         auction_item = NULL;
         return;
         break;
      default:
         break;
   }

   if(  IS_OBJ_STAT(auction_item,ITEM_EXTRA_NO_DROP) )
   {
      send_to_char( "You can't let go of it!\r\n", ch );
      auction_item = NULL;
      return;
   }

   snprintf( buf, MSL, "You have placed %s up for auction.  %s @@Whas been charged for these services.\r\n",
            auction_item->name, cost_to_money( (int)(reserve * .1) ) );
   send_to_char( buf, ch );
   auction_owner = ch;
   auction_bidder = NULL;
   auction_bid = 0;
   auction_reserve = (int)reserve;
   if( auction_reserve > 0 )
      auction_bid = auction_reserve;
   auction_stage = 0;
   obj_from_char( auction_item );
   auction_update(  );
   auction_flop = TRUE;
   return;
}


DO_FUN(do_connect)
{
   OBJ_DATA *first_ob;
   OBJ_DATA *second_ob;
   OBJ_DATA *new_ob;
   char arg1[MAX_STRING_LENGTH], arg2[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "Your claws are too clumsy!!!@@N\r\n", ch );
      return;
   }

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "You must connect one piece to another.\r\n", ch );
      return;
   }

   if( ( first_ob = get_obj_carry( ch, arg1 ) ) == NULL )
   {
      snprintf( buf, MSL, "You aren't carrying that!!\r\n" );
      send_to_char( buf, ch );
      return;
   }



   if( ( second_ob = get_obj_carry( ch, arg2 ) ) == NULL )
   {
      snprintf( buf, MSL, "You aren't carrying that!!\r\n" );
      send_to_char( buf, ch );
      return;
   }

   if( first_ob->item_type != ITEM_PIECE || second_ob->item_type != ITEM_PIECE )
   {
      send_to_char( "Both items must be pieces of another item!\r\n", ch );
      return;
   }

   /*
    * check to see if the pieces connect 
    */

   if( ( first_ob->value[0] == second_ob->pIndexData->vnum ) || ( first_ob->value[1] == second_ob->pIndexData->vnum ) )
      /*
       * good connection  
       */
   {
      new_ob = create_object( get_obj_index( first_ob->value[2] ), ch->level );
      extract_obj( first_ob );
      extract_obj( second_ob );
      obj_to_char( new_ob, ch );
      act( "$n jiggles some pieces together, and suddenly they snap in place, creating $p! Perfect Fit!!", ch, new_ob,
           NULL, TO_ROOM );
      act( "You jiggle the pieces together, and suddenly they snap into place, creating $p! Nice job!", ch, new_ob, NULL,
           TO_CHAR );
   }
   else
   {
      act( "$n jiggles some pieces together, but can't seem to make them connect.", ch, NULL, NULL, TO_ROOM );
      act( "You try to fit them together every which way, but they just don't want to fit together.", ch, NULL, NULL,
           TO_CHAR );
      return;
   }

   return;
}

DO_FUN(do_repair)
{
 OBJ_DATA *obj;
 CHAR_DATA *mob;
 char *cbuf = '\0';
 char changebuf[MSL], mbuf[MSL];
 int cost = 0, change = 0;
 double mod = 1.05;
 bool found = FALSE;

 changebuf[0] = '\0';
 mbuf[0] = '\0';

 if( argument[0] == '\0' )
 {
  send_to_char("You can either repair a specific item or repair 'all'.\r\n",ch);
  return;
 }

 for( mob = ch->in_room->first_person; mob != NULL; mob = mob->next_in_room )
  if( IS_NPC(mob) && mob->npcdata->pIndexData->pShop != NULL )
   break;

 if( mob == NULL || !can_see(ch,mob) )
 {
  send_to_char("You need to be at a shop keeper to repair your equipment.\r\n",ch);
  return;
 }

 if( !str_cmp(argument,"all") )
 {
  for( obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list )
  {
   if( obj->durability < obj->max_durability )
   {
    found = TRUE;
    cost = static_cast<int>(((((obj->durability - obj->max_durability) * mod) * obj->level) * -1));
    if( money_value(ch->money) - cost < 0 )
    {
     act("You don't have enough money to repair $p.",ch,obj,NULL,TO_CHAR);
     continue;
    }
    obj->durability = obj->max_durability;
    cbuf = take_best_coins(ch->money,cost); /* Gives a string of the coins we need to steal. */
    cbuf = one_argument(cbuf,changebuf);
    change = is_number(changebuf) ? atoi(changebuf) : 0;
    snprintf(changebuf,MSL,"%s to %s",cbuf,mob->name.c_str());
    do_give(ch,changebuf);
    act("$N repairs $p.",ch,obj,mob,TO_CHAR);
    if( change > 0 )
    {
     MONEY_TYPE *transaction;
     transaction = round_money(change,TRUE);
     join_money(transaction,ch->money);
    }
   }
  }
  if( !found )
   send_to_char("None of your equipment needs repairing.\r\n",ch);
  return;
 }

 if( (obj = get_obj_carry(ch,argument)) == NULL && (obj = get_obj_wear(ch,argument)) == NULL )
 {
  send_to_char("You do not have that item.\r\n",ch);
  return;
 }
 if( obj->durability >= obj->max_durability )
 {
  send_to_char("That item is not in need of repair.\r\n",ch);
  return;
 }
 cost = static_cast<int>(((((obj->durability - obj->max_durability) * mod) * obj->level) * -1));
 if( money_value(ch->money) - cost < 0 )
 {
  act("You don't have enough money to repair $p.",ch,obj,NULL,TO_CHAR);
  return;
 }
 obj->durability = obj->max_durability;
 cbuf = take_best_coins(ch->money,cost); /* Gives a string of the coins we need to steal. */
 cbuf = one_argument(cbuf,changebuf);
 change = is_number(changebuf) ? atoi(changebuf) : 0;
 snprintf(changebuf,MSL,"%s to %s",cbuf,mob->name.c_str());
 do_give(ch,changebuf);
 act("$N repairs $p.",ch,obj,mob,TO_CHAR);
 if( change > 0 )
 {
  MONEY_TYPE *transaction;
  transaction = round_money(change,TRUE);
  join_money(transaction,ch->money);
 }
}
