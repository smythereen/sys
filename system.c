/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2007-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	These subroutines allow the system() function to work like a printf()
 *
 *	This file defines the following public functions.
 *		System( );
 ******************************************************************************/
#include	<common.h>

/* FUNCTION:	SystemAP
 *
 *	DESCRIPTION:
 *		This subroutine is a wrapper for the system() library function
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

int	SystemAP( char * fmt, va_list ap )
{
	int	ret;
	char	buf[ 4096 ];

	vsprintf( buf, fmt, ap );
	ret = system( buf );
	Trace(	SYS_TRC_LVL, "System( \"%s\" ) = %d", buf, ret );
	return( ret );
}

/* FUNCTION:	System
 *
 *	DESCRIPTION:
 *		This subroutine is a wrapper for the system() library function
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

int	System( char * fmt, ... )
{
	int	ret;
	va_list	ap;

	va_start( ap, fmt );
	ret = SystemAP( fmt, ap );
	va_end( ap );
	return( ret );
}
