#include "EnemyTile.h"

/*
=========================================================
    Static members
=========================================================
*/

std::map<enemyTextureType_t, std::vector<SDL_Texture*>> EnemyTile::textures;

/*
=========================================================
    Constructors
=========================================================
*/

EnemyTile::EnemyTile(const iPoint2& _tileCoord, SDL_Texture* const _texture) {
    type = tileType_t::ENEMY;
    texture = _texture;
    tileCoord = _tileCoord;
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