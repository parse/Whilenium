#include "Interrupt.h"

/* example - function that prints a string to the terminal window  */
static void printstr(char print_array[])
{
  int i=0;
  while(print_array[i] != '\0')
    {
      while (!tty->lsr.field.thre);
      tty->thr = print_array[i];
      
      if(print_array[i]=='\n'){ 
	while (!tty->lsr.field.thre);
	tty->thr = '\r';
      }
      i++;
    }
}

/**
 * enableInterrupt()
 * Enables interrupt handling in the registry
 */
void enableInterrupt() {
 	display_word(1);
	printstr("\n\nSex\n\n");


 	/* Initialise timer to interrupt in 100 ms (simulated time). */
 	kload_timer(100 * timer_msec);

 	/* Update the status register to enable timer interrupts. */
 	kset_sr(0xFFBF00E8, 0x10008001);
	
	putc('a');
	putc('b');
	putc('c');
}

/* kexception:
 *   Application-specific exception handler, called after registers
 *   have been saved.
 */
void kexception()
{
	cause_reg_t cause;

	//Make sure that we are here because of a timer interrupt.
	cause.reg = kget_cause();
	
	kdebug_assert(cause.field.exc == 0);	/* External interrupt */
	kdebug_assert(cause.field.ip & 0x80);   /* Timer interrupt */
	
 	/* Reload timer for another 100 ms (simulated time) */
	kload_timer(100 * timer_msec);
}