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
        brick->draw(&video_buffer);
    }

    blit(video_buffer);
};

void Controller::check_complete_line(){
    std::vector<std::vector<int>> idx(20, std::vector<int>(10, 0));
    for (auto brick : data->bricks) {
        if (!brick->active) {
            for (auto point : brick->points) {
                int index_r = (brick->get_actual_y(point) / data->window.brick_height);
                int index_c = (point.x / data->window.brick_width);
                idx[index_r][index_c] = 1;
            }
        }
    }
    
    for (int i = 0; i < idx.size(); i++) {
        int sum_of_elems = 0;
        for (auto& n : idx[i])
            sum_of_elems += n;
        if (sum_of_elems == data->window.columns) {
            for (auto brick : data->bricks) {
                if (!brick->active) {
                    for (auto it = brick->points.begin(); it != brick->points.end();) {
                        auto& point = *it;
                        int index_r = (brick->get_actual_y(point) / data->window.brick_height);
                        if (index_r == i) {
                            it = brick->points.erase(it);
                        } else {
                            if (index_r < i){
                                point.y += data->window.brick_height;
                            }
                            ++it;
                        }
                    }
                }
            }
        }
    }
}

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
    if (!active_found || active_brick->bottom_collides()) {
        active_brick = new Brick(data);
        data->bricks.push_back(active_brick);
    }
    return active_brick;
}

error_type_t Controller::do_process(audio_buffer_t& buffer) {
    if (is_stopped()) {
        return error_type_t::failed;
    }
    check_complete_line();
    redraw_screen();

    for (auto sample : buffer.data) {
        data->time += data->time_step * data->speed;
    }
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
            data->last_move_time = data->time;
            active_brick->move_x(-1);
            break;
        }
        case KEY_ARROW_RIGHT: {
            Brick* active_brick = ensure_active_brick();
            data->last_move_time = data->time;
            active_brick->move_x(1);
            break;
        }
    }

    return true;
}