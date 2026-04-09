#ifndef ALLOCATION_H
#define ALLOCATION_H

typedef unsigned int size_t;
#define NULL ((void *)0)

void init_heap(void);
void *malloc(size_t size);
void free(void *ptr);

#endif // ALLOCATION_H
