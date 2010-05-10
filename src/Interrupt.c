#include "Interrupt.h"

/**
 * enableInterrupt()
 * Enables interrupt handling in the registry
 */
void enableInterrupt() {
 	displayWord(1);

 	// Initialise timer to interrupt in 100 ms (simulated time).
 	kload_timer(1 * timer_msec);

 	// Update the status register to enable timer interrupts.
 	kset_sr(0xFFBF00E8, 0x10008001);
	
	putsln("enableInterrupt(): Interrupts are now enabled!\n\n");
}

/* Kernels internal definition of my system call (prefix 'k'). */
/*void syscall_display(uint32_t v)
{
  // Implementation of my_system_call:
  // Displays value of its argument.  
  displayWord(v);
}*/

/* kexception:
 *   Application-specific exception handler, called after registers
 *   have been saved.
 */
void kexception()
{
	static int i = 0;
	cause_reg_t cause;
	registers_t* reg;
	
	cause.reg = kget_cause();
	
	//Make sure that we are here because of a timer interrupt.
	if ( cause.field.exc == 0 ) {
		run();
		
		/* Reload timer for another 100 ms (simulated time) */
		kload_timer(1 * timer_msec);

		// Increase marta
		displayWord(++i);
		
	// Make sure we're here because of a syscall
	} else if (cause.field.exc == 8) {
		/* Get pointer to stored registers. */
		reg = kget_registers();

		/* Handle the system call (see syscall.S). */
		ksyscall_handler(reg);
		
		

		/* Return from exception to instruction following syscall. */
		reg->epc_reg += 4;

		/* Acknowledge syscall exception. */
		kset_cause(~0x60, 0);
		
		// Get state of running process and get a new PCB if current is Terminated. Reload the timer to reset the next timer interrupt.
		State prevState = getPrevState();
		if (prevState == Terminated) {
			putsln("DO ANOTHER RUN!");
			
			/* Reload timer for another 100 ms (simulated time) */
			kload_timer(1 * timer_msec);
			
			run();
		}
	}
}
