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
 write_ackfuss_room(file,type);

 file << "#$" << endl; /* the end */

 return;
}

void write_ackfuss_area( ofstream &file, int type )
{
 char delim = '~';

 file << "#AREA" << endl;
 file << "Revision  " << area.revision << endl;
 file << "CanRead  " << area.can_read << delim << endl;
 file << "CanWrite " << area.can_write << delim << endl;
 file << "Flags     ";
 for( int i = 0; i < MAX_BITSET; i++ )
  if( area.bitset_flags_out.test(i) )
   file << i << " ";
 file << "EOL" << endl;
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

void write_ackfuss_room( ofstream &file, int type )
{
 char delim = '~';
 list<room_data *>::iterator it;
 room_data *room;

 for( it = room_list.begin(); it != room_list.end(); it++ )
 {
  room = *it;
  file << "#ROOM" << endl;
  file << "Vnum  " << room->vnum << endl;
  file << "Desc " << room->description << delim << endl;
  file << "Flags EOL" << endl;
  file << "Name  " << room->name << delim << endl;
  file << "Sect  " << room->sector << endl;
  file << "End" << endl;
 }

 return;
}
