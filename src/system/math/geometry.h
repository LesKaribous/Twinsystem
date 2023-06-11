#pragma once

#include "geometry2D.h"
#include "geometry3D.h"

struct WayPoint{
    float x, y, heading;
};

enum class TableCompass{
    NORTH,
    SOUTH,
    EAST,
    WEST
};

enum class RobotCompass{
    A, //Turbine
    AB,
    B, //Battery
    BC,
    C, //Screen
    CA
};

float getCompassOrientation(TableCompass tc);
float getCompassOrientation(RobotCompass rc);
float getBorderDistance( Vec2 cartesianPos, float heading);
float getOffsets(RobotCompass rc);
