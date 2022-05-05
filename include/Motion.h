#pragma once

#include "Geometry.h"

namespace Motion{
    extern Vec3 position;

    void init();

    //----- COMMANDES DE DEPLACEMENT -----

    //Moves
    //void trajectory(Trajectory traj);

    
    void turn(float);
    void goPolar(float heading, float length);
    
    void go(float x, float y);
    void go(Vec2);

    void go(float x, float y, float heading);
    void go(Vec3);


    //Raw Move
    void move(Vec3 target);

    bool running();

    //Setters
    void SetPosition(Vec3);

    void SetAbsolute(bool = true);
    void SetRelative(bool = true);

    //Getters
    Vec3 GetPosition();
	Vec3 GetTarget();
    bool isAbsolute();
    bool isRelative();

    //Inverse Kinematics
    Vec3 ik(Vec3);
}