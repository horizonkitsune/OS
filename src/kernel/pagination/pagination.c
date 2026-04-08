#include <stdio.h>
#include "pagination.h"

void show_pagination_example(void)
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
