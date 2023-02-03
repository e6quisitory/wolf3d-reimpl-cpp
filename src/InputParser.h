/*
 * input_parser.h:
 *
 * Scans inputs from keyboard and sets Inputs->curr_commands.
 * I.e. converts raw inputs into commands, which are then read by other managers (player_manager, etc.)
 *
 */

#pragma once

#include <algorithm>
#include <SDL2/SDL.h>

#include "Inputs.h"
#include "GameData.h"

class InputParser {
public:
    void Init(Inputs* _inputs) {
        Inputs = _inputs;

        // Initialize current commands std::map in GameData->Inputs
        for (int cmd_type = 0; cmd_type < NUM_COMMAND_TYPES; ++cmd_type)
            Inputs->curr_commands.insert({static_cast<COMMAND_TYPE>(cmd_type), NONE});

        // Mouse not moving at start
        Inputs->mouse_abs_xrel = 0;

        // Save local copy of keyboard_state pointer
        keyboard_state = SDL_GetKeyboardState(nullptr);
    }

    void ParseInputs() {

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
                    Inputs->curr_commands[LOOKING] = LOOK_RIGHT;
                else if (curr_xrel < 0)
                    Inputs->curr_commands[LOOKING] = LOOK_LEFT;

                Inputs->mouse_abs_xrel = abs(curr_xrel);
            }
        }

        // Detect when mouse is now moving anymore and reset xrel and looking command (to prevent player spin)
        static int prev_xrel = 0;
        static int cutoff_counter = 0;

        int curr_xrel = Inputs->mouse_abs_xrel;

        if (curr_xrel == prev_xrel && get_xrel() == 0)
            ++cutoff_counter;

        if (cutoff_counter >= 5) {
            cutoff_counter = 0;
            Inputs->mouse_abs_xrel = 0;
            Inputs->curr_commands[LOOKING] = NONE;
        }

        prev_xrel = curr_xrel;

        /*
        ===========================
         W,A,S,D to move player
        ===========================
        */
        
        bool up         = keyboard_state[SDL_SCANCODE_W];
        bool back       = keyboard_state[SDL_SCANCODE_S];
        bool right      = keyboard_state[SDL_SCANCODE_D];
        bool left       = keyboard_state[SDL_SCANCODE_A];
        bool up_right   = up && right;
        bool up_left    = up && left;
        bool back_right = back && right;
        bool back_left  = back && left;

        if      (up_right)   Inputs->curr_commands[MOVEMENT] = MOVE_NORTHEAST;
        else if (up_left)    Inputs->curr_commands[MOVEMENT] = MOVE_NORTHWEST;
        else if (back_right) Inputs->curr_commands[MOVEMENT] = MOVE_SOUTHEAST;
        else if (back_left)  Inputs->curr_commands[MOVEMENT] = MOVE_SOUTHWEST;
        else if (up)         Inputs->curr_commands[MOVEMENT] = MOVE_NORTH;
        else if (back)       Inputs->curr_commands[MOVEMENT] = MOVE_SOUTH;
        else if (left)       Inputs->curr_commands[MOVEMENT] = MOVE_WEST;
        else if (right)      Inputs->curr_commands[MOVEMENT] = MOVE_EAST;
        else                 Inputs->curr_commands[MOVEMENT] = NONE;

        /*
        ===========================
         Spacebar to open door
        ===========================
        */

        Inputs->curr_commands[DOORS] = keyboard_state[SDL_SCANCODE_SPACE] ? OPEN_DOOR : NONE;
        
        /*
        ===========================
         Escape key to exit game
        ===========================
        */

        if (keyboard_state[SDL_SCANCODE_ESCAPE])
            Inputs->quit_flag = true;
        
    }

private:
    // Returns current relative x position of mouse (an alternative way to get it aside from using SDL_Events)
    int get_xrel() const {
        int x;
        SDL_GetRelativeMouseState(&x, nullptr);
        return x;
    }

private:
    Inputs* Inputs;
    const Uint8* keyboard_state;
};
