/*
 * Copyright Matt Goff (Kline) 2009
 * If you use my code, please give credit where it is due.
 * Support provided at www.ackmud.net
 */

#include "include.h"
#include "ack431.h"
#include "ackfuss.h"
#include "flags.h"

#define VERS "AreaConvert 1.0"
#define SPACER "--------------------------------------------------"

#define TYPE_NONE   -1
#define TYPE_ACK431  0
#define TYPE_ACKFUSS 1

#define MAX_BITSET    256
#define MAX_EXIT      6
#define MAX_OBJ_VALUE 10
#define MAX_TRADE     5

#define I_BIT(var,bit) (var & bit)
#define S_BIT(var,bit) (var |= bit)
#define R_BIT(var,bit) (var &= ~bit)

void display_help     ( void );
void cleanup_outfile  ( string filename );

bool infile_init      ( string filename, ifstream &file );
bool outfile_init     ( string filename, ofstream &file );
bool typein_init      ( string name, int &type );
bool typeout_init     ( string name, int &type );

void process_infile   ( ifstream &file, int type );
void process_outfile  ( ofstream &file, int typein, int typeout );
void shutdown_cleanup ( void );

int str2int           ( string &str );
string int2str        ( int i );

class DeleteObject
{
 public:
  template <typename T> void operator() (const T* ptr) const { delete ptr; };
};

class affect_data
{
 public:
  int int_location_in;
  int int_location_out;
  int modifier;
};

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
  room_data()  { for( short i = 0; i < MAX_EXIT; i++ ) exit[i] = NULL; };
  ~room_data() { for( short i = 0; i < MAX_EXIT; i++ ) if(exit[i]) delete exit[i]; for_each(extra_list.begin(),extra_list.end(),DeleteObject()); };
  string name;
  string description;

  list<extra_data *> extra_list;
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
  string player_name;
  string short_descr;
  string long_descr;
  string description;

  bitset<MAX_BITSET> bitset_act_flags_out;
  int affected_by;
  int alignment;
  int level;
  int sex;
  int ac_mod;
  int hr_mod;
  int dr_mod;
  int pclass;
  int clan;
  int position;
  int race;
  int skills;
  int cast;
  int def;
  int strong_magic;
  int weak_magic;
  int race_mod;
  int power_skill;
  int power_cast;
  int resist;
  int suscept;
  int int_act_flags_in;
  int vnum;
};

class obj_data
{
 public:
  obj_data()  { int i = 0; int_item_apply_out = 0; for( i = 0; i < MAX_OBJ_VALUE; i++ ) value[i] = 0; };
  ~obj_data() { for_each(apply_list.begin(),apply_list.end(),DeleteObject()); for_each(extra_list.begin(),extra_list.end(),DeleteObject()); };
  string name;
  string short_descr;
  string long_descr;

  list<affect_data *> apply_list;
  list<extra_data *> extra_list;
  bitset<MAX_BITSET> bitset_extra_flags_out;
  bitset<MAX_BITSET> bitset_wear_flags_out;
  int type;
  int int_extra_flags_in;
  int int_wear_flags_in;
  int int_item_apply_in;
  int int_item_apply_out;
  int value[MAX_OBJ_VALUE];
  int weight;
  int level;
  int vnum;
};

class shop_data
{
 public:
  shop_data() { int i = 0; for( i = 0; i < MAX_TRADE; i++ ) buy_type[i] = 0; }
  int buy_type[MAX_TRADE];
  int hour_close;
  int hour_open;
  int keeper;
  int prof_buy;
  int prof_sell;
};

class reset_data
{
 public:
  string command;
  string notes;

  int arg1;
  int arg2;
  int arg3;
  int count;
};
