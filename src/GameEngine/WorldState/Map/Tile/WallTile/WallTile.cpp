#include "WallTile.h"

/*
=========================================================
    Constructors
=========================================================
*/

WallTile::WallTile(const texturePair_t _texture) : texture(_texture) {
    type = tileType_t::WALL;
    enemyContainer = false;
}

/*
=========================================================
    Public methods
=========================================================
*/

rayTileHitReturn_t WallTile::RayTileHit(RayHitMarker& hitInfo) const {
    SDL_Texture* litTexture = LightTexture(texture, hitInfo);
    return std::make_pair(textureSliceDistPair_t(textureSlice_t(litTexture, hitInfo.GetWidthPercent()), hitInfo.GetDistToHitPoint()), std::nullopt);
}

bool WallTile::PlayerTileHit() const {
    return true;
}