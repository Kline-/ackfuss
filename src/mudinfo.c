/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#include "h/globals.h"

#ifndef ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_MUDINFO_H
#include "h/mudinfo.h"
#endif

#define KEY( literal, field, value )  if ( !str_cmp( word, literal ) ) { field  = value; fMatch = TRUE;  break;}
#define SKEY( literal, field, value )  if ( !str_cmp( word, literal ) ) { if (field!=NULL) free_string(field);field  = value; fMatch = TRUE;  break;}


void load_mudinfo( void )
{
 FILE *fp;
 const char *word;
 bool fMatch;

 snprintf(log_buf,(2 * MIL),"Loading %s",MUDINFO_FILE);
 log_f("%s",log_buf);

 init_mudinfo(); /* Need to set some defaults --Kline */

 if( (fp = file_open(MUDINFO_FILE,"r")) == NULL )
 {
  file_close(fp);
  log_f("Failed to load mudinfo data.");
  return;
 }

 for( ; ; )
 {
  word = feof(fp) ? "End" : fread_word(fp);
  fMatch = FALSE;

  switch( UPPER(word[0]) )
  {
   case 'E':
    if( !str_cmp(word,"End") )
    {
     file_close(fp);
     log_f("Done.");
     return;
    }
    break;
   case 'F':
    KEY("First_Boot",   mudinfo.first_boot,   fread_number(fp));
    break;
   case 'M':
    KEY("MK_By_NPC",    mudinfo.mk_by_npc,    fread_number(fp));
    KEY("MK_By_PC",     mudinfo.mk_by_pc,     fread_number(fp));
    break;
   case 'P':
    KEY("PK_By_NPC",    mudinfo.pk_by_npc,    fread_number(fp));
    KEY("PK_By_PC",     mudinfo.pk_by_pc,     fread_number(fp));
    break;
   case 'T':
    KEY("Total_Pfiles", mudinfo.total_pfiles, fread_number(fp));
    break;
  }
  if( !fMatch )
  {
   snprintf(log_buf,(2 * MIL),"Loading in mudinfo: no match for ( %s ).",word);
   monitor_chan(log_buf,MONITOR_BAD);
   fread_to_eol(fp);
  }
 }

 return;
}

void save_mudinfo( void )
{
 FILE *fp;

 if( (fp = file_open(MUDINFO_FILE,"w")) == NULL )
 {
  file_close(fp);
  log_f("Failed to save mudinfo data.");
  return;
 }

 fprintf(fp, "First_Boot   %lu\n", mudinfo.first_boot);
 fprintf(fp, "MK_By_NPC    %lu\n", mudinfo.mk_by_npc);
 fprintf(fp, "MK_By_PC     %lu\n", mudinfo.mk_by_pc);
 fprintf(fp, "PK_By_NPC    %lu\n", mudinfo.pk_by_npc);
 fprintf(fp, "PK_By_PC     %lu\n", mudinfo.pk_by_pc);
 fprintf(fp, "Total_Pfiles %d\n",  mudinfo.total_pfiles);
 fprintf(fp, "End\n\n");

 file_close(fp);

 return;
}


DO_FUN(do_mudinfo)
{
 string str;

 send_to_char("      MUD info for " mudnamecolor ":\r\n",ch);
 send_to_char("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n",ch);

 str = ctime(&mudinfo.first_boot);
 str.erase(str.end()-1,str.end());
 ch_printf(ch,"The MUD was first booted on: %s.\r\n",str.c_str());

 if( mudinfo.mk_by_npc == 0 )
  send_to_char("No mobs have died to other mobs yet!\r\n",ch);
 else if( mudinfo.mk_by_npc == 1 )
  send_to_char("1 mob has died to another mob. How sad :(.\r\n",ch);
 else
  ch_printf(ch,"%lu mobs have died to other mobs.\r\n",mudinfo.mk_by_npc);

 if( mudinfo.mk_by_pc == 0 )
  send_to_char("No mobs have died to players yet!\r\n",ch);
 else if( mudinfo.mk_by_pc == 1 )
  send_to_char("1 mob has died to a player. How sad :(.\r\n",ch);
 else
  ch_printf(ch,"%lu mobs have died to players.\r\n",mudinfo.mk_by_pc);

 if( mudinfo.pk_by_npc == 0 )
  send_to_char("No players have died to mobs yet!\r\n",ch);
 else if( mudinfo.pk_by_npc == 1 )
  send_to_char("1 player has died to a mob. How sad :(.\r\n",ch);
 else
  ch_printf(ch,"%lu players have died to mobs.\r\n",mudinfo.pk_by_npc);

 if( mudinfo.pk_by_pc == 0 )
  send_to_char("No players have died to other players yet!\r\n",ch);
 else if( mudinfo.pk_by_pc == 1 )
  send_to_char("1 player has died to another player. How sad :(.\r\n",ch);
 else
  ch_printf(ch,"%lu players have died to other players.\r\n",mudinfo.pk_by_pc);

 if( mudinfo.total_pfiles == 1 )
  send_to_char("There is only a single pfile. How lonely :(.\r\n",ch);
 else
  ch_printf(ch,"There are currently @@W%d @@Npfiles.\r\n",mudinfo.total_pfiles);

 return;
}

void init_mudinfo( void )
{
 mudinfo.first_boot = 0;
 mudinfo.mk_by_npc = 0;
 mudinfo.mk_by_pc = 0;
 mudinfo.pk_by_npc = 0;
 mudinfo.pk_by_pc = 0;
 mudinfo.total_pfiles = 0;

 return;
}
