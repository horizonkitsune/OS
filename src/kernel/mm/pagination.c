#include "pagination.h"

typedef struct
{
    int frame;   // Index du cadre en RAM
    int present; // 1 si la page est en RAM, 0 si sur disque
    int dirty;   // 1 si la page a été modifiée
} page_t;

static char    ram[RAM_SIZE][PAGE_SIZE];
static char    disk[NUM_PAGES][PAGE_SIZE];
static page_t  page_table[NUM_PAGES];
static int     current_frame = 0;

static void *memcpy(void *dest, const void *src, unsigned int n)
{
    char *d = (char *)dest;
    const char *s = (const char *)src;
    for (unsigned int i = 0; i < n; i++)
        d[i] = s[i];
    return dest;
}

void load_page(int page)
{
    int frame = current_frame % RAM_SIZE;

    // Éjecter la page occupant ce frame si nécessaire
    for (int i = 0; i < NUM_PAGES; i++)
    {
        if (page_table[i].present && page_table[i].frame == frame)
        {
            if (page_table[i].dirty)
            {
                memcpy(disk[i], ram[frame], PAGE_SIZE);
                page_table[i].dirty = 0;
            }
            page_table[i].present = 0;
            page_table[i].frame = -1;
        }
    }

    memcpy(ram[frame], disk[page], PAGE_SIZE);
    page_table[page].frame = frame;
    page_table[page].present = 1;
    page_table[page].dirty = 0;
    current_frame++;
}

int get_physical_address(int virtual_address)
{
    int page   = virtual_address / PAGE_SIZE;
    int offset = virtual_address % PAGE_SIZE;

    if (page < 0 || page >= NUM_PAGES)
        return -1;

    if (!page_table[page].present)
        load_page(page);

    return page_table[page].frame * PAGE_SIZE + offset;
}

char read_memory(int virtual_address)
{
    int phys   = get_physical_address(virtual_address);
    int frame  = phys / PAGE_SIZE;
    int offset = phys % PAGE_SIZE;
    return ram[frame][offset];
}

void write_memory(int virtual_address, char value)
{
    int phys   = get_physical_address(virtual_address);
    int frame  = phys / PAGE_SIZE;
    int offset = phys % PAGE_SIZE;
    ram[frame][offset] = value;
    page_table[virtual_address / PAGE_SIZE].dirty = 1;
}

void init_pagination(void)
{
    for (int i = 0; i < NUM_PAGES; i++)
    {
        page_table[i].present = 0;
        page_table[i].frame   = -1;
        page_table[i].dirty   = 0;
    }
}
