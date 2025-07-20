#include "kernel.h"
#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../gui/gui.h"

// Global kernel state
struct kernel_state {
    uint32_t ticks;
    uint8_t current_process;
    bool gui_initialized;
} kernel;

void kernel_main(void) {
    // Initialize VGA display
    vga_init();
    vga_clear_screen(VGA_COLOR_BLACK);
    
    vga_print("UAOS v1.0 - Ultimate Aslil Operating System\n", VGA_COLOR_WHITE);
    vga_print("Kernel loaded successfully!\n", VGA_COLOR_GREEN);
    
    // Initialize memory management
    memory_init();
    vga_print("Memory manager initialized\n", VGA_COLOR_CYAN);
    
    // Initialize process management
    process_init();
    vga_print("Process manager initialized\n", VGA_COLOR_CYAN);
    
    // Initialize interrupts
    interrupts_init();
    vga_print("Interrupt handlers installed\n", VGA_COLOR_CYAN);
    
    // Initialize keyboard driver
    keyboard_init();
    vga_print("Keyboard driver loaded\n", VGA_COLOR_CYAN);
    
    // Initialize GUI system
    gui_init();
    kernel.gui_initialized = true;
    vga_print("GUI system initialized\n", VGA_COLOR_CYAN);
    
    // Load core applications
    load_applications();
    vga_print("Core applications loaded\n", VGA_COLOR_CYAN);
    
    vga_print("\nStarting desktop environment...\n", VGA_COLOR_YELLOW);
    
    // Start GUI main loop
    gui_main_loop();
    
    // Should never reach here
    while(1) {
        asm("hlt");
    }
}

void load_applications(void) {
    // Register built-in applications
    app_register("UFileX", file_manager_main);
    app_register("UBrowse", browser_main);
    app_register("UMedia", media_player_main);
    app_register("UPack", package_manager_main);
}
