/*
 * Copyright Matt Goff (Kline) 2009
 * If you use my code, please give credit where it is due.
 * Support provided at www.ackmud.net
 */

#include "include.h"
#include "ack431.h"
#include "ackfuss.h"

#define VERS "AreaConvert 1.0"
#define SPACER "--------------------------------------------------"

#define TYPE_NONE   -1
#define TYPE_ACK431  0
#define TYPE_ACKFUSS 1

#define MAX_BITSET 256
#define MAX_EXIT   6

#define I_BIT(var,bit) (var & bit)
#define S_BIT(var,bit) (var |= bit)
#define R_BIT(var,bit) (var &= ~bit)

void display_help();
void cleanup_outfile ( string filename );
void flag_handler    ( int typein, int typeout );
void clear_area_flag ( string name );

bool infile_init     ( string filename, ifstream &file );
bool outfile_init    ( string filename, ofstream &file );
bool typein_init     ( string name, int &type );
bool typeout_init    ( string name, int &type );

void process_infile  ( ifstream &file, int type );
void process_outfile ( ofstream &file, int typein, int typeout );

int str2int          ( string &str );
string int2str       ( int i );

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
  string flags_found;

  bitset<MAX_BITSET> bitset_flags_in;
  bitset<MAX_BITSET> bitset_flags_out;
  int int_flags_in;
  int int_flags_out;
  int min_vnum;
  int max_vnum;
  int min_level;
  int max_level;
  int reset_rate;
  int revision;
};

class exit_data
{
 public:
  string description;
  string keyword;

  bitset<MAX_BITSET> bitset_flags_in;
  bitset<MAX_BITSET> bitset_flags_out;
  int int_flags_in;
  int int_flags_out;
  int key;
  int vnum;
};

class extra_data
{
 public:
  string keyword;
  string description;
};

class room_data
{
 public:
  string name;
  string description;

  bitset<MAX_BITSET> bitset_flags_in;
  bitset<MAX_BITSET> bitset_flags_out;
  int vnum;
  int int_flags_in;
  int int_flags_out;
  int sector;
  exit_data *exit[MAX_EXIT];
};

class npc_data
{
 public:
};

class obj_data
{
 public:
};

class reset_data
{
 public:
};
