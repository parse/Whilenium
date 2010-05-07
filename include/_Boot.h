#ifndef __ASM_SYSCALL__
#define __ASM_SYSCALL__

void ksyscall_handler(registers_t* reg);
void my_system_call(uint32_t v);

#endif

