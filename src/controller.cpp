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

    if (!data->gameover) {
        ensure_active_brick();
        for (auto brick : data->bricks) {
            brick->draw(&video_buffer);
        }
    } else {
        // Rainbow cross
        draw_rectangle(video_buffer, {0, 0, 100, 100}, Utils::random_color());
        draw_rectangle(video_buffer, {0, 100, 100, 100}, Utils::random_color());
        draw_rectangle(video_buffer, {0, 900, 100, 100}, Utils::random_color());
        draw_rectangle(video_buffer, {0, 800, 100, 100}, Utils::random_color());
        draw_rectangle(video_buffer, {400, 0, 100, 100}, Utils::random_color());
        draw_rectangle(video_buffer, {400, 100, 100, 100},
                       Utils::random_color());
        draw_rectangle(video_buffer, {400, 900, 100, 100},
                       Utils::random_color());
        draw_rectangle(video_buffer, {400, 800, 100, 100},
                       Utils::random_color());
        draw_rectangle(video_buffer, {100, 200, 100, 100},
                       Utils::random_color());
        draw_rectangle(video_buffer, {100, 300, 100, 100},
                       Utils::random_color());
        draw_rectangle(video_buffer, {100, 600, 100, 100},
                       Utils::random_color());
        draw_rectangle(video_buffer, {100, 700, 100, 100},
                       Utils::random_color());
        draw_rectangle(video_buffer, {300, 600, 100, 100},
                       Utils::random_color());
        draw_rectangle(video_buffer, {300, 700, 100, 100},
                       Utils::random_color());
        draw_rectangle(video_buffer, {300, 200, 100, 100},
                       Utils::random_color());
        draw_rectangle(video_buffer, {300, 200, 100, 100},
                       Utils::random_color());
        draw_rectangle(video_buffer, {300, 300, 100, 100},
                       Utils::random_color());
        draw_rectangle(video_buffer, {300, 300, 100, 100},
                       Utils::random_color());
        draw_rectangle(video_buffer, {200, 400, 100, 100},
                       Utils::random_color());
        draw_rectangle(video_buffer, {200, 500, 100, 100},
                       Utils::random_color());
    }

    blit(video_buffer);
};

void Controller::check_gameover() {
    vector<int> line_states = get_lines_state();
    if (line_states[0] > 0) {
        data->gameover = true;
        printf("Game Over!\n");
    }
}

vector<int> Controller::get_lines_state() {
    vector<int> line_states = {};

    for (int y = 0; y < data->window.rows; ++y) {
        line_states.push_back(0);
    }

    for (auto brick : data->bricks) {
        if (brick->active) {
            continue;
        }
        for (auto point : brick->points) {
            int y = brick->get_actual_y(point);
            int row = y / data->window.brick_height;
            line_states[row] += 1;
        }
    }

    return line_states;
}

int Controller::clear_complete_lines() {
    int erased = 0;

    vector<int> line_states = get_lines_state();
    for (auto state : line_states) {
        if (state == data->window.columns) {
            data->erase_line = true;
            erased += 1;
        }
    }

    for (auto bit = data->bricks.begin(); bit != data->bricks.end();) {
        auto& brick = *bit;
        if (brick->active) {
            bit += 1;
            continue;
        }
        for (auto it = brick->points.begin(); it != brick->points.end();) {
            auto& point = *it;
            int y = brick->get_actual_y(point);
            int row = y / data->window.brick_height;
            if (line_states[row] == data->window.columns) {
                it = brick->points.erase(it);
            } else {
                it += 1;
            }
        }
        if (brick->points.size() == 0) {
            bit = data->bricks.erase(bit);
        } else {
            bit += 1;
        }
    }

    while (true) {
        bool stable = true;
        for (auto brick : data->bricks) {
            if (brick->active) {
                continue;
            }
            if (!brick->bottom_collides()) {
                stable = false;
                brick->fall();
            }
        }
        if (stable) {
            break;
        }
    }

    return erased;
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
    if (!data->gameover) {
        check_gameover();

        int erased_lines = clear_complete_lines();
        if (erased_lines) {
            data->score.points += erased_lines;
            data->speed += 0.3 * erased_lines;
            printf(
                "[Score] %d -> %d %s\n", data->score.points - erased_lines,
                data->score.points,
                data->score.points > 7
                    ? "☠️"
                    : (data->score.points > 6
                           ? "😱"
                           : (data->score.points > 5
                                  ? "😨"
                                  : (data->score.points > 4
                                         ? "🤯"
                                         : (data->score.points > 3
                                                ? "😳"
                                                : (data->score.points > 2
                                                       ? "😎"
                                                       : (data->score.points > 1
                                                              ? "🙂"
                                                              : "")))))));
        }
    }

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
        case keys::key_a: {
            Brick* active_brick = ensure_active_brick();
            data->last_move_time = data->time;
            active_brick->rotate();
            break;
        }
        case KEY_ARROW_UP:
        case KEY_ARROW_DOWN: {
            Brick* active_brick = ensure_active_brick();
            data->last_move_time = data->time;
            data->fall = true;
            if (key == KEY_ARROW_DOWN) {
                active_brick->move_y();
            } else {
                active_brick->fall();
            }
            break;
        }
        case KEY_ARROW_RIGHT:
        case KEY_ARROW_LEFT: {
            Brick* active_brick = ensure_active_brick();
            data->last_move_time = data->time;
            active_brick->move_x(key == KEY_ARROW_LEFT ? -1 : 1);
            break;
        }
    }
    return true;
}