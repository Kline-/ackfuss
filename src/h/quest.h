/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_QUEST_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

DECLARE_DO_FUN(do_iquest);
CHAR_DATA *get_quest_target P_((int min_level, int max_level));
OBJ_DATA *load_quest_object P_((CHAR_DATA * target));
CHAR_DATA *get_quest_giver P_((int min_level, int max_level));
void quest_inform P_((void));
void quest_complete P_((CHAR_DATA * ch));
void quest_cancel P_((void));
void clear_quest P_((void));
void generate_auto_quest P_((void));
void crusade_reward P_((CHAR_DATA *ch));

#undef P_
