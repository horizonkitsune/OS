#pragma once
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
#include "stdint.h"

typedef struct {
    uint32_t ds;
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
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

// IRQ_ matérielles (32-47)
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
extern void (*irq_routines[16])(register_t*);

/*------------------ISR------------------*/
#define ISR_DIVIDE_ERROR                0x00
#define ISR_DEBUG                       0x01
#define ISR_NON_MASKABLE_INTERRUPT      0x02
#define ISR_BREAKPOINT                  0x03
#define ISR_OVERFLOW                    0x04
#define ISR_BOUND_RANGE_EXCEEDED        0x05
#define ISR_INVALID_OPCODE              0x06
#define ISR_DEVICE_NOT_AVAILABLE        0x07
#define ISR_DOUBLE_FAULT                0x08
#define ISR_COPROCESSOR_SEG_OVERRUN     0x09
#define ISR_INVALID_TSS                 0x0A
#define ISR_SEGMENT_NOT_PRESENT         0x0B
#define ISR_STACK_SEGMENT_FAULT         0x0C
#define ISR_GENERAL_PROTECTION_FAULT    0x0D
#define ISR_PAGE_FAULT                  0x0E
#define ISR_RESERVED_0F                 0x0F
#define ISR_X87_FPU_ERROR               0x10
#define ISR_ALIGNMENT_CHECK             0x11
#define ISR_MACHINE_CHECK               0x12
#define ISR_SIMD_FLOATING_POINT         0x13
#define ISR_VIRTUALIZATION              0x14
#define ISR_CONTROL_PROTECTION          0x15
#define ISR_RESERVED_16                 0x16
#define ISR_RESERVED_17                 0x17
#define ISR_RESERVED_18                 0x18
#define ISR_RESERVED_19                 0x19
#define ISR_RESERVED_1A                 0x1A
#define ISR_RESERVED_1B                 0x1B
#define ISR_HYPERVISOR_INJECTION        0x1C
#define ISR_VMM_COMMUNICATION           0x1D
#define ISR_SECURITY_EXCEPTION          0x1E
#define ISR_RESERVED_1F                 0x1F

/*------------------IRQ-----------------*/
#define IRQ_TIMER                       0x20
#define IRQ_KEYBOARD                    0x21
#define IRQ_CASCADE                     0x22
#define IRQ_COM2                        0x23
#define IRQ_COM1                        0x24
#define IRQ_LPT2                        0x25
#define IRQ_FLOPPY                      0x26
#define IRQ_LPT1                        0x27
#define IRQ_RTC                         0x28
#define IRQ_FREE1                       0x29
#define IRQ_FREE2                       0x2A
#define IRQ_FREE3                       0x2B
#define IRQ_MOUSE                       0x2C
#define IRQ_FPU                         0x2D
#define IRQ_PRI_AT                      0x2E
#define IRQ_SEC_ATA                     0x2F