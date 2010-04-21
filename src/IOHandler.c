#include "IOHandler.h"

static volatile tty_t* const tty = (tty_t*) 0xb80003f8;

void putc(char c) {
  // Poll until ready to transmit.
  while ( !tty->lsr.thre ) {}		

  // Write character to Transmitter Holding Register
  tty->thr = c;		    
	  
}

void puts(const char* text) {
  while (text[0] != '\0') {
    putc(text[0]);
    ++text;
  }
}

/**
 * initIO()
 * Initiates the IO-devices needed at the start of the OS, such as a terminal for output to the user
 */
void initIO() {
	putc('a');
}
