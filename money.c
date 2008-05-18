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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"

/*
 * money.c 
 */
#ifndef DEC_MONEY_H
#include "money.h"
#endif

const struct currency_type currency_table[MAX_CURRENCY] = {
   {"bits", "@@bcopper@@N Bit", "@@bcopper@@N Bits", .01, 1},
   {"moons", "@@Wsilver@@N Moon", "@@Wsilver@@N Moons", .01, 5},
   {"crowns", "@@ygold@@N Crown", "@@ygold@@N Crowns", .05, 20},
   {"crescents", "@@lelectrum@@N Crescent", "@@lelectrum@@N Crescents", .1, 50},
   {"pentacles", "@@amithril@@N Pentacle", "@@amithril@@N Pentacles", .2, 100},
   {"karant", "@@mmalachite@@N Karant", "@@mmalachite@@N Karants", .2, 200},
   {"marks", "@@debony@@N Mark", "@@debony@@N Marks", .2, 500},
   {"sunbursts", "@@pRoyal Sunburst@@N", "@@pRoyal Sunbursts@@N", .5, 1000}
};

sh_int money_lookup( char *money_name )
{
   sh_int mn;
   for( mn = 0; mn < MAX_CURRENCY; mn++ )
   {
      if( ( !str_prefix( money_name, currency_table[mn].keyword ) )
          || ( !str_cmp( money_name, currency_table[mn].keyword ) ) )
         break;
   }
   if( mn == MAX_CURRENCY )
      return -1;
   else
      return mn;
}

MONEY_TYPE *join_money( MONEY_TYPE * source, MONEY_TYPE * receiver )
{
   sh_int looper;
   for( looper = 0; looper < MAX_CURRENCY; looper++ )
   {
      receiver->cash_unit[looper] += source->cash_unit[looper];
   }
   PUT_FREE( source, money_type_free );
   return receiver;
}

float money_weight( MONEY_TYPE * money )
{
   float weight = 0.0;
   sh_int looper;
   for( looper = 0; looper < MAX_CURRENCY; looper++ )
   {
      weight += ( money->cash_unit[looper] * currency_table[looper].unit_weight );
   }
   return weight;
}

char *cost_to_money( int cost )
{
   static char outbuf[MSL];
   MONEY_TYPE *money;
   money = round_money( cost, TRUE );
   sprintf( outbuf, "%s", money_string( money ) );
   PUT_FREE( money, money_type_free );
   return outbuf;
}

int money_to_cost( char *money_list )
{
   char *parse = money_list;
   char numbuf[MSL];
   char coinbuf[MSL];
   bool valid = TRUE;
   int value = 0;
   int unit;
   sh_int coin;
   for( ;; )
   {
      parse = one_argument( parse, numbuf );
      if( numbuf[0] == '\0' )
         break;
      if( is_number( numbuf ) )
      {
         unit = atoi( numbuf );
         parse = one_argument( parse, coinbuf );
         if( coinbuf[0] == '\0' )
            break;
      }
      else
      {
         unit = 1;
         sprintf( coinbuf, "%s", numbuf );
      }
      coin = money_lookup( coinbuf );
      if( coin == -1 )
      {
         valid = FALSE;
         break;
      }
      else
      {
         value += ( unit * currency_table[coin].exchange_val );
      }
   }
   if( valid )
      return ( value );
   else
      return -1;
}

MONEY_TYPE *round_money( int base, bool round_up )
{
   MONEY_TYPE *money;
   int money_left = base;
   int unit_worth;
   int looper;
   GET_FREE( money, money_type_free );
#ifdef DEBUG_MONEY
   money->money_key = str_dup( "Rounding money" );
#endif
   switch ( round_up )
   {
      case TRUE:
      {
         for( looper = MAX_CURRENCY - 1; looper >= 0; looper-- )
         {
            if( ( unit_worth = ( money_left / currency_table[looper].exchange_val ) ) > 0 )
            {
               money->cash_unit[looper] = unit_worth;
               money_left = ( money_left % currency_table[looper].exchange_val );
/*          if ( money_left == 0 )
          break;  */
            }
            else
            {
               money->cash_unit[looper] = 0;
            }
         }
         break;
      }
      case FALSE:   /* NOTE: this will always turn base into your smallest unit, just here for 
                      * completeness 
                      */
      {
         for( looper = 0; looper < MAX_CURRENCY; looper++ )
         {
            if( ( unit_worth = ( money_left / currency_table[looper].exchange_val ) ) > 0 )
            {
               money->cash_unit[looper] = unit_worth;
               money_left = money_left % currency_table[looper].exchange_val;
            }
            else
            {
               money->cash_unit[looper] = 0;
            }
         }
         break;
      }
   }  /* end switch */
   return money;
}

MONEY_TYPE *round_money_off( int base, sh_int accuracy )
{
   MONEY_TYPE *money;
   int money_left = base;
   int unit_worth;
   int looper;
   GET_FREE( money, money_type_free );
#ifdef DEBUG_MONEY
   money->money_key = str_dup( "Round_money_off" );
#endif
   for( looper = MAX_CURRENCY - 1; ( looper >= 0 ) && ( accuracy >= 0 ); looper-- )
   {
      if( ( unit_worth = ( money_left / currency_table[looper].exchange_val ) ) > 0 )
      {
         money->cash_unit[looper] = unit_worth;
         money_left = ( money_left % currency_table[looper].exchange_val );
         accuracy--;
      }
      else
      {
         money->cash_unit[looper] = 0;
      }
   }
   return money;
}

int money_value( MONEY_TYPE * money )
{
   int base_val = 0;
   sh_int looper;
   for( looper = 0; looper < MAX_CURRENCY; looper++ )
   {
      base_val += money->cash_unit[looper] * currency_table[looper].exchange_val;
   }
   return base_val;
}

char *money_string( MONEY_TYPE * money )
{
   static char outbuf[MSL];
   char catbuf[MSL];
   sh_int looper;
   sh_int last_found = -1;
   bool multiple = FALSE;
   bool first = TRUE;
   for( looper = MAX_CURRENCY - 1; looper >= 0; looper-- )
   {
      if( money->cash_unit[looper] > 0 )
      {
         if( last_found > -1 )
         {
            multiple = TRUE;
            last_found = looper;
         }
         else
         {
            last_found = looper;
         }
      }
   }
   outbuf[0] = '\0';
   for( looper = MAX_CURRENCY - 1; looper >= 0; looper-- )
   {
      if( money->cash_unit[looper] > 0 )
      {
         sprintf( catbuf, "%s%d %s", ( !( first ) ? ( multiple && ( last_found == looper ) ? ", and " : ", " ) : "" ), money->cash_unit[looper], ( money->cash_unit[looper] == 1 ? currency_table[looper].singular : currency_table[looper].plural )  /* ,
                                                                                                                                                                                                                                                       * ( last_found == looper ?
                                                                                                                                                                                                                                                       * "" :
                                                                                                                                                                                                                                                       * " " ) */  );
         safe_strcat( MSL, outbuf, catbuf );
         first = FALSE;
      }
   }
   return outbuf;
}

char *unit_string( MONEY_TYPE * money )
{
   static char outbuf[MSL];
   char catbuf[MSL];
   sh_int looper;

   outbuf[0] = '\0';
   for( looper = MAX_CURRENCY - 1; looper >= 0; looper-- )
   {
      if( money->cash_unit[looper] > 0 )
      {
         sprintf( catbuf, "%d %s ", money->cash_unit[looper], currency_table[looper].keyword );
         safe_strcat( MSL, outbuf, catbuf );
      }
   }
   return outbuf;
}

bool give_money( CHAR_DATA * ch, CHAR_DATA * victim, char *argument )
{
   MONEY_TYPE *transfer;
   sh_int looper;
   char m_number[MSL];
   char m_name[MSL];
   char outbuf[MSL];

   GET_FREE( transfer, money_type_free );
#ifdef DEBUG_MONEY
   {
      char testbuf[MSL];
      sprintf( testbuf, "give_money, %s, %s, %s", ch->name, victim->name, argument );
      transfer->money_key = str_dup( testbuf );
   }
#endif
   for( looper = 0; looper < MAX_CURRENCY; looper++ )
   {
      transfer->cash_unit[looper] = 0;
   }
   for( ;; )
   {
      sh_int mn;
      argument = one_argument( argument, m_number );
      if( m_number[0] == '\0' )
         break;
      argument = one_argument( argument, m_name );
      if( m_name[0] == '\0' )
         break;
      if( ( ( mn = money_lookup( m_name ) ) < 0 ) || ( !is_number( m_number ) ) )
      {
         sprintf( outbuf, "%s %s isn't a valid money type!\n\r", m_number, m_name );
         send_to_char( outbuf, ch );
         join_money( transfer, ch->money );
         return FALSE;
      }
      if( ch->money->cash_unit[mn] < atoi( m_number ) )
      {
         sprintf( outbuf, "You don't have %s %s!\n\r", m_number, m_name );
         send_to_char( outbuf, ch );
         join_money( transfer, ch->money );
         return FALSE;
      }
      ch->money->cash_unit[mn] -= atoi( m_number );
      transfer->cash_unit[mn] += atoi( m_number );
   }
   if( ( victim->carry_weight + money_weight( transfer ) ) > can_carry_w( victim ) )
   {
      sprintf( outbuf, "%s cannot carry that much weight!\n\r", PERS( ch, victim ) );
      send_to_char( outbuf, ch );
      join_money( transfer, ch->money );
      return FALSE;
   }
   ch->carry_weight -= money_weight( transfer );
   victim->carry_weight += money_weight( transfer );
   join_money( transfer, victim->money );
   return TRUE;
}


bool withdraw_money( CHAR_DATA * ch, char *argument )
{
   MONEY_TYPE *transfer;
   sh_int looper;
   char m_number[MSL];
   char m_name[MSL];
   char outbuf[MSL];

   GET_FREE( transfer, money_type_free );
#ifdef DEBUG_MONEY
   {
      char testbuf[MSL];
      sprintf( testbuf, "withdraw_money, %s, %s", ch->name, argument );
      transfer->money_key = str_dup( testbuf );
   }
#endif

   for( looper = 0; looper < MAX_CURRENCY; looper++ )
   {
      transfer->cash_unit[looper] = 0;
   }
   for( ;; )
   {
      sh_int mn;
      argument = one_argument( argument, m_number );
      if( m_number[0] == '\0' )
         break;
      argument = one_argument( argument, m_name );
      if( m_name[0] == '\0' )
         break;
      if( ( ( mn = money_lookup( m_name ) ) < 0 ) || ( !is_number( m_number ) ) )
      {
         sprintf( outbuf, "%s %s isn't a valid money type!\n\r", m_number, m_name );
         send_to_char( outbuf, ch );
         join_money( transfer, ch->bank_money );
         return FALSE;
      }
      if( ch->bank_money->cash_unit[mn] < atoi( m_number ) )
      {
         sprintf( outbuf, "You don't have %s %s in the bank!\n\r", m_number, m_name );
         send_to_char( outbuf, ch );
         join_money( transfer, ch->bank_money );
         return FALSE;
      }
      ch->bank_money->cash_unit[mn] -= atoi( m_number );
      transfer->cash_unit[mn] += atoi( m_number );
   }
   if( ( ch->carry_weight + money_weight( transfer ) ) > can_carry_w( ch ) )
   {
      sprintf( outbuf, "%s", "You cannot carry that much weight!\n\r" );
      send_to_char( outbuf, ch );
      join_money( transfer, ch->bank_money );
      return FALSE;
   }
   ch->carry_weight += money_weight( transfer );
   join_money( transfer, ch->money );
   return TRUE;
}

void deposit_money( CHAR_DATA * ch, char *argument )
{
   MONEY_TYPE *transfer;
   sh_int looper;
   char m_number[MSL];
   char m_name[MSL];
   char outbuf[MSL];

   GET_FREE( transfer, money_type_free );
#ifdef DEBUG_MONEY
   {
      char testbuf[MSL];
      sprintf( testbuf, "deposit_money, %s, %s", ch->name, argument );
      transfer->money_key = str_dup( testbuf );
   }
#endif

   for( looper = 0; looper < MAX_CURRENCY; looper++ )
   {
      transfer->cash_unit[looper] = 0;
   }
   for( ;; )
   {
      sh_int mn;
      argument = one_argument( argument, m_number );
      if( m_number[0] == '\0' )
         break;
      argument = one_argument( argument, m_name );
      if( m_name[0] == '\0' )
         break;
      if( ( ( mn = money_lookup( m_name ) ) < 0 ) || ( !is_number( m_number ) ) )
      {
         sprintf( outbuf, "%s %s isn't a valid money type!\n\r", m_number, m_name );
         send_to_char( outbuf, ch );
         join_money( transfer, ch->money );
         return;
      }
      if( ch->money->cash_unit[mn] < atoi( m_number ) )
      {
         sprintf( outbuf, "You don't have %s %s!\n\r", m_number, m_name );
         send_to_char( outbuf, ch );
         join_money( transfer, ch->money );
         return;
      }
      ch->money->cash_unit[mn] -= atoi( m_number );
      transfer->cash_unit[mn] += atoi( m_number );
   }
   ch->carry_weight -= money_weight( transfer );
   join_money( transfer, ch->bank_money );
   return;
}

int exchange_money( CHAR_DATA * ch, char *argument )
{
   MONEY_TYPE *transfer;
   sh_int looper;
   char m_number[MSL];
   char m_name[MSL];
   char outbuf[MSL];
   int base_val;
   int taxed = 0;
   GET_FREE( transfer, money_type_free );
#ifdef DEBUG_MONEY
   {
      char testbuf[MSL];
      sprintf( testbuf, "exchange_money, %s, %s", ch->name, argument );
      transfer->money_key = str_dup( testbuf );
   }
#endif

   for( looper = 0; looper < MAX_CURRENCY; looper++ )
   {
      transfer->cash_unit[looper] = 0;
   }
   for( ;; )
   {
      sh_int mn;
      argument = one_argument( argument, m_number );
      if( m_number[0] == '\0' )
         break;
      argument = one_argument( argument, m_name );
      if( m_name[0] == '\0' )
         break;
      if( ( ( mn = money_lookup( m_name ) ) < 0 ) || ( !is_number( m_number ) ) )
      {
         sprintf( outbuf, "%s %s isn't a valid money type!\n\r", m_number, m_name );
         send_to_char( outbuf, ch );
         join_money( transfer, ch->money );
         return -1;
      }
      if( ch->money->cash_unit[mn] < atoi( m_number ) )
      {
         sprintf( outbuf, "You don't have %s %s!\n\r", m_number, m_name );
         send_to_char( outbuf, ch );
         join_money( transfer, ch->money );
         return -1;
      }
      ch->money->cash_unit[mn] -= atoi( m_number );
      transfer->cash_unit[mn] += atoi( m_number );
   }
   ch->carry_weight -= money_weight( transfer );
   base_val = money_value( transfer );
   taxed = EXCHANGE_COST * base_val;
   base_val -= taxed;
   PUT_FREE( transfer, money_type_free );
   transfer = round_money( base_val, TRUE );
   ch->carry_weight += money_weight( transfer );
   join_money( transfer, ch->money );
   return taxed;
}

void do_mgive( CHAR_DATA * ch, char *argument )
{
   char v_name[MSL];
   CHAR_DATA *victim;
   bool good_give = FALSE;
   char arg1[MSL];
   argument = one_argument( argument, v_name );

   if( ( victim = get_char_room( ch, v_name ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }
   one_argument( argument, arg1 );
   if( !str_cmp( arg1, "create" ) )
   {
      char m_name[MSL];
      char m_number[MSL];
      sh_int mn;
      sh_int cnt;
      MONEY_TYPE *transfer;
      if( get_trust( ch ) < 84 )
      {
         send_to_char( "Huh?\n\r", ch );
         return;
      }
      argument = one_argument( argument, arg1 );
      argument = one_argument( argument, m_number );
      argument = one_argument( argument, m_name );
      if( ( mn = money_lookup( m_name ) ) < 0 )
      {
         send_to_char( "No such money unit.\n\r", ch );
         return;
      }
      GET_FREE( transfer, money_type_free );
#ifdef DEBUG_MONEY
      {
         char testbuf[MSL];
         sprintf( testbuf, "mgive, %s, %s", ch->name, argument );
         transfer->money_key = str_dup( testbuf );
      }
#endif

      for( cnt = 0; cnt < MAX_CURRENCY; cnt++ )
         transfer->cash_unit[cnt] = 0;
      transfer->cash_unit[mn] += ( is_number( m_number ) ? atoi( m_number ) : 10 );
      victim->carry_weight += money_weight( transfer );
      join_money( transfer, victim->money );
      send_to_char( "Done.\n\r", ch );
      return;
   }
   if( !str_cmp( arg1, "exchange" ) )
   {
      int m_val;
      if( get_trust( ch ) < 84 )
      {
         send_to_char( "Huh?\n\r", ch );
         return;
      }
      m_val = money_value( victim->money );
      victim->carry_weight -= money_weight( victim->money );
      PUT_FREE( victim->money, money_type_free );
      victim->money = round_money( m_val, TRUE );
      victim->carry_weight += money_weight( victim->money );
      send_to_char( "Done.\n\r", ch );
      return;
   }
   good_give = give_money( ch, victim, argument );
   if( good_give )
      send_to_char( "Gave it to them.\n\r", ch );
   else
      send_to_char( "Failed give.\n\r", ch );
   return;
}

void drop_money( CHAR_DATA * ch, char *argument )
{
   MONEY_TYPE *transfer;
   sh_int looper;
   char m_number[MSL];
   char m_name[MSL];
   char outbuf[MSL];

   GET_FREE( transfer, money_type_free );
#ifdef DEBUG_MONEY
   {
      char testbuf[MSL];
      sprintf( testbuf, "drop_money, %s, %s", ch->name, argument );
      transfer->money_key = str_dup( testbuf );
   }
#endif
   for( looper = 0; looper < MAX_CURRENCY; looper++ )
   {
      transfer->cash_unit[looper] = 0;
   }
   for( ;; )
   {
      sh_int mn;
      argument = one_argument( argument, m_number );
      if( m_number[0] == '\0' )
         break;
      argument = one_argument( argument, m_name );
      if( m_name[0] == '\0' )
         break;
      if( ( ( mn = money_lookup( m_name ) ) < 0 ) || ( !is_number( m_number ) ) )
      {
         sprintf( outbuf, "%s %s isn't a valid money type!\n\r", m_number, m_name );
         send_to_char( outbuf, ch );
         join_money( transfer, ch->money );
         return;
      }
      if( ch->money->cash_unit[mn] < atoi( m_number ) )
      {
         sprintf( outbuf, "You don't have %s %s!\n\r", m_number, m_name );
         send_to_char( outbuf, ch );
         join_money( transfer, ch->money );
         return;
      }
      ch->money->cash_unit[mn] -= atoi( m_number );
      transfer->cash_unit[mn] += atoi( m_number );
   }
   ch->carry_weight -= money_weight( transfer );
   join_money( transfer, ch->in_room->treasure );
   return;
}

int money_to_value( CHAR_DATA * ch, char *argument )
{
/* if successful, takes money from player */
   MONEY_TYPE *transfer;
   sh_int looper;
   char m_number[MSL];
   char m_name[MSL];
   char outbuf[MSL];
   int value = 0;
   GET_FREE( transfer, money_type_free );
#ifdef DEBUG_MONEY
   {
      char testbuf[MSL];
      sprintf( testbuf, "money_to_value, %s, %s", ch->name, argument );
      transfer->money_key = str_dup( testbuf );
   }
#endif
   for( looper = 0; looper < MAX_CURRENCY; looper++ )
   {
      transfer->cash_unit[looper] = 0;
   }
   for( ;; )
   {
      sh_int mn;
      argument = one_argument( argument, m_number );
      if( m_number[0] == '\0' )
         break;
      argument = one_argument( argument, m_name );
      if( m_name[0] == '\0' )
         break;
      if( ( ( mn = money_lookup( m_name ) ) < 0 ) || ( !is_number( m_number ) ) )
      {
         sprintf( outbuf, "%s %s isn't a valid money type!\n\r", m_number, m_name );
         send_to_char( outbuf, ch );
         join_money( transfer, ch->money );
         return -1;
      }
      if( ch->money->cash_unit[mn] < atoi( m_number ) )
      {
         sprintf( outbuf, "You don't have %s %s!\n\r", m_number, m_name );
         send_to_char( outbuf, ch );
         join_money( transfer, ch->money );
         return -1;
      }
      ch->money->cash_unit[mn] -= atoi( m_number );
      transfer->cash_unit[mn] += atoi( m_number );
   }
   ch->carry_weight -= money_weight( transfer );
   value = money_value( transfer );
   PUT_FREE( transfer, money_type_free );
   return value;

}

bool get_money_room( CHAR_DATA * ch, char *argument )
{
   MONEY_TYPE *transfer;
   sh_int looper;
   char m_number[MSL];
   char m_name[MSL];
   char outbuf[MSL];


   GET_FREE( transfer, money_type_free );
#ifdef DEBUG_MONEY
   {
      char testbuf[MSL];
      sprintf( testbuf, "get_money_room, %s, %s", ch->name, argument );
      transfer->money_key = str_dup( testbuf );
   }
#endif
   if( !str_cmp( "all", argument ) )
   {
      for( looper = 0; looper < MAX_CURRENCY; looper++ )
      {
         transfer->cash_unit[looper] = ch->in_room->treasure->cash_unit[looper];
         ch->in_room->treasure->cash_unit[looper] = 0;
      }
   }
   else
   {
      for( looper = 0; looper < MAX_CURRENCY; looper++ )
      {
         transfer->cash_unit[looper] = 0;
      }
      for( ;; )
      {
         sh_int mn;
         argument = one_argument( argument, m_number );
         if( m_number[0] == '\0' )
            break;
         argument = one_argument( argument, m_name );
         if( m_name[0] == '\0' )
            break;
         if( ( ( mn = money_lookup( m_name ) ) < 0 ) || ( !is_number( m_number ) ) )
         {
            sprintf( outbuf, "%s %s isn't a valid money type!\n\r", m_number, m_name );
            send_to_char( outbuf, ch );
            join_money( transfer, ch->in_room->treasure );
            return FALSE;
         }
         if( ch->in_room->treasure->cash_unit[mn] < atoi( m_number ) )
         {
/*        sprintf( outbuf, "There isn't that much %s here!\n\r", m_name );
        send_to_char( outbuf, ch );  */
            join_money( transfer, ch->in_room->treasure );
            return FALSE;
         }
         ch->in_room->treasure->cash_unit[mn] -= atoi( m_number );
         transfer->cash_unit[mn] += atoi( m_number );
      }
   }
   if( ( ch->carry_weight + money_weight( transfer ) ) > can_carry_w( ch ) )
   {
      sprintf( outbuf, "%s", "You cannot carry that much weight!\n\r" );
      send_to_char( outbuf, ch );
      join_money( transfer, ch->in_room->treasure );
      return FALSE;
   }
   ch->carry_weight += money_weight( transfer );
   if( money_value( transfer ) <= 0 )
      return FALSE;
   sprintf( outbuf, "You pick up %s.\n\r", money_string( transfer ) );
   send_to_char( outbuf, ch );
   join_money( transfer, ch->money );
   return TRUE;
}

bool get_money_obj( CHAR_DATA * ch, char *argument, OBJ_DATA * obj )
{
   MONEY_TYPE *transfer;
   sh_int looper;
   char m_number[MSL];
   char m_name[MSL];
   char outbuf[MSL];



   GET_FREE( transfer, money_type_free );
#ifdef DEBUG_MONEY
   {
      char testbuf[MSL];
      sprintf( testbuf, "get_money_obj, %s, %s", ch->name, argument );
      transfer->money_key = str_dup( testbuf );
   }
#endif
   if( !str_cmp( "all", argument ) )
   {
      for( looper = 0; looper < MAX_CURRENCY; looper++ )
      {
         transfer->cash_unit[looper] = obj->money->cash_unit[looper];
         obj->money->cash_unit[looper] = 0;
      }
   }
   else
   {
      for( looper = 0; looper < MAX_CURRENCY; looper++ )
      {
         transfer->cash_unit[looper] = 0;
      }
      for( ;; )
      {
         sh_int mn;
         argument = one_argument( argument, m_number );
         if( m_number[0] == '\0' )
            break;
         argument = one_argument( argument, m_name );
         if( m_name[0] == '\0' )
            break;
         if( ( ( mn = money_lookup( m_name ) ) < 0 ) || ( !is_number( m_number ) ) )
         {
            sprintf( outbuf, "%s %s isn't a valid money type!\n\r", m_number, m_name );
            send_to_char( outbuf, ch );
            join_money( transfer, obj->money );
            return FALSE;
         }
         if( obj->money->cash_unit[mn] < atoi( m_number ) )
         {
/*        sprintf( outbuf, "There isn't that much %s in %s!\n\r", m_name, obj->short_descr );
        send_to_char( outbuf, ch );  */
            join_money( transfer, obj->money );
            return FALSE;
         }
         obj->money->cash_unit[mn] -= atoi( m_number );
         transfer->cash_unit[mn] += atoi( m_number );
      }
   }
   if( ( ch->carry_weight + money_weight( transfer ) ) > can_carry_w( ch ) )
   {
      sprintf( outbuf, "%s", "You cannot carry that much weight!\n\r" );
      send_to_char( outbuf, ch );
      join_money( transfer, obj->money );
      return FALSE;
   }
   ch->carry_weight += money_weight( transfer );
   if( money_value( transfer ) <= 0 )
      return FALSE;
   sprintf( outbuf, "You take %s from %s.\n\r", money_string( transfer ), obj->short_descr );
   send_to_char( outbuf, ch );
   join_money( transfer, ch->money );
   return TRUE;
}

void do_bank( CHAR_DATA * ch, char *argument )
{
   /*
    * Simple banking system.  Allow ch to check balance, make a 
    * * deposit or withdrawl some money.
    * * -- Stephen
    */

   CHAR_DATA *mob;
   char buf[MAX_STRING_LENGTH];
   char arg1[MAX_INPUT_LENGTH];
/*    char arg2[MAX_INPUT_LENGTH];  */
   buf[0] = '\0';

   if( IS_NPC( ch ) )
   {
      send_to_char( "Banking Services are only available to players!\n\r", ch );
      return;
   }

   /*
    * Check for mob with act->heal 
    */
   for( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
   {
      if( IS_NPC( mob ) && IS_SET( mob->act, ACT_BANKER ) )
         break;
   }

   if( mob == NULL )
   {
      send_to_char( "You can't do that here.\n\r", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "                  BANK : Options:\n\r\n\r", ch );
      send_to_char( "          BANK BALANCE : Displays your balance.\n\r", ch );
      send_to_char( " BANK DEPOSIT <amount> : Deposit money into your account.\n\r", ch );
      send_to_char( "BANK WITHDRAW <amount> : Withdraw money from your account.\n\r", ch );
      send_to_char( "BANK EXCHANGE <amount> : Exchange money. A nominal fee will be charged.\n\r", ch );
      return;
   }

   argument = one_argument( argument, arg1 );
/*   argument = one_argument( argument, arg2 ); */

   /*
    * Now work out what to do... 
    */


   if( !str_cmp( arg1, "balance" ) )
   {
      sprintf( buf, "Your current balance is: %s.\n\r", money_string( ch->bank_money ) );
      send_to_char( buf, ch );
      return;
   }
   if( !str_cmp( arg1, "exchange" ) )
   {
      int taxes;
      MONEY_TYPE *ex_cost;
      char test[MSL];
      one_argument( argument, test );
      if( test[0] == '\0' )
      {
         send_to_char( "What exactly would you like to exchange?\n\r", ch );
         return;
      }
      taxes = exchange_money( ch, argument );
      if( taxes > -1 )
      {
         ex_cost = round_money( taxes, TRUE );
         sprintf( buf, "You exchange %s. You were charged %s for the transaction.\n\r", argument, money_string( ex_cost ) );
         send_to_char( buf, ch );
         PUT_FREE( ex_cost, money_type_free );
      }

      return;
   }
   if( !str_cmp( arg1, "deposit" ) )
   {
      deposit_money( ch, argument );
      sprintf( buf, "You deposit %s.  Your new balance is %s.\n\r", argument, money_string( ch->bank_money ) );
      send_to_char( buf, ch );
      do_save( ch, "" );
      return;

   }

   if( !str_cmp( arg1, "withdraw" ) )
   {
      bool good_withdraw = FALSE;
      good_withdraw = withdraw_money( ch, argument );
      if( good_withdraw )
      {
         sprintf( buf, "You withdraw %s.  Your new balance is %s.\n\r", argument, money_string( ch->bank_money ) );
      }
      else
      {
         sprintf( buf, "You were unable to withdraw %s.\n\r", argument );
      }
      send_to_char( buf, ch );
      do_save( ch, "" );
      return;
   }

   send_to_char( "That option not recognised!\n\r", ch );
   return;
}

void do_gold( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];

   sprintf( buf, "You are carrying: %s.\n\r", money_string( ch->money ) );
   send_to_char( buf, ch );

   return;
}

char *take_best_coins( MONEY_TYPE * money, int cost )
{
/* best fits a money to a base cost, returns a buf with change val as first arg, 
   then the coin string in keyword format to take */
   int still_needs = cost;
   int unit_level, cnt, can_take_this, change;
   char takecatbuf[MSL], debug_moneybuf[MSL], takebuf[MSL];
   static char returnbuf[MSL];
   MONEY_TYPE *transaction;
   MONEY_TYPE *debug_money;


   unit_level = 0;   /* start at smallest currency unit */
   change = 0;
   GET_FREE( transaction, money_type_free );
#ifdef DEBUG_MONEY
   {
      char testbuf[MSL];
      sprintf( testbuf, "take_best_coins, %s, %d", money->money_key, cost );
      transaction->money_key = str_dup( testbuf );
   }
#endif
   for( cnt = 0; cnt < MAX_CURRENCY; cnt++ )
      transaction->cash_unit[cnt] = money->cash_unit[cnt];
   takebuf[0] = '\0';
   for( ; still_needs > 0; )
   {
      /*
       * take smallest unit, see what happens 
       */
      can_take_this = transaction->cash_unit[unit_level] * currency_table[unit_level].exchange_val;
      if( can_take_this >= still_needs )
      {
         sh_int how_many;
         how_many = ( still_needs / currency_table[unit_level].exchange_val ) +
            ( ( still_needs % currency_table[unit_level].exchange_val != 0 ) ? 1 : 0 );
         change = change + ( how_many * currency_table[unit_level].exchange_val ) - still_needs;
         still_needs = 0;
         sprintf( takecatbuf, " %d %s", how_many, currency_table[unit_level].keyword );
         safe_strcat( MSL, takebuf, takecatbuf );
         break;
      }
      else
      {
         sprintf( takecatbuf, " %d %s", transaction->cash_unit[unit_level], currency_table[unit_level].keyword );
         safe_strcat( MSL, takebuf, takecatbuf );
         transaction->cash_unit[unit_level] = 0;
         still_needs -= can_take_this;
         unit_level++;
      }
   }
   change = change + ( 0 - still_needs );
   debug_money = round_money( cost, TRUE );
   sprintf( debug_moneybuf, "%s", money_string( debug_money ) );
   sprintf( log_buf, "Buy: cost: %d ( %s )\n\r, money has %s\n\r, wants to take %s\n\r, change %d.",
            cost, debug_moneybuf, money_string( money ), takebuf, change );
   monitor_chan( log_buf, MONITOR_DEBUG );
   PUT_FREE( transaction, money_type_free );
   PUT_FREE( debug_money, money_type_free );
   sprintf( returnbuf, "%d %s", change, takebuf );
   return returnbuf;
}
