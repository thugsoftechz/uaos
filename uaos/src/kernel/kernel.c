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
    vga_clear_screen(VGA_COLOR_BLUE); // BSOD style background

    // Rude Boot Messages
    vga_print("UAOS v1.0 - Ultimate Aslil Operating System\n", VGA_COLOR_WHITE);
    vga_print("------------------------------------------\n", VGA_COLOR_WHITE);
    vga_print("Booting this garbage... ugh.\n", VGA_COLOR_LIGHT_GREY);

    // Initialize memory management
    memory_init();
    vga_print("Memory wasted successfully.\n", VGA_COLOR_LIGHT_GREY);

    // Initialize process management
    process_init();
    vga_print("Process manager broke something probably.\n", VGA_COLOR_LIGHT_GREY);

    // Initialize interrupts
    interrupts_init();
    vga_print("Interrupts enabled (so you can interrupt me).\n", VGA_COLOR_LIGHT_GREY);

    // Initialize keyboard driver
    keyboard_init();
    vga_print("Keyboard detected. Don't smash it.\n", VGA_COLOR_LIGHT_GREY);

    // Initialize GUI system
    gui_init();
    kernel.gui_initialized = true;
    vga_print("Fake GUI initialized. It looks terrible.\n", VGA_COLOR_LIGHT_GREY);

    // Load core applications
    load_applications();
    vga_print("Bloatware loaded.\n", VGA_COLOR_LIGHT_GREY);

    vga_print("\nStarting the worst desktop environment ever...\n", VGA_COLOR_YELLOW);

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
