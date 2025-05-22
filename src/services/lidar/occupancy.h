#pragma once
#include "utils/geometry.h"
#include "os/singleton.h"
#include "settings.h"

class OccupancyMap{
public:
    OccupancyMap();

    
    bool isInBounds(int x, int y) const; //return true if the cell coordinates are valid (inbounds)

    bool isOccupied(int x, int y) const;
    bool isCellOccupied(int x, int y) const;
    
    float distanceToNearestObstacle(const Vec2& pos) const;
    Vec2 repulsiveGradient(const Vec2& pos) const;
    void decompress(const String& encoded);

    Vec2 gridToWorld(int x, int y) const;
    Vec2 worldToGrid(int x, int y) const;

private:
    uint8_t m_map[GRID_WIDTH][GRID_HEIGHT];

    SINGLETON(OccupancyMap);
};

SINGLETON_EXTERN(OccupancyMap, occupancy)