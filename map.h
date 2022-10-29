#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "vec2.h"
#include "ivec2.h"
#include "misc.h"

typedef struct hit_info {
    hit_info(const double& d, const char& wall): dist(d), wall_type(wall) { hit = true; }
    hit_info(bool hit_occur): hit(hit_occur) {}

    bool hit;
    double dist;
    char wall_type;

} hit_info;

class map {
public:
    map(std::string filename) {
        std::ifstream map_file(filename);

        width = 0;
        height = 0;
        std::vector<int> raw_nums;

        while (map_file.is_open()) {
            char c = map_file.get();
            if (c == '\n') {
                ++height;
                continue;
            } else if (c == EOF) {
                ++height;
                map_file.close();
                continue;
            } else if (height == 0)
                ++width;

            raw_nums.push_back(c - '0');
        }

        tiles = new int[width*height];
        for (int i = 0; i < width*height; ++i)
            tiles[i] = raw_nums[i];
    }

    ~map() {
        delete[] tiles;
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

    bool check_tile(const ipoint2& tile) const {
        if (!within_map(tile))
            return false;
        else
            return (*this)(tile.x(), tile.y()) == 1 ? true : false;
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

    char wall_type(const point2& coord) const {
        // v for vertical wall, h for horizontal wall, c for corner
        if (is_integer(coord.x()) && !is_integer(coord.y()))
            return 'v';
        else if (!is_integer(coord.x()) && is_integer(coord.y()))
            return 'h';
        else
            return 'c';

    }

    hit_info hit(const ray& r) const {
        point2 ray_pt = r.origin;
        ipoint2 tile_pt = get_tile(r.origin);

        while (within_map(tile_pt)) {
            // Calculate next x and y intersections of ray
            point2 next_x_inter(r.near_x(ray_pt), r.y_at(r.near_x(ray_pt)));
            point2 next_y_inter(r.x_at(r.near_y(ray_pt)), r.near_y(ray_pt));
            double x_dist = r.get_ray_dist_dx(next_x_inter.x() - ray_pt.x());
            double y_dist = r.get_ray_dist_dy(next_y_inter.y() - ray_pt.y());

            // Deduce which tile ray intersects next based on comparison b/w ray distance to each intersection
            if (x_dist < y_dist) {
                tile_pt.x() += r.x_dir();
                ray_pt = next_x_inter;
            } else if (y_dist < x_dist) {
                tile_pt.y() += r.y_dir();
                ray_pt = next_y_inter;
            } else {
                tile_pt.x() += r.x_dir();
                tile_pt.y() += r.y_dir();
                ray_pt = point2(tile_pt.x(), tile_pt.y());
            }

            // Check tile to see if there's a box/wall there on the map
            if (check_tile(tile_pt)) {
                return hit_info(r.dist_to_pt(ray_pt), wall_type(ray_pt));
            }
        }
        return hit_info(false);
    }

public:
    int* tiles;
    int width;
    int height;
};