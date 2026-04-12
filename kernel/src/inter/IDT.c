#include "io.h"
#include "keyboard.h"
#include "isr.h"
#include "IDT.h"

#define IDT_SIZE 256

struct IDT_entry {
    uint16_t offset_lowerbits;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t offset_higherbits;
} __attribute__((packed));

struct IDT_entry IDT[IDT_SIZE];

struct idt_ptr_struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static void load_idt(struct idt_ptr_struct *idt_ptr) {
    __asm__ volatile ("lidt %0" :: "m"(*idt_ptr));
    sti();
}

void set_idt(uint8_t index, uint32_t handler) {
    IDT[index].offset_lowerbits  =  handler & 0xFFFF;
    IDT[index].offset_higherbits = (handler >> 16) & 0xFFFF;
    IDT[index].selector          = 0x08;
    IDT[index].zero              = 0;
    IDT[index].type_attr         = 0x8E;
}

static void pic_remap(void) {
    outb(PIC1_CMD,  ICW1_INIT);
    outb(PIC2_CMD,  ICW1_INIT);
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);
    outb(PIC1_DATA, 0xFD); // masque tout sauf IRQ1 (clavier)
    outb(PIC2_DATA, 0xFF);
}

void idt_init(void) {
    pic_remap();

    // Exceptions 0-31
    set_idt(0,  (uint32_t)isr0);
    set_idt(1,  (uint32_t)isr1);
    set_idt(2,  (uint32_t)isr2);
    set_idt(3,  (uint32_t)isr3);
    set_idt(4,  (uint32_t)isr4);
    set_idt(5,  (uint32_t)isr5);
    set_idt(6,  (uint32_t)isr6);
    set_idt(7,  (uint32_t)isr7);
    set_idt(8,  (uint32_t)isr8);
    set_idt(9,  (uint32_t)isr9);
    set_idt(10, (uint32_t)isr10);
    set_idt(11, (uint32_t)isr11);
    set_idt(12, (uint32_t)isr12);
    set_idt(13, (uint32_t)isr13);
    set_idt(14, (uint32_t)isr14);
    set_idt(15, (uint32_t)isr15);
    set_idt(16, (uint32_t)isr16);
    set_idt(17, (uint32_t)isr17);
    set_idt(18, (uint32_t)isr18);
    set_idt(19, (uint32_t)isr19);
    set_idt(20, (uint32_t)isr20);
    set_idt(21, (uint32_t)isr21);
    set_idt(22, (uint32_t)isr22);
    set_idt(23, (uint32_t)isr23);
    set_idt(24, (uint32_t)isr24);
    set_idt(25, (uint32_t)isr25);
    set_idt(26, (uint32_t)isr26);
    set_idt(27, (uint32_t)isr27);
    set_idt(28, (uint32_t)isr28);
    set_idt(29, (uint32_t)isr29);
    set_idt(30, (uint32_t)isr30);
    set_idt(31, (uint32_t)isr31);

    // IRQ 0-15 → entrées 32-47
    set_idt(32, (uint32_t)irq0);
    set_idt(33, (uint32_t)irq1);
    set_idt(34, (uint32_t)irq2);
    set_idt(35, (uint32_t)irq3);
    set_idt(36, (uint32_t)irq4);
    set_idt(37, (uint32_t)irq5);
    set_idt(38, (uint32_t)irq6);
    set_idt(39, (uint32_t)irq7);
    set_idt(40, (uint32_t)irq8);
    set_idt(41, (uint32_t)irq9);
    set_idt(42, (uint32_t)irq10);
    set_idt(43, (uint32_t)irq11);
    set_idt(44, (uint32_t)irq12);
    set_idt(45, (uint32_t)irq13);
    set_idt(46, (uint32_t)irq14);
    set_idt(47, (uint32_t)irq15);

    struct idt_ptr_struct idt_ptr;
    idt_ptr.limit = sizeof(IDT) - 1;
    idt_ptr.base  = (uint32_t)IDT;
    load_idt(&idt_ptr);
}