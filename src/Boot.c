#include "Boot.h"


/**
 * main()
 * The main function takes no arguments and is the function that initiate the
 * first essential functions of the OS
 * This function will never reach its end, since run is a non terminating function.
 */
int main() {
	// Initiate the I/O-devices needed
	// Module: IOHandler
	initIO();
	
	// Initiate the OS essentials (including the first process/PCB)
	// Module: Process
	initOS();
	
	// Enable the interrupt bit and the interrupt handler
	// Module: Interrupt
	enableInterrupt();
	
	// Run the scheduler/OS
	// Module: Scheduler
	runScheduler();
	
	return 0;
}
