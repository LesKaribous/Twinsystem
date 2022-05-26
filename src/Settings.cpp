#include "Settings.h"
#include "Pin.h"
#include "Actuators.h"
#include "Twinsystem.h"

namespace Settings{

    bool ROBOT;
    bool AVOIDANCE = true;
    bool TEAM;

    const Vec3 
    YellowTransform = { 1.0f, 1.0f, 1.0f },
    PurpleTransform = { 1.0f,-1.0f,-1.0f };

    Vec3 Team::transform; 

    void init(){ 
        pinMode(Pin::robotSelect, INPUT_PULLUP);
        ROBOT = digitalRead(Pin::robotSelect) ? PRIMARY : SECONDARY;
    }

    void setTeam(bool team){
        if(team == Team::PURPLE){
            Actuators::BrasAU.setAngle(-Actuators::BrasAU.GetAngle());
            Actuators::BrasInit.setAngle(-Actuators::BrasInit.GetAngle());
            Actuators::BrasTirette.setAngle(-Actuators::BrasTirette.GetAngle()); 
            Team::transform = PurpleTransform;
        }else{
            Team::transform = YellowTransform;
        }
        TEAM = team;
        
    }

    void setAvoidance(bool state){
        AVOIDANCE = state;
    }
}

