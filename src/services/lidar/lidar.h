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

private :


    SERVICE(Lidar);
};