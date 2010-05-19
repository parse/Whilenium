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

int exec(char* program, int priority, uint32_t arg);
void sleep(int PID, int sleep);
int changePrio(int PID, int prio);
int block(int PID);
int unblock(int PID);

int getPrio(int PID);
State getState(int PID);
char* getName(int PID);

#endif
