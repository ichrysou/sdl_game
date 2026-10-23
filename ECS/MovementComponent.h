#pragma once

#include "ECS.h"

// Marks an entity as a mover that has to deal with the world: it is kept out
// of walls instead of walking through them. Entities without it (projectiles
// for example) move freely and are removed by whatever handles their
// collisions.
class MovementComponent : public Component
{
public:
    // Slide along a wall when it blocks the move, instead of stopping dead.
    bool slideAlongWalls = true;
    // Push apart from other agents of the same kind.
    bool separateFromAgents = true;
};
