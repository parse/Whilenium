#ifndef __ASM_SYSCALL__
#define __ASM_SYSCALL__

#include "Structs.h"

void ksyscall_handler(registers_t* reg);

void syscall_die();
void syscall_display_c(uint8_t v, uint8_t pos);
void syscall_display_number(uint32_t v);
void syscall_put(BFifo* bfifo, char c);
void syscall_puts(BFifo* bfifo, char* str);
void syscall_schedule();
int syscall_block();
int syscall_unblock();
int syscall_kill(uint32_t pid);
int syscall_sleep(uint32_t pid, uint32_t sleepTime);
int syscall_prio(uint32_t pid, uint32_t prio);
int syscall_spawn(NewPCBArgs* newPCBArgs);
void syscall_displayS(uint32_t msg, uint8_t offset);
//char syscall_bfifo_get(struct bounded_fifo* bfifo);
char* syscall_gets();

#endif

