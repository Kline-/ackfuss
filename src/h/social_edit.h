/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_SOCIAL_EDIT_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* social_edit.c */
void load_social P_((FILE *fp, SOCIAL_TYPE *social));
void load_social_table P_((void));
void save_social P_((const SOCIAL_TYPE *s, FILE *fp));
void save_social_table P_((void));
int social_lookup P_((const char *name));
DECLARE_DO_FUN(do_sedit);

#undef P_
