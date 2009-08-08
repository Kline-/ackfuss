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
DECLARE_DO_FUN(build_interpret);
DECLARE_DO_FUN(build_commands);
DECLARE_DO_FUN(build_showmob);
DECLARE_DO_FUN(build_showobj);
DECLARE_DO_FUN(build_showroom);
DECLARE_DO_FUN(build_showresets);
char *reset_to_text P_((BUILD_DATA_LIST **pList, int *pcount));
char *build_docount P_((int *pcount));
DECLARE_DO_FUN(build_findmob);
DECLARE_DO_FUN(build_findmobroom);
DECLARE_DO_FUN(build_findobject);
DECLARE_DO_FUN(build_findroom);
DECLARE_DO_FUN(build_setmob);
void nuke_exit_resets P_((ROOM_INDEX_DATA *pRoomIndex, int door));
DECLARE_DO_FUN(build_setroom);
DECLARE_DO_FUN(build_setobject);
DECLARE_DO_FUN(build_dig);
DECLARE_DO_FUN(build_stop);
DECLARE_DO_FUN(do_build);
DECLARE_DO_FUN(build_addmob);
DECLARE_DO_FUN(build_addobject);
DECLARE_DO_FUN(build_addroom);
DECLARE_DO_FUN(build_addreset);
DECLARE_DO_FUN(build_delreset);
DECLARE_DO_FUN(build_delwarn);
DECLARE_DO_FUN(build_delhelp);
DECLARE_DO_FUN(build_delroom);
DECLARE_DO_FUN(build_delobject);
DECLARE_DO_FUN(build_delmob);
DECLARE_DO_FUN(build_help);
DECLARE_DO_FUN(build_forcereset);
char *build_simpstrdup P_((char *buf));
void build_strdup P_((char **dest, char *src, bool freesrc, bool newline, CHAR_DATA *ch));
void build_editstr P_((char **dest, char *src, CHAR_DATA *ch));
void build_finishedstr P_((char *orig, char **dest, CHAR_DATA *ch, bool saved));
DECLARE_DO_FUN(build_set_oedit);
DECLARE_DO_FUN(build_set_redit);
DECLARE_DO_FUN(build_set_medit);
DECLARE_DO_FUN(build_set_nedit);
DECLARE_DO_FUN(build_setvnum);
DECLARE_DO_FUN(build_list);
DECLARE_DO_FUN(build_set);
DECLARE_DO_FUN(build_listvalues);
DECLARE_DO_FUN(build_listweapons);
DECLARE_DO_FUN(build_listliquids);
DECLARE_DO_FUN(build_listspells);
int get_dir P_((char dir));
DECLARE_DO_FUN(build_urooms);
DECLARE_DO_FUN(build_uobjs);
DECLARE_DO_FUN(build_umobs);
DECLARE_DO_FUN(build_findhelp);
DECLARE_DO_FUN(build_helpedit);
DECLARE_DO_FUN(build_addhelp);
DECLARE_DO_FUN(build_clone);
void check_autodig P_((CHAR_DATA *ch, int dir));
DECLARE_DO_FUN(build_sysdata);

#undef P_
