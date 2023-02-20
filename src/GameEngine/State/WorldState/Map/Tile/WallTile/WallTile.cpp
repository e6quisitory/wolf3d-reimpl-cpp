#include "WallTile.h"

/*
================================
    Constructors
================================
*/

WallTile::WallTile(const iPoint2 &tileCoord, const texturePair_t _texture) : texture(_texture) {
    coordinate = tileCoord;
    type = tileType_t::WALL;
}

/*
================================
    Public methods
================================
*/

textureSliceDistPair_o WallTile::RayTileHit(RayHitMarker& hitInfo, const texturePair_o textureOverride) const {
    SDL_Texture* litTexture = LightTexture(textureOverride.has_value() ? textureOverride.value() : texture, hitInfo);
    SDL_Rect textureRect = {static_cast<int>(hitInfo.GetWidthPercent() * TEXTURE_PITCH), 0, 1, TEXTURE_PITCH};

    textureSlice_t textureSlice(litTexture, textureRect);

    return std::pair(textureSlice, hitInfo.GetDistToHitPoint());
}

bool WallTile::PlayerTileHit() const {
    return true;
}