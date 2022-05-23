#pragma once

#include "Geometry.h"

namespace Motion{
    extern Vec3 position;
    extern bool probedX, probedY;
    /**
     * @brief Init kinematics
     */
    void init();

    /**
     * @brief Execute this routine while moving.
     */
    void computeSync();
    //Moves
    //void trajectory(Trajectory traj);

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
     * @brief Set the robot position using position of the bumped border
     * @param border position vector of the border relative to the robot current position
     * @param orientation of the robot when probing (side selection, default back)
     * @warning disabled
     * /
    //void probeBorder(Vec2, float);


    /**
     * 
     * @brief Align (turn) the robot toward the desired vector
     * @param target the target vector to align with
     * @param orientation offset angle to align specific face of the robot
     */
    void align(Vec2 target, float orientation);

    //Raw Move
    void move(Vec3 target);

    bool running();

    //Setters
    Vec3 SetTarget(Vec3);
    void SetPosition(Vec2);
    void SetPosition(Vec3);
    void SetAbsolute(bool = true);
    void SetRelative(bool = true);

    void updatePosition();
    //void SetControlPoint(Vec2 point);

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

    //Inverse Kinematics
    Vec3 ik(Vec3);
    Vec3 fk(Vec3);
}