/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2009-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	This header describes functions and variables for the RS-232
 *	port control module that is available to all program units
 ******************************************************************************/
#ifndef	PORT_H
#define	PORT_H

#if	defined( __cplusplus )
extern	"C"	{
#endif

#include	<common.h>

typedef	enum	PorSpd_e	{
	SPEED_300,
	SPEED_1200,
	SPEED_2400,
	SPEED_4800,
	SPEED_9600,
	SPEED_19200,
	SPEED_38400,
	MAX_SPEEDS
}	PorSpd_t;

extern	int	TrmFD;		/* RS-232 Terminal Port File Descriptor */
extern	char	Port_Name[ ];	/* RS-232 Terminal Port Name */

extern	void	Flow_Control( int off_on_flag );
extern	void	Port_Blocking_ON( void );
extern	void	Port_Blocking_OFF( void );
extern	int	Port_Init( PorSpd_t speed, uchar timeout );
extern	int	Port_Open( char * port, PorSpd_t speed, uchar timeout );
extern	int	Port_Read( char * buffer );
extern	int	Port_Send( char * buffer, char * fmt, u32 VLIST );
extern	void	Reset_Flow_Control( void );
extern	void	ErrMsg( char * str, ... );
extern	void	FtlMsg( char * str, ... );

#if	defined( __cplusplus )
}
#endif

#endif
