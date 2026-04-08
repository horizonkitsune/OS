#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 100
#define PAGE_SIZE 1024
#define FRAME_COUNT 8
#define MAX_PAGES 16

typedef struct
{
    int start;
    int size;
    int pid;
    int used;
} Hole;

typedef struct
{
    int valid;
    int frame;
    int present;
} PageEntry;

void print_divider()
{
    printf("------------------------------------------------------------\n");
}

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
    }
    for (int i = 0; i < holeCount; i++)
    {
        if (holes[i].used)
        {
            int requested = holes[i].size - 5; // simulation d'un dépassement de taille
            internal += holes[i].size - requested;
        }
    }
    printf("External fragmentation total: %d bytes in %d holes\n", external, freeCount);
    printf("Internal fragmentation example: %d bytes (simulated)\n", internal);
    print_divider();
}

int allocate_first_fit(Hole holes[], int holeCount, int pid, int size)
{
    for (int i = 0; i < holeCount; i++)
    {
        if (!holes[i].used && holes[i].size >= size)
        {
            int leftover = holes[i].size - size;
            holes[i].used = 1;
            holes[i].pid = pid;
            holes[i].size = size;
            if (leftover > 0)
            {
                for (int j = holeCount - 1; j > i; j--)
                {
                    holes[j] = holes[j - 1];
                }
                holes[i + 1].start = holes[i].start + size;
                holes[i + 1].size = leftover;
                holes[i + 1].pid = 0;
                holes[i + 1].used = 0;
                holeCount++;
            }
            return 1;
        }
    }
    return 0;
}

int allocate_best_fit(Hole holes[], int holeCount, int pid, int size)
{
    int best = -1;
    for (int i = 0; i < holeCount; i++)
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
        for (int j = holeCount - 1; j > best; j--)
        {
            holes[j] = holes[j - 1];
        }
        holes[best + 1].start = holes[best].start + size;
        holes[best + 1].size = leftover;
        holes[best + 1].pid = 0;
        holes[best + 1].used = 0;
        holeCount++;
    }
    return 1;
}

int allocate_worst_fit(Hole holes[], int holeCount, int pid, int size)
{
    int worst = -1;
    for (int i = 0; i < holeCount; i++)
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
        for (int j = holeCount - 1; j > worst; j--)
        {
            holes[j] = holes[j - 1];
        }
        holes[worst + 1].start = holes[worst].start + size;
        holes[worst + 1].size = leftover;
        holes[worst + 1].pid = 0;
        holes[worst + 1].used = 0;
        holeCount++;
    }
    return 1;
}

void simulate_allocation()
{
    Hole initial[] = {{0, MEMORY_SIZE, 0, 0}};
    int holeCount = 1;
    Hole copy[20];
    memcpy(copy, initial, sizeof(initial));
    allocate_first_fit(copy, holeCount, 1, 20);
    allocate_first_fit(copy, holeCount, 2, 15);
    allocate_first_fit(copy, holeCount, 3, 30);
    print_allocation_result("First Fit", copy, 20);

    memcpy(copy, initial, sizeof(initial));
    allocate_best_fit(copy, holeCount, 1, 20);
    allocate_best_fit(copy, holeCount, 2, 15);
    allocate_best_fit(copy, holeCount, 3, 30);
    print_allocation_result("Best Fit", copy, 20);

    memcpy(copy, initial, sizeof(initial));
    allocate_worst_fit(copy, holeCount, 1, 20);
    allocate_worst_fit(copy, holeCount, 2, 15);
    allocate_worst_fit(copy, holeCount, 3, 30);
    print_allocation_result("Worst Fit", copy, 20);

    printf("Explication:\n");
    printf("- First Fit alloue à la première zone libre assez grande.\n");
    printf("- Best Fit choisit la zone la plus petite possible qui convient, pour réduire l’espace libre restant.\n");
    printf("- Worst Fit choisit la plus grande zone libre, en tentant d’éviter de créer des petits trous.\n");
    printf("- Fragmentation externe : mémoire libre dispersée en plusieurs petits trous.\n");
    printf("- Fragmentation interne : mémoire réservée dans une zone allouée mais pas utilisée par le processus.\n");
    print_divider();
}

void show_pagination_example()
{
    PageEntry pageTable[MAX_PAGES];
    for (int i = 0; i < MAX_PAGES; i++)
    {
        pageTable[i].valid = 0;
        pageTable[i].present = 0;
        pageTable[i].frame = -1;
    }
    int frames[FRAME_COUNT] = {0};
    pageTable[0].valid = pageTable[1].valid = pageTable[2].valid = 1;
    pageTable[0].present = 1;
    pageTable[0].frame = 2;
    pageTable[1].present = 1;
    pageTable[1].frame = 5;
    pageTable[2].present = 0;
    pageTable[2].frame = -1;
    pageTable[3].valid = 1;
    pageTable[3].present = 0;

    printf("Pagination: exemple de table des pages\n");
    printf("Page | Present | Cadre\n");
    for (int i = 0; i < 4; i++)
    {
        printf(" %3d |   %3d   | %4d\n", i, pageTable[i].present, pageTable[i].frame);
    }
    printf("\nPhysique: cadres libres = 0,1,3,4,6,7 ; occupés = 2,5\n");
    printf("Vous pouvez allouer un cadre libre à la page 3. Entrez un numéro de cadre libre: ");

    int frameChoice = -1;
    scanf("%d", &frameChoice);
    if (frameChoice >= 0 && frameChoice < FRAME_COUNT && frames[frameChoice] == 0)
    {
        pageTable[3].present = 1;
        pageTable[3].frame = frameChoice;
        frames[frameChoice] = 1;
        printf("Page 3 allouée au cadre %d.\n", frameChoice);
    }
    else
    {
        printf("Choix invalide. Aucun changement effectué.\n");
    }

    print_divider();
    printf("Table de pages mise à jour:\n");
    printf("Page | Present | Cadre\n");
    for (int i = 0; i < 4; i++)
    {
        printf(" %3d |   %3d   | %4d\n", i, pageTable[i].present, pageTable[i].frame);
    }

    printf("\nExemple de traduction d'adresse:\n");
    int logicalAddress = 2500;
    int pageNumber = logicalAddress / PAGE_SIZE;
    int offset = logicalAddress % PAGE_SIZE;
    if (pageTable[pageNumber].present)
    {
        int physicalAddress = pageTable[pageNumber].frame * PAGE_SIZE + offset;
        printf("Adresse logique %d -> page %d, offset %d\n", logicalAddress, pageNumber, offset);
        printf("Adresse physique = cadre %d * %d + %d = %d\n", pageTable[pageNumber].frame, PAGE_SIZE, offset, physicalAddress);
    }
    else
    {
        printf("La page %d est absente. Il faut déclencher un page fault pour charger la page en mémoire physique.\n", pageNumber);
    }
    print_divider();
}

void print_page_table_header()
{
    printf("Ref | Etat memoire | Fault | Remplacement\n");
    print_divider();
}

void run_replacement_algorithm(int refs[], int refCount, int frameCount, const char *name)
{
    int frames[FRAME_COUNT];
    int lastUsed[FRAME_COUNT];
    int order[FRAME_COUNT];
    int faults = 0;
    for (int i = 0; i < frameCount; i++)
        frames[i] = -1;
    for (int i = 0; i < frameCount; i++)
        lastUsed[i] = -1, order[i] = i;

    printf("Algorithm: %s\n", name);
    for (int i = 0; i < refCount; i++)
    {
        int page = refs[i];
        int hit = 0;
        for (int f = 0; f < frameCount; f++)
        {
            if (frames[f] == page)
            {
                hit = 1;
                lastUsed[f] = i;
                break;
            }
        }
        if (!hit)
        {
            faults++;
            int replace = -1;
            for (int f = 0; f < frameCount; f++)
            {
                if (frames[f] == -1)
                {
                    replace = f;
                    break;
                }
            }
            if (replace == -1)
            {
                if (strcmp(name, "FIFO") == 0)
                {
                    static int fifoIndex = 0;
                    replace = fifoIndex % frameCount;
                    fifoIndex++;
                }
                else if (strcmp(name, "LRU") == 0)
                {
                    int min = 1e9;
                    for (int f = 0; f < frameCount; f++)
                    {
                        if (lastUsed[f] < min)
                        {
                            min = lastUsed[f];
                            replace = f;
                        }
                    }
                }
                else if (strcmp(name, "Horloge") == 0)
                {
                    static int clockHand = 0;
                    while (1)
                    {
                        if (order[clockHand] == -1)
                            order[clockHand] = 1;
                        if (order[clockHand] == 0)
                        {
                            replace = clockHand;
                            break;
                        }
                        order[clockHand] = 0;
                        clockHand = (clockHand + 1) % frameCount;
                    }
                }
                else if (strcmp(name, "OPT") == 0)
                {
                    int farthest = -1;
                    int farIdx = -1;
                    for (int f = 0; f < frameCount; f++)
                    {
                        int nextUse = 1e9;
                        for (int k = i + 1; k < refCount; k++)
                        {
                            if (refs[k] == frames[f])
                            {
                                nextUse = k;
                                break;
                            }
                        }
                        if (nextUse > farthest)
                        {
                            farthest = nextUse;
                            farIdx = f;
                        }
                    }
                    replace = farIdx;
                }
            }
            frames[replace] = page;
            lastUsed[replace] = i;
            if (strcmp(name, "Horloge") == 0)
                order[replace] = 1;
            printf("%2d: page %d -> fault, remplace cadre %d | ", i, page, replace);
        }
        else
        {
            printf("%2d: page %d -> hit | ", i, page);
        }
        for (int f = 0; f < frameCount; f++)
        {
            if (frames[f] == -1)
                printf("[ ] ");
            else
                printf("[%d] ", frames[f]);
        }
        printf("\n");
    }
    printf("Faults: %d/%d\n", faults, refCount);
    print_divider();
}

void simulate_virtual_memory()
{
    int refs[] = {1, 2, 3, 2, 4, 1, 5, 2, 1, 3, 4, 5};
    int refCount = sizeof(refs) / sizeof(refs[0]);
    int frameCount = 3;
    printf("Simulation de mémoire virtuelle avec références pages:\n");
    for (int i = 0; i < refCount; i++)
        printf("%d ", refs[i]);
    printf("\n\n");

    run_replacement_algorithm(refs, refCount, frameCount, "FIFO");
    run_replacement_algorithm(refs, refCount, frameCount, "LRU");
    run_replacement_algorithm(refs, refCount, frameCount, "OPT");
    run_replacement_algorithm(refs, refCount, frameCount, "Horloge");

    printf("Comparaison des algorithmes de remplacement:\n");
    printf("- OPT: optimal si on connaît l’avenir, remplace la page qui ne sera pas utilisée le plus longtemps.\n");
    printf("- FIFO: remplace la page la plus ancienne en mémoire, simple mais peut provoquer le belady anomaly.\n");
    printf("- LRU: remplace la page la moins récemment utilisée, bonne approximation du comportement réel.\n");
    printf("- Horloge: implémente LRU approximatif avec un bit de seconde chance.\n");
    printf("\nThrashing:\n");
    printf("- Se produit quand le système passe beaucoup de temps à charger/décharger des pages plutôt qu’à exécuter le programme.\n");
    printf("- Si le nombre de cadres alloués est trop faible pour la charge de travail, le taux de fautes de page monte, et l’exécution ralentit fortement.\n");
    printf("- Le système peut réagir en augmentant le nombre de cadres ou en réduisant la taille active du working set.\n");
    print_divider();
}

int main()
{
    int choix = 0;
    while (1)
    {
        printf("\n=== Simulateur memoire C ===\n");
        printf("1. Allocation dynamique (First Fit, Best Fit, Worst Fit)\n");
        printf("2. Pagination et traduction d'adresse\n");
        printf("3. Memoire virtuelle et remplacement de pages\n");
        printf("4. Quitter\n");
        printf("Votre choix: ");
        if (scanf("%d", &choix) != 1)
            break;
        print_divider();
        if (choix == 1)
            simulate_allocation();
        else if (choix == 2)
            show_pagination_example();
        else if (choix == 3)
            simulate_virtual_memory();
        else
            break;
    }
    printf("Fin du simulateur.\n");
    return 0;
}
