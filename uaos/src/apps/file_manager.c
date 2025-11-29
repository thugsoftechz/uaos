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
    if (current_language == LANG_HINGLISH)
        gui_create_window("UFileX - Sarkari Office", 2, 2, 40, 20);
    else
        gui_create_window("UFileX - File Explorer", 2, 2, 40, 20);

    file_manager_scan_directory(current_path);
    file_manager_draw_interface();
    return 0;
}

void file_manager_scan_directory(const char* path) {
    (void)path;

    file_count = 0;

    strcpy(files[0].name, "..");
    files[0].is_dir = true;
    file_count++;

    if (current_language == LANG_HINGLISH) {
        strcpy(files[1].name, "Homework");
        files[1].is_dir = true;
        file_count++;

        strcpy(files[2].name, "Resume_For_Google.pdf");
        files[2].is_dir = false;
        files[2].size = 100;
        file_count++;

        strcpy(files[3].name, "Kundli_Matching.exe");
        files[3].is_dir = false;
        files[3].size = 500;
        file_count++;
    } else {
        strcpy(files[1].name, "Documents");
        files[1].is_dir = true;
        file_count++;

        strcpy(files[2].name, "CV.pdf");
        files[2].is_dir = false;
        files[2].size = 100;
        file_count++;

        strcpy(files[3].name, "Solitaire.exe");
        files[3].is_dir = false;
        files[3].size = 500;
        file_count++;
    }
}

void file_manager_draw_interface(void) {
    int window_id = gui_get_active_window();
    if (window_id == -1) return;

    struct window* win = gui_get_window(window_id);

    // Path
    char path_str[64];
    if (current_language == LANG_HINGLISH)
        sprintf(path_str, "Folder: %s", current_path);
    else
        sprintf(path_str, "Path: %s", current_path);
    gui_draw_text(win->x + 2, win->y + 2, path_str, VGA_COLOR_WHITE);

    // List
    for (int i = 0; i < file_count; i++) {
        uint8_t color = (i == selected_file) ? VGA_COLOR_YELLOW : VGA_COLOR_LIGHT_GREY;

        char line[40];
        if (files[i].is_dir) {
            sprintf(line, "[DIR] %s", files[i].name);
        } else {
            sprintf(line, "      %s (%d MB)", files[i].name, files[i].size);
        }

        gui_draw_text(win->x + 2, win->y + 4 + i, line, color);
    }

    // Help
    if (current_language == LANG_HINGLISH)
        gui_draw_text(win->x + 2, win->y + 15, "[Ent] Khol  [Del] Feiko", VGA_COLOR_CYAN);
    else
        gui_draw_text(win->x + 2, win->y + 15, "[Ent] Open  [Del] Delete", VGA_COLOR_CYAN);
}

void file_manager_open_selected(void) {
    if (files[selected_file].is_dir) {
        if (strcmp(files[selected_file].name, "..") == 0) {
            strcpy(current_path, "/");
        } else {
            if (current_language == LANG_HINGLISH)
                gui_show_message("UFileX", "File missing! Did you check under the sofa?");
            else
                gui_show_message("UFileX", "Directory access denied (Admin only).");
        }
    } else {
        if (current_language == LANG_HINGLISH)
            gui_show_message("UFileX", "Cannot open. Computer says 'No'.");
        else
            gui_show_message("UFileX", "Error: File format not supported.");
    }
}

void file_manager_copy_file(void) {
    gui_show_message("UFileX", "Copying... taking photocopy, please wait.");
}

void file_manager_delete_file(void) {
    if (strcmp(files[selected_file].name, "Homework") == 0) {
        if (current_language == LANG_HINGLISH)
            gui_show_message("UFileX", "Mummy will beat you if you delete this!");
        else
             gui_show_message("UFileX", "Access Denied: Protected System File.");
    } else {
        if (current_language == LANG_HINGLISH)
            gui_show_message("UFileX", "File gone. Tata bye bye.");
        else
            gui_show_message("UFileX", "File deleted successfully.");
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
        case KEY_S:
            file_manager_delete_file();
            break;
    }
}
