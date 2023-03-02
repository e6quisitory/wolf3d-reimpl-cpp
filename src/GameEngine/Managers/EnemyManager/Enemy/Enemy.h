#pragma once

/*
=========================================================
    Relevant type definitions
=========================================================
*/

enum class enemyState_t {
    UNAWARE,
    ALERTED,
    ATTACKING,
    SHOOTING,
    DAMAGE,
    DYING
};

/*
=========================================================
    Enemy
=========================================================
*/

class Enemy {
public:
    Point2        location;
    Vec2          viewDir;
    enemyState_t  state;

public:
    Enemy(const Point2& initialLocation);
};