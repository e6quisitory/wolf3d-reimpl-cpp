#pragma once

#include "vec2.h"
#include "misc.h"
#include <cmath>

class ray {
public:
    ray(const vec2& o, const vec2& dir) {
        origin = o;
        direction = unit_vector(dir);

        dx_const = std::sqrt(1+pow(direction.y()/direction.x(),2));
        dy_const = std::sqrt(1+pow(direction.x()/direction.y(),2));
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


    int x_dir() const {
        return direction.x() > 0 ? 1 : -1;
    }

    int y_dir() const {
        return direction.y() > 0 ? 1 : -1;
    }

    int near_x(const point2& pt) const {
        if (!is_integer(pt.x())) {
            if (x_dir() == 1)
                return static_cast<int>(ceil(pt.x()));
            else
                return static_cast<int>(floor(pt.x()));
        } else {
            return pt.x() + x_dir();
        }
    }

    int near_y(const point2& pt) const {
        if (!is_integer(pt.y())) {
            if (y_dir() == 1)
                return static_cast<int>(ceil(pt.y()));
            else
                return static_cast<int>(floor(pt.y()));
        } else {
            return pt.y() + y_dir();
        }
    }

    double get_ray_dist_dx(double dx) const {
        return abs(dx)*dx_const;
    }

    double get_ray_dist_dy(double dy) const {
        return abs(dy)*dy_const;
    }

    double dist_to_pt(const point2& p) const {
        return (origin - p).length();
    }

public:
    vec2 origin;
    vec2 direction;
    double dx_const;
    double dy_const;
};