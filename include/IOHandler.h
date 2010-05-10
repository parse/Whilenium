#ifndef __IOHANDLER__
#define __IOHANDLER__

#include "mips/types.h"
#include "mips/malta.h"
	
#include "Structs.h"
#include "Process.h"
	
void putc(char c);
void puts(const char* text);
void putsln(const char* text);
void displayC(uint8_t c, uint8_t pos);
void displayNumber(uint32_t word);
void putc2(char c);
void puts2(const char* text);

void bfifo_put(struct bounded_fifo* bfifo, uint8_t ch);
uint8_t bfifo_get(struct bounded_fifo* bfifo);

void initIO();

#endif
