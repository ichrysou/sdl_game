#pragma once

#include "ECS.h"

// Whether an enemy has noticed the player. Once an enemy has seen the hero
// it keeps chasing.
class EnemyAwarenessComponent : public Component
{
public:
    bool alerted = false;
};
