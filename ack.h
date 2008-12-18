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
#include <bitset>

#define DEC_ACK_H

#ifndef DEC_CONFIG_H
#include "h/config.h"
#endif

#ifndef DEC_GLOBALS_H
#include "globals.h"
#endif

#ifdef IMC
#ifndef DEC_IMC_H
#include "h/imc.h"
#endif
#endif

#ifndef DEC_LISTS_H
#include "lists.h"
#endif

#ifndef DEC_MQUEST_H
#include "h/mquest.h"
#endif

#ifndef DEC_STRFUNS_H
#include "h/strfuns.h"
#endif

#ifndef DEC_TYPEDEFS_H
#include "h/typedefs.h"
#endif

#ifndef DEC_UTILS_H
#include "h/utils.h"
#endif

struct board_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   BOARD_DATA *next;
   BOARD_DATA *prev;
   int vnum;
   MESSAGE_DATA *first_message;
   MESSAGE_DATA *last_message;
   int min_read_lev;
   int min_write_lev;
   int expiry_time;
   int clan;
};

struct message_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
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
struct ban_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   BAN_DATA *next;
   BAN_DATA *prev;
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

struct corpse_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   CORPSE_DATA *next;
   CORPSE_DATA *prev;
   OBJ_DATA *this_corpse;
};

struct member_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   CHAR_DATA *this_member;
   MEMBER_DATA *next;
   MEMBER_DATA *prev;
};

struct mark_data
{
   bool is_free;
   int room_vnum;
   char *message;
   char *author;
   short duration;
   short type;   /* VAMP, WOLF */
   MARK_DATA *next;
   MARK_DATA *prev;  /* for freelist only */
};

struct mark_list_member
{
   bool is_free;
   MARK_LIST_MEMBER *next;
   MARK_LIST_MEMBER *prev;
   MARK_DATA *mark;
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
struct descriptor_data
{
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

#define DESC_FLAG_PASSTHROUGH 1  /* Used when data is being passed to */
                 /*
                  * Another prog.                     
                  */



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
   bool is_free;
   SHOP_DATA *next;  /* Next shop in list            */
   SHOP_DATA *prev;  /* Prev shop in list    */
   int keeper; /* Vnum of shop keeper mob      */
   short buy_type[MAX_TRADE];   /* Item types shop will buy     */
   short profit_buy;   /* Cost multiplier for buying   */
   short profit_sell;  /* Cost multiplier for selling  */
   short open_hour; /* First opening hour           */
   short close_hour;   /* First closing hour           */
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
   bool is_free;
   NOTE_DATA *next;
   NOTE_DATA *prev;
   char *sender;
   char *date;
   char *to_list;
   char *subject;
   char *text;
   time_t date_stamp;
};

/*
 * An affect.
 */
class affect_data
{
 public:
  affect_data();
  ~affect_data();
   bool is_free;
   AFFECT_DATA *next;
   AFFECT_DATA *prev;
   short type;
   short duration;
   short location;
   short modifier;
   int bitvector;
   CHAR_DATA *caster;
   int level;
};

struct room_affect_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
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


struct magic_shield
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   MAGIC_SHIELD *next;
   MAGIC_SHIELD *prev;
   short type;   /* what kind is it? Electric, Fire, etc... */
   bool harmfull; /* does the shield damage the attacker? */
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
   std::bitset<MAX_BITSET> act;
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
   MPROG_DATA *first_mprog;
   int hunt_flags;
   bool is_free;
   short killed;
   MPROG_DATA *last_mprog;
   short learned[MAX_SKILL];
   short level;
   char *long_descr;
   MOB_INDEX_DATA *next;
   char *player_name;
   short position;
   int power_cast;
   int power_skills;
   int progtypes;
   SHOP_DATA *pShop;
   short p_class;
   short race;
   int race_mods;
   int resist;
   short sex;
   char *short_descr;
   int skills;
   SPEC_FUN *spec_fun;
   float speed[MAX_SPEED];
   int strong_magic;
   int suscept;
   char *target;
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
   bool is_free;
   CHAR_DATA *next;
   CHAR_DATA *prev;
   bool is_quitting;
   CHAR_DATA *next_in_room;
   CHAR_DATA *prev_in_room;
   CHAR_DATA *master;
   CHAR_DATA *leader;
   CHAR_DATA *fighting;
   CHAR_DATA *reply;
   bool npc; /* For NPC's, no more flag */
   CHAR_DATA *hunting;  /* For hunting PC's/mobs   */
   OBJ_DATA *hunt_obj;  /* Looking for objects     */
   CHAR_DATA *hunt_for; /* Employer (crs, mercs)   */
   ROOM_INDEX_DATA *hunt_home;   /* Return to after hunting */
   char *searching;  /* For PC's that quit      */
   int hunt_flags;   /* Action flags         */
   bool switched; /* = not isnull(descriptor->original) */
   CHAR_DATA *old_body; /* = descriptor->original */
   int poly_level;
   SPEC_FUN *spec_fun;
   MOB_INDEX_DATA *pIndexData;
   DESCRIPTOR_DATA *desc;
   AFFECT_DATA *first_affect;
   AFFECT_DATA *last_affect;
   AFFECT_DATA *first_saved_aff;
   AFFECT_DATA *last_saved_aff;
   NOTE_DATA *pnote;
   OBJ_DATA *first_carry;
   OBJ_DATA *last_carry;
   ROOM_INDEX_DATA *in_room;
   ROOM_INDEX_DATA *was_in_room;
   PC_DATA *pcdata;
   char *name;
   OBJ_DATA *sitting;   /* What they're resting or sleeping on */
   char *short_descr;
   char *long_descr;
   char *long_descr_orig;
   char *description;
   char *prompt;
   char *old_prompt; /* used to hold prompt when writing */
   short sex;
   short login_sex;
   short p_class;
   short clan;   /* need to convert from pcdata to this */
   short race;
   short level;  /* For m/c this = max of levels */
   int lvl[MAX_CLASS];  /* Holds details for m/c levels */
   int lvl2[MAX_CLASS]; /* for remort (if any) */
   int adept_level;
   int sentence;  /* For new legal system */
   short invis;  /* For wizinvis imms - lvl invis to */
   short incog;  /* Same as above except for incognito --Flar */
   short trust;
   bool wizbit;
   int played;
   time_t logon;
   time_t save_time;
   time_t last_note;
   short timer;
   short wait;
   int hit;
   int max_hit;
   int mana;
   int max_mana;
   int move;
   int max_move;
   int gold;
   int balance;   /* Amount of gold (if any) in bank */
   int exp;
   int intell_exp;
   std::bitset<MAX_BITSET> act;
   std::bitset<MAX_BITSET> deaf;
   int act_build; /* for setting what ya editing */
   int build_vnum;   /* the current vnum for w-y-e  */
   int affected_by;
   short position;
   short practice;
   float carry_weight;
   short carry_number;
   short saving_throw;
   short alignment;
   short hitroll;
   short damroll;
   short armor;
   short ac_mod; /* ac modifier */
   short stance_ac_mod;
   short hr_mod; /* hitroll modifier */
   short stance_hr_mod;
   short dr_mod; /* damroll modifier */
   short stance_dr_mod;
   char *target;  /* last ch to attack */
   short wimpy;
   MPROG_ACT_LIST *first_mpact;  /* Used by MOBprogram */
   MPROG_ACT_LIST *last_mpact;
   int mpactnum;  /* Used by MOBprogram */
   int skills; /* Used for MOBs */
   int cast;
   int def;
   int strong_magic;
   int weak_magic;
   int resist;
   int suscept;
   int race_mods;
   int power_skills;
   int power_cast;
   CHAR_DATA *riding;
   CHAR_DATA *rider;
   MAGIC_SHIELD *first_shield;
   MAGIC_SHIELD *last_shield;
   int stunTimer;
   short num_followers;
   short extract_timer;   /* same as object timer, only use for charmies */
   BRAND_DATA *current_brand;
   int stance;
   bool using_named_door;
   MONEY_TYPE *money;
   MONEY_TYPE *bank_money;
   NPC_GROUP_DATA *ngroup;
   float speed[MAX_SPEED];
   RESET_DATA *reset;
};



/*
 * Data which only PC's have.
 */

class pc_data
{
 public:
  pc_data();
  ~pc_data();
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   PC_DATA *next;
   PC_DATA *prev;
   int color[MAX_COLOR];
   char *pwd;
   char *bamfin;
   char *room_enter;
   char *room_exit;
   char *bamfout;
   char *title;
   char *host; /* Used to tell PC last login site */
   short failures;  /* Failed logins */
   short clan;   /* will be used to denote clan membership */
   short perm_str;
   short perm_int;
   short perm_wis;
   short perm_dex;
   short perm_con;
   short max_str;
   short max_int;
   short max_wis;
   short max_dex;
   short max_con;
   short mod_str;
   short mod_int;
   short mod_wis;
   short mod_dex;
   short mod_con;
   std::bitset<MAX_BITSET> monitor;   /* Imm channel monitoring */
   short condition[3];
   SUPER_DATA *super; /* struct for supers: vamp, wolves, hunters */
   short pagelen;
   short learned[MAX_SKILL];
   char *header;  /* filename for helpedit */
   char *message; /* helpedit file contents */
   char *alias_name[MAX_ALIASES];
   char *alias[MAX_ALIASES];
   char *who_name;   /* To show on who name */
   RECORD_DATA *records; /* kill/damage/etc records */
   char *lastlogin;
   short order[MAX_CLASS];   /* Class Order */
   short quest_points;
   char *ignore_list[MAX_IGNORES];  /* Ignore this person */
   int recall_vnum;
   int mana_from_gain;  /* saves non-item oriented mana total */
   int hp_from_gain; /* same for hitpoints */
   int move_from_gain;
   char *load_msg;
   char hicol;
   char dimcol;
   short ruler_rank;
   char *pedit_state;
   char *pedit_string[5];
   short term_rows;
   short term_columns;
   char *email_address;
   bool valid_email;
   char *assist_msg;
   QUEST_INFO *quest_info;
   short movement; /* Quick movement */
#ifdef IMC
   IMC_CHARDATA *imcchardata;
#endif
};

/*
 * MOBprogram block
*/

struct mob_prog_act_list
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   MPROG_ACT_LIST *next;
   MPROG_ACT_LIST *prev;
   char *buf;
   CHAR_DATA *ch;
   OBJ_DATA *obj;
   void *vo;
};

struct mob_prog_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   MPROG_DATA *next;
   MPROG_DATA *prev;
   int type;
   char *arglist;
   char *comlist;
   char *filename;
};

extern bool MOBtrigger;





struct liq_type
{
   char *liq_name;
   char *liq_color;
   short liq_affect[3];
};



/*
 * Extra description data for a room or object.
 */
struct extra_descr_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   EXTRA_DESCR_DATA *next; /* Next in list                     */
   EXTRA_DESCR_DATA *prev; /* Prev in list             */
   char *keyword; /* Keyword in look/examine          */
   char *description;   /* What to see                      */
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
   std::bitset<MAX_BITSET> extra_flags;
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
   char *short_descr;
   float speed;
   int value[MAX_OBJ_VALUE];
   int vnum;
   std::bitset<MAX_BITSET> wear_flags;
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
   OBJ_DATA *next;
   OBJ_DATA *prev;
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
   char *short_descr;
   char *long_descr;
   int item_type;
   std::bitset<MAX_BITSET> extra_flags;
   std::bitset<MAX_BITSET> wear_flags;
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
  std::bitset<MAX_BITSET> exit_info;
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

/*
 *   Npc Interaction stuff  Zen
 */

struct queued_interact_list
{
   bool is_free;
   QUEUED_INTERACT_LIST *next;
   QUEUED_INTERACT_LIST *prev;
   CHAR_DATA *mob;
};




struct influence_list
{
   bool is_free;
   INFLUENCE_LIST *next;
   INFLUENCE_LIST *prev;
   INFLUENCE_DATA *this_one;
};


struct control_list
{
   bool is_free;
   CONTROL_LIST *next;
   CONTROL_LIST *prev;
   CONTROL_DATA *this_one;
};

struct ruler_list
{
   bool is_free;
   RULER_LIST *next;
   RULER_LIST *prev;
   RULER_DATA *this_one;
};


struct control_data
{
   bool is_free;
   char *keyword;
   AREA_DATA *area;
   RULER_DATA *ruler;   /* what entity controls the area */
   INFLUENCE_LIST *first_influence; /* what current influences are for the area */
   INFLUENCE_LIST *last_influence;
   INTERACT_DATA *first_interact;   /* tells the mobs what to do */
   INTERACT_DATA *last_interact;
};


struct influence_data
{
   bool is_free;
   RULER_DATA *actor;
   int influence;
};


struct interact_data
{
   bool is_free;
   INTERACT_DATA *next;
   INTERACT_DATA *prev;
   int type;
   int min_value;
   int max_value;
   int action;
   char *say;
   CHAR_DATA *target;
};
#define	RULER_NONE	BIT_0
#define RULER_SOLO	BIT_1
#define RULER_GROUP	BIT_2
#define RULER_MALE	BIT_3
#define RULER_FEMALE	BIT_4
#define RULER_NEUTRAL	BIT_5



struct ruler_data
{
   bool is_free;
   RULER_DATA *next;
   RULER_DATA *prev;
   char *name;
   short ruler_rank;
   char *affiliation_name;
   short affiliation_index;
   char *keywords;
   short flags;
   CONTROL_LIST *first_control;
   CONTROL_LIST *last_control;
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
  CONTROL_DATA            *control;
  char                    *filename;
  std::bitset<MAX_BITSET> flags;
  BUILD_DATA_LIST         *first_area_mobile;
  BUILD_DATA_LIST         *first_area_mobprog;
  BUILD_DATA_LIST         *first_area_object;
  BUILD_DATA_LIST         *first_area_room;
  BUILD_DATA_LIST         *first_area_shop;
  RESET_DATA              *first_reset;
  int                     gold;
  bool                    is_free;
  char                    *keyword;
  BUILD_DATA_LIST         *last_area_mobile;
  BUILD_DATA_LIST         *last_area_mobprog;
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
  AREA_DATA               *next;
  short                   nplayer;
  char                    *owner;
  AREA_DATA               *prev;
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
   MARK_LIST_MEMBER        *first_mark_list;
   CHAR_DATA               *first_person;
   ROOM_AFFECT_DATA        *first_room_affect;
   BUILD_DATA_LIST         *first_room_reset;
   bool                    is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   OBJ_DATA                *last_content;
   EXTRA_DESCR_DATA        *last_exdesc;
   MARK_LIST_MEMBER        *last_mark_list;
   CHAR_DATA               *last_person;
   ROOM_AFFECT_DATA        *last_room_affect;
   BUILD_DATA_LIST         *last_room_reset;
   short                   light;
   char                    *name;
   ROOM_INDEX_DATA         *next;
   std::bitset<MAX_BITSET> room_flags;
   short                   sector_type;
   MONEY_TYPE              *treasure;
   int                     vnum;
};

/* Big MAG mod */
/* Area building stuff */

struct build_data_list  /* Used for storing area file data. */
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   BUILD_DATA_LIST *next;
   BUILD_DATA_LIST *prev;
   void *data;
};

struct mobprog_item  /* For re-creating #MOBPROGS section */
{
   MOB_INDEX_DATA *mob;
   char *filename;
};


struct lookup_type
{
   char *text;
   unsigned long int value;
   int cost;   /* if == NO_USE, only creators can set. */
};

struct npc_group_data
{
   bool is_free;
   NPC_GROUP_DATA *next;
   NPC_GROUP_DATA *prev;
   short state;
   DL_LIST *first_follower;
   DL_LIST *last_follower;
   CHAR_DATA *leader;
   ROOM_INDEX_DATA *seek_room;
   char *enemies;
   char *last_fighting;
   char *wants;
   char *needs;
};

/*
 * Skills include spells as a particular case.
 */
struct skill_type
{
   short flag1;  /* mort or remort?      */
   short flag2;  /* normal and/or vamp?     */
   char *name; /* Name of skill                */
   short skill_level[MAX_CLASS];   /* Level needed by class        */
   SPELL_FUN *spell_fun;   /* Spell pointer (for spells)   */
   short target; /* Legal targets                */
   short minimum_position;   /* Position for caster / user   */
   short *pgsn;  /* Pointer to associated gsn    */
   short slot;   /* Slot for #OBJECT loading     */
   short min_mana;  /* Minimum mana used            */
   short beats;  /* Waiting time after use       */
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
struct chanhistory
{
 time_t time[30][MAX_HISTORY];
 char message[30][MAX_HISTORY][MAX_STRING_LENGTH]; /* 30 channels, each with MAX_HISTORY, length of MSL */
 char aname[30][MAX_HISTORY][128]; /* To track area, for yell */
 int cbit[30][MAX_HISTORY]; /* To track which clan, family, tribe, or race */
};

struct record_data
{
 bool is_free;
 RECORD_DATA *next;
 RECORD_DATA *prev;
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

struct super_data
{
 bool is_free;
 SUPER_DATA *next;
 SUPER_DATA *prev;
 int energy; /* rage, blood, or focus */
 int energy_max;
 int level;
 int skills_learned; /* total # skills known */
 int skills_max;     /* total # skills may learn; based on gen, etc */
 int exp;
 int pracs;
 short bloodline;
 short generation;
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

struct fight_data
{
 bool is_free;
 FIGHT_DATA *next;
 FIGHT_DATA *prev;
 CHAR_DATA *ch;
};

struct quest_info
{
 bool is_free;
 QUEST_INFO *next;
 QUEST_INFO *prev;
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

/*
 * Our function prototypes.
 * One big lump ... this is every function in Merc.
 */
#define CD      CHAR_DATA
#define MID     MOB_INDEX_DATA
#define OD      OBJ_DATA
#define OID     OBJ_INDEX_DATA
#define RID     ROOM_INDEX_DATA
#define SF      SPEC_FUN
#define OF	OBJ_FUN

/* interp.c */
void interpret args( ( CHAR_DATA * ch, char *argument ) );
bool IS_SWITCHED args( ( CHAR_DATA * ch ) );
bool check_social args( ( CHAR_DATA * ch, char *command, char *argument ) );

/* macros.c */
short get_remort_level   args( ( CHAR_DATA * ch ) );
short get_psuedo_level   args( ( CHAR_DATA * ch ) );
bool   ok_to_use          args( ( CHAR_DATA * ch, int value ) );
bool   check_level_use    args( ( CHAR_DATA * ch, int level ) );
int    exp_to_level       args( ( CHAR_DATA * ch, int p_class, int index ) );
int    exp_for_mobile     args( ( int level, CHAR_DATA * mob ) );
int    exp_mob_base       args( ( int level ) );
int    skill_table_lookup args( ( CHAR_DATA * ch, int sn, int return_type ) );
bool   is_remort          args( ( CHAR_DATA * ch ) );
bool   is_adept           args( ( CHAR_DATA * ch ) );
int    exp_to_level_vamp  args( ( int level ) );
int    exp_to_level_adept args( ( CHAR_DATA * ch ) );
void   reset_gain_stats   args( ( CHAR_DATA * ch ) );
int    exp_to_level_wolf  args( ( int level ) );
int    get_item_value     args( ( OBJ_DATA * obj ) );

/* magic.c */
int mana_cost args( ( CHAR_DATA * ch, int sn ) );
int skill_lookup args( ( const char *name ) );
int slot_lookup args( ( int slot ) );
bool saves_spell args( ( int level, CHAR_DATA * victim ) );
void obj_cast_spell args( ( int sn, int level, CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * obj ) );

/* mob_prog.c */
#ifdef DUNNO_STRSTR
char *strstr args( ( const char *s1, const char *s2 ) );
#endif

void mprog_wordlist_check args( ( char *arg, CHAR_DATA * mob, CHAR_DATA * actor, OBJ_DATA * object, void *vo, int type ) );
void mprog_percent_check args( ( CHAR_DATA * mob, CHAR_DATA * actor, OBJ_DATA * object, void *vo, int type ) );
void mprog_act_trigger args( ( char *buf, CHAR_DATA * mob, CHAR_DATA * ch, OBJ_DATA * obj, void *vo ) );
void mprog_bribe_trigger args( ( CHAR_DATA * mob, CHAR_DATA * ch, int amount ) );
void mprog_entry_trigger args( ( CHAR_DATA * mob ) );
void mprog_give_trigger args( ( CHAR_DATA * mob, CHAR_DATA * ch, OBJ_DATA * obj ) );
void mprog_greet_trigger args( ( CHAR_DATA * mob ) );
void mprog_fight_trigger args( ( CHAR_DATA * mob, CHAR_DATA * ch ) );
void mprog_hitprcnt_trigger args( ( CHAR_DATA * mob, CHAR_DATA * ch ) );
void mprog_death_trigger args( ( CHAR_DATA * mob ) );
void mprog_random_trigger args( ( CHAR_DATA * mob ) );
void mprog_speech_trigger args( ( char *txt, CHAR_DATA * mob ) );


           /*-------*\
			  ) quest.c (
			  \*-------*/

void quest_inform args( ( void ) );
void quest_complete args( ( CHAR_DATA * ch ) );
void quest_cancel args( ( void ) );
void generate_auto_quest args( ( void ) );
void crusade_reward args( ( CHAR_DATA *ch ) );


            /*------*\
			   ) save.c (
			   \*------*/
void save_char_obj args( ( CHAR_DATA * ch ) );
bool load_char_obj args( ( DESCRIPTOR_DATA * d, char *name, bool system_call ) );
void save_corpses args( ( void ) );
void fread_corpse args( ( FILE * fp ) );
void save_marks args( ( void ) );
void save_bans args( ( void ) );
char *initial args( ( const char *str ) );


          /*---------*\
			 ) special.c (
			 \*---------*/
SF *spec_lookup args( ( const char *name ) );
char *rev_spec_lookup args( ( SPEC_FUN *func ) );
void print_spec_lookup args( ( char *buf ) );

/* social-edit.c  */

void load_social_table args( ( void ) );


          /*---------*\
			 ) obj_fun.c (
			 \*---------*/
OF *obj_fun_lookup args( ( const char *name ) );
char *rev_obj_fun_lookup args( ( OBJ_FUN *func ) );
void print_obj_fun_lookup args( ( char *buf ) );


         /*---------*\
			) trigger.c (
			\*---------*/

void trigger_handler args( ( CHAR_DATA * ch, OBJ_DATA * obj, int trigger ) );


          /*--------*\
			 ) update.c# (
			 \*--------*/
void advance_level args( ( CHAR_DATA * ch, int p_class, bool show, bool remort ) );
void gain_exp args( ( CHAR_DATA * ch, int gain ) );
void gain_bloodlust args( ( CHAR_DATA * ch, int value ) );
void gain_condition args( ( CHAR_DATA * ch, int iCond, int value ) );
void update_handler args( ( void ) );
bool check_rewield args( ( CHAR_DATA * ch ) );
bool check_re_equip args( ( CHAR_DATA * ch ) );
void auction_update args( ( void ) );
void check_vamp args( ( CHAR_DATA * ch ) );
int move_gain args( ( CHAR_DATA * ch ) );

/* write.c */
void write_start args( ( char **dest, RET_FUN *retfunc, void *retparm, CHAR_DATA * ch ) );
void write_interpret args( ( CHAR_DATA * ch, char *argument ) );

/* hunt.c */
void hunt_victim args( ( CHAR_DATA * ch ) );
void unhunt args( ( CHAR_DATA * ch ) );
int make_hunt args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void hunt_obj args( ( CHAR_DATA * ch ) );
bool make_move args( ( CHAR_DATA * ch, int vnum ) );
char *find_path args( ( int, int, CHAR_DATA *, int, int, int ) );
bool mob_hunt args( ( CHAR_DATA * mob ) );
void char_hunt args( ( CHAR_DATA * ch ) );
bool set_hunt args( ( CHAR_DATA * ch, CHAR_DATA * fch, CHAR_DATA * vch, OBJ_DATA * vobj, int set_flags, int rem_flags ) );
void end_hunt args( ( CHAR_DATA * ch ) );
short h_find_dir args(( ROOM_INDEX_DATA * room, ROOM_INDEX_DATA * target, int h_flags ));
void hunt_move args(( CHAR_DATA * mob, short dir ));

  /*
   * update.c 
   */

void init_alarm_handler args( ( void ) );
void alarm_update args( ( void ) );

/*
 *      Rulers
 *
 */

void save_rulers args( ( void ) );
void load_rulers args( ( void ) );
const char *get_ruler_title args( ( int ruler_rank, int sex ) );

/*
 *  sysdat.c
 */
void load_sysdata args( ( void ) );
void save_sysdata args( ( void ) );


#undef  CD
#undef  MID
#undef  OD
#undef  OID
#undef  RID
#undef  SF
#undef  OF
