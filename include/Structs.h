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

// Size of stack
typedef struct _stack {
	char space[(MEMORY_SIZE/PROCESSES)-200];
} Stack;

typedef struct _PCB {
	registers_t registers; // Register base for the PCB to store state at scheduling
	int stackLowEnd; // Low end of stack
	Stack stack; //Stack program;
	int stackHighEnd; // High end of the stack
	struct _PCB* next; // Next PCB in prio-queue
	struct _PCB* prev; // Previous PCB in prio-queue
	struct _PCB* nextIO; // If PCB is in IOQueue, this pointer points to the next PCB in queue
	char inputBuf[200]; // Input buffer
	int inputLength; // Length of the noncompleted inputBuf
	int ID; // PCB number
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

// A simple FIFO queue of bounded size.
typedef struct bounded_fifo {
  uint8_t  buf[FIFO_SIZE];
  uint32_t length;
} BFifo;

// Structure used to pass arguments to syscall, since we need more arguments than 4 and haven't implemented the syscall with a stack
typedef struct _newPCBArgs {
	int prio;
	int PC;
	char* name;
	uint32_t arg;
	State state; 
	int sleep;
} NewPCBArgs;
NewPCBArgs newPCBArgs;

// Output fifo queue
BFifo bFifoOut;

// Unique process id incremented int
int currentPID;

// Counter to indicate where we are in time
int timeCount;

// Tables of user programs and table containing addresses
int userProgramsAddresses[USERPROGRAMS];
char* userProgramsNames[USERPROGRAMS];

// String for the malta
char maltaText[8];

// Interrupt enabled flag
char interruptsEnabled;


#endif

