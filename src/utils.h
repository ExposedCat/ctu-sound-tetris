#ifndef UTILS_H
#define UTILS_H

#include <iimavlib/video_ops.h>

#include <random>

using namespace std;
using namespace iimavlib;

class Utils {
   public:
    static random_device rd;
    static mt19937 gen;

    static int random_int(int min, int max);

    static rgb_t random_color(int min = 25, int max = 255);
};

#endif