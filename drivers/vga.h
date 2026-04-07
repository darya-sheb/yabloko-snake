#pragma once

void vga_clear_screen();

void vga_print_string(const char* s);

void vgaMode13();
void vgaMode3();

void vga_backspace();
void vga_set_char_color(unsigned offset, char c, unsigned char fg, unsigned char bg);