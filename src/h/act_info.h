/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_ACT_INFO_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* act_info.c */
char *format_obj_to_char P_((OBJ_DATA * obj, CHAR_DATA * ch, bool fShort));
void show_char_to_char_0 P_((CHAR_DATA *victim, CHAR_DATA *ch));
void show_char_to_char_1 P_((CHAR_DATA *victim, CHAR_DATA *ch));
void show_char_to_char P_((CHAR_DATA *list, CHAR_DATA *ch));
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_examine);
DECLARE_DO_FUN(do_exits);
DECLARE_DO_FUN(do_score);
DECLARE_DO_FUN(do_affected);
DECLARE_DO_FUN(do_time);
DECLARE_DO_FUN(do_weather);
DECLARE_DO_FUN(do_help);
DECLARE_DO_FUN(do_who);
DECLARE_DO_FUN(do_inventory);
DECLARE_DO_FUN(do_equipment);
DECLARE_DO_FUN(do_compare);
DECLARE_DO_FUN(do_credits);
DECLARE_DO_FUN(do_where);
DECLARE_DO_FUN(do_consider);
DECLARE_DO_FUN(do_title);
DECLARE_DO_FUN(do_description);
DECLARE_DO_FUN(do_report);
DECLARE_DO_FUN(do_practice);
DECLARE_DO_FUN(do_wimpy);
DECLARE_DO_FUN(do_password);
DECLARE_DO_FUN(do_socials);
DECLARE_DO_FUN(do_commands);
DECLARE_DO_FUN(do_channels);
DECLARE_DO_FUN(do_config);
DECLARE_DO_FUN(do_wizlist);
DECLARE_DO_FUN(do_race_list);
DECLARE_DO_FUN(do_clan_list);
DECLARE_DO_FUN(do_spells);
DECLARE_DO_FUN(do_slist);
DECLARE_DO_FUN(do_autoexit);
DECLARE_DO_FUN(do_autoloot);
DECLARE_DO_FUN(do_autosac);
DECLARE_DO_FUN(do_blank);
DECLARE_DO_FUN(do_brief);
DECLARE_DO_FUN(do_combine);
DECLARE_DO_FUN(do_autodig);
DECLARE_DO_FUN(do_autobrief);
DECLARE_DO_FUN(do_pagelen);
DECLARE_DO_FUN(do_prompt);
DECLARE_DO_FUN(do_diagnose);
DECLARE_DO_FUN(do_heal);
DECLARE_DO_FUN(do_gain);
DECLARE_DO_FUN(do_assassinate);
DECLARE_DO_FUN(do_alias);
DECLARE_DO_FUN(do_color);
char *color_string P_((CHAR_DATA *ch, char *argument));
DECLARE_DO_FUN(do_worth);
DECLARE_DO_FUN(do_whois);
DECLARE_DO_FUN(do_shelp);
DECLARE_DO_FUN(do_afk);
DECLARE_DO_FUN(do_colist);
DECLARE_DO_FUN(do_loot);
char *output_race_wear P_((int iRace));
DECLARE_DO_FUN(do_safe);
void area_message P_((AREA_DATA *area, const char *message));
DECLARE_DO_FUN(do_whitelist);
DECLARE_DO_FUN(do_logins);

#undef P_
