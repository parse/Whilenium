#ifndef __INTERRUPT__
#define __INTERRUPT__

#include "mips/types.h"
#include "mips/asm.h"
#include "mips/malta.h"
#include "mips/debug.h"

#include "_Boot.h"
#include "Structs.h"
#include "Process.h"
#include "IOHandler.h"
#include "Scheduler.h"

void enableInterrupt();
void kexception();

#endif
