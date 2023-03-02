#include "Enemy.h"

/*
=========================================================
    Constructors
=========================================================
*/

Enemy::Enemy(const Point2& initialLocation) {
    location = initialLocation;
    viewDir  = RandomUnitVector();
    eastDir  = viewDir.Rotate(-PI/2);
    state    = enemyState_t::UNAWARE;
}