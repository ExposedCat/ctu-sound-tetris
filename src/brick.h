#ifndef BRICK_H
#define BRICK_H

#include "main.h"
#include "utils.h"

using namespace iimavlib;

class SharedData;

class Brick {
   public:
    Brick(SharedData* shared_data);
    bool active = true;

    void draw(video_buffer_t* video_buffer, double time);
    void move_x(int dir);

   private:
    rgb_t color = random_color(25);
    vector<vector<int>> rectangles_coords = {};
    double creation_time;
    SharedData* data;
};

#endif