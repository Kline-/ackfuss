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

#define DEC_ACK_H 1


#ifndef DEC_TYPEDEFS_H
#include "typedefs.h"
#endif

#ifndef DEC_CONFIG_H
#include "config.h"
#endif

#ifndef DEC_UTILS_H
#include "utils.h"
#endif

#ifndef DEC_GLOBALS_H
#include "globals.h"
#endif


/* Various linked lists head/tail pointer declarations. -- Altrag */
#ifndef DEC_LISTS_H
#include "lists.h"
#endif

#ifndef DEC_STRFUNS_H
#include "strfuns.h"
#endif

#ifndef DEC_ACT_MOB_H
#include "act_mob.h"
#endif

#ifdef IMC
#include "imc.h"
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

struct brand_data
{
   bool is_free;
   BRAND_DATA *next;
   BRAND_DATA *prev;
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
   sh_int duration;
   sh_int type;   /* VAMP, WOLF */
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
   sh_int council_time;
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
   sh_int descriptor;
   sh_int connected;
   bool fcommand;
   char inbuf[4 * MAX_INPUT_LENGTH];
   char incomm[MAX_INPUT_LENGTH];
   char inlast[MAX_INPUT_LENGTH];
   int repeat;
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
   sh_int clan_enemy;

};

struct politics_data_type
{

   sh_int diplomacy[MAX_CLAN][MAX_CLAN];
   bool daily_negotiate_table[MAX_CLAN][MAX_CLAN];
   long int treasury[MAX_CLAN];
   bool end_current_state[MAX_CLAN][MAX_CLAN];
};

struct stance_app_type
{
   char *name;
   sh_int ac_mod;
   sh_int dr_mod;
   sh_int hr_mod;
   float speed_mod;
};

/*
 * Attribute bonus structures.
 */
struct str_app_type
{
   sh_int tohit;
   sh_int todam;
   sh_int carry;
   sh_int wield;
};

struct int_app_type
{
   sh_int learn;
   sh_int spell_mod;
   sh_int mana_regen;
};

struct wis_app_type
{
   sh_int practice;
   sh_int spell_save;
};

struct dex_app_type
{
   sh_int defensive;
};

struct con_app_type
{
   sh_int hitp;
   sh_int shock;
};

/*
 * Shop types.
 */

struct shop_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   SHOP_DATA *next;  /* Next shop in list            */
   SHOP_DATA *prev;  /* Prev shop in list    */
   int keeper; /* Vnum of shop keeper mob      */
   sh_int buy_type[MAX_TRADE];   /* Item types shop will buy     */
   sh_int profit_buy;   /* Cost multiplier for buying   */
   sh_int profit_sell;  /* Cost multiplier for selling  */
   sh_int open_hour; /* First opening hour           */
   sh_int close_hour;   /* First closing hour           */
};






/*
 * Per-class stuff.
 */
struct class_type
{
   char who_name[4]; /* Three-letter name for 'who'  */
   char *class_name; /* Full name                    */
   sh_int attr_prime;   /* Prime attribute              */
   char *attr; /* Prime          */
   sh_int weapon; /* First weapon                 */
   int guild;  /* Vnum of guild room           */
   sh_int skill_adept;  /* Maximum skill level          */
   sh_int thac0_00;  /* Thac0 for level  0           */
   sh_int thac0_32;  /* Thac0 for level 32           */
   sh_int hp_min; /* Min hp gained on leveling    */
   sh_int hp_max; /* Max hp gained on leveling    */
   bool fMana; /* Class gains mana on level    */
   char *skill1;  /* Auto-learnt skill if any     */
};


struct race_type
{
   char race_name[4];   /* Three letter name for race */
   char *race_title; /* Full race name */
   sh_int recall; /* Race's recall location */
   int race_room; /* vnum of race-only room */
   sh_int race_str;  /* max_str to use for race */
   sh_int race_int;
   sh_int race_wis;
   sh_int race_dex;
   sh_int race_con;
   int race_flags;   /* flags for the various racial stuff    */
   bool wear_locs[MAX_WEAR];  /* on/off for each wear loc */
   int classes;   /* Number of classes for race   */
   sh_int limit[MAX_CLASS];   /* Max for each class */
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
   sh_int donat_room;   /* clan donation */
   sh_int clan_room; /* Clan-only room */
   char *leader;  /* Clan leader */
   char *enemies; /* Enemies (if any) */
   int eq[MAX_CLAN_EQ]; /* vnums of objects to load */

};


/*
 * Data structure for notes.
 */
struct note_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
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
struct affect_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   AFFECT_DATA *next;
   AFFECT_DATA *prev;
   sh_int type;
   sh_int duration;
   sh_int location;
   sh_int modifier;
   int bitvector;
   CHAR_DATA *caster;
   int level;
};

struct room_affect_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   ROOM_AFFECT_DATA *next;
   ROOM_AFFECT_DATA *prev;
   sh_int duration;
   sh_int level;
   int type;
   int bitvector;
   int applies_spell;   /* what spell is cast on a ch by the room..a sn */
   sh_int modifier;
   sh_int location;
   CHAR_DATA *caster;

};



/*
 * A kill structure (indexed by level).
 */
struct kill_data
{
   sh_int number;
   sh_int killed;
};


struct magic_shield
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   MAGIC_SHIELD *next;
   MAGIC_SHIELD *prev;
   sh_int type;   /* what kind is it? Electric, Fire, etc... */
   bool harmfull; /* does the shield damage the attacker? */
   sh_int attack_dam;   /* then hurt the attacker ;) */
   sh_int percent;   /* percent of damage it absorbs per attack */
   sh_int hits;   /* Shield hitpoints */
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
struct mob_index_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   MOB_INDEX_DATA *next;
   SPEC_FUN *spec_fun;
   SHOP_DATA *pShop;
   AREA_DATA *area;  /* MAG Mod */
   char *player_name;
   char *short_descr;
   char *long_descr;
   char *description;
   int vnum;
   sh_int count;
   sh_int killed;
   sh_int sex;
   sh_int level;
   int act;
   int affected_by;
   int aggro_list;
   sh_int alignment;
   sh_int ac_mod; /* ac modifier */
   sh_int hr_mod; /* hitroll modifier */
   sh_int dr_mod; /* damroll modifier */
   char *target;  /* last ch to attack */
   MPROG_DATA *first_mprog;   /* Used by MOBprogram   */
   MPROG_DATA *last_mprog;
   int progtypes; /* Used by MOBprogram   */
   int skills; /* skill flags       */
   int power_skills;
   int cast;   /* casting flags  */
   int power_cast;
   int def; /* casting flags  */
   int strong_magic;
   int weak_magic;
   int resist;
   int suscept;
   int race_mods;
   sh_int class;
   sh_int clan;
   sh_int race;
   sh_int position;
   int hunt_flags;
   float speed[4];
   sh_int learned[MAX_SKILL]; /* To replace skill/cast flags */
};



/*
 * One character (PC or NPC).
 */
struct char_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   bool is_quitting;
   CHAR_DATA *next;
   CHAR_DATA *prev;
   CHAR_DATA *next_in_room;
   CHAR_DATA *prev_in_room;
   CHAR_DATA *master;
   CHAR_DATA *leader;
   CHAR_DATA *fighting;
   CHAR_DATA *reply;
/*    CHAR_DATA *         hunting;
    char *              huntdirs;
    int                 huntdirno;
    int                 huntlastvnum;
    char *              huntname;
    int			act_hunt;
    OBJ_DATA	*	hunt_obj;
    int			move_to;
    char *		movename;*/
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
   sh_int sex;
   sh_int login_sex;
   sh_int class;
   sh_int clan;   /* need to convert from pcdata to this */
   sh_int race;
   sh_int level;  /* For m/c this = max of levels */
   int lvl[MAX_CLASS];  /* Holds details for m/c levels */
   int lvl2[MAX_CLASS]; /* for remort (if any) */
   int adept_level;
   int sentence;  /* For new legal system */
   sh_int invis;  /* For wizinvis imms - lvl invis to */
   sh_int incog;  /* Same as above except for incognito --Flar */
   sh_int trust;
   bool wizbit;
   int played;
   time_t logon;
   time_t save_time;
   time_t last_note;
   sh_int timer;
   sh_int wait;
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
   int act;
   int config;
   int act_build; /* for setting what ya editing */
   int build_vnum;   /* the current vnum for w-y-e  */
   int affected_by;
   sh_int position;
   sh_int practice;
   float carry_weight;
   sh_int carry_number;
   sh_int saving_throw;
   sh_int alignment;
   sh_int hitroll;
   sh_int damroll;
   sh_int armor;
   sh_int ac_mod; /* ac modifier */
   sh_int stance_ac_mod;
   sh_int hr_mod; /* hitroll modifier */
   sh_int stance_hr_mod;
   sh_int dr_mod; /* damroll modifier */
   sh_int stance_dr_mod;
   char *target;  /* last ch to attack */
   sh_int wimpy;
   int deaf;
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
   sh_int num_followers;
   sh_int extract_timer;   /* same as object timer, only use for charmies */
   BRAND_DATA *current_brand;
   int stance;
   bool using_named_door;
   MONEY_TYPE *money;
   MONEY_TYPE *bank_money;
   NPC_GROUP_DATA *ngroup;
   float speed[4];
};



/*
 * Data which only PC's have.
 */

struct pc_data
{
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
   sh_int failures;  /* Failed logins */
   sh_int clan;   /* will be used to denote clan membership */
   sh_int perm_str;
   sh_int perm_int;
   sh_int perm_wis;
   sh_int perm_dex;
   sh_int perm_con;
   sh_int max_str;
   sh_int max_int;
   sh_int max_wis;
   sh_int max_dex;
   sh_int max_con;
   sh_int mod_str;
   sh_int mod_int;
   sh_int mod_wis;
   sh_int mod_dex;
   sh_int mod_con;
   sh_int condition[3];
   SUPER_DATA *super; /* struct for supers: vamp, wolves, hunters */
   sh_int pagelen;
   sh_int learned[MAX_SKILL];
   char *header;  /* filename for helpedit */
   char *message; /* helpedit file contents */
   char *alias_name[MAX_ALIASES];
   char *alias[MAX_ALIASES];
   char *who_name;   /* To show on who name */
   RECORD_DATA *records; /* kill/damage/etc records */
   int pflags;
   char *lastlogin;
   sh_int order[MAX_CLASS];   /* Class Order */
   int monitor;   /* monitor channel for imms */
   sh_int quest_points;
   char *ignore_list[MAX_IGNORES];  /* Ignore this person */
   int recall_vnum;
   int mana_from_gain;  /* saves non-item oriented mana total */
   int hp_from_gain; /* same for hitpoints */
   int move_from_gain;
   char *load_msg;
   char hicol;
   char dimcol;
   sh_int ruler_rank;
   char *pedit_state;
   char *pedit_string[5];
   sh_int term_rows;
   sh_int term_columns;
   char *email_address;
   bool valid_email;
   char *assist_msg;
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
   sh_int liq_affect[3];
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


struct obj_index_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   OBJ_INDEX_DATA *next;
   EXTRA_DESCR_DATA *first_exdesc;
   EXTRA_DESCR_DATA *last_exdesc;
   OBJ_FUN *obj_fun;
   AFFECT_DATA *first_apply;
   AFFECT_DATA *last_apply;
   /*
    * MAG Mod 
    */
   AREA_DATA *area;
   char *owner;
   char *name;
   sh_int level;
   char *short_descr;
   char *description;
   int vnum;
   int item_type;
   int extra_flags;
   int wear_flags;
   /*
    * class_flags changed to item_apply for magic apply, etc 
    */
   int item_apply;
   sh_int count;
   sh_int weight;
   int cost;   /* Unused */
   int value[10];
   TRIGGER_DATA *first_trigger;
   TRIGGER_DATA *last_trigger;
   float speed;
};



/*
 * One object.
 */
struct obj_data
{
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
   char *description;
   int item_type;
   int extra_flags;
   int wear_flags;
   int item_apply;
   int wear_loc;
   sh_int weight;
   int cost;
   sh_int level;
   sh_int timer;
   int value[10];
   sh_int condition; /* % value for condition */
   MONEY_TYPE *money;
   float speed;
};



/*
 * Exit data.
 */
struct exit_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   EXIT_DATA *next;
   ROOM_INDEX_DATA *to_room;
   int vnum;
   sh_int exit_info;
   sh_int key;
   char *keyword;
   char *description;
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
struct reset_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   RESET_DATA *next;
   RESET_DATA *prev;
   char command;
   sh_int ifflag;
   sh_int arg1;
   sh_int arg2;
   sh_int arg3;
   char *notes;
   char *auto_message;  /* Ugly - wasteful of space. */
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
   sh_int ruler_rank;
   char *affiliation_name;
   sh_int affiliation_index;
   char *keywords;
   sh_int flags;
   CONTROL_LIST *first_control;
   CONTROL_LIST *last_control;
};


struct area_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   AREA_DATA *next;
   AREA_DATA *prev;
   RESET_DATA *first_reset;
   RESET_DATA *last_reset;
   char *name;
   sh_int age;
   sh_int nplayer;
/* Saving to envy mod, to renumber vnums! */
   int offset;

/* MAG mod */
   int modified;
   int min_vnum;
   int max_vnum;
   int area_num;
   char *owner;
   char *can_read;
   char *can_write;
   int gold;
   char *filename;
   int flags;
   int aggro_list;
   BUILD_DATA_LIST *first_area_room;
   BUILD_DATA_LIST *last_area_room;
   BUILD_DATA_LIST *first_area_object;
   BUILD_DATA_LIST *last_area_object;
   BUILD_DATA_LIST *first_area_mobile;
   BUILD_DATA_LIST *last_area_mobile;
   BUILD_DATA_LIST *first_area_mobprog;
   BUILD_DATA_LIST *last_area_mobprog;
   BUILD_DATA_LIST *first_area_shop;
   BUILD_DATA_LIST *last_area_shop;
   BUILD_DATA_LIST *first_area_specfunc;
   BUILD_DATA_LIST *last_area_specfunc;
   BUILD_DATA_LIST *first_area_objfunc;
   BUILD_DATA_LIST *last_area_objfunc;
   /*
    * BUILD_DATA_LIST *   spec_funs; in mob index data. 
    */
   /*
    * BUILD_DATA_LIST *   area_shops; in mob index data. 
    */
   /*
    * BUILD_DATA_LIST *   resets; already there! 
    */
   CONTROL_DATA *control;
   char *keyword;
   sh_int min_level;
   sh_int max_level;
   char *level_label;
   sh_int reset_rate;
   char *reset_msg;
};



/*
 * Room type.
 */
struct room_index_data
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   ROOM_INDEX_DATA *next;
   CHAR_DATA *first_person;
   CHAR_DATA *last_person;
   OBJ_DATA *first_content;
   OBJ_DATA *last_content;
   EXTRA_DESCR_DATA *first_exdesc;
   EXTRA_DESCR_DATA *last_exdesc;
   AREA_DATA *area;
   EXIT_DATA *exit[6];
   char *name;
   char *description;
   char *auto_message;  /* If != NULL, send_to_room each tick */
   sh_int block_timer;
   int vnum;
   int room_flags;
   sh_int light;
   sh_int sector_type;
   BUILD_DATA_LIST *first_room_reset;
   BUILD_DATA_LIST *last_room_reset;
/* -S- mod... don't save this with OLC :P */
   ROOM_AFFECT_DATA *first_room_affect;
   ROOM_AFFECT_DATA *last_room_affect;
   int affected_by;
   MARK_LIST_MEMBER *first_mark_list;
   MARK_LIST_MEMBER *last_mark_list;
   MONEY_TYPE *treasure;

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




/*
 * Skills include spells as a particular case.
 */
struct skill_type
{
   sh_int flag1;  /* mort or remort?      */
   sh_int flag2;  /* normal and/or vamp?     */
   char *name; /* Name of skill                */
   sh_int skill_level[MAX_CLASS];   /* Level needed by class        */
   SPELL_FUN *spell_fun;   /* Spell pointer (for spells)   */
   sh_int target; /* Legal targets                */
   sh_int minimum_position;   /* Position for caster / user   */
   sh_int *pgsn;  /* Pointer to associated gsn    */
   sh_int slot;   /* Slot for #OBJECT loading     */
   sh_int min_mana;  /* Minimum mana used            */
   sh_int beats;  /* Waiting time after use       */
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
   sh_int position;
   sh_int level;
   sh_int log;
   sh_int type;   /*added by Aeria for do_commands */
   sh_int show;   /*added by Aeria for do_commands */

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
struct bmlist // Serialized bitmasks
{
 BM_LIST *next;
 long set;
 long tar_mask;
};

struct bitmask
{
 long bits; // number of bits set in total.
 long masks; // number of masks in all.
 BM_LIST *int_list;
};

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
 sh_int mdam_gsn;
 int pdam_amt;
 sh_int pdam_gsn;
 int pd;
 int pk;
 int md;
 int mk;
 int mquest;
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
 sh_int bloodline;
 sh_int generation;
};

struct sysdata_type
{
 int damcap;
 float expmult;
 sh_int killperlev;
 float mob_ac;
 float mob_dr;
 float mob_hp;
 float mob_hr;
 float mob_mp;
 float mob_mv;
 char *playtesters;
 sh_int pulse;
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

/* act_comm.c */
bool is_note_to args( ( CHAR_DATA * ch, NOTE_DATA * pnote ) );
void add_follower args( ( CHAR_DATA * ch, CHAR_DATA * master ) );
void stop_follower args( ( CHAR_DATA * ch ) );
void die_follower args( ( CHAR_DATA * ch ) );
bool is_same_group args( ( CHAR_DATA * ach, CHAR_DATA * bch ) );
bool is_group_leader args( ( CHAR_DATA * ch ) );
char *slur_text args( ( char *argument ) );
void send_to_room args( ( char *message, ROOM_INDEX_DATA * room ) );
void list_who_to_output args( ( void ) );

/* act_info.c */
void set_title            args( ( CHAR_DATA * ch, char *title ) );
char *color_string        args( ( CHAR_DATA * CH, char *argument ) );
char *get_family_name     args( ( CHAR_DATA * ch ) );
char *get_tribe_name      args( ( CHAR_DATA * ch ) );
char *output_race_wear    args( ( int iRace ) );

/* act_mob.c */
void int_combat_handler args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void int_handler args( ( CHAR_DATA * ch ) );


/* act_move.c */
void move_char args( ( CHAR_DATA * ch, int door ) );

/* act_obj.c */

void get_obj args( ( CHAR_DATA * ch, OBJ_DATA * obj, OBJ_DATA * container ) );
bool can_wear_at( CHAR_DATA * ch, OBJ_DATA * obj, int location );



/* act_wiz.c */
ROOM_INDEX_DATA *find_location args( ( CHAR_DATA * ch, char *arg ) );

/* bitmask.c */
bool    remove_bit         args( ( BITMASK *mask, sh_int bit ) );
bool    set_bit            args( ( BITMASK *mask, sh_int bit ) );
bool    is_set             args( ( BITMASK *mask, sh_int bit ) );
int     *serialize_bitmask args( ( BITMASK *mask ) );
bool    free_bitmask       args( ( BITMASK *pBmask ) );
BITMASK init_bitmask       args( ( BITMASK *bm ) );
void    load_bitmask       args( ( BITMASK *pBmask, FILE *fp ) );
char    *save_bitmask      args( ( BITMASK *pBmask ) );

/* board.c */
BOARD_DATA *load_board( OBJ_INDEX_DATA * pObj );
void do_show_contents args( ( CHAR_DATA * ch, OBJ_DATA * obj ) );
void do_show_message args( ( CHAR_DATA * ch, int mess_num, OBJ_DATA * obj ) );
void do_edit_message args( ( CHAR_DATA * ch, int mess_num, OBJ_DATA * obj ) );
void do_add_to_message args( ( CHAR_DATA * ch, char *argument ) );
void do_start_a_message args( ( CHAR_DATA * ch, char *argument ) );
void save_message_data args( ( void ) );
void load_messages args( ( void ) );

/* comm.c */
void close_socket args( ( DESCRIPTOR_DATA * dclose ) );
void show_menu_to args( ( DESCRIPTOR_DATA * d ) ); /* Main */
void show_amenu_to args( ( DESCRIPTOR_DATA * d ) );   /* Attributes */
void show_stotal_to args( ( DESCRIPTOR_DATA * d ) );  /* Attribute Totals */
void show_rmenu_to args( ( DESCRIPTOR_DATA * d ) );   /* Race */
void show_smenu_to args( ( DESCRIPTOR_DATA * d ) );   /* Sex */
void show_cmenu_to args( ( DESCRIPTOR_DATA * d ) );   /* Class */
void write_to_buffer args( ( DESCRIPTOR_DATA * d, const char *txt, int length ) );
void send_to_char args( ( const char *txt, CHAR_DATA * ch ) );
void show_string args( ( DESCRIPTOR_DATA * d, char *input ) );
void act args( ( const char *format, CHAR_DATA * ch, const void *arg1, const void *arg2, int type ) );
void hang args( ( const char *str ) );

 /*
  * db.c 
  */
void  perm_update      args( ( void ) );
void  boot_db          args( ( void ) );
void  area_update      args( ( void ) );
void  message_update   args( ( void ) );
CD    *create_mobile   args( ( MOB_INDEX_DATA * pMobIndex ) );
OD    *create_object   args( ( OBJ_INDEX_DATA * pObjIndex, int level ) );
void  clear_char       args( ( CHAR_DATA * ch ) );
void  free_char        args( ( CHAR_DATA * ch ) );
char  *get_extra_descr args( ( const char *name, EXTRA_DESCR_DATA * ed ) );
MID   *get_mob_index   args( ( int vnum ) );
OID   *get_obj_index   args( ( int vnum ) );
RID   *get_room_index  args( ( int vnum ) );
char  fread_letter     args( ( FILE * fp ) );
float fread_float      args( ( FILE * fp ) );
int   fread_number     args( ( FILE * fp ) );
char  *fread_string    args( ( FILE * fp ) );
void  fread_to_eol     args( ( FILE * fp ) );
char  *fsave_to_eol    args( ( FILE * fp ) );
char  *fread_word      args( ( FILE * fp ) );
bool  char_exists      args( ( char *argument ) );
float number_speed     args( ( void ) );
int   bv_log           args( ( int n ) );
void  check_chistory   args( ( CHAR_DATA *ch, int channel ) );
void  update_chistory  args( ( CHAR_DATA *ch, char *argument, int channel ) );
int   count_helps      args( ( void ) );
char  *search_helps    args( ( const char *string ) );
char  *_popen          args( ( const char *string ) );
FILE  *file_open       args( ( const char *file, const char *opt ) );
void  file_close       args( ( FILE *file ) );

/* void *  alloc_mem       args( ( int sMem ) );
void    check_freed     args( ( unsigned int first, unsigned int last) );
void    check_free_mem  args( ( void ) );
void *  alloc_perm      args( ( int sMem ) );
void    free_mem        args( ( void *pMem, int sMem ) );*/

/* spec: renamed getmem -> _getmem, nuked unused alloc_perm */
/* void *  alloc_perm      args( ( int sMem ) ); */
void *_getmem args( ( int size, const char *caller, int log ) );
void dispose args( ( void *mem, int size ) );
char *str_dup args( ( const char *str ) );
void free_string args( ( char *pstr ) );
int number_fuzzy args( ( int number ) );
int number_range args( ( int from, int to ) );
int number_percent args( ( void ) );
int number_door args( ( void ) );
int number_bits args( ( int width ) );
int number_mm args( ( void ) );
int dice args( ( int number, int size ) );
int interpolate args( ( int level, int value_00, int value_32 ) );
void append_file args( ( CHAR_DATA * ch, char *file, char *str ) );
void bug args( ( const char *str, int param ) );
void log_string args( ( const char *str ) );
void tail_chain args( ( void ) );
void send_to_descrips args( ( const char *message ) );
void bug_string args( ( const char *str, const char *str2 ) );
/* Added stuff -Flar */
void bugf( char *fmt, ... ) __attribute__ ( ( format( printf, 1, 2 ) ) );
void log_f( char *fmt, ... ) __attribute__ ( ( format( printf, 1, 2 ) ) );

/* fight.c */
void  violence_update args( ( void ) );
void  damage          args( ( CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dt ) );
void  update_pos      args( ( CHAR_DATA * victim ) );
void  stop_fighting   args( ( CHAR_DATA * ch, bool fBoth ) );
void  death_cry       args( ( CHAR_DATA * ch ) );
void  raw_kill        args( ( CHAR_DATA * victim, char *argument ) );
void  check_killer    args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
float get_speed       args( ( CHAR_DATA *ch, int slot ) );
void  combat_update   args( ( void ) );
void  one_hit         args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );

/* handler.c */
bool remove_obj       args( ( CHAR_DATA * ch, int iWear, bool fReplace ) );
int  get_trust        args( ( CHAR_DATA * ch ) );
void my_get_age       args( ( CHAR_DATA * ch, char *buf ) );
int  my_get_hours     args( ( CHAR_DATA * ch ) );
int  get_age          args( ( CHAR_DATA * ch ) );
int  get_curr_str     args( ( CHAR_DATA * ch ) );
int  get_curr_int     args( ( CHAR_DATA * ch ) );
int  get_curr_wis     args( ( CHAR_DATA * ch ) );
int  get_curr_dex     args( ( CHAR_DATA * ch ) );
int  get_curr_con     args( ( CHAR_DATA * ch ) );
int  can_carry_n      args( ( CHAR_DATA * ch ) );
int  can_carry_w      args( ( CHAR_DATA * ch ) );
void affect_to_room   args( ( ROOM_INDEX_DATA * room, ROOM_AFFECT_DATA * raf ) );
void r_affect_remove  args( ( ROOM_INDEX_DATA * room, ROOM_AFFECT_DATA * raf ) );
void affect_to_char   args( ( CHAR_DATA * ch, AFFECT_DATA * paf ) );
void affect_remove    args( ( CHAR_DATA * ch, AFFECT_DATA * paf ) );
void affect_strip     args( ( CHAR_DATA * ch, int sn ) );
bool is_affected      args( ( CHAR_DATA * ch, int sn ) );
void affect_join      args( ( CHAR_DATA * ch, AFFECT_DATA * paf ) );
void char_from_room   args( ( CHAR_DATA * ch ) );
void char_to_room     args( ( CHAR_DATA * ch, ROOM_INDEX_DATA * pRoomIndex ) );
void obj_to_char      args( ( OBJ_DATA * obj, CHAR_DATA * ch ) );
void obj_from_char    args( ( OBJ_DATA * obj ) );
int  apply_ac         args( ( OBJ_DATA * obj, int iWear ) );
OD   *get_eq_char     args( ( CHAR_DATA * ch, int iWear ) );
void equip_char       args( ( CHAR_DATA * ch, OBJ_DATA * obj, int iWear ) );
void unequip_char     args( ( CHAR_DATA * ch, OBJ_DATA * obj ) );
int  count_obj_list   args( ( OBJ_INDEX_DATA * obj, OBJ_DATA * list ) );
int  count_obj_room   args( ( OBJ_INDEX_DATA * obj, OBJ_DATA * list ) );
void obj_from_room    args( ( OBJ_DATA * obj ) );
void obj_to_room      args( ( OBJ_DATA * obj, ROOM_INDEX_DATA * pRoomIndex ) );
void obj_to_obj       args( ( OBJ_DATA * obj, OBJ_DATA * obj_to ) );
void obj_from_obj     args( ( OBJ_DATA * obj ) );
void extract_obj      args( ( OBJ_DATA * obj ) );
void extract_char     args( ( CHAR_DATA * ch, bool fPull ) );
CD   *get_char_room   args( ( CHAR_DATA * ch, char *argument ) );
CD   *get_char_world  args( ( CHAR_DATA * ch, char *argument ) );
CD   *get_char_area   args( ( CHAR_DATA * ch, char *argument ) );
OD   *get_obj_type    args( ( OBJ_INDEX_DATA * pObjIndexData ) );
OD   *get_obj_list    args( ( CHAR_DATA * ch, char *argument, OBJ_DATA * list ) );
OD   *get_obj_room    args( ( CHAR_DATA * ch, char *argument, OBJ_DATA * list ) );
OD   *get_obj_carry   args( ( CHAR_DATA * ch, char *argument ) );
OD   *get_obj_wear    args( ( CHAR_DATA * ch, char *argument ) );
OD   *get_obj_here    args( ( CHAR_DATA * ch, char *argument ) );
OD   *get_obj_world   args( ( CHAR_DATA * ch, char *argument ) );
OD   *create_money    args( ( int amount ) );
int  get_obj_number   args( ( OBJ_DATA * obj ) );
int  get_obj_weight   args( ( OBJ_DATA * obj ) );
bool room_is_dark     args( ( ROOM_INDEX_DATA * pRoomIndex ) );
bool room_is_private  args( ( ROOM_INDEX_DATA * pRoomIndex ) );
bool can_see          args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool can_see_obj      args( ( CHAR_DATA * ch, OBJ_DATA * obj ) );
bool can_drop_obj     args( ( CHAR_DATA * ch, OBJ_DATA * obj ) );
bool can_sac_obj      args( ( CHAR_DATA * ch, OBJ_DATA * obj ) );
bool can_use          args( ( CHAR_DATA * ch, OBJ_DATA * obj ) );
char *who_can_use     args( ( OBJ_DATA * obj ) );
void notify           args( ( char *message, int lv ) );
void info             args( ( char *message, int lv ) );
void auction          args( ( char *message ) );
void log_chan         args( ( const char *message, int lv ) );
bool item_has_apply   args( ( CHAR_DATA * ch, int bit ) );
CD   *switch_char     args( ( CHAR_DATA * victim, int mvnum, int poly_level ) );
CD   *unswitch_char   args( ( CHAR_DATA * ch ) );
int  best_class       args( ( CHAR_DATA * ch, int sn ) );
int  best_level       args( ( CHAR_DATA * ch, int sn ) );
void monitor_chan     args( ( const char *message, int channel ) );
void set_stun         args( ( CHAR_DATA * victim, int stunTime ) );
CD   *get_char        args( ( CHAR_DATA * ch ) );
void mark_from_room   args( ( int this_room_vnum, MARK_DATA * mark ) );
void mark_to_room     args( ( int this_room_vnum, MARK_DATA * mark ) );
void char_reference   args( ( struct char_ref_type * ref ) );
void char_unreference args( ( CHAR_DATA ** var ) );
void obj_reference    args( ( struct obj_ref_type * ref ) );
void obj_unreference  args( ( OBJ_DATA ** var ) );

/* interp.c */
void interpret args( ( CHAR_DATA * ch, char *argument ) );
bool is_number args( ( char *arg ) );
bool IS_SWITCHED args( ( CHAR_DATA * ch ) );
bool is_shielded args( ( CHAR_DATA * ch, sh_int shield_type ) );
void remove_shield args( ( CHAR_DATA * ch, MAGIC_SHIELD * shield ) );
bool check_social args( ( CHAR_DATA * ch, char *command, char *argument ) );

/* macros.c */
sh_int get_remort_level   args( ( CHAR_DATA * ch ) );
sh_int get_psuedo_level   args( ( CHAR_DATA * ch ) );
bool   ok_to_use          args( ( CHAR_DATA * ch, int value ) );
bool   check_level_use    args( ( CHAR_DATA * ch, int level ) );
int    exp_to_level       args( ( CHAR_DATA * ch, int class, int index ) );
int    exp_for_mobile     args( ( int level, CHAR_DATA * mob ) );
int    exp_mob_base       args( ( int level ) );
int    skill_table_lookup args( ( CHAR_DATA * ch, int sn, int return_type ) );
bool   is_remort          args( ( CHAR_DATA * ch ) );
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
bool spell_identify( int sn, int level, CHAR_DATA * ch, void *vo, OBJ_DATA * obj );

/* mob_commands.c */
char *mprog_type_to_name args( ( int type ) );

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
char *rev_spec_lookup args( ( void *func ) );
void print_spec_lookup args( ( char *buf ) );

/* social-edit.c  */

void load_social_table args( ( void ) );


          /*---------*\
			 ) obj_fun.c (
			 \*---------*/
OF *obj_fun_lookup args( ( const char *name ) );
char *rev_obj_fun_lookup args( ( void *func ) );
void print_obj_fun_lookup args( ( char *buf ) );


         /*---------*\
			) trigger.c (
			\*---------*/

void trigger_handler args( ( CHAR_DATA * ch, OBJ_DATA * obj, int trigger ) );


          /*--------*\
			 ) update.c# (
			 \*--------*/
void advance_level args( ( CHAR_DATA * ch, int class, bool show, bool remort ) );
void gain_exp args( ( CHAR_DATA * ch, int gain ) );
void gain_bloodlust args( ( CHAR_DATA * ch, int value ) );
void gain_condition args( ( CHAR_DATA * ch, int iCond, int value ) );
void update_handler args( ( void ) );
bool check_rewield args( ( CHAR_DATA * ch ) );
bool check_re_equip args( ( CHAR_DATA * ch ) );
void auction_update args( ( void ) );
void check_vamp args( ( CHAR_DATA * ch ) );

/* write.c */
void write_start args( ( char **dest, void *retfunc, void *retparm, CHAR_DATA * ch ) );
void write_interpret args( ( CHAR_DATA * ch, char *argument ) );

/* build.c */
extern const char *cDirs;
void build_strdup      args( ( char **dest, char *src, bool freesrc, bool newline, CHAR_DATA * ch ) );
char *build_simpstrdup args( ( char *buf ) );   /* A plug in alternative to str_dup */
void build_save        args( ( void ) );
int  get_dir           args( ( char ) );
char *show_values      args( ( const struct lookup_type *table, int value, bool fBit ) );
void check_autodig     args( ( CHAR_DATA *ch, int dir ) );

/* buildtab.c  */
/*
int table_lookup	args( (const struct lookup_type * table,char * name) );
char * rev_table_lookup	args( (const struct lookup_type * table,int number) );
char * bit_table_lookup	args( (const struct lookup_type * table,int number) );
*/

/* buildare.c */
/* Area manipulation funcs in buildare.c */
int build_canread( AREA_DATA * Area, CHAR_DATA * ch, int showerror );
int build_canwrite( AREA_DATA * Area, CHAR_DATA * ch, int showerror );
#define AREA_NOERROR   0
#define AREA_SHOWERROR 1

/* areasave.c */
void area_modified( AREA_DATA * );
void build_save_flush( void );

/* hunt.c */
void hunt_victim args( ( CHAR_DATA * ch ) );
void unhunt args( ( CHAR_DATA * ch ) );
int make_hunt args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
/* void	hunt_move	args( ( CHAR_DATA *ch ) ); */
void hunt_obj args( ( CHAR_DATA * ch ) );
bool make_move args( ( CHAR_DATA * ch, int vnum ) );
char *find_path args( ( int, int, CHAR_DATA *, int, int, int ) );

bool mob_hunt args( ( CHAR_DATA * mob ) );
void char_hunt args( ( CHAR_DATA * ch ) );
bool set_hunt args( ( CHAR_DATA * ch, CHAR_DATA * fch, CHAR_DATA * vch, OBJ_DATA * vobj, int set_flags, int rem_flags ) );
void end_hunt args( ( CHAR_DATA * ch ) );
sh_int h_find_dir args( ( ROOM_INDEX_DATA * room, ROOM_INDEX_DATA * target, int h_flags ) );

  /*
   * update.c 
   */

void init_alarm_handler args( ( void ) );
void alarm_update args( ( void ) );
/*    SSM   */
void temp_fread_string args( ( FILE * fp, char *buf ) );


/*
 *      Rulers
 *
 */

void save_rulers args( ( void ) );
void load_rulers args( ( void ) );
char *get_ruler_title args( ( int ruler_rank, int sex ) );

/*
 *	spendqp
 */
void save_brands args( ( void ) );


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
