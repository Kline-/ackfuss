/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#include "h/globals.h"

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_MAGIC_H
#include "h/magic.h"
#endif

bool char_data::check_cooldown( const char *skill )
{
 int sn = skill_lookup(skill);

 if( sn < 0 )
  return false;

 if( skill_table[sn].cooldown <= COOLDOWN_NONE )
  return false;

 if( this->cooldown[skill_table[sn].cooldown] > 0 )
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

 if( this->cooldown[pos] > 0 )
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
 if( IS_NPC(this) && !this->act.test(ACT_INTELLIGENT) )
  return;
 if( IS_IMMORTAL(this) )
  return;

 if( (this->exp + gain) >= MAX_EXP )
  this->exp = MAX_EXP;
 else
  this->exp += gain;

 if( this->exp < 0 )
  this->exp = 0;

 return;
}

short char_data::get_level( const char *what )
{
 if( IS_NPC(this) )
  return this->level;

 if( !str_cmp(what,"adept") )
  return this->pcdata->adept_level;
 if( !str_prefix(what,"mage") )
  return this->lvl[CLS_MAG];
 if( !str_prefix(what,"cleric") )
  return this->lvl[CLS_CLE];
 if( !str_prefix(what,"thief") )
  return this->lvl[CLS_THI];
 if( !str_prefix(what,"warrior") )
  return this->lvl[CLS_WAR];
 if( !str_prefix(what,"psionicist") )
  return this->lvl[CLS_PSI];
 if( !str_prefix(what,"sorcerer") )
  return this->lvl2[CLS_SOR];
 if( !str_prefix(what,"monk") )
  return this->lvl2[CLS_MON];
 if( !str_prefix(what,"assassin") )
  return this->lvl2[CLS_ASS];
 if( !str_prefix(what,"knight") )
  return this->lvl2[CLS_KNI];
 if( !str_prefix(what,"necromancer") )
  return this->lvl2[CLS_NEC];

 return this->level;
}

void char_data::set_cooldown( const char *skill )
{
 int sn = skill_lookup(skill);

 if( sn < 0 )
  return;

 if( skill_table[sn].cooldown <= COOLDOWN_NONE )
  return;

 this->cooldown[skill_table[sn].cooldown] += skill_table[sn].beats;

 return;
}

void char_data::set_cooldown( int pos, float duration )
{
 if( pos <= COOLDOWN_NONE )
  return;

 this->cooldown[pos] += duration;

 return;
}
