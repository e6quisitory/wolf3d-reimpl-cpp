/*
 * global.h:
 *
 * Various constants, conventions and misc. functions used across multiple files in project.
 *
 */

#pragma once

/*
================================
 Global constants
================================
*/

const double PI = 3.1415926535897932385;
const int TEXTURE_PITCH = 64;
const int NUM_WALL_TEXTURES = 110;

/*
================================
 Global conventions / definitions
================================
*/

// NOTE: These directions are all relative to the *player*, not the map grid. I.e. the player's view_dir is *always* NORTH.

enum HORIZONTAL_DIR {
    EAST = 1,
    WEST = -1
};

using X_DIR = HORIZONTAL_DIR;

enum VERTICAL_DIR {
    NORTH = 1,
    SOUTH = -1
};

using Y_DIR = VERTICAL_DIR;

enum SWIVEL_DIR {
    COUNTER_CLOCKWISE = 1,
    CLOCKWISE = -1
};

// A pair of SDL_Texture pointers; the first texture is the lit texture, the second is the unlit version
typedef std::pair<SDL_Texture*, SDL_Texture*> texture_pair;

/*
================================
 Miscellaneous functions
================================
*/

bool is_integer(double d) {
    if (d - static_cast<int>(d) != 0.0)
        return false;
    else
        return true;
}

double degrees_to_radians(double degrees) {
    return degrees*PI/180;
}