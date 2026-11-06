/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2007-2026 John R. Smythe Jr. */

#include	<common.h>

int	fixed2long( char * s, s32 * value, int numchars )
{
	if( value == NULL ) return( 0 );
	*value = 0;
	if( s == NULL ) return( 0 );
	switch( numchars ) {
	case 6:	if( *s < '0' || *s > '9' ) break;
		*value += ( ( *s++ - '0' ) * 100000);
	case 5:	if( *s < '0' || *s > '9' ) break;
		*value += ( ( *s++ - '0' ) * 10000 );
	case 4:	if( *s < '0' || *s > '9' ) break;
		*value += ( ( *s++ - '0' ) * 1000  );
	case 3:	if( *s < '0' || *s > '9' ) break;
		*value += ( ( *s++ - '0' ) * 100   );
	case 2:	if( *s < '0' || *s > '9' ) break;
		*value += ( ( *s++ - '0' ) * 10    );
	case 1:	if( *s < '0' || *s > '9' ) break;
		*value += ( ( *s++ - '0' ) * 1     );
	case 0:	return( 1 );
	}
	*value = 0;
	return( 0 );
}

