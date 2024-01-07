#ifndef BRICK_H
#define BRICK_H

#include <iimavlib/video_ops.h>

#include "utils.h"

using namespace iimavlib;

class Brick {
   public:
    Brick(double creation_time);
    bool active = true;

    void draw(video_buffer_t* video_buffer);

   private:
    vector<rectangle_t> rectangles = {};
    rgb_t color = random_color(25);
    double creation_time;
};

#endif