#include "InputsParser.h"

/*
================================
    Public Methods
================================
*/

void InputsParser::Init(InputsBuffer* const _inputsBuffer) {
    inputsBuffer = _inputsBuffer;
    _inputsBuffer->quitGameFlag = false;

    keyboardState = SDL_GetKeyboardState(nullptr);
}

void InputsParser::ParseInputs() const {

    /************************* Mouse movement (to look side to side) *************************/

    auto& LookingCmd = inputsBuffer->currentCommands[inputCommandType_t::LOOKING];

    SDL_Event currPendingEvent;
    int currXrel;
    while (SDL_PollEvent(&currPendingEvent)){
        if (currPendingEvent.type == SDL_MOUSEMOTION) {
            currXrel = currPendingEvent.motion.xrel;
            if (currXrel > 0)
                LookingCmd = inputCommand_t::LOOK_RIGHT;
            else if (currXrel < 0)
                LookingCmd = inputCommand_t::LOOK_LEFT;

            inputsBuffer->mouseAbsXrel = abs(currXrel);
        }
    }

    // Detect when mouse is not moving anymore and reset xrel and looking command (to prevent player spin)
    static int prevXrel = 0;

    if (currXrel == prevXrel && GetXrel() == 0) {
        inputsBuffer->mouseAbsXrel = 0;
        LookingCmd = inputCommand_t::NONE;
    }

    prevXrel = currXrel;

    /************************* W, A, S, D (to move player) *************************/

    bool north        = keyboardState[SDL_SCANCODE_W];
    bool south        = keyboardState[SDL_SCANCODE_S];
    bool east         = keyboardState[SDL_SCANCODE_D];
    bool west         = keyboardState[SDL_SCANCODE_A];
    bool northEast    = north && east;
    bool northWest    = north && west;
    bool southEast    = south && east;
    bool southWest    = south && west;

    auto& MovementCmd = inputsBuffer->currentCommands[inputCommandType_t::MOVEMENT];

    if        (northEast)   MovementCmd    = inputCommand_t::MOVE_NORTHEAST;
    else if   (northWest)   MovementCmd    = inputCommand_t::MOVE_NORTHWEST;
    else if   (southEast)   MovementCmd    = inputCommand_t::MOVE_SOUTHEAST;
    else if   (southWest)   MovementCmd    = inputCommand_t::MOVE_SOUTHWEST;
    else if   (north)       MovementCmd    = inputCommand_t::MOVE_NORTH;
    else if   (south)       MovementCmd    = inputCommand_t::MOVE_SOUTH;
    else if   (west)        MovementCmd    = inputCommand_t::MOVE_WEST;
    else if   (east)        MovementCmd    = inputCommand_t::MOVE_EAST;
    else                    MovementCmd    = inputCommand_t::NONE;

    /************************* Spacebar (to open doors) *************************/

    auto& DoorsCmd = inputsBuffer->currentCommands[inputCommandType_t::DOORS];
          DoorsCmd = keyboardState[SDL_SCANCODE_SPACE] ? inputCommand_t::OPEN_DOOR : inputCommand_t::NONE;

    /************************* Escape (to exit game) *************************/

    if (keyboardState[SDL_SCANCODE_ESCAPE])
        inputsBuffer->quitGameFlag = true;
}

/*
================================
    Private Methods
================================
*/

int InputsParser::GetXrel() const {
    int currXrel;
    SDL_GetRelativeMouseState(&currXrel, nullptr);
    return currXrel;
}