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

    inline bool debug() const {return m_debug;}
    inline void enableDebug(){m_debug = true;}
    inline void disableDebug(){m_debug = false;}

protected:
    const ServiceID m_ID;
    bool m_enabled = false;
    bool m_debug = false;

};

