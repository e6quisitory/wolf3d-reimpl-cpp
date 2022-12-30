#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <cmath>

int digits_vec_to_int(std::vector<int>& arr) {
    if (arr.size() == 0)  // If temp_digits array is empty, that means a field in the csv file was left blank, indicating an empty block (i.e. 0 in the map array)
        return 0;
    else {
        int final_num = 0;
        for (int i = 0; i < arr.size(); ++i)
            final_num += arr[i]*pow(10, arr.size()-1-i);
        return final_num;
    }
}

struct csv_data {
    csv_data(): width(0), height(0) {}

    std::vector<int> data;   // Grid of numbers stored in csv file
    int width;               // Width of grid
    int height;              // Height of grid
};

csv_data parse_csv(std::string filename) {
    csv_data csv_d;

    std::ifstream map_file(filename);

    bool num_in_progress = false;
    std::vector<int> temp_digits;

    while (map_file.is_open()) {
        char c = map_file.get();
        bool comma = (c == ',');
        bool newline = (c == '\r') || (c == '\n');

        if (comma) {
            num_in_progress = false;
            csv_d.data.push_back(digits_vec_to_int(temp_digits));
            temp_digits.clear();
        } else if (newline) {
            if (num_in_progress == true) {
                ++csv_d.height;
                num_in_progress = false;
                csv_d.data.push_back(digits_vec_to_int(temp_digits));
                temp_digits.clear();
            } else
                continue;
        } else if (c == EOF) {
            ++csv_d.height;
            num_in_progress = false;
            csv_d.data.push_back(digits_vec_to_int(temp_digits));
            temp_digits.clear();
            map_file.close();
        } else if (num_in_progress == true) {
            temp_digits.push_back(c - '0');
        } else {
            num_in_progress = true;
            temp_digits.push_back(c - '0');
            if (csv_d.height == 0) ++csv_d.width;
        }
    }

    return csv_d;
};