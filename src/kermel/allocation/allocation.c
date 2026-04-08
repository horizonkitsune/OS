// Types de base pour bare-metal
typedef unsigned int size_t;
#define NULL ((void *)0)

// Définition de la heap : 1 Mo de mémoire pour l'exemple
#define HEAP_SIZE 1024 * 1024
static char heap[HEAP_SIZE];

// Structure pour les blocs libres
typedef struct block
{
    size_t size;
    struct block *next;
} block_t;

static block_t *free_list = (block_t *)heap;

// Initialisation de la heap
void init_heap()
{
    free_list->size = HEAP_SIZE - sizeof(block_t);
    free_list->next = NULL;
}

// Fonction d'allocation simple (first-fit)
void *malloc(size_t size)
{
    block_t *current = free_list;
    block_t *prev = NULL;

    while (current)
    {
        if (current->size >= size + sizeof(block_t))
        {
            // Diviser le bloc
            block_t *new_block = (block_t *)((char *)current + sizeof(block_t) + size);
            new_block->size = current->size - size - sizeof(block_t);
            new_block->next = current->next;

            current->size = size;
            current->next = NULL; // Le bloc alloué n'est pas dans la free_list

            if (prev)
            {
                prev->next = new_block;
            }
            else
            {
                free_list = new_block;
            }

            return (void *)((char *)current + sizeof(block_t));
        }
        prev = current;
        current = current->next;
    }
    return NULL; // Pas assez de mémoire
}

// Fonction de libération (simple, sans fusion pour cet exemple)
void free(void *ptr)
{
    if (!ptr)
        return;

    block_t *block = (block_t *)((char *)ptr - sizeof(block_t));
    block->next = free_list;
    free_list = block;
}

// Fonction principale du kernel
void kernel_main()
{
    init_heap();

    // Exemple d'utilisation
    char *str = (char *)malloc(100);
    if (str)
    {
        // Utiliser str
        free(str);
    }

    // Boucle infinie pour éviter que le kernel se termine
    while (1)
    {
    }
}