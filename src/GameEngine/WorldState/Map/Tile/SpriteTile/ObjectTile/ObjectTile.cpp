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

ObjectTile::ObjectTile(const iPoint2& _tileCoord, SDL_Texture* const _texture) {
    texture = _texture;
    type = tileType_t::OBJECT;
    tileCoord = _tileCoord;
    centerCoord = _tileCoord + Point2(0.5, 0.5);
    passthrough = std::find(noPassthroughList.begin(), noPassthroughList.end(), texture) == noPassthroughList.end();
}

/*
=========================================================
    Public methods
=========================================================
*/

rayTileHitVariant_o ObjectTile::RayTileHit(RayHitMarker& hitInfo) const {
    return textureCoordinatePair_t(texture, centerCoord);
}

bool ObjectTile::PlayerTileHit() const {
    return !passthrough;
}