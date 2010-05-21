#include "API.h"

/*
 * kill(int PID)
 * Kill the process with the given PID with a syscall, process 1 can't be destroyed
 * @param int PID - Process to kill
 */
int kill(int PID) {
	
	if ( syscall_kill(PID) == -1 || PID == 1 ) {
		return -1;
	}
	
	return 1;
}

/*
 * sleep(int PID, int sleep)
 * Sleep the process PID for sleep iterations, if PID == 0: sleep on current process
 * @param int PID - Process to sleep
 * @param int sleep - Time to sleep
 */
int sleep(int PID, int sleep) {
	return syscall_sleep(PID, sleep);
}

/*
 * getPrio(int PID)
 * Get priority for process PID
 * @param int PID - Process to get priority for
 */
int getPrio(int PID) {
	Process p = getProcess(PID);
	
	return p.prio;
}

/*
 * getState(int PID)
 * Get state for process PID
 * @param int PID - Process to get state for
 */
State getState(int PID) {
	Process p = getProcess(PID);
	
	return p.state;	
}

/*
 * getName(int PID)
 * Get name for process PID
 * @param int PID - Process to get name for
 */
char* getName(int PID) { 
	Process p = getProcess(PID);
	
	return p.name;
}

/*
 * changePrio(int PID, int prio)
 * Change priority for process PID
 * @param int PID - Process to change
 * @param int prio - New priority
 */
int changePrio(int PID, int prio) {
	return syscall_prio(PID, prio);
}

/*
 * block(int PID)
 * Block process PID
 * @param int PID - Process to block
 */
int block(int PID) {
	return syscall_block(PID);
}

/*
 * unblock(int PID)
 * Unblock process PID
 * @param int PID - Process to unblock
 */
int unblock(int PID) {
	return syscall_unblock(PID);
}

/*
 * displayS(uint32_t str, uint8_t offset)
 * Display string S on malta with offset
 * @param uint32_t str - String to display
 * @param uint8_t offset - Offset to use
 */
void displayS(uint32_t str, uint8_t offset) {
	syscall_displayS(str, offset);
}

/*
 * spawn(int prio, int PC, char* name, uint32_t arg, State state, int sleep) 
 * Spawn a new process with given parameters and run it with a syscall
 * @param int prio - Priority to use, from 0-PRIORITIES 
 * @param int PC - Address to the program
 * @param char* name - Name of the program
 * @param uint32_t arg - Argument to pass to our program
 * @param State state - State of the process
 * @param int sleep - If state is Waiting, enter sleeptime here
 * @return -1 if fails
 */
int spawn(int prio, int PC, char* name, uint32_t arg, State state, int sleep) {
	newPCBArgs.prio = prio;
	newPCBArgs.PC = PC;
	newPCBArgs.name = name;
	newPCBArgs.arg = arg;
	newPCBArgs.state = state;
	newPCBArgs.sleep = sleep;
	
	return syscall_spawn(&newPCBArgs);
}

/*
 * scroller(char* msg)
 * Display string msg on Malta display
 * @param char* msg - String to display
 * @return -1
 */
int scroller(char* msg) {
	int i = 0, strEnd = -1;

	for (i = 0; i <= 7; i++) {
		if (msg[i] == '\0') {
			strEnd = i;
			break;
		} else
			maltaText[i] = msg[i];
	}

	if (strEnd == -1)
		strEnd = 8;

	for (i = strEnd; i <= 7; i++)
		maltaText[i] = ' ';
		
	return 1;
}

/*
 * top()
 * Show process information for the whole system
 */
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
				case Terminated:
				case Undefined:
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

