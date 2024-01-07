#ifndef MAIN_H
#define MAIN_H

#include <iimavlib/AudioFilter.h>
#include <iimavlib/SDLDevice.h>
#include <iimavlib/Utils.h>
#include <iimavlib/keys.h>
#include <iimavlib/video_ops.h>
#include <iimavlib_high_api.h>

#include <iostream>

#include "brick.h"

using namespace std;
using namespace iimavlib;

namespace {
const double max_amplitude = 255.0 / 2;

const int KEY_ARROW_LEFT = 276;

const int KEY_ARROW_RIGHT = 275;
}  // namespace

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct WindowData {
    int width;
    int height;
    int rows;
    int columns;
    int brick_width;
    int brick_height;
} WindowData;

class Brick;

typedef struct SharedData {
    WindowData window;
    double speed;
    double time;
    double time_step;
    vector<Brick*> bricks;

    bool brick_moved;
} SharedData;

#endif