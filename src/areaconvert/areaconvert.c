#include <cstdlib>
#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

#define VERS "AreaConvert 1.0"
#define SPACER "--------------------------------------------------"

#define TYPE_ACK431  0
#define TYPE_ACKFUSS 1

void display_help();
void cleanup_outfile    ( string filename );

bool infile_init        ( string filename, ifstream &file );
bool outfile_init       ( string filename, ofstream &file );
bool typein_init        ( string name, int &type );
bool typeout_init       ( string name, int &type );

void process_infile     ( ifstream &file, int type );
void process_outfile    ( ofstream &file, int type );

int str2int             ( string &str );
string int2str          ( int i );

void read_ack431        ( ifstream &file );
void read_ack431_area   ( ifstream &file );

void write_ackfuss      ( ofstream &file );
void write_ackfuss_area ( ofstream &file );

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
class area_data
{
 public:
  string filename;
  string name;
  string owner;
  string can_read;
  string can_write;
  string keyword;
  string level_label;
  string reset_msg;

  int flags;
  int min_vnum;
  int max_vnum;
  int min_level;
  int max_level;
  int reset_rate;
  int revision;
};

area_data area;
string flags_found;

int main( int argc, char *argv[] )
{
 string filename, source, dest;
 ifstream infile;
 ofstream outfile;
 int typein, typeout;

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

 process_infile(infile,typein);
 process_outfile(outfile,typeout);

 infile.close();
 outfile.close();

 return 0;
}

void display_help()
{
 cout << endl << VERS << endl;
 cout << SPACER << endl;
 cout << "areaconvert filename [source format] [dest format]" << endl;
 cout << SPACER << endl;
 cout << "filename will be saved as filename.converted" << endl;
 cout << "default options are listed in (parenthesis)" << endl;
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
  else
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
  else
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

void process_outfile( ofstream &file, int type )
{
 switch( type )
 {
  case TYPE_ACKFUSS: write_ackfuss(file); break;
 }

 return;
}

int str2int( string &str )
{
 stringstream ss(str);
 int i;
 ss >> i;
 return i;
}

string int2str( int i )
{
 stringstream ss;
 ss << i;
 return ss.str();
}

void read_ack431( ifstream &file )
{
 string tmp;

 read_ack431_area(file);

 return;
}

void read_ack431_area( ifstream &file )
{
 string tmp;
 char delim = '~';
 char c;

 file.ignore(6);    /* #AREA */
 getline(file,area.name,delim);

 for( ;; )
 {
  c = file.get();

  if( c == '#' )
   break;

  switch( c )
  {
   case 'B': getline(file,tmp); flags_found += "building ";     break;
   case 'F': getline(file,tmp); area.reset_rate = str2int(tmp); break;
   case 'I': file.ignore(1); getline(file,tmp,' '); area.min_level = str2int(tmp); getline(file,tmp); area.max_level = str2int(tmp); break;
   case 'K': getline(file,area.keyword,delim);                  break;
   case 'L': getline(file,area.level_label,delim);              break;
   case 'M': getline(file,tmp); flags_found += "no_room_affs "; break;
   case 'N': getline(file,tmp);                                 break;
   case 'O': getline(file,area.owner,delim);                    break;
   case 'P': getline(file,tmp); flags_found += "pay_area ";     break;
   case 'Q': getline(file,tmp); area.revision = str2int(tmp);   break;
   case 'R': getline(file,area.can_read,delim);                 break;
   case 'S': getline(file,tmp); flags_found += "no_show ";      break;
   case 'T': getline(file,tmp); flags_found += "teleport ";     break;
   case 'U': getline(file,area.reset_msg,delim);                break;
   case 'V': file.ignore(1); getline(file,tmp,' '); area.min_vnum = str2int(tmp); getline(file,tmp); area.max_vnum = str2int(tmp); break;
   case 'W': getline(file,area.can_write,delim);                break;
   case 'X': getline(file,tmp);                                 break;
  }
 }

 return;
}

void write_ackfuss( ofstream &file )
{
 write_ackfuss_area(file);

 return;
}

void write_ackfuss_area( ofstream &file )
{
 char delim = '~';

 file << "#AREA" << endl;
 file << "Revision  " << area.revision << endl;
 file << "CanRead  " << area.can_read << delim << endl;
 file << "CanWrite " << area.can_write << delim << endl;
 file << "Flags     EOL" << endl;
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
