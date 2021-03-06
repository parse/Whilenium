#include "Boot.h"
static registers_t regs;

/**
 * main()
 * The main function takes no arguments and is the function that initiate the
 * first essential functions of the OS
 * This function will never reach its end, since run is a non terminating function.
 */
int OS(int memoryMin) {
	// Interrupts are disabled
	interruptsEnabled = 0;
	
	// Initiate spot for registers to be saved
 	kset_registers(&regs);
	
	// Initiate User Program tables
	initUserPrograms();
	
	// Initiate the I/O-devices needed
	// Module: IOHandler
	initIO();
		
	// Initiate the OS essentials (including the first process/PCB)
	// Module: Process
	initOS(memoryMin);
	
	initScheduler(&regs);
	
	// Enable the interrupt bit and the interrupt handler
	// Module: Interrupt
	enableInterrupt();
		
	while (1);
	
	return memoryMin;
}
