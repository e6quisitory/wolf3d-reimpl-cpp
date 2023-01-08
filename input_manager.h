#pragma once

#include <algorithm>
#include <SDL2/SDL.h>

#include "game_data.h"

class input_manager {
public:
    void init(game_data* gm_dat) {
        // Get game_data object
        GameData = gm_dat;

        // Initialize current commands std::map in GameData->Inputs
        for (int cmd_type = 0; cmd_type < GameData->Inputs.NUM_COMMAND_TYPES; ++cmd_type)
            GameData->Inputs.curr_commands.insert({static_cast<COMMAND_TYPE>(cmd_type), NONE});

        // Save local copy of keyboard_state pointer
        keyboard_state = SDL_GetKeyboardState(nullptr);
    }

    bool poll_inputs() {
        bool up =         keyboard_state[SDL_SCANCODE_W];
        bool back =       keyboard_state[SDL_SCANCODE_S];
        bool right =      keyboard_state[SDL_SCANCODE_D];
        bool left =       keyboard_state[SDL_SCANCODE_A];
        bool up_right =   up && right;
        bool up_left =    up && left;
        bool back_right = back && right;
        bool back_left =  back && left;

        bool door  = keyboard_state[SDL_SCANCODE_SPACE];
        bool fire =  keyboard_state[SDL_SCANCODE_LSHIFT];

        bool look_right = keyboard_state[SDL_SCANCODE_RIGHT];
        bool look_left  = keyboard_state[SDL_SCANCODE_LEFT];

        // Set MOVEMENT command
        if      (up_right)   GameData->Inputs.curr_commands[MOVEMENT] = MOVE_NORTHEAST;
        else if (up_left)    GameData->Inputs.curr_commands[MOVEMENT] = MOVE_NORTHWEST;
        else if (back_right) GameData->Inputs.curr_commands[MOVEMENT] = MOVE_SOUTHEAST;
        else if (back_left)  GameData->Inputs.curr_commands[MOVEMENT] = MOVE_SOUTHWEST;
        else if (up)         GameData->Inputs.curr_commands[MOVEMENT] = MOVE_NORTH;
        else if (back)       GameData->Inputs.curr_commands[MOVEMENT] = MOVE_SOUTH;
        else if (left)       GameData->Inputs.curr_commands[MOVEMENT] = MOVE_WEST;
        else if (right)      GameData->Inputs.curr_commands[MOVEMENT] = MOVE_EAST;
        else                 GameData->Inputs.curr_commands[MOVEMENT] = NONE;

        // Set LOOKING command
        if ((look_left && look_right) || (!look_left && !look_right))
            GameData->Inputs.curr_commands[LOOKING] = NONE;
        else if (look_right)
            GameData->Inputs.curr_commands[LOOKING] = LOOK_RIGHT;
        else
            GameData->Inputs.curr_commands[LOOKING] = LOOK_LEFT;

        // Set DOOR and WEAPON commands
        GameData->Inputs.curr_commands[DOORS] = door ? OPEN_DOOR : NONE;
        GameData->Inputs.curr_commands[WEAPON] = fire ? FIRE_WEAPON : NONE;

        return any_input();
    }

private:
    bool any_input() {
        for (auto const& [cmd_type, cmd] : GameData->Inputs.curr_commands)
            if (cmd != NONE)
                return true;

        return false;
    }

private:
    game_data* GameData;

    const Uint8* keyboard_state;
};