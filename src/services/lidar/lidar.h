#pragma once
#include "services/service.h"
#include "settings.h"

class Lidar : public Service{
public:

    Lidar();

    void onAttach() override;
    void onUpdate() override;

    void enable() override;
    void disable() override;

    void showRadarLED();
    void showStatusLED();

    void setLidarPosition(Vec3 pos);

private :
    long m_lastPosUpdate = 0;
    long m_lastOccupancyRequest = 0;

    SERVICE(Lidar);
};