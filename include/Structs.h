#ifndef __STRUCTS__
#define __STRUCTS__

#include "Settings.h"
#include "mips/registers.h"
#include "mips/types.h"

typedef enum _State {
	New,
	Running,
	RequestingIO,
	Waiting,
	Blocked, 
	Ready,
	Terminated,
	Undefined
} State;

typedef struct _IOQueue {
	struct _PCB* last;
	struct _PCB* current;
} IOQueue;

IOQueue ioqueue;

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
	struct _PCB* nextIO;
	char inputBuf[200];
	int inputLength;
	int ID;
	int PID; // Unique identifier
	int sleep; // Sleeptime
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
} Process;

/* A simple FIFO queue of bounded size. */
typedef struct bounded_fifo {
  uint8_t  buf[FIFO_SIZE];
  uint32_t length;
} BFifo;

typedef struct _newPCBArgs {
	int prio;
	int PC;
	char* name;
	uint32_t arg;
	State state; 
	int sleep;
} NewPCBArgs;

BFifo bFifoOut;
BFifo bFifoIn;

int currentPID;
int timeCount;

// Tables of user programs and table containing addresses
int userProgramsAddresses[USERPROGRAMS];
char* userProgramsNames[USERPROGRAMS];

char maltaText[8];

char interruptsEnabled;

NewPCBArgs newPCBArgs;

#endif

