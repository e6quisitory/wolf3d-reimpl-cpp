#include "WallTile.h"

/*
=========================================================
    Constructors
=========================================================
*/

WallTile::WallTile(const texturePair_t _texture) : texture(_texture) {
    type = tileType_t::WALL;
}

/*
=========================================================
    Public methods
=========================================================
*/

rayTileHitVariant_o WallTile::RayTileHit(RayHitMarker& hitInfo) const {
    SDL_Texture* litTexture = LightTexture(texture, hitInfo);
    SDL_Rect textureRect = {static_cast<int>(hitInfo.GetWidthPercent() * TEXTURE_PITCH), 0, 1, TEXTURE_PITCH};
    textureSlice_t textureSlice(litTexture, textureRect);

    return textureSliceDistPair_t(textureSlice, hitInfo.GetDistToHitPoint());
}

bool WallTile::PlayerTileHit() const {
    return true;
}