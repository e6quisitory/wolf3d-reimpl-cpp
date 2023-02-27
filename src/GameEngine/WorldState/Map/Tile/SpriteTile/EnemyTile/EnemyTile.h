#pragma once

#include "../SpriteTile.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

enum class enemyTextureClass_t {
    STATIC,
    RUN1,
    RUN2,
    RUN3,
    RUN4,
    DAMAGE_LEFT,
    DAMAGE_RIGHT,
    DEATH
};

/*
=========================================================
    EnemyTile
=========================================================
*/

class EnemyTile : public SpriteTile {
private:
    SDL_Texture* texture;
    Point2 centerCoord;

public:
    EnemyTile(const iPoint2 &tileCoord, SDL_Texture* const _texture);

    virtual rayTileHitVariant_o  RayTileHit(RayHitMarker& hitInfo)  const override;
    virtual bool                 PlayerTileHit()                    const override;
};