#include "API.h"

int kill(int PID) {
	syscall_kill(PID);
}

void sleep(int PID, int sleep) {
	syscall_sleep(PID, sleep);
}


int getPrio(int PID) {
	Process p = getProcess(PID);
	
	return p.prio;
}

State getState(int PID) {
	Process p = getProcess(PID);
	
	return p.state;	
}

char* getName(int PID) { 
	Process p = getProcess(PID);
	
	return p.name;
}
