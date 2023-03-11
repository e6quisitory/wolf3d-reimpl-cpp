#include "Enemy.h"
#include "../../../WorldState/Map/Tile/EnemyContainerTile/EnemyContainerTile.h"

/*
=========================================================
    Static members
=========================================================
*/

double Enemy::moveIncrement;

Map* Enemy::map;

/*
=========================================================
    Constructors
=========================================================
*/

Enemy::Enemy(const enemyType_t enemyType, const Point2& initialLocation) {
    type     = enemyType;
    location = initialLocation;
    viewDir  = RandomUnitVector();
    eastDir  = viewDir.Rotate(-PI/2);
    state    = enemyState_t::UNAWARE;
}

/*
=========================================================
    Public methods
=========================================================
*/

void Enemy::Walk() {
    Point2 prevLoc = location;
    if (MoveEnemyIfValid(location + moveIncrement*viewDir)) {
        Point2  nextLoc = location;
        iPoint2 nextLocTilePt  = iPoint2(nextLoc);
        iPoint2 prevLocTilePt = iPoint2(prevLoc);
        if (nextLocTilePt != prevLocTilePt) {
            auto prevLocTile = map->GetTile(prevLocTilePt);
            auto nextLocTile = map->GetTile(nextLocTilePt);
            if (nextLocTile->enemyContainer) {
                auto nextEnemyContainerTile = static_cast<EnemyContainerTile*>(nextLocTile);
                nextEnemyContainerTile->enemies.insert(this);
                auto prevEnemyContainerTile = static_cast<EnemyContainerTile*>(prevLocTile);
                prevEnemyContainerTile->enemies.erase(this);
            }
        }
    }
}

/*
=========================================================
    Private methods
=========================================================
*/

bool Enemy::MoveEnemyIfValid(const Point2& proposedLoc) {
    if (iPoint2(location) == iPoint2(proposedLoc) || !map->GetTile(proposedLoc)->PlayerTileHit()) {
        location = proposedLoc;
        return true;
    } else
        return false;
}