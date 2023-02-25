#pragma once

#include "../SpriteTile.h"

/*
=========================================================
    ObjectTile
=========================================================
*/

class ObjectTile : public SpriteTile {
public:
    static std::vector<SDL_Texture*> noPassthroughList;

private:
    SDL_Texture* texture;
    bool passthrough;

public:
    ObjectTile(const iPoint2 &tileCoord, SDL_Texture* const _texture);

    virtual  textureSliceDistPair_o  RayTileHit(RayHitMarker& hitInfo, const texturePair_o textureOverride)  const override;
    virtual  bool                    PlayerTileHit()                                                         const override;
};