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

bool ObjectTile::PlayerTileHit() const {
    return !passthrough;
}