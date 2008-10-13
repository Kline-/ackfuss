/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  Ack 2.2 improvements copyright (C) 1994 by Stephen Dooley              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *       _/          _/_/_/     _/    _/     _/    ACK! MUD is modified    *
 *      _/_/        _/          _/  _/       _/    Merc2.0/2.1/2.2 code    *
 *     _/  _/      _/           _/_/         _/    (c)Stephen Zepp 1998    *
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3          *
 *   _/      _/      _/_/_/     _/    _/     _/                            *
 *                                                                         *
 *                        http://ackmud.nuc.net/                           *
 *                        zenithar@ackmud.nuc.net                          *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include <math.h>
#include <stdlib.h>

#ifndef DEC_ACT_INFO_H
#include "h/act_info.h"
#endif

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#define KEY( literal, field, value )  if ( !str_cmp( word, literal ) ) { field  = value; fMatch = TRUE;  break;}
#define SKEY( literal, field, value )  if ( !str_cmp( word, literal ) ) { if (field!=NULL) free_string(field);field  = value; fMatch = TRUE;  break;}

void init_sysdata args( ( void ) );

extern bool wizlock;

void load_sysdata( void )
{
 FILE *fp;
 const char *word;
 bool fMatch;

 xprintf_2(log_buf,"Loading %s",SYSDAT_FILE);
 log_f(log_buf);

 init_sysdata(); /* Need to set some defaults --Kline */

 if( (fp = file_open(SYSDAT_FILE,"r")) == NULL )
 {
  log_f("Failed to load system data.");
  return;
 }

 for( ; ; )
 {
  word = feof(fp) ? "End" : fread_word(fp);
  fMatch = FALSE;

  switch( UPPER(word[0]) )
  {
   case 'B':
    KEY("Build_Obj_AC",  sysdata.build_obj_ac,  fread_number(fp));
    KEY("Build_Obj_DR",  sysdata.build_obj_dr,  fread_number(fp));
    KEY("Build_Obj_HP",  sysdata.build_obj_hp,  fread_number(fp));
    KEY("Build_Obj_HR",  sysdata.build_obj_hr,  fread_number(fp));
    KEY("Build_Obj_MP",  sysdata.build_obj_mp,  fread_number(fp));
    KEY("Build_Obj_MV",  sysdata.build_obj_mv,  fread_number(fp));
    KEY("Build_Obj_SVS", sysdata.build_obj_svs, fread_number(fp));
    break;
   case 'D':
    KEY("Damcap",        sysdata.damcap,        fread_number(fp));
    break;
   case 'E':
    if( !str_cmp(word,"End") )
    {
     file_close(fp);
     wizlock = sysdata.w_lock;
     log_f("Done.");
     return;
    }
    KEY("Expmult",       sysdata.expmult,       fread_float(fp));
    break;
   case 'K':
    KEY("KillPerLev",    sysdata.killperlev,    fread_number(fp));
    break;
   case 'M':
    KEY("Mob_AC",        sysdata.mob_ac,        fread_float(fp));
    KEY("Mob_DR",        sysdata.mob_dr,        fread_float(fp));
    KEY("Mob_HP",        sysdata.mob_hp,        fread_float(fp));
    KEY("Mob_HR",        sysdata.mob_hr,        fread_float(fp));
    KEY("Mob_MP",        sysdata.mob_mp,        fread_float(fp));
    KEY("Mob_MV",        sysdata.mob_mv,        fread_float(fp));
    KEY("Mob_SVS",       sysdata.mob_svs,       fread_float(fp));
    break;
   case 'P':
    KEY("Playtesters",   sysdata.playtesters,   fread_string(fp));
    KEY("Pulse",         sysdata.pulse,         fread_number(fp));
    break;
   case 'S':
    KEY("Shownumbers",   sysdata.shownumbers,   fread_number(fp));
    break;
   case 'W':
    KEY("Wizlock",       sysdata.w_lock,        fread_number(fp));
    break;
  }
  if( !fMatch )
  {
   xprintf_2(log_buf,"Loading in sysdata: no match for ( %s ).",word);
   monitor_chan(log_buf,MONITOR_BAD);
   fread_to_eol(fp);
  }
 }

 return;
}

void save_sysdata( void )
{
 FILE *fp;

 if( (fp = file_open(SYSDAT_FILE,"w")) == NULL )
 {
  log_f("Failed to save system data.");
  return;
 }

 fprintf(fp, "Build_Obj_AC  %d\n",    sysdata.build_obj_ac);
 fprintf(fp, "Build_Obj_DR  %d\n",    sysdata.build_obj_dr);
 fprintf(fp, "Build_Obj_HP  %d\n",    sysdata.build_obj_hp);
 fprintf(fp, "Build_Obj_HR  %d\n",    sysdata.build_obj_hr);
 fprintf(fp, "Build_Obj_MP  %d\n",    sysdata.build_obj_mp);
 fprintf(fp, "Build_Obj_MV  %d\n",    sysdata.build_obj_mv);
 fprintf(fp, "Build_Obj_SVS %d\n",    sysdata.build_obj_svs);
 fprintf(fp, "Damcap        %d\n",    sysdata.damcap);
 fprintf(fp, "Expmult       %0.4f\n", sysdata.expmult);
 fprintf(fp, "KillPerLev    %d\n",    sysdata.killperlev);
 fprintf(fp, "Mob_AC        %0.4f\n", sysdata.mob_ac);
 fprintf(fp, "Mob_DR        %0.4f\n", sysdata.mob_dr);
 fprintf(fp, "Mob_HP        %0.4f\n", sysdata.mob_hp);
 fprintf(fp, "Mob_HR        %0.4f\n", sysdata.mob_hr);
 fprintf(fp, "Mob_MP        %0.4f\n", sysdata.mob_mp);
 fprintf(fp, "Mob_MV        %0.4f\n", sysdata.mob_mv);
 fprintf(fp, "Mob_SVS       %0.4f\n", sysdata.mob_svs);
 fprintf(fp, "Playtesters   %s~\n",   sysdata.playtesters);
 fprintf(fp, "Pulse         %d\n",    sysdata.pulse);
 fprintf(fp, "Shownumbers   %d\n",    sysdata.shownumbers);
 fprintf(fp, "Wizlock       %d\n",    wizlock);
 fprintf(fp, "End\n\n");

 file_close(fp);

 return;
}


void do_sysdata( CHAR_DATA * ch, char *argument )
{
 char outbuf[MSL];
 char catbuf[MSL];
 char arg1[MSL];
 char arg2[MSL];

 argument = one_argument(argument,arg1);
 argument = one_argument(argument,arg2);

 if( arg1[0] == '\0' )
 {
  send_to_char("Syntax for sysdata:\n\r",ch);
  send_to_char("  sysdata help | show | <option> <value> | <string> <+/-> <new_word>\n\r",ch);
  send_to_char("  strings: playtesters\n\r",ch);
  send_to_char("  options: damcap expmult killsperlev mob[ac | dr | hp | hr | mp | mv | svs] pulse shownumbers\n\r",ch);
  return;
 }

 if( !str_prefix(arg1,"help") )
 {
  do_help(ch,"sysdata");
  return;
 }

 if( !str_prefix(arg1,"show") )
 {
  xprintf(outbuf,"%s","      System data for " mudnamecolor ":\n\r");
  xprintf(catbuf,"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r");
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Option         ]       [Value          ]\n\r");
  xcat(outbuf,catbuf);
  xprintf(catbuf,"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r");
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Damcap         ]       [%15d]\n\r",sysdata.damcap);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Exp Multiplier ]       [%15.4f]\n\r",sysdata.expmult);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Kills Per Lev  ]       [%15d]\n\r",sysdata.killperlev);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Mob AC         ]       [%15.4f]\n\r",sysdata.mob_ac);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Mob DR         ]       [%15.4f]\n\r",sysdata.mob_dr);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Mob HP         ]       [%15.4f]\n\r",sysdata.mob_hp);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Mob HR         ]       [%15.4f]\n\r",sysdata.mob_hr);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Mob MP         ]       [%15.4f]\n\r",sysdata.mob_mp);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Mob MV         ]       [%15.4f]\n\r",sysdata.mob_mv);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Mob SVS        ]       [%15.4f]\n\r",sysdata.mob_svs);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Playtesters    ]       [%15s]\n\r",sysdata.playtesters);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Pulse Per Sec  ]       [%15d]\n\r",sysdata.pulse);
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Show Damage    ]       [%15s]\n\r",(sysdata.shownumbers ? "Yes" : "No"));
  xcat(outbuf,catbuf);
  xprintf(catbuf,"[Wizlocked      ]       [%15s]\n\r",(wizlock ? "Yes" : "No"));
  xcat(outbuf,catbuf);
  xprintf(catbuf,"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r");
  xcat(outbuf,catbuf);
  send_to_char(outbuf,ch);
  return;
 }

 if( !str_prefix(arg1,"damcap") )
  sysdata.damcap = atoi(arg2) > 0 ? atoi(arg2) : 1;
 else if( !str_prefix(arg1,"expmult") )
  sysdata.expmult = atof(arg2) != 0 ? atof(arg2) : 1;
 else if( !str_prefix(arg1,"killsperlev") )
  sysdata.killperlev = atoi(arg2) > 0 ? atoi(arg2) : 1;
 else if( !str_cmp(arg1,"mobac") )
  sysdata.mob_ac = atof(arg2) != 0 ? atof(arg2) : 1;
 else if( !str_cmp(arg1,"mobdr") )
  sysdata.mob_dr = atof(arg2) != 0 ? atof(arg2) : 1;
 else if( !str_cmp(arg1,"mobhp") )
  sysdata.mob_hp = atof(arg2) != 0 ? atof(arg2) : 1;
 else if( !str_cmp(arg1,"mobhr") )
  sysdata.mob_hr = atof(arg2) != 0 ? atof(arg2) : 1;
 else if( !str_cmp(arg1,"mobmp") )
  sysdata.mob_mp = atof(arg2) != 0 ? atof(arg2) : 1;
 else if( !str_cmp(arg1,"mobmv") )
  sysdata.mob_mv = atof(arg2) != 0 ? atof(arg2) : 1;
 else if( !str_cmp(arg1,"mobsvs") )
  sysdata.mob_svs = atof(arg2) != 0 ? atof(arg2) : 1;
 else if( !str_prefix(arg1,"playtesters") )
  sysdata.playtesters = str_mod(sysdata.playtesters,arg2);
 else if( !str_prefix(arg1,"pulse") )
  sysdata.pulse = atoi(arg2) > 0 ? atoi(arg2) : 1;
 else if( !str_prefix(arg1,"shownumbers") )
  sysdata.shownumbers = (sysdata.shownumbers ? FALSE : TRUE);
 else
 {
  do_sysdata(ch,"");
  return;
 }

 save_sysdata( );
 do_sysdata(ch,"show");

 return;
}

void init_sysdata( void )
{
 sysdata.build_obj_ac = -500;
 sysdata.build_obj_dr = 125;
 sysdata.build_obj_hp = 250;
 sysdata.build_obj_hr = 125;
 sysdata.build_obj_mp = 250;
 sysdata.build_obj_mv = 250;
 sysdata.build_obj_svs = -25;
 sysdata.damcap = 3000;
 sysdata.expmult = 1;
 sysdata.killperlev = 60;
 sysdata.mob_ac = 1;
 sysdata.mob_dr = 1;
 sysdata.mob_hp = 1;
 sysdata.mob_hr = 1;
 sysdata.mob_mp = 1;
 sysdata.mob_mv = 1;
 sysdata.mob_svs = 1;
 sysdata.playtesters = "";
 sysdata.pulse = 8;
 sysdata.shownumbers = TRUE;
 sysdata.w_lock = FALSE;

 return;
}
