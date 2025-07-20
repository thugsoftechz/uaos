#include "../kernel/kernel.h"
#include "../gui/gui.h"
#include "apps.h"

#define MAX_FILES 50
#define MAX_FILENAME 32

struct file_entry {
    char name[MAX_FILENAME];
    uint32_t size;
    bool is_directory;
};

static struct file_entry files[MAX_FILES];
static int file_count = 0;
static int selected_file = 0;
static char current_path[256] = "/";

int file_manager_main(void) {
    file_manager_scan_directory(current_path);
    file_manager_draw_interface();
    
    return 0;
}

void file_manager_scan_directory(const char* path) {
    // Simulate directory listing
    file_count = 0;
    
    // Add parent directory entry
    strcpy(files[file_count].name, "..");
    files[file_count].is_directory = true;
    files[file_count].size = 0;
    file_count++;
    
    // Add some sample files and directories
    strcpy(files[file_count].name, "Documents");
    files[file_count].is_directory = true;
    files[file_count].size = 0;
    file_count++;
    
    strcpy(files[file_count].name, "Pictures");
    files[file_count].is_directory = true;
    files[file_count].size = 0;
    file_count++;
    
    strcpy(files[file_count].name, "readme.txt");
    files[file_count].is_directory = false;
    files[file_count].size = 1024;
    file_count++;
    
    strcpy(files[file_count].name, "config.sys");
    files[file_count].is_directory = false;
    files[file_count].size = 512;
    file_count++;
    
    strcpy(files[file_count].name, "boot.bin");
    files[file_count].is_directory = false;
    files[file_count].size = 8192;
    file_count++;
}

void file_manager_draw_interface(void) {
    // Clear window content area
    int window_id = gui_get_active_window();
    if (window_id == -1) return;
    
    struct window* win = gui_get_window(window_id);
    
    // Draw header
    gui_draw_text(win->x + 5, win->y + 25, "UFileX - File Manager", 0x00);
    gui_draw_text(win->x + 5, win->y + 35, current_path, 0x08);
    
    // Draw file list
    for (int i = 0; i < file_count && i < 10; i++) {
        int y_pos = win->y + 50 + i * 12;
        uint8_t color = (i == selected_file) ? 0x70 : 0x00; // Highlight selected
        
        // Draw selection background
        if (i == selected_file) {
            vga_draw_rect(win->x + 5, y_pos - 2, win->width - 10, 12, 0x70);
        }
        
        // Draw file icon and name
        char display_line[64];
        if (files[i].is_directory) {
            sprintf(display_line, "[DIR]  %s", files[i].name);
        } else {
            sprintf(display_line, "[FILE] %s (%d bytes)", files[i].name, files[i].size);
        }
        
        gui_draw_text(win->x + 8, y_pos, display_line, color);
    }
    
    // Draw status bar
    gui_draw_text(win->x + 5, win->y + win->height - 15, "F1:Open F2:Copy F3:Delete ESC:Exit", 0x08);
}

void file_manager_handle_key(uint8_t key) {
    switch (key) {
        case KEY_UP:
            if (selected_file > 0) {
                selected_file--;
                file_manager_draw_interface();
            }
            break;
            
        case KEY_DOWN:
            if (selected_file < file_count - 1) {
                selected_file++;
                file_manager_draw_interface();
            }
            break;
            
        case KEY_ENTER:
            file_manager_open_selected();
            break;
            
        case KEY_F2:
            file_manager_copy_file();
            break;
            
        case KEY_F3:
            file_manager_delete_file();
            break;
    }
}

void file_manager_open_selected(void) {
    if (selected_file >= 0 && selected_file < file_count) {
        if (files[selected_file].is_directory) {
            // Change directory
            if (strcmp(files[selected_file].name, "..") == 0) {
                // Go up one directory
                file_manager_go_up_directory();
            } else {
                // Enter directory
                strcat(current_path, files[selected_file].name);
                strcat(current_path, "/");
            }
            file_manager_scan_directory(current_path);
            selected_file = 0;
            file_manager_draw_interface();
        } else {
            // Open file (placeholder - would launch appropriate viewer)
            gui_show_message("File opened: ", files[selected_file].name);
        }
    }
}
