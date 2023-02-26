#pragma once

#include "../Tile.h"

/*
=========================================================
    WallTile (basic textured wall)
=========================================================
*/

class WallTile : public Tile {
private:
    texturePair_t texture;

public:
    WallTile(const texturePair_t _texture);

    virtual rayTileHitVariant_o  RayTileHit        (RayHitMarker& hitInfo)  const override;
    virtual bool                 PlayerTileHit()                            const override;
};