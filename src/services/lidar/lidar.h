#pragma once
#include "system/core/service.h"
#include "services/intercom/intercom.h"
#include "system/math/geometry.h"

class Lidar : public Service{
private:
    Intercom intercom;

    bool _ignoreObstacles = false;

    uint32_t _lastUID;

    long _lastSeen = 0;
    long _lastLidarCheck = 0;
public:

    Lidar();
    ~Lidar();

    void update() override;
    void enable() override;
    void disable() override;

    void ignoreObstacles(bool = true);

    bool isConnected();
    bool obstacleDetected();

    void onObstacle();

    void displayRadar(bool = true);

    void checkObstacle();
    void checkLidar(float heading);

    float getMaxLidarDist(Vec2 pos, float angle);
};