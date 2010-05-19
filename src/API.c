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
int displayS(uint32_t str, uint8_t offset) {
	return syscall_displayS(str, offset);
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