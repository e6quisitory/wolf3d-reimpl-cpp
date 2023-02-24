#include "Enemy.h"

/*
=========================================================
    Constructors
=========================================================
*/

Enemy::Enemy() : state(enemyState_t::UNAWARE), viewDir(RandomUnitVector()) {}