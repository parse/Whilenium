#include "Scheduler.h"


/**
 * run()
 * The scheduler starts its work from the place where it stopped. (If it has not been running, it
 * starts the scheduler from the beginning.)
 */
void run() {
	while(1) {
	}
}


/**
 * insertPCB(PCB)
 * inserts the PCB in the priority queue
 * returns 0 if succeded
 */
int insertPCB (PCB* entry) {
	PCB* current = PriorityArray[entry->prio].current;
	
	if (current == NULL) {
		current = entry;
		entry->next = entry;
		entry->prev = entry;
	} else {
		PCB* last = current->prev;
		
		last->next = entry;
		entry->next = current;
		entry->prev = last;
		current->prev = entry;
	}

	return 0;
}
