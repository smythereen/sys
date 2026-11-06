/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2026 John R. Smythe Jr. */
/*
 *	Galois Field GF(8) Routines
 *
 *	Initialise log and exponent tables using generator x^8 + x^4 + x^3 + x^2 + 1 = 0x11D
 */

#include	<common.h>


static	u8	gl[ 0x100 ], ge[ 0x200 ];

#define	DATA_T	u8

void	ginit( void )
{
	unsigned int b, l;

	for( l = 0, b = 1; l < 255; l++ ) {
		gl[ b ] = l;
		ge[ l ] = b;
		ge[ l + 255 ] = b;
		b += b;
		if( b & 0x100 ) b ^= 0x11d;
	}
}

static	inline	int	gadd( int a, int b )
{
	return( a ^ b );
}

static	inline	int	gsub( int a, int b )
{
	return( a ^ b );
}

/*
 *	Multiply: a * b = exp( log( a ) + log( b ) )
 */
static	int	gmul( int a, int b )
{
	return( ( ( a == 0 ) || ( b == 0 ) ) ? 0 : ge[ gl[ a ] + gl[ b ] ] );
}

/*
 *	Divide: a / b = exp( log( a ) - log( b ) )
 */
static	int	gdiv( int a, int b )
{
	return( ( ( a == 0 ) || ( b == 0 ) ) ? 0 : ge[ gl[ a ] - gl[ b ] + 255 ] );
}

/*
 *	Power: a^b = exp( log( a ) * b )
 */
static	int	gpow( int a, int b )
{
	return( ( a == 0 ) ? 0 : ge[ ( gl[ a ] * b ) % 255 ] );
}


#ifdef	JUNK_F

#define	DATA_T	f64

#define	gadd( a, b )	( a + b )

#define	gsub( a, b )	( a - b )

#define	gmul( a, b )	( a * b )

#define	gdiv( a, b )	( a / b )

static	int	gpow( int a, int b )
{
	int	x = 0;
	if( a == 0 ) return( 0 );
	if( b == 0 ) return( 1 );
	if( b == 1 ) return( a );
	for( x = a; b > 1; b-- ) x *= a;
	return( x );
}

#endif

