/*
 * tvconfig.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <ssigala@globalnet.it>
 */

#if !defined( __CONFIG_H )
#define __CONFIG_H

#include <limits.h>

const eventQSize = 16;
const maxCollectionSize = (int)(( (long) UINT_MAX - 16)/sizeof( void * ));

const maxViewWidth = 132;

const maxFindStrLen    = 80;
const maxReplaceStrLen = 80;

#endif  // __CONFIG_H
