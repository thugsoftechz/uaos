#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// Minimal sprintf implementation
// Supports %s, %d, %c, %x
static void itoa(int value, char* str, int base) {
    char* ptr = str;
    char* ptr1 = str;
    char tmp_char;
    int tmp_value;

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }

    if (value < 0 && base == 10) {
        *ptr++ = '-';
        ptr1++;
        value = -value;
    }

    while (value > 0) {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789ABCDEF"[tmp_value - (value * base)];
    }
    *ptr = '\0';

    // Reverse string
    ptr--;
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

int sprintf(char* str, const char* format, ...) {
    va_list args;
    va_start(args, format);

    char* ptr = str;
    const char* f = format;

    while (*f) {
        if (*f != '%') {
            *ptr++ = *f++;
            continue;
        }

        f++;
        switch (*f) {
            case 's': {
                const char* s = va_arg(args, const char*);
                while (*s) *ptr++ = *s++;
                break;
            }
            case 'd': {
                int i = va_arg(args, int);
                char buf[32];
                itoa(i, buf, 10);
                char* b = buf;
                while (*b) *ptr++ = *b++;
                break;
            }
            case 'x': {
                int i = va_arg(args, int);
                char buf[32];
                itoa(i, buf, 16);
                char* b = buf;
                while (*b) *ptr++ = *b++;
                break;
            }
            case 'c': {
                int c = va_arg(args, int);
                *ptr++ = (char)c;
                break;
            }
            default:
                *ptr++ = *f;
                break;
        }
        f++;
    }

    *ptr = '\0';
    va_end(args);
    return ptr - str;
}
