/*
 * system.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <ssigala@globalnet.it>
 */

#if !defined( __EVENT_CODES )
#define __EVENT_CODES

/* Event codes */

const evMouseDown = 0x0001;
const evMouseUp   = 0x0002;
const evMouseMove = 0x0004;
const evMouseAuto = 0x0008;
const evKeyDown   = 0x0010;
const evCommand   = 0x0100;
const evBroadcast = 0x0200;

/* Event masks */

const evNothing   = 0x0000;
const evMouse     = 0x000f;
const evKeyboard  = 0x0010;
const evMessage   = 0xFF00;

/* Mouse button state masks */

const mbLeftButton  = 0x01;
const mbRightButton = 0x02;

/* Mouse event flags */

const meMouseMoved = 0x01;
const meDoubleClick = 0x02;

#endif  // __EVENT_CODES


#if defined( Uses_TEvent ) && !defined( __TEvent )
#define __TEvent

struct MouseEventType
{
    TPoint where;
    ulong eventFlags;           // Replacement for doubleClick.
    ulong controlKeyState;
    uchar buttons;
};

/*
 * SS: some non-portable code changed.
 */
#ifdef __FreeBSD__
#include <machine/endian.h>
#else
#include <endian.h>
#endif

struct CharScanType
{
#if (BYTE_ORDER == LITTLE_ENDIAN)
    uchar charCode;
    uchar scanCode;
#elif (BYTE_ORDER == BIG_ENDIAN)
    uchar scanCode;
    uchar charCode;
#else
    #error architecture not supported by this library
#endif
};

struct KeyDownEvent
{
    union
        {
        ushort keyCode;
        CharScanType charScan;
        };
    ulong controlKeyState;
};

struct MessageEvent
{
    ushort command;
    union
        {
        void *infoPtr;
        long infoLong;
        ushort infoWord;
        short infoInt;
        uchar infoByte;
        char infoChar;
        };
};

struct TEvent
{

    ushort what;
    union
    {
        MouseEventType mouse;
        KeyDownEvent keyDown;
        MessageEvent message;
    };

};

#endif  // Uses_TEvent

#if defined( Uses_TEventQueue ) && !defined( __TEventQueue )
#define __TEventQueue

class TEventQueue
{
public:
    static ushort doubleDelay;
    static Boolean mouseReverse;
};

#endif  // Uses_TEventQueue

#if defined( Uses_TScreen ) && !defined( __TScreen )
#define __TScreen

class TDisplay
{
public:
	enum videoModes
	{
		smBW80		= 0x0002,
		smCO80		= 0x0003,
		smMono		= 0x0007,
		smFont8x8	= 0x0100
        };
};

/*
 * Platform-dependent functions removed.
 * Date: Mon Feb  3 13:19:06 CET 1997
 */
class TScreen: public TDisplay
{
public:
	TScreen();
	~TScreen();
	static void getEvent(TEvent &event);
	static void makeBeep();
	static void putEvent(TEvent &event);
	static void resume();
	static void suspend();
	static void drawCursor(int show);
	static void drawMouse(int show);
	static void moveCursor(int x, int y);
	static void writeRow(int dst, ushort *src, int len);

	/* public data */

	static ushort screenMode;
	static uchar screenWidth;
	static uchar screenHeight;
	static ushort *screenBuffer;
};

#endif  // Uses_TScreen
