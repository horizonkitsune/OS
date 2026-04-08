#ifndef PAGINATION_H
#define PAGINATION_H

#define PAGE_SIZE 4096
#define NUM_PAGES 256
#define RAM_SIZE 64

// Initialisation de la pagination
void init_pagination(void);

// Obtenir l'adresse physique à partir de virtuelle
int get_physical_address(int virtual_address);

// Lire un octet de la mémoire virtuelle
char read_memory(int virtual_address);

// Écrire un octet dans la mémoire virtuelle
void write_memory(int virtual_address, char value);

// Charger une page en RAM
void load_page(int page);

#endif // PAGINATION_H
