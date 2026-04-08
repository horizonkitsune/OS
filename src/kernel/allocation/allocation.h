#ifndef ALLOCATION_H
#define ALLOCATION_H

#include "memory_common.h"

typedef struct
{
    int start;
    int size;
    int pid;
    int used;
} Hole;

void print_memory_map(Hole holes[], int holeCount);
void print_allocation_result(const char *title, Hole holes[], int holeCount);
int allocate_first_fit(Hole holes[], int *holeCount, int pid, int size);
int allocate_best_fit(Hole holes[], int *holeCount, int pid, int size);
int allocate_worst_fit(Hole holes[], int *holeCount, int pid, int size);
void simulate_allocation(void);

#endif // ALLOCATION_H
