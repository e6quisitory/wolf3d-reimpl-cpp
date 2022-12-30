#pragma once

#include "vec2.h"
#include "ivec2.h"
#include "tile.h"

enum COMMAND_TYPE {
    MOVEMENT,
    LOOKING,
    DOORS,
    WEAPON
};

// North = player view direction, South = back of player, East = right of player, West = left of player
enum COMMAND {
    NONE = 0,
    MOVE_NORTH,
    MOVE_SOUTH,
    MOVE_EAST,
    MOVE_WEST,
    MOVE_NORTHEAST,
    MOVE_NORTHWEST,
    MOVE_SOUTHEAST,
    MOVE_SOUTHWEST,
    LOOK_RIGHT,
    LOOK_LEFT,
    OPEN_DOOR,
    FIRE_WEAPON,
};

struct inputs {
    COMMAND get_command(COMMAND_TYPE _command_type) {
        return curr_commands[_command_type];
    }

    void set_command(COMMAND_TYPE _command_type, COMMAND _command) {
        curr_commands[_command_type] = _command;
    }

    COMMAND curr_commands[NUM_COMMANDS];  // [MOVEMENT, LOOKING, DOOR, WEAPON]
};

struct player {
    point2 location;
    vec2 view_dir;
    vec2 east;
};

struct map {
    tile* operator [] (int index) const {
        return tiles[index];
    }

    tile* operator () (int x, int y) const {
        return tiles[y*width + x];
    }

    tile* operator () (ipoint2 ipt) const {
        return (*this)(ipt.x(), ipt.y());
    }

    tile* get_tile(const ipoint2& ipt) {
        return (*this)(ipt);
    }

    template<class point_type>
    bool within_map(point_type pt) {
        if((pt.x() >= 0 && pt.x() <= width) && (pt.y() >= 0 && pt.y() <= height))
            return true;
        else
            return false;
    }

    tile** tiles;  // Tiles need to be stored as pointers for polymorphism to work
    int width;
    int height;
    int num_tiles;

    int active_doors;
};

struct multimedia {
    SDL_Texture* get_texture(int texture_id) const {
        return textures[texture_id-1];
    }

    void set_texture(int texture_id, SDL_Texture* texture) {
        textures[texture_id-1] = texture;
    }

    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;
    SDL_Texture* textures[110];  // perhaps make this 110 value flexible?

    int screen_width;
    int screen_height;
    int refresh_rate;
};

typedef struct game_data {

    inputs Inputs;
    player Player;
    map Map;
    multimedia Multimedia;

} game_date;