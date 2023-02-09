#pragma once

#include "../../../Utilities/Vec2D.h"

/*
=========================================================
    Player
=========================================================
*/

struct Player {
    Point2  location;
    Vec2    viewDir;
    Vec2    eastDir;
};