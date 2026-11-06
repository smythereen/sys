/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2009-2026 John R. Smythe Jr. */
/*******************************************************************************
 *
 *	Common Header File For All Units And/Or Programs
 *
 *		This file defines global structures, variables, and routines
 *		needed sortation server processing.
 ******************************************************************************/
#ifndef	COMMON_H
#define	COMMON_H

#if	defined( __cplusplus )
extern	"C"	{
#endif

#include	<ctype.h>
#include	<errno.h>
#include	<float.h>
#include	<stdarg.h>
#include	<stdint.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/errno.h>
#include	<sys/types.h>
#include	<time.h>
#include	<sys/time.h>
#include	<sched.h>
#include	<unistd.h>
#include	<linux/types.h>

#define linux 1

#ifndef offsetof
#define offsetof(type, member) ((u_t) &((type *)0)->member)
#endif

#ifndef delta_of
#define delta_of( T, B, E ) ( (u_t)( &((T *)0)->E ) - (u_t)( &((T *)0)->B ) )
#endif

/*
 *	Common Data Types
 */

#ifndef	UCHAR_DEFINED
	typedef	u_int8_t	uchar;
#define	UCHAR_DEFINED
#endif

#define	xN(N, MODE)\
	typedef unsigned int	u##N	__attribute__((__mode__( __##MODE##I__ )));\
	typedef   signed int	s##N	__attribute__((__mode__( __##MODE##I__ )))

	xN(8,Q);
	xN(16,H);
	xN(32,S);
	xN(64,D);
	xN(128,T);

#undef	xN

#define	fN(N, MODE)\
	typedef		float	f##N	__attribute__((__mode__( __##MODE##F__ )))

	fN(32,S);
	fN(64,D);
//	fN(128,X);
//	fN(128,T);

#undef	fN

	typedef	intptr_t			s_t;	/* signed (void *) equivalent */
	typedef	uintptr_t			u_t;	/* unsigned (void *) equivalent */

/*
 *	Larger Floats: only the ones this machine really has
 *
 *	f80	x87 extended (64 bit mantissa): long double on x86
 *	f128	IEEE quad (113 bit mantissa): __float128, or long double
 *		where long double is quad (aarch64)
 *	f_t	largest real float available: f128, else f80, else f64
 *
 *	F80_MANT_DIG / F128_MANT_DIG are defined only when the type is.
 */

#if	( defined( __i386__ ) || defined( __x86_64__ ) ) && LDBL_MANT_DIG == 64
	typedef	long		double		f80;	/* x87 extended */
#define	F80_MANT_DIG	LDBL_MANT_DIG
#endif

#if	LDBL_MANT_DIG == 113
	typedef	long		double		f128;	/* IEEE quad */
#define	F128_MANT_DIG	LDBL_MANT_DIG
#elif	defined( __SIZEOF_FLOAT128__ )
	typedef	__float128			f128;	/* IEEE quad */
#define	F128_MANT_DIG	113
#endif

#if	defined( F128_MANT_DIG )
	typedef	f128				f_t;	/* Largest Float */
#define	F_T_MANT_DIG	F128_MANT_DIG
#elif	defined( F80_MANT_DIG )
	typedef	f80				f_t;	/* Largest Float */
#define	F_T_MANT_DIG	F80_MANT_DIG
#else
	typedef	f64				f_t;	/* Largest Float */
#define	F_T_MANT_DIG	DBL_MANT_DIG
#endif

	typedef	struct	timeval	stamp_t;


/*
 *	Sizes Of Things
 */

#define	MAX_PATH		256	/* Maximum File & Path Length */
#define	MAX_EVT_LEN		256	/* Maximum Message Length In Bytes */

/*
 *	Common Global Data
 */

extern	char *	EventLogFilePath;			/* Pointer to Log File Path */
extern	u32	TraceLevel;				/* Current Level Of Tracing */
extern	u32 *	TracePointer;				/* Current Level Of Tracing */
extern	char *	LocalName;				/* Local Process Name For Trace */
extern	char 	PrintName;				/* Print Process Name Boolean For Trace */
extern	s32 	PrintStamp;				/* Print Time Stamp Boolean For Trace */
extern	char 	my_name[ ];				/* Process Message System Name For Trace */
extern	int	EventLevel;				/* Event Level */

extern	int	I_Am_Child;
extern	int	ChildrenProcesses;
extern	pid_t	ChildProcess;
extern	int	ChildStatus;

extern	pid_t		get_stat_pid;
extern	char	*	get_stat_comm;
extern	char	*	get_stat_state;
extern	pid_t		get_stat_ppid;
extern	pid_t		get_stat_pgrp;
extern	pid_t		get_stat_session;
extern	int		get_stat_tty_nr;
extern	pid_t		get_stat_tpgid;
extern	u32		get_stat_flags;

extern	int		get_stat_result;

/*
 *	Common Macro Definitions
 */

static	inline	__attribute__((always_inline))	int	Is_Trace_Level( u32 x )
{
	return( ( x ) == 0 || ( ( x ) & ( TracePointer != 0 ? *TracePointer : TraceLevel ) ) != 0 );
}

#define	If_Trace_Level( x )	if( Is_Trace_Level( x ) )
#define	If_Not_Trace_Level( x )	if( !Is_Trace_Level( x ) )

#define	EventLogAP	(*Event_LogAP)
#define	LOGIT		Trace
#define	SYS_TRC_LVL	0x80000000	/* SYS Unit's Trace Level */
#define	MAX_QUIT_ROUTINES	10

#define	BIT_BYTE(x)	( ( (unsigned)x ) >> 3 )
#define	BIT_NUM(x)	( ( (unsigned)x )  & 7 )
#define	BIT_TST(s,x)	( ( ( s[ BIT_BYTE( x ) ] ) >>  BIT_NUM( x ) ) & 0x01 )
#define	BIT_SET(s,x)	(   ( s[ BIT_BYTE( x ) ] ) |=  ( 1 << BIT_NUM( x ) ) )
#define	BIT_CLR(s,x)	(   ( s[ BIT_BYTE( x ) ] ) &= ~( 1 << BIT_NUM( x ) ) )

typedef enum {
	EVENT_NONE,
	EVENT_CRITICAL,
	EVENT_MAJOR,
	EVENT_MINOR,
	EVENT_INFO,
	EVENT_CLEARED
}	event_e;

/*
 *	Common Global Function Prototypes
 */

int	Add_Line(	char * file,	char * fmt, ...		);

int	Check_Func_Children(		void			);
pid_t	CheckChildren(			void			);
void	ChildExit(			int exit_status_value	);

void	ErrorLog(			char * fmt, ...		);
void	EventLevelLog(	int level,	char * fmt, ...		);
void	EventLog(			char * fmt, ...		);
extern	char*	(*Event_LogAP)(		char * fmt, va_list ap	);

int	Exec_AP(			char ** ap		);
int	Exec(				char * program, ...	);

void	FatalError(			char * fmt, ...		);
void	FatalErrorAP(			char * fmt, va_list ap	);

int	FlushChildren(			void			);
int	Fork(				void			);
int	ForkFunction(			void (*func)( char * fmt, va_list ap ), char * fmt, ... );
int	Fork_AP_Func(			int (*func)( va_list ap ),  void (*death)( pid_t pid, int status ), ... );

int	Initialize_Print(		char * tracefile	);

int	MkDir(				const char * pathname, mode_t mode );
int	Reset_Print(			void			);
void	New_Err_File(			void			);

#define Spawn( a... )	Fork_AP_Func( ( void (*)( ) )Exec_AP, ##a )

void	SysPrt(				char * fmt, ...		);
void	SysPrtAP(			char * fmt, va_list ap	);

int	System(		char * cmd,	...			);
int	SystemAP(	char * cmd,	va_list ap		);

void	SystemError(	char * func,	char * fmt, ...		);
void	SystemErrorAP(	char * func,	char * fmt, va_list ap	);

void	SystemWarn(	char * func,	char * fmt, ...		);
void	SystemWarnAP(	char * func,	char * fmt, va_list ap	);

int	Write_Text(	int FD, char * line, ...        );
int	Write_TextAP(	int FD, char * line, va_list ap );

char *	Name_Translate(	char * name );

char *	Get_Path(	char * buf, int size, char * pth );

char *	File_Path(	char * buf, int size, char * env, char * file, ... );

char * 	strupr( char * );
char * 	strlwr( char * );

extern	int	AddQuitRoutine( void (*quit)( void ) );
extern	void	CallItQuits( void );

extern	int	fixed2long( char * numeric_string, s32 * value, int numchars );
extern	char *	Trim( char * string );

extern	int	get_stat( pid_t pid_to_stat );
extern	int	lineage( pid_t pid_to_stat );

static	inline	__attribute__((always_inline))	void	TraceAP( u32 level, char * fmt, va_list ap )
{

	If_Trace_Level( level ) SysPrtAP( fmt, ap );
}

#define Trace( level, ... )	{ If_Trace_Level( level ) SysPrt( __VA_ARGS__ ); }

#if	defined( __cplusplus )
}
#endif

#endif
