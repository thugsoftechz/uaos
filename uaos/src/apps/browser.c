#include "apps.h"

#define MAX_URL_LENGTH 64
#define MAX_PAGE_SIZE 1024

static char current_url[MAX_URL_LENGTH] = "http://localhost/";
static char page_content[MAX_PAGE_SIZE];
static int scroll_position = 0;

int browser_main(void) {
    gui_create_window("UBrowse - Internet for Idiots", 5, 5, 70, 18);
    strcpy(current_url, "http://nowhere.com/");
    browser_load_page(current_url);
    browser_draw_interface();
    return 0;
}

void browser_load_page(const char* url) {
    // Rude page loading
    strcpy(page_content,
        "ERROR 404: Your Brain Not Found\n"
        "\n"
        "Why are you here? There is no internet.\n"
        "Go touch grass.\n"
        "\n"
        "Features you want but can't have:\n"
        "* Google (Blocked)\n"
        "* Facebook (Who uses this?)\n"
        "* Download RAM (Try it)\n"
        "\n"
        "Current URL: ");
    strcat(page_content, url);
}

void browser_draw_interface(void) {
    int window_id = gui_get_active_window();
    if (window_id == -1) return;

    struct window* win = gui_get_window(window_id);

    // Draw address bar (Fake)
    // In TUI, we draw a line
    gui_draw_text(win->x + 2, win->y + 2, "URL: [____________________] (F5: Refresh)", VGA_COLOR_WHITE);
    gui_draw_text(win->x + 7, win->y + 2, current_url, VGA_COLOR_LIGHT_GREY);

    // Draw content area background
    vga_draw_rect(win->x + 1, win->y + 4, win->width - 2, win->height - 5, VGA_COLOR_WHITE);

    // Draw text content
    // We manually parse newlines for the TUI
    char* content = page_content;
    int line_y = win->y + 4;
    int col_x = win->x + 2;

    while (*content && line_y < win->y + win->height - 1) {
        if (*content == '\n') {
            line_y++;
            col_x = win->x + 2;
        } else {
            char str[2] = {*content, '\0'};
            gui_draw_text(col_x, line_y, str, VGA_COLOR_BLACK); // Black text on white
            col_x++;
            if (col_x > win->x + win->width - 3) {
                line_y++;
                col_x = win->x + 2;
            }
        }
        content++;
    }
}

void browser_render_html(int x, int y, int width, int height) {
    // Deprecated in TUI version, logic moved to draw_interface
    (void)x; (void)y; (void)width; (void)height;
}

void browser_handle_key(uint8_t key) {
    switch (key) {
        case KEY_F5:
            gui_show_message("UBrowse", "Refreshing... Still no internet, dummy.");
            break;
        case KEY_S:
             // Close
             gui_close_window(gui_get_active_window());
             break;
    }
}
