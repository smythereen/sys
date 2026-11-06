/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2007-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	Appends A Variable Length ASCII Line To A Text File
 ******************************************************************************/
#include	<fcntl.h>
#include	<common.h>

#ifdef	JUNK
int	Add_LineAP( char * filename, char * line, va_list ap )
{
	int	FD;
	int	r;
	int	s;
	char	buf[ 2048 ];

	FD = open( filename, ( O_WRONLY | O_APPEND | O_CREAT ), 0666 );
	if( FD == -1 ) return( FD );

	s = vsprintf( buf, line, ap );
	if( s <= 0 ) {
		s = 0;
		buf[ s++ ] = '\n';
		buf[ s   ] =   0;
	} else if( buf[ s - 1 ] != '\n' ) {
		buf[ s++ ] = '\n';
		buf[ s   ] =   0;
	}

	r = write( FD, buf, s );
	close( FD );
	return( r );
}
#endif

int	Add_Line( char * filename, char * line, ... )
{
	int	FD;
	int	r;
	va_list	ap;

	FD = open( filename, ( O_WRONLY | O_APPEND | O_CREAT ), 0666 );
	if( FD == -1 ) return( FD );

	va_start( ap, line );
	r = Write_TextAP( FD, line, ap );
	va_end( ap );
	close( FD );
	return( r );
}
