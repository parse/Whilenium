#ifndef __IOHANDLER__
#define __IOHANDLER__

#include "mips/types.h"
#include "mips/malta.h"
	
#include "Structs.h"
#include "Process.h"
	
void putc(char c);
void puts(const char* text);
void putsln(const char* text);
void displayWord(uint32_t word);

void bfifo_put(struct bounded_fifo* bfifo, uint8_t ch);
uint8_t bfifo_get(struct bounded_fifo* bfifo);

void initIO();

#endif
