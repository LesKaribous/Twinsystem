#include "Settings.h"
#include "Pin.h"

namespace Settings{

    bool ROBOT;
    bool AVOIDANCE = true;

    const Vec3 
    YellowTransform = { 1.0f, 1.0f, 1.0f },
    PurpleTransform = { 1.0f,-1.0f,-1.0f };

    Vec3 Team::transform; 

    void init(){ 
        pinMode(Pin::robotSelect, INPUT_PULLUP);
        ROBOT = digitalRead(Pin::robotSelect) ? PRIMARY : SECONDARY;
    }

    void setTeam(bool team){
        Team::transform = (team == Team::YELLOW) ? YellowTransform : PurpleTransform;
    }

    void setAvoidance(bool state){
        AVOIDANCE = state;
    }
}

