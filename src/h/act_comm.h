/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_ACT_COMM_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* act_comm.c */
void note_attach P_((CHAR_DATA *ch));
void note_remove P_((CHAR_DATA *ch, NOTE_DATA *pnote));
DECLARE_DO_FUN(do_note);
void talk_channel P_((CHAR_DATA *ch, char *argument, int channel, const char *verb));
DECLARE_DO_FUN(do_creator);
DECLARE_DO_FUN(do_gossip);
DECLARE_DO_FUN(do_crusade);
DECLARE_DO_FUN(do_music);
DECLARE_DO_FUN(do_quest);
DECLARE_DO_FUN(do_game);
DECLARE_DO_FUN(do_race);
DECLARE_DO_FUN(do_clan);
DECLARE_DO_FUN(do_familytalk);
DECLARE_DO_FUN(do_howl);
DECLARE_DO_FUN(do_newbie);
DECLARE_DO_FUN(do_question);
DECLARE_DO_FUN(do_answer);
DECLARE_DO_FUN(do_shout);
DECLARE_DO_FUN(do_flame);
DECLARE_DO_FUN(do_zzz);
DECLARE_DO_FUN(do_yell);
DECLARE_DO_FUN(do_immtalk);
DECLARE_DO_FUN(do_diptalk);
DECLARE_DO_FUN(do_remorttalk);
DECLARE_DO_FUN(do_ooc);
DECLARE_DO_FUN(do_adepttalk);
DECLARE_DO_FUN(do_say);
DECLARE_DO_FUN(do_ignore);
DECLARE_DO_FUN(do_tell);
DECLARE_DO_FUN(do_reply);
DECLARE_DO_FUN(do_emote);
DECLARE_DO_FUN(do_pose);
DECLARE_DO_FUN(do_bug);
DECLARE_DO_FUN(do_idea);
DECLARE_DO_FUN(do_typo);
DECLARE_DO_FUN(do_qui);
DECLARE_DO_FUN(do_quit);
DECLARE_DO_FUN(do_save);
DECLARE_DO_FUN(do_follow);
void add_follower P_((CHAR_DATA *ch, CHAR_DATA *master));
void stop_follower P_((CHAR_DATA *ch));
void die_follower P_((CHAR_DATA *ch));
DECLARE_DO_FUN(do_order);
DECLARE_DO_FUN(do_group);
DECLARE_DO_FUN(do_split);
DECLARE_DO_FUN(do_gtell);
DECLARE_DO_FUN(do_pemote);
DECLARE_DO_FUN(do_pray);
DECLARE_DO_FUN(do_tongue);
char *ghost_text P_((char *argument));
char *slur_text P_((char *argument));
DECLARE_DO_FUN(do_whisper);
DECLARE_DO_FUN(do_ask);
void send_to_room P_((char *message, ROOM_INDEX_DATA *room));
DECLARE_DO_FUN(do_beep);
void ask_quest_question P_((CHAR_DATA *ch, char *argument));
bool is_same_group P_(( CHAR_DATA * ach, CHAR_DATA * bch ));
bool is_group_leader P_(( CHAR_DATA * ch ));
bool is_note_to P_(( CHAR_DATA * ch, NOTE_DATA * pnote ));

#undef P_
