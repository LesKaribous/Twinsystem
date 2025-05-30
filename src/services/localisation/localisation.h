#pragma once
#include "services/service.h"
#include "utils/geometry.h"
#include <SparkFun_Qwiic_OTOS_Arduino_Library.h>

class Localisation : public Service{
public:
   
    void attach() override;
    void run() override;
    
    void enable()override;
    void disable() override;
    //void onUpdateThread(void* arg) override;
    
    void setPosition(Vec3);
    Vec3 getPosition();
    Vec3 getVelocity();
    void read();
    void calibrate();
    void setLinearScale(float value);

    inline bool useIMU() const {return m_use_IMU && m_connected & m_calibrated;}
    
    Localisation(): Service(ID_LOCALISATION){};
    SINGLETON(Localisation)

private : 
    float m_scale = 0.955f;
    bool m_use_IMU = false;
    bool m_connected = false;
    bool m_calibrated = false;
    bool m_isMoving = false;
    bool m_isRotating = false;
    long m_refresh = 10; //ms  previous 300

    Vec3 _unsafePosition = {0,0,0};
    Vec3 _unsafeVelocity = {0,0,0};

    QwiicOTOS otos;
};
SINGLETON_EXTERN(Localisation, localisation)