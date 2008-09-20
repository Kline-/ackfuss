/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

/*
 * Original mquest code kindly donated by Abel@StormHunters. Thanks!
 */

#define DEC_MQUEST_H          1

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
