#pragma once

#include <string>
#include "game_data.h"
#include "csv.h"
#include "global.h"

class map_manager {
public:

    void init(game_data* gm_dat) {
        GameData = gm_dat;

        gate_texture = GameData->Multimedia.get_texture_pair(99);
        gate_sidewall_texture = GameData->Multimedia.get_texture_pair(101);
    }

    void exit() const {
        for (int i = 0; i < GameData->Map.num_tiles; ++i)
            delete GameData->Map.tiles[i];
        delete[] GameData->Map.tiles;
    }

    void load_map(std::string map_file) const {
        // Parse map csv file
        csv_data CsvData = parse_csv(map_file);

        // Store width, height, num_tiles from parser results
        GameData->Map.width = CsvData.width;
        GameData->Map.height = CsvData.height;
        GameData->Map.num_tiles = CsvData.width * CsvData.height;

        // Fill map with corresponding tiles
        GameData->Map.tiles = new tile*[GameData->Map.num_tiles];
        for (int tile_index = 0; tile_index < GameData->Map.num_tiles; ++tile_index) {
            int texture_id = CsvData.data[tile_index];

            if (texture_id == 0)
                GameData->Map.tiles[tile_index] = new empty();
            else if (texture_id == 99) {
                GameData->Map.tiles[tile_index] = new door(gate_texture, gate_sidewall_texture);
            } else
                GameData->Map.tiles[tile_index] = new wall(GameData->Multimedia.get_texture_pair(texture_id), gate_sidewall_texture);
        }
    }

private:
    game_data* GameData;

    texture_pair gate_texture;
    texture_pair gate_sidewall_texture;
};