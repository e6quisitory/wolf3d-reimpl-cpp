#include "EnemyTile.h"

/*
=========================================================
    Static members
=========================================================
*/

std::map<textureType_t, std::map<enemyTextureType_t, std::vector<SDL_Texture*>>> EnemyTile::textures;

/*
=========================================================
    Constructors
=========================================================
*/

EnemyTile::EnemyTile(const iPoint2& _tileCoord, const textureType_t _textureType) {
    type = tileType_t::ENEMY;
    textureType = _textureType;

    tileCoord = _tileCoord;
    centerCoord = tileCoord + Point2(0.5, 0.5);

    enemy = new Enemy(centerCoord);
}

/*
=========================================================
    Destructors
=========================================================
*/

EnemyTile::~EnemyTile() {
    delete enemy;
}

/*
=========================================================
    Public methods
=========================================================
*/

rayTileHitVariant_o EnemyTile::RayTileHit(RayHitMarker& hitInfo) const {
    const auto& playerViewDir     = UnitVector(enemy->location - hitInfo.ray.origin);
    const auto& enemyViewDir      = enemy->viewDir;
    const auto& enemyEastDir      = enemy->eastDir;
    double playerViewDotEnemyView = Dot(playerViewDir, enemyViewDir);
    double playerViewDotEnemyEast = Dot(playerViewDir, enemyEastDir);

    double angle;
    if (playerViewDotEnemyView >= 0) {
        angle = std::acos(playerViewDotEnemyEast);
    } else
        angle = 2*PI - std::acos(playerViewDotEnemyEast);

    SDL_Texture* texture;
    if ((angle >= 15*PI/8 && angle <= 2*PI) || (angle >= 0 && angle < PI/8))
        texture = textures[textureType][enemyTextureType_t::STATIC][2];
    else if (angle >= PI/8 && angle < 3*PI/8)
        texture = textures[textureType][enemyTextureType_t::STATIC][3];
    else if (angle >= 3*PI/8 && angle < 5*PI/8)
        texture = textures[textureType][enemyTextureType_t::STATIC][4];
    else if (angle >= 5*PI/8 && angle < 7*PI/8)
        texture = textures[textureType][enemyTextureType_t::STATIC][5];
    else if (angle >= 7*PI/8 && angle < 9*PI/8)
        texture = textures[textureType][enemyTextureType_t::STATIC][6];
    else if (angle >= 9*PI/8 && angle < 11*PI/8)
        texture = textures[textureType][enemyTextureType_t::STATIC][7];
    else if (angle >= 11*PI/8 && angle < 13*PI/8)
        texture = textures[textureType][enemyTextureType_t::STATIC][0];
    else
        texture = textures[textureType][enemyTextureType_t::STATIC][1];

    return textureCoordinatePair_t(texture, centerCoord);
}

bool EnemyTile::PlayerTileHit() const {
    return true;
}