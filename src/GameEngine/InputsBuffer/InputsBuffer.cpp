#include "InputsBuffer.h"

/*
================================
    Public Methods
================================
*/

bool InputsBuffer::AnyActiveInputs() const {
    for (const auto [commandType, command] : currentCommands)
        if (command != inputCommand_t::NONE)
            return true;
    return false;
}