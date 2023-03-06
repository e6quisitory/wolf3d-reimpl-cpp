#pragma once

#include "../Tile.h"

/*
=========================================================
    Sprite Tile (objects, enemies)
=========================================================
*/

class SpriteTile : public Tile {
public:
    iPoint2 tileCoord;  // needed for marking tile hit in spriteTileHitMap in Renderer
};
