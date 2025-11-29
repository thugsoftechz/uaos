#include "../drivers/keyboard.h"
#include "../drivers/vga.h"

// Simple keyboard driver using polling or mock for now
// In a real OS, this would be an interrupt handler

static uint8_t last_key = 0;
static bool key_available = false;

void keyboard_init(void) {
    // Stub
}

// In a real OS, the ISR would call this
void keyboard_handler(uint8_t scan_code) {
    last_key = scan_code;
    key_available = true;
}

uint8_t keyboard_read_scan_code(void) {
    // Read from port 0x60
    uint8_t scan_code = inb(0x60);
    return scan_code;
}

bool keyboard_has_input(void) {
    // Check status register port 0x64 bit 0
    return (inb(0x64) & 1);
}

uint8_t keyboard_get_key(void) {
    return inb(0x60);
}
