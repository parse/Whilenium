#include "Process.h"

/**
 * initOS()
 * Initiates the OS essentials (including the first process)
 */
void initOS(int memoryMin) {
	putslnDebug(" __      __.__    .__.__                  .__");               
	putslnDebug("/  \\    /  \\  |__ |__|  |    ____    ____ |__|__ __  _____ "); 
	putslnDebug("\\   \\/\\/   /  |  \\|  |  |  _/ __ \\  /    \\|  |  |  \\/     \\ ");
	putslnDebug(" \\        /|   Y  \\  |  |__\\  ___/ |   |  \\  |  |  /  Y Y  \\");
	putslnDebug("  \\__/\\  / |___|  /__|____/ \\___  >|___|  /__|____/|__|_|  /");
	putslnDebug("       \\/       \\/              \\/      \\/               \\/ ");
	putslnDebug(" Authors: Jesper Friberg, Anders Hassis, Daniel Lervik & Oskar Wiren");
	putslnDebug(" Uppsala University 2010.\n");
	initPCBTable(memoryMin);
	
	newPCB(PRIORITIES, (int)&Idle, "Idle process",  (int)NULL, Ready, 0);
	newPCB(PRIORITIES-1, (int)&Shell, "Shell", (int)NULL, Ready, 0);
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
 * @param uint32_t arg - Argument to pass to process
 * @param State state - State to start the process in
 * @param int sleep - If state is Waiting, the sleeptime is how long the process waits
 * @return The PCB created with above values
 */
int newPCB(int prio, int PC, char* name, uint32_t arg, State state, int sleep) {
	PCB* pcb = getFreePCB();
	
	if ((int)pcb == -1)
		return -1;
		
	pcb->prio = prio;
	
	strcpy(pcb->name, name);
	
	currentPID++;
	pcb->PID = currentPID;
	pcb->registers.epc_reg = PC;
	pcb->registers.sp_reg = (uint32_t)&(pcb->stackHighEnd);
	pcb->registers.a_reg[0] = arg;
	pcb->registers.a_reg[1] = 0;
	pcb->registers.a_reg[2] = 0;
	pcb->registers.a_reg[3] = 0;
	pcb->registers.ra_reg = (uint32_t)&exitProcess;
	
	pcb->state = state;
	pcb->sleep = timeCount + sleep;
	insertPCB(pcb);
	
	if (interruptsEnabled)
		syscall_schedule();
	
	return (int)pcb;
}

/*
 * top()
 * Show process information for the whole system
 */
void top() {
	int i;
	char buf[10];
	
	for (i = 1; i <= PRIORITIES; i++) {
		PCB* current = PriorityArray[i].current; 
		PCB* first = current;
		
		while (current != NULL) {
			puts("Process ID: ");
			puts(itoa(current->PID, buf, 10));
			puts(", Priority: ");
			puts(itoa(i, buf, 10));
			puts(", State: ");

			switch (current->state) {
				case New:
					puts("New");
					break;
				case Running:
					puts("Running");
					break;
				case Waiting:
					puts("Waiting");
					break;
				case Blocked:
					puts("Blocked");
					break;
				case Ready:
					puts("Ready");
					break; 
				case Terminated:
				case Undefined:
					break;
			}

			puts(", Name: ");
			putsln(current->name);
			
			if (current->next == first)
				break;
			else
				current = current->next;
		}
	}
}

/*
 * changePrio(int PID, int prio)
 * Change priority for process PID
 * @param int PID - Process to change
 * @param int prio - New priority
 */
int changePrio(int PID, int prio) {
	return syscall_prio(PID, prio);
}

/*
 * block(int PID)
 * Block process PID
 * @param int PID - Process to block
 */
int block(int PID) {
	return syscall_block(PID);
}

/*
 * unblock(int PID)
 * Unblock process PID
 * @param int PID - Process to unblock
 */
int unblock(int PID) {
	return syscall_unblock(PID);
}

/*
 * Idle()
 * An idle process that the OS always keep running
 */
void Idle() {
	while(1);
}


