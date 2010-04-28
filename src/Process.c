#include "Process.h"

/**
 * initOS()
 * Initiates the OS essentials (including the first process)
 */
void initOS(int memoryMin) {
	initPCBTable(memoryMin);
	
	PCB* PCBTable = (PCB*)memoryMin;
	
	puts("Running: ");
	puts(PCBTable[0].name);
	
}

void initPCBTable(int memoryMin) {
	PCB PCBTable[PROCESSES];
	//PCB* PCBTable = (PCB*)memoryMin;
	
	PCBTable[0].PID = -1;
	PCBTable[0].memMax = memoryMin+(MEMORY_SIZE/PROCESSES)-1;
	PCBTable[0].memMin = memoryMin;
	PCBTable[0].prio = 1;
	PCBTable[0].PC = 0;
	PCBTable[0].name = "Test";
	insertPCB(PCBTable[0]);
	
	PCBTable[1].PID = -1;
	PCBTable[1].memMax = memoryMin+2*(MEMORY_SIZE/PROCESSES)-1;
	PCBTable[1].memMin = memoryMin+(MEMORY_SIZE/PROCESSES);
	PCBTable[1].prio = 1;
	PCBTable[1].PC = 0;
	PCBTable[1].name = "Snorboll";
	insertPCB(PCBTable[1]);
	
	
/*	
	int i;
	for (i = 0; i < PROCESSES; i++) {
		PCBTable[i].PID = -1;
		PCBTable[i].memMax = 0;
		PCBTable[i].memMin = 0;
		PCBTable[i].prio = -1;
		PCBTable[i].PC = 0;
		PCBTable[i].name = "Test";

		insertPCB(PCBTable[i]);
	}*/
}
