#include "Boot.h"
static registers_t regs;

/**
 * main()
 * The main function takes no arguments and is the function that initiate the
 * first essential functions of the OS
 * This function will never reach its end, since run is a non terminating function.
 */
int OS(int memoryMin) {
	
	// Initiate the I/O-devices needed
	// Module: IOHandler
	initIO();
		
	// Initiate the OS essentials (including the first process/PCB)
	// Module: Process
	initOS(memoryMin);
	
	// Enable the interrupt bit and the interrupt handler
	// Module: Interrupt
	enableInterrupt();
	
	while (1);
	
	// Run the scheduler
	// Module: Scheduler
	//HelloWorld();
	//run(memoryMin);
	
	return memoryMin;
}

/* kexception:
 *   Application-specific exception handler, called after registers
 *   have been saved.
 */
void kexception()
{
	/*
  static int i = 0;
  cause_reg_t cause;
*/
  /* Make sure that we are here because of a timer interrupt. */
 /* cause.reg = kget_cause();
  kdebug_assert(cause.field.exc == 0);  */  /* External interrupt */
 /* kdebug_assert(cause.field.ip & 0x80);*/   /* Timer interrupt */

  /* Reload timer for another 100 ms (simulated time) */
 /* kload_timer(100 * timer_msec);*/
}
