#pragma once

#include "vec2.h"
#include "global.h"
#include <cmath>

class ray {
public:
    ray(const vec2& o, const vec2& dir): origin(o), direction(unit_vector(dir)) {
        dx_const = std::sqrt(1+pow(direction.y()/direction.x(),2));  // Change in length along ray upon change in x of 1 unit
        dy_const = std::sqrt(1+pow(direction.x()/direction.y(),2));  // Change in length along ray upon change in y of 1 unit

        x_dir = direction.x() > 0 ? EAST : WEST;
        x_dir_vec = ivec2(x_dir, 0);

        y_dir = direction.y() > 0 ? NORTH : SOUTH;
        y_dir_vec = ivec2(0, y_dir);

        y_step = std::sqrt(dx_const*dx_const - 1);  // Amount y changes for change of 1 unit in x
        x_step = std::sqrt(dy_const*dy_const - 1);  // Amount x changes for change of 1 unit in y
    }

    ray() {};

    vec2 at(double t) const {
        return origin + t*direction;
    }

    point2 next_x_intersection(const point2& pt) const {
        int next_x = near_x(pt);
        return point2(next_x, y_at(next_x));
    }

    point2 next_y_intersection(const point2& pt) const {
        int next_y = near_y(pt);
        return point2(x_at(next_y), next_y);
    }

    double dist_to_pt(const point2& p) const {
        return get_ray_dist_dx(p.x()-origin.x());
    }

private:
    double y_at(double x) const {
        return origin.y() + ((x-origin.x())/direction.x())*direction.y();
    }

    double x_at(double y) const {
        return origin.x() + ((y-origin.y())/direction.y())*direction.x();
    }

    int near_x(const point2& pt) const {
        if (!is_integer(pt.x())) {
            if (x_dir == EAST)
                return static_cast<int>(ceil(pt.x()));
            else
                return static_cast<int>(floor(pt.x()));
        } else {
            return pt.x() + x_dir;
        }
    }

    int near_y(const point2& pt) const {
        if (!is_integer(pt.y())) {
            if (y_dir == NORTH)
                return static_cast<int>(ceil(pt.y()));
            else
                return static_cast<int>(floor(pt.y()));
        } else {
            return pt.y() + y_dir;
        }
    }

    double get_ray_dist_dx(double dx) const {
        return abs(dx)*dx_const;
    }

    double get_ray_dist_dy(double dy) const {
        return abs(dy)*dy_const;
    }

public:
    vec2 origin;
    vec2 direction;

    X_DIR x_dir;
    ivec2 x_dir_vec;

    Y_DIR y_dir;
    ivec2 y_dir_vec;

    double x_step;
    double y_step;

private:
    double dx_const;
    double dy_const;
};