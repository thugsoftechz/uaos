#include "../kernel/kernel.h"
#include "../gui/gui.h"
#include "apps.h"

#define MAX_PLAYLIST_SIZE 100
#define MAX_FILENAME 64

struct media_file {
    char filename[MAX_FILENAME];
    char title[MAX_FILENAME];
    uint32_t duration; // in seconds
    uint8_t type; // 0=audio, 1=video, 2=image
};

static struct media_file playlist[MAX_PLAYLIST_SIZE];
static int playlist_count = 0;
static int current_track = 0;
static bool is_playing = false;
static uint32_t current_position = 0;

int media_player_main(void) {
    media_player_load_playlist();
    media_player_draw_interface();
    
    return 0;
}

void media_player_load_playlist(void) {
    // Load sample media files
    playlist_count = 0;
    
    strcpy(playlist[0].filename, "music/song1.mp3");
    strcpy(playlist[0].title, "Sample Song 1");
    playlist[0].duration = 180;
    playlist[0].type = 0;
    playlist_count++;
    
    strcpy(playlist[1].filename, "music/song2.wav");
    strcpy(playlist[1].title, "Sample Song 2");
    playlist[1].duration = 240;
    playlist[1].type = 0;
    playlist_count++;
    
    strcpy(playlist[2].filename, "video/demo.avi");
    strcpy(playlist[2].title, "Demo Video");
    playlist[2].duration = 300;
    playlist[2].type = 1;
    playlist_count++;
    
    strcpy(playlist[3].filename, "images/photo.bmp");
    strcpy(playlist[3].title, "Sample Photo");
    playlist[3].duration = 0;
    playlist[3].type = 2;
    playlist_count++;
}

void media_player_draw_interface(void) {
    int window_id = gui_get_active_window();
    if (window_id == -1) return;
    
    struct window* win = gui_get_window(window_id);
    
    // Draw title
    gui_draw_text(win->x + 5, win->y + 25, "UMedia - Media Player", 0x00);
    
    // Draw current track info
    if (current_track < playlist_count) {
        char info[128];
        sprintf(info, "Now %s: %s", 
               is_playing ? "Playing" : "Stopped", 
               playlist[current_track].title);
        gui_draw_text(win->x + 5, win->y + 40, info, 0x08);
        
        // Draw progress bar
        int bar_width = win->width - 20;
        int progress_width = (current_position * bar_width) / playlist[current_track].duration;
        
        vga_draw_rect(win->x + 5, win->y + 55, bar_width, 8, 0x08);
        if (progress_width > 0) {
            vga_draw_rect(win->x + 5, win->y + 55, progress_width, 8, 0x02); // Green progress
        }
        
        // Draw time info
        sprintf(info, "%02d:%02d / %02d:%02d", 
               current_position / 60, current_position % 60,
               playlist[current_track].duration / 60, playlist[current_track].duration % 60);
        gui_draw_text(win->x + 5, win->y + 68, info, 0x00);
    }
    
    // Draw control buttons
    vga_draw_rect(win->x + 20, win->y + 85, 30, 20, 0x08);
    gui_draw_text(win->x + 28, win->y + 90, "Prev", 0x0F);
    
    vga_draw_rect(win->x + 55, win->y + 85, 30, 20, 0x08);
    gui_draw_text(win->x + 63, win->y + 90, is_playing ? "Pause" : "Play", 0x0F);
    
    vga_draw_rect(win->x + 90, win->y + 85, 30, 20, 0x08);
    gui_draw_text(win->x + 98, win->y + 90, "Next", 0x0F);
    
    vga_draw_rect(win->x + 125, win->y + 85, 30, 20, 0x08);
    gui_draw_text(win->x + 133, win->y + 90, "Stop", 0x0F);
    
    // Draw playlist
    gui_draw_text(win->x + 5, win->y + 110, "Playlist:", 0x00);
    for (int i = 0; i < playlist_count && i < 5; i++) {
        int y_pos = win->y + 125 + i * 10;
        uint8_t color = (i == current_track) ? 0x0C : 0x00; // Red for current, black for others
        
        char entry[80];
        sprintf(entry, "%d. %s", i + 1, playlist[i].title);
        gui_draw_text(win->x + 8, y_pos, entry, color);
    }
}

void media_player_handle_key(uint8_t key) {
    switch (key) {
        case KEY_SPACE:
            media_player_toggle_play();
            break;
            
        case KEY_LEFT:
            media_player_previous_track();
            break;
            
        case KEY_RIGHT:
            media_player_next_track();
            break;
            
        case KEY_UP:
            media_player_volume_up();
            break;
            
        case KEY_DOWN:
            media_player_volume_down();
            break;
            
        case KEY_S:
            media_player_stop();
            break;
    }
    media_player_draw_interface();
}

void media_player_toggle_play(void) {
    is_playing = !is_playing;
    // In a real implementation, this would start/stop audio playback
}

void media_player_next_track(void) {
    if (current_track < playlist_count - 1) {
        current_track++;
        current_position = 0;
    }
}

void media_player_previous_track(void) {
    if (current_track > 0) {
        current_track--;
        current_position = 0;
    }
}

void media_player_stop(void) {
    is_playing = false;
    current_position = 0;
}
