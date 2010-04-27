#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "types.h"
#include "Process.h"
#include "IOHandler.h"
#include "Settings.h"

typedef struct _Priority {
	PCB* current; // First process
	int spinTimes; // Stores the number of iterations
} Priority;

Priority PriorityArray[PRIORITIES];
//int currentPID = 0;

void run(int memoryMin);
int insertPCB(PCB* entry);

#endif
