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

vector<vector<int>> Controller::create_matrix(){
    vector<vector<int>> filled_points(data->window.rows,
                                      vector<int>(data->window.columns, 0));
    for (auto brick : data->bricks) {
        if (!brick->active) {
            for (auto point : brick->points) {
                int row =
                    (brick->get_actual_y(point) / data->window.brick_height);
                int column = point.x / data->window.brick_width;
                filled_points[row][column] = 1;
            }
        }
    }
    return filled_points;
}

void Controller::check_gameover(vector<vector<int>> filled_points){
    for (auto is_filled : filled_points[0]) {
        if(is_filled){
            printf("Game Over!\n");
            data->gameover = true;
        }
    };
}

int Controller::check_complete_line(vector<vector<int>> filled_points) {
    int erased = 0;

    for (int y = 0; y < filled_points.size(); y++) {
        int filled_number = 0;
        for (auto is_filled : filled_points[y]) {
            filled_number += is_filled;
        };
        if (filled_number != data->window.columns) {
            continue;
        }
        erased += 1;
        for (auto brick : data->bricks) {
            if (brick->active) {
                continue;
            }
            for (auto iterator = brick->points.begin();
                 iterator != brick->points.end();) {
                auto& point = *iterator;
                int row =
                    brick->get_actual_y(point) / data->window.brick_height;
                if (row == y) {
                    iterator = brick->points.erase(iterator);
                } else {
                    if (row < y) {
                        point.y += data->window.brick_height;
                    }
                    iterator += 1;
                }
            }
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
    if (data->gameover){
        return error_type_t::ok;
    }
    vector<vector<int>> filled_points = create_matrix();
    // int erased_lines = check_complete_line(filled_points);
    check_gameover(filled_points);
    // // TODO: Move to separate method
    // data->score.points += erased_lines;
    // data->speed += 0.5 * erased_lines;
    // if (erased_lines) {
    //     printf("[Score] %d -> %d %s\n", data->score.points - erased_lines,
    //            data->score.points,
    //            data->score.points > 9
    //                ? "☠️"
    //                : (data->score.points > 8
    //                       ? "😱"
    //                       : (data->score.points > 7
    //                              ? "😨"
    //                              : (data->score.points > 6
    //                                     ? "🤯"
    //                                     : (data->score.points > 5
    //                                            ? "😳"
    //                                            : (data->score.points > 4
    //                                                   ? "😎"
    //                                                   : (data->score.points > 3
    //                                                          ? "🙂"
    //                                                          : "")))))));
    // }
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