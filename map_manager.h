/*
 * map_manager.h:
 *
 * Parses map.csv file and loads it into memory, with tiles and textures.
 *
 */

#pragma once

#include <string>
#include "game_data.h"
#include "csv.h"
#include "global.h"

class map_manager {
public:

    void init(game_data* _game_data) {
        GameData = _game_data;
        
        gate_texture = GameData->Multimedia.get_wall_texture_pair(99);
        gate_sidewall_texture = GameData->Multimedia.get_wall_texture_pair(101);
        guard = texture_pair(GameData->Multimedia.get_texture(GUARD, 1), GameData->Multimedia.get_texture(GUARD, 1));
        tables = texture_pair(GameData->Multimedia.get_texture(OBJECTS, 5), GameData->Multimedia.get_texture(OBJECTS, 5));
        light = texture_pair(GameData->Multimedia.get_texture(OBJECTS, 17), GameData->Multimedia.get_texture(OBJECTS, 17));

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
        GameData->Map.width = CsvData.columns;
        GameData->Map.height = CsvData.rows;
        GameData->Map.num_tiles = CsvData.columns * CsvData.rows;

        // Allocate and fill map with corresponding tiles
        GameData->Map.tiles = new tile*[GameData->Map.num_tiles];
        for (int tile_index = 0; tile_index < GameData->Map.num_tiles; ++tile_index) {
            int texture_id = CsvData.data[tile_index];
            point2 center = GameData->Map.get_center(tile_index);
            
            if (texture_id == 0)
                GameData->Map.tiles[tile_index] = new empty(center);
            else if (texture_id == 99)
                GameData->Map.tiles[tile_index] = new door(center, gate_texture, gate_sidewall_texture);
            else if (texture_id == 6969) {
                sprite* s = new sprite(center, tables);
                GameData->Map.tiles[tile_index] = s;
                GameData->Map.sprites.push_back(s);
            } else if (texture_id == 1000) {
                sprite* s = new sprite(center, guard);
                GameData->Map.tiles[tile_index] = s;
                GameData->Map.sprites.push_back(s);
            } else if (texture_id == 1001) {
                sprite* s = new sprite(center, light);
                GameData->Map.tiles[tile_index] = s;
                GameData->Map.sprites.push_back(s);
            } else
                GameData->Map.tiles[tile_index] = new wall(center, GameData->Multimedia.get_wall_texture_pair(texture_id), gate_sidewall_texture);
        }
    }
    
    void update_sprite_perp_lines() {
        for (sprite* s : GameData->Map.sprites)
            s->calculate_perp_line(GameData->Player.view_dir);
    }

private:
    game_data* GameData;

    texture_pair guard;
    texture_pair tables;
    texture_pair light;
    texture_pair gate_texture;
    texture_pair gate_sidewall_texture;
};
