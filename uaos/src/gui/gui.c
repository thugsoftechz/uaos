#include "../gui/gui.h"
#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../apps/apps.h"
#include <string.h>

#define MAX_WINDOWS 10
static struct window windows[MAX_WINDOWS];
static int window_count = 0;
static int active_window = -1;

void gui_init(void) {
    window_count = 0;
    active_window = -1;
    // Draw initial desktop
    gui_draw_desktop();
}

void gui_main_loop(void) {
    while (1) {
        if (keyboard_has_input()) {
            uint8_t key = keyboard_get_key();
            // Debounce or process
            gui_handle_keyboard(key);

            // Consume extra bytes if needed
            while(keyboard_has_input()) keyboard_get_key();
        }
        // Redraw
        gui_update();
    }
}

void gui_draw_desktop(void) {
    // Clear screen to blue
    vga_clear_screen(VGA_COLOR_BLUE);

    // Draw taskbar
    vga_draw_rect(0, VGA_HEIGHT - 1, VGA_WIDTH, 1, VGA_COLOR_LIGHT_GREY);
    gui_draw_text(0, VGA_HEIGHT - 1, " Start ", VGA_COLOR_BLACK);

    // Draw Icons
    gui_draw_icon(2, 2, "UFileX");
    gui_draw_icon(12, 2, "UBrowse");
    gui_draw_icon(22, 2, "UMedia");
    gui_draw_icon(32, 2, "UPack");
}

void gui_draw_icon(int x, int y, const char* label) {
    gui_draw_text(x, y, "[ICON]", VGA_COLOR_CYAN);
    gui_draw_text(x, y + 1, label, VGA_COLOR_WHITE);
}

int gui_create_window(const char* title, int x, int y, int width, int height) {
    if (window_count >= MAX_WINDOWS) return -1;

    struct window* win = &windows[window_count];
    win->id = window_count;
    win->x = x;
    win->y = y;
    win->width = width;
    win->height = height;
    strcpy(win->title, title);
    win->active = true;
    win->visible = true;

    active_window = window_count;
    window_count++;

    gui_draw_window(win->id);
    return win->id;
}

void gui_draw_window(int window_id) {
    if (window_id < 0 || window_id >= window_count) return;
    struct window* win = &windows[window_id];
    if (!win->visible) return;

    // Window Border
    vga_draw_rect(win->x, win->y, win->width, win->height, VGA_COLOR_LIGHT_GREY);

    // Title Bar
    vga_draw_rect(win->x, win->y, win->width, 1, win->active ? VGA_COLOR_BLUE : VGA_COLOR_DARK_GREY);
    gui_draw_text(win->x + 1, win->y, win->title, VGA_COLOR_WHITE);

    // Content Area (White)
    vga_draw_rect(win->x + 1, win->y + 1, win->width - 2, win->height - 2, VGA_COLOR_WHITE);
}

void gui_draw_text(int x, int y, const char* text, uint8_t color) {
    vga_draw_string_at(x, y, text, color);
}

void gui_show_message(const char* title, const char* message) {
    // Simple popup
    int width = strlen(message) + 4;
    if (width < 20) width = 20;
    int height = 5;
    int x = (VGA_WIDTH - width) / 2;
    int y = (VGA_HEIGHT - height) / 2;

    int win = gui_create_window(title, x, y, width, height);
    gui_draw_text(x + 2, y + 2, message, VGA_COLOR_RED);
}

void gui_close_window(int window_id) {
    if (window_id >= 0 && window_id < window_count) {
        windows[window_id].visible = false;
        if (active_window == window_id) {
            active_window = -1;
            // Find another active window
            for(int i=window_count-1; i>=0; i--) {
                if (windows[i].visible) {
                    active_window = i;
                    break;
                }
            }
        }
        gui_draw_desktop(); // Redraw everything
        gui_update();
    }
}

struct window* gui_get_window(int id) {
    if (id >= 0 && id < window_count) return &windows[id];
    return NULL;
}

int gui_get_active_window(void) {
    return active_window;
}

void gui_set_active_window(int id) {
    if (id >= 0 && id < window_count) active_window = id;
}

void gui_update(void) {
    for (int i = 0; i < window_count; i++) {
        if (windows[i].visible) {
            gui_draw_window(i);
        }
    }
}

void gui_handle_keyboard(uint8_t key) {
    // Global shortcuts
    if (key == KEY_F1) gui_launch_app("UFileX");
    if (key == KEY_F2) gui_launch_app("UBrowse");
    if (key == KEY_F3) gui_launch_app("UMedia");
    if (key == KEY_F4) gui_launch_app("UPack");

    // Pass to active app
    if (active_window != -1) {
        struct window* win = &windows[active_window];
        if (strstr(win->title, "UFileX")) file_manager_handle_key(key);
        else if (strstr(win->title, "UBrowse")) browser_handle_key(key);
        else if (strstr(win->title, "UMedia")) media_player_handle_key(key);
        else if (strstr(win->title, "UPack")) package_manager_handle_key(key);
    }
}

void gui_launch_app(const char* app_name) {
    if (strcmp(app_name, "UFileX") == 0) file_manager_main();
    else if (strcmp(app_name, "UBrowse") == 0) browser_main();
    else if (strcmp(app_name, "UMedia") == 0) media_player_main();
    else if (strcmp(app_name, "UPack") == 0) package_manager_main();
}

void switch_to_graphics_mode(void) {
    // Stub
}
