#pragma once

#include "Geometry2D.h"
#include "Geometry3D.h"
#include <vector>

struct Point{
    float x, y, heading;
};

//Kinematics
Vec3 ik(Vec3 target);
Vec3 fk(Vec3 target);

enum class TableCompass{
    NORTH,
    SOUTH,
    EAST,
    WEST
};

enum class GripperSide{
    LEFT,
    RIGHT
};

enum class RobotCompass{
    A,
    AB, // Emergency Button
    B,
    BC, // Init button
    C,  // Screen & Main PCB
    CA  // Tirette
};

float GetCompassOrientation(TableCompass tc);
float GetCompassOrientation(RobotCompass rc);
float GetOffsets(RobotCompass rc);
//float GetOffset(TableCompass tc);

struct Path{
    Point origin, end;
    Path(Point origin, Point end);
    ~Path();

    bool isClosed();
    virtual void execute() = 0;
};

struct Arc : public Path{ 
    float radius, length;
    virtual void execute();
};

struct Line : public Path{
    float length;
    virtual void execute();
};

class Trajectory : public Path{
    std::vector<Path*> path;

public :
    virtual void execute();

    Trajectory(Point origin);
    Trajectory(Point origin, Point end);

    ~Trajectory();

    void add(Path*);
    void remove(int i);
    void clear();
};


