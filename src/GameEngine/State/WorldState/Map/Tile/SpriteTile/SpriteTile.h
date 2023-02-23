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
    bool          passthrough;
    Point2        perplineOrigin;    // Unique to each SpriteTile

public:
    static Vec2   perplinesDir;      // Same for ALL SpriteTiles

public:
    SpriteTile(const iPoint2 &tileCoord, const texturePair_t &_texture, const bool _passthrough);
    
    virtual textureSliceDistPair_o  RayTileHit                (RayHitMarker& hitInfo, const texturePair_o textureOverride)  const override;
    virtual bool                    PlayerTileHit()                                                                         const override;
    static  void                    UpdateUniversalPerpline   (const Vec2& playerViewDir);

private:
    RayHitMarker_o RayPerplineHit(const Ray& incomingRay) const;  // Calculates intersection between incoming ray and perpline
};
