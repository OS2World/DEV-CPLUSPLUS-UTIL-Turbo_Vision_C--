/*
 * stddlg.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <ssigala@globalnet.it>
 */

#if !defined( __FILE_CMDS )
#define __FILE_CMDS

const

//  Commands

    cmFileOpen    = 1001,   // Returned from TFileDialog when Open pressed
    cmFileReplace = 1002,   // Returned from TFileDialog when Replace pressed
    cmFileClear   = 1003,   // Returned from TFileDialog when Clear pressed
    cmFileInit    = 1004,   // Used by TFileDialog internally
    cmChangeDir   = 1005,   //
    cmRevert      = 1006,   // Used by TChDirDialog internally
#ifndef __UNPATCHED
    cmDirSelection= 1007,   //!! New event - Used by TChDirDialog internally
#endif

//  Messages

    cmFileFocused = 102,    // A new file was focused in the TFileList
    cmFileDoubleClicked     // A file was selected in the TFileList
            = 103;

#endif  // __FILE_CMDS

#if defined( Uses_TSearchRec ) && !defined( __TSearchRec )
#define __TSearchRec

#include <limits.h>

/* SS: dos stuff */

#define FA_ARCH		0x01
#define FA_DIREC	0x02
#define FA_RDONLY	0x04

struct  ftime   {
    unsigned    ft_tsec  : 5;   /* Two second interval */
    unsigned    ft_min   : 6;   /* Minutes */
    unsigned    ft_hour  : 5;   /* Hours */
    unsigned    ft_day   : 5;   /* Days */
    unsigned    ft_month : 4;   /* Months */
    unsigned    ft_year  : 7;   /* Year */
};

struct TSearchRec
{
    uchar attr;
    long time;
    long size;
    char name[PATH_MAX];
};

#endif  // Uses_TSearchRec

#if defined( Uses_TFileInputLine ) && !defined( __TFileInputLine )
#define __TFileInputLine

class TRect;
class TEvent;

class TFileInputLine : public TInputLine
{

public:

    TFileInputLine( const TRect& bounds, short aMaxLen );

    virtual void handleEvent( TEvent& event );

private:

    virtual const char *streamableName() const
        { return name; }

protected:

    TFileInputLine( StreamableInit );

public:

    static const char * const name;
    static TStreamable *build();

};

inline ipstream& operator >> ( ipstream& is, TFileInputLine& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TFileInputLine*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TFileInputLine& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TFileInputLine* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TFileInputLine

#if defined( Uses_TFileCollection ) && !defined( __TFileCollection )
#define __TFileCollection

class TSearchRec;

class TFileCollection: public TSortedCollection
{

public:

    TFileCollection( ccIndex aLimit, ccIndex aDelta) :
        TSortedCollection( aLimit, aDelta ) {}

    TSearchRec *at( ccIndex index )
        { return (TSearchRec *)TSortedCollection::at( index ); }
    virtual ccIndex indexOf( TSearchRec *item )
        { return TSortedCollection::indexOf( item ); }

    void remove( TSearchRec *item )
        { TSortedCollection::remove( item ); }
    void free( TSearchRec *item )
        { TSortedCollection::free( item ); }
    void atInsert( ccIndex index, TSearchRec *item )
        { TSortedCollection::atInsert( index, item ); }
    void atPut( ccIndex index, TSearchRec *item )
        { TSortedCollection::atPut( index, item ); }
    virtual ccIndex insert( TSearchRec *item )
        { return TSortedCollection::insert( item ); }

    TSearchRec *firstThat( ccTestFunc Test, void *arg );
    TSearchRec *lastThat( ccTestFunc Test, void *arg );

private:

    virtual void freeItem( void *item )
        { delete (TSearchRec *)item; }

    virtual int compare( void *key1, void *key2 );

    virtual const char *streamableName() const
        { return name; }

    virtual void *readItem( ipstream& );
    virtual void writeItem( void *, opstream& );

protected:

    TFileCollection( StreamableInit ) : TSortedCollection ( streamableInit ) {}

public:

    static const char * const name;
    static TStreamable *build();

};

inline ipstream& operator >> ( ipstream& is, TFileCollection& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TFileCollection*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TFileCollection& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TFileCollection* cl )
    { return os << (TStreamable *)cl; }

inline TSearchRec *TFileCollection::firstThat( ccTestFunc func, void *arg )
{
    return (TSearchRec *)TSortedCollection::firstThat( ccTestFunc(func), arg );
}

inline TSearchRec *TFileCollection::lastThat( ccTestFunc func, void *arg )
{
    return (TSearchRec *)TSortedCollection::lastThat( ccTestFunc(func), arg );
}

#endif  // Uses_TFileCollection


#if defined( Uses_TSortedListBox ) && !defined( __TSortedListBox )
#define __TSortedListBox

class TRect;
class TScrollBar;
class TEvent;
class TSortedCollection;

class TSortedListBox: public TListBox
{

public:

    TSortedListBox( const TRect& bounds,
                    ushort aNumCols,
                    TScrollBar *aScrollBar
                  );

    virtual void handleEvent( TEvent& event );
    void newList( TSortedCollection *aList );
#ifndef __UNPATCHED
    void *read( ipstream& is );
#endif

    TSortedCollection *list();

protected:

    uchar shiftState;

private:

    virtual void *getKey( const char *s );

    short searchPos;

    virtual const char *streamableName() const
        { return name; }

protected:

    TSortedListBox( StreamableInit ) : TListBox ( streamableInit ) {}

public:

    static const char * const name;
    static TStreamable *build();

};

inline ipstream& operator >> ( ipstream& is, TSortedListBox& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TSortedListBox*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TSortedListBox& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TSortedListBox* cl )
    { return os << (TStreamable *)cl; }

inline TSortedCollection *TSortedListBox::list()
{
    return (TSortedCollection *)TListBox::list();
}

#endif  // Uses_TSortedListBox

#if defined( Uses_TFileList ) && !defined( __TFileList )
#define __TFileList

class TRect;
class TScrollBar;
class TEvent;

class TFileList : public TSortedListBox
{

public:

    TFileList( const TRect& bounds,
               TScrollBar *aScrollBar
             );
    ~TFileList();

    virtual void focusItem( short item );
    virtual void selectItem( short item );
    virtual void getText( char *dest, short item, short maxLen );
    void newList( TFileCollection *aList );
    void readDirectory( const char *dir, const char *wildCard );
    void readDirectory( const char *wildCard );

    virtual ushort dataSize();
    virtual void getData( void *rec );
    virtual void setData( void *rec );

    TFileCollection *list();

private:

    virtual void *getKey( const char *s );

    static const char * tooManyFiles;

    virtual const char *streamableName() const
        { return name; }

protected:

    TFileList( StreamableInit ) : TSortedListBox ( streamableInit ) {}

public:

    static const char * const name;
    static TStreamable *build();

};

inline ipstream& operator >> ( ipstream& is, TFileList& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TFileList*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TFileList& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TFileList* cl )
    { return os << (TStreamable *)cl; }

inline void TFileList::newList( TFileCollection *f )
{
    TSortedListBox::newList( f );
}

inline TFileCollection *TFileList::list()
{
    return (TFileCollection *)TSortedListBox::list();
}

#endif  // Uses_TFileList


#if defined( Uses_TFileInfoPane ) && !defined( __TFileInfoPane )
#define __TFileInfoPane

class TRect;
class TEvent;

class TFileInfoPane : public TView
{

public:

    TFileInfoPane( const TRect& bounds );

    virtual void draw();
    virtual TPalette& getPalette() const;
    virtual void handleEvent( TEvent& event );

private:

    TSearchRec file_block;

    static const char * const months[13];
    static const char * pmText;
    static const char * amText;

    virtual const char *streamableName() const
        { return name; }

protected:

    TFileInfoPane( StreamableInit ) : TView ( streamableInit ) {}

public:

    static const char * const name;
    static TStreamable *build();

};

inline ipstream& operator >> ( ipstream& is, TFileInfoPane& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TFileInfoPane*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TFileInfoPane& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TFileInfoPane* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TFileInfoPane

#if defined( Uses_TFileDialog ) && !defined( __TFileDialog )
#define __TFileDialog

const
    fdOKButton      = 0x0001,      // Put an OK button in the dialog
    fdOpenButton    = 0x0002,      // Put an Open button in the dialog
    fdReplaceButton = 0x0004,      // Put a Replace button in the dialog
    fdClearButton   = 0x0008,      // Put a Clear button in the dialog
    fdHelpButton    = 0x0010,      // Put a Help button in the dialog
    fdNoLoadDir     = 0x0100;      // Do not load the current directory
                                   // contents into the dialog at Init.
                                   // This means you intend to change the
                                   // WildCard by using SetData or store
                                   // the dialog on a stream.

class TEvent;
class TFileInputLine;
class TFileList;

class TFileDialog : public TDialog
{

public:

    TFileDialog( const char *aWildCard, const char *aTitle,
                 const char *inputName, ushort aOptions, uchar histId );
    ~TFileDialog();

    virtual void getData( void *rec );
    void getFileName( char *s );
    virtual void handleEvent( TEvent& event );
    virtual void setData( void *rec );
    virtual Boolean valid( ushort command );
    virtual void shutDown();

    TFileInputLine *fileName;
    TFileList *fileList;
    char wildCard[PATH_MAX];
    const char *directory;

private:

    void readDirectory();

    Boolean checkDirectory( const char * );

    static const char * filesText;
    static const char * openText;
    static const char * okText;
    static const char * replaceText;
    static const char * clearText;
    static const char * cancelText;
    static const char * helpText;
    static const char * invalidDriveText;
    static const char * invalidFileText;

    virtual const char *streamableName() const
        { return name; }

protected:

    TFileDialog( StreamableInit ) : TDialog ( streamableInit ),
        TWindowInit( TFileDialog::initFrame ) {}
    virtual void write( opstream& );
    virtual void *read( ipstream& );

public:

    static const char * const name;
    static TStreamable *build();

};

inline ipstream& operator >> ( ipstream& is, TFileDialog& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TFileDialog*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TFileDialog& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TFileDialog* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TFileDialog


#if defined( Uses_TDirEntry ) && !defined( __TDirEntry )
#define __TDirEntry

class TDirEntry
{

public:

    TDirEntry( const char *, const char * );
    ~TDirEntry();
    char *dir() { return directory; }
    char *text() { return displayText; }

private:

    char *displayText;
    char *directory;

};

inline TDirEntry::TDirEntry( const char *txt, const char *dir ) :
    displayText( newStr( txt ) ), directory( newStr( dir ) )
{
}

inline TDirEntry::~TDirEntry()
{
    delete displayText;
    delete directory;
}

#endif  // Uses_TDirEntry

#if defined( Uses_TDirCollection ) && !defined( __TDirCollection )
#define __TDirCollection

class TDirEntry;

class TDirCollection : public TCollection
{

public:

    TDirCollection( ccIndex aLimit, ccIndex aDelta) :
        TCollection( aLimit, aDelta ) {}

    TDirEntry *at( ccIndex index )
        { return (TDirEntry *)TCollection::at( index );}
    virtual ccIndex indexOf( TDirEntry *item )
        { return TCollection::indexOf( item ); }

    void remove( TDirEntry *item )
        { TCollection::remove( item ); }
    void free( TDirEntry *item )
        { TCollection::free( item ); }
    void atInsert( ccIndex index, TDirEntry *item )
        { TCollection::atInsert( index, item ); }
    void atPut( ccIndex index, TDirEntry *item )
        { TCollection::atPut( index, item ); }
    virtual ccIndex insert( TDirEntry *item )
        { return TCollection::insert( item ); }

    TDirEntry *firstThat( ccTestFunc Test, void *arg );
    TDirEntry *lastThat( ccTestFunc Test, void *arg );

private:

    virtual void freeItem( void *item )
        { delete (TDirEntry *)item; }

    virtual const char *streamableName() const
        { return name; }
    virtual void *readItem( ipstream& );
    virtual void writeItem( void *, opstream& );

protected:

    TDirCollection( StreamableInit ) : TCollection ( streamableInit ) {}

public:

    static const char * const name;
    static TStreamable *build();

};

inline ipstream& operator >> ( ipstream& is, TDirCollection& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TDirCollection*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TDirCollection& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TDirCollection* cl )
    { return os << (TStreamable *)cl; }

inline TDirEntry *TDirCollection::firstThat( ccTestFunc func, void *arg )
{
    return (TDirEntry *)TCollection::firstThat( ccTestFunc(func), arg );
}

inline TDirEntry *TDirCollection::lastThat( ccTestFunc func, void *arg )
{
    return (TDirEntry *)TCollection::lastThat( ccTestFunc(func), arg );
}

#endif  // Uses_TDirCollection


#if defined( Uses_TDirListBox ) && !defined( __TDirListBox )
#define __TDirListBox

class TRect;
class TScrollBar;
class TEvent;
class TDirCollection;

class TDirListBox : public TListBox
{

public:

    TDirListBox( const TRect& bounds, TScrollBar *aScrollBar );
    ~TDirListBox();

    virtual void getText( char *, short, short );
//    virtual void handleEvent( TEvent& );
    virtual Boolean isSelected( short );
    virtual void selectItem( short item );
    void newDirectory( const char * );
    virtual void setState( ushort aState, Boolean enable );

    TDirCollection *list();
    static const char * pathDir;
    static const char * firstDir;
    static const char * middleDir;
    static const char * lastDir;
    static const char * graphics;

private:

    void showDrives( TDirCollection * );
    void showDirs( TDirCollection * );

    char dir[PATH_MAX];
    ushort cur;

//    static const char * pathDir;
//    static const char * firstDir;
//    static const char * middleDir;
//    static const char * lastDir;
    static const char * drives;
//    static const char * graphics;

    virtual const char *streamableName() const
        { return name; }

protected:

    TDirListBox( StreamableInit ): TListBox( streamableInit ) {}

public:

    static const char * const name;
    static TStreamable *build();

};

inline ipstream& operator >> ( ipstream& is, TDirListBox& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TDirListBox*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TDirListBox& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TDirListBox* cl )
    { return os << (TStreamable *)cl; }

inline TDirCollection *TDirListBox::list()
{
    return (TDirCollection *)TListBox::list();
}

#endif  // Uses_TDirListBox

#if defined( Uses_TChDirDialog ) && !defined( __TChDirDialog )
#define __TChDirDialog

const
    cdNormal     = 0x0000, // Option to use dialog immediately
    cdNoLoadDir  = 0x0001, // Option to init the dialog to store on a stream
    cdHelpButton = 0x0002; // Put a help button in the dialog

class TEvent;
class TInputLine;
class TDirListBox;
class TButton;

class TChDirDialog : public TDialog
{

public:

    friend class TDirListBox;

    TChDirDialog( ushort aOptions, ushort histId );
    virtual ushort dataSize();
    virtual void getData( void *rec );
    virtual void handleEvent( TEvent& );
    virtual void setData( void *rec );
    virtual Boolean valid( ushort );
    virtual void shutDown();

private:

    void setUpDialog();

    TInputLine *dirInput;
    TDirListBox *dirList;
    TButton *okButton;
    TButton *chDirButton;

    static const char * changeDirTitle;
    static const char * dirNameText;
    static const char * dirTreeText;
    static const char * okText;
    static const char * chdirText;
    static const char * revertText;
    static const char * helpText;
    static const char * drivesText;
    static const char * invalidText;

    virtual const char *streamableName() const
        { return name; }

protected:

    TChDirDialog( StreamableInit ) : TDialog( streamableInit ),
        TWindowInit( TChDirDialog::initFrame ) {}
    virtual void write( opstream& );
    virtual void *read( ipstream& );

public:

    static const char * const name;
    static TStreamable *build();

};

inline ipstream& operator >> ( ipstream& is, TChDirDialog& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TChDirDialog*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TChDirDialog& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TChDirDialog* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TChDirDialog
