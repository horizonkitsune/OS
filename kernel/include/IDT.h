#pragma once

#define IDT_SIZE  256

/*------------------I/O Port PIC------------------*/
#define PIC1_CMD     0x20
#define PIC1_DATA    0x21
#define PIC2_CMD     0xA0
#define PIC2_DATA    0xA1

/*------------------ICW------------------*/
#define ICW1_INIT   0x11
#define ICW4_8086    0x01

/*------------------FONCTION------------------*/
void idt_init(void);