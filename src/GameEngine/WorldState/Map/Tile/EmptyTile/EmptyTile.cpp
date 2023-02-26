#include "EmptyTile.h"

/*
=========================================================
    Constructors
=========================================================
*/

EmptyTile::EmptyTile() {
    type = tileType_t::EMPTY;
}

/*
=========================================================
    Public methods
=========================================================
*/

rayTileHitVariant_o EmptyTile::RayTileHit(RayHitMarker& hitInfo) const {
    return std::nullopt;
}

bool EmptyTile::PlayerTileHit() const {
    return false;
}