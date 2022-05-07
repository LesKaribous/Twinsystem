#pragma once

#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define FLOOR 0
#define DISPENSER 1
#define WORK_SHED 2
#define GALLERY 3
#define PEDESTAL 4

#define SERVO_ELEVATOR 0
#define SERVO_ARM 1
#define SERVO_TOOL 2

namespace Actuators{
       
    void init();
    class Bras
    {
        public:

        Bras();
        ~Bras();

        void setPin(int pinServoElevator, int pinServoArm, int pinServoTool);
        void setPin(int pinServoElevator, int pinServoArm, int pinServoTool, int pinPump, int pinEv);
        void setLimit(  int minServoElevator, 
                        int maxServoElevator, 
                        int minServoArm, 
                        int maxServoArm, 
                        int minServoTool, 
                        int maxServoTool );
    
        void setGeometry(int angle, int decalage);

        int calcPositionElevator(byte posServoElevator);
        int calcPositionArm     (byte posServoArm);
        int calcPositionTool    (byte posServoTool);

        void detachBras();
        void attachBras();

        void setPosition(byte posServoElevator, byte posServoArm, byte posServoTool, int wait = 0);

        void setElevator    (byte posServoElevator  , int wait = 0);
        void setArm         (byte posServoArm       , int wait = 0);
        void setTool        (byte posServoTool      , int wait = 0);

        void enablePump (bool state = true);
        void openEv     (bool state = true);

        void grab(int wait = 0);
        void ungrab(int wait = 0);


        private:

        Servo _servoElevator ;
        Servo _servoArm ;
        Servo _servoTool ;

        int _pinServoElevator;
        int _pinServoArm;
        int _pinServoTool;

        int _pinPump ;
        int _pinEv ;
        bool _pinEvAvailable = false;

        int _angle ;
        int _centerDistance ;

        int _minServoElevator ;
        int _maxServoElevator ;
        int _minServoArm ;
        int _maxServoArm ;
        int _minServoTool ;
        int _maxServoTool ;

    };

    //Bras
    void sleep();
    void unsuck();

    extern Bras BrasAU;
    extern Bras BrasInit;
    extern Bras BrasTirette;
}


