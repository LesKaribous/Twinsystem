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

    void decompressOccupancyMap(const String& encoded);

    bool isOccupied(int x, int y);

private :
    long m_lastPosUpdate = 0;
    long m_lastOccupancyRequest = 0;
    uint8_t m_map[GRID_WIDTH][GRID_HEIGHT];
    float x, y, theta;//abs

    SERVICE(Lidar);
};

EXTERN_DECLARATION(Lidar, lidar)