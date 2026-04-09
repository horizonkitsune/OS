#ifndef PROCESS_H
#define PROCESS_H

#include "../mm/allocation.h"

#define MAX_PROCESSES  16
#define STACK_SIZE     4096

/* -------------------------------------------------------------------------- */
/*  États d'un processus                                                       */
/* -------------------------------------------------------------------------- */
typedef enum {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_ZOMBIE,
} process_state_t;

/* -------------------------------------------------------------------------- */
/*  Contexte CPU (registres sauvegardés lors d'un changement de contexte)     */
/* -------------------------------------------------------------------------- */
typedef struct {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int esi;
    unsigned int edi;
    unsigned int esp;
    unsigned int ebp;
    unsigned int eip;
    unsigned int eflags;
} cpu_context_t;

/* -------------------------------------------------------------------------- */
/*  Bloc de contrôle de processus (PCB)                                       */
/* -------------------------------------------------------------------------- */
typedef struct {
    int              pid;
    process_state_t  state;
    cpu_context_t    context;
    char            *stack;
} process_t;

/* -------------------------------------------------------------------------- */
/*  Interface publique                                                         */
/* -------------------------------------------------------------------------- */
void       init_process_manager(void);
int        process_create(void (*entry)(void));
void       process_exit(int pid);
process_t *schedule(void);

#endif /* PROCESS_H */
