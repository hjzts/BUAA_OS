#include <asm/asm.h>

LEAF(msyscall)
	// Just use 'syscall' instruction and return.

	/* Exercise 4.1: Your code here. */
	syscall
	jr ra
	// 在include/asm/regdef.h中
	// #define ra $31 /* return address */ 

END(msyscall)
