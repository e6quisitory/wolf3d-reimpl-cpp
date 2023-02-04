/*
 * inputs.h:
 *
 * Stores the current inputs commands (for each frame/cycle of the game loop)
 *
 */

#pragma once

#include <map>

enum class inputCommandType_t {
    MOVEMENT,
    LOOKING,
    DOORS
};

#define NUM_COMMAND_TYPES 3

enum class inputCommand_t {
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

struct Inputs {
    bool AnyActiveInputs() {
        for (const auto& [commandType, command] : currentCommands)
            if (command != inputCommand_t::NONE)
                return true;
        return false;
    }

    std::map<inputCommandType_t, inputCommand_t> currentCommands;
    int mouseAbsXrel;

    bool quitGameFlag;
};
