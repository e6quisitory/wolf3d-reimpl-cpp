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
#include "ivec2.h"
#include "tile.h"

/*
================================
 Player
================================
*/

struct player {
    point2 location;
    vec2 view_dir;
    vec2 east;
};

/*
================================
 Map
================================
*/

struct map {
    tile* operator [] (const int& index) const {
        return tiles[index];
    }

    tile* operator () (const int& x, const int& y) const {
        return tiles[y*width + x];
    }

    tile* get_tile(const ipoint2& ipt) const {
        return (*this)(ipt.x(), ipt.y());
    }
    
    point2 get_center(const int& index) {
        int x = index % width;
        int y = index / width;
        return point2(x + 0.5, y + 0.5);
    }

    template<class point_type>
    bool within_map(point_type pt) {
        if((pt.x() >= 0 && pt.x() <= width) && (pt.y() >= 0 && pt.y() <= height))
            return true;
        else
            return false;
    }

    void add_active_door(door* const _door) {
        active_doors.insert({_door, _door});
    }

    void remove_active_door(door* const _door) {
        active_doors.erase(_door);
    }

    tile** tiles;  // Tiles need to be stored as pointers for polymorphism to work
    int width;
    int height;
    int num_tiles;

    /* Doors */
    std::map<door*, door*> active_doors;
    bool any_doors_awaiting_rendering;
    
    /* Sprites */
    std::vector<sprite*> sprites;
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
    TEXTURE_WEAPONS
};

#define NUM_TEXTURE_TYPES 4

struct texture_info {
    texture_info(const TEXTURE_TYPE& _texture_type, const int& _texture_id): TextureType(_texture_type), TextureID(_texture_id) {}

    TEXTURE_TYPE TextureType;
    int TextureID;
};

struct multimedia {
    SDL_Texture* get_texture(const TEXTURE_TYPE& _texture_type, const int& texture_id) {
        return textures[_texture_type][texture_id-1];
    }

    SDL_Texture* get_texture(const texture_info& _texture_info) {
        return textures[_texture_info.TextureType][_texture_info.TextureID-1];
    }

    void add_texture(const TEXTURE_TYPE& _texture_type, SDL_Texture* const texture) {
        textures[_texture_type].push_back(texture);
    }

    texture_pair get_texture_pair(const texture_info& _texture_info) {
        if (_texture_info.TextureType == TEXTURE_WALLS)
            return get_wall_texture_pair(_texture_info.TextureID);
        else {
            SDL_Texture* t = get_texture(_texture_info);
            return texture_pair(t,t);
        }
    }

    texture_pair get_wall_texture_pair(int texture_id) {
        static int no_lighting_list[10] = {31,32,41,42,43,44,107,108,109,110};
        for (int i = 0; i < 10; ++i) {
            if (texture_id == no_lighting_list[i]) {
                SDL_Texture* t = get_texture(TEXTURE_WALLS, texture_id);
                return texture_pair(t,t);
            }
        }
        return texture_pair(get_texture(TEXTURE_WALLS, texture_id), get_texture(TEXTURE_WALLS, texture_id + 1));
    }

    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;
    
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

struct game_data {

    player Player;
    map Map;
    multimedia Multimedia;

};
