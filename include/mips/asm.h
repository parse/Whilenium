
#ifndef ASM_H
#define ASM_H

#include "types.h"
#include "mips/registers.h"

/* Timer 67MHz (?) */
static const int timer_usec = 67;
static const int timer_msec = 67000;

/* Functions provided by 'asm.S' */
uint32_t kset_sr(uint32_t and_mask, uint32_t or_mask);
uint32_t kget_sr();
uint32_t kset_cause(uint32_t and_mask, uint32_t or_mask);
uint32_t kget_cause();
void kload_timer(uint32_t timer_value);
void kset_registers(registers_t* regs);
registers_t* kget_registers();
void kdebug_magic_break();

/*
 * kinit is called at startup, contains application-specific
 * system initialisation.
 * Applications should make sure that 'kset_registers' is
 * called, to that the exception handler can save registers.
 */
void kinit();

/*
 * kexception is called when an exception occurs, after registers
 * have been saved. Use 'kget_registers' to access the saved
 * registers, and 'kset_registers' to update the entire register
 * set when 'kexception' returns (useful for task switching).
 */
void kexception();

#endif
