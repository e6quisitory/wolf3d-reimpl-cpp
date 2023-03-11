#include "EmptyTile.h"

/*
=========================================================
    Constructors
=========================================================
*/

EmptyTile::EmptyTile(const iPoint2 _tileCoord) {
    enemyContainer = true;
    tileCoord = _tileCoord;
    type      = tileType_t::EMPTY;
}

/*
=========================================================
    Public methods
=========================================================
*/

rayTileHitReturn_t EmptyTile::RayTileHit(RayHitMarker& hitInfo) const {
    return std::make_pair(std::nullopt, GetEnemiesTextureCoordPairs(hitInfo));
}

bool EmptyTile::PlayerTileHit() const {
    return false;
}