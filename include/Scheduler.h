#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "mips/types.h"

#include "Structs.h"
#include "Process.h"
#include "IOHandler.h"
#include "Settings.h"

typedef struct _priority {
	PCB* current; // First process
} Priority;

Priority PriorityArray[PRIORITIES+1];

void run();
void copyRegisters(registers_t *target, registers_t *source);
int insertPCB(PCB* entry);
PCB* getFreePCB();
PCB* getPCB(int PID);
Process getProcess(int PID);
void freePID(int PID);
void freePCB(PCB* entry);
void initScheduler(registers_t *regs, int mem);
// TODO: ProcessTable* getProcessTable();

#endif
