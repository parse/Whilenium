#include "regdef.h"
	
	.data

	.text
	
	.globl getPC
	.globl changeSP



changePC:
	# Set the current PC as output argument
	ori v0, a0, 0
	
	# Set the PC from input argument
	ori a0, a0, 0
	
	# Jump back
	jr 	ra

changeSP:
	# Get the previous stack pointer and store it as output argument
	ori v0, sp, 0
	
	# Set the stackpointer from the input argument
	ori	sp, a0, 0
	
	# Jump back
	jr 	ra
	