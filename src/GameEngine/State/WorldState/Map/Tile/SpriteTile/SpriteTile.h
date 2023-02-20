#pragma once

#include "../Tile.h"

/*
=========================================================
    Sprite Tile (objects, enemies)
=========================================================
*/

class SpriteTile : public Tile {
private:
    texturePair_t texture;
    Point2        perplineOrigin;    // Unique to each SpriteTile

public:
    static Vec2   perplinesDir;      // Same for ALL SpriteTiles

public:
    SpriteTile(const Point2& _center, const texturePair_t& _texture);
    
    virtual textureSliceDistPair_o  RayTileHit        (RayHitMarker& hitInfo, const texturePair_o textureOverride)  const override;
    virtual bool                    PlayerTileHit()                                                                 const override;

private:
    RayHitMarker_o RayPerplineHit(const Ray& incomingRay) const;  // Calculates intersection between incoming ray and perpline
};
