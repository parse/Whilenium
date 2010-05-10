#include "Process.h"

/**
 * initOS()
 * Initiates the OS essentials (including the first process)
 */
void initOS(int memoryMin) {
	puts("ALERT ALERT!!! INITIALIZING THE OS!\n\n");
	
	int i;
	
	initPCBTable(memoryMin);

	for (i = 0; i < PROCESSES; i++) {
		switch (i % 3) {
			case 0:
				newPCB((i % PRIORITIES) + 1, (int)&HelloWorld, "Executable: Hello World");
				break;
			case 1:
				newPCB((i % PRIORITIES) + 1, (int)&HelloWorld2, "Executable: Hello World");
				break;
			case 2:
				newPCB((i % PRIORITIES) + 1, (int)&HelloWorld3, "Executable: Hello World");
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
		
		insertPCB(&PCBTable[i]);
	}
}

/*
 * exitProcess()
 * Takes care of the process ended
 */
void exitProcess() {
	putsln("\n\t------ Process ended ------");
	
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
int newPCB(int prio, int PC, char* name) {
	static int currentPID = 0;
	PCB* pcb = getFreePCB();
	
	pcb->prio = prio;
	
	strcpy(pcb->name, name);
	
	currentPID++;
	pcb->PID = currentPID;
	
	pcb->registers.epc_reg = PC;
	pcb->registers.sp_reg = (uint32_t)&(pcb->stackHighEnd);
	pcb->registers.ra_reg = (uint32_t)&exitProcess;
	
	insertPCB(pcb);
	
	return (int)pcb;
}

