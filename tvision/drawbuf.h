/*
 * drawbuf.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <ssigala@globalnet.it>
 */

#if defined( Uses_TDrawBuffer ) && !defined( __TDrawBuffer )
#define __TDrawBuffer

class TDrawBuffer
{

    friend class TFrame;
    friend class TView;
    friend void genRefs();

public:

    void moveChar( ushort indent, char c, ushort attr, ushort count );
    void moveStr( ushort indent, const char *str, ushort attrs );
    void moveCStr( ushort indent, const char *str, ushort attrs );
    void moveBuf( ushort indent, const void *source,
                  ushort attr, ushort count );

    void putAttribute( ushort indent, ushort attr );
    void putChar( ushort indent, ushort c );

protected:

    ushort data[maxViewWidth];

};

/*
 * SS: some non-portable code changed.
 */
#ifdef __FreeBSD__
#include <machine/endian.h>
#else
#include <endian.h>
#endif

#if (BYTE_ORDER == LITTLE_ENDIAN)
	#define loByte(w)    (((uchar *)&w)[0])
	#define hiByte(w)    (((uchar *)&w)[1])
#elif (BYTE_ORDER == BIG_ENDIAN)
	#define loByte(w)    (((uchar *)&w)[1])
	#define hiByte(w)    (((uchar *)&w)[0])
#else
	#error architecture not supported by this library
#endif

inline void TDrawBuffer::putAttribute( ushort indent, ushort attr )
{
    hiByte(data[indent]) = uchar(attr);
/* data[indent] = (data[indent] & 0x00ff) | ((attr & 0xff) << 8); */
}

inline void TDrawBuffer::putChar( ushort indent, ushort c )
{
    loByte(data[indent]) = uchar(c);
/* data[indent] = (data[indent] & 0xff00) | (c & 0xff); */
}

#endif  // Uses_TDrawBuffer
