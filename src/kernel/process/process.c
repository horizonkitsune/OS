#include "process.h"

// Table des processus et état global du scheduler /
static process_t process_table[MAX_PROCESSES];
static int next_pid = 0;
static int current_pid = -1;

void init_process_manager(void)
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        process_table[i].pid = -1;
        process_table[i].state = PROCESS_ZOMBIE;
        process_table[i].stack = NULL;
    }
}

// Création d'un processus à partir d'une fonction d'entrée
int process_create(void (*entry)(void))
{
    if (next_pid >= MAX_PROCESSES)
        return -1;

    process_t *proc = &process_table[next_pid];

    proc->stack = (char *)malloc(STACK_SIZE);
    if (!proc->stack)
        return -1;

    proc->pid = next_pid;
    proc->state = PROCESS_READY;

    // Contexte initial : eip pointe sur entry, esp en haut du stack
    proc->context.eip = (unsigned int)entry;
    proc->context.esp = (unsigned int)(proc->stack + STACK_SIZE);
    proc->context.ebp = proc->context.esp;
    proc->context.eflags = 0x202; // Interrupts enabled
    proc->context.eax = 0;
    proc->context.ebx = 0;
    proc->context.ecx = 0;
    proc->context.edx = 0;
    proc->context.esi = 0;
    proc->context.edi = 0;

    return next_pid++;
}

// Termine un processus et libère ses ressources
void process_exit(int pid)
{
    if (pid < 0 || pid >= MAX_PROCESSES)
        return;

    process_t *proc = &process_table[pid];
    if (proc->pid == -1)
        return;

    proc->state = PROCESS_ZOMBIE;
    free(proc->stack);
    proc->stack = NULL;
    proc->pid = -1;
}

// Scheduler simple round-robin
process_t *schedule(void)
{
    int start = (current_pid < 0) ? 0 : (current_pid + 1) % MAX_PROCESSES;
    int i = start;

    do
    {
        if (process_table[i].pid != -1 &&
            process_table[i].state == PROCESS_READY)
        {
            // Mettre à jour l'état du processus courant avant de switcher
            if (current_pid >= 0 &&
                process_table[current_pid].state == PROCESS_RUNNING)
                process_table[current_pid].state = PROCESS_READY;

            process_table[i].state = PROCESS_RUNNING;
            current_pid = i;
            return &process_table[i];
        }
        i = (i + 1) % MAX_PROCESSES;
    } while (i != start);

    return NULL; // Aucun processus prêt trouvé
}
