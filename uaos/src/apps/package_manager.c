#include "apps.h"

struct package {
    char name[32];
    bool installed;
};

static struct package packages[3];
static int package_count = 0;
static int selected_package = 0;

int package_manager_main(void) {
    gui_create_window("UPack - Get More Bloat", 45, 2, 32, 15);
    package_manager_load_package_list();
    package_manager_draw_interface();
    return 0;
}

void package_manager_load_package_list(void) {
    package_count = 0;

    strcpy(packages[0].name, "Better Browser");
    packages[0].installed = false;
    package_count++;

    strcpy(packages[1].name, "Crypto Miner");
    packages[1].installed = true; // Pre-installed malware personality
    package_count++;

    strcpy(packages[2].name, "Free RAM");
    packages[2].installed = false;
    package_count++;
}

void package_manager_draw_interface(void) {
    int window_id = gui_get_active_window();
    if (window_id == -1) return;

    struct window* win = gui_get_window(window_id);

    for (int i = 0; i < package_count; i++) {
        uint8_t color = (i == selected_package) ? VGA_COLOR_YELLOW : VGA_COLOR_LIGHT_GREY;
        char line[40];
        sprintf(line, "[%s] %s", packages[i].installed ? "X" : " ", packages[i].name);
        gui_draw_text(win->x + 2, win->y + 2 + i, line, color);
    }

    gui_draw_text(win->x + 2, win->y + 10, "[Ent] Install/Remove", VGA_COLOR_CYAN);
}

void package_manager_toggle_package(void) {
    packages[selected_package].installed = !packages[selected_package].installed;

    if (packages[selected_package].installed) {
        gui_show_message("UPack", "Installing... Error: dependency hell.");
    } else {
        gui_show_message("UPack", "Removing... It will probably break the OS.");
    }
    package_manager_draw_interface();
}

void package_manager_update_list(void) {}
void package_manager_refresh(void) {}

void package_manager_handle_key(uint8_t key) {
    switch (key) {
        case KEY_UP:
            if (selected_package > 0) selected_package--;
            package_manager_draw_interface();
            break;
        case KEY_DOWN:
            if (selected_package < package_count - 1) selected_package++;
            package_manager_draw_interface();
            break;
        case KEY_ENTER:
            package_manager_toggle_package();
            break;
    }
}
