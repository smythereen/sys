/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	This file defines the following functions.
 *		lineage( );
 ******************************************************************************/
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	<common.h>

	pid_t	The_Line[ 32 ];
	pid_t	New_Line[ 32 ];

static	pid_t	parent_pid( pid_t c )
{
	char		buf[ 2048 ];
	char	*	w[ 64 ];

	int	proc;
	int	i;
	char *	cp;

	sprintf( buf, "/proc/%d/stat", c );
	if( ( proc = open( buf, O_RDONLY ) ) < 0 ) return( 0 );
	memset( buf, 0, sizeof( buf ) );
	if( read( proc, buf, sizeof( buf ) ) <= 0 ) {
		close( proc );
		return( 0 );
	}
	close( proc );

	memset( w, 0, sizeof( w ) );
	for( i = 1, cp = buf, w[ 0 ] = buf; *cp != 0; cp++ ) {
		if( !isspace( *cp ) ) continue;
		*cp = 0;
		w[ i++ ] = &cp[ 1 ];
		if( i > 4 ) break;
	}
	c = strtol( w[ 3 ], 0, 10 );
	return( c );
}

static	int	no_lineage;

int	lineage( pid_t p )
{
	pid_t	*	pp;
	pid_t	*	np;

	if( p == 0 && no_lineage ) return( 0 );
	pp = The_Line; 
	if( *pp == 0 ) {
		if( p == 0 ) *pp++ = p = getppid( );
		while( p > 1 && ( p = parent_pid( p ) ) != 0 ) *pp++ = p;
		return( 1 );
	}
	np = New_Line; 
	if( p == 0 ) {
		*np++ = p = getppid( );
	} else {
		*np++ = p = parent_pid( p );
	}
	for( ; *pp == p; pp++, ( *np++ = p = parent_pid( p ) ) ) {
		if( p == 0 && *pp == 0 ) return( 1 );
	}
	*np = 0;
	if( p == 0 ) no_lineage = 1;
	return( 0 );
}
