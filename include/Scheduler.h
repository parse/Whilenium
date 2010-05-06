#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "types.h"
#include "Process.h"
#include "IOHandler.h"
#include "Settings.h"

typedef struct _Priority {
	PCB* current; // First process
	//char ; // Stores the number of iterations
} Priority;

Priority PriorityArray[PRIORITIES+1];
//PCB* lastPCB;

void run();
void copyRegisters(char* target, char* source);
int insertPCB(PCB* entry);
PCB* getPCB(int PID);
Process getProcess(int PID);
void freePID(int PID);
void initScheduler(registers_t *regs, int mem);
// TODO: ProcessTable* getProcessTable();

#endif
