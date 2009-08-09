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

#ifndef DEC_BUILDTAB_H
#include "h/buildtab.h"
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

#ifndef DEC_SPENDQP_H
#include "h/spendqp.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

DO_FUN(do_enchant)
{
   char arg1[MSL];
   char arg2[MSL];
   char debug[MSL];
   char msg_buf[MSL];
   char cat_buf[MSL];
   char enchant_buf[MSL];
   char enchant_catbuf[MSL];



   OBJ_DATA *matrix;
   OBJ_DATA *unique = NULL;
   OBJ_DATA *this_obj;

   AFFECT_DATA *this_aff;
   int cur_str = 0, cur_int = 0, cur_dex = 0, cur_wis = 0, cur_con = 0;
   int mod_str = 0, mod_int = 0, mod_dex = 0, mod_wis = 0, mod_con = 0;
   int cur_mana = 0, cur_hitroll = 0, cur_move = 0, cur_armor = 0, cur_save = 0, cur_hit = 0, cur_damroll = 0;
   int mod_mana = 0, mod_hitroll = 0, mod_move = 0, mod_armor = 0, mod_save = 0, mod_hit = 0, mod_damroll = 0;
   short mod_item_weight = 0;
   bool good_item = FALSE, legal_enchant = TRUE, bad_enchant_mix = FALSE;
   short qp_cost = 20;
   int mod = 0;
   short min_level = 10;
   int new_apply = ITEM_APPLY_NONE;
   bitset<MAX_BITSET> new_extras;

   new_extras.reset();

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( IS_NPC( ch ) )
   {
      send_to_char( "Not for Npcs.\r\n", ch );
      return;
   }

   if( arg1[0] == '\0' || ( !is_name( arg1, "show buy" ) ) )
   {
      send_to_char( "Syntax for enchanting objects:\r\n", ch );
      send_to_char( "  First, you must place at least one wearable item, and 1 or more enchantments\r\n", ch );
      send_to_char( "  into a spell matrix container, which you must hold.  Then, you must type \r\n", ch );
      send_to_char( "  enchant show to see the results of applying those enchantments to the item.  \r\n", ch );
      send_to_char( "  You may not enchant an item so that it's power would be greater than you can \r\n", ch );
      send_to_char( "  control at your current level.  Finally, once you are satisfied with the new \r\n", ch );
      send_to_char( "  item's powers, type enchant buy.\r\n", ch );
      return;
   }


/* first, we check for good enchantments and an item template in the matrix */

   if( ( matrix = get_eq_char( ch, WEAR_HOLD_HAND_R ) ) == NULL )
   {

      if( ( matrix = get_eq_char( ch, WEAR_HOLD_HAND_L ) ) == NULL )
      {
         send_to_char( "@@NYou must be holding a @@lspell matrix@@N for this ability!\r\n", ch );
         return;
      }
   }
   if( matrix->item_type != ITEM_SPELL_MATRIX )
   {
      send_to_char( "@@NThat is not a @@lspell matrix@@N you are holding, you fool!!\r\n", ch );
      return;
   }


/* now, we tear apart the enchantments, and put them in add_mod vars */

   for( this_obj = matrix->first_in_carry_list; this_obj != NULL; this_obj = this_obj->next_in_carry_list )
   {
      if( ( this_obj->item_type == ITEM_WEAPON )
          || ( this_obj->item_type == ITEM_ARMOR ) || ( this_obj->item_type == ITEM_LIGHT ) )
      {
         good_item = TRUE;
         unique = this_obj;
         continue;
      }
      if( this_obj->item_type != ITEM_ENCHANTMENT )
      {
         bad_enchant_mix = TRUE;
         if( !str_cmp( arg2, "debug" ) )
         {
            snprintf( debug, MSL, "Non-enchantment item in matrix..%s\r\n", this_obj->name );
            send_to_char( debug, ch );
         }
         break;
      }
      if( this_obj->value[0] == ENCHANT_OBJFUNS )
      {
         bad_enchant_mix = TRUE;
      }
      else if( this_obj->value[0] == ENCHANT_APPLY_FLAGS )
      {
         if( ( this_obj->value[3] > 0 ) && ( !IS_SET( new_apply, ( 1 << ( this_obj->value[3] - 1 ) ) ) ) )
            SET_BIT( new_apply, ( 1 << ( this_obj->value[3] - 1 ) ) );
      }
      else if( this_obj->value[0] == ENCHANT_EXTRA_FLAGS )
      {
         if( ( this_obj->value[3] > 0 ) && ( !new_extras.test(this_obj->value[3] - 1) ) )
            new_extras.set(this_obj->value[3] - 1);
      }
      else
      {

         mod = this_obj->value[1];
         switch ( this_obj->value[0] )
         {
            default:
               if( !str_cmp( arg2, "debug" ) )
               {
                  snprintf( debug, MSL, "Bad location in v0 of enchantment %s\r\n", this_obj->name );
                  send_to_char( debug, ch );
               }
               break;

            case APPLY_NONE:
               break;
            case APPLY_STR:
               mod_str += mod;
               break;
            case APPLY_DEX:
               mod_dex += mod;
               break;
            case APPLY_INT:
               mod_int += mod;
               break;
            case APPLY_WIS:
               mod_wis += mod;
               break;
            case APPLY_CON:
               mod_con += mod;
               break;
            case APPLY_SEX:
               break;
            case APPLY_CLASS:
               break;
            case APPLY_LEVEL:
               mod_item_weight -= 5;
               break;
            case APPLY_AGE:
               break;
            case APPLY_HEIGHT:
               break;
            case APPLY_WEIGHT:
               break;
            case APPLY_MANA:
               mod_mana += mod;
               break;
            case APPLY_HIT:
               mod_hit += mod;
               break;
            case APPLY_MOVE:
               mod_move += mod;
               break;
            case APPLY_EXP:
               break;
            case APPLY_AC:
               mod_armor += mod;
               break;
            case APPLY_HITROLL:
               mod_hitroll += mod;
               break;
            case APPLY_DAMROLL:
               mod_damroll += mod;
               break;
            case APPLY_SAVING_PARA:
               mod_save += mod;
               break;
            case APPLY_SAVING_ROD:
               mod_save += mod;
               break;
            case APPLY_SAVING_PETRI:
               mod_save += mod;
               break;
            case APPLY_SAVING_BREATH:
               mod_save += mod;
               break;
            case APPLY_SAVING_SPELL:
               mod_save += mod;
               break;
         }

      }
      qp_cost += this_obj->value[2];
   }

/* now, we have the enchantment totals, and qp costs, need to get the current non-spell affs */
/* now, we tear apart the current item's affs and store them in cur_mod vars */
   if( unique == NULL )
   {
      if( !str_cmp( arg2, "debug" ) )
      {
         snprintf( debug, MSL, "%s", "No item within matrix.\r\n" );
         send_to_char( debug, ch );
      }
      return;
   }
   if( ( ( ch->get_level("maxremortal") < unique->level )
         && ( IS_OBJ_STAT(unique,ITEM_EXTRA_REMORT) ) ) || ( ch->get_level("psuedo") < unique->level ) )
   {
      send_to_char( "You can't use this item in the first place..enchanting it is NOT going to help!\r\n", ch );
      return;
   }
   if( IS_OBJ_STAT(unique,ITEM_EXTRA_UNIQUE) )
      qp_cost -= 20;

   for( this_aff = unique->first_apply; this_aff != NULL; this_aff = this_aff->next )
   {
      if( this_aff->type != -1 )
         continue;

      mod = this_aff->modifier;
      switch ( this_aff->location )
      {
         default:
            if( !str_cmp( arg2, "debug" ) )
            {
               snprintf( debug, MSL, "Unknown aff on %s\r\n", unique->name );
               send_to_char( debug, ch );
            }
            break;

         case APPLY_NONE:
            break;
         case APPLY_STR:
            cur_str += mod;
            break;
         case APPLY_DEX:
            cur_dex += mod;
            break;
         case APPLY_INT:
            cur_int += mod;
            break;
         case APPLY_WIS:
            cur_wis += mod;
            break;
         case APPLY_CON:
            cur_con += mod;
            break;
         case APPLY_SEX:
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
            cur_mana += mod;
            break;
         case APPLY_HIT:
            cur_hit += mod;
            break;
         case APPLY_MOVE:
            cur_move += mod;
            break;
         case APPLY_EXP:
            break;
         case APPLY_AC:
            cur_armor += mod;
            break;
         case APPLY_HITROLL:
            cur_hitroll += mod;
            break;
         case APPLY_DAMROLL:
            cur_damroll += mod;
            break;
         case APPLY_SAVING_PARA:
            cur_save += mod;
            break;
         case APPLY_SAVING_ROD:
            cur_save += mod;
            break;
         case APPLY_SAVING_PETRI:
            cur_save += mod;
            break;
         case APPLY_SAVING_BREATH:
            cur_save += mod;
            break;
         case APPLY_SAVING_SPELL:
            cur_save += mod;
            break;
      }
   }

   if( !str_cmp( arg1, "show" ) )
   {
      snprintf( msg_buf, MSL, "Your enchantment will add the following to %s\r\n", unique->short_descr );

      if( new_extras.count() != 0 )
      {
         snprintf( cat_buf, MSL, "Extra Flags : %s\r\n", bs_show_values( tab_obj_flags, new_extras ) );
         strncat( msg_buf, cat_buf, MSL-1 );
      }
      if( new_apply != ITEM_APPLY_NONE )
      {
         snprintf( cat_buf, MSL, "Magic Flags : %s\r\n", bit_table_lookup( tab_item_apply, ( new_apply ) ) );
         strncat( msg_buf, cat_buf, MSL-1 );
      }

      if( mod_str != 0 )
      {
         snprintf( cat_buf, MSL, "Str: %d  Total: %d  ", mod_str, mod_str + cur_str );
         strncat( msg_buf, cat_buf, MSL-1 );
         if( mod_str + cur_str > 10 )
         {
            legal_enchant = FALSE;
            snprintf( cat_buf, MSL, "%s", "@@RILLEGAL@@N\r\n" );
         }
         else
         {
            snprintf( cat_buf, MSL, "%s", "\r\n" );
         }
         strncat( msg_buf, cat_buf, MSL-1 );
         min_level = UMAX( ( mod_str + cur_str ) * 10, min_level );

      }
      if( mod_int != 0 )
      {
         snprintf( cat_buf, MSL, "Int: %d  Total: %d  ", mod_int, mod_int + cur_int );
         strncat( msg_buf, cat_buf, MSL-1 );
         if( mod_int + cur_int > 10 )
         {
            legal_enchant = FALSE;
            snprintf( cat_buf, MSL, "%s", "@@RILLEGAL@@N\r\n" );
         }
         else
         {
            snprintf( cat_buf, MSL, "%s", "\r\n" );
         }
         strncat( msg_buf, cat_buf, MSL-1 );
         min_level = UMAX( ( mod_int + cur_int ) * 10, min_level );
      }
      if( mod_wis != 0 )
      {
         snprintf( cat_buf, MSL, "Wis: %d  Total: %d  ", mod_wis, mod_wis + cur_wis );
         strncat( msg_buf, cat_buf, MSL-1 );
         if( mod_wis + cur_wis > 10 )
         {
            legal_enchant = FALSE;
            snprintf( cat_buf, MSL, "%s", "@@RILLEGAL@@N\r\n" );
         }
         else
         {
            snprintf( cat_buf, MSL, "%s", "\r\n" );
         }
         strncat( msg_buf, cat_buf, MSL-1 );
         min_level = UMAX( ( mod_wis + cur_wis ) * 10, min_level );

      }
      if( mod_dex != 0 )
      {
         snprintf( cat_buf, MSL, "Dex: %d  Total: %d\r\n  ", mod_dex, mod_dex + cur_dex );
         strncat( msg_buf, cat_buf, MSL-1 );
         if( mod_dex + cur_dex > 10 )
         {
            legal_enchant = FALSE;
            snprintf( cat_buf, MSL, "%s", "@@RILLEGAL@@N\r\n" );
         }
         else
         {
            snprintf( cat_buf, MSL, "%s", "\r\n" );
         }
         strncat( msg_buf, cat_buf, MSL-1 );
         min_level = UMAX( ( mod_dex + cur_dex ) * 10, min_level );
      }
      if( mod_con != 0 )
      {
         snprintf( cat_buf, MSL, "Con: %d  Total: %d  ", mod_con, mod_con + cur_con );
         strncat( msg_buf, cat_buf, MSL-1 );
         if( mod_con + cur_con > 10 )
         {
            legal_enchant = FALSE;
            snprintf( cat_buf, MSL, "%s", "@@RILLEGAL@@N\r\n" );
         }
         else
         {
            snprintf( cat_buf, MSL, "%s", "\r\n" );
         }
         strncat( msg_buf, cat_buf, MSL-1 );
         min_level = UMAX( ( mod_con + cur_con ) * 10, min_level );
      }
      if( mod_hit != 0 )
      {
         snprintf( cat_buf, MSL, "Hit Points: %d  Total: %d  ", mod_hit, mod_hit + cur_hit );
         strncat( msg_buf, cat_buf, MSL-1 );
         if( mod_hit + cur_hit > 480 )
         {
            legal_enchant = FALSE;
            snprintf( cat_buf, MSL, "%s", "@@RILLEGAL@@N\r\n" );
         }
         else
         {
            snprintf( cat_buf, MSL, "%s", "\r\n" );
         }
         strncat( msg_buf, cat_buf, MSL-1 );
         min_level = UMAX( ( mod_hit + cur_hit ) / 4, min_level );
      }
      if( mod_mana != 0 )
      {
         snprintf( cat_buf, MSL, "Mana: %d  Total: %d  ", mod_mana, mod_mana + cur_mana );
         strncat( msg_buf, cat_buf, MSL-1 );
         if( mod_mana + cur_mana > 480 )
         {
            legal_enchant = FALSE;
            snprintf( cat_buf, MSL, "%s", "@@RILLEGAL@@N\r\n" );
         }
         else
         {
            snprintf( cat_buf, MSL, "%s", "\r\n" );
         }
         strncat( msg_buf, cat_buf, MSL-1 );
         min_level = UMAX( ( mod_mana + cur_mana ) / 4, min_level );
      }
      if( mod_move != 0 )
      {
         snprintf( cat_buf, MSL, "Move: %d  Total: %d  ", mod_move, mod_move + cur_move );
         strncat( msg_buf, cat_buf, MSL-1 );
         if( mod_move + cur_move > 240 )
         {
            legal_enchant = FALSE;
            snprintf( cat_buf, MSL, "%s", "@@RILLEGAL@@N\r\n" );
         }
         else
         {
            snprintf( cat_buf, MSL, "%s", "\r\n" );
         }
         strncat( msg_buf, cat_buf, MSL-1 );
         min_level = UMAX( ( mod_move + cur_move ) / 2 * 10, min_level );
      }

      if( mod_armor != 0 )
      {
         snprintf( cat_buf, MSL, "AC: %d  Total: %d  ", mod_armor, mod_armor + cur_armor );
         strncat( msg_buf, cat_buf, MSL-1 );
         if( mod_armor + cur_armor < -480 )
         {
            legal_enchant = FALSE;
            snprintf( cat_buf, MSL, "%s", "@@RILLEGAL@@N\r\n" );
         }
         else
         {
            snprintf( cat_buf, MSL, "%s", "\r\n" );
         }
         strncat( msg_buf, cat_buf, MSL-1 );
         min_level = UMAX( ( mod_armor + cur_armor ) / -4, min_level );

      }
      if( mod_hitroll != 0 )
      {
         snprintf( cat_buf, MSL, "Hitroll: %d  Total: %d  ", mod_hitroll, mod_hitroll + cur_hitroll );
         strncat( msg_buf, cat_buf, MSL-1 );
         if( mod_hitroll + cur_hitroll > 50 )
         {
            legal_enchant = FALSE;
            snprintf( cat_buf, MSL, "%s", "@@RILLEGAL@@N\r\n" );
         }
         else
         {
            snprintf( cat_buf, MSL, "%s", "\r\n" );
         }
         strncat( msg_buf, cat_buf, MSL-1 );
         min_level = UMAX( ( mod_hitroll + cur_hitroll ) * 2 + 20, min_level );
      }
      if( mod_damroll != 0 )
      {
         snprintf( cat_buf, MSL, "Damroll: %d  Total: %d  ", mod_damroll, mod_damroll + cur_damroll );
         strncat( msg_buf, cat_buf, MSL-1 );
         if( mod_damroll + cur_damroll > 50 )
         {
            legal_enchant = FALSE;
            snprintf( cat_buf, MSL, "%s", "@@RILLEGAL@@N\r\n" );
         }
         else
         {
            snprintf( cat_buf, MSL, "%s", "\r\n" );
         }
         strncat( msg_buf, cat_buf, MSL-1 );
         min_level = UMAX( ( mod_damroll + cur_damroll ) * 2 + 20, min_level );
      }
      if( mod_save != 0 )
      {
         snprintf( cat_buf, MSL, "Save vs Spell: %d  Total: %d  ", mod_save, mod_save + cur_save );
         strncat( msg_buf, cat_buf, MSL-1 );
         if( mod_save + cur_save < -36 )
         {
            legal_enchant = FALSE;
            snprintf( cat_buf, MSL, "%s", "@@RILLEGAL@@N\r\n" );
         }
         else
         {
            snprintf( cat_buf, MSL, "%s", "\r\n" );
         }
         strncat( msg_buf, cat_buf, MSL-1 );
         min_level = UMAX( ( mod_save + cur_save ) / -3 * 10, min_level );

      }
      if( mod_item_weight != 0 )
      {
         snprintf( cat_buf, MSL, "New item weight: %d  ", mod_item_weight + unique->weight );
         strncat( msg_buf, cat_buf, MSL-1 );
         if( mod_item_weight + unique->weight < 1 )
         {
            legal_enchant = FALSE;
            snprintf( cat_buf, MSL, "%s", "@@RILLEGAL@@N\r\n" );
         }
         else
         {
            snprintf( cat_buf, MSL, "%s", "\r\n" );
         }
         strncat( msg_buf, cat_buf, MSL-1 );

      }
      if( IS_OBJ_STAT(unique,ITEM_EXTRA_CLAN_EQ) )
      {
         snprintf( cat_buf, MSL, "%s is clan equipment, and may not be enchanted.\r\n", unique->short_descr );
         strncat( msg_buf, cat_buf, MSL-1 );
         legal_enchant = FALSE;
      }

      if( legal_enchant )
      {
         snprintf( cat_buf, MSL, "Total Cost: %d @@aqp@@N\r\n", qp_cost );
      }
      else
      {
         snprintf( cat_buf, MSL, "%s", "This is not a legal enchantment.  Remove some enchantments, and try again.\r\n" );
      }
      strncat( msg_buf, cat_buf, MSL-1 );
      if( legal_enchant )
      {
         if( ch->pcdata->quest_points >= qp_cost )

         {
            snprintf( cat_buf, MSL, "%s", "You may type enchant buy to purchase this upgrade.\r\n" );
         }
         else
         {
            snprintf( cat_buf, MSL, "%s", "You do not have enough quest points for this upgrade.\r\n" );
         }
         strncat( msg_buf, cat_buf, MSL-1 );
         if( ( min_level > unique->level ) && ( min_level > ch->get_level("psuedo") ) )
         {
            snprintf( cat_buf, MSL,
                     "@@eWARNING: @@WEnchanting %s with these affects will make the item level %d, which is higher than your current ability to use.\r\n",
                     unique->short_descr, min_level );
            strncat( msg_buf, cat_buf, MSL-1 );
         }
      }

      send_to_char( msg_buf, ch );
      return;
   }

   if( !str_cmp( arg1, "buy" ) )
   {
      AFFECT_DATA *this_aff_next;
      AFFECT_DATA *new_aff;
      OBJ_DATA *this_obj_next;

      if( ch->pcdata->quest_points < qp_cost )
      {
         send_to_char( "You don't have enough quest points for this enchantment.\r\n", ch );
         return;
      }
      if( qp_cost == 0 )
      {
         send_to_char( "Now that would be worthwhile. Let's reconsider, and try adding some enchantments!\r\n", ch );
         return;
      }
      if( ( mod_str != 0 ) && ( mod_str + cur_str > 10 ) )
      {
         legal_enchant = FALSE;
      }
      if( ( mod_int != 0 ) && ( mod_int + cur_int > 10 ) )
      {
         legal_enchant = FALSE;
      }
      if( ( mod_wis != 0 ) && ( mod_wis + cur_wis > 10 ) )
      {
         legal_enchant = FALSE;
      }
      if( ( mod_dex != 0 ) && ( mod_dex + cur_dex > 10 ) )
      {
         legal_enchant = FALSE;
      }
      if( ( mod_con != 0 ) && ( mod_con + cur_con > 10 ) )
      {
         legal_enchant = FALSE;
      }
      if( ( mod_hit != 0 ) && ( mod_hit + cur_hit > 480 ) )
      {
         legal_enchant = FALSE;
      }
      if( ( mod_mana != 0 ) && ( mod_mana + cur_mana > 480 ) )
      {
         legal_enchant = FALSE;
      }
      if( ( mod_move != 0 ) && ( mod_move + cur_move > 240 ) )
      {
         legal_enchant = FALSE;
      }
      if( ( mod_armor != 0 ) && ( mod_armor + cur_armor < -480 ) )
      {
         legal_enchant = FALSE;
      }
      if( ( mod_hitroll != 0 ) && ( mod_hitroll + cur_hitroll > 50 ) )
      {
         legal_enchant = FALSE;
      }
      if( ( mod_damroll != 0 ) && ( mod_damroll + cur_damroll > 50 ) )
      {
         legal_enchant = FALSE;
      }
      if( ( mod_save != 0 ) && ( mod_save + cur_save < -36 ) )
      {
         legal_enchant = FALSE;
      }
      if( ( mod_item_weight != 0 ) && ( mod_item_weight + unique->weight < 1 ) )
      {
         legal_enchant = FALSE;
      }
      if( IS_OBJ_STAT(unique,ITEM_EXTRA_CLAN_EQ) )
      {
         legal_enchant = FALSE;
      }

      if( legal_enchant == FALSE )
      {
         do_enchant( ch, "show" );
         return;
      }
      for( this_aff = unique->first_apply; this_aff != NULL; this_aff = this_aff_next )
      {
         this_aff_next = this_aff->next;
         if( this_aff->type != -1 )
            continue;
         UNLINK( this_aff, unique->first_apply, unique->last_apply, next, prev );
         delete this_aff;
      }
      if( mod_str + cur_str != 0 )
      {
         new_aff = new AFFECT_DATA;
         new_aff->location = APPLY_STR;
         new_aff->modifier = mod_str + cur_str;
         LINK( new_aff, unique->first_apply, unique->last_apply, next, prev );
         if( mod_str != 0 )
            min_level = UMAX( ( mod_str + cur_str ) * 10, min_level );
      }
      if( mod_int + cur_int != 0 )
      {
         new_aff = new AFFECT_DATA;
         new_aff->location = APPLY_INT;
         new_aff->modifier = mod_int + cur_int;
         LINK( new_aff, unique->first_apply, unique->last_apply, next, prev );
         if( mod_int != 0 )
            min_level = UMAX( ( mod_int + cur_int ) * 10, min_level );

      }
      if( mod_wis + cur_wis != 0 )
      {
         new_aff = new AFFECT_DATA;
         new_aff->location = APPLY_WIS;
         new_aff->modifier = mod_wis + cur_wis;
         LINK( new_aff, unique->first_apply, unique->last_apply, next, prev );
         if( mod_wis != 0 )
            min_level = UMAX( ( mod_wis + cur_wis ) * 10, min_level );

      }
      if( mod_dex + cur_dex != 0 )
      {
         new_aff = new AFFECT_DATA;
         new_aff->location = APPLY_DEX;
         new_aff->modifier = mod_dex + cur_dex;
         LINK( new_aff, unique->first_apply, unique->last_apply, next, prev );
         if( mod_dex != 0 )
            min_level = UMAX( ( mod_dex + cur_dex ) * 10, min_level );

      }
      if( mod_con + cur_con != 0 )
      {
         new_aff = new AFFECT_DATA;
         new_aff->location = APPLY_CON;
         new_aff->modifier = mod_con + cur_con;
         LINK( new_aff, unique->first_apply, unique->last_apply, next, prev );
         if( mod_con != 0 )
            min_level = UMAX( ( mod_con + cur_con ) * 10, min_level );

      }
      if( mod_hit + cur_hit != 0 )
      {
         new_aff = new AFFECT_DATA;
         new_aff->location = APPLY_HIT;
         new_aff->modifier = mod_hit + cur_hit;
         LINK( new_aff, unique->first_apply, unique->last_apply, next, prev );
         if( mod_hit != 0 )
            min_level = UMAX( ( mod_hit + cur_hit ) / 4, min_level );

      }
      if( mod_mana + cur_mana != 0 )
      {
         new_aff = new AFFECT_DATA;
         new_aff->location = APPLY_MANA;
         new_aff->modifier = mod_mana + cur_mana;
         LINK( new_aff, unique->first_apply, unique->last_apply, next, prev );
         if( mod_mana != 0 )
            min_level = UMAX( ( mod_mana + cur_mana ) / 4, min_level );

      }
      if( mod_move + cur_move != 0 )
      {
         new_aff = new AFFECT_DATA;
         new_aff->location = APPLY_MOVE;
         new_aff->modifier = mod_move + cur_move;
         LINK( new_aff, unique->first_apply, unique->last_apply, next, prev );
         if( mod_move != 0 )
            min_level = UMAX( ( mod_move + cur_move ) / 2 * 10, min_level );

      }
      if( mod_hitroll + cur_hitroll != 0 )
      {
         new_aff = new AFFECT_DATA;
         new_aff->location = APPLY_HITROLL;
         new_aff->modifier = mod_hitroll + cur_hitroll;
         LINK( new_aff, unique->first_apply, unique->last_apply, next, prev );
         if( mod_hitroll != 0 )
            min_level = UMAX( ( mod_hitroll + cur_hitroll ) * 2 + 20, min_level );

      }
      if( mod_damroll + cur_damroll != 0 )
      {
         new_aff = new AFFECT_DATA;
         new_aff->location = APPLY_DAMROLL;
         new_aff->modifier = mod_damroll + cur_damroll;
         LINK( new_aff, unique->first_apply, unique->last_apply, next, prev );
         if( mod_damroll != 0 )
            min_level = UMAX( ( mod_damroll + cur_damroll ) * 2 + 20, min_level );

      }
      if( mod_save + cur_save != 0 )
      {
         new_aff = new AFFECT_DATA;
         new_aff->location = APPLY_SAVING_SPELL;
         new_aff->modifier = mod_save + cur_save;
         LINK( new_aff, unique->first_apply, unique->last_apply, next, prev );
         if( mod_save != 0 )
            min_level = UMAX( ( mod_save + cur_save ) / -3 * 10, min_level );

      }
      if( mod_armor + cur_armor != 0 )
      {
         new_aff = new AFFECT_DATA;
         new_aff->location = APPLY_AC;
         new_aff->modifier = mod_armor + cur_armor;
         LINK( new_aff, unique->first_apply, unique->last_apply, next, prev );
         if( mod_armor != 0 )
            min_level = UMAX( ( mod_armor + cur_armor ) / -4, min_level );

      }
      if( !IS_OBJ_STAT(unique,ITEM_EXTRA_UNIQUE) )
         unique->extra_flags.set(ITEM_EXTRA_UNIQUE);
      if( IS_OBJ_STAT(unique,ITEM_EXTRA_REMORT) )
      {
         min_level = UMAX( 80 + ( unique->level / 4 ), min_level );
         unique->extra_flags.reset(ITEM_EXTRA_REMORT);
      }
      if( mod_item_weight + unique->weight > 0 )
         unique->weight = mod_item_weight + unique->weight;
      for( short i = 0; i < MAX_BITSET; i++ )
       if( unique->extra_flags.test(i) && !new_extras.test(i) )
        new_extras.set(i);
      new_apply = new_apply | unique->item_apply;

      if( unique->extra_flags != new_extras )
         unique->extra_flags = new_extras;
      if( unique->item_apply != new_apply )
         unique->item_apply = new_apply;


      ch->pcdata->quest_points = ch->pcdata->quest_points - qp_cost;
      snprintf( enchant_buf, MSL, "%s", "Enchantments Used: " );
      for( this_obj = matrix->first_in_carry_list; this_obj != NULL; this_obj = this_obj_next )
      {
         this_obj_next = this_obj->next_in_carry_list;
         if( this_obj->item_type == ITEM_ENCHANTMENT )
         {
            snprintf( enchant_catbuf, MSL, "%s (%d) ", this_obj->name, this_obj->pIndexData->vnum );
            strncat( enchant_buf, enchant_catbuf, MSL-1 );
            extract_obj( this_obj );
         }
         strncat( enchant_buf, "\r\n", MSL-1 );
      }
      if( unique->level < min_level )
         unique->level = UMIN( 120, min_level );

      act( "$n dances about wildly, chanting wierd mantras, and gestures crazily over $p.", ch, matrix, NULL, TO_ROOM );
      act( "You enchant $p with additional powers!", ch, unique, NULL, TO_CHAR );
      do_save( ch, "auto" );
      {
         BRAND_DATA *brand;
         DL_LIST *brand_member;
         char brandbuf[MSL];
         char cat2_buf[MSL];
         AFFECT_DATA *one_aff;
         char *strtime;

         snprintf( brandbuf, MSL, "UNIQUE ITEM: keyword: %s, Name: %s, flags: %s \r\n level: %d, affects:\r\n",
                  unique->name, unique->short_descr, extra_bit_name( unique->extra_flags ), unique->level );
         for( one_aff = unique->first_apply; one_aff != NULL; one_aff = one_aff->next )
         {
            if( one_aff->location != APPLY_NONE && one_aff->modifier != 0 )
            {
               snprintf( cat2_buf, MSL, "Affects %s by %d.\r\n", affect_loc_name( one_aff->location ), one_aff->modifier );
               strncat( brandbuf, cat2_buf, MSL-1 );

            }
         }
         snprintf( cat2_buf, MSL, "Total cost: %d qp\r\n", qp_cost );
         strncat( brandbuf, cat2_buf, MSL-1 );
         strncat( brandbuf, enchant_buf, MSL-1 );
         brand = new BRAND_DATA;
         GET_FREE( brand_member, dl_list_free );
         brand->branded = str_dup( ch->name.c_str() );
         brand->branded_by = str_dup( "@@rSystem@@N" );
         brand->priority = str_dup( "unique" );
         brand->message = str_dup( brandbuf );
         strtime = ctime( &current_time );
         strtime[strlen( strtime ) - 1] = '\0';
         brand->dt_stamp = str_dup( strtime );
         brand_member->next = NULL;
         brand_member->prev = NULL;
         brand_member->this_one = brand;
         LINK( brand_member, first_brand, last_brand, next, prev );
         save_brands(  );
         send_to_char( "Your messages have been updated, and logged for inspection by an Immortal.\r\n", ch );
      }
      return;
   }

   do_enchant( ch, "" );
   return;
}

/* now, we make sure this is a legal enchantment  */

/* if everything is good, and they want to buy, we add effects to item, remove enchantments
   from bag  */
