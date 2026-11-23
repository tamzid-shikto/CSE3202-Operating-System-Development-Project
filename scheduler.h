#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "types.h"                                   // UPDATED: needed for uint32_t

extern int quantum;

void scheduler_init(void);
void timer_init(void);
void timer_tick(void);
void resched(void);
void ctxsw(char **old_sp, char **new_sp);

static inline uint32_t irq_save(void)                // UPDATED: disable interrupts, return old flags
{
    uint32_t flags;
    __asm__ volatile ("pushfl; popl %0; cli" : "=r"(flags) : : "memory");
    return flags;
}

static inline void irq_restore(uint32_t flags)       // UPDATED: restore the saved flags
{
    __asm__ volatile ("pushl %0; popfl" : : "r"(flags) : "memory", "cc");
}

#endif