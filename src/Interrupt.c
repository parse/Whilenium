#include "Interrupt.h"

/**
 * enableInterrupt()
 * Enables interrupt handling in the registry
 */
void enableInterrupt() {
	__enable_interrupts();
	
	putc('a');
	putc('b');
	putc('c');
}
