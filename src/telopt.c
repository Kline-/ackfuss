/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/
/* Telopt handling by Igor van den Hoven (Scandum) */

#include "globals.h"

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_TELENT_H
#include "h/telnet.h"
#endif

#ifndef DEC_TELOPT_H
#include "h/telopt.h"
#endif

struct telopt_type
{
 int size;
 char *code;
 int (* func)(DESCRIPTOR_DATA *d, char *src, int srclen);
};

const struct telopt_type telopt_table [] =
{
 {3, IAC_DO_MSSP, &process_mssp},
 {0, NULL, NULL}
};

void send_telopts( DESCRIPTOR_DATA *d )
{
 write_to_descriptor(d->descriptor,IAC_WILL_MSSP,0);
 return;
}

int telopt_handler( DESCRIPTOR_DATA *d, char *src, int srclen, char *out )
{
 int cnt = 0, skip = 0;
 char *pti, *pto;

 pti = src;
 pto = out;

 while( srclen > 0 )
 {
  switch( *pti )
  {
   case IAC:
    for( cnt = 0; telopt_table[cnt].code; cnt++ )
    {
     if( srclen >= telopt_table[cnt].size && !memcmp(pti,telopt_table[cnt].code,telopt_table[cnt].size) )
     {
      skip = telopt_table[cnt].func(d,pti,srclen);
      break;
     }
    }

    if( telopt_table[cnt].code == NULL )
    {
     switch( pti[1] )
     {
      case WILL:
      case DO:
      case WONT:
      case DONT:
       skip = 3;
       break;
      case SB: //fixme
       break;
      case IAC:
       *pto++ = *pti++;
       srclen--;
       skip = 1;
       break;
      default:
       if( TELCMD_OK(pti[1]) )
        skip = 2;
       else
        skip = 1;
       break;
     }
    }

    skip = URANGE(0,skip,srclen);
    pti += skip;
    srclen -= skip;
    break;
   case '\0':
    pti++;
    srclen--;
    break;
   case '\r':
    pti++;
    srclen--;
    break;
   case '\n':
    *pto++ = *pti++;
    srclen--;
    break;
   default:
    *pto++ = *pti++;
    srclen--;
    break;
  }
 }
 *pto = 0;

 return strlen(out);
}

void mssp_reply( DESCRIPTOR_DATA* d, const char* key, const char* value )
{
 char buf[MSL];
 snprintf(buf,MSL,"%c%s%c%s",MSSP_VAR,key,MSSP_VAL,value);
 write_to_descriptor(d->descriptor,buf,0);
}

void mssp_reply( DESCRIPTOR_DATA* d, const char* key, int value )
{
 char buf[MSL];
 snprintf(buf,MSL,"%c%s%c%d",MSSP_VAR,key,MSSP_VAL,value);
 write_to_descriptor(d->descriptor,buf,0);
}

int process_mssp( DESCRIPTOR_DATA *d, char *src, int srclen )
{
 DESCRIPTOR_DATA *dc;
 int cnt = 0;
 extern int int_boot_time;

 for( dc = first_desc; dc != NULL; dc = dc->next )
  if( dc->character != NULL )
   cnt++;

 write_to_descriptor(d->descriptor,IAC_SB_MSSP,0);

 mssp_reply(d,"PLAYERS",cnt);
 mssp_reply(d,"UPTIME",int_boot_time);

 mssp_reply(d,"CODEBASE",VERS_STRING);
 mssp_reply(d,"CONTACT","genocaust@gmail.com");
 mssp_reply(d,"CREATED","2008");
 mssp_reply(d,"HOSTNAME","home.gotr00t.us");
 mssp_reply(d,"PORT","3000");
 mssp_reply(d,"WEBSITE","http://www.ackmud.net");

 write_to_descriptor(d->descriptor,IAC_SE,0);
 return 3;
}
