#include "Scheduler.h"

/**
 * run()
 * The scheduler starts its work from the place where it stopped. (If it has not been running, it
 * starts the scheduler from the beginning.)
 * @param int memoryMin - The "start" memory address
 */
void run(int memoryMin) {
	puts("Run:\n");
	PCB* current = PriorityArray[1].current; 
	PCB* first = current;
	
	int i = 0;
	char buf[10];
	
	while (current != NULL) {
		puts("Queue elem #");
		puts(itoa(i, buf, 10));
		i++;
		puts(" PID: ");
		puts(itoa(current->PID, buf, 10));
		puts(" Prio: ");
		puts(itoa(current->prio, buf, 10));
		puts(" Current: ");
		puts(itoa(current, buf, 10));
		puts(" Next: ");
		puts(itoa(current->next, buf, 10));
		puts(" Name: ");
		puts(current->name);
		puts("_\n");
		
		if (current->next == first)
			break;
		else
			current = current->next;
	}	
	
	/*
	PCB* p = getPCB(2);
	puts("GetPCB test: ");
	puts(p->name);
	*/
	
	Process p = getProcess(2);
	puts("GetProcess() test");
	puts(p.name);
	
	while(1) {
	}
}

/**
 * insertPCB(PCB* entry)
 * Inserts the PCB in the priority queue
 * @param PCB* entry - PCB entry to insert
 * @return 0 if succeded
 */
int insertPCB (PCB* entry) {
	char tmp[10];
	puts("\nPrio: ");
	puts(entry->name);
	puts(itoa(entry->prio, tmp, 10));
	puts("\n");
	
	PCB* current = PriorityArray[entry->prio].current;
	
	if (current == NULL) {
		PriorityArray[entry->prio].current = entry;
		entry->next = entry;
		entry->prev = entry;
	} else {
		//PCB* last = current->prev;
		current->prev->next = entry;
		entry->next = current;
		entry->prev = current->prev;
		current->prev = entry;
	}
	return 0;
}

/**
 * getFreePCB()
 * Fetch a free PCB-slot and remove it from PriorityQueue[0]
 * This is not a safe move - We take away a PCB from the free queue and do not make sure it gets in a new queue.
 * It is the responsibility of the Process module to put it in a new queue with insertP.
 * @return PCB pointer to the freed PCB
 */
PCB* getFreePCB() {
	PCB* ret = PriorityArray[0].current;
	PCB* prev = ret->prev;
	PCB* next = ret->next;
	
	PriorityArray[0].current = next;
	prev->next = next;
	next->prev = prev;
	
	ret->next = NULL;
	ret->prev = NULL;
	
	return ret;
}

/**
 * getPCB(int PID)
 * Get the PCB at PID by searching all priority-queues.
 * @param int PID - The PID to get PCB for
 * @return Pointer to PCB with pid PID, -1 if it's not found
 */
PCB* getPCB(int PID) {
	int i;
	for (i = 1; i <= PRIORITIES; i++) {
		PCB* current = PriorityArray[i].current; 
		PCB* first = current;
		
		while (current != NULL) {
			if (current->PID == PID)
				return current;
			
			if (current->next == first)
				break;
			else
				current = current->next;
		}
	}
	
	return -1;
}

/**
 * getProcess(int PID)
 * Get the Process information with PID
 * @param int PID - The PID to get Process information for for
 * @return Process with pid PID
 */
Process getProcess(int PID) {
	PCB* entry = getPCB(PID);
	
	Process p;
	p.PID = entry->PID;
	p.name = entry->name;
	p.prio = entry->prio;
	p.state = entry->state;
	p.programName = entry->name;
	
	return p;
}

/**
 * freePID(int PID)
 * Reset prio, pid and PC for PID
 * @param int PID - The PID to free
 */
void freePID(int PID) {
	PCB* entry = getPCB(PID);
	
	entry->prio = 0;
	entry->PID = -1;
	entry->PC = 0;
}
