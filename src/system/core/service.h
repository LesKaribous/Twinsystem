#pragma once
#include "system/core/lib.h"

enum ServiceID{
    LIDAR,
    CHRONO,
    SCREEN,
    INPUTS,
    MOTION,
    MOTION_PID,
    PLANNER,
    NEOPIXEL,
    INTERCOM,
    TERMINAL,
    ACTUATORS,
    LOCALISATION,
    NOT_A_SERVICE
};

class OperatingSystem;
class Service {
public:
    Service(ServiceID module);
    inline void setOS(OperatingSystem* parent){ os = parent;};
    virtual void update() = 0;

    virtual void enable();
    virtual void disable();
    bool isEnabled() const;

    ServiceID getID() const;
    String toString() const;
    static ServiceID toID(String);
    static String toString(ServiceID);

    inline bool debug() const {return m_debug;}
    inline void toggleDebug(){m_debug = !m_debug;}

protected:
    OperatingSystem* os;
    bool m_enabled = false;
    bool m_debug = false;
    const ServiceID m_ID;
};

