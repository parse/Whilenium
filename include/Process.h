#ifndef __PROCESS__
#define __PROCESS__

#include "IOHandler.h"
#include "Interrupt.h"
#include "API.h"
#include "Scheduler.h"
#include "Memory.h"
#include "Settings.h"
#include "types.h"
#include "stdlib.h"
#include "UserPrograms.h"

enum states {
	New,
	Running,
	Waiting,
	Blocked, 
	Ready,
	Terminated 
};

// Disclaimer: Because anders told me to comment, here we go.
// Size of stack
typedef struct _stack {
	char space[(MEMORY_SIZE/PROCESSES)-60];
} Stack;
 
typedef struct _PCB {
	Stack program;//Stack program;
	struct _PCB* next;
	struct _PCB* prev;
	int PID; // Unique identifier
	int memMax; // Max memory access space
	int memMin; // Min memory access space, start value
	int prio; // Integer value describing the process priority
	int PC; // Program counter
	int SP; // Stack pointer
	char name[20]; // Human readable name
	enum states state; // State of the PCB-entry (New, Running, Waiting, Blocked, Ready, Terminated)
} PCB;

/*
TODO: Make it public (and maybe move it)
*/
typedef struct _Process {
	int PID; // Unique identifier
	char* name; // Name of the program
	int prio; // Integer value describing the process priority
	enum states state;// State of the PCB-entry (New, Running, Waiting, Blocked, Ready, Terminated)
	char* programName; // Name of the program
} Process;

/*
TODO:
typedef struct _ProcessTable {
	PCB* next;
	Process p;
} ProcessTable;
*/

void initOS(int memoryMin);
void initPCBTable(int memoryMin);
int newPCB(int prio, int PC, char* name);

#endif
