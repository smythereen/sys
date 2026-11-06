/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2007-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	These subroutine allows the exec??() functions to take a va_list
 *
 *	This file defines the following public functions.
 *		Exec_AP( );
 *		Exec( );
 ******************************************************************************/
#include	<common.h>

extern	char	**	environ;

/* FUNCTION:	Exec_AP
 *
 *	DESCRIPTION:
 *		This subroutine is a wrapper for the execve() system function
 *		(designed to be call directly from Fork_AP_Func() function)
 *
 *	INPUTS:
 *		va_list ap = ( a list of program name and arguments
 *
 *	RETURNS:
 *		Returns Nothing On Success
 *
 *	ON ERROR:
 *		Returns -1
 *
 */

int	Exec_AP( char ** ap )
{
	return( execve( *ap, ap, environ ) );
}

/* FUNCTION:	Exec( )
 *
 *	DESCRIPTION:
 *		This subroutine is a wrapper for the execve() system function
 *
 *	INPUTS:
 *		program	= Full_Path/Program
 *		...	= Additional Args (char *)
 *
 *	RETURNS:
 *		Returns Nothing On Success
 *
 *	ON ERROR:
 *		Returns -1
 *
 */

int	Exec( char * program, ... )
{
	return( Exec_AP( (char **)&program ) );
}
