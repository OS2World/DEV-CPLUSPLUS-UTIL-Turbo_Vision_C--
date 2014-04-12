/*
 * util.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <ssigala@globalnet.it>
 */

#if !defined( __UTIL_H )
#define __UTIL_H

inline int min( int a, int b )
{
    return (a>b) ? b : a;
}

inline int max( int a, int b )
{
    return (a<b) ? b : a;
}

void fexpand( char * );
void backSlash2Slash( char * path );
void Slash2backSlash( char * path );

char hotKey( const char *s );
ushort ctrlToArrow( ushort );
char getAltChar( ushort keyCode );
ushort getAltCode( char ch );
char getCtrlChar(ushort);
ushort getCtrlCode(uchar);

ushort historyCount( uchar id );
const char *historyStr( uchar id, int index );
void historyAdd( uchar id, const char * );

int cstrlen( const char * );

class TView;
void *message( TView *receiver, ushort what, ushort command, void *infoPtr );
Boolean lowMemory();

char *newStr( const char * );

Boolean isDir( const char *str );

Boolean pathValid( const char *path );

Boolean validFileName( const char *fileName );

void getCurDir( char *dir );

Boolean isWild( const char *f );

// from system.cc

void expandPath(const char *path, char *dir, char *file);
//long int filelength(int fd);

#endif  // __UTIL_H
