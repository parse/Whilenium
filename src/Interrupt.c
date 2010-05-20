#include "Interrupt.h"

/**
 * enableInterrupt()
 * Enables interrupt handling in the registry
 */
void enableInterrupt() {
	int i;
	for (i = 0; i < 8; i++) {
		displayC(' ', i);	
	}

 	// Initialise timer to interrupt in 100 ms (simulated time).
 	kload_timer(1 * timer_msec);

 	// Update the status register to enable timer interrupts.
 	kset_sr(0xFFBF00E8, 0x10008001);
	
	if (DEBUG)
		putslnDebug("enableInterrupt: interruptsEnabled = 1");
		
	interruptsEnabled = 1;
	//putsln("enableInterrupt(): Interrupts are now enabled!\n\n");
}

/* kexception:
 *   Application-specific exception handler, called after registers
 *   have been saved.
 */
void kexception()
{
	//static int i = 0;
	cause_reg_t cause;
	registers_t* reg;
	cause.reg = kget_cause();
	
	timeCount++;
	
	// Check if we are here because of UART interrupt	
	if (cause.field.ip & 4) {
		uint8_t c;
		
		if (tty->lsr.field.dr) {
			// Data ready: add character to buffer
			c = tty->rbr;
			bfifo_put(&bFifoIn, c, 0);
			//bfifo_put(&bFifoOut, c, 1);
			if (c == '\r') {
				bfifo_put(&bFifoIn, '\n', 0);
				//bfifo_put(&bFifoOut, '\n', 1);
			}
		}
		
		if (bFifoOut.length > 0 && tty->lsr.field.thre) {
			//Transmitter idle: transmit buffered character
			tty->thr = bfifo_get(&bFifoOut);

			//Determine if we should be notified when transmitter becomes idle
			tty->ier.field.etbei = (bFifoOut.length > 0);
		}
		
		// Acknowledge UART interrupt.
		kset_cause(~0x1000, 0);
	} //Make sure that we are here because of a timer interrupt.
	else if ( cause.field.exc == 0 ) {
			run();
			/* Reload timer for another 100 ms (simulated time) */
			//kload_timer(1 * timer_msec);
	} // Make sure we're here because of a syscall
	else if (cause.field.exc == 8) {
		/* Get pointer to stored registers. */
		reg = kget_registers();

		/* Handle the system call (see syscall.S). */
		ksyscall_handler(reg);

		/* Return from exception to instruction following syscall. */
		reg->epc_reg += 4;

		/* Acknowledge syscall exception. */
		kset_cause(~0x60, 0);
		/*
		// Get state of running process and get a new PCB if current is Terminated. Reload the timer to reset the next timer interrupt.
		State prevState = getPrevState();
		if (prevState == Terminated) {
			if (DEBUG)
				putslnDebug("kexception: Terminated!");
				
			
			run();
			
			// Reload timer for another 100 ms (simulated time) 
			kload_timer(1 * timer_msec);
		}*/
	}
}
