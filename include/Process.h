#ifndef __PROCESS__
#define __PROCESS__

#include "mips/types.h"
#include "mips/registers.h"
#include "stdlib.h"

#include "Settings.h"
#include "Structs.h"

#include "IOHandler.h"
#include "Interrupt.h"
#include "API.h"
#include "Scheduler.h"
#include "UserPrograms.h"
#include "Shell.h"
#include "_Boot.h"

void initOS(int memoryMin);
void initPCBTable(int memoryMin);
void exitProcess();

void Idle();

#endif
