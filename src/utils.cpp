#include "utils.h"

int random_int(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

rgb_t random_color(int min, int max) {
    return {(uint8_t)random_int(min, max), (uint8_t)random_int(min, max),
            (uint8_t)random_int(min, max)};
}
