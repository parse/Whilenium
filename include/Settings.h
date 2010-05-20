#ifndef __SETTINGS__
#define __SETTINGS__

// ###############
// Settings for the OS
// PROCESSES are the maxmimum number of processes in the system at the same time
// PROCESSES_MEMORY_SIZE is the total size of the PCB array we are using as memory
// ###############

#define PROCESSES 1000
#define MEMORY_SIZE 3000000
#define FIFO_SIZE 1000

// Number of priorities in the system
#define PRIORITIES 400

// Names of available user programs are stored in the tables at the bottom
#define USERPROGRAMS 5

#define SCROLLWAIT 500

#define DEBUG 0

#endif
