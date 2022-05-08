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

#define SQUARE_PURPLE 0
#define SQUARE_YELLOW 1
#define SQUARE_BAD 2
#define SQUARE_ERROR 3

#define RESISTOR_YELLOW 511
#define RESISTOR_PURPLE 327
#define RESISTOR_BAD 842

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
        void setPin(int pinServoElevator, int pinServoArm, int pinServoTool, int pinServoTool2, int pinPump, int pinEv);
        void setLimit(  int minServoElevator, 
                        int maxServoElevator, 
                        int minServoArm, 
                        int maxServoArm, 
                        int minServoTool, 
                        int maxServoTool );
        void setLimit(  int minServoElevator, 
                        int maxServoElevator, 
                        int minServoArm, 
                        int maxServoArm, 
                        int minServoTool, 
                        int maxServoTool ,
                        int minServoTool2, 
                        int maxServoTool2 );
    
        void setGeometry(int angle, int decalage);

        int calcPositionElevator(byte posServoElevator);
        int calcPositionArm     (byte posServoArm);
        int calcPositionTool    (byte posServoTool);
        int calcPositionTool2   (byte posServoTool);

        void detachBras();
        void attachBras();

        void setPosition(byte posServoElevator, byte posServoArm, byte posServoTool, int wait = 0);

        void setElevator    (byte posServoElevator  , int wait = 0);
        void setArm         (byte posServoArm       , int wait = 0);
        void setTool        (byte posServoTool      , int wait = 0);
        void setTool2       (byte posServoTool2     , int wait = 0);

        void enablePump (bool state = true);
        void openEv     (bool state = true);

        void grab(int wait = 0);
        void ungrab(int wait = 0);


        private:

        Servo _servoElevator ;
        Servo _servoArm ;
        Servo _servoTool ;
        Servo _servoTool2;

        int _pinServoElevator;
        int _pinServoArm;
        int _pinServoTool;
        int _pinServoTool2 = 0;

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
        int _minServoTool2 ;
        int _maxServoTool2 ;

    };

    //Bras
    void sleep();
    void unsuck();
    int getProbingValue();

    extern Bras BrasAU;
    extern Bras BrasInit;
    extern Bras BrasTirette;
}


