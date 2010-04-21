#include "Boot.h"


/**
 * main()
 * The main function takes no arguments and is the function that initiate the
 * first essential functions of the OS
 * This function will never reach its end, since run is a non terminating function.
 */
void main() {
	// Initiate the I/O-devices needed
	// Module: IOHandler
	extern void initIO();
	
	// Initiate the OS essentials (including the first process/PCB)
	// Module: Process
	extern void initOS();
	
	// Enable the interrupt bit and the interrupt handler
	// Module: Interrupt
	extern void enableInterrupt();
	
	// Run the scheduler/OS
	// Module: Scheduler
	extern void run();
	
	
	//--------------------------------------------------------
	
	
	// Test code to check in the registry while running simics
	int a;
	a = 3+3;
}
