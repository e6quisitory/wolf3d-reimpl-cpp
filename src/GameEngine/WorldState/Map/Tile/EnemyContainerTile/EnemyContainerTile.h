#pragma once

#include "../Tile.h"
#include "../../../../Managers/EnemyManager/Enemy/Enemy.h"

/*
=========================================================
    Relevant Type Definitions
=========================================================
*/

enum class enemyTextureType_t {
    STATIC,
    RUN1,
    RUN2,
    RUN3,
    RUN4,
    ALERTED,
    SHOOTING,
    DAMAGE_RIGHT,
    DAMAGE_LEFT,
    DEATH
};

/*
=========================================================
    EnemyContainerTile
=========================================================
*/


class EnemyContainerTile : public Tile {
public:
    static std::map<textureType_t, std::map<enemyTextureType_t, std::vector<SDL_Texture*>>> textures;
    iPoint2 tileCoord;
    std::set<Enemy*> enemies;

protected:
    textureCoordPairVec_t GetEnemiesTextureCoordPairs(RayHitMarker& hitInfo) const;
};