#pragma once

#include "../SpriteTile.h"
#include "../../../../../Managers/EnemyManager/Enemy/Enemy.h"

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
    EnemyTile
=========================================================
*/

class EnemyTile : public SpriteTile {
public:
    static std::map<textureType_t, std::map<enemyTextureType_t, std::vector<SDL_Texture*>>> textures;
    Enemy* enemy;

private:
    Point2 centerCoord;
    textureType_t textureType;

public:
    EnemyTile(const iPoint2& _tileCoord, const textureType_t _textureType);
    ~EnemyTile();

    virtual rayTileHitVariant_o  RayTileHit(RayHitMarker& hitInfo)  const override;
    virtual bool                 PlayerTileHit()                    const override;
};