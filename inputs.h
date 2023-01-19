/*
 * inputs.h:
 *
 * Stores the current inputs commands (for each frame/cycle of the game loop)
 *
 */

#pragma once

#include <map>

enum COMMAND_TYPE {
    MOVEMENT,
    LOOKING,
    DOORS
};

#define NUM_COMMAND_TYPES 3

enum COMMAND {
    NONE,
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
    OPEN_DOOR
};

struct inputs {
    bool any_active_inputs() {
        for (int cmd_type = 0; cmd_type < NUM_COMMAND_TYPES; ++cmd_type)
            if (curr_commands[static_cast<COMMAND_TYPE>(cmd_type)] != NONE)
                return true;
        return false;
    }

    std::map<COMMAND_TYPE, COMMAND> curr_commands;
    int mouse_abs_xrel;

    bool quit_flag;
};
