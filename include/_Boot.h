#ifndef __ASM_SYSCALL__
#define __ASM_SYSCALL__

#include "Structs.h"

void ksyscall_handler(registers_t* reg);
void syscall_die();
void syscall_display_c(uint8_t v, uint8_t pos);
void syscall_display_number(uint32_t v);
void syscall_put(BFifo* bfifo, char c);
void syscall_puts(BFifo* bfifo, char* str);

#endif

