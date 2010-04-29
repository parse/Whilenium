#include "IOHandler.h"

/**
 * putc(char c)
 * Output character c
 * @param char c - Character to output
 */
void putc(char c) {
	// Poll until ready to transmit.
	while ( !tty->lsr.thre ) {}		

	// Write character to Transmitter Holding Register
	tty->thr = c;		    	  
}

/**
 * puts(char* text)
 * Output string text
 * @param const char* text - String to output
 */
void puts(const char* text) {
	while (text[0] != '\0') {
		putc(text[0]);
    	++text;
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

/**
 * initIO()
 * Initialize I/O
 */
void initIO() {
	/* 
	TODO: 
		* Interrupt driven I/O
		* Handle input
	*/
}
