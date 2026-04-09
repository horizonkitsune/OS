#include "allocation.h"
#include "pagination.h"

#define HEAP_SIZE (1024 * 1024)
#define ALIGNMENT 4
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

static char heap[HEAP_SIZE];

typedef struct block
{
    size_t size;
    struct block *next;
} block_t;

static block_t *free_list = (block_t *)heap;

void init_heap(void)
{
    free_list->size = HEAP_SIZE - sizeof(block_t);
    free_list->next = NULL;
}

void *malloc(size_t size)
{
    if (size == 0)
        return NULL;

    size = ALIGN(size);

    block_t *current = free_list;
    block_t *prev = NULL;

    while (current)
    {
        if (current->size >= size + sizeof(block_t))
        {
            block_t *new_block = (block_t *)((char *)current + sizeof(block_t) + size);
            new_block->size = current->size - size - sizeof(block_t);
            new_block->next = current->next;

            current->size = size;
            current->next = NULL;

            if (prev)
                prev->next = new_block;
            else
                free_list = new_block;

            return (void *)((char *)current + sizeof(block_t));
        }
        prev = current;
        current = current->next;
    }
    return NULL;
}

void free(void *ptr)
{
    if (!ptr)
        return;

    block_t *block = (block_t *)((char *)ptr - sizeof(block_t));

    // Insérer dans la free_list dans l'ordre des adresses (pour la coalescence)
    block_t *current = free_list;
    block_t *prev = NULL;

    while (current && current < block)
    {
        prev = current;
        current = current->next;
    }

    block->next = current;
    if (prev)
        prev->next = block;
    else
        free_list = block;

    // Fusionner avec le bloc suivant si adjacent
    if (current != NULL &&
        (char *)block + sizeof(block_t) + block->size == (char *)current)
    {
        block->size += sizeof(block_t) + current->size;
        block->next = current->next;
    }

    // Fusionner avec le bloc précédent si adjacent
    if (prev != NULL &&
        (char *)prev + sizeof(block_t) + prev->size == (char *)block)
    {
        prev->size += sizeof(block_t) + block->size;
        prev->next = block->next;
    }
}
