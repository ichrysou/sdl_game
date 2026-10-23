#pragma once

// Engine-independent pathfinding over a tile grid. No SDL, no game types: the
// game feeds it a walkability grid and gets back smoothed world-space
// waypoints. Kept in the same spirit as DungeonGen.h, which is likewise
// standalone and unit tested (test/test_pathfinding.cpp).

#include <vector>

#include "Vector2D.h"

namespace nav
{

// Tile grid describing which cells an agent may stand on. The game uses a
// 32 pixel tile everywhere, so one cell is exactly one tile.
struct Grid
{
    int width = 0;
    int height = 0;
    int tileSize = 32;
    // width * height entries, 1 when the cell can be walked on.
    std::vector<unsigned char> walkable;

    void reset(int newWidth, int newHeight, int newTileSize = 32);
    bool inBounds(int x, int y) const;
    bool isWalkable(int x, int y) const;
    void setWalkable(int x, int y, bool value);
    int index(int x, int y) const { return y * width + x; }

    // World space <-> cell space.
    int cellX(float worldX) const;
    int cellY(float worldY) const;
    float centerX(int cellX) const;
    float centerY(int cellY) const;
};

// Passability for one agent footprint, on top of a Grid.
//
// agentTiles is how many tiles across the agent is: 1 for the 32 pixel
// enemies, 2 for the 64 pixel boss. A larger footprint needs the blocked
// cells dilated, otherwise the agent would be routed through gaps its body
// does not fit through.
class NavMesh
{
public:
    NavMesh() = default;
    NavMesh(const Grid &grid, int agentTiles) { rebuild(grid, agentTiles); }

    // Rebuilds the passability mask for a grid and an agent footprint.
    void rebuild(const Grid &grid, int agentTiles);

    bool ready() const { return m_grid != nullptr && m_grid->width > 0 && m_grid->height > 0; }
    int agentTiles() const { return m_agentTiles; }
    const Grid &grid() const { return *m_grid; }

    bool passable(int x, int y) const;

    // True when the straight segment from a to b stays on passable cells.
    // Cells the segment only grazes at a corner count as blocked: a box
    // sized agent cannot squeeze through a corner.
    bool hasLineOfSight(float ax, float ay, float bx, float by) const;

    // Same, but the cell the segment starts in is exempt. Used to look ahead
    // from an agent that was shoved half into a wall by another agent.
    bool canTraverse(float ax, float ay, float bx, float by) const;

    // A* over the grid: 8 connected, diagonal steps may not cut corners.
    // Returns the cell indices from start to goal, empty when unreachable.
    std::vector<int> findCellPath(int startX, int startY, int goalX, int goalY) const;

    // A* followed by string pulling. Returns world-space waypoints from a to
    // b, empty when b cannot be reached.
    std::vector<Vector2D> findPath(float ax, float ay, float bx, float by) const;

private:
    bool lineIsClear(float ax, float ay, float bx, float by, bool exemptStartCell) const;
    int nearestPassableCell(int x, int y) const;
    std::vector<Vector2D> pullString(const std::vector<Vector2D> &points) const;

    const Grid *m_grid = nullptr;
    int m_agentTiles = 1;
    std::vector<unsigned char> m_passable;
};

} // namespace nav
