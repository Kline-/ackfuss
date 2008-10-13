/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_BOARD_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* board.c */
void do_show_contents P_((CHAR_DATA *ch, OBJ_DATA *obj));
BOARD_DATA *load_board P_((OBJ_INDEX_DATA *pObj));
void save_board P_((BOARD_DATA *board, CHAR_DATA *ch));
void do_delete P_((CHAR_DATA *ch, char *argument));
void do_show_message P_((CHAR_DATA *ch, int mess_num, OBJ_DATA *obj));
void do_write P_((CHAR_DATA *ch, char *argument));
void finished_editing P_((MESSAGE_DATA *msg, char **dest, CHAR_DATA *ch, bool saved));
void do_read P_((CHAR_DATA *ch, char *argument));
void do_edit_message P_((CHAR_DATA *ch, int mess_num, OBJ_DATA *obj));
void do_edit P_((CHAR_DATA *ch, char *argument));

#undef P_
