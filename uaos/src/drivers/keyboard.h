#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

// Keyboard scan codes
#define KEY_ESC 0x01
#define KEY_F1 0x3B
#define KEY_F2 0x3C
#define KEY_F3 0x3D
#define KEY_F4 0x3E
#define KEY_F5 0x3F
#define KEY_UP 0x48
#define KEY_DOWN 0x50
#define KEY_LEFT 0x4B
#define KEY_RIGHT 0x4D
#define KEY_HOME 0x47
#define KEY_SPACE 0x39
#define KEY_ENTER 0x1C
#define KEY_TAB 0x0F
#define KEY_U 0x16
#define KEY_R 0x13
#define KEY_S 0x1F

void keyboard_init(void);
uint8_t keyboard_read_scan_code(void);
bool keyboard_has_input(void);
uint8_t keyboard_get_key(void);

#endif
