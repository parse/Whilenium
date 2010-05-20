#include "Process.h"

char scroll[] = "While(1)";

void welcomeText() {
	putslnDebug("              ___________________________________________________");
	putslnDebug("            /                                                    \\");
	putslnDebug("           |    _____________________________________________     |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |  Whilenium:\\> _                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |_____________________________________________|    |");
	putslnDebug("           |                                                      |");
	putslnDebug("            \\_____________________________________________________/");
	putslnDebug("                   \\_______________________________________/");
	putslnDebug("                _______________________________________________");
	putslnDebug("             _-'    .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.  --- `-_");
	putslnDebug("          _-'.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.--.  .-.-.`-_");
	putslnDebug("       _-'.-.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-`__`. .-.-.-.`-_");
	putslnDebug("    _-'.-.-.-.-. .-----.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-----. .-.-.-.-.`-_");
	putslnDebug(" _-'.-.-.-.-.-. .---.-. .-----------------------------. .-.---. .---.-.-.-.`-_");
	putslnDebug(":-----------------------------------------------------------------------------:");
	putslnDebug("`---._.-----------------------------------------------------------------._.---'");
	putslnDebug("");
	putslnDebug(" __      __.__    .__.__                  .__");               
	putslnDebug("/  \\    /  \\  |__ |__|  |    ____    ____ |__|__ __  _____ "); 
	putslnDebug("\\   \\/\\/   /  |  \\|  |  |  _/ __ \\  /    \\|  |  |  \\/     \\ ");
	putslnDebug(" \\        /|   Y  \\  |  |__\\  ___/ |   |  \\  |  |  /  Y Y  \\");
	putslnDebug("  \\__/\\  / |___|  /__|____/ \\___  >|___|  /__|____/|__|_|  /");
	putslnDebug("       \\/       \\/              \\/      \\/               \\/ ");
	putslnDebug(" Authors: Jesper Friberg, Anders Hassis, Daniel Lervik & Oskar Wiren");
	putslnDebug(" Uppsala University 2010.\n");
}

/**
 * initOS()
 * Initiates the OS essentials (including the first process)
 */
void initOS(int memoryMin) {
	welcomeText();
	
	initPCBTable(memoryMin);
	
	// Create base processes for the OS
	kNewPCBWithArgs(PRIORITIES, (int)&Idle, "Idle process",  (int)NULL, New, 0);
	kNewPCBWithArgs(2, (int)&Scroller, "Scroller", (int)&scroll, Waiting, 0);
	kNewPCBWithArgs(PRIORITIES-1, (int)&Shell, "Shell", (int)NULL, New, 0);
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
	// Kill the process by making the die syscall
	syscall_die();
	
	// Error safety, making sure we never fall out into outer space
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
int kNewPCB(NewPCBArgs* newPCBArgs) {
	PCB* pcb = getFreePCB();

	if ((int)pcb == -1)
		return -1;
		
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
			
		run();
	}
	
	return (int)pcb;
}

int kNewPCBWithArgs(int prio, int PC, char* name, uint32_t arg, State state, int sleep) {
	newPCBArgs.prio = prio;
	newPCBArgs.PC = PC;
	newPCBArgs.name = name;
	newPCBArgs.arg = arg;
	newPCBArgs.state = state;
	newPCBArgs.sleep = sleep;

	return kNewPCB(&newPCBArgs);
}

/*
 * Idle()
 * An idle process that the OS always keep running
 */
void Idle() {
	while(1);
}

