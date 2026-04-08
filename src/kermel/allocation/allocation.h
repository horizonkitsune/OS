#ifndef ALLOCATION_H
#define ALLOCATION_H

typedef unsigned int size_t;

// Initialisation de la heap
void init_heap(void);

// Allocation de mémoire
void *malloc(size_t size);

// Libération de mémoire
void free(void *ptr);

#endif // ALLOCATION_H
