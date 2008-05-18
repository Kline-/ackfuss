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

/*
 *  money.h -- handles multi unit money transactions.
 */
#ifndef DEC_TYPEDEFS_H
#include "typedefs.h"
#endif

#define DEC_MONEY_H     1

#define MAX_CURRENCY    8
#define EXCHANGE_COST   .1
#define CURRENCY_NAMES   "bit bits moon moons crown crowns crescent crescents pentacle pentacles karant karants mark marks sunburst sunbursts"
struct currency_type
{
   char *keyword;
   char *singular;
   char *plural;
   float unit_weight;   /* in hundreths of a weight unit */
   int exchange_val; /* this is in base units, your lowest money form */
};


struct money_type
{
   bool is_free;
   MONEY_TYPE *next;
   MONEY_TYPE *prev;
#ifdef DEBUG_MONEY
   char *money_key;
#endif
   int cash_unit[MAX_CURRENCY];
};


int money_to_value( CHAR_DATA * ch, char *argument );
int money_to_cost( char *money_list );
char *cost_to_money( int cost );
sh_int money_lookup( char *money_name );
char *money_string( MONEY_TYPE * money );
char *unit_string( MONEY_TYPE * money );
void deposit_money( CHAR_DATA * ch, char *argument );
bool withdraw_money( CHAR_DATA * ch, char *argument );
int exchange_money( CHAR_DATA * ch, char *argument );
MONEY_TYPE *round_money( int base, bool round_up );
MONEY_TYPE *round_money_off( int base, sh_int accuracy );
MONEY_TYPE *join_money( MONEY_TYPE * source, MONEY_TYPE * receiver );
void drop_money( CHAR_DATA * ch, char *argument );
bool get_money_room( CHAR_DATA * ch, char *argument );
bool get_money_obj( CHAR_DATA * ch, char *argument, OBJ_DATA * obj );
bool give_money( CHAR_DATA * ch, CHAR_DATA * victim, char *argument );

int money_value( MONEY_TYPE * money );
float money_weight( MONEY_TYPE * money );
char *take_best_coins( MONEY_TYPE * money, int cost );

extern const struct currency_type currency_table[MAX_CURRENCY];
