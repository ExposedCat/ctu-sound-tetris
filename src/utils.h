#ifndef UTILS_H
#define UTILS_H

#include <random>

#include <iimavlib/video_ops.h>

using namespace std;
using namespace iimavlib;

int random_int(int min, int max);

rgb_t random_color(int min = 25, int max = 255);

#endif