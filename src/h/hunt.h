/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_HUNT_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* hunt.c */
void h_dequeue P_((void));
void h_clear P_((void));
void h_enqueue P_((ROOM_INDEX_DATA *room, short dir));
bool h_is_valid_exit P_((ROOM_INDEX_DATA *room, short dir, int h_flags));
void h_enqueue_room P_((ROOM_INDEX_DATA *room, short dir, int h_flags));
short h_find_dir P_((ROOM_INDEX_DATA *room, ROOM_INDEX_DATA *target, int h_flags));
bool set_hunt P_((CHAR_DATA *ch, CHAR_DATA *fch, CHAR_DATA *vch, OBJ_DATA *vobj, int set_flags, int rem_flags));
void end_hunt P_((CHAR_DATA *ch));
void hunt_move P_((CHAR_DATA *mob, short dir));
bool mob_hunt P_((CHAR_DATA *mob));
void char_hunt P_((CHAR_DATA *ch));
DECLARE_DO_FUN(do_hunt);

#undef P_
