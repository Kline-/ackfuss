/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_HANDLER_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* handler.c */
void my_get_age P_((CHAR_DATA *ch, char *buf));
int my_get_hours P_((CHAR_DATA *ch));
int get_age P_((CHAR_DATA *ch));
int get_curr_str P_((CHAR_DATA *ch));
int get_curr_int P_((CHAR_DATA *ch));
int get_curr_wis P_((CHAR_DATA *ch));
int get_curr_dex P_((CHAR_DATA *ch));
int get_curr_con P_((CHAR_DATA *ch));
int can_carry_n P_((CHAR_DATA *ch));
int can_carry_w P_((CHAR_DATA *ch));
void affect_modify P_((CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd));
void affect_to_room P_((ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *raf));
void r_affect_remove P_((ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *raf));
void affect_to_char P_((CHAR_DATA *ch, AFFECT_DATA *paf));
void affect_remove P_((CHAR_DATA *ch, AFFECT_DATA *paf));
void affect_strip P_((CHAR_DATA *ch, int sn));
bool is_affected P_((CHAR_DATA *ch, int sn));
void affect_join P_((CHAR_DATA *ch, AFFECT_DATA *paf));
OBJ_DATA *get_light_char P_((CHAR_DATA *ch));
void char_from_room P_((CHAR_DATA *ch));
void char_to_room P_((CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex));
void obj_to_char P_((OBJ_DATA *obj, CHAR_DATA *ch));
void obj_from_char P_((OBJ_DATA *obj));
int apply_ac P_((OBJ_DATA *obj, int iWear));
OBJ_DATA *get_eq_char P_((CHAR_DATA *ch, int iWear));
void equip_char P_((CHAR_DATA *ch, OBJ_DATA *obj, int iWear));
void unequip_char P_((CHAR_DATA *ch, OBJ_DATA *obj));
int count_obj_list P_((OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list));
int count_obj_room P_((OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list));
void obj_from_room P_((OBJ_DATA *obj));
void obj_to_room P_((OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex));
void obj_to_obj P_((OBJ_DATA *obj, OBJ_DATA *obj_to));
void obj_from_obj P_((OBJ_DATA *obj));
void extract_obj P_((OBJ_DATA *obj));
void extract_char P_((CHAR_DATA *ch, bool fPull));
bool char_exists_world P_((const char *who));
CHAR_DATA *get_char_room P_((CHAR_DATA *ch, char *argument));
CHAR_DATA *get_char_world P_((CHAR_DATA *ch, char *argument));
CHAR_DATA *get_char_area P_((CHAR_DATA *ch, char *argument));
CHAR_DATA *get_char P_((CHAR_DATA *ch));
OBJ_DATA *get_obj_type P_((OBJ_INDEX_DATA *pObjIndex));
OBJ_DATA *get_obj_room P_((CHAR_DATA *ch, char *argument, OBJ_DATA *list));
OBJ_DATA *get_obj_list P_((CHAR_DATA *ch, char *argument, OBJ_DATA *list));
OBJ_DATA *get_obj_carry P_((CHAR_DATA *ch, char *argument));
OBJ_DATA *get_obj_wear P_((CHAR_DATA *ch, char *argument));
OBJ_DATA *get_obj_here P_((CHAR_DATA *ch, char *argument));
OBJ_DATA *get_obj_world P_((CHAR_DATA *ch, char *argument));
OBJ_DATA *create_money P_((int amount));
int get_obj_number P_((OBJ_DATA *obj));
int get_obj_weight P_((OBJ_DATA *obj));
bool room_is_dark P_((ROOM_INDEX_DATA *pRoomIndex));
bool room_is_private P_((ROOM_INDEX_DATA *pRoomIndex));
bool can_see P_((CHAR_DATA *ch, CHAR_DATA *victim));
bool can_see_obj P_((CHAR_DATA *ch, OBJ_DATA *obj));
bool can_drop_obj P_((CHAR_DATA *ch, OBJ_DATA *obj));
bool can_sac_obj P_((CHAR_DATA *ch, OBJ_DATA *obj));
bool can_use P_((CHAR_DATA *ch, OBJ_DATA *obj));
char *who_can_use P_((OBJ_DATA *obj));
void notify P_((char *message, int lv));
void auction P_((char *message));
void info P_((char *message, int lv));
void log_chan P_((const char *message, int lv));
bool item_has_apply P_((CHAR_DATA *ch, int bit));
CHAR_DATA *switch_char P_((CHAR_DATA *victim, int mvnum, int poly_level));
CHAR_DATA *unswitch_char P_((CHAR_DATA *victim));
void set_stun P_((CHAR_DATA *victim, int stunTime));
bool is_shielded P_((CHAR_DATA *ch, short shield_type));
void remove_shield P_((CHAR_DATA *ch, MAGIC_SHIELD *shield));
bool check_charm_aff P_((CHAR_DATA *ch, short charm_aff));
short get_charm_bonus P_((CHAR_DATA *ch, short charm_aff));
ROOM_INDEX_DATA *in_loc P_((OBJ_DATA *obj));
bool is_switched P_((CHAR_DATA *ch));

#undef P_
