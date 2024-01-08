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
const double bit8_amplitude = 255.0 / 2;
const double amplitude = numeric_limits<int16_t>::max();

const int KEY_ARROW_LEFT = 276;
const int KEY_ARROW_RIGHT = 275;
const int KEY_ARROW_DOWN = 274;
const int KEY_ARROW_UP = 273;
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

typedef struct Score {
    int points;
} Score;

// Hack for cyclic dependencies
class Brick;

typedef struct SharedData {
    WindowData window;
    Score score;
    bool gameover;
    bool fall;
    bool erase_line;
    double speed;
    double time;
    double time_step;
    vector<Brick*> bricks;
    double last_move_time;
    double frequency;
} SharedData;

#endif