#pragma once

/*
=========================================================
    Relevant type definitions
=========================================================
*/

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

enum class mouseClickType_t {
    NONE,
    LEFT,
    RIGHT
};

/*
=========================================================
    InputsBuffer
=========================================================
*/

class InputsBuffer {
public:
    // Main game related
    std::map<inputCommandType_t, inputCommand_t>  currentCommands;
    int                                           mouseAbsXrel;
    bool                                          quitGameFlag;

    // Devtool related
    mouseClickType_t nonGameWindowMouseClick;
    Pixel            mouseClickLocation;
    bool             toggleMouseLock;
};
