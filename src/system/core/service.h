#pragma once
#include "system/core/lib.h"

enum ServiceID{
    LIDAR,
    CHRONO,
    SCREEN,
    INPUTS,
    MOTION,
    PLANNER,
    NEOPIXEL,
    INTERCOM,
    TERMINAL,
    ACTUATORS,
    LOCALISATION,
    NOT_A_SERVICE
};



// Base class for subsystems
class Service {
public:

    Service(ServiceID module);
    virtual void update() = 0;

    virtual void enable();
    virtual void disable();
    bool isEnabled() const;

    ServiceID getID() const;
    String toString() const;
    static ServiceID toID(String);
    static String toString(ServiceID);

protected:
    const ServiceID m_ID;
    bool m_enabled = false;

};

