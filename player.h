#pragma once

#include <cmath>

#include "misc.h"
#include "vec2.h"

class player {
public:
    player(point2 player_location, vec2 view_direction, double FOV, map* const world):
        player_loc(player_location),
        view_dir(unit_vector(view_direction)),
        left(rotate2d(view_direction, pi/2)),
        right(rotate2d(view_direction, -pi/2)),
        fov(degrees_to_radians(FOV)),
        world_map(world) {}

    player() {}

    void calculate_ray_angles(int screen_width_pixels) {
        double proj_plane_width = 2*std::tan(fov/2);
        double segment_len = proj_plane_width/screen_width_pixels;

        // Pre-calculate the angles and the cosines of them, as they do not change
        for (int i = 0; i < screen_width_pixels; ++i) {
            angles.push_back(std::atan(-(i*segment_len-(proj_plane_width/2))));
            cosines.push_back(std::cos(angles[i]));
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

    bool open_door() {
        int start = angles.size() / 3;
        int end = 2*start;
        for (int i = start; i < end; ++i) {
            hit_info ray_hit = world_map->hit(get_ray(i));
            if (world_map->door_currently_opening(ray_hit.arr_index))
                return true;
            else if (ray_hit.texture_id == 99 && ray_hit.dist < 3) {
                world_map->doors_currently_opening.push_back(ray_hit.arr_index);
                return true;
            }
        }
        return false;
    }

private:
    bool collision(const point2& pt) const {
        ipoint2 tile = world_map->get_tile(pt);
        int tile_index = world_map->tile_to_index(tile);
        int texture_id = world_map->get_texture_id(world_map->get_tile(pt));

        if (texture_id == 99) {
            return world_map->doors_amount_open[tile_index] < 100;
        } else if(texture_id != 0)
            return true;
        else
            return false;
    }

    bool move_if_valid(const point2& proposed_loc) {
        if (world_map->within_map(proposed_loc)) {
            if (!collision(proposed_loc))
                player_loc = proposed_loc;
        }
        return true;
    }

public:
    double fov;

private:
    point2 player_loc;
    vec2 view_dir;
    vec2 left;
    vec2 right;
    std::vector<double> angles;
    std::vector<double> cosines;
    map* world_map;
};