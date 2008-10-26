/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_BUILD_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* build.c */
void build_interpret P_((CHAR_DATA *ch, char *argument));
void build_commands P_((CHAR_DATA *ch, char *argument));
void build_showmob P_((CHAR_DATA *ch, char *argument));
void build_showobj P_((CHAR_DATA *ch, char *argument));
void build_showroom P_((CHAR_DATA *ch, char *argument));
void build_showresets P_((CHAR_DATA *ch, char *argument));
char *reset_to_text P_((BUILD_DATA_LIST **pList, int *pcount));
char *build_docount P_((int *pcount));
void build_findmob P_((CHAR_DATA *ch, char *argument));
void build_findmobroom P_((CHAR_DATA *ch, char *argument));
void build_findobject P_((CHAR_DATA *ch, char *argument));
void build_findroom P_((CHAR_DATA *ch, char *argument));
void build_setmob P_((CHAR_DATA *ch, char *argument));
void nuke_exit_resets P_((ROOM_INDEX_DATA *pRoomIndex, int door));
void build_setroom P_((CHAR_DATA *ch, char *argument));
void build_setobject P_((CHAR_DATA *ch, char *argument));
void build_dig P_((CHAR_DATA *ch, char *argument));
void build_stop P_((CHAR_DATA *ch, char *argument));
void do_build P_((CHAR_DATA *ch, char *argument));
void build_addmob P_((CHAR_DATA *ch, char *argument));
void build_addobject P_((CHAR_DATA *ch, char *argument));
void build_addroom P_((CHAR_DATA *ch, char *argument));
void build_addreset P_((CHAR_DATA *ch, char *argument));
void build_delreset P_((CHAR_DATA *ch, char *argument));
void build_delwarn P_((CHAR_DATA *ch, char *argument));
void build_delhelp P_((CHAR_DATA *ch, char *argument));
void build_delroom P_((CHAR_DATA *ch, char *argument));
void build_delobject P_((CHAR_DATA *ch, char *argument));
void build_delmob P_((CHAR_DATA *ch, char *argument));
void build_help P_((CHAR_DATA *ch, char *argument));
void build_forcereset P_((CHAR_DATA *ch, char *argument));
char *build_simpstrdup P_((char *buf));
void build_strdup P_((char **dest, char *src, bool freesrc, bool newline, CHAR_DATA *ch));
void build_editstr P_((char **dest, char *src, CHAR_DATA *ch));
void build_finishedstr P_((char *orig, char **dest, CHAR_DATA *ch, bool saved));
void build_set_oedit P_((CHAR_DATA *ch, char *argument));
void build_set_redit P_((CHAR_DATA *ch, char *argument));
void build_set_medit P_((CHAR_DATA *ch, char *argument));
void build_set_nedit P_((CHAR_DATA *ch, char *argument));
void build_setvnum P_((CHAR_DATA *ch, char *argument));
void build_list P_((CHAR_DATA *ch, char *argument));
void build_set P_((CHAR_DATA *ch, char *argument));
void build_listvalues P_((CHAR_DATA *ch, char *argument));
void build_listweapons P_((CHAR_DATA *ch, char *argument));
void build_listliquids P_((CHAR_DATA *ch, char *argument));
void build_listspells P_((CHAR_DATA *ch, char *argument));
int get_dir P_((char dir));
void build_urooms P_((CHAR_DATA *ch, char *argument));
void build_uobjs P_((CHAR_DATA *ch, char *argument));
void build_umobs P_((CHAR_DATA *ch, char *argument));
void build_findhelp P_((CHAR_DATA *ch, char *argument));
void build_helpedit P_((CHAR_DATA *ch, char *argument));
void build_addhelp P_((CHAR_DATA *ch, char *argument));
void build_clone P_((CHAR_DATA *ch, char *argument));
void check_autodig P_((CHAR_DATA *ch, int dir));
void build_sysdata P_((CHAR_DATA *ch, char *argument));

#undef P_
