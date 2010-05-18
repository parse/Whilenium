#include "API.h"

int kill(int PID) {
	syscall_kill(PID);
}

void sleep(int PID, int sleep) {
	syscall_sleep(PID, sleep);
}