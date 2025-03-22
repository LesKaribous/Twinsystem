#pragma once
#include "services/service.h"
#include "utils/geometry.h"
#include <SparkFun_Qwiic_OTOS_Arduino_Library.h>

class Localisation : public Service{
public:
    void onAttach() override;
    void enable()override;
    void disable() override;
    void onUpdate() override;
    //void onUpdateThread(void* arg) override;
    
    void setPosition(Vec3);
    Vec3 getPosition();
    void read();
    void calibrate();

    bool isRotating() const;
    bool isMoving() const;

    inline bool useIMU() const {return m_use_IMU && m_connected & m_calibrated;}
    
    Localisation(): Service(ID_LOCALISATION){};
    SERVICE(Localisation)

private : 

    bool m_use_IMU = false;
    bool m_connected = false;
    bool m_calibrated = false;
    bool m_isMoving = false;
    bool m_isRotating = false;
    long m_refresh = 10; //ms  previous 300

    Vec3 _unsafePosition = {0,0,0};

    QwiicOTOS otos;
};
EXTERN_DECLARATION(Localisation, localisation)