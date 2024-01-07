#include "brick.h"

Brick::Brick(double creation_time) : creation_time(creation_time) {
    // TODO: Add more complicated shapes
    rectangle_t rectangle = {225, 0, 50, 50};
    rectangles.push_back(rectangle);
}

void Brick::draw(video_buffer_t* video_buffer) {
    for (auto rectangle : rectangles) {
        draw_rectangle(*video_buffer, rectangle, color);
    }
}