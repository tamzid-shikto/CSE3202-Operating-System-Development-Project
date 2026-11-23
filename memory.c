#include "types.h"

#define MEM_END 0x04000000u
extern char __kernel_end;

static uint32_t heap_free_addr;
static uint32_t stack_free_addr;

void memory_init(void)
{
    heap_free_addr = ((uint32_t)&__kernel_end + 7) & ~7u;
    stack_free_addr = MEM_END;
}

void *new_heap(uint32_t size)
{
    void *p = (void *)heap_free_addr;
    heap_free_addr += size;
    return p;
}

void *new_stack(uint32_t size)
{
    stack_free_addr -= size;
    return (void *)(stack_free_addr + size - 4);
}