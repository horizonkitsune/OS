#pragma once

#include "stdint.h"

void vga_init(void);
void vga_putchar(char c);
void vga_print(const char *str);
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void update_cursor(void);