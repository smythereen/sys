/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	This file defines the following functions.
 *		get_stat( );
 ******************************************************************************/
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<common.h>

	pid_t		get_stat_pid;
	char	*	get_stat_comm;
	char	*	get_stat_state;
	pid_t		get_stat_ppid;
	pid_t		get_stat_pgrp;
	pid_t		get_stat_session;
	int		get_stat_tty_nr;
	pid_t		get_stat_tpgid;
	u32		get_stat_flags;
	int		get_stat_result;

static	char		buf[ 2048 ];
static	char	*	w[ 64 ];

int	get_stat( pid_t c )
{
	int	proc;
	int	i;
	char *	cp;
	char	file[ MAX_PATH ];

	if( c <= 0 ) c = getpid( );
	sprintf( file, "/proc/%d/stat", c );
	memset( buf, 0, sizeof( buf ) );

	if( ( proc = open( file, O_RDONLY ) ) < 0 ||
	    read( proc, buf, sizeof( buf  ) ) <= 0 ) {
		Trace( 0, "get_stat( ) ERROR Cannot Open File %s For Reading", file );
		return( -1 );
	}
	close( proc );

	memset( w, 0, sizeof( w ) );
	for( i = 1, cp = buf, w[ 0 ] = buf; *cp != 0; cp++ ) {
		if( !isspace( *cp ) ) continue;
		*cp = 0;
		w[ i++ ] = &cp[ 1 ];
	}

	i = 0;
	if( w[i] != NULL ) get_stat_pid		= strtol( w[i++], 0, 10 );
	if( w[i] != NULL ) get_stat_comm	= w[i++];
	if( w[i] != NULL ) get_stat_state	= w[i++];
	if( w[i] != NULL ) get_stat_ppid	= strtol( w[i++], 0, 10 );
	if( w[i] != NULL ) get_stat_pgrp	= strtol( w[i++], 0, 10 );
	if( w[i] != NULL ) get_stat_session	= strtol( w[i++], 0, 10 );
	if( w[i] != NULL ) get_stat_tty_nr	= strtol( w[i++], 0, 10 );
	if( w[i] != NULL ) get_stat_tpgid	= strtol( w[i++], 0, 10 );
	if( w[i] != NULL ) get_stat_flags	= strtoul( w[i++], 0, 10 );

	return( ( get_stat_result = i ) );
}

