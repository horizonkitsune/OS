/*
Registre généraux poussé par pusha du isr.asm
eax = accumulateur
ebx :  base
ecx : compteur, boucles
edx : données
esi : index source, pointeur source
edi : index destination, pointeur destination
ebp : base pointer, base de la pile
esp : pointeur de pile, pointe le dernier element push

Regisdtre spéciaux poussé par le CPU
eip : pointeur d'instruction, add prochaine instruction
cs : segment du code actif
ss : segment de pile
useresp : esp utilisateur
eflags : flags cpu, resultat comparaison
*/
#include "../include/stdint.h"

typedef struct {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t num_int, code_err;
    uint32_t eip, cs, eflags, useresp, ss;
} register_t;


// Exceptions CPU (0-31)
extern void isr0();   // Division Error
extern void isr1();   // Debug
extern void isr2();   // Non-maskable Interrupt
extern void isr3();   // Breakpoint
extern void isr4();   // Overflow
extern void isr5();   // Bound Range Exceeded
extern void isr6();   // Invalid Opcode
extern void isr7();   // Device Not Available
extern void isr8();   // Double Fault
extern void isr9();   // Coprocessor (obsolète)
extern void isr10();  // Invalid TSS
extern void isr11();  // Segment Not Present
extern void isr12();  // Stack Segment Fault
extern void isr13();  // General Protection Fault
extern void isr14();  // Page Fault
extern void isr15();  // Réservé
extern void isr16();  // x87 Floating Point Eception
extern void isr17();  // Alignment Check
extern void isr18();  // Machine Check
extern void isr19();  // SIMD Floating Point
extern void isr20();  // Virtualization
extern void isr21();  // Control Protection
// 22-27 réservés Intel
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28(); // Hypervisor Injection Eception
extern void isr29(); // VMM Communication excetion
extern void isr30(); // Security Excetion
extern void isr31(); // reserve

// IRQ matérielles (32-47)
extern void irq0();   // Timer
extern void irq1();   // Clavier
extern void irq2();   // Cascade PIC2
extern void irq3();   // COM2
extern void irq4();   // COM1
extern void irq5();   // LPT2
extern void irq6();   // Floppy (plus utilisé)
extern void irq7();   // LPT1
extern void irq8();   // RTC (real time clock)
extern void irq9();   // ACPI
extern void irq10();  // Libre
extern void irq11();  // Libre
extern void irq12();  // Souris PS/2
extern void irq13();  // FPU
extern void irq14();  // ATA primaire
extern void irq15();  // ATA secondaire

void isr_handler(register_t* regs);
void irq_handler(register_t* regs);