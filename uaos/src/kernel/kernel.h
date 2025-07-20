#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stdbool.h>

// Basic type definitions
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;

// Kernel constants
#define KERNEL_VERSION "1.0.0"
#define MAX_PROCESSES 32
#define HEAP_SIZE (8 * 1024 * 1024) // 8MB heap

// Memory layout
#define KERNEL_BASE 0x100000
#define USER_BASE 0x400000
#define STACK_SIZE 0x2000

// System calls
#define SYS_EXIT 1
#define SYS_READ 2
#define SYS_WRITE 3
#define SYS_OPEN 4
#define SYS_CLOSE 5

// Function declarations
void kernel_main(void);
void load_applications(void);
void kernel_panic(const char* message);

// Application prototypes
int file_manager_main(void);
int browser_main(void);
int media_player_main(void);
int package_manager_main(void);

#endif
