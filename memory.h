#ifndef MEMORY_H
#define MEMORY_H
#include "types.h"

void  memory_init(void);
void *new_heap(uint32_t size);
void *new_stack(uint32_t size);

void free_heap(void *startAddress, void *endAddress);
void free_stack(void *startAddress, void *endAddress);

void *memory_alloc(int size);
void *memory_free(void *startAddress, int size);

#endif