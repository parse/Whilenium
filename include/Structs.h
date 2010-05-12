#ifndef __STRUCTS__
#define __STRUCTS__

#include "Settings.h"
#include "mips/registers.h"
#include "mips/types.h"

typedef enum _State {
	New,
	Running,
	Waiting,
	Blocked, 
	Ready,
	Terminated,
	Undefined
} State;

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
	int sleep; // Sleeptime
	int memMax; // Max memory access space
	int memMin; // Min memory access space, start value
	int prio; // Integer value describing the process priority
	char name[20]; // Human readable name
	State state; // State of the PCB-entry (New, Running, Waiting, Blocked, Ready, Terminated)
} PCB;


/*
TODO: Make it public (and maybe move it)
*/
typedef struct _Process {
	int PID; // Unique identifier
	char* name; // Name of the program
	int prio; // Integer value describing the process priority
	State state;// State of the PCB-entry (New, Running, Waiting, Blocked, Ready, Terminated)
	char* programName; // Name of the program
} Process;

/* A simple FIFO queue of bounded size. */
typedef struct bounded_fifo {
  uint8_t  buf[FIFO_SIZE];
  uint32_t length;
} BFifo;

BFifo bFifoOut;
BFifo bFifoIn;

int currentPID;
int timeCount;


#endif

