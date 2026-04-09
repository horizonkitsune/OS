#include "mm/allocation.h"
#include "mm/pagination.h"

void kernel_main(void)
{
    init_heap();
    init_pagination();

    while (1)
    {
    }
}
