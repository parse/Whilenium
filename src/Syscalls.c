#include "Syscalls.h"

int kBlock(int PID) {
	PCB* entry = getPCB(PID);
	PCB* currentPCB = getCurrentPCB();
	
	if ( (int)entry == -1 )
		return -1;
		
	entry->state = Blocked;
	
	if (currentPCB->PID == PID)
		run();
		
	return 1;
}

int kUnblock(int PID) {
	PCB* entry = getPCB(PID);
	PCB* currentPCB = getCurrentPCB();
	
	if ( (int)entry == -1 )
		return -1;
		
	entry->state = New;
	
	if (entry->prio < currentPCB->prio)
		run();
	
	return 1;
}

int kKill(int PID) {
	if (freePID(PID) == -1) {
					// TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/* // TODOOOOOO */ putslnDebug("Error: Couldn't terminate process"); // TODO!!!!!!!!!!!!!!!
 					// TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		return -1;
	}
		
	PCB* currentPCB = getCurrentPCB();
	
	if (currentPCB->PID == PID)
		run();
	
	return 0;
}

int kSleep(int PID, int sleepTime) {
	PCB* entry;
	PCB* currentPCB = getCurrentPCB();
	
	if (PID == 0) 
		entry = currentPCB;
	else
		entry = getPCB(PID);
	
	if ( (int)entry == -1 )
		return -1;
	
	entry->state = Waiting;
	entry->sleep = timeCount + sleepTime;
	
	if (currentPCB == entry)
		run();
		
	return 1;
}

int kChangePrio(int PID, int prio) {
	PCB* entry = getPCB(PID);
	PCB* currentPCB = getCurrentPCB();
	
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
	
	if (prio < currentPCB->prio)
		run();
		
	return 1;
}

int kExec(char* program, int priority, uint32_t arg) {
	return -1;
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
