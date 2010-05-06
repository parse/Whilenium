#include "Scheduler.h"

PCB* previousPCB = NULL;
registers_t *regSpace;
int memoryMin;

/**
 * run()
 * The scheduler starts its work from the place where it stopped. (If it has not been running, it
 * starts the scheduler from the beginning.)
 * @param int memoryMin - The "start" memory address
 */
void run() {	
 	/* Setup storage-area for saving registers on exception. */
 	//kset_registers(&regs);
	//copyRegisters((char*)&(previousPCB->registers), (char*)regSpace);
	if (previousPCB != NULL) {
		char buf[10];
		puts("previousPCB != NULL: ");
		puts(itoa(previousPCB, buf, 10));
		puts(" ");
		puts(itoa(&(previousPCB->registers), buf, 10));
		puts(" ");
		puts(itoa(regSpace, buf, 10));
		putsln("");
		copyRegisters((char*)&(previousPCB->registers), (char*)regSpace);
	}

	int i;
	PCB* cur;
	
	int prevPC;
	int prevSP;
	
	for (i = 1; i <= PRIORITIES; i++) {
		cur = PriorityArray[i].current;
		
		if (cur != NULL) {
			// Changes the PC and SP to the new values and gets the old values
			//prevPC = changeEPC(cur->PC);
			//prevSP = changeSP(cur->SP);
			copyRegisters((char*)regSpace, (char*)&(cur->registers));
			puts("\tFound a process to run! (");
			puts(cur->name);
			putsln(")");
			
			break;
		}
	}
	
	if (previousPCB != NULL) {
		// Sets the previous PCB SP and PC to the values of PC and SP
		previousPCB->PC = prevPC;
		previousPCB->SP = prevSP;
	}
	
	if (cur != NULL) {
		// Sets previous to the one that is to be run
		previousPCB = PriorityArray[i].current;
	
		// Current changes to the next PCB in queue to be run next time
		PriorityArray[i].current = cur->next;
	}
}

void copyRegisters(char *target, char *source) {
	int i;
	
	for (i = 0; i < 120; i++) {
		target[i] = source[i];
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
	puts("Name: ");
	puts(entry->name);
	puts(" Prio: ");
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
 * getProcessTable()
 * Get the Process table
 * @return First process in PriorityQueue
 * TODO!

ProcessTable getProcessTable() {
	PCB* entry = getPCB(PID);
} */

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

void initScheduler(registers_t *regs, int mem) {
	regSpace = regs;
	memoryMin = mem;
}