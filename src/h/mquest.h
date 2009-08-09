/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_MQUEST_H

/*
 * Original mquest code kindly donated by Abel@StormHunters. Thanks!
 */

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* mquest.c */
DECLARE_DO_FUN(do_mquest);
DECLARE_DO_FUN(do_qstat);
void mquest_info P_((CHAR_DATA *ch));
void mquest_complete_message P_((CHAR_DATA *ch));
void mquest_calc_rewards P_((CHAR_DATA *ch));
void mquest_clear P_((CHAR_DATA *ch, bool error));
void generate_killing_quest P_((CHAR_DATA *ch));
CHAR_DATA *get_quest_kill P_((int min_lev, int max_lev, CHAR_DATA *ch));
void generate_retrieval_quest P_((CHAR_DATA *ch));
OBJ_DATA *get_quest_item P_((int min_lev, int max_lev, CHAR_DATA *ch));
char *display_mob_target P_((CHAR_DATA *ch, CHAR_DATA *victim));
char *display_obj_target P_((CHAR_DATA *ch, OBJ_DATA *obj));
void update_mquest_wait_time P_((CHAR_DATA *ch));
void update_mquest_kill P_((CHAR_DATA *ch, CHAR_DATA *victim));

#undef P_

#define	QUEST_NONE            0
#define QUEST_RETRIEVAL       1 /* Find X item(s) Y time           */
#define QUEST_KILLING         2 /* Kill X mob(s) Y time            */
#define QUEST_MULTI_KILL      3 /* Kill X mob(s) Y times           */
#define QUEST_MULTI_RETRIEVE  4 /* Find X item(s) Y times          */
#define QUEST_KILLING_R       5 /* Kill X mob(s) of race Y Z time  */
#define QUEST_MULTI_KILL_R    6 /* Kill X mob(s) of race Y Z times */

#define QUEST_MAX_DATA        5
#define QUEST_MAX_REWARD      3

#define QUEST_REWARD_QP       0
#define QUEST_REWARD_GOLD     1
#define QUEST_REWARD_EXP      2
