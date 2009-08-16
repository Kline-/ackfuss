/*
 * Copyright Matt Goff (Kline) 2009
 * If you use my code, please give credit where it is due.
 * Support provided at www.ackmud.net
 */

#include "h/areaconvert.h"
#include "h/extern.h"

void write_ackfuss( ofstream &file, int type )
{
 write_ackfuss_area(file,type);

 return;
}

void write_ackfuss_area( ofstream &file, int type )
{
 char delim = '~';

 file << "#AREA" << endl;
 file << "Revision  " << area.revision << endl;
 file << "CanRead  " << area.can_read << delim << endl;
 file << "CanWrite " << area.can_write << delim << endl;
 switch( type )
 {
  case TYPE_ACK431:
   file << "Flags     ";
   if( I_BIT(area.int_flags,ACK431_AFLAG_BUILDING) )    { file << ACKFUSS_AFLAG_BUILDING << " "; }
   if( I_BIT(area.int_flags,ACK431_AFLAG_NO_ROOM_AFF) ) { file << ACKFUSS_AFLAG_NO_ROOM_AFF << " "; }
   if( I_BIT(area.int_flags,ACK431_AFLAG_PAYAREA) )     { file << ACKFUSS_AFLAG_PAYAREA << " "; }
   if( I_BIT(area.int_flags,ACK431_AFLAG_NOSHOW) )      { file << ACKFUSS_AFLAG_NOSHOW << " "; }
   if( I_BIT(area.int_flags,ACK431_AFLAG_TELEPORT) )    { file << ACKFUSS_AFLAG_TELEPORT << " "; }
   file << "EOL" << endl;
   break;
 }
 file << "Keyword  " << area.keyword << delim << endl;
 file << "LevLabel " << area.level_label << delim << endl;
 file << "LevRange  " << area.min_level << " " << area.max_level << endl;
 file << "Name      " << area.name << delim << endl;
 file << "Owner    " << area.owner << delim << endl;
 file << "ResetMsg " << area.reset_msg << delim << endl;
 file << "ResetRate " << area.reset_rate << endl;
 file << "VnumRange " << area.min_vnum << " " << area.max_vnum << endl;
 file << "End" << endl;

 return;
}
