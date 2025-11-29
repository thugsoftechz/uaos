#include "apps.h"

struct song {
    char title[32];
    char artist[32];
};

static struct song playlist[5];
static int current_track = 0;
static bool is_playing = false;
static int volume = 80;

int media_player_main(void) {
    gui_create_window("UMedia - DJ Wale Babu", 10, 8, 50, 12);
    media_player_load_playlist();
    media_player_draw_interface();
    return 0;
}

void media_player_load_playlist(void) {
    strcpy(playlist[0].title, "Tadap Tadap Ke.mp3");
    strcpy(playlist[0].artist, "Broken Heart");

    strcpy(playlist[1].title, "Tunak Tunak Tun.wav");
    strcpy(playlist[1].artist, "Legend");

    strcpy(playlist[2].title, "Saree Ke Fall Sa.ogg");
    strcpy(playlist[2].artist, "Random Movie");
}

void media_player_draw_interface(void) {
    int window_id = gui_get_active_window();
    if (window_id == -1) return;

    struct window* win = gui_get_window(window_id);

    // Status
    char status[64];
    sprintf(status, "%s", is_playing ? "PLAYING (Full Volume)" : "PAUSED (Shanti)");
    gui_draw_text(win->x + 2, win->y + 2, status, VGA_COLOR_YELLOW);

    // Song Info
    char info[64];
    sprintf(info, "Gaana: %s", playlist[current_track].title);
    gui_draw_text(win->x + 2, win->y + 4, info, VGA_COLOR_WHITE);

    sprintf(info, "Singer: %s", playlist[current_track].artist);
    gui_draw_text(win->x + 2, win->y + 5, info, VGA_COLOR_LIGHT_GREY);

    // Controls help
    gui_draw_text(win->x + 2, win->y + 8, "[SPC] Nacho  [N] Next  [S] Bas Kar", VGA_COLOR_CYAN);
}

void media_player_toggle_play(void) {
    is_playing = !is_playing;
    media_player_draw_interface();
    if (is_playing) {
        gui_show_message("UMedia", "Neighbors will complain now!");
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
