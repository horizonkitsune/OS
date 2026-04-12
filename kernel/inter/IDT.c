#include "../include/io.h"
#include "../drivers/keyboard/keyboard.h"
#include "IDT.h"

#define IDT_SIZE 256 // table idt de 256 bits

struct IDT_entry {
    unsigned short int offset_lowerbits; // partie basse de l'adresse du handler
    unsigned short int selector; // segment kernel = 0x08
    unsigned char zero; // toujours 0
    unsigned char type_attr; // type = 0x8E (interrupt gate)
    unsigned short int offset_higherbits;  // partie haute de l'adresse du handler
};

struct IDT_entry IDT[IDT_SIZE];

struct idt_ptr_struct {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

static void load_idt(struct idt_ptr_struct *idt_ptr) {
    __asm__ volatile ("lidt %0"::"m"(*idt_ptr)); // chargement idt dans le cpu
    sti(); // activation interuption, cli desactive interuption
}

void idt_init(void) {
    unsigned int keyboard_add = (unsigned int)keyboard_key; //addresse clavier
    
    IDT[INT_KEYBOARD].offset_lowerbits  = keyboard_add & 0xffff;        // garde bits 0-15
    IDT[INT_KEYBOARD].offset_higherbits = (keyboard_add & 0xffff0000) >> 16; // garde bits 16-31
    IDT[INT_KEYBOARD].selector  = 0x08;  // segment kernel
    IDT[INT_KEYBOARD].type_attr = 0x8e;  // interrupt gate
    IDT[INT_KEYBOARD].zero      = 0;

    // ICW1
    outb(PIC_cmd, ICW1_init); //pic = Programmable Interrupt Controller, ->0x20 commande
    outb(PIC2_cmd, ICW1_init); //pic2 -> 0xA0
    // ICW2
    outb(PIC_data, 0x20); //0x21 -> données commence à 0x20 soit 32 -> 39
    outb(PIC2_data, 0x28); //0xA1 -> données commence à 40 -> 47
    // ICW3
    outb(PIC_data, 0x04); // PIC1 : PIC2 branché sur IRQ2
    outb(PIC2_data, 0x02); // PIC2 : identifiant 2
    // ICW4
    outb(PIC_data, ICW4_8086); // mode 8086
    outb(PIC2_data, ICW4_8086); //mode 8086

    outb(PIC_data, 0xFD); //masque clavier actif, 11111101 
    outb(PIC2_data, 0xFF); // desactivation PIC2, 11111111

    struct idt_ptr_struct idt_ptr;
    idt_ptr.limit = sizeof(IDT) - 1;
    idt_ptr.base  = (unsigned int)IDT;

    load_idt(&idt_ptr);
}