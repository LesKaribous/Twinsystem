#include "module.h"

Module::Module(SystemModule module) : m_module(module){}

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