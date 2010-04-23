#include "Process.h"
#include <string.h>

/**
 * initOS()
 * Initiates the OS essentials (including the first process)
 */
void initOS(int memoryMin) {
	initPCBTable(memoryMin);
	
	PCB* PCBTable = (PCB*)memoryMin;
	
	puts(PCBTable[0].name);
}

void initPCBTable(int memoryMin) {
	PCB* PCBTable = (PCB*)memoryMin;
	int i;
	
	for (i = 0; i < PROCESSES; i++) {
		PCBTable[i].PID = -1;
		PCBTable[i].memMax = 0;
		PCBTable[i].memMin = 0;
		PCBTable[i].prio = -1;
		PCBTable[i].PC = 0;
		PCBTable[i].name[0] = 'F';
		PCBTable[i].name[1] = 'u';
		PCBTable[i].name[2] = 'c';
		PCBTable[i].name[3] = 'k';
		PCBTable[i].name[4] = '\0';
	}
}
