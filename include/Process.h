#ifndef __PROCESS__
#define __PROCESS__

#include "IOHandler.h"
#include "Interrupt.h"
#include "API.h"
#include "Scheduler.h"
#include "Memory.h"
#include "Settings.h"
#include "types.h"

enum states {
	New,
	Running,
	Waiting,
	Blocked, 
	Ready,
	Terminated 
};
 
typedef struct _PCB {
	struct _PCB* next;
	struct _PCB* prev;
	int PID; // Unique identifier
	int memMax; // Max memory access space
	int memMin; // Min memory access space, start value
	int prio; // Integer value describing the process priority
	int PC; // Program counter
	char name[50]; // Human readable name
	enum states state; // State of the PCB-entry (New, Running, Waiting, Blocked, Ready, Terminated)
} PCB;

/*
TODO: Make it public (and maybe move it)
*/
typedef struct _Process {
	int PID; // Unique identifier
	char* name; // Name of the program
	int prio; // Integer value describing the process priority
	char* state; // State of the PCB-entry (New, Running, Waiting, Blocked, Ready, Terminated)
	char* programName; // Name of the program
} Process;

void initOS(int memoryMin);
void initPCBTable(int memoryMin);

#endif
