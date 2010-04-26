/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#include "h/globals.h"

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
#endif

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_BUILDTAB_H
#include "h/buildtab.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_EMAIL_H
#include "h/email.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_SAVE_H
#include "h/save.h"
#endif

#ifndef DEC_SPENDQP_H
#include "h/spendqp.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

DO_FUN(do_email)
{
    if ( IS_NPC( ch ) )
        return;
    return;
}

DO_FUN(do_tog_email)
{
    bool mort = false, imm = false;
    int value = 0;

    if ( IS_NPC( ch ) )
        return;

    if ( argument[0] == '\0' )
    {
        if ( ch->pcdata->email->flags.test(table_lookup(tab_email,"mort")) )
            mort = true;
        if ( ch->pcdata->email->flags.test(table_lookup(tab_email,"imm")) )
            imm = true;

        ch->send("You are currently accepting emails from ");

        if ( mort && imm )
            ch->send("players and immortals.\r\n");
        else if ( mort )
            ch->send("players only.\r\n");
        else if ( imm )
            ch->send("immortals only.\r\n");
        else
            ch->send("no one.\r\n");

        ch->send("To enable emails from players or immortals, use togemail <mortal/immortal>.\r\n");
        return;
    }

    if ( !str_prefix( argument, "mortal" ) )
    {
        value = table_lookup(tab_email,"mort");

        if ( ch->pcdata->email->flags.test(value) )
            ch->send("You will no longer receive emails from other players.\r\n");
        else
            ch->send("You will now be able to receive emails from other players.\r\n");

        ch->pcdata->email->flags.flip(value);

        return;
    }

    if ( !str_prefix( argument, "immortal" ) )
    {
        value = table_lookup(tab_email,"imm");

        if ( ch->pcdata->email->flags.test(value) )
            ch->send("You will no longer receive emails from immortals.\r\n");
        else
            ch->send("You will now be able to receive emails from immortals.\r\n");

        ch->pcdata->email->flags.flip(value);

        return;
    }

    /* Generate usage message */
    do_tog_email( ch );

    return;
}

DO_FUN(do_verify_email)
{
    if ( IS_NPC( ch ) )
        return;
    if ( ch->pcdata->email->address.empty() )
    {
        ch->send( "You need to provide an email address first with setemail <address>.\r\n" );
        return;
    }
    if ( ch->pcdata->email->verified )
    {
        ch->send( "You have already verified your email. If you wish to update your email, please use setemail <address>.\r\n" );
        return;
    }
    if ( argument[0] == '\0' )
    {
        ch->send( "To verify your email address you must everify <code> using the code you received.\r\n" );
        return;
    }
    if ( !str_cmp( argument, ch->pcdata->email->confirmation_code ) )
    {
       ch->send( "Thank you for verifying your email address.\r\n");
       ch->pcdata->email->confirmation_code.clear();
       ch->pcdata->email->verified = true;
       snprintf( log_buf, MSL, "%s (%s) has verified their email address.", ch->get_name(), ch->pcdata->email->address.c_str() );
       monitor_chan( log_buf, MONITOR_EMAIL );
       return;
    }
    else
    {
        ch->send( "Invalid code. If you did not receive a validation code please reset your email using the setemail command.\r\n" );
        return;
    }

    return;
}

DO_FUN(do_set_email)
{
    char body[MSL], subject[MSL];

    if ( IS_NPC( ch ) )
        return;
    if ( argument[0] == '\0' )
    {
        ch->send( "You didn't provide an email address. Syntax is setemail <email>.\r\n" );
        return;
    }

    ch->pcdata->email->address = argument;
    ch->pcdata->email->confirmation_code.clear();
    ch->pcdata->email->confirmation_code = gen_rand_string(8);
    ch->pcdata->email->verified = false;
    ch->send( "An email has been sent to %s with a confirmation code. Please verify your address by typing everify <code> once your have received the email. If you do not receive an email, set your email again and a new code will be sent.\r\n", argument );

    snprintf( subject, MSL, "%s Email Confirmation Code", mudnamenocolor );
    snprintf( body, MSL, "<html>This email was used by a player of %s. If this was not you, please disregard it.<br><br>Confirmation code: <b>%s</b></html>", mudnamenocolor, ch->pcdata->email->confirmation_code.c_str() );
    send_email( argument, subject, body, false, ch);

    return;
}

bool send_email( const char *address, const char *subject, const char *body, bool validate, CHAR_DATA *ch )
{
    char mailbuf[MSL];
    FILE *fp;

    if( IS_NPC(ch) ) /* Safety check for later --Kline */
        ch = NULL;

    if ( validate && ch != NULL && !ch->pcdata->email->verified )
    {
        snprintf( log_buf, MSL, "Unable to send email to %s (%s); email not verified.", ch->get_name(), ch->pcdata->email->address.c_str() );
        monitor_chan( log_buf, MONITOR_EMAIL );
        return false;
    }

    snprintf( mailbuf, MSL, "echo \"%s\" | mail -a \"From: '%s' <%s>\" -a \"Content-type: text/html;\" -s \"%s\" \"%s\"", body, mudnamenocolor, mudemailaddr, subject, ch == NULL ? address : ch->pcdata->email->address.c_str() );

    /*
     * system() is if() encapsulated to suppress a warning. system() returns different results on different distros,
     * so there is no reliable return value to check against. --Kline
     */
    if ( system( mailbuf ) ) {}

    if( ch == NULL )
        snprintf( log_buf, MSL, "An email was sent to (%s) with subject (%s).", address, subject );
    else
        snprintf( log_buf, MSL, "An email was sent to %s (%s) with subject (%s).", ch->get_name(), ch->pcdata->email->address.c_str(), subject );
    monitor_chan( log_buf, MONITOR_EMAIL );

    if ( ( fp = file_open( EMAIL_FILE, "a" ) ) != NULL )
    {
        fprintf( fp, "%s :: %s\n", current_time_str(), mailbuf );
        file_close( fp );
    }

    return true;
}

LOOKUP_TYPE tab_email[] =
{
 {"mort", F_EMAIL_MORT, 0},
 {"imm",  F_EMAIL_IMM,  0},
 {NULL,0}
};
