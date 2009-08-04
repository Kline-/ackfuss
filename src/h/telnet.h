/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

/* Telopt handling by Igor van den Hoven (Scandum) */

#define DEC_TELNET_H

/* telnet options */
#define TELOPT_MSSP          70   /* used to send mud server information */
#define TELOPT_MCCP          86   /* used to toggle Mud Client Compression Protocol */
#define TELOPT_MSP           90   /* used to toggle Mud Sound Protocol */
#define TELOPT_MXP           91   /* used to toggle Mud Extention Protocol */

/* TELOPT_MSSP */
#define MSSP_VAR              1
#define MSSP_VAL              2

/* commands */
char IAC_SE[]        = {IAC, SE, 0 };

char IAC_DO_MSSP[]   = {IAC, DO, TELOPT_MSSP, 0};
char IAC_SB_MSSP[]   = {IAC, SB, TELOPT_MSSP, 0};
char IAC_WILL_MSSP[] = {IAC, WILL, TELOPT_MSSP, 0};

char IAC_DO_MCCP[]   = {IAC, DO, TELOPT_MCCP, 0};
char IAC_SB_MCCP[]   = {IAC, SB, TELOPT_MCCP, 0};
char IAC_WILL_MCCP[] = {IAC, WILL, TELOPT_MCCP, 0};
