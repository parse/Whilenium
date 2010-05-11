#include "Process.h"

/**
 * initOS()
 * Initiates the OS essentials (including the first process)
 */
void initOS(int memoryMin) {
	puts("ALERT ALERT!!! INITIALIZING THE OS!\n\n");
	
	int i;
	
	initPCBTable(memoryMin);
	//newPCB(1, (int)&HelloWorld, "Increment 1, wait", Waiting, 2000);
	//newPCB(1, (int)&HelloWorld, "Increment 2", Ready, 0);
	//newPCB(2, (int)&HelloWorld, "Increment 3", Ready, 0);

	for (i = 0; i < PROCESSES; i++) {
		switch (i % 3) {
			case 0:
				newPCB((i % PRIORITIES) + 1, (int)&HelloWorld2, "Increment 1", Ready, 0);
				break;
			case 1:
				newPCB((i % PRIORITIES) + 1, (int)&HelloWorld3, "Increment 2", Waiting, 40);
				break;
			case 2:
				newPCB((i % PRIORITIES) + 1, (int)&HelloWorld, "Hello", Blocked, 0);
				break;
		}		
	}
}

/**
 * initPCBTable(int memoryMin)
 * Initialize PCB table
 * @param int memoryMin - The "start" memory address
 */
void initPCBTable(int memoryMin) {
	PCB* PCBTable = (PCB*)memoryMin;
	
	int i;	
	for (i = 0; i < PROCESSES; i++) {
		int pcb = (int)&PCBTable[i];
		
		PCBTable[i].PID = -1;
		PCBTable[i].memMax = 0;
		PCBTable[i].memMin = pcb;
		PCBTable[i].prio = 0;
		PCBTable[i].state = New;
		
		insertPCB(&PCBTable[i]);
	}
}

/*
 * exitProcess()
 * Takes care of the process ended
 */
void exitProcess() {
	//putsln("\n\t------ Process ended ------");
	
	// Kill the process by making the die syscall
	syscall_die();
	
	while (1);
}

/**
 * newPCB(int prio, int PC, char* name)
 * Important! We must insert the PCB from getFreePCB into a new queue with insertPCB
 * Pre: prio > 0
 * @param int prio - The priority to set
 * @param int PC - Program counter to set
 * @param char* name - Program name to set
 */
int newPCB(int prio, int PC, char* name, State state, int sleep) {
	static int currentPID = 0;
	PCB* pcb = getFreePCB();
	
	pcb->prio = prio;
	
	strcpy(pcb->name, name);
	
	currentPID++;
	pcb->PID = currentPID;
	
	pcb->registers.epc_reg = PC;
	pcb->registers.sp_reg = (uint32_t)&(pcb->stackHighEnd);
	pcb->registers.ra_reg = (uint32_t)&exitProcess;
	
	pcb->state = state;
	pcb->sleep = timeCount + sleep;
	insertPCB(pcb);
	
	return (int)pcb;
}

