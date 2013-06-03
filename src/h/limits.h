/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/
/**
 * @file limits.h
 * @brief Custom defined numeric types and system limits.
 *
 *  AckFUSS utilizes two numeric types, both defined within this file. These
 *  types, #sint_t (signed) and #uint_t (unsigned) are implemented as the
 *  numeric maximums of the host operating system. Further OS-specific
 *  limitations may be placed here in the future.
 */
#ifndef DEC_LIMITS_H
#define DEC_LIMITS_H

using namespace std;

#if __WORDSIZE == 64
    /** @name Signed */ /**@{*/
    /**
     * @typedef sint_t
     * @brief A signed integer value based upon system limits.
     */
    typedef signed long int sint_t;

    /**
     * @def sintmax_t
     * @brief The maximum value of any #sint_t variable.
     */
    #define sintmax_t numeric_limits<signed long int>::max()

    /**
     * @def sintmin_t
     * @brief The minimum value of any #sint_t variable.
     */
    #define sintmin_t numeric_limits<signed long int>::min()
    /**@}*/

    /** @name Unsigned */ /**@{*/
    /**
     * @typedef uint_t
     * @brief An unsigned integer value based upon system limits.
     */
    typedef unsigned long int uint_t;

    /**
     * @def uintmax_t
     * @brief The maximum value of any #uint_t variable.
     */
    #define uintmax_t numeric_limits<unsigned long int>::max()

    /**
     * @def uintmin_t
     * @brief The minimum value of any #uint_t variable.
     */
    #define uintmin_t numeric_limits<unsigned long int>::min()
    /**@}*/
#else
    /** @name Signed */ /**@{*/
    /**
     * @typedef sint_t
     * @brief A signed integer value based upon system limits.
     */
    typedef signed int sint_t;

    /**
     * @def sintmax_t
     * @brief The maximum value of any #sint_t variable.
     */
    #define sintmax_t numeric_limits<signed int>::max()

    /**
     * @def sintmin_t
     * @brief The minimum value of any #sint_t variable.
     */
    #define sintmin_t numeric_limits<signed int>::min()
    /**@}*/

    /** @name Unsigned */ /**@{*/
    /**
     * @typedef uint_t
     * @brief An unsigned integer value based upon system limits.
     */
    typedef unsigned int uint_t;

    /**
     * @def uintmax_t
     * @brief The maximum value of any #uint_t variable.
     */
    #define uintmax_t numeric_limits<unsigned int>::max()

    /**
     * @def uintmin_t
     * @brief The minimum value of any #uint_t variable.
     */
    #define uintmin_t numeric_limits<unsigned int>::min()
    /**@}*/
#endif

#endif
