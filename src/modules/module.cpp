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

String Module::toString() const{
    switch (m_module){
    case LIDAR:
        return "LIDAR";
        break;
    case SCREEN:
        return "SCREEN";
        break;
    case INPUTS:
        return "INPUTS";
        break;  
    case MOTION:
        return "MOTION";
        break;  
    case PLANNER:
        return "PLANNER";
        break;  
    case NEOPIXEL:
        return "NEOPIXEL";
        break;  
    case TERMINAL:
        return "TERMINAL";
        break;   
    case ACTUATORS:
        return "ACTUATORS";
        break;   
    case LOCALISATION:
        return "LOCALISATION";
        break;   
    default:
        return "UNKNOWN";
        break;
    }
}