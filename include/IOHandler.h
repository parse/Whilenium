#ifndef __IOHANDLER__
#define __IOHANDLER__

#include "mips/types.h"
#include "mips/malta.h"
	
#include "Structs.h"
#include "Process.h"

	
void putc(char c);
void puts(const char* text);
void putsln(const char* text);

void display_word(uint32_t word);

void initIO();


#endif
