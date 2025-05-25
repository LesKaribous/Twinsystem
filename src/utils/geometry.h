#pragma once

#include "geometry/geometry2D.h"
#include "geometry/geometry3D.h"

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
    A,
    AB,
    B,
    BC,
    C, //Screen
    CA
};

enum class Side{
    LEFT,
    RIGHT, 
    BOTH
};

float getCompassOrientation(TableCompass tc);
float getCompassOrientation(RobotCompass rc);
float getBorderDistance( Vec2 cartesianPos, float heading);
float getOffsets(RobotCompass rc);
//RobotCompass getNextCompass(RobotCompass rc);