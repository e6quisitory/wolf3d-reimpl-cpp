#include "Enemy.h"

/*
=========================================================
    Constructors
=========================================================
*/

Enemy::Enemy(const Point2& initialLocation) {
    location = initialLocation;
    viewDir  = RandomUnitVector();
    state    = enemyState_t::UNAWARE;
}