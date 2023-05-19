#pragma once

enum SystemModule{
    CLI,
    LIDAR,
    SCREEN,
    INPUTS,
    MOTION,
    PLANNER,
    NEOPIXEL,
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

protected:
    const SystemModule m_module;
    bool m_enabled = false;

};

