/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/
/* Telopt handling by Igor van den Hoven (Scandum)                         */

#include "h/globals.h"

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
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
 {3, IAC_DO_MCCP, &process_do_mccp},
 {3, IAC_DO_MSSP, &process_do_mssp},
 {0, NULL, NULL}
};

void send_telopts( DESCRIPTOR_DATA *d )
{
 write_to_buffer(d,IAC_WILL_MSSP);
// write_to_buffer(d,IAC_WILL_MCCP);
 return;
}

int telopt_handler( DESCRIPTOR_DATA *d, char *src, int srclen, char *out )
{
 int cnt = 0, skip = 0;
 unsigned char *pti, *pto;

 pti = (unsigned char *)src;
 pto = (unsigned char *)out;

 while( srclen > 0 )
 {
  switch( *pti )
  {
   case IAC:
    for( cnt = 0; telopt_table[cnt].code; cnt++ )
    {
     if( srclen >= telopt_table[cnt].size && !memcmp(pti,telopt_table[cnt].code,telopt_table[cnt].size) )
     {
      skip = telopt_table[cnt].func(d,(char *)pti,srclen);
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
 write_to_buffer(d,buf);
}

void mssp_reply( DESCRIPTOR_DATA* d, const char* key, int value )
{
 char buf[MSL];
 snprintf(buf,MSL,"%c%s%c%d",MSSP_VAR,key,MSSP_VAL,value);
 write_to_buffer(d,buf);
}

int process_do_mssp( DESCRIPTOR_DATA *d, char *src, int srclen )
{
 DESCRIPTOR_DATA *dc;
 int cnt = 0;
 extern int int_boot_time;

 for( dc = first_desc; dc != NULL; dc = dc->next )
  if( dc->character != NULL )
   cnt++;

 write_to_buffer(d,IAC_SB_MSSP);

 /* Required */
 mssp_reply(d,"PLAYERS",cnt);
 mssp_reply(d,"UPTIME",int_boot_time);

 /* Generic */
 mssp_reply(d,"HOSTNAME",MSSP_HOSTNAME);
 mssp_reply(d,"PORT",MSSP_PORT);
 mssp_reply(d,"CODEBASE",VERS_STRING);
 mssp_reply(d,"CONTACT",MSSP_CONTACT);
 mssp_reply(d,"CREATED",MSSP_CREATED);
 mssp_reply(d,"ICON",MSSP_ICON);
 mssp_reply(d,"IP",MSSP_IP);
 mssp_reply(d,"LANGUAGE",MSSP_LANGUAGE);
 mssp_reply(d,"LOCATION",MSSP_LOCATION);
 mssp_reply(d,"MINIMUM AGE",MSSP_MIN_AGE);
 mssp_reply(d,"NAME",MSSP_NAME);
 mssp_reply(d,"WEBSITE",MSSP_WEBSITE);

 /* Categorization */
 mssp_reply(d,"FAMILY",MSSP_FAMILY);
 mssp_reply(d,"GENRE",MSSP_GENRE);
 mssp_reply(d,"GAMEPLAY",MSSP_GAMEPLAY);
 mssp_reply(d,"GAMESYSTEM",MSSP_GAMESYSTEM);
 mssp_reply(d,"INTERMUD",MSSP_INTERMUD);
 mssp_reply(d,"STATUS",MSSP_STATUS);
 mssp_reply(d,"SUBGENRE",MSSP_SUBGENRE);

 /* World */
 mssp_reply(d,"AREAS",area_list.size());
 mssp_reply(d,"HELPFILES",count_helps());
 mssp_reply(d,"MOBILES",mob_index_list.size());
 mssp_reply(d,"OBJECTS",obj_index_list.size());
 mssp_reply(d,"ROOMS",room_index_list.size());
 mssp_reply(d,"RESETS",reset_list.size());
 mssp_reply(d,"MUDPROGS",0);
 mssp_reply(d,"MUDTRIGS",0);
 mssp_reply(d,"CLASSES",MSSP_CLASSES);
 mssp_reply(d,"LEVELS",MSSP_LEVELS);
 mssp_reply(d,"RACES",MSSP_RACES);
 mssp_reply(d,"SKILLS",count_skills());

 /* Protocols */
 mssp_reply(d,"ANSI",1);
 mssp_reply(d,"MCCP",0);
 mssp_reply(d,"MCP",0);
 mssp_reply(d,"MSP",0);
 mssp_reply(d,"SSL",0);
 mssp_reply(d,"MXP",0);
 mssp_reply(d,"PUEBLO",0);
 mssp_reply(d,"VT100",0);
 mssp_reply(d,"XTERM 256 COLORS",0);

 /* Commercial */
 mssp_reply(d,"PAY TO PLAY",0); 
 mssp_reply(d,"PAY FOR PERKS",0);

 /* Hiring */
 mssp_reply(d,"HIRING BUILDERS",MSSP_HIRE_BUILDER);
 mssp_reply(d,"HIRING CODERS",MSSP_HIRE_CODER);

 write_to_buffer(d,IAC_SE);
 return 3;
}

int process_do_mccp( DESCRIPTOR_DATA *d, char *src, int srclen )
{
 return 3;
}
