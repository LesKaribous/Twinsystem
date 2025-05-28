#include "pathfinder.h"
#include <limits>
#include <cmath>

/*

// Helper function to compare directions
bool isSameDirection(const Vec2& a, const Vec2& b, float toleranceDegrees = 1.0f) {
    float angle = Vec2::angleBetween(a, b) * 180.0f / M_PI;
    return std::abs(angle) < toleranceDegrees;
}


// Dijkstra grid pathfinding with 8-direction movement and simplified logic
std::vector<Vec2> computePath(const Vec2& startMM, const Vec2& endMM){

    float cellSize = GRID_CELLSIZE;
    
    int startX = static_cast<int>(startMM.x / cellSize);
    int startY = static_cast<int>(startMM.y / cellSize);
    int endX   = static_cast<int>(endMM.x / cellSize);
    int endY   = static_cast<int>(endMM.y / cellSize);

    int rows = GRID_HEIGHT;
    int cols = GRID_WIDTH;

    std::vector<std::vector<bool>> visited(cols, std::vector<bool>(rows, false));
    std::vector<std::vector<float>> dist(cols, std::vector<float>(rows, std::numeric_limits<float>::max()));
    std::vector<std::vector<Vec2>> prev(cols, std::vector<Vec2>(rows));

    dist[startX][startY] = 0.0f;

    std::vector<Vec2> openList;
    openList.push_back(Vec2(static_cast<float>(startX), static_cast<float>(startY)));

    int directions[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };

    while (!openList.empty()) {
        // Find the point in openList with the lowest cost
        int bestIndex = 0;
        float bestCost = dist[static_cast<int>(openList[0].x)][static_cast<int>(openList[0].y)];

        for (int i = 1; i < openList.size(); ++i) {
            int ix = static_cast<int>(openList[i].x);
            int iy = static_cast<int>(openList[i].y);
            float cost = dist[ix][iy];
            if (cost < bestCost) {
                bestCost = cost;
                bestIndex = i;
            }
        }

        Vec2 current = openList[bestIndex];
        openList.erase(openList.begin() + bestIndex);
        int cx = static_cast<int>(current.x);
        int cy = static_cast<int>(current.y);

        if (visited[cx][cy]) continue;
        visited[cx][cy] = true;

        if (cx == endX && cy == endY) break;

        for (int d = 0; d < 8; ++d) {
            int nx = cx + directions[d][0];
            int ny = cy + directions[d][1];

            if (nx < 0 || ny < 0 || nx >= cols || ny >= rows) continue;
            if (occupancy.isCellOccupied(nx,ny) != 0 || visited[nx][ny]) continue;

            float stepCost = (abs(directions[d][0]) + abs(directions[d][1]) == 2) ? 1.414f : 1.0f;
            float newCost = dist[cx][cy] + stepCost;

            if (newCost < dist[nx][ny]) {
                dist[nx][ny] = newCost;
                prev[nx][ny] = Vec2(static_cast<float>(cx), static_cast<float>(cy));
                openList.push_back(Vec2(static_cast<float>(nx), static_cast<float>(ny)));
            }
        }
    }

    // Reconstruct the path
    std::vector<Vec2> path;
    if (dist[endY][endX] == std::numeric_limits<float>::max()) {
        return path; // No path found
    }

    Vec2 step(static_cast<float>(endX), static_cast<float>(endY));
    while (true) {
        Vec2 waypoint = step * cellSize + Vec2(cellSize / 2.0f, cellSize / 2.0f);
        path.insert(path.begin(), waypoint);

        int sx = static_cast<int>(step.x);
        int sy = static_cast<int>(step.y);
        if (sx == startX && sy == startY) break;

        step = prev[sx][sy];
    }

    return path;
}


std::vector<Vec2> simplifyPath(const std::vector<Vec2>& input) {
    std::vector<Vec2> simplified;
    if (input.size() < 2) return input;

    simplified.push_back(input[0]); // Always include the first point

    for (size_t i = 1; i < input.size() - 1; ++i) {
        Vec2 prev = input[i - 1];
        Vec2 curr = input[i];
        Vec2 next = input[i + 1];

        Vec2 dir1 = Vec2::sub(curr, prev).normalize();
        Vec2 dir2 = Vec2::sub(next, curr).normalize();

        if (!isSameDirection(dir1, dir2)) {
            simplified.push_back(curr);
        }
    }

    simplified.push_back(input.back()); // Always include the last point
    return simplified;
}

*/