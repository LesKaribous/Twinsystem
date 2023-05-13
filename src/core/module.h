#pragma once

// Base class for subsystems
class Module {
public:
    virtual void update() = 0;

    void enable();
    void disable();
    bool isEnabled() const;

protected:
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