/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_HELP_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

DECLARE_DO_FUN(do_help);
DECLARE_DO_FUN(do_shelp);
bool exists_help P_((const char *help));
HELP_DATA *load_help P_((const char *help));
void cache_check_help P_((void));
int count_helps P_((void));
char *find_helps P_((const char *search, bool imm));
char *grep_helps P_((const char *search, bool imm));

#undef P_
