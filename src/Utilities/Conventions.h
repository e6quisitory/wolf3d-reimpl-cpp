#pragma once

#include <SDL2/SDL.h>

/*
================================
 Global constants
================================
*/

const double  PI                 = 3.1415926535897932385;
const int     TEXTURE_PITCH      = 64;
const Uint32  TRANSPARENCY_COLOR = 0xFF980088;

/*
===================================
 Global conventions / definitions
===================================
*/

// NOTE: These directions are all relative to the *player*, not the map grid. I.e. the player's view_dir is *always* NORTH.

enum class xDir_t {
    EAST = 1,
    WEST = -1
};

enum class yDir_t {
    NORTH = 1,
    SOUTH = -1
};

enum class swivelDir_t {
    COUNTER_CLOCKWISE = 1,
    CLOCKWISE = -1
};