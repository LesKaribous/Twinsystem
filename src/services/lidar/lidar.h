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



private :


    SERVICE(Lidar);
};