#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>
#include <stddef.h>

int sprintf(char* str, const char* format, ...);
int snprintf(char* str, size_t size, const char* format, ...);
int vsprintf(char* str, const char* format, va_list ap);

#endif
