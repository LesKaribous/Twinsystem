#pragma once
#include <Arduino.h>

using callback_ptr = void (*)();

enum ServiceID{
    ID_LIDAR,
    ID_CHRONO,
    ID_IHM,
    ID_MOTION,
    ID_MOTION_PID,
    ID_PLANNER,
    ID_NEOPIXEL,
    ID_INTERCOM,
    ID_TERMINAL,
    ID_ACTUATORS,
    ID_LOCALISATION,
    ID_NOT_A_SERVICE
    //Add new services here
}; 

/*

in header file : 

class Example : public Service{
public:
    void onUpdate()override;
    void onAttach()override;
    
    Example(): Service(EXAMPLE_ID){};
    SERVICE(Screen)
};


in source file : 

INSTANTIATE_SERVICE(Actuators)

*/

#define SERVICE(X) public: \
    static inline X& instance(){return m_instance;} \
    X(const X&) = delete; \
    X(X&&) = delete; \
    X& operator=(const X&) = delete; \
    X& operator=(X&&) = delete; \
private: \
    static X m_instance;

#define INSTANTIATE_SERVICE(X) X X::m_instance;

class Service{
private:
    bool m_enabled = false;
    bool m_debug = false;
    const ServiceID m_ID;

public:
    Service(ServiceID id) : m_ID(id){};
    inline ServiceID id(){return m_ID;}

    virtual void onAttach() = 0;
    virtual void onUpdate() = 0;
    
    inline virtual void enable(){m_enabled = true;}
    inline virtual void disable(){m_enabled = false;}
    
    inline bool debug() const {return m_debug;}
    inline void toggleDebug(){m_debug = !m_debug;}
    inline bool enabled() const {return m_enabled;}

    static ServiceID toID(String);
    static String toString(ServiceID);
    String Service::toString() const;
};


