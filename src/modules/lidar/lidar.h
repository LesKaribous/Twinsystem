#pragma once
#include "core/module.h"
#include "com/intercom.h"
#include "math/geometry.h"

class Lidar : public Module{
private:
    bool m_obstacle;
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
    void activateDisplay();
    bool obstacleDetected();

    void checkLidar(float heading);
    float getMaxLidarDist(Vec2 pos, float angle);
};