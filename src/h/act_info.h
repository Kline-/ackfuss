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
void do_look P_((CHAR_DATA *ch, char *argument));
void do_examine P_((CHAR_DATA *ch, char *argument));
void do_exits P_((CHAR_DATA *ch, char *argument));
void do_score P_((CHAR_DATA *ch, char *argument));
void do_affected P_((CHAR_DATA *ch, char *argument));
void do_time P_((CHAR_DATA *ch, char *argument));
void do_weather P_((CHAR_DATA *ch, char *argument));
void do_help P_((CHAR_DATA *ch, char *argument));
void do_who P_((CHAR_DATA *ch, char *argument));
void do_inventory P_((CHAR_DATA *ch, char *argument));
void do_equipment P_((CHAR_DATA *ch, char *argument));
void do_compare P_((CHAR_DATA *ch, char *argument));
void do_credits P_((CHAR_DATA *ch, char *argument));
void do_where P_((CHAR_DATA *ch, char *argument));
void do_consider P_((CHAR_DATA *ch, char *argument));
void set_title P_((CHAR_DATA *ch, char *title));
void do_title P_((CHAR_DATA *ch, char *argument));
void do_description P_((CHAR_DATA *ch, char *argument));
void do_report P_((CHAR_DATA *ch, char *argument));
void do_practice P_((CHAR_DATA *ch, char *argument));
void do_wimpy P_((CHAR_DATA *ch, char *argument));
void do_password P_((CHAR_DATA *ch, char *argument));
void do_socials P_((CHAR_DATA *ch, char *argument));
void do_commands P_((CHAR_DATA *ch, char *argument));
void do_channels P_((CHAR_DATA *ch, char *argument));
void do_config P_((CHAR_DATA *ch, char *argument));
void do_wizlist P_((CHAR_DATA *ch, char *argument));
void do_race_list P_((CHAR_DATA *ch, char *argument));
void do_clan_list P_((CHAR_DATA *ch, char *argument));
void do_spells P_((CHAR_DATA *ch, char *argument));
void do_slist P_((CHAR_DATA *ch, char *argument));
void do_autoexit P_((CHAR_DATA *ch, char *argument));
void do_autoloot P_((CHAR_DATA *ch, char *argument));
void do_autosac P_((CHAR_DATA *ch, char *argument));
void do_blank P_((CHAR_DATA *ch, char *argument));
void do_brief P_((CHAR_DATA *ch, char *argument));
void do_combine P_((CHAR_DATA *ch, char *argument));
void do_autodig P_((CHAR_DATA *ch, char *argument));
void do_autobrief P_((CHAR_DATA *ch, char *argument));
void do_pagelen P_((CHAR_DATA *ch, char *argument));
void do_prompt P_((CHAR_DATA *ch, char *argument));
void do_auto P_((CHAR_DATA *ch, char *argument));
void do_players P_((CHAR_DATA *ch, char *argument));
void do_diagnose P_((CHAR_DATA *ch, char *argument));
void do_heal P_((CHAR_DATA *ch, char *argument));
void do_gain P_((CHAR_DATA *ch, char *argument));
void do_assassinate P_((CHAR_DATA *ch, char *argument));
void do_alias P_((CHAR_DATA *ch, char *argument));
void do_color P_((CHAR_DATA *ch, char *argument));
char *color_string P_((CHAR_DATA *ch, char *argument));
void do_worth P_((CHAR_DATA *ch, char *argument));
void do_whois P_((CHAR_DATA *ch, char *argument));
void do_shelp P_((CHAR_DATA *ch, char *argument));
void do_afk P_((CHAR_DATA *ch, char *argument));
void do_colist P_((CHAR_DATA *ch, char *argument));
void do_loot P_((CHAR_DATA *ch, char *argument));
char *output_race_wear P_((int iRace));
void do_safe P_((CHAR_DATA *ch, char *argument));
void area_message P_((AREA_DATA *area, const char *message));

#undef P_
