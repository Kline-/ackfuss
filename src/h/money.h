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

#define DEC_MONEY_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* money.c */
short money_lookup P_((char *money_name));
MONEY_TYPE *join_money P_((MONEY_TYPE *source, MONEY_TYPE *receiver));
float money_weight P_((MONEY_TYPE *money));
char *cost_to_money P_((int cost));
int money_to_cost P_((char *money_list));
MONEY_TYPE *round_money P_((int base, bool round_up));
MONEY_TYPE *round_money_off P_((int base, short accuracy));
int money_value P_((MONEY_TYPE *money));
char *money_string P_((MONEY_TYPE *money));
char *unit_string P_((MONEY_TYPE *money));
bool give_money P_((CHAR_DATA *ch, CHAR_DATA *victim, char *argument));
bool withdraw_money P_((CHAR_DATA *ch, char *argument));
void deposit_money P_((CHAR_DATA *ch, char *argument));
int exchange_money P_((CHAR_DATA *ch, char *argument));
DECLARE_DO_FUN(do_mgive);
void drop_money P_((CHAR_DATA *ch, char *argument));
int money_to_value P_((CHAR_DATA *ch, char *argument));
bool get_money_room P_((CHAR_DATA *ch, char *argument));
bool get_money_obj P_((CHAR_DATA *ch, char *argument, OBJ_DATA *obj));
DECLARE_DO_FUN(do_bank);
DECLARE_DO_FUN(do_gold);
char *take_best_coins P_((MONEY_TYPE *money, int cost));

#undef P_

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

class money_type
{
 public:
  money_type();
  ~money_type();
  int cash_unit[MAX_CURRENCY];
};

extern const struct currency_type currency_table[MAX_CURRENCY];
