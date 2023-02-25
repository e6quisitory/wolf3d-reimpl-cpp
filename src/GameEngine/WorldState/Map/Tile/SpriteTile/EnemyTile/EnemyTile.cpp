#include "EnemyTile.h"

/*
=========================================================
    Constructors
=========================================================
*/

EnemyTile::EnemyTile(const iPoint2 &tileCoord, SDL_Texture *const _texture) {
    texture = _texture;
    coordinate = tileCoord;
    type = tileType_t::ENEMY;
    perplineOrigin = coordinate + Point2(0.5, 0.5);
}

/*
=========================================================
    Public methods
=========================================================
*/

textureSliceDistPair_o EnemyTile::RayTileHit(RayHitMarker& hitInfo, const texturePair_o textureOverride) const {
    return SpriteRayTileHit(hitInfo, textureOverride, texture);
}

bool EnemyTile::PlayerTileHit() const {
    return true;
}