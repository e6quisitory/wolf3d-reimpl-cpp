#pragma once

#include "../../../State/WorldState/Map/Tile/SpriteTile/SpriteTile.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

enum class enemyState_t {
    UNAWARE,
    ALERTED,
    ATTACKING,
    SHOOTING,
    DAMAGE,
    DYING
};

/*
=========================================================
    Enemy
=========================================================
*/

class Enemy {
public:
    SpriteTile*   tile;
    enemyState_t  state;
    Vec2          viewDir;

public:
    Enemy();
};