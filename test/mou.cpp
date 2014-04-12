#define INCL_MOU
#define INCL_DOSPROCESS

#include <os2.h>
#include <stdio.h>

void main()
{
int rc;
HMOU msHandle;
MOUEVENTINFO msEventInfo;
USHORT ReadType = 0;

rc = MouOpen( NULL, &msHandle );
if( rc )
{
		printf("MouOpen error, rc = %d\n", rc);
		return;
}

rc = MouDrawPtr( msHandle );
while(1)
{
	rc = MouReadEventQue( &msEventInfo, &ReadType, msHandle );
	if( rc )
	{
		printf("MouReadEventQue error, rc = %d\n", rc);
		return;
	}
	if( msEventInfo.time )
	printf( "mousestate = %d\n",
		msEventInfo.fs
		);
	
	DosSleep( 100 );
}

}
