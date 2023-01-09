/*
 * csv.h:
 *
 * Various functions (and helper functions) to parse csv files. Used for parsing the map csv file.
 *
 */

#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <cmath>

// Converts an std::vector of integers containing digits to an int. For ex {1,3,4} --> 134
int digits_to_int(std::vector<int>& digits) {
    if (digits.size() == 0)  // If temp_digits array is empty, that means a field in the csv file was left blank, indicating an empty block (i.e. 0 in the map array)
        return 0;
    else {
        int final_num = 0;
        for (int i = 0; i < digits.size(); ++i)
            final_num += digits[i] * pow(10, digits.size() - 1 - i);
        return final_num;
    }
}

// Container for the data extracted from a csv file. std::vector contains the actual data.
struct csv_data {
    csv_data(): columns(0), rows(0) {}

    std::vector<int> data;   // Grid of numbers stored in csv file
    int columns;               // Width of grid
    int rows;              // Height of grid
};

// Pushes digits stored in temp_digits csv_data struct
void push_digit(bool& num_in_progress, csv_data& csvData, std::vector<int>& temp_digits) {
    num_in_progress = false;
    csvData.data.push_back(digits_to_int(temp_digits));
    temp_digits.clear();
}

// Parses a csv file and outputs a csv_data struct, containing the data in the file + number of columns and rows
csv_data parse_csv(std::string filename) {
    csv_data csvData;

    std::ifstream map_file(filename);

    bool num_in_progress = false;
    std::vector<int> temp_digits;

    while (map_file.is_open()) {
        char c = map_file.get();
        bool comma = (c == ',');
        bool newline = (c == '\r') || (c == '\n');
        bool eof = c == EOF;

        if (comma) {
            push_digit(num_in_progress, csvData, temp_digits);
        } else if (newline) {
            if (num_in_progress == true) {
                ++csvData.rows;
                push_digit(num_in_progress, csvData, temp_digits);
            } else
                continue;
        } else if (eof) {
            ++csvData.rows;
            push_digit(num_in_progress, csvData, temp_digits);
            map_file.close();
        } else if (num_in_progress == true) {
            temp_digits.push_back(c - '0');
        } else {
            num_in_progress = true;
            temp_digits.push_back(c - '0');
            if (csvData.rows == 0) ++csvData.columns;
        }
    }

    return csvData;
};