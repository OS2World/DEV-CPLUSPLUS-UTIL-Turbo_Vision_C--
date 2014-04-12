/*
 * TApplication.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <ssigala@globalnet.it>
 */

#define Uses_TEventQueue
#define Uses_TScreen
#define Uses_TObject
#define Uses_TMouse
#define Uses_TApplication
#ifndef __UNPATCHED
#define Uses_TVMemMgr
#endif
#include <tvision/tv.h>

//static TScreen tsc;
//static TEventQueue teq;

void initHistory();
void doneHistory();

TApplication::TApplication() :
    TProgInit( &TApplication::initStatusLine,
                  &TApplication::initMenuBar,
                  &TApplication::initDeskTop
                )
{
    initHistory();
}

TApplication::~TApplication()
{
    doneHistory();
}

void TApplication::suspend()
{
    /* SS: changed */

//    TSystemError::suspend();
//    TEventQueue::suspend();
    TScreen::suspend();
#ifndef __UNPATCHED
    TVMemMgr::suspend();       // Release discardable memory.
#endif
}

void TApplication::resume()
{
    /* SS: changed */

    TScreen::resume();
//    TEventQueue::resume();
//    TSystemError::resume();
}
