#pragma once

#include "misc.h"
#include "vec2.h"

class camera {
public:
    camera(point2 player_location, vec2 view_direction, double FOV):
        player_loc(player_location),
        view_dir(view_direction),
        left(rotate2d(view_direction, pi/2)),
        right(rotate2d(view_direction, -pi/2)),
        fov(degrees_to_radians(FOV)) {}

    ray get_ray(double progress) {
        vec2 leftmost = rotate2d(view_dir, fov/2);
        vec2 curr_ray_dir = rotate2d(leftmost, -progress*fov);
        return ray(player_loc, curr_ray_dir);
    }

    void move_x(double amount) {
        player_loc += amount*right;
    }

    void move_y(double amount) {
        player_loc += amount*view_dir;
    }

    void swivel(double radians) {
        view_dir = rotate2d(view_dir, radians);
        left = rotate2d(view_dir, pi/2);
        right = rotate2d(view_dir, -pi/2);
    }

    void print_location() {
        std::cout << player_loc << std::endl;
    }


public:
    point2 player_loc;
    vec2 view_dir;
    vec2 left;
    vec2 right;
    double fov;
};