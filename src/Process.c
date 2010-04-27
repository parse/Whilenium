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
	PCB* PCBTable = (PCB*)memoryMin;
	int i;
	
	PCBTable[0].PID = -1;
	PCBTable[0].memMax = 0;
	PCBTable[0].memMin = 0;
	PCBTable[0].prio = -1;
	PCBTable[0].PC = 0;
	PCBTable[0].name = "Test";
	insertPCB(PCBTable[0]);
	
/*	for (i = 0; i < PROCESSES; i++) {
		PCBTable[i].PID = -1;
		PCBTable[i].memMax = 0;
		PCBTable[i].memMin = 0;
		PCBTable[i].prio = -1;
		PCBTable[i].PC = 0;
		PCBTable[i].name = "Test";

		insertPCB(PCBTable[i]);
	}*/
}
