#include "serial.h"
#include "process.h"
#include "scheduler.h"

int cycle_next_process();                            // UPDATED: prototype, the function lives in process.c

void scheduler_init(void){
    timer_init();
}

static volatile int tick_counter;
int quantum = 10;

void timer_tick(void)
{
    if (++tick_counter >= quantum) {
        tick_counter = 0;
        resched();
    }
}

void resched(void){
    if(process_count <= 0) return;
    /* serial_puts(" RESCHED \n"); */                // UPDATED: removed, too slow inside an interrupt
    uint32_t im = irq_save();                        // UPDATED: no timer tick in the middle of a switch

    int old = current_pid;
    tick_counter = 0;                                // UPDATED: new process gets a full time slice
    int next = cycle_next_process();

    if (process_table[old].state == PR_STATE_CURRENT)   // UPDATED: a finished (FREE) process stays FREE
        process_table[old].state = PR_STATE_READY;
    process_table[next].state = PR_STATE_CURRENT;    // UPDATED
    current_pid = next;                              // UPDATED: was never updated

    if (next != old)                                 // UPDATED: nothing to switch if only one process
        ctxsw(&process_table[old].stkptr, &process_table[next].stkptr);
    irq_restore(im);                                 // UPDATED
}