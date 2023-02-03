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
#include <utility>
#include <optional>

class MapFile {
public:
    MapFile(const std::string& filename): rows(0), columns(0), numCells(0) {
        parse(filename);
    }

private:
    void parse(const std::string& filename) {
        std::ifstream map_file(filename);

        bool num_in_progress = false;
        std::string curr_cell_value;

        int tileIndex = 0;
        while (map_file.is_open()) {
            char c = map_file.get();

            bool comma = (c == ',');
            bool newline = (c == '\r') || (c == '\n');
            bool eof = c == EOF;

            if (comma) {
                push_val(num_in_progress, tileIndex, curr_cell_value);
            } else if (newline) {
                if (num_in_progress == true) {
                    ++rows;
                    push_val(num_in_progress, tileIndex, curr_cell_value);
                } else
                    continue;
            } else if (eof) {
                ++rows;
                push_val(num_in_progress, tileIndex, curr_cell_value);
                map_file.close();
            } else if (num_in_progress == true) {
                curr_cell_value.push_back(c);
            } else {
                num_in_progress = true;
                curr_cell_value.push_back(c);
                if (rows == 0) ++columns;
            }
        }
        numCells = rows * columns;
    };

    void push_val(bool& num_in_progress, int& index, std::string& cell_value) {
        num_in_progress = false;
        tiles.push_back({index, parse_tile_code(cell_value)});
        cell_value.clear();
        ++index;
    }

    typedef std::pair<textureType_t, int> tileTextureInfo_t;

    std::optional<tileTextureInfo_t> parse_tile_code(const std::string& texture_code) const {
        if (texture_code == "")
            return {};  // empty tile
        else {
            std::string type_code = texture_code.substr(0, texture_code.find("-"));
            int id = std::stoi(texture_code.substr(texture_code.find("-") + 1));

            if (type_code == "W")
                return std::pair(textureType_t::WALLS, id);
            else if (type_code == "O")
                return std::pair(textureType_t::OBJECTS, id);
            else if (type_code == "G1")
                return std::pair(textureType_t::GUARD, id);
            else
                return {};  // invalid format ; default to empty tile
        }
    }

public:
    std::vector<std::pair<int, std::optional<tileTextureInfo_t>>> tiles;
    int columns;
    int rows;
    int numCells;
};