#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "vec2.h"
#include "ivec2.h"
#include "misc.h"

typedef struct hit_info {
    hit_info(const double& d, const double& w_percent, const int& text_id, const int& arr_ind):
        dist(d), width_percent(w_percent), texture_id(text_id), arr_index(arr_ind) { hit = true; }
    hit_info(bool hit_occur): hit(hit_occur) {}

    bool hit;
    double dist;
    double width_percent;
    int texture_id;
    int arr_index;

} hit_info;

typedef struct intersection_info {
    intersection_info(const point2& ray_pt, const ipoint2& tile_pt, const double& wp): pt(ray_pt), tile(tile_pt), width_percent(wp) {
        // v for vertical wall, h for horizontal wall, c for corner
        if (is_integer(ray_pt.x()) && !is_integer(ray_pt.y()))
            wall_type = 'v';
        else if (!is_integer(ray_pt.x()) && is_integer(ray_pt.y()))
            wall_type = 'h';
        else
            wall_type = 'c';
    }
    point2 pt;
    ipoint2 tile;
    char wall_type;
    double width_percent;
} intersection_info;

class map {
public:
    map(std::string filename) {
        std::ifstream map_file(filename);

        width = 0;
        height = 0;
        std::vector<int> raw_nums;

        bool num_in_progress = false;
        std::vector<int> digits;
        while (map_file.is_open()) {
            char c = map_file.get();
            bool comma = (c == ',');
            bool newline = (c == '\r') || (c == '\n');

            if (comma) {
                num_in_progress = false;
                raw_nums.push_back(digits_vec_to_int(digits));
                digits.clear();
            } else if (newline) {
                if (num_in_progress == true) {
                    ++height;
                    num_in_progress = false;
                    raw_nums.push_back(digits_vec_to_int(digits));
                    digits.clear();
                } else
                    continue;
            } else if (c == EOF) {
                ++height;
                num_in_progress = false;
                raw_nums.push_back(digits_vec_to_int(digits));
                digits.clear();
                map_file.close();
            } else if (num_in_progress == true) {
                digits.push_back(c - '0');

            } else {
                num_in_progress = true;
                digits.push_back(c - '0');
                if (height == 0) ++width;
            }
        }

        tiles = new int[width*height];
        doors_amount_open = new int[width*height];

        for (int i = 0; i < width*height; ++i) {
            tiles[i] = raw_nums[i];
            if (tiles[i] == 99) {   // 99 is texture ID for a door
                doors_amount_open[i] = 0;  // 100 is fully open, 0 is fully closed
            } else {
                doors_amount_open[i] = -1; // -1 indicates there is no door at that tile
            }
        }
    }

    map() {}

    ~map() {
        delete[] tiles;
        delete[] doors_amount_open;
    }

    int& operator [] (int index) {   // If operator is called on non-const object, allow modification, hence return by reference.
        return tiles[index];
    }

    int operator [] (int index) const {   // If operator is called on const object, do not allow modification, hence return by value.
        return tiles[index];
    }

    int& operator () (int x, int y) {
        return tiles[y*width + x];
    }

    int operator () (int x, int y) const {
        return tiles[y*width + x];
    }

    void print_map() const {
        std::cout << "Width = " << width << ", Height = " << height << std::endl;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j)
                std::cout << tiles[i*width + j] << " ";
            std::cout << std::endl;
        }
    }

    ivec2 get_tile(vec2 pt) const {
        return ivec2(static_cast<int>(pt.x()), static_cast<int>(pt.y()));
    }

    int tile_to_index(ivec2 pt) const {
        return pt.y()*width + pt.x();
    }

    int get_texture_id(const ipoint2& tile) const {
        if (!within_map(tile))
            return false;
        else
            return (*this)(tile.x(), tile.y());
    }

    bool within_map(const point2& pt) const {
        if (within_bounds<point2, double>(pt, 0, width, 0, height))
            return true;
        else
            return false;
    }

    bool within_map(const ipoint2& pt) const {
        if (within_bounds<ipoint2, int>(pt, 0, width-1, 0, height-1))
            return true;
        else
            return false;
    }

    double point_width_percent(const point2& hitpoint) const {
        bool x_is_int = is_integer(hitpoint.x());
        bool y_is_int = is_integer(hitpoint.y());
        double x_dec = hitpoint.x() - int(hitpoint.x());
        double y_dec = hitpoint.y() - int(hitpoint.y());

        if ((x_is_int || x_dec == 0.5) && !y_is_int)
            return y_dec;
        else if ((y_is_int || y_dec == 0.5) && !x_is_int)
            return x_dec;
        else
            return 0.0;
    }

    hit_info hit(const ray& r) const {
        point2 ray_pt = r.origin;
        ipoint2 tile_pt = get_tile(r.origin);
        bool inside_door = get_texture_id(get_tile(r.origin)) == 99;

        while (within_map(tile_pt)) {
            intersection_info curr = next_intersection(r, ray_pt, tile_pt);
            ray_pt = curr.pt;
            tile_pt = curr.tile;

            int texture_id = get_texture_id(tile_pt);

            if (texture_id == 99) {
                intersection_info next = next_intersection(r, ray_pt, tile_pt);
                bool pass_through = ray_passthrough_door(center_door_pt(r, curr), curr.tile);

                bool vh = curr.wall_type == 'v' && next.wall_type == 'h';
                bool hv = curr.wall_type == 'h' && next.wall_type == 'v';
                bool vh_side = r.direction.x() > 0 ? (next.width_percent < 0.5 || pass_through) : (next.width_percent > 0.5 || pass_through);
                bool hv_side = r.direction.y() > 0 ? (next.width_percent < 0.5 || pass_through) : (next.width_percent > 0.5 || pass_through);

                if ((vh && vh_side) || (hv && hv_side)) {
                    texture_id = 101;
                    ray_pt = next.pt;
                    tile_pt = next.tile;
                } else if (pass_through)
                    continue;
                else
                    ray_pt = center_door_pt(r, curr);

            } else if (inside_door) {
                ipoint2 standing_tile = get_tile(r.origin);

                bool x_close = abs(curr.tile.x()-standing_tile.x()) == 1;
                bool y_close = abs(curr.tile.y()-standing_tile.y()) == 1;
                bool x_same = curr.tile.x() == standing_tile.x();
                bool y_same = curr.tile.y() == standing_tile.y();
                bool close = (x_close && y_same) || (y_close && x_same);

                if (close && texture_id != 0)
                    texture_id = 101;
            }

            if (texture_id != 0) {
                return hit_info(r.dist_to_pt(ray_pt), point_width_percent(ray_pt), texture_id,
                                tile_to_index(tile_pt));
            } else
                continue;
        }

        return hit_info(false);
    }

    bool doors_opening() const {
        return doors_currently_opening.size() != 0;
    }

    void set_tile(int tile_index, int new_texture) {
        tiles[tile_index] = new_texture;
    }

    bool ray_passthrough_door(const point2& pt, const ipoint2& tile) const {
        return 100.0*point_width_percent(pt) < doors_amount_open[tile_to_index(tile)];
    }

    bool door_currently_opening(int index_num) {
        for (int door_index : doors_currently_opening) {
            if (door_index == index_num) return true;
        }
        return false;
    }

    point2 center_door_pt(const ray& r, intersection_info curr) const {
        if (curr.wall_type == 'v') {
            curr.pt.x() += r.x_dir()/2;
            curr.pt.y() += r.y_dir()*(r.y_step/2);
        } else {
            curr.pt.y() += r.y_dir()/2;
            curr.pt.x() += r.x_dir()*(r.x_step/2);
        }
        return curr.pt;
    }

private:
    intersection_info next_intersection(const ray& r, const point2& curr_pt, const ipoint2& curr_tile) const {
        point2 next_x_inter(r.near_x(curr_pt), r.y_at(r.near_x(curr_pt)));
        point2 next_y_inter(r.x_at(r.near_y(curr_pt)), r.near_y(curr_pt));
        double x_dist = r.get_ray_dist_dx(next_x_inter.x() - curr_pt.x());
        double y_dist = r.get_ray_dist_dy(next_y_inter.y() - curr_pt.y());

        if (x_dist < y_dist)
            return intersection_info(next_x_inter, ipoint2(curr_tile.x() + r.x_dir(), curr_tile.y()),
                                     point_width_percent(next_x_inter));
        else if (y_dist < x_dist)
            return intersection_info(next_y_inter, ipoint2(curr_tile.x(), curr_tile.y() + r.y_dir()),
                                     point_width_percent(next_y_inter));
        else {
            ipoint2 next_tile = ipoint2(curr_tile.x() + r.x_dir(), curr_tile.y() + r.y_dir());
            point2 next_pt = point2(next_tile.x(), next_tile.y());
            return intersection_info(next_pt, next_tile, point_width_percent(next_pt));
        }
    }


public:
    std::vector<int> doors_currently_opening;
    int* doors_amount_open;

private:
    int* tiles;
    int width;
    int height;
};