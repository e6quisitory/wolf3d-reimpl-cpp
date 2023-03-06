#pragma once

#include "Enemy/Enemy.h"
#include "../../Multimedia/Multimedia.h"
#include "../../WorldState/WorldState.h"


class EnemyManager {
private:
    WorldState* worldState;
    Multimedia* multimedia;

    std::vector<Enemy*> enemies;

public:
    void Init(WorldState* const _worldState, Multimedia* const _multimedia);
    void Update() const;

private:
    void LoadEnemyTypeTextures(const textureType_t enemyType);
};