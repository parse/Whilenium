#ifndef __IOHANDLER__
#define __IOHANDLER__

#include "mips/types.h"
#include "mips/malta.h"
	
#include "Structs.h"
#include "Process.h"
	
void putc(char c);
void puts(char* text);
void putsln(char* text);
char getc();
void displayC(uint8_t c, uint8_t pos);
void displayNumber(uint32_t word);
void putcDebug(char c);
void putsDebug(char* text);

void bfifo_put(struct bounded_fifo* bfifo, uint8_t ch, char output);
void bfifo_puts(struct bounded_fifo* bfifo, uint32_t s);
uint8_t bfifo_get(struct bounded_fifo* bfifo);

void initIO();

#endif
