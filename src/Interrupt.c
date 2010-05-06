#include "Interrupt.h"

#include "UserPrograms.h"
#include "stdlib.h"
/**
 * enableInterrupt()
 * Enables interrupt handling in the registry
 */
void enableInterrupt() {
 	display_word(1);

 	/* Initialise timer to interrupt in 100 ms (simulated time). */
 	kload_timer(1 * timer_msec);

 	/* Update the status register to enable timer interrupts. */
 	kset_sr(0xFFBF00E8, 0x10008001);
	
	putsln("enableInterrupt(): Interrupts are now enabled!\n\n");
}

/* kexception:
 *   Application-specific exception handler, called after registers
 *   have been saved.
 */
void kexception()
{
	static int i = 0;
	cause_reg_t cause;
	
	//Make sure that we are here because of a timer interrupt.
	cause.reg = kget_cause();
	
	kdebug_assert(cause.field.exc == 0);	/* External interrupt */
	kdebug_assert(cause.field.ip & 0x80);   /* Timer interrupt */
	
	run();
	
 	/* Reload timer for another 100 ms (simulated time) */
	kload_timer(1 * timer_msec);
	
	// Inclease marta
	display_word(++i);
}