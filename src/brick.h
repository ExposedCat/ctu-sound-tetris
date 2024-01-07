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
    vector<Point> points = {};
    bool active = true;
    double creation_time;

    void draw(video_buffer_t* video_buffer);
    bool bottom_collides();
    bool side_collides(int side);  // -1 = left, 1 = right
    int get_actual_y(Point point);
    void move_x(int dir);
    void freeze();
    void rotate();

   private:
    int form = 0;
    rgb_t color = Utils::random_color(25);
    SharedData* data;
};

#endif