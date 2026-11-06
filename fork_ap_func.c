/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2009-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	This file defines the following public functions.
 *		Fork_AP_Function( );
 ******************************************************************************/
#include	<sys/wait.h>
#include	<common.h>

#define	MAX_CHILDREN	1000

typedef	struct	child_s	{
	pid_t	pid;
	void	(*func)( pid_t pid, int status );
}	child_t;

static	child_t	Children[ MAX_CHILDREN ];


int	Check_Func_Children( void )
{
	int		i;
	child_t	*	c;
	pid_t		pid;
	int		status;

	status = -1;
	for( ; ; ) {
		switch( ( pid = waitpid( -1, &status, WNOHANG | WUNTRACED ) ) ) {
		case -1:
			switch( errno ) {
			case ECHILD:	/* The calling process does not have any unwaited-for children. */
				return( 0 );

			case EINTR:	/* WNOHANG was not set and an unblocked signal or a SIGCHLD was caught. */
				continue;

			case EINVAL:	/* The options argument was invalid. */
			default:
				return( -1 );
			}
			continue;

		case 0:	/* No Children Died */
			return( 0 );

		default:
			break;
		}
		for( i = 0, c = Children; i < MAX_CHILDREN; i++, c++ ) {
			if( c->pid != pid ) continue;
			if( c->func != NULL ) c->func( pid, status );
			c->func = NULL;
			c->pid = 0;
			break;
		}
	}
	return( 0 );
}

/* FUNCTION:	Fork_AP_Func
 *
 *	DESCRIPTION:
 *		This subroutine is a wrapper for the fork() system function
 *		WITHOUT Auto-Flush Of Child Processes
 *		(so child PIDs can be track/managed)
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

int	Fork_AP_Func( int (*func)( va_list ap ),  void (*death)( pid_t pid, int status ), ... )
{
	pid_t		pid;
	int		status;
	int		i;
	child_t	*	c;
	va_list		ap;

	if( func == NULL ) return( (int)Check_Func_Children( ) );

	for( i = 0, c = Children; i < MAX_CHILDREN; i++, c++ ) {

		if( c->pid != 0 ) continue;
		c->func = death;

		switch( ( c->pid = fork( ) ) ) {

		case    -1:	/* Error - No Child Spawned */
				Check_Func_Children( );
				va_start( ap, death );
				status = func( ap );
				va_end( ap );
				if( death != NULL ) death( pid, status );
				c->func = NULL;
				c->pid = 0;
				Check_Func_Children( );
				return( pid );

		case    0:	/* Child */
				va_start( ap, death );
				status = func( ap );
				va_end( ap );
				exit( status );

		default:	/* Parent */
				break;
		}
		Check_Func_Children( );
	}
	return( pid );
}
