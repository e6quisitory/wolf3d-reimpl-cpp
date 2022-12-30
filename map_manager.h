#pragma once

#include <string>
#include "game_data.h"
#include "csv.h"

class map_manager {
public:

    void init(game_data* gm_dat) {
        GameData = gm_dat;

        door_gate_lit = GameData->Multimedia.get_texture(99);
        door_gate_unlit = GameData->Multimedia.get_texture(100);
        door_sidewall_lit = GameData->Multimedia.get_texture(101);
        door_sidewall_unlit = GameData->Multimedia.get_texture(102);
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
                GameData->Map.tiles[tile_index] = new door(door_gate_lit, door_gate_unlit, door_sidewall_lit, door_sidewall_unlit);
            } else {
                SDL_Texture* wall_texture_lit = GameData->Multimedia.get_texture(texture_id);
                if (!has_lighting(texture_id))
                    GameData->Map.tiles[tile_index] = new wall(wall_texture_lit, wall_texture_lit, door_sidewall_lit, door_sidewall_unlit);
                else {
                    SDL_Texture* wall_texture_unlit = GameData->Multimedia.get_texture(texture_id+1);
                    GameData->Map.tiles[tile_index] = new wall(wall_texture_lit, wall_texture_unlit, door_sidewall_lit, door_sidewall_unlit);
                }
            }
        }

    }

private:
    game_data* GameData;

    SDL_Texture* door_gate_lit;
    SDL_Texture* door_gate_unlit;
    SDL_Texture* door_sidewall_lit;
    SDL_Texture* door_sidewall_unlit;

    int no_lighting_list[10] = {31,32,41,42,43,44,107,108,109,110};
    bool has_lighting(int texture_id) const {
        for (int i = 0; i < 10; ++i)
            if (no_lighting_list[i] == texture_id)
                return false;

        return true;
    }
};