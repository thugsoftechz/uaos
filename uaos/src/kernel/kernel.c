#include "../kernel/kernel.h"
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
    vga_clear_screen(VGA_COLOR_BLUE);

    // Indian Style Boot Messages
    vga_print("UAOS v2.0 - Ultimate Aslil Operating System (Desi Edition)\n", VGA_COLOR_WHITE);
    vga_print("------------------------------------------------------\n", VGA_COLOR_WHITE);
    vga_print("Powering on... Arre bhai, wait na, why in hurry?\n", VGA_COLOR_YELLOW);

    // Initialize memory management
    memory_init();
    vga_print("Memory Check: Sab theek hai (Everything is fine).\n", VGA_COLOR_LIGHT_GREY);

    // Initialize process management
    process_init();
    vga_print("Process Manager: Doing the needful...\n", VGA_COLOR_LIGHT_GREY);

    // Initialize interrupts
    interrupts_init();
    vga_print("Interrupts: Don't disturb me, I am sleeping.\n", VGA_COLOR_LIGHT_GREY);

    // Initialize keyboard driver
    keyboard_init();
    vga_print("Keyboard: Connected. Don't type rubbish okay?\n", VGA_COLOR_LIGHT_GREY);

    // Initialize GUI system
    gui_init();
    kernel.gui_initialized = true;
    vga_print("Graphics: It is looking like wow.\n", VGA_COLOR_LIGHT_GREY);

    // Load core applications
    load_applications();
    vga_print("Apps: Loaded all the useless things.\n", VGA_COLOR_LIGHT_GREY);

    vga_print("\nSystem is ready! Chalo shuru karte hai (Let's start)...\n", VGA_COLOR_YELLOW);

    // Small delay loop simulation
    for(volatile int i=0; i<1000000; i++);

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
