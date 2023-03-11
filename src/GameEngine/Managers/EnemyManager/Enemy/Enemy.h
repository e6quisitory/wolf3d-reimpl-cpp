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

typedef textureType_t enemyType_t;

/*
=========================================================
    Enemy
=========================================================
*/

class Enemy {
public:
    enemyType_t   type;

    Point2        location;
    Vec2          viewDir;
    Vec2          eastDir;
    enemyState_t  state;

    static double moveIncrement;
    static Map*   map;

public:
    Enemy(const enemyType_t enemyType, const Point2& initialLocation);

    void Walk();
    void Swivel();

private:
    bool MoveEnemyIfValid(const Point2& proposedLoc);
};