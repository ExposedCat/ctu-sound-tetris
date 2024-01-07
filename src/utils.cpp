#include "utils.h"

int Utils::random_int(int min, int max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

rgb_t Utils::random_color(int min, int max) {
    return {(uint8_t)random_int(min, max), (uint8_t)random_int(min, max),
            (uint8_t)random_int(min, max)};
}
