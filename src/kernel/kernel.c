#include "mm/allocation.h"
#include "mm/pagination.h"
#include "process/process.h"

void kernel_main(void)
{
    init_heap();
    init_pagination();
    init_process_manager();

    while (1)
    {
    }
}
