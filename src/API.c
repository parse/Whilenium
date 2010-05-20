#include "API.h"

/*
 * kill(int PID)
 * Kill the process with the given PID with a syscall, process 1 can't be destroyed
 * @param int PID - Process to kill
 */
int kill(int PID) {
	if (PID != 1)
		return syscall_kill(PID);
	else
		return -1;
}

/*
 * sleep(int PID, int sleep)
 * Sleep the process PID for sleep iterations, if PID == 0: sleep on current process
 * @param int PID - Process to sleep
 * @param int sleep - Time to sleep
 */
void sleep(int PID, int sleep) {
	syscall_sleep(PID, sleep);
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
 * unblock(int PID)
 * Unblock process PID
 * @param int PID - Process to unblock
 */
void displayS(uint32_t str, uint8_t offset) {
	syscall_displayS(str, offset);
}

int spawn(int prio, int PC, char* name, uint32_t arg, State state, int sleep) {
	newPCBArgs.prio = prio;
	newPCBArgs.PC = PC;
	newPCBArgs.name = name;
	newPCBArgs.arg = arg;
	newPCBArgs.state = state;
	newPCBArgs.sleep = sleep;
	
	return syscall_spawn(&newPCBArgs);
}

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
		
	return 0;
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

