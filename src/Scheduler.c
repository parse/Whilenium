#include "Scheduler.h"

PCB* currentPCB = NULL;
registers_t *regSpace;
int memoryMin;

/**
 * run()
 * The scheduler starts its work from the place where it stopped. (If it has not been running, it
 * starts the scheduler from the beginning.)
 * @param int memoryMin - The "start" memory address
 */
void run() {
	if (DEBUG)
		putslnDebug("Run!");

 	// Setup storage-area for saving registers on exception. 
	if (currentPCB != NULL) {
		copyRegisters(&(currentPCB->registers), regSpace);
		
		if (currentPCB->state == Running)
			currentPCB->state = Ready;
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
	
	if (cur != NULL && cur != currentPCB && DEBUG) {
		char buf[10];
		if (currentPCB != NULL) {
			putsDebug("run: currentPCB = ");
			putslnDebug(itoa((int)currentPCB, buf, 10));
			putsDebug("run: currentPCB->prev = ");
			putslnDebug(itoa((int)currentPCB->prev, buf, 16));
			putsDebug("run: Previous->PID = ");
			putslnDebug(itoa(currentPCB->PID, buf, 16));
			putsDebug("run: Previous->prio = ");
			putslnDebug(itoa(currentPCB->prio, buf, 16));
			putsDebug("run: Previous->name = ");
			putslnDebug(currentPCB->name);
		}
		
		putsDebug("run: cur = ");
		putslnDebug(itoa((int)cur, buf, 16));
		putsDebug("run: cur->prev = ");
		putslnDebug(itoa((int)cur->prev, buf, 16));
		putsDebug("run: cur->PID = ");
		putslnDebug(itoa(cur->PID, buf, 16));
		putsDebug("run: cur->prio = ");
		putslnDebug(itoa(cur->prio, buf, 16));
		putsDebug("run: cur->name = ");
		putslnDebug(cur->name);
		putsDebug("run: cur->ra = ");
		putslnDebug(itoa(cur->registers.ra_reg, buf, 16));
		putsDebug("run: cur->epc = ");
		putslnDebug(itoa(cur->registers.epc_reg, buf, 16));
	}
	
	if (cur != NULL) {
		preparePCB(cur);
		
		// Sets previous to the one that is to be run
		currentPCB = cur;
		// Current changes to the next PCB in queue to be run next time
		PriorityArray[i].current = cur->next;
	} else {
		currentPCB = NULL;
	}
	
	kload_timer(1 * timer_msec);
}

void preparePCB(PCB* entry) {
	copyRegisters(regSpace, &(entry->registers));
	
	entry->state = Running;
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
	
	if (DEBUG) {
		char buf[10];
		
		putsDebug("insertPCB: entry = ");
		putslnDebug(itoa((int)entry, buf, 16));
		putsDebug("insertPCB: entry->PID = ");
		putslnDebug(itoa(entry->PID, buf, 16));
		putsDebug("insertPCB: entry->prio = ");
		putslnDebug(itoa(entry->prio, buf, 16));
		putsDebug("insertPCB: entry->name = ");
		putslnDebug(entry->name);
		
		if (current == NULL)
			putslnDebug("insertPCB: current == NULL");
		if (entry == NULL)
			putslnDebug("insertPCB: entry == NULL");
	}
		
	if (current == NULL) {
		PriorityArray[entry->prio].current = entry;
		entry->next = entry;
		entry->prev = entry;
	} else {	
		(current->prev)->next = entry;
		entry->next = current;
		entry->prev = current->prev;
		current->prev = entry;
	}
	
	return 0;
}

PCB* getCurrentPCB() {
	return currentPCB;
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
	
	if (ret == NULL)
		return (PCB*)-1;
	
	PCB* prev = ret->prev;
	PCB* next = ret->next;
	 
	if (ret != next) {
		PriorityArray[0].current = next;
		
		if (DEBUG) {
			char buf[10];
			if (prev == NULL)
				putslnDebug("getFreePCB: prev == NULL");
			if (next == NULL)
				putslnDebug("getFreePCB: next == NULL");

			putsDebug("getFreePCB: ret = ");
			putslnDebug(itoa((int)ret, buf, 16));
		}
		
		prev->next = next;
		next->prev = prev;
	} else {
		PriorityArray[0].current = NULL;
	}
	
	ret->next = NULL;
	ret->prev = NULL;
	
	if (DEBUG)
		putslnDebug("getFreePCB done!");
	
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
	
	return p;
}

/**
 * freePID(int PID)
 * Reset prio, pid for PID
 * @param int PID - The PID to free
 */
int freePID(int PID) {
	PCB* entry = getPCB(PID);
	
	if ( (int)entry == -1)
		return -1;
		
	freePCB(entry);
	
	return 1;
}


void freeIO(int PID) {
	PCB* prev = NULL;
	PCB* cur = NULL;
	
	cur = ioqueue.current;
	while (cur != NULL) {
		if (cur->PID == PID) {
			if (prev == NULL)
				ioqueue.current = cur->nextIO;
			else
				prev->nextIO = cur->nextIO;
				
			break;
		}
		
		prev = cur;
		cur = cur->nextIO;
	}
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
	char buf[10];
	if (entry != next) {
		if (DEBUG) {
			putsDebug("freePCB: entry = ");
			putslnDebug(itoa((int)entry, buf, 16));
			putsDebug("freePCB: entry->prev = ");
			putslnDebug(itoa((int)prev, buf, 16));
			putsDebug("freePCB: entry->PID = ");
			putslnDebug(itoa((int)entry->PID, buf, 16));
			putsDebug("freePCB: entry->prio = ");
			putslnDebug(itoa((int)entry->prio, buf, 16));
			putsDebug("freePCB: entry->name = ");
			putslnDebug(entry->name);
			putsDebug("freePCB: currentPCB = ");
			putslnDebug(itoa((int)currentPCB, buf, 16));
			putsDebug("freePCB: currentPCB->prev = ");
			putslnDebug(itoa((int)currentPCB->prev, buf, 16));
			putsDebug("freePCB: currentPCB->PID = ");
			putslnDebug(itoa((int)currentPCB->PID, buf, 16));
			putsDebug("freePCB: currentPCB->prio = ");
			putslnDebug(itoa((int)currentPCB->prio, buf, 16));
			putsDebug("freePCB: currentPCB->name = ");
			putslnDebug(currentPCB->name);
		}
		PriorityArray[entry->prio].current = next;
		prev->next = next;
		next->prev = prev;
	} else
		PriorityArray[entry->prio].current = NULL;
	
	entry->next = NULL;
	entry->prev = NULL;
	
	entry->prio = 0;
	//entry->PID = -1; // This should not be here, since we can't compare pid after freePID if we set -1

	insertPCB(entry);
	
	if (DEBUG)
		putslnDebug("freePCB done!");
}

/*
 * die()
 * Takes care of a dying process by removing it from our queue and update state
 */
void die() {
	char buf[10];
	PCB* currentPCB = getCurrentPCB();
	
	if (DEBUG)
		putslnDebug("die start!");
	
	if (currentPCB != NULL) {
		if (DEBUG) {
			putsDebug("currentPCB: entry = ");
			putslnDebug(itoa((int)currentPCB, buf, 16));
		}
		freePCB(currentPCB);
		currentPCB->state = Terminated;
	}

	run();
	
	if (DEBUG)
		putslnDebug("die done!");
}

/*
 * getPrevState()
 * Get the state of the currentPCB
 * @return State of the process
 */
State getPrevState() {
	if (currentPCB != NULL)
		return currentPCB->state;
	else
		return Undefined;
}

/*
 * initScheduler(registers_t *regs, int mem)
 * Initialise scheduler
 * @param registers_t *regs - Memory register to use
 * @param int mem - Lower memory limit
 */
void initScheduler(registers_t *regs, int mem) {
	regSpace = regs;
	memoryMin = mem;
}

