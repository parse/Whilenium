#ifndef __BOOT__
#define __BOOT__

#include "mips/types.h"
#include "mips/asm.h"
#include "Settings.h"

#include "Structs.h"
#include "Scheduler.h"
#include "Process.h"
#include "IOHandler.h"
#include "Interrupt.h"


int OS(int memoryMin);
void ksyscall_handler(registers_t* reg);

#endif
