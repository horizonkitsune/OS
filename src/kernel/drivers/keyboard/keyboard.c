#include "keyboard.h"
#include "../../include/io.h"
#include "../vga/vga.h"

static char scancode_ascii(uint8_t scancode)
{
    char table[] = {
        0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', ')', '=', 0,    0,
        'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n', 0,
        'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 0,   0,   0,   '*',
        'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0,   0,   0,   0,   ' '
    };
    if (scancode && scancode < sizeof(table))
        return table[scancode];
    return 0;
}

void keyboard_init(void)
{
}

void keyboard_key(void)
{
    uint8_t scancode = inb(0x60);
    if (scancode & 0x80)
        return;
    char c = scancode_ascii(scancode);
    if (c)
        vga_putchar(c);
}
