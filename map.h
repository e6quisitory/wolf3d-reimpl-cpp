#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

    int num_cells() const {
        return width*height;
    }

    void print_map() const {
        std::cout << "Width = " << width << ", Height = " << height << std::endl;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j)
                std::cout << tiles[i*width + j] << " ";
            std::cout << std::endl;
        }
    }

    double hit(const ray& r) {
        std::vector<vec2> x_ints, y_ints;
        x_ints.reserve(width);
        y_ints.reserve(height);

        for (int x = r.near_x(); (r.x_dir() == 1) ? x < width : x >= 0; x += r.x_dir()) {
            if (r.y_at(x) >= 0 && r.y_at(x) < height) {
                vec2 curr_pt(x, r.y_at(x));
                if ((*this)(static_cast<int>(curr_pt.x()), static_cast<int>(curr_pt.y())) == 1)
                    x_ints.push_back(curr_pt);
            }
        }

        for (int y = r.near_y(); (r.y_dir() == 1) ? y < height : y >= 0; y += r.y_dir()) {
            if (r.x_at(y) >= 0 && r.x_at(y) < width) {
                vec2 curr_pt(r.x_at(y), y);
                if ((*this)(static_cast<int>(curr_pt.x()), static_cast<int>(curr_pt.y())) == 1)
                    y_ints.push_back(curr_pt);
            }
        }

        if (x_ints.size() == 0 && y_ints.size() == 0)
            return -1;
        else {
            double t = DBL_MAX;
            for (int i = 0; i < x_ints.size(); ++i)
                t = r.get_t(x_ints[i]) < t ? r.get_t(x_ints[i]) : t;
            for (int i = 0; i < y_ints.size(); ++i)
                t = r.get_t(y_ints[i]) < t ? r.get_t(y_ints[i]) : t;
            return t;
        }
    }

public:
    int* tiles;
    int width;
    int height;
};