/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  Ack 2.2 improvements copyright (C) 1994 by Stephen Dooley              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *       _/          _/_/_/     _/    _/     _/    ACK! MUD is modified    *
 *      _/_/        _/          _/  _/       _/    Merc2.0/2.1/2.2 code    *
 *     _/  _/      _/           _/_/         _/    (c)Stephen Zepp 1998    *
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3          *
 *   _/      _/      _/_/_/     _/    _/     _/                            *
 *                                                                         *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#define DEC_INTERACT_H

#ifndef DEC_UTILS_H
#include "utils.h"
#endif
/*  Types of interactions to check  */
#define	CHECKS_NONE		BIT_0
#define	CHECKS_CLAN		BIT_1
#define	CHECKS_RACE		BIT_2
#define	CHECKS_LEVEL		BIT_3
#define	CHECKS_REP_GEN		BIT_4
#define	CHECKS_REP_SHOP		BIT_5
#define	CHECKS_REP_ROYAL	BIT_6
#define	CHECKS_REP_UNDEAD	BIT_7
#define	CHECKS_REP_NATURE	BIT_8
#define	CHECKS_REP_HOLY		BIT_9
#define	CHECKS_REP_CLAN		BIT_10
#define	CHECKS_REP_UNDERWORLD	BIT_11
#define	CHECKS_REP_LAW		BIT_12

#define	ACTION_NONE		BIT_0
#define	ACTION_ATTACK		BIT_1
#define	ACTION_HAIL		BIT_2
#define	ACTION_BLOCK		BIT_3
#define	ACTION_AID		BIT_4
#define	ACTION_HEAL		BIT_5
#define	ACTION_TRANSFER		BIT_6
#define	ACTION_FLEE		BIT_7
#define	ACTION_ALARM		BIT_8
#define	ACTION_COWER		BIT_9
#define	ACTION_REFUSE		BIT_10
