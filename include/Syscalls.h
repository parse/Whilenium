#ifndef __SYSCALLS__
#define __SYSCALLS__

#include "mips/types.h"
#include "mips/malta.h"

#include "Structs.h"
#include "Scheduler.h"
#include "Process.h"
#include "IOHandler.h"
#include "Settings.h"

// PILAR FUNKAR EJ! (inte chars)
#define RIGHTARROW 0x43
#define LEFTARROW 0x44
#define UPARROW 0x41
#define DOWNARROW 0x42

#define ESCAPE 0x1b
#define SKIP 0x5b
#define BACKSPACE 0x8
#define TAB 0x9

int kExec(char* program, int priority, uint32_t arg);
int kBlock(int pid);
int kUnblock(int pid);
int kKill(int pid);
int kSleep(int pid, int sleepTime);
int kChangePrio(int pid, int prio);
char* kGets();
void kInput(char c);

int kNewPCB(NewPCBArgs* newPCBArgs);
int kNewPCBWithArgs(int prio, int PC, char* name, uint32_t arg, State state, int sleep);

#endif
