#include "Process.h"

/**
 * initOS()
 * Initiates the OS essentials (including the first process)
 */
void initOS(int memoryMin) {
	int i;
	char buf[10];
	
	initPCBTable(memoryMin);
	
	PCB* PCBTable = (PCB*)memoryMin;
	
	puts("Running: \n");
	for (i = 0; i < PROCESSES; i++) {
		puts(PCBTable[i].name);
		puts(itoa(i, buf, 10));
		puts("\n");
	}
	
	puts("sizeof(PCB) = ");
	puts(itoa(sizeof(PCB), buf, 10));
}

void initPCBTable(int memoryMin) {
	PCB* PCBTable = (PCB*)memoryMin;
	
	int i;	
	char tmp[10];
	for (i = 0; i < PROCESSES; i++) {
		puts("Inserted PCB: 0x");
		int pcb = &PCBTable[i];
		pcb += 2147483648;
		puts(itoa(pcb, tmp, 16));
		puts(" ");
		
		PCBTable[i].PID = -1;
		PCBTable[i].memMax = 0;
		PCBTable[i].memMin = 0;
		PCBTable[i].prio = 0;
		PCBTable[i].PC = 0;
		PCBTable[i].name = "Test ";

		insertPCB(&PCBTable[i]);
	}
	
	puts("\n");
}
