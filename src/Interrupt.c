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

 	// Initialise timer to interrupt in 1 ms (simulated time).
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
	cause_reg_t cause;
	registers_t* reg;
	cause.reg = kget_cause();
	
	// Check if we are here because of UART interrupt (console in/out)
	if (cause.field.ip & 4) {
		uint8_t c;
		
		// Check if there is a char on the input
		if (tty->lsr.field.dr) {
			// Data ready: get character from UART
			c = tty->rbr;
			kInput(c); // Send the character to the IOQueue
			
			if (c == '\r') {
				kInput('\n');
			}
		}
		
		// If chars in output buffer, we should put the next char to output if transmitter idle
		if (bFifoOut.length > 0 && tty->lsr.field.thre) {
			//Transmitter idle: transmit buffered character
			tty->thr = bfifo_get(&bFifoOut);

			//Determine if we should be notified when transmitter becomes idle
			tty->ier.field.etbei = (bFifoOut.length > 0);
		}
		
		// Acknowledge UART interrupt and return.
		kset_cause(~0x1000, 0);
	} //Make sure that we are here because of a timer interrupt.
	else if ( cause.field.exc == 0 ) {
		// Increase the timer (for sleeps)
		timeCount++;
		
		// Schedule a new round
		run();
	} // Make sure we're here because of a syscall
	else if (cause.field.exc == 8) {
		/* Get pointer to stored registers. */
		reg = kget_registers();
		
		/* Return from exception to instruction following syscall. */
		reg->epc_reg += 4;

		/* Handle the system call (see syscall.S). */
		ksyscall_handler(reg);
		
		/* Acknowledge syscall exception. */
		kset_cause(~0x60, 0);
	}
}
