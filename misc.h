#pragma once

#include <vector>
#include <string>
#include <algorithm>

double pi = 3.1415926535897932385;

int nearest_int(const double& a, const int& dir) {
    return dir > 0 ? ceil(a) : floor(a);
}

double degrees_to_radians(double degrees) {
    return degrees*pi/180;
}

double min(const double& a, const double& b) {
    return a < b ? a : b;
}

template<class point_type, class limit_type>
bool within_bounds(point_type tile, limit_type x_lim1, limit_type x_lim2, limit_type y_lim1, limit_type y_lim2) {
    if((tile.x() >= x_lim1 && tile.x() <= x_lim2) && (tile.y() >= y_lim1 && tile.y() <= y_lim2))
        return true;
    else
        return false;
}

bool is_integer(double d) {
    if (d - static_cast<int>(d) != 0.0)
        return false;
    else
        return true;
}

int digits_vec_to_int(std::vector<int>& arr) {
    if (arr.size() == 0)  // If digits array is empty, that means a field in the csv file was left blank, indicating an empty block (i.e. 0 in the map array)
        return 0;
    else {
        int final_num = 0;
        for (int i = 0; i < arr.size(); ++i)
            final_num += arr[i]*pow(10, arr.size()-1-i);
        return final_num;
    }
}

std::string double_to_string(const double& d, int precision) {
    std::string initial_str = std::to_string(d);
    return initial_str.substr(0, initial_str.find(".") + precision + 1);
}

bool in_vec(const std::vector<int>& vec, const int& num) {
    return (std::find(vec.begin(), vec.end(), num) != vec.end());
}