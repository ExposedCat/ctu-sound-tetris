#include "controller.h"

Controller::Controller(const pAudioFilter& child, SharedData* data)
    : SDLDevice(data->window.width, data->window.height,
                "Sound Tetris by Chipi & Chapa Corp."),
      AudioFilter(child),
      data(data),
      video_buffer(data->window.width, data->window.height) {
    start();
};

Controller::~Controller() { stop(); };

void Controller::redraw_screen() {
    // Clear screen - draw a fullscreen black rectangle
    draw_rectangle(video_buffer,
                   {0, 0, video_buffer.size.width, video_buffer.size.height},
                   {0, 0, 0});

    ensure_active_brick();

    for (auto brick : data->bricks) {
        brick->draw(&video_buffer, data->time);
    }

    blit(video_buffer);
};

Brick* Controller::ensure_active_brick() {
    bool active_found = false;
    Brick* active_brick;
    for (auto brick : data->bricks) {
        if (brick->active) {
            active_brick = brick;
            active_found = true;
            break;
        }
    }
    if (!active_found) {
        active_brick = new Brick(data);
        data->bricks.push_back(active_brick);
    }
    return active_brick;
}

error_type_t Controller::do_process(audio_buffer_t& buffer) {
    if (is_stopped()) {
        return error_type_t::failed;
    }
    redraw_screen();
    return error_type_t::ok;
}

bool Controller::do_key_pressed(const int key, bool pressed) {
    if (!pressed) {
        return true;
    }
    switch (key) {
        case keys::key_q: {
            return false;
        }
        case KEY_ARROW_LEFT: {
            Brick* active_brick = ensure_active_brick();
            active_brick->move_x(-1);
            break;
        }
        case KEY_ARROW_RIGHT: {
            Brick* active_brick = ensure_active_brick();
            active_brick->move_x(1);
            break;
        }
    }

    return true;
}