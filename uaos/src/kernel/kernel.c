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

int current_language = LANG_HINGLISH; // Default to Hinglish

void kernel_main(void) {
    // Initialize VGA display
    vga_init();
    vga_clear_screen(VGA_COLOR_BLUE);

    // Boot Messages (Mixed/Hinglish by default because context)
    vga_print("UAOS v2.1 - Ultimate Aslil OS (Bilingual Edition)\n", VGA_COLOR_WHITE);
    vga_print("------------------------------------------------------\n", VGA_COLOR_WHITE);
    vga_print("Booting... Arre bhai, sabr karo (Have patience)...\n", VGA_COLOR_YELLOW);

    // Initialize memory management
    memory_init();
    vga_print("Memory: Sab theek hai (All good).\n", VGA_COLOR_LIGHT_GREY);

    // Initialize process management
    process_init();
    vga_print("Process Manager: Kaam chalu hai (Work in progress)...\n", VGA_COLOR_LIGHT_GREY);

    // Initialize interrupts
    interrupts_init();
    vga_print("Interrupts: Do not disturb, okay?\n", VGA_COLOR_LIGHT_GREY);

    // Initialize keyboard driver
    keyboard_init();
    vga_print("Keyboard: Connected. Type properly.\n", VGA_COLOR_LIGHT_GREY);

    // Initialize GUI system
    gui_init();
    kernel.gui_initialized = true;
    vga_print("Graphics: Ek number (Awesome).\n", VGA_COLOR_LIGHT_GREY);

    // Load core applications
    load_applications();
    vga_print("Apps: Loaded bloatware successfully.\n", VGA_COLOR_LIGHT_GREY);

    vga_print("\nStarting Desktop... Chalo bhai (Let's go)!\n", VGA_COLOR_YELLOW);

    // Small delay loop
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
    app_register("USet", settings_main);
}
