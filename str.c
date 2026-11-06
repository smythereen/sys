/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2007-2026 John R. Smythe Jr. */

#include	<common.h>

char * strupr( char * x )
{
	char  * y;
	y = x;
	while( *x ) {
		if( islower( *x ) ) *x = toupper( *x );
		x++;
	}
	return( y );
}

char * strlwr( char * x )
{
	char  * y;
	y = x;
	while( *x ) {
		if( isupper( *x ) ) *x = tolower( *x );
		x++;
	}
	return( y );
}
