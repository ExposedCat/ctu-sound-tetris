#ifndef BRICK_H
#define BRICK_H

#include "main.h"
#include "utils.h"

using namespace iimavlib;

// Hack for cyclic dependency
class SharedData;
class Point;

class Brick {
   public:
    Brick(SharedData* shared_data);
    bool active = true;
    double creation_time;

    void draw(video_buffer_t* video_buffer);
    bool bottom_collides();
    int get_actual_y(Point point);
    void move_x(int dir);

   private:
    rgb_t color = random_color(25);
    vector<Point> points = {};
    SharedData* data;
};

#endif