#pragma once

#define IDT_SIZE  256

#define INT_KEYBOARD 0x21

#define PIC_cmd      0x20
#define PIC_data     0x21
#define PIC2_cmd     0xA0
#define PIC2_data    0xA1

#define ICW1_init 0x11
#define ICW4_8086    0x01


void idt_init(void);