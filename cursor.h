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
 *                        http://ackmud.nuc.net/                           *
 *                        zenithar@ackmud.nuc.net                          *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#define MAP_TOP           "0"
#define MAP_BOTTOM        "10"
#define MAIN_TOP          "11"
#define MAIN_BOTTOM       "30"

#define CRS_CMD          "\033["
#define CRS_RESET         CRS_CMD "c" CRS_CMD "r"
#define CRS_CLS           CRS_CMD "2J"
#define CRS_HOME          CRS_CMD "0;0H"
#define CRS_SAVE_ALL      CRS_CMD "s"
#define CRS_REST_ALL      CRS_CMD "u"
#define CRS_MOVE( top, bottom )   ( CRS_CMD top ";" bottom "H" )
#define CRS_SCRL_MAP      CRS_CMD "1;10r"
#define CRS_SCRL_MAIN     CRS_CMD "11;30r"
#define CRS_SET_ALL       CRS_RESET CRS_CLS CRS_SCRL_MAIN CRS_CMD MAIN_TOP ";0H"
