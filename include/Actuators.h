#pragma once

#include <Arduino.h>
#include <Servo.h>

#define FLOOR 0
#define DISPENSER 1
#define WORK_SHED 2
#define GALLERY 3
#define PEDESTAL 4

namespace Actuators{
       
    void init();
    
    class Bras
    {
        public:

        Bras();
        ~Bras();

        void setPin(int pinServoElevator, int pinServoArm, int pinServoTool, int pinPump);
        void setLimit(  int minServoElevator, 
                        int maxServoElevator, 
                        int minServoArm, 
                        int maxServoArm, 
                        int minServoTool, 
                        int maxServoTool );
    
        int calcPositionElevator(byte posServoElevator);
        int calcPositionArm     (byte posServoArm);
        int calcPositionTool    (byte posServoTool);

        void detachBras();
        void attachBras();

        void setPosition(byte posServoElevator, byte posServoArm, byte posServoTool);
        void setPosition(byte posServoElevator, byte posServoArm, byte posServoTool, int wait);

        void setElevator    (byte posServoElevator);
        void setElevator    (byte posServoElevator, int wait);

        void setArm (byte posServoArm);    
        void setArm (byte posServoArm, int wait);

        void setTool (byte posServoTool);
        void setTool (byte posServoTool, int wait);

        void grab();
        void grab(int wait);

        void ungrab();
        void ungrab(int wait);


        private:

        Servo _servoElevator ;
        Servo _servoArm ;
        Servo _servoTool ;

        int _pinServoElevator;
        int _pinServoArm;
        int _pinServoTool;

        int _pinPump ;

        int _minServoElevator ;
        int _maxServoElevator ;
        int _minServoArm ;
        int _maxServoArm ;
        int _minServoTool ;
        int _maxServoTool ;

    };

    //Bras
    void takeElement(Bras robotArm,int location);
    
    extern Bras BrasAU;
    extern Bras BrasInit;
    extern Bras BrasTirette;
    
}


