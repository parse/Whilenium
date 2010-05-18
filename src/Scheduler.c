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
 	// Setup storage-area for saving registers on exception. 
	if (previousPCB != NULL) {
		copyRegisters(&(previousPCB->registers), regSpace);
		
		if (previousPCB->state == Running)
			previousPCB->state = Ready;
	}
	
	
	PCB* cur = NULL;
	PCB* firstCheck = NULL;
	int first;
	int i;
	
	for (i=1; i<=PRIORITIES; i++) {
		cur = firstCheck = PriorityArray[i].current;
		first = 1;
		
		while (cur != NULL && (cur->state == Blocked || (cur->state == Waiting && cur->sleep > timeCount))) {
			if (first == 0 && cur == firstCheck) {
				cur = NULL;
				break;
			}
			
			PriorityArray[i].current = cur->next;
			cur = PriorityArray[i].current;
			
			first = 0;
		}
		
		if (cur != NULL) {
			break;
		}
	}
	
	
	if (cur != NULL) {
		preparePCB(cur);
		
		// Sets previous to the one that is to be run
		previousPCB = PriorityArray[i].current;

		// Current changes to the next PCB in queue to be run next time
		PriorityArray[i].current = cur->next;
	} else {
		previousPCB = NULL;
		//char buf[10];
		//puts("Ready: ");
		//puts(itoa(currentPID, buf, 10));
		//putsln("");
	}
}

void preparePCB(PCB* entry) {
	copyRegisters(regSpace, &(entry->registers));
	
	entry->state = Running;
}

/*
 * die()
 * Takes care of a dying process by removing it from our queue and update state
 */
void die() {
	if (previousPCB != NULL) {
		freePCB(previousPCB);
		previousPCB->state = Terminated;
	}
}


void kBlock(int PID) {
	PCB* entry = getPCB(PID);

	entry->state = Blocked;
	if (previousPCB->PID == PID)
		run();
		
	/*
	if ( (entry->state = Blocked) ) {
		if (previousPCB)
		return 1;
	}
	else
		return -1; */
}

void kUnblock(int PID) {
	PCB* entry = getPCB(PID);
	
	entry->state = Ready;
	
	if (entry->prio > previousPCB->prio)
		run();
		
	/*
	if ( (entry->state = Ready) )
		return 1;
	else
		return -1;
		*/
}

void kKill(int PID) {
	//if (freePID(PID) == -1)
	//	return -1; //putsln("Error: Could not kill process with given PID");*/
		
	putsDebug("----------Kill!\n");
	freePID(PID);
	
	if (previousPCB->PID == PID)
		run();
	//return 1;
}

void kSleep(int PID, int sleepTime) {
	PCB* entry = getPCB(PID);
	
	entry->state = Waiting;
	entry->sleep = timeCount + sleepTime;
	
	if (previousPCB == entry)
		run();
}

void kChangePrio(int PID, int prio) {
	PCB* entry = getPCB(PID);

	PCB* prev = entry->prev;
	PCB* next = entry->next;

	if (entry != next) {
		PriorityArray[entry->prio].current = next;
		prev->next = next;
		next->prev = prev;
	} else
		PriorityArray[entry->prio].current = NULL;

	entry->next = NULL;
	entry->prev = NULL;

	entry->prio = prio;

	insertPCB(entry);
	
	if (prio > previousPCB->prio)
		run();
}


/*
 * copyRegisters (registers_t *target, registers_t *source)
 * Copy one register to another register, uses a max size of 30
 */
void copyRegisters(registers_t *target, registers_t *source) {
	int i;
	uint32_t *_target = (uint32_t *)target;
	uint32_t *_source = (uint32_t *)source;
	
	for (i = 0; i < 30; i++) {
		_target[i] = _source[i];
	}
}

/**
 * insertPCB(PCB* entry)
 * Inserts the PCB in the priority queue
 * @param PCB* entry - PCB entry to insert
 * @return 0 if succeded
 */
int insertPCB (PCB* entry) {
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
	 
	if (ret != next) {
		PriorityArray[0].current = next;
		prev->next = next;
		next->prev = prev;
	} else {
		PriorityArray[0].current = NULL;
	}
		
	
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
	
	return (PCB*)-1;
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
 * Reset prio, pid for PID
 * @param int PID - The PID to free
 */
void freePID(int PID) {
	PCB* entry = getPCB(PID);
	
	freePCB(entry);
}

/**
 * freePCB(PCB* entry)
 * Reset prio, pid for PCB
 * @param PCB* entry - The PCB to free
 */
void freePCB(PCB* entry) {	
	// Remove PCB from queue
	PCB* prev = entry->prev;
	PCB* next = entry->next;
	
	if (entry != next) {
		PriorityArray[entry->prio].current = next;
		prev->next = next;
		next->prev = prev;
	} else
		PriorityArray[entry->prio].current = NULL;
	
	entry->next = NULL;
	entry->prev = NULL;
	
	entry->prio = 0;
	entry->PID = -1;

	insertPCB(entry);
}

State getPrevState() {
	if (previousPCB != NULL)
		return previousPCB->state;
	else
		return Undefined;
}

void initScheduler(registers_t *regs, int mem) {
	regSpace = regs;
	memoryMin = mem;
}

