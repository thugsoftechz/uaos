#include "../kernel/kernel.h"
#include "gui.h"
#include "../drivers/vga.h"
#include "../drivers/keyboard.h"

#define MAX_WINDOWS 16

static struct window windows[MAX_WINDOWS];
static int window_count = 0;
static int active_window = -1;
static bool desktop_mode = false;

// Desktop colors and layout
#define DESKTOP_COLOR 0x01  // Blue background
#define TASKBAR_COLOR 0x70  // Gray taskbar
#define TASKBAR_HEIGHT 20

void gui_init(void) {
    // Switch to graphics mode (320x200, 256 colors)
    switch_to_graphics_mode();
    
    window_count = 0;
    active_window = -1;
    desktop_mode = true;
    
    // Draw desktop
    gui_draw_desktop();
}

void switch_to_graphics_mode(void) {
    // Set VGA mode 13h (320x200, 256 colors)
    asm volatile (
        "mov $0x13, %%ax\n"
        "int $0x10\n"
        :
        :
        : "ax"
    );
}

void gui_draw_desktop(void) {
    // Clear screen with desktop color
    for (int y = 0; y < 200 - TASKBAR_HEIGHT; y++) {
        for (int x = 0; x < 320; x++) {
            vga_set_pixel(x, y, DESKTOP_COLOR);
        }
    }
    
    // Draw taskbar
    vga_draw_rect(0, 200 - TASKBAR_HEIGHT, 320, TASKBAR_HEIGHT, TASKBAR_COLOR);
    
    // Draw desktop icons
    gui_draw_icon(10, 10, "UFileX");
    gui_draw_icon(10, 60, "UBrowse");
    gui_draw_icon(10, 110, "UMedia");
    gui_draw_icon(10, 160, "UPack");
}

void gui_draw_icon(int x, int y, const char* label) {
    // Draw icon background
    vga_draw_rect(x, y, 40, 40, 0x0F); // White background
    vga_draw_rect(x + 1, y + 1, 38, 38, 0x08); // Gray icon
    
    // Draw simple icon pattern
    for (int dy = 10; dy < 30; dy++) {
        for (int dx = 10; dx < 30; dx++) {
            if ((dx + dy) % 4 == 0) {
                vga_set_pixel(x + dx, y + dy, 0x0C); // Light red
            }
        }
    }
    
    // Draw label (simplified text rendering)
    gui_draw_text(x, y + 42, label, 0x0F);
}

void gui_draw_text(int x, int y, const char* text, uint8_t color) {
    // Simplified 8x8 font rendering
    static const uint8_t font_a[] = {
        0x18, 0x24, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x00
    };
    
    int pos = 0;
    while (text[pos]) {
        char c = text[pos];
        // Use simple font pattern for all characters
        for (int row = 0; row < 8; row++) {
            uint8_t pattern = font_a[row];
            for (int col = 0; col < 8; col++) {
                if (pattern & (1 << (7 - col))) {
                    vga_set_pixel(x + pos * 8 + col, y + row, color);
                }
            }
        }
        pos++;
    }
}

int gui_create_window(const char* title, int x, int y, int width, int height) {
    if (window_count >= MAX_WINDOWS) return -1;
    
    struct window* win = &windows[window_count];
    win->id = window_count;
    win->x = x;
    win->y = y;
    win->width = width;
    win->height = height;
    win->visible = true;
    win->active = false;
    
    // Copy title
    int i;
    for (i = 0; i < sizeof(win->title) - 1 && title[i]; i++) {
        win->title[i] = title[i];
    }
    win->title[i] = '\0';
    
    gui_draw_window(window_count);
    
    return window_count++;
}

void gui_draw_window(int window_id) {
    struct window* win = &windows[window_id];
    if (!win->visible) return;
    
    // Draw window frame
    vga_draw_rect(win->x, win->y, win->width, win->height, 0x07); // Light gray
    vga_draw_rect(win->x + 2, win->y + 20, win->width - 4, win->height - 22, 0x0F); // White content area
    
    // Draw title bar
    vga_draw_rect(win->x, win->y, win->width, 18, win->active ? 0x01 : 0x08); // Blue if active, gray if not
    
    // Draw title text
    gui_draw_text(win->x + 4, win->y + 4, win->title, 0x0F);
    
    // Draw close button
    vga_draw_rect(win->x + win->width - 16, win->y + 2, 14, 14, 0x04); // Red close button
    gui_draw_text(win->x + win->width - 12, win->y + 5, "X", 0x0F);
}

void gui_main_loop(void) {
    while (1) {
        // Handle keyboard input
        if (keyboard_has_input()) {
            uint8_t key = keyboard_get_key();
            gui_handle_keyboard(key);
        }
        
        // Update display
        gui_update();
        
        // Small delay to prevent 100% CPU usage
        for (volatile int i = 0; i < 10000; i++);
    }
}

void gui_handle_keyboard(uint8_t key) {
    switch (key) {
        case KEY_F1:
            gui_launch_app("UFileX");
            break;
        case KEY_F2:
            gui_launch_app("UBrowse");
            break;
        case KEY_F3:
            gui_launch_app("UMedia");
            break;
        case KEY_F4:
            gui_launch_app("UPack");
            break;
        case KEY_ESC:
            if (active_window != -1) {
                gui_close_window(active_window);
            }
            break;
    }
}

void gui_launch_app(const char* app_name) {
    int window_id = gui_create_window(app_name, 50 + window_count * 20, 30 + window_count * 20, 250, 180);
    if (window_id != -1) {
        gui_set_active_window(window_id);
        
        // Launch specific application
        if (strcmp(app_name, "UFileX") == 0) {
            file_manager_main();
        } else if (strcmp(app_name, "UBrowse") == 0) {
            browser_main();
        } else if (strcmp(app_name, "UMedia") == 0) {
            media_player_main();
        } else if (strcmp(app_name, "UPack") == 0) {
            package_manager_main();
        }
    }
}

void gui_update(void) {
    // Redraw all visible windows
    for (int i = 0; i < window_count; i++) {
        if (windows[i].visible) {
            gui_draw_window(i);
        }
    }
}
