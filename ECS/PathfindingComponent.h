#pragma once

#include <cstddef>
#include <vector>

#include "../Vector2D.h"
#include "ECS.h"

// Navigation state of an agent: the goal it walks to and the waypoints of
// the path it is currently following.
class PathfindingComponent : public Component
{
public:
    // World space goal. hasTarget is false while the agent has nothing to
    // walk to, in which case it stands still.
    Vector2D target;
    bool hasTarget = false;

    // The path, in world space, already pulled taut so it consists of long
    // straight segments instead of one point per tile.
    std::vector<Vector2D> waypoints;
    // Index of the waypoint the agent is heading for. Index 0 is the place
    // the agent was when the path was planned.
    std::size_t nextWaypoint = 0;

    // The goal cell the current path was planned for, used to notice that
    // the goal moved and the path has to be planned again.
    int goalCellX = -1;
    int goalCellY = -1;
    // Time (SDL_GetTicks) at which the path is planned again even when the
    // goal did not move.
    Uint32 repathAt = 0;

    // Magnitude of the velocity, as a fraction of TransformComponent::speed.
    // It is a constant: the shape of the corridor never changes how fast the
    // agent walks.
    float speedScale = 1.0f / 3.0f;

    // Distance at which a waypoint counts as reached.
    float waypointRadius = 6.0f;

    void clearPath()
    {
        waypoints.clear();
        nextWaypoint = 0;
        goalCellX = -1;
        goalCellY = -1;
    }
};
