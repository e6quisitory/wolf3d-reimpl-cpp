#pragma once

#include "../InputsBuffer/InputsBuffer.h"

/*
=========================================================
    InputsParser
=========================================================
*/

class InputsParser {
private:
    InputsBuffer*  inputsBuffer;
    SDL_Window*    mainGameWindow;
    const Uint8*   keyboardState;

public:
    void   Init            (InputsBuffer *const _inputsBuffer, SDL_Window* const _mainGameWindow);
    void   ParseInputs()                                                                           const;

private:
    int    GetXrel()                                                                               const;
};
