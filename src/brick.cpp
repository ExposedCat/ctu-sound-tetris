#include "brick.h"

#include "main.h"

const int variations = 5;
const vector<vector<Point>> possible_bricks = {
    {{1, 1}, {-1, 1}, {0, 1}, {2, 1}},   // --
    {{0, 1}, {-1, 0}, {-1, 1}, {1, 1}},  // -^
    {{0, 1}, {1, 0}, {-1, 1}, {1, 1}},   // ^-
    {{1, 1}, {0, 0}, {1, 0}, {0, 1}},    // Square
    {{0, 1}, {-1, 1}, {0, 0}, {1, 0}},   // _^-
    {{0, 1}, {0, 0}, {-1, 1}, {1, 1}},   // T
    {{0, 1}, {0, 0}, {-1, 0}, {1, 1}}    // -|_
};

Brick::Brick(SharedData* data) : data(data), creation_time(data->time) {
    // TODO: Add more complicated shapes
    int center = (data->window.columns / 2 - 1) * data->window.brick_width;

    vector<Point> new_points =
        possible_bricks[Utils::random_int(0, variations - 1)];
    for (int i = 0; i < 4; ++i) {
        new_points[i].x *= data->window.brick_width;
        new_points[i].x += center;
        new_points[i].y *= data->window.brick_height;
    }
    points = new_points;
}

int Brick::get_actual_y(Point point) {
    if (!active) {
        return point.y;
    }
    int shift = (int)(data->time - creation_time) * data->window.brick_height;
    if (shift + point.y >= data->window.height - data->window.brick_height) {
        return data->window.height - data->window.brick_height;
    }
    return point.y + shift;
}

void Brick::draw(video_buffer_t* video_buffer) {
    for (auto point : points) {
        rectangle_t rectangle = {point.x, get_actual_y(point),
                                 data->window.brick_width,
                                 data->window.brick_height};
        draw_rectangle(*video_buffer, rectangle, color);
    }
}

void Brick::rotate() {
    int centerX = 0, centerY = 0;
    for (auto& point : points) {
        centerX += point.x;
        centerY += point.y;
    }
    Point center = points[0];

    for (auto& point : points) {
        int newX = center.y - point.y + center.x;
        int newY = point.x - center.x + center.y;
        int actualY = get_actual_y({newX, newY});
        if (newX < 0 || newX >= data->window.width || actualY < 0 ||
            actualY >= data->window.height) {
            return;
        }
        for (auto brick : data->bricks) {
            if (brick->active) {
                continue;
            }
            for (auto remote_point : brick->points) {
                int actual_remote_y = brick->get_actual_y(remote_point);
                if (remote_point.x == newX && remote_point.y == actualY) {
                    return;
                }
            }
        }
    }

    for (auto& point : points) {
        int newX = center.y - point.y + center.x;
        int newY = point.x - center.x + center.y;
        point.x = newX;
        point.y = newY;
    }
}

void Brick::freeze() {
    for (auto& point : points) {
        point.y = get_actual_y(point);
    }
    active = false;
}

bool Brick::bottom_collides() {
    for (auto brick : data->bricks) {
        if (brick->active || brick == this) {
            continue;
        }
        for (auto point : brick->points) {
            int y = brick->get_actual_y(point);
            for (auto& current_point : points) {
                if (current_point.x != point.x) {
                    continue;
                }
                int current_y = get_actual_y(current_point);
                if (current_y == y - data->window.brick_height) {
                    freeze();
                    return true;
                }
            }
        }
    }
    bool collides = false;
    for (auto& point : points) {
        int y = get_actual_y(point);
        if (y == data->window.height - data->window.brick_height) {
            collides = true;
            break;
        }
    }
    if (collides) {
        freeze();
        return true;
    }
    return false;
}

bool Brick::side_collides(int side) {
    for (auto brick : data->bricks) {
        if (brick->active) {
            continue;
        }
        for (auto point : brick->points) {
            int y = brick->get_actual_y(point);
            for (auto current_point : points) {
                int current_y = get_actual_y(current_point);
                if (current_y != y) {
                    continue;
                }
                if (current_point.x ==
                    point.x - data->window.brick_width * side) {
                    return true;
                }
            }
        }
    }
    for (auto point : points) {
        if (point.x == 0 && side == -1 ||
            point.x == data->window.width - data->window.brick_width &&
                side == 1) {
            return true;
        }
    }
    return false;
}

void Brick::move_x(int dir) {
    if (!active || side_collides(dir)) {
        return;
    }
    for (auto& point : points) {
        int width = data->window.width / data->window.columns;
        point.x += width * dir;
    }
}

void Brick::fall() {
    bool can_fall;
    do {
        can_fall = move_y();
    } while (can_fall);
}

bool Brick::move_y() {
    if (!bottom_collides()) {
        for (auto& point : points) {
            point.y += data->window.brick_height;
        }
        return true;
    }
    return false;
}