#include "Door.h"

/*
===========================================
    Constructors
===========================================
*/

Door::Door() {
    status   = doorStatus_t::CLOSED;
    position = static_cast<double>(doorPosition_t::CLOSED);
    timerVal = static_cast<double>(doorTimerVal_t::FULL_TIME_LEFT);
};