#define PAGE_SIZE 4096
#define NUM_PAGES 256
#define RAM_SIZE 64

#include "virtual_memory.h"

// Table of Pages Structure
typedef struct
{
    int frame;   // Index of the frame in RAM
    int present; // 1 if the page is in RAM, 0 if it's on disk
} page_t;

// Simulated RAM and Disk
char ram[RAM_SIZE][PAGE_SIZE];
char disk[NUM_PAGES][PAGE_SIZE];

page_t page_table[NUM_PAGES];
int current_frame = 0;

void *memcpy(void *dest, const void *src, unsigned int n);

void load_page(int page)
{
    // Remplacer une page si RAM pleine (FIFO simple)
    int frame = current_frame % RAM_SIZE;

    // Trouver quelle page est dans cette frame
    for (int i = 0; i < NUM_PAGES; i++)
    {
        if (page_table[i].present && page_table[i].frame == frame)
        {
            page_table[i].present = 0;
            // sauvegarde sur disque (optionnel ici)
        }
    }

    // Charger depuis le disque
    memcpy(ram[frame], disk[page], PAGE_SIZE);

    page_table[page].frame = frame;
    page_table[page].present = 1;

    current_frame++;
}

int get_physical_address(int virtual_address)
{
    int page = virtual_address / PAGE_SIZE;
    int offset = virtual_address % PAGE_SIZE;

    if (!page_table[page].present)
    {
        load_page(page);
    }

    int frame = page_table[page].frame;
    return frame * PAGE_SIZE + offset;
}

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
}

// Initialisation de la mémoire virtuelle
void init_memory()
{
    for (int i = 0; i < NUM_PAGES; i++)
    {
        page_table[i].present = 0;
        page_table[i].frame = -1;
    }
}