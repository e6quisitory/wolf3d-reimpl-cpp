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
    static std::map<enemyTextureType_t, std::vector<SDL_Texture*>> textures;

private:
    Point2 centerCoord;
    SDL_Texture* texture;

public:
    EnemyTile(const iPoint2& _tileCoord, SDL_Texture* const _texture);

    virtual rayTileHitVariant_o  RayTileHit(RayHitMarker& hitInfo)  const override;
    virtual bool                 PlayerTileHit()                    const override;
};