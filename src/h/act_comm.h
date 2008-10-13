/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_ACT_COMM_H 1

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* act_comm.c */
void note_attach P_((CHAR_DATA *ch));
void note_remove P_((CHAR_DATA *ch, NOTE_DATA *pnote));
void do_note P_((CHAR_DATA *ch, char *argument));
void talk_channel P_((CHAR_DATA *ch, char *argument, int channel, const char *verb));
void do_creator P_((CHAR_DATA *ch, char *argument));
void do_gossip P_((CHAR_DATA *ch, char *argument));
void do_crusade P_((CHAR_DATA *ch, char *argument));
void do_music P_((CHAR_DATA *ch, char *argument));
void do_quest2 P_((CHAR_DATA *ch, char *argument));
void do_game P_((CHAR_DATA *ch, char *argument));
void do_race P_((CHAR_DATA *ch, char *argument));
void do_clan P_((CHAR_DATA *ch, char *argument));
void do_familytalk P_((CHAR_DATA *ch, char *argument));
void do_howl P_((CHAR_DATA *ch, char *argument));
void do_newbie P_((CHAR_DATA *ch, char *argument));
void do_question P_((CHAR_DATA *ch, char *argument));
void do_answer P_((CHAR_DATA *ch, char *argument));
void do_shout P_((CHAR_DATA *ch, char *argument));
void do_flame P_((CHAR_DATA *ch, char *argument));
void do_zzz P_((CHAR_DATA *ch, char *argument));
void do_yell P_((CHAR_DATA *ch, char *argument));
void do_immtalk P_((CHAR_DATA *ch, char *argument));
void do_diptalk P_((CHAR_DATA *ch, char *argument));
void do_remorttalk P_((CHAR_DATA *ch, char *argument));
void do_ooc P_((CHAR_DATA *ch, char *argument));
void do_adepttalk P_((CHAR_DATA *ch, char *argument));
void do_say P_((CHAR_DATA *ch, char *argument));
void do_ignore P_((CHAR_DATA *ch, char *argument));
void do_tell P_((CHAR_DATA *ch, char *argument));
void do_reply P_((CHAR_DATA *ch, char *argument));
void do_emote P_((CHAR_DATA *ch, char *argument));
void do_pose P_((CHAR_DATA *ch, char *argument));
void do_bug P_((CHAR_DATA *ch, char *argument));
void do_idea P_((CHAR_DATA *ch, char *argument));
void do_typo P_((CHAR_DATA *ch, char *argument));
void do_rent P_((CHAR_DATA *ch, char *argument));
void do_qui P_((CHAR_DATA *ch, char *argument));
void do_quit P_((CHAR_DATA *ch, char *argument));
void do_save P_((CHAR_DATA *ch, char *argument));
void do_follow P_((CHAR_DATA *ch, char *argument));
void add_follower P_((CHAR_DATA *ch, CHAR_DATA *master));
void stop_follower P_((CHAR_DATA *ch));
void die_follower P_((CHAR_DATA *ch));
void do_order P_((CHAR_DATA *ch, char *argument));
void group_all P_((CHAR_DATA *ch));
void do_group P_((CHAR_DATA *ch, char *argument));
void do_split P_((CHAR_DATA *ch, char *argument));
void do_gtell P_((CHAR_DATA *ch, char *argument));
void do_pemote P_((CHAR_DATA *ch, char *argument));
void do_pray P_((CHAR_DATA *ch, char *argument));
void do_tongue P_((CHAR_DATA *ch, char *argument));
char *slur_text P_((char *argument));
void do_whisper P_((CHAR_DATA *ch, char *argument));
void do_ask P_((CHAR_DATA *ch, char *argument));
void send_to_room P_((char *message, ROOM_INDEX_DATA *room));
void do_beep P_((CHAR_DATA *ch, char *argument));
void ask_quest_question P_((CHAR_DATA *ch, char *argument));
bool is_same_group P_(( CHAR_DATA * ach, CHAR_DATA * bch ));
bool is_group_leader P_(( CHAR_DATA * ch ));
bool is_note_to P_(( CHAR_DATA * ch, NOTE_DATA * pnote ));

#undef P_
