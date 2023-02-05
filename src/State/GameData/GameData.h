#pragma once

#include "Player/Player.h"
#include "Map/Map.h"
#include "Multimedia/Multimedia.h"

struct GameData {
    Player      player;
    Map         map;
    Multimedia  multimedia;
};
