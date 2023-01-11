/*
 * input_manager.h:
 *
 * Scans inputs from keyboard and sets GameData->Inputs.curr_commands.
 * I.e. converts raw inputs into commands, which are then read by other managers (player_manager, etc.)
 *
 */

#pragma once

#include <algorithm>
#include <SDL2/SDL.h>

#include "game_data.h"
#include "global.h"

class input_manager {
public:
    void init(game_data* gm_dat) {
        GameData = gm_dat;

        // Initialize current commands std::map in GameData->Inputs
        for (int cmd_type = 0; cmd_type < NUM_COMMAND_TYPES; ++cmd_type)
            GameData->Inputs.curr_commands.insert({static_cast<COMMAND_TYPE>(cmd_type), NONE});

        // Mouse not moving at start
        GameData->Inputs.mouse_abs_xrel = 0;

        // Save local copy of keyboard_state pointer
        keyboard_state = SDL_GetKeyboardState(nullptr);
    }

    void poll_inputs() {

        /*
        ====================================================
         Mouse movement (for player to look side to side)
        ====================================================
        */

        SDL_Event e;
        while (SDL_PollEvent(&e)){
            int curr_xrel = e.motion.xrel;
            if (e.type == SDL_MOUSEMOTION) {
                if (curr_xrel > 0)
                    GameData->Inputs.curr_commands[LOOKING] = LOOK_RIGHT;
                else if (curr_xrel < 0)
                    GameData->Inputs.curr_commands[LOOKING] = LOOK_LEFT;

                GameData->Inputs.mouse_abs_xrel = abs(curr_xrel);
            }
        }

        // Detect when mouse is now moving anymore and reset xrel and looking command (to prevent player spin)
        static int prev_xrel = 0;
        static int cutoff_counter = 0;

        int curr_xrel = GameData->Inputs.mouse_abs_xrel;

        if (curr_xrel == prev_xrel && get_xrel() == 0)
            ++cutoff_counter;

        if (cutoff_counter >= 5) {
            cutoff_counter = 0;
            GameData->Inputs.mouse_abs_xrel = 0;
            GameData->Inputs.curr_commands[LOOKING] = NONE;
        }

        prev_xrel = curr_xrel;

        /*
        ===========================
         Escape key to exit game
        ===========================
        */

        if (keyboard_state[SDL_SCANCODE_ESCAPE]) GameData->quit_flag = true;

        // Movement (to move player in the game world)
        bool up =         keyboard_state[SDL_SCANCODE_W];
        bool back =       keyboard_state[SDL_SCANCODE_S];
        bool right =      keyboard_state[SDL_SCANCODE_D];
        bool left =       keyboard_state[SDL_SCANCODE_A];
        bool up_right =   up && right;
        bool up_left =    up && left;
        bool back_right = back && right;
        bool back_left =  back && left;

        if      (up_right)   GameData->Inputs.curr_commands[MOVEMENT] = MOVE_NORTHEAST;
        else if (up_left)    GameData->Inputs.curr_commands[MOVEMENT] = MOVE_NORTHWEST;
        else if (back_right) GameData->Inputs.curr_commands[MOVEMENT] = MOVE_SOUTHEAST;
        else if (back_left)  GameData->Inputs.curr_commands[MOVEMENT] = MOVE_SOUTHWEST;
        else if (up)         GameData->Inputs.curr_commands[MOVEMENT] = MOVE_NORTH;
        else if (back)       GameData->Inputs.curr_commands[MOVEMENT] = MOVE_SOUTH;
        else if (left)       GameData->Inputs.curr_commands[MOVEMENT] = MOVE_WEST;
        else if (right)      GameData->Inputs.curr_commands[MOVEMENT] = MOVE_EAST;
        else                 GameData->Inputs.curr_commands[MOVEMENT] = NONE;

        /*
        ===========================
         Spacebar to open door
        ===========================
        */

        GameData->Inputs.curr_commands[DOORS] = keyboard_state[SDL_SCANCODE_SPACE] ? OPEN_DOOR : NONE;
    }

private:
    // Returns current relative x position of mouse (an alternative way to get it aside from using SDL_Events)
    int get_xrel() {
        int x;
        SDL_GetRelativeMouseState(&x, nullptr);
        return x;
    }

private:
    game_data* GameData;
    const Uint8* keyboard_state;
};