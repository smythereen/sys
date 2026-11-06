/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2010-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	Recursive mkdir
 ******************************************************************************/
#include	<common.h>
#include	<sys/stat.h>

int	MkDir( const char * dir, mode_t mode )
{
	int	i;
	char *	bp;
	char	buf[ 10 ][ MAX_PATH ];

	strncpy( buf[ 0 ], dir, sizeof( buf[ 0 ]) );

	for( i = 1; i < 10; i++ ) {
		strcpy( buf[ i ], buf[ i - 1 ] );

		if( ( bp = strrchr( buf[ i ], '/' ) ) == NULL ) return( 0 );
		if( bp == buf[ i ] ) return( 0 );
		*bp = 0;

		if( mkdir( buf[ i ], mode ) != -1 ) break;
		switch( errno ) {
		case EEXIST:	return(  0 );
		case ENOENT:	continue;
		default:	return( -1 );
		}
	}
	for( --i ; i > 0; i-- ) {
		if( mkdir( buf[ i ], mode ) == -1 ) return( -1 );
	}
	return( 0 );
}
