#include "Process.h"

/**
 * initOS()
 * Initiates the OS essentials (including the first process)
 */
void initOS(int memoryMin) {
	putsDebug("ALERT ALERT!!! INITIALIZING THE OS!\n\n");
	
	int i;
		
	initPCBTable(memoryMin);
	
	uint32_t argv[4];
	
	/*for (i = 0; i < PROCESSES; i++) {
		argv[0] = 0;
		
		switch (i % 3) {
			case 0:
				argv[0] = 5;
				newPCB((i % PRIORITIES-1) + 1, (int)&Increment, "Increment", argv, Ready, 0);
				break;
			case 1:
				argv[0] = 6;
				newPCB((i % PRIORITIES-1) + 1, (int)&Fibonacci, "Fibonacci", argv, Ready, 0);
				break;
			case 2:
				newPCB((i % PRIORITIES-1) + 1, (int)&HelloWorld, "Hello World", argv, Ready, 00);
				break;
		}		
	}*/
	
	argv[0] = 0;
	newPCB(PRIORITIES, (int)&Shell, "Shell", argv, Ready, 0);
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
		
		PCBTable[i].ID = i;
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
int newPCB(int prio, int PC, char* name, uint32_t* argv, State state, int sleep) {
	PCB* pcb = getFreePCB();
	
	pcb->prio = prio;
	
	strcpy(pcb->name, name);
	
	currentPID++;
	pcb->PID = currentPID;
	pcb->registers.epc_reg = PC;
	pcb->registers.sp_reg = (uint32_t)&(pcb->stackHighEnd);
	pcb->registers.a_reg[0] = argv[0];
	pcb->registers.a_reg[1] = 0;
	pcb->registers.a_reg[2] = 0;
	pcb->registers.a_reg[3] = 0;
	pcb->registers.ra_reg = (uint32_t)&exitProcess;
	
	pcb->state = state;
	pcb->sleep = timeCount + sleep;
	insertPCB(pcb);
	
	return (int)pcb;
}

void parser(char* s) {
	// Split the string into arguments [0] = program name, [1],... = arguments
	
	// Check which program to run and pass arguments by comparing strings in the Userprogram table
}