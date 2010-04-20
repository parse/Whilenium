# -------------------------------------------------------------------------
#  boot.S
# -------------------------------------------------------------------------

#include "regdef.h"
	
# -------------------------------------------------------------------------
# Data segment
# -------------------------------------------------------------------------

#define	KERNEL_BOOT_STACK_SIZE		0x4000

		.data

kernel_boot_stack_low_limit:
        .space  KERNEL_BOOT_STACK_SIZE
kernel_boot_stack_high_limit:


			
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

	la	sp, kernel_boot_stack_high_limit
	j	main
