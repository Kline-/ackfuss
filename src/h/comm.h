/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_COMM_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* comm.c */
int main P_((int argc, char **argv));
int init_socket P_((int sock_port));
void reopen_socket P_((int sig));
void game_loop P_((int game_control));
void *lookup_address P_((void *input));
void new_descriptor P_((int d_control));
void init_descriptor P_((DESCRIPTOR_DATA *dnew, int desc));
void close_socket P_((DESCRIPTOR_DATA *dclose));
bool read_from_descriptor P_((DESCRIPTOR_DATA *d));
void read_from_buffer P_((DESCRIPTOR_DATA *d));
bool process_output P_((DESCRIPTOR_DATA *d, bool fPrompt));
void bust_a_prompt P_((DESCRIPTOR_DATA *d));
void write_to_buffer P_((DESCRIPTOR_DATA *d, const char *txt, int length = 0));
bool write_to_descriptor P_((int desc, char *txt, int length = 0));
void show_stotal_to P_((DESCRIPTOR_DATA *d));
void show_menu_to P_((DESCRIPTOR_DATA *d));
void show_smenu_to P_((DESCRIPTOR_DATA *d));
void show_rmenu_to P_((DESCRIPTOR_DATA *d));
void show_amenu_to P_((DESCRIPTOR_DATA *d));
void show_ahelp_menu_to P_((DESCRIPTOR_DATA *d));
void show_cmenu_to P_((DESCRIPTOR_DATA *d));
void nanny P_((DESCRIPTOR_DATA *d, char *argument));
bool check_parse_name P_((char *name));
bool check_reconnect P_((DESCRIPTOR_DATA *d, bool fConn));
bool check_playing P_((DESCRIPTOR_DATA *d, string name));
void stop_idling P_((CHAR_DATA *ch));
void send_to_char P_((string txt, CHAR_DATA *ch));
void show_string P_((struct descriptor_data *d, char *input));
void act P_((const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type));
DECLARE_DO_FUN(do_finger);
void send_to_descrips P_((const char *message));
void copyover_recover P_((void));
void hang P_((const char *str));

#undef P_
