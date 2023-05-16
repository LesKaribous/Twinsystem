#pragma once
#include "core/module.h"
#include "com/intercom.h"
#include "math/geometry.h"

class Lidar : public Module{
private:
    Intercom intercom;

    long _lastSeen = 0;
    long _lastLidarCheck = 0;
public:

    Lidar();
    ~Lidar();

    void update() override;
    void enable() override;
    void disable() override;

    bool isConnected();
    bool obstacleDetected();

    void onObstacleResponse(String answer);

    void displayRadar(bool);

    void checkLidar(float heading);
    float getMaxLidarDist(Vec2 pos, float angle);
};