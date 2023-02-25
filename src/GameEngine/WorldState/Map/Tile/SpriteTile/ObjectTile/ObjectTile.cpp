#include "ObjectTile.h"

/*
=========================================================
    Static members
=========================================================
*/

std::vector<SDL_Texture*> ObjectTile::noPassthroughList;

/*
=========================================================
    Constructors
=========================================================
*/

ObjectTile::ObjectTile(const iPoint2& tileCoord, SDL_Texture* const _texture) {
    texture = _texture;
    coordinate = tileCoord;
    type = tileType_t::OBJECT;
    perplineOrigin = coordinate + Point2(0.5, 0.5);
    passthrough = std::find(noPassthroughList.begin(), noPassthroughList.end(), texture) == noPassthroughList.end();
}

/*
=========================================================
    Public methods
=========================================================
*/

textureSliceDistPair_o ObjectTile::RayTileHit(RayHitMarker& hitInfo, const texturePair_o textureOverride) const {
    return SpriteRayTileHit(hitInfo, textureOverride, texture);
}

bool ObjectTile::PlayerTileHit() const {
    return !passthrough;
}