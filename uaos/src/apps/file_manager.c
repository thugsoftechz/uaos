#include "apps.h"

struct file_entry {
    char name[32];
    bool is_dir;
    int size;
};

static struct file_entry files[10];
static int file_count = 0;
static int selected_file = 0;
static char current_path[64] = "/";

int file_manager_main(void) {
    gui_create_window("UFileX - Delete Everything?", 2, 2, 40, 20);
    file_manager_scan_directory(current_path);
    file_manager_draw_interface();
    return 0;
}

void file_manager_scan_directory(const char* path) {
    (void)path; // Mock implementation

    file_count = 0;

    strcpy(files[0].name, "..");
    files[0].is_dir = true;
    file_count++;

    strcpy(files[1].name, "system32");
    files[1].is_dir = true;
    file_count++;

    strcpy(files[2].name, "passwords.txt");
    files[2].is_dir = false;
    files[2].size = 123;
    file_count++;

    strcpy(files[3].name, "virus.exe");
    files[3].is_dir = false;
    files[3].size = 666;
    file_count++;
}

void file_manager_draw_interface(void) {
    int window_id = gui_get_active_window();
    if (window_id == -1) return;

    struct window* win = gui_get_window(window_id);

    // Path
    char path_str[64];
    sprintf(path_str, "Path: %s", current_path);
    gui_draw_text(win->x + 2, win->y + 2, path_str, VGA_COLOR_WHITE);

    // List
    for (int i = 0; i < file_count; i++) {
        uint8_t color = (i == selected_file) ? VGA_COLOR_YELLOW : VGA_COLOR_LIGHT_GREY;

        char line[40];
        if (files[i].is_dir) {
            sprintf(line, "[DIR] %s", files[i].name);
        } else {
            sprintf(line, "      %s (%d B)", files[i].name, files[i].size);
        }

        gui_draw_text(win->x + 2, win->y + 4 + i, line, color);
    }

    // Help
    gui_draw_text(win->x + 2, win->y + 15, "[Ent] Open  [Del] Destroy", VGA_COLOR_CYAN);
}

void file_manager_open_selected(void) {
    if (files[selected_file].is_dir) {
        if (strcmp(files[selected_file].name, "..") == 0) {
            strcpy(current_path, "/"); // Simple mock
        } else {
            // Mock entering dir
            gui_show_message("UFileX", "Access Denied. You are not root, pleb.");
        }
    } else {
        gui_show_message("UFileX", "Cannot open file. Format unknown (and I don't care).");
    }
}

void file_manager_copy_file(void) {
    gui_show_message("UFileX", "Copy failed. Drive full of bloat.");
}

void file_manager_delete_file(void) {
    if (strcmp(files[selected_file].name, "system32") == 0) {
        gui_show_message("UFileX", "Deleting System32... Just kidding, I wish.");
    } else {
        gui_show_message("UFileX", "File deleted. Gone forever.");
        // Mock delete
    }
}

void file_manager_go_up_directory(void) {
    strcpy(current_path, "/");
}

void file_manager_handle_key(uint8_t key) {
    switch (key) {
        case KEY_UP:
            if (selected_file > 0) selected_file--;
            file_manager_draw_interface();
            break;
        case KEY_DOWN:
            if (selected_file < file_count - 1) selected_file++;
            file_manager_draw_interface();
            break;
        case KEY_ENTER:
            file_manager_open_selected();
            break;
        case KEY_S: // Used as delete key here for simplicity
            file_manager_delete_file();
            break;
    }
}
