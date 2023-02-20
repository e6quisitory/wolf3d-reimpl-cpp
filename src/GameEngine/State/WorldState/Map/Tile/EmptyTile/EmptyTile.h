#pragma once

#include "../Tile.h"

/*
=========================================================
    Empty Tile
=========================================================
*/

class EmptyTile : public Tile {
public:
    EmptyTile();

    virtual textureSliceDistPair_o  RayTileHit       (RayHitMarker& hitInfo, const texturePair_o textureOverride) const override;
    virtual bool                    PlayerTileHit()                                                               const override;
};