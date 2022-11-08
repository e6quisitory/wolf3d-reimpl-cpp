#pragma once

#include <cmath>

#include "misc.h"
#include "vec2.h"

class player {
public:
    player(point2 player_location, vec2 view_direction, double FOV, map* const world):
        player_loc(player_location),
        view_dir(view_direction),
        left(rotate2d(view_direction, pi/2)),
        right(rotate2d(view_direction, -pi/2)),
        fov(degrees_to_radians(FOV)),
        world_map(world) {}

    player() {}

    ~player() {
        delete[] angles;
        delete[] cosines;
    }

    void calculate_ray_angles(int screen_width_pixels) {
        double proj_plane_width = 2*std::tan(fov/2);
        double segment_len = proj_plane_width/screen_width_pixels;

        // Pre-calculate the angles and the cosines of them, as they do not change
        angles = new double[screen_width_pixels];
        cosines = new double[screen_width_pixels];
        for (int i = 0; i < screen_width_pixels; ++i) {
            angles[i] = std::atan(-(i*segment_len-(proj_plane_width/2)));
            cosines[i] = std::cos(angles[i]);
        }
    }

    ray get_ray(int segment_num) const {
        vec2 direction = rotate2d(view_dir, angles[segment_num]);
        return ray(player_loc, direction, cosines[segment_num]);
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
        if( world_map->get_texture_id(world_map->get_tile(vec2(pt.x(), pt.y()))) != 0)
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
    double fov;

private:
    point2 player_loc;
    vec2 view_dir;
    vec2 left;
    vec2 right;
    double* angles;
    double* cosines;
    map* world_map;
};