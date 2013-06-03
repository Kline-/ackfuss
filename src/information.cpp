/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/
/**
 * @file information.cpp
 * @brief All non-template member functions of the Information namespace.
 *
 * The Information namespace contains all functions that display general
 * information to characters.
 */
#include "h/includes.h"
#include "h/information.h"

/*** @name Core */ /**@{*/ /*
const string Information::FormatObjectToCharacter( OBJ_DATA* obj, CHAR_DATA* ch, const bool& short, const bool& name )
{
    static char buf[MSL];
    static char buf2[MSL];

    snprintf( buf, MSL, "%s", color_string( ch, "objects" ) );

    if ( IS_IMMORTAL(ch) && ch->position == POS_BUILDING ) * Imms should see vnums, <3 builders :) --Kline *
    {
        snprintf(buf2, MSL, "(%d) ", obj->pIndexData->vnum);
        strncat(buf, buf2, MSL - 1);
    }

    * Check for mquest target *
    strncat(buf, display_obj_target(ch, obj), MSL - 1);

    if ( IS_OBJ_STAT( obj, ITEM_EXTRA_INVIS ) )
        strncat( buf, "(Invis) ", MSL - 1 );
    if ( ( IS_AFFECTED( ch, AFF_DETECT_EVIL ) || item_has_apply( ch, ITEM_APPLY_DET_EVIL ) ) && IS_OBJ_STAT( obj, ITEM_EXTRA_EVIL ) )
        strncat( buf, "(Red Aura) ", MSL - 1 );
    if ( ( IS_AFFECTED( ch, AFF_DETECT_MAGIC ) || item_has_apply( ch, ITEM_APPLY_DET_MAG ) ) && IS_OBJ_STAT( obj, ITEM_EXTRA_MAGIC ) )
        strncat( buf, "(Magical) ", MSL - 1 );
    if ( IS_OBJ_STAT( obj, ITEM_EXTRA_GLOW ) )
        strncat( buf, "(Glowing) ", MSL - 1 );
    if ( IS_OBJ_STAT( obj, ITEM_EXTRA_HUM ) )
        strncat( buf, "(Humming) ", MSL - 1 );

    if ( fShort )
    {
        if ( obj->short_descr != NULL )
            strncat( buf, obj->short_descr, MSL - 1 );
    }
    else
    {
        if ( obj->long_descr != NULL )
            strncat( buf, obj->long_descr, MSL - 1 );
    }
    strncat( buf, color_string( ch, "normal" ), MSL - 1 );

    if ( iName ) * Display the name of items, in case you don't know or forgot them. --Kline *
    {
        snprintf(buf2, MSL, " [%s]", obj->name);
        strncat(buf, buf2, MSL - 1);
    }

    return buf;
}*/
/**@}*/

/*** @name Query */ /**@{*/
/**@}*/

/*** @name Manipulate */ /**@{*/
/**@}*/

/*** @name Internal */ /**@{*/
/**@}*/
