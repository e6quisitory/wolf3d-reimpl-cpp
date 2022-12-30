#pragma once

#include "vec2.h"
#include "ivec2.h"
#include "ray.h"

struct intersection {
    intersection(ray _r, point2 _pt, ipoint2 _ipt): Ray(_r), Point(_pt), iPoint(_ipt) {}
    intersection(ray _r, point2 _pt): Ray(_r), Point(_pt), iPoint(_pt) {}
    intersection() {}

    double dist() {
        return Ray.dist_to_pt(Point);
    }

    ray Ray;
    point2 Point;
    ipoint2 iPoint;
};

intersection next_intersection(const intersection& curr_inter) {
    point2 next_x = curr_inter.Ray.next_x_intersection(curr_inter.Point);
    point2 next_y = curr_inter.Ray.next_y_intersection(curr_inter.Point);

    double dist_next_x = curr_inter.Ray.dist_to_pt(next_x);
    double dist_next_y = curr_inter.Ray.dist_to_pt(next_y);

    if (dist_next_x < dist_next_y)
        return intersection(curr_inter.Ray, next_x, curr_inter.iPoint + curr_inter.Ray.x_dir_vec());
    else if (dist_next_y < dist_next_x)
        return intersection(curr_inter.Ray, next_y, curr_inter.iPoint + curr_inter.Ray.y_dir_vec());
    else
        return intersection(curr_inter.Ray, curr_inter.Point + curr_inter.Ray.direction, curr_inter.iPoint + curr_inter.Ray.x_dir_vec() + curr_inter.Ray.y_dir_vec());
}

