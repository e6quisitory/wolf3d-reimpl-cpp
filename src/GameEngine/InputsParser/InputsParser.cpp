#include "InputsParser.h"

/*
=========================================================
    Public methods
=========================================================
*/

void InputsParser::Init(InputsBuffer *const _inputsBuffer, SDL_Window* const _mainGameWindow) {
    inputsBuffer = _inputsBuffer;
    mainGameWindow = _mainGameWindow;

    inputsBuffer->toggleMouseLock = false;
    inputsBuffer->quitGameFlag = false;
    keyboardState = SDL_GetKeyboardState(nullptr);
}

void InputsParser::ParseInputs() const {

    bool mainGameWindowMouseFocus = SDL_GetMouseFocus() == mainGameWindow;
    bool cursorVisible            = SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE ? true : false;

    /************************* Mouse movement + clicks + toggle mouse lock *************************/

    auto& lookingCmd        = inputsBuffer->currentCommands[inputCommandType_t::LOOKING];
    auto& nonGameMouseClick = inputsBuffer->nonGameWindowMouseClick;

    SDL_Event currPendingEvent;
    int currXrel;
    while (SDL_PollEvent(&currPendingEvent)){
        if (currPendingEvent.type == SDL_MOUSEMOTION && mainGameWindowMouseFocus && !cursorVisible) {
            currXrel = currPendingEvent.motion.xrel;
            if (currXrel > 0)
                lookingCmd = inputCommand_t::LOOK_RIGHT;
            else if (currXrel < 0)
                lookingCmd = inputCommand_t::LOOK_LEFT;

            inputsBuffer->mouseAbsXrel = abs(currXrel);
        }

        if (currPendingEvent.type == SDL_MOUSEBUTTONUP && !mainGameWindowMouseFocus) {
            if (currPendingEvent.button.button == SDL_BUTTON_LEFT)
                nonGameMouseClick = mouseClickType_t::LEFT;
            else if (currPendingEvent.button.button == SDL_BUTTON_RIGHT)
                nonGameMouseClick = mouseClickType_t::RIGHT;
            else
                nonGameMouseClick = mouseClickType_t::NONE;

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            inputsBuffer->mouseClickLocation = Pixel(mouseX, mouseY);
        } else
            nonGameMouseClick = mouseClickType_t::NONE;

        if (currPendingEvent.type == SDL_KEYDOWN && currPendingEvent.key.keysym.sym == SDLK_BACKQUOTE)
            inputsBuffer->toggleMouseLock = true;
    }

    // Detect when mouse is not moving anymore and reset xrel and looking command (to prevent player spin)
    static int prevXrel = 0;
    if (currXrel == prevXrel && GetXrel() == 0) {
        inputsBuffer->mouseAbsXrel = 0;
        lookingCmd = inputCommand_t::NONE;
    }

    prevXrel = currXrel;

    if (mainGameWindowMouseFocus) {

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

        if        (northEast)   MovementCmd  = inputCommand_t::MOVE_NORTHEAST;
        else if   (northWest)   MovementCmd  = inputCommand_t::MOVE_NORTHWEST;
        else if   (southEast)   MovementCmd  = inputCommand_t::MOVE_SOUTHEAST;
        else if   (southWest)   MovementCmd  = inputCommand_t::MOVE_SOUTHWEST;
        else if   (north)       MovementCmd  = inputCommand_t::MOVE_NORTH;
        else if   (south)       MovementCmd  = inputCommand_t::MOVE_SOUTH;
        else if   (west)        MovementCmd  = inputCommand_t::MOVE_WEST;
        else if   (east)        MovementCmd  = inputCommand_t::MOVE_EAST;
        else                    MovementCmd  = inputCommand_t::NONE;

        /************************* Spacebar (to open doors) *************************/

        auto& DoorsCmd = inputsBuffer->currentCommands[inputCommandType_t::DOORS];
              DoorsCmd = keyboardState[SDL_SCANCODE_SPACE] ? inputCommand_t::OPEN_DOOR : inputCommand_t::NONE;
    }

    /************************* Escape (to exit game) *************************/

    inputsBuffer->quitGameFlag = keyboardState[SDL_SCANCODE_ESCAPE];
}

/*
=========================================================
    Private methods
=========================================================
*/

int InputsParser::GetXrel() const {
    int currXrel;
    SDL_GetRelativeMouseState(&currXrel, nullptr);
    return currXrel;
}