/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_BUILDARE_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

#define AREA_NOERROR   0
#define AREA_SHOWERROR 1

/* buildare.c */
int build_canread P_((AREA_DATA *Area, CHAR_DATA *ch, int showerror));
int build_canwrite P_((AREA_DATA *Area, CHAR_DATA *ch, int showerror));
void build_save_area_list P_((void));
void build_save_area_gold P_((void));
void build_makearea P_((CHAR_DATA *ch, char *argument));
void do_change_gold P_((CHAR_DATA *ch, char *argument));
void build_setarea P_((CHAR_DATA *ch, char *argument));
void build_findarea P_((CHAR_DATA *ch, char *argument));
void build_showarea P_((CHAR_DATA *ch, char *argument));
void build_arealist P_((CHAR_DATA *ch, char *argument));

#undef P_
