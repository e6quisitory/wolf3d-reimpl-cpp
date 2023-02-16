#pragma once

#include "../State/InputsBuffer/InputsBuffer.h"

/*
=========================================================
    InputsParser
=========================================================
*/

class InputsParser {
private:
    InputsBuffer*  inputsBuffer;
    const Uint8*   keyboardState;

public:
    void   Init            (InputsBuffer* const _inputsBuffer);
    void   ParseInputs()                                          const;

private:
    int    GetXrel()                                              const;
};
