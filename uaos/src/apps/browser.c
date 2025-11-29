#include "apps.h"

#define MAX_URL_LENGTH 64
#define MAX_PAGE_SIZE 1024

static char current_url[MAX_URL_LENGTH] = "http://localhost/";
static char page_content[MAX_PAGE_SIZE];
static int scroll_position = 0;

int browser_main(void) {
    gui_create_window("UBrowse", 5, 5, 70, 18);
    strcpy(current_url, "http://shaadi.com/");
    browser_load_page(current_url);
    browser_draw_interface();
    return 0;
}

void browser_load_page(const char* url) {
    if (current_language == LANG_HINGLISH) {
        strcpy(page_content,
            "ERROR 420: Internet Cut By Mummy\n"
            "\n"
            "Beta, padhai kar lo!\n"
            "Sharma ji ka beta CEO ban gaya.\n"
            "\n"
            "Search Options:\n"
            "* Study Material (Locked)\n"
            "* Rishta/Marriage (Pending)\n"
            "* Free WiFi (Sapne mat dekho)\n");
    } else {
        strcpy(page_content,
            "ERROR 404: Network Unreachable\n"
            "\n"
            "Sir, please do your work.\n"
            "Everyone else is successful except you.\n"
            "\n"
            "Search Options:\n"
            "* Educational Content (Restricted)\n"
            "* Matrimony (Pending)\n"
            "* Public WiFi (Unavailable)\n");
    }
    strcat(page_content, "\nURL: ");
    strcat(page_content, url);
}

void browser_draw_interface(void) {
    int window_id = gui_get_active_window();
    if (window_id == -1) return;

    struct window* win = gui_get_window(window_id);

    // Draw address bar
    if (current_language == LANG_HINGLISH) {
        gui_draw_text(win->x + 2, win->y + 2, "Link: [____________________] (F5: Do Refresh)", VGA_COLOR_WHITE);
    } else {
        gui_draw_text(win->x + 2, win->y + 2, "Address: [____________________] (F5: Reload)", VGA_COLOR_WHITE);
    }
    gui_draw_text(win->x + 8, win->y + 2, current_url, VGA_COLOR_LIGHT_GREY);

    // Draw content area background
    vga_draw_rect(win->x + 1, win->y + 4, win->width - 2, win->height - 5, VGA_COLOR_WHITE);

    // Draw text content
    char* content = page_content;
    int line_y = win->y + 4;
    int col_x = win->x + 2;

    while (*content && line_y < win->y + win->height - 1) {
        if (*content == '\n') {
            line_y++;
            col_x = win->x + 2;
        } else {
            char str[2] = {*content, '\0'};
            gui_draw_text(col_x, line_y, str, VGA_COLOR_BLACK);
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
    (void)x; (void)y; (void)width; (void)height;
}

void browser_handle_key(uint8_t key) {
    switch (key) {
        case KEY_F5:
            if (current_language == LANG_HINGLISH)
                gui_show_message("UBrowse", "Refreshing... Net slow hai bhai.");
            else
                gui_show_message("UBrowse", "Reloading... Connection timed out.");
            break;
        case KEY_S:
             gui_close_window(gui_get_active_window());
             break;
    }
}
