#include "isr.h"
#include "../drivers/vga/vga.h"
#include "../include/io.h"

static const char* exception_messages[] = {
    "Division Error",           // 0
    "Debug",                    
    "Non-maskable Interrupt",   
    "Breakpoint",               
    "Overflow",                 
    "Bound Range Exceeded",     
    "Invalid Opcode",           
    "Device Not Available",    
    "Double Fault",           
    "Coprocessor",             
    "Invalid TSS",            
    "Segment Not Present",      
    "Stack Segment Fault",      
    "General Protection Fault", 
    "Page Fault",               
    "Reserved",                 
    "x87 Floating Point",      
    "Alignment Check",        
    "Machine Check",            
    "SIMD Floating Point",      
    "Virtualization",           
    "Control Protection",       
    "Reserved",                 
    "Reserved",                 
    "Reserved",               
    "Reserved",                 
    "Reserved",                 
    "Reserved",                 
    "Hypervisor Injection",     
    "VMM Communication",        
    "Security Exception",       
    "Reserved",                 // 31
};

void isr_handler(register_t* regs) {
    vga_print(exception_messages[regs->num_int]);
    vga_print("\n");
    for(;;);
}

void irq_handler(register_t* regs) {
    // envoyer EOI (End Of Interrupt) au PIC
    // obligatoire sinon le PIC ne renverra plus d'interruptions
    if (regs->num_int >= 40) {
        outb(0xA0, 0x20); // EOI au PIC2 si IRQ8-15
    }
    outb(0x20, 0x20); // EOI au PIC1 toujours
}