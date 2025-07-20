#include "../kernel/kernel.h"
#include "../gui/gui.h"
#include "apps.h"

#define MAX_URL_LENGTH 256
#define MAX_PAGE_SIZE 4096

static char current_url[MAX_URL_LENGTH] = "http://localhost/";
static char page_content[MAX_PAGE_SIZE];
static int scroll_position = 0;

int browser_main(void) {
    strcpy(current_url, "http://example.com/");
    browser_load_page(current_url);
    browser_draw_interface();
    
    return 0;
}

void browser_load_page(const char* url) {
    // Simulate loading a simple HTML page
    strcpy(page_content, 
        "<html><head><title>UAOS Browser</title></head>"
        "<body><h1>Welcome to UBrowse</h1>"
        "<p>This is a simple web browser for UAOS.</p>"
        "<p>Features:</p>"
        "<ul><li>Basic HTML rendering</li>"
        "<li>Simple navigation</li>"
        "<li>Local page support</li></ul>"
        "<p>Current URL: ");
    strcat(page_content, url);
    strcat(page_content, "</p></body></html>");
}

void browser_draw_interface(void) {
    int window_id = gui_get_active_window();
    if (window_id == -1) return;
    
    struct window* win = gui_get_window(window_id);
    
    // Draw address bar
    vga_draw_rect(win->x + 5, win->y + 25, win->width - 10, 16, 0x0F);
    gui_draw_text(win->x + 8, win->y + 28, current_url, 0x00);
    
    // Draw navigation buttons
    vga_draw_rect(win->x + 5, win->y + 45, 30, 16, 0x08);
    gui_draw_text(win->x + 10, win->y + 48, "Back", 0x0F);
    
    vga_draw_rect(win->x + 40, win->y + 45, 30, 16, 0x08);
    gui_draw_text(win->x + 45, win->y + 48, "Fwd", 0x0F);
    
    vga_draw_rect(win->x + 75, win->y + 45, 30, 16, 0x08);
    gui_draw_text(win->x + 80, win->y + 48, "Ref", 0x0F);
    
    // Draw page content (simplified HTML rendering)
    browser_render_html(win->x + 5, win->y + 70, win->width - 10, win->height - 90);
}

void browser_render_html(int x, int y, int width, int height) {
    // Very basic HTML parsing and rendering
    char* content = page_content;
    int line_y = y;
    int char_x = x;
    bool in_tag = false;
    bool in_title = false;
    
    while (*content && line_y < y + height - 12) {
        if (*content == '<') {
            in_tag = true;
            // Check for specific tags
            if (strncmp(content, "<h1>", 4) == 0) {
                // Heading - make it bold/larger (simulate with color)
                line_y += 16;
                char_x = x;
                content += 3; // Skip to end of opening tag
            } else if (strncmp(content, "<p>", 3) == 0) {
                // Paragraph - new line
                line_y += 12;
                char_x = x;
                content += 2; // Skip to end of opening tag
            } else if (strncmp(content, "<li>", 4) == 0) {
                // List item
                line_y += 12;
                char_x = x + 8; // Indent
                gui_draw_text(x + 4, line_y, "*", 0x00);
                content += 3; // Skip to end of opening tag
            }
        } else if (*content == '>') {
            in_tag = false;
        } else if (!in_tag && *content != '\n' && *content != '\r') {
            // Regular character - render it
            if (char_x < x + width - 8) {
                char str[2] = {*content, '\0'};
                gui_draw_text(char_x, line_y, str, 0x00);
                char_x += 8;
            } else {
                // Wrap to next line
                line_y += 12;
                char_x = x;
            }
        }
        content++;
    }
}

void browser_handle_key(uint8_t key) {
    switch (key) {
        case KEY_F5:
            // Refresh page
            browser_load_page(current_url);
            browser_draw_interface();
            break;
            
        case KEY_UP:
            if (scroll_position > 0) {
                scroll_position--;
                browser_draw_interface();
            }
            break;
            
        case KEY_DOWN:
            scroll_position++;
            browser_draw_interface();
            break;
            
        case KEY_HOME:
            strcpy(current_url, "http://localhost/");
            browser_load_page(current_url);
            browser_draw_interface();
            break;
    }
}
