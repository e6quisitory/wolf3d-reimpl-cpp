#pragma once

#include "../SpriteTile.h"

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
    EnemyTile(const iPoint2& _tileCoord, SDL_Texture* const _texture);

    virtual rayTileHitVariant_o  RayTileHit(RayHitMarker& hitInfo)  const override;
    virtual bool                 PlayerTileHit()                    const override;
};