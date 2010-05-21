#ifndef __SYSCALLS__
#define __SYSCALLS__

#include "mips/types.h"
#include "mips/malta.h"

#include "Structs.h"
#include "Scheduler.h"
#include "Process.h"
#include "IOHandler.h"
#include "Settings.h"

int kExec(char* program, int priority, uint32_t arg);
int kBlock(int pid);
int kUnblock(int pid);
int kKill(int pid);
int kSleep(int pid, int sleepTime);
int kChangePrio(int pid, int prio);

int kNewPCB(NewPCBArgs* newPCBArgs);
int kNewPCBWithArgs(int prio, int PC, char* name, uint32_t arg, State state, int sleep);

#endif
