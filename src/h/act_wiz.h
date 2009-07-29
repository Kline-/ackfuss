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
void do_transdm P_((CHAR_DATA *ch, char *argument));
void do_wizhelp P_((CHAR_DATA *ch, char *argument));
void do_bamfin P_((CHAR_DATA *ch, char *argument));
void do_bamfout P_((CHAR_DATA *ch, char *argument));
void do_deny P_((CHAR_DATA *ch, char *argument));
void do_disconnect P_((CHAR_DATA *ch, char *argument));
void do_pardon P_((CHAR_DATA *ch, char *argument));
void do_echo P_((CHAR_DATA *ch, char *argument));
void recho P_((ROOM_INDEX_DATA *room, const char *argument));
void do_recho P_((CHAR_DATA *ch, char *argument));
ROOM_INDEX_DATA *find_location P_((CHAR_DATA *ch, char *arg));
void do_transfer P_((CHAR_DATA *ch, char *argument));
void do_at P_((CHAR_DATA *ch, char *argument));
void do_goto P_((CHAR_DATA *ch, char *argument));
void do_rstat P_((CHAR_DATA *ch, char *argument));
void do_ostat P_((CHAR_DATA *ch, char *argument));
void do_mstat P_((CHAR_DATA *ch, char *argument));
void do_olmsg P_((CHAR_DATA *ch, char *argument));
void do_ofindlev P_((CHAR_DATA *ch, char *argument));
void do_mfind P_((CHAR_DATA *ch, char *argument));
void do_mfindlev P_((CHAR_DATA *ch, char *argument));
void do_ofind P_((CHAR_DATA *ch, char *argument));
void do_mwhere P_((CHAR_DATA *ch, char *argument));
void do_reboo P_((CHAR_DATA *ch, char *argument));
void do_reboot P_((CHAR_DATA *ch, char *argument));
void do_shutdow P_((CHAR_DATA *ch, char *argument));
void do_shutdown P_((CHAR_DATA *ch, char *argument));
void do_snoop P_((CHAR_DATA *ch, char *argument));
void do_switch P_((CHAR_DATA *ch, char *argument));
void do_return P_((CHAR_DATA *ch, char *argument));
void do_mload P_((CHAR_DATA *ch, char *argument));
void do_oload P_((CHAR_DATA *ch, char *argument));
void do_purge P_((CHAR_DATA *ch, char *argument));
void do_trust P_((CHAR_DATA *ch, char *argument));
void do_restore P_((CHAR_DATA *ch, char *argument));
void do_freeze P_((CHAR_DATA *ch, char *argument));
void do_log P_((CHAR_DATA *ch, char *argument));
void do_noemote P_((CHAR_DATA *ch, char *argument));
void do_notell P_((CHAR_DATA *ch, char *argument));
void do_silence P_((CHAR_DATA *ch, char *argument));
void do_nopray P_((CHAR_DATA *ch, char *argument));
void do_peace P_((CHAR_DATA *ch, char *argument));
void do_ban P_((CHAR_DATA *ch, char *argument));
void do_allow P_((CHAR_DATA *ch, char *argument));
void do_deathmatc P_((CHAR_DATA *ch, char *argument));
void do_deathmatch P_((CHAR_DATA *ch, char *argument));
void do_wizlock P_((CHAR_DATA *ch, char *argument));
void do_slookup P_((CHAR_DATA *ch, char *argument));
void do_sset P_((CHAR_DATA *ch, char *argument));
void do_mset P_((CHAR_DATA *ch, char *argument));
void do_oset P_((CHAR_DATA *ch, char *argument));
void do_rset P_((CHAR_DATA *ch, char *argument));
void do_users P_((CHAR_DATA *ch, char *argument));
void do_force P_((CHAR_DATA *ch, char *argument));
void do_invis P_((CHAR_DATA *ch, char *argument));
void do_holylight P_((CHAR_DATA *ch, char *argument));
void do_wizify P_((CHAR_DATA *ch, char *argument));
void do_owhere P_((CHAR_DATA *ch, char *argument));
void do_mpcr P_((CHAR_DATA *ch, char *victim));
void do_resetpassword P_((CHAR_DATA *ch, char *argument));
void do_iscore P_((CHAR_DATA *ch, char *argument));
void do_fights P_((CHAR_DATA *ch, char *argument));
void do_iwhere P_((CHAR_DATA *ch, char *argument));
void do_setclass P_((CHAR_DATA *ch, char *argument));
void do_isnoop P_((CHAR_DATA *ch, char *argument));
void do_dog P_((CHAR_DATA *ch, char *argument));
void do_togbuild P_((CHAR_DATA *ch, char *argument));
void do_togleader P_((CHAR_DATA *ch, char *argument));
void do_whoname P_((CHAR_DATA *ch, char *argument));
void do_lhunt P_((CHAR_DATA *ch, char *argument));
void do_sstat P_((CHAR_DATA *ch, char *argument));
void do_monitor P_((CHAR_DATA *ch, char *argument));
void monitor_chan P_((const char *message, int channel));
void do_reward P_((CHAR_DATA *ch, char *argument));
void do_fhunt P_((CHAR_DATA *ch, char *argument));
void do_alink P_((CHAR_DATA *ch, char *argument));
void do_togcouncil P_((CHAR_DATA *ch, char *argument));
void do_gain_stat_reset P_((CHAR_DATA *ch, char *argument));
const char *name_expand P_((CHAR_DATA *ch));
void do_for P_((CHAR_DATA *ch, char *argument));
void do_otype P_((CHAR_DATA *ch, char *argument));
void do_owear P_((CHAR_DATA *ch, char *argument));
void do_areasave P_((CHAR_DATA *ch, char *argument));
void do_findreset P_((CHAR_DATA *ch, char *argument));
void do_census P_((CHAR_DATA *ch, char *argument));
void do_sla P_((CHAR_DATA *ch, char *argument));
void do_slay P_((CHAR_DATA *ch, char *argument));
void do_hotreboo P_((CHAR_DATA *ch, char *argument));
void do_hotreboot P_((CHAR_DATA *ch, char *argument));
void do_disable P_((CHAR_DATA *ch, char *argument));
void do_lua P_((CHAR_DATA *ch, char *argument));
void do_olua P_((CHAR_DATA *ch, char *argument));
void do_rlua P_((CHAR_DATA *ch, char *argument));

#undef P_
