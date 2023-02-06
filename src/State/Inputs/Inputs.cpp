#include "Inputs.h"

/*
================================
    Public Methods
================================
*/

bool Inputs::AnyActiveInputs() const {
    for (const auto [commandType, command] : currentCommands)
        if (command != inputCommand_t::NONE)
            return true;
    return false;
}