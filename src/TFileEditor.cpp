/*
 * TFileEditor.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <ssigala@globalnet.it>
 */

#define Uses_TProgram
#define Uses_TGroup
#define Uses_TEditor
#define Uses_TFileEditor
#define Uses_TEvent
#define Uses_opstream
#define Uses_ipstream
#include <tvision/tv.h>

#include <fstream.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

inline ushort min( ushort u1, ushort u2 )
{
    return u1 < u2 ? u1 : u2;
}

inline uint min( uint u1, uint u2 )	/* XXX */
{					/* XXX */
    return u1 < u2 ? u1 : u2;		/* XXX */
}					/* XXX */

TFileEditor::TFileEditor( const TRect& bounds,
                          TScrollBar *aHScrollBar,
                          TScrollBar *aVScrollBar,
                          TIndicator *aIndicator,
                          const char *aFileName
                        ) :
    TEditor( bounds, aHScrollBar, aVScrollBar, aIndicator, 0 )
{
    if( aFileName == 0 )
        fileName[0] = EOS;
    else
        {
        strcpy( fileName, aFileName );
        fexpand( fileName );
        if( isValid )
            isValid = loadFile();
        }
}

void TFileEditor::doneBuffer()
{
    delete buffer;
}

void TFileEditor::handleEvent( TEvent& event )
{
    TEditor::handleEvent(event);
    switch( event.what )
        {
        case evCommand:
            switch( event.message.command )
                {
                case cmSave:
                    save();
                    break;
                case cmSaveAs:
                    saveAs();
                    break;
                default:
                    return;
                }
            break;
        default:
            return;
        }
    clearEvent(event);
}

void TFileEditor::initBuffer()
{
    buffer = new char[bufSize];
}

Boolean TFileEditor::loadFile()
{
#ifdef __OS2__
    ifstream f( fileName, ios::in | ios::binary );
#else
    ifstream f( fileName, ios::in | ios::bin );
#endif
    if( !f )
        {
        setBufLen( 0 );
        return True;
        }
    else
        {
//        long fSize = filelength( f.rdbuf()->fd() );	/* XXX */
//        if( fSize > 0xFFE0L || setBufSize(ushort(fSize)) == False ) /* XXX */
        uint fSize = filelength( f.rdbuf()->fd() );	/* XXX */
        if( setBufSize(fSize) == False ) /* XXX */
            {
            editorDialog( edOutOfMemory );
            return False;
            }
        else
            f.read( &buffer[bufSize - fSize], fSize );	/* XXX */
            if( !f )
            {
                editorDialog( edReadError, fileName );
                return False;
            }
            else
            {
//Orlik code
char *p, *b; 
p = &buffer[bufSize - fSize]; b = p;
char *n, *a; 
n = new char[fSize]; a = n;
int i, crlfcount = 0;
for( i = 0 ; i < fSize ; ++i ) {
	if( *p == 0x0d ) {	++crlfcount; }
	else { *n = *p; ++n;	}
	++p;
	}
fSize -= crlfcount;
memcpy( b+crlfcount, a, fSize );
delete a;
//end of Orlik code
                setBufLen( fSize );
                return True;
            }
        }
}

Boolean TFileEditor::save()
{
    if( *fileName == EOS )
        return saveAs();
    else
        return saveFile();
}

Boolean TFileEditor::saveAs()
{
    Boolean res = False;
    if( editorDialog( edSaveAs, fileName ) != cmCancel )
        {
        fexpand( fileName );
        message( owner, evBroadcast, cmUpdateTitle, 0 );
        res = saveFile();
        if( isClipboard() == True )
            *fileName = EOS;
        }
    return res;
}

//static void writeBlock( ofstream& f, char *buf, unsigned len ) /* XXX */
static void writeBlock( ofstream& f, char *buf, uint len ) /* XXX */
{
    while( len > 0 )
        {
        int l = len < INT_MAX ? len : INT_MAX;
        f.write( buf, l );
        buf += l;
        len -= l;
        }
}

Boolean TFileEditor::saveFile()
{
/*    char drive[MAXDRIVE];
    char dir[MAXDIR];
    char file[MAXFILE];
    char ext[MAXEXT];*/

    if( (editorFlags & efBackupFiles) != 0 )
        {
//        fnsplit( fileName, drive, dir, file, ext );
//        char backupName[MAXPATH];
//        fnmerge( backupName, drive, dir, file, backupExt );
//        unlink( backupName );

	/* SS: little change */

	char backupName[PATH_MAX];
	sprintf(backupName, "%s~", fileName);
        rename( fileName, backupName );
        }

#ifdef __OS2__
//    ofstream f( fileName, ios::out | ios::binary );
    ofstream f( fileName, ios::out );
#else
    ofstream f( fileName, ios::out | ios::bin );
#endif

    if( !f )
        {
        editorDialog( edCreateError, fileName );
        return False;
        }
    else
        {
        writeBlock( f, buffer, curPtr );
        writeBlock( f, buffer+curPtr+gapLen, bufLen-curPtr );

        if( !f )
            {
            editorDialog( edWriteError, fileName );
            return False;
            }
        else
            {
            modified = False;
            update(ufUpdate);
            }
        }
    return True;
}

//Boolean TFileEditor::setBufSize( ushort newSize )	/* XXX */
Boolean TFileEditor::setBufSize( uint newSize )	/* XXX */
{
    if( newSize == 0)
    	newSize = 0x1000;
//    else if( newSize > 0xF000 )	/* XXX */
//        newSize = 0xFFE0;		/* XXX */
    else
//        newSize = (newSize + 0x0FFF) & 0xF000;	/* XXX */
        newSize = (newSize + 0x0FFF) & 0xFFFFF000;	/* XXX */
    if( newSize != bufSize )
        {
        char *temp = buffer;
        /* Bypass safety pool to allocate buffer, but check for possible
           NULL return value. */
        if( (buffer = (char *) malloc( newSize )) == 0 )
            {
            delete temp;
            return False;
            }
//        ushort n = bufLen - curPtr + delCount;	/* XXX */
        ulong n = bufLen - curPtr + delCount;	/* XXX */
        memcpy( buffer, temp, min( newSize, bufSize ) );
        memmove( &buffer[newSize - n], &temp[bufSize - n], n );
        delete temp;
        bufSize = newSize;
        gapLen = bufSize - bufLen;
        }
    return True;
}

void TFileEditor::shutDown()
{
    setCmdState(cmSave, False);
    setCmdState(cmSaveAs, False);
    TEditor::shutDown();
}

void TFileEditor::updateCommands()
{
    TEditor::updateCommands();
    setCmdState(cmSave, True);
    setCmdState(cmSaveAs, True);
}

Boolean TFileEditor::valid( ushort command )
{
    if( command == cmValid )
        return isValid;
    else
        {
        if( modified == True )
            {
            int d;
            if( *fileName == EOS )
                d = edSaveUntitled;
            else
                d = edSaveModify;

            switch( editorDialog( d, fileName ) )
                {
                case cmYes:
                    return save();
                case cmNo:
                    modified = False;
                    return True;
                case cmCancel:
                    return False;
                }
            }
        }
    return True;
}

#if !defined(NO_STREAMABLE)

void TFileEditor::write( opstream& os )
{
    TEditor::write( os );
    os.writeString( fileName );
    os << selStart << selEnd << curPtr;
}

void *TFileEditor::read( ipstream& is )
{
    TEditor::read( is );
    bufSize = 0;
    is.readString( fileName, sizeof( fileName ) );
    if( isValid )
        {
        isValid = loadFile();
//        ushort sStart, sEnd, curs;	/* XXX */
        ulong sStart, sEnd, curs;	/* XXX */
        is >> sStart >> sEnd >> curs;
        if( isValid && sEnd <= bufLen )
            {
            setSelect( sStart, sEnd, Boolean(curs == sStart) );
            trackCursor( True );
            }
        }
    return this;
}

TStreamable *TFileEditor::build()
{
    return new TFileEditor( streamableInit );
}

TFileEditor::TFileEditor( StreamableInit ) : TEditor( streamableInit )
{
}

#endif
