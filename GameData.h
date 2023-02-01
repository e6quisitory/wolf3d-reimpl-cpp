/*
 * game_data.h:
 *
 * Houses game_data struct, which contains all of the static, "behind the scenes" data of the game world at any point in time.
 * This data, stored in memory, is accessed and modified by the various managers.
 * Ultimately, the game_data struct is passed into the renderer.
 * The renderer turns this data representation of the game world into a graphical representation, to be outputted to the screen.
 *
 */

#pragma once

#include <vector>
#include <map>

#include "vec2.h"
#include "Tile.h"

/*
================================
 Player
================================
*/

struct player {
    Point2 location;
    Vec2 viewDir;
    Vec2 east;
};

/*
================================
 Map
================================
*/

struct map {
    Tile* operator [] (const int& index) const {
        return tiles[index];
    }

    Tile* operator () (const int& x, const int& y) const {
        return tiles[y*width + x];
    }

    Tile* GetTile(const iPoint2& ipt) const {
        return (*this)(ipt.x(), ipt.y());
    }

    bool within_map(const iPoint2& pt) {
        if((pt.x() >= 0 && pt.x() <= width) && (pt.y() >= 0 && pt.y() <= height))
            return true;
        else
            return false;
    }

    void add_active_door(DoorTile* const _door) {
        active_doors.insert({_door, _door});
    }

    void remove_active_door(DoorTile* const _door) {
        active_doors.erase(_door);
    }

    Tile** tiles;  // Tiles need to be stored as pointers for polymorphism to work
    int width;
    int height;
    int numTiles;

    /* Doors */
    std::map<DoorTile*, DoorTile*> active_doors;
    bool anyDoorsAwaitingRendering;
    
    /* Sprites */
    std::vector<SpriteTile*> sprites;
};

/*
================================
 Multimedia (game assets)
================================
*/

enum TEXTURE_TYPE {
    TEXTURE_WALLS,
    TEXTURE_GUARD,
    TEXTURE_OBJECTS,
    TEXTURE_WEAPONS,
};

#define NUM_TEXTURE_TYPES 4

struct texture {
    texture(SDL_Texture* const texture, const TEXTURE_TYPE& texture_type, const int& texture_id):
        Texture(texture), TextureType(texture_type), TextureID(texture_id) {}

    SDL_Texture* Texture;
    TEXTURE_TYPE TextureType;
    int TextureID;
};

struct multimedia {
    SDL_Texture* get_texture(const TEXTURE_TYPE& _texture_type, const int& texture_id) const {
        return textures.at(_texture_type)[texture_id-1];
    }

    void add_texture(const TEXTURE_TYPE& _texture_type, SDL_Texture* const texture) {
        textures[_texture_type].push_back(texture);
    }

    texturePair_t get_texture_pair(const TEXTURE_TYPE& _texture_type, const int& texture_id) const {
        if (_texture_type == TEXTURE_WALLS)
            return get_wall_texture_pair(texture_id);
        else {
            SDL_Texture* t = get_texture(_texture_type, texture_id);
            return texturePair_t(t, t);
        }
    }

    texturePair_t get_texture_pair(const std::pair<TEXTURE_TYPE, int>& texture_info) const {
        auto& [TextureType, TextureID] = texture_info;
        return get_texture_pair(TextureType, TextureID);
    }

    texturePair_t get_wall_texture_pair(int texture_id) const {
        static int no_lighting_list[10] = {31,32,41,42,43,44,107,108,109,110};
        for (int i = 0; i < 10; ++i) {
            if (texture_id == no_lighting_list[i]) {
                SDL_Texture* t = get_texture(TEXTURE_WALLS, texture_id);
                return texturePair_t(t, t);
            }
        }
        return texturePair_t(get_texture(TEXTURE_WALLS, texture_id), get_texture(TEXTURE_WALLS, texture_id + 1));
    }

    SDL_Window* sdl_window;
    SDL_Renderer* sdlRenderer;
    
    int screen_width;
    int screen_height;
    int refresh_rate;

    std::map<TEXTURE_TYPE, std::vector<SDL_Texture*>> textures;
};

/*
================================
 Final game data container
================================
*/

struct GameData {

    player Player;
    map Map;
    multimedia Multimedia;

};
