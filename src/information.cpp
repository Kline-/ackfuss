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

#include "h/handler.h"

/*** @name Core */ /**@{*/
const string Information::FormatObjectToCharacter( Object* object, Character* character, const bool& shortd, const bool& name )
{
    string output;

    output = character->gChannelColor( "objects" );

    if ( IS_IMMORTAL( character ) && character->position == POS_BUILDING ) /* Imms should see vnums, <3 builders :) --Kline */
    {
        output += "(";
        output += object->pIndexData->vnum;
        output += ") ";
    }

    /* Check for mquest target */
    output += display_obj_target( character, object );

    if ( IS_OBJ_STAT( object, ITEM_EXTRA_INVIS ) )
        output += "(Invis) ";
    if ( ( IS_AFFECTED( character, AFF_DETECT_EVIL ) || item_has_apply( character, ITEM_APPLY_DET_EVIL ) ) && IS_OBJ_STAT( object, ITEM_EXTRA_EVIL ) )
        output += "(Red Aura) ";
    if ( ( IS_AFFECTED( character, AFF_DETECT_MAGIC ) || item_has_apply( character, ITEM_APPLY_DET_MAG ) ) && IS_OBJ_STAT( object, ITEM_EXTRA_MAGIC ) )
        output += "(Magical) ";
    if ( IS_OBJ_STAT( object, ITEM_EXTRA_GLOW ) )
        output += "(Glowing) ";
    if ( IS_OBJ_STAT( object, ITEM_EXTRA_HUM ) )
        output += "(Humming) ";

    if ( shortd && !object->short_descr.empty() )
        output += object->short_descr;
    else if ( !object->long_descr.empty() )
        output += object->long_descr;

    output += character->gChannelColor( "normal" );

    if ( name ) /* Display the name of items, in case you don't know or forgot them. --Kline */
    {
        output += " [";
        output += object->name;
        output += "]";
    }

    return output;
}
/**@}*/

/*** @name Query */ /**@{*/
/**@}*/

/*** @name Manipulate */ /**@{*/
/**@}*/

/*** @name Internal */ /**@{*/
/**@}*/
