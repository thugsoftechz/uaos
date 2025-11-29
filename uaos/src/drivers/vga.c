#include "../kernel/kernel.h"
#include "vga.h"

// We are assuming standard VGA text mode 80x25 at 0xB8000
static uint16_t* vga_buffer = (uint16_t*)0xB8000;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

void vga_init(void) {
    cursor_x = 0;
    cursor_y = 0;
    // Hide cursor if possible or just reset
    vga_update_cursor();
}

void vga_clear_screen(uint8_t color) {
    // Fill screen with space characters of the given background color
    uint16_t blank = vga_entry(' ', color);
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = blank;
    }
    cursor_x = 0;
    cursor_y = 0;
    vga_update_cursor();
}

uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

void vga_putchar(char c, uint8_t color) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\t') {
        cursor_x = (cursor_x + 4) & ~3;
    } else {
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c, color);
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT) {
        vga_scroll();
    }

    vga_update_cursor();
}

void vga_print(const char* str, uint8_t color) {
    while (*str) {
        vga_putchar(*str++, color);
    }
}

void vga_scroll(void) {
    // Move all lines up by one
    for (int y = 0; y < VGA_HEIGHT - 1; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = vga_buffer[(y + 1) * VGA_WIDTH + x];
        }
    }

    // Clear bottom line
    uint16_t blank = vga_entry(' ', VGA_COLOR_BLACK);
    for (int x = 0; x < VGA_WIDTH; x++) {
        vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = blank;
    }

    cursor_y = VGA_HEIGHT - 1;
}

void vga_update_cursor(void) {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;

    // Send position to VGA registers
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// TUI Implementation of "Graphics" functions

// Set a "pixel" in text mode implies changing the character/color at that cell
// Since we don't have real pixels, we treat the 80x25 grid as our resolution.
void vga_set_pixel(int x, int y, uint8_t color) {
    // Bounds check
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT) return;

    // In TUI, "setting a pixel" usually means setting the background color of a block
    // We use a space char with the background color set to 'color'
    // The attribute byte is (Background << 4) | Foreground
    // If we want a solid block of 'color', we set background to 'color'.
    // Foreground doesn't matter if char is space, but let's set it to color too.

    uint8_t attribute = (color << 4) | (color & 0x0F);
    vga_buffer[y * VGA_WIDTH + x] = (uint16_t)' ' | ((uint16_t)attribute << 8);
}

void vga_draw_rect(int x, int y, int width, int height, uint8_t color) {
    for (int dy = 0; dy < height; dy++) {
        for (int dx = 0; dx < width; dx++) {
            vga_set_pixel(x + dx, y + dy, color);
        }
    }
}

void vga_draw_string_at(int x, int y, const char* str, uint8_t color) {
    int cx = x;
    int cy = y;
    while (*str) {
        if (cx >= VGA_WIDTH) break;
        if (cy >= VGA_HEIGHT) break;

        // Combine character with color attribute (default background black or transparent?)
        // In TUI, we usually want to preserve background or set it.
        // Let's assume black background for text unless specified.
        // Or better, read current background?
        // For simplicity, use the passed color as foreground, and keep current background?
        // Reading video memory is possible.

        uint16_t current = vga_buffer[cy * VGA_WIDTH + cx];
        uint8_t current_bg = (current >> 12) & 0x0F; // High nibble of high byte

        // If color passed has high nibble 0, maybe we keep current bg?
        // But usually color argument is just foreground 0-15.
        // Let's construct attribute: (current_bg << 4) | (color & 0x0F)

        uint8_t attribute = (current_bg << 4) | (color & 0x0F);
        vga_buffer[cy * VGA_WIDTH + cx] = (uint16_t)(*str) | ((uint16_t)attribute << 8);

        cx++;
        str++;
    }
}
