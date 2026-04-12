#include "vga.h"
#include "keyboard.h"
#include "IDT.h"
#include "io.h"

void kernel_main(void) {
    vga_init();
    vga_print("vga ok\n");
    idt_init();
    vga_print("idt ok\n");
    keyboard_init();
    vga_print("keyboard ok\n");
    vga_print("> ");
    while (1) {
        __asm__ volatile("hlt");
    }
}

