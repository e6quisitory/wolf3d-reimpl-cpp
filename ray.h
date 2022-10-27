#pragma once
#include "vec2.h"

class ray {
public:
    ray(const vec2& o, const vec2& dir) {
        origin = o;
        direction = unit_vector(dir);
    }

    ray() {};

    vec2 at(double t) const {
        return origin + t*direction;
    }

    double y_at(double x) const {
        return origin.y() + ((x-origin.x())/direction.x())*direction.y();
    }

    double x_at(double y) const {
        return origin.x() + ((y-origin.y())/direction.y())*direction.x();
    }

    double get_t(const vec2& coord) const {
        return (coord.x() - origin.x())/direction.x();
    }

    int x_dir() const {
        return direction.x() > 0 ? 1 : -1;
    }

    int y_dir() const {
        return direction.y() > 0 ? 1 : -1;
    }

    int near_x() const {
        if (x_dir() == 1)
            return static_cast<int>(ceil(origin.x()));
        else
            return static_cast<int>(floor(origin.x()));
    }

    int near_y() const {
        if (y_dir() == 1)
            return static_cast<int>(ceil(origin.y()));
        else
            return static_cast<int>(floor(origin.y()));
    }

public:
    vec2 origin;
    vec2 direction;
};