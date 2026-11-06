/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2007-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	Log Error Message And Die!
 *
 *	This file defines the following functions.
 *		SystemWarn( );
 ******************************************************************************/
#include	<common.h>

/*
extern char *	sys_errlist[ ];
*/

/* FUNCTION:	SystemWarnAP
 *
 *	DESCRIPTION:
 *		This subroutine will log a system error with errno and
 *		after that it terminates the current process.
 *
 *	INPUTS:
 *		func =	caller's function/subroutine name
 *		fmt  =	message format string pointer
 *		ap   =	additional argument list pointer
 *
 *	RETURNS:
 *		Never Returns
 */

void	SystemWarnAP( char * func, char * fmt, va_list ap )
{
	int	save;
	char	buf[ 256 ];

	save = EventLevel;
	EventLog(
		"%s: %s (errno=%u)",
		func,
		strerror( errno ),
		errno
	);
	sprintf( buf, "%s: %s", func, fmt );
	EventLevel = EVENT_MAJOR;
	EventLogAP( buf, ap );		/* Annunciate to print log */
	EventLevel = save;
}

/* FUNCTION:	SystemError
 *
 *	DESCRIPTION:
 *		This subroutine will log a system error with errno and
 *		after that it terminates the current process.
 *
 *	INPUTS:
 *		func =	caller's function/subroutine name
 *		fmt  =	message format string pointer
 *		...  =	additional arguments
 *
 *	RETURNS:
 *		Never Returns
 */

void	SystemWarn( char * func, char * fmt, ... )
{
	va_list	ap;

	va_start( ap, fmt );
	SystemWarnAP( func, fmt, ap );	/* Annunciate to print log */
	va_end( ap );
}
