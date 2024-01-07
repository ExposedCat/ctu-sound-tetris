#ifndef BRICK_H
#define BRICK_H

#include <iimavlib/video_ops.h>

#include "utils.h"

using namespace iimavlib;

class Brick {
   public:
    Brick();

   private:
    vector<rectangle_t> rectangles = {};
    rgb_t color = random_color(25);
    bool active = true;
};

#endif