#ifndef __PROCESS__
#define __PROCESS__

#include "IOHandler.h"
#include "Interrupt.h"
#include "API.h"
#include "Scheduler.h"
#include "Memory.h"

struct _PCB {
	int PID; // Unique identifier
	int memMax; // Max memory access space
	int memMin; // Min memory access space, start value
	int prio; // Integer value describing the process priority
	struct PCB* nextPCB; // Address to the next PCB to "run"
//	IOList* IOList, // List of devices that the PCB is using
	int PC; // Program counter
	char* name; // Human readable name
	char* state; // State of the PCB-entry (New, Running, Waiting, Blocked, Ready, Terminated)
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

void initOS();

#endif
