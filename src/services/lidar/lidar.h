#pragma once
#include "services/service.h"
#include "settings.h"
#include "occupancy.h"

class Lidar : public Service{
public:

    Lidar();

    void onAttach() override;
    void onUpdate() override;

    void enable() override;
    void disable() override;

    void showRadarLED();
    void showStatusLED();

    

    

private :
    long m_lastPosUpdate = 0;
    long m_lastOccupancyRequest = 0;
    
    float x, y, theta;//abs

    SINGLETON(Lidar);
};

SINGLETON_EXTERN(Lidar, lidar)