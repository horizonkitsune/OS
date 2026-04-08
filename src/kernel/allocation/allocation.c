#include <string.h>
#include "allocation.h"

void print_memory_map(Hole holes[], int holeCount)
{
    printf("Memory map:\n");
    for (int i = 0; i < holeCount; i++)
    {
        printf("[%2d..%2d] %4d bytes - ", holes[i].start, holes[i].start + holes[i].size - 1, holes[i].size);
        if (holes[i].used)
            printf("PID=%d\n", holes[i].pid);
        else
            printf("free\n");
    }
}

void print_allocation_result(const char *title, Hole holes[], int holeCount)
{
    printf("%s\n", title);
    print_memory_map(holes, holeCount);
    int internal = 0;
    int external = 0;
    int freeCount = 0;
    for (int i = 0; i < holeCount; i++)
    {
        if (!holes[i].used)
        {
            external += holes[i].size;
            freeCount++;
        }
        else
        {
            int requested = holes[i].size - 5;
            internal += holes[i].size - requested;
        }
    }
    printf("External fragmentation total: %d bytes in %d holes\n", external, freeCount);
    printf("Internal fragmentation example: %d bytes (simulated)\n", internal);
    print_divider();
}

static void insert_free_hole(Hole holes[], int *holeCount, int index, int start, int size)
{
    for (int j = *holeCount; j > index; j--)
    {
        holes[j] = holes[j - 1];
    }
    holes[index].start = start;
    holes[index].size = size;
    holes[index].pid = 0;
    holes[index].used = 0;
    (*holeCount)++;
}

int allocate_first_fit(Hole holes[], int *holeCount, int pid, int size)
{
    for (int i = 0; i < *holeCount; i++)
    {
        if (!holes[i].used && holes[i].size >= size)
        {
            int leftover = holes[i].size - size;
            holes[i].used = 1;
            holes[i].pid = pid;
            holes[i].size = size;
            if (leftover > 0)
            {
                insert_free_hole(holes, holeCount, i + 1, holes[i].start + size, leftover);
            }
            return 1;
        }
    }
    return 0;
}

int allocate_best_fit(Hole holes[], int *holeCount, int pid, int size)
{
    int best = -1;
    for (int i = 0; i < *holeCount; i++)
    {
        if (!holes[i].used && holes[i].size >= size)
        {
            if (best < 0 || holes[i].size < holes[best].size)
                best = i;
        }
    }
    if (best == -1)
        return 0;
    int leftover = holes[best].size - size;
    holes[best].used = 1;
    holes[best].pid = pid;
    holes[best].size = size;
    if (leftover > 0)
    {
        insert_free_hole(holes, holeCount, best + 1, holes[best].start + size, leftover);
    }
    return 1;
}

int allocate_worst_fit(Hole holes[], int *holeCount, int pid, int size)
{
    int worst = -1;
    for (int i = 0; i < *holeCount; i++)
    {
        if (!holes[i].used && holes[i].size >= size)
        {
            if (worst < 0 || holes[i].size > holes[worst].size)
                worst = i;
        }
    }
    if (worst == -1)
        return 0;
    int leftover = holes[worst].size - size;
    holes[worst].used = 1;
    holes[worst].pid = pid;
    holes[worst].size = size;
    if (leftover > 0)
    {
        insert_free_hole(holes, holeCount, worst + 1, holes[worst].start + size, leftover);
    }
    return 1;
}

void simulate_allocation(void)
{
    Hole initial[20] = {{0, MEMORY_SIZE, 0, 0}};
    Hole copy[20];
    int holeCount;

    holeCount = 1;
    memcpy(copy, initial, sizeof(initial));
    allocate_first_fit(copy, &holeCount, 1, 20);
    allocate_first_fit(copy, &holeCount, 2, 15);
    allocate_first_fit(copy, &holeCount, 3, 30);
    print_allocation_result("First Fit", copy, holeCount);

    holeCount = 1;
    memcpy(copy, initial, sizeof(initial));
    allocate_best_fit(copy, &holeCount, 1, 20);
    allocate_best_fit(copy, &holeCount, 2, 15);
    allocate_best_fit(copy, &holeCount, 3, 30);
    print_allocation_result("Best Fit", copy, holeCount);

    holeCount = 1;
    memcpy(copy, initial, sizeof(initial));
    allocate_worst_fit(copy, &holeCount, 1, 20);
    allocate_worst_fit(copy, &holeCount, 2, 15);
    allocate_worst_fit(copy, &holeCount, 3, 30);
    print_allocation_result("Worst Fit", copy, holeCount);

    printf("Explication:\n");
    printf("- First Fit alloue à la première zone libre assez grande.\n");
    printf("- Best Fit choisit la zone la plus petite possible qui convient, réduisant l’espace libre restant.\n");
    printf("- Worst Fit choisit la plus grande zone libre, en tentant d’éviter la création de petits trous.\n");
    printf("- Fragmentation externe : mémoire libre dispersée en plusieurs petits trous.\n");
    printf("- Fragmentation interne : mémoire réservée dans une zone allouée mais pas utilisée par le processus.\n");
    print_divider();
}
