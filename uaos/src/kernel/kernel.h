#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Kernel constants
#define KERNEL_VERSION "1.0.0"
#define MAX_PROCESSES 32
#define HEAP_SIZE (8 * 1024 * 1024) // 8MB heap

// Language Settings
#define LANG_ENGLISH 0
#define LANG_HINGLISH 1
extern int current_language;

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

// Initialization functions
void memory_init(void);
void process_init(void);
void interrupts_init(void);

// App registration
typedef int (*app_entry_t)(void);
void app_register(const char* name, app_entry_t entry);

// Application prototypes
int file_manager_main(void);
int browser_main(void);
int media_player_main(void);
int package_manager_main(void);
int settings_main(void);

#endif
