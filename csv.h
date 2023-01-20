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
#include <optional>

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

    std::vector<std::optional<std::string>> data;
    int columns;
    int rows;
};

// Pushes digits stored in temp_digits csv_data struct
void push_val(bool& num_in_progress, csv_data& csvData, std::string& cell_value) {
    num_in_progress = false;

    if (cell_value == "")
        csvData.data.push_back({});
    else
        csvData.data.push_back(cell_value);

    cell_value.clear();
}

// Parses a csv file and outputs a csv_data struct, containing the data in the file + number of columns and rows
csv_data parse_csv(std::string filename) {
    csv_data csvData;

    std::ifstream map_file(filename);

    bool num_in_progress = false;
    std::string curr_cell_value;

    while (map_file.is_open()) {
        char c = map_file.get();
        bool comma = (c == ',');
        bool newline = (c == '\r') || (c == '\n');
        bool eof = c == EOF;

        if (comma) {
            push_val(num_in_progress, csvData, curr_cell_value);
        } else if (newline) {
            if (num_in_progress == true) {
                ++csvData.rows;
                push_val(num_in_progress, csvData, curr_cell_value);
            } else
                continue;
        } else if (eof) {
            ++csvData.rows;
            push_val(num_in_progress, csvData, curr_cell_value);
            map_file.close();
        } else if (num_in_progress == true) {
            curr_cell_value.push_back(c);
        } else {
            num_in_progress = true;
            curr_cell_value.push_back(c);
            if (csvData.rows == 0) ++csvData.columns;
        }
    }

    return csvData;
};