#pragma once

#include "Player/Player.h"
#include "Map/Map.h"
#include "../../Multimedia/Multimedia.h"

/*
=========================================================
    GameState
=========================================================
*/

struct GameState {
    Player  player;
    Map     map;
};
