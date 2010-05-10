#include "IOHandler.h"

/**
 * putc(char c)
 * Output character c
 * @param char c - Character to output
 */
void putc(char c) {
	//syscall_putc(c);
	// Write character to Transmitter Holding Register
    while (!tty->lsr.field.thre);
    	tty->thr = c;
      
    if(c == '\n') { 
		while (!tty->lsr.field.thre);
		tty->thr = '\r';
    }	    	  
}

/**
 * puts(char* text)
 * Output string text
 * @param const char* text - String to output
 */
void puts(const char* text)
{
	//syscall_puts(&text);
	int i = 0;
	while(text[i] != '\0')
	{
		putc(text[i]);
    	i++;
	}
}

/**
 * putsln(char* text)
 * Output text with line-break
 * @param const char* text - String to output
 */
void putsln(const char* text) {
	puts(text);
	putc('\n');
}

/* display_word:
 *   Display a value on the Malta display.
 */
void displayWord(uint32_t word)
{
	int i;
	malta->ledbar.reg = 0xFF;
	
	for (i = 7; i >= 0; --i) {
    	malta->asciipos[i].value = '0' + word % 10;	
		word /= 10;
	}
}


/* bfifo_put: Inserts a character at the end of the queue. */
void bfifo_put(struct bounded_fifo* bfifo, uint8_t ch)
{
  /* Make sure the 'bfifo' pointer is not 0. */
  kdebug_assert(bfifo != 0);

  if (bfifo->length < FIFO_SIZE) {
    bfifo->buf[(bfifo->length)++] = ch;
  }
}

/* bfifo_get: Returns a character removed from the front of the queue. */
uint8_t bfifo_get(struct bounded_fifo* bfifo)
{
  int i;
  uint8_t ch;

  /* Make sure the 'bfifo' pointer is not 0, and that queue is not empty. */
  kdebug_assert(bfifo != 0);
  kdebug_assert(bfifo->length > 0);

  bfifo->length--;
  ch = bfifo->buf[0];
  for (i = 0; i < bfifo->length; i++) {
    bfifo->buf[i] = bfifo->buf[i+1];
  }
  return ch;
}

/**
 * initIO()
 * Initialize I/O
 */
void initIO() {
	// If there are any commands needed to be executed for IO to work, do them here...
	
	status_reg_t and, or;
	bfifo.length = 0;
	
	/* Set UART word length ('3' meaning 8 bits).
	* Do this early to enable debug printouts (e.g. kdebug_print).
	*/
	tty->lcr.field.wls = 3;
	
	/* Below is an alternative way of setting the status register,
	 * using the 'status_reg_t' type defined in 'mips4kc.h'.
	*/
	
	and.reg = 0xFFFFFFFF;
	and.field.exl = 0; // Normal level (not exception)
	and.field.erl = 0; // Error level
	and.field.um  = 0; // Kernel mode
	and.field.im  = 4; // Disable all except HW interrupt 0
	and.field.bev = 0; // Use normal exception vector (not bootsptrap)
 
	or.reg  = 0;
	or.field.ie   = 1;   // Enable interrupts
	or.field.im   = 4;   // Enable HW interrupt 0
	or.field.cu0  = 1;   // Coprocessor 0 usable

	kset_sr(and.reg, or.reg);
	
	// Generate interrupts when data is received by UART.
	tty->ier.field.erbfi = 1;

	// Some obscure bit that need to be set for UART interrupts to work. 
	tty->mcr.field.out2 = 1;
	
}
