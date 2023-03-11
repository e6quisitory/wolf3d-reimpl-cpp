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
    enemyContainer = true;
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

rayTileHitReturn_t ObjectTile::RayTileHit(RayHitMarker& hitInfo) const {
    auto spritesVec = GetEnemiesTextureCoordPairs(hitInfo);
    spritesVec.emplace_back(textureCoordPair_t(texture, centerCoord));
    return std::make_pair(std::nullopt, std::move(spritesVec));
}

bool ObjectTile::PlayerTileHit() const {
    return !passthrough;
}