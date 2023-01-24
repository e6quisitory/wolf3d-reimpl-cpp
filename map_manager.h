/*
 * map_manager.h:
 *
 * Parses map.csv file and loads it into memory, with tiles and textures.
 *
 */

#pragma once

#include <string>

#include "game_data.h"
#include "map_file.h"
#include "global.h"

class map_manager {
public:

    void init(game_data* _game_data) {
        GameData = _game_data;
        
        gate_texture = GameData->Multimedia.get_wall_texture_pair(99);
        gate_sidewall_texture = GameData->Multimedia.get_wall_texture_pair(101);
        guard = texture_pair(GameData->Multimedia.get_texture(TEXTURE_GUARD, 1), GameData->Multimedia.get_texture(TEXTURE_GUARD, 1));
        tables = texture_pair(GameData->Multimedia.get_texture(TEXTURE_OBJECTS, 5), GameData->Multimedia.get_texture(TEXTURE_OBJECTS, 5));
        light = texture_pair(GameData->Multimedia.get_texture(TEXTURE_OBJECTS, 17), GameData->Multimedia.get_texture(TEXTURE_OBJECTS, 17));

    }

    void exit() const {
        for (int i = 0; i < GameData->Map.num_tiles; ++i)
            delete GameData->Map.tiles[i];
        delete[] GameData->Map.tiles;
    }

    void load_map(const std::string& file) const {
        map_file MapFile(file);

        GameData->Map.width = MapFile.columns;
        GameData->Map.height = MapFile.rows;
        GameData->Map.num_tiles = MapFile.num_cells;

        // Allocate and fill map with corresponding tiles
        GameData->Map.tiles = new tile*[MapFile.num_cells];
        for (int tile_index = 0; tile_index < MapFile.num_cells; ++tile_index) {
            auto tile_info = MapFile.data[tile_index];

            if (tile_info.has_value() == false)
                GameData->Map.tiles[tile_index] = new empty();
            else {
                auto texture_info = tile_info.value();
                auto [TextureType, TextureID] = tile_info.value();
                switch (TextureType) {
                    case TEXTURE_WALLS:
                        if (TextureID == 99)
                            GameData->Map.tiles[tile_index] = new door(gate_texture, gate_sidewall_texture);
                        else
                            GameData->Map.tiles[tile_index] = new wall(
                                    GameData->Multimedia.get_texture_pair(texture_info), gate_sidewall_texture);
                        break;
                    case TEXTURE_OBJECTS:
                    case TEXTURE_GUARD:
                        point2 tile_center_pt = get_tile_center_pt(tile_index, MapFile.columns);
                        sprite *s = new sprite(tile_center_pt, GameData->Multimedia.get_texture_pair(texture_info));
                        GameData->Map.tiles[tile_index] = s;
                        GameData->Map.sprites.push_back(s);
                        break;
                }
            }
        }
    }
    
    void update_sprite_perp_lines() {
        for (sprite* s : GameData->Map.sprites)
            s->calculate_perp_line(GameData->Player.view_dir);
    }

private:
    point2 get_tile_center_pt(const int& index, const int& map_width) const {
        int x = index % map_width;
        int y = index / map_width;
        return point2(x + 0.5, y + 0.5);
    }
    
private:
    game_data* GameData;

    texture_pair guard;
    texture_pair tables;
    texture_pair light;
    texture_pair gate_texture;
    texture_pair gate_sidewall_texture;
};
