#ifndef GUI_H
#define GUI_H

#include <stdint.h>
#include <stdbool.h>

struct window {
    int id;
    int x, y;
    int width, height;
    char title[32];
    bool active;
    bool visible;
};

void gui_init(void);
void gui_main_loop(void);
int gui_get_active_window(void);
void gui_set_active_window(int id);
struct window* gui_get_window(int id);
void gui_draw_text(int x, int y, const char* text, uint8_t color);
void gui_show_message(const char* title, const char* message);

// Internal drawing functions
void gui_draw_desktop(void);
void gui_draw_icon(int x, int y, const char* label, int type);
void gui_draw_window(int window_id);
int gui_create_window(const char* title, int x, int y, int width, int height);
void gui_close_window(int window_id);
void gui_handle_keyboard(uint8_t key);
void gui_launch_app(const char* app_name);
void gui_update(void);
void switch_to_graphics_mode(void);
void gui_draw_frame(int x, int y, int width, int height, uint8_t color, bool double_line);

#endif
