#include "MiscMath.h"

bool IsInteger(const double d) {
    if (d - static_cast<int>(d) != 0.0)
        return false;
    else
        return true;
}

double DegreesToRadians(const double degrees) {
    return degrees*PI/180;
}