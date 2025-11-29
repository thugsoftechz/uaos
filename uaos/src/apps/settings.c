#include "apps.h"

int settings_main(void) {
    gui_create_window("USet - Settings / Jugad", 20, 5, 40, 10);
    settings_draw_interface();
    return 0;
}

void settings_draw_interface(void) {
    int window_id = gui_get_active_window();
    if (window_id == -1) return;

    struct window* win = gui_get_window(window_id);

    // Title
    gui_draw_text(win->x + 2, win->y + 2, "Language Selection:", VGA_COLOR_WHITE);

    // Status
    char status[64];
    if (current_language == LANG_HINGLISH) {
        sprintf(status, "Current: [ Hinglish (Desi Style) ]");
    } else {
        sprintf(status, "Current: [ English (Boring Style) ]");
    }

    gui_draw_text(win->x + 2, win->y + 4, status, VGA_COLOR_YELLOW);

    // Help
    gui_draw_text(win->x + 2, win->y + 7, "[Space] Switch  [S] Save & Exit", VGA_COLOR_CYAN);
}

void settings_toggle_language(void) {
    if (current_language == LANG_HINGLISH) {
        current_language = LANG_ENGLISH;
        gui_show_message("USet", "Switched to English. How posh!");
    } else {
        current_language = LANG_HINGLISH;
        gui_show_message("USet", "Switched to Hinglish. Swag wapas aa gaya!");
    }
    settings_draw_interface();
}

void settings_handle_key(uint8_t key) {
    switch (key) {
        case KEY_SPACE:
            settings_toggle_language();
            break;
        case KEY_S:
            gui_close_window(gui_get_active_window());
            break;
    }
}
