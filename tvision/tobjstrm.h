/*
 * tobjstrm.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <ssigala@globalnet.it>
 */

typedef unsigned P_id_type;

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class TStreamable                                                     */
/*                                                                         */
/*   This is the base class for all storable objects.  It provides         */
/*   three member functions, streamableName(), read(), and write(), which  */
/*   must be overridden in every derived class.                            */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if !defined( __fLink_def )
#define __fLink_def
struct fLink
{
    fLink *f;
    class TStreamableClass *t;
};
#endif

#define __link( s )             \
  extern TStreamableClass s;    \
  static fLink force ## s =     \
    { (fLink *)&force ## s, (TStreamableClass *)&s };

#if defined( Uses_TStreamable ) && !defined( __TStreamable )
#define __TStreamable

class TStreamable
{

    friend class opstream;
    friend class ipstream;

private:

    virtual const char *streamableName() const = 0;

protected:

    virtual void *read( ipstream& ) = 0;
    virtual void write( opstream& ) = 0;

};

#endif  // Uses_TStreamable

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class TStreamableClass                                                */
/*                                                                         */
/*   Used internally by TStreamableTypes and pstream.                      */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_TStreamableClass ) && !defined( __TStreamableClass )
#define __TStreamableClass

#include <limits.h>

const P_id_type P_id_notFound = UINT_MAX;

typedef TStreamable *(*BUILDER)();

#define __DELTA( d ) ((int)(TStreamable*)(d*)1-1 )

class TStreamableClass
{

    friend TStreamableTypes;
    friend opstream;
    friend ipstream;

public:

    TStreamableClass( const char *n, BUILDER b, int d );

private:

    const char *name;
    BUILDER build;
    int delta;

};

#endif  // Uses_TStreamableClass

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class TStreamableTypes                                                */
/*                                                                         */
/*   Maintains a database of all registered types in the application.      */
/*   Used by opstream and ipstream to find the functions to read and       */
/*   write objects.                                                        */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_TStreamableTypes ) && !defined( __TStreamableTypes )
#define __TStreamableTypes

class TStreamableTypes : private TNSSortedCollection
{

public:

    TStreamableTypes();
    ~TStreamableTypes();

    void registerType( const TStreamableClass * );
    const TStreamableClass *lookup( const char * );

    void *operator new( size_t sz ) { return ::operator new( sz ); }
    void *operator new( size_t, void * );

private:

    virtual void *keyOf( void * );
    int compare( void *, void * );

};

#endif  // Uses_TStreamableTypes

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class TPWrittenObjects                                                */
/*                                                                         */
/*   Maintains a database of all objects that have been written to the     */
/*   current object stream.                                                */
/*                                                                         */
/*   Used by opstream when it writes a pointer onto a stream to determine  */
/*   whether the object pointed to has already been written to the stream. */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_TPWrittenObjects ) && !defined( __TPWrittenObjects )
#define __TPWrittenObjects

class TPWrittenObjects : public TNSSortedCollection
{

    friend opstream;

public:

    void removeAll() { curId = 0; TNSSortedCollection::freeAll(); }

private:

    TPWrittenObjects();
    ~TPWrittenObjects();

    void registerObject( const void *adr );
    P_id_type find( const void *adr );

    void *keyOf( void * );
    int compare( void *, void * );

    P_id_type curId;

};

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class TPWObj                                                          */
/*                                                                         */
/*   Used internally by TPWrittenObjects.                                  */
/*                                                                         */
/* ------------------------------------------------------------------------*/

class TPWObj
{

    friend TPWrittenObjects;

private:

    TPWObj( const void *adr, P_id_type id );

    const void *address;
    P_id_type ident;

};

#endif  // Uses_TPWrittenObjects

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class TPReadObjects                                                   */
/*                                                                         */
/*   Maintains a database of all objects that have been read from the      */
/*   current persistent stream.                                            */
/*                                                                         */
/*   Used by ipstream when it reads a pointer from a stream to determine   */
/*   the address of the object being referred to.                          */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_TPReadObjects ) && !defined( __TPReadObjects )
#define __TPReadObjects

class TPReadObjects : public TNSCollection
{

    friend ipstream;

public:

    void removeAll() { curId = 0; TNSCollection::removeAll(); }

private:

    TPReadObjects();
    ~TPReadObjects();

    void registerObject( const void *adr );
    const void *find( P_id_type id );

    P_id_type curId;

};

#endif  // Uses_TPReadObjects

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class pstream                                                         */
/*                                                                         */
/*   Base class for handling streamable objects.                           */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_pstream ) && !defined( __pstream )
#define __pstream

#include <iostream.h>

class TStreamableTypes;

class pstream
{

    friend TStreamableTypes;

public:

    enum StreamableError { peNotRegistered, peInvalidType };
    enum PointerTypes { ptNull, ptIndexed, ptObject };

    pstream( streambuf * );
    virtual ~pstream();

    int rdstate() const;
    int eof() const;
    int fail() const;
    int bad() const;
    int good() const;
    void clear( int = 0 );
    operator void *() const;
    int operator ! () const;

    streambuf * rdbuf() const;

    static void initTypes();

    void error( StreamableError );
    void error( StreamableError, const TStreamable& );
    static void registerType( TStreamableClass *ts );

protected:

    pstream();

    streambuf *bp;
    int state;

    void init( streambuf * );
    void setstate( int );

    static TStreamableTypes * types;

};

#endif  // Uses_pstream

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class ipstream                                                        */
/*                                                                         */
/*   Base class for reading streamable objects                             */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_ipstream ) && !defined( __ipstream )
#define __ipstream

#include <iostream.h>

class TStreamableClass;

class ipstream : virtual public pstream
{

public:

    ipstream( streambuf * );
    ~ipstream();

    streampos tellg();
    ipstream& seekg( streampos );
    ipstream& seekg( streamoff, ios::seek_dir );

    uchar readByte();
    void readBytes( void *, size_t );
    ushort readWord();
    char * readString();
    char * readString( char *, unsigned );

    friend ipstream& operator >> ( ipstream&, char& );
    friend ipstream& operator >> ( ipstream&, signed char& );
    friend ipstream& operator >> ( ipstream&, unsigned char& );
    friend ipstream& operator >> ( ipstream&, signed short& );
    friend ipstream& operator >> ( ipstream&, unsigned short& );
    friend ipstream& operator >> ( ipstream&, signed int& );
    friend ipstream& operator >> ( ipstream&, unsigned int& );
    friend ipstream& operator >> ( ipstream&, signed long& );
    friend ipstream& operator >> ( ipstream&, unsigned long& );
    friend ipstream& operator >> ( ipstream&, float& );
    friend ipstream& operator >> ( ipstream&, double& );
    friend ipstream& operator >> ( ipstream&, long double& );

    friend ipstream& operator >> ( ipstream&, TStreamable& );
    friend ipstream& operator >> ( ipstream&, void *& );

protected:

    ipstream();

    const TStreamableClass * readPrefix();
    void * readData( const TStreamableClass *,
                                        TStreamable * );
    void readSuffix();

    const void * find( P_id_type );
    void registerObject( const void *adr );

private:

    TPReadObjects objs;

};

#endif  // Uses_ipstream

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class opstream                                                        */
/*                                                                         */
/*   Base class for writing streamable objects                             */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_opstream ) && !defined( __opstream )
#define __opstream

#include <iostream.h>

class TStreamableClass;

class opstream : virtual public pstream
{

public:

    opstream( streambuf * );
    ~opstream();

    streampos tellp();
    opstream& seekp( streampos );
    opstream& seekp( streamoff, ios::seek_dir );
    opstream& flush();

    void writeByte( uchar );
    void writeBytes( const void *, size_t );
    void writeWord( ushort );
    void writeString( const char * );

    friend opstream& operator << ( opstream&, char );
    friend opstream& operator << ( opstream&, signed char );
    friend opstream& operator << ( opstream&, unsigned char );
    friend opstream& operator << ( opstream&, signed short );
    friend opstream& operator << ( opstream&, unsigned short );
    friend opstream& operator << ( opstream&, signed int );
    friend opstream& operator << ( opstream&, unsigned int );
    friend opstream& operator << ( opstream&, signed long );
    friend opstream& operator << ( opstream&, unsigned long );
    friend opstream& operator << ( opstream&, float );
    friend opstream& operator << ( opstream&, double );
    friend opstream& operator << ( opstream&, long double );

    friend opstream& operator << ( opstream&, TStreamable& );
    friend opstream& operator << ( opstream&, TStreamable * );

protected:

    opstream();

    void writePrefix( const TStreamable& );
    void writeData( TStreamable& );
    void writeSuffix( const TStreamable& );

    P_id_type find( const void *adr );
    void registerObject( const void *adr );

private:

    TPWrittenObjects *objs;

};

#endif  // Uses_opstream

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class iopstream                                                       */
/*                                                                         */
/*   Base class for reading and writing streamable objects                 */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_iopstream ) && !defined( __iopstream )
#define __iopstream

#include <iostream.h>

class iopstream : public ipstream, public opstream
{

public:

    iopstream( streambuf * );
    ~iopstream();

protected:

    iopstream();

};

#endif  // Uses_iopstream

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class fpbase                                                          */
/*                                                                         */
/*   Base class for handling streamable objects on file streams            */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_fpbase ) && !defined( __fpbase )
#define __fpbase

#include <fstream.h>

class fpbase : virtual public pstream
{

public:

    fpbase();
    fpbase( const char *, int, int = filebuf::openprot );
    fpbase( int );
    fpbase( int, char *, int );
    ~fpbase();

    void open( const char *, int, int = filebuf::openprot );
    void attach( int );
    void close();
    void setbuf( char *, int );
    filebuf * rdbuf();

private:

    filebuf buf;

};

#endif  // Uses_fpbase

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class ifpstream                                                       */
/*                                                                         */
/*   Base class for reading streamable objects from file streams           */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_ifpstream ) && !defined( __ifpstream )
#define __ifpstream

#include <iostream.h>

class ifpstream : public fpbase, public ipstream
{

public:

    ifpstream();
    ifpstream( const char *,
                      int = ios::in,
                      int = filebuf::openprot
                    );
    ifpstream( int );
    ifpstream( int, char *, int );
    ~ifpstream();

    filebuf * rdbuf();
    void open( const char *,
                      int = ios::in,
                      int = filebuf::openprot
                    );

};

#endif  // Uses_ifpstream

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class ofpstream                                                       */
/*                                                                         */
/*   Base class for writing streamable objects to file streams             */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_ofpstream ) && !defined( __ofpstream )
#define __ofpstream

#include <iostream.h>

class ofpstream : public fpbase, public opstream
{

public:

    ofpstream();
    ofpstream( const char *,
                      int = ios::out,
                      int = filebuf::openprot
                    );
    ofpstream( int );
    ofpstream( int, char *, int );
    ~ofpstream();

    filebuf * rdbuf();
    void open( const char *,
                      int = ios::out,
                      int = filebuf::openprot
                    );

};

#endif  // Uses_ofpstream

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class fpstream                                                        */
/*                                                                         */
/*   Base class for reading and writing streamable objects to              */
/*   bidirectional file streams                                            */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_fpstream ) && !defined( __fpstream )
#define __fpstream

#include <iostream.h>

class fpstream : public fpbase, public iopstream
{

public:

    fpstream();
    fpstream( const char *, int, int = filebuf::openprot );
    fpstream( int );
    fpstream( int, char *, int );
    ~fpstream();

    filebuf * rdbuf();
    void open( const char *, int, int = filebuf::openprot );

};


#endif  // Uses_fpstream
