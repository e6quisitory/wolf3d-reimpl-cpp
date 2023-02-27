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
    textureSlice_t textureSlice(litTexture, hitInfo.GetWidthPercent());

    return textureSliceDistPair_t(textureSlice, hitInfo.GetDistToHitPoint());
}

bool WallTile::PlayerTileHit() const {
    return true;
}