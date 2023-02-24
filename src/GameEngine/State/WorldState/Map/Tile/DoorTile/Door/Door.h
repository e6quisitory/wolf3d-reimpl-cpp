#pragma once

/*
===========================================
    Relevant type definitions
===========================================
*/

enum class doorStatus_t {
    CLOSED,
    OPENING,
    OPEN,
    CLOSING
};

enum class doorPosition_t {
    OPEN   = 0,
    CLOSED = 1
};

enum class doorTimerVal_t {
    NO_TIME_LEFT   = 0,
    FULL_TIME_LEFT = 1
};

/*
===========================================
    Door
===========================================
*/

struct Door {
    Door();

    doorStatus_t   status;
    double         position;
    double         timerVal;
};