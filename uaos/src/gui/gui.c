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
    gui_draw_desktop();
}

void gui_main_loop(void) {
    while (1) {
        if (keyboard_has_input()) {
            uint8_t key = keyboard_get_key();
            gui_handle_keyboard(key);
            while(keyboard_has_input()) keyboard_get_key();
        }
        gui_update();
    }
}

void gui_draw_frame(int x, int y, int width, int height, uint8_t color, bool double_line) {
    uint8_t tl = double_line ? 0xC9 : 0xDA;
    uint8_t tr = double_line ? 0xBB : 0xBF;
    uint8_t bl = double_line ? 0xC8 : 0xC0;
    uint8_t br = double_line ? 0xBC : 0xD9;
    uint8_t h  = double_line ? 0xCD : 0xC4;
    uint8_t v  = double_line ? 0xBA : 0xB3;

    vga_set_pixel_char(x, y, tl, color);
    vga_set_pixel_char(x + width - 1, y, tr, color);
    vga_set_pixel_char(x, y + height - 1, bl, color);
    vga_set_pixel_char(x + width - 1, y + height - 1, br, color);

    for (int i = 1; i < width - 1; i++) {
        vga_set_pixel_char(x + i, y, h, color);
        vga_set_pixel_char(x + i, y + height - 1, h, color);
    }
    for (int i = 1; i < height - 1; i++) {
        vga_set_pixel_char(x, y + i, v, color);
        vga_set_pixel_char(x + width - 1, y + i, v, color);
    }
}

void gui_draw_desktop(void) {
    vga_clear_screen(VGA_COLOR_BLUE);

    // Taskbar
    vga_draw_rect(0, VGA_HEIGHT - 1, VGA_WIDTH, 1, VGA_COLOR_LIGHT_GREY);
    gui_draw_text(0, VGA_HEIGHT - 1, "\xF0 Start \xB3", VGA_COLOR_BLACK);

    // Icons
    gui_draw_icon(2, 2, "UFileX", 0);
    gui_draw_icon(14, 2, "UBrowse", 1);
    gui_draw_icon(26, 2, "UMedia", 2);
    gui_draw_icon(38, 2, "UPack", 3);
    gui_draw_icon(50, 2, "USet", 4);
}

void gui_draw_icon(int x, int y, const char* label, int type) {
    uint8_t color = VGA_COLOR_CYAN;

    // Simple 3x2 ASCII Art for Icons
    // Folder
    if (type == 0) {
        vga_set_pixel_char(x, y, 0xDC, color);     // Lower half block
        vga_set_pixel_char(x+1, y, 0xDC, color);
        vga_set_pixel_char(x+2, y, 0xDC, color);
        vga_set_pixel_char(x, y+1, 0xDB, color);   // Full block
        vga_set_pixel_char(x+1, y+1, 0xDB, color);
        vga_set_pixel_char(x+2, y+1, 0xDB, color);
    }
    // Globe/Browser
    else if (type == 1) {
        vga_set_pixel_char(x+1, y, 0x0F, color);   // Sun/Globe char
        vga_set_pixel_char(x, y+1, 0xDC, color);
        vga_set_pixel_char(x+1, y+1, 0xDB, color);
        vga_set_pixel_char(x+2, y+1, 0xDC, color);
    }
    // Music
    else if (type == 2) {
        vga_set_pixel_char(x+1, y, 0x0E, color);   // Music note
        vga_set_pixel_char(x+1, y+1, 0xDB, color);
    }
    // Box/Package
    else if (type == 3) {
        vga_set_pixel_char(x, y, 0xDB, color);
        vga_set_pixel_char(x+2, y, 0xDB, color);
        vga_set_pixel_char(x+1, y+1, 0xDB, color);
    }
    // Settings (Gearish)
    else {
        vga_set_pixel_char(x+1, y, 0x0F, VGA_COLOR_LIGHT_GREY);
        vga_set_pixel_char(x+1, y+1, 0xCF, VGA_COLOR_LIGHT_GREY);
    }

    gui_draw_text(x, y + 2, label, VGA_COLOR_WHITE);
}

int gui_create_window(const char* title, int x, int y, int width, int height) {
    if (window_count >= MAX_WINDOWS) return -1;

    struct window* win = &windows[window_count];
    win->id = window_count;
    win->x = x; win->y = y; win->width = width; win->height = height;
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

    uint8_t border_col = VGA_COLOR_LIGHT_GREY;
    uint8_t title_col = win->active ? VGA_COLOR_BLUE : VGA_COLOR_DARK_GREY;

    // Fill Background
    vga_draw_rect(win->x, win->y, win->width, win->height, VGA_COLOR_WHITE);

    // Draw Frame
    gui_draw_frame(win->x, win->y, win->width, win->height, border_col, true);

    // Draw Title Bar (Filled rect + text)
    vga_draw_rect(win->x + 1, win->y + 1, win->width - 2, 1, title_col);
    gui_draw_text(win->x + 2, win->y + 1, win->title, VGA_COLOR_WHITE);

    // Close Button [X]
    gui_draw_text(win->x + win->width - 4, win->y + 1, "[X]", VGA_COLOR_WHITE);

    // Shadow (simulated)
    vga_draw_rect(win->x + win->width, win->y + 1, 1, win->height, VGA_COLOR_DARK_GREY);
    vga_draw_rect(win->x + 1, win->y + win->height, win->width, 1, VGA_COLOR_DARK_GREY);
}

void gui_draw_text(int x, int y, const char* text, uint8_t color) {
    vga_draw_string_at(x, y, text, color);
}

void gui_show_message(const char* title, const char* message) {
    int width = strlen(message) + 6;
    if (width < 25) width = 25;
    int height = 7;
    int x = (VGA_WIDTH - width) / 2;
    int y = (VGA_HEIGHT - height) / 2;

    int win = gui_create_window(title, x, y, width, height);
    gui_draw_text(x + 2, y + 3, message, VGA_COLOR_RED);
    gui_draw_text(x + 2, y + 5, "[ OK ]", VGA_COLOR_BLACK);
}

void gui_close_window(int window_id) {
    if (window_id >= 0 && window_id < window_count) {
        windows[window_id].visible = false;
        if (active_window == window_id) {
            active_window = -1;
            for(int i=window_count-1; i>=0; i--) {
                if (windows[i].visible) {
                    active_window = i;
                    break;
                }
            }
        }
        gui_draw_desktop();
        gui_update();
    }
}

struct window* gui_get_window(int id) {
    if (id >= 0 && id < window_count) return &windows[id];
    return NULL;
}

int gui_get_active_window(void) { return active_window; }
void gui_set_active_window(int id) { if (id >= 0 && id < window_count) active_window = id; }

void gui_update(void) {
    for (int i = 0; i < window_count; i++) {
        if (windows[i].visible) gui_draw_window(i);
    }
}

void gui_handle_keyboard(uint8_t key) {
    if (key == KEY_F1) gui_launch_app("UFileX");
    if (key == KEY_F2) gui_launch_app("UBrowse");
    if (key == KEY_F3) gui_launch_app("UMedia");
    if (key == KEY_F4) gui_launch_app("UPack");

    if (active_window != -1) {
        struct window* win = &windows[active_window];
        if (strstr(win->title, "UFileX")) file_manager_handle_key(key);
        else if (strstr(win->title, "UBrowse")) browser_handle_key(key);
        else if (strstr(win->title, "UMedia")) media_player_handle_key(key);
        else if (strstr(win->title, "UPack")) package_manager_handle_key(key);
        else if (strstr(win->title, "USet")) settings_handle_key(key);
    }
}

void gui_launch_app(const char* app_name) {
    if (strcmp(app_name, "UFileX") == 0) file_manager_main();
    else if (strcmp(app_name, "UBrowse") == 0) browser_main();
    else if (strcmp(app_name, "UMedia") == 0) media_player_main();
    else if (strcmp(app_name, "UPack") == 0) package_manager_main();
    else if (strcmp(app_name, "USet") == 0) settings_main();
}

void switch_to_graphics_mode(void) {}
