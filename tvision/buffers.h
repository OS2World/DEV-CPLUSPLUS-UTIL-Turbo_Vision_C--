/*
 * buffers.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <ssigala@globalnet.it>
 */

#if defined( Uses_TVMemMgr ) && !defined( __TVMemMgr )
#define __TVMemMgr

const DEFAULT_SAFETY_POOL_SIZE = 4096;

class TBufListEntry
{

private:

    TBufListEntry( void*& );
    ~TBufListEntry();

    void *operator new( size_t, size_t );
    void *operator new( size_t );
    void operator delete( void * );

    TBufListEntry *next;
    TBufListEntry *prev;
    void*& owner;

    static TBufListEntry *bufList;
    static Boolean freeHead();

    friend class TVMemMgr;
    friend void *operator new( size_t );
    friend void * allocBlock( size_t );

};

class TVMemMgr
{

public:

    TVMemMgr();

    static void resizeSafetyPool( size_t = DEFAULT_SAFETY_POOL_SIZE );
    static int safetyPoolExhausted();
#ifndef __UNPATCHED
    static void clearSafetyPool();
#endif

    static void allocateDiscardable( void *&, size_t );
    static void freeDiscardable( void * );
#ifndef __UNPATCHED
    static void suspend(void);
#endif

private:

    static void * safetyPool;
    static size_t safetyPoolSize;
    static int inited;
    static int initMemMgr();

};

#endif  // Uses_TVMemMgr
