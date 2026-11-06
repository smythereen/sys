/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2001-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	This file defines the following function.
 *		Trim( );
 ******************************************************************************/
#include	<common.h>

char *	Trim( char * string )
{
	char *	s;

	if(  string == (char*)0 )	return(   ""   );
	if( *string == 0 )		return( string );
	for( ; *string != 0; string++ ) {
		//Trace( 0, "string=\"%s\" *string='%c'", string, *string );
		switch( *string ) {
		case ' ':
		case '\t':
		case '\n':
		case '\r':
				continue;
		}
		break;
	}
	if( *string == 0 ) return( string );
	for( s = string; *s != 0; s++ );
	for( s--; s >= string; s-- ) {
		switch( *s ) {
		//Trace( 0, "string=%s* *s=%c", string, *s );
		case ' ':
		case '\t':
		case '\n':
		case '\r':	*s = 0;
				continue;
		}
		return( string );
	}
	return( string );
}
