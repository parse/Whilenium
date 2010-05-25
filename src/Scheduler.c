#include "Scheduler.h"

PCB* currentPCB = NULL;
registers_t *regSpace;

/**
 * run()
 * The scheduler starts its work from the place where it stopped. (If it has not been running, it
 * starts the scheduler from the beginning.)
 * @param int memoryMin - The "start" memory address
 */
void run() {
 	// Setup storage-area for saving registers on exception. 
	// currentPCB is the previously run process
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
		
		while (cur != NULL && (cur->state == RequestingIO || cur->state == Blocked || (cur->state == Waiting && cur->sleep > timeCount))) {
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
	
	if ((int)entry == -1) {
		p.PID = -1;
		return p;
	}
	
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
	
	if (entry != next) {
		PriorityArray[entry->prio].current = next;
		prev->next = next;
		next->prev = prev;
	} else
		PriorityArray[entry->prio].current = NULL;
	
	entry->next = NULL;
	entry->prev = NULL;
	
	entry->prio = 0;
	freeIO(entry->PID);
	
	entry->state = Terminated;
	
	insertPCB(entry);
}

/*
 * die()
 * Takes care of a dying process by removing it from our queue and update state
 */
void die() {
	PCB* currentPCB = getCurrentPCB();
	
	if (currentPCB != NULL) {
		freePCB(currentPCB);
	}

	run();
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
 * initScheduler(registers_t *regs)
 * Initialise scheduler
 * @param registers_t *regs - Memory register to use
 */
void initScheduler(registers_t *regs) {
	regSpace = regs;
}

