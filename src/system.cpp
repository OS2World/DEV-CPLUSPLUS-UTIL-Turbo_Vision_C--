#define INCL_SUB
#define INCL_MOU
#define INCL_DOSPROCESS
#include <os2.h>

#define Uses_TButton
#define Uses_TColorSelector
#define Uses_TDeskTop
#define Uses_TDirListBox
#define Uses_TDrawBuffer
#define Uses_TEvent
#define Uses_TEventQueue
#define Uses_TFrame
#define Uses_THistory
#define Uses_TIndicator
#define Uses_TKeys
#define Uses_TListViewer
#define Uses_TMenuBox
#define Uses_TOutlineViewer
#define Uses_TScreen
#define Uses_TScrollBar
#define Uses_TStatusLine
#include <tvision/tv.h>

#include <conio.h>
#include <fstream.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dir.h>
#include <stdio.h>
#include <sys/stat.h>

/*
 * This is the delay in ms before the first evMouseAuto is generated when the
 * user holds down a mouse button.
 */
#define DELAY_AUTOCLICK_FIRST	400

/*
 * This is the delay in ms between next evMouseAuto events.  Must be greater
 * than DELAY_SIGALRM (see below).
 */
#define DELAY_AUTOCLICK_NEXT	100

/*
 * This is the time limit in ms within button presses are recognized as
 * double-click events.  Used only under FreeBSD because Gpm has its own
 * double-click detecting machanism.
 */
#define DELAY_DOUBLECLICK	300

/*
 * This is the time limit in ms within Esc-key sequences are detected as
 * Alt-letter sequences.  Useful when we can't generate Alt-letter sequences
 * directly.
 */
#define DELAY_ESCAPE		400

/*
 * This is the delay in ms between consecutive SIGALRM signals.  This
 * signal is used to generate evMouseAuto and cmSysWakeup events.
 */
#define DELAY_SIGALRM		100

/*
 * This broadcast event is used to update the StatusLine.
 */
#define DELAY_WAKEUP		200

/* this array stores the corresponding ncurses attribute for each TV color */

static unsigned attributeMap[256];

ushort TEventQueue::doubleDelay = 8;
Boolean TEventQueue::mouseReverse = False;

ushort TScreen::screenMode;
uchar TScreen::screenWidth;
uchar TScreen::screenHeight;
ushort *TScreen::screenBuffer;

static TEvent *evIn;		/* message queue system */
static TEvent *evOut;
static TEvent evQueue[eventQSize];
static TPoint msWhere;		/* mouse coordinates */
static char env[PATH_MAX];	/* value of the TVOPT environment variable */
static int curX, curY;		/* current cursor coordinates */
static int currentTime;		/* current timer value */
static int doRepaint;		/* should redraw the screen ? */
static int doResize;		/* resize screen ? */
static int evLength;		/* number of events in the queue */
static int msOldButtons;	/* mouse button status */
static ofstream xlog;		/* a logging file */
static int msRepaint = 0;

/* key modifiers */

#define MALT		(kbLeftAlt | kbRightAlt)
#define MCTRL		(kbLeftCtrl | kbRightCtrl)
#define MSHIFT		(kbLeftShift | kbRightShift)

/* key types */

#define TALT		0x01		/* alt-letter key */

HKBD KbdHandle;
KBDKEYINFO pkbci;
USHORT rc;
KBDINFO StatData;

typedef struct
{
	int	code;
	int	scancode;
	int	shiftstate;
	int	modifiers;
	short	out;
}
keym_t;

static keym_t keym[] =
{
#include "src/keycodes"
};

static keym_t keymext[] =
{
#include "src/keycodes.ext"
};

// Mouse options

HMOU msHandle;
MOUEVENTINFO msEventInfo;
USHORT ReadType = 0;
static void mouseHandle();
static void msPutEvent(TEvent &event, int buttons, int flags, int what);

/*
 * A simple class which implements a timer.
 */

class Timer
{
	int limit;
public:
	Timer() { limit = -1; }
	int isExpired() { return limit != -1 && currentTime >= limit; }
	int isRunning() { return limit != -1; }
	void start(int timeout) { limit = currentTime + timeout; }
	void stop() { limit = -1; }
};

//static Timer kbEscTimer;	/* time limit to detect Esc-key sequences */
static Timer msAutoTimer;	/* time when generate next cmMouseAuto */
static Timer wakeupTimer;	/* time when generate next cmWakeup */
static Timer msDoubleTimer;	/* time limit to detect double-click events */
static int msFlag;		/* set if there are mouse events */
static int msUseArrow;		/* use arrow pointer */

// Codepage definitions
HAB hab;
ULONG Codepage=866;
ULONG Country=7;

/*
 * GENERAL FUNCTIONS
 */

#define LOG(s) xlog << s << endl

inline int range(int test, int min, int max)
{
	return test < min ? min : test > max ? max : test;
}
/*
 * KEYBOARD FUNCTIONS
 */

/*
 * Builds a keycode from code and modifiers.
 */

int	EscapeState=0;
 
static int kbMapKey(int code, int scancode, int *shiftstate)
{
	keym_t *best = NULL, *p;

	if( EscapeState == 0 ){
	for (p = keym; p < keym + sizeof(keym) / sizeof(keym_t); p++)
	{
		if (p->code == code && p->scancode == scancode && p->shiftstate == *shiftstate )
		{
			best = p;
			*shiftstate = p->modifiers;
			break;
		}
	}
	}
	else
	{
	for (p = keymext; p < keymext + sizeof(keym) / sizeof(keym_t); p++)
	{
		if (p->code == code && p->scancode == scancode && p->shiftstate == *shiftstate )
		{
			best = p;
			*shiftstate = p->modifiers;
			break;
		}
	}
	}
	if (best != NULL) return best->out;	/* keycode found */
	if ( 0 < code <= 255) return code;	/* it is an ascii character */
	return kbNoKey;		/* unknown code */
}

/*
 * Reads a key from the keyboard.
 */
static void kbHandle()
{
	int code, modifiers;
	int kbReady;

NextChar:
	kbReady = 0;
	rc = KbdCharIn( &pkbci, 1, KbdHandle );
	if( rc ) LOG("KbdCharIn error, rc = " << rc);
	
	kbReady |= ( (pkbci.fbStatus & 64) > 0 ) ? 1 : 0;
	if( kbReady == 0 ) return;
	
	modifiers = 0;
	modifiers |= ( (pkbci.fsState & 3) > 0 ) ? kbShift : 0;	// Shift state
	modifiers |= ( (pkbci.fsState & 4) > 0 ) ? kbCtrlShift : 0;	// Ctrl state
	modifiers |= ( (pkbci.fsState & 8) > 0 ) ? kbAltShift : 0;	// Alt state
	
	if( EscapeState == 0 )
	{
		if( pkbci.chChar == 91 && pkbci.chScan == 26 && modifiers == 0 )
		{ EscapeState = 1; goto NextChar; }
	}
	
	if ((code = kbMapKey( pkbci.chChar, pkbci.chScan, &modifiers)) != kbNoKey)
	{
		TEvent event;

		event.what = evKeyDown;
		event.keyDown.keyCode = code;
		event.keyDown.controlKeyState = modifiers;
		TScreen::putEvent(event);
		msRepaint = 1;
	}
	EscapeState = 0;
}

/*
 * CLASS FUNCTIONS
 */

/*
 * TScreen constructor.
 */

TScreen::TScreen()
{
	char *p = getenv("TVLOG");
	if (p != NULL && *p != '\0')
	{
		xlog.open(p);
		LOG("using environment variable TVLOG=" << p);
	}
	else xlog.open("/dev/null");
	env[0] = '\0';
	if ((p = getenv("TVOPT")) != NULL)
	{
		LOG("using environment variable TVOPT=" << p);
		for (char *d = env; *p != '\0'; p++) *d++ = tolower(*p);
	}

	/* acquire screen size */

	screenWidth = 80;
	screenHeight = 25;
	screenHeight--;
	LOG("screen size is " << (int) screenWidth << "x" <<
		(int) screenHeight);
	screenBuffer = new ushort[screenWidth * screenHeight];
	drawCursor( 0 );

	/* internal stuff */

	curX = curY = 0;
	currentTime = doRepaint = doResize = evLength = 0;
	evIn = evOut = &evQueue[0];
	msOldButtons = msWhere.x = msWhere.y = 0;

	/* catch useful signals */

/*
	struct sigaction dfl_handler;

	dfl_handler.sa_handler = sigHandler;
	sigemptyset(&dfl_handler.sa_mask);
	dfl_handler.sa_flags = SA_RESTART;

	sigaction(SIGALRM, &dfl_handler, NULL);
	sigaction(SIGCONT, &dfl_handler, NULL);
	sigaction(SIGINT, &dfl_handler, NULL);
	sigaction(SIGQUIT, &dfl_handler, NULL);
	sigaction(SIGTSTP, &dfl_handler, NULL);
	sigaction(SIGWINCH, &dfl_handler, NULL);
*/

rc = KbdOpen( &KbdHandle );if( rc ) LOG("KbdOpen error");
rc = KbdGetFocus( 0, KbdHandle );if( rc ) LOG("KbdGetFocus error, rc = " << rc);
StatData.cb = sizeof( StatData );StatData.fsMask = 0x4;
StatData.chTurnAround = 0;StatData.fsInterim = 0;
StatData.fsState = 0;
rc = KbdSetStatus( &StatData, KbdHandle );if( rc ) LOG("KbdSetStatus error, rc = " << rc);

// Now Mouse!
rc = MouOpen( NULL, &msHandle );
if( rc )
{
		LOG("MouOpen error, rc = %d" << rc);
}
SCALEFACT sf;
sf.rowScale = 12;
sf.colScale = 6;
rc = MouSetScaleFact( &sf, msHandle );
if( rc )
{
		LOG("MouSetScaleFact error, rc = %d" << rc);
}

msAutoTimer.stop();
msDoubleTimer.stop();
msFlag = msOldButtons = 0;

hab = WinInitialize( 0 );
if( hab == NULLHANDLE )
	LOG( "Translation of national chars is not supported" );
}

/*
 * TScreen destructor.
 */

TScreen::~TScreen()
{
rc = KbdFreeFocus( KbdHandle );
if( rc ) LOG("KbdFreeFocus error, rc = " << rc);
KbdClose( KbdHandle );
delete[] screenBuffer;
LOG("terminated");
if( hab != NULLHANDLE ) WinTerminate( hab );
}

void TScreen::resume()
{
	KbdGetFocus( 0, KbdHandle );
	doRepaint++;
}

void TScreen::suspend()
{
rc = KbdFreeFocus( KbdHandle );
}

/*
 * Gets an event from the queue.
 */

void TScreen::getEvent(TEvent &event)
{
	event.what = evNothing;
	if (doRepaint > 0)
	{
		doRepaint = 0;
		event.message.command = cmSysRepaint;
		event.what = evCommand;
	}
	else if (doResize > 0)
	{
		clrscr();	/* blank the screen */
		doResize = 0;
	}
	else if (evLength > 0)	/* handles pending events */
	{
		evLength--;
		event = *evOut;
		if (++evOut >= &evQueue[eventQSize]) evOut = &evQueue[0];
	}
	else
	{
		mouseHandle();
		kbHandle();
		DosSleep( 5 );
	}
}

/*
 * Generates a beep.
 */

void TScreen::makeBeep()
{
	putch( 7 );
}

/*
 * Puts an event in the queue.  If the queue is full the event will be
 * discarded.
 */

void TScreen::putEvent(TEvent &event)
{
	if (evLength < eventQSize)
	{
		evLength++;
		*evIn = event;
		if (++evIn >= &evQueue[eventQSize]) evIn = &evQueue[0];
	}
}

/*
 * Hides or shows the cursor.
 */

void TScreen::drawCursor(int show)
{
	if (show) _setcursortype( _NORMALCURSOR );	/* cursor normal */
	else _setcursortype( _NOCURSOR );	/* cursor invisible */
}

/*
 * Hides or shows the mouse pointer.
 */

void TScreen::drawMouse(int show)
{
if( show )
rc = MouDrawPtr( msHandle );
else
{
	NOPTRRECT PtrArea;
	PtrArea.row = 0;
	PtrArea.col = 0;
	PtrArea.cRow = screenHeight;
	PtrArea.cCol = screenWidth;
	rc = MouRemovePtr( &PtrArea, msHandle );
}
}

/*
 * Moves the cursor to another place.
 */

void TScreen::moveCursor(int x, int y)
{
//	gotoxy( x+1, y+1 );
	VioSetCurPos( y, x, 0 );
	curX = x;
	curY = y;
}

/*
 * Draws a line of text on the screen.
 */

void TScreen::writeRow(int dst, ushort *src, int len)
{
char buff[4096]; int i;

int x = dst % TScreen::screenWidth;
int y = dst / TScreen::screenWidth;

for( i = 0 ; i < len ; i++ )
	{
	buff[i*2] = (char)(src[i] & 0xff);
	buff[(i*2)+1] = (char)((src[i] & 0xff00) >> 8);
	}
VioWrtCellStr( buff, len*2, y, x, 0);

}

/*
 * Returns the length of a file.
 */

long int filelength(int fd)
{
	struct stat s;
	fstat(fd, &s);
	return s.st_size;
}

/*
 * Expands a path into its directory and file components.
 */

void expandPath(const char *path, char *dir, char *file)
{
	char *tag = strrchr( (char *)path, '/');
	char *drivetag = NULL;
	/* the path is in the form /dir1/dir2/file ? */

	if (tag != NULL)
	{
		strcpy(file, tag + 1);
		strncpy(dir, path, tag - path + 1);
		dir[tag - path + 1] = '\0';
	}
	else
	{
		/* there is only the file name */
		drivetag = strrchr( (char*)path, ':' );
		if( drivetag == NULL )
			{
			strcpy(file, path);
			dir[0] = '\0';
			}
		else
			{
			strcpy(file, ++drivetag);
			strcpy(dir, path);
			dir[drivetag - path] = '/';
			dir[drivetag - path +1] = 0;
			}
	}
}

void fexpand(char *path)
{
	char dir[PATH_MAX];
	char file[PATH_MAX];
	char oldPath[PATH_MAX];

	expandPath(path, dir, file);
//	LOG( "After expandPath: path = " << path << " dir = " << dir << " file = " << file );
	getcwd(oldPath, sizeof(oldPath));
	if( strlen(dir) > 3 )
	if( dir[strlen(dir) -1 ] == '/' ) dir[strlen(dir) -1 ] = 0;
	Slash2backSlash( dir );
	chdir(dir);
//	LOG( "chdir: dir = " << dir );
	getcwd(dir, sizeof(dir));
//	LOG( "getcwd: dir = " << dir );
	backSlash2Slash( dir );
	if( strlen(dir) > 0 )
	if( dir[strlen(dir) -1 ] == '/' ) dir[strlen(dir) -1 ] = 0;
	chdir(oldPath);
	if (strcmp(dir, "/") == 0) sprintf(path, "/%s", file);
	else sprintf(path, "%s/%s", dir, file);
//	LOG( "After fexpand: path = " << path << " dir = " << dir << " file = " << file );
}

void backSlash2Slash( char * path )
{
while( *path )
	{
	if( *path == '\\' ) *path = '/';
	++path;
	}

}

void Slash2backSlash( char * path )
{
while( *path )
	{
	if( *path == '/' ) *path = '\\';
	++path;
	}

}

/*
 * Handles mouse events.
 *
 * This function was changed to fit the message handling of the ncurses mouse
 * support that differs from the gpm by the following:
 *
 * - sends mouse clicks, double clicks, etc;
 * - sends the message only once: we needn't lock messages;
 * - doesn't send mouse drag and mouse move messages (or may be I couldn't
 *   find the way to get them).
 */

static void mouseHandle()
{

	TEvent event;

	rc = MouReadEventQue( &msEventInfo, &ReadType, msHandle );
	if( rc )
		LOG("MouReadEventQue error, rc = " << rc);
	if( msEventInfo.time == 0 )
		{
		if( msRepaint == 0 ) return;
		msRepaint = 0;
		PTRLOC loc;
		loc.col = msWhere.x;
		loc.row = msWhere.y;
		MouSetPtrPos( &loc, msHandle );
		return;
		}
		
	//	event.mouse.controlKeyState = kbReadShiftState();
	event.mouse.controlKeyState = 0;
	event.mouse.where.x = msEventInfo.col;
	event.mouse.where.y = msEventInfo.row;
	/* convert button bits to TV standard */

	int buttons = 0;
	if ( msEventInfo.fs & 0x06 ) buttons |= mbLeftButton;
	if ( msEventInfo.fs & 0x18 ) buttons |= mbRightButton;

	/* is mouse moved ? */

//	LOG( "Button release state:" << buttons << "msOldButtons: " << msOldButtons );
	if ( event.mouse.where != msWhere )
	{
		msAutoTimer.stop();
		msDoubleTimer.stop();
		msPutEvent(event, buttons, meMouseMoved, evMouseMove);
		msWhere = event.mouse.where;
	}
	if ( buttons > msOldButtons )	/* is any button pressed ? */
	{
		msAutoTimer.start(DELAY_AUTOCLICK_FIRST);
		if (msDoubleTimer.isRunning() && !msDoubleTimer.isExpired())
		{
			msDoubleTimer.stop();
			msPutEvent(event, buttons & ~msOldButtons, meDoubleClick, evMouseDown);
		}
		else
		{
			msDoubleTimer.start(DELAY_DOUBLECLICK);
			msPutEvent(event, buttons & ~msOldButtons, 0, evMouseDown);
		}
	}
	if ( buttons < msOldButtons )	/* is any button released ? */
	{
		msAutoTimer.stop();
		msPutEvent(event, buttons, 0, evMouseUp);
	}
	msOldButtons = buttons;
}

static void msPutEvent(TEvent &event, int buttons, int flags, int what)
{
	msRepaint = 1;
	event.mouse.buttons = 0;
	event.mouse.eventFlags = flags;
	event.what = what;
	if (TEventQueue::mouseReverse)	/* swap buttons ? */
	{
		if (buttons & mbLeftButton) event.mouse.buttons |=
			mbRightButton;
		if (buttons & mbRightButton) event.mouse.buttons |=
			mbLeftButton;
	}
	else event.mouse.buttons = buttons;	/* no swapping */
	TScreen::putEvent(event);
}
