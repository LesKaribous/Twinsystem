#include "Stepper.h"

namespace Stepper{
    void init(){
        controller.moveAsync(sLeft,sRight);

         sLeft
        .setAcceleration(20000)
        .setMaxSpeed(15000)
        .setInverseRotation(true)
        .setStepPinPolarity(LOW);

        sRight
        .setAcceleration(20000)
        .setMaxSpeed(15000)
        .setInverseRotation(true)
        .setStepPinPolarity(LOW);
    }


    
    void turn(long step, bool abs, bool async){
        sRight.setTarget()
    }

    void go(long step, bool async){
        
    }

    void stop(bool async){

    }


    void sleep(){

    }

    void run(){

    }

    void emergencyStop(){

    }

    void setSpeed(){

    }

    void setAccel(){

    }

    int getAccel(){

    }

    int getSpeed(){

    }

    bool isRunning(){

    }

    bool isSleeping(){

    }
}