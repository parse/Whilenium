#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "mips/types.h"
#include "mips/malta.h"

#include "Structs.h"
#include "Process.h"
#include "IOHandler.h"
#include "Settings.h"


typedef struct _priority {
	PCB* current; // First process
} Priority;

Priority PriorityArray[PRIORITIES+1];
 
State getPrevState();
void run();
void die();
void copyRegisters(registers_t *target, registers_t *source);
int insertPCB(PCB* entry);
PCB* getFreePCB();
PCB* getPCB(int PID);
Process getProcess(int PID);
void freePID(int PID);
void freePCB(PCB* entry);
void initScheduler(registers_t *regs, int mem);
void preparePCB(PCB* entry);
// TODO: ProcessTable* getProcessTable();

int kExec(char* program, int priority, uint32_t arg);
int kBlock(int pid);
int kUnblock(int pid);
int kKill(int pid);
int kSleep(int pid, int sleepTime);
int kChangePrio(int pid, int prio);

#endif
