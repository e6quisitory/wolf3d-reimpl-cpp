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

public:
    EnemyTile(const iPoint2 &tileCoord, SDL_Texture* const _texture);

    virtual  textureSliceDistPair_o  RayTileHit(RayHitMarker& hitInfo, const texturePair_o textureOverride)  const override;
    virtual  bool                    PlayerTileHit()                                                         const override;
};