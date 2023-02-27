#include "EnemyTile.h"

/*
=========================================================
    Constructors
=========================================================
*/

EnemyTile::EnemyTile(const iPoint2 &tileCoord, SDL_Texture *const _texture) {
    texture = _texture;
    type = tileType_t::ENEMY;
    centerCoord = tileCoord + Point2(0.5, 0.5);
}

/*
=========================================================
    Public methods
=========================================================
*/

rayTileHitVariant_o EnemyTile::RayTileHit(RayHitMarker& hitInfo) const {
    return textureCoordinatePair_t(texture, centerCoord);
}

bool EnemyTile::PlayerTileHit() const {
    return true;
}