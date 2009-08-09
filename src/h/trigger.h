/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_TRIGGER_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* trigger.c */
void trigger_show P_((CHAR_DATA *ch));
void trigger_handler P_((CHAR_DATA *ch, OBJ_DATA *obj, int trigger));
DECLARE_TRIG_FUN(trig_transfer);
DECLARE_TRIG_FUN(trig_restore);
DECLARE_TRIG_FUN(trig_slay);
DECLARE_TRIG_FUN(trig_transform);

#undef P_
