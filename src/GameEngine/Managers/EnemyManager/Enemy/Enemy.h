#pragma once

#include "../../../WorldState/Map/Map.h"

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
    Vec2          eastDir;
    enemyState_t  state;

    double moveIncrement;

    static Map* map;

public:
    Enemy(const Point2& initialLocation, const int _screenRefreshRate);

    void Walk();
    void Swivel();

private:
    bool MoveEnemyIfValid(const Point2& proposedLoc);
};