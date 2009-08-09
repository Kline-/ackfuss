/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_WIZUTIL_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* wizutil.c */
char *area_name P_((AREA_DATA *pArea));
void room_pair P_((ROOM_INDEX_DATA *left, ROOM_INDEX_DATA *right, exit_status ex, char *buffer));
void checkexits P_((ROOM_INDEX_DATA *room, AREA_DATA *pArea, char *buffer));
DECLARE_DO_FUN(do_exlist);
DECLARE_DO_FUN(do_vlist);
DECLARE_DO_FUN(do_rename);

#undef P_
