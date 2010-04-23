#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "Process.h"
#include "Settings.h";

typedef struct _PCBQueue {
	PCB* current;
	PCB* next;
	PCB* prev;
} PCBQueue;

typedef struct _Priority {
	PCBQueue* current; // First process
	int spinTimes; // Stores the number of iterations
} Priority;

Priority PriorityArray[PRIORITIES];

void run();
//int insertPCB(PCB* entry);

#endif
