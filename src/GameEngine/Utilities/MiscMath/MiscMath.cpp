#include "MiscMath.h"
#include "../Conventions.h"

bool IsInteger(double d) {
    if (d - static_cast<int>(d) != 0.0)
        return false;
    else
        return true;
}

double DegreesToRadians(double degrees) {
    return degrees*PI/180;
}