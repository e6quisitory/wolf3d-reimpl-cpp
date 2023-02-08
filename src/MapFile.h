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

struct parsedTileInfo_t {
    int            index;
    textureType_t  textureType;
    int            textureID;
};

typedef std::optional<parsedTileInfo_t> parsedTileInfo_o;

typedef std::pair<textureType_t, int> parsedTextureInfo_t;
typedef std::optional<parsedTextureInfo_t> parsedTextureInfo_o;

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

    void push_val(bool& num_in_progress, int& tileIndex, std::string& cell_value) {
        num_in_progress = false;
        if (cell_value.empty())
            tiles.emplace_back(std::nullopt);
        else {
            auto parsedTextureInfo = parse_tile_code(cell_value);
            if (parsedTextureInfo.has_value()) {
                auto [textureType, textureID] = parsedTextureInfo.value();
                parsedTileInfo_t currTileInfo = {tileIndex, textureType, textureID};
                tiles.emplace_back(currTileInfo);
            } else
                tiles.emplace_back(std::nullopt);  // invalid format ; default to empty tile

            cell_value.clear();
        }
        ++tileIndex;
    }

    parsedTextureInfo_o parse_tile_code(const std::string& texture_code) const {
        std::string type_code = texture_code.substr(0, texture_code.find("-"));
        int id = std::stoi(texture_code.substr(texture_code.find("-") + 1));

        if (type_code == "W")
            return std::pair(textureType_t::WALLS, id);
        else if (type_code == "O")
            return std::pair(textureType_t::OBJECTS, id);
        else if (type_code == "G1")
            return std::pair(textureType_t::GUARD, id);
        else
            return std::nullopt;  // invalid format ; default to empty tile
    }

public:
    std::vector<parsedTileInfo_o> tiles;
    int columns;
    int rows;
    int numCells;
};