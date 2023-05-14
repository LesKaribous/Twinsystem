#pragma once

enum SystemModule{
    TEST,
    LIDAR,
    SCREEN,
    INPUTS,
    MOTION,
    PLANNER,
    NEOPIXEL,
    ACTUATOR
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

void Module::enable() {
    m_enabled = true;
}

void Module::disable() {
    m_enabled = false;
}

bool Module::isEnabled() const { 
    return m_enabled; 
}

SystemModule Module::getModule() const { 
    return m_module; 
}