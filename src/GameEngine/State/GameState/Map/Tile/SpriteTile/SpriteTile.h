#pragma once

#include "../Tile.h"

/*
===========================================
    Sprite tile (objects, enemies)
===========================================
*/

class SpriteTile : public Tile {
private:
    texturePair_t texture;
    Ray           perpLine;  // Vector line (ray) perpendicular to player view direction ; same for ALL sprites

public:
    SpriteTile(const Point2& _center, const texturePair_t& _texture);
    
    virtual textureSliceDistPair_o  RayTileHit          (HitInfo& hitInfo, const texturePair_o textureOverride)   const override;
    virtual bool                    PlayerTileHit()                                                               const override;
            void                    CalculatePerpLine   (const Vec2& view_dir);
    
private:
    HitInfo_o RayPerpLineHit(const Ray& incomingRay) const;  // Calculates intersection between incoming ray and perp_line
};
