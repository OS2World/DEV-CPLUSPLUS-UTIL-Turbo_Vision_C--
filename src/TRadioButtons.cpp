/*
 * TRadioButtons.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <ssigala@globalnet.it>
 */

#define Uses_TRadioButtons
#include <tvision/tv.h>

void TRadioButtons::draw()
{
    drawMultiBox( button, " \x7" );
}

Boolean TRadioButtons::mark( int item )
{
//    return Boolean( item == value );	/* XXX */
    return Boolean( item == (int)value );	/* XXX */
}

void TRadioButtons::press( int item )
{
    value = item;
}

void TRadioButtons::movedTo( int item )
{
    value = item;
}

void TRadioButtons::setData( void * rec )
{
    TCluster::setData(rec);
    sel = (int)value;
}

#if !defined(NO_STREAMABLE)

TStreamable *TRadioButtons::build()
{
    return new TRadioButtons( streamableInit );
}

TRadioButtons::TRadioButtons( StreamableInit ) : TCluster( streamableInit )
{
}


#endif
