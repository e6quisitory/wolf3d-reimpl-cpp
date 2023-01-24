/*
 * map_file.h:
 *
 * Takes in a csv map file and parses each tile into a texture type and texture ID.
 * The map manager takes said parsed data and fills GameData->Map tiles accordingly,
 * with correct tile types and textures.
 *
 */

#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <optional>

class map_file {
public:
    map_file(const std::string& filename): rows(0), columns(0), num_cells(0) {
        parse(filename);
    }

private:
    void parse(const std::string& filename) {
        std::ifstream map_file(filename);

        bool num_in_progress = false;
        std::string curr_cell_value;

        while (map_file.is_open()) {
            char c = map_file.get();
            bool comma = (c == ',');
            bool newline = (c == '\r') || (c == '\n');
            bool eof = c == EOF;

            if (comma) {
                push_val(num_in_progress, curr_cell_value);
            } else if (newline) {
                if (num_in_progress == true) {
                    ++rows;
                    push_val(num_in_progress, curr_cell_value);
                } else
                    continue;
            } else if (eof) {
                ++rows;
                push_val(num_in_progress, curr_cell_value);
                map_file.close();
            } else if (num_in_progress == true) {
                curr_cell_value.push_back(c);
            } else {
                num_in_progress = true;
                curr_cell_value.push_back(c);
                if (rows == 0) ++columns;
            }
        }
        num_cells = rows*columns;
    };

    void push_val(bool& num_in_progress, std::string& cell_value) {
        num_in_progress = false;
        data.push_back(parse_tile_code(cell_value));
        cell_value.clear();
    }

    typedef std::pair<TEXTURE_TYPE, int> tile_info;

    std::optional<tile_info> parse_tile_code(const std::string& texture_code) const {
        if (texture_code == "")
            return {};  // empty tile
        else {
            std::string type_code = texture_code.substr(0, texture_code.find("-"));
            int id = std::stoi(texture_code.substr(texture_code.find("-") + 1));

            if (type_code == "W")
                return std::pair(TEXTURE_WALLS, id);
            else if (type_code == "O")
                return std::pair(TEXTURE_OBJECTS, id);
            else if (type_code == "G1")
                return std::pair(TEXTURE_GUARD, id);
            else
                return {};  // invalid format ; default to empty tile
        }
    }

public:
    std::vector<std::optional<tile_info>> data;
    int columns;
    int rows;
    int num_cells;
};