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
    tile* operator [] (int index) const {
        return tiles[index];
    }

    tile* operator () (int x, int y) const {
        return tiles[y*width + x];
    }

    tile* get_tile(const ipoint2& ipt) const {
        return (*this)(ipt.x(), ipt.y());
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

    std::map<door*, door*> active_doors;
    bool any_doors_awaiting_rendering;
};

/*
================================
 Multimedia (game assets)
================================
*/

struct multimedia {
    SDL_Texture* get_texture(int texture_id) const {
        return textures[texture_id-1];
    }

    void set_texture(int texture_id, SDL_Texture* texture) {
        textures[texture_id-1] = texture;
    }

    texture_pair get_texture_pair(int texture_id) const {
        static int no_lighting_list[10] = {31,32,41,42,43,44,107,108,109,110};
        for (int i = 0; i < 10; ++i) {
            if (texture_id == no_lighting_list[i]) {
                SDL_Texture* t = get_texture(texture_id);
                return texture_pair(t,t);
            }
        }
        return texture_pair(get_texture(texture_id), get_texture(texture_id+1));
    }

    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;
    SDL_Texture* textures[110];

    int screen_width;
    int screen_height;
    int refresh_rate;
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