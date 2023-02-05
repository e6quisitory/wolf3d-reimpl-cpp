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

#include "State/Inputs.h"
#include "State/GameData/GameData.h"

class InputParser {
public:
    void Init(Inputs* _inputs) {
        Inputs = _inputs;

        // Initialize current commands std::map in GameData->Inputs
        for (int inputCommandType = 0; inputCommandType < NUM_COMMAND_TYPES; ++inputCommandType)
            Inputs->currentCommands[static_cast<inputCommandType_t>(inputCommandType)] = inputCommand_t::NONE;

        // Mouse not moving at start
        Inputs->mouseAbsXrel = 0;

        keyboard_state = SDL_GetKeyboardState(nullptr);
    }

    void ParseInputs() {


        /*
        ====================================================
         Mouse movement (for player to look side to side)
        ====================================================
        */

        inputCommand_t& currentLookingCommand = Inputs->currentCommands[inputCommandType_t::LOOKING];

        SDL_Event e;
        while (SDL_PollEvent(&e)){
            int curr_xrel = e.motion.xrel;
            if (e.type == SDL_MOUSEMOTION) {
                if (curr_xrel > 0)
                    currentLookingCommand = inputCommand_t::LOOK_RIGHT;
                else if (curr_xrel < 0)
                    currentLookingCommand = inputCommand_t::LOOK_LEFT;

                Inputs->mouseAbsXrel = abs(curr_xrel);
            }
        }

        // Detect when mouse is not moving anymore and reset xrel and looking command (to prevent player spin)
        static int prev_xrel = 0;
        static int cutoff_counter = 0;

        int curr_xrel = Inputs->mouseAbsXrel;

        if (curr_xrel == prev_xrel && get_xrel() == 0)
            ++cutoff_counter;

        if (cutoff_counter >= 5) {
            cutoff_counter = 0;
            Inputs->mouseAbsXrel = 0;
            currentLookingCommand = inputCommand_t::NONE;
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

        inputCommand_t& currentMovementCommand = Inputs->currentCommands[inputCommandType_t::MOVEMENT];

        if      (up_right)   currentMovementCommand = inputCommand_t::MOVE_NORTHEAST;
        else if (up_left)    currentMovementCommand = inputCommand_t::MOVE_NORTHWEST;
        else if (back_right) currentMovementCommand = inputCommand_t::MOVE_SOUTHEAST;
        else if (back_left)  currentMovementCommand = inputCommand_t::MOVE_SOUTHWEST;
        else if (up)         currentMovementCommand = inputCommand_t::MOVE_NORTH;
        else if (back)       currentMovementCommand = inputCommand_t::MOVE_SOUTH;
        else if (left)       currentMovementCommand = inputCommand_t::MOVE_WEST;
        else if (right)      currentMovementCommand = inputCommand_t::MOVE_EAST;
        else                 currentMovementCommand = inputCommand_t::NONE;

        /*
        ===========================
         Spacebar to open door
        ===========================
        */

        Inputs->currentCommands[inputCommandType_t::DOORS] = keyboard_state[SDL_SCANCODE_SPACE] ? inputCommand_t::OPEN_DOOR : inputCommand_t::NONE;
        
        /*
        ===========================
         Escape key to exit game
        ===========================
        */

        if (keyboard_state[SDL_SCANCODE_ESCAPE])
            Inputs->quitGameFlag = true;
        
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
