#pragma once

#include "misc.h"
#include "vec2.h"

class camera {
public:
    camera(point2 player_location, vec2 view_direction, double FOV, double x_limit1, double x_limit2, double y_limit1, double y_limit2):
        player_loc(player_location),
        view_dir(view_direction),
        left(rotate2d(view_direction, pi/2)),
        right(rotate2d(view_direction, -pi/2)),
        fov(degrees_to_radians(FOV)),
        x_lim1(x_limit1),
        x_lim2(x_limit2),
        y_lim1(y_limit1),
        y_lim2(y_limit2) {}

    ray get_ray(double progress) {
        vec2 leftmost = rotate2d(view_dir, fov/2);
        vec2 curr_ray_dir = rotate2d(leftmost, -progress*fov);
        return ray(player_loc, curr_ray_dir);
    }

    bool move_x(double amount) {
        point2 proposed_loc = player_loc + amount*right;
        if (within_bounds<point2, double>(proposed_loc, x_lim1, x_lim2, y_lim1, y_lim2)) {
            player_loc = proposed_loc;
            return true;
        } else
            return false;
    }

    bool move_y(double amount) {
        point2 proposed_loc = player_loc + amount*view_dir;
        if (within_bounds<point2, double>(proposed_loc, x_lim1, x_lim2, y_lim1, y_lim2)) {
            player_loc = proposed_loc;
            return true;
        } else
            return false;
    }

    bool swivel(double radians) {
        view_dir = rotate2d(view_dir, radians);
        left = rotate2d(view_dir, pi/2);
        right = rotate2d(view_dir, -pi/2);
        return true;
    }

    void print_location() {
        std::cout << player_loc << std::endl;
    }


public:
    point2 player_loc;
    vec2 view_dir;
    vec2 left;
    vec2 right;
    double x_lim1;
    double x_lim2;
    double y_lim1;
    double y_lim2;
    double fov;
};