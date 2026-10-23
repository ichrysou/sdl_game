#include "Pathfinding.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace
{
int failures = 0;

void fail(const std::string &what)
{
    std::cerr << "FAIL - " << what << std::endl;
    ++failures;
}

using Cell = std::pair<int, int>;

nav::Grid makeGrid(int width, int height, const std::vector<Cell> &walls)
{
    nav::Grid grid;
    grid.reset(width, height, 32);
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            grid.setWalkable(x, y, true);
    for (const Cell &wall : walls)
        grid.setWalkable(wall.first, wall.second, false);
    return grid;
}

bool isWall(const std::vector<Cell> &walls, int x, int y)
{
    return std::find(walls.begin(), walls.end(), Cell(x, y)) != walls.end();
}

// The path must be a chain of single steps, and a diagonal step may never
// cut a corner: the two orthogonal cells it slides between have to be free.
void checkPathIsWalkable(const nav::Grid &grid, const nav::NavMesh &mesh,
                         const std::vector<int> &cells, const std::string &what)
{
    if (cells.empty())
    {
        fail(what + ": path is empty");
        return;
    }
    for (std::size_t i = 0; i + 1 < cells.size(); ++i)
    {
        const int ax = cells[i] % grid.width;
        const int ay = cells[i] / grid.width;
        const int bx = cells[i + 1] % grid.width;
        const int by = cells[i + 1] / grid.width;
        const int dx = bx - ax;
        const int dy = by - ay;
        if (std::max(std::abs(dx), std::abs(dy)) != 1)
        {
            fail(what + ": path step is not adjacent");
            return;
        }
        if (dx != 0 && dy != 0)
        {
            if (!mesh.passable(ax + dx, ay) || !mesh.passable(ax, ay + dy))
            {
                fail(what + ": path cuts a corner");
                return;
            }
        }
    }
}

void testOpenRoom()
{
    const nav::Grid grid = makeGrid(12, 12, {});
    const nav::NavMesh mesh(grid, 1);

    const std::vector<int> cells = mesh.findCellPath(0, 0, 11, 11);
    checkPathIsWalkable(grid, mesh, cells, "open room");
    if (cells.empty() || cells.front() != grid.index(0, 0) ||
        cells.back() != grid.index(11, 11))
        fail("open room: path does not start and end where asked");

    // In open ground the whole path collapses into a single straight segment.
    const std::vector<Vector2D> points = mesh.findPath(16.0f, 16.0f, 352.0f, 352.0f);
    if (points.size() > 2)
        fail("open room: string pulling did not collapse the path");
    if (points.size() == 2 && !mesh.hasLineOfSight(points[0].x, points[0].y,
                                                   points[1].x, points[1].y))
        fail("open room: smoothed segment is not clear");
}

void testPathAroundWall()
{
    // A wall with a gap at the top: the path has to walk around it.
    std::vector<Cell> walls;
    for (int y = 2; y <= 9; ++y)
        walls.push_back(Cell(6, y));
    const nav::Grid grid = makeGrid(14, 12, walls);
    const nav::NavMesh mesh(grid, 1);

    const std::vector<int> cells = mesh.findCellPath(2, 5, 11, 5);
    checkPathIsWalkable(grid, mesh, cells, "around wall");
    if (cells.empty())
        return;
    for (int cell : cells)
    {
        const int x = cell % grid.width;
        const int y = cell / grid.width;
        if (isWall(walls, x, y))
        {
            fail("around wall: path enters a wall");
            return;
        }
    }
    // The wall is 8 cells tall, so walking around it costs more than the 9
    // steps a straight line would take.
    if (cells.size() <= 10)
        fail("around wall: path is suspiciously short, it probably went through the wall");
}

void testUnreachable()
{
    // A sealed pocket: the goal cannot be reached at all.
    std::vector<Cell> walls;
    for (int x = 3; x <= 7; ++x)
    {
        walls.push_back(Cell(x, 3));
        walls.push_back(Cell(x, 7));
    }
    for (int y = 4; y <= 6; ++y)
    {
        walls.push_back(Cell(3, y));
        walls.push_back(Cell(7, y));
    }
    const nav::Grid grid = makeGrid(12, 11, walls);
    const nav::NavMesh mesh(grid, 1);

    if (!mesh.findCellPath(1, 1, 5, 5).empty())
        fail("unreachable: found a path into a sealed pocket");
    if (!mesh.findPath(32.0f, 32.0f, 160.0f, 160.0f).empty())
        fail("unreachable: found a smoothed path into a sealed pocket");
}

void testCornerCutting()
{
    // Two walls meeting in a corner: an agent may not slip through the seam.
    const std::vector<Cell> walls = {Cell(5, 5), Cell(5, 6), Cell(6, 6), Cell(6, 5)};
    const nav::Grid grid = makeGrid(12, 12, walls);
    const nav::NavMesh mesh(grid, 1);

    // Straight through the seam of the two walls.
    if (mesh.hasLineOfSight(4.5f * 32.0f, 5.5f * 32.0f, 6.5f * 32.0f, 5.5f * 32.0f))
        fail("corner cutting: line of sight slips through a wall seam");
    // Well clear of it.
    if (!mesh.hasLineOfSight(1.5f * 32.0f, 1.5f * 32.0f, 2.5f * 32.0f, 1.5f * 32.0f))
        fail("corner cutting: clear line is reported as blocked");
}

void testAgentSize()
{
    // A one tile wide corridor: a 32 pixel enemy fits, the 64 pixel boss does not.
    std::vector<Cell> walls;
    for (int y = 0; y < 8; ++y)
        walls.push_back(Cell(3, y));
    for (int y = 5; y < 12; ++y)
        walls.push_back(Cell(6, y));
    const nav::Grid grid = makeGrid(10, 12, walls);

    const nav::NavMesh small(grid, 1);
    const nav::NavMesh large(grid, 2);

    // The corridor between x=4 and x=5 leads from the top room to the bottom one.
    if (small.findCellPath(5, 0, 5, 11).empty())
        fail("agent size: the one tile agent cannot use the corridor");
    if (!large.findCellPath(5, 0, 5, 11).empty())
        fail("agent size: the two tile agent is routed through a one tile corridor");
}

void testStringPulling()
{
    // A dog leg corridor: the smoothed path keeps the corner but drops the
    // tile centres in between.
    std::vector<Cell> walls;
    for (int y = 0; y < 9; ++y)
        walls.push_back(Cell(4, y));
    for (int x = 9; x < 12; ++x)
        walls.push_back(Cell(x, 4));
    const nav::Grid grid = makeGrid(12, 12, walls);
    const nav::NavMesh mesh(grid, 1);

    const std::vector<int> cells = mesh.findCellPath(2, 2, 10, 10);
    checkPathIsWalkable(grid, mesh, cells, "string pulling");
    if (cells.empty())
        return;

    const std::vector<Vector2D> points =
        mesh.findPath(2.5f * 32.0f, 2.5f * 32.0f, 10.5f * 32.0f, 10.5f * 32.0f);
    if (points.empty())
    {
        fail("string pulling: no smoothed path");
        return;
    }
    if (points.size() >= cells.size())
        fail("string pulling: the path was not shortened");
    for (std::size_t i = 0; i + 1 < points.size(); ++i)
    {
        if (!mesh.hasLineOfSight(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y))
        {
            fail("string pulling: a smoothed segment is not clear");
            return;
        }
    }
}

void testStartOutsideGrid()
{
    const nav::Grid grid = makeGrid(8, 8, {});
    const nav::NavMesh mesh(grid, 1);

    // A start position far outside the map is clamped onto the grid instead
    // of crashing or walking through the void.
    if (mesh.findPath(-500.0f, -500.0f, 100.0f, 100.0f).empty())
        fail("outside grid: expected a clamped path");
    // Raw cell requests outside the grid are rejected.
    if (!mesh.findCellPath(-1, -1, 3, 3).empty())
        fail("outside grid: expected no path for out of bounds cells");
}
} // namespace

int main()
{
    testOpenRoom();
    testPathAroundWall();
    testUnreachable();
    testCornerCutting();
    testAgentSize();
    testStringPulling();
    testStartOutsideGrid();

    if (failures == 0)
        std::cout << "all pathfinding tests passed" << std::endl;
    return failures == 0 ? 0 : 1;
}
