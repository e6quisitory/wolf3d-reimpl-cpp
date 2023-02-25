#pragma once

#include "Player/Player.h"
#include "Map/Map.h"

/*
=========================================================
    WorldState
=========================================================
*/

struct WorldState {
    Player  player;
    Map     map;
};
