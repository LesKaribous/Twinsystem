#pragma once
#include "utils/geometry.h"



class Movement{
public:
    Movement(){};


};

class Translation : public Movement{
public:
    Translation(){};


private:
    Vec2 m_a, m_b;//two points

};

class Rotation : public Movement{
public:
    Rotation(float a, float b){};


private:
    float m_a, m_b;//two angle

};

class LinearMove : public Movement{
public:
    LinearMove(Vec3 a, Vec3 b){};


private:
    Vec3 m_a, m_b;//two points with rotation

};

//TODO arc, trajectories