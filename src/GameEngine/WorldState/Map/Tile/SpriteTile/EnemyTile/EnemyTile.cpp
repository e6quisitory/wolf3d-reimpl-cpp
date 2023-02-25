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
    // Get intersection of incoming ray with perpline
    Ray incomingRay = hitInfo.ray;
    RayHitMarker_o perpLineHitInfo = RayPerplineHit(incomingRay);

    if (perpLineHitInfo.has_value()) {
        SDL_Rect textureRect = {static_cast<int>(perpLineHitInfo.value().GetWidthPercent() * TEXTURE_PITCH), 0, 1, TEXTURE_PITCH};
        double   hitDistance = perpLineHitInfo->GetDistToHitPoint();
        return std::pair(textureSlice_t(texture, textureRect), hitDistance);
    } else
        return std::nullopt;
}

bool EnemyTile::PlayerTileHit() const {
    return true;
}