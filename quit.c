/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2007-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	Declare Message System Global Data Area
 *
 *	This file defines the following functions.
 *		AddQuitRoutine( );
 ******************************************************************************/
#include	<common.h>

static	void	(*my_quit[ MAX_QUIT_ROUTINES ])( );

/* FUNCTION:	AddQuitRoutine
 *
 *	DESCRIPTION:
 *		This subroutine will add the address of a subroutine to
 *		the quit subroutine list.
 *
 *	INPUTS:
 *		quit =	Quit subroutine pointer/address
 *
 *	RETURNS:
 *		1 = success
 *		0 = failure
 *
 *	ON ERROR:
 *		Not Possible
 *
 */

int	AddQuitRoutine( void (*quit)( void ) )
{
	int	i;
	for( i = 0; i < MAX_QUIT_ROUTINES; i++ ) {
		if( my_quit[ i ] != 0 ) continue;
		my_quit[ i ] = quit;
		return( 1 );
	}
	return( 0 );
}

/* FUNCTION:	CallItQuits( )
 *
 *	DESCRIPTION:
 *		This subroutine will execute all quit Subroutines
 *
 *	INPUTS:
 *		N/A
 *
 *	RETURNS:
 *		1 = success
 *		0 = failure
 *
 *	ON ERROR:
 *		Not Possible
 *
 */

void	CallItQuits( void )
{
	int	i;
	for( i = ( MAX_QUIT_ROUTINES - 1 ); i >= 0; i-- ) {
		if( my_quit[ i ] == 0 ) continue;
		(*my_quit[ i ])( );
	}
	return;
}

