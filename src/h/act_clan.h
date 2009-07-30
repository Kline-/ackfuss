/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_ACT_CLAN_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* act_clan.c */
void save_clan_table P_((void));
DECLARE_DO_FUN(do_ctoggle);
char *get_diplo_name P_((short value));
DECLARE_DO_FUN(do_politics);
DECLARE_DO_FUN(do_negotiate);
DECLARE_DO_FUN(do_leav);
DECLARE_DO_FUN(do_cset);
DECLARE_DO_FUN(do_accept);
DECLARE_DO_FUN(do_cwhere);
DECLARE_DO_FUN(do_leave);
DECLARE_DO_FUN(do_banish);
DECLARE_DO_FUN(do_make);
DECLARE_DO_FUN(do_council);

#undef P_
