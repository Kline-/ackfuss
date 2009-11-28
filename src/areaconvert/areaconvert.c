/*
 * Copyright Matt Goff (Kline) 2009
 * If you use my code, please give credit where it is due.
 * Support provided at www.ackmud.net
 */

#include "h/areaconvert.h"

/* Store variables for all formats we will support, just don't write the ones we don't need later :) */

/*
 * format: ack431                      format: ackfuss
 *
 * #AREA                               #AREA
 * name                                Revision
 * Q -- revision                       CanRead   none~
 * K -- keyword                        CanWrite  none~
 * L -- level_label                    Flags     EOL~
 * N -- ignore                         Keyword   none~
 * I -- min_level, max_level           LevLabel  none~
 * V -- min_vnum, max_vnum             LevRange  1 20
 * X -- offset                         Name      none~
 * F -- reset_rate                     Owner     none~
 * U -- reset_msg                      ResetMsg  none~
 * O -- owner                          ResetRate 15
 * R -- can_read                       VnumRange 1 100
 * W -- can_write                      End
 * T -- flag TELEPORT
 * P -- flag PAY_AREA
 * M -- flag NO_ROOM_AFF
 * B -- flag BUILDING
 * S -- flag NO_SHOW
*/

area_data        area;
list<room_data *>  room_list;
list<npc_data *>   npc_list;
list<obj_data *>   obj_list;
list<shop_data *>  shop_list;
list<reset_data *> reset_list;

int main( int argc, char *argv[] )
{
 string filename, source, dest;
 ifstream infile;
 ofstream outfile;
 int typein = TYPE_NONE, typeout = TYPE_NONE;

 if( argc < 2 || argc > 4 )
 {
  display_help();
  return 0;
 }

 if( argc >= 2 )
  filename = argv[1];
 if( argc >= 3 )
  source = argv[2];
 if( argc == 4 )
  dest = argv[3];

 if( !infile_init(filename,infile) || !outfile_init(filename,outfile) )
 {
  cleanup_outfile(filename);
  return 0;
 }

 if( !typein_init(source,typein) || !typeout_init(dest,typeout) )
 {
  cleanup_outfile(filename);
  return 0;
 }

 if( typein == typeout )
 {
  cout << "You can not have the same source and destination format. Aborting." << endl;
  cleanup_outfile(filename);
  return 0;
 }

 process_infile(infile,typein);
 flag_handler(typein,typeout);
 process_outfile(outfile,typein,typeout);

 infile.close();
 outfile.close();
 shutdown_cleanup();

 return 0;
}

void display_help( void )
{
 cout << endl << VERS << endl;
 cout << SPACER << endl;
 cout << "areaconvert filename [source format] [dest format]" << endl;
 cout << SPACER << endl;
 cout << "filename will be saved as filename.converted" << endl;
 cout << "default options are listed in (parenthesis)" << endl;
 cout << SPACER << endl;
 cout << "if a bit/flag is not converted you will receive" << endl;
 cout << "a list of them so you may manually attempt to" << endl;
 cout << SPACER << endl;
 cout << "supported src formats: (ack431)" << endl;
 cout << "supported dst formats: (ackfuss)" << endl << endl;
 return;
}

void cleanup_outfile( string filename )
{
 string cmd = "rm -f ";

 filename += ".converted";
 cmd += filename;             /* system returns different values on different platforms */
 if( system(cmd.c_str()) ) {} /* so we wrap it inside an if() to silence ignore the return result */

 return;
}

bool infile_init( string filename, ifstream &file )
{
 file.open(filename.c_str(),ios::in);
 if( file.is_open() )
  return true;

 cout << "Unable to open input file [" << filename << "]. Aborting." << endl;

 return false;
}

bool outfile_init( string filename, ofstream &file )
{
 filename += ".converted";
 file.open(filename.c_str(),ios::out);
 if( file.is_open() )
  return true;

 cout << "Unable to open output file [" << filename << "]. Aborting." << endl;

 return false;
}

bool typein_init( string name, int &type )
{
 if( name.empty() )
  type = TYPE_ACK431;
 else
 {
  if( name == "ack431" )
   type = TYPE_ACK431;
  if( type == TYPE_NONE )
  {
   cout << "Unknown input type [" << name << "]. Aborting." << endl;
   return false;
  }
 }

 return true;
}

bool typeout_init( string name, int &type )
{
 if( name.empty() )
  type = TYPE_ACKFUSS;
 else
 {
  if( name == "ackfuss" )
   type = TYPE_ACKFUSS;
  if( type == TYPE_NONE )
  {
   cout << "Unknown output type [" << name << "]. Aborting." << endl;
   return false;
  }
 }

 return true;
}

void process_infile( ifstream &file, int type )
{
 switch( type )
 {
  case TYPE_ACK431: read_ack431(file); break;
 }

 return;
}

void process_outfile( ofstream &file, int typein, int typeout )
{
 switch( typeout )
 {
  case TYPE_ACKFUSS: write_ackfuss(file,typein); break;
 }

 return;
}

void shutdown_cleanup( void )
{
 if( !area.flags_found.empty() )
  cout << "The following area flags were not converted: " << area.flags_found << endl;
 else
  cout << "Area converted successfully." << endl;

 for_each(room_list.begin(),room_list.end(),DeleteObject());
 for_each(npc_list.begin(),npc_list.end(),DeleteObject());
 for_each(obj_list.begin(),obj_list.end(),DeleteObject());
 for_each(shop_list.begin(),shop_list.end(),DeleteObject());
 for_each(reset_list.begin(),reset_list.end(),DeleteObject());

 return;
}

int str2int( string &str )
{
 stringstream ss(str);
 int i = 0;

 ss >> i;

 return i;
}

string int2str( int i )
{
 stringstream ss;

 ss << i;

 return ss.str();
}
