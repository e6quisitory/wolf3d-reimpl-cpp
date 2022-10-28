#pragma once

#include "misc.h"
#include "vec2.h"

class camera {
public:
    camera(point2 player_location, vec2 view_direction, double FOV, map* world):
        player_loc(player_location),
        view_dir(view_direction),
        left(rotate2d(view_direction, pi/2)),
        right(rotate2d(view_direction, -pi/2)),
        fov(degrees_to_radians(FOV)),
        world_map(world) {}

    ray get_ray(double progress) const {
        vec2 leftmost = rotate2d(view_dir, fov/2);
        vec2 curr_ray_dir = rotate2d(leftmost, -progress*fov);
        return ray(player_loc, curr_ray_dir);
    }

    bool move_x(double amount) {
        point2 proposed_loc = player_loc + amount*right;
        return move_if_valid(proposed_loc);
    }

    bool move_y(double amount) {
        point2 proposed_loc = player_loc + amount*view_dir;
        return move_if_valid(proposed_loc);
    }

    bool swivel(double radians) {
        view_dir = rotate2d(view_dir, radians);
        left = rotate2d(view_dir, pi/2);
        right = rotate2d(view_dir, -pi/2);
        return true;
    }

    void print_location() const {
        std::cout << player_loc << std::endl;
    }

private:
    bool collision(const point2& pt) const {
        if( world_map->check_tile(world_map->get_tile(vec2(pt.x(), pt.y()))) )
            return true;
        else
            return false;
    }

    bool move_if_valid(const point2& proposed_loc) {
        if (world_map->within_map(proposed_loc)) {
            if (collision(proposed_loc))
                return false;
            else {
                player_loc = proposed_loc;
                return true;
            }
        } else
            return false;
    }

public:
    point2 player_loc;
    vec2 view_dir;
    vec2 left;
    vec2 right;
    double fov;
    map* world_map;
};