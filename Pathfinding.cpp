#include "Pathfinding.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>

namespace nav
{
namespace
{
constexpr float kDiagonalCost = 1.41421356f;
constexpr float kCornerEpsilon = 1e-5f;

const int kNeighborX[8] = {1, -1, 0, 0, 1, 1, -1, -1};
const int kNeighborY[8] = {0, 0, 1, -1, 1, -1, 1, -1};

// Exact distance in an 8 connected grid, used as the A* heuristic. It is
// consistent and never overestimates, so A* returns an optimal path.
float octileDistance(int fromX, int fromY, int toX, int toY)
{
    const float a = static_cast<float>(std::abs(toX - fromX));
    const float b = static_cast<float>(std::abs(toY - fromY));
    return (a + b) + (kDiagonalCost - 2.0f) * std::min(a, b);
}
}

void Grid::reset(int newWidth, int newHeight, int newTileSize)
{
    width = std::max(0, newWidth);
    height = std::max(0, newHeight);
    tileSize = newTileSize > 0 ? newTileSize : 32;
    walkable.assign(static_cast<std::size_t>(width) * height, 0);
}

bool Grid::inBounds(int x, int y) const
{
    return x >= 0 && y >= 0 && x < width && y < height;
}

bool Grid::isWalkable(int x, int y) const
{
    if (!inBounds(x, y))
        return false;
    return walkable[static_cast<std::size_t>(index(x, y))] != 0;
}

void Grid::setWalkable(int x, int y, bool value)
{
    if (!inBounds(x, y))
        return;
    walkable[static_cast<std::size_t>(index(x, y))] = value ? 1 : 0;
}

int Grid::cellX(float worldX) const
{
    if (width <= 0)
        return 0;
    const int cell = static_cast<int>(std::floor(worldX / static_cast<float>(tileSize)));
    return std::max(0, std::min(width - 1, cell));
}

int Grid::cellY(float worldY) const
{
    if (height <= 0)
        return 0;
    const int cell = static_cast<int>(std::floor(worldY / static_cast<float>(tileSize)));
    return std::max(0, std::min(height - 1, cell));
}

float Grid::centerX(int cellX) const
{
    return static_cast<float>(cellX * tileSize) + tileSize * 0.5f;
}

float Grid::centerY(int cellY) const
{
    return static_cast<float>(cellY * tileSize) + tileSize * 0.5f;
}

void NavMesh::rebuild(const Grid &grid, int agentTiles)
{
    m_grid = &grid;
    m_agentTiles = std::max(1, agentTiles);
    m_passable.assign(static_cast<std::size_t>(grid.width) * grid.height, 0);

    // A bigger agent needs more room around it: dilate the blocked cells so
    // the footprint is guaranteed to fit wherever it is routed.
    const int radius = m_agentTiles - 1;
    for (int y = 0; y < grid.height; ++y)
    {
        for (int x = 0; x < grid.width; ++x)
        {
            bool fits = true;
            for (int dy = -radius; dy <= radius && fits; ++dy)
            {
                for (int dx = -radius; dx <= radius; ++dx)
                {
                    if (!grid.isWalkable(x + dx, y + dy))
                    {
                        fits = false;
                        break;
                    }
                }
            }
            m_passable[static_cast<std::size_t>(grid.index(x, y))] = fits ? 1 : 0;
        }
    }
}

bool NavMesh::passable(int x, int y) const
{
    if (!ready() || !m_grid->inBounds(x, y))
        return false;
    return m_passable[static_cast<std::size_t>(m_grid->index(x, y))] != 0;
}

bool NavMesh::lineIsClear(float ax, float ay, float bx, float by, bool exemptStartCell) const
{
    if (!ready())
        return false;

    const float tileSize = static_cast<float>(m_grid->tileSize);
    const float cx0 = ax / tileSize;
    const float cy0 = ay / tileSize;
    const float cx1 = bx / tileSize;
    const float cy1 = by / tileSize;

    int x = static_cast<int>(std::floor(cx0));
    int y = static_cast<int>(std::floor(cy0));
    const int endX = static_cast<int>(std::floor(cx1));
    const int endY = static_cast<int>(std::floor(cy1));
    const int startCellX = x;
    const int startCellY = y;

    const float dx = cx1 - cx0;
    const float dy = cy1 - cy0;
    const int stepX = (dx > 0.0f) - (dx < 0.0f);
    const int stepY = (dy > 0.0f) - (dy < 0.0f);
    const float adx = std::fabs(dx);
    const float ady = std::fabs(dy);
    const float infinity = std::numeric_limits<float>::infinity();

    // Parametric distance (0..1 along the segment) to the next cell border.
    float tMaxX = infinity;
    float tMaxY = infinity;
    float tDeltaX = infinity;
    float tDeltaY = infinity;
    if (stepX != 0)
    {
        tMaxX = (stepX > 0 ? (x + 1 - cx0) : (cx0 - x)) / adx;
        tDeltaX = 1.0f / adx;
    }
    if (stepY != 0)
    {
        tMaxY = (stepY > 0 ? (y + 1 - cy0) : (cy0 - y)) / ady;
        tDeltaY = 1.0f / ady;
    }

    for (;;)
    {
        if (!m_grid->inBounds(x, y))
            return false;
        const bool isStartCell = (x == startCellX && y == startCellY);
        if (!(exemptStartCell && isStartCell) && !passable(x, y))
            return false;
        if (x == endX && y == endY)
            break;

        if (std::fabs(tMaxX - tMaxY) < kCornerEpsilon)
        {
            // The segment grazes a cell corner. An agent of this size cannot
            // squeeze between the two cells it only touches at that point.
            if (stepX != 0 && !passable(x + stepX, y))
                return false;
            if (stepY != 0 && !passable(x, y + stepY))
                return false;
            x += stepX;
            y += stepY;
            tMaxX += tDeltaX;
            tMaxY += tDeltaY;
        }
        else if (tMaxX < tMaxY)
        {
            x += stepX;
            tMaxX += tDeltaX;
        }
        else
        {
            y += stepY;
            tMaxY += tDeltaY;
        }
    }
    return true;
}

bool NavMesh::hasLineOfSight(float ax, float ay, float bx, float by) const
{
    return lineIsClear(ax, ay, bx, by, false);
}

bool NavMesh::canTraverse(float ax, float ay, float bx, float by) const
{
    return lineIsClear(ax, ay, bx, by, true);
}

int NavMesh::nearestPassableCell(int x, int y) const
{
    if (!ready())
        return -1;
    if (passable(x, y))
        return m_grid->index(x, y);

    // The agent may be standing somewhere it does not fit (shoved by another
    // agent, or spawned inside geometry). Snap to the closest cell it fits in.
    const int maxRadius = std::max(m_grid->width, m_grid->height);
    for (int radius = 1; radius <= maxRadius; ++radius)
    {
        int best = -1;
        long long bestDistance = std::numeric_limits<long long>::max();
        for (int dy = -radius; dy <= radius; ++dy)
        {
            for (int dx = -radius; dx <= radius; ++dx)
            {
                if (std::max(std::abs(dx), std::abs(dy)) != radius)
                    continue;
                if (!passable(x + dx, y + dy))
                    continue;
                const long long distance =
                    static_cast<long long>(dx) * dx + static_cast<long long>(dy) * dy;
                if (distance < bestDistance)
                {
                    bestDistance = distance;
                    best = m_grid->index(x + dx, y + dy);
                }
            }
        }
        if (best >= 0)
            return best;
    }
    return -1;
}

std::vector<int> NavMesh::findCellPath(int startX, int startY, int goalX, int goalY) const
{
    std::vector<int> path;
    if (!ready())
        return path;
    if (!m_grid->inBounds(startX, startY) || !m_grid->inBounds(goalX, goalY))
        return path;

    const int start = nearestPassableCell(startX, startY);
    const int goal = nearestPassableCell(goalX, goalY);
    if (start < 0 || goal < 0)
        return path;
    if (start == goal)
    {
        path.push_back(goal);
        return path;
    }

    const int width = m_grid->width;
    const int cellCount = width * m_grid->height;
    const float infinity = std::numeric_limits<float>::infinity();
    std::vector<float> cost(cellCount, infinity);
    std::vector<int> cameFrom(cellCount, -1);
    std::vector<unsigned char> closed(cellCount, 0);

    struct Node
    {
        float priority;
        int cell;
    };
    struct Compare
    {
        bool operator()(const Node &a, const Node &b) const { return a.priority > b.priority; }
    };
    std::priority_queue<Node, std::vector<Node>, Compare> open;

    const int goalCellX = goal % width;
    const int goalCellY = goal / width;
    cost[static_cast<std::size_t>(start)] = 0.0f;
    open.push(Node{octileDistance(start % width, start / width, goalCellX, goalCellY), start});

    while (!open.empty())
    {
        const int current = open.top().cell;
        open.pop();
        if (closed[static_cast<std::size_t>(current)])
            continue;
        closed[static_cast<std::size_t>(current)] = 1;
        if (current == goal)
            break;

        const int currentX = current % width;
        const int currentY = current / width;
        for (int i = 0; i < 8; ++i)
        {
            const int dx = kNeighborX[i];
            const int dy = kNeighborY[i];
            const int nextX = currentX + dx;
            const int nextY = currentY + dy;
            if (!m_grid->inBounds(nextX, nextY))
                continue;
            const int next = m_grid->index(nextX, nextY);
            if (closed[static_cast<std::size_t>(next)] || !passable(nextX, nextY))
                continue;
            if (dx != 0 && dy != 0)
            {
                // No corner cutting: both orthogonal neighbours have to be free
                // so the agent's body can sweep through the corner.
                if (!passable(currentX + dx, currentY) || !passable(currentX, currentY + dy))
                    continue;
            }

            const float step = (dx != 0 && dy != 0) ? kDiagonalCost : 1.0f;
            const float tentative = cost[static_cast<std::size_t>(current)] + step;
            if (tentative < cost[static_cast<std::size_t>(next)])
            {
                cost[static_cast<std::size_t>(next)] = tentative;
                cameFrom[static_cast<std::size_t>(next)] = current;
                open.push(Node{tentative + octileDistance(nextX, nextY, goalCellX, goalCellY),
                               next});
            }
        }
    }

    std::vector<int> reversed;
    for (int current = goal; current >= 0; current = cameFrom[static_cast<std::size_t>(current)])
    {
        reversed.push_back(current);
        if (current == start)
            break;
    }
    if (reversed.empty() || reversed.back() != start)
        return path; // unreachable

    path.assign(reversed.rbegin(), reversed.rend());
    return path;
}

std::vector<Vector2D> NavMesh::pullString(const std::vector<Vector2D> &points) const
{
    std::vector<Vector2D> result;
    if (points.empty())
        return result;

    result.push_back(points.front());
    std::size_t anchor = 0;
    while (anchor + 1 < points.size())
    {
        // Keep the furthest point that is still reachable in a straight line.
        // That is what turns a per-tile path into a handful of long segments,
        // so the agent cuts corners instead of pivoting on every tile centre.
        std::size_t furthest = anchor + 1;
        for (std::size_t candidate = anchor + 1; candidate < points.size(); ++candidate)
        {
            const bool clear = lineIsClear(points[anchor].x, points[anchor].y,
                                           points[candidate].x, points[candidate].y,
                                           anchor == 0);
            if (!clear)
                break;
            furthest = candidate;
        }
        result.push_back(points[furthest]);
        anchor = furthest;
    }
    return result;
}

std::vector<Vector2D> NavMesh::findPath(float ax, float ay, float bx, float by) const
{
    if (!ready())
        return {};

    const std::vector<int> cells =
        findCellPath(m_grid->cellX(ax), m_grid->cellY(ay), m_grid->cellX(bx), m_grid->cellY(by));
    if (cells.empty())
        return {};

    std::vector<Vector2D> points;
    points.reserve(cells.size() + 1);
    points.push_back(Vector2D(ax, ay));
    for (int cell : cells)
    {
        points.push_back(Vector2D(m_grid->centerX(cell % m_grid->width),
                                  m_grid->centerY(cell / m_grid->width)));
    }
    return pullString(points);
}

} // namespace nav
