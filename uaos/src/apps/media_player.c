#include "apps.h"

struct song {
    char title[32];
    char artist[32];
    int duration;
};

static struct song playlist[5];
static int current_track = 0;
static bool is_playing = false;
static int volume = 50;

int media_player_main(void) {
    gui_create_window("UMedia - Deafening Noise", 10, 8, 50, 12);
    media_player_load_playlist();
    media_player_draw_interface();
    return 0;
}

void media_player_load_playlist(void) {
    strcpy(playlist[0].title, "Silence.mp3");
    strcpy(playlist[0].artist, "The Void");

    strcpy(playlist[1].title, "Fan Noise.wav");
    strcpy(playlist[1].artist, "Your CPU");

    strcpy(playlist[2].title, "Beep.ogg");
    strcpy(playlist[2].artist, "System Speaker");
}

void media_player_draw_interface(void) {
    int window_id = gui_get_active_window();
    if (window_id == -1) return;

    struct window* win = gui_get_window(window_id);

    // Status
    char status[64];
    sprintf(status, "%s", is_playing ? "PLAYING (Annoying)" : "PAUSED (Thank god)");
    gui_draw_text(win->x + 2, win->y + 2, status, VGA_COLOR_YELLOW);

    // Song Info
    char info[64];
    sprintf(info, "Track: %s", playlist[current_track].title);
    gui_draw_text(win->x + 2, win->y + 4, info, VGA_COLOR_WHITE);

    sprintf(info, "Artist: %s", playlist[current_track].artist);
    gui_draw_text(win->x + 2, win->y + 5, info, VGA_COLOR_LIGHT_GREY);

    // Controls help
    gui_draw_text(win->x + 2, win->y + 8, "[SPC] Play/Pause  [N] Next  [S] Stop/Exit", VGA_COLOR_CYAN);
}

void media_player_toggle_play(void) {
    is_playing = !is_playing;
    media_player_draw_interface();
    if (is_playing) {
        gui_show_message("UMedia", "Imagine music playing right now.");
    }
}

void media_player_next_track(void) {
    current_track = (current_track + 1) % 3;
    media_player_draw_interface();
}

void media_player_previous_track(void) {
    if (current_track > 0) current_track--;
    media_player_draw_interface();
}

void media_player_volume_up(void) {
    if (volume < 100) volume += 10;
}

void media_player_volume_down(void) {
    if (volume > 0) volume -= 10;
}

void media_player_stop(void) {
    is_playing = false;
    gui_close_window(gui_get_active_window());
}

void media_player_handle_key(uint8_t key) {
    switch (key) {
        case KEY_SPACE:
            media_player_toggle_play();
            break;
        case KEY_RIGHT:
            media_player_next_track();
            break;
        case KEY_LEFT:
            media_player_previous_track();
            break;
        case KEY_S:
            media_player_stop();
            break;
    }
}
