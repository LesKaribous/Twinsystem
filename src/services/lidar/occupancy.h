#pragma once
#include "utils/geometry.h"
#include "os/singleton.h"
#include "config/settings.h"

class OccupancyMap{
public:
    OccupancyMap();

    
    bool isInBounds(int x, int y) const; // true si coordonnées de cellule valides

    // Coordonnées monde (mm, origine coin bas-gauche, 0..3000 x 0..2000)
    bool isOccupied(int x, int y) const;
    bool isOccupied(const Vec2& pos) const;

    // Retourne true si au moins une cellule occupée se trouve dans le rayon `radius` (mm)
    // autour de `center` (coordonnées monde).
    bool isZoneOccupied(const Vec2& center, float radius) const;

    // Coordonnées de cellule (0..GRID_WIDTH-1, 0..GRID_HEIGHT-1)
    bool isCellOccupied(int gx, int gy) const;

    float distanceToNearestObstacle(const Vec2& pos) const;
    Vec2  repulsiveGradient(const Vec2& pos) const;
    void  decompress(const String& encoded);

    // Conversions monde ↔ grille
    Vec2 gridToWorld(int gx, int gy) const; // centre de la cellule (gx, gy) en mm
    Vec2 worldToGrid(float x, float y) const; // cellule contenant le point (x, y)

private:
    uint8_t m_map[GRID_WIDTH][GRID_HEIGHT];

    SINGLETON(OccupancyMap);
};

SINGLETON_EXTERN(OccupancyMap, occupancy)