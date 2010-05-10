# -------------------------------------------------------------------------
#  boot.S
# -------------------------------------------------------------------------

#include "mips/regdef.h"
#include "mips/cp0_regdef.h"
#include "mips/asm_regs.h"
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

# --------- ADDED FROM EXAMPLE FILES! ---------

#define	INIT_STACK_SIZE		0x8000
#define	EXCN_STACK_SIZE		0x8000

	.data

excn_regs:  .word	0
cause_reg:  .word	0

	.space	INIT_STACK_SIZE
init_stack:
	.space	EXCN_STACK_SIZE
excn_stack:

# --------- END OF ADD ----------------

# -------------------------------------------------------------------------
# Code segment
# -------------------------------------------------------------------------

.text
.set nomove

.globl	_start

# --------- ADDED FROM EXAMPLE FILES! ---------
.globl  kget_sr
.globl  kset_sr
.globl  kget_cause
.globl  kset_cause
.globl  kload_timer
.globl  kset_registers
.globl  kget_registers
.globl  kexception
.globl	kdebug_magic_break		
.globl	my_system_call
.globl  syscall_die
.globl	kmy_system_call
.globl 	ksyscall_handler
# --------- END OF ADD ----------------

# -------------------------------------------------------------------------
# Set cpu status register
#  a0 - and mask (clear sr-bits not set in a0)
#  a1 - or mask (set sr-bits set in a1)
# -------------------------------------------------------------------------

kset_sr:
	mfc0 v0, sr		#loads cp0's status reg
	and  v0, v0, a0
	or   v0, v0, a1
	mtc0 v0, sr
	jr   ra

# -------------------------------------------------------------------------
# Get cpu status register
# -------------------------------------------------------------------------

kget_sr:
	mfc0 v0, sr		#loads cp0's status reg
	jr   ra

# -------------------------------------------------------------------------
# Set cause register
#  a0 - and mask (clear cause-bits not set in a0)
#  a1 - or mask (set cause-bits set in a1)
# -------------------------------------------------------------------------

kset_cause:
	mfc0 v0, cause		#loads cp0's cause reg
	and  v0, v0, a0
	or   v0, v0, a1
	mtc0 v0, cause
	jr   ra

# -------------------------------------------------------------------------
# Get cause register
# -------------------------------------------------------------------------

kget_cause:
	lw   v0, cause_reg(zero)
	jr   ra

# -------------------------------------------------------------------------
# Load timer with an interval
# PARAMETER: a0 - timer compare value
# -------------------------------------------------------------------------

kload_timer:
	mtc0 zero, count	#loads timer
	mtc0 a0, compare	#loads time for next timer-intr
	jr   ra

# -------------------------------------------------------------------------
# Set register area used by exception handler
# -------------------------------------------------------------------------
kset_registers:
	sw a0,excn_regs
	jr ra

# -------------------------------------------------------------------------
# Get register area used by exception handler
# -------------------------------------------------------------------------
kget_registers:
	lw v0,excn_regs
	jr ra

# -------------------------------------------------------------------------
# kdebug_magic:
#   Executes a magic instruction, this will stop simics simulation if
#   preceded by the 'magic-break-enable' simics command.
# -------------------------------------------------------------------------
kdebug_magic_break:
	li	zero,0	# This is the simics magic break instruction for MIPS
	jr	ra

# -------------------------------------------------------------------------
# Startup code
#    This is where the system boots from
#     1. Copy 'exceptstub' to the MIPS exception vector
#     2. Load the stackpointer 'sp' with top of initial stack.
#     3. Load 'gp' with 0x80000000 (needed by the compiler).
#     4. Jump to application-specific initialisation 'OS'.
# -------------------------------------------------------------------------

copyloop:
	lw v0,(t0)		#copy one word at a time
	sw v0, (t1)
	addu t0, 4
	addu t1, 4
	subu t2, 4
	bgez t2, copyloop

	la gp, 0x80000000
	la sp,init_stack-32
	jr	ra
	#j  OS

# -------------------------------------------------------------------------
# On exception: jump to exception handler.
# This stub is copied to the exception handler address 0xa0000180,
# it calls 'exception_handler' below when an exception occurs.
# -------------------------------------------------------------------------

	.set noreorder
exceptstub:
	.set noat
	addi k1,$at,0		  #preserve the $at register
	.set at
	mfc0 k0, cause
	sw k0, cause_reg(zero)	  #preserve the cause register
	la k0, exception_handler  #jumps to correct segment
	jr k0
	nop
exceptstubend:
	.set reorder


# -------------------------------------------------------------------------
# The exception handler
#   Calls 'kexception' when registers are saved and
#   stack pointer is loaded.
#
# PARAMETER: k1 - preserved $at register
# -------------------------------------------------------------------------

exception_handler:
	lw k0, excn_regs(zero)
	sw k1, REG_AT(k0)
	sw v0, REG_V0(k0)
	sw v1, REG_V1(k0)
	sw a0, REG_A0(k0)
	sw a1, REG_A1(k0)
	sw a2, REG_A2(k0)
	sw a3, REG_A3(k0)
	sw t0, REG_T0(k0)
	sw t1, REG_T1(k0)
	sw t2, REG_T2(k0)
	sw t3, REG_T3(k0)
	sw t4, REG_T4(k0)
	sw t5, REG_T5(k0)
	sw t6, REG_T6(k0)
	sw t7, REG_T7(k0)
	sw t8, REG_T8(k0)
	sw t9, REG_T9(k0)
	sw s0, REG_S0(k0)
	sw s1, REG_S1(k0)
	sw s2, REG_S2(k0)
	sw s3, REG_S3(k0)
	sw s4, REG_S4(k0)
	sw s5, REG_S5(k0)
	sw s6, REG_S6(k0)
	sw s7, REG_S7(k0)
	sw sp, REG_SP(k0)
	sw fp, REG_FP(k0)
	sw gp, REG_GP(k0)
	sw ra, REG_RA(k0)
		
	.set noreorder
	mfc0 k1, epc
	nop
	nop
	nop
	sw k1, REG_EPC(k0)
	.set reorder

	la sp, excn_stack-32
	jal kexception

	lw k0, excn_regs(zero)
	lw v0, REG_V0(k0)
	lw v1, REG_V1(k0)
	lw a0, REG_A0(k0)
	lw a1, REG_A1(k0)
	lw a2, REG_A2(k0)
	lw a3, REG_A3(k0)
	lw t0, REG_T0(k0)
	lw t1, REG_T1(k0)
	lw t2, REG_T2(k0)
	lw t3, REG_T3(k0)
	lw t4, REG_T4(k0)
	lw t5, REG_T5(k0)
	lw t6, REG_T6(k0)
	lw t7, REG_T7(k0)
	lw t8, REG_T8(k0)
	lw t9, REG_T9(k0)
	lw s0, REG_S0(k0)
	lw s1, REG_S1(k0)
	lw s2, REG_S2(k0)
	lw s3, REG_S3(k0)
	lw s4, REG_S4(k0)
	lw s5, REG_S5(k0)
	lw s6, REG_S6(k0)
	lw s7, REG_S7(k0)
	lw sp, REG_SP(k0)
	lw fp, REG_FP(k0)
	lw gp, REG_GP(k0)
	lw ra, REG_RA(k0)
	lw k1, REG_EPC(k0)
	lw k0, REG_AT(k0)

	.set noreorder
	mtc0 k1, epc
	.set noat
	addi $at, k0, 0		# restore AT
	nop
	nop
 	nop
	eret
	.set at
	.set reorder

my_system_call:
	la v0,kmy_system_call	# Load adress of syscall implementation in v0.
	syscall			# Enter kernel mode by 'syscall' exception.
	jr ra			# Back in user mode, return to caller

# -------------------------------------------------------------------------
# Redirects our syscall to the correct function in Scheduler.c
# -------------------------------------------------------------------------
syscall_die:
	la v0, die
	syscall
	jr ra

# syscall_handler:
#   Enables argument passing for up
#   to four parameters of a system call.
#
# Input:
#   a0 - Pointer to saved registers.
#
ksyscall_handler:
	addi	v0, a0, 0	# Move a0 into v0.
	lw	a0, REG_A0(v0)	# Load saved a0 (argument 1) into a0.
	lw	a1, REG_A1(v0)	# Load saved a1 (argument 2) into a1.
	lw	a2, REG_A2(v0)	# Load saved a2 (argument 3) into a2.
	lw	a3, REG_A3(v0)	# Load saved a3 (argument 4) into a3.
	lw	v0, REG_V0(v0)	# Load saved v0 (implementation) into v0.

	jr	v0		# Jump to system call implementation.

	
# -------------------------------------------------------------------------
# Startup code
#    This is where the system boots from
#     1. Load the stackpointer 'sp' with top of initial stack.
#     2. Jump to application-specific 'main' (defined in a C-file).
# -------------------------------------------------------------------------
_start:	
	la t0, exceptstub
	la t2, exceptstubend
	la t1, 0xa0000180
	subu t2, t2, t0	#calculate length of handler

	jal	copyloop

	la	sp, kernel_boot_stack_high_limit

	la	a0, memory_min
	li a1, MEMORY_SIZE

	j	OS


