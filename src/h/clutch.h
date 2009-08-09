/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_CLUTCH_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

#define SPELL_NUMBER 3

/* clutch.c */
bool valid_clutch_number P_((int number));
DECLARE_DO_FUN(do_clutch);
void clutch_portal P_((int v1, int v2, int v3, CHAR_DATA *ch, OBJ_DATA *obj));
void clutch_gate P_((int v1, int v2, int v3, CHAR_DATA *ch, OBJ_DATA *obj));

#undef P_
