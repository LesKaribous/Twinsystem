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
    A, //BAU
    AB, //hugger
    B, //Screen
    BC, //vide
    C, //tirette
    CA // manipulator
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

RobotCompass compassFromString(const String& str);
String compassToString(RobotCompass rc);
bool validCompassString(const String& str);