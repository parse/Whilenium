#include "regdef.h"
	
	##############################################################################
	##
	## 		DESCRIPTION: Small kernel for SPIM, based on the 
	##		standard SPIM exception handler (exception.s). 
	##
	##############################################################################


	##############################################################################
	## Kernel Data Segment
	##############################################################################

		.data


	__m1_:	.asciiz "  Exception "
	__m2_:	.asciiz " occurred and ignored\n"
	__e0_:	.asciiz "  [Interrupt] "
	__e1_:	.asciiz	"  [TLB]"
	__e2_:	.asciiz	"  [TLB]"
	__e3_:	.asciiz	"  [TLB]"
	__e4_:	.asciiz	"  [Address error in inst/data fetch] "
	__e5_:	.asciiz	"  [Address error in store] "
	__e6_:	.asciiz	"  [Bad instruction address] "
	__e7_:	.asciiz	"  [Bad data address] "
	__e8_:	.asciiz	"  [Error in syscall] "
	__e9_:	.asciiz	"  [Breakpoint] "
	__e10_:	.asciiz	"  [Reserved instruction] "
	__e11_:	.asciiz	"  [11]"
	__e12_:	.asciiz	"  [Arithmetic overflow] "
	__e13_:	.asciiz	"  [Trap] "
	__e14_:	.asciiz	"  [14]"
	__e15_:	.asciiz	"  [Floating point] "
	__e16_:	.asciiz	"  [16]"
	__e17_:	.asciiz	"  [17]"
	__e18_:	.asciiz	"  [Coproc 2]"
	__e19_:	.asciiz	"  [19]"
	__e20_:	.asciiz	"  [20]"
	__e21_:	.asciiz	"  [21]"
	__e22_:	.asciiz	"  [MDMX]"
	__e23_:	.asciiz	"  [Watch]"
	__e24_:	.asciiz	"  [Machine check]"
	__e25_:	.asciiz	"  [25]"
	__e26_:	.asciiz	"  [26]"
	__e27_:	.asciiz	"  [27]"
	__e28_:	.asciiz	"  [28]"
	__e29_:	.asciiz	"  [29]"
	__e30_:	.asciiz	"  [Cache]"
	__e31_:	.asciiz	"  [31]"

	__excp:	.word __e0_, __e1_, __e2_, __e3_, __e4_, __e5_, __e6_, __e7_, __e8_, __e9_
		.word __e10_, __e11_, __e12_, __e13_, __e14_, __e15_, __e16_, __e17_, __e18_
		.word __e19_, __e20_, __e21_, __e22_, __e23_, __e24_, __e25_, __e26_, __e27_
		.word __e28_, __e29_, __e30_, __e31_


	__str_un_handled_interrupt:	.asciiz "\n\nNON-HANDLED INTERRUPT\n\n"


	# Beside the kernel reserved registers k0 and k1 we want to kernel to be able
	# to use the following:	

	__KERNEL_SAVED_v0:	.word 0
	__KERNEL_SAVED_a0:	.word 0
	__KERNEL_SAVED_at:	.word 0
	__KERNEL_SAVED_ra:	.word 0			


	# Data structures needed by the task-switcher.


	__TASK_SWITCH_FREQUENCY:	.word 100 # change to 1 for realy fast context switch...



	__CURRENT_TASK_NR:		.word 0

	__TASK_0_STATE:			.space 120
	__TASK_1_STATE:			.space 120

	__TASK_STATE_LIST:		.word __TASK_0_STATE, __TASK_1_STATE



	##############################################################################
	## Kernel Text Segment
	##############################################################################

		.text 0x80000000


		.set noat	# Turn off SPIM complaints about explicit use
					# of the AT register (used by pseudo instructions 
					# to translate labels to addresses).

		# Because we are running in the kernel, we can use k0/k1 without
		# saving their old values.

		# Before we use any labels (store, loads, load-address 
		# instructions) we must save the old AT value (and restore on
		# return to user space).

		# Cannot save AT using label without saving the wrong value beacuse
		# the address translation of the label uses AT. 

		move	k1, AT	# Save AT in the special kernel register k1
		.set at
		sw	k1, __KERNEL_SAVED_at

		#.set at			# Turn on SPIM complaints about use 
					# of the AT register.


		# Not re-entrent and we can't trust sp, but we need to use
		# these registers (if we are to use syscall, e.g., print_str).


		sw	v0, __KERNEL_SAVED_v0	
		sw	a0, __KERNEL_SAVED_a0

		# If use subrutin calls in the kernel.

		sw	ra, __KERNEL_SAVED_ra



		# Why are we entering the kernel.

		mfc0	k0, $13		# Cause register
		srl	a0, k0, 2			# Extract ExcCode Field (bits 2-6)
		andi	a0, a0, 0x1f	

		beqz	a0, __interrupt

	#-----------------------------------------------------------------------------
	# Exceptions
	#-----------------------------------------------------------------------------


		# Ok, it's an exception.

		# Print information about the exception.

		li	v0, 4		# syscall 4 (print_str)
		la	a0, __m1_
		syscall

		li	v0, 1		# syscall 1 (print_int)
		srl	a0, k0, 2	# Extract ExcCode Field
		andi	a0, a0, 0x1f
		syscall

		li	v0, 4		# syscall 4 (print_str)
		andi	a0, k0, 0x7c  # bit mask:	0111 1100
		lw	a0, __excp(a0)
		nop
		syscall


		li	v0, 10
		syscall

		# Bad PC exception requires special checks.

		bne	k0, 0x18, __ok_pc	
		nop

		mfc0	a0, $14	# EPC
		andi	a0, a0, 0x3	# Is EPC word-aligned?
		beq	a0, 0, __ok_pc
		nop

		li v0, 10		# Exit on really bad PC
		syscall

	__ok_pc:

		li	v0, 4		# syscall 4 (print_str)
		la	a0, __m2_
		syscall

		j __return_from_exception


	#-----------------------------------------------------------------------------
	# Interrupts
	#-----------------------------------------------------------------------------

	__interrupt:

		# What sort of interrupt is it?

		##########################################
	    ########## WRITE YOUR CODE HERE ##########
	    ##########################################
		# Is it a keyboard interrupt? - Do some stuff...
		# Is it a timer interrupt? - Context switch (do the run(mem) function)
		# Is it a console interrupt? - Output char
		
		# Not a timer interrupt.


		# NOTE:	In SPIM 7.0 it doesn't matter if the interrupt enable bit
		#	in the receiver control register is set or not, a receiver
		#	interrupt is issued anyway.
		#
		#	This seems to be fixed in SPIM 7.1
		
		# j	__return_from_interrupt


		li	v0, 4
		la	a0, __str_un_handled_interrupt
		syscall

		j	__return_from_interrupt


	__timer_interrupt:

		##########################################
		##########    CONTEXT SWITCH    ##########
		##########                      ##########
	    ########## WRITE YOUR CODE HERE ##########
	    ##########################################



		# Make sure to use this (and not __return_from_interrupt) to
		# return to the newly scheduled task.

		j __return_from_kernel




	#-----------------------------------------------------------------------------
	# Return from kernel to user space
	#-----------------------------------------------------------------------------

	__return_from_interrupt:

		j __restore_and_reset

	__return_from_exception:

		# Return from (non-interrupt) exception. Skip offending instruction
		# at EPC to avoid infinite loop.

		mfc0	k0, $14	# Bump EPC register
		addiu	k0, k0, 4	# Skip faulting instruction.
					# (Need to handle delayed branch case here)
		mtc0	k0, $14

	__restore_and_reset:

		# Restore registers and reset procesor state

		.set noat
		move	AT, k1	# Restore AT
		lw	AT, __KERNEL_SAVED_at
		.set at

		# Restore other registers.
		#sw k0, 
		#j __restore_task

		lw	v0, __KERNEL_SAVED_v0	
		lw	a0, __KERNEL_SAVED_a0

	__return_from_kernel:


		# Uses only zero and k0

		mtc0	zero, $13		# Clear Cause register

		mfc0	k0, $12		# Set Status register
		ori	k0, 0x1		# Interrupts enabled
		mtc0	k0, $12

		# Return from exception on MIPS32:

		# Reset timer.

		mtc0	zero, $9

		eret		# Return to instruction pointed to by EPC register. 
				# This automatically sets the exception level 
				# bit in the status register back to 0.



	#------------------------------------------------------------------------------
	# DESCRIPTION:  Saves the state of a task.
	#
	# INPUT:	k0 - Address to a  task STATE found in the .kdata segment. 
	#		      The size of the STATE is 120 bytes.
	#------------------------------------------------------------------------------
	__save_task:

		# Program counter.

		mfc0	k1, $14	# EPC (PC of the interrupted thread).
		sw	k1, 0(k0)	# Save PC.

		# Don't need to save zero

		lw	k1, __KERNEL_SAVED_at	# This register is used by the kernel.
		sw	k1,    4(k0)		

		lw	k1, __KERNEL_SAVED_v0  # This register is used by the kernel.
		sw	k1,   8(k0)		

		sw	v1,  12(k0)

		lw	k1, __KERNEL_SAVED_a0  # This register is used by the kernel.
		sw	k1,   16(k0) 

		sw	a1,   20(k0)	
		sw	a2,   24(k0)	
		sw	a3,   28(k0)	

		sw	t0,  32(k0)	
		sw	t1,  36(k0)	
		sw	t2,  40(k0)
		sw	t3,  44(k0)
		sw	t4,  48(k0)
		sw	t5,  52(k0)
		sw	t6,  56(k0)
		sw	t7,  60(k0)	

		sw	s0,  64(k0)   
		sw	s1,  68(k0)
		sw	s2,  72(k0)
		sw	s3,  76(k0)
		sw	s4,  80(k0)
		sw	s5,  84(k0)
		sw	s6,  88(k0)
		sw	s7,  92(k0)	

		sw	t8,  96(k0)	
		sw	t9, 100(k0)	

		# Don't need to save k0 and k1.

		sw	gp, 104(k0)	# gp
		sw	sp, 108(k0)	# sp
		sw	s8, 112(k0)	# s8

		lw	k1, __KERNEL_SAVED_ra	# This register is used by the kernel.
		sw	k1, 116(k0)	

		jr	ra

	#------------------------------------------------------------------------------
	# DESCRIPTION:  Restore the state of a task.
	#
	# INPUT:	k0 - Address to a  task STATE found in the .kdata segment. 
	#		      The size of the STATE is 120 bytes.
	#
	# NOTE:		After a call to this subroutine, you are only allowed 
	#		to  use register zero, k0 and k1.
	#------------------------------------------------------------------------------
	__restore_task:

		# Restore program counter.

		lw	k1, 0(k0)	# Restore PC and set EPC, the eret 
		mtc0	k1, $14	# instruction returns to this address when we 
					# return from kernel space to user space.

		# Don't need to restore zero register


		# After AT is restored, labels cannot be used in loads,
		# stores and load address instructions. Use of labels in
		# jump-instructions are ok.

		.set noat	
		lw	AT,   4(k0)		
		.set at

		lw	v0,   8(k0)		
		lw	v1,  12(k0)		

		lw	a0,  16(k0) 
		lw	a1,  20(k0)	
		lw	a2,  24(k0)	
		lw	a3,  28(k0)	

		lw	t0,  32(k0)	
		lw	t1,  36(k0)	
		lw	t2,  40(k0)
		lw	t3,  44(k0)
		lw	t4,  48(k0)
		lw	t5,  52(k0)
		lw	t6,  56(k0)
		lw	t7,  60(k0)	

		lw	s0,  64(k0)   
		lw	s1,  68(k0)
		lw	s2,  72(k0)
		lw	s3,  76(k0)
		lw	s4,  80(k0)
		lw	s5,  84(k0)
		lw	s6,  88(k0)
		lw	s7,  92(k0)	

		lw	t8,  96(k0)	
		lw	t9, 100(k0)	

		# Don't restore k0 and k1

		lw	gp, 104(k0)	# gp
		lw	sp, 108(k0)	# sp
		lw	s8, 112(k0)	# s8
		
		move	k1, ra	# Must use k1 as a substitue for  ra to be 
					# able to restore ra and  return from 
					# this subroutine without messing up ra.

		lw	ra, 116(k0)	# ra = PC

		jr	k1



	#------------------------------------------------------------------------------
	# STARTUP CODE:	Initialize the context switcher (scheduler)...
	#------------------------------------------------------------------------------

		.text
		.globl __enable_interrupts
	__enable_interrupts:

		# Enable interrupts.

		mfc0	t0, $12	# Get the value of the status register.
		ori	t0, 1		# Mask the first bit to one.
		mtc0	t0, $12	# Set the value of the status register.


		# Set timer interrupt frequency, i.e., the context switch frequency.

		lw	t0, __TASK_SWITCH_FREQUENCY
		mtc0	t0, $11


		##########################################
		#########      EXAMPLE TWO      ##########
		#########                       ##########
	    ######### UNCOMMENT THESE LINES ##########
	    ##########################################
		
		jr	ra
		
		.data
		.text	
