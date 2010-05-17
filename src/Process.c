#include "Process.h"

/**
 * initOS()
 * Initiates the OS essentials (including the first process)
 */
void initOS(int memoryMin) {
	putsDebug("ALERT ALERT!!! INITIALIZING THE WHILENIUM!\n\n");
	
	initPCBTable(memoryMin);
	
	newPCB(PRIORITIES, (int)&Shell, "Shell", (int)NULL, Ready, 0);
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
int newPCB(int prio, int PC, char* name, uint32_t arg, State state, int sleep) {
	PCB* pcb = getFreePCB();
	
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
	
	return (int)pcb;
}

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

void changePrio(int PID, int prio) {
	// 
	PCB* entry = getPCB(PID);
	
	PCB* prev = entry->prev;
	PCB* next = entry->next;
	
	if (entry != next) {
		PriorityArray[entry->prio].current = next;
		prev->next = next;
		next->prev = prev;
	} else
		PriorityArray[entry->prio].current = NULL;
	
	entry->next = NULL;
	entry->prev = NULL;
	
	entry->prio = prio;

	insertPCB(entry);
}

int block(int PID) {
	PCB* entry = getPCB(PID);
	
	if ( (entry->state = Blocked) )
		return 1;
	else
		return -1;
}

int unblock(int PID) {
	PCB* entry = getPCB(PID);
	
	if ( (entry->state = Ready) )
		return 1;
	else
		return -1;
}




