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

template<class point_type, class limit_type>
bool within_bounds(point_type tile, limit_type x_lim1, limit_type x_lim2, limit_type y_lim1, limit_type y_lim2) {
    if((tile.x() >= x_lim1 && tile.x() <= x_lim2) && (tile.y() >= y_lim1 && tile.y() <= y_lim2))
        return true;
    else
        return false;
}

bool is_integer(double d) {
    if (d - static_cast<int>(d) != 0.0)
        return false;
    else
        return true;
}