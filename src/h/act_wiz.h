/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_ACT_WIZ_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* act_wiz.c */
DECLARE_DO_FUN(do_transdm);
DECLARE_DO_FUN(do_wizhelp);
DECLARE_DO_FUN(do_bamfin);
DECLARE_DO_FUN(do_bamfout);
DECLARE_DO_FUN(do_deny);
DECLARE_DO_FUN(do_disconnect);
DECLARE_DO_FUN(do_pardon);
DECLARE_DO_FUN(do_echo);
void recho P_((ROOM_INDEX_DATA *room, const char *argument));
DECLARE_DO_FUN(do_recho);
ROOM_INDEX_DATA *find_location P_((CHAR_DATA *ch, char *arg));
DECLARE_DO_FUN(do_transfer);
DECLARE_DO_FUN(do_at);
DECLARE_DO_FUN(do_goto);
DECLARE_DO_FUN(do_rstat);
DECLARE_DO_FUN(do_ostat);
DECLARE_DO_FUN(do_mstat);
DECLARE_DO_FUN(do_olmsg);
DECLARE_DO_FUN(do_ofindlev);
DECLARE_DO_FUN(do_mfind);
DECLARE_DO_FUN(do_mfindlev);
DECLARE_DO_FUN(do_ofind);
DECLARE_DO_FUN(do_mwhere);
DECLARE_DO_FUN(do_reboo);
DECLARE_DO_FUN(do_reboot);
DECLARE_DO_FUN(do_shutdow);
DECLARE_DO_FUN(do_shutdown);
DECLARE_DO_FUN(do_snoop);
DECLARE_DO_FUN(do_switch);
DECLARE_DO_FUN(do_return);
DECLARE_DO_FUN(do_mload);
DECLARE_DO_FUN(do_oload);
DECLARE_DO_FUN(do_purge);
DECLARE_DO_FUN(do_trust);
DECLARE_DO_FUN(do_restore);
DECLARE_DO_FUN(do_freeze);
DECLARE_DO_FUN(do_log);
DECLARE_DO_FUN(do_noemote);
DECLARE_DO_FUN(do_notell);
DECLARE_DO_FUN(do_silence);
DECLARE_DO_FUN(do_nopray);
DECLARE_DO_FUN(do_peace);
DECLARE_DO_FUN(do_ban);
DECLARE_DO_FUN(do_allow);
DECLARE_DO_FUN(do_deathmatc);
DECLARE_DO_FUN(do_deathmatch);
DECLARE_DO_FUN(do_wizlock);
DECLARE_DO_FUN(do_slookup);
DECLARE_DO_FUN(do_sset);
DECLARE_DO_FUN(do_mset);
DECLARE_DO_FUN(do_oset);
DECLARE_DO_FUN(do_rset);
DECLARE_DO_FUN(do_users);
DECLARE_DO_FUN(do_force);
DECLARE_DO_FUN(do_invis);
DECLARE_DO_FUN(do_holylight);
DECLARE_DO_FUN(do_wizify);
DECLARE_DO_FUN(do_owhere);
DECLARE_DO_FUN(do_mpcr);
DECLARE_DO_FUN(do_resetpassword);
DECLARE_DO_FUN(do_iscore);
DECLARE_DO_FUN(do_fights);
DECLARE_DO_FUN(do_iwhere);
DECLARE_DO_FUN(do_setclass);
DECLARE_DO_FUN(do_isnoop);
DECLARE_DO_FUN(do_dog);
DECLARE_DO_FUN(do_togbuild);
DECLARE_DO_FUN(do_togleader);
DECLARE_DO_FUN(do_whoname);
DECLARE_DO_FUN(do_lhunt);
DECLARE_DO_FUN(do_sstat);
DECLARE_DO_FUN(do_monitor);
void monitor_chan P_((const char *message, int channel));
DECLARE_DO_FUN(do_reward);
DECLARE_DO_FUN(do_fhunt);
DECLARE_DO_FUN(do_alink);
DECLARE_DO_FUN(do_togcouncil);
DECLARE_DO_FUN(do_gain_stat_reset);
const char *name_expand P_((CHAR_DATA *ch));
DECLARE_DO_FUN(do_for);
DECLARE_DO_FUN(do_otype);
DECLARE_DO_FUN(do_owear);
DECLARE_DO_FUN(do_areasave);
DECLARE_DO_FUN(do_findreset);
DECLARE_DO_FUN(do_census);
DECLARE_DO_FUN(do_sla);
DECLARE_DO_FUN(do_slay);
DECLARE_DO_FUN(do_hotreboo);
DECLARE_DO_FUN(do_hotreboot);
DECLARE_DO_FUN(do_disable);
DECLARE_DO_FUN(do_lua);
DECLARE_DO_FUN(do_olua);
DECLARE_DO_FUN(do_rlua);
DECLARE_DO_FUN(do_ldebug);
#undef P_
