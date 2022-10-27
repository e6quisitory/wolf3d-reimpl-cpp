#pragma once

double pi = 3.1415926535897932385;

int nearest_int(const double& a, const int& dir) {
    return dir > 0 ? ceil(a) : floor(a);
}

double degrees_to_radians(double degrees) {
    return degrees*pi/180;
}

double min(const double& a, const double& b) {
    return a < b ? a : b;
}