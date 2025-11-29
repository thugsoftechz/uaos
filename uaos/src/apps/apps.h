#ifndef APPS_H
#define APPS_H

#include "../kernel/kernel.h"
#include "../drivers/keyboard.h"
#include "../gui/gui.h"
#include "../drivers/vga.h"
#include <string.h>
#include <stdio.h>

// Browser
int browser_main(void);
void browser_load_page(const char* url);
void browser_draw_interface(void);
void browser_render_html(int x, int y, int width, int height);
void browser_handle_key(uint8_t key);

// Media Player
int media_player_main(void);
void media_player_load_playlist(void);
void media_player_draw_interface(void);
void media_player_toggle_play(void);
void media_player_next_track(void);
void media_player_previous_track(void);
void media_player_volume_up(void);
void media_player_volume_down(void);
void media_player_stop(void);
void media_player_handle_key(uint8_t key);

// File Manager
int file_manager_main(void);
void file_manager_scan_directory(const char* path);
void file_manager_draw_interface(void);
void file_manager_open_selected(void);
void file_manager_copy_file(void);
void file_manager_delete_file(void);
void file_manager_go_up_directory(void);
void file_manager_handle_key(uint8_t key);

// Package Manager
int package_manager_main(void);
void package_manager_load_package_list(void);
void package_manager_draw_interface(void);
void package_manager_toggle_package(void);
void package_manager_update_list(void);
void package_manager_refresh(void);
void package_manager_handle_key(uint8_t key);

// Settings
int settings_main(void);
void settings_draw_interface(void);
void settings_toggle_language(void);
void settings_handle_key(uint8_t key);

#endif
