/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2007-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	Log Error Message And Die!
 *
 *	This file defines the following functions.
 *		FatalError( );
 ******************************************************************************/
#include	<common.h>

/*
extern char *	sys_errlist[ ];
*/

/* FUNCTION:	SystemErrorAP
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

void	SystemErrorAP( char * func, char * fmt, va_list ap )
{
	char	buf[ 256 ];

	ErrorLog(
		"%s: %s (errno=%u)",
		func,
		strerror( errno ),
		errno
	);
	sprintf( buf, "%s: %s", func, fmt );
	FatalErrorAP( buf, ap );		/* Annunciate to print log */
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

void	SystemError( char * func, char * fmt, ... )
{
	va_list	ap;

	va_start( ap, fmt );
	SystemErrorAP( func, fmt, ap );	/* Annunciate to print log */
	va_end( ap );
}
