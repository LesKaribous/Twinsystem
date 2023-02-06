#pragma once
#include "core/Core.h"
#include "BistableServo.h"


namespace TwinSystem{

    struct GripperGroup{
        BistableServo rightGripper;
        BistableServo leftGripper;
        BistableServo cherryLocker;
    };

    struct VacuumGroup{
        BistableServo cherryLocker;
        //Turbine t;
        //Pump p;
    };
    

    class Actuators{
    private:
        GripperGroup GripperAB; //Right group when facing screen
        GripperGroup GripperBC; //Opposed to the screen
        GripperGroup GripperCA; //Left group when facing screen
        VacuumGroup Vacuum;
    public:
        Actuators();
        ~Actuators();
    };

    Actuators::Actuators(){

    }

    Actuators::~Actuators()
    {
    }

} // namespace TwinSystem

