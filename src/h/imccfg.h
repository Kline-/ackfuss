/* Codebase macros - Change as you might need.
 * Yes, Rogel, you can gloat all you want. You win, this is cleaner, though not by a whole lot.
 */
/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#ifndef __IMC2CFG_H__
#define __IMC2CFG_H__

#define CH_IMCDATA(ch)           ((ch)->pcdata->imcchardata)
#define CH_IMCLEVEL(ch)          ((ch)->level)
#define CH_IMCNAME(ch)           (const_cast<char *>(ch->get_name()))
#define CH_IMCSEX(ch)            ((ch)->sex)
#define CH_IMCTITLE(ch)          ((ch)->get_title())

#define first_descriptor first_desc
#define CH_IMCRANK(ch)           (str_cmp(ch->pcdata->who_name,"off") ? ch->pcdata->who_name : IS_IMMORTAL(ch) ? "Imm" : class_table[(ch)->p_class].who_name)

#endif
