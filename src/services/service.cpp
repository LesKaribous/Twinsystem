#include "service.h"


ServiceID Service::toID(String str){
    if(str.equalsIgnoreCase("LIDAR"))
        return ID_LIDAR;

    if(str.equalsIgnoreCase("CHRONO"))
        return ID_CHRONO;

    if(str.equalsIgnoreCase("SAFETY"))
        return ID_SAFETY;

    if(str.equalsIgnoreCase("IHM"))
        return ID_IHM;

    if(str.equalsIgnoreCase("MOTION"))
        return ID_MOTION;

    if(str.equalsIgnoreCase("PLANNER"))
        return ID_PLANNER;

    if(str.equalsIgnoreCase("NEOPIXEL"))
        return ID_NEOPIXEL;

    if(str.equalsIgnoreCase("INTERCOM"))
        return ID_INTERCOM;

    if(str.equalsIgnoreCase("TERMINAL"))
        return ID_TERMINAL;
  
    if(str.equalsIgnoreCase("ACTUATORS"))
        return ID_ACTUATORS;

    if(str.equalsIgnoreCase("LOCALISATION"))
        return ID_LOCALISATION;
    
    return ID_NOT_A_SERVICE;
}

 String Service::toString(ServiceID id){
    if(id == ID_LIDAR)
        return "LIDAR";

    if(id == ID_CHRONO)
        return "CHRONO";

    if(id == ID_SAFETY)
        return "SAFETY";

    if(id == ID_IHM)
        return "IHM";

    if(id ==  ID_MOTION)
        return "MOTION";

    if(id ==  ID_PLANNER)
        return "PLANNER";

    if(id ==  ID_NEOPIXEL)
        return "NEOPIXEL";

    if(id ==  ID_INTERCOM)
        return "INTERCOM";

    if(id ==  ID_TERMINAL)
        return "TERMINAL";
  
    if(id ==  ID_ACTUATORS)
        return "ACTUATORS";

    if(id ==  ID_LOCALISATION)
        return "LOCALISATION";
    
    return "NOT_A_SERVICE";
}

String Service::toString() const{
    return toString(m_ID);
}