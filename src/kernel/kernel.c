#include "mm/allocation.h"
#include "mm/pagination.h"
#include "process/process.h"
#include "drivers/vga/vga.h"
#include "drivers/keyboard/keyboard.h"
#include "include/io.h"

#define MULTIBOOT_MAGIC    0x1BADB002
#define MULTIBOOT_FLAGS    0x00
#define MULTIBOOT_CHECKSUM -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

__attribute__((section(".multiboot"))) int multiboot_header[] = {
    MULTIBOOT_MAGIC,
    MULTIBOOT_FLAGS,
    MULTIBOOT_CHECKSUM
};

void kernel_main(void)
{
    init_heap();
    init_pagination();
    init_process_manager();

    vga_init();
    vga_print("Hello OS!\n");

    keyboard_init();

    while (1)
    {
        if (inb(0x64) & 0x01)
        {
            keyboard_key();
            update_cursor();
        }
    }
}
