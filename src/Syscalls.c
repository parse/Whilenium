#include "Syscalls.h"

/*
 * kBlock (int PID)
 * Block process PID
 * @param int PID - Process to block
 * @return -1 if fails, else 1
 */
int kBlock(int PID) {
	PCB* entry = getPCB(PID);
	PCB* currentPCB = getCurrentPCB();
	
	if ( (int)entry == -1 ) {
		kget_registers()->v_reg[0] = -1;
		return (int)NULL;
	}
		
	entry->state = Blocked;
	
	kget_registers()->v_reg[0] = 1;	
	
	if (currentPCB->PID == PID)
		run();
		
	return (int)NULL;
}

/*
 * kUnblock (int PID)
 * Unblock process PID
 * @param int PID - Process to unblock
 * @return -1 if fails, else 1
 */
int kUnblock(int PID) {
	PCB* entry = getPCB(PID);
	PCB* currentPCB = getCurrentPCB();
	
	if ( (int)entry == -1 ) {
		kget_registers()->v_reg[0] = -1;
		return (int)NULL;
	}	
	
	entry->state = New;
	
	kget_registers()->v_reg[0] = 1;	
	
	if (entry->prio < currentPCB->prio)
		run();

	return (int)NULL;
}

/*
 * kKill (int PID)
 * Terminate process PID
 * @param int PID - Process to terminate
 * @return -1 if fails, else 1
 */
int kKill(int PID) {
	if (freePID(PID) == -1) {
		kget_registers()->v_reg[0] = -1;
		return (int)NULL;
	}
		
	PCB* currentPCB = getCurrentPCB();
	
	kget_registers()->v_reg[0] = 1;
	
	if (currentPCB->PID == PID)
		run();
	
	return (int)NULL;
}

/*
 * kSleep (int PID, int sleepTime)
 * Sleep process, if PID = 0 - use the current process
 * @param int PID - Process to sleep
 * @param int sleepTime - Time to sleep
 * @return -1 if fails, else 1
 */
int kSleep(int PID, int sleepTime) {
	PCB* entry;
	PCB* currentPCB = getCurrentPCB();
	
	if (PID == 0) 
		entry = currentPCB;
	else
		entry = getPCB(PID);
	
	if ( (int)entry == -1 ) {
		kget_registers()->v_reg[0] = -1;
		return (int)NULL;
	}
	
	entry->state = Waiting;
	entry->sleep = timeCount + sleepTime;
	
	kget_registers()->v_reg[0] = 1;
	
	if (currentPCB == entry)
		run();
		
	return (int)NULL;
}

/*
 * kChangePrio (int PID, int prio)
 * Change priority for process PID
 * @param int PID - Process to change priority for
 * @param int prio - New priority
 * @return -1 if fails, else 1
 */
int kChangePrio(int PID, int prio) {
	PCB* entry = getPCB(PID);
	PCB* currentPCB = getCurrentPCB();
	
	if ( (int)entry == -1 ) {
		kget_registers()->v_reg[0] = -1;
		return (int)NULL;
	}
		
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
	
	kget_registers()->v_reg[0] = 1;
	
	if (prio < currentPCB->prio)
		run();
		
	return (int)NULL;
}


/**
 * kNewPCB(NewPCBArgs* newPCBArgs)
 * Important! We must insert the PCB from getFreePCB into a new queue with insertPCB
 * Pre: prio > 0
 * @param NewPCBArgs* newPCBArgs - Program to queue
 * @return The newly created process PID
 */
int kNewPCB(NewPCBArgs* newPCBArgs) {
	PCB* pcb = getFreePCB();

	if ((int)pcb == -1) {
		kget_registers()->v_reg[0] = -1;
		return (int)NULL;
	}
		
	pcb->prio = newPCBArgs->prio;
	
	strcpy(pcb->name, newPCBArgs->name);
	
	currentPID++;
	pcb->PID = currentPID;
	pcb->registers.epc_reg = newPCBArgs->PC;
	pcb->registers.sp_reg = (uint32_t)&(pcb->stackHighEnd);
	pcb->registers.a_reg[0] = newPCBArgs->arg;
	pcb->registers.a_reg[1] = 0;
	pcb->registers.a_reg[2] = 0;
	pcb->registers.a_reg[3] = 0;
	pcb->registers.ra_reg = (uint32_t)&exitProcess;
	
	pcb->state = newPCBArgs->state;
	pcb->sleep = timeCount + newPCBArgs->sleep;
	
	insertPCB(pcb);
	
	if (interruptsEnabled) {
		if (DEBUG)
			putslnDebug("newPCB: interruptsEnabled = 1. Do run!");
		
		kget_registers()->v_reg[0] = pcb->PID;
		run();
	}
	
	return pcb->PID;
}

/**
 * kNewPCBWithArgs(iN)
 * Construct a "holder" for all arguments to use later
 * @param int prio - The priority to set
 * @param int PC - Program counter to set
 * @param char* name - Program name to set
 * @param uint32_t arg - Argument to pass to process
 * @param State state - State to start the process in
 * @param int sleep - If state is Waiting, the sleeptime is how long the process waits
 * @return The PCB created with above values
 */
int kNewPCBWithArgs(int prio, int PC, char* name, uint32_t arg, State state, int sleep) {
	newPCBArgs.prio = prio;
	newPCBArgs.PC = PC;
	newPCBArgs.name = name;
	newPCBArgs.arg = arg;
	newPCBArgs.state = state;
	newPCBArgs.sleep = sleep;

	return kNewPCB(&newPCBArgs);
}
