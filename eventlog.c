/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2007-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	Provides function(s) to log user event messages to a log file.
 *
 *	This file defines the following public functions.
 *		EventLog( );
 ******************************************************************************/
#include	<fcntl.h>
#include	<common.h>

#define BAD		-1
#define MSGPRT		TRUE
#define ERROR_SLEEP	30
#define TIME_FORMAT	"%y-%m-%d %H:%M:%S"
#define LogFileName	"event.log"
#define Flags		(O_WRONLY | O_CREAT | O_APPEND)
#define Mode		0666

char *	EventLogFilePath;				/* Pointer to path string */
int	EventLevel;

/* FUNCTION:	EventLog
 *
 *	DESCRIPTION:
 *		This subroutine is used to format a user's event message and
 *		log it to a log file.
 *
 *	INPUTS:
 *		Follows printf technique.
 *		fmt =	Print Format String
 *		ap  =	Additional Argument List Pointer
 *
 *	RETURNS:
 *		Nothing
 *
 *	ON ERROR:
 *		Errors are annunciated using SysPrt().  After a timeout
 *		period, the program exits, returning the global variable
 *		errno as an exit code.
 *
 */

static	char*	Event_Log_AP( char * fmt, va_list ap )
{
	char	b1[ MAX_EVT_LEN ];	/* Working buffers */
	char	b2[ MAX_EVT_LEN ];
static	char	b3[ MAX_EVT_LEN ];

	struct  timeval now;

	/*
	 *	If not already done, establish a log file pathname.
	 *	Attempt to obtain a global pathname from the environment.
	 *	If the global path is not available, substitute a local pathname.
	 */
	if( ! EventLogFilePath ) {
		if ( (EventLogFilePath = (char *)getenv( "ELOG_FILE" )) == NULL ) {
			EventLogFilePath = LogFileName;
		}
	}
#ifdef	JUNK
	/*
	 *	Print event to trace file/display
	 */
	SysPrtAP( fmt, ap );
#endif
	/*
	 *	Format caller's message
	 */
	vsprintf( b1, fmt, ap );
	/*
	 *	Format message's time stamp
	 */
	now.tv_sec = 0;
	now.tv_usec = 0;
	gettimeofday( &now, 0 );
	strftime( b2, sizeof( b2 ), TIME_FORMAT, localtime( &now.tv_sec ) );
	sprintf( b3, 
		"%s.%06lu  (%s) %s\n", 
		b2, now.tv_usec, 
		( LocalName ? LocalName : my_name ), 
		b1 
	);

	/*
	 *	Append caller message to log file.
	 */
	if( Add_Line( EventLogFilePath, b3 ) > 0 ) return ( b3 );
	SysPrt( "Log file write failed. (%u)", errno );
	SysPrt( "Exiting after %u seconds.", ERROR_SLEEP );
	sleep( ERROR_SLEEP );
	exit( errno );
}

char*	(*Event_LogAP)( char * fmt, va_list ap ) = Event_Log_AP;

/* FUNCTION:	EventLog
 *
 *	DESCRIPTION:
 *		This subroutine is used to format a user's event message and
 *		log it to a log file.
 *
 *	INPUTS:
 *		Follows printf technique.
 *		fmt =	Print Format String
 *		... =	Additional Arguments
 *
 *	RETURNS:
 *		Nothing
 *
 *	ON ERROR:
 *		Errors are annunciated using SysPrt().  After a timeout
 *		period, the program exits, returning the global variable
 *		errno as an exit code.
 *
 */

void	EventLog( char * fmt, ... )
{
	int	save;
	va_list	ap;

	save = EventLevel;
	EventLevel = EVENT_INFO;

	va_start( ap, fmt );
	EventLogAP( fmt, ap );
	va_end( ap );

	EventLevel = save;
}

/* FUNCTION:	EventLevelLog
 *
 *	DESCRIPTION:
 *		This subroutine is used to format a user's event message and
 *		log it to a log file.
 *
 *	INPUTS:
 *		Follows printf technique.
 *		fmt =	Print Format String
 *		... =	Additional Arguments
 *
 *	RETURNS:
 *		Nothing
 *
 *	ON ERROR:
 *		Errors are annunciated using SysPrt().  After a timeout
 *		period, the program exits, returning the global variable
 *		errno as an exit code.
 *
 */

void	EventLevelLog( int level, char * fmt, ... )
{
	int	save;
	va_list	ap;

	save = EventLevel;
	EventLevel = level;

	va_start( ap, fmt );
	EventLogAP( fmt, ap );
	va_end( ap );

	EventLevel = save;
}

/* FUNCTION:	ErrorLog
 *
 *	DESCRIPTION:
 *		This subroutine is used to format a user's event message and
 *		log it to a log file.
 *
 *	INPUTS:
 *		Follows printf technique.
 *		fmt =	Print Format String
 *		... =	Additional Arguments
 *
 *	RETURNS:
 *		Nothing
 *
 *	ON ERROR:
 *		Errors are annunciated using SysPrt().  After a timeout
 *		period, the program exits, returning the global variable
 *		errno as an exit code.
 *
 */

void	ErrorLog( char * fmt, ... )
{
	int	save;
	va_list	ap;

	save = EventLevel;
	EventLevel = EVENT_MINOR;

	va_start( ap, fmt );
	EventLogAP( fmt, ap );
	va_end( ap );

	EventLevel = save;
}
