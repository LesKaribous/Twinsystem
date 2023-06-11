#pragma once
#include "system/core/lib.h"

enum SystemModule{
    LIDAR,
    CHRONO,
    SCREEN,
    INPUTS,
    MOTION,
    PLANNER,
    NEOPIXEL,
    TERMINAL,
    ACTUATORS,
    LOCALISATION
};



// Base class for subsystems
class Module {
public:

    Module(SystemModule module);
    virtual void update() = 0;

    virtual void enable();
    virtual void disable();
    bool isEnabled() const;

    SystemModule getModule() const;
    String toString() const;

protected:
    const SystemModule m_module;
    bool m_enabled = false;

};

