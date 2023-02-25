#include "EmptyTile.h"

/*
=========================================================
    Constructors
=========================================================
*/

EmptyTile::EmptyTile(const iPoint2 &tileCoord) {
    coordinate = tileCoord;
    type = tileType_t::EMPTY;
}

/*
=========================================================
    Public methods
=========================================================
*/

textureSliceDistPair_o EmptyTile::RayTileHit(RayHitMarker& hitInfo, const texturePair_o textureOverride) const {
    return std::nullopt;
}

bool EmptyTile::PlayerTileHit() const {
    return false;
}