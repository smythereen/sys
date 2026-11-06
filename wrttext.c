/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2025-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	Writes A Variable Length ASCII Line To A Text File
 *	( No Buffering! )
 ******************************************************************************/
#include	<fcntl.h>
#include	<common.h>

#ifdef	JUNK

size_t	Write( int FD, char * buf, size_t size )
{
	size_t	r, t;

	t =  0;
	while( ( r = write( 2, buf, size ) ) != size ) {
		if( r > 0 ) {
			t += r;
			size -= r;
			buf = &((char *)buf)[ r ];
			continue;
		} else if ( r == 0 ) {
			continue;
		}

		switch( errno ) {
		case EAGAIN:	return(  0 );
		case EINTR:	continue;
		default:	return( -1 );
		}
	}
	return( ( r + t ) );
}

#endif

int	Write_TextAP( int FD, char * line, va_list ap )
{
	int	s;
	char	buf[ 4096 ];

	s = vsprintf( buf, line, ap );
	if( s <= 0 ) {
		s = 0;
		buf[ s++ ] = '\n';
		buf[ s   ] =   0;
	} else if( buf[ s - 1 ] != '\n' ) {
		buf[ s++ ] = '\n';
		buf[ s   ] =   0;
	}
	return( write( FD, buf, s ) );
}

int	Write_Text( int FD, char * line, ... )
{
	int	r;
	va_list	ap;

	va_start( ap, line );
	r = Write_TextAP( FD, line, ap );
	va_end( ap );
	return( r );
}
