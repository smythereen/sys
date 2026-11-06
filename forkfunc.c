/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2007-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	These subroutines allow the system() function to work like a printf()
 *
 *	This file defines the following public functions.
 *		ForkFunction( );
 ******************************************************************************/
#include	<sys/wait.h>
#include	<common.h>

	int	I_Am_Child = 0;

	int	ChildrenProcesses = -1;
	pid_t	ChildProcess = -1;
	int	ChildStatus = -1;

void	ChildExit( int value )
{
	if( I_Am_Child ) exit( value );
}

/* FUNCTION:	CheckChildren
 *
 *	DESCRIPTION:
 *		This subroutine is a helper for the Fork Function
 *
 *	INPUTS:
 *		none
 *
 *	RETURNS:
 *		Returns -1 If No Child Death Has Occured
 *		Returns  0 If No Children Left
 *		Returns Child's PID ( >0 ) On Death
 *
 */

pid_t	CheckChildren( void )
{
	if( ChildrenProcesses == 0 ) return( 0 );
	ChildStatus = -1;
	if( ( ChildProcess = waitpid( -1, &ChildStatus, WNOHANG | WUNTRACED ) ) > 0 ) {
		if( ChildrenProcesses > 0 ) ChildrenProcesses -= 1;
	}
	return( ChildProcess );
}

/* FUNCTION:	FlushChildren
 *
 *	DESCRIPTION:
 *		This subroutine is a helper for the Fork Function
 *
 *	INPUTS:
 *		none
 *
 *	RETURNS:
 *		Returns: >0 Number Of Children Known(Used Fork()) To Be Alive
 *			  0 No Children Alive
 *			 -1 Do Not Know How Many Children Are Alive ( Fork() Not Used )
 *
 */

int	FlushChildren( void )
{
	while( CheckChildren( ) > 0 );
	return( ChildrenProcesses );
}

/* FUNCTION:	Fork
 *
 *	DESCRIPTION:
 *		This subroutine is a wrapper for the fork() system function
 *
 *	INPUTS:
 *		none
 *
 *	RETURNS:
 *		Returns > 0 is pid of child process
 *		Returns 0 is child process
 *
 *	ON ERROR:
 *		Returns -1 Cannot Fork a child process
 *
 */

int	Fork( )
{
	New_Err_File( );

	for( ; ; ) {
		switch( ( ChildProcess = fork( ) ) ) {

		case    -1:	if( FlushChildren( ) ) continue; /* Auto Flush */
				Trace(  0,
					"Cannot Fork (errno=%u:%s)",
					errno,
					strerror( errno )
				);
				return( -1 );

		case    0:	I_Am_Child = 1;
				ChildrenProcesses = -1;
				ChildProcess = -1;
				ChildStatus = -1;
				return( 0 );

		default:	ChildStatus = -1;
				if( ChildrenProcesses <= 0 ) {
					ChildrenProcesses = 1;
				} else {
					ChildrenProcesses += 1;
				}
				return( (int)ChildProcess );
		}
	}
}

/* FUNCTION:	ForkFunction
 *
 *	DESCRIPTION:
 *		This subroutine is a wrapper for the fork() system function
 *		WITH Auto-Flush Of Child Processes
 *
 *	INPUTS:
 *		func =	Function Pointer
 *		...  =	Additional Function Args
 *
 *	RETURNS:
 *		Returns 1 On Successful Fork
 *
 *	ON ERROR:
 *		Returns 0
 *
 */

int	ForkFunction( void (*func)( char * fmt, va_list ap ), char * fmt, ... )
{
	int	ret;
	va_list	ap;

	if( func == NULL ) return( (int)FlushChildren( ) );

	switch( ( ret = Fork( ) ) ) {

	case    -1:	va_start( ap, fmt );
			func( fmt, ap );
			va_end( ap );
			break;

	case    0:	va_start( ap, fmt );
			func( fmt, ap );
			va_end( ap );
			exit( 0 );
	}
	FlushChildren( );
	return( ret );
}
