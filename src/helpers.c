#include "helpers.h"
#include <allegro.h>
#include <allegro/gfx.h>
#include <math.h>

int random_range(unsigned int low, unsigned int high) {
    return low + (rand() % (high - low));
}

int point_distance(unsigned int x, unsigned int targetX) {
    return abs((int)(x - targetX));
}