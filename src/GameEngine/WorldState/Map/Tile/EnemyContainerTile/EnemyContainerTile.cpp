#include "EnemyContainerTile.h"

/*
=========================================================
    Static members
=========================================================
*/

std::map<textureType_t, std::map<enemyTextureType_t, std::vector<SDL_Texture*>>> EnemyContainerTile::textures;

/*
=========================================================
    Protected methods
=========================================================
*/

textureCoordPairVec_t EnemyContainerTile::GetEnemiesTextureCoordPairs(RayHitMarker& hitInfo) const {
    std::vector<textureCoordPair_t> enemiesTextureCoordPairs;
    for (auto const& enemy : enemies) {
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
            texture = textures[enemy->type][enemyTextureType_t::STATIC][2];
        else if (angle >= PI/8 && angle < 3*PI/8)
            texture = textures[enemy->type][enemyTextureType_t::STATIC][3];
        else if (angle >= 3*PI/8 && angle < 5*PI/8)
            texture = textures[enemy->type][enemyTextureType_t::STATIC][4];
        else if (angle >= 5*PI/8 && angle < 7*PI/8)
            texture = textures[enemy->type][enemyTextureType_t::STATIC][5];
        else if (angle >= 7*PI/8 && angle < 9*PI/8)
            texture = textures[enemy->type][enemyTextureType_t::STATIC][6];
        else if (angle >= 9*PI/8 && angle < 11*PI/8)
            texture = textures[enemy->type][enemyTextureType_t::STATIC][7];
        else if (angle >= 11*PI/8 && angle < 13*PI/8)
            texture = textures[enemy->type][enemyTextureType_t::STATIC][0];
        else
            texture = textures[enemy->type][enemyTextureType_t::STATIC][1];

        enemiesTextureCoordPairs.emplace_back(textureCoordPair_t(texture, enemy->location));
    }
    return std::move(enemiesTextureCoordPairs);
}