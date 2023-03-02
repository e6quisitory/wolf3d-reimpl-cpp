#pragma once

#include "Enemy/Enemy.h"
#include "../../Multimedia/Multimedia.h"

class EnemyManager {
public:
    Multimedia* multimedia;

public:
    void Init(Multimedia* const _multimedia);
};