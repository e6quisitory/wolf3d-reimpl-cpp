#pragma once

#include "Player/Player.h"
#include "Map/Map.h"

/*
=========================================================
    GameState
=========================================================
*/

struct GameState {
    Player  player;
    Map     map;
};
