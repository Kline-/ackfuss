/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_EMAIL_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* email.c */
bool valid_email_addy P_((char *address));
DECLARE_DO_FUN(do_email);
void send_email P_((const char *m_address, const char *m_subject, const char *mfilename));
bool save_mail_file P_((const char *mfilename, char *mtext));
void send_rep_out P_((CHAR_DATA *ch, char *outbuf, bool mailme, char *msub));

#undef P_
