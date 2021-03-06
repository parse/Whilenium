#include "API.h"

/*
 * kill(int PID)
 * Kill the process with the given PID with a syscall, process 1 and 2 can't be destroyed
 * If PID = 0, kill the current process
 * @param int PID - Process to kill
 * @return -1 if PID = 1 or 2, or syscall_kill fails. Otherwise 1
 */
int kill(int PID) {
	
	if ( PID == 1 || PID == 2 || syscall_kill(PID) == -1 ) {
		return -1;
	}
	
	return 1;
}

/*
 * sleep(int PID, int sleep)
 * Sleep the process PID for sleep iterations, if PID == 0: sleep on current process
 * @param int PID - Process to sleep
 * @param int sleep - Time to sleep
 * @return -1 if (PID == 1) else syscall_sleep
 */
int sleep(int PID, int sleep) {
	if (PID != 1)
		return syscall_sleep(PID, sleep);
	
	return -1;
}

/*
 * getPrio(int PID)
 * Get priority for process PID
 * @param int PID - Process to get priority for
 */
int getPrio(int PID) {
	Process p = getProcess(PID);
	
	if(p.PID == -1)
		return -1;
		
	return p.prio;
		
}

/*
 * getState(int PID)
 * Get state for process PID
 * @param int PID - Process to get state for
 */
State getState(int PID) {
	Process p = getProcess(PID);
	
	if (p.PID == -1)
		return Undefined;
	
	return p.state;	
}

/*
 * getName(int PID)
 * Get name for process PID
 * @param int PID - Process to get name for
 */
char* getName(int PID) { 
	Process p = getProcess(PID);
	
	if(p.PID == -1)
		return (char*)-1;
	
	return p.name;
}

/*
 * changePrio(int PID, int prio)
 * Change priority for process PID
 * @param int PID - Process to change
 * @param int prio - New priority
 * @return 1 if succeeded, else -1 for failing
 */
int changePrio(int PID, int prio) {
	if ( PID == 1 || syscall_prio(PID, prio) == -1 ) {
		return -1;
	}
	
	return 1;
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
int displayS(uint32_t str, uint8_t offset) {
	syscall_displayS(str, offset);
	
	return 1;
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
 * @return Newly created PID on success, else -1 when failing
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
 * @return 1
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
int top() {
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
				case RequestingIO:
					puts("RequestingIO");
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
	
	return 1;
}

