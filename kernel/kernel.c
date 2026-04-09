#define MULTIBOOT_MAGIC    0x1BADB002
#define MULTIBOOT_FLAGS    0x00
#define MULTIBOOT_CHECKSUM -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

__attribute__((section(".multiboot"))) int multiboot_header[] = {
    MULTIBOOT_MAGIC,
    MULTIBOOT_FLAGS,
    MULTIBOOT_CHECKSUM
};

#include "drivers/vga/vga.h"
#include "drivers/keyboard/keyboard.h"
#include "include/io.h"

void kernel_main(void) {
    vga_init();
    vga_print("Hello OS ! \n");
    vga_print("test \n");
    keyboard_init();
    while (1)
    {
        if(inb(0x64) & 0x01)
            {
                keyboard_key();
                update_cursor();
            }
    }
}

