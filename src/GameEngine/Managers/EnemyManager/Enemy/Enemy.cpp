#include "Enemy.h"

/*
=========================================================
    Static members
=========================================================
*/

Map* Enemy::map;

/*
=========================================================
    Constructors
=========================================================
*/

Enemy::Enemy(const Point2& initialLocation, const int _screenRefreshRate) {
    location = initialLocation;
    viewDir  = RandomUnitVector();
    eastDir  = viewDir.Rotate(-PI/2);
    state    = enemyState_t::UNAWARE;

    moveIncrement = (0.02/(_screenRefreshRate/60.0))        /10    ;
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
            map->SetTile(nextLocTilePt, prevLocTile);
            map->SetTile(prevLocTilePt, nextLocTile);
            static_cast<SpriteTile*>(prevLocTile)->tileCoord = nextLocTilePt;
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