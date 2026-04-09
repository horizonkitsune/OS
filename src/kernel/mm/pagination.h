#ifndef PAGINATION_H
#define PAGINATION_H

#define PAGE_SIZE 4096
#define NUM_PAGES 256
#define RAM_SIZE   64

void init_pagination(void);
int  get_physical_address(int virtual_address);
char read_memory(int virtual_address);
void write_memory(int virtual_address, char value);
void load_page(int page);

#endif // PAGINATION_H
