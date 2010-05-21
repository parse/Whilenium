#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "mips/types.h"
#include "mips/malta.h"

#include "Structs.h"
#include "Syscalls.h"
#include "Process.h"
#include "IOHandler.h"
#include "Settings.h"

typedef struct _priority {
	PCB* current; // First process
} Priority;

Priority PriorityArray[PRIORITIES+1];

State getPrevState();
void run();
void copyRegisters(registers_t *target, registers_t *source);

int insertPCB(PCB* entry);
PCB* getCurrentPCB();
PCB* getFreePCB();
PCB* getPCB(int PID);
PCB* getCurrentPCB();
void preparePCB(PCB* entry);

Process getProcess(int PID);

int freePID(int PID);
void freePCB(PCB* entry);

void initScheduler(registers_t *regs, int mem);
void die();

#endif
