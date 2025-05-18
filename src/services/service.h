#pragma once
#include <Arduino.h>
#include "os/singleton.h"
#include "os/tw_threads.h"

using callback_ptr = void (*)();
using wrapper_ptr = void (*)();

enum ServiceID{
    ID_LIDAR,
    ID_CHRONO,
    ID_IHM,
    ID_SAFETY,
    ID_MOTION,
    ID_NAVIGATION,
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
    SINGLETON(Screen)
};
SINGLETON_EXTERN(Example, example)

in source file : 

SINGLETON_INSTANTIATE(example)

*/

class Service{
protected:
    bool m_enabled = false;
    bool m_threaded = false;
    bool m_debug = false;
    const ServiceID m_ID;

public:
    Service(ServiceID id) : m_ID(id){};
    inline ServiceID id(){return m_ID;}

    virtual void onAttach() = 0;
    virtual void onUpdate() = 0;
    
    inline bool threaded(){return m_threaded;}

    inline virtual void enable(){m_enabled = true;}
    inline virtual void disable(){m_enabled = false;}
    
    inline bool debug() const {return m_debug;}
    inline void toggleDebug(){m_debug = !m_debug;}
    inline bool enabled() const {return m_enabled;}

    static ServiceID toID(String);
    static String toString(ServiceID);
    String toString() const;
};



class ThreadedService : public Service{
protected:
    std::thread *servicethread;
	virtual void onUpdateThread(void *arg) = 0;
public:    
    ThreadedService(ServiceID id) : Service(id){
        m_threaded = true;
    };

    static void runThread(void *arg)
	{
		ThreadedService *_runnable = static_cast<ThreadedService*> (arg);
		_runnable->onUpdateThread(arg);
	}
};