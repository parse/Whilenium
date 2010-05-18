#include "API.h"

/*
 * kill(int PID)
 * Kill the process with the given PID with a syscall
 * @param int PID - Process to kill
 */
int kill(int PID) {
	syscall_kill(PID);
}

/*
 * sleep(int PID, int sleep)
 * Sleep the process PID for sleep iterations
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
