/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_ACT_MOVE_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* act_move.c */
DECLARE_DO_FUN(do_north);
DECLARE_DO_FUN(do_east);
DECLARE_DO_FUN(do_south);
DECLARE_DO_FUN(do_west);
DECLARE_DO_FUN(do_up);
DECLARE_DO_FUN(do_down);
int find_door P_((CHAR_DATA *ch, char *arg));
DECLARE_DO_FUN(do_open);
DECLARE_DO_FUN(do_close);
bool has_key P_((CHAR_DATA *ch, int key));
DECLARE_DO_FUN(do_lock);
DECLARE_DO_FUN(do_unlock);
DECLARE_DO_FUN(do_pick);
DECLARE_DO_FUN(do_stand);
DECLARE_DO_FUN(do_rest);
DECLARE_DO_FUN(do_sleep);
DECLARE_DO_FUN(do_wake);
DECLARE_DO_FUN(do_shadowform);
DECLARE_DO_FUN(do_sneak);
DECLARE_DO_FUN(do_warcry);
DECLARE_DO_FUN(do_hide);
DECLARE_DO_FUN(do_visible);
DECLARE_DO_FUN(do_clan_recall);
DECLARE_DO_FUN(do_recall);
DECLARE_DO_FUN(do_train);
DECLARE_DO_FUN(do_halls);
DECLARE_DO_FUN(do_smash);
DECLARE_DO_FUN(do_scan);
DECLARE_DO_FUN(do_enter);
DECLARE_DO_FUN(do_scout);
DECLARE_DO_FUN(do_abandon);
void move_char P_(( CHAR_DATA * ch, int door, bool look ));

#undef P_
