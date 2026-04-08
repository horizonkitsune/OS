#ifndef PAGINATION_H
#define PAGINATION_H

#include "memory_common.h"

typedef struct
{
    int valid;
    int frame;
    int present;
} PageEntry;

void show_pagination_example(void);

#endif // PAGINATION_H
