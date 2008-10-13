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
void do_savearea P_((CHAR_DATA *ch, char *argument));
void build_save P_((void));
void build_save_area P_((void));
void build_save_mobs P_((void));
void build_save_mobprogs P_((void));
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
