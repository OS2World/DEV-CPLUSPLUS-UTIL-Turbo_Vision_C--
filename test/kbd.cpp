#define INCL_SUB
#define INCL_DOSPROCESS
#include <os2.h>
#include <stdio.h>

void main()
{
HKBD KbdHandle;
KBDKEYINFO pkbci;
USHORT rc;
KBDINFO StatData;

rc = KbdOpen( &KbdHandle );
if( rc ){
	printf("KbdOpen error\n");
	return;
	}
rc = KbdGetFocus( 0, KbdHandle );
if( rc ){
	printf("KbdGetFocus error, rc = %d\n", rc);
	return;
	}

StatData.cb = sizeof( StatData );
StatData.fsMask = 0x4;
StatData.chTurnAround = 0;
StatData.fsInterim = 0;
StatData.fsState = 0;

rc = KbdSetStatus( &StatData, KbdHandle );
if( rc ){
	printf("KbdSetStatus error, rc = %d\n", rc);
	return;
	}

while(1)
	{
	rc = KbdCharIn( &pkbci, 1, KbdHandle );
	if( rc ){
		printf("KbdCharIn error, rc = %d\n", rc);
		break;
		}
	int kbReady = 0;
	int shiftstate;	// 0-bit: Shift
	shiftstate = 0;	// 1-bit: Ctrl
							// 2-bit: Alt
	shiftstate |= ( (pkbci.fsState & 3) > 0 ) ? 1 : 0;	// Shift state
	shiftstate |= ( (pkbci.fsState & 4) > 0 ) ? 2 : 0;	// Ctrl state
	shiftstate |= ( (pkbci.fsState & 8) > 0 ) ? 4 : 0;	// Alt state
	kbReady |= ( (pkbci.fbStatus & 64) > 0 ) ? 1 : 0;
	if( kbReady )
	printf(" { %d,\t%d,\t%d,\t%d },\n",
		pkbci.chChar,
		pkbci.chScan, 
		shiftstate,
		pkbci.fbStatus
		);
	DosSleep( 10 );
	}
rc = KbdFreeFocus( KbdHandle );
if( rc ){
	printf("KbdFreeFocus error, rc = %d\n", rc);
	return;
	}

KbdClose( KbdHandle );

}
