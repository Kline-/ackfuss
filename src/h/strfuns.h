/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_STRFUNS_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* strfuns.c */
void pre_parse P_((char *list, char *victimname, char *containername, char *things));
bool is_number P_((char *arg));
bool is_name P_((const char *str, char *namelist));
bool is_name_pfx P_((const char *str, char *namelist));
char *space_pad P_((const char *str, short final_size));
void smash_tilde P_((char *str));
void smash_replace P_((char *str, char *sold, char *snew));
bool str_cmp P_((const char *astr, string str));
bool str_cmp P_((string str, const char *bstr));
bool str_cmp P_((const char *astr, const char *bstr));
bool str_prefix P_((const char *astr, const char *bstr));
bool str_infix P_((const char *astr, const char *bstr));
bool str_suffix P_((const char *astr, const char *bstr));
char *capitalize P_((const char *str));
int my_strlen P_((const char *text));
char *learnt_name P_((int learnt));
char *get_adept_name P_((CHAR_DATA *ch));
int nocol_strlen P_((const char *text));
int ccode_len P_((const char *text, short desired));
char *center_text P_((char *text, int width));
char *str_mod P_((char *mod_string, char *argument));
void rand_arg P_((char *argument, char *output));
int number_argument P_((char *argument, char *arg));
char *one_argument P_((char *argument, char *arg_first));
char *one_word P_((char *argument, char *arg_first));
char *two_args P_((char *argument, char *arg_first, char *arg_second));
char *strip_out P_((const char *orig, const char *strip));
char *strip_color P_((const char *orig, const char *strip));
char *get_tribe_standing_name P_((int standing));
char *get_moon_phase_name P_((void));
bool list_in_list P_((char *first_list, char *second_list));
char *item_type_name P_((OBJ_DATA *obj));
char *affect_loc_name P_((int location));
const char *raffect_bit_name P_((int vector));
const char *affect_bit_name P_((int vector));
const char *exit_bit_name P_((bitset<MAX_BITSET> &exit_flags));
const char *extra_bit_name P_((bitset<MAX_BITSET> &extra_flags));
const char *wear_bit_name P_((bitset<MAX_BITSET> &wear_flags));
char *race_name P_((CHAR_DATA *ch));
char *short_race_name P_((CHAR_DATA *ch));
char *get_family_name P_((CHAR_DATA *ch));
char *get_tribe_name P_((CHAR_DATA *ch));
char *strlower P_((char *ip));
char *strupper P_((char *ip));
void ch_printf P_((CHAR_DATA * ch, const char *fmt, ...)) __attribute__ ((format (printf, 2, 3)));
char *tagline_format P_((const char *txt, CHAR_DATA *ch));
bool check_tag P_((char *arg1, char *arg2, int value, CHAR_DATA *ch));
bool evaluate_tag P_((const char *opr, int v1, int v2));

#undef P_
