/*
 * editors.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <ssigala@globalnet.it>
 */

#if !defined( __EDIT_COMMAND_CODES )
#define __EDIT_COMMAND_CODES

const
  ufUpdate = 0x01,
  ufLine   = 0x02,
  ufView   = 0x04;

const
  smExtend = 0x01,
  smDouble = 0x02;

const unsigned
//  sfSearchFailed = 0xFFFF; /* XXX */
  sfSearchFailed = UINT_MAX; /* XXX */

const
  cmFind        = 82,
  cmReplace     = 83,
  cmSearchAgain = 84;

const
  cmCharLeft    = 500,
  cmCharRight   = 501,
  cmWordLeft    = 502,
  cmWordRight   = 503,
  cmLineStart   = 504,
  cmLineEnd     = 505,
  cmLineUp      = 506,
  cmLineDown    = 507,
  cmPageUp      = 508,
  cmPageDown    = 509,
  cmTextStart   = 510,
  cmTextEnd     = 511,
  cmNewLine     = 512,
  cmBackSpace   = 513,
  cmDelChar     = 514,
  cmDelWord     = 515,
  cmDelStart    = 516,
  cmDelEnd      = 517,
  cmDelLine     = 518,
  cmInsMode     = 519,
  cmStartSelect = 520,
  cmHideSelect  = 521,
  cmIndentMode  = 522,
  cmUpdateTitle = 523;

const
  edOutOfMemory   = 0,
  edReadError     = 1,
  edWriteError    = 2,
  edCreateError   = 3,
  edSaveModify    = 4,
  edSaveUntitled  = 5,
  edSaveAs        = 6,
  edFind          = 7,
  edSearchFailed  = 8,
  edReplace       = 9,
  edReplacePrompt = 10;

const
  efCaseSensitive   = 0x0001,
  efWholeWordsOnly  = 0x0002,
  efPromptOnReplace = 0x0004,
  efReplaceAll      = 0x0008,
  efDoReplace       = 0x0010,
  efBackupFiles     = 0x0100;

const
  maxLineLength = 256;

#endif  // __EDIT_COMMAND_CODES

typedef ushort (*TEditorDialog)( int, ... );
ushort defEditorDialog( int dialog, ... );

#if defined( Uses_TIndicator ) && !defined( __TIndicator )
#define __TIndicator

class TRect;

class TIndicator : public TView
{

public:

    TIndicator( const TRect& );

    virtual void draw();
    virtual TPalette& getPalette() const;
    virtual void setState( ushort, Boolean );
    void setValue( const TPoint&, Boolean );
    static /*const*/ char dragFrame;
    static /*const*/ char normalFrame;

protected:

    TPoint location;
    Boolean modified;

private:

//    static const char dragFrame;
//    static const char normalFrame;

    virtual const char *streamableName() const
        { return name; }

protected:

    TIndicator( StreamableInit );

public:

    static const char * const name;
    static TStreamable *build();

};

inline ipstream& operator >> ( ipstream& is, TIndicator& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TIndicator*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TIndicator& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TIndicator* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TIndicator


#if defined( Uses_TEditor ) && !defined( __TEditor )
#define __TEditor

class TRect;
class TScrollBar;
class TIndicator;
class TEvent;

class TEditor : public TView
{

public:

    friend void genRefs();

//    TEditor( const TRect&, TScrollBar *, TScrollBar *, TIndicator *, ushort ); /* XXX */
    TEditor( const TRect&, TScrollBar *, TScrollBar *, TIndicator *, uint ); /* XXX */
    virtual ~TEditor();

    virtual void shutDown();

//    char bufChar( ushort ); /* XXX */
//    ushort bufPtr( ushort ); /* XXX */
    char bufChar( uint ); /* XXX */
    uint bufPtr( uint ); /* XXX */ 
    virtual void changeBounds( const TRect& );
    virtual void convertEvent( TEvent& );
    Boolean cursorVisible();
    void deleteSelect();
    virtual void doneBuffer();
    virtual void draw();
    virtual TPalette& getPalette() const;
    virtual void handleEvent( TEvent& );
    virtual void initBuffer();
//    Boolean insertBuffer( char *, ushort, ushort, Boolean, Boolean ); /* XXX */
    Boolean insertBuffer( char *, uint, uint, Boolean, Boolean ); /* XXX */
    virtual Boolean insertFrom( TEditor * );
//    Boolean insertText( const void *, ushort, Boolean ); /* XXX */
    Boolean insertText( const void *, uint, Boolean ); /* XXX */
    void scrollTo( int, int );
    Boolean search( const char *, ushort );
//    virtual Boolean setBufSize( ushort ); /* XXX */
    virtual Boolean setBufSize( uint ); /* XXX */
    void setCmdState( ushort, Boolean );
//    void setSelect( ushort, ushort, Boolean); /* XXX */
    void setSelect( uint, uint, Boolean); /* XXX */
    virtual void setState( ushort, Boolean );
    void trackCursor( Boolean );
    void undo();
    virtual void updateCommands();
    virtual Boolean valid( ushort );

//    int charPos( ushort, ushort ); /* XXX */
//    ushort charPtr( ushort, int ); /* XXX */
    uint charPos( uint, uint ); /* XXX */
    uint charPtr( uint, uint ); /* XXX */
    Boolean clipCopy();
    void clipCut();
    void clipPaste();
//    void deleteRange( ushort, ushort, Boolean ); /* XXX */
    void deleteRange( uint, uint, Boolean ); /* XXX */
    void doUpdate();
    void doSearchReplace();
//    void drawLines( int, int, ushort ); /* XXX */
//    void formatLine(void *, ushort, int, ushort ); /* XXX */
    void drawLines( int, uint, uint ); /* XXX */
    void formatLine(void *, uint, int, ushort ); /* XXX */
    void find();
//    ushort getMousePtr( TPoint ); /* XXX */
    uint getMousePtr( TPoint ); /* XXX */
    Boolean hasSelection();
    void hideSelect();
    Boolean isClipboard();
//    ushort lineEnd( ushort ); /* XXX */
//    ushort lineMove( ushort, int ); /* XXX */
//    ushort lineStart( ushort ); /* XXX */
    uint lineEnd( uint ); /* XXX */
    uint lineMove( uint, int ); /* XXX */
    uint lineStart( uint ); /* XXX */
    void lock();
    void newLine();
//    ushort nextChar( ushort ); /* XXX */
//    ushort nextLine( ushort ); /* XXX */
//    ushort nextWord( ushort ); /* XXX */
//    ushort prevChar( ushort ); /* XXX */
//    ushort prevLine( ushort ); /* XXX */
//    ushort prevWord( ushort ); /* XXX */
    uint nextChar( uint ); /* XXX */
    uint nextLine( uint ); /* XXX */
    uint nextWord( uint ); /* XXX */
    uint prevChar( uint ); /* XXX */
    uint prevLine( uint ); /* XXX */
    uint prevWord( uint ); /* XXX */
    void replace();
//    void setBufLen( ushort ); /* XXX */
//    void setCurPtr( ushort, uchar ); /* XXX */
    void setBufLen( uint ); /* XXX */
    void setCurPtr( uint, uchar ); /* XXX */
    void startSelect();
    void toggleInsMode();
    void unlock();
    void update( uchar );
    void checkScrollBar( const TEvent&, TScrollBar *, int& );
	 void TEditor::setWidth( int );	// Orlik
	 
	 int widthSize;	//Orlik
    TScrollBar *hScrollBar;
    TScrollBar *vScrollBar;
    TIndicator *indicator;
    char *buffer;
//    ushort bufSize; /* XXX */
//    ushort bufLen; /* XXX */
//    ushort gapLen; /* XXX */
//    ushort selStart; /* XXX */
//    ushort selEnd; /* XXX */
//    ushort curPtr; /* XXX */
    uint bufSize; /* XXX */
    uint bufLen; /* XXX */
    uint gapLen; /* XXX */
    uint selStart; /* XXX */
    uint selEnd; /* XXX */
    uint curPtr; /* XXX */
    TPoint curPos;
    TPoint delta;
    TPoint limit;
    int drawLine;
//    ushort drawPtr; /* XXX */
//    ushort delCount; /* XXX */
//    ushort insCount; /* XXX */
    uint drawPtr; /* XXX */
    uint delCount; /* XXX */
    uint insCount; /* XXX */
    Boolean isValid;
    Boolean canUndo;
    Boolean modified;
    Boolean selecting;
    Boolean overwrite;
    Boolean autoIndent;

    static TEditorDialog editorDialog;
    static ushort editorFlags;
    static char findStr[maxFindStrLen];
    static char replaceStr[maxReplaceStrLen];
    static TEditor * clipboard;
    uchar lockCount;
    uchar updateFlags;
    int keyState;

private:

    virtual const char *streamableName() const
        { return name; }

protected:

    TEditor( StreamableInit );
    virtual void write( opstream& );
    virtual void *read( ipstream& );

public:

    static const char * const name;
    static TStreamable *build();

};

inline ipstream& operator >> ( ipstream& is, TEditor& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TEditor*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TEditor& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TEditor* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TEditor

#if defined( Uses_TMemo ) && !defined( __TMemo )
#define __TMemo

class TEvent;

struct TMemoData
{
    ushort length;
    char buffer[1];
};

class TMemo : public TEditor
{

public:

    TMemo( const TRect&, TScrollBar *, TScrollBar *, TIndicator *, ushort );
    virtual void getData( void *rec );
    virtual void setData( void *rec );
    virtual ushort dataSize();
    virtual TPalette& getPalette() const;
    virtual void handleEvent( TEvent& );

private:

    virtual const char *streamableName() const
        { return name; }

protected:

    TMemo( StreamableInit );
    virtual void write( opstream& );
    virtual void *read( ipstream& );

public:

    static const char * const name;
    static TStreamable *build();

};

inline ipstream& operator >> ( ipstream& is, TMemo& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TMemo*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TMemo& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TMemo* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TMemo


#if defined( Uses_TFileEditor ) && !defined( __TFileEditor )
#define __TFileEditor

#include <limits.h>

class TRect;
class TScrollBar;
class TIndicator;
class TEvent;

class TFileEditor : public TEditor
{

public:

    char fileName[PATH_MAX];
    TFileEditor( const TRect&,
                 TScrollBar *,
                 TScrollBar *,
                 TIndicator *,
                 const char *
               );
    virtual void doneBuffer();
    virtual void handleEvent( TEvent& );
    virtual void initBuffer();
    Boolean loadFile();
    Boolean save();
    Boolean saveAs();
    Boolean saveFile();
//    virtual Boolean setBufSize( ushort ); /* XXX */
    virtual Boolean setBufSize( uint ); /* XXX */
    virtual void shutDown();
    virtual void updateCommands();
    virtual Boolean valid( ushort );

private:

    static const char * backupExt;

    virtual const char *streamableName() const
        { return name; }

protected:

    TFileEditor( StreamableInit );
    virtual void write( opstream& );
    virtual void *read( ipstream& );

public:

    static const char * const name;
    static TStreamable *build();

};

inline ipstream& operator >> ( ipstream& is, TFileEditor& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TFileEditor*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TFileEditor& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TFileEditor* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TFileEditor


#if defined( Uses_TEditWindow ) && !defined( __TEditWindow )
#define __TEditWindow

class TFileEditor;

class TEditWindow : public TWindow
{

public:

    TEditWindow( const TRect&, const char *, int );
    virtual void close();
    virtual const char *getTitle( short );
    virtual void handleEvent( TEvent& );
    virtual void sizeLimits( TPoint& min, TPoint& max );

    TFileEditor *editor;

private:

    static const char * clipboardTitle;
    static const char * untitled;

    virtual const char *streamableName() const
        { return name; }

protected:

    TEditWindow( StreamableInit );
    virtual void write( opstream& );
    virtual void *read( ipstream& );

public:

    static const char * const name;
    static TStreamable *build();

};

inline ipstream& operator >> ( ipstream& is, TEditWindow& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TEditWindow*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TEditWindow& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TEditWindow* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TEditWindow


#if defined( Uses_TFindDialogRec ) && !defined( __TFindDialogRec )
#define __TFindDialogRec

#include <string.h>

struct TFindDialogRec
{
    TFindDialogRec( const char *str, ushort flgs )
        {
        strcpy( find, str );
        options = flgs;
        }
    char find[maxFindStrLen];
    ushort options;
};

#endif  // Uses_TFindDialogRec

#if defined( Uses_TReplaceDialogRec ) && !defined( __TReplaceDialogRec )
#define __TReplaceDialogRec

#include <string.h>

struct TReplaceDialogRec
{
    TReplaceDialogRec( const char *str, const char *rep, ushort flgs )
        {
        strcpy( find, str );
        strcpy( replace, rep );
        options = flgs;
        }
    char find[maxFindStrLen];
    char replace[maxReplaceStrLen];
    ushort options;
};

#endif  // Uses_TReplaceDialogRec
