#ifndef _BITOPS_H
#define _BITOPS_H

/*
 * Copyright 1992, Linus Torvalds.
 *
 * $Id$
 */

#include	<linux/config.h>

/*
 * These have to be done with inline assembly: that way the bit-setting
 * is guaranteed to be atomic. All bit operations return 0 if the bit
 * was cleared before the operation and != 0 if it was not.
 *
 * bit 0 is the LSB of addr; bit 32 is the LSB of (addr+1).
 */

#define BIT_TRC_LVL	0x04000000

extern	void	set_bit( int nr, volatile void * addr );

extern	void	clear_bit( int nr, volatile void * addr );

extern	void	change_bit( int nr, volatile void * addr );

extern	int	test_and_set_bit( int nr, volatile void * addr );

extern	int	test_and_clear_bit( int nr, volatile void * addr );

extern	int	test_and_change_bit( int nr, volatile void * addr );

extern	int	test_bit( int nr, volatile void * addr );

extern	int	find_first_zero_bit( void * addr, unsigned size );

extern	int	find_next_zero_bit( void * addr, int size, int offset );

extern	u32	ffz( u32 word );

#endif	/* BITOPS_H */

