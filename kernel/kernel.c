#include "drivers/vga/vga.h"
#include "drivers/keyboard/keyboard.h"
#include "inter/IDT.h"
#include "include/io.h"

void kernel_main(void) {
    vga_init();
    vga_print("test OS\n");
    idt_init();
    vga_print("> ");
    while (1);
}

