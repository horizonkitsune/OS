#define PAGE_SIZE 4096
#define NUM_PAGES 256
#define RAM_SIZE 64

#include "pagination.h"

// Table of pages
typedef struct
{
    int frame;   // Index du cadre en RAM
    int present; // 1 si la page est en RAM, 0 si elle est sur disque
    int dirty;   // 1 si page modifiée
} page_t;

char ram[RAM_SIZE][PAGE_SIZE];
char disk[NUM_PAGES][PAGE_SIZE];
page_t page_table[NUM_PAGES];
int current_frame = 0;

// Fonction de copie mémoire simple
void *memcpy(void *dest, const void *src, unsigned int n)
{
    char *d = (char *)dest;
    const char *s = (const char *)src;
    for (unsigned int i = 0; i < n; i++)
    {
        d[i] = s[i];
    }
    return dest;
}

// Charger une page en RAM
void load_page(int page)
{
    int frame = current_frame % RAM_SIZE;

    // Éjecter la page actuelle si nécessaire
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

// Obtenir l'adresse physique à partir de virtuelle
int get_physical_address(int virtual_address)
{
    int page = virtual_address / PAGE_SIZE;
    int offset = virtual_address % PAGE_SIZE;

    if (page < 0 || page >= NUM_PAGES)
        return -1;

    if (!page_table[page].present)
        load_page(page);

    int frame = page_table[page].frame;
    return frame * PAGE_SIZE + offset;
}

// Fonction de lecture dans la mémoire virtuelle
char read_memory(int virtual_address)
{
    int phys = get_physical_address(virtual_address);
    int frame = phys / PAGE_SIZE;
    int offset = phys % PAGE_SIZE;
    return ram[frame][offset];
}

// Fonction d'écriture dans la mémoire virtuelle
void write_memory(int virtual_address, char value)
{
    int phys = get_physical_address(virtual_address);
    int frame = phys / PAGE_SIZE;
    int offset = phys % PAGE_SIZE;
    ram[frame][offset] = value;
    int page = virtual_address / PAGE_SIZE;
    page_table[page].dirty = 1;
}

// Initialisation de la pagination
void init_pagination()
{
    for (int i = 0; i < NUM_PAGES; i++)
    {
        page_table[i].present = 0;
        page_table[i].frame = -1;
        page_table[i].dirty = 0;
    }
}
