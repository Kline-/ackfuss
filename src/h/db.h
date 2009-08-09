/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_DB_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* db.c */
void boot_db P_((void));
void load_areas P_((void));
void load_area P_((FILE *fp));
void load_door P_((FILE *fp));
void load_corpses P_((void));
void load_marks P_((void));
void load_bans P_((void));
void load_mobile P_((FILE *fp));
void load_oaffect P_((FILE *fp));
void load_object P_((FILE *fp));
void load_oextra P_((FILE *fp));
void load_resets P_((FILE *fp));
void load_rextra P_((FILE *fp));
void load_room P_((FILE *fp));
void load_shop P_((FILE *fp));
void load_notes P_((void));
void fix_exits P_((void));
void load_disabled P_((void));
void save_disabled P_((void));
void check_resets P_((void));
void area_update P_((void));
void reset_area P_((AREA_DATA *pArea));
CHAR_DATA *create_mobile P_((MOB_INDEX_DATA *pMobIndex));
OBJ_DATA *create_object P_((OBJ_INDEX_DATA *pObjIndex, int level));
char *get_extra_descr P_((const char *name, EXTRA_DESCR_DATA *ed));
MOB_INDEX_DATA *get_mob_index P_((int vnum));
OBJ_INDEX_DATA *get_obj_index P_((int vnum));
ROOM_INDEX_DATA *get_room_index P_((int vnum));
char fread_letter P_((FILE *fp));
float fread_float P_((FILE *fp));
int fread_number P_((FILE *fp));
void fread_to_eol P_((FILE *fp));
char *fsave_to_eol P_((FILE *fp));
char *fread_word P_((FILE *fp));
void *_getmem P_((int size, const char *caller, int log));
DECLARE_DO_FUN(do_areas);
void perm_update P_((void));
DECLARE_DO_FUN(do_memory);
DECLARE_DO_FUN(do_status);
float number_speed P_((void));
int number_fuzzy P_((int number));
int number_range P_((int from, int to));
int number_percent P_((void));
int number_door P_((void));
int number_bits P_((int width));
void init_mm P_((void));
int number_mm P_((void));
int dice P_((int number, int size));
int interpolate P_((int level, int value_00, int value_32));
void append_file P_((CHAR_DATA *ch, char *file, char *str));
void bugf P_((char *fmt, ...));
void log_f P_((char *fmt, ...));
void bug P_((const char *str, int param));
void bug_string P_((const char *str, const char *str2));
void log_string P_((const char *str));
void message_update P_((void));
bool char_exists P_((char *argument));
int bv_log P_((int n));
void check_chistory P_((CHAR_DATA *ch, int channel));
void update_chistory P_((CHAR_DATA *ch, char *argument, int channel));
int count_helps P_((void));
int count_skills P_((void));
char *find_helps P_((const char *search, bool imm));
char *grep_helps P_((const char *search, bool imm));
char *_popen P_((const char *search));
FILE *file_open P_((const char *file, const char *opt));
void file_close P_((FILE *file));
void clear_lists P_((void));

#undef P_
