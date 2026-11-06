/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2007-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	write a message subroutine - Prints user messages
 *
 *	This file defines the following public functions.
 *		SysPrt( );
 ******************************************************************************/
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>

#include	<common.h>
#include	<atomic_32.h>

	u32	TraceLevel;
	u32 *	TracePointer;

#define TIME_FORMAT     "%y-%m-%d %H:%M:%S."

	char	TraceFile[ MAX_PATH ];
static	struct  timeval now;
	char	my_name[ MAX_PATH ];
	char	*LocalName;
	s32	PrintStamp = 1;
	char	PrintName = 0;
	char	PrintPID = 0;
	int	PrintDays = 7;
	int	PrintDay = 0;
	int	PrintStdOut = 1;

int	Initialize_Print( char * trace )
{
	int	fd;
	char *	cp;

	if( trace == NULL ) return( -1 );
	if( trace[ 0 ] != '/' && trace[ 0 ] != '.' ) {
		if( ( cp = getenv( "FILE_PATH" ) ) == NULL ) cp = "./";
		sprintf( TraceFile, "%s%s", cp, trace );
	} else {
		strncpy( TraceFile, trace, sizeof( TraceFile ) );
	}
	close( 2 );
	fd = open( TraceFile, O_CREAT | O_APPEND | O_NOCTTY | O_WRONLY, 0666 );
	if( fd == -1 ) return( -1 );
	if( fd != 2 ) {
		if( dup2( fd, 2 ) == -1 ) return( -1 );
		close( fd );
	}
	if( !PrintStdOut ) return( 2 );
	return( dup2( 2, 1 ) );
}

int	Reset_Print( void )
{
	int		i;
	char	*	cp;
	struct	stat	fd;
	struct	stat	file;
	char		src[ MAX_PATH ];
	char		tgt[ MAX_PATH ];

	if( TraceFile[ 0 ] == 0 ) {
		sprintf( src, "/proc/%u/fd/2", (u32)getpid( ) );
		if( readlink( src, TraceFile, sizeof( TraceFile ) ) == -1 ) return( -1 );
	}
	if( ( cp = strstr( TraceFile, ".trc" ) ) != NULL ) cp[ 4 ] = 0;
	if( PrintDays > 0			&&
	    stat( TraceFile, &file ) != -1	&&
	    fstat( 2, &fd ) != -1		&&
	    file.st_dev == fd.st_dev		&&
	    file.st_ino == fd.st_ino		) {
		for( i = PrintDays; i > 1; i -= 1 ) {
			sprintf( tgt, "%s.%u", TraceFile, i );
			sprintf( src, "%s.%u", TraceFile, ( i - 1 ) );
			if( i == PrintDays ) unlink( tgt );
			rename( src, tgt );
		}
		sprintf( tgt, "%s.%u", TraceFile, 1 );
		rename( TraceFile, tgt );
	}
	strncpy( src, TraceFile, sizeof( src ) );
	return( Initialize_Print( src ) );
}

void	New_Err_File( void )
{
	u32	day;

	if( PrintDays <= 0 ) return;
	gettimeofday( &now, 0 );
	day = ( now.tv_sec / ( 60 * 60 * 24 ) );
	if( PrintDay != 0 && day != PrintDay ) {
		if( Reset_Print( ) == -1 ) return;
	}
	PrintDay = day;
}

static	inline	int	Err_Write( void * buf, u32 size )
{
	int	r;
	int	t;

	t =  0;
	while( ( r = write( 2, buf, size ) ) != size ) {
		if( r > 0 ) {
			t += r;
			size -= r;
			buf = &((char *)buf)[ r ];
			continue;
		} else if ( r == 0 ) {
			continue;
		}

		switch( errno ) {
		case EAGAIN:	return(  0 );
		case EINTR:	continue;
		default:	return( -1 );
		}
	}
	return( ( r + t ) );
}

/* FUNCTION:	SysPrtAP
 *
 *	DESCRIPTION:
 *		This subroutine is used to format and print a user's message.
 *
 *	INPUTS:
 *		fmt =	Print Format String
 *		ap  =	Additional Argument List Pointer
 *
 *	RETURNS:
 *		Nothing
 *
 *	ON ERROR:
 *		Not Possible
 *
 */

static	char *	pfmts[ ] =	{
		"\r> %5$s\n",				//	0 =	NoStamp	NoName	NoPID
		"\r%1$s%2$06u> %5$s\n",			//	1 =	Stamp	NoName	NoPID
		"\r%3$-9s> %5$s\n",			//	2 =	NoStamp	Name	NoPID
		"\r%1$s%2$06u %3$-9s> %5$s\n",		//	3 =	Stamp	Name	NoPID
		"\r(%4$5u) %5$s\n",			//	4 =	NoStamp	NoName	PID
		"\r%1$s%2$06u(%4$5u) %5$s\n",		//	5 =	Stamp	NoName	PID
		"\r%3$-9s(%4$5u) %5$s\n",		//	2 =	NoStamp	Name	PID
		"\r%1$s%2$06u %3$-9s(%4$5u) %5$s\n",	//	3 =	Stamp	Name	PID
		NULL
};

void	SysPrtAP( char * fmt, va_list ap )
{
	int	i;
	char	b3[ 8192 ];
	char	b2[ 8192 ];
	char	b1[ 8192 ];

	now.tv_sec = 0;
	now.tv_usec = 0;

	i = 0;
	if( S32_Dec_And_Test( &PrintStamp ) ) {
		// This Prevents A Signal Handler Routine That Calls strftime FROM LOCKING UP THE ENTIRE PROCESS!
		// The asshole that put mutex locking in the timezone routine of strftime should be shot!
		// This prevents the use of strftime in signal handler routines!
		// But SysPrt/Trace has Historically ALWAYS been called from the "sig" unit's generic signal handler
		gettimeofday( &now, 0 );
		strftime( b3, sizeof( b3 ), TIME_FORMAT, localtime( &now.tv_sec ) );
		i |= 1;
	}
	S32_Inc( &PrintStamp );
	if( PrintName ) i |= 2;
	if( PrintPID  ) i |= 4;

	vsprintf( b1, fmt, ap );
	for( fmt=&b1[strlen(b1)-1];(fmt >= b1 && *fmt == '\n'); *fmt--=0 );

	if( PrintDays > 0 ) New_Err_File( );

	Err_Write(
		b2,
		sprintf(	b2,
				pfmts[ i ],
				b3,
				now.tv_usec,
				( LocalName ? LocalName : my_name ),
				getpid( ),
				b1
		)
	);
}

/* FUNCTION:	SysPrt
 *
 *	DESCRIPTION:
 *		This subroutine is used to format and print a user's message.
 *
 *	INPUTS:
 *		fmt =	Print Format String
 *		... =	Additional Arguments
 *
 *	RETURNS:
 *		Nothing
 *
 *	ON ERROR:
 *		Not Possible
 *
 */

void	SysPrt( char * fmt, ... )
{
	va_list	ap;

	va_start( ap, fmt );
	SysPrtAP( fmt, ap );
	va_end( ap );
}
