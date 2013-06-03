/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/
/**
 * @file utils.cpp
 * @brief All non-template member functions of the Utils namespace.
 *
 * The Utils namespace contains all general purpose, multi-user, and non-class
 * functions. Classes that are implemented to extend default operators or
 * designed to be used in a utility sense, rather than actual objects, are also
 * contained within this namespace.
 */
#include "h/includes.h"
#include "h/utils.h"

/*** @name Core */ /**@{*/
/**@}*/

/*** @name Query */ /**@{*/
/**@}*/

/*** @name Manipulate */ /**@{*/
/**@}*/

/*** @name Internal */ /**@{*/
/**
 * @brief This is a nested wrapper for Utils::__FormatString and should not be called directly.
 * @param[in] narg A #uint_t variable of the total number of arguments passed. Handled automatically.
 * @param[in] flags Any number of flags from #UTILS_OPTS to control output formatting and options.
 * @param[in] caller A string value containing the calling function. Handled automatically.
 * @param[in] fmt A string value containing a printf-style format string.
 * @param[in] ... A variable arguments list to populate fmt with.
 * @retval string A printf-style formatted string.
 */
const string Utils::_FormatString( const uint_t& narg, const bitset<CFG_MEM_MAX_BITSET>& flags, const string& caller, const string& fmt, ... )
{
    UFLAGS_DE( uflags );
    va_list args;
    string output;

    if ( fmt.empty() )
    {
        LOGSTR( uflags, "Utils::_FormatString()-> called with empty fmt" );
        return output;
    }

    va_start( args, fmt );
    output = __FormatString( narg, flags, caller, fmt, args );
    va_end( args );

    return output;
}

/**
 * @brief This is the printf-style string formatter. It should not be invoked directly, but rather by using Utils::FormatString() to ensure proper argument count and caller passing.
 * @param[in] narg A #uint_t variable of the total number of arguments passed. Handled automatically.
 * @param[in] flags Any number of flags from #UTILS_OPTS to control output formatting and options.
 * @param[in] caller A string value containing the calling function. Handled automatically.
 * @param[in] fmt A string value containing a printf-style format string.
 * @param[in] val A variable arguments list to populate fmt with.
 * @retval string A printf-style formatted string.
 */
const string Utils::__FormatString( const uint_t& narg, const bitset<CFG_MEM_MAX_BITSET>& flags, const string& caller, const string& fmt, va_list& val ) // Thanks go to Darien @ MudBytes.net for the start of this
{
    UFLAGS_DE( uflags );
    va_list args;
    vector<string> arguments;
    ITER( vector, string, si );
    vector<char> buf;
    string output, token;
    sint_t size = 0;

    if ( fmt.empty() )
    {
        LOGSTR( uflags, "Utils::__FormatString()-> called with empty fmt" );
        return output;
    }

    arguments = StrTokens( fmt );
    for ( si = arguments.begin(); si != arguments.end(); si++ )
    {
        token = *si;
        if ( token.find( "%" ) != string::npos ) // not foolproof, but it should catch some worst cases by attempting
            size++;                              // to ensure a matching narg : format specifier count
    }

    if ( narg != 1 && narg != static_cast<uint_t>( size ) && narg != NumChar( fmt, "%" ) ) // if narg == 1 invocation was func( flags, string )
    {
        bitset<CFG_MEM_MAX_BITSET> eflags;

        eflags.set( UTILS_TYPE_ERROR );
        Logger( eflags, "Number of arguments (%lu) did not match number of format specifiers (%lu) at: %s", narg, size, CSTR( caller ) );
        return output = "";
    }

    va_copy( args, val );
    size = vsnprintf( NULL, 0, CSTR( fmt ), args );
    va_end( args );

    va_copy( args, val );
    buf.resize( size + 1 );
    vsnprintf( &buf[0], ( size + 1 ), CSTR( fmt ), args );
    va_end( args );

    return output = &buf[0];
}
/**@}*/