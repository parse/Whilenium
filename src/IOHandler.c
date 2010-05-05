#include "IOHandler.h"

/**
 * putc(char c)
 * Output character c
 * @param char c - Character to output
 */
void putc(char c) {
	// Write character to Transmitter Holding Register
    while (!tty->lsr.field.thre);
    	tty->thr = c;
      
    if(c=='\n'){ 
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
	int i=0;
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
	putc('\r');
}

/* display_word:
 *   Display a value on the Malta display.
 */
void display_word(uint32_t word)
{
	int i;
	malta->ledbar.reg = 0xFF;
	
	for (i = 7; i >= 0; --i) {
    	malta->asciipos[i].value = '0' + word % 10;	
		word /= 10;
	}
}


/**
 * initIO()
 * Initialize I/O
 */
void initIO() {
	// If there are any commands needed to be executed for IO to work, do them here...
	
	/* Set UART word length ('3' meaning 8 bits).
	* Do this early to enable debug printouts (e.g. kdebug_print).
	*/
	tty->lcr.field.wls = 3;
}
