/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2009-2026 John R. Smythe Jr. */
/*******************************************************************************
 *
 *	Galois Field GF(8) Routines
 *
 */
#ifndef	GF8_H
#define	GF8_H

#include	<common.h>

#if	defined( __cplusplus )
extern	"C"	{
#endif

static	u8	gl[   0x100 ];	// Galois Log Table
static	u8	ex[ 0x10100 ];	// Galois Exponent Table 65536(overflow)+256(underflow) 
static	u8 *ge=&ex[   0x100 ];	// Supports negative underflow & postive overflow

void	ginit( u32 poly )
{
	u32	b;
	s32	l;
	s32	m;

//	Initialise log and exponent tables using
//	generator x^8 + x^4 + x^3 + x^2 + 1 = 0x11D

	if( !poly ) poly = 0x011D;

	for( l = 0, b = 1; l < 255; l++ ) {
		gl[ b ]		= (u8)l;
		ge[ l - 255 ]	= (u8)b;
		for( m = 0; m < 65535; m += 255 ) ge[ l + m ] = (u8)b;
		b += b;
		if( b & 0x100 ) b ^= poly;
	}
}

#define	GLOG( a )	(s32)( gl[  (u8)( a ) ] )
#define	GEXP( a )	(s32)( ge[ (s32)( a ) ] )

/*
 *	Multiply: a * b = exp( log( a ) + log( b ) )
 *	Divide:   a / b = exp( log( a ) - log( b ) )
 *	Power:    a**b  = exp( log( a ) * b )
 */

#define	GADD( a, b )	(s32)( (u8)a ^ (u8)b )
#define	GSUB( a, b )	(s32)( (u8)a ^ (u8)b )
#define	GMUL( a, b )	(s32)( ( !(u8)a || !(u8)b ) ? 0 : GEXP( GLOG( a ) + GLOG( b ) ) )
#define	GDIV( a, b )	(s32)( ( !(u8)a || !(u8)b ) ? 0 : GEXP( GLOG( a ) - GLOG( b ) ) )
#define	GPOW( a, b )	(s32)( ( !(u8)a )           ? 0 : GEXP( GLOG( a ) *   (u8)b   ) )
//#define	GPOW( a, b )	(s32)( ( !(u8)a )           ? 0 : GEXP( ( GLOG( a ) * (u8)b ) % 255 ) )

static	inline	s32	gadd( s32 a, s32 b ) { return( GADD( a, b ) ); }
static	inline	s32	gsub( s32 a, s32 b ) { return( GSUB( a, b ) ); }
static	inline	s32	gmul( s32 a, s32 b ) { return( GMUL( a, b ) ); }
static	inline	s32	gdiv( s32 a, s32 b ) { return( GDIV( a, b ) ); }
static	inline	s32	gpow( s32 a, s32 b ) { return( GPOW( a, b ) ); }

#if	defined( __cplusplus )
}
#endif

#endif
