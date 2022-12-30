#pragma once

#include <algorithm>
#include <SDL2/SDL.h>

#include "game_data.h"

class input_manager {
public:
    void init(game_data* gm_dat) {
        GameData = gm_dat;
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
        if      (up_right)   GameData->Inputs.set_command(MOVEMENT, MOVE_NORTHEAST);
        else if (up_left)    GameData->Inputs.set_command(MOVEMENT, MOVE_NORTHWEST);
        else if (back_right) GameData->Inputs.set_command(MOVEMENT, MOVE_SOUTHEAST);
        else if (back_left)  GameData->Inputs.set_command(MOVEMENT, MOVE_SOUTHWEST);
        else if (up)         GameData->Inputs.set_command(MOVEMENT, MOVE_NORTH);
        else if (back)       GameData->Inputs.set_command(MOVEMENT, MOVE_SOUTH);
        else if (left)       GameData->Inputs.set_command(MOVEMENT, MOVE_WEST);
        else if (right)      GameData->Inputs.set_command(MOVEMENT, MOVE_EAST);
        else                 GameData->Inputs.set_command(MOVEMENT, NONE);

        // Set LOOKING command
        if ((look_left && look_right) || (!look_left && !look_right))
            GameData->Inputs.set_command(LOOKING, NONE);
        else if (look_right)
            GameData->Inputs.set_command(LOOKING, LOOK_RIGHT);
        else
            GameData->Inputs.set_command(LOOKING, LOOK_LEFT);

        // Set DOOR and WEAPON commands
        GameData->Inputs.set_command(DOORS, door ? OPEN_DOOR : NONE);
        GameData->Inputs.set_command(WEAPON, fire ? FIRE_WEAPON : NONE);

        return any_input();
    }

private:
    bool any_input() {
        for (int i = 0; i < NUM_COMMANDS; ++i)
            if (GameData->Inputs.curr_commands[i] != NONE)
                return true;

        return false;
    }

private:
    game_data* GameData;
    const Uint8* keyboard_state;
};