#pragma once

#include "Geometry.h"

namespace Motion{

    enum class State{
        IDLE,
        RUNNING,
        PAUSED,
        CANCELLED,
        ARRIVED,
    };

    extern bool probedX, probedY;
    /**
     * @brief Init kinematics
     */
    void init();
    void pause();
    void resume();
    
    void debugState();

    /**
     * @brief Execute this routine while moving.
     */
    void computeSync();

    /**
     * @brief updatePosition by calculating foward kinematics. (In realtime folk's B-D )
     */
    void updatePosition();

    /**
     * @brief Turn robot arround its center by desired angle
     * @param angle angle in degrees
     */
    void turn(float);

    /**
     * @brief Move the robot using polar coordinates
     * @param heading move direction
     * @param length distance to go
     */
    void goPolar(float heading, float length);
    
    /**
     * @brief Move the robot using cartesian coordinates
     * @param x X coordinate
     * @param y Y coordinate
     */
    void go(float x, float y);
    void go(Vec2);

    /**
     * @brief Move the robot using cartesian coordinates & turn by desired angle
     * @param x X coordinate
     * @param y Y coordinate
     * @param w Y coordinate
     */
    void goTurn(float x, float y, float w);
    void goTurn(Vec3);

    /**
     * @brief Set the robot position using position of the bumped border
     * @param border position vector of the border relative to the robot current position
     */
    void probeBorder(Vec2);

    /**
     * @brief Align (turn) the robot toward the desired vector
     * @param target the target vector to align with
     * @param orientation offset angle to align specific face of the robot
     */
    void align(Vec2 target, float orientation);

    /**
     * @brief Move the robot to the desired target (last params of the Vec3 is used for arc curvature)
     * @param target the target vector to align with
     */
    void move(Vec3 target);

    /**
     * @brief Return true if a move is running
     */
    bool running();


    //Setters
    Vec3 SetTarget(Vec3);
    void SetPosition(Vec2);
    void SetPosition(Vec3);
    void SetAbsolute(bool = true);
    void SetRelative(bool = true);


    //Getters
    Vec3 GetPosition();
	Vec3 GetTarget();
    Vec3 GetAbsTarget();
    bool isAbsolute();
    bool isRelative();
    bool isProbed();
    bool isXProbed();
    bool isYProbed();
    //Return true while probing
    bool isProbing();

}