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
 *                                                                         *
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

using namespace std;

#define DEC_ACK_H

#ifndef DEC_CONFIG_H
#include "config.h"
#endif

#ifdef IMC
#ifndef DEC_IMC_H
#include "imc.h"
#endif
#endif

#ifndef DEC_LISTS_H
#include "lists.h"
#endif

#ifndef DEC_MQUEST_H
#include "mquest.h"
#endif

#ifndef DEC_STRFUNS_H
#include "strfuns.h"
#endif

#ifndef DEC_TYPEDEFS_H
#include "typedefs.h"
#endif

#ifndef DEC_UTILS_H
#include "utils.h"
#endif

struct DeleteObject
{
 template <typename T> void operator() (const T* ptr) const { delete ptr; };
};

class lua_data
{
 public:
  lua_data();
  ~lua_data();
  lua_State *L;
  bool loaded;
  void *owner;
  int type;
};

class board_data
{
 public:
  board_data();
  ~board_data();
  int vnum;
  MESSAGE_DATA *first_message;
  MESSAGE_DATA *last_message;
  int min_read_lev;
  int min_write_lev;
  int expiry_time;
  int clan;
};

class disabled_data
{
 public:
  disabled_data();
  ~disabled_data();
  struct cmd_type const *command;
  char                  *disabled_by;
  short                 level;
};

class message_data
{
 public:
  message_data();
  ~message_data();
  bool is_free;
  MESSAGE_DATA *next;
  MESSAGE_DATA *prev;  /* Only used in save_board */
  BOARD_DATA *board;
  time_t datetime;
  char *author;
  char *title;
  char *message;
};

/*
 * color look-up table structure thingy.
 */
struct color_type
{
   char *name; /* eg, gossip, say, look */
   int index;  /* unique index */
};

struct ansi_type
{
   char *name;
   char *value;   /* escape sequence, or whatever */
   int index;
   char letter;
   int stlen;
};

#define color_NORMAL "\033[0m"

struct dl_list
{
   bool is_free;
   DL_LIST *next;
   DL_LIST *prev;
   void *this_one;
};

/*
 * Site ban structure.
 */
class ban_data
{
 public:
  ban_data();
  ~ban_data();
  char *name;
  char *banned_by;
  bool newbie;
};

class brand_data
{
 public:
  brand_data();
  ~brand_data();
  char *branded;
  char *branded_by;
  char *dt_stamp;
  char *message;
  char *priority;
};

class buf_data_struct
{
 public:
  buf_data_struct();
  ~buf_data_struct();
  bool is_free;
  BUF_DATA_STRUCT *next;
  BUF_DATA_STRUCT *prev;
  CHAR_DATA *ch;
  char **dest;
  char *buf;
  RET_FUN *returnfunc;
  MESSAGE_DATA *returnparm;
  int old_char_pos;
};

struct time_info_data
{
   int hour;
   int day;
   int month;
   int year;
   int moon;
};

struct weather_data
{
   int mmhg;
   int change;
   int sky;
   int sunlight;
   int moon_phase;
   int moon_loc;
   bool phase_changed;
};

struct member_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   CHAR_DATA *this_member;
   MEMBER_DATA *next;
   MEMBER_DATA *prev;
};

class mark_data
{
 public:
  mark_data();
  ~mark_data();
  int room_vnum;
  char *message;
  char *author;
  short duration;
  short type;   /* VAMP, WOLF */
};

struct council_data
{
   char *council_name;
   MEMBER_DATA *first_member;
   MEMBER_DATA *last_member;
   bool quorum;
   short council_time;
};

/*
 * Descriptor (channel) structure.
 */
class descriptor_data
{
 public:
  descriptor_data();
  ~descriptor_data();
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   DESCRIPTOR_DATA *next;
   DESCRIPTOR_DATA *prev;
   DESCRIPTOR_DATA *snoop_by;
   CHAR_DATA *character;
   CHAR_DATA *original;
   char *host;
   short descriptor;
   short connected;
   bool fcommand;
   char inbuf[4 * MAX_INPUT_LENGTH];
   char incomm[MAX_INPUT_LENGTH];
   char inlast[MAX_INPUT_LENGTH];
   short repeat;
   char *showstr_head;
   char *showstr_point;
   char *outbuf;
   int outsize;
   int outtop;
   unsigned int remote_port;  /* 'Pair Port' ? -S- */
   int check;  /* For new players */
   int flags;
   int childpid;  /* Child process id */
   time_t timeout;
};

#define DESC_FLAG_PASSTHROUGH 1  /* Used when data is being passed to another program */

struct family_name_type
{
   char *name;
   short clan_enemy;

};

struct politics_data_type
{

   short diplomacy[MAX_CLAN][MAX_CLAN];
   bool daily_negotiate_table[MAX_CLAN][MAX_CLAN];
   long int treasury[MAX_CLAN];
   bool end_current_state[MAX_CLAN][MAX_CLAN];
};

struct stance_app_type
{
   char *name;
   short ac_mod;
   short dr_mod;
   short hr_mod;
   float speed_mod;
};

/*
 * Attribute bonus structures.
 */
struct str_app_type
{
   short tohit;
   short todam;
   short carry;
   short wield;
};

struct int_app_type
{
   short learn;
   short spell_mod;
   short mana_regen;
};

struct wis_app_type
{
   short practice;
   short spell_save;
};

struct dex_app_type
{
   short defensive;
};

struct con_app_type
{
   short hitp;
   short shock;
};

/*
 * Shop types.
 */
class shop_data
{
 public:
  shop_data();
  ~shop_data();
  short buy_type[MAX_TRADE]; /* Item types shop will buy     */
  short close_hour;          /* First closing hour           */
  int keeper;                /* Vnum of shop keeper mob      */
  short open_hour;           /* First opening hour           */
  short profit_buy;          /* Cost multiplier for buying   */
  short profit_sell;         /* Cost multiplier for selling  */
};

/*
 * Per-class stuff.
 */
struct class_type
{
   char who_name[4]; /* Three-letter name for 'who'  */
   char *class_name; /* Full name                    */
   short attr_prime;   /* Prime attribute              */
   char *attr; /* Prime          */
   int guild;  /* Vnum of guild room           */
   short skill_adept;  /* Maximum skill level          */
   short thac0_00;  /* Thac0 for level  0           */
   short thac0_32;  /* Thac0 for level 32           */
   short hp_min; /* Min hp gained on leveling    */
   short hp_max; /* Max hp gained on leveling    */
   bool fMana; /* Class gains mana on level    */
   char *skill1;  /* Auto-learnt skill if any     */
};


struct race_type
{
   char race_name[4];   /* Three letter name for race */
   char *race_title; /* Full race name */
   short recall; /* Race's recall location */
   int race_room; /* vnum of race-only room */
   short race_str;  /* max_str to use for race */
   short race_int;
   short race_wis;
   short race_dex;
   short race_con;
   int race_flags;   /* flags for the various racial stuff    */
   bool wear_locs[MAX_WEAR];  /* on/off for each wear loc */
   int classes;   /* Number of classes for race   */
   short limit[MAX_CLASS];   /* Max for each class */
   char *comment; /* comments shown for new players */
   char *skill1;
   int strong_realms;
   int weak_realms;
   int resist_realms;
   int suscept_realms;
   bool player_allowed;
   bool has_money;
};

struct clan_type
{
   char *clan_name;  /* The name of the Clan */
   char *clan_abbr;  /* Abbrev. name - FIVE CHARS */
   short donat_room;   /* clan donation */
   short clan_room; /* Clan-only room */
   char *leader;  /* Clan leader */
   char *enemies; /* Enemies (if any) */
   int eq[MAX_CLAN_EQ]; /* vnums of objects to load */

};

/*
 * Data structure for notes.
 */
class note_data
{
 public:
  note_data();
  ~note_data();
  char *date;
  time_t date_stamp;
  char *sender;
  char *subject;
  char *text;
  char *to_list;
};

/*
 * An affect.
 */
class affect_data
{
 public:
  affect_data();
  ~affect_data();
  int bitvector;
  CHAR_DATA *caster;
  short duration;
  bool is_free;
  int level;
  short location;
  short modifier;
  AFFECT_DATA *next;
  AFFECT_DATA *prev;
  short type;
};

class room_affect_data
{
 public:
  room_affect_data();
  ~room_affect_data();
  bool is_free;
  ROOM_AFFECT_DATA *next;
  ROOM_AFFECT_DATA *prev;
  short duration;
  short level;
  int type;
  int bitvector;
  int applies_spell;   /* what spell is cast on a ch by the room..a sn */
  short modifier;
  short location;
  CHAR_DATA *caster;
};



/*
 * A kill structure (indexed by level).
 */
struct kill_data
{
   short number;
   short killed;
};


class magic_shield
{
 public:
  magic_shield();
  ~magic_shield();
  bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
  MAGIC_SHIELD *next;
  MAGIC_SHIELD *prev;
  short type;   /* what kind is it? Electric, Fire, etc... */
  bool harmful; /* does the shield damage the attacker? */
  short attack_dam;   /* then hurt the attacker ;) */
  short percent;   /* percent of damage it absorbs per attack */
  short hits;   /* Shield hitpoints */
  int sn;
  char *absorb_message_room;
  char *absorb_message_victim;
  char *absorb_message_self;
  char *name;
  char *wearoff_room;
  char *wearoff_self;
};

/*
 * Prototype for a mob.
 * This is the in-memory version of #MOBILES.
 */
class mob_index_data
{
 public:
  mob_index_data();
  ~mob_index_data();
  bitset<MAX_BITSET> act;
  short ac_mod;
  int affected_by;
  short alignment;
  AREA_DATA *area;
  int cast;
  short clan;
  int def;
  char *description;
  short dr_mod;
  short hr_mod;
  int hunt_flags;
  bool is_free;
  short killed;
  short learned[MAX_SKILL];
  short level;
  char *long_descr;
  MOB_INDEX_DATA *next;
  char *player_name;
  short position;
  SHOP_DATA *pShop;
  short p_class;
  short race;
  int race_mods;
  int resist;
  char *script_name;
  short sex;
  char *short_descr;
  int skills;
  SPEC_FUN *spec_fun;
  float speed[MAX_SPEED];
  int strong_magic;
  int suscept;
  int vnum;
  int weak_magic;
};

/*
 * One character (PC or NPC).
 */
class char_data
{
 public:
  char_data();
  ~char_data();
  bool check_cooldown( const char *skill );
  bool check_cooldown( int pos );
  void gain_exp( int gain );
  short get_level( const char *what = '\0' );
  const char *get_name( CHAR_DATA *looker = NULL );
  const char *get_title( void );
  void send( string txt, ... ); __attribute__ ((format (printf, 2, 3)));
  void set_cooldown( const char *skill );
  void set_cooldown( int pos, float duration );
  void set_title( const char *title );

  bitset<MAX_BITSET> act;
  int affected_by;
  short alignment;
  short armor;
  short carry_number;
  float carry_weight;
  CAST_DATA *casting;
  short clan;
  float cooldown[MAX_COOLDOWN];
  short damroll;
  bitset<MAX_BITSET> deaf;
  DESCRIPTOR_DATA *desc;
  string description;
  int exp;
  CHAR_DATA *fighting;
  AFFECT_DATA *first_affect;
  OBJ_DATA *first_carry;
  AFFECT_DATA *first_saved_aff;
  MAGIC_SHIELD *first_shield;
  int hit;
  short hitroll;
  CHAR_DATA *hunting;  /* For hunting PC's/mobs   */
  int hunt_flags;   /* Action flags         */
  OBJ_DATA *hunt_obj;  /* Looking for objects     */
  ROOM_INDEX_DATA *in_room;
  bool is_free; /* kept for room lists link/unlink */
  bool is_quitting;
  LUA_DATA *lua; /* Lua scripting */
  AFFECT_DATA *last_affect;
  OBJ_DATA *last_carry;
  AFFECT_DATA *last_saved_aff;
  MAGIC_SHIELD *last_shield;
  CHAR_DATA *leader;
  short level;  /* For m/c this = max of levels */
  time_t logon;
  string long_descr;
  string long_descr_orig;
  int lvl[MAX_CLASS];  /* Holds details for m/c levels */
  int lvl2[MAX_CLASS]; /* for remort (if any) */
  int mana;
  CHAR_DATA *master;
  int max_hit;
  int max_mana;
  int max_move;
  MONEY_TYPE *money;
  int move;
  string name;
  CHAR_DATA *next_in_room;
  bool npc; /* For NPC's, no more flag */
  NPC_DATA *npcdata;
  short num_followers;
  CHAR_DATA *old_body; /* = descriptor->original */
  string old_prompt; /* used to hold prompt when writing */
  PC_DATA *pcdata;
  int played;
  int poly_level;
  short position;
  CHAR_DATA *prev_in_room;
  string prompt;
  short p_class;
  short race;
  int race_mods;
  CHAR_DATA *reply;
  CHAR_DATA *rider;
  CHAR_DATA *riding;
  short saving_throw;
  string searching; /* For NPCs hunting continuously */
  short sex;
  OBJ_DATA *sitting;   /* What they're resting or sleeping on */
  float speed[MAX_SPEED];
  int stance;
  short stance_ac_mod;
  short stance_dr_mod;
  short stance_hr_mod;
  short stun_timer;
  bool switched; /* = not isnull(descriptor->original) */
  string target;  /* For Hunting and ACT_REMEMBER */
  short timer;
  short trust;
  bool using_named_door;
  short wait;
  ROOM_INDEX_DATA *was_in_room;
  short wimpy;
  bool wizbit;
};

/*
 * Data which only NPC's have.
 */
class npc_data
{
 public:
  npc_data();
  ~npc_data();
  short ac_mod;
  int cast;
  int def;
  short dr_mod;
  short extract_timer; /* charmie timer */
  short hr_mod;
  CHAR_DATA *hunt_for; /* who hired the merc / corpse return */
  ROOM_INDEX_DATA *hunt_home; /* return to loc */
  NPC_GROUP_DATA *ngroup;
  MOB_INDEX_DATA *pIndexData;
  RESET_DATA *reset;
  int resist;
  char *short_descr;
  int skills;
  SPEC_FUN *spec_fun;
  int strong_magic;
  int suscept;
  int weak_magic;
};

/*
 * Data which only PC's have.
 */
class pc_data
{
 public:
  pc_data();
  ~pc_data();
  short adept_level;
  char *alias[MAX_ALIASES];
  char *alias_name[MAX_ALIASES];
  char *assist_msg;
  char *bamfin;
  char *bamfout;
  MONEY_TYPE *bank_money;
  short build_mode;
  int build_vnum;
  short condition[MAX_COND];
  int color[MAX_COLOR];
  BRAND_DATA *current_brand;
  short death_cnt;
  char dimcol;
  char *email_address;
  short failures;  /* Failed logins */
  char *header;  /* filename for helpedit */
  char hicol;
  char *host[MAX_HOSTS]; /* Used to tell PC last login sites */
  int hp_from_gain; /* same for hitpoints */
  char *ignore_list[MAX_IGNORES];  /* Ignore this person */
#ifdef IMC
  IMC_CHARDATA *imcchardata;
#endif
  short invis; /* wizinvis imm, level invis to */
  char *lastlogin;
  time_t last_note;
  short learned[MAX_SKILL];
  char *load_msg;
  short login_sex;
  int mana_from_gain;  /* saves non-item oriented mana total */
  short max_con;
  short max_dex;
  short max_int;
  short max_str;
  short max_wis;
  char *message; /* helpedit file contents */
  short mod_con;
  short mod_dex;
  short mod_int;
  short mod_str;
  short mod_wis;
  bitset<MAX_BITSET> monitor;   /* Imm channel monitoring */
  int move_from_gain;
  short movement; /* Quick movement */
  short order[MAX_CLASS];   /* Class Order */
  short pagelen;
  char *pedit_state;
  char *pedit_string[MAX_PEDIT];
  short perm_con;
  short perm_dex;
  short perm_int;
  short perm_str;
  short perm_wis;
  NOTE_DATA *pnote;
  short practice;
  char *pwd;
  QUEST_INFO *quest_info;
  short quest_points;
  int recall_vnum;
  RECORD_DATA *records; /* kill/damage/etc records */
  char *room_enter;
  char *room_exit;
  short ruler_rank;
  time_t save_time;
  short sentence;
  SUPER_DATA *super; /* struct for supers: vamp, wolves, hunters */
  short term_columns;
  short term_rows;
  char *title;
  bool valid_email;
  char *whitelist[MAX_HOSTS]; /* Used to restrict login domains */
  char *who_name;   /* To show on who name */
};

struct liq_type
{
   char *liq_name;
   char *liq_color;
   short liq_affect[3];
};

/*
 * Extra description data for a room or object.
 */
class extra_descr_data
{
 public:
  extra_descr_data();
  ~extra_descr_data();
  char *description;   /* What to see                      */
  char *keyword; /* Keyword in look/examine          */
  bool is_free;
  EXTRA_DESCR_DATA *next;
  EXTRA_DESCR_DATA *prev;
};

struct trigger_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   TRIGGER_DATA *next;
   TRIGGER_DATA *prev;
   char *message; /* properly formatted act format string to use in a TO_ROOM */
   int trigger;   /* command used on object  */
   int event;  /* trigger function index to be called  */
   int data;   /* data used in the event call..vnum, spell index, etc. */
   int register_data;   /* for storing generic info  */
   int on_value;  /* for conditional triggers..happens when register is higher than 
                   * on_value  */
   int at_vnum;   /* for at another room triggers. event will happen there */
   bool force_message;  /* Always does an act message, in addition to trigger */
   char *spell_name;
};


class obj_index_data
{
 public:
  obj_index_data();
  ~obj_index_data();
   AREA_DATA *area;
   short durability;
   bitset<MAX_BITSET> extra_flags;
   AFFECT_DATA *first_apply;
   EXTRA_DESCR_DATA *first_exdesc;
   TRIGGER_DATA *first_trigger;
   bool is_free;
   int item_apply;
   int item_type;
   AFFECT_DATA *last_apply;
   EXTRA_DESCR_DATA *last_exdesc;
   TRIGGER_DATA *last_trigger;
   short level;
   char *long_descr;
   short max_durability;
   char *name;
   OBJ_INDEX_DATA *next;
   OBJ_FUN *obj_fun;
   char *owner;
   char *script_name;
   char *short_descr;
   float speed;
   int value[MAX_OBJ_VALUE];
   int vnum;
   bitset<MAX_BITSET> wear_flags;
   short weight;
};

/*
 * One object.
 */
class obj_data
{
 public:
  obj_data();
  ~obj_data();
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   OBJ_DATA *next_in_carry_list; /* carry list is the list on a char, or in a container */
   OBJ_DATA *prev_in_carry_list;
   OBJ_DATA *first_in_carry_list;
   OBJ_DATA *last_in_carry_list;
   OBJ_DATA *next_in_room;
   OBJ_DATA *prev_in_room;
   OBJ_DATA *first_in_room;
   OBJ_DATA *last_in_room;
   OBJ_DATA *first_content;
   OBJ_DATA *last_content;
   OBJ_DATA *next_content;
   OBJ_DATA *prev_content;
   OBJ_DATA *in_obj;
   OBJ_FUN *obj_fun;
   CHAR_DATA *carried_by;
   EXTRA_DESCR_DATA *first_exdesc;
   EXTRA_DESCR_DATA *last_exdesc;
   AFFECT_DATA *first_apply;
   AFFECT_DATA *last_apply;
   OBJ_INDEX_DATA *pIndexData;
   ROOM_INDEX_DATA *in_room;
   char *owner;
   char *name;
   char *script_name;
   char *short_descr;
   char *long_descr;
   int item_type;
   bitset<MAX_BITSET> extra_flags;
   bitset<MAX_BITSET> wear_flags;
   int item_apply;
   int wear_loc;
   short weight;
   int cost;
   short level;
   short timer;
   int value[MAX_OBJ_VALUE];
   short durability;
   short max_durability;
   MONEY_TYPE *money;
   float speed;
   RESET_DATA *reset;
   LUA_DATA *lua;
};

/*
 * Exit data.
 */
class exit_data
{
 public:
  exit_data();
  ~exit_data();
  char *description;
  bitset<MAX_BITSET> exit_info;
  bool is_free;
  short key;
  char *keyword;
  EXIT_DATA *next;
  ROOM_INDEX_DATA *to_room;
  int vnum;
};

/*
 * Reset commands:
 *   '*': comment
 *   'M': read a mobile
 *   'O': read an object
 *   'P': put object in object
 *   'G': give object to mobile
 *   'E': equip object to mobile
 *   'D': set state of door
 *   'R': randomize room exits
 *   'S': stop (end of list)
 */

/*
 * Area-reset definition.
 */
class reset_data
{
 public:
  reset_data();
  ~reset_data();
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   RESET_DATA *next;
   RESET_DATA *prev;
   char command;
   short arg1;
   short arg2;
   short arg3;
   char *notes;
   short count;
};



/*
 * Area definition.
 */
#define	RULER_NONE	BIT_0
#define RULER_SOLO	BIT_1
#define RULER_GROUP	BIT_2
#define RULER_MALE	BIT_3
#define RULER_FEMALE	BIT_4
#define RULER_NEUTRAL	BIT_5

class ruler_data
{
 public:
  ruler_data();
  ~ruler_data();
  char *name;
  short ruler_rank;
  char *affiliation_name;
  short affiliation_index;
  char *keywords;
  short flags;
};


class area_data
{
 public:
  area_data();
  ~area_data();
  short                   age;
  short                   area_num;
  char                    *can_read;
  char                    *can_write;
  char                    *filename;
  bitset<MAX_BITSET> flags;
  BUILD_DATA_LIST         *first_area_mobile;
  BUILD_DATA_LIST         *first_area_object;
  BUILD_DATA_LIST         *first_area_room;
  BUILD_DATA_LIST         *first_area_shop;
  RESET_DATA              *first_reset;
  char                    *keyword;
  BUILD_DATA_LIST         *last_area_mobile;
  BUILD_DATA_LIST         *last_area_object;
  BUILD_DATA_LIST         *last_area_room;
  BUILD_DATA_LIST         *last_area_shop;
  RESET_DATA              *last_reset;
  char                    *level_label;
  short                   max_level;
  int                     max_vnum;
  bool                    modified;
  short                   min_level;
  int                     min_vnum;
  char                    *name;
  char                    *owner;
  list<CHAR_DATA *>  player_list;
  char                    *reset_msg;
  short                   reset_rate;
};



/*
 * Room type.
 */
class room_index_data
{
 public:
  room_index_data();
  ~room_index_data();
   int                     affected_by;
   AREA_DATA               *area;
   char                    *description;
   EXIT_DATA               *exit[MAX_DIR];
   OBJ_DATA                *first_content;
   EXTRA_DESCR_DATA        *first_exdesc;
   CHAR_DATA               *first_person;
   ROOM_AFFECT_DATA        *first_room_affect;
   BUILD_DATA_LIST         *first_room_reset;
   OBJ_DATA                *last_content;
   EXTRA_DESCR_DATA        *last_exdesc;
   CHAR_DATA               *last_person;
   ROOM_AFFECT_DATA        *last_room_affect;
   BUILD_DATA_LIST         *last_room_reset;
   short                   light;
   LUA_DATA                *lua;
   list<MARK_DATA *>  mark_list;
   char                    *name;
   ROOM_INDEX_DATA         *next;
   bitset<MAX_BITSET> room_flags;
   char                    *script_name;
   short                   sector_type;
   MONEY_TYPE              *treasure;
   int                     vnum;
};

/* Big MAG mod */
/* Area building stuff */

class build_data_list  /* Used for storing area file data. */
{
 public:
  build_data_list();
  ~build_data_list();
  bool is_free;
  BUILD_DATA_LIST *next;
  BUILD_DATA_LIST *prev;
  void *data;
};

struct lookup_type
{
   char *text;
   unsigned long int value;
   int cost;   /* if == NO_USE, only creators can set. */
};

class npc_group_data
{
 public:
  npc_group_data();
  ~npc_group_data();
  char *enemies;
  DL_LIST *first_follower;
  bool is_free;
  char *last_fighting;
  DL_LIST *last_follower;
  CHAR_DATA *leader;
  char *needs;
  NPC_GROUP_DATA *next;
  NPC_GROUP_DATA *prev;
  ROOM_INDEX_DATA *seek_room;
  short state;
  char *wants;
};

/*
 * Skills include spells as a particular case.
 */
struct skill_type
{
   short flag1;  /* mort or remort?      */
   short flag2;  /* normal and/or vamp?     */
   short cooldown; /* what cooldown slot does it use? */
   char *name; /* Name of skill                */
   short skill_level[MAX_CLASS];   /* Level needed by class        */
   SPELL_FUN *spell_fun;   /* Spell pointer (for spells)   */
   short target; /* Legal targets                */
   short minimum_position;   /* Position for caster / user   */
   short *pgsn;  /* Pointer to associated gsn    */
   short slot;   /* Slot for #OBJECT loading     */
   short min_mana;  /* Minimum mana used            */
   float beats;  /* Waiting time after use       */
   char *noun_damage;   /* Damage message               */
   char *msg_off; /* Wear off message             */
   char *room_off;   /* Wear off msg TO_ROOM    */
};




/*
 * Structure for a command in the command lookup table.
 */
struct cmd_type
{
   char *const name;
   DO_FUN *do_fun;
   short position;
   short level;
   short log;
   short type;   /*added by Aeria for do_commands */
   short show;   /*added by Aeria for do_commands */
   bool ghost_cmd;
};


/*
 * Structure for a social in the socials table.
 */
struct social_type
{
   char *name;
   char *char_no_arg;
   char *others_no_arg;
   char *char_found;
   char *others_found;
   char *vict_found;
   char *char_auto;
   char *others_auto;
};

/* Kline */
class cast_data
{
 public:
  cast_data();
  ~cast_data();
  float max_time;
  float pushback;
  float time;
  char *arg;
};

class lookup_data
{
 public:
  lookup_data();
  ~lookup_data();
  DESCRIPTOR_DATA *d;
  char *buf;
};

struct chanhistory
{
 time_t time[30][MAX_HISTORY];
 char message[30][MAX_HISTORY][MAX_STRING_LENGTH]; /* 30 channels, each with MAX_HISTORY, length of MSL */
 char aname[30][MAX_HISTORY][128]; /* To track area, for yell */
 int cbit[30][MAX_HISTORY]; /* To track which clan, family, tribe, or race */
};

class record_data
{
 public:
  record_data();
  ~record_data();
  int crusade;
  int mdam_amt;
  short mdam_gsn;
  int pdam_amt;
  short pdam_gsn;
  int pd;
  int pk;
  int md;
  int mk;
  int mquest_c;
  int mquest_f;
  int qp;
  int qp_tot;
};

class super_data
{
 public:
  super_data();
  ~super_data();
  short bloodline;
  int energy; /* rage, blood, or focus */
  int energy_max;
  short exp;
  short generation;
  int level;
  int skills_learned; /* total # skills known */
  int skills_max;     /* total # skills may learn; based on gen, etc */
  int pracs;
};

struct mudinfo
{
 time_t            first_boot;
 unsigned long int mk_by_npc;
 unsigned long int mk_by_pc;
 unsigned long int pk_by_npc;
 unsigned long int pk_by_pc;
 unsigned int      total_pfiles;
};

struct sysdata_type
{
 short build_obj_ac;
 short build_obj_dr;
 short build_obj_hp;
 short build_obj_hr;
 short build_obj_mp;
 short build_obj_mv;
 short build_obj_svs;
 short damcap;
 float expmult;
 short killperlev;
 float max_pushback;
 float mob_ac;
 float mob_dr;
 float mob_hp;
 float mob_hr;
 float mob_mp;
 float mob_mv;
 float mob_svs;
 char *playtesters;
 short pulse;
 bool shownumbers;
 bool w_lock;
};

class quest_info
{
 public:
  quest_info();
  ~quest_info();
  short amount[QUEST_MAX_DATA];
  bool is_questing;
  bool quest_complete;
  bool quest_hint[QUEST_MAX_DATA];
  int quest_item_vnum[QUEST_MAX_DATA];
  int quest_mob_vnum[QUEST_MAX_DATA];
  int quest_reward[QUEST_MAX_REWARD];
  short quest_type;
  short time_left;
  short wait_time;
};

struct h_queue
{
  struct h_queue *next;
  ROOM_INDEX_DATA *room;
  short dir;
};

struct clutch_type
{
   int command_number;
   bool destroy;  /* Destory object once clutched? */
   CLUTCH_FUN *func_name;  /* The name of the function.... */
};

struct trigger_type
{
   char *name;
   bool always_extract;
   TRIG_FUN *func;
};

struct eq_type
{
 char *const name;
 float ac;
 float dr;
 float hp;
 float hr;
 float mp;
 float mv;
 float svs;
};
