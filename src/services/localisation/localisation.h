#pragma once
#include "services/service.h"
#include "utils/geometry.h"
#include <SparkFun_Qwiic_OTOS_Arduino_Library.h>

class Localisation : public ThreadedService{
public:
    void onAttach() override;
    void enable()override;
    void disable()override;
    void onUpdate()override{}
    void onUpdateThread(void* arg) override;
    
    void setPosition(Vec3);
    Vec3 getPosition();
    void read();
    void calibrate();

    inline bool useIMU() const {return m_use_IMU && m_connected & m_calibrated;}
    
    Localisation(): ThreadedService(ID_LOCALISATION){};
    SERVICE(Localisation)


private : 

    bool m_use_IMU = false;
    bool m_connected = false;
    bool m_calibrated = false;

    long m_refresh = 300;

    Vec3 _unsafePosition = {0,0,0};

    QwiicOTOS otos;
};
EXTERN_DECLARATION(Localisation, localisation)