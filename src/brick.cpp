#include "brick.h"

Brick::Brick(double creation_time) : creation_time(creation_time) {
    // TODO: Add more complicated shapes
    vector<int> start_coords{200, 0};
    rectangles_coords.push_back(start_coords);
}

void Brick::draw(video_buffer_t* video_buffer, double time) {
    for (auto coords : rectangles_coords) {
        int shift = (int)(time - creation_time) * 50;
        if (shift + coords[1] >= 1000-50) {
            active = false;
            shift = 1000 - 50;
        }
        rectangle_t rectangle = {coords[0], coords[1] + shift, 50, 50};
        draw_rectangle(*video_buffer, rectangle, color);
    }
}

void Brick::move_x(int dir) {
    for (auto& coords : rectangles_coords) {
        if(coords[0] == 0 && dir == -1 || coords[0] == 500-50 && dir == 1 || !active){
            return;
        }
    }
    for (auto& coords : rectangles_coords) {
        coords[0] += 50 * dir;
    }
}
