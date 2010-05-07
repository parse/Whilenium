#ifndef __PROCESS__
#define __PROCESS__

#include "mips/types.h"
#include "mips/registers.h"
#include "stdlib.h"

#include "Settings.h"
#include "Structs.h"

#include "IOHandler.h"
#include "Interrupt.h"
#include "API.h"
#include "Scheduler.h"
#include "Memory.h"
#include "UserPrograms.h"
#include "_Boot.h"



/*
TODO:
typedef struct _ProcessTable {
	PCB* next;
	Process p;
} ProcessTable;
*/
void initOS(int memoryMin);
void initPCBTable(int memoryMin);
int newPCB(int prio, int PC, char* name);
void exitProcess();


#endif
