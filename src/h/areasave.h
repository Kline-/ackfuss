/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_AREASAVE_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* areasave.c */
DECLARE_DO_FUN(do_savearea);
void build_save P_((void));
void build_save_area P_((void));
void build_save_mobs P_((void));
void build_save_objects P_((void));
void build_save_rooms P_((void));
void build_save_shops P_((void));
void build_save_specs P_((void));
void build_save_objfuns P_((void));
void build_save_resets P_((void));
void build_save_end P_((void));
void build_save_flush P_((void));
void area_modified P_((AREA_DATA *pArea));

#undef P_

#define SAVEQUEUESIZE 50
#define NOT_SAVING 0
#define START_SAVING 1
#define AM_SAVING 2
#define BUILD_OK -1
#define BUILD_CANTSAVE 1
#define BUILD_TOOMANY  2

#define BUILD_SEC_AREA     1
#define BUILD_SEC_ROOMS    2
#define BUILD_SEC_MOBILES  3
#define BUILD_SEC_OBJECTS  4
#define BUILD_SEC_SHOPS    5
#define BUILD_SEC_RESETS   6
#define BUILD_SEC_END      7
#define AREA_VERSION       22
