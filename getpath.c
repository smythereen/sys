/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2007-2026 John R. Smythe Jr. */

#include	<common.h>

char *	Get_Path( char * buf, int size, char * pth )
{
	char *	new;

	if( buf == 0 || size <= 0 ) return( "" );
	memset( buf, 0, size );
	size -= 1;
	if( *( pth = Name_Translate( pth ) ) == 0 ) return( buf );
	for( new = buf; ( *pth != 0 && size > 0 ); *new++ = *pth++, size-- );
	if( size > 0 && *--pth != '/' ) *new++ = '/';
	*new = 0;
	return( buf );
}
