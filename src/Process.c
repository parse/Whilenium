#include "Process.h"

int currentPID = 0;

/**
 * initOS()
 * Initiates the OS essentials (including the first process)
 */
void initOS(int memoryMin) {
	puts("ALERT ALERT!!! INITIALIZING THE OS!\n\n");
	
	int i;
	
	initPCBTable(memoryMin);

	for (i = 0; i < PROCESSES; i++) {
		newPCB((i % PRIORITIES) + 1, (int)&HelloWorld, "Executable: Hello World");
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

void exitProcess() {
	putsln("\n\t------ Process ended ------");
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

