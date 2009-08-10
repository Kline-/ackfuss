/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#include "h/globals.h"

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_MAGIC_H
#include "h/magic.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

bool char_data::check_cooldown( const char *skill )
{
 int sn = skill_lookup(skill);

 if( sn < 0 )
  return false;

 if( skill_table[sn].cooldown <= COOLDOWN_NONE )
  return false;

 if( cooldown[skill_table[sn].cooldown] > 0 )
 {
  switch( skill_table[sn].cooldown )
  {
   case COOLDOWN_OFF: send_to_char("@@eYour offensive abilities are on cooldown right now.\r\n",this); break;
   case COOLDOWN_DEF: send_to_char("@@lYour defensive abilities are on cooldown right now.\r\n",this); break;
  }
  return true;
 }

 return false;
}

bool char_data::check_cooldown( int pos )
{
 if( pos <= COOLDOWN_NONE )
  return false;

 if( cooldown[pos] > 0 )
 {
  switch( pos )
  {
   case COOLDOWN_OFF: send_to_char("@@eYour offensive abilities are on cooldown right now.\r\n",this); break;
   case COOLDOWN_DEF: send_to_char("@@lYour defensive abilities are on cooldown right now.\r\n",this); break;
  }
  return true;
 }

 return false;
}

void char_data::gain_exp( int gain )
{
 if( IS_NPC(this) && !act.test(ACT_INTELLIGENT) )
  return;
 if( IS_IMMORTAL(this) )
  return;

 if( (exp + gain) >= MAX_EXP )
  exp = MAX_EXP;
 else
  exp += gain;

 if( exp < 0 )
  exp = 0;

 return;
}

short char_data::get_level( const char *what )
{
 short i = 0, max = 0;;

 if( what == '\0' )
  return level;

 if( !str_prefix(what,"maxmortal") )
 {
  for( i = 0; i < MAX_CLASS; i++ )
   if( lvl[i] > max )
    max = lvl[i];
  return max;
 }
 if( !str_prefix(what,"maxremortal") )
 {
  for( i = 0; i < MAX_CLASS; i++ )
   if( lvl2[i] > max )
    max = lvl2[i];
  return max;
 }
 if( !str_prefix(what,"mortal") )
  return (lvl[CLS_MAG] + lvl[CLS_CLE] + lvl[CLS_THI] + lvl[CLS_WAR] + lvl[CLS_PSI]);
 if( !str_prefix(what,"remortal") )
  return (lvl2[CLS_SOR] + lvl2[CLS_MON] + lvl2[CLS_ASS] + lvl2[CLS_KNI] + lvl2[CLS_NEC]);
 if( !str_prefix(what,"psuedo") )
  return (level + (get_level("remortal")/4));

 if( !str_prefix(what,"adept") )
  return IS_NPC(this) ? level / 7 : pcdata->adept_level;
 if( !str_prefix(what,"mage") )
  return lvl[CLS_MAG];
 if( !str_prefix(what,"cleric") )
  return lvl[CLS_CLE];
 if( !str_prefix(what,"thief") )
  return lvl[CLS_THI];
 if( !str_prefix(what,"warrior") )
  return lvl[CLS_WAR];
 if( !str_prefix(what,"psionicist") )
  return lvl[CLS_PSI];
 if( !str_prefix(what,"sorcerer") )
  return lvl2[CLS_SOR];
 if( !str_prefix(what,"monk") )
  return lvl2[CLS_MON];
 if( !str_prefix(what,"assassin") )
  return lvl2[CLS_ASS];
 if( !str_prefix(what,"knight") )
  return lvl2[CLS_KNI];
 if( !str_prefix(what,"necromancer") )
  return lvl2[CLS_NEC];

 snprintf(log_buf,(2 * MIL),"char_data::get_level(): Received invalid request for '%s'.",what);
 monitor_chan(log_buf,MONITOR_DEBUG);
 return level;
}

const char *char_data::get_name( CHAR_DATA *looker )
{
 if( looker != NULL )
 {
  if( can_see(looker,this) )
  {
   if( IS_NPC(this) )
    return npcdata->short_descr;
   else
   {
    if( IS_WOLF(this) && (IS_SHIFTED(this) || IS_RAGED(this)) )
     return "A Large @@bWolf@@N";
    else
     return name.c_str();
   }
  }
  else
  {
   if( IS_IMMORTAL(this) )
    return "A Mystical Being";
   else
    return "Someone";
  }
 }
 else
 {
  if( IS_NPC(this) )
   return npcdata->short_descr;
  else
   return name.c_str();
 }

 return "Unknown!";
}

const char *char_data::get_title( )
{
 if( IS_NPC(this) )
  return "";
 else
  return pcdata->title;
}

void char_data::send( string txt, ... )
{
 char buf[MSL];
 va_list args;

 if( this == NULL || desc == NULL )
  return;

 if( txt.empty() )
  return;

 va_start(args,txt);
 vsprintf(buf,txt.c_str(),args);
 va_end(args);

 if( desc->showstr_head != NULL )
 {
  char *ssh;

  ssh = (char *)qgetmem( strlen( desc->showstr_head ) + strlen( buf ) + 1 );
  strcpy( ssh, desc->showstr_head );
  strcat( ssh, buf );

  if( desc->showstr_point )
   desc->showstr_point += ( ssh - desc->showstr_head );
  else
   desc->showstr_point = ssh;

  free(desc->showstr_head);
  desc->showstr_head = ssh;
 }
 else
 {
  desc->showstr_head = (char *)malloc( strlen( buf ) + 1);
  strcpy( desc->showstr_head, buf );
  desc->showstr_point = desc->showstr_head;
 }
 if( desc->showstr_point == desc->showstr_head )
  show_string( desc, "" );
 return;
}

void char_data::set_cooldown( const char *skill )
{
 int sn = skill_lookup(skill);

 if( sn < 0 )
  return;

 if( skill_table[sn].cooldown <= COOLDOWN_NONE )
  return;

 cooldown[skill_table[sn].cooldown] += skill_table[sn].beats;

 return;
}

void char_data::set_cooldown( int pos, float duration )
{
 if( pos <= COOLDOWN_NONE )
  return;

 cooldown[pos] += duration;

 return;
}

void char_data::set_title( const char *title )
{
 if( IS_NPC(this) )
  return;

 free_string(pcdata->title);
 pcdata->title = str_dup(title);

 return;
}
