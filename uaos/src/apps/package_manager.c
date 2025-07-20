#include "../kernel/kernel.h"
#include "../gui/gui.h"
#include "apps.h"

#define MAX_PACKAGES 50
#define MAX_PACKAGE_NAME 32

struct package {
    char name[MAX_PACKAGE_NAME];
    char version[16];
    char description[128];
    bool installed;
    uint32_t size; // in KB
};

static struct package packages[MAX_PACKAGES];
static int package_count = 0;
static int selected_package = 0;
static int view_mode = 0; // 0=all, 1=installed, 2=available

int package_manager_main(void) {
    package_manager_load_package_list();
    package_manager_draw_interface();
    
    return 0;
}

void package_manager_load_package_list(void) {
    package_count = 0;
    
    // Sample packages
    strcpy(packages[0].name, "text-editor");
    strcpy(packages[0].version, "1.0.0");
    strcpy(packages[0].description, "Simple text editor for UAOS");
    packages[0].installed = true;
    packages[0].size = 45;
    package_count++;
    
    strcpy(packages[1].name, "calculator");
    strcpy(packages[1].version, "2.1.0");
    strcpy(packages[1].description, "Basic calculator application");
    packages[1].installed = false;
    packages[1].size = 32;
    package_count++;
    
    strcpy(packages[2].name, "game-snake");
    strcpy(packages[2].version, "1.5.2");
    strcpy(packages[2].description, "Classic Snake game");
    packages[2].installed = false;
    packages[2].size = 28;
    package_count++;
    
    strcpy(packages[3].name, "network-tools");
    strcpy(packages[3].version, "3.0.1");
    strcpy(packages[3].description, "Network utilities and tools");
    packages[3].installed = true;
    packages[3].size = 156;
    package_count++;
    
    strcpy(packages[4].name, "paint-program");
    strcpy(packages[4].version, "1.2.3");
    strcpy(packages[4].description, "Simple paint and drawing program");
    packages[4].installed = false;
    packages[4].size = 89;
    package_count++;
}

void package_manager_draw_interface(void) {
    int window_id = gui_get_active_window();
    if (window_id == -1) return;
    
    struct window* win = gui_get_window(window_id);
    
    // Draw title and view mode
    gui_draw_text(win->x + 5, win->y + 25, "UPack - Package Manager", 0x00);
    
    const char* view_names[] = {"All Packages", "Installed", "Available"};
    char status[64];
    sprintf(status, "View: %s (%d packages)", view_names[view_mode], package_count);
    gui_draw_text(win->x + 5, win->y + 40, status, 0x08);
    
    // Draw package list
    int displayed = 0;
    for (int i = 0; i < package_count && displayed < 8; i++) {
        // Filter based on view mode
        if (view_mode == 1 && !packages[i].installed) continue;
        if (view_mode == 2 && packages[i].installed) continue;
        
        int y_pos = win->y + 60 + displayed * 15;
        uint8_t color = (i == selected_package) ? 0x70 : 0x00;
        
        // Draw selection background
        if (i == selected_package) {
            vga_draw_rect(win->x + 5, y_pos - 2, win->width - 10, 15, 0x70);
        }
        
        // Draw package info
        char info[128];
        sprintf(info, "%s %s %s (%dKB)", 
               packages[i].installed ? "[*]" : "[ ]",
               packages[i].name, 
               packages[i].version, 
               packages[i].size);
        gui_draw_text(win->x + 8, y_pos, info, color);
        
        // Draw description on next line
        gui_draw_text(win->x + 12, y_pos + 8, packages[i].description, color);
        
        displayed++;
    }
    
    // Draw commands
    gui_draw_text(win->x + 5, win->y + win->height - 25, 
                 "ENTER:Install/Remove  TAB:Change View", 0x08);
    gui_draw_text(win->x + 5, win->y + win->height - 15, 
                 "U:Update List  R:Refresh  ESC:Exit", 0x08);
}

void package_manager_handle_key(uint8_t key) {
    switch (key) {
        case KEY_UP:
            if (selected_package > 0) {
                selected_package--;
                package_manager_draw_interface();
            }
            break;
            
        case KEY_DOWN:
            if (selected_package < package_count - 1) {
                selected_package++;
                package_manager_draw_interface();
            }
            break;
            
        case KEY_ENTER:
            package_manager_toggle_package();
            break;
            
        case KEY_TAB:
            view_mode = (view_mode + 1) % 3;
            package_manager_draw_interface();
            break;
            
        case KEY_U:
            package_manager_update_list();
            break;
            
        case KEY_R:
            package_manager_refresh();
            break;
    }
}

void package_manager_toggle_package(void) {
    if (selected_package >= 0 && selected_package < package_count) {
        packages[selected_package].installed = !packages[selected_package].installed;
        
        char message[128];
        sprintf(message, "Package %s %s", 
               packages[selected_package].name,
               packages[selected_package].installed ? "installed" : "removed");
        
        gui_show_message("UPack", message);
        package_manager_draw_interface();
    }
}

void package_manager_update_list(void) {
    // Simulate updating package list from repository
    gui_show_message("UPack", "Updating package list...");
    
    // Add a new package to simulate update
    if (package_count < MAX_PACKAGES - 1) {
        strcpy(packages[package_count].name, "new-utility");
        strcpy(packages[package_count].version, "1.0.0");
        strcpy(packages[package_count].description, "Newly available utility");
        packages[package_count].installed = false;
        packages[package_count].size = 64;
        package_count++;
    }
    
    package_manager_draw_interface();
}

void package_manager_refresh(void) {
    gui_show_message("UPack", "Refreshing package database...");
    package_manager_draw_interface();
}
