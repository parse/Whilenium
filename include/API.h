#ifndef __API__
#define __API__

#include "Structs.h"
#include "IOHandler.h"
#include "_Boot.h"
#include "stdlib.h"

/*
void putc(char c); // From IOHandler
void puts(char* text); // From IOHandler
void putsln(char* text); // From IOHandler
char getc(); // From IOHandler
void displayC(uint8_t c, uint8_t pos); // From IOHandler
void displayNumber(uint32_t word); // From IOHandler
*/
int kill(int PID);

/*
Process* getProcess(int PID);
Process** getProcessTable();
*/

int exec(char* program);
int changePriority(int PID, int priority);
int sleep(int PID, int sleepTime);
int block(int PID);
int unblock(int PID);

#endif
