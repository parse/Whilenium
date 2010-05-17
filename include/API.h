#ifndef __API__
#define __API__

#include "mips/types.h"

#include "Structs.h"
#include "Interrupt.h"

void putc(char c);
void puts(char* text);
void putsln(char* text);

char getc();
char kill(int pid);

Process* getProcess(int pid);
Process** getProcessTable();

int exec(char* program);
int changePriority(int pid, int priority);
int sleep(int pid, int sleepTime);
int block(int pid);
int unblock(int pid);

#endif
