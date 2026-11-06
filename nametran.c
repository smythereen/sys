/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2007-2026 John R. Smythe Jr. */

#include	<common.h>

char *	Name_Translate( char * name )
{
	int	i;
	char *	new;

	if( name == (char *)0 ) return( "" );
	if( strchr( name, '/' ) != 0 ) return( name );
	for( i = 0; ( ( new = getenv( name ) ) != (char *)0 && i < 8 ); i++ ) {
		if( strcmp( new, name ) == 0 ) break;
		name = new;
		if( strchr( name, '/' ) != 0 ) break;
	}
	return( name );
}
