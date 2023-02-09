#pragma once

#include "../Tile.h"

/*
================================
    Empty tile
================================
*/

class EmptyTile : public Tile {
public:
    EmptyTile();

    virtual textureSliceDistPair_o  RayTileHit      (HitInfo& hitInfo, const texturePair_o& textureOverride)  const override;
    virtual bool                    PlayerTileHit()                                                           const override;
};