#include <string.h>
#include "virtual_memory.h"

static void run_replacement_algorithm(int refs[], int refCount, int frameCount, const char *name)
{
    int frames[FRAME_COUNT];
    int lastUsed[FRAME_COUNT];
    int useBit[FRAME_COUNT];
    int faults = 0;
    int fifoIndex = 0;
    int clockHand = 0;

    for (int i = 0; i < frameCount; i++)
    {
        frames[i] = -1;
        lastUsed[i] = -1;
        useBit[i] = 0;
    }

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
                useBit[f] = 1;
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
                    while (1)
                    {
                        if (useBit[clockHand] == 0)
                        {
                            replace = clockHand;
                            clockHand = (clockHand + 1) % frameCount;
                            break;
                        }
                        useBit[clockHand] = 0;
                        clockHand = (clockHand + 1) % frameCount;
                    }
                }
                else if (strcmp(name, "OPT") == 0)
                {
                    int farthest = -1;
                    int farIdx = 0;
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
            useBit[replace] = 1;
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

void simulate_virtual_memory(void)
{
    int refs[] = {1, 2, 3, 2, 4, 1, 5, 2, 1, 3, 4, 5};
    int refCount = sizeof(refs) / sizeof(refs[0]);
    int frameCount = 3;

    printf("Simulation de memoire virtuelle avec references pages :\n");
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
    printf("- Si le nombre de cadres alloués est trop faible, le taux de fautes de page monte et l’exécution ralentit fortement.\n");
    printf("- Le système peut réagir en augmentant le nombre de cadres ou en réduisant le working set actif.\n");
    print_divider();
}
