#ifndef PROCESS_H
#define PROCESS_H
#include "types.h"

#define NPROC 8

#define PR_STATE_FREE    0
#define PR_STATE_CURRENT 1
#define PR_STATE_READY   2

struct process_entry {
    int pid;
    int state;
    int priority;

    char name[32];
    char *stkptr;
    char *stkbase;
    void *heap;
    int heap_size;
    int stack_size;
};

struct queue_entry{
    int pid;
    struct queue_entry *next;
    struct queue_entry *prev;
};

extern struct queue_entry ready_queue_head;
extern struct queue_entry ready_queue_tail;

extern struct queue_entry suspend_queue_head;
extern struct queue_entry suspend_queue_tail;

void enqueue(struct queue_entry *head, struct queue_entry *entry);
void dequeue(struct queue_entry *head, struct queue_entry *entry);

extern struct process_entry process_table[NPROC];
extern int process_count;

extern int current_pid;

void process_init(void);
int  create_process(void *func, uint32_t stack_size, uint32_t heap_size, char *name);
void kill(int pid);
void ready(int pid);
void suspend(int pid);
void userret(void);

#endif