#include "brick.h"

#include "main.h"

Brick::Brick(SharedData* data) : data(data), creation_time(data->time) {
    // TODO: Add more complicated shapes
    int center =
        ((int)(data->window.width / data->window.brick_width) / 2 - 1) *
        data->window.brick_width;
    vector<int> start_coords{center, 0};
    rectangles_coords.push_back(start_coords);
}

void Brick::draw(video_buffer_t* video_buffer, double time) {
    for (auto coords : rectangles_coords) {
        int shift = (int)(time - creation_time) * data->window.brick_height;
        if (shift + coords[1] >=
            data->window.height - data->window.brick_height) {
            active = false;
            shift = data->window.height - data->window.brick_height;
        }
        rectangle_t rectangle = {coords[0], coords[1] + shift,
                                 data->window.brick_width,
                                 data->window.brick_height};
        draw_rectangle(*video_buffer, rectangle, color);
    }
}

void Brick::move_x(int dir) {
    for (auto& coords : rectangles_coords) {
        if (coords[0] == 0 && dir == -1 ||
            coords[0] == data->window.width - data->window.brick_width &&
                dir == 1 ||
            !active) {
            return;
        }
    }
    for (auto& coords : rectangles_coords) {
        int width = data->window.width / data->window.columns;
        coords[0] += width * dir;
    }
}
