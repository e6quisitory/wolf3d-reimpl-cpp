#pragma once

#include "../Tile.h"

/*
=========================================================
    Sprite Tile (objects, enemies)
=========================================================
*/

class SpriteTile : public Tile {
public:
    static Vec2 perplinesDir;   // Same for ALL SpriteTiles

protected:
    Point2 perplineOrigin;     // Unique to each SpriteTile

public:
    static void UpdateUniversalPerpline(const Vec2& playerViewDir);

protected:
    RayHitMarker_o RayPerplineHit(const Ray& incomingRay) const;
};
