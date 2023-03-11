#pragma once

#include "../EnemyContainerTile/EnemyContainerTile.h"

/*
=========================================================
    ObjectTile
=========================================================
*/

class ObjectTile : public EnemyContainerTile {
public:
    static std::vector<SDL_Texture*> noPassthroughList;

private:
    SDL_Texture* texture;
    Point2 centerCoord;
    bool passthrough;

public:
    ObjectTile(const iPoint2& _tileCoord, SDL_Texture* const _texture);

    virtual  rayTileHitReturn_t  RayTileHit(RayHitMarker& hitInfo)  const override;
    virtual  bool                PlayerTileHit()                    const override;
};