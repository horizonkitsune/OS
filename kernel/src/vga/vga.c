#include "vga.h"
#include "io.h"

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY (volatile uint16_t *)0xB8000

static int cursor_x = 0;
static int cursor_y = 0;

void update_cursor(void) {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    outb(0x3D4, 0x0A);
    uint8_t val = inb(0x3D5);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
    outb(0x3D4, 0x0B);
    val = inb(0x3D5);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

static void scroll(void) {
    volatile uint16_t *vga = VGA_MEMORY;

    // décale toutes les lignes d'une vers le haut
    for (int y = 0; y < VGA_HEIGHT - 1; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga[y * VGA_WIDTH + x] = vga[(y + 1) * VGA_WIDTH + x];
        }
    }

    // efface la dernière ligne
    for (int x = 0; x < VGA_WIDTH; x++) {
        vga[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (uint16_t)(' ' | (0x0F << 8));
    }

    cursor_y = VGA_HEIGHT - 1;
}

void vga_init(void) {
    volatile uint16_t *vga = VGA_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga[i] = (uint16_t)(' ' | (0x0F << 8));
    cursor_x = 0;
    cursor_y = 0;
    enable_cursor(0, 15);
    update_cursor();
}

void vga_putchar(char c) {
    volatile uint16_t *vga = VGA_MEMORY;
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        vga[cursor_y * VGA_WIDTH + cursor_x] = (uint16_t)(c | (0x0F << 8));
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }
    if (cursor_y >= VGA_HEIGHT) {
        scroll();
    }
    update_cursor();
}

void vga_print(const char *str) {
    while (*str)
        vga_putchar(*str++);
}

