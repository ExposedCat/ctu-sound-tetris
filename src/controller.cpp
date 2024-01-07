#include "controller.h"

Controller::Controller(const pAudioFilter& child, WindowData window,
                       SharedData* data)
    : SDLDevice(window.width, window.height,
                "Sound Tetris by Chipi & Chapa Corp."),
      AudioFilter(child),
      window(window),
      data(data),
      video_buffer(window.width, window.height) {
    start();
};

Controller::~Controller() { stop(); };

void Controller::draw_sequence() {
    draw_rectangle(video_buffer, {0, 0, video_buffer.size.width, video_buffer.size.height}, {0, 0, 0});
    Brick* active_brick = get_active_brick();
    active_brick->draw(&video_buffer, data->time);
    blit(video_buffer);
};

Brick* Controller::get_active_brick() {
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
        active_brick = new Brick(data->time);
        data->bricks.push_back(active_brick);
    }
    return active_brick;
}

error_type_t Controller::do_process(audio_buffer_t& buffer) {
    if (is_stopped()) {
        return error_type_t::failed;
    }
    draw_sequence();
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
            Brick* active_brick = get_active_brick();
            active_brick->move_x(-1);
            break;
        }
        case KEY_ARROW_RIGHT: {
            Brick* active_brick = get_active_brick();
            active_brick->move_x(1);
            break;
        }
    }

    return true;
}