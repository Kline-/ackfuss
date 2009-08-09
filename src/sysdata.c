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

#include "h/globals.h"

#ifndef DEC_ACT_INFO_H
#include "h/act_info.h"
#endif

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

#ifndef DEC_SYSDATA_H
#include "h/sysdata.h"
#endif

#define KEY( literal, field, value )  if ( !str_cmp( word, literal ) ) { field  = value; fMatch = TRUE;  break;}
#define SKEY( literal, field, value )  if ( !str_cmp( word, literal ) ) { if (field!=NULL) free_string(field);field  = value; fMatch = TRUE;  break;}

extern bool wizlock;

void load_sysdata( void )
{
 FILE *fp;
 const char *word;
 bool fMatch;

 snprintf(log_buf,(2 * MIL),"Loading %s",SYSDAT_FILE);
 log_f("%s",log_buf);

 init_sysdata(); /* Need to set some defaults --Kline */

 if( (fp = file_open(SYSDAT_FILE,"r")) == NULL )
 {
  file_close(fp);
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
    KEY("Max_Pushback",  sysdata.max_pushback,  fread_float(fp));
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
   snprintf(log_buf,(2 * MIL),"Loading in sysdata: no match for ( %s ).",word);
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
  file_close(fp);
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
 fprintf(fp, "Max_Pushback  %0.4f\n", sysdata.max_pushback);
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
  send_to_char("Syntax for sysdata:\r\n",ch);
  send_to_char("  sysdata help | show | <option> <value> | <string> <+/-> <new_word>\r\n",ch);
  send_to_char("  strings: playtesters\r\n",ch);
  send_to_char("  options: damcap expmult killsperlev maxpushback mob[ac | dr | hp | hr | mp | mv | svs] pulse shownumbers\r\n",ch);
  return;
 }

 if( !str_prefix(arg1,"help") )
 {
  do_help(ch,"sysdata");
  return;
 }

 if( !str_prefix(arg1,"show") )
 {
  snprintf(outbuf,MSL,"%s","      System data for " mudnamecolor ":\r\n");
  snprintf(catbuf,MSL,"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n");
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Option         ]       [Value          ]\r\n");
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n");
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Damcap         ]       [%15d]\r\n",sysdata.damcap);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Exp Multiplier ]       [%15.4f]\r\n",sysdata.expmult);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Kills Per Lev  ]       [%15d]\r\n",sysdata.killperlev);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Max Pushback   ]       [%15.4f]\r\n",sysdata.max_pushback);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Mob AC         ]       [%15.4f]\r\n",sysdata.mob_ac);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Mob DR         ]       [%15.4f]\r\n",sysdata.mob_dr);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Mob HP         ]       [%15.4f]\r\n",sysdata.mob_hp);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Mob HR         ]       [%15.4f]\r\n",sysdata.mob_hr);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Mob MP         ]       [%15.4f]\r\n",sysdata.mob_mp);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Mob MV         ]       [%15.4f]\r\n",sysdata.mob_mv);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Mob SVS        ]       [%15.4f]\r\n",sysdata.mob_svs);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Playtesters    ]       [%15s]\r\n",sysdata.playtesters);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Pulse Per Sec  ]       [%15d]\r\n",sysdata.pulse);
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Show Damage    ]       [%15s]\r\n",(sysdata.shownumbers ? "Yes" : "No"));
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"[Wizlocked      ]       [%15s]\r\n",(wizlock ? "Yes" : "No"));
  strncat(outbuf,catbuf,MSL-1);
  snprintf(catbuf,MSL,"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n");
  strncat(outbuf,catbuf,MSL-1);
  send_to_char(outbuf,ch);
  return;
 }

 if( !str_prefix(arg1,"damcap") )
  sysdata.damcap = atoi(arg2) > 0 ? atoi(arg2) : 1;
 else if( !str_prefix(arg1,"expmult") )
  sysdata.expmult = atof(arg2) != 0 ? atof(arg2) : 1;
 else if( !str_prefix(arg1,"killsperlev") )
  sysdata.killperlev = atoi(arg2) > 0 ? atoi(arg2) : 1;
 else if( !str_prefix(arg1,"maxpushback") )
  sysdata.max_pushback = atof(arg2) != 0 ? atof(arg2) : 1;
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
 sysdata.max_pushback = 1;
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
