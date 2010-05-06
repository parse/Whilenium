#ifndef __BOOT__
#define __BOOT__

/* #include "types.h"  */
#include "mips/asm.h"

#include "Scheduler.h"
#include "Process.h"
#include "IOHandler.h"
#include "Interrupt.h"
#include "Settings.h"

int OS(int memoryMin);
void ksyscall_handler(registers_t* reg);

#endif
