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
	if (newPCBFlag)
		putslnDebug("run: newPCB is running!");
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
	
	
	char buf[10];
	if (cur != previousPCB) {
		if (previousPCB != NULL) {
			putsDebug("run: previousPCB = ");
			putslnDebug(itoa((int)previousPCB, buf, 10));
			putsDebug("run: previousPCB->prev = ");
			putslnDebug(itoa((int)previousPCB->prev, buf, 16));
			putsDebug("run: Previous->PID = ");
			putslnDebug(itoa(previousPCB->PID, buf, 16));
			putsDebug("run: Previous->prio = ");
			putslnDebug(itoa(previousPCB->prio, buf, 16));
			putsDebug("run: Previous->name = ");
			putslnDebug(previousPCB->name);
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
	}
	
	if (cur != NULL) {
		preparePCB(cur);
		
		// Sets previous to the one that is to be run
		previousPCB = cur;
		// Current changes to the next PCB in queue to be run next time
		PriorityArray[i].current = cur->next;
	} else {
		previousPCB = NULL;
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
	char buf[10];
	putslnDebug("die start!");
	
	if (previousPCB != NULL) {
		putsDebug("previousPCB: entry = ");
		putslnDebug(itoa((int)previousPCB, buf, 16));
		freePCB(previousPCB);
		previousPCB->state = Terminated;
	}
	putslnDebug("die done!");
}


int kBlock(int PID) {
	PCB* entry = getPCB(PID);
	
	if ( (int)entry == -1 )
		return -1;
		
	entry->state = Blocked;
	
	if (previousPCB->PID == PID)
		run();
		
	return 1;
}

int kUnblock(int PID) {
	PCB* entry = getPCB(PID);
	
	if ( (int)entry == -1 )
		return -1;
		
	entry->state = Ready;
	
	if (entry->prio > previousPCB->prio)
		run();
	
	return 1;
}

int kKill(int PID) {
	//if (freePID(PID) == -1)
	//	return -1; //putsln("Error: Could not kill process with given PID");*/
	
	freePID(PID);
	
	if (previousPCB->PID == PID)
		run();
	
	return 0;
}

int kSleep(int PID, int sleepTime) {
	PCB* entry = getPCB(PID);
	
	putslnDebug("----------Jag ska sova1!\n");
	
	if ( (int)entry == -1 )
		return -1;
		
	putsDebug("----------Jag ska sova!\n");
	
	entry->state = Waiting;
	entry->sleep = timeCount + sleepTime;
	
	putsDebug("----------Installd på sova!\n");
	
	if (previousPCB == entry)
		run();
		
	return 1;
}

int kChangePrio(int PID, int prio) {
	PCB* entry = getPCB(PID);

	if ( (int)entry == -1 )
		return -1;
		
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
		
	return 1;
}

int kExec(char* program, int priority, uint32_t arg) {
	return -1;
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
		
		char buf[10];
		if (prev == NULL)
			putslnDebug("getFreePCB: prev == NULL");
		if (next == NULL)
			putslnDebug("getFreePCB: next == NULL");
		putsDebug("getFreePCB: ret = ");
		putslnDebug(itoa((int)ret, buf, 16));
		
		prev->next = next;
		next->prev = prev;
	} else {
		PriorityArray[0].current = NULL;
	}
	
	ret->next = NULL;
	ret->prev = NULL;
	
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
	char buf[10];
	if (entry != next) {
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
		putsDebug("freePCB: previousPCB = ");
		putslnDebug(itoa((int)previousPCB, buf, 16));
		putsDebug("freePCB: previousPCB->prev = ");
		putslnDebug(itoa((int)previousPCB->prev, buf, 16));
		putsDebug("freePCB: previousPCB->PID = ");
		putslnDebug(itoa((int)previousPCB->PID, buf, 16));
		putsDebug("freePCB: previousPCB->prio = ");
		putslnDebug(itoa((int)previousPCB->prio, buf, 16));
		putsDebug("freePCB: previousPCB->name = ");
		putslnDebug(previousPCB->name);
		
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
	
	putslnDebug("freePCB done!");
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

