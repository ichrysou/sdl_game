#pragma once

#include <cmath>
#include <map>

#include "../Map.h"
#include "../Pathfinding.h"
#include "Components.h"
#include "ECS.h"
#include "game.h"

// Turns the goal of an agent into a movement direction.
//
// The agent follows a path through the map that was planned with A* and then
// pulled taut, so it walks in long straight lines and cuts corners instead of
// pivoting on every tile. The velocity magnitude is a constant, so the shape
// of the corridor never changes how fast the agent walks.
class PathfindingSystem
{
private:
    Manager &m_manager;
    // Copy of the walkability grid of the current map.
    nav::Grid m_grid;
    unsigned int m_gridVersion = 0;
    // One passability mask per agent size, in tiles.
    std::map<int, nav::NavMesh> m_meshes;

    // Picks up a new map as soon as one is loaded.
    void refreshGrid();
    // Passability mask for an agent of the given size, in tiles.
    const nav::NavMesh &meshFor(int agentTiles);
    void followPath(Entity &agent, PathfindingComponent &path, const nav::NavMesh &mesh);

public:
    PathfindingSystem(Manager &manager) : m_manager(manager) {}
    void update();
};

inline void PathfindingSystem::refreshGrid()
{
    const unsigned int version = Map::Version();
    if (version == m_gridVersion)
        return;
    m_grid = Map::WalkabilityGrid();
    m_meshes.clear();
    m_gridVersion = version;
}

inline const nav::NavMesh &PathfindingSystem::meshFor(int agentTiles)
{
    auto found = m_meshes.find(agentTiles);
    if (found == m_meshes.end())
    {
        nav::NavMesh mesh(m_grid, agentTiles);
        found = m_meshes.emplace(agentTiles, std::move(mesh)).first;
    }
    return found->second;
}

inline void PathfindingSystem::followPath(Entity &agent, PathfindingComponent &path,
                                           const nav::NavMesh &mesh)
{
    auto &transform = agent.getComponent<TransformComponent>();
    const auto &collider = agent.getComponent<ColliderComponent>().collider;
    // The grid is indexed by the centre of the agent, not by its corner.
    const float centerX = transform.position.x + collider.w * 0.5f;
    const float centerY = transform.position.y + collider.h * 0.5f;

    const auto steer = [&](float targetX, float targetY) {
        const float dx = targetX - centerX;
        const float dy = targetY - centerY;
        const float length = std::sqrt(dx * dx + dy * dy);
        if (length <= 0.0001f)
        {
            transform.velocity.Zero();
            return;
        }
        transform.velocity.x = dx / length * path.speedScale;
        transform.velocity.y = dy / length * path.speedScale;
    };

    // When the goal is in sight the agent walks straight at it: no path is
    // needed and the enemy does not walk around walls it can see past.
    if (mesh.hasLineOfSight(centerX, centerY, path.target.x, path.target.y))
    {
        steer(path.target.x, path.target.y);
        return;
    }

    const int goalCellX = m_grid.cellX(path.target.x);
    const int goalCellY = m_grid.cellY(path.target.y);
    const Uint32 now = SDL_GetTicks();
    const bool goalMoved = (goalCellX != path.goalCellX || goalCellY != path.goalCellY);
    if (goalMoved || SDL_TICKS_PASSED(now, path.repathAt))
    {
        path.waypoints = mesh.findPath(centerX, centerY, path.target.x, path.target.y);
        path.nextWaypoint = path.waypoints.empty() ? 0 : 1;
        path.goalCellX = goalCellX;
        path.goalCellY = goalCellY;
        path.repathAt = now + 250;
    }

    // Drop the waypoints the agent already reached or overshot, otherwise it
    // would turn back to a point it is already past.
    const float radiusSquared = path.waypointRadius * path.waypointRadius;
    while (path.nextWaypoint < path.waypoints.size())
    {
        const float waypointX = path.waypoints[path.nextWaypoint].x;
        const float waypointY = path.waypoints[path.nextWaypoint].y;
        const float toX = waypointX - centerX;
        const float toY = waypointY - centerY;
        if (toX * toX + toY * toY <= radiusSquared)
        {
            ++path.nextWaypoint;
            continue;
        }
        // Direction of travel at this waypoint, to tell whether the agent
        // has already gone past it.
        float directionX = 0.0f;
        float directionY = 0.0f;
        if (path.nextWaypoint + 1 < path.waypoints.size())
        {
            directionX = path.waypoints[path.nextWaypoint + 1].x - waypointX;
            directionY = path.waypoints[path.nextWaypoint + 1].y - waypointY;
        }
        else if (path.nextWaypoint > 0)
        {
            directionX = waypointX - path.waypoints[path.nextWaypoint - 1].x;
            directionY = waypointY - path.waypoints[path.nextWaypoint - 1].y;
        }
        const float length = std::sqrt(directionX * directionX + directionY * directionY);
        if (length > 0.0f && (toX * directionX + toY * directionY) / length <= 0.0f)
        {
            ++path.nextWaypoint;
            continue;
        }
        break;
    }

    if (path.nextWaypoint >= path.waypoints.size())
    {
        transform.velocity.Zero();
        return;
    }

    // Aim at the furthest waypoint that is still in sight. Looking ahead is
    // what lets the agent start cutting a corner instead of pivoting on it.
    std::size_t aim = path.nextWaypoint;
    while (aim + 1 < path.waypoints.size() &&
           mesh.canTraverse(centerX, centerY, path.waypoints[aim + 1].x,
                            path.waypoints[aim + 1].y))
        ++aim;

    steer(path.waypoints[aim].x, path.waypoints[aim].y);
}

inline void PathfindingSystem::update()
{
    refreshGrid();
    if (m_grid.width <= 0 || m_grid.height <= 0)
        return;

    for (auto &agent : m_manager.getEntities<PathfindingComponent>())
    {
        if (agent->hasComponent<DeadComponent>())
            continue;
        auto &path = agent->getComponent<PathfindingComponent>();
        if (!path.hasTarget)
        {
            agent->getComponent<TransformComponent>().velocity.Zero();
            continue;
        }

        const auto &collider = agent->getComponent<ColliderComponent>().collider;
        const int agentTiles = std::max(
            1, (std::max(collider.w, collider.h) + m_grid.tileSize - 1) / m_grid.tileSize);
        followPath(*agent, path, meshFor(agentTiles));
    }
}
