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

/* FUNCTION:	FatalErrorAP
 *
 *	DESCRIPTION:
 *		This subroutine will log an error and then it will wait 10
 *		seconds after that it terminates the current process.
 *
 *	INPUTS:
 *		fmt =	message format string pointer
 *		ap  =	additional argument list pointer
 *
 *	RETURNS:
 *		Never Returns
 */

void	FatalErrorAP( char * fmt, va_list ap )
{
	EventLevel = EVENT_CRITICAL;
	EventLogAP( fmt, ap );		/* Annunciate to print log */
	EventLevelLog( EVENT_CRITICAL, "Process going down" );
	CallItQuits( );			/* Execute All Shutdown Routines */
	sleep( 1 );			/* Sleep to prevent rapid respawning */
	exit( 1 );			/*  of inittab based processes */
}

/* FUNCTION:	Fatal
 *
 *	DESCRIPTION:
 *		This subroutine will log an error and then it will wait 10
 *		seconds after that it terminates the current process.
 *
 *	INPUTS:
 *		fmt =	message format string pointer
 *		... =	additional arguments
 *
 *	RETURNS:
 *		Never Returns
 */

void	FatalError( char * fmt, ... )
{
	va_list	ap;

	EventLevel = EVENT_CRITICAL;
	va_start( ap, fmt );
	EventLogAP( fmt, ap );		/* Annunciate to print log */
	va_end( ap );
	EventLevelLog( EVENT_CRITICAL, "Process going down" );
	CallItQuits( );			/* Execute All Shutdown Routines */
	sleep( 1 );			/* Sleep to prevent rapid respawning */
	exit( 1 );			/*  of inittab based processes */
}
