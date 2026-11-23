#include "types.h"
#include "string.h"
#include "memory.h"
#include "process.h"
#include "scheduler.h"
#include "serial.h"

int ready_queue[NPROC];
int head_ptr;
int tail_ptr;

struct process_entry process_table[NPROC];
int current_pid;
int process_count;

struct queue_entry ready_queue_head;
struct queue_entry ready_queue_tail;
struct queue_entry suspend_queue_head;
struct queue_entry suspend_queue_tail;

void queue_init(void)
{
    head_ptr = 0;
    tail_ptr = 0;
}

void enqueue_ready(int pid){
    ready_queue[tail_ptr] = pid;
    tail_ptr = (tail_ptr + 1) % NPROC;               // UPDATED: wrap around
}
int dequeue_ready(void){
    int pid = ready_queue[head_ptr];
    head_ptr = (head_ptr + 1) % NPROC;               // UPDATED: wrap around
    return pid;
}

int cycle_next_process(){
    int pid = dequeue_ready();
    if (process_table[pid].state != PR_STATE_FREE)   // UPDATED: a finished process is dropped, not re-queued
        enqueue_ready(pid);
    return ready_queue[head_ptr];
}

void test1(){
    // int *arr[10] = (int)(*memory_alloc(sizeof(int) * 10));
    // memory_free(arr, sizeof(int) * 10);
}

void dummy1(){                                       // UPDATED: new process, prints 'a'
    while(1){
        serial_putc('a');
        for (volatile int j = 0; j < 100000; j++);
    }
}

void dummy2(){                                       // UPDATED: new process, prints 'b'
    while(1){
        serial_putc('b');
        for (volatile int j = 0; j < 100000; j++);
    }
}

void process_init(void)
{
    queue_init();
    process_count = 1;                               // UPDATED: pid 0 is the null process (kmain)
    current_pid = 0;                                 // UPDATED
    process_table[0].pid = 0;                        // UPDATED
    process_table[0].state = PR_STATE_CURRENT;       // UPDATED
    strcpy(process_table[0].name, "null");           // UPDATED
    enqueue_ready(0);                                      // UPDATED: the running process is always the queue front

    //create_process(&test1, 1024*16, 1024*16, "test1");
    create_process(&dummy1, 1024*16, 1024*16, "dummy1");   // UPDATED
    create_process(&dummy2, 1024*16, 1024*16, "dummy2");   // UPDATED
}

void userret(void)
{
    irq_save();                                      // UPDATED: stay off until the next process runs
    process_table[current_pid].state = PR_STATE_FREE;
    resched();
    for (;;);
}

int create_process(void *func, uint32_t stack_size, uint32_t heap_size, char *name)
{
    uint32_t im = irq_save();                        // UPDATED: timer must not interrupt creation
    int pid = process_count++;
    uint32_t *sp = new_stack(stack_size);
    int i;

    process_table[pid].pid = pid;
    strcpy(process_table[pid].name, name);
    process_table[pid].stkbase = (char *)sp;
    process_table[pid].heap = new_heap(heap_size);

    *--sp = (uint32_t)userret;
    *--sp = (uint32_t)func;
    *--sp = 0;
    *--sp = 0x202;
    for (i = 0; i < 8; i++)
        *--sp = 0;

    process_table[pid].stkptr = (char *)sp;
    process_table[pid].state = PR_STATE_READY;
    enqueue_ready(pid);

    irq_restore(im);                                 // UPDATED
    return pid;
}