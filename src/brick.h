#ifndef BRICK_H
#define BRICK_H

#include <iimavlib/video_ops.h>

#include "utils.h"

using namespace iimavlib;

class Brick {
   public:
    Brick(double creation_time);
    bool active = true;

    void draw(video_buffer_t* video_buffer, double time);
    void move_x(int dir);

   private:
    rgb_t color = random_color(25);
    vector<vector<int>> rectangles_coords = {};
    double creation_time;
};

#endif