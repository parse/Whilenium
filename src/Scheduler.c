#include "Scheduler.h"

/**
 * run()
 * The scheduler starts its work from the place where it stopped. (If it has not been running, it
 * starts the scheduler from the beginning.)
 */
void run(int memoryMin) {
	while(1) {
	}
}


/**
 * insertPCB(PCB)
 * inserts the PCB in the priority queue
 * returns 0 if succeded
 */
int insertPCB (PCB* entry) {
	/*if (entry == NULL)
		putc('n');
	else
		putc('y');
	
	return 0;
	*/
	//puts(entry->name);
	//puts("Lol1: ");
	PCB* current = PriorityArray[1].current;
//	currentPID++;	
	char tmp[10];
	puts(itoa((unsigned int)entry, tmp, 10));
	
	if (current == NULL) {
		puts("\ncurrent == NULL");
		current = entry;
		entry->next = entry;
		entry->prev = entry;
		PriorityArray[1].current = entry;
	} else {
		if (entry == NULL)
			puts("\nMycket märkligt");
		puts("\ncurrent exists");
		PCB* last = current->prev;
		last->next = entry;
		entry->next = current;
		entry->prev = last;
		current->prev = entry;
	}
	
	return 0;
}
