# -------------------------------------------------------------------------
#  boot.S
# -------------------------------------------------------------------------

#include "regdef.h"
#include "Settings.h"
	
# -------------------------------------------------------------------------
# Data segment
# -------------------------------------------------------------------------

#define	KERNEL_BOOT_STACK_SIZE		0x50000

		.data

kernel_boot_stack_low_limit:
	# MUST ALIGN THE STACK on a 2^2 = 4 byte word bondary!	
	.align 2   
        .space  KERNEL_BOOT_STACK_SIZE
kernel_boot_stack_high_limit:

memory_min:
	.align 2
		.space MEMORY_SIZE
memory_max:


# -------------------------------------------------------------------------
# Code segment
# -------------------------------------------------------------------------

	.text
	.set nomove

	.globl	_start
		
		
# -------------------------------------------------------------------------
# Startup code
#    This is where the system boots from
#     1. Load the stackpointer 'sp' with top of initial stack.
#     2. Jump to application-specific 'main' (defined in a C-file).
# -------------------------------------------------------------------------
_start:

	# Set the UART word length to 8 bits.
	
	li	t0, 0xB80003f8	# UART Base address
	lb	t1, 0x03(t0)	# Line Status register

	# Set word length to 8 bits (two least significant bits to 1), see TTY manual, page 14 and 15.
	
	ori	t1, t1, 0x03    
	sb	t1, 0x03(t0)


	la	sp, kernel_boot_stack_high_limit
	
	la	a0, memory_min
	li a1, MEMORY_SIZE
	
	j	OS
	
	