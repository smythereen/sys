/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2007-2026 John R. Smythe Jr. */

#include	<stdarg.h>
#include	<common.h>

char *	File_Path( char * buf, int size, char * env, char * file, ... )
{
	va_list	ap;

	if( buf == 0 || size <= 0 ) {
		Trace(	SYS_TRC_LVL,
			"Error File_Path( \"%s\", \"%s\" ): No Buffer !",
			env,
			file
		);
		return( (char *)0 );
	}
	env = Get_Path( buf, size, env );
	if( file == (char *)0 || *file == 0 ) {
		Trace(	SYS_TRC_LVL,
			"Error File_Path( \"%s\", \"\" ): No File Name !",
			env
		);
		return( buf );
	}
	for( --size; ( *env != 0 && size > 0 ); env++, size-- );
	if( size <= 0 ) {
		Trace(	SYS_TRC_LVL,
			"Error File_Path( \"%s\", \"%s\" ): "
			"No Room For File Name !",
			buf,
			file
		);
		return( buf );
	}
	va_start( ap, file );
	size -= vsprintf( env, file, ap );
	va_end( ap );
	if( size >= 0 ) return( buf );
	Trace(	SYS_TRC_LVL,
		"Error File_Path( \"%s\" ): "
		"Exceeded buffer size by %u bytes!",
		buf,
		-size
	);
	return( (char *)0 );
}
