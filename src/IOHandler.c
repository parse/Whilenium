#include "IOHandler.h"

/**
 * putcDebug(char c)
 * Output character c, debug mode
 * @param char c - Character to output
 */
void putcDebug(char c) {
	// Write character to Transmitter Holding Register
    while (!tty->lsr.field.thre);
    	tty->thr = c;
      
    if(c == '\n') { 
		while (!tty->lsr.field.thre);
		tty->thr = '\r';
    }	   	  
}

/**
 * putsDebug(char* text)
 * Output string text, debug mode
 * @param const char* text - String to output
 */
void putsDebug(char* text)
{
	int i = 0;
	while(text[i] != '\0')
	{
		putcDebug(text[i]);
    	i++;
	}
}

void putslnDebug(char* text) {
	putsDebug(text);
	putcDebug('\n');
}

/**
 * putc(char c)
 * Output character c
 * @param char c - Character to output
 */
void putc(char c) {	
	syscall_put(&bFifoOut, c);
	
	if (c == '\n')
		syscall_put(&bFifoOut, '\r');
}

/**
 * puts(char* text)
 * Output string text
 * @param const char* text - String to output
 */
void puts(char* text)
{
	syscall_puts(&bFifoOut, text);
}

/**
 * putsln(char* text)
 * Output text with line-break
 * @param const char* text - String to output
 */
void putsln(char* text) {
	puts(text);
	putc('\n');
}

/**
 * getc()
 * If there is a char on the buffer it is returned, else while loop until char ready
 * @return The char on the buffer
 */
char getc() {
	while (bFifoIn.length == 0);
	
	return bfifo_get(&bFifoIn);
}

/* displayC(uint8_t word, uint8_t pos)
 * Display a char on the Malta display.
 * @param uint8_t word - Word to display
 * @param uint8_t pos - Position on the display
 */
void displayC(uint8_t word, uint8_t pos)
{
	malta->ledbar.reg = 0xFF;
	
    malta->asciipos[pos].value = word;
}

void displayS(uint32_t str, uint8_t offset) {
	int i;
	
	malta->ledbar.reg = 0xFF;
	for (i = 0; i < 8; i++) {
		malta->asciipos[i].value = str[(i+offset)%8];
	}
}

/* displayNumber(uint32_t word)
 * Display a value on the Malta display.
 * @param uint32_t word - Number to show on the Malta display
 */
void displayNumber(uint32_t word) {
	int i;
	malta->ledbar.reg = 0xFF;
		for (i = 7; i >= 0; --i) {
    		malta->asciipos[i].value = '0' + word % 10;	
			word /= 10;
	}
}

/*
 * bfifo_put(struct bounded_fifo* bfifo, uint8_t ch, char output)
 * Put ch into buffer bfifo
 * @param struct bounded_fifo* bfifo - Queue to use
 * @param uint8_t ch - Char to insert
 * @param char output - Output
 */
void bfifo_put(struct bounded_fifo* bfifo, uint8_t ch, char output) {
	/* Make sure the 'bfifo' pointer is not 0. */
	kdebug_assert(bfifo != 0);

	while (bfifo->length > FIFO_SIZE);
	
	bfifo->buf[(bfifo->length)++] = ch;

	if (output && tty->lsr.field.thre) {
		//Transmitter idle: transmit buffered character
		tty->thr = bfifo_get(bfifo);

		//Determine if we should be notified when transmitter becomes idle
		tty->ier.field.etbei = (bfifo->length > 0);
	}
}

/*
 * bfifo_puts(struct bounded_fifo* bfifo, uint32_t s)
 * Put string s into bfifo buffer
 * @param struct bounded_fifo* bfifo - Queue to use
 * @param uint32_t s - Char to insert
 */
void bfifo_puts(struct bounded_fifo* bfifo, uint32_t s) {
	char* str = (char*)s;
	int i = 0;
	
	while (str[i] != '\0') {
		bfifo_put(bfifo, str[i], 1);
		i++;
	}
}

/*
 * bfifo_get(struct bounded_fifo* bfifo)
 * Returns a character removed from the front of the queue.
 * @param struct bounded_fifo* bfifo - Queue to use
 * @return Character from the front of the queue
 */
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
	bFifoOut.length = 0;
	bFifoIn.length = 0;
	
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
