#include "API.h"

int kill(int PID) {
	freePID(PID);
	return 1;
	/*if (freePID(PID) == -1)
		putsln("Error: Could not kill process with given PID");*/
}

void sleep(int PID, int sleep) {
	syscall_sleep(PID, sleep);
}