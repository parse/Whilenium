#include "Process.h"

int currentPID = 0;

/**
 * initOS()
 * Initiates the OS essentials (including the first process)
 */
void initOS(int memoryMin) {
	puts("initOS: \n");
	
	int i;
	char buf[10];
	
	initPCBTable(memoryMin);
	
	PCB* PCBTable = (PCB*)memoryMin;
	/*
	for (i = 0; i < PROCESSES; i++) {
		puts(PCBTable[i].name);
		puts(itoa(i, buf, 10));
		puts("\n");
	}*/
	
	//puts("sizeof(PCB) = ");
	//puts(itoa(sizeof(PCB), buf, 10));

	newPCB(1, &HelloWorld, "Process #1");
	newPCB(1, &HelloWorld2, "Process #2");
	newPCB(1, &HelloWorld3, "Process #3");
	//i = newPCB(2, 0, "Process #2");
	//puts("\nnewPCB done: ");
	//puts(itoa(i, buf, 10));	
}

/**
 * initPCBTable(int memoryMin)
 * Initialize PCB table
 * @param int memoryMin - The "start" memory address
 */
void initPCBTable(int memoryMin) {
	PCB* PCBTable = (PCB*)memoryMin;
	
	int i;	
	char tmp[10];
	for (i = 0; i < PROCESSES; i++) {
		puts("Inserted PCB: 0x");
		int pcb = (int)&PCBTable[i];
		//pcb += 2147483648;
		puts(itoa(pcb, tmp, 16));
		puts(" ");
		
		PCBTable[i].PID = -1;
		PCBTable[i].memMax = 0;
		PCBTable[i].memMin = pcb;
		PCBTable[i].prio = 0;
		PCBTable[i].PC = 0;
		PCBTable[i].SP = pcb;
		PCBTable[i].name[0] = 'T';
		PCBTable[i].name[1] = 'e';
		PCBTable[i].name[2] = 's';
		PCBTable[i].name[3] = 't';
		PCBTable[i].name[4] = ' ';
		PCBTable[i].name[5] = ' ';
		PCBTable[i].name[6] = '\0'; //= "Test  ";
		
		const char c = *itoa(i, tmp, 10);
		PCBTable[i].name[5] = c;
		
		insertPCB(&PCBTable[i]);
	}
	
	puts("\n");
}

void exitProcess() {
	putsln("A process is terminated!");
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
	pcb->PC = PC;
	
	strcpy(pcb->name, name);
	
	currentPID++;
	pcb->PID = currentPID;
	
	puts("newPCB->prio ");
	char buf[10];
	puts(itoa(pcb->prio, buf, 10));
	
	pcb->registers.epc_reg = PC;
	pcb->registers.ra_reg = &exitProcess;
	pcb->registers.sp_reg = &(pcb->program);
	//pcb->registers.s_reg[0]
	
	insertPCB(pcb);
	
	return (int)pcb;
}
