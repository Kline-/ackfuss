/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_TELOPT_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* telopt.c */
void send_telopts P_((DESCRIPTOR_DATA *d));
int telopt_handler P_((DESCRIPTOR_DATA *d, char *src, int srclen, char *out));
void mssp_reply P_((DESCRIPTOR_DATA* d, const char* key, const char* value));
void mssp_reply P_((DESCRIPTOR_DATA* d, const char* key, int value));
int process_mssp P_((DESCRIPTOR_DATA *d, char *src, int srclen));

#undef P_
