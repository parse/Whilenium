#ifndef __STRUCTS__
#define __STRUCTS__

#include "Settings.h"
#include "mips/registers.h"
#include "mips/types.h"

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
	char space[(MEMORY_SIZE/PROCESSES)-200];
} Stack;

typedef struct _PCB {
	registers_t registers;
	int stackLowEnd;
	Stack stack; //Stack program;
	int stackHighEnd;
	struct _PCB* next;
	struct _PCB* prev;
	int PID; // Unique identifier
	int memMax; // Max memory access space
	int memMin; // Min memory access space, start value
	int prio; // Integer value describing the process priority
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
#endif

