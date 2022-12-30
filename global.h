#pragma once

#include "constants.h"

bool is_integer(double d) {
    if (d - static_cast<int>(d) != 0.0)
        return false;
    else
        return true;
}

double degrees_to_radians(double degrees) {
    return degrees*PI/180;
}

enum X_DIR {
    EAST = 1,
    WEST = -1
};

enum Y_DIR {
    NORTH = 1,
    SOUTH = -1
};

enum SWIVEL_DIR {
    COUNTER_CLOCKWISE = 1,
    CLOCKWISE = -1
};