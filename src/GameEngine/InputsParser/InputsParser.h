#pragma once

#include <algorithm>
#include <SDL2/SDL.h>

#include "../State/InputsBuffer/InputsBuffer.h"
#include "../State/GameState/GameState.h"

/*
=========================================================
    InputsParser
=========================================================
*/

class InputsParser {
private:
          InputsBuffer*  inputsBuffer;
    const Uint8*         keyboardState;

public:
    void   Init(InputsBuffer* _inputsBuffer);
    void   ParseInputs()                        const;

private:
    int    GetXrel()                            const;
};
