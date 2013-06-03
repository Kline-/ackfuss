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
/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/
#include "h/includes.h"
#include "h/list.h"

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
#endif

#ifndef DEC_ACT_INFO_H
#include "h/act_info.h"
#endif

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_BUILD_H
#include "h/build.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_RULERS_H
#include "h/rulers.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

void do_qpspend( CHAR_DATA * ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    /*    char arg3 [MAX_INPUT_LENGTH];  */
    char buf[MAX_STRING_LENGTH];
    char catbuf[MSL];

    snprintf( catbuf, MSL, "%s", "" );

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    /*    argument = one_argument( argument, arg2 );  */

    if ( IS_NPC( ch ) )
    {
        send_to_char( "Not for NPC's.\r\n", ch );
        return;
    }

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Syntax: qpspend enter | exit | assist <message> \r\n", ch );
        send_to_char( "or:     qpspend show | buy | clear | recall | home | corpse | rescue\r\n", ch );
        send_to_char( "or:     qpspend set name |short | long <message>\r\n", ch );
        send_to_char( "To set your enter or exit message, for example, type:\r\n", ch );
        send_to_char( "qpspend enter waddles in from\r\n", ch );
        send_to_char( "qpspend show to see a test, then qpspend buy\r\n", ch );
        return;
    }

    if ( !str_cmp( arg1, "enter" ) )
    {
        if ( str_cmp( ch->pcdata->pedit_state, "walk" ) )
        {
            free_string( ch->pcdata->pedit_string[0] );
            ch->pcdata->pedit_string[0] = str_dup( "none" );
            free_string( ch->pcdata->pedit_state );
            ch->pcdata->pedit_state = str_dup( "walk" );
        }
        free_string( ch->pcdata->pedit_string[0] );
        ch->pcdata->pedit_string[0] = str_dup( argument );
        return;
    }
    if ( !str_cmp( arg1, "exit" ) )
    {
        if ( str_cmp( ch->pcdata->pedit_state, "walk" ) )
        {
            free_string( ch->pcdata->pedit_string[0] );
            ch->pcdata->pedit_string[0] = str_dup( "none" );
            free_string( ch->pcdata->pedit_state );
            ch->pcdata->pedit_state = str_dup( "walk" );
        }

        free_string( ch->pcdata->pedit_string[1] );
        ch->pcdata->pedit_string[1] = str_dup( argument );
        return;
    }
    if ( !str_cmp( arg1, "assist" ) )
    {
        if ( str_cmp( ch->pcdata->pedit_state, "assist" ) )
        {
            free_string( ch->pcdata->pedit_string[0] );
            ch->pcdata->pedit_string[0] = str_dup( "none" );
            free_string( ch->pcdata->pedit_string[1] );
            ch->pcdata->pedit_string[1] = str_dup( "none" );
            free_string( ch->pcdata->pedit_state );
            ch->pcdata->pedit_state = str_dup( "assist" );
        }
        free_string( ch->pcdata->pedit_string[0] );
        if ( !is_name( "*name*", argument ) )
        {
            send_to_char( "You must include *name* by itself somewhere in your assist message!\r\n", ch );
            ch->pcdata->pedit_string[0] = str_dup( "none" );
            return;
        }
        if ( str_cmp( "assist", ch->pcdata->pedit_state ) )
        {
            free_string( ch->pcdata->pedit_state );
            ch->pcdata->pedit_state = str_dup( "assist" );
        }
        ch->pcdata->pedit_string[0] = str_dup( argument );
        return;
    }

    if ( !str_cmp( arg1, "show" ) )
    {
        if ( !str_cmp( ch->pcdata->pedit_state, "walk" ) )
        {
            char move_buf[MSL];
            char test_string[MSL];
            short qp_cost = 0;

            if ( !str_cmp( ch->pcdata->pedit_string[0], "none" ) )
            {
                snprintf( test_string, MSL, "%s", ch->pcdata->room_enter );
            }
            else
            {
                snprintf( test_string, MSL, "%s", ch->pcdata->pedit_string[0] );
                qp_cost++;
            }
            snprintf( move_buf, MSL, "$L%s$n %s $T.", get_ruler_title( ch->pcdata->ruler_rank, ch->pcdata->login_sex ), test_string );
            act( move_buf, ch, NULL, rev_name[1], TO_CHAR );
            if ( !str_cmp( ch->pcdata->pedit_string[1], "none" ) )
            {
                snprintf( test_string, MSL, "%s", ch->pcdata->room_exit );
            }
            else
            {
                snprintf( test_string, MSL, "%s", ch->pcdata->pedit_string[1] );
                qp_cost++;
            }


            snprintf( move_buf, MSL, "$L%s$n %s $T.", get_ruler_title( ch->pcdata->ruler_rank, ch->pcdata->login_sex ), test_string );
            act( move_buf, ch, NULL, dir_name[1], TO_CHAR );

            snprintf( buf, MSL, "Purchase cost is %d qps.\r\n", qp_cost );
            send_to_char( buf, ch );
            return;
        }
        else if ( !str_cmp( ch->pcdata->pedit_state, "assist" ) )
        {
            snprintf( buf, MSL, "Current message:\r\n  %s\r\n",
                      ( !str_cmp( ch->pcdata->pedit_string[0], "none" ) ?
                        ch->pcdata->assist_msg : ch->pcdata->pedit_string[0] ) );
            send_to_char( buf, ch );
        }
        return;
    }

    if ( !str_cmp( arg1, "buy" ) )
    {
        short qp_cost = 0;
        short i;

        if ( !str_cmp( ch->pcdata->pedit_state, "walk" ) )
        {
            if ( str_cmp( ch->pcdata->pedit_string[0], "none" ) )
                qp_cost++;
            if ( str_cmp( ch->pcdata->pedit_string[1], "none" ) )
                qp_cost++;
            if ( ch->pcdata->quest_points < qp_cost )
            {
                send_to_char( "You don't have enough quest points!\r\n", ch );
                for ( i = 0; i < 5; i++ )
                {
                    if ( str_cmp( ch->pcdata->pedit_string[i], "none" ) )
                    {
                        free_string( ch->pcdata->pedit_string[i] );
                        ch->pcdata->pedit_string[i] = str_dup( "none" );
                    }
                }
                return;
            }
            else if ( qp_cost == 0 )
            {
                send_to_char( "No changes.\r\n", ch );
                return;
            }
            else
            {
                if ( str_cmp( ch->pcdata->pedit_string[0], "none" ) )
                {
                    free_string( ch->pcdata->room_enter );
                    ch->pcdata->room_enter = str_dup( ch->pcdata->pedit_string[0] );
                    free_string( ch->pcdata->pedit_string[0] );
                    ch->pcdata->pedit_string[0] = str_dup( "none" );
                    snprintf( catbuf, MSL, "Enter message changed to %s\r\n", ch->pcdata->room_enter );
                }
                if ( str_cmp( ch->pcdata->pedit_string[1], "none" ) )
                {
                    free_string( ch->pcdata->room_exit );
                    ch->pcdata->room_exit = str_dup( ch->pcdata->pedit_string[1] );
                    free_string( ch->pcdata->pedit_string[1] );
                    ch->pcdata->pedit_string[1] = str_dup( "none" );
                    snprintf( catbuf, MSL, "Exit message changed to %s\r\n", ch->pcdata->room_exit );
                }
                free_string( ch->pcdata->pedit_string[0] );
                ch->pcdata->pedit_string[0] = str_dup( "none" );
                free_string( ch->pcdata->pedit_string[1] );
                ch->pcdata->pedit_string[1] = str_dup( "none" );
                free_string( ch->pcdata->pedit_state );
                ch->pcdata->pedit_state = str_dup( "none" );
                ch->pcdata->quest_points -= qp_cost;
                do_save( ch, "auto" );
                return;
            }
        }
        else if ( !str_cmp( ch->pcdata->pedit_state, "assist" ) )
        {
            char assistbuf[MSL];
            char *parse = ch->pcdata->pedit_string[0];
            char word1[MSL];
            assistbuf[0] = '\0';
            if ( ch->pcdata->quest_points < 3 )
            {
                send_to_char( "You do not have enough quest points!\r\n", ch );
                for ( i = 0; i < 5; i++ )
                {
                    if ( str_cmp( ch->pcdata->pedit_string[i], "none" ) )
                    {
                        free_string( ch->pcdata->pedit_string[i] );
                        ch->pcdata->pedit_string[i] = str_dup( "none" );
                    }
                    free_string( ch->pcdata->pedit_state );
                    ch->pcdata->pedit_state = str_dup( "none" );
                }
                return;
            }
            for ( ;; )
            {
                parse = one_word( parse, word1 );
                if ( word1[0] == '\0' )
                    break;
                if ( !str_cmp( "*name*", word1 ) )
                {
                    strncat( assistbuf, "$N ", MSL - 1 );
                    continue;
                }
                else
                {
                    snprintf( catbuf, MSL, "%s ", word1 );
                    strncat( assistbuf, catbuf, MSL - 1 );
                }
            }
            free_string( ch->pcdata->assist_msg );
            ch->pcdata->assist_msg = str_dup( assistbuf );
            for ( i = 0; i < 5; i++ )
            {
                if ( str_cmp( ch->pcdata->pedit_string[i], "none" ) )
                {
                    free_string( ch->pcdata->pedit_string[i] );
                    ch->pcdata->pedit_string[i] = str_dup( "none" );
                }
            }
            free_string( ch->pcdata->pedit_state );
            ch->pcdata->pedit_state = str_dup( "none" );
            send_to_char( "Done!\r\n", ch );
            ch->pcdata->quest_points -= 3;
            do_save( ch, "auto" );
            return;
        }
        return;
    }
    if ( !str_cmp( arg1, "rescue" ) )
    {
        if ( ch->in_room->vnum == ROOM_VNUM_JAIL )
        {
            send_to_char( "No one can rescue you from here.\r\n", ch );
            return;
        }

        if ( ch->pcdata->quest_points < 10 )
        {
            send_to_char( "You don't have enough quest points. Cost is 10 qp.\r\n", ch );
            return;
        }
        else
        {
            ROOM_INDEX_DATA *location;
            if ( ( location = get_room_index( ch->pcdata->recall_vnum ) ) == NULL )
                location = get_room_index( ROOM_VNUM_TEMPLE );
            act( "@@mKline @@Wappears before you, and takes you to your Happy Place!", ch, NULL, NULL, TO_CHAR );
            act( "@@mKline @@Wappears before you, and takes $n to $s Happy Place!", ch, NULL, NULL, TO_ROOM );
            char_from_room( ch );
            char_to_room( ch, location );
            do_look( ch, "" );
            act( "$n steps into the room from a @@apulsating @@mvortex@@N.", ch, NULL, NULL, TO_ROOM );
            ch->pcdata->quest_points -= 10;
            do_save( ch, "auto" );
            return;
        }
    }
    if ( !str_cmp( arg1, "home" ) )
    {
        if ( ch->pcdata->quest_points < 50 )
        {
            send_to_char( "You don't have enough quest points. Cost is 50 qp.\r\n", ch );
            return;
        }
        else
        {
            if ( ch->in_room->sector_type != SECT_RECALL_OK )
            {
                send_to_char( "This is not a legal location to call your home.\r\n", ch );
                return;
            }
            else if ( ( ch->in_room == NULL ) || ( ch->in_room->vnum < 3 ) )
            {
                send_to_char( "You are LOST!\r\n", ch );
                return;
            }
            else
            {

                ch->pcdata->recall_vnum = ch->in_room->vnum;
                ch->pcdata->quest_points -= 50;
                do_save( ch, "auto" );
                send_to_char( "You know call this room your home, and will recall here!\r\n", ch );
                return;
            }
        }
    }

    if ( !str_cmp( arg1, "corpse" ) )
    {
        if ( ch->pcdata->quest_points < 10 )
        {
            send_to_char( "You don't have enough quest points. Cost is 10 qp.\r\n", ch );
            return;
        }
        else if ( ( ch->in_room == NULL ) || ( ch->in_room->vnum < 3 ) )
        {
            send_to_char( "You are completly LOST!!\r\n", ch );
            return;
        }
        else
        {
            OBJ_DATA *obj;
            bool found = FALSE;
            list<OBJ_DATA *>::iterator li;

            for ( li = obj_list.begin(); li != obj_list.end(); li++ )
            {
                obj = *li;
                if ( ( ( obj->pIndexData->vnum ) == OBJ_VNUM_CORPSE_PC ) && ( !str_cmp( ch->name, obj->owner ) ) && ( !( obj->in_room == ch->in_room ) ) ) /*don't work! */
                {
                    found = TRUE;
                    obj_from_room( obj );
                    obj_to_room( obj, ch->in_room );
                    act( "Got the blighter!", ch, NULL, NULL, TO_CHAR );
                }
            }
            /*
             * act used to enable mobiles to check for CR triggers...
             */

            if ( !found )
            {
                act( "Couldn't find it.", ch, NULL, NULL, TO_CHAR );
            }
            ch->pcdata->quest_points -= 10;
            do_save( ch, "auto" );
            return;
        }
    }


    if ( !str_cmp( arg1, "set" ) )
    {
        if ( !is_name( arg2, "unique rename" ) )
        {
            send_to_char( "That is not a currently implemented editing state.\r\n", ch );
            return;
        }
        free_string( ch->pcdata->pedit_state );
        ch->pcdata->pedit_state = str_dup( arg2 );
        return;
    }
    if ( !str_cmp( arg1, "clear" ) )
    {
        short i;
        for ( i = 0; i < 5; i++ )
        {
            free_string( ch->pcdata->pedit_string[i] );
            ch->pcdata->pedit_string[i] = str_dup( "none" );
        }
        return;
    }

}

void do_statraise(CHAR_DATA *ch, char *argument)
{
    int cost = 5;
    int statcap = 25;
    char buf[MSL];

    /* Note, you'll need to edit your stat tables in const.c
     * to give bonuses past 25 for each stat, based on whatever
     * limit you set in this function --Kline
     */

    if ( IS_NPC(ch) ) /* Not on mobs */
        return;

    if ( argument[0] == '\0' ) /* No arg supplied, show the syntax */
    {
        send_to_char("Syntax is: statraise <str/int/wis/dex/con>\r\n", ch);
        return;
    }
    if ( !str_prefix(argument, "str") ) /* Matches on 's', 'st', 'str' */
    {
        if ( ch->pcdata->max_str >= statcap )
        {
            snprintf(buf, MSL, "You can not raise your Str any further. The limit is %d.\r\n", statcap);
            send_to_char(buf, ch);
            return;
        }
        cost *= ch->pcdata->max_str; /* Take base cost (5) mult by current value */
        if ( ch->pcdata->quest_points < cost )
        {
            snprintf(buf, MSL, "It costs %d QP to raise your Str, but you only have %d.\r\n", cost, ch->pcdata->quest_points);
            send_to_char(buf, ch);
            return;
        }
        ch->pcdata->max_str++; /* Raise the actual cap */
        ch->pcdata->perm_str++; /* Give them a free stat train */
        ch->pcdata->quest_points -= cost;
        snprintf(buf, MSL, "You have spent %d QP to raise your Str! It is now %d and you have %d QP remaining.\r\n", cost, ch->pcdata->max_str, ch->pcdata->quest_points);
        send_to_char(buf, ch);
        return;
    }
    else if ( !str_prefix(argument, "int") ) /* Matches on 'i', 'in', 'int' */
    {
        if ( ch->pcdata->max_int >= statcap )
        {
            snprintf(buf, MSL, "You can not raise your Int any further. The limit is %d.\r\n", statcap);
            send_to_char(buf, ch);
            return;
        }
        cost *= ch->pcdata->max_int; /* Take base cost (5) mult by current value */
        if ( ch->pcdata->quest_points < cost )
        {
            snprintf(buf, MSL, "It costs %d QP to raise your Int, but you only have %d.\r\n", cost, ch->pcdata->quest_points);
            send_to_char(buf, ch);
            return;
        }
        ch->pcdata->max_int++; /* Raise the actual cap */
        ch->pcdata->perm_int++; /* Give them a free stat train */
        ch->pcdata->quest_points -= cost;
        snprintf(buf, MSL, "You have spent %d QP to raise your Int! It is now %d and you have %d QP remaining.\r\n", cost, ch->pcdata->max_int, ch->pcdata->quest_points);
        send_to_char(buf, ch);
        return;
    }
    else if ( !str_prefix(argument, "wis") ) /* Matches on 'w', 'wi', 'wis' */
    {
        if ( ch->pcdata->max_wis >= statcap )
        {
            snprintf(buf, MSL, "You can not raise your Wis any further. The limit is %d.\r\n", statcap);
            send_to_char(buf, ch);
            return;
        }
        cost *= ch->pcdata->max_wis; /* Take base cost (5) mult by current value */
        if ( ch->pcdata->quest_points < cost )
        {
            snprintf(buf, MSL, "It costs %d QP to raise your Wis, but you only have %d.\r\n", cost, ch->pcdata->quest_points);
            send_to_char(buf, ch);
            return;
        }
        ch->pcdata->max_wis++; /* Raise the actual cap */
        ch->pcdata->perm_wis++; /* Give them a free stat train */
        ch->pcdata->quest_points -= cost;
        snprintf(buf, MSL, "You have spent %d QP to raise your Wis! It is now %d and you have %d QP remaining.\r\n", cost, ch->pcdata->max_wis, ch->pcdata->quest_points);
        send_to_char(buf, ch);
        return;
    }
    else if ( !str_prefix(argument, "dex") ) /* Matches on 'd', 'de', 'dex' */
    {
        if ( ch->pcdata->max_dex >= statcap )
        {
            snprintf(buf, MSL, "You can not raise your Dex any further. The limit is %d.\r\n", statcap);
            send_to_char(buf, ch);
            return;
        }
        cost *= ch->pcdata->max_dex; /* Take base cost (5) mult by current value */
        if ( ch->pcdata->quest_points < cost )
        {
            snprintf(buf, MSL, "It costs %d QP to raise your Dex, but you only have %d.\r\n", cost, ch->pcdata->quest_points);
            send_to_char(buf, ch);
            return;
        }
        ch->pcdata->max_dex++; /* Raise the actual cap */
        ch->pcdata->perm_dex++; /* Give them a free stat train */
        ch->pcdata->quest_points -= cost;
        snprintf(buf, MSL, "You have spent %d QP to raise your Dex! It is now %d and you have %d QP remaining.\r\n", cost, ch->pcdata->max_dex, ch->pcdata->quest_points);
        send_to_char(buf, ch);
        return;
    }
    else if ( !str_prefix(argument, "con") ) /* Matches on 'c', 'co', 'con' */
    {
        if ( ch->pcdata->max_con >= statcap )
        {
            snprintf(buf, MSL, "You can not raise your Con any further. The limit is %d.\r\n", statcap);
            send_to_char(buf, ch);
            return;
        }
        cost *= ch->pcdata->max_con; /* Take base cost (5) mult by current value */
        if ( ch->pcdata->quest_points < cost )
        {
            snprintf(buf, MSL, "It costs %d QP to raise your Con, but you only have %d.\r\n", cost, ch->pcdata->quest_points);
            send_to_char(buf, ch);
            return;
        }
        ch->pcdata->max_con++; /* Raise the actual cap */
        ch->pcdata->perm_con++; /* Give them a free stat train */
        ch->pcdata->quest_points -= cost;
        snprintf(buf, MSL, "You have spent %d QP to raise your Con! It is now %d and you have %d QP remaining.\r\n", cost, ch->pcdata->max_con, ch->pcdata->quest_points);
        send_to_char(buf, ch);
        return;
    }
    else /* Invalid argument, display syntax */
    {
        send_to_char("Syntax is: statraise <str/int/wis/dex/con>\r\n", ch);
        return;
    }
}
